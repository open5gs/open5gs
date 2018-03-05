#define TRACE_MODULE _mme_sm
#include "core_debug.h"

#include "s1ap/s1ap_message.h"
#include "nas/nas_message.h"
#include "gtp/gtp_xact.h"
#include "fd/fd_lib.h"

#include "mme_event.h"
#include "mme_sm.h"

#include "s1ap_handler.h"
#include "s1ap_path.h"
#include "nas_security.h"
#include "nas_path.h"
#include "emm_handler.h"
#include "esm_handler.h"
#include "mme_gtp_path.h"
#include "mme_s11_handler.h"
#include "mme_fd_path.h"
#include "mme_s6a_handler.h"
#include "mme_path.h"

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
    char buf[CORE_ADDRSTRLEN];

    mme_sm_trace(3, e);

    d_assert(s, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            rv = mme_gtp_open();
            if (rv != CORE_OK)
            {
                d_error("Can't establish S11-GTP path");
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
                d_error("Can't close S11-GTP path");
            }
            rv = s1ap_close();
            if (rv != CORE_OK)
            {
                d_error("Can't close S1AP path");
            }

            break;
        }
        case MME_EVT_S1AP_LO_ACCEPT:
        {
            sock_id sock = (sock_id)event_get_param1(e);
            d_assert(sock, break,);
            c_sockaddr_t *addr = (c_sockaddr_t *)event_get_param2(e);
            d_assert(addr, break,);
            mme_enb_t *enb = NULL;

            d_trace(1, "eNB-S1 accepted[%s] in master_sm module\n", 
                CORE_ADDR(addr, buf));
                    
            enb = mme_enb_find_by_addr(addr);
            if (!enb)
            {
#if USE_USRSCTP != 1
                status_t rv = sock_register(sock, s1ap_recv_handler, NULL);
                d_assert(rv == CORE_OK, break, "register s1ap_recv_cb failed");
#endif

                mme_enb_t *enb = mme_enb_add(sock, addr);
                d_assert(enb, break, "Null param");
            }
            else
            {
                d_warn("eNB context duplicated with IP-address [%s]!!!", 
                        CORE_ADDR(addr, buf));
                sock_delete(sock);
                d_warn("S1 Socket Closed");
            }

            break;
        }
        case MME_EVT_S1AP_LO_CONNREFUSED:
        {
            mme_enb_t *enb = NULL;
            sock_id sock = 0;
            c_sockaddr_t *addr = NULL;

            sock = (sock_id)event_get_param1(e);
            d_assert(sock, break, "Null param");
            addr = (c_sockaddr_t *)event_get_param2(e);
            d_assert(addr, break, "Null param");

#ifdef NO_FD_LOCK
            enb = mme_enb_find_by_addr(addr);
#else
#error do not use lock in socket fd
            /* 
             * <Connection Refused>
             * if socket type is SOCK_STREAM,
             * I'm not sure whether address is available or not.
             * So, I'll use 'sock_id' at this point.
             */
            if (mme_enb_sock_type(sock) == SOCK_STREAM)
                enb = mme_enb_find_by_sock(sock);
            else
                enb = mme_enb_find_by_addr(addr);
#endif
            CORE_FREE(addr);

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
            sock_id sock = 0;
            c_sockaddr_t *addr = NULL;
            pkbuf_t *pkbuf = NULL;

            sock = (sock_id)event_get_param1(e);
            d_assert(sock, break, "Null param");

            addr = (c_sockaddr_t *)event_get_param2(e);
            d_assert(addr, break, "Null param");
            
            pkbuf = (pkbuf_t *)event_get_param3(e);
            d_assert(pkbuf, break, "Null param");

            enb = mme_enb_find_by_addr(addr);
            CORE_FREE(addr);

            d_assert(enb, pkbuf_free(pkbuf); break, "No eNB context");
            d_assert(FSM_STATE(&enb->sm), pkbuf_free(pkbuf); break,
                    "No S1AP State Machine");

            rv = s1ap_decode_pdu(&message, pkbuf);
            if (rv != CORE_OK)
            {
                d_print_hex(pkbuf->payload, pkbuf->len);
                d_assert(0, pkbuf_free(pkbuf); break, "Can't decode S1AP_PDU");
            }

            event_set_param1(e, (c_uintptr_t)enb->index);
            event_set_param4(e, (c_uintptr_t)&message);
            fsm_dispatch(&enb->sm, (fsm_event_t*)e);

            s1ap_free_pdu(&message);
            pkbuf_free(pkbuf);
            break;
        }
        case MME_EVT_S1AP_DELAYED_SEND:
        {
            mme_enb_t *enb = NULL;
            pkbuf_t *pkbuf = NULL;
            tm_block_id timer = 0;

            enb = mme_enb_find(event_get_param1(e));
            d_assert(enb, break,);

            pkbuf = (pkbuf_t *)event_get_param2(e);
            d_assert(pkbuf, break,);

            timer = event_get_param3(e);
            d_assert(timer, pkbuf_free(pkbuf); break,);

            rv = s1ap_send_to_enb(enb, pkbuf);
            d_assert(rv == CORE_OK, pkbuf_free(pkbuf),);

            tm_delete(timer);
            break;
        }
        case MME_EVT_S1AP_S1_HOLDING_TIMER:
        {
            enb_ue_t *enb_ue = NULL;

            enb_ue = enb_ue_find(event_get_param1(e));
            d_assert(enb_ue, break, "No ENB UE context");
            d_warn("Implicit S1 release");
            d_warn("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

            rv = enb_ue_remove(enb_ue);
            d_assert(rv == CORE_OK,,);
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
            pkbuf = (pkbuf_t *)event_get_param4(e);
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
                    d_assert(mme_ue, pkbuf_free(pkbuf); break, "Null param");
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
                    h.type = (c_uint8_t)event_get_param3(e);
                    if (h.integrity_protected)
                    {
                        /* Decryption was performed in S1AP handler.
                         * So, we disabled 'ciphered' 
                         * not to decrypt NAS message */
                        h.ciphered = 0;
                        d_assert(
                            nas_security_decode(mme_ue, h, pkbuf) == CORE_OK,
                            pkbuf_free(pkbuf); return, 
                            "nas_security_decode failed");
                    }
                }

                /* If NAS(mme_ue_t) has already been associated with
                 * older S1(enb_ue_t) context */
                if (ECM_CONNECTED(mme_ue))
                {
#if IMPLICIT_S1_RELEASE
                   /* Implcit S1 release */
                    d_warn("Implicit S1 release");
                    d_warn("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                          mme_ue->enb_ue->enb_ue_s1ap_id,
                          mme_ue->enb_ue->mme_ue_s1ap_id);
                    rv = enb_ue_remove(mme_ue->enb_ue);
                    d_assert(rv == CORE_OK,,);

#else /* S1_HOLDING_TIMER */

                    /* Previous S1(enb_ue_t) context the holding timer(30secs)
                     * is started.
                     * Newly associated S1(enb_ue_t) context holding timer
                     * is stopped. */
                    d_trace(5, "Start S1 Holding Timer\n");
                    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
                            mme_ue->enb_ue->enb_ue_s1ap_id, 
                            mme_ue->enb_ue->mme_ue_s1ap_id);
                    tm_start(mme_ue->enb_ue->holding_timer);

                    /* De-associate S1 with NAS/EMM */
                    rv = enb_ue_deassociate(mme_ue->enb_ue);
                    d_assert(rv == CORE_OK,,);
#endif
                }
                tm_stop(enb_ue->holding_timer);
                mme_ue_associate_enb_ue(mme_ue, enb_ue);
            }

            d_assert(mme_ue, pkbuf_free(pkbuf); break, "No MME UE context");
            d_assert(FSM_STATE(&mme_ue->sm), pkbuf_free(pkbuf); break, 
                    "No EMM State Machine");

            event_set_param1(e, (c_uintptr_t)mme_ue->index);
            event_set_param5(e, (c_uintptr_t)&message);

            fsm_dispatch(&mme_ue->sm, (fsm_event_t*)e);
            if (FSM_CHECK(&mme_ue->sm, emm_state_exception))
            {
                rv = mme_send_delete_session_or_ue_context_release(
                        mme_ue, enb_ue);
                d_assert(rv == CORE_OK,,
                        "mme_send_delete_session_or_ue_context_release() failed");
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
            mme_ue_t *mme_ue = NULL;
            mme_bearer_t *bearer = NULL;
            mme_bearer_t *default_bearer = NULL;
            mme_sess_t *sess = NULL;
            pkbuf_t *pkbuf = NULL;

            mme_ue = mme_ue_find(event_get_param1(e));
            d_assert(mme_ue, break, "No UE context");

            pkbuf = (pkbuf_t *)event_get_param2(e);
            d_assert(pkbuf, break, "Null param");
            d_assert(nas_esm_decode(&message, pkbuf) == CORE_OK,
                    pkbuf_free(pkbuf); break, "Can't decode NAS_ESM");

            bearer = mme_bearer_find_or_add_by_message(mme_ue, &message);
            d_assert(bearer, pkbuf_free(pkbuf); break, "No Bearer context");
            sess = bearer->sess;
            d_assert(sess, pkbuf_free(pkbuf); break, "Null param");
            default_bearer = mme_default_bearer_in_sess(sess);
            d_assert(default_bearer, pkbuf_free(pkbuf); break, "Null param");

            event_set_param1(e, (c_uintptr_t)bearer->index);
            event_set_param3(e, (c_uintptr_t)&message);

            fsm_dispatch(&bearer->sm, (fsm_event_t*)e);
            if (FSM_CHECK(&bearer->sm, esm_state_bearer_deactivated) ||
                FSM_CHECK(&bearer->sm, esm_state_exception))
            {
                if (default_bearer->ebi == bearer->ebi)
                {
                    /* if the bearer is a default bearer,
                     * remove all session context linked the default bearer */
                    mme_sess_remove(sess);
                }
                else
                {
                    /* if the bearer is not a default bearer,
                     * just remove the bearer context */
                    mme_bearer_remove(bearer);
                }
            }
            else if (FSM_CHECK(&bearer->sm, esm_state_pdn_did_disconnect))
            {
                d_assert(default_bearer->ebi == bearer->ebi,
                        pkbuf_free(pkbuf); break,
                        "Bearer[%d] is not Default Bearer",
                        default_bearer->ebi, bearer->ebi);
                mme_sess_remove(sess);
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

            if (s6a_message->result_code != ER_DIAMETER_SUCCESS)
            {
                enb_ue_t *enb_ue = NULL;

                rv = nas_send_attach_reject(mme_ue,
                    EMM_CAUSE_IMSI_UNKNOWN_IN_HSS,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                d_assert(rv == CORE_OK,,
                        "nas_send_attach_reject failed");

                enb_ue = mme_ue->enb_ue;
                d_assert(enb_ue, break, "No ENB UE context");

                rv = s1ap_send_ue_context_release_command(enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                d_assert(rv == CORE_OK,, "s1ap send error");

                pkbuf_free(s6abuf);
                break;
            }

            switch(s6a_message->cmd_code)
            {
                case S6A_CMD_CODE_AUTHENTICATION_INFORMATION:
                {
                    mme_s6a_handle_aia(mme_ue, &s6a_message->aia_message);
                    break;
                }
                case S6A_CMD_CODE_UPDATE_LOCATION:
                {
                    mme_s6a_handle_ula(mme_ue, &s6a_message->ula_message);

                    if (FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup))
                    {
                        if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST)
                        {
                            rv = nas_send_emm_to_esm(mme_ue,
                                    &mme_ue->pdn_connectivity_request);
                            d_assert(rv == CORE_OK,,
                                    "nas_send_emm_to_esm() failed");
                        }
                        else
                            d_assert(0,, "Invalid Type[%d]",
                                    mme_ue->nas_eps.type);
                    }
                    else if (FSM_CHECK(&mme_ue->sm, emm_state_registered))
                    {
                        if (mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST)
                        {
                            rv = nas_send_tau_accept(mme_ue,
                                    S1AP_ProcedureCode_id_InitialContextSetup);
                            d_assert(rv == CORE_OK,,
                                    "nas_send_tau_accept() failed");
                        }
                        else if (mme_ue->nas_eps.type ==
                            MME_EPS_TYPE_SERVICE_REQUEST)
                        {
                            rv = s1ap_send_initial_context_setup_request(
                                    mme_ue);
                            d_assert(rv == CORE_OK,,
                                "s1ap_send_initial_context_setup_request()"
                                "failed");
                        }
                        else
                            d_assert(0,, "Invalid EPS-Type[%d]",
                                    mme_ue->nas_eps.type);
                    }
                    else
                        d_assert(0,, "Invaild EMM state for EPS-Type[%d]",
                                    mme_ue->nas_eps.type);
                    break;
                }
                default:
                {
                    d_error("Invalid Type[%d]", event_get_param2(e));
                    break;
                }
            }
            pkbuf_free(s6abuf);
            break;
        }
        case MME_EVT_S11_MESSAGE:
        {
            status_t rv;
            pkbuf_t *pkbuf = (pkbuf_t *)event_get_param1(e);
            gtp_xact_t *xact = NULL;
            gtp_message_t message;
            mme_ue_t *mme_ue = NULL;

            d_assert(pkbuf, break, "Null param");
            rv = gtp_parse_msg(&message, pkbuf);
            d_assert(rv == CORE_OK, pkbuf_free(pkbuf); break, "parse error");

            mme_ue = mme_ue_find_by_teid(message.h.teid);
            d_assert(mme_ue, pkbuf_free(pkbuf); break, 
                    "No UE Context(TEID:%d)", message.h.teid);

            rv = gtp_xact_receive(mme_ue->gnode, &message.h, &xact);
            if (rv != CORE_OK)
            {
                pkbuf_free(pkbuf);
                break;
            }

            switch(message.h.type)
            {
                case GTP_CREATE_SESSION_RESPONSE_TYPE:
                    mme_s11_handle_create_session_response(
                        xact, mme_ue, &message.create_session_response);
                    break;
                case GTP_MODIFY_BEARER_RESPONSE_TYPE:
                    mme_s11_handle_modify_bearer_response(
                        xact, mme_ue, &message.modify_bearer_response);
                    break;
                case GTP_DELETE_SESSION_RESPONSE_TYPE:
                    mme_s11_handle_delete_session_response(
                        xact, mme_ue, &message.delete_session_response);
                    break;
                case GTP_CREATE_BEARER_REQUEST_TYPE:
                    mme_s11_handle_create_bearer_request(
                        xact, mme_ue, &message.create_bearer_request);
                    break;
                case GTP_UPDATE_BEARER_REQUEST_TYPE:
                    mme_s11_handle_update_bearer_request(
                        xact, mme_ue, &message.update_bearer_request);
                    break;
                case GTP_DELETE_BEARER_REQUEST_TYPE:
                    mme_s11_handle_delete_bearer_request(
                        xact, mme_ue, &message.delete_bearer_request);
                    break;
                case GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
                    mme_s11_handle_release_access_bearers_response(
                        xact, mme_ue, &message.release_access_bearers_response);
                    break;
                case GTP_DOWNLINK_DATA_NOTIFICATION_TYPE:
                    mme_s11_handle_downlink_data_notification(
                        xact, mme_ue, &message.downlink_data_notification);

/*
 * 5.3.4.2 in Spec 23.401
 * Under certain conditions, the current UE triggered Service Request 
 * procedure can cause unnecessary Downlink Packet Notification messages 
 * which increase the load of the MME.
 *
 * This can occur when uplink data sent in step 6 causes a response 
 * on the downlink which arrives at the Serving GW before the Modify Bearer 
 * Request message, step 8. This data cannot be forwarded from the Serving GW 
 * to the eNodeB and hence it triggers a Downlink Data Notification message.
 *
 * If the MME receives a Downlink Data Notification after step 2 and 
 * before step 9, the MME shall not send S1 interface paging messages
 */
                    if (ECM_IDLE(mme_ue))
                    {
                        s1ap_handle_paging(mme_ue);
                        /* Start T3413 */
                        tm_start(mme_ue->t3413);
                    }
                    break;
                case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
                    mme_s11_handle_create_indirect_data_forwarding_tunnel_response(
                        xact, mme_ue,
                        &message.create_indirect_data_forwarding_tunnel_response);
                    break;
                case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
                    mme_s11_handle_delete_indirect_data_forwarding_tunnel_response(
                        xact, mme_ue,
                        &message.delete_indirect_data_forwarding_tunnel_response);
                    break;
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
