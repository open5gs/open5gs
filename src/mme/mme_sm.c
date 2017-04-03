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
            rv = mme_s11_listen();
            if (rv != CORE_OK)
            {
                d_error("Can't establish S11 path");
                break;
            }

            rv = s1ap_listen();
            if (rv != CORE_OK)
            {
                d_error("Can't establish S1AP path");
                break;
            }
            break;
        }
        case FSM_EXIT_SIG:
        {
            rv = mme_s11_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close S11 path");
                break;
            }

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
                rc = net_register_sock(sock, _s1ap_recv_cb, NULL);
                d_assert(rc == 0, break, "register _s1ap_recv_cb failed");

                enb_ctx_t *enb = mme_ctx_enb_add();
                d_assert(enb, break, "Null param");
                enb->s1ap_sock = sock;

                fsm_create((fsm_t*)&enb->s1ap_sm, 
                        s1ap_state_initial, s1ap_state_final);
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
        case EVT_MSG_UE_ESM:
        {
            ue_ctx_t *ue = (ue_ctx_t *)event_get_param1(e);
            d_assert(ue, break, "Null param");

            d_assert(FSM_STATE(&ue->esm_sm), break, "Null param");
            fsm_dispatch((fsm_t*)&ue->esm_sm, (fsm_event_t*)e);

            break;
        }
        case EVT_MSG_MME_S11:
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
                xact = gtp_xact_remote_create(&mme_self()->gtp_xact_ctx, 
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
                            d_info("received response");
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
                        case GTP_CREATE_SESSION_RESPONSE_TYPE:
                        {
                            d_info("received response");
                            break;
                        }
                    }
                }
            }

            pkbuf_free(pkbuf);
            break;
        }
        case EVT_TM_MME_S11_T3:
        {
            gtp_xact_t *xact = (gtp_xact_t *)event_get_param1(e);
            d_assert(xact, break, "Null param");

            gtp_xact_timeout(xact);
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
