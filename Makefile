 ##
 #    Makefile: Build the project
 #
 #    Copyright (C) 2016 Tom Bettany
 #
 #    This file is part of the Magic Packet Wake on Lan (WoL) project.
 #
 #    This program is free software; you can redistribute it and/or modify
 #    it under the terms of the GNU General Public License as published by
 #    the Free Software Foundation; either version 3 of the License, or
 #    (at your option) any later version.
 #
 #    This program is distributed in the hope that it will be useful,
 #    but WITHOUT ANY WARRANTY; without even the implied warranty of
 #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
 #    GNU General Public License for more details.
 #
 #    You should have received a copy of the GNU General Public License
 #    along with this program; if not, write to the Free Software
 #    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
 #    USA. Alternatively, see <http://www.gnu.org/licenses/>.
 ##

CFLAGS+=-Wall -g
LDFLAGS+=
INCLUDES=-I./include
OBJS=main packet mac_addr sockets
BUILD_DIR=out
TARGET=magic-packet

all: magic-packet

create-dirs:
	mkdir -p $(BUILD_DIR)

magic-packet: $(patsubst %, %.o, $(OBJS))
	$(CC) $(CFLAGS) $(LDFLAGS) $(patsubst %, $(BUILD_DIR)/%, $^) -o $(TARGET)

%.o: create-dirs src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c src/$*.c -o $(BUILD_DIR)/$*.o

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(TARGET)
