#define TRACE_MODULE _mme_sm
#include "core_debug.h"

#include "sm.h"
#include "context.h"
#include "event.h"
#include "s1_path.h"

void mme_state_initial(mme_sm_t *s, event_t *e)
{
    sm_trace(1, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &mme_state_operational);
}

void mme_state_final(mme_sm_t *s, event_t *e)
{
    sm_trace(1, e);

    d_assert(s, return, "Null param");
}

void mme_state_operational(mme_sm_t *s, event_t *e)
{
    status_t rv;
    char buf[INET_ADDRSTRLEN];

    sm_trace(1, e);

    d_assert(s, return, "Null param");
    d_assert(s->queue_id, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            rv = s1_open(s->queue_id);
            if (rv != CORE_OK)
            {
                d_error("Can't establish S1 path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = s1_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close S1 path");
                break;
            }
            break;
        }
        case EVT_LO_ENB_S1_CONNREFUSED:
        {
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            d_assert(sock, break, "Null param");

            c_uint32_t ip_addr = (c_uint32_t)event_get_param2(e);
            d_info("Socket[%s] connection refused", INET_NTOP(&ip_addr, buf));

            net_unregister_sock(sock);
            net_close(sock);

            enb_ctx_t *enb = enb_ctx_find_by_ip(ip_addr);
            if (enb) 
            {
                /* Remove eNB S1 state machine if exist */
                if (FSM_STATE(&enb->s1_sm))
                {
                    fsm_final((fsm_t*)&enb->s1_sm, 0);
                    fsm_clear((fsm_t*)&enb->s1_sm);
                }

                enb_ctx_remove(enb);
                d_info("eNB-S1[%x] connection refused!!!", enb->id);
            }
            else
            {
                d_error("Can't find eNB-S1 for [%s]!!!", 
                        INET_NTOP(&ip_addr, buf));
            }
            break;
        }
        default:
        {
            d_error("No handler for event %s", event_get_name(e));
            break;
        }
    }

    /* If event was packet type, its buffer allocated by data-plane should
     * be freed here */
    if (event_is_msg(e))
        pkbuf_free(event_get_msg_pkb(e));
}

void mme_state_exception(mme_sm_t *s, event_t *e)
{
    sm_trace(1, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        default:
        {
            d_error("No handler for event %s", event_get_name(e));
            break;
        }
    }
}
