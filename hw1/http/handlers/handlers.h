#ifndef HW1_HANDLERS_H
#define HW1_HANDLERS_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../../common/types.h"
#include "../../common/defines.h"
#include "../../common/context.h"
#include "../../common/messages.h"
#include "../../common/log.h"
#include "../utils/utils.h"
#include "../file/file.h"

extern struct context_t *ctx;

int http_handler_get(struct client_info_t*, struct http_request_info_t*);
int http_handler_post(struct client_info_t*, struct http_request_info_t*, char*, long);

int generate_http_header(int, unsigned long, char*, char*);
int send_response_message(int, int, char*, char*);

#endif //HW1_HANDLERS_H
