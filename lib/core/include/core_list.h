#ifndef __CORE_LIST_H__
#define __CORE_LIST_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct _ln_t {
    struct _ln_t *prev;
    struct _ln_t *next;
};

typedef struct _ln_t ln_t;
typedef struct _ln_t list_t;
typedef struct _ln_t lnode_t;

#define list_init(__pname) do {\
    (__pname)->prev = NULL; \
    (__pname)->next = NULL; \
} while (0)

#define list_first(__pname) ((void*)((__pname)->next))

#define list_last(__pname) ((void*)((__pname)->prev))

#define list_prev(__ptr_node) ((void*)(((ln_t *)(__ptr_node))->prev))

#define list_next(__ptr_node) ((void*)(((ln_t *)(__ptr_node))->next))

#define list_is_empty(__pname) ((__pname)->next == NULL)

#define list_prepend(__pname, __ptr_new) do { \
    ((ln_t*)(__ptr_new))->prev = NULL; \
    ((ln_t*)(__ptr_new))->next = (__pname)->next; \
    if ((__pname)->next) \
        ((__pname)->next)->prev = (ln_t*)(__ptr_new); \
    else \
        (__pname)->prev = (ln_t*)(__ptr_new); \
    (__pname)->next = (ln_t*)(__ptr_new); \
} while (0)

#define list_append(__pname, __ptr_new) do { \
    ((ln_t*)(__ptr_new))->prev = (__pname)->prev; \
    ((ln_t*)(__ptr_new))->next = NULL; \
    if ((__pname)->prev) \
        ((__pname)->prev)->next = (ln_t*)(__ptr_new); \
    else \
        (__pname)->next = (ln_t*)(__ptr_new); \
    ((__pname)->prev) = (ln_t*)(__ptr_new); \
} while (0)

#define list_insert_prev(__pname, __ptr_node, __ptr_new) do { \
    ((ln_t*)(__ptr_new))->prev = ((ln_t*)(__ptr_node))->prev; \
    ((ln_t*)(__ptr_new))->next = (ln_t*)(__ptr_node); \
    if (((ln_t*)(__ptr_node))->prev) \
        ((ln_t*)(__ptr_node))->prev->next = (ln_t*)(__ptr_new); \
    else \
        (__pname)->next = (ln_t*)(__ptr_new); \
    ((ln_t*)(__ptr_node))->prev = ((ln_t*)(__ptr_new)); \
} while (0)

#define list_insert_next(__pname, __ptr_node, __ptr_new) do { \
    ((ln_t*)(__ptr_new))->prev = (ln_t*)(__ptr_node); \
    ((ln_t*)(__ptr_new))->next = ((ln_t*)(__ptr_node))->next; \
    if (((ln_t*)(__ptr_node))->next) \
        ((ln_t*)(__ptr_node))->next->prev = (ln_t*)(__ptr_new); \
    else \
        (__pname)->prev = (ln_t*)(__ptr_new); \
    ((ln_t*)(__ptr_node))->next = ((ln_t*)(__ptr_new)); \
} while (0)

#define list_remove(__pname, __ptr_node) do { \
    ln_t *_iter = (__pname)->next; \
    while (_iter) { \
        if (_iter == (ln_t*)(__ptr_node)) { \
            if (_iter->prev) \
                _iter->prev->next = _iter->next; \
            else \
                (__pname)->next = _iter->next; \
            if (_iter->next) \
                _iter->next->prev = _iter->prev; \
            else \
                (__pname)->prev = _iter->prev; \
            break; \
        } \
        _iter = _iter->next; \
    } \
} while (0)

typedef int (*ln_cmp_cb)(lnode_t *pnode1, lnode_t *pnode2);

#define list_insert_sorted(__pname, __ptr_new, __cmp_callback) do { \
    ln_cmp_cb __pcb = (ln_cmp_cb)__cmp_callback; \
    ln_t *_iter = list_first(__pname); \
    while (_iter) { \
        if ((*__pcb)((ln_t*)(__ptr_new), _iter) < 0) { \
            list_insert_prev(__pname, _iter, __ptr_new); \
            break; \
        } \
        _iter = list_next(_iter); \
    } \
    if (_iter == NULL) \
        list_append(__pname, __ptr_new); \
} while (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __CORE_LIST_H__ */
