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

#include "nas/nas-message.h"
#include "fd/fd-lib.h"
#include "fd/s6a/s6a-message.h"

#include "mme-event.h"
#include "mme-kdf.h"
#include "s1ap-handler.h"
#include "mme-fd-path.h"
#include "emm-handler.h"
#include "emm-build.h"
#include "esm-handler.h"
#include "nas-path.h"
#include "s1ap-path.h"
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
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

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
    nas_message_t *message = NULL;
    S1AP_ProcedureCode_t procedureCode;
    
    ogs_assert(e);
        
    mme_ue = e->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        return;
    case OGS_FSM_EXIT_SIG:
        return;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = mme_ue->enb_ue;
        ogs_assert(enb_ue);

        if (message->emm.h.security_header_type
                == NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("[EMM] Service request");
            rv = emm_handle_service_request(
                    mme_ue, &message->emm.service_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_service_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("[EMM] Service request : Unknown UE");
                rv = nas_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_assert(rv == OGS_OK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                return;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_warn("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                rv = nas_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_assert(rv == OGS_OK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                return;
            }

            rv = s1ap_send_initial_context_setup_request(mme_ue);
            ogs_assert(rv == OGS_OK);
            return;
        }

        switch (message->emm.h.message_type) {
        case NAS_IDENTITY_RESPONSE:
            ogs_debug("[EMM] Identity response");
            rv = emm_handle_identity_response(mme_ue,
                    &message->emm.identity_response);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_identity_response() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_error("No IMSI");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            break;
        case NAS_ATTACH_REQUEST:
            ogs_debug("[EMM] Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }
            break;
        case NAS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("[EMM] Tracking area update request");
            rv = emm_handle_tau_request(
                    mme_ue, &message->emm.tracking_area_update_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_tau_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("[EMM] TAU request : Unknown UE");
                rv = nas_send_tau_reject(mme_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_assert(rv == OGS_OK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                return;
            }

            break;
        case NAS_TRACKING_AREA_UPDATE_COMPLETE:
            ogs_debug("[EMM] Tracking area update complete");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            return;
        case NAS_EXTENDED_SERVICE_REQUEST:
            ogs_debug("[EMM] Extended service request");
            rv = emm_handle_extended_service_request(
                    mme_ue, &message->emm.extended_service_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_extended_service_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("[EMM] Extended Service request : Unknown UE");
                rv = nas_send_service_reject(mme_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_assert(rv == OGS_OK);
                OGS_FSM_TRAN(s, &emm_state_exception);
                return;
            }

            break;
        case NAS_EMM_STATUS:
            ogs_warn("[EMM] EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.emm_status.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            return;
        case NAS_DETACH_REQUEST:
            ogs_debug("[EMM] Detach request");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK)
            {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            if (mme_ue->p_tmsi) {
                rv = sgsap_send_detach_indication(mme_ue);
            } else {
                rv = mme_send_delete_session_or_detach(mme_ue);
                ogs_assert(rv == OGS_OK);
            }

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            return;
        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
            return;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", mme_event_get_name(e));
        return;
    }

    if (!MME_UE_HAVE_IMSI(mme_ue)) {
        rv = nas_send_identity_request(mme_ue);
        ogs_assert(rv == OGS_OK);
        return;
    }

    enb_ue = mme_ue->enb_ue;
    ogs_assert(enb_ue);

    switch (mme_ue->nas_eps.type) {
    case MME_EPS_TYPE_ATTACH_REQUEST:
        if (SECURITY_CONTEXT_IS_VALID(mme_ue)) {
            rv = nas_send_emm_to_esm(mme_ue,
                    &mme_ue->pdn_connectivity_request);
            ogs_assert(rv == OGS_OK);
            OGS_FSM_TRAN(s, &emm_state_initial_context_setup);
        } else {
            if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                rv = mme_gtp_send_delete_all_sessions(mme_ue);
                ogs_assert(rv == OGS_OK);
            } else {
                mme_s6a_send_air(mme_ue, NULL);
            }
            OGS_FSM_TRAN(s, &emm_state_authentication);
        }
        break;
    case MME_EPS_TYPE_TAU_REQUEST:
        procedureCode = e->s1ap_code;

        if (!SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
            ogs_warn("No PDN Connection : UE[%s]", mme_ue->imsi_bcd);
            rv = nas_send_tau_reject(mme_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            ogs_assert(rv == OGS_OK);
            OGS_FSM_TRAN(s, emm_state_exception);
            break;
        }

        if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
            mme_s6a_send_air(mme_ue, NULL);
            OGS_FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
            break;
        }

        if (procedureCode == S1AP_ProcedureCode_id_initialUEMessage) {
            ogs_debug("    Iniital UE Message");
            if (mme_ue->nas_eps.update.active_flag) {
                rv = nas_send_tau_accept(mme_ue,
                        S1AP_ProcedureCode_id_InitialContextSetup);
                ogs_assert(rv == OGS_OK);
            } else {
                rv = nas_send_tau_accept(mme_ue,
                        S1AP_ProcedureCode_id_downlinkNASTransport);
                ogs_assert(rv == OGS_OK);

                rv = mme_send_release_access_bearer_or_ue_context_release(
                        mme_ue, enb_ue);
                ogs_assert(rv == OGS_OK);
            }
        } else if (procedureCode == S1AP_ProcedureCode_id_uplinkNASTransport) {
            ogs_debug("    Uplink NAS Transport");
            rv = nas_send_tau_accept(mme_ue,
                    S1AP_ProcedureCode_id_downlinkNASTransport);
            ogs_assert(rv == OGS_OK);
        } else {
            ogs_fatal("Invalid Procedure Code[%d]", (int)procedureCode);
            ogs_assert_if_reached();
        }
        break;
    case MME_EPS_TYPE_EXTENDED_SERVICE_REQUEST:
        procedureCode = e->s1ap_code;

        if (!SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
            ogs_warn("No PDN Connection : UE[%s]", mme_ue->imsi_bcd);
            rv = nas_send_service_reject(mme_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            ogs_assert(rv == OGS_OK);
            OGS_FSM_TRAN(s, emm_state_exception);
            break;
        }

        if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
            ogs_warn("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
            rv = nas_send_service_reject(mme_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            ogs_assert(rv == OGS_OK);
            OGS_FSM_TRAN(s, &emm_state_exception);
            return;
        }

        if (procedureCode == S1AP_ProcedureCode_id_initialUEMessage) {
            ogs_debug("    Iniital UE Message");
            rv = s1ap_send_initial_context_setup_request(mme_ue);
            ogs_assert(rv == OGS_OK);
        } else if (procedureCode == S1AP_ProcedureCode_id_uplinkNASTransport) {
            ogs_error("    Uplink NAS Transport : Not Implemented");
            ogs_assert_if_reached();
        } else {
            ogs_fatal("Invalid Procedure Code[%d]", (int)procedureCode);
            ogs_assert_if_reached();
        }
        break;
    default:
        ogs_fatal("Invalid NAS-EPS[%d]", mme_ue->nas_eps.type);
        ogs_assert_if_reached();
        break;
    }
}

void emm_state_authentication(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    nas_message_t *message = NULL;

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
        case NAS_AUTHENTICATION_RESPONSE:
        {
            nas_authentication_response_t *authentication_response =
                &message->emm.authentication_response;
            nas_authentication_response_parameter_t
                *authentication_response_parameter =
                    &authentication_response->
                        authentication_response_parameter;

            ogs_debug("[EMM] Authentication response");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

            if (memcmp(authentication_response_parameter->res,
                mme_ue->xres,
                authentication_response_parameter->length) != 0) {
                ogs_log_hexdump(OGS_LOG_WARN,
                        authentication_response_parameter->res,
                        authentication_response_parameter->length);
                ogs_log_hexdump(OGS_LOG_WARN,
                        mme_ue->xres, MAX_RES_LEN);
                rv = nas_send_authentication_reject(mme_ue);
                ogs_assert(rv == OGS_OK);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            } else {
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_security_mode);
            }
            break;
        }
        case NAS_AUTHENTICATION_FAILURE:
        {
            nas_authentication_failure_t *authentication_failure =
                &message->emm.authentication_failure;
            nas_authentication_failure_parameter_t
                *authentication_failure_parameter = 
                    &authentication_failure->
                        authentication_failure_parameter;

            ogs_debug("[EMM] Authentication failure");
            ogs_debug("    IMSI[%s] EMM_CAUSE[%d]", mme_ue->imsi_bcd,
                    authentication_failure->emm_cause);

            switch (authentication_failure->emm_cause) {
            case EMM_CAUSE_MAC_FAILURE:
                ogs_warn("Authentication failure(MAC failure)");
                break;
            case EMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE:
                ogs_error("Authentication failure"
                        "(Non-EPS authentication unacceptable)");
                break;
            case EMM_CAUSE_SYNCH_FAILURE:
                ogs_warn("Authentication failure(Synch failure)");
                mme_s6a_send_air(mme_ue,
                        authentication_failure_parameter);
                return;
            default:
                ogs_error("Unknown EMM_CAUSE{%d] in Authentication"
                        " failure",
                        authentication_failure->emm_cause);
                break;
            }

            rv = nas_send_authentication_reject(mme_ue);
            ogs_assert(rv == OGS_OK);
            OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            break;
        }
        case NAS_ATTACH_REQUEST:
            ogs_warn("[EMM] Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            mme_s6a_send_air(mme_ue, NULL);
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;
        case NAS_EMM_STATUS:
            ogs_warn("[EMM] EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.emm_status.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case NAS_DETACH_REQUEST:
            ogs_debug("[EMM] Detach request");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            rv = mme_send_delete_session_or_detach(mme_ue);
            ogs_assert(rv == OGS_OK);

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
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
    ogs_pkbuf_t *emmbuf = NULL;
    nas_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = e->mme_ue;
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        rv = emm_build_security_mode_command(&emmbuf, mme_ue);
        ogs_assert(rv == OGS_OK && emmbuf);

        rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
        ogs_assert(rv == OGS_OK && emmbuf);

        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        if (message->emm.h.security_header_type
                == NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("[EMM] Service request");
            rv = nas_send_service_reject(mme_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            ogs_assert(rv == OGS_OK);
            OGS_FSM_TRAN(s, &emm_state_exception);
            return;
        }

        switch (message->emm.h.message_type) {
        case NAS_SECURITY_MODE_COMPLETE:
            ogs_debug("[EMM] Security mode complete");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

            /* Update Kenb */
            if (SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, 
                        mme_ue->kenb);
                mme_kdf_nh(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
                mme_ue->nhcc = 1;
            }

            mme_s6a_send_ulr(mme_ue);
            if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
                OGS_FSM_TRAN(s, &emm_state_initial_context_setup);
            } else if (mme_ue->nas_eps.type ==
                    MME_EPS_TYPE_SERVICE_REQUEST ||
                    mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST) {
                OGS_FSM_TRAN(s, &emm_state_registered);
            } else {
                ogs_fatal("Invalid NAS_EPS[%d]", mme_ue->nas_eps.type);
                ogs_assert_if_reached();
            }
            break;
        case NAS_SECURITY_MODE_REJECT:
            ogs_warn("[EMM] Security mode reject : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.security_mode_reject.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case NAS_ATTACH_REQUEST:
            ogs_warn("[EMM] Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            mme_s6a_send_air(mme_ue, NULL);
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;
        case NAS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("[EMM] Tracking area update request");
            rv = nas_send_tau_reject(mme_ue,
                EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            ogs_assert(rv == OGS_OK);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case NAS_EMM_STATUS:
            ogs_warn("[EMM] EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.emm_status.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case NAS_DETACH_REQUEST:
            ogs_debug("[EMM] Detach request");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            rv = mme_send_delete_session_or_detach(mme_ue);
            ogs_assert(rv == OGS_OK);

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
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
    nas_message_t *message = NULL;

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
        case NAS_ATTACH_COMPLETE:
            ogs_debug("[EMM] Attach complete");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_complete(
                    mme_ue, &message->emm.attach_complete);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_complete() failed "
                        "in emm_state_initial_context_setup");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }
            if (mme_ue->p_tmsi)
                sgsap_send_tmsi_reallocation_complete(mme_ue);

            OGS_FSM_TRAN(s, &emm_state_registered);
            break;
        case NAS_ATTACH_REQUEST:
            ogs_warn("[EMM] Attach request[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    mme_ue, &message->emm.attach_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            rv = mme_gtp_send_delete_all_sessions(mme_ue);
            ogs_assert(rv == OGS_OK);
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;
        case NAS_EMM_STATUS:
            ogs_warn("[EMM] EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd,
                    message->emm.emm_status.emm_cause);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case NAS_DETACH_REQUEST:
            ogs_debug("[EMM] Detach request");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                return;
            }

            rv = mme_send_delete_session_or_detach(mme_ue);
            ogs_assert(rv == OGS_OK);

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", 
                    message->emm.h.message_type);
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
    ogs_assert(e);
    mme_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event[%s]", mme_event_get_name(e));
        break;
    }
}
