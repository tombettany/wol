#include "sockets.h"
#include "log.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


void print_sockaddr(char *msg, int indent_level, struct sockaddr *sock_addr)
{
    const int space_sz = 4;

    struct sockaddr_in * sock_inet = (struct sockaddr_in *) sock_addr;

    char spacer[space_sz * indent_level + 1];
    memset(spacer, ' ', space_sz * indent_level);
    spacer[space_sz * indent_level] = '\0';

    LOG(INFO, "%s%s: ", spacer, msg);

    if (sock_addr == NULL) {
        LOG(WARN, " ... Nothing to print\n");
        return;
    }

    LOG(
        INFO,
        "sin_family: %d, sin_port: %d, sin_addr: %x (%s), sin_zero: ",
        sock_inet->sin_family, sock_inet->sin_port,
        sock_inet->sin_addr.s_addr, inet_ntoa(sock_inet->sin_addr)
    );

    for (int i = 0; i < 8; i++) {
        LOG(INFO, ", %d", sock_inet->sin_zero[i]);
    }

    LOG(INFO, "\n");

}

int cpy_sockaddr(struct sockaddr *src, struct sockaddr **dest)
{
    if (src == NULL) {
        *dest = NULL;
        return 1;
    }

    LOG(DEBUG, "        ... Allocating sockaddr...\n");
    *dest = malloc(sizeof(struct sockaddr));

    LOG(DEBUG, "         ... Setting values\n");
    LOG(DEBUG, "             ... Setting sa_len: \n");
    (*dest)->sa_len = src->sa_len;
    LOG(DEBUG, "             ... Setting sa_family\n");
    (*dest)->sa_family = src->sa_family;
    LOG(DEBUG, "             ... Setting sa_data\n");
    for (int i = 0; i < 14; i++) {
        (*dest)->sa_data[i] = src->sa_data[i];
    }

    return 0;
}

void free_sockaddr(struct sockaddr *sock_addr)
{
    free(sock_addr);
}

int cpy_ifaddrs(struct ifaddrs *src, struct ifaddrs **dest)
{
    /**
     * struct ifaddrs {
     *     char             *ifa_name;
     *     unsigned int      ifa_flags;
     *     struct sockaddr  *ifa_addr;
     *     struct sockaddr  *ifa_netmask;
     *     struct sockaddr  *ifa_dstaddr;
     *     void             *ifa_data;
     * }
     */

    LOG(DEBUG, "Allocating ifaddrs...\n");
    *dest = malloc(sizeof(struct ifaddrs));

    LOG(DEBUG, " ... Setting values\n");
    LOG(DEBUG, "     ... Setting ifa_next\n");
    (*dest)->ifa_next = NULL;
    LOG(DEBUG, "     ... Setting ifa_name\n");
    (*dest)->ifa_name = src->ifa_name;
    LOG(DEBUG, "     ... Setting ifa_flags\n");
    (*dest)->ifa_flags = src->ifa_flags;
    LOG(DEBUG, "     ... Setting ifa_addr\n");
    cpy_sockaddr(src->ifa_addr, &(*dest)->ifa_addr);
    LOG(DEBUG, "     ... Setting ifa_netmask\n");
    cpy_sockaddr(src->ifa_netmask, &(*dest)->ifa_netmask);
    LOG(DEBUG, "     ... Setting ifa_dstaddr\n");
    cpy_sockaddr(src->ifa_dstaddr, &(*dest)->ifa_dstaddr);
    LOG(DEBUG, "     ... Setting ifa_dat\n");
    (*dest)->ifa_data = NULL;
    //memcpy((*dest)->ifa_data, src->ifa_data, sizeof());


    return 0;
}
