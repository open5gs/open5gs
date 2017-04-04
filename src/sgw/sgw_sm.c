#define TRACE_MODULE _sgw_sm
#include "core_debug.h"

#include "sgw_ctx.h"
#include "sgw_event.h"
#include "sgw_path.h"
#include "sgw_handler.h"

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
            rv = sgw_path_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish S11 path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = sgw_path_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close S11 path");
                break;
            }
            break;
        }
        case EVT_MSG_SGW_S11:
        case EVT_MSG_SGW_S5C:
        {
            status_t rv;
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            gtp_node_t *gnode = (gtp_node_t *)event_get_param2(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            gtp_xact_t *xact = NULL;
            c_uint8_t type;
            gtp_message_t gtp_message;

            d_assert(pkbuf, break, "Null param");
            d_assert(sock, pkbuf_free(pkbuf); break, "Null param");
            d_assert(gnode, pkbuf_free(pkbuf); break, "Null param");

            xact = gtp_xact_preprocess(&sgw_self()->gtp_xact_ctx, 
                    sock, gnode, &type, pkbuf);
            rv = gtp_xact_receive(xact, &gtp_message, type, pkbuf);
            if (rv != CORE_OK) 
                break;

            switch(type)
            {
                case GTP_CREATE_SESSION_REQUEST_TYPE:
                    sgw_handle_create_session_request(
                            xact, type, &gtp_message);
                    break;
                case GTP_CREATE_SESSION_RESPONSE_TYPE:
                    sgw_handle_create_session_response(
                            xact, type, &gtp_message);
                    break;
                default:
                    d_warn("Not implmeneted(type:%d)", type);
                    break;
            }
            pkbuf_free(pkbuf);
            break;
        }
        case EVT_TM_SGW_T3:
        {
            gtp_xact_t *xact = (gtp_xact_t *)event_get_param1(e);
            d_assert(xact, break, "Null param");

            gtp_xact_timeout(xact);
            break;
        }
        default:
        {
            d_error("No handler for event %s", sgw_event_get_name(e));
            break;
        }
    }
}
