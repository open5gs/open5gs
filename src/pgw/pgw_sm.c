#define TRACE_MODULE _pgw_sm
#include "core_debug.h"

#include "sm.h"
#include "context.h"
#include "event.h"
#include "pgw_path.h"
#include "s5c_handler.h"

void pgw_state_initial(pgw_sm_t *s, event_t *e)
{
    pgw_sm_trace(1, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &pgw_state_operational);
}

void pgw_state_final(pgw_sm_t *s, event_t *e)
{
    pgw_sm_trace(1, e);

    d_assert(s, return, "Null param");
}

void pgw_state_operational(pgw_sm_t *s, event_t *e)
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
                xact = gtp_xact_remote_create(&pgw_self()->gtp_xact_ctx, 
                        sock, gnode, h->sqn);
            }

            if (xact->org == GTP_LOCAL_ORIGINATOR)
            {
                /* TODO */
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
                            pgw_s5c_handle_create_session_request(
                                xact, &gtp_message.create_session_request);
                            break;
                        }
                    }
                }
            }

            pkbuf_free(pkbuf);
            break;
        }
        case EVT_TM_PGW_T3:
        {
            gtp_xact_t *xact = (gtp_xact_t *)event_get_param1(e);
            d_assert(xact, break, "Null param");
            
            gtp_xact_timeout(xact);
            break;
        }
        default:
        {
            d_error("No handler for event %s", pgw_event_get_name(e));
            break;
        }
    }
}
