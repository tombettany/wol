#include <inttypes.h>

#define MAC_ADDR_LEN         6
#define MAC_ADDR_STR_LEN     3 * MAC_ADDR_LEN - 1
#define MAC_ADDR_BIT_SZ      MAC_ADDR_LEN * 8

uint8_t char_to_binary(char c);
uint8_t *parse_mac_addr_str(char **mac_addr_str);
