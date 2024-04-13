#ifndef HW1_SERVER_H
#define HW1_SERVER_H
#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../common/log.h"
#include "../common/context.h"

#include "file/file.h"

// @todo implement basic socket programming with multi threading
// @todo implement callback function
// @todo implement thread pool workers with multi processing and multi threading

extern struct context_t *ctx;

int serve();

#endif //HW1_SERVER_H
