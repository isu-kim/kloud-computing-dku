#ifndef HW1_TYPES_H
#define HW1_TYPES_H
#pragma once

#include <stdint.h>
#include <arpa/inet.h>

#include "defines.h"

struct client_info_t {
    int fd;
    char addr_str[MAX_IP_STRING];
    uint16_t port;
};

struct http_request_info_t {
    uint8_t method;
    char http_version[HTTP_MAX_VERSION_STR];
    char endpoint_str[HTTP_MAX_ENDPOINT_LEN];
    long content_size;
};

#endif //HW1_TYPES_H
