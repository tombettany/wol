/**
 *    packet.h: Generate and send packets
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

#ifndef __WOL_PACKET_H__
#define __WOL_PACKET_H__

#include "mac_addr.h"

#include <inttypes.h>
#include <ifaddrs.h>

#define PAYLOAD_SZ       6 + MAC_ADDR_LEN * 16

uint8_t * create_magic_packet_payload(uint8_t *mac_addr);
int select_iface(struct ifaddrs **iface_addr_ptr);
void free_iface(struct ifaddrs *iface_addr);
int send_magic_packet(uint8_t *mac_addr);

#endif
