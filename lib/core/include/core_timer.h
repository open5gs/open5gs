#ifndef __CORE_TIMER_H__
#define __CORE_TIMER_H__

#include "core.h"
#include "core_errno.h"
#include "core_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum _tm_type {
    TIMER_TYPE_ONE_SHOT,
    TIMER_TYPE_PERIODIC
} tm_type_e;

typedef struct _tm_service_t {
    list_t active_list;
    list_t idle_list;
    /* timer basic element */
} tm_service_t;

typedef c_uintptr_t tm_block_id;

typedef void (*expire_func_t)(c_uintptr_t data,
        c_uintptr_t param1, c_uintptr_t param2, c_uintptr_t param3,
        c_uintptr_t param4, c_uintptr_t param5, c_uintptr_t param6);

typedef struct _tm_desc_t {
    tm_type_e       type;
    c_uint32_t      duration;
    expire_func_t   expire_func;
    c_uintptr_t     param1;
    c_uintptr_t     param2;
    c_uintptr_t     param3;
    c_uintptr_t     param4;
    c_uintptr_t     param5;
    c_uintptr_t     param6;
} tm_desc_t;

CORE_DECLARE(status_t) tm_init(void);
CORE_DECLARE(status_t) tm_final(void);

CORE_DECLARE(c_uint32_t) tm_pool_avail(void);

CORE_DECLARE(void) tm_service_init(tm_service_t *tm_service);
CORE_DECLARE(status_t) tm_execute_tm_service(
        tm_service_t *p_tm_s, c_uintptr_t data);

CORE_DECLARE(tm_block_id) tm_create(tm_service_t *tm_service,
        tm_type_e type, c_uint32_t duration, expire_func_t expire_func);
CORE_DECLARE(void) tm_delete(tm_block_id id);
CORE_DECLARE(status_t) tm_set_duration(tm_block_id id, c_uint32_t duration);
CORE_DECLARE(status_t) tm_set_param1(tm_block_id id, c_uintptr_t param);
CORE_DECLARE(status_t) tm_set_param2(tm_block_id id, c_uintptr_t param);
CORE_DECLARE(status_t) tm_set_param3(tm_block_id id, c_uintptr_t param);
CORE_DECLARE(status_t) tm_set_param4(tm_block_id id, c_uintptr_t param);
CORE_DECLARE(status_t) tm_set_param5(tm_block_id id, c_uintptr_t param);
CORE_DECLARE(status_t) tm_set_param6(tm_block_id id, c_uintptr_t param);
CORE_DECLARE(status_t) tm_start(tm_block_id id);
CORE_DECLARE(status_t) tm_stop(tm_block_id id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
