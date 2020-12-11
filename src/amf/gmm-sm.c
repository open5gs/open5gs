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
#include "nas-path.h"
#include "nas-security.h"
#include "ngap-path.h"
#include "nausf-handler.h"
#include "nsmf-handler.h"
#include "nudm-handler.h"
#include "npcf-handler.h"
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
    int rv, xact_count = 0;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;
    ogs_nas_security_header_type_t h;

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
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case AMF_EVT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        ran_ue = ran_ue_cycle(amf_ue->ran_ue);
        ogs_assert(ran_ue);

        h.type = e->nas.type;
        amf_ue->nas.ngapProcedureCode = e->ngap.code;

        xact_count = amf_sess_xact_count(amf_ue);

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            rv = gmm_handle_registration_request(
                    amf_ue, &nas_message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                CLEAR_AMF_UE_TIMER(amf_ue->t3570);
                nas_5gs_send_identity_request(amf_ue);
                break;
            }

            if (h.integrity_protected && SECURITY_CONTEXT_IS_VALID(amf_ue)) {

                rv = gmm_handle_registration_update(
                        amf_ue, &nas_message->gmm.registration_request);
                if (rv != OGS_OK) {
                    ogs_error("gmm_handle_registration_update() failed");
                    OGS_FSM_TRAN(s, gmm_state_exception);
                    break;
                }

                if (amf_sess_xact_count(amf_ue) == xact_count)
                    nas_5gs_send_registration_accept(amf_ue);

                OGS_FSM_TRAN(s, &gmm_state_registered);

            } else {

                amf_sbi_send_release_all_sessions(
                        amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);
                if (amf_sess_xact_count(amf_ue) == xact_count) {
                    amf_ue_sbi_discover_and_send(
                            OpenAPI_nf_type_AUSF, amf_ue, NULL,
                            amf_nausf_auth_build_authenticate);
                }

                OGS_FSM_TRAN(s, &gmm_state_authentication);
            }
            break;

        case OGS_NAS_5GS_SERVICE_REQUEST:
            rv = gmm_handle_service_request(
                    amf_ue, &nas_message->gmm.service_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_service_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                ogs_error("Service request : Unknown UE");
                nas_5gs_send_service_reject(amf_ue,
                OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!h.integrity_protected || !SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_error("No Security Context");
                nas_5gs_send_service_reject(amf_ue,
                OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            rv = gmm_handle_service_update(
                    amf_ue, &nas_message->gmm.service_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_service_update() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
            }

            OGS_FSM_TRAN(s, gmm_state_registered);
            break;

        case OGS_NAS_5GS_IDENTITY_RESPONSE:
            CLEAR_AMF_UE_TIMER(amf_ue->t3570);

            rv = gmm_handle_identity_response(amf_ue,
                    &nas_message->gmm.identity_response);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_identity_response() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                ogs_error("No SUCI");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_sbi_send_release_all_sessions(
                        amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);
            if (amf_sess_xact_count(amf_ue) == xact_count) {
                amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue, NULL,
                        amf_nausf_auth_build_authenticate);
            }

            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;

#if 0
        case OGS_NAS_5GS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_error("[%s] Tracking area update request",
                    amf_ue->supi);
            rv = gmm_handle_tau_request(
                    amf_ue, &nas_message->gmm.tracking_area_update_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_tau_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                ogs_warn("TAU request : Unknown UE");
                nas_5gs_send_tau_reject(amf_ue,
                    GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }
            break;
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
        case OGS_NAS_5GS_TRACKING_AREA_UPDATE_COMPLETE:
            ogs_error("[%s] Tracking area update complete", amf_ue->supi);
            break;
#endif
        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]", amf_ue->suci,
                    nas_message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;

        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMPLETE:
            ogs_debug("[%s] Configuration update complete", amf_ue->supi);

            CLEAR_AMF_UE_TIMER(amf_ue->t3555);
            break;

        case OGS_NAS_5GS_UL_NAS_TRANSPORT:
            if (!h.integrity_protected || !SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_error("No Security Context");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            gmm_handle_ul_nas_transport(
                    amf_ue, &nas_message->gmm.ul_nas_transport);
            break;

        default:
            ogs_error("Unknown message [%d]", nas_message->gmm.h.message_type);
        }
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
                nas_5gs_send_configuration_update_command(amf_ue, NULL);
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
                nas_5gs_send_identity_request(amf_ue);
            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
        }
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
    }
}

