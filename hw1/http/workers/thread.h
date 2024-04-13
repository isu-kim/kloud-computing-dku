#ifndef HW1_THREAD_POOL_H
#define HW1_THREAD_POOL_H
#pragma once

#include <pthread.h>
#include <strings.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "../../common/log.h"
#include "../../common/types.h"
#include "utils.h"

// track total count of worker threads
sem_t alive_workers;

int spawn_worker_thread(void *);
void* worker_func(void *);

#endif
