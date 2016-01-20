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
