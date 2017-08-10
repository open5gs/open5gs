#define TRACE_MODULE _mme_sm
#include "core_debug.h"

#include "s1ap_message.h"
#include "nas_message.h"

#include "mme_event.h"

#include "s1ap_handler.h"
#include "s1ap_path.h"
#include "mme_s6a_handler.h"
#include "nas_security.h"
#include "emm_handler.h"
#include "mme_s11_path.h"
#include "mme_s11_handler.h"
#include "emm_handler.h"

void mme_state_initial(fsm_t *s, event_t *e)
{
    mme_sm_trace(3, e);

    d_assert(s, return, "Null param");

    FSM_TRAN(s, &mme_state_operational);
}

void mme_state_final(fsm_t *s, event_t *e)
{
    mme_sm_trace(3, e);

    d_assert(s, return, "Null param");
}

int test = 0;

void mme_state_operational(fsm_t *s, event_t *e)
{
    status_t rv;
    char buf[INET_ADDRSTRLEN];

    mme_sm_trace(3, e);

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
        case MME_EVT_S1AP_ENB_LO_ACCEPT:
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
        case MME_EVT_S1AP_ENB_LO_CONNREFUSED:
        {
            index_t index = event_get_param1(e);
            mme_enb_t *enb = NULL;

            d_assert(index, break, "Null param");
            enb = mme_enb_find(index);
            if (enb)
            {
                d_trace(1, "eNB-S1[%x] connection refused!!!\n", 
                        enb->enb_id);
                mme_enb_remove(enb);
            }
            else
            {
                d_warn("Socket connection refused, Already Removed!");
            }

            break;
        }
        case MME_EVT_S1AP_ENB_MSG:
        {
            s1ap_message_t message;
            index_t index = event_get_param1(e);
            mme_enb_t *enb = NULL;
            pkbuf_t *pkbuf = NULL;
            
            d_assert(index, break, "Null param");
            d_assert(enb = mme_enb_find(index), break, "No eNB context");
            d_assert(FSM_STATE(&enb->sm), break, "No S1AP State Machine");

            pkbuf = (pkbuf_t *)event_get_param2(e);
            d_assert(pkbuf, break, "Null param");
            d_assert(s1ap_decode_pdu(&message, pkbuf) == CORE_OK,
                    pkbuf_free(pkbuf); break, "Can't decode S1AP_PDU");
            event_set_param3(e, (c_uintptr_t)&message);

            fsm_dispatch(&enb->sm, (fsm_event_t*)e);

            s1ap_free_pdu(&message);
            pkbuf_free(pkbuf);
            break;
        }
        case MME_EVT_EMM_UE_MSG:
        {
            nas_message_t message;
            index_t index = event_get_param1(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            enb_ue_t *enb_ue = NULL;
            mme_ue_t *mme_ue = NULL;

            enb_ue = enb_ue_find(index);
            d_assert(enb_ue, break, "No ENB UE context");

            d_assert(pkbuf, break, "Null param");
            d_assert(nas_emm_decode(&message, pkbuf) == CORE_OK,
                    pkbuf_free(pkbuf); break, "Can't decode NAS_EMM");

            mme_ue = enb_ue->mme_ue;
            if (!mme_ue)
            {
                mme_ue = mme_ue_find_by_message(&message);
                if (!mme_ue)
                {
                    mme_ue = mme_ue_add(enb_ue);
                    d_assert(mme_ue, break, "Null param");
                }
                else
                {
                    /* Here, if the MME_UE Context is found,
                     * the integrity check is not performed
                     * For example, ATTACH_REQUEST, 
                     * TRACKING_AREA_UPDATE_REQUEST message
                     *
                     * Now, We will check the MAC in the NAS message*/
                    nas_security_header_type_t h;
                    h.type = (c_uint8_t)event_get_param2(e);
                    if (h.integrity_protected)
                    {
                        /* Decryption was performed in S1AP handler.
                         * So, we disabled 'ciphered' 
                         * not to decrypt NAS message */
                        h.ciphered = 0;
                        d_assert(
                            nas_security_decode(mme_ue, h, pkbuf) == CORE_OK,
                            pkbuf_free(pkbuf);return, 
                            "nas_security_decode failed");
                    }
                }
                /* Set mme_ue */
                mme_associate_ue_context(mme_ue, enb_ue);
            }

            d_assert(mme_ue, pkbuf_free(pkbuf);break, "No MME UE context");
            d_assert(FSM_STATE(&mme_ue->sm), pkbuf_free(pkbuf);break, 
                    "No EMM State Machine");

            /* Set event */
            event_set_param1(e, (c_uintptr_t)mme_ue->index);/* mme_ue index */
            event_set_param4(e, (c_uintptr_t)&message);

            fsm_dispatch(&mme_ue->sm, (fsm_event_t*)e);

            pkbuf_free(pkbuf);

            break;
        }
        case MME_EVT_EMM_UE_FROM_S6A:
        case MME_EVT_EMM_UE_T3413:
        {
            index_t index = event_get_param1(e);
            mme_ue_t *mme_ue = NULL;

            d_assert(index, break, "Null param");
            mme_ue = mme_ue_find(index);
            d_assert(mme_ue, break, "No UE context");
            d_assert(FSM_STATE(&mme_ue->sm), break, "No EMM State Machine");

            fsm_dispatch(&mme_ue->sm, (fsm_event_t*)e);

            break;
        }
        case MME_EVT_ESM_BEARER_MSG:
        {
            nas_message_t message;
            index_t index = event_get_param1(e);
            mme_bearer_t *bearer = NULL;
            mme_ue_t *mme_ue = NULL;
            pkbuf_t *pkbuf = NULL;

            d_assert(index, break, "Null param");
            bearer = mme_bearer_find(index);
            d_assert(bearer, break, "No ESM context");
            d_assert(mme_ue = bearer->mme_ue, break, "No UE context");
            d_assert(FSM_STATE(&bearer->sm), break, "No ESM State Machine");

            if (event_get(e) == MME_EVT_ESM_BEARER_MSG)
            {
                pkbuf = (pkbuf_t *)event_get_param3(e);
                d_assert(pkbuf, break, "Null param");
                d_assert(nas_esm_decode(&message, pkbuf) == CORE_OK,
                        pkbuf_free(pkbuf); break, "Can't decode NAS_ESM");

                event_set_param4(e, (c_uintptr_t)&message);
            }

            fsm_dispatch(&bearer->sm, (fsm_event_t*)e);

            if (event_get(e) == MME_EVT_ESM_BEARER_MSG)
            {
                pkbuf_free(pkbuf);
            }

            break;
        }
        case MME_EVT_S11_UE_MSG:
        {
            status_t rv;
            net_sock_t *sock = (net_sock_t *)event_get_param1(e);
            gtp_node_t *gnode = (gtp_node_t *)event_get_param2(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param3(e);
            gtp_xact_t *xact = NULL;
            c_uint8_t type;
            c_uint32_t teid;
            gtp_message_t gtp_message;
            mme_sess_t *sess = NULL;

            d_assert(pkbuf, break, "Null param");
            d_assert(sock, pkbuf_free(pkbuf); break, "Null param");
            d_assert(gnode, pkbuf_free(pkbuf); break, "Null param");

            rv = gtp_xact_receive(
                    &mme_self()->gtp_xact_ctx, sock, gnode,
                    &xact, &type, &teid, &gtp_message, pkbuf);
            if (rv != CORE_OK)
                break;

            sess = mme_sess_find_by_teid(teid);
            d_assert(sess, pkbuf_free(pkbuf); break, 
                    "No Session Context(TEID:%d)", teid);
            switch(type)
            {
                case GTP_CREATE_SESSION_RESPONSE_TYPE:
                {
                    mme_s11_handle_create_session_response(
                            sess, &gtp_message.create_session_response);
                    emm_handle_attach_accept(sess);
                    break;
                }
                case GTP_MODIFY_BEARER_RESPONSE_TYPE:
                    mme_s11_handle_modify_bearer_response(
                            sess, &gtp_message.modify_bearer_response);
                    break;
                case GTP_DELETE_SESSION_RESPONSE_TYPE:
                {
                    mme_ue_t *mme_ue = NULL;
                    nas_message_t *message = NULL;

                    d_assert(sess, break, "Null param");
                    mme_ue = sess->mme_ue;
                    d_assert(mme_ue, break, "Null param");
                    message = &mme_ue->last_emm_message;
                    d_assert(message, break, "Null param");

                    mme_s11_handle_delete_session_response(
                            sess, &gtp_message.delete_session_response);

                    switch(message->emm.h.message_type)
                    {
                        case NAS_DETACH_REQUEST:
                        {
                            emm_handle_detach_accept(mme_ue,
                                    &message->emm.detach_request_from_ue);
                            break;
                        }
                        case NAS_ATTACH_REQUEST:
                        case NAS_IDENTITY_RESPONSE:
                        {
                            mme_s6a_send_air(mme_ue);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
                {
                    mme_ue_t *mme_ue = NULL;
                    enb_ue_t *enb_ue = NULL;

                    mme_s11_handle_release_access_bearers_response(
                            sess, &gtp_message.release_access_bearers_response);

                    mme_ue = sess->mme_ue;
                    d_assert(mme_ue, break, "Null param");
                    enb_ue = mme_ue->enb_ue;
                    d_assert(enb_ue, break, "Null param");
                    
                    s1ap_handle_release_access_bearers_response(enb_ue);
                    break;
                }

                case GTP_DOWNLINK_DATA_NOTIFICATION_TYPE:
                {
                    mme_ue_t *mme_ue = NULL;

                    mme_s11_handle_downlink_data_notification(
                            xact,
                            sess, &gtp_message.downlink_data_notification);

                    mme_ue = sess->mme_ue;
                    d_assert(mme_ue, break, "Null param");

                    s1ap_handle_paging(mme_ue);
                    /* Start T3413 */
                    tm_start(mme_ue->t3413);
                    break;
                }
                default:
                    d_warn("Not implmeneted(type:%d)", type);
                    break;
            }
            pkbuf_free(pkbuf);
            break;
        }
        case MME_EVT_S11_TRANSACTION_T3:
        {
            gtp_xact_timeout(event_get_param1(e));
            break;
        }
        default:
        {
            d_error("No handler for event %s", mme_event_get_name(e));
            break;
        }
    }
}
