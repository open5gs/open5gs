#define TRACE_MODULE _sgw_s5c_sm
#include "core_debug.h"

#include "sm.h"
#include "context.h"
#include "event.h"
#include "sgw_path.h"

void sgw_s11_state_initial(s11_sm_t *s, event_t *e)
{
    sgw_sm_trace(1, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &sgw_s11_state_operational);
}

void sgw_s11_state_final(s11_sm_t *s, event_t *e)
{
    sgw_sm_trace(1, e);

    d_assert(s, return, "Null param");
}

void sgw_s11_state_operational(s11_sm_t *s, event_t *e)
{
    sgw_sm_trace(1, e);

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
        case EVT_MSG_SGW_S11:
        {
            gtp_node_t *gnode = (gtp_node_t *)event_get_param1(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param2(e);
            d_assert(gnode, break, "Null param");
            d_assert(pkbuf, break, "Null param");

            d_info("EVT_MSG_MME_S11 received");
            pkbuf_free(pkbuf);
            break;
        }
        default:
        {
            d_error("No handler for event %s", sgw_event_get_name(e));
            break;
        }
    }
}
