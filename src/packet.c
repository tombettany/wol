/**
 *    packet.c: Generate and send packets
 *
 *    Copyright (C) 2016 Tom Bettany
 *
 #    This file is part of the Magic Packet Wake on Lan (WoL) project.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
 *    USA. Alternatively, see <http://www.gnu.org/licenses/>.
 */

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

    LOG(DEBUG, "\n\nmac address: ");
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

    struct ifaddrs *candidate_iface = iface_addr;

    LOG(INFO, "Looking for interfaces...\n");

    while (candidate_iface != NULL) {
        LOG(INFO, "Found interface: %s\n", candidate_iface->ifa_name);

        print_sockaddr_in("Broadcast addr", 2,
                          (struct sockaddr_in *)candidate_iface->ifa_broadaddr);
        print_sockaddr_in("Netmask addr", 2,
                          (struct sockaddr_in *)candidate_iface->ifa_netmask);
        print_sockaddr_in("Dst addr", 2,
                          (struct sockaddr_in *)candidate_iface->ifa_dstaddr);

        if (
                strcmp(candidate_iface->ifa_name, "en0") == 0
                && candidate_iface->ifa_broadaddr != NULL
                && candidate_iface->ifa_netmask != NULL
                && candidate_iface->ifa_dstaddr != NULL
        )
            break;

        candidate_iface = candidate_iface->ifa_next;
    }

    if (candidate_iface == NULL) {
        LOG(DEBUG, "Could not find interface\n");
        freeifaddrs(iface_addr);

        return E_NO_INTERFACE;
    }

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
    struct ifaddrs *iface_addr;

    int iface_select_res = select_iface(&iface_addr);
    if (iface_select_res < 0)
        return iface_select_res;

    if (iface_addr == NULL)
        return E_NO_INTERFACE;

    LOG(INFO, "Using interface: %s\n", iface_addr->ifa_name);
    print_sockaddr_in("Broadcast addr", 2,
                      (struct sockaddr_in *)iface_addr->ifa_broadaddr);
    print_sockaddr_in("Netmask addr", 2,
                      (struct sockaddr_in *)iface_addr->ifa_netmask);
    print_sockaddr_in("Dst addr", 2,
                      (struct sockaddr_in *)iface_addr->ifa_dstaddr);

    struct sockaddr_in target_addr;
    bzero((char *)& target_addr, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = (in_port_t)htons(9);
    target_addr.sin_addr.s_addr =
        ((struct sockaddr_in *)iface_addr->ifa_broadaddr)->sin_addr.s_addr;

    print_sockaddr_in("\nTarget", 0, &target_addr);

    uint8_t *payload = create_magic_packet_payload(mac_addr);

    if (payload == NULL) {
        freeifaddrs(iface_addr);

        return E_COULD_NOT_ALLOCATE_MEMORY;
    }

    unsigned int flags = 0;  // TODO

    int res = sendto(sock_fd, payload, PAYLOAD_SZ, flags,
                     (struct sockaddr *)&target_addr, sizeof(target_addr));

    if (res == -1) {
        LOG(ERROR, "Error: %d\n", errno);
        freeifaddrs(iface_addr);
        free((uint8_t *)payload);

        return E_DATA_SEND_FAILURE;
    }

    freeifaddrs(iface_addr);
    free((uint8_t *)payload);

    if (close(sock_fd) == -1)
        return E_SOCKET_CLOSE_FAIL;

    return 0;
}
