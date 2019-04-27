#include "asn1c/s1ap_message.h"
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

void mme_state_initial(ogs_fsm_t *s, mme_event_t *e)
{
    mme_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &mme_state_operational);
}

void mme_state_final(ogs_fsm_t *s, mme_event_t *e)
{
    mme_sm_debug(e);

    ogs_assert(s);
}

void mme_state_operational(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(e);
    mme_sm_debug(e);

    ogs_assert(s);

    switch (e->id)
    {
        case OGS_FSM_ENTRY_SIG:
        {
            rv = mme_gtp_open();
            if (rv != OGS_OK)
            {
                ogs_error("Can't establish S11-GTP path");
                break;
            }
            rv = s1ap_open();
            if (rv != OGS_OK)
            {
                ogs_error("Can't establish S1AP path");
                break;
            }
    
            break;
        }
        case OGS_FSM_EXIT_SIG:
        {
            rv = mme_gtp_close();
            if (rv != OGS_OK)
            {
                ogs_error("Can't close S11-GTP path");
            }
            rv = s1ap_close();
            if (rv != OGS_OK)
            {
                ogs_error("Can't close S1AP path");
            }

            break;
        }
        case MME_EVT_S1AP_LO_ACCEPT:
        {
            ogs_sock_t *sock = NULL;
            ogs_sockaddr_t *addr = NULL;
            mme_enb_t *enb = NULL;

            sock = e->enb_sock;
            ogs_assert(sock);
            addr = e->enb_addr;
            ogs_assert(addr);

            ogs_info("eNB-S1 accepted[%s] in master_sm module", 
                OGS_ADDR(addr, buf));
                    
            enb = mme_enb_find_by_addr(addr);
            if (!enb)
            {
                enb = mme_enb_add(sock, addr);
                ogs_assert(enb);
            }
            else
            {
                ogs_warn("eNB context duplicated with IP-address [%s]!!!", 
                        OGS_ADDR(addr, buf));
                ogs_sock_destroy(sock);
                ogs_warn("S1 Socket Closed");
            }

            break;
        }
        case MME_EVT_S1AP_LO_SCTP_COMM_UP:
        {
            ogs_sock_t *sock = NULL;
            ogs_sockaddr_t *addr = NULL;
            mme_enb_t *enb = NULL;
            uint16_t outbound_streams = 0;

            sock = e->enb_sock;
            ogs_assert(sock);
            addr = e->enb_addr;
            ogs_assert(addr);

            outbound_streams = e->outbound_streams;

            enb = mme_enb_find_by_addr(addr);
            if (!enb)
            {
                enb = mme_enb_add(sock, addr);
                ogs_assert(enb);
            }
            else
            {
                ogs_free(addr);
            }

            enb->outbound_streams =
                    ogs_min(outbound_streams, enb->outbound_streams);

            ogs_debug("eNB-S1 SCTP_COMM_UP[%s] Outbound Streams[%d]", 
                OGS_ADDR(addr, buf), enb->outbound_streams);

            break;
        }
        case MME_EVT_S1AP_LO_CONNREFUSED:
        {
            ogs_sock_t *sock = NULL;
            ogs_sockaddr_t *addr = NULL;
            mme_enb_t *enb = NULL;

            sock = e->enb_sock;
            ogs_assert(sock);
            addr = e->enb_addr;
            ogs_assert(addr);

            enb = mme_enb_find_by_addr(addr);
            ogs_free(addr);

            if (enb)
            {
                ogs_info("eNB-S1[%x] connection refused!!!", enb->enb_id);
                mme_enb_remove(enb);
            }
            else
            {
                ogs_warn("Socket connection refused, Already Removed!");
            }

            break;
        }
        case MME_EVT_S1AP_MESSAGE:
        {
            s1ap_message_t message;
            mme_enb_t *enb = NULL;
            ogs_sock_t *sock = NULL;
            ogs_sockaddr_t *addr = NULL;
            ogs_pkbuf_t *pkbuf = NULL;
            int rc;

            sock = e->enb_sock;
            ogs_assert(sock);
            addr = e->enb_addr;
            ogs_assert(addr);
            pkbuf = e->pkbuf;
            ogs_assert(pkbuf);

            enb = mme_enb_find_by_addr(addr);
            ogs_free(addr);

            ogs_assert(enb);
            ogs_assert(OGS_FSM_STATE(&enb->sm));

            rc = s1ap_decode_pdu(&message, pkbuf);
            if (rc == OGS_OK)
            {
                e->enb = enb;
                e->s1ap_message = &message;
                ogs_fsm_dispatch(&enb->sm, e);
            }
            else
            {
                ogs_error("Cannot process S1AP message");
            }

            s1ap_free_pdu(&message);
            ogs_pkbuf_free(pkbuf);
            break;
        }
        case MME_EVT_EMM_MESSAGE:
        {
            nas_message_t message;
            ogs_pkbuf_t *pkbuf = NULL;
            enb_ue_t *enb_ue = NULL;
            mme_ue_t *mme_ue = NULL;

            enb_ue = e->enb_ue;
            ogs_assert(enb_ue);
            pkbuf = e->pkbuf;
            ogs_assert(pkbuf);
            ogs_assert(nas_emm_decode(&message, pkbuf) == OGS_OK);

            mme_ue = enb_ue->mme_ue;
            if (!mme_ue)
            {
                mme_ue = mme_ue_find_by_message(&message);
                if (!mme_ue)
                {
                    mme_ue = mme_ue_add(enb_ue);
                    ogs_assert(mme_ue);
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
                    h.type = e->nas_type;
                    if (h.integrity_protected)
                    {
                        /* Decryption was performed in S1AP handler.
                         * So, we disabled 'ciphered' 
                         * not to decrypt NAS message */
                        h.ciphered = 0;
                        ogs_assert(
                            nas_security_decode(mme_ue, h, pkbuf) == OGS_OK);
                    }
                }

                /* If NAS(mme_ue_t) has already been associated with
                 * older S1(enb_ue_t) context */
                if (ECM_CONNECTED(mme_ue))
                {
                   /* Implcit S1 release */
                    ogs_debug("Implicit S1 release");
                    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                          mme_ue->enb_ue->enb_ue_s1ap_id,
                          mme_ue->enb_ue->mme_ue_s1ap_id);
                    rv = enb_ue_remove(mme_ue->enb_ue);
                    ogs_assert(rv == OGS_OK);
                }
                mme_ue_associate_enb_ue(mme_ue, enb_ue);
            }

            ogs_assert(mme_ue);
            ogs_assert(OGS_FSM_STATE(&mme_ue->sm));

            e->mme_ue = mme_ue;
            e->nas_message = &message;

            ogs_fsm_dispatch(&mme_ue->sm, e);
            if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_exception))
            {
                rv = mme_send_delete_session_or_ue_context_release(
                        mme_ue, enb_ue);
                ogs_assert(rv == OGS_OK);
            }

            ogs_pkbuf_free(pkbuf);
            break;
        }
        case MME_EVT_ESM_MESSAGE:
        {
            nas_message_t message;
            mme_ue_t *mme_ue = NULL;
            mme_bearer_t *bearer = NULL;
            mme_bearer_t *default_bearer = NULL;
            mme_sess_t *sess = NULL;
            ogs_pkbuf_t *pkbuf = NULL;

            mme_ue = e->mme_ue;
            ogs_assert(mme_ue);

            pkbuf = e->pkbuf;
            ogs_assert(pkbuf);
            ogs_assert(nas_esm_decode(&message, pkbuf) == OGS_OK);

            bearer = mme_bearer_find_or_add_by_message(mme_ue, &message);
            ogs_assert(bearer);
            sess = bearer->sess;
            ogs_assert(sess);
            default_bearer = mme_default_bearer_in_sess(sess);
            ogs_assert(default_bearer);

            e->bearer = bearer;
            e->nas_message = &message;

            ogs_fsm_dispatch(&bearer->sm, e);
            if (OGS_FSM_CHECK(&bearer->sm, esm_state_bearer_deactivated) ||
                OGS_FSM_CHECK(&bearer->sm, esm_state_exception))
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
            else if (OGS_FSM_CHECK(&bearer->sm, esm_state_pdn_did_disconnect))
            {
                ogs_assert(default_bearer->ebi == bearer->ebi);
                mme_sess_remove(sess);
            }

            ogs_pkbuf_free(pkbuf);
            break;
        }
        case MME_EVT_S6A_MESSAGE:
        {
            int rv;
            mme_ue_t *mme_ue = NULL;
            ogs_pkbuf_t *s6abuf = NULL;
            s6a_message_t *s6a_message = NULL;

            mme_ue = e->mme_ue;
            ogs_assert(mme_ue);
            s6abuf = e->pkbuf;
            ogs_assert(s6abuf);
            s6a_message = s6abuf->data;
            ogs_assert(s6a_message);

            if (s6a_message->result_code != ER_DIAMETER_SUCCESS)
            {
                enb_ue_t *enb_ue = NULL;

                rv = nas_send_attach_reject(mme_ue,
                    EMM_CAUSE_IMSI_UNKNOWN_IN_HSS,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                ogs_assert(rv == OGS_OK);
                ogs_warn("EMM_CAUSE : IMSI Unknown in HSS");

                enb_ue = mme_ue->enb_ue;
                ogs_assert(enb_ue);

                rv = s1ap_send_ue_context_release_command(enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                ogs_assert(rv == OGS_OK);

                ogs_pkbuf_free(s6abuf);
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

                    if (OGS_FSM_CHECK(&mme_ue->sm,
                                emm_state_initial_context_setup))
                    {
                        if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST)
                        {
                            rv = nas_send_emm_to_esm(mme_ue,
                                    &mme_ue->pdn_connectivity_request);
                            ogs_assert(rv == OGS_OK);
                        }
                        else
                        {
                            ogs_fatal("Invalid Type[%d]", mme_ue->nas_eps.type);
                            ogs_assert_if_reached();
                        }
                    }
                    else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_registered))
                    {
                        if (mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST)
                        {
                            rv = nas_send_tau_accept(mme_ue,
                                    S1AP_ProcedureCode_id_InitialContextSetup);
                            ogs_assert(rv == OGS_OK);
                        }
                        else if (mme_ue->nas_eps.type ==
                            MME_EPS_TYPE_SERVICE_REQUEST)
                        {
                            rv = s1ap_send_initial_context_setup_request(
                                    mme_ue);
                            ogs_assert(rv == OGS_OK);
                        }
                        else
                        {
                            ogs_fatal("Invalid Type[%d]", mme_ue->nas_eps.type);
                            ogs_assert_if_reached();
                        }
                    }
                    else
                        ogs_assert_if_reached();
                    break;
                }
                default:
                {
                    ogs_error("Invalid Type[%d]", s6a_message->cmd_code);
                    break;
                }
            }
            ogs_pkbuf_free(s6abuf);
            break;
        }
        case MME_EVT_S11_MESSAGE:
        {
            int rv;
            ogs_pkbuf_t *pkbuf = NULL;
            gtp_xact_t *xact = NULL;
            gtp_message_t message;
            mme_ue_t *mme_ue = NULL;

            pkbuf = e->pkbuf;
            ogs_assert(pkbuf);
            rv = gtp_parse_msg(&message, pkbuf);
            ogs_assert(rv == OGS_OK);

            mme_ue = mme_ue_find_by_teid(message.h.teid);
            ogs_assert(mme_ue);

            rv = gtp_xact_receive(mme_ue->gnode, &message.h, &xact);
            if (rv != OGS_OK)
            {
                ogs_pkbuf_free(pkbuf);
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
                        ogs_timer_start(mme_ue->t3413, mme_self()->t3413_value);
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
                    ogs_warn("Not implmeneted(type:%d)", message.h.type);
                    break;
            }
            ogs_pkbuf_free(pkbuf);
            break;
        }
        default:
        {
            ogs_error("No handler for event %s", mme_event_get_name(e));
            break;
        }
    }
}
