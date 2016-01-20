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
