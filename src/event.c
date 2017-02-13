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

int g_initialized = 0;

msgq_id g_mme_evt_q = 0;

tm_service_t g_tm_serv;

status_t event_init(void)
{
    if (g_initialized)
        return CORE_OK;

    tm_service_init(&g_tm_serv);

    /* Start threads */
    g_mme_evt_q = msgq_create(EVT_Q_DEPTH, EVENT_SIZE, MSGQ_O_BLOCK);
    d_assert(g_mme_evt_q != 0, return CORE_ERROR, 
            "Message queue creation failed");

    g_initialized = 1;

    return CORE_OK;
}

status_t event_final(void)
{
    if (!g_initialized)
        return CORE_OK;

    msgq_delete(g_mme_evt_q);

    g_initialized = 0;

    return CORE_OK;
}

int event_send(event_t *e)
{
    int r;

    d_assert(g_initialized, return -1, "event framework isn't initialized");
    d_assert(g_mme_evt_q, return -1, "event queue isn't initialized");
    d_assert(e, return -1, "Null param");

    r = msgq_send(g_mme_evt_q, (const char*)e, EVENT_SIZE);
    if (r != EVENT_SIZE)
    {
        d_error("msgq_send() failed");
        return -1;
    }

    return r;
}

int event_timedrecv(event_t *e, c_time_t timeout)
{
    int r;

    d_assert(g_initialized, return -1, "event framework isn't initialized");
    d_assert(g_mme_evt_q, return -1, "event queue isn't initialized");
    d_assert(e, return -1, "Null param");

    r = msgq_timedrecv(g_mme_evt_q, (char*)e, EVENT_SIZE, timeout);
    if (r != CORE_TIMEUP && r != EVENT_SIZE)
    {
        d_error("msgq_timedrecv() failed");
        return -1;
    }

    return r;
}

void* event_timer_expire_func(c_uintptr_t arg1, c_uintptr_t arg2, c_uintptr_t arg3)
{
    event_t e;
    int r;

    event_set(&e, arg1, arg2);
    event_set_param2(&e, arg3);

    r = msgq_send(g_mme_evt_q, (const char*)&e, EVENT_SIZE);
    if (r <= 0)
    {
        d_error("msgq_send() failed");
    }

    return NULL;
}

tm_block_id event_timer_create(void)
{
    tm_block_id id;

    d_assert(g_initialized, return 0, "event framework isn't initialized");

    id = tm_create(&g_tm_serv);
    d_assert(id, return 0, "tm_create() failed");

    return id;
}

status_t event_timer_set(tm_block_id id, event_e te, tm_type_e type, 
        c_uint32_t duration, c_uintptr_t param1, c_uintptr_t param2)
{
    d_assert(g_initialized, return 0, "event framework isn't initialized");
    d_assert(type == TIMER_TYPE_ONE_SHOT || type == TIMER_TYPE_PERIODIC,
            return 0, "param 'type' is invalid");

    tm_set(id, type, duration, (expire_func_t)event_timer_expire_func, 
            te, param1, param2);

    return id;
}

status_t event_timer_delete(tm_block_id id)
{
    d_assert(g_initialized, return CORE_ERROR,
            "event framework isn't initialized");
    d_assert(id, return CORE_ERROR, "param 'id' is zero");

    tm_delete(id);

    return CORE_OK;
}

status_t event_timer_execute(void)
{
    d_assert(g_initialized, return CORE_ERROR,
            "event framework isn't initialized");

    return tm_execute_tm_service(&g_tm_serv);
}

static char FSM_NAME_INIT_SIG[] = "INIT";
static char FSM_NAME_ENTRY_SIG[] = "ENTRY";
static char FSM_NAME_EXIT_SIG[] = "EXIT";

static char EVT_NAME_LO_ENB_S1_ACCEPT[] = "LO_ENB_S1_ACCEPT";
static char EVT_NAME_LO_ENB_S1_CONNREFUSED[] = "LO_ENB_S1_CONNREFUSED";

static char EVT_NAME_TM_MME_S1_WAIT_CONN[] = "TM_MME_S1_WAIT_CONN";

static char EVT_NAME_S1_ENB_INF[] = "S1_ENB_INF";

static char EVT_NAME_UNKNOWN[] = "UNKNOWN";

char* event_get_name(event_t *e)
{
    if (e == NULL)
        return FSM_NAME_INIT_SIG;

    if (event_get(e) < EVT_MSG_BASE)
    {
        switch (event_get(e))
        {
            case FSM_ENTRY_SIG: return FSM_NAME_ENTRY_SIG;
            case FSM_EXIT_SIG: return FSM_NAME_EXIT_SIG;

            case EVT_LO_ENB_S1_ACCEPT: return EVT_NAME_LO_ENB_S1_ACCEPT;
            case EVT_LO_ENB_S1_CONNREFUSED: 
                   return EVT_NAME_LO_ENB_S1_CONNREFUSED;

            case EVT_TM_MME_S1_WAIT_CONN: return EVT_NAME_TM_MME_S1_WAIT_CONN;

            default: return EVT_NAME_UNKNOWN;
        }
    }
    else if (event_get(e) < EVT_MSG_TOP)
    {
        switch (event_get(e))
        {
            case EVT_S1_ENB_INF: return EVT_NAME_S1_ENB_INF;
            default: return EVT_NAME_UNKNOWN;
        }
    }

    return EVT_NAME_UNKNOWN;
}
