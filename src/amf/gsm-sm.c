/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#if 0
#include "5gmm-handler.h"
#include "5gsm-build.h"
#include "5gsm-handler.h"
#include "nas-path.h"
#endif
#include "context.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gsm_log_domain

void gsm_state_initial(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);

    OGS_FSM_TRAN(s, &gsm_state_inactive);
}

void gsm_state_final(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);
}

void gsm_state_inactive(ogs_fsm_t *s, amf_event_t *e)
{
#if 0
    int rv;
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    amf_bearer_t *bearer = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    bearer = e->bearer;
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_BEARER_ALL_TIMERS(bearer);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_ESM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->gsm.h.message_type) {
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST:
            ogs_debug("[ESM] PDN Connectivity request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            rv = gsm_handle_pdn_connectivity_request(
                    bearer, &message->gsm.pdn_connectivity_request);
            if (rv != OGS_OK) {
                OGS_FSM_TRAN(s, gsm_state_exception);
                break;
            }
            break;
        case OGS_NAS_EPS_ESM_INFORMATION_RESPONSE:
            ogs_debug("[ESM] ESM information response");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);

            CLEAR_BEARER_TIMER(bearer->t3489);
            rv = gsm_handle_information_response(
                    sess, &message->gsm.esm_information_response);
            if (rv != OGS_OK) {
                OGS_FSM_TRAN(s, gsm_state_exception);
                break;
            }
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("[ESM] Activate default EPS bearer "
                    "context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            /* Check if Initial Context Setup Response or 
             *          E-RAB Setup Response is received */
            if (MME_HAVE_ENB_S1U_PATH(bearer)) {
                amf_gtp_send_modify_bearer_request(bearer, 0);
            }

            nas_eps_send_activate_all_dedicated_bearers(bearer);
            OGS_FSM_TRAN(s, gsm_state_active);
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("[ESM] Activate dedicated EPS bearer "
                    "context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            /* Check if Initial Context Setup Response or 
             *          E-RAB Setup Response is received */
            if (MME_HAVE_ENB_S1U_PATH(bearer)) {
                amf_gtp_send_create_bearer_response(bearer);
            }

            OGS_FSM_TRAN(s, gsm_state_active);
            break;
        default:
            ogs_error("Unknown message(type:%d)", 
                    message->gsm.h.message_type);
            break;
        }
        break;
    case MME_EVT_ESM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3489:
            if (bearer->t3489.retry_count >=
                    amf_timer_cfg(MME_TIMER_T3489)->max_count) {
                ogs_warn("[EMM] Retransmission of IMSI[%s] failed. "
                        "Stop retransmission",
                        amf_ue->imsi_bcd);
                OGS_FSM_TRAN(&bearer->sm, &gsm_state_exception);

                nas_eps_send_pdn_connectivity_reject(sess,
                    ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED);
            } else {
                bearer->t3489.retry_count++;
                nas_eps_send_esm_information_request(bearer);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
#endif
}

#if 0
void gsm_state_active(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    amf_bearer_t *bearer = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    bearer = e->bearer;
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_ESM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->gsm.h.message_type) {
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST:
            ogs_debug("[ESM] PDN Connectivity request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            rv = gsm_handle_pdn_connectivity_request(
                    bearer, &message->gsm.pdn_connectivity_request);
            if (rv != OGS_OK) {
                OGS_FSM_TRAN(s, gsm_state_exception);
                break;
            }

            OGS_FSM_TRAN(s, gsm_state_inactive);
            break;
        case OGS_NAS_EPS_PDN_DISCONNECT_REQUEST:
            ogs_debug("[ESM] PDN disconnect request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            if (MME_HAVE_SGW_S1U_PATH(sess)) {
                amf_gtp_send_delete_session_request(sess);
            } else {
                nas_eps_send_deactivate_bearer_context_request(bearer);
            }
            OGS_FSM_TRAN(s, gsm_state_pdn_will_disconnect);
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("[ESM] Modify EPS bearer context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);

            amf_gtp_send_update_bearer_response(bearer);
            break;
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("[ESM] Deactivate EPS bearer "
                    "context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            amf_gtp_send_delete_bearer_response(bearer);
            OGS_FSM_TRAN(s, gsm_state_bearer_deactivated);
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST:
            ogs_debug("[ESM] Bearer resource allocation request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            gsm_handle_bearer_resource_allocation_request(bearer, message);
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST:
            ogs_debug("[ESM] Bearer resource modification request");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            gsm_handle_bearer_resource_modification_request(bearer, message);
            break;
        default:
            ogs_error("Unknown message(type:%d)", 
                    message->gsm.h.message_type);
            break;
        }
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}

void gsm_state_pdn_will_disconnect(ogs_fsm_t *s, amf_event_t *e)
{
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    amf_bearer_t *bearer = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    bearer = e->bearer;
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_ESM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->gsm.h.message_type) {
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
            ogs_debug("[ESM] [D] Deactivate EPS bearer "
                    "context accept");
            ogs_debug("    IMSI[%s] PTI[%d] EBI[%d]",
                    amf_ue->imsi_bcd, sess->pti, bearer->ebi);
            OGS_FSM_TRAN(s, gsm_state_pdn_did_disconnect);
            break;
        default:
            ogs_error("Unknown message(type:%d)", 
                    message->gsm.h.message_type);
            break;
        }
        break;

    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}

void gsm_state_pdn_did_disconnect(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(e);
    amf_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}

void gsm_state_bearer_deactivated(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(e);
    amf_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}
#endif

void gsm_state_exception(ogs_fsm_t *s, amf_event_t *e)
{
    amf_bearer_t *bearer = NULL;
    ogs_assert(e);
    amf_sm_debug(e);

    bearer = e->bearer;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_BEARER_ALL_TIMERS(bearer);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}