void gmm_state_authentication(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_nas_5gs_authentication_failure_t *authentication_failure = NULL;
    ogs_nas_authentication_failure_parameter_t
        *authentication_failure_parameter = NULL;

    ogs_sbi_response_t *sbi_response = NULL;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_assert(s);
    ogs_assert(e);
    
    amf_sm_debug(e);

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
            authentication_failure = &nas_message->gmm.authentication_failure;
            ogs_assert(authentication_failure);
            authentication_failure_parameter = &authentication_failure->
                authentication_failure_parameter;
            ogs_assert(authentication_failure_parameter);

            ogs_debug("[%s] Authentication failure [%d]", amf_ue->suci,
                    authentication_failure->gmm_cause);

            CLEAR_AMF_UE_TIMER(amf_ue->t3560);

            switch (authentication_failure->gmm_cause) {
            case OGS_5GMM_CAUSE_MAC_FAILURE:
                ogs_warn("Authentication failure(MAC failure)");
                break;

            case OGS_5GMM_CAUSE_NON_5G_AUTHENTICATION_UNACCEPTABLE:
                ogs_error("Authentication failure"
                        "(Non-5GS authentication unacceptable)");
                break;

            case OGS_5GMM_CAUSE_NGKSI_ALREADY_IN_USE:
                ogs_warn("Authentication failure(ngKSI already in use)");
                amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue,
                        authentication_failure_parameter->auts,
                        amf_nausf_auth_build_authenticate);
                return;

            case OGS_5GMM_CAUSE_SYNCH_FAILURE:
                ogs_warn("Authentication failure(Synch failure)");
                if (authentication_failure_parameter->length != OGS_AUTS_LEN) {
                    ogs_error("Invalid AUTS Length [%d]",
                            authentication_failure_parameter->length);
                    break;
                }
                amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue,
                        authentication_failure_parameter->auts,
                        amf_nausf_auth_build_authenticate);
                return;

            default:
                ogs_error("Unknown GMM_CAUSE{%d] in Authentication"
                        " failure",
                        authentication_failure->gmm_cause);
                break;
            }

            nas_5gs_send_authentication_reject(amf_ue);
            OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);

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
            break;

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->suci, nas_message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;

        default:
            ogs_error("Unknown message[%d]", nas_message->gmm.h.message_type);
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

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NAUSF_AUTH)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS)

                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                    if (sbi_message->res_status ==
                            OGS_SBI_HTTP_STATUS_NOT_FOUND) {
                        ogs_warn("[%s] Cannot find SUCI [%d]",
                            amf_ue->suci, sbi_message->res_status);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                            amf_ue->suci, sbi_message->res_status);
                    }
                    nas_5gs_send_gmm_reject_from_sbi(
                            amf_ue, sbi_message->res_status);
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                    break;
                }

                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    rv = amf_nausf_auth_handle_authenticate(
                            amf_ue, sbi_message);
                    if (rv != OGS_OK) {
                        ogs_error("[%s] Cannot handle SBI message",
                                amf_ue->suci);
                        nas_5gs_send_authentication_reject(amf_ue);
                        OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                    }
                    break;
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    rv = amf_nausf_auth_handle_authenticate_confirmation(
                            amf_ue, sbi_message);
                    if (rv != OGS_OK) {
                        ogs_error("[%s] Cannot handle SBI message",
                                amf_ue->suci);
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

        DEFAULT
            ogs_error("Invalid service name [%s]", sbi_message->h.service.name);
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

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_SECURITY_MODE_COMPLETE:
            ogs_debug("[%s] Security mode complete", amf_ue->supi);

            CLEAR_AMF_UE_TIMER(amf_ue->t3560);

            /* Now, We will check the MAC in the NAS message*/
            h.type = e->nas.type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] Security-mode : No Integrity Protected",
                        amf_ue->supi);

                nas_5gs_send_gmm_reject(amf_ue,
                    OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_warn("[%s] No Security Context", amf_ue->supi);
                nas_5gs_send_gmm_reject(amf_ue,
                    OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }

            rv = gmm_handle_security_mode_complete(
                    amf_ue, &nas_message->gmm.security_mode_complete);
            if (rv != OGS_OK) {
                ogs_error("[%s] Cannot handle NAS message", amf_ue->suci);
                nas_5gs_send_gmm_reject(amf_ue,
                    OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            ogs_kdf_kgnb_and_kn3iwf(
                    amf_ue->kamf, amf_ue->ul_count.i32,
                    amf_ue->nas.access_type, amf_ue->kgnb);
            ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->kgnb, amf_ue->nh);
            amf_ue->nhcc = 1;

            amf_ue_sbi_discover_and_send(OpenAPI_nf_type_UDM, amf_ue, NULL,
                    amf_nudm_uecm_build_registration);

            if (amf_ue->nas.message_type == OGS_NAS_5GS_REGISTRATION_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
            } else if (amf_ue->nas.message_type ==
                        OGS_NAS_5GS_SERVICE_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_registered);
#if 0
            } else if (amf_ue->nas.message_type ==
                        OGS_NAS_5GS_SERVICE_REQUEST ||
                            amf_ue->nas.message_type ==
                                AMF_EPS_TYPE_TAU_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_registered);
#endif
            } else {
                ogs_fatal("Invalid OGS_NAS_5GS[%d]", amf_ue->nas.message_type);
                ogs_assert_if_reached();
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
        case OGS_NAS_5GS_SERVICE_REQUEST:
            ogs_debug("[%s] Service request", amf_ue->supi);
            nas_5gs_send_service_reject(amf_ue,
                    OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &gmm_state_exception);
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
            gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;

        default:
            ogs_error("Unknown message[%d]", nas_message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVT_5GMM_TIMER:
        switch (e->timer_id) {
        case AMF_TIMER_T3560:
            if (amf_ue->t3560.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3560)->max_count) {
                ogs_warn("[%s] Retransmission failed. Stop", amf_ue->supi);
                nas_5gs_send_gmm_reject(amf_ue,
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
    int rv, xact_count = 0;
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_sbi_response_t *sbi_response = NULL;
    ogs_sbi_message_t *sbi_message = NULL;

    gmm_configuration_update_command_param_t param;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

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
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case AMF_EVT_SBI_CLIENT:
        sbi_response = e->sbi.response;
        ogs_assert(sbi_response);
        sbi_message = e->sbi.message;
        ogs_assert(sbi_message);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                    ogs_error("[%s] HTTP response error [%d]",
                            amf_ue->supi, sbi_message->res_status);
                    nas_5gs_send_gmm_reject(
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

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[1]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
            CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                    ogs_error("[%s] HTTP response error [%d]",
                            amf_ue->supi, sbi_message->res_status);
                    nas_5gs_send_gmm_reject(
                            amf_ue, OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                    break;
                }

                amf_nudm_sdm_handle_provisioned(amf_ue, sbi_message);
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[1]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_POLICIES)
                rv = amf_npcf_am_policy_control_handle_create(
                        amf_ue, sbi_message);
                if (rv != OGS_OK) {
                    ogs_error("amf_npcf_am_policy_control_handle_create() "
                            "failed");
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                    break;
                }

                /*
                 * Issues #553
                 *
                 * o Tester
                 * 1. UE registered to 5GS and can connect to internet.
                 * 2. Turn off the UE and turn on the UE immediately
                 * 3. UE send PDU session request message
                 *    without sending registration complete
                 *
                 * o Analysis Result
                 * 1. UE sends registration request with unknown GUTI
                 * 2. AMF send registration accept without GUTI
                 * 3. UE skips the registration complete
                 *
                 * So, if GUTI is not present,
                 * we need to move REGISTERED state.
                 */
                if (amf_ue->guti_present == 0)
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_registered);

                /*
                 * Do not use nas_5gs_send_registration_accept()
                 * instead of nas_5gs_send_accept() here.
                 *
                 * nas_5gs_send_service_accept() could be used later.
                 * The reason is why the design could be changed to handle this.
                 *
                 * So we'll use nas_5gs_send_accept() at this point.
                 */
                nas_5gs_send_accept(amf_ue);
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("Invalid service name [%s]", sbi_message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    case AMF_EVT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_COMPLETE:
            ogs_debug("[%s] Registration complete", amf_ue->supi);

            /*
             * TS24.501
             * 5.4.4.2 Generic UE configuration update procedure initiated
             *         by the network
             *
             * If an acknowledgement from the UE is requested, the AMF shall
             * indicate acknowledgement requested in the Configuration update
             * indication IE in the CONFIGURATION UPDATE COMMAND message and
             * shall start timer T3555. Acknowledgement shall be requested
             * for all parameters except when only NITZ is included.
             *
             * TS23.502
             * 4.2.4.2 UE Configuration Update procedure for access and
             * mobility management related parameters
             *
             * The AMF includes one or more of 5G-GUTI, TAI List,
             * Allowed NSSAI, Mapping Of Allowed NSSAI, Configured NSSAI
             * for the Serving PLMN, Mapping Of Configured NSSAI,
             * rejected S-NSSAIs, NITZ (Network Identity and Time Zone),
             * Mobility Restrictions parameters, LADN Information,
             * Operator-defined access category definitions or SMS Subscribed
             * Indication if the AMF wants to update these NAS parameters
             * without triggering a UE Registration procedure.
             */
            memset(&param, 0, sizeof(param));
            param.nitz = 1;
            nas_5gs_send_configuration_update_command(amf_ue, &param);

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

            amf_sbi_send_release_all_sessions(
                        amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);
            if (amf_sess_xact_count(amf_ue) == xact_count) {
                amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue, NULL,
                        amf_nausf_auth_build_authenticate);
            }
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->supi, nas_message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;

        default:
            ogs_error("Unknown message [%d]", nas_message->gmm.h.message_type);
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
    int rv, xact_count = 0;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(e);
    amf_sm_debug(e);

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
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

        amf_sbi_send_release_all_sessions(
                amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);

        if (ogs_list_count(&amf_ue->sess_list) == 0)
            ngap_send_amf_ue_context_release_command(amf_ue,
                    NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                    NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case AMF_EVT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        ran_ue = ran_ue_cycle(amf_ue->ran_ue);
        ogs_assert(ran_ue);

        h.type = e->nas.type;
        amf_ue->nas.ngapProcedureCode = e->ngap.code;

        xact_count = amf_sess_xact_count(amf_ue);

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            rv = gmm_handle_registration_request(
                    amf_ue, &nas_message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                CLEAR_AMF_UE_TIMER(amf_ue->t3570);
                nas_5gs_send_identity_request(amf_ue);
                break;
            }

            if (h.integrity_protected && SECURITY_CONTEXT_IS_VALID(amf_ue)) {

                rv = gmm_handle_registration_update(
                        amf_ue, &nas_message->gmm.registration_request);
                if (rv != OGS_OK) {
                    ogs_error("gmm_handle_registration_update() failed");
                    OGS_FSM_TRAN(s, gmm_state_exception);
                    break;
                }

                if (amf_sess_xact_count(amf_ue) == xact_count)
                    nas_5gs_send_registration_accept(amf_ue);

                OGS_FSM_TRAN(s, &gmm_state_registered);

            } else {

                amf_sbi_send_release_all_sessions(
                        amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);
                if (amf_sess_xact_count(amf_ue) == xact_count) {
                    amf_ue_sbi_discover_and_send(
                            OpenAPI_nf_type_AUSF, amf_ue, NULL,
                            amf_nausf_auth_build_authenticate);
                }

                OGS_FSM_TRAN(s, &gmm_state_authentication);
            }
            break;

        default:
            ogs_error("Unknown message [%d]", nas_message->gmm.h.message_type);
        }
        break;

    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
    }
}
