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

#include "ngap-handler.h"
#include "gmm-handler.h"
#include "gmm-build.h"
#if 0
#include "gsm-handler.h"
#endif
#include "nas-path.h"
#include "nas-security.h"
#include "ngap-path.h"
#if 0
#include "sgsap-types.h"
#include "sgsap-path.h"
#include "amf-path.h"
#endif
#include "nausf-handler.h"
#include "nsmf-handler.h"
#include "sbi-path.h"
#include "amf-sm.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gmm_log_domain

void gmm_state_initial(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);

    OGS_FSM_TRAN(s, &gmm_state_de_registered);
}

void gmm_state_final(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);
}

static void common_register_state(ogs_fsm_t *s, amf_event_t *e);

void gmm_state_de_registered(ogs_fsm_t *s, amf_event_t *e)
{
    amf_ue_t *amf_ue = NULL;
    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        break;
    }

    common_register_state(s, e);
}

void gmm_state_registered(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    common_register_state(s, e);
}

static void common_register_state(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;
#if 0
    NGAP_ProcedureCode_t procedureCode;
#endif

    ogs_sbi_response_t *sbi_response = NULL;
    ogs_sbi_message_t *sbi_message = NULL;
    
    ogs_assert(e);
        
    if (e->sess) {
        sess = e->sess;
        amf_ue = sess->amf_ue;
        ogs_assert(amf_ue);
    } else {
        amf_ue = e->amf_ue;
        ogs_assert(amf_ue);
    }

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        return;
    case OGS_FSM_EXIT_SIG:
        return;
    case AMF_EVT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        ran_ue = amf_ue->ran_ue;
        ogs_assert(ran_ue);

#if 0
        if (nas_message->gmm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("Service request");
            rv = gmm_handle_service_request(
                    amf_ue, &nas_message->gmm.service_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_service_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!AMF_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("Service request : Unknown UE");
                nas_5gs_send_service_reject(amf_ue,
                    GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_warn("No Security Context : IMSI[%s]", amf_ue->imsi_bcd);
                nas_5gs_send_service_reject(amf_ue,
                    GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
                ogs_warn("No Session Context : IMSI[%s]", amf_ue->imsi_bcd);
                nas_5gs_send_service_reject(amf_ue,
                    GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_initial_context_setup_request(amf_ue);
            return;
        }
#endif

        switch (nas_message->gmm.h.message_type) {
#if 0
        case OGS_NAS_5GS_IDENTITY_RESPONSE:
            ogs_debug("Identity response");
            CLEAR_AMF_UE_TIMER(amf_ue->t3570);

            rv = gmm_handle_identity_response(amf_ue,
                    &nas_message->gmm.identity_response);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_identity_response() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!AMF_UE_HAVE_IMSI(amf_ue)) {
                ogs_error("No IMSI");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            break;
#endif
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_debug("Registration request");
            rv = gmm_handle_registration_request(
                    amf_ue, &nas_message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }
            break;
#if 0
        case OGS_NAS_5GS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("Tracking area update request");
            rv = gmm_handle_tau_request(
                    amf_ue, &nas_message->gmm.tracking_area_update_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_tau_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!AMF_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("TAU request : Unknown UE");
                nas_5gs_send_tau_reject(amf_ue,
                    GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            break;
        case OGS_NAS_5GS_TRACKING_AREA_UPDATE_COMPLETE:
            ogs_debug("Tracking area update complete");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            return;
        case OGS_NAS_5GS_EXTENDED_SERVICE_REQUEST:
            ogs_debug("Extended service request");
            rv = gmm_handle_extended_service_request(
                    amf_ue, &nas_message->gmm.extended_service_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_extended_service_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!AMF_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("Extended Service request : Unknown UE");
                nas_5gs_send_service_reject(amf_ue,
                    GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            break;
#endif
        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]", amf_ue->suci,
                    nas_message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
#if 0
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            ogs_debug("Deregistration request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            if (rv != OGS_OK)
            {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (AMF_P_TMSI_IS_AVAILABLE(amf_ue)) {
                sgsap_send_deregistration_indication(amf_ue);
            } else {
                amf_send_delete_session_or_deregistration(amf_ue);
            }

            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            return;
#endif
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMPLETE:
            ogs_debug("[%s] Configuration update complete", amf_ue->supi);

            CLEAR_AMF_UE_TIMER(amf_ue->t3555);
            return;

        case OGS_NAS_5GS_UL_NAS_TRANSPORT:
            rv = gmm_handle_ul_nas_transport(
                    amf_ue, &nas_message->gmm.ul_nas_transport);
            if (rv != OGS_OK) {
                nas_5gs_send_gmm_status(
                        amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION);
            }
            return;

        default:
            ogs_error("Unknown message[%d]", nas_message->gmm.h.message_type);
            return;
        }
        break;

    case AMF_EVT_SBI_CLIENT:
        sbi_response = e->sbi.response;
        ogs_assert(sbi_response);
        sbi_message = e->sbi.message;
        ogs_assert(sbi_message);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)

            ogs_assert(sess);
            ogs_timer_stop(sess->sbi.client_wait.timer);

            SWITCH(sbi_message->h.resource.component[2])
            CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                amf_nsmf_pdu_session_handle_update_sm_context(
                        sess, sbi_message);
                break;
            CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                ogs_fatal("asdfasdfasdf");

                break;
            DEFAULT
                amf_nsmf_pdu_session_handle_create_sm_context(
                        sess, sbi_message);
            END
            break;

        DEFAULT
            ogs_error("Invalid service name [%s]", sbi_message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    case AMF_EVT_5GMM_TIMER:
        switch (e->timer_id) {
        case AMF_TIMER_T3513:
            if (amf_ue->t3513.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3513)->max_count) {
                /* Paging failed */
                ogs_warn("[%s] Paging failed. Stop", amf_ue->supi);
                CLEAR_AMF_UE_TIMER(amf_ue->t3513);

            } else {
                amf_ue->t3513.retry_count++;
                /*
                 * If t3513 is timeout, the saved pkbuf is used.
                 * We don't have to set CNDomain.
                 * So, we just set CNDomain to 0
                 */
#if 0
                ngap_send_paging(amf_ue, 0);
#endif
            }
            break;
        case AMF_TIMER_T3555:
            if (amf_ue->t3555.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3555)->max_count) {
                /* Configuration update command failed */
                ogs_warn("[%s] Configuration update failed. Stop",
                        amf_ue->supi);
                CLEAR_AMF_UE_TIMER(amf_ue->t3555);

            } else {
                amf_ue->t3555.retry_count++;

                /*
                 * If t3555 is timeout, the saved pkbuf is used.
                 * In this case, ack should be set to 1 for timer expiration
                 */
                nas_5gs_send_configuration_update_command(amf_ue, 1, 0);
            }
            break;
        case AMF_TIMER_T3570:
            if (amf_ue->t3570.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3570)->max_count) {
                ogs_warn("Retransmission of Identity-Request failed. "
                        "Stop retransmission");
                CLEAR_AMF_UE_TIMER(amf_ue->t3570);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3570.retry_count++;
#if 0
                nas_5gs_send_identity_request(amf_ue);
#endif
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        return;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        return;
    }

#if 0
    if (!AMF_UE_HAVE_IMSI(amf_ue)) {
        CLEAR_AMF_UE_TIMER(amf_ue->t3570);
        nas_5gs_send_identity_request(amf_ue);
        return;
    }
#endif

    ran_ue = amf_ue->ran_ue;
    ogs_assert(ran_ue);

    switch (amf_ue->nas.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
#if 0
            rv = nas_5gs_send_emm_to_esm(amf_ue,
                    &amf_ue->pdn_connectivity_request);
            if (rv != OGS_OK) {
                ogs_error("nas_5gs_send_emm_to_esm() failed");
                nas_5gs_send_attach_reject(amf_ue,
                    GMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                    GSM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
            } else {
                OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
            }
#endif
        } else {
#if 0
            if (SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
                amf_gtp_send_delete_all_sessions(amf_ue);
            } else {
                amf_sbi_send_authenticate(amf_ue);
            }
#else
            amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue, NULL,
                    amf_nausf_auth_build_authenticate);
#endif
            OGS_FSM_TRAN(s, &gmm_state_authentication);
        }
        break;
#if 0
    case AMF_EPS_TYPE_TAU_REQUEST:
        procedureCode = e->ngap_code;

        if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No PDN Connection : UE[%s]", amf_ue->imsi_bcd);
            nas_5gs_send_tau_reject(amf_ue,
                GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
            amf_s6a_send_air(amf_ue, NULL);
            OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_authentication);
            break;
        }

        if (procedureCode == NGAP_ProcedureCode_id_initialUEMessage) {
            ogs_debug("    Iniital UE Message");
            if (amf_ue->nas_5gs.update.active_flag) {
                nas_5gs_send_tau_accept(amf_ue,
                        NGAP_ProcedureCode_id_InitialContextSetup);
            } else {
                nas_5gs_send_tau_accept(amf_ue,
                        NGAP_ProcedureCode_id_downlinkNASTransport);
                amf_send_release_access_bearer_or_ue_context_release(ran_ue);
            }
        } else if (procedureCode == NGAP_ProcedureCode_id_uplinkNASTransport) {
            ogs_debug("    Uplink NAS Transport");
            nas_5gs_send_tau_accept(amf_ue,
                    NGAP_ProcedureCode_id_downlinkNASTransport);
        } else {
            ogs_fatal("Invalid Procedure Code[%d]", (int)procedureCode);
        }
        break;
    case AMF_EPS_TYPE_EXTENDED_SERVICE_REQUEST:
        procedureCode = e->ngap_code;

        if (!AMF_P_TMSI_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No P-TMSI : UE[%s]", amf_ue->imsi_bcd);
            nas_5gs_send_service_reject(amf_ue,
                GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No PDN Connection : UE[%s]", amf_ue->imsi_bcd);
            nas_5gs_send_service_reject(amf_ue,
                GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
            ogs_warn("No Security Context : IMSI[%s]", amf_ue->imsi_bcd);
            nas_5gs_send_service_reject(amf_ue,
                GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
        }

        if (procedureCode == NGAP_ProcedureCode_id_initialUEMessage) {
            ogs_debug("    Initial UE Message");
            if (amf_ue->nas_5gs.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE ||
                amf_ue->nas_5gs.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE) {
                ogs_debug("    MO-CSFB-INDICATION[%d]",
                        amf_ue->nas_5gs.service.value);
                sgsap_send_mo_csfb_indication(amf_ue);
            } else if (amf_ue->nas_5gs.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE) {
                ogs_debug("    SERVICE_REQUEST[%d]",
                        amf_ue->nas_5gs.service.value);
                sgsap_send_service_request(amf_ue, SGSAP_EMM_IDLE_MODE);
            } else {
                ogs_warn(" Unknown CSFB Service Type[%d]",
                        amf_ue->nas_5gs.service.value);
                nas_5gs_send_service_reject(amf_ue,
                    GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_initial_context_setup_request(amf_ue);

        } else if (procedureCode == NGAP_ProcedureCode_id_uplinkNASTransport) {
            ogs_debug("    Uplink NAS Transport");
            if (amf_ue->nas_5gs.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE ||
                amf_ue->nas_5gs.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE) {
                ogs_debug("    MO-CSFB-INDICATION[%d]",
                        amf_ue->nas_5gs.service.value);
                sgsap_send_mo_csfb_indication(amf_ue);
            } else if (amf_ue->nas_5gs.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE) {
                ogs_debug("    SERVICE_REQUEST[%d]",
                        amf_ue->nas_5gs.service.value);
                sgsap_send_service_request(amf_ue, SGSAP_EMM_CONNECTED_MODE);
            } else {
                ogs_warn(" Unknown CSFB Service Type[%d]",
                        amf_ue->nas_5gs.service.value);
                nas_5gs_send_service_reject(amf_ue,
                    GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_ue_context_modification_request(amf_ue);
        } else {
            ogs_fatal("Invalid Procedure Code[%d]", (int)procedureCode);
        }

        break;
#endif
    default:
        ogs_fatal("Invalid NAS-5GS[%d]", amf_ue->nas.message_type);
        break;
    }
}

void gmm_state_authentication(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_sbi_response_t *sbi_response = NULL;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_assert(s);
    ogs_assert(e);
    
    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case AMF_EVT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_AUTHENTICATION_RESPONSE:
            rv = gmm_handle_authentication_response(
                    amf_ue, &nas_message->gmm.authentication_response);

            if (rv != OGS_OK) {
                nas_5gs_send_authentication_reject(amf_ue);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            }
            break;

        case OGS_NAS_5GS_AUTHENTICATION_FAILURE:
        {
            ogs_fatal("[%s] Not implemented", amf_ue->suci);
            ogs_assert_if_reached();
#if 0
            ogs_nas_5gs_authentication_failure_t *authentication_failure =
                &nas_message->gmm.authentication_failure;
            ogs_nas_authentication_failure_parameter_t
                *authentication_failure_parameter = 
                    &authentication_failure->
                        authentication_failure_parameter;

            ogs_debug("Authentication failure");
            ogs_debug("    IMSI[%s] GMM_CAUSE[%d]", amf_ue->imsi_bcd,
                    authentication_failure->gmm_cause);

            CLEAR_AMF_UE_TIMER(amf_ue->t3560);

            switch (authentication_failure->gmm_cause) {
            case GMM_CAUSE_MAC_FAILURE:
                ogs_warn("Authentication failure(MAC failure)");
                break;
            case GMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE:
                ogs_error("Authentication failure"
                        "(Non-EPS authentication unacceptable)");
                break;
            case GMM_CAUSE_SYNCH_FAILURE:
                ogs_warn("Authentication failure(Synch failure)");
                amf_s6a_send_air(amf_ue,
                        authentication_failure_parameter);
                return;
            default:
                ogs_error("Unknown GMM_CAUSE{%d] in Authentication"
                        " failure",
                        authentication_failure->gmm_cause);
                break;
            }

            nas_5gs_send_authentication_reject(amf_ue);
            OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
#endif

            break;
        }
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_warn("Registration request");
            rv = gmm_handle_registration_request(
                    amf_ue, &nas_message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] Cannot handle NAS message", amf_ue->suci);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue, NULL,
                    amf_nausf_auth_build_authenticate);
            break;

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->suci, nas_message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            ogs_debug("[%s] Deregistration request", amf_ue->suci);
#if 0
            rv = gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_send_delete_session_or_deregistration(amf_ue);
#endif
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", nas_message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVT_5GMM_TIMER:
        switch (e->timer_id) {
        case AMF_TIMER_T3560:
            if (amf_ue->t3560.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3560)->max_count) {
                ogs_warn("[%s] Retransmission failed. Stop retransmission",
                        amf_ue->suci);
                nas_5gs_send_authentication_reject(amf_ue);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3560.retry_count++;
                nas_5gs_send_authentication_request(amf_ue);
            }
            break;
        default:
            ogs_error("[%s] Unknown timer[%s:%d]", amf_ue->suci,
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    case AMF_EVT_SBI_CLIENT:
        sbi_response = e->sbi.response;
        ogs_assert(sbi_response);
        sbi_message = e->sbi.message;
        ogs_assert(sbi_message);

        SWITCH(sbi_message->h.resource.component[0])
        CASE(OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS)
            ogs_timer_stop(amf_ue->sbi.client_wait.timer);

            if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                ogs_error("[%s] HTTP response error [%d]",
                        amf_ue->suci, sbi_message->res_status);
                nas_5gs_send_authentication_reject(amf_ue);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                break;
            }

            SWITCH(sbi_message->h.method)
            CASE(OGS_SBI_HTTP_METHOD_POST)
                rv = amf_nausf_auth_handle_authenticate(amf_ue, sbi_message);
                if (rv != OGS_OK) {
                    ogs_error("[%s] Cannot handle SBI message", amf_ue->suci);
                    nas_5gs_send_authentication_reject(amf_ue);
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                }
                break;
            CASE(OGS_SBI_HTTP_METHOD_PUT)
                rv = amf_nausf_auth_handle_authenticate_confirmation(
                        amf_ue, sbi_message);
                if (rv != OGS_OK) {
                    ogs_error("[%s] Cannot handle SBI message", amf_ue->suci);
                    nas_5gs_send_authentication_reject(amf_ue);
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                } else {
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_security_mode);
                }
                break;
            DEFAULT
                ogs_error("[%s] Invalid HTTP method [%s]",
                        amf_ue->suci, sbi_message->h.method);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("Invalid resource name [%s]",
                    sbi_message->h.resource.component[0]);
            ogs_assert_if_reached();
        END
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}

void gmm_state_security_mode(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_AMF_UE_TIMER(amf_ue->t3560);
        nas_5gs_send_security_mode_command(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case AMF_EVT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

#if 0
        if (nas_message->gmm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("Service request");
            nas_5gs_send_service_reject(amf_ue,
                    GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
        }
#endif

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_SECURITY_MODE_COMPLETE:
            ogs_debug("[%s] Security mode complete", amf_ue->supi);

            CLEAR_AMF_UE_TIMER(amf_ue->t3560);

            /* Now, We will check the MAC in the NAS message*/
            h.type = e->nas.type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] Security-mode : No Integrity Protected",
                        amf_ue->supi);

                nas_5gs_send_registration_reject(amf_ue,
                    OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_warn("[%s] No Security Context", amf_ue->supi);
                nas_5gs_send_registration_reject(amf_ue,
                    OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }

            gmm_handle_security_mode_complete(
                    amf_ue, &nas_message->gmm.security_mode_complete);

            ogs_kdf_kgnb_and_kn3iwf(
                    amf_ue->kamf, amf_ue->ul_count.i32,
                    amf_ue->nas.access_type, amf_ue->kgnb);
            ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->kgnb, amf_ue->nh);
            amf_ue->nhcc = 1;

            amf_ue_sbi_discover_and_send(OpenAPI_nf_type_UDM, amf_ue, NULL,
                    amf_nudm_uecm_build_registration);

            if (amf_ue->nas.message_type == OGS_NAS_5GS_REGISTRATION_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
#if 0
            } else if (amf_ue->nas.message_type ==
                    AMF_EPS_TYPE_SERVICE_REQUEST ||
                    amf_ue->nas.message_type == AMF_EPS_TYPE_TAU_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_registered);
            } else {
                ogs_fatal("Invalid OGS_NAS_5GS[%d]", amf_ue->nas.message_type);
#endif
            }
            break;
        case OGS_NAS_5GS_SECURITY_MODE_REJECT:
            ogs_warn("[%s] Security mode reject : Cause[%d]",
                    amf_ue->supi,
                    nas_message->gmm.security_mode_reject.gmm_cause);
            CLEAR_AMF_UE_TIMER(amf_ue->t3560);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_warn("Registration request");
            rv = gmm_handle_registration_request(
                    amf_ue, &nas_message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] Cannot handle NAS message", amf_ue->suci);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue, NULL,
                    amf_nausf_auth_build_authenticate);

            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;
#if 0
        case OGS_NAS_5GS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("Tracking area update request");
            nas_5gs_send_tau_reject(amf_ue,
                GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
#endif

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->supi, nas_message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
#if 0
            ogs_debug("Deregistration request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_send_delete_session_or_deregistration(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
#endif
            break;
        default:
            ogs_warn("Unknown message[%d]", nas_message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVT_5GMM_TIMER:
        switch (e->timer_id) {
        case AMF_TIMER_T3560:
            if (amf_ue->t3560.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3560)->max_count) {
                ogs_warn("[%s] Retransmission failed. Stop", amf_ue->supi);
                nas_5gs_send_registration_reject(amf_ue,
                    OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3560.retry_count++;
                nas_5gs_send_security_mode_command(amf_ue);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}

void gmm_state_initial_context_setup(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_sbi_response_t *sbi_response = NULL;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case AMF_EVT_SBI_CLIENT:
        sbi_response = e->sbi.response;
        ogs_assert(sbi_response);
        sbi_message = e->sbi.message;
        ogs_assert(sbi_message);

        ogs_timer_stop(amf_ue->sbi.client_wait.timer);

        SWITCH(sbi_message->h.resource.component[1])
        CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
            if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                ogs_error("[%s] HTTP response error [%d]",
                        amf_ue->supi, sbi_message->res_status);
                nas_5gs_send_registration_reject(
                        amf_ue, OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                break;
            }

            SWITCH(sbi_message->h.method)
            CASE(OGS_SBI_HTTP_METHOD_PUT)
                amf_ue_sbi_discover_and_send(OpenAPI_nf_type_UDM, amf_ue,
                        (char *)OGS_SBI_RESOURCE_NAME_AM_DATA,
                        amf_nudm_sdm_build_get);
                break;

            DEFAULT
                ogs_error("[%s] Invalid HTTP method [%s]",
                        amf_ue->suci, sbi_message->h.method);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
        CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
        CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
            if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                ogs_error("[%s] HTTP response error [%d]",
                        amf_ue->supi, sbi_message->res_status);
                nas_5gs_send_registration_reject(
                        amf_ue, OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                break;
            }

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                if (sbi_message->AccessAndMobilitySubscriptionData) {
                    OpenAPI_ambr_rm_t *subscribed_ue_ambr =
                        sbi_message->AccessAndMobilitySubscriptionData->
                            subscribed_ue_ambr;
                    if (subscribed_ue_ambr) {
                        amf_ue->subscribed_ue_ambr.uplink =
                            ogs_sbi_bitrate_from_string(
                                    subscribed_ue_ambr->uplink);
                        amf_ue->subscribed_ue_ambr.downlink =
                            ogs_sbi_bitrate_from_string(
                                    subscribed_ue_ambr->downlink);
                    }
                }

                amf_ue_sbi_discover_and_send(OpenAPI_nf_type_UDM, amf_ue,
                        (char *)OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA,
                        amf_nudm_sdm_build_get);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
                amf_ue_sbi_discover_and_send(OpenAPI_nf_type_UDM, amf_ue,
                        (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA,
                        amf_nudm_sdm_build_get);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                nas_5gs_send_registration_accept(amf_ue);
                break;

            DEFAULT
            END

            break;

        DEFAULT
            ogs_error("Invalid resource name [%s]",
                    sbi_message->h.resource.component[0]);
            ogs_assert_if_reached();
        END
        break;
    case AMF_EVT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_COMPLETE:
            ogs_debug("[%s] Registration complete", amf_ue->supi);

            /* Ack/Red to 0
             *  - No need to receive configuration update complete */
            nas_5gs_send_configuration_update_command(amf_ue, 0, 0);

            OGS_FSM_TRAN(s, &gmm_state_registered);
            break;

        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_warn("Registration request");
            rv = gmm_handle_registration_request(
                    amf_ue, &nas_message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] Cannot handle NAS message", amf_ue->suci);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

#if 0
            amf_gtp_send_delete_all_sessions(amf_ue);
#endif
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->supi, nas_message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;

#if 0
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            ogs_debug("Deregistration request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_send_delete_session_or_deregistration(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;
#endif
        default:
            ogs_warn("Unknown message[%d]", nas_message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVT_5GMM_TIMER:
        switch (e->timer_id) {
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}

void gmm_state_exception(ogs_fsm_t *s, amf_event_t *e)
{
    amf_ue_t *amf_ue = NULL;

    ogs_assert(e);
    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}
