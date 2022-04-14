/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "mme-context.h"
#include "mme-sm.h"
#include "mme-timer.h"

#include "s1ap-handler.h"
#include "s1ap-path.h"
#include "sgsap-path.h"
#include "nas-security.h"
#include "nas-path.h"
#include "emm-handler.h"
#include "esm-handler.h"
#include "mme-gtp-path.h"
#include "mme-s11-handler.h"
#include "mme-fd-path.h"
#include "mme-s6a-handler.h"
#include "mme-path.h"

/* 3GPP TS 29.272 Annex A; Table !.a:
 * Mapping from S6a error codes to NAS Cause Codes */
static uint8_t emm_cause_from_diameter(
        mme_ue_t *mme_ue, const uint32_t *dia_err, const uint32_t *dia_exp_err)
{
    ogs_assert(mme_ue);

    if (dia_exp_err) {
        switch (*dia_exp_err) {
        case OGS_DIAM_S6A_ERROR_USER_UNKNOWN:                   /* 5001 */
            ogs_info("[%s] User Unknown in HSS DB", mme_ue->imsi_bcd);
            return EMM_CAUSE_PLMN_NOT_ALLOWED;
        case OGS_DIAM_S6A_ERROR_UNKNOWN_EPS_SUBSCRIPTION:       /* 5420 */
            /* FIXME: Error diagnostic? */
            return EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA;
        case OGS_DIAM_S6A_ERROR_RAT_NOT_ALLOWED:                /* 5421 */
            return EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA;
        case OGS_DIAM_S6A_ERROR_ROAMING_NOT_ALLOWED:            /* 5004 */
            return EMM_CAUSE_PLMN_NOT_ALLOWED;
            //return EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN; (ODB_HPLMN_APN)
            //return EMM_CAUSE_ESM_FAILURE; (ODB_ALL_APN)
        case OGS_DIAM_S6A_AUTHENTICATION_DATA_UNAVAILABLE:      /* 4181 */
            return EMM_CAUSE_NETWORK_FAILURE;
        }
    }
    if (dia_err) {
        switch (*dia_err) {
        case ER_DIAMETER_AUTHORIZATION_REJECTED:                /* 5003 */
        case ER_DIAMETER_UNABLE_TO_DELIVER:                     /* 3002 */
        case ER_DIAMETER_REALM_NOT_SERVED:                      /* 3003 */
            return EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA;
        case ER_DIAMETER_UNABLE_TO_COMPLY:                      /* 5012 */
        case ER_DIAMETER_INVALID_AVP_VALUE:                     /* 5004 */
        case ER_DIAMETER_AVP_UNSUPPORTED:                       /* 5001 */
        case ER_DIAMETER_MISSING_AVP:                           /* 5005 */
        case ER_DIAMETER_RESOURCES_EXCEEDED:                    /* 5006 */
        case ER_DIAMETER_AVP_OCCURS_TOO_MANY_TIMES:             /* 5009 */
            return EMM_CAUSE_NETWORK_FAILURE;
        }
    }

    ogs_error("Unexpected Diameter Result Code %d/%d, defaulting to severe "
              "network failure",
              dia_err ? *dia_err : -1, dia_exp_err ? *dia_exp_err : -1);
    return EMM_CAUSE_SEVERE_NETWORK_FAILURE;
}

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

    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;
    mme_enb_t *enb = NULL;
    uint16_t max_num_of_ostreams = 0;

    ogs_s1ap_message_t s1ap_message;
    ogs_pkbuf_t *pkbuf = NULL;
    int rc;

    ogs_nas_eps_message_t nas_message;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    mme_bearer_t *bearer = NULL;
    mme_bearer_t *default_bearer = NULL;
    mme_sess_t *sess = NULL;

    ogs_diam_s6a_message_t *s6a_message = NULL;

    ogs_gtp_node_t *gnode = NULL;
    ogs_gtp_xact_t *xact = NULL;
    ogs_gtp2_message_t gtp_message;

    mme_vlr_t *vlr = NULL;

    ogs_assert(e);
    mme_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case MME_EVT_S1AP_LO_ACCEPT:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        ogs_info("eNB-S1 accepted[%s] in master_sm module",
            OGS_ADDR(addr, buf));

        enb = mme_enb_find_by_addr(addr);
        if (!enb) {
            enb = mme_enb_add(sock, addr);
            ogs_assert(enb);
        } else {
            ogs_warn("eNB context duplicated with IP-address [%s]!!!",
                    OGS_ADDR(addr, buf));
            ogs_sock_destroy(sock);
            ogs_free(addr);
            ogs_warn("S1 Socket Closed");
        }

        break;

    case MME_EVT_S1AP_LO_SCTP_COMM_UP:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        max_num_of_ostreams = e->max_num_of_ostreams;

        enb = mme_enb_find_by_addr(addr);
        if (!enb) {
            enb = mme_enb_add(sock, addr);
            ogs_assert(enb);
        } else {
            ogs_free(addr);
        }

        if (enb->max_num_of_ostreams)
            enb->max_num_of_ostreams =
                    ogs_min(max_num_of_ostreams, enb->max_num_of_ostreams);
        else
            enb->max_num_of_ostreams = max_num_of_ostreams;

        ogs_info("eNB-N2[%s] max_num_of_ostreams : %d",
            OGS_ADDR(enb->sctp.addr, buf), enb->max_num_of_ostreams);

        break;

    case MME_EVT_S1AP_LO_CONNREFUSED:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        enb = mme_enb_find_by_addr(addr);
        if (enb) {
            ogs_info("eNB-S1[%s] connection refused!!!", OGS_ADDR(addr, buf));
            mme_gtp_send_release_all_ue_in_enb(
                    enb, OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_LO_CONNREFUSED);
            mme_enb_remove(enb);
        } else {
            ogs_warn("eNB-S1[%s] connection refused, Already Removed!",
                    OGS_ADDR(addr, buf));
        }
        ogs_free(addr);

        break;
    case MME_EVT_S1AP_MESSAGE:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        enb = mme_enb_find_by_addr(addr);
        ogs_free(addr);

        ogs_assert(enb);
        ogs_assert(OGS_FSM_STATE(&enb->sm));

        rc = ogs_s1ap_decode(&s1ap_message, pkbuf);
        if (rc == OGS_OK) {
            e->enb = enb;
            e->s1ap_message = &s1ap_message;
            ogs_fsm_dispatch(&enb->sm, e);
        } else {
            ogs_warn("Cannot decode S1AP message");
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication(
                    enb, NULL, NULL, S1AP_Cause_PR_protocol,
                    S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message));
        }

        ogs_s1ap_free(&s1ap_message);
        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVT_S1AP_TIMER:
        enb_ue = e->enb_ue;
        ogs_assert(enb_ue);

        switch (e->timer_id) {
        case MME_TIMER_S1_DELAYED_SEND:
            enb = e->enb;
            ogs_assert(enb);
            pkbuf = e->pkbuf;
            ogs_assert(pkbuf);

            ogs_expect(OGS_OK == s1ap_send_to_enb_ue(enb_ue, pkbuf));
            ogs_timer_delete(e->timer);
            break;
        case MME_TIMER_S1_HOLDING:
            ogs_warn("Implicit S1 release");
            ogs_warn("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                  enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);
            s1ap_handle_ue_context_release_action(enb_ue);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;

    case MME_EVT_EMM_MESSAGE:
        enb_ue = e->enb_ue;
        ogs_assert(enb_ue);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        if (ogs_nas_emm_decode(&nas_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_nas_emm_decode() failed");
            ogs_pkbuf_free(pkbuf);
            return;
        }

        mme_ue = enb_ue->mme_ue;
        if (!mme_ue) {
            mme_ue = mme_ue_find_by_message(&nas_message);
            if (!mme_ue) {
                mme_ue = mme_ue_add(enb_ue);
                ogs_assert(mme_ue);
            } else {
                /* Here, if the MME_UE Context is found,
                 * the integrity check is not performed
                 * For example, ATTACH_REQUEST,
                 * TRACKING_AREA_UPDATE_REQUEST message
                 *
                 * Now, We will check the MAC in the NAS message*/
                ogs_nas_security_header_type_t h;
                h.type = e->nas_type;
                if (h.integrity_protected) {
                    /* Decryption was performed in S1AP handler.
                     * So, we disabled 'ciphered'
                     * not to decrypt NAS message */
                    h.ciphered = 0;
                    if (nas_eps_security_decode(mme_ue, h, pkbuf) != OGS_OK) {
                        ogs_error("nas_security_decode() failed");
                        ogs_pkbuf_free(pkbuf);
                        return;
                    }
                }
            }

            /* If NAS(mme_ue_t) has already been associated with
             * older S1(enb_ue_t) context */
            if (ECM_CONNECTED(mme_ue)) {
                /* Previous S1(enb_ue_t) context the holding timer(30secs)
                 * is started.
                 * Newly associated S1(enb_ue_t) context holding timer
                 * is stopped. */
                ogs_debug("Start S1 Holding Timer");
                ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                        mme_ue->enb_ue->enb_ue_s1ap_id,
                        mme_ue->enb_ue->mme_ue_s1ap_id);

                /* De-associate S1 with NAS/EMM */
                enb_ue_deassociate(mme_ue->enb_ue);

                ogs_assert(OGS_OK ==
                    s1ap_send_ue_context_release_command(mme_ue->enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE, 0));
            }
            mme_ue_associate_enb_ue(mme_ue, enb_ue);
        }

        ogs_assert(mme_ue);
        ogs_assert(OGS_FSM_STATE(&mme_ue->sm));

        e->mme_ue = mme_ue;
        e->nas_message = &nas_message;

        ogs_fsm_dispatch(&mme_ue->sm, e);
        if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_exception)) {
            mme_send_delete_session_or_mme_ue_context_release(mme_ue);
        }

        ogs_pkbuf_free(pkbuf);
        break;
    case MME_EVT_EMM_TIMER:
        mme_ue = e->mme_ue;
        ogs_assert(mme_ue);
        ogs_assert(OGS_FSM_STATE(&mme_ue->sm));

        ogs_fsm_dispatch(&mme_ue->sm, e);
        break;

    case MME_EVT_ESM_MESSAGE:
        mme_ue = e->mme_ue;
        ogs_assert(mme_ue);

        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);
        if (ogs_nas_esm_decode(&nas_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_nas_esm_decode() failed");
            ogs_pkbuf_free(pkbuf);
            break;
        }

        bearer = mme_bearer_find_or_add_by_message(
                    mme_ue, &nas_message, e->create_action);
        if (!bearer) {
            ogs_pkbuf_free(pkbuf);
            break;
        }

        sess = bearer->sess;
        ogs_assert(sess);
        default_bearer = mme_default_bearer_in_sess(sess);
        ogs_assert(default_bearer);

        e->bearer = bearer;
        e->nas_message = &nas_message;

        ogs_fsm_dispatch(&bearer->sm, e);
        if (OGS_FSM_CHECK(&bearer->sm, esm_state_bearer_deactivated)) {
            if (default_bearer->ebi == bearer->ebi) {
                /* if the bearer is a default bearer,
                 * remove all session context linked the default bearer */
                mme_sess_remove(sess);
            } else {
                /* if the bearer is not a default bearer,
                 * just remove the bearer context */
                mme_bearer_remove(bearer);
            }

        } else if (OGS_FSM_CHECK(&bearer->sm, esm_state_pdn_did_disconnect)) {
            ogs_assert(default_bearer->ebi == bearer->ebi);
            mme_sess_remove(sess);

        } else if (OGS_FSM_CHECK(&bearer->sm, esm_state_exception)) {

            /*
             * The UE requested the wrong APN.
             *
             * From the Issues #568, MME need to accept further service request.
             * To do this, we are not going to release UE context.
             *
             * Just we'll remove MME session context.
             */
            mme_sess_remove(sess);
        }

        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVT_ESM_TIMER:
        bearer = e->bearer;
        ogs_assert(bearer);
        ogs_assert(OGS_FSM_STATE(&bearer->sm));

        ogs_fsm_dispatch(&bearer->sm, e);
        break;

    case MME_EVT_S6A_MESSAGE:
        mme_ue = e->mme_ue;
        ogs_assert(mme_ue);
        s6a_message = e->s6a_message;
        ogs_assert(s6a_message);

        enb_ue = enb_ue_cycle(mme_ue->enb_ue);
        if (!enb_ue) {
            ogs_error("S1 context has already been removed");

            ogs_subscription_data_free(
                    &s6a_message->ula_message.subscription_data);
            ogs_free(s6a_message);
            break;
        }

        if (s6a_message->result_code != ER_DIAMETER_SUCCESS) {
            /* Unfortunately fd doesn't distinguish
             * between result-code and experimental-result-code.
             *
             * However, e.g. 5004 has different meaning
             * if used in result-code than in experimental-result-code */
            uint8_t emm_cause = emm_cause_from_diameter(
                    mme_ue, s6a_message->err, s6a_message->exp_err);

            ogs_info("[%s] Attach reject [EMM_CAUSE:%d]",
                    mme_ue->imsi_bcd, emm_cause);
            ogs_assert(OGS_OK ==
                nas_eps_send_attach_reject(mme_ue,
                    emm_cause, ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED));

            ogs_assert(OGS_OK ==
                s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0));

            ogs_subscription_data_free(
                    &s6a_message->ula_message.subscription_data);
            ogs_free(s6a_message);
            break;
        }

        switch (s6a_message->cmd_code) {
        case OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION:
            mme_s6a_handle_aia(mme_ue, &s6a_message->aia_message);
            break;
        case OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION:
            mme_s6a_handle_ula(mme_ue, &s6a_message->ula_message);

            if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
                rv = nas_eps_send_emm_to_esm(mme_ue,
                        &mme_ue->pdn_connectivity_request);
                if (rv != OGS_OK) {
                    ogs_error("nas_eps_send_emm_to_esm() failed");
                    ogs_assert(OGS_OK ==
                        nas_eps_send_attach_reject(mme_ue,
                        EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                        ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED));
                }
            } else if (mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST) {
                ogs_assert(OGS_OK ==
                    nas_eps_send_tau_accept(mme_ue,
                        S1AP_ProcedureCode_id_InitialContextSetup));
            } else if (mme_ue->nas_eps.type == MME_EPS_TYPE_SERVICE_REQUEST) {
                ogs_error("[%s] Service request", mme_ue->imsi_bcd);
            } else if (mme_ue->nas_eps.type ==
                    MME_EPS_TYPE_DETACH_REQUEST_FROM_UE) {
                ogs_error("[%s] Detach request", mme_ue->imsi_bcd);
            } else {
                ogs_fatal("Invalid Type[%d]", mme_ue->nas_eps.type);
                ogs_assert_if_reached();
            }
            break;
        default:
            ogs_error("Invalid Type[%d]", s6a_message->cmd_code);
            break;
        }
        ogs_subscription_data_free(&s6a_message->ula_message.subscription_data);
        ogs_free(s6a_message);
        break;

    case MME_EVT_S11_MESSAGE:
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        if (ogs_gtp2_parse_msg(&gtp_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_gtp2_parse_msg() failed");
            ogs_pkbuf_free(pkbuf);
            break;
        }

        /*
         * 5.5.2 in spec 29.274
         *
         * If a peer's TEID is not available, the TEID field still shall be
         * present in the header and its value shall be set to "0" in the
         * following messages:
         *
         * - Create Session Request message on S2a/S2b/S5/S8
         *
         * - Create Session Request message on S4/S11, if for a given UE,
         *   the SGSN/MME has not yet obtained the Control TEID of the SGW.
         *
         * - If a node receives a message and the TEID-C in the GTPv2 header of
         *   the received message is not known, it shall respond with
         *   "Context not found" Cause in the corresponding response message
         *   to the sender, the TEID used in the GTPv2-C header in the response
         *   message shall be then set to zero.
         *
         * - If a node receives a request message containing protocol error,
         *   e.g. Mandatory IE missing, which requires the receiver to reject
         *   the message as specified in clause 7.7, it shall reject
         *   the request message. For the response message, the node should
         *   look up the remote peer's TEID and accordingly set the GTPv2-C
         *   header TEID and the message cause code. As an implementation
         *   option, the node may not look up the remote peer's TEID and
         *   set the GTPv2-C header TEID to zero in the response message.
         *   However in this case, the cause code shall not be set to
         *   "Context not found".
         */
        if (gtp_message.h.teid_presence && gtp_message.h.teid != 0) {
            /* Cause is not "Context not found" */
            mme_ue = mme_ue_find_by_teid(gtp_message.h.teid);
        }

        if (mme_ue) {
            gnode = mme_ue->gnode;
            ogs_assert(gnode);

        } else {
            gnode = e->gnode;
            ogs_assert(gnode);
        }

        rv = ogs_gtp_xact_receive(gnode, &gtp_message.h, &xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(pkbuf);
            break;
        }

        switch (gtp_message.h.type) {
        case OGS_GTP2_ECHO_REQUEST_TYPE:
            mme_s11_handle_echo_request(xact, &gtp_message.echo_request);
            break;
        case OGS_GTP2_ECHO_RESPONSE_TYPE:
            mme_s11_handle_echo_response(xact, &gtp_message.echo_response);
            break;
        case OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE:
            mme_s11_handle_create_session_response(
                xact, mme_ue, &gtp_message.create_session_response);
            break;
        case OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE:
            mme_s11_handle_modify_bearer_response(
                xact, mme_ue, &gtp_message.modify_bearer_response);
            break;
        case OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE:
            mme_s11_handle_delete_session_response(
                xact, mme_ue, &gtp_message.delete_session_response);
            break;
        case OGS_GTP2_CREATE_BEARER_REQUEST_TYPE:
            mme_s11_handle_create_bearer_request(
                xact, mme_ue, &gtp_message.create_bearer_request);
            break;
        case OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE:
            mme_s11_handle_update_bearer_request(
                xact, mme_ue, &gtp_message.update_bearer_request);
            break;
        case OGS_GTP2_DELETE_BEARER_REQUEST_TYPE:
            mme_s11_handle_delete_bearer_request(
                xact, mme_ue, &gtp_message.delete_bearer_request);
            break;
        case OGS_GTP2_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
            mme_s11_handle_release_access_bearers_response(
                xact, mme_ue, &gtp_message.release_access_bearers_response);
            break;
        case OGS_GTP2_DOWNLINK_DATA_NOTIFICATION_TYPE:
            if (!mme_ue) {
                if (gtp_message.h.teid_presence)
                    ogs_warn("No Context : TEID[%d]", gtp_message.h.teid);
                else
                    ogs_warn("No Context : No TEID");

                break;
            }
            mme_s11_handle_downlink_data_notification(
                xact, mme_ue, &gtp_message.downlink_data_notification);
            break;
        case OGS_GTP2_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            mme_s11_handle_create_indirect_data_forwarding_tunnel_response(
                xact, mme_ue,
                &gtp_message.create_indirect_data_forwarding_tunnel_response);
            break;
        case OGS_GTP2_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            mme_s11_handle_delete_indirect_data_forwarding_tunnel_response(
                xact, mme_ue,
                &gtp_message.delete_indirect_data_forwarding_tunnel_response);
            break;
        case OGS_GTP2_BEARER_RESOURCE_FAILURE_INDICATION_TYPE:
            mme_s11_handle_bearer_resource_failure_indication(
                xact, mme_ue,
                &gtp_message.bearer_resource_failure_indication);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", gtp_message.h.type);
            break;
        }
        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVT_SGSAP_LO_SCTP_COMM_UP:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        max_num_of_ostreams = e->max_num_of_ostreams;

        vlr = mme_vlr_find_by_addr(addr);
        ogs_free(addr);

        ogs_assert(vlr);
        ogs_assert(OGS_FSM_STATE(&vlr->sm));

        vlr->max_num_of_ostreams =
                ogs_min(max_num_of_ostreams, vlr->max_num_of_ostreams);

        ogs_debug("VLR-SGs SCTP_COMM_UP[%s] Max Num of Outbound Streams[%d]",
            OGS_ADDR(vlr->addr, buf), vlr->max_num_of_ostreams);

        e->vlr = vlr;
        ogs_fsm_dispatch(&vlr->sm, e);
        break;

    case MME_EVT_SGSAP_LO_CONNREFUSED:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        vlr = mme_vlr_find_by_addr(addr);
        ogs_free(addr);

        ogs_assert(vlr);
        ogs_assert(OGS_FSM_STATE(&vlr->sm));

        if (OGS_FSM_CHECK(&vlr->sm, sgsap_state_connected)) {
            e->vlr = vlr;
            ogs_fsm_dispatch(&vlr->sm, e);

            ogs_info("VLR-SGs[%s] connection refused!!!",
                    OGS_ADDR(vlr->addr, buf));

        } else {
            ogs_warn("VLR-SGs[%s] connection refused, Already Removed!",
                    OGS_ADDR(vlr->addr, buf));
        }

        break;
    case MME_EVT_SGSAP_MESSAGE:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        vlr = mme_vlr_find_by_addr(addr);
        ogs_free(addr);

        ogs_assert(vlr);
        ogs_assert(OGS_FSM_STATE(&vlr->sm));

        e->vlr = vlr;
        ogs_fsm_dispatch(&vlr->sm, e);

        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVT_SGSAP_TIMER:
        vlr = e->vlr;
        ogs_assert(vlr);
        ogs_assert(OGS_FSM_STATE(&vlr->sm));

        ogs_fsm_dispatch(&vlr->sm, e);
        break;

    default:
        ogs_error("No handler for event %s", mme_event_get_name(e));
        break;
    }
}
