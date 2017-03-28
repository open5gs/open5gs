#define TRACE_MODULE _mme_s11_sm

#include "core_debug.h"

#include "event.h"
#include "context.h"

void mme_s11_state_initial(s11_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);

    FSM_TRAN(s, &mme_s11_state_operational);
}

void mme_s11_state_final(s11_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);
}

void mme_s11_state_operational(s11_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    s11_ctx_t *s11 = s->ctx;
    d_assert(s11, return, "Null param");

    mme_sm_trace(1, e);

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
        case EVT_MSG_MME_S11:
        {
            break;
        }
        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

void mme_s11_state_exception(s11_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(1, e);

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
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}
