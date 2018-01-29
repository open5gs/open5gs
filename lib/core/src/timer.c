#define TRACE_MODULE _timer
#include "core_debug.h"
#include "core_timer.h"
#include "core_time.h"
#include "core_param.h"
#include "core_pool.h"

typedef struct _tm_block_t {
    lnode_t node;

    tm_service_t    *tm_s;
    c_uint32_t      expire_time;

    expire_func_t   expire_func;
    c_uintptr_t     param1;
    c_uintptr_t     param2;
    c_uintptr_t     param3;
    c_uintptr_t     param4;
    c_uintptr_t     param5;
    c_uintptr_t     param6;

    tm_type_e       type;
    c_uint8_t       running;
    c_uint32_t      duration;
} tm_block_t;

pool_declare(timer_pool, tm_block_t, MAX_NUM_OF_TIMER);

static int _tm_cmp_func(lnode_t *pnode1, lnode_t *pnode2);
static tm_block_t *_tm_get(void);
static void _tm_free(tm_block_t *tm);

#define _tm_add(__l, __tm) \
    list_insert_sorted(__l, __tm, _tm_cmp_func)

#define _tm_remove(__l, __tm) \
    list_remove(__l, __tm)

status_t tm_init(void)
{
    pool_init(&timer_pool, MAX_NUM_OF_TIMER);
    return CORE_OK;
}

status_t tm_final(void)
{
    if (pool_size(&timer_pool) != pool_avail(&timer_pool))
        d_error("%d not freed in timer_pool[%d]",
                pool_size(&timer_pool) - pool_avail(&timer_pool),
                pool_size(&timer_pool));
    d_trace(9, "%d not freed in timer_pool[%d]\n",
            pool_size(&timer_pool) - pool_avail(&timer_pool),
            pool_size(&timer_pool));
    pool_final(&timer_pool);
    return CORE_OK;
}

c_uint32_t tm_pool_avail(void)
{
    return pool_avail(&timer_pool);
}

void tm_service_init(tm_service_t *tm_service)
{
    memset(tm_service, 0x00, sizeof(tm_service_t));
    list_init(&tm_service->active_list);
    list_init(&tm_service->idle_list);
    return;
}

status_t tm_execute_tm_service(tm_service_t *p_tm_s, c_uintptr_t data)
{
    c_uint32_t cur_time;
    tm_block_t *tm;

    cur_time = time_as_msec(time_now());
    tm = list_first(&(p_tm_s->active_list));

    while(tm)
    {
        if(tm->expire_time < cur_time)
        {
            /* execute expiry function */
            tm->expire_func(data, tm->param1, tm->param2, tm->param3,
                    tm->param4, tm->param5, tm->param6);

            /* remove this tm_block from the active list */
            _tm_remove(&(p_tm_s->active_list), tm);

            if(tm->type == TIMER_TYPE_PERIODIC)
            {
                tm->expire_time = cur_time + tm->duration;

                /* add this tm_block to the active list */
                _tm_add(&(p_tm_s->active_list), tm);
            }
            else
            {
                /* add this tm_block to the idle list */
                _tm_add(&(p_tm_s->idle_list), tm);
                tm->running = 0;
            }

            tm = list_first(&(p_tm_s->active_list));
        }
        else
        {
            break;
        }
    }
    return CORE_OK;
}

static int _tm_cmp_func(lnode_t *pnode1, lnode_t *pnode2)
{
    tm_block_t *tm1 = (tm_block_t*)pnode1;
    tm_block_t *tm2 = (tm_block_t*)pnode2;

    if(tm1->expire_time < tm2->expire_time)
        return -1;
    else
        return 1;
}

static tm_block_t *_tm_get(void)
{
    tm_block_t *tm_b = NULL;

    /* get timer node from node pool */
    pool_alloc_node(&timer_pool, &tm_b);

    /* check for error */
    d_assert(tm_b != NULL, return NULL, "fail to get timer pool\n");

    /* intialize timer node */
    memset((char*)tm_b, 0x00, sizeof(tm_block_t));
    return tm_b;

}

static void _tm_free(tm_block_t *tm)
{
    /* free tlv node to the node pool */
    pool_free_node(&timer_pool, tm);
    return;

}

tm_block_id tm_create(tm_service_t *tm_service,
        tm_type_e type, c_uint32_t duration, expire_func_t expire_func)
{
    tm_block_t *tm = NULL;
    tm = _tm_get();

    d_assert(tm, return 0, "tm_create failed\n");
    tm->tm_s = tm_service;

    _tm_add(&(tm->tm_s->idle_list), tm);

    tm->type = type;
    tm->duration = duration;
    tm->expire_func = expire_func;

    return (tm_block_id)tm;
}

void tm_delete(tm_block_id id)
{
    tm_block_t *tm = (tm_block_t *)id;

    /* If running timer, pop it from active list */
    if (tm->running == 1)
        _tm_remove(&(tm->tm_s->active_list), tm);
    /* If not running timer, pop it from idle list */
    else
        _tm_remove(&(tm->tm_s->idle_list), tm);

    _tm_free(tm);

    return;
}

status_t tm_set_duration(tm_block_id id, c_uint32_t duration)
{
    tm_block_t *tm = (tm_block_t *)id;

    tm->duration = duration;

    return CORE_OK;
}

status_t tm_set_param1(tm_block_id id, c_uintptr_t param)
{
    tm_block_t *tm = (tm_block_t *)id;

    tm->param1 = param;

    return CORE_OK;
}

status_t tm_set_param2(tm_block_id id, c_uintptr_t param)
{
    tm_block_t *tm = (tm_block_t *)id;

    tm->param2 = param;

    return CORE_OK;
}

status_t tm_set_param3(tm_block_id id, c_uintptr_t param)
{
    tm_block_t *tm = (tm_block_t *)id;

    tm->param3 = param;

    return CORE_OK;
}

status_t tm_set_param4(tm_block_id id, c_uintptr_t param)
{
    tm_block_t *tm = (tm_block_t *)id;

    tm->param4 = param;

    return CORE_OK;
}

status_t tm_set_param5(tm_block_id id, c_uintptr_t param)
{
    tm_block_t *tm = (tm_block_t *)id;

    tm->param5 = param;

    return CORE_OK;
}

status_t tm_set_param6(tm_block_id id, c_uintptr_t param)
{
    tm_block_t *tm = (tm_block_t *)id;

    tm->param6 = param;

    return CORE_OK;
}

status_t tm_start(tm_block_id id)
{
    c_uint32_t cur_time = time_as_msec(time_now());
    tm_block_t *tm = (tm_block_t *)id;

    /* If already running timer, pop it from active list for put again */
    if (tm->running == 1)
        _tm_remove(&(tm->tm_s->active_list), tm);
    /* If not running, tm is in idle list. pop it */
    else
        _tm_remove(&(tm->tm_s->idle_list), tm);

    /* Calculate expiration */
    tm->expire_time = cur_time + tm->duration;

    /* Push tm to active list */
    _tm_add(&(tm->tm_s->active_list), tm);

    tm->running = 1;

    return CORE_OK;
}

status_t tm_stop(tm_block_id id)
{
    tm_block_t *tm = (tm_block_t *)id;

    /* If already stopped timer, no operations needed */
    if (tm->running == 0)
        return CORE_OK;

    _tm_remove(&(tm->tm_s->active_list), tm);
    _tm_add(&(tm->tm_s->idle_list), tm);

    tm->running = 0;

    return CORE_OK;
}
