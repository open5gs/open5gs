#ifndef __CORE_INDEX_H__
#define __CORE_INDEX_H__

#include "core.h"
#include "core_mutex.h"

typedef c_uint32_t index_t;

#define INVALID_INDEX 0

#define index_declare(__name, __type, __size) \
    typedef struct { \
        int head, tail; \
        int size, avail; \
        index_t free_index[__size]; \
        __type *index_pool[__size + 1]; \
        __type *free_pool[__size], node_pool[__size]; \
        mutex_id mut; \
    } index_##__name##_t; \
    index_##__name##_t __name

#define index_init(__pname, __size) do { \
    int __i; \
    mutex_create(&(__pname)->mut, MUTEX_DEFAULT); \
    (__pname)->size = (__pname)->avail = __size; \
    (__pname)->head = (__pname)->tail = 0; \
    for (__i = 0; __i < __size; __i++) \
    { \
        (__pname)->free_index[__i] = (__i + 1); \
        (__pname)->index_pool[__i + 1] = NULL; \
        (__pname)->free_pool[__i] = &((__pname)->node_pool[__i]); \
    } \
} while (0)

#define index_final(__pname) \
    ((__pname)->mut ? mutex_delete((__pname)->mut) : CORE_OK)

#define index_init_wo_lock(__pname, __size) do { \
    int __i; \
    (__pname)->mut = 0; \
    (__pname)->size = (__pname)->avail = __size; \
    (__pname)->head = (__pname)->tail = 0; \
    for (__i = 0; __i < __size; __i++) \
    { \
        (__pname)->free_index[__i] = (__i + 1); \
        (__pname)->index_pool[__i + 1] = NULL; \
        (__pname)->free_pool[__i] = &((__pname)->node_pool[__i]); \
    } \
} while (0)

#define index_alloc(__pname, __pptr_node) do { \
    if ((__pname)->mut) mutex_lock((__pname)->mut); \
    if ((__pname)->avail > 0) { \
        (__pname)->avail--; \
        *(__pptr_node) = (void*)(__pname)->free_pool[(__pname)->head]; \
        memset(*(__pptr_node), 0, sizeof(**(__pptr_node))); \
        (__pname)->free_pool[(__pname)->head] = NULL; \
        ((*(__pptr_node))->index) = (__pname)->free_index[(__pname)->head]; \
        (__pname)->index_pool[((*(__pptr_node))->index)] = *(__pptr_node); \
        (__pname)->free_index[(__pname)->head] = INVALID_INDEX; \
        (__pname)->head = ((__pname)->head + 1) % ((__pname)->size); \
    } \
    if ((__pname)->mut) mutex_unlock((__pname)->mut); \
} while (0)

#define index_free(__pname, __ptr_node) do { \
    if ((__pname)->mut) mutex_lock((__pname)->mut); \
    if ((__pname)->avail < (__pname)->size) { \
        (__pname)->avail++; \
        (__pname)->index_pool[((__ptr_node)->index)] = NULL; \
        (__pname)->free_index[(__pname)->tail] = ((__ptr_node)->index); \
        (__pname)->free_pool[(__pname)->tail] = (void*)(__ptr_node); \
        (__pname)->tail = ((__pname)->tail + 1) % ((__pname)->size); \
    } \
    if ((__pname)->mut) mutex_unlock((__pname)->mut); \
} while (0)

#define index_find(__pname, __index) (void*)(__pname)->index_pool[(__index)];

#define index_size(__pname) ((__pname)->size)

#define index_avail(__pname) ((__pname)->avail)

#define index_used(__pname) (index_size(__pname) - index_avail(__pname))

#endif /* ! __CORE_INDEX_H__ */
