/**
 *    sockets.c: Utilities for handling socket connections
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


void print_sockaddr_in(char *msg, int indent_level, struct sockaddr_in *sock_addr)
{
    const int space_sz = 4;

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
        sock_addr->sin_family, sock_addr->sin_port,
        sock_addr->sin_addr.s_addr, inet_ntoa(sock_addr->sin_addr)
    );

    for (int i = 0; i < 8; i++) {
        LOG(INFO, ", %d", sock_addr->sin_zero[i]);
    }

    LOG(INFO, "\n");

}

int cpy_sockaddr_in(struct sockaddr_in *src, struct sockaddr_in **dest)
{
    if (src == NULL) {
        *dest = NULL;
        return 1;
    }

    LOG(DEBUG, "        ... Allocating sockaddr...\n");
    *dest = malloc(sizeof(struct sockaddr_in));

    LOG(DEBUG, "         ... Setting values\n");
    LOG(DEBUG, "             ... Setting sin_family: \n");
    (*dest)->sin_family = src->sin_family;
    LOG(DEBUG, "             ... Setting sin_port\n");
    (*dest)->sin_port = src->sin_port;
    LOG(DEBUG, "             ... Setting sin_addr\n");
    (*dest)->sin_addr = src->sin_addr;

    return 0;
}

void free_sockaddr_in(struct sockaddr_in *sock_addr)
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
    cpy_sockaddr_in((struct sockaddr_in *)src->ifa_addr,
                    (struct sockaddr_in **)&(*dest)->ifa_addr);
    LOG(DEBUG, "     ... Setting ifa_netmask\n");
    cpy_sockaddr_in((struct sockaddr_in *)src->ifa_netmask,
                    (struct sockaddr_in **)&(*dest)->ifa_netmask);
    LOG(DEBUG, "     ... Setting ifa_dstaddr\n");
    cpy_sockaddr_in((struct sockaddr_in *)src->ifa_dstaddr,
                    (struct sockaddr_in **)&(*dest)->ifa_dstaddr);
    LOG(DEBUG, "     ... Setting ifa_dat\n");
    (*dest)->ifa_data = NULL;

    return 0;
}
