/**
 *    mac_addr.h: Utilities to manipulate MAC addresses
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

#ifndef __WOL_MAC_ADDR_H__
#define __WOL_MAC_ADDR_H__

#include <inttypes.h>

#define MAC_ADDR_LEN         6
#define MAC_ADDR_STR_LEN     3 * MAC_ADDR_LEN - 1
#define MAC_ADDR_BIT_SZ      MAC_ADDR_LEN * 8

uint8_t char_to_binary(char c);
uint8_t *parse_mac_addr_str(char **mac_addr_str);

#endif
