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
#include "s1ap-handler.h"
#include "mme-fd-path.h"
#include "emm-handler.h"
#include "emm-build.h"
#include "esm-handler.h"
#include "nas-path.h"
#include "nas-security.h"
#include "s1ap-path.h"
#include "sgsap-types.h"
#include "sgsap-path.h"
#include "mme-gtp-path.h"
#include "mme-path.h"
#include "mme-sm.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __emm_log_domain

void emm_state_initial(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);

    mme_sm_debug(e);

    OGS_FSM_TRAN(s, &emm_state_de_registered);
}

void emm_state_final(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);

    mme_sm_debug(e);
}

static void common_register_state(ogs_fsm_t *s, mme_event_t *e);

void emm_state_de_registered(ogs_fsm_t *s, mme_event_t *e)
{
    mme_ue_t *mme_ue = NULL;
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = e->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_SERVICE_INDICATOR(mme_ue);
        CLEAR_MME_UE_ALL_TIMERS(mme_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        break;
    }

    common_register_state(s, e);
}

void emm_state_registered(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    common_register_state(s, e);
}

static void common_register_state(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;
    
    ogs_assert(e);
        
    mme_ue = e->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = enb_ue_cycle(mme_ue->enb_ue);
        ogs_assert(enb_ue);

        h.type = e->nas_type;

        if (message->emm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("Service request");
            rv = emm_handle_service_request(
                    mme_ue, &message->emm.service_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_service_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_info("Service request : Unknown UE");
                nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_warn("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                ogs_warn("No Session Context : IMSI[%s]", mme_ue->imsi_bcd);
                nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!ACTIVE_EPS_BEARERS_IS_AVAIABLE(mme_ue)) {
                ogs_warn("No active EPS bearers : IMSI[%s]", mme_ue->imsi_bcd);
                nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            s1ap_send_initial_context_setup_request(mme_ue);
            OGS_FSM_TRAN(s, &emm_state_registered);
            break;
        }

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_IDENTITY_RESPONSE:
            ogs_debug("Identity response");
            CLEAR_MME_UE_TIMER(mme_ue->t3470);

            rv = emm_handle_identity_response(mme_ue,
                    &message->emm.identity_response);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_identity_response() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_error("No IMSI");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

            if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                mme_gtp_send_delete_all_sessions(mme_ue,
                        OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST);
            } else {
                mme_s6a_send_air(mme_ue, NULL);
            }
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;

        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_debug("Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                CLEAR_MME_UE_TIMER(mme_ue->t3470);
                nas_eps_send_identity_request(mme_ue);
                break;
            }

            if (h.integrity_protected && SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                    mme_gtp_send_delete_all_sessions(mme_ue,
                        OGS_GTP_DELETE_HANDLE_PDN_CONNECTIVITY_REQUEST);
                } else {
                    rv = nas_eps_send_emm_to_esm(mme_ue,
                            &mme_ue->pdn_connectivity_request);
                    if (rv != OGS_OK) {
                        ogs_error("nas_eps_send_emm_to_esm() failed");
                        nas_eps_send_attach_reject(mme_ue,
                            EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                            ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                        OGS_FSM_TRAN(s, &emm_state_exception);
                        break;
                    }
                }
                OGS_FSM_TRAN(s, &emm_state_initial_context_setup);
            } else {
                if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                    mme_gtp_send_delete_all_sessions(mme_ue,
                        OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST);
                } else {
                    mme_s6a_send_air(mme_ue, NULL);
                }
                OGS_FSM_TRAN(s, &emm_state_authentication);
            }
            break;

        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("Tracking area update request");
            rv = emm_handle_tau_request(mme_ue,
                    &message->emm.tracking_area_update_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_tau_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_info("TAU request : Unknown UE");
                nas_eps_send_tau_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                ogs_warn("No PDN Connection : UE[%s]", mme_ue->imsi_bcd);
                nas_eps_send_tau_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!ACTIVE_EPS_BEARERS_IS_AVAIABLE(mme_ue)) {
                ogs_warn("No active EPS bearers : IMSI[%s]", mme_ue->imsi_bcd);
                nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                mme_s6a_send_air(mme_ue, NULL);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
                break;
            }

            if (e->s1ap_code == S1AP_ProcedureCode_id_initialUEMessage) {
                ogs_debug("    Iniital UE Message");
                if (mme_ue->nas_eps.update.active_flag) {
                    nas_eps_send_tau_accept(mme_ue,
                            S1AP_ProcedureCode_id_InitialContextSetup);
                } else {
                    nas_eps_send_tau_accept(mme_ue,
                            S1AP_ProcedureCode_id_downlinkNASTransport);
                    mme_send_release_access_bearer_or_ue_context_release(
                            enb_ue);
                }
            } else if (e->s1ap_code ==
                    S1AP_ProcedureCode_id_uplinkNASTransport) {
                ogs_debug("    Uplink NAS Transport");
                nas_eps_send_tau_accept(mme_ue,
                        S1AP_ProcedureCode_id_downlinkNASTransport);
            } else {
                ogs_fatal("Invalid Procedure Code[%d]", (int)e->s1ap_code);
            }
            OGS_FSM_TRAN(s, &emm_state_registered);

            break;

        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_COMPLETE:
            ogs_debug("Tracking area update complete");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            break;

        case OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST:
            ogs_debug("Extended service request");
            rv = emm_handle_extended_service_request(
                    mme_ue, &message->emm.extended_service_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_extended_service_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("Extended Service request : Unknown UE");
                nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                ogs_warn("No PDN Connection : UE[%s]", mme_ue->imsi_bcd);
                nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_warn("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (e->s1ap_code == S1AP_ProcedureCode_id_initialUEMessage) {
                ogs_debug("    Initial UE Message");

                if (!MME_P_TMSI_IS_AVAILABLE(mme_ue)) {
                    ogs_warn("No P-TMSI : UE[%s]", mme_ue->imsi_bcd);
                    nas_eps_send_service_reject(mme_ue,
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    mme_send_release_access_bearer_or_ue_context_release(
                            enb_ue);
                    break;
                }

                if (mme_ue->nas_eps.service.value ==
                        OGS_NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE ||
                    mme_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE) {
                    ogs_debug("    MO-CSFB-INDICATION[%d]",
                            mme_ue->nas_eps.service.value);
                    sgsap_send_mo_csfb_indication(mme_ue);
                } else if (mme_ue->nas_eps.service.value ==
                        OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE) {
                    ogs_debug("    SERVICE_REQUEST[%d]",
                            mme_ue->nas_eps.service.value);
                    sgsap_send_service_request(mme_ue, SGSAP_EMM_IDLE_MODE);
                } else {
                    ogs_warn(" Unknown CSFB Service Type[%d]",
                            mme_ue->nas_eps.service.value);
                    nas_eps_send_service_reject(mme_ue,
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    OGS_FSM_TRAN(s, &emm_state_exception);
                    break;
                }

                s1ap_send_initial_context_setup_request(mme_ue);

            } else if (e->s1ap_code ==
                    S1AP_ProcedureCode_id_uplinkNASTransport) {
                ogs_debug("    Uplink NAS Transport");

                if (!MME_P_TMSI_IS_AVAILABLE(mme_ue)) {
                    ogs_warn("No P-TMSI : UE[%s]", mme_ue->imsi_bcd);
                    nas_eps_send_service_reject(mme_ue,
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    break;
                }

                if (mme_ue->nas_eps.service.value ==
                        OGS_NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE ||
                    mme_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE) {
                    ogs_debug("    MO-CSFB-INDICATION[%d]",
                            mme_ue->nas_eps.service.value);
                    sgsap_send_mo_csfb_indication(mme_ue);
                } else if (mme_ue->nas_eps.service.value ==
                        OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE) {
                    ogs_debug("    SERVICE_REQUEST[%d]",
                            mme_ue->nas_eps.service.value);
                    sgsap_send_service_request(
                            mme_ue, SGSAP_EMM_CONNECTED_MODE);
                } else {
                    ogs_warn(" Unknown CSFB Service Type[%d]",
                            mme_ue->nas_eps.service.value);
                    nas_eps_send_service_reject(mme_ue,
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    OGS_FSM_TRAN(s, &emm_state_exception);
                    break;
                }

                s1ap_send_ue_context_modification_request(mme_ue);
            } else {
                ogs_error("Invalid Procedure Code[%d]", (int)e->s1ap_code);
            }
            break;

        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.emm_status.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;

        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_debug("Detach request");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_detach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (MME_P_TMSI_IS_AVAILABLE(mme_ue)) {
                sgsap_send_detach_indication(mme_ue);
            } else {
                mme_send_delete_session_or_detach(mme_ue);
            }

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;

        case OGS_NAS_EPS_UPLINK_NAS_TRANSPORT:
            ogs_debug("Uplink NAS Transport");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            if (MME_SGSAP_IS_CONNECTED(mme_ue)) {
                sgsap_send_uplink_unitdata(mme_ue, &message->emm.
                        uplink_nas_transport.nas_message_container);
            } else {
                S1AP_MME_UE_S1AP_ID_t MME_UE_S1AP_ID;
                S1AP_ENB_UE_S1AP_ID_t ENB_UE_S1AP_ID;

                ogs_warn("No connection of MSC/VLR");
                MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
                ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

                s1ap_send_error_indication(enb_ue->enb, 
                        &MME_UE_S1AP_ID, &ENB_UE_S1AP_ID,
                        S1AP_Cause_PR_transport,
                        S1AP_CauseTransport_transport_resource_unavailable);
            }
            break;

        case OGS_NAS_EPS_ATTACH_COMPLETE:
            ogs_warn("[%s] should not send Attach-Complete"
                    " if GUTI is not present in Attach-Accept",
                    mme_ue->imsi_bcd);
            break;

        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
        }
        break;

    case MME_EVT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3413:
            if (mme_ue->t3413.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3413)->max_count) {
                /* Paging failed */
                ogs_warn("Paging to IMSI[%s] failed. Stop paging",
                        mme_ue->imsi_bcd);
                CLEAR_MME_UE_TIMER(mme_ue->t3413);

                mme_send_after_paging(mme_ue, OGS_GTP_CAUSE_UNABLE_TO_PAGE_UE);

                if (CS_CALL_SERVICE_INDICATOR(mme_ue) ||
                    SMS_SERVICE_INDICATOR(mme_ue)) {
                    sgsap_send_ue_unreachable(mme_ue,
                            SGSAP_SGS_CAUSE_UE_UNREACHABLE);

                }
    
                CLEAR_SERVICE_INDICATOR(mme_ue);

            } else {
                mme_ue->t3413.retry_count++;
                /*
                 * If t3413 is timeout, the saved pkbuf is used.
                 * We don't have to set CNDomain.
                 * So, we just set CNDomain to 0
                 */
                s1ap_send_paging(mme_ue, 0);
            }
            break;

        case MME_TIMER_T3470:
            if (mme_ue->t3470.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3470)->max_count) {
                ogs_warn("Retransmission of Identity-Request failed. "
                        "Stop retransmission");
                CLEAR_MME_UE_TIMER(mme_ue->t3470);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            } else {
                mme_ue->t3470.retry_count++;
                nas_eps_send_identity_request(mme_ue);
            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    default:
        ogs_error("Unknown event[%s]", mme_event_get_name(e));
    }
}

void emm_state_authentication(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);
    
    mme_sm_debug(e);

    mme_ue = e->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_AUTHENTICATION_RESPONSE:
        {
            ogs_nas_eps_authentication_response_t *authentication_response =
                &message->emm.authentication_response;
            ogs_nas_authentication_response_parameter_t
                *authentication_response_parameter =
                    &authentication_response->
                        authentication_response_parameter;

            ogs_debug("Authentication response");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

            CLEAR_MME_UE_TIMER(mme_ue->t3460);

            if (authentication_response_parameter->length == 0 ||
                memcmp(authentication_response_parameter->res,
                mme_ue->xres,
                authentication_response_parameter->length) != 0) {
                ogs_log_hexdump(OGS_LOG_WARN,
                        authentication_response_parameter->res,
                        authentication_response_parameter->length);
                ogs_log_hexdump(OGS_LOG_WARN,
                        mme_ue->xres, OGS_MAX_RES_LEN);
                nas_eps_send_authentication_reject(mme_ue);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            } else {
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_security_mode);
            }

            break;
        }
        case OGS_NAS_EPS_AUTHENTICATION_FAILURE:
        {
            ogs_nas_eps_authentication_failure_t *authentication_failure =
                &message->emm.authentication_failure;
            ogs_nas_authentication_failure_parameter_t
                *authentication_failure_parameter = 
                    &authentication_failure->
                        authentication_failure_parameter;

            ogs_debug("Authentication failure");
            ogs_debug("    IMSI[%s] EMM_CAUSE[%d]", mme_ue->imsi_bcd,
                    authentication_failure->emm_cause);

            CLEAR_MME_UE_TIMER(mme_ue->t3460);

            switch (authentication_failure->emm_cause) {
            case EMM_CAUSE_MAC_FAILURE:
                ogs_warn("Authentication failure(MAC failure)");
                break;
            case EMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE:
                ogs_error("Authentication failure"
                        "(Non-EPS authentication unacceptable)");
                break;
            case EMM_CAUSE_SYNCH_FAILURE:
                ogs_info("Authentication failure(Synch failure)");
                mme_s6a_send_air(mme_ue,
                        authentication_failure_parameter);
                return;
            default:
                ogs_error("Unknown EMM_CAUSE{%d] in Authentication"
                        " failure",
                        authentication_failure->emm_cause);
                break;
            }

            nas_eps_send_authentication_reject(mme_ue);
            OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);

            break;
        }
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_s6a_send_air(mme_ue, NULL);
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.emm_status.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_debug("Detach request");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_detach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_send_delete_session_or_detach(mme_ue);
            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
            break;
        }
        break;
    case MME_EVT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3460:
            if (mme_ue->t3460.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3460)->max_count) {
                ogs_warn("Retransmission of IMSI[%s] failed. "
                        "Stop retransmission",
                        mme_ue->imsi_bcd);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);

                nas_eps_send_authentication_reject(mme_ue);
            } else {
                mme_ue->t3460.retry_count++;
                nas_eps_send_authentication_request(mme_ue);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", mme_event_get_name(e));
        break;
    }
}

