#ifndef __CORE_ARCH_THREAD_H__
#define __CORE_ARCH_THREAD_H__

#include "core_thread.h"
#include "core_semaphore.h"

typedef struct _thread_t {
    pthread_t thread;
    void *data;
    thread_start_t func;
    status_t exitval;

    semaphore_id semaphore;
} thread_t;

struct threadattr_t {
    pthread_attr_t attr;
};

#endif  /* __CORE_ARCH_THREAD_H__ */

