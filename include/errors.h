/**
 *    errors.h: Error codes
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

#ifndef __WOL_ERRORS_H__
#define __WOL_ERRORS_H__

enum error_codes {
    E_NOT_ENOUGH_ARGS = -1,
    E_SOCKET_CREATION_FAIL = -2,
    E_DATA_SEND_FAILURE = -3,
    E_SOCKET_CLOSE_FAIL = -4,
    E_NO_INTERFACE = -5,
    E_INCORRECT_MAC_ADDR_FORMAT = -6,
    E_COULD_NOT_ALLOCATE_MEMORY = -7
};

#endif