void emm_state_security_mode(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = e->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_MME_UE_TIMER(mme_ue->t3460);
        nas_eps_send_security_mode_command(mme_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        if (message->emm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("Service request");
            nas_eps_send_service_reject(mme_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        }

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_SECURITY_MODE_COMPLETE:
            ogs_debug("Security mode complete");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

            CLEAR_MME_UE_TIMER(mme_ue->t3460);

            /* Now, We will check the MAC in the NAS message*/
            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] No Integrity Protected", mme_ue->imsi_bcd);

                nas_eps_send_attach_reject(mme_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_warn("[%s] No Security Context", mme_ue->imsi_bcd);
                nas_eps_send_attach_reject(mme_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            emm_handle_security_mode_complete(
                    mme_ue, &message->emm.security_mode_complete);

            ogs_kdf_kenb(mme_ue->kasme, mme_ue->ul_count.i32,
                    mme_ue->kenb);
            ogs_kdf_nh_enb(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
            mme_ue->nhcc = 1;

            mme_s6a_send_ulr(mme_ue);
            if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
                if (mme_ue->guti_present)
                    OGS_FSM_TRAN(s, &emm_state_initial_context_setup);
                else
                    OGS_FSM_TRAN(s, &emm_state_registered);
            } else if (mme_ue->nas_eps.type ==
                    MME_EPS_TYPE_SERVICE_REQUEST ||
                    mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST) {
                OGS_FSM_TRAN(s, &emm_state_registered);
            } else {
                ogs_fatal("Invalid OGS_NAS_EPS[%d]", mme_ue->nas_eps.type);
            }
            break;
        case OGS_NAS_EPS_SECURITY_MODE_REJECT:
            ogs_warn("Security mode reject : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.security_mode_reject.emm_cause);
            CLEAR_MME_UE_TIMER(mme_ue->t3460);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_s6a_send_air(mme_ue, NULL);
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("Tracking area update request");
            nas_eps_send_tau_reject(mme_ue,
                EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.emm_status.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_debug("Detach request");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_detach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_send_delete_session_or_detach(mme_ue);
            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
            break;
        }
        break;
    case MME_EVT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3460:
            if (mme_ue->t3460.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3460)->max_count) {
                ogs_warn("Retransmission of IMSI[%s] failed. "
                        "Stop retransmission",
                        mme_ue->imsi_bcd);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);

                nas_eps_send_attach_reject(mme_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
            } else {
                mme_ue->t3460.retry_count++;
                nas_eps_send_security_mode_command(mme_ue);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", mme_event_get_name(e));
        break;
    }
}

void emm_state_initial_context_setup(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = e->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_ATTACH_COMPLETE:
            ogs_debug("Attach complete");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] No Integrity Protected", mme_ue->imsi_bcd);

                nas_eps_send_attach_reject(mme_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_warn("[%s] No Security Context", mme_ue->imsi_bcd);
                nas_eps_send_attach_reject(mme_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            rv = emm_handle_attach_complete(
                    mme_ue, &message->emm.attach_complete);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_complete() failed "
                        "in emm_state_initial_context_setup");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }
            if (MME_P_TMSI_IS_AVAILABLE(mme_ue))
                sgsap_send_tmsi_reallocation_complete(mme_ue);

            OGS_FSM_TRAN(s, &emm_state_registered);
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_gtp_send_delete_all_sessions(mme_ue,
                OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST);
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.emm_status.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_debug("Detach request");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_detach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_send_delete_session_or_detach(mme_ue);
            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMPLETE:
            ogs_warn("[%s] Duplicated : Security mode complete",
                    mme_ue->imsi_bcd);
            break;
        default:
            ogs_warn("Unknown message[%d]", 
                    message->emm.h.message_type);
            break;
        }
        break;
    case MME_EVT_EMM_TIMER:
        switch (e->timer_id) {
        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", mme_event_get_name(e));
        break;
    }
}

void emm_state_exception(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(e);
    mme_sm_debug(e);

    mme_ue = e->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_SERVICE_INDICATOR(mme_ue);
        CLEAR_MME_UE_ALL_TIMERS(mme_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = enb_ue_cycle(mme_ue->enb_ue);
        ogs_assert(enb_ue);

        h.type = e->nas_type;

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_debug("Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                CLEAR_MME_UE_TIMER(mme_ue->t3470);
                nas_eps_send_identity_request(mme_ue);
                break;
            }

            if (h.integrity_protected && SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                    mme_gtp_send_delete_all_sessions(mme_ue,
                        OGS_GTP_DELETE_HANDLE_PDN_CONNECTIVITY_REQUEST);
                } else {
                    rv = nas_eps_send_emm_to_esm(mme_ue,
                            &mme_ue->pdn_connectivity_request);
                    if (rv != OGS_OK) {
                        ogs_error("nas_eps_send_emm_to_esm() failed");
                        nas_eps_send_attach_reject(mme_ue,
                            EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                            ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                        OGS_FSM_TRAN(s, &emm_state_exception);
                        break;
                    }
                }
                OGS_FSM_TRAN(s, &emm_state_initial_context_setup);
            } else {
                if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                    mme_gtp_send_delete_all_sessions(mme_ue,
                        OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST);
                } else {
                    mme_s6a_send_air(mme_ue, NULL);
                }
                OGS_FSM_TRAN(s, &emm_state_authentication);
            }
            break;

        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
        }
        break;

    default:
        ogs_error("Unknown event[%s]", mme_event_get_name(e));
    }
}
