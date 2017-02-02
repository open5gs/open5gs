#ifndef __CORE_ARCH_MUTEX_H__
#define __CORE_ARCH_MUTEX_H__

#include "core.h"
#include "core_mutex.h"

typedef struct _mutex_t {
    pthread_mutex_t mutex;
} mutex_t;

#endif  /* __CORE_ARCH_MUTEX_H__ */

