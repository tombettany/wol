#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void print_sockaddr(char *msg, int indent_level, struct sockaddr *sock_addr);
int cpy_sockaddr(struct sockaddr *src, struct sockaddr **dest);
void free_sockaddr(struct sockaddr *sock_addr);
int cpy_ifaddrs(struct ifaddrs *src, struct ifaddrs **dest);
void free_iface(struct ifaddrs *iface_addr);
