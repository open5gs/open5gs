#define TRACE_MODULE _mme_sm
#include "core_debug.h"

#include "sm.h"
#include "context.h"
#include "event.h"
#include "s1ap_path.h"

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
            rv = s1ap_open(s->queue_id);
            if (rv != CORE_OK)
            {
                d_error("Can't establish S1AP path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = s1ap_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close S1AP path");
                break;
            }
            break;
        }
        case EVT_LO_ENB_S1AP_ACCEPT:
        {
            int rc;

            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            d_assert(sock, break, "Null param");

            d_trace(1, "eNB-S1 accepted[%s] in master_sm module\n", 
                INET_NTOP(&sock->remote.sin_addr.s_addr, buf));
                    
            enb_ctx_t *enb = mme_ctx_enb_find_by_sock(sock);
            if (!enb)
            {
                rc = net_register_sock(sock, _s1ap_recv_cb, (void*)s->queue_id);
                d_assert(rc == 0, break, "register _s1ap_recv_cb failed");

                enb_ctx_t *enb = mme_ctx_enb_add();
                d_assert(enb, break, "Null param");
                enb->s1ap_sock = sock;

                fsm_create((fsm_t*)&enb->s1ap_sm, 
                        enb_s1ap_state_initial, enb_s1ap_state_final);
                enb->s1ap_sm.ctx = enb;
                enb->s1ap_sm.queue_id = s->queue_id;
                enb->s1ap_sm.tm_service = s->tm_service;
                fsm_init((fsm_t*)&enb->s1ap_sm, 0);
            }
            else
            {
                d_warn("eNB context duplicated with IP-address [%s]!!!", 
                        INET_NTOP(&sock->remote.sin_addr.s_addr, buf));
                net_close(sock);
                d_warn("S1 Socket Closed");
            }
            
            break;
        }
        case EVT_ENB_S1AP_INF:
        {
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            d_assert(sock, break, "Null param");

            enb_ctx_t *enb = mme_ctx_enb_find_by_sock(sock);
            if (enb)
            {
                d_assert(FSM_STATE(&enb->s1ap_sm), break, "Null param");
                fsm_dispatch((fsm_t*)&enb->s1ap_sm, (fsm_event_t*)e);
            }
            else
            {
                d_error("eNB context is not created[%s]",
                        INET_NTOP(&sock->remote.sin_addr.s_addr, buf));
            }
            break;
        }
        case EVT_LO_ENB_S1AP_CONNREFUSED:
        {
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            d_assert(sock, break, "Null param");

            d_info("Socket[%s] connection refused", 
                    INET_NTOP(&sock->remote.sin_addr.s_addr, buf));

            enb_ctx_t *enb = mme_ctx_enb_find_by_sock(sock);
            if (enb) 
            {
                /* Remove eNB S1 state machine if exist */
                d_assert(FSM_STATE(&enb->s1ap_sm), break, "Null param");

                fsm_final((fsm_t*)&enb->s1ap_sm, 0);
                fsm_clear((fsm_t*)&enb->s1ap_sm);

                net_unregister_sock(sock);
                net_close(sock);

                mme_ctx_enb_remove(enb);
                d_info("eNB-S1[%x] connection refused!!!", enb->enb_id);
            }
            else
            {
                d_error("Can't find eNB-S1 for [%s]!!!", 
                        INET_NTOP(&sock->remote.sin_addr.s_addr, buf));
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
        pkbuf_free(event_get_msg(e));
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
