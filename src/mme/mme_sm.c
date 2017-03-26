#define TRACE_MODULE _mme_sm
#include "core_debug.h"

#include "event.h"

#include "s1ap_path.h"
#include "s11_path.h"

void mme_state_initial(mme_sm_t *s, event_t *e)
{
    mme_sm_trace(1, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &mme_state_operational);
}

void mme_state_final(mme_sm_t *s, event_t *e)
{
    mme_sm_trace(1, e);

    d_assert(s, return, "Null param");
}

void mme_state_operational(mme_sm_t *s, event_t *e)
{
    status_t rv;
    char buf[INET_ADDRSTRLEN];

    mme_sm_trace(1, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            /* FIXME: for test */
#if 0
            {
                event_t e;
                sgw_ctx_t *sgw = mme_ctx_sgw_add();

                d_assert(sgw, break, "Can't add SGW context");

                sgw->gnode.local_addr = inet_addr("127.0.0.1");
                sgw->gnode.local_port = GTPV2_C_UDP_PORT;
                sgw->gnode.remote_addr = inet_addr("127.0.0.1");
                sgw->gnode.remote_port = GTPV2_C_UDP_PORT+1;

                event_set(&e, EVT_LO_MME_ENGAGE_SGW);
                event_set_param1(&e, sgw);
                mme_event_send(&e);
            }
#endif

            rv = s1ap_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish S1AP path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            sgw_ctx_t *sgw = mme_ctx_sgw_first();
            while(sgw)
            {
                mme_s11_close(sgw);
                sgw = mme_ctx_sgw_next(sgw);
            }

            rv = s1ap_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close S1AP path");
                break;
            }

            break;
        }
        case EVT_LO_MME_ENGAGE_SGW:
        {
            sgw_ctx_t *sgw = (sgw_ctx_t *)event_get_param1(e);
            d_assert(sgw, break, "LO_MME_ENGAGE_SGW has no BS context");

            mme_s11_open(sgw);
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
                rc = net_register_sock(sock, _s1ap_recv_cb, NULL);
                d_assert(rc == 0, break, "register _s1ap_recv_cb failed");

                enb_ctx_t *enb = mme_ctx_enb_add();
                d_assert(enb, break, "Null param");
                enb->s1ap_sock = sock;

                fsm_create((fsm_t*)&enb->s1ap_sm, 
                        enb_s1ap_state_initial, enb_s1ap_state_final);
                enb->s1ap_sm.ctx = enb;
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
        case EVT_MSG_ENB_S1AP:
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
        case EVT_MSG_UE_EMM:
        {
            ue_ctx_t *ue = (ue_ctx_t *)event_get_param1(e);
            d_assert(ue, break, "Null param");

            d_assert(FSM_STATE(&ue->emm_sm), break, "Null param");
            fsm_dispatch((fsm_t*)&ue->emm_sm, (fsm_event_t*)e);

            break;
        }
        case EVT_MSG_MME_S11:
        {
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            d_assert(sock, break, "Null param");

            d_info("EVT_MSG_MME_S11 received");
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
                d_warn("Can't find eNB-S1 for [%s]!!!", 
                        INET_NTOP(&sock->remote.sin_addr.s_addr, buf));
            }

            break;
        }
        default:
        {
            d_error("No handler for event %s", mme_event_get_name(e));
            break;
        }
    }
}
