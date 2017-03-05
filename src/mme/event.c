/**
 * @file event.c
 */

/* Core libaray */
#define TRACE_MODULE _event
#include "core_debug.h"
#include "core_msgq.h"

/* Server */
#include "event.h"

#define EVT_Q_DEPTH 16

msgq_id event_create(void)
{
    msgq_id queue_id = 0;

    /* Start threads */
    queue_id = msgq_create(EVT_Q_DEPTH, EVENT_SIZE, MSGQ_O_BLOCK);
    d_assert(queue_id != 0, return CORE_ERROR, "Message queue creation failed");

    return queue_id;
}

status_t event_delete(msgq_id queue_id)
{
    msgq_delete(queue_id);

    return CORE_OK;
}

int event_send(msgq_id queue_id, event_t *e)
{
    int r;

    d_assert(e, return -1, "Null param");
    d_assert(queue_id, return -1, "event queue isn't initialized");

    r = msgq_send(queue_id, (const char*)e, EVENT_SIZE);
    if (r != EVENT_SIZE)
    {
        d_warn("msgq_send() failed");
        return -1;
    }

    return r;
}

int event_timedrecv(msgq_id queue_id, event_t *e, c_time_t timeout)
{
    int r;

    d_assert(e, return -1, "Null param");
    d_assert(queue_id, return -1, "event queue isn't initialized");

    r = msgq_timedrecv(queue_id, (char*)e, EVENT_SIZE, timeout);
    if (r != CORE_TIMEUP && r != EVENT_SIZE)
    {
        d_warn("msgq_timedrecv() failed");
        return -1;
    }

    return r;
}

void* event_timer_expire_func(
        c_uintptr_t queue_id, c_uintptr_t event, c_uintptr_t param)
{
    event_t e;
    int r;

    d_assert(queue_id, return NULL, "Null param");
    event_set(&e, event);
    event_set_param1(&e, param);

    r = msgq_send(queue_id, (const char*)&e, EVENT_SIZE);
    if (r <= 0)
    {
        d_warn("msgq_send() failed");
    }

    return NULL;
}

tm_block_id event_timer_create(tm_service_t *tm_service)
{
    tm_block_id id;

    id = tm_create(tm_service);
    d_assert(id, return 0, "tm_create() failed");

    return id;
}

status_t event_timer_set(tm_block_id id, event_e event, tm_type_e type, 
        c_uint32_t duration, c_uintptr_t queue_id, c_uintptr_t param)
{
    d_assert(type == TIMER_TYPE_ONE_SHOT || type == TIMER_TYPE_PERIODIC,
            return 0, "param 'type' is invalid");

    tm_set(id, type, duration, (expire_func_t)event_timer_expire_func, 
            queue_id, event, param);

    return id;
}

status_t event_timer_delete(tm_block_id id)
{
    d_assert(id, return CORE_ERROR, "param 'id' is zero");

    tm_delete(id);

    return CORE_OK;
}

status_t event_timer_execute(tm_service_t *tm_service)
{
    return tm_execute_tm_service(tm_service);
}

static char FSM_NAME_INIT_SIG[] = "INIT";
static char FSM_NAME_ENTRY_SIG[] = "ENTRY";
static char FSM_NAME_EXIT_SIG[] = "EXIT";

static char EVT_NAME_LO_ENB_S1AP_ACCEPT[] = "LO_ENB_S1AP_ACCEPT";
static char EVT_NAME_LO_ENB_S1AP_CONNREFUSED[] = "LO_ENB_S1AP_CONNREFUSED";

static char EVT_NAME_MSG_ENB_S1AP[] = "MSG_ENB_S1AP";

static char EVT_NAME_UNKNOWN[] = "UNKNOWN";

char* event_get_name(event_t *e)
{
    if (e == NULL)
        return FSM_NAME_INIT_SIG;

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG: 
            return FSM_NAME_ENTRY_SIG;
        case FSM_EXIT_SIG: 
            return FSM_NAME_EXIT_SIG;

        case EVT_LO_ENB_S1AP_ACCEPT: 
               return EVT_NAME_LO_ENB_S1AP_ACCEPT;
        case EVT_LO_ENB_S1AP_CONNREFUSED: 
               return EVT_NAME_LO_ENB_S1AP_CONNREFUSED;
        case EVT_MSG_ENB_S1AP: 
               return EVT_NAME_MSG_ENB_S1AP;

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
