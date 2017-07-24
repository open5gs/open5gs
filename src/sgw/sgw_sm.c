#define TRACE_MODULE _sgw_sm
#include "core_debug.h"

#include "sgw_context.h"
#include "sgw_event.h"
#include "sgw_path.h"
#include "sgw_handler.h"

void sgw_state_initial(fsm_t *s, event_t *e)
{
    sgw_sm_trace(1, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &sgw_state_operational);
}

void sgw_state_final(fsm_t *s, event_t *e)
{
    sgw_sm_trace(1, e);

    d_assert(s, return, "Null param");
}

void sgw_state_operational(fsm_t *s, event_t *e)
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
                d_error("Can't establish SGW path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = sgw_path_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close SGW path");
                break;
            }
            break;
        }
        case SGW_EVT_S11_SESSION_MSG:
        case SGW_EVT_S5C_SESSION_MSG:
        {
            status_t rv;
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            gtp_node_t *gnode = (gtp_node_t *)event_get_param2(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            gtp_xact_t *xact = NULL;
            c_uint8_t type;
            c_uint32_t teid;
            gtp_message_t gtp_message;
            sgw_sess_t *sess = NULL;

            d_assert(pkbuf, break, "Null param");
            d_assert(sock, pkbuf_free(pkbuf); break, "Null param");
            d_assert(gnode, pkbuf_free(pkbuf); break, "Null param");

            rv = gtp_xact_receive(
                    &sgw_self()->gtp_xact_ctx, sock, gnode,
                    &xact, &type, &teid, &gtp_message, pkbuf);
            if (rv != CORE_OK)
                break;

            if (type == GTP_CREATE_SESSION_REQUEST_TYPE)
            {
                sgw_handle_create_session_request(xact, type, &gtp_message);
                pkbuf_free(pkbuf);

                break;
            }

            sess = sgw_sess_find_by_teid(teid);
            d_assert(sess, pkbuf_free(pkbuf); break, 
                    "No Session Context(TEID:%d)", teid);
            switch(type)
            {
                case GTP_CREATE_SESSION_RESPONSE_TYPE:
                    sgw_handle_create_session_response(
                            xact, sess, &gtp_message);
                    break;
                case GTP_MODIFY_BEARER_REQUEST_TYPE:
                    sgw_handle_modify_bearer_request(
                            xact, sess, &gtp_message.modify_bearer_request);
                    break;
                case GTP_DELETE_SESSION_REQUEST_TYPE:
                    sgw_handle_delete_session_request(
                            xact, sess, &gtp_message);
                    break;
                case GTP_DELETE_SESSION_RESPONSE_TYPE:
#if 0
                    sgw_handle_delete_session_response(
                            xact, sess, &gtp_message);
#endif
                    break;
                default:
                    d_warn("Not implmeneted(type:%d)", type);
                    break;
            }
            pkbuf_free(pkbuf);
            break;
        }
        case SGW_EVT_TRANSACTION_T3:
        {
            gtp_xact_timeout(event_get_param1(e));
            break;
        }
        default:
        {
            d_error("No handler for event %s", sgw_event_get_name(e));
            break;
        }
    }
}
