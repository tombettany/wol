#include "mac_addr.h"

#include <inttypes.h>
#include <ifaddrs.h>

#define PAYLOAD_SZ       6 + MAC_ADDR_LEN * 16

uint8_t * create_magic_packet_payload(uint8_t *mac_addr);
int select_iface(struct ifaddrs **iface_addr_ptr);
void free_iface(struct ifaddrs *iface_addr);
int send_magic_packet(uint8_t *mac_addr);
