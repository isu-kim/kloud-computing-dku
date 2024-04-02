#ifndef HW1_CONTEXT_H
#define HW1_CONTEXT_H
#pragma once

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "types.h"
#include "defines.h"

int init_ctx(struct context_t **ctx, int argc, char** argv);
int dump_ctx(struct context_t ctx);

#endif //HW1_CONTEXT_H
