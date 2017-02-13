#define TRACE_MODULE _master_sm
#include "core_debug.h"

#include "sm.h"
#include "context.h"
#include "event.h"

void mme_state_initial(mme_sm_t *s, event_t *e)
{
    sm_trace(1, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &mme_state_operational);
}

void mme_state_final(mme_sm_t *s, event_t *e)
{
    sm_trace(1, e);

    d_assert(s, return, "Null param");
}

void mme_state_operational(mme_sm_t *s, event_t *e)
{
#if 0
    char buf[INET_ADDRSTRLEN];
#endif

    sm_trace(1, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        default:
        {
            d_error("No handler for event %s", event_get_name(e));
            break;
        }
    }

    /* If event was packet type, its buffer allocated by data-plane should
     * be freed here */
    if (event_is_msg(e))
        pkbuf_free(event_get_msg_pkb(e));
}

void mme_state_exception(mme_sm_t *s, event_t *e)
{
    sm_trace(1, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        default:
        {
            d_error("No handler for event %s", event_get_name(e));
            break;
        }
    }
}
