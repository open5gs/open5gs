/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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
#include "mme-gn-handler.h"
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

typedef enum {
    EMM_COMMON_STATE_DEREGISTERED,
    EMM_COMMON_STATE_REGISTERED,
} emm_common_state_e;

static void common_register_state(ogs_fsm_t *s, mme_event_t *e,
        emm_common_state_e state);


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


void emm_state_de_registered(ogs_fsm_t *s, mme_event_t *e)
{
    int r;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = mme_ue_find_by_id(e->mme_ue_id);
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_SERVICE_INDICATOR(mme_ue);
        CLEAR_MME_UE_ALL_TIMERS(mme_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case MME_EVENT_EMM_MESSAGE:
        common_register_state(s, e, EMM_COMMON_STATE_DEREGISTERED);
        break;

    case MME_EVENT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3470:
            if (mme_ue->t3470.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3470)->max_count) {
                ogs_warn("Retransmission of Identity-Request failed. "
                        "Stop retransmission");
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            } else {
                mme_ue->t3470.retry_count++;
                r = nas_eps_send_identity_request(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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

void emm_state_registered(ogs_fsm_t *s, mme_event_t *e)
{
    int r;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = mme_ue_find_by_id(e->mme_ue_id);
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case MME_EVENT_EMM_MESSAGE:
        common_register_state(s, e, EMM_COMMON_STATE_REGISTERED);
        break;

    case MME_EVENT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3413:
            if (mme_ue->t3413.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3413)->max_count) {
                /* Paging failed */
                ogs_warn("Paging to IMSI[%s] failed. Stop paging",
                        mme_ue->imsi_bcd);
                CLEAR_MME_UE_TIMER(mme_ue->t3413);
                mme_ue->paging.failed = true;

                if (MME_PAGING_ONGOING(mme_ue))
                    mme_send_after_paging(mme_ue, true);
            } else {
                mme_ue->t3413.retry_count++;
                /*
                 * If t3413 is timeout, the saved pkbuf is used.
                 * We don't have to set CNDomain.
                 * So, we just set CNDomain to 0
                 */
                r = s1ap_send_paging(mme_ue, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;

        case MME_TIMER_T3470:
            if (mme_ue->t3470.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3470)->max_count) {
                ogs_warn("Retransmission of Identity-Request failed. "
                        "Stop retransmission");
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            } else {
                mme_ue->t3470.retry_count++;
                r = nas_eps_send_identity_request(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;

        case MME_TIMER_T3422:
            if (mme_ue->t3422.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3422)->max_count) {
                ogs_warn("Retransmission of Detach Request failed. "
                        "Stop retransmission");
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            } else {
                mme_ue->t3422.retry_count++;
                r = nas_eps_send_detach_request(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;

        case MME_TIMER_MOBILE_REACHABLE:
            ogs_info("[%s] Mobile Reachable timer expired", mme_ue->imsi_bcd);
            CLEAR_MME_UE_TIMER(mme_ue->t_mobile_reachable);
        /*
         * TS 24.301
         * Section 5.3.5
         * Handling of the periodic tracking area update timer and
         * mobile reachable timer (S1 mode only)
         *
         * The periodic tracking area updating procedure is used to
         * periodically notify the availability of the UE to the network.
         * The procedure is controlled in the UE by timer T3412.
         * The value of timer T3412 is sent by the network to the UE
         * in the ATTACH ACCEPT message and can be sent in the TRACKING AREA
         * UPDATE ACCEPT message. The UE shall apply this value in all tracking
         * areas of the list of tracking areas assigned to the UE
         * until a new value is received.
         *
         * If timer T3412 received by the UE in an ATTACH ACCEPT or TRACKING
         * AREA UPDATE ACCEPT message contains an indication that the timer is
         * deactivated or the timer value is zero, then timer T3412 is
         * deactivated and the UE shall not perform the periodic tracking area
         * updating procedure.
         *
         * Timer T3412 is reset and started with its initial value,
         * when the UE changes from EMM-CONNECTED to EMM-IDLE mode.
         *
         * Timer T3412 is stopped when the UE enters EMM-CONNECTED mode or
         * the EMM-DEREGISTERED state. If the UE is attached for emergency
         * bearer services, and timer T3412 expires, the UE shall not initiate
         * a periodic tracking area updating procedure, but shall locally detach
         * from the network. When the UE is camping on a suitable cell, it may
         * re-attach to regain normal service.
         *
         * When a UE is not attached for emergency bearer services, and timer
         * T3412 expires, the periodic tracking area updating procedure shall
         * be started and the timer shall be set to its initial value
         * for the next start.
         *
         * If the UE is not attached for emergency bearer services, the mobile
         * reachable timer shall be longer than T3412. In this case, by default,
         * the mobile reachable timer is 4 minutes greater than timer T3412.
         *
         * Upon expiry of the mobile reachable timer the network shall start
         * the implicit detach timer. The value of the implicit detach timer is
         * network dependent. If ISR is activated, the default value of
         * the implicit detach timer is 4 minutes greater than timer T3423.
         * If the implicit detach timer expires before the UE contacts
         * the network, the network shall implicitly detach the UE. If the MME
         * includes timer T3346 in the TRACKING AREA UPDATE REJECT message or
         * the SERVICE REJECT message and timer T3346 is greater than timer
         * T3412, the MME sets the mobile reachable timer and the implicit
         * detach timer such that the sum of the timer values is greater than
         * timer T3346.
         */
            ogs_debug("[%s] Starting Implicit Detach timer",
                mme_ue->imsi_bcd);
            ogs_timer_start(mme_ue->t_implicit_detach.timer,
                ogs_time_from_sec(mme_self()->time.t3412.value + 240));
            break;

        case MME_TIMER_IMPLICIT_DETACH:
            ogs_info("[%s] Implicit Detach timer expired, detaching UE",
                mme_ue->imsi_bcd);
            CLEAR_MME_UE_TIMER(mme_ue->t_implicit_detach);
            /* TS 24.301 5.3.5
             * If the implicit detach timer expires before the UE contacts
             * the network, the network shall implicitly detach the UE.
             */
            mme_ue->detach_type = MME_DETACH_TYPE_MME_IMPLICIT;
            if (MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                ogs_assert(OGS_OK == sgsap_send_detach_indication(mme_ue));
            } else {
                enb_ue_t *enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
                if (enb_ue)
                    mme_send_delete_session_or_detach(enb_ue, mme_ue);
                else
                    ogs_error("ENB-S1 Context has already been removed");
            }

            OGS_FSM_TRAN(s, &emm_state_de_registered);
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

static void common_register_state(ogs_fsm_t *s, mme_event_t *e,
        emm_common_state_e state)
{
    int r, rv, xact_count = 0;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    mme_sgsn_t *sgsn = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_rai_t rai;
    ogs_nas_security_header_type_t h;
    ogs_nas_p_tmsi_signature_t *ptmsi_sig = NULL;

    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = mme_ue_find_by_id(e->mme_ue_id);
    ogs_assert(mme_ue);

    switch (e->id) {
    case MME_EVENT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
        ogs_assert(enb_ue);

        h.type = e->nas_type;

        xact_count = mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR);

        if (message->emm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_info("[%s] Service request", mme_ue->imsi_bcd);

            if (state != EMM_COMMON_STATE_REGISTERED) {
                ogs_info("Service request : Not registered[%s]",
                        mme_ue->imsi_bcd);
                r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            rv = emm_handle_service_request(
                    enb_ue, mme_ue, &message->emm.service_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_service_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_info("Service request : Unknown UE");
                r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                ogs_error("No Session Context : IMSI[%s]", mme_ue->imsi_bcd);
                r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!ACTIVE_EPS_BEARERS_IS_AVAIABLE(mme_ue)) {
                ogs_error("No active EPS bearers : IMSI[%s]", mme_ue->imsi_bcd);
                r = nas_eps_send_service_reject(enb_ue, mme_ue,
                        OGS_NAS_EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            r = s1ap_send_initial_context_setup_request(mme_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            OGS_FSM_TRAN(s, &emm_state_registered);
            break;
        }

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_IDENTITY_RESPONSE:
            if (mme_ue->nas_eps.type == 0) {
                ogs_warn("No Received NAS message");
                r = s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            ogs_info("Identity response");
            CLEAR_MME_UE_TIMER(mme_ue->t3470);

            rv = emm_handle_identity_response(enb_ue, mme_ue,
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

            mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
                    OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST);

            if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
                mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
                    xact_count) {
                mme_s6a_send_air(enb_ue, mme_ue, NULL);
            }

            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;

        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_info("[%s] Attach request", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    enb_ue, mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                CLEAR_MME_UE_TIMER(mme_ue->t3470);
                r = nas_eps_send_identity_request(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                break;
            }

            if (h.integrity_protected && SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                /*
                 * If the OLD ENB_UE is being maintained in MME-UE Context,
                 * it deletes the S1 Context after exchanging
                 * the UEContextReleaseCommand/Complete with the eNB
                 */
                CLEAR_S1_CONTEXT(mme_ue);

                mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
                    OGS_GTP_DELETE_HANDLE_PDN_CONNECTIVITY_REQUEST);

                if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
                    mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
                        xact_count) {
                    rv = nas_eps_send_emm_to_esm(mme_ue,
                            &mme_ue->pdn_connectivity_request);
                    if (rv != OGS_OK) {
                        ogs_error("nas_eps_send_emm_to_esm() failed");
                        r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                                OGS_NAS_EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                                OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        OGS_FSM_TRAN(s, &emm_state_exception);
                        break;
                    }
                }

                OGS_FSM_TRAN(s, &emm_state_initial_context_setup);

            } else {
                mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
                    OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST);

                if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
                    mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
                        xact_count) {
                    mme_s6a_send_air(enb_ue, mme_ue, NULL);
                }

                OGS_FSM_TRAN(s, &emm_state_authentication);

            }
            break;

        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_info("[%s] Tracking area update request", mme_ue->imsi_bcd);
            rv = emm_handle_tau_request(enb_ue, mme_ue,
                    &message->emm.tracking_area_update_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_tau_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (emm_tau_request_ue_comes_from_gb_or_iu(&message->emm.tracking_area_update_request)) {
                ogs_info("TAU request : UE comes from SGSN, attempt retrieving context");
                guti_to_rai_ptmsi(&mme_ue->next.guti, &rai, NULL);
                sgsn = mme_sgsn_find_by_routing_address(&rai, 0xffff);
                if (!sgsn) {
                    ogs_plmn_id_t plmn_id;
                    ogs_nas_to_plmn_id(&plmn_id, &rai.lai.nas_plmn_id);
                    ogs_warn("No SGSN route matching RAI[MCC:%u MNC:%u LAC:%u RAC:%u]",
                             ogs_plmn_id_mcc(&plmn_id), ogs_plmn_id_mnc(&plmn_id),
                             rai.lai.lac, rai.rac);
                    r = nas_eps_send_tau_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    OGS_FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                if (message->emm.tracking_area_update_request.presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE)
                    ptmsi_sig = &message->emm.tracking_area_update_request.old_p_tmsi_signature;
                mme_gtp1_send_sgsn_context_request(sgsn, mme_ue, ptmsi_sig);
                /* FIXME: use a specific FSM state here to state we are waiting for resolution from Gn? */
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_info("TAU request : Unknown UE");
                r = nas_eps_send_tau_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                ogs_warn("No PDN Connection : UE[%s]", mme_ue->imsi_bcd);
                r = nas_eps_send_tau_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!ACTIVE_EPS_BEARERS_IS_AVAIABLE(mme_ue)) {
                ogs_warn("No active EPS bearers : IMSI[%s]", mme_ue->imsi_bcd);
                r = nas_eps_send_tau_reject(enb_ue, mme_ue,
                        OGS_NAS_EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!h.integrity_protected || !SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                mme_s6a_send_air(enb_ue, mme_ue, NULL);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            /*
             * <EMM-IDLE State>
             * 1. InitialUEMessage + Tracking area update request
             *    Active flag : No bearer establishment requested (0)
             *    EPS update type : TA updating (0) or Periodic updating (3)
             * 2. DownlinkNASTransport + Tracking area update accept
             *    EPS update result value : TA updated (0)
             * 3. UEContextReleaseCommand
             *    Cause : nas(2) + normal-release(0)
             * 4. UEContextReleaseComplete
             *
             * <EMM-IDLE State>
             * 1. InitialUEMessage + Tracking area update request
             *    Active flag : bearer establishment requested (1)
             *    EPS update type : Combined TA/LA updating with IMSI attach (2)
             * 2. InitialContextSetupRequest + Tracking area update accept
             *    EPS update result : Combined TA/LA updated (1)
             *    New GUTI
             * 3. InitialContextSetupResponse
             * 4. UplinkNASTransport + Tracking area update complete
             *
             * <De-registered State>
             * 1. S1SetupRequest/S1SetupResponse
             * 2. InitialUEMessage + Tracking area update request
             *    Active flag : bearer establishment requested (1)
             *    EPS update type : Periodic updating (3)
             * 3. InitialContextSetupRequest + Tracking area update accept
             *    EPS update result : TA updated (0)
             *    No GUTI
             * 4. InitialContextSetupResponse
             *
             * <Handover>
             * 1. HandoverNotify (Target)
             * 2. UplinkNASTransport + Tracking area update request (Target)
             *    Active flag : bearer establishment requested (1)
             *    EPS update type : TA updating (0)
             * 3. UEContextReleaseCommand (Source)
             *    Cause : radioNetwork(0) + successful-handover(2)
             * 4. UEcontextReleaseComplete (Source)
             * 5. DownlinkNASTransport + Tracking area update accept (Target)
             *    EPS update result : TA updated (0)
             *
             * <Handover + EMM-Idle State>
             * 1. HandoverNotify (Target)
             *
             * 2. UEContextReleaseCommand (Source)
             *    Cause : radioNetwork(0) + successful-handover(2)
             * 3. UEcontextReleaseComplete (Source)
             * 4. UEContextReleaseRequest (Target)
             *    Cause : transport(1) + transport-resource-unavailable(0)
             * 5. UEContextReleaseCommand (Target)
             *    Cause : nas(2) + normal-release(0)
             * 6. UEcontextReleaseComplete (Target)
             *
             * 7. InitialUEMessage + Tracking area update request (Target)
             *    Active flag : bearer establishment requested (1)
             *    EPS update type : TA updating (0)
             * 8. InitialContextSetupRequest + Tracking area update accept
             *    EPS update result : TA updated (0)
             *    New GUTI
             * 9. InitialContextSetupResponse (Target)
             *    EPS update result : TA updated (0)
             * 10. UplinkNASTransport + Tracking area update complete (Target)
             */

            /* Update CSMAP from Tracking area update request */
            mme_ue->csmap = mme_csmap_find_by_tai(&mme_ue->tai);
            if (mme_ue->csmap &&
                mme_ue->network_access_mode ==
                    OGS_NETWORK_ACCESS_MODE_PACKET_AND_CIRCUIT &&
                (mme_ue->nas_eps.update.value ==
                 OGS_NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING ||
                 mme_ue->nas_eps.update.value ==
                 OGS_NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING_WITH_IMSI_ATTACH)) {

                if (e->s1ap_code == S1AP_ProcedureCode_id_initialUEMessage)
                    mme_ue->tracking_area_update_request_type =
                        MME_TAU_TYPE_INITIAL_UE_MESSAGE;
                else if (e->s1ap_code ==
                        S1AP_ProcedureCode_id_uplinkNASTransport)
                    mme_ue->tracking_area_update_request_type =
                        MME_TAU_TYPE_UPLINK_NAS_TRANPORT;
                else {
                    ogs_error("Invalid Procedure Code[%d]", (int)e->s1ap_code);
                    break;
                }

                ogs_assert(OGS_OK ==
                    sgsap_send_location_update_request(mme_ue));

            } else {

                if (e->s1ap_code == S1AP_ProcedureCode_id_initialUEMessage) {
                    ogs_debug("    Initial UE Message");
                    if (mme_ue->nas_eps.update.active_flag) {

    /*
     * TS33.401
     * 7 Security procedures between UE and EPS access network elements
     * 7.2 Handling of user-related keys in E-UTRAN
     * 7.2.7 Key handling for the TAU procedure when registered in E-UTRAN
     *
     * If the "active flag" is set in the TAU request message or
     * the MME chooses to establish radio bearers when there is pending downlink
     * UP data or pending downlink signalling, radio bearers will be established
     * as part of the TAU procedure and a KeNB derivation is necessary.
     */
                        ogs_kdf_kenb(mme_ue->kasme, mme_ue->ul_count.i32,
                                mme_ue->kenb);
                        ogs_kdf_nh_enb(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
                        mme_ue->nhcc = 1;

                        r = nas_eps_send_tau_accept(mme_ue,
                                S1AP_ProcedureCode_id_InitialContextSetup);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                    } else {
                        r = nas_eps_send_tau_accept(mme_ue,
                                S1AP_ProcedureCode_id_downlinkNASTransport);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                    }
                } else if (e->s1ap_code ==
                        S1AP_ProcedureCode_id_uplinkNASTransport) {
                    ogs_debug("    Uplink NAS Transport");
                    r = nas_eps_send_tau_accept(mme_ue,
                            S1AP_ProcedureCode_id_downlinkNASTransport);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else {
                    ogs_error("Invalid Procedure Code[%d]", (int)e->s1ap_code);
                    break;
                }

        /*
         * When active_flag is 0, check if the P-TMSI has been updated.
         * If the P-TMSI has changed, wait to receive the TAU Complete message
         * from the UE before sending the UEContextReleaseCommand.
         *
         * This ensures that the UE has acknowledged the new P-TMSI,
         * allowing the TAU procedure to complete successfully
         * and maintaining synchronization between the UE and the network.
         */
                if (!mme_ue->nas_eps.update.active_flag &&
                    !MME_NEXT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                    enb_ue->relcause.group = S1AP_Cause_PR_nas;
                    enb_ue->relcause.cause = S1AP_CauseNas_normal_release;
                    mme_send_release_access_bearer_or_ue_context_release(
                            enb_ue);
                }
            }

            if (MME_NEXT_GUTI_IS_AVAILABLE(mme_ue)) {
                ogs_fatal("MME does not create new GUTI");
                ogs_assert_if_reached();
                OGS_FSM_TRAN(s, &emm_state_initial_context_setup);
            } else {
                OGS_FSM_TRAN(s, &emm_state_registered);
            }
            break;

        case OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST:
            ogs_info("[%s] Extended service request", mme_ue->imsi_bcd);

            rv = emm_handle_extended_service_request(
                    enb_ue, mme_ue, &message->emm.extended_service_request);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_extended_service_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("Extended Service request : Unknown UE");
                r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
                ogs_warn("No PDN Connection : UE[%s]", mme_ue->imsi_bcd);
                r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            if (e->s1ap_code == S1AP_ProcedureCode_id_initialUEMessage) {
                ogs_debug("    Initial UE Message");

                if (!MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                    ogs_warn("No P-TMSI : UE[%s]", mme_ue->imsi_bcd);
                    r = nas_eps_send_service_reject(enb_ue, mme_ue,
                        OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    enb_ue->relcause.group = S1AP_Cause_PR_nas;
                    enb_ue->relcause.cause = S1AP_CauseNas_normal_release;
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
                    ogs_assert(OGS_OK ==
                        sgsap_send_mo_csfb_indication(mme_ue));
                } else if (mme_ue->nas_eps.service.value ==
                        OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE) {
                    ogs_debug("    SERVICE_REQUEST[%d]",
                            mme_ue->nas_eps.service.value);
                    ogs_assert(OGS_OK ==
                        sgsap_send_service_request(mme_ue, SGSAP_EMM_IDLE_MODE)
                    );
                } else {
                    ogs_warn(" Unknown CSFB Service Type[%d]",
                            mme_ue->nas_eps.service.value);
                    r = nas_eps_send_service_reject(enb_ue, mme_ue,
                        OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    OGS_FSM_TRAN(s, &emm_state_exception);
                    break;
                }

                r = s1ap_send_initial_context_setup_request(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

            } else if (e->s1ap_code ==
                    S1AP_ProcedureCode_id_uplinkNASTransport) {
                ogs_debug("    Uplink NAS Transport");

                if (!MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                    ogs_warn("No P-TMSI : UE[%s]", mme_ue->imsi_bcd);
                    r = nas_eps_send_service_reject(enb_ue, mme_ue,
                        OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    break;
                }

                if (mme_ue->nas_eps.service.value ==
                        OGS_NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE ||
                    mme_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE) {
                    ogs_debug("    MO-CSFB-INDICATION[%d]",
                            mme_ue->nas_eps.service.value);
                    ogs_assert(OGS_OK ==
                        sgsap_send_mo_csfb_indication(mme_ue));
                } else if (mme_ue->nas_eps.service.value ==
                        OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE) {
                    ogs_debug("    SERVICE_REQUEST[%d]",
                            mme_ue->nas_eps.service.value);
                    ogs_assert(OGS_OK ==
                        sgsap_send_service_request(
                            mme_ue, SGSAP_EMM_CONNECTED_MODE));
                } else {
                    ogs_warn(" Unknown CSFB Service Type[%d]",
                            mme_ue->nas_eps.service.value);
                    r = nas_eps_send_service_reject(enb_ue, mme_ue,
                        OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    OGS_FSM_TRAN(s, &emm_state_exception);
                    break;
                }

                r = s1ap_send_ue_context_modification_request(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else {
                ogs_error("Invalid Procedure Code[%d]", (int)e->s1ap_code);
            }
            break;

        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd, message->emm.emm_status.emm_cause);
            break;

        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_info("[%s] Detach request", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    enb_ue, mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_detach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("Detach request : Unknown UE");
                ogs_assert(OGS_OK ==
                    nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK));
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                ogs_assert(OGS_OK ==
                    nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK));
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            if (MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                ogs_assert(OGS_OK == sgsap_send_detach_indication(mme_ue));
            } else {
                mme_send_delete_session_or_detach(enb_ue, mme_ue);
            }

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;

        case OGS_NAS_EPS_DETACH_ACCEPT:
            ogs_info("[%s] Detach accept", mme_ue->imsi_bcd);

            CLEAR_MME_UE_TIMER(mme_ue->t3422);

            r = s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_detach,
                    S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;

        case OGS_NAS_EPS_UPLINK_NAS_TRANSPORT:
            ogs_debug("Uplink NAS Transport");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
            if (MME_SGSAP_IS_CONNECTED(mme_ue)) {
                ogs_assert(OGS_OK ==
                    sgsap_send_uplink_unitdata(mme_ue, &message->emm.
                        uplink_nas_transport.nas_message_container));
            } else {
                S1AP_MME_UE_S1AP_ID_t MME_UE_S1AP_ID;
                S1AP_ENB_UE_S1AP_ID_t ENB_UE_S1AP_ID;
                mme_enb_t *enb = NULL;

                ogs_warn("No connection of MSC/VLR");
                MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
                ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

                enb = mme_enb_find_by_id(enb_ue->enb_id);
                if (enb) {
                    r = s1ap_send_error_indication(enb,
                            &MME_UE_S1AP_ID, &ENB_UE_S1AP_ID,
                            S1AP_Cause_PR_transport,
                            S1AP_CauseTransport_transport_resource_unavailable);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else
                    ogs_error("eNB has already been removed");
            }
            break;

        case OGS_NAS_EPS_ATTACH_COMPLETE:
            ogs_error("[%s] Attach complete in INVALID-STATE",
                        mme_ue->imsi_bcd);
            break;

        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_COMPLETE:
            ogs_info("[%s] Tracking area update complete", mme_ue->imsi_bcd);

        /*
         * TS24.301
         * Section 4.4.4.3
         * Integrity checking of NAS signalling messages in the MME:
         *
         * Once the secure exchange of NAS messages has been established
         * for the NAS signalling connection, the receiving EMM or ESM entity
         * in the MME shall not process any NAS signalling messages
         * unless they have been successfully integrity checked by the NAS.
         * If any NAS signalling message, having not successfully passed
         * the integrity check, is received, then the NAS in the MME shall
         * discard that message. If any NAS signalling message is received,
         * as not integrity protected even though the secure exchange
         * of NAS messages has been established, then the NAS shall discard
         * this message.
         */
            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] No Integrity Protected", mme_ue->imsi_bcd);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("[%s] No Security Context", mme_ue->imsi_bcd);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            CLEAR_MME_UE_TIMER(mme_ue->t3450);

            /* Confirm GUTI */
            if (MME_NEXT_GUTI_IS_AVAILABLE(mme_ue))
                mme_ue_confirm_guti(mme_ue);

            /* Confirm P-TMSI */
            if (MME_NEXT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                mme_ue_confirm_p_tmsi(mme_ue);

                ogs_assert(OGS_OK ==
                    sgsap_send_tmsi_reallocation_complete(mme_ue));

                if (!mme_ue->nas_eps.update.active_flag) {
                    enb_ue->relcause.group = S1AP_Cause_PR_nas;
                    enb_ue->relcause.cause = S1AP_CauseNas_normal_release;
                    mme_send_release_access_bearer_or_ue_context_release(
                            enb_ue);
                }
            }
            break;

        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
        }
        break;

    default:
        ogs_fatal("Unknown event[%s]", mme_event_get_name(e));
        ogs_assert_if_reached();
    }
}

void emm_state_authentication(ogs_fsm_t *s, mme_event_t *e)
{
    int r, rv;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = mme_ue_find_by_id(e->mme_ue_id);
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVENT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
        ogs_assert(enb_ue);

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
                r = nas_eps_send_authentication_reject(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            } else {
                mme_ue->selected_int_algorithm =
                    mme_selected_int_algorithm(mme_ue);
                mme_ue->selected_enc_algorithm =
                    mme_selected_enc_algorithm(mme_ue);

                if (mme_ue->selected_int_algorithm ==
                        OGS_NAS_SECURITY_ALGORITHMS_EIA0) {
                    ogs_error("Encrypt[0x%x] can be skipped with EEA0, "
                        "but Integrity[0x%x] cannot be bypassed with EIA0",
                        mme_ue->selected_enc_algorithm,
                        mme_ue->selected_int_algorithm);
                    OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
                    break;
                }

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
            ogs_debug("    IMSI[%s] OGS_NAS_EMM_CAUSE[%d]", mme_ue->imsi_bcd,
                    authentication_failure->emm_cause);

            CLEAR_MME_UE_TIMER(mme_ue->t3460);

            switch (authentication_failure->emm_cause) {
            case OGS_NAS_EMM_CAUSE_MAC_FAILURE:
                ogs_warn("Authentication failure(MAC failure)");
                break;
            case OGS_NAS_EMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE:
                ogs_error("Authentication failure"
                        "(Non-EPS authentication unacceptable)");
                break;
            case OGS_NAS_EMM_CAUSE_SYNCH_FAILURE:
                ogs_info("Authentication failure(Synch failure)");
                mme_s6a_send_air(enb_ue, mme_ue,
                        authentication_failure_parameter);
                return;
            default:
                ogs_error("Unknown OGS_NAS_EMM_CAUSE{%d] in Authentication"
                        " failure",
                        authentication_failure->emm_cause);
                break;
            }

            r = nas_eps_send_authentication_reject(mme_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);

            break;
        }
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("[%s] Attach request", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    enb_ue, mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_s6a_send_air(enb_ue, mme_ue, NULL);
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd, message->emm.emm_status.emm_cause);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_warn("[%s] Detach request", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    enb_ue, mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_detach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("Detach request : Unknown UE");
                ogs_assert(OGS_OK ==
                    nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK));
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                ogs_assert(OGS_OK ==
                    nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK));
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            if (MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                ogs_assert(OGS_OK == sgsap_send_detach_indication(mme_ue));
            } else {
                mme_send_delete_session_or_detach(enb_ue, mme_ue);
            }

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
            break;
        }
        break;
    case MME_EVENT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3460:
            if (mme_ue->t3460.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3460)->max_count) {
                ogs_warn("Retransmission of IMSI[%s] failed. "
                        "Stop retransmission", mme_ue->imsi_bcd);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);

                r = nas_eps_send_authentication_reject(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else {
                mme_ue->t3460.retry_count++;
                r = nas_eps_send_authentication_request(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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
    int r, rv;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = mme_ue_find_by_id(e->mme_ue_id);
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_MME_UE_TIMER(mme_ue->t3460);
        r = nas_eps_send_security_mode_command(mme_ue);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVENT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
        ogs_assert(enb_ue);

        if (message->emm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("Service request");
            r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        }

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_SECURITY_MODE_COMPLETE:
            ogs_debug("Security mode complete");
            ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

            CLEAR_MME_UE_TIMER(mme_ue->t3460);

        /*
         * TS24.301
         * Section 4.4.4.3
         * Integrity checking of NAS signalling messages in the MME:
         *
         * Once the secure exchange of NAS messages has been established
         * for the NAS signalling connection, the receiving EMM or ESM entity
         * in the MME shall not process any NAS signalling messages
         * unless they have been successfully integrity checked by the NAS.
         * If any NAS signalling message, having not successfully passed
         * the integrity check, is received, then the NAS in the MME shall
         * discard that message. If any NAS signalling message is received,
         * as not integrity protected even though the secure exchange
         * of NAS messages has been established, then the NAS shall discard
         * this message.
         */
            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] No Integrity Protected", mme_ue->imsi_bcd);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("[%s] No Security Context", mme_ue->imsi_bcd);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            emm_handle_security_mode_complete(
                    enb_ue, mme_ue, &message->emm.security_mode_complete);

            ogs_kdf_kenb(mme_ue->kasme, mme_ue->ul_count.i32,
                    mme_ue->kenb);
            ogs_kdf_nh_enb(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
            mme_ue->nhcc = 1;

            /* Create New GUTI */
            mme_ue_new_guti(mme_ue);

             /* Special path when SGSN (Gn interface) is involved: */
            if (mme_ue->gn.gtp_xact_id != OGS_INVALID_POOL_ID) {
                ogs_gtp_xact_t *gtp_xact = ogs_gtp_xact_find_by_id(mme_ue->gn.gtp_xact_id);
                if (!gtp_xact) {
                    ogs_warn("Not xact found!");
                    OGS_FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                uint8_t pti = OGS_POINTER_TO_UINT(gtp_xact->data);
                rv = mme_gtp1_send_sgsn_context_ack(mme_ue, OGS_GTP1_CAUSE_REQUEST_ACCEPTED, gtp_xact);
                if (rv != OGS_OK) {
                    ogs_warn("Tx SGSN Context Request failed(%d)", rv);
                    OGS_FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                mme_ue->gn.gtp_xact_id = OGS_INVALID_POOL_ID;

                mme_sess_t *sess = mme_sess_find_by_pti(mme_ue, pti);
                ogs_assert(sess);
                mme_gtp_send_create_session_request(enb_ue, sess,
                                                    OGS_GTP_CREATE_IN_TRACKING_AREA_UPDATE);
                OGS_FSM_TRAN(s, &emm_state_initial_context_setup);
                break;
            }

            mme_s6a_send_ulr(enb_ue, mme_ue);

            if (MME_NEXT_GUTI_IS_AVAILABLE(mme_ue)) {
                OGS_FSM_TRAN(s, &emm_state_initial_context_setup);
            } else {
                ogs_fatal("MME always creates new GUTI");
                ogs_assert_if_reached();
                OGS_FSM_TRAN(s, &emm_state_registered);
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
            ogs_warn("[%s] Attach request", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    enb_ue, mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_s6a_send_air(enb_ue, mme_ue, NULL);
            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("Tracking area update request");
            r = nas_eps_send_tau_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd, message->emm.emm_status.emm_cause);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_warn("[%s] Detach request", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    enb_ue, mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_detach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("Detach request : Unknown UE");
                ogs_assert(OGS_OK ==
                    nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK));
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                ogs_assert(OGS_OK ==
                    nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK));
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            if (MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                ogs_assert(OGS_OK == sgsap_send_detach_indication(mme_ue));
            } else {
                mme_send_delete_session_or_detach(enb_ue, mme_ue);
            }

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
            break;
        }
        break;
    case MME_EVENT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3460:
            if (mme_ue->t3460.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3460)->max_count) {
                ogs_warn("Retransmission of IMSI[%s] failed. "
                        "Stop retransmission", mme_ue->imsi_bcd);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);

                r = nas_eps_send_attach_reject(
                        enb_ue_find_by_id(mme_ue->enb_ue_id), mme_ue,
                        OGS_NAS_EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                        OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else {
                mme_ue->t3460.retry_count++;
                r = nas_eps_send_security_mode_command(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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
    int r, rv, xact_count;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    mme_ue = mme_ue_find_by_id(e->mme_ue_id);
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVENT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
        ogs_assert(enb_ue);

        xact_count = mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR);

        if (message->emm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("Service request");
            r = nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            OGS_FSM_TRAN(s, &emm_state_exception);
            break;
        }

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_ATTACH_COMPLETE:
            ogs_info("[%s] Attach complete", mme_ue->imsi_bcd);

        /*
         * TS24.301
         * Section 4.4.4.3
         * Integrity checking of NAS signalling messages in the MME:
         *
         * Once the secure exchange of NAS messages has been established
         * for the NAS signalling connection, the receiving EMM or ESM entity
         * in the MME shall not process any NAS signalling messages
         * unless they have been successfully integrity checked by the NAS.
         * If any NAS signalling message, having not successfully passed
         * the integrity check, is received, then the NAS in the MME shall
         * discard that message. If any NAS signalling message is received,
         * as not integrity protected even though the secure exchange
         * of NAS messages has been established, then the NAS shall discard
         * this message.
         */
            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] No Integrity Protected", mme_ue->imsi_bcd);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("[%s] No Security Context", mme_ue->imsi_bcd);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            CLEAR_MME_UE_TIMER(mme_ue->t3450);

            rv = emm_handle_attach_complete(
                    enb_ue, mme_ue, &message->emm.attach_complete);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_complete() failed "
                        "in emm_state_initial_context_setup");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            /* Confirm GUTI */
            if (MME_NEXT_GUTI_IS_AVAILABLE(mme_ue))
                mme_ue_confirm_guti(mme_ue);

            /* Confirm P-TMSI */
            if (MME_NEXT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                mme_ue_confirm_p_tmsi(mme_ue);
                ogs_assert(OGS_OK ==
                    sgsap_send_tmsi_reallocation_complete(mme_ue));
            }

            OGS_FSM_TRAN(s, &emm_state_registered);
            break;

        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_COMPLETE:
            ogs_info("[%s] Tracking area update complete", mme_ue->imsi_bcd);

        /*
         * TS24.301
         * Section 4.4.4.3
         * Integrity checking of NAS signalling messages in the MME:
         *
         * Once the secure exchange of NAS messages has been established
         * for the NAS signalling connection, the receiving EMM or ESM entity
         * in the MME shall not process any NAS signalling messages
         * unless they have been successfully integrity checked by the NAS.
         * If any NAS signalling message, having not successfully passed
         * the integrity check, is received, then the NAS in the MME shall
         * discard that message. If any NAS signalling message is received,
         * as not integrity protected even though the secure exchange
         * of NAS messages has been established, then the NAS shall discard
         * this message.
         */
            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("[%s] No Integrity Protected", mme_ue->imsi_bcd);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("[%s] No Security Context", mme_ue->imsi_bcd);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            CLEAR_MME_UE_TIMER(mme_ue->t3450);

            /* Confirm GUTI */
            if (MME_NEXT_GUTI_IS_AVAILABLE(mme_ue))
                mme_ue_confirm_guti(mme_ue);

            /* Confirm P-TMSI */
            if (MME_NEXT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                mme_ue_confirm_p_tmsi(mme_ue);

                ogs_assert(OGS_OK ==
                    sgsap_send_tmsi_reallocation_complete(mme_ue));

                if (!mme_ue->nas_eps.update.active_flag) {
                    enb_ue->relcause.group = S1AP_Cause_PR_nas;
                    enb_ue->relcause.cause = S1AP_CauseNas_normal_release;
                    mme_send_release_access_bearer_or_ue_context_release(
                            enb_ue);
                }
            }

            OGS_FSM_TRAN(s, &emm_state_registered);
            break;

        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("[%s] Attach request", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    enb_ue, mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
                OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST);

            if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
                mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
                    xact_count) {
                mme_s6a_send_air(enb_ue, mme_ue, NULL);
            }

            OGS_FSM_TRAN(s, &emm_state_authentication);
            break;

        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("EMM STATUS : IMSI[%s] Cause[%d]",
                    mme_ue->imsi_bcd, message->emm.emm_status.emm_cause);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_warn("[%s] Detach request", mme_ue->imsi_bcd);
            rv = emm_handle_detach_request(
                    enb_ue, mme_ue, &message->emm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_detach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                ogs_warn("Detach request : Unknown UE");
                ogs_assert(OGS_OK ==
                    nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK));
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                ogs_error("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
                ogs_assert(OGS_OK ==
                    nas_eps_send_service_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK));
                OGS_FSM_TRAN(s, &emm_state_exception);
                break;
            }

            /*
             * If the OLD ENB_UE is being maintained in MME-UE Context,
             * it deletes the S1 Context after exchanging
             * the UEContextReleaseCommand/Complete with the eNB
             */
            CLEAR_S1_CONTEXT(mme_ue);

            if (MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                ogs_assert(OGS_OK == sgsap_send_detach_indication(mme_ue));
            } else {
                mme_send_delete_session_or_detach(enb_ue, mme_ue);
            }

            OGS_FSM_TRAN(s, &emm_state_de_registered);
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMPLETE:
            ogs_warn("[%s] Duplicated : Security mode complete",
                    mme_ue->imsi_bcd);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->emm.h.message_type);
            break;
        }
        break;
    case MME_EVENT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3450:
            if (mme_ue->t3450.retry_count >=
                    mme_timer_cfg(MME_TIMER_T3450)->max_count) {
                ogs_warn("Retransmission of IMSI[%s] failed. "
                        "Stop retransmission", mme_ue->imsi_bcd);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            } else {
                ogs_pkbuf_t *emmbuf = NULL;

                mme_ue->t3450.retry_count++;

                emmbuf = mme_ue->t3450.pkbuf;
                if (!emmbuf) {
                    ogs_error("No emmbuf");
                    return;
                }

                mme_ue->t3450.pkbuf = ogs_pkbuf_copy(emmbuf);
                if (!mme_ue->t3450.pkbuf) {
                    ogs_error("ogs_pkbuf_copy() failed");
                    ogs_pkbuf_free(emmbuf);
                    return;
                }

                ogs_timer_start(mme_ue->t3450.timer,
                        mme_timer_cfg(MME_TIMER_T3450)->duration);

                r = nas_eps_send_to_downlink_nas_transport(
                        enb_ue_find_by_id(mme_ue->enb_ue_id), emmbuf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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

void emm_state_exception(ogs_fsm_t *s, mme_event_t *e)
{
    int r, rv, xact_count;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(e);
    mme_sm_debug(e);

    mme_ue = mme_ue_find_by_id(e->mme_ue_id);
    ogs_assert(mme_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_SERVICE_INDICATOR(mme_ue);
        CLEAR_MME_UE_ALL_TIMERS(mme_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case MME_EVENT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
        ogs_assert(enb_ue);

        h.type = e->nas_type;

        xact_count = mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR);

        switch (message->emm.h.message_type) {
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("[%s] Attach request", mme_ue->imsi_bcd);
            rv = emm_handle_attach_request(
                    enb_ue, mme_ue, &message->emm.attach_request, e->pkbuf);
            if (rv != OGS_OK) {
                ogs_error("emm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, emm_state_exception);
                break;
            }

            if (!MME_UE_HAVE_IMSI(mme_ue)) {
                CLEAR_MME_UE_TIMER(mme_ue->t3470);
                r = nas_eps_send_identity_request(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                OGS_FSM_TRAN(s, &emm_state_de_registered);
                break;
            }

            if (h.integrity_protected && SECURITY_CONTEXT_IS_VALID(mme_ue)) {
                /*
                 * If the OLD ENB_UE is being maintained in MME-UE Context,
                 * it deletes the S1 Context after exchanging
                 * the UEContextReleaseCommand/Complete with the eNB
                 */
                CLEAR_S1_CONTEXT(mme_ue);

                mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
                    OGS_GTP_DELETE_HANDLE_PDN_CONNECTIVITY_REQUEST);

                if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
                    mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
                        xact_count) {
                    rv = nas_eps_send_emm_to_esm(mme_ue,
                            &mme_ue->pdn_connectivity_request);
                    if (rv != OGS_OK) {
                        ogs_error("nas_eps_send_emm_to_esm() failed");
                        r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                                OGS_NAS_EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                                OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        OGS_FSM_TRAN(s, &emm_state_exception);
                        break;
                    }
                }

                OGS_FSM_TRAN(s, &emm_state_initial_context_setup);

            } else {
                mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
                    OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST);

                if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
                    mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
                        xact_count) {
                    mme_s6a_send_air(enb_ue, mme_ue, NULL);
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
