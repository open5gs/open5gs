#ifndef __EVENT_H__
#define __EVENT_H__

#include "core_msgq.h"
#include "core_timer.h"
#include "core_fsm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EVENT_SIZE sizeof(event_t)

#define event_set(__ptr_e, __evnt) ((__ptr_e)->event = (__evnt))
#define event_get(__ptr_e) ((__ptr_e)->event)

#define event_set_param1(__ptr_e, __param) \
    ((__ptr_e)->param1 = (c_uintptr_t)(__param))
#define event_set_param2(__ptr_e, __param) \
    ((__ptr_e)->param2 = (c_uintptr_t)(__param))
#define event_set_param3(__ptr_e, __param) \
    ((__ptr_e)->param3 = (c_uintptr_t)(__param))
#define event_set_param4(__ptr_e, __param) \
    ((__ptr_e)->param4 = (c_uintptr_t)(__param))
#define event_set_param5(__ptr_e, __param) \
    ((__ptr_e)->param5 = (c_uintptr_t)(__param))
#define event_set_param6(__ptr_e, __param) \
    ((__ptr_e)->param6 = (c_uintptr_t)(__param))
#define event_set_param7(__ptr_e, __param) \
    ((__ptr_e)->param7 = (c_uintptr_t)(__param))
#define event_set_param8(__ptr_e, __param) \
    ((__ptr_e)->param8 = (c_uintptr_t)(__param))

#define event_get_param1(__ptr_e) ((__ptr_e)->param1)
#define event_get_param2(__ptr_e) ((__ptr_e)->param2)
#define event_get_param3(__ptr_e) ((__ptr_e)->param3)
#define event_get_param4(__ptr_e) ((__ptr_e)->param4)
#define event_get_param5(__ptr_e) ((__ptr_e)->param5)
#define event_get_param6(__ptr_e) ((__ptr_e)->param6)
#define event_get_param7(__ptr_e) ((__ptr_e)->param7)
#define event_get_param8(__ptr_e) ((__ptr_e)->param8)

#define timer_create(__tm_service, __ptr_e, __duration) \
    event_timer_create((__tm_service), TIMER_TYPE_ONE_SHOT, \
            (__duration), (__ptr_e))
#define periodic_timer_create(__tm_service, __ptr_e, __duration) \
    event_timer_create((__tm_service), TIMER_TYPE_PERIODIC, \
            (__duration), (__ptr_e))

#define timer_set_param1(__ptr_e, __param) tm_set_param2(__ptr_e, __param);
#define timer_set_param2(__ptr_e, __param) tm_set_param3(__ptr_e, __param);
#define timer_set_param3(__ptr_e, __param) tm_set_param4(__ptr_e, __param);
#define timer_set_param4(__ptr_e, __param) tm_set_param5(__ptr_e, __param);
#define timer_set_param5(__ptr_e, __param) tm_set_param6(__ptr_e, __param);

typedef struct {
    fsm_event_t event;
    c_uintptr_t param1;
    c_uintptr_t param2;
    c_uintptr_t param3;
    c_uintptr_t param4;
    c_uintptr_t param5;
    c_uintptr_t param6;
    c_uintptr_t param7;
    c_uintptr_t param8;
} event_t;

extern char *EVT_NAME_UNKNOWN;

/**
 * Create event message queue
 *
 * @return event queue or 0
 */
CORE_DECLARE(msgq_id) event_create(int opt);

/**
 * Delete event message queue
 *
 * @return CORE_OK or CORE_ERROR
 */
CORE_DECLARE(status_t) event_delete(msgq_id queue_id);


/**
 * Send a event to event queue
 *
 * @return If success, return CORE_OK
 *         If queue is full, return CORE_EAGAIN
 *         If else, return CORE_ERROR
 */
CORE_DECLARE(status_t) event_send(msgq_id queue_id, event_t *e);

/**
 * Receive a event from event queue
 *
 * @return If success, return CORE_OK
 *         If queue is empty, return CORE_EAGAIN
 *         If else, return CORE_ERROR.
 */
CORE_DECLARE(status_t) event_recv(msgq_id queue_id, event_t *e);

/**
 * Receive a event from event queue with timeout
 *
 * @return If success, return CORE_OK
 *         If timout occurs, return CORE_TIMEUP.
 *         If queue is empty, return CORE_EAGAIN
 *         If else, return CORE_ERROR.
 */
CORE_DECLARE(status_t) event_timedrecv(
        msgq_id queue_id, event_t *e, c_time_t timeout);

/**
 * Create a timer
 */
CORE_DECLARE(tm_block_id) event_timer_create(tm_service_t *tm_service,
        tm_type_e type, c_uint32_t duration, c_uintptr_t event);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EVENT_H__ */
