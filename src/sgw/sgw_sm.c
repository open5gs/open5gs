#define TRACE_MODULE _sgw_sm
#include "core_debug.h"

#include "sm.h"
#include "context.h"
#include "event.h"
#include "s11_path.h"

void sgw_state_initial(sgw_sm_t *s, event_t *e)
{
    sgw_sm_trace(1, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &sgw_state_operational);
}

void sgw_state_final(sgw_sm_t *s, event_t *e)
{
    sgw_sm_trace(1, e);

    d_assert(s, return, "Null param");
}

void sgw_state_operational(sgw_sm_t *s, event_t *e)
{
    status_t rv;

    sgw_sm_trace(1, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            rv = sgw_s11_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish S11 path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = sgw_s11_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close S11 path");
                break;
            }
            break;
        }
        case EVT_MSG_SGW_S11:
        {
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            d_assert(sock, break, "Null param");

            d_info("EVT_MSG_MME_S11 received");
            break;
        }
        default:
        {
            d_error("No handler for event %s", sgw_event_get_name(e));
            break;
        }
    }

    /* If event was packet type, its buffer allocated by data-plane should
     * be freed here */
}
