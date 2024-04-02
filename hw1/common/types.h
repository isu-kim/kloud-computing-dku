#ifndef HW1_TYPES_H
#define HW1_TYPES_H
#pragma once

#include <stdint.h>

#include "defines.h"

struct context_t {
    uint16_t port;
    char address[MAX_IP_STRING];
};

#endif //HW1_TYPES_H
