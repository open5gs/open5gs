#ifndef __CORE_ARCH_SEMAPHORE_H__
#define __CORE_ARCH_SEMAPHORE_H__

#include "core.h"
#include "core_semaphore.h"

typedef struct _semaphore_t {
    sem_t *semaphore;
} semaphore_t;

#endif  /* __CORE_ARCH_SEMAPHORE_H__ */

