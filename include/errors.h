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
