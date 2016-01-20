#include "packet.h"
#include "errors.h"
#include "mac_addr.h"
#include "sockets.h"
#include "log.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


uint8_t * create_magic_packet_payload(uint8_t *mac_addr)
{
    uint8_t * payload = malloc(PAYLOAD_SZ * sizeof(uint8_t));

    if (payload == NULL)
        return NULL;

    int offset = 6;
    memset(payload, 0xff, offset);

    for (int i = 0; i <= 16; i++) {
        memcpy(
            (char *)payload + i * MAC_ADDR_LEN + offset,
            (char *) mac_addr,
            MAC_ADDR_LEN
        );
    }

    LOG(DEBUG, "\n\nPayload: \n");
    for (int j = 0; j < PAYLOAD_SZ; j++) {
        LOG(DEBUG, "%x ", payload[j]);
    }
    LOG(DEBUG, "\n\n");

    LOG(DEBUG, "mac address: ");
    for (int i = 0; i < MAC_ADDR_LEN; i++) {
        LOG(DEBUG, "%x", mac_addr[i]);
    }
    LOG(DEBUG, "\n");

    return payload;
}


int select_iface(struct ifaddrs **iface_addr_ptr)
{
    struct ifaddrs *iface_addr = *iface_addr_ptr;

    if (getifaddrs(&iface_addr) == -1) {
        return E_NO_INTERFACE;
    }

    struct ifaddrs *current_addr = iface_addr;
    struct ifaddrs *candidate_iface = NULL;

    LOG(INFO, "Looking for interfaces...\n");
    do {
        LOG(INFO, "Found interface: %s\n", current_addr->ifa_name);

        print_sockaddr("Broadcast addr", 2, current_addr->ifa_broadaddr);
        print_sockaddr("Netmask addr", 2, current_addr->ifa_netmask);
        print_sockaddr("Dst addr", 2, current_addr->ifa_dstaddr);

        if (strcmp(current_addr->ifa_name, "en0") == 0) {
            //break;
            candidate_iface = current_addr;
        }

        if (current_addr->ifa_next == NULL) {
            candidate_iface = current_addr;
        } else {
            current_addr = current_addr->ifa_next;
        }
    } while (current_addr->ifa_next != NULL);

    cpy_ifaddrs(candidate_iface, iface_addr_ptr);
    LOG(DEBUG, "Memory copied\n");
    freeifaddrs(iface_addr);
    LOG(DEBUG, "Iface freed\n\n");

    return 0;
}

int send_magic_packet(uint8_t *mac_addr)
{
    /**
     * Magic Packet:
     *   - Broadcast frame
     *   - containing anywhere within its payload: 6 bytes of all 255,
     *     followed by 16 repetitions of the target computer's 48-bit
     *     MAC address
     *   - Total of 102 bytes
     *
     * Typically sent either:
     *   - UDP datagram to port 0, 7 or 9
     *   - Directly over Ethernet as EtherType 0x0842
     *
     * See: https://en.wikipedia.org/wiki/Wake-on-LAN#Magic_packet
     */

    int sock_fd = socket(PF_INET, SOCK_DGRAM, getprotobyname("udp")->p_proto);

    if (sock_fd == -1)
        return E_SOCKET_CREATION_FAIL;

    int broadcastEnable = 1;
    // TODO: Is this needed? Is the return value valuable?
    setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST,
               &broadcastEnable, sizeof(broadcastEnable));

    // Get information about the interface
    struct ifaddrs iface_addr_tmp;
    struct ifaddrs *iface_addr = &iface_addr_tmp;
    struct ifaddrs **iface_addr_ptr = &iface_addr;
    // *iface_addr_ptr = &iface_addr;

    int iface_select_res = select_iface(iface_addr_ptr);
    if (iface_select_res < 0)
        return iface_select_res;

    iface_addr = *iface_addr_ptr;

    if (iface_addr == NULL) {
        return E_NO_INTERFACE;
    }

    LOG(INFO, "Using interface: %s\n", iface_addr->ifa_name);
    print_sockaddr("Broadcast addr", 2, iface_addr->ifa_broadaddr);
    print_sockaddr("Netmask addr", 2, iface_addr->ifa_netmask);
    print_sockaddr("Dst addr", 2, iface_addr->ifa_dstaddr);


    /*
     * TODO: Use this to get broadcast IP dynamically
    struct sockaddr_in *target_addr =
        (struct sockaddr_in *)iface_addr->ifa_broadaddr;

    target_addr->sin_family = AF_INET;
    target_addr->sin_port = 9;
    print_sockaddr("Target", 0, (struct sockaddr *)target_addr);
    */


    struct sockaddr_in target_addr;
    bzero((char *)& target_addr, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = (in_port_t)htons(9);
    target_addr.sin_addr.s_addr =
        ((struct sockaddr_in *)iface_addr->ifa_broadaddr)->sin_addr.s_addr;

    print_sockaddr("\nTarget", 0, (struct sockaddr *)&target_addr);

    uint8_t *payload = create_magic_packet_payload(mac_addr);

    if (payload == NULL) {
        free_iface(*iface_addr_ptr);
        return E_COULD_NOT_ALLOCATE_MEMORY;
    }

    unsigned int flags = 0;  // TODO

    int res = sendto(sock_fd, payload, PAYLOAD_SZ, flags,
                     (struct sockaddr *)&target_addr, sizeof(target_addr));

    if (res == -1) {
        LOG(ERROR, "Error: %d\n", errno);
        free_iface(*iface_addr_ptr);
        free((uint8_t *)payload);
        return E_DATA_SEND_FAILURE;
    }

    free_iface(*iface_addr_ptr);
    free((uint8_t *)payload);

    if (close(sock_fd) == -1)
        return E_SOCKET_CLOSE_FAIL;

    return 0;
}
