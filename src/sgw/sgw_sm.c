#define TRACE_MODULE _sgw_sm
#include "core_debug.h"

#include "gtp_types.h"

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
    {
        _sgw_sm = 100;
    }
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
                xact = gtp_xact_new_remote(&sgw_self()->gtp_xact_ctx,
                        sock, gnode, pkbuf);
            }

            if (h->teid_presence)
                pkbuf_header(pkbuf, -GTPV2C_HEADER_LEN);
            else
                pkbuf_header(pkbuf, -(GTPV2C_HEADER_LEN-GTPV2C_TEID_LEN));

            rv = gtp_parse_msg(h->type, &gtp_message, pkbuf);
            if (rv != CORE_OK)
            {
                d_error("failed to parse GTPv2-C TLV message(type:%d)",
                        h->type);
                pkbuf_free(pkbuf);
            }

            switch(h->type)
            {
                case GTP_CREATE_SESSION_REQUEST_TYPE:
                {
                    sgw_handle_create_session_request(
                        xact, &gtp_message.create_session_request);
                    break;
                }
            }

            pkbuf_free(pkbuf);
            break;
        }
        case EVT_TM_SGW_T3:
        {
            gtp_xact_t *xact = (gtp_xact_t *)event_get_param1(e);
            d_assert(xact, break, "Nill param");

            gtp_xact_delete(xact);
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
