/**
 *    main.c: Main entry point to the program
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

#include "errors.h"
#include "mac_addr.h"
#include "packet.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>


int parse_cmd_line(int argc, char **argv)
{
    if (argc < 2)
        return E_NOT_ENOUGH_ARGS;

    uint8_t *mac_addr = parse_mac_addr_str(&argv[1]);

    if (mac_addr == NULL)
        return E_INCORRECT_MAC_ADDR_FORMAT;

    int res = send_magic_packet(mac_addr);

    free(mac_addr);

    return res;
}


int main(int argc, char **argv)
{
    int rc = parse_cmd_line(argc, argv);

    LOG(INFO, "Result: %d\n", rc);
    return rc;
}
