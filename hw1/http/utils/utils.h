#ifndef HW1_UTILS_H
#define HW1_UTILS_H

#include <string.h>
#include <ctype.h>
#include <stddef.h>

#include "../../common/types.h"
#include "../../common/defines.h"

#ifdef DEBUG
#include "../../common/log.h"
#endif


int parse_request_info(char *, struct http_request_info_t*);

uint16_t atom(char *);
int mtoa(uint16_t type, char *ret);

#endif //HW1_UTILS_H
