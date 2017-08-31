#define TRACE_MODULE _sgw_sm
#include "core_debug.h"

#include "sgw_context.h"
#include "sgw_event.h"
#include "sgw_gtp_path.h"
#include "sgw_handler.h"

void sgw_state_initial(fsm_t *s, event_t *e)
{
    sgw_sm_trace(3, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &sgw_state_operational);
}

void sgw_state_final(fsm_t *s, event_t *e)
{
    sgw_sm_trace(3, e);

    d_assert(s, return, "Null param");
}

void sgw_state_operational(fsm_t *s, event_t *e)
{
    status_t rv;

    sgw_sm_trace(3, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            rv = sgw_gtp_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish SGW path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = sgw_gtp_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close SGW path");
                break;
            }
            break;
        }
        case SGW_EVT_S11_MESSAGE:
        case SGW_EVT_S5C_MESSAGE:
        {
            status_t rv;
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            gtp_node_t *gnode = (gtp_node_t *)event_get_param2(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            gtp_xact_t *xact = NULL;
            gtp_message_t message;
            sgw_sess_t *sess = NULL;

            d_assert(pkbuf, break, "Null param");
            d_assert(sock, pkbuf_free(pkbuf); break, "Null param");
            d_assert(gnode, pkbuf_free(pkbuf); break, "Null param");

            rv = gtp_xact_receive(
                    &sgw_self()->gtp_xact_ctx, sock, gnode, pkbuf,
                    &xact, &message);
            if (rv != CORE_OK)
                break;

            if (message.h.type == GTP_CREATE_SESSION_REQUEST_TYPE)
                sess = sgw_sess_find_or_add_by_message(&message);
            else
                sess = sgw_sess_find_by_teid(message.h.teid);
            d_assert(sess, pkbuf_free(pkbuf); break, "No Session Context");

            switch(message.h.type)
            {
                case GTP_CREATE_SESSION_REQUEST_TYPE:
                    sgw_handle_create_session_request(
                            xact, sess, &message);
                    break;
                case GTP_CREATE_SESSION_RESPONSE_TYPE:
                    sgw_handle_create_session_response(
                            xact, sess, &message);
                    break;
                case GTP_MODIFY_BEARER_REQUEST_TYPE:
                    sgw_handle_modify_bearer_request(
                            xact, sess, &message.modify_bearer_request);
                    break;
                case GTP_DELETE_SESSION_REQUEST_TYPE:
                    sgw_handle_delete_session_request(
                            xact, sess, &message);
                    break;
                case GTP_DELETE_SESSION_RESPONSE_TYPE:
                    sgw_handle_delete_session_response(
                            xact, sess, &message);
                    break;
                case GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE:
                    sgw_handle_release_access_bearers_request(
                            xact, sess, 
                            &message.release_access_bearers_request);
                    break;
                case GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
                    sgw_handle_downlink_data_notification_ack(
                          sess,
                          &message.downlink_data_notification_acknowledge);
                    break;
                default:
                    d_warn("Not implmeneted(type:%d)", message.h.type);
                    break;
            }
            pkbuf_free(pkbuf);
            break;
        }
        case SGW_EVT_GTP_T3:
        {
            gtp_xact_timeout(event_get_param1(e));
            break;
        }
        case SGW_EVT_LO_DLDATA_NOTI:
        {
            index_t index = (index_t)event_get_param1(e);
            sgw_bearer_t* bearer = sgw_bearer_find(index);

            if (!bearer)
            {
                d_error("Can not find bearer with index(%d)",index);
                break;
            }

            sgw_handle_lo_dldata_notification(bearer);

            break;
        }
        default:
        {
            d_error("No handler for event %s", sgw_event_get_name(e));
            break;
        }
    }
}
