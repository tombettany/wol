#include "errors.h"
#include "mac_addr.h"
#include "log.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint8_t char_to_binary(char c)
{
    if ('0' <= c && c <= '9') {
        return c - '0';
    } else if ('a' <= c && c <= 'f') {
        return 10 + c - 'a';
    } else if ('A' <= c && c <= 'F') {
        return 10 + c - 'A';
    } else {
        // Error
        return -1;
    }
}

/**
 * Ensure that this is called with string of length at least MAC_ADDR_STR_LEN
 * First iteration will assume separation characters
 * Caller is responsible for `free`ing returned value
 */
uint8_t *parse_mac_addr_str(char **mac_addr_str)
{
    // TODO: E_INCORRECT_MAC_ADDR_FORMAT;
    if (strlen(*mac_addr_str) != MAC_ADDR_STR_LEN)
        return NULL;

    uint8_t * mac_addr = malloc(MAC_ADDR_LEN * sizeof(int8_t));

    // return E_COULD_NOT_ALLOCATE_MEMORY;
    if (mac_addr == NULL)
        return NULL;

    LOG(INFO, "MAC Address: ");

    // MAC addr of format XX:XX:XX:XX:XX:XX so parse byte at a time
    for (int i = 0; i < MAC_ADDR_LEN; i++) {
        mac_addr[i] = (char_to_binary((*mac_addr_str)[3 * i]) << 4) |
            char_to_binary((*mac_addr_str)[3 * i + 1]);

        LOG(INFO, "%x", mac_addr[i]);
    }

    LOG(INFO, "\n");

    return mac_addr;
}
