#ifndef HW1_FILE_H
#define HW1_FILE_H
#pragma once

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../../common/log.h"
#include "../../common/context.h"

#define IS_EXISTING_FILE(target) (access(target, F_OK) != -1)
#define IS_AVAILABLE_FILE(target) (access(target, F_OK | W_OK | R_OK) != -1)

extern struct context_t* ctx;

int check_files_dir();

#endif //HW1_FILE_H
