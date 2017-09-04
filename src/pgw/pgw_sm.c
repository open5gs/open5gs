#define TRACE_MODULE _pgw_sm

#include "core_debug.h"
#include "core_lib.h"

#include "fd_lib.h"
#include "gx_message.h"

#include "pgw_sm.h"
#include "pgw_context.h"
#include "pgw_event.h"
#include "pgw_gtp_path.h"
#include "pgw_s5c_handler.h"
#include "pgw_gx_handler.h"
#include "pgw_fd_path.h"

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
            rv = pgw_gtp_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish PGW path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = pgw_gtp_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close PGW path");
                break;
            }
            break;
        }
        case PGW_EVT_S5C_MESSAGE:
        {
            status_t rv;
            gtp_node_t *gnode = (gtp_node_t *)event_get_param1(e);
            pkbuf_t *recvbuf = (pkbuf_t *)event_get_param2(e);
            pkbuf_t *copybuf = NULL;
            c_uint16_t copybuf_len = 0;
            gtp_xact_t *xact = NULL;
            gtp_message_t *message = NULL;
            pgw_sess_t *sess = NULL;

            d_assert(recvbuf, break, "Null param");
            d_assert(gnode, pkbuf_free(recvbuf); break, "Null param");

            copybuf_len = sizeof(gtp_message_t);
            copybuf = pkbuf_alloc(0, copybuf_len);
            d_assert(copybuf, break, "Null param");
            message = copybuf->payload;
            d_assert(message, break, "Null param");

            rv = gtp_xact_receive(gnode, recvbuf, &xact, message);
            if (rv != CORE_OK)
                break;

            if (message->h.type == GTP_CREATE_SESSION_REQUEST_TYPE)
                sess = pgw_sess_find_or_add_by_message(message);
            else
                sess = pgw_sess_find_by_teid(message->h.teid);
            d_assert(sess, pkbuf_free(recvbuf); break, "No Session Context");

            switch(message->h.type)
            {
                case GTP_CREATE_SESSION_REQUEST_TYPE:
                    pgw_s5c_handle_create_session_request(
                        xact, sess, &message->create_session_request);
                    pgw_gx_send_ccr(xact, sess, copybuf,
                        GX_CC_REQUEST_TYPE_INITIAL_REQUEST);
                    break;
                case GTP_DELETE_SESSION_REQUEST_TYPE:
                    pgw_s5c_handle_delete_session_request(
                        xact, sess, &message->delete_session_request);
                    pgw_gx_send_ccr(xact, sess, copybuf,
                        GX_CC_REQUEST_TYPE_TERMINATION_REQUEST);
                    break;
                default:
                    pkbuf_free(copybuf);

                    d_warn("Not implmeneted(type:%d)", message->h.type);
                    break;
            }
            pkbuf_free(recvbuf);
            break;
        }
        case PGW_EVT_S5C_T3_RESPONSE:
        case PGW_EVT_S5C_T3_HOLDING:
        {
            gtp_xact_timeout(event_get_param1(e), event_get(e));
            break;
        }
        case PGW_EVT_GX_MESSAGE:
        {
            index_t xact_index = event_get_param1(e);
            gtp_xact_t *xact = NULL;
            index_t sess_index = event_get_param2(e);
            pgw_sess_t *sess = NULL;
            pkbuf_t *gxbuf = (pkbuf_t *)event_get_param3(e);
            gx_message_t *gx_message = NULL;
            pkbuf_t *gtpbuf = (pkbuf_t *)event_get_param4(e);
            gtp_message_t *message = NULL;

            d_assert(xact_index, return, "Null param");
            xact = gtp_xact_find(xact_index);
            d_assert(xact, return, "Null param");

            d_assert(sess_index, return, "Null param");
            sess = pgw_sess_find(sess_index);
            d_assert(sess, return, "Null param");

            d_assert(gxbuf, return, "Null param");
            gx_message = gxbuf->payload;
            d_assert(gx_message, return, "Null param");

            d_assert(gtpbuf, return, "Null param");
            message = gtpbuf->payload;

            switch(gx_message->cmd_code)
            {
                case GX_CMD_CODE_CREDIT_CONTROL:
                {
                    gx_cca_message_t *cca_message = &gx_message->cca_message;
                    if (gx_message->result_code != ER_DIAMETER_SUCCESS)
                    {
                        d_error("Not impleneted(%d)", gx_message->result_code);
                        break;
                    }
                    switch(cca_message->cc_request_type)
                    {
                        case GX_CC_REQUEST_TYPE_INITIAL_REQUEST:
                        {
                            pgw_gx_handle_cca_initial_request(
                                    xact, sess, cca_message,
                                    &message->create_session_request);
                            break;
                        }
                        case GX_CC_REQUEST_TYPE_TERMINATION_REQUEST:
                        {
                            pgw_gx_handle_cca_termination_request(
                                    xact, sess, cca_message,
                                    &message->delete_session_request);
                            break;
                        }
                        default:
                        {
                            d_error("Not implemented(%d)", event_get_param4(e));
                            break;
                        }
                    }
                    gx_cca_message_free(cca_message);
                    break;
                }
                default:
                {
                    d_error("Invalid type(%d)", event_get_param3(e));
                    break;
                }
            }

            pkbuf_free(gxbuf);
            pkbuf_free(gtpbuf);
            break;
        }
        default:
        {
            d_error("No handler for event %s", pgw_event_get_name(e));
            break;
        }
    }
}
