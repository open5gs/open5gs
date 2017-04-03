#define TRACE_MODULE _sgw_sm
#include "core_debug.h"

#include "sm.h"
#include "context.h"
#include "event.h"
#include "sgw_path.h"
#include "s11_handler.h"

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
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            gtp_node_t *gnode = (gtp_node_t *)event_get_param2(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            gtp_xact_t *xact = NULL;
            gtpv2c_header_t *h = NULL;
            gtp_message_t gtp_message;

            d_assert(sock, break, "Null param");
            d_assert(gnode, break, "Null param");
            d_assert(pkbuf, break, "Null param");

            h = pkbuf->payload;
            d_assert(h, break, "Null param");

            xact = gtp_xact_find(gnode, pkbuf);
            if (!xact)
            {
                xact = gtp_xact_remote_create(&sgw_self()->gtp_xact_ctx, 
                        sock, gnode, h->sqn);
            }

            if (xact->org == GTP_LOCAL_ORIGINATOR)
            {
                if (xact->assoc_xact)
                {
                    if (h->teid_presence)
                        pkbuf_header(pkbuf, -GTPV2C_HEADER_LEN);
                    else
                        pkbuf_header(pkbuf, -(GTPV2C_HEADER_LEN-GTPV2C_TEID_LEN));

                    memset(&gtp_message, 0, sizeof(gtp_message_t));
                    gtp_message.type = h->type;
                    d_assert(gtp_parse_msg(&gtp_message, pkbuf) == CORE_OK,
                            break, "parse error");

                    switch(gtp_message.type)
                    {
                        case GTP_CREATE_SESSION_RESPONSE_TYPE:
                        {
                            sgw_s11_handle_create_session_response(
                                    xact->assoc_xact, &gtp_message);
                            break;
                        }
                    }
                }

                gtp_xact_delete(xact);
            }
            else
            {
                if (xact->pkbuf)
                {
                    d_assert(gtp_send(xact->sock, xact->gnode, xact->pkbuf) 
                            == CORE_OK, break, "gtp_send error");
                }
                else
                {
                    if (h->teid_presence)
                        pkbuf_header(pkbuf, -GTPV2C_HEADER_LEN);
                    else
                        pkbuf_header(pkbuf, -(GTPV2C_HEADER_LEN-GTPV2C_TEID_LEN));

                    memset(&gtp_message, 0, sizeof(gtp_message_t));
                    gtp_message.type = h->type;
                    d_assert(gtp_parse_msg(&gtp_message, pkbuf) == CORE_OK,
                            break, "parse error");

                    switch(gtp_message.type)
                    {
                        case GTP_CREATE_SESSION_REQUEST_TYPE:
                        {
                            sgw_s11_handle_create_session_request(xact, &gtp_message);
                            break;
                        }
                    }
                }
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
