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

#include "mme-event.h"
#include "mme-timer.h"
#include "mme-sm.h"
#include "mme-fd-path.h"
#include "emm-handler.h"
#include "esm-build.h"
#include "esm-handler.h"
#include "mme-s11-handler.h"
#include "s1ap-path.h"
#include "nas-path.h"
#include "mme-gtp-path.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __esm_log_domain

static uint8_t gtp_cause_from_esm(uint8_t esm_cause)
{
    switch (esm_cause) {
    case ESM_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION:
        return OGS_GTP2_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION;
    case ESM_CAUSE_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION:
        return OGS_GTP2_CAUSE_SYNTACTIC_ERROR_IN_THE_TFT_OPERATION;
    case ESM_CAUSE_SYNTACTICAL_ERROR_IN_PACKET_FILTERS:
        return OGS_GTP2_CAUSE_SYNTACTIC_ERRORS_IN_PACKET_FILTER;
    case ESM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTERS:
        return OGS_GTP2_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER;
    default:
        break;
    }

    return OGS_GTP2_CAUSE_SYSTEM_FAILURE;
}

void esm_state_initial(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);

    mme_sm_debug(e);

    OGS_FSM_TRAN(s, &esm_state_inactive);
}

void esm_state_final(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);

    mme_sm_debug(e);
}

