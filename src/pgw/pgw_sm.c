#define TRACE_MODULE _pgw_sm
#include "core_debug.h"

#include "pgw_sm.h"
#include "pgw_context.h"
#include "pgw_event.h"
#include "pgw_path.h"
#include "pgw_handler.h"

void pgw_state_initial(fsm_t *s, event_t *e)
{
    pgw_sm_trace(1, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &pgw_state_operational);
}

void pgw_state_final(fsm_t *s, event_t *e)
{
    pgw_sm_trace(1, e);

    d_assert(s, return, "Null param");
}

void pgw_state_operational(fsm_t *s, event_t *e)
{
    status_t rv;

    pgw_sm_trace(1, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            rv = pgw_path_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish S11 path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = pgw_path_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close S11 path");
                break;
            }
            break;
        }
        case EVT_MSG_PGW_S5C:
        {
            status_t rv;
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            gtp_node_t *gnode = (gtp_node_t *)event_get_param2(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            gtp_xact_t *xact = NULL;
            c_uint8_t type;
            c_uint32_t teid;
            gtp_message_t gtp_message;

            d_assert(pkbuf, break, "Null param");
            d_assert(sock, pkbuf_free(pkbuf); break, "Null param");
            d_assert(gnode, pkbuf_free(pkbuf); break, "Null param");

            rv = gtp_xact_receive(
                    &pgw_self()->gtp_xact_ctx, sock, gnode,
                    &xact, &type, &teid, &gtp_message, pkbuf);
            if (rv != CORE_OK)
                break;

            switch(type)
            {
                case GTP_CREATE_SESSION_REQUEST_TYPE:
                    pgw_handle_create_session_request(
                        xact, &gtp_message.create_session_request);
                    break;
                default:
                    d_warn("Not implmeneted(type:%d)", type);
                    break;
            }
            pkbuf_free(pkbuf);
            break;
        }
        case EVT_TM_PGW_T3:
        {
            gtp_xact_timeout(event_get_param1(e));
            break;
        }
        default:
        {
            d_error("No handler for event %s", pgw_event_get_name(e));
            break;
        }
    }
}
