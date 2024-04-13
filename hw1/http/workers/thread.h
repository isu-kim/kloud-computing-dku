#ifndef HW1_THREAD_POOL_H
#define HW1_THREAD_POOL_H
#pragma once

#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "../../common/log.h"
#include "../../common/types.h"
#include "../handlers/handlers.h"
#include "../utils/utils.h"

int spawn_worker_thread(void *);
void* worker_func(void *);

#endif
