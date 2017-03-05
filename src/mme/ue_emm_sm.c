#define TRACE_MODULE _ue_emm_sm

#include "core_debug.h"

#include "sm.h"
#include "context.h"
#include "event.h"

void ue_emm_state_initial(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    sm_trace(1, e);

    FSM_TRAN(s, &ue_emm_state_operational);
}

void ue_emm_state_final(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    sm_trace(1, e);
}

void ue_emm_state_operational(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    ue_ctx_t *ue = s->ctx;
    d_assert(ue, return, "Null param");

    sm_trace(1, e);

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
        case EVT_ENB_S1AP_INF:
        {
            break;
        }

        default:
        {
            d_error("Unknown event %s", event_get_name(e));
            break;
        }
    }
}

void ue_emm_state_exception(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    sm_trace(1, e);

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
            d_error("Unknown event %s", event_get_name(e));
            break;
        }
    }
}
