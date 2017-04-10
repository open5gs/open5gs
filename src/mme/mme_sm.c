#define TRACE_MODULE _mme_sm
#include "core_debug.h"

#include "s1ap_message.h"
#include "nas_message.h"

#include "mme_event.h"

#include "s1ap_path.h"
#include "nas_security.h"
#include "mme_s11_path.h"

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
        case EVT_LO_MME_S1AP_ACCEPT:
        {
            int rc;

            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            d_assert(sock, break, "Null param");

            d_trace(1, "eNB-S1 accepted[%s] in master_sm module\n", 
                INET_NTOP(&sock->remote.sin_addr.s_addr, buf));
                    
            mme_enb_t *enb = mme_enb_find_by_sock(sock);
            if (!enb)
            {
                rc = net_register_sock(sock, _s1ap_recv_cb, NULL);
                d_assert(rc == 0, break, "register _s1ap_recv_cb failed");

                mme_enb_t *enb = mme_enb_add(sock);
                d_assert(enb, break, "Null param");
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
        case EVT_LO_MME_S1AP_CONNREFUSED:
        {
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            d_assert(sock, break, "Null param");

            d_info("Socket[%s] connection refused", 
                    INET_NTOP(&sock->remote.sin_addr.s_addr, buf));

            mme_enb_t *enb = mme_enb_find_by_sock(sock);
            if (enb) 
            {
                mme_enb_remove(enb);
                d_info("eNB-S1[%x] connection refused!!!", enb->enb_id);
            }
            else
            {
                d_warn("Can't find eNB-S1 for [%s]!!!", 
                        INET_NTOP(&sock->remote.sin_addr.s_addr, buf));
            }

            break;
        }
        case EVT_MSG_MME_S1AP:
        {
            s1ap_message_t message;
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            mme_enb_t *enb = NULL;
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param2(e);

            d_assert(pkbuf, break, "Null param");
            d_assert(sock, pkbuf_free(pkbuf); break, "Null param");
            d_assert(enb = mme_enb_find_by_sock(sock), 
                    pkbuf_free(pkbuf); break, "No eNB context");
            d_assert(FSM_STATE(&enb->s1ap_sm), 
                    pkbuf_free(pkbuf); break, "No S1AP State Machine");

            d_assert(s1ap_decode_pdu(&message, pkbuf) == CORE_OK,
                    pkbuf_free(pkbuf); break, "Can't decode S1AP_PDU");

            event_set_param3(e, (c_uintptr_t)&message);
            fsm_dispatch((fsm_t*)&enb->s1ap_sm, (fsm_event_t*)e);

            s1ap_free_pdu(&message);
            pkbuf_free(pkbuf);
            break;
        }
        case EVT_MSG_MME_EMM:
        {
            nas_message_t message;
            mme_ue_t *ue = (mme_ue_t *)event_get_param1(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param2(e);

            d_assert(pkbuf, break, "Null param");
            d_assert(ue, pkbuf_free(pkbuf); break, "No UE context");
            d_assert(FSM_STATE(&ue->emm_sm), 
                    pkbuf_free(pkbuf); break, "No EMM State Machine");

            d_assert(nas_security_decode(&message, ue, pkbuf) == CORE_OK,
                    pkbuf_free(pkbuf); break, "Can't decode NAS_EMM");

            event_set_param3(e, (c_uintptr_t)&message);
            fsm_dispatch((fsm_t*)&ue->emm_sm, (fsm_event_t*)e);

            pkbuf_free(pkbuf);
            break;
        }
        case EVT_MSG_MME_ESM:
        {
            nas_message_t message;
            mme_esm_t *esm = (mme_esm_t *)event_get_param1(e);
            mme_ue_t *ue = NULL;
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param2(e);

            d_assert(pkbuf, break, "Null param");
            d_assert(esm, pkbuf_free(pkbuf); break, "No ESM context");
            d_assert(ue = esm->ue, pkbuf_free(pkbuf); break, "No UE context");
            d_assert(FSM_STATE(&esm->sm), 
                    pkbuf_free(pkbuf); break, "No ESM State Machine");

            d_assert(nas_security_decode(&message, ue, pkbuf) == CORE_OK,
                    pkbuf_free(pkbuf); break, "Can't decode NAS_ESM");

            event_set_param3(e, (c_uintptr_t)&message);
            fsm_dispatch((fsm_t*)&esm->sm, (fsm_event_t*)e);

            pkbuf_free(pkbuf);
            break;
        }
        case EVT_MSG_MME_S11:
        {
            status_t rv;
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            gtp_node_t *gnode = (gtp_node_t *)event_get_param2(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            gtp_xact_t *xact = NULL;
            c_uint8_t type;
            c_uint32_t teid;
            gtp_message_t gtp_message;

            d_assert(pkbuf, break, "Null param");
            d_assert(sock, pkbuf_free(pkbuf); break, "Null param");
            d_assert(gnode, pkbuf_free(pkbuf); break, "Null param");

            rv = gtp_xact_receive(
                    &mme_self()->gtp_xact_ctx, sock, gnode,
                    &xact, &type, &teid, &gtp_message, pkbuf);
            if (rv != CORE_OK)
                break;

            switch(type)
            {
                case GTP_CREATE_SESSION_RESPONSE_TYPE:
                    d_info("receive reponse");
                    break;
                default:
                    d_warn("Not implmeneted(type:%d)", type);
                    break;
            }
            pkbuf_free(pkbuf);
            break;
        }
        case EVT_TM_MME_S11_T3:
        {
            index_t index = (index_t)event_get_param1(e);
            d_assert(index, break, "Null param");

            gtp_xact_timeout(index);
            break;
        }
        default:
        {
            d_error("No handler for event %s", mme_event_get_name(e));
            break;
        }
    }
}
