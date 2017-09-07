#define TRACE_MODULE _mme_sm
#include "core_debug.h"

#include "s1ap_message.h"
#include "nas_message.h"

#include "mme_event.h"

#include "s1ap_build.h"
#include "s1ap_handler.h"
#include "s1ap_path.h"
#include "mme_fd_path.h"
#include "mme_gtp_path.h"
#include "mme_s11_handler.h"
#include "nas_security.h"
#include "nas_path.h"
#include "emm_handler.h"
#include "esm_build.h"
#include "esm_handler.h"
#include "fd_lib.h"
#include "mme_s6a_handler.h"

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
            rv = mme_gtp_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish S11 path");
                break;
            }

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
            rv = mme_gtp_close();
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
        case MME_EVT_S1AP_LO_ACCEPT:
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
        case MME_EVT_S1AP_LO_CONNREFUSED:
        {
            mme_enb_t *enb = mme_enb_find(event_get_param1(e));
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
        case MME_EVT_S1AP_MESSAGE:
        {
            s1ap_message_t message;
            mme_enb_t *enb = NULL;
            pkbuf_t *pkbuf = NULL;
            
            enb = mme_enb_find(event_get_param1(e));
            d_assert(enb, break, "No eNB context");
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
        case MME_EVT_EMM_MESSAGE:
        {
            nas_message_t message;
            pkbuf_t *pkbuf = NULL;
            enb_ue_t *enb_ue = NULL;
            mme_ue_t *mme_ue = NULL;

            enb_ue = enb_ue_find(event_get_param1(e));
            d_assert(enb_ue, break, "No ENB UE context");
            pkbuf = (pkbuf_t *)event_get_param3(e);
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
            if (FSM_CHECK(&mme_ue->sm, emm_state_exception))
            {
                mme_ue_remove(mme_ue);
            }

            pkbuf_free(pkbuf);

            break;
        }
        case MME_EVT_EMM_T3413:
        {
            mme_ue_t *mme_ue = mme_ue_find(event_get_param1(e));
            d_assert(mme_ue, break, "No UE context");
            d_assert(FSM_STATE(&mme_ue->sm), break, "No EMM State Machine");

            fsm_dispatch(&mme_ue->sm, (fsm_event_t*)e);

            break;
        }
        case MME_EVT_ESM_MESSAGE:
        {
            nas_message_t message;
            mme_bearer_t *bearer = NULL;
            pkbuf_t *pkbuf = NULL;

            bearer = mme_bearer_find(event_get_param1(e));
            d_assert(bearer, break, "No Bearer context");

            pkbuf = (pkbuf_t *)event_get_param2(e);
            d_assert(pkbuf, break, "Null param");
            d_assert(nas_esm_decode(&message, pkbuf) == CORE_OK,
                    pkbuf_free(pkbuf); break, "Can't decode NAS_ESM");

            event_set_param3(e, (c_uintptr_t)&message);

            fsm_dispatch(&bearer->sm, (fsm_event_t*)e);
            if (FSM_CHECK(&bearer->sm, esm_state_session_exception))
            {
                mme_sess_t *sess = bearer->sess;
                d_assert(sess, break, "Null param");
                mme_sess_remove(sess);
            }
            else if (FSM_CHECK(&bearer->sm, esm_state_bearer_exception))
            {
                mme_bearer_remove(bearer);
            }

            pkbuf_free(pkbuf);

            break;
        }
        case MME_EVT_S6A_MESSAGE:
        {
            status_t rv;
            mme_ue_t *mme_ue = mme_ue_find(event_get_param1(e));
            pkbuf_t *s6abuf = (pkbuf_t *)event_get_param2(e);
            s6a_message_t *s6a_message = NULL;

            d_assert(mme_ue, return, "Null param");
            d_assert(s6abuf, return, "Null param");
            s6a_message = s6abuf->payload;
            d_assert(s6a_message, return, "Null param");

            switch(s6a_message->cmd_code)
            {
                case S6A_CMD_CODE_AUTHENTICATION_INFORMATION:
                {
                    if (s6a_message->result_code != ER_DIAMETER_SUCCESS)
                    {
                        rv = nas_send_attach_reject(mme_ue,
                            S1ap_CauseNas_authentication_failure,
                            EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED,
                            ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                        d_assert(rv == CORE_OK,,
                                "nas_send_attach_reject failed");

                        mme_ue_remove(mme_ue);
                        break;
                    }

                    mme_s6a_handle_aia(mme_ue, &s6a_message->aia_message);
                    break;
                }
                case S6A_CMD_CODE_UPDATE_LOCATION:
                {
                    if (s6a_message->result_code != ER_DIAMETER_SUCCESS)
                    {
                        rv = nas_send_attach_reject(mme_ue,
                            S1ap_CauseNas_unspecified,
                            EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED,
                            ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                        d_assert(rv == CORE_OK,,
                                "nas_send_attach_reject failed");

                        mme_ue_remove(mme_ue);
                        break;
                    }

                    mme_s6a_handle_ula(mme_ue, &s6a_message->ula_message);

                    if (FSM_CHECK(&mme_ue->sm, emm_state_default_esm))
                    {
                        rv = nas_send_emm_to_esm(mme_ue,
                                &mme_ue->pdn_connectivity_request);
                        d_assert(rv == CORE_OK,,
                                "nas_send_emm_to_esm failed");
                    }
                    else if (FSM_CHECK(&mme_ue->sm, emm_state_attached))
                    {
                        d_error("Not implemented for Tracking Area Update");
                    }
                    else
                        d_assert(0,, "Invaild EMM state");
                    break;
                }
                default:
                {
                    d_error("Invalid type(%d)", event_get_param2(e));
                    break;
                }
            }
            pkbuf_free(s6abuf);
            break;
        }
        case MME_EVT_S11_MESSAGE:
        {
            status_t rv;
            gtp_node_t *gnode = (gtp_node_t *)event_get_param1(e);
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param2(e);
            gtp_xact_t *xact = NULL;
            gtp_message_t message;
            mme_ue_t *mme_ue = NULL;
            enb_ue_t *enb_ue = NULL;

            d_assert(pkbuf, break, "Null param");
            d_assert(gnode, pkbuf_free(pkbuf); break, "Null param");

            rv = gtp_xact_receive(gnode, pkbuf, &xact, &message);
            if (rv != CORE_OK)
                break;

            d_assert(xact, return, "Null param");

            mme_ue = mme_ue_find_by_teid(message.h.teid);
            d_assert(mme_ue, pkbuf_free(pkbuf); break, 
                    "No UE Context(TEID:%d)", message.h.teid);
            enb_ue = mme_ue->enb_ue;
            d_assert(enb_ue, pkbuf_free(pkbuf);break, "Null param");
                    
            switch(message.h.type)
            {
                case GTP_CREATE_SESSION_RESPONSE_TYPE:
                {
                    mme_bearer_t *bearer = NULL;

                    gtp_create_session_response_t *rsp
                        = &message.create_session_response;
                    d_assert(rsp, return, "Null param");

                    if (rsp->bearer_contexts_created.presence == 0)
                    {
                        d_error("No Bearer");
                        return;
                    }
                    if (rsp->bearer_contexts_created.
                            eps_bearer_id.presence == 0)
                    {
                        d_error("No EPS Bearer ID");
                        return;
                    }

                    bearer = mme_bearer_find_by_ue_ebi(mme_ue, 
                            rsp->bearer_contexts_created.eps_bearer_id.u8);
                    d_assert(bearer, return, "Null param");

                    mme_s11_handle_create_session_response(xact, bearer, rsp);

                    if (FSM_CHECK(&mme_ue->sm, emm_state_default_esm))
                    {
                        rv = nas_send_attach_accept(mme_ue);
                        d_assert(rv == CORE_OK, return,
                                "nas_send_attach_accept failed");
                    }
                    else if (FSM_CHECK(&mme_ue->sm, emm_state_attached))
                    {
                        mme_sess_t *sess = NULL;
                        enb_ue_t *enb_ue = NULL;
                        pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;

                        sess = bearer->sess;
                        d_assert(sess, return, "Null param");
                        enb_ue = mme_ue->enb_ue;
                        d_assert(enb_ue, return, "Null param");

                        rv = esm_build_activate_default_bearer_context_request(
                                &esmbuf, sess);
                        d_assert(rv == CORE_OK && esmbuf, return,
                                "esm build error");

                        d_trace(3, "[NAS] Activate default bearer "
                                "context request : EMM <-- ESM\n");

                        rv = s1ap_build_e_rab_setup_request(
                                &s1apbuf, bearer, esmbuf);
                        d_assert(rv == CORE_OK && s1apbuf, 
                                pkbuf_free(esmbuf); return, "s1ap build error");

                        d_assert(nas_send_to_enb(enb_ue, s1apbuf) == CORE_OK,,);
                    }
                    else
                        d_assert(0, break, "Invalid EMM state");

                    break;
                }
                case GTP_MODIFY_BEARER_RESPONSE_TYPE:
                    mme_s11_handle_modify_bearer_response(
                        xact, mme_ue, &message.modify_bearer_response);
                    break;
                case GTP_DELETE_SESSION_RESPONSE_TYPE:
                {
                    mme_sess_t *sess = NULL;
                    gtp_delete_session_response_t *rsp = 
                        &message.delete_session_response;

                    d_assert(rsp, return, "Null param");
                    sess = GTP_XACT_RETRIEVE_SESSION(xact);
                    d_assert(sess, return, "Null param");

                    if (rsp->cause.presence == 0)
                    {
                        d_error("No Cause");
                        return;
                    }

                    d_trace(3, "[GTP] Delete Session Response : "
                            "MME[%d] <-- SGW[%d]\n",
                            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

                    rv = gtp_xact_commit(xact);
                    d_assert(rv == CORE_OK, return, "xact_commit error");

                    if (FSM_CHECK(&mme_ue->sm, emm_state_authentication))
                    {
                        mme_sess_remove(sess);
                        if (mme_sess_first(mme_ue) == NULL)
                        {
                            CLEAR_SGW_S11_PATH(mme_ue);
                            mme_s6a_send_air(mme_ue);
                        }
                    }
                    else if (FSM_CHECK(&mme_ue->sm, emm_state_detached))
                    {
                        mme_sess_remove(sess);
                        if (mme_sess_first(mme_ue) == NULL)
                        {
                            CLEAR_SGW_S11_PATH(mme_ue);
                            emm_handle_detach_accept(mme_ue);
                        }
                    }
                    else if (FSM_CHECK(&mme_ue->sm, emm_state_attached))
                    {
                        mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);
                        d_assert(bearer, return, "Null param");

                        if (FSM_CHECK(&bearer->sm, esm_state_disconnect))
                        {
                        }
                        else
                        {
                            d_assert(0, break, "Invalid ESM state");
                        }
                    }
                    else
                        d_assert(0, break, "Invalid EMM state");

                    break;
                }
                case GTP_CREATE_BEARER_REQUEST_TYPE:
                    mme_s11_handle_create_bearer_request(
                        xact, mme_ue, &message.create_bearer_request);
                    break;
                case GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
                {
                    mme_s11_handle_release_access_bearers_response(
                        xact, mme_ue, &message.release_access_bearers_response);

                    s1ap_handle_release_access_bearers_response(enb_ue);
                    break;
                }

                case GTP_DOWNLINK_DATA_NOTIFICATION_TYPE:
                {
                    mme_s11_handle_downlink_data_notification(
                        xact, mme_ue, &message.downlink_data_notification);

                    s1ap_handle_paging(mme_ue);
                    /* Start T3413 */
                    tm_start(mme_ue->t3413);
                    break;
                }
                default:
                    d_warn("Not implmeneted(type:%d)", message.h.type);
                    break;
            }
            pkbuf_free(pkbuf);
            break;
        }
        case MME_EVT_S11_T3_RESPONSE:
        case MME_EVT_S11_T3_HOLDING:
        {
            gtp_xact_timeout(event_get_param1(e), event_get(e));
            break;
        }
        default:
        {
            d_error("No handler for event %s", mme_event_get_name(e));
            break;
        }
    }
}
