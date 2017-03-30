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
            gtp_message_t gtp_message;

            d_assert(sock, break, "Null param");
            d_assert(gnode, break, "Null param");
            d_assert(pkbuf, break, "Null param");

            xact = gtp_xact_recv(&pgw_self()->gtp_xact_ctx, sock, gnode, pkbuf);
            d_assert(xact, break, "Null param");

            rv = gtp_parse_msg(xact->type, &gtp_message, pkbuf);
            d_assert(rv == CORE_OK, break, "parse error");

            switch(xact->type)
            {
                case GTP_CREATE_SESSION_REQUEST_TYPE:
                {
                    pgw_s5c_handle_create_session_request(
                        xact, &gtp_message.create_session_request);
                    break;
                }
            }

            break;
        }
        case EVT_TM_PGW_T3:
        {
            gtp_xact_t *xact = (gtp_xact_t *)event_get_param1(e);
            d_assert(xact, break, "Nill param");
            
            gtp_xact_commit(xact);
            break;
        }
        default:
        {
            d_error("No handler for event %s", pgw_event_get_name(e));
            break;
        }
    }
}