void esm_state_inactive(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_nas_eps_activate_dedicated_eps_bearer_context_reject_t
        *activate_dedicated_eps_bearer_context_reject = NULL;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    bearer = e->bearer;
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_BEARER_ALL_TIMERS(bearer);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_ESM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->esm.h.message_type) {
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST:
            ogs_debug("PDN Connectivity request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            rv = esm_handle_pdn_connectivity_request(
                    bearer, &message->esm.pdn_connectivity_request,
                    e->create_action);
            if (rv != OGS_OK) {
                OGS_FSM_TRAN(s, esm_state_exception);
                break;
            }
            break;
        case OGS_NAS_EPS_PDN_DISCONNECT_REQUEST:
            ogs_debug("PDN disconnect request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            if (MME_HAVE_SGW_S1U_PATH(sess)) {
                ogs_assert(OGS_OK ==
                    mme_gtp_send_delete_session_request(mme_ue->sgw_ue, sess,
                    OGS_GTP_DELETE_SEND_DEACTIVATE_BEARER_CONTEXT_REQUEST));
            } else {
                ogs_assert(OGS_OK ==
                    nas_eps_send_deactivate_bearer_context_request(bearer));
            }
            OGS_FSM_TRAN(s, esm_state_pdn_will_disconnect);
            break;
        case OGS_NAS_EPS_ESM_INFORMATION_RESPONSE:
            ogs_debug("ESM information response");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);

            CLEAR_BEARER_TIMER(bearer->t3489);

            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] No Integrity Protected", mme_ue->imsi_bcd);
                ogs_assert(OGS_OK ==
                    nas_eps_send_attach_reject(mme_ue,
                        EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                        ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED));
                ogs_assert(mme_ue->enb_ue);
                ogs_assert(OGS_OK ==
                    s1ap_send_ue_context_release_command(mme_ue->enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0));
                OGS_FSM_TRAN(s, &esm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_warn("[%s] No Security Context", mme_ue->imsi_bcd);
                ogs_assert(OGS_OK ==
                    nas_eps_send_attach_reject(mme_ue,
                        EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                        ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED));
                ogs_assert(mme_ue->enb_ue);
                ogs_assert(OGS_OK ==
                    s1ap_send_ue_context_release_command(mme_ue->enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0));
                OGS_FSM_TRAN(s, &esm_state_exception);
                break;
            }

            rv = esm_handle_information_response(
                    sess, &message->esm.esm_information_response);
            if (rv != OGS_OK) {
                OGS_FSM_TRAN(s, esm_state_exception);
                break;
            }
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("Activate default EPS bearer context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            /* Check if Initial Context Setup Response or 
             *          E-RAB Setup Response is received */
            if (MME_HAVE_ENB_S1U_PATH(bearer)) {
                ogs_list_init(&mme_ue->bearer_to_modify_list);
                ogs_list_add(&mme_ue->bearer_to_modify_list,
                                &bearer->to_modify_node);
                ogs_assert(OGS_OK ==
                    mme_gtp_send_modify_bearer_request(mme_ue, 0, 0));
            }

            nas_eps_send_activate_all_dedicated_bearers(bearer);
            OGS_FSM_TRAN(s, esm_state_active);
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("Activate dedicated EPS bearer context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            /* Check if Initial Context Setup Response or 
             *          E-RAB Setup Response is received */
            if (MME_HAVE_ENB_S1U_PATH(bearer)) {
                ogs_assert(OGS_OK ==
                    mme_gtp_send_create_bearer_response(
                        bearer, OGS_GTP2_CAUSE_REQUEST_ACCEPTED));
            }

            OGS_FSM_TRAN(s, esm_state_active);
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT:
            ogs_error("Activate dedicated EPS bearer context reject");
            ogs_error("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            activate_dedicated_eps_bearer_context_reject =
                &message->esm.activate_dedicated_eps_bearer_context_reject;
            ogs_assert(activate_dedicated_eps_bearer_context_reject);
            ogs_assert(OGS_OK ==
                mme_gtp_send_create_bearer_response(bearer,
                gtp_cause_from_esm(
                    activate_dedicated_eps_bearer_context_reject->esm_cause)));
            OGS_FSM_TRAN(s, esm_state_bearer_deactivated);
            break;
        default:
            ogs_error("Unknown message(type:%d)", message->esm.h.message_type);
            break;
        }
        break;
    case MME_EVT_ESM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3489:
            if (bearer->t3489.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3489)->max_count) {
                ogs_warn("Retransmission of IMSI[%s] failed. "
                        "Stop retransmission", mme_ue->imsi_bcd);
                OGS_FSM_TRAN(&bearer->sm, &esm_state_exception);

                ogs_assert(OGS_OK ==
                    nas_eps_send_pdn_connectivity_reject(sess,
                        ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED,
                        e->create_action));
            } else {
                rv = nas_eps_send_esm_information_request(bearer);
                if (rv == OGS_OK) {
                    bearer->t3489.retry_count++;
                } else {
                    ogs_error("nas_eps_send_esm_information_request() failed");
                    OGS_FSM_TRAN(&bearer->sm, &esm_state_exception);
                }
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void esm_state_active(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    bearer = e->bearer;
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_ESM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->esm.h.message_type) {
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST:
            ogs_debug("PDN Connectivity request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            rv = esm_handle_pdn_connectivity_request(
                    bearer, &message->esm.pdn_connectivity_request,
                    e->create_action);
            if (rv != OGS_OK) {
                OGS_FSM_TRAN(s, esm_state_exception);
                break;
            }

            OGS_FSM_TRAN(s, esm_state_inactive);
            break;
        case OGS_NAS_EPS_PDN_DISCONNECT_REQUEST:
            ogs_debug("PDN disconnect request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            if (MME_HAVE_SGW_S1U_PATH(sess)) {
                ogs_assert(OGS_OK ==
                    mme_gtp_send_delete_session_request(mme_ue->sgw_ue, sess,
                    OGS_GTP_DELETE_SEND_DEACTIVATE_BEARER_CONTEXT_REQUEST));
            } else {
                ogs_assert(OGS_OK ==
                    nas_eps_send_deactivate_bearer_context_request(bearer));
            }
            OGS_FSM_TRAN(s, esm_state_pdn_will_disconnect);
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("Modify EPS bearer context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);

            ogs_assert(OGS_OK ==
                mme_gtp_send_update_bearer_response(
                    bearer, OGS_GTP2_CAUSE_REQUEST_ACCEPTED));
            break;
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("Deactivate EPS bearer "
                    "context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            ogs_assert(OGS_OK ==
                mme_gtp_send_delete_bearer_response(
                    bearer, OGS_GTP2_CAUSE_REQUEST_ACCEPTED));
            OGS_FSM_TRAN(s, esm_state_bearer_deactivated);
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST:
            ogs_debug("Bearer resource allocation request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            esm_handle_bearer_resource_allocation_request(bearer, message);
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST:
            ogs_debug("Bearer resource modification request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            esm_handle_bearer_resource_modification_request(bearer, message);
            break;
        default:
            ogs_error("Unknown message(type:%d)", 
                    message->esm.h.message_type);
            break;
        }
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void esm_state_pdn_will_disconnect(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    bearer = e->bearer;
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_ESM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->esm.h.message_type) {
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("[D] Deactivate EPS bearer "
                    "context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            OGS_FSM_TRAN(s, esm_state_pdn_did_disconnect);
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST:
            ogs_debug("PDN Connectivity request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    mme_ue->imsi_bcd, sess->pti, bearer->ebi);
            rv = esm_handle_pdn_connectivity_request(
                    bearer, &message->esm.pdn_connectivity_request,
                    e->create_action);
            if (rv != OGS_OK) {
                OGS_FSM_TRAN(s, esm_state_exception);
                break;
            }

            OGS_FSM_TRAN(s, esm_state_inactive);
            break;
        default:
            ogs_error("Unknown message(type:%d)", 
                    message->esm.h.message_type);
            break;
        }
        break;

    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void esm_state_pdn_did_disconnect(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(e);
    mme_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void esm_state_bearer_deactivated(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(e);
    mme_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void esm_state_exception(ogs_fsm_t *s, mme_event_t *e)
{
    mme_bearer_t *bearer = NULL;
    ogs_assert(e);
    mme_sm_debug(e);

    bearer = e->bearer;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_BEARER_ALL_TIMERS(bearer);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}
