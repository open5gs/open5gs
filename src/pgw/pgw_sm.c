#define TRACE_MODULE _pgw_sm

#include "core_debug.h"

#include "fd_lib.h"
#include "gx_lib.h"

#include "pgw_sm.h"
#include "pgw_context.h"
#include "pgw_event.h"
#include "pgw_gtp_path.h"
#include "pgw_s5c_handler.h"
#include "pgw_gx_handler.h"

void pgw_state_initial(fsm_t *s, event_t *e)
{
    pgw_sm_trace(3, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &pgw_state_operational);
}

void pgw_state_final(fsm_t *s, event_t *e)
{
    pgw_sm_trace(3, e);

    d_assert(s, return, "Null param");
}

void pgw_state_operational(fsm_t *s, event_t *e)
{
    status_t rv;

    pgw_sm_trace(3, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            rv = pgw_path_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish PGW path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = pgw_path_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close PGW path");
                break;
            }
            break;
        }
        case PGW_EVT_S5C_SESSION_MSG:
        {
            status_t rv;
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            gtp_node_t *gnode = (gtp_node_t *)event_get_param2(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            gtp_xact_t *xact = NULL;
            c_uint8_t type;
            c_uint32_t teid;
            gtp_message_t gtp_message;
            pgw_sess_t *sess = NULL;

            d_assert(pkbuf, break, "Null param");
            d_assert(sock, pkbuf_free(pkbuf); break, "Null param");
            d_assert(gnode, pkbuf_free(pkbuf); break, "Null param");

            rv = gtp_xact_receive(
                    &pgw_self()->gtp_xact_ctx, sock, gnode,
                    &xact, &type, &teid, &gtp_message, pkbuf);
            if (rv != CORE_OK)
                break;

            if (type == GTP_CREATE_SESSION_REQUEST_TYPE)
            {
                gtp_create_session_request_t *req = 
                    &gtp_message.create_session_request;
                c_int8_t apn[MAX_APN_LEN];

                if (req->access_point_name.presence == 0)
                {
                    d_error("No APN");

                    pkbuf_free(pkbuf);
                    break;
                }

                memcpy(apn,
                    req->access_point_name.data, req->access_point_name.len);
                apn[req->access_point_name.len] = 0;
                sess = pgw_sess_find_by_apn(apn);
                if (!sess)
                {
                    pgw_bearer_t *bearer = NULL;
                    bearer = pgw_sess_add(apn,
                            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
                    d_assert(bearer, pkbuf_free(pkbuf); break,
                            "No Bearer Context");
                    sess = bearer->sess;
                }
            }
            else
            {
                sess = pgw_sess_find_by_teid(teid);
            }
            d_assert(sess, pkbuf_free(pkbuf); break, "No Session Context");

            /* Store Last GTP Message for Session */
            memcpy(&sess->last_gtp_message,
                    &gtp_message, sizeof(gtp_message_t));

            switch(type)
            {
                case GTP_CREATE_SESSION_REQUEST_TYPE:
                    pgw_handle_create_session_request(
                        xact, sess, &gtp_message.create_session_request);
                    pgw_gx_send_ccr(xact, sess,
                        GX_CC_REQUEST_TYPE_INITIAL_REQUEST);
                    break;
                case GTP_DELETE_SESSION_REQUEST_TYPE:
                    pgw_handle_delete_session_request(
                        xact, sess, &gtp_message.delete_session_request);
                    pgw_gx_send_ccr(xact, sess,
                        GX_CC_REQUEST_TYPE_TERMINATION_REQUEST);
                    break;
                default:
                    d_warn("Not implmeneted(type:%d)", type);
                    break;
            }
            pkbuf_free(pkbuf);
            break;
        }
        case PGW_EVT_TRANSACTION_T3:
        {
            gtp_xact_timeout(event_get_param1(e));
            break;
        }
        case PGW_EVT_S5C_SESSION_FROM_GX:
        {
            index_t xact_index = event_get_param1(e);
            gtp_xact_t *xact = NULL;
            index_t sess_index = event_get_param2(e);
            pgw_sess_t *sess = NULL;

            d_assert(xact_index, return, "Null param");
            xact = gtp_xact_find(xact_index);
            d_assert(xact, return, "Null param");

            d_assert(sess_index, return, "Null param");
            sess = pgw_sess_find(sess_index);
            d_assert(sess, return, "Null param");

            switch(event_get_param3(e))
            {
                case GX_CMD_CREDIT_CONTROL:
                {
                    c_uint32_t result_code = event_get_param5(e);
                    if (result_code != ER_DIAMETER_SUCCESS)
                    {
                        d_error("Not impleneted");
                        return;
                    }
                    switch(event_get_param4(e))
                    {
                        case GX_CC_REQUEST_TYPE_INITIAL_REQUEST:
                        {
                            pgw_handle_create_session_response(xact, sess);
                            break;
                        }
                        case GX_CC_REQUEST_TYPE_TERMINATION_REQUEST:
                        {
                            pgw_handle_delete_session_response(xact, sess);
                            break;
                        }
                        default:
                        {
                            d_error("Not implemented(%d)", event_get_param4(e));
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    d_error("Invalid type(%d)", event_get_param3(e));
                    break;
                }
            }

            break;
        }
        default:
        {
            d_error("No handler for event %s", pgw_event_get_name(e));
            break;
        }
    }
}
