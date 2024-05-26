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
#include "namf-handler.h"
#include "sbi-path.h"
#include "amf-sm.h"
#include "namf-build.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gmm_log_domain

typedef enum {
    GMM_COMMON_STATE_DEREGISTERED,
    GMM_COMMON_STATE_REGISTERED,
} gmm_common_state_e;

static void common_register_state(ogs_fsm_t *s, amf_event_t *e,
        gmm_common_state_e state);


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

void gmm_state_de_registered(ogs_fsm_t *s, amf_event_t *e)
{
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    ogs_sbi_message_t *sbi_message = NULL;

    int r, state = 0, xact_count;

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

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        AMF_UE_CLEAR_PAGING_INFO(amf_ue);
        AMF_UE_CLEAR_N2_TRANSFER(amf_ue, pdu_session_resource_setup_request);
        AMF_UE_CLEAR_5GSM_MESSAGE(amf_ue);
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

        /*
         * Issue #2040
         *
         * TS23.501
         * 5.4.4 UE Radio Capability handling
         * 5.4.4.1 UE radio capability information storage in the AMF
         *
         * The AMF deletes the UE radio capability when the UE RM state
         * in the AMF transitions to RM-DEREGISTERED. When the AMF receives
         * Registration Request with the Registration type set to Initial
         * Registration or when it receives the first Registration Request
         * after E-UTRA/EPC Attach with Registration type set to Mobility
         * Registration Update, the AMF deletes the UE radio capability.
         *
         * TS24.501
         * 5.5.2 De-registration procedure
         * 5.5.2.1 General
         *
         * When the AMF enters the state 5GMM-DEREGISTERED for 3GPP access,
         * the AMF shall delete the stored UE radio capability information
         * or the UE radio capability ID, if any.
         *
         * (DEPRECATED) Issue #1917 (from Switch-Off to De-Registration)
         *
         * When the UE sends a De-registration Request with Switch-Off,
         * AMF should remove the the stored UE Radio Capability.
         *
         * Otherwise, the Radio Capability will not match
         * because the gNB will not query the Radio Capability
         * when the UE changes USIM.
         */
        OGS_ASN_CLEAR_DATA(&amf_ue->ueRadioCapability);
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case AMF_EVENT_5GMM_MESSAGE:
        common_register_state(s, e, GMM_COMMON_STATE_DEREGISTERED);
        break;

    case AMF_EVENT_5GMM_TIMER:
        switch (e->h.timer_id) {
        case AMF_TIMER_T3570:
            if (amf_ue->t3570.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3570)->max_count) {
                ogs_warn("Retransmission of Identity-Request failed. "
                        "Stop retransmission");
                CLEAR_AMF_UE_TIMER(amf_ue->t3570);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3570.retry_count++;
                r = nas_5gs_send_identity_request(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;

        case AMF_TIMER_T3522:
            if (amf_ue->t3522.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3522)->max_count) {
                ogs_warn("Retransmission of Deregistration-Request failed. "
                        "Stop retransmission");
                CLEAR_AMF_UE_TIMER(amf_ue->t3522);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3522.retry_count++;
                r = nas_5gs_send_de_registration_request(amf_ue,
                        OpenAPI_deregistration_reason_NULL, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;

        case AMF_TIMER_MOBILE_REACHABLE:

            /*
             * TS 24.501
             * 5.3.7 Handling of the periodic registration update timer and
             *
             * Upon expiry of the mobile reachable timer the network shall
             * start the implicit de-registration timer over 3GPP access.
             * The default value of the implicit de-registration timer over
             * 3GPP access is 4 minutes greater than the value of timer T3512.
             */

            ogs_warn("[%s] Mobile Reachable Timer Expired", amf_ue->supi);

            ogs_list_for_each(&amf_ue->sess_list, sess) {
                if (sess->paging.ongoing == true &&
                    sess->paging.n1n2_failure_txf_notif_uri != NULL) {
                    ogs_assert(true ==
                        amf_sbi_send_n1_n2_failure_notify(
                            sess,
                            OpenAPI_n1_n2_message_transfer_cause_UE_NOT_REACHABLE_FOR_SESSION));
                }
            }

            /* Stop Paging */
            AMF_UE_CLEAR_PAGING_INFO(amf_ue);
            AMF_UE_CLEAR_N2_TRANSFER(
                    amf_ue, pdu_session_resource_setup_request);
            AMF_UE_CLEAR_5GSM_MESSAGE(amf_ue);
            CLEAR_AMF_UE_TIMER(amf_ue->t3513);

            ogs_timer_start(amf_ue->implicit_deregistration.timer,
                    ogs_time_from_sec(amf_self()->time.t3512.value + 240));
            break;

        case AMF_TIMER_IMPLICIT_DEREGISTRATION:

            /*
             * TS 24.501
             * 5.3.7 Handling of the periodic registration update timer and
             *
             * If the implicit de-registration timer expires before the UE
             * contacts the network, the network shall implicitly de-register
             * the UE.
             *
             * TS 23.502
             * 4.2.2.3.3 Network-initiated Deregistration
             *
             * The AMF does not send the Deregistration Request message
             * to the UE for Implicit Deregistration.
             */

            ogs_info("[%s] Do Network-initiated De-register UE", amf_ue->supi);

            state = AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED;

            if (UDM_SDM_SUBSCRIBED(amf_ue)) {
                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                        amf_nudm_sdm_build_subscription_delete,
                        amf_ue, state, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                    r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                        NULL,
                        amf_npcf_am_policy_control_build_delete,
                        amf_ue, state, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);
        state = e->h.sbi.state;

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NAUSF_AUTH)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS)

                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                    if (sbi_message->res_status ==
                            OGS_SBI_HTTP_STATUS_NOT_FOUND) {
                        ogs_warn("[%s] Cannot find SUCI [%d]",
                            amf_ue->suci, sbi_message->res_status);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                            amf_ue->suci, sbi_message->res_status);
                    }
                }

                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    ogs_warn("[%s] Ignore SBI message", amf_ue->suci);
                    break;
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    ogs_warn("[%s] Ignore SBI message", amf_ue->suci);
                    break;
                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    CLEAR_5G_AKA_CONFIRMATION(amf_ue);

                    if (state == AMF_RELEASE_SM_CONTEXT_NO_STATE ||
                        state == AMF_UE_INITIATED_DE_REGISTERED) {

                        if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                            r = amf_ue_sbi_discover_and_send(
                                    OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                    NULL,
                                    amf_npcf_am_policy_control_build_delete,
                                    amf_ue, state, NULL);
                            ogs_expect(r == OGS_OK);
                            ogs_assert(r != OGS_ERROR);
                        } else {
                            r = nas_5gs_send_de_registration_accept(amf_ue);
                            ogs_expect(r == OGS_OK);
                            ogs_assert(r != OGS_ERROR);
                        }

                    } else if (state ==
                            AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED ||
                               state ==
                            AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED) {

                        xact_count = amf_sess_xact_count(amf_ue);
                        amf_sbi_send_release_all_sessions(NULL, amf_ue, state);

                        if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
                            amf_sess_xact_count(amf_ue) == xact_count) {

                            if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                                r = amf_ue_sbi_discover_and_send(
                                        OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                        NULL,
                                        amf_npcf_am_policy_control_build_delete,
                                        amf_ue, state, NULL);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);
                            }
                        }
                    }
                    break;
                DEFAULT
                    ogs_error("[%s] Invalid HTTP method [%s]",
                            amf_ue->suci, sbi_message->h.method);
                    ogs_assert_if_reached();
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_5G_AKA)
            CASE(OGS_SBI_RESOURCE_NAME_5G_AKA_CONFIRMATION)
            CASE(OGS_SBI_RESOURCE_NAME_EAP_SESSION)
                ogs_warn("[%s] Ignore SBI message", amf_ue->supi);
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            if ((sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) &&
                (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED) &&
                (sbi_message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT)) {
                ogs_error("[%s] HTTP response error [%d]",
                          amf_ue->supi, sbi_message->res_status);
            }

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
            CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                ogs_warn("[%s] Ignore SBI message", amf_ue->supi);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    /*
                     * - AMF_UE_INITIATED_DE_REGISTERED
                     * 1. PDU session establishment request
                     * 2. PDUSessionResourceSetupRequest +
                     *    PDU session establishment accept
                     * 3. PDUSessionResourceSetupResponse
                     * 4. Deregistration request
                     * 5. UEContextReleaseCommand
                     * 6. UEContextReleaseComplete
                     *
                     * - AMF_RELEASE_SM_CONTEXT_NO_STATE
                     * 1. PDU session release request
                     * 2. PDUSessionResourceReleaseCommand +
                     *    PDU session release command
                     * 3. PDUSessionResourceReleaseREsponse
                     * 4. PDU session release complete
                     * 5. Deregistration request
                     * 6. UEContextReleaseCommand
                     * 7. UEContextReleaseComplete
                     */
                    if (state == AMF_RELEASE_SM_CONTEXT_NO_STATE ||
                        state == AMF_UE_INITIATED_DE_REGISTERED) {
                        UDM_SDM_CLEAR(amf_ue);

                        r = amf_ue_sbi_discover_and_send(
                                OGS_SBI_SERVICE_TYPE_NUDM_UECM, NULL,
                                amf_nudm_uecm_build_registration_delete,
                                amf_ue, state, NULL);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                    } else {
                        ogs_fatal("Invalid state [%d]", state);
                        ogs_assert_if_reached();
                    }
                    break;
                DEFAULT
                    ogs_warn("[%s] Ignore invalid HTTP method [%s]",
                            amf_ue->suci, sbi_message->h.method);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[1]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)
            if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                sbi_message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT &&
                sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                ogs_error("[%s] HTTP response error [%d]",
                        amf_ue->supi, sbi_message->res_status);
            }

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    ogs_warn("[%s] Ignore SBI message", amf_ue->supi);
                    break;
                CASE(OGS_SBI_HTTP_METHOD_PATCH)
                    SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS)
                        /*
                         * - AMF_UE_INITIATED_DE_REGISTERED
                         * 1. PDU session establishment request
                         * 2. PDUSessionResourceSetupRequest +
                         *    PDU session establishment accept
                         * 3. PDUSessionResourceSetupResponse
                         * 4. Deregistration request
                         * 5. UEContextReleaseCommand
                         * 6. UEContextReleaseComplete
                         *
                         * - AMF_RELEASE_SM_CONTEXT_NO_STATE
                         * 1. PDU session release request
                         * 2. PDUSessionResourceReleaseCommand +
                         *    PDU session release command
                         * 3. PDUSessionResourceReleaseREsponse
                         * 4. PDU session release complete
                         * 5. Deregistration request
                         * 6. UEContextReleaseCommand
                         * 7. UEContextReleaseComplete
                         */
                        if (state == AMF_RELEASE_SM_CONTEXT_NO_STATE ||
                            state == AMF_UE_INITIATED_DE_REGISTERED) {

                            if (CHECK_5G_AKA_CONFIRMATION(amf_ue)) {
                                r = amf_ue_sbi_discover_and_send(
                                    OGS_SBI_SERVICE_TYPE_NAUSF_AUTH,
                                    NULL,
                                    amf_nausf_auth_build_authenticate_delete,
                                    amf_ue, state, NULL);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);
                            } else if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                                r = amf_ue_sbi_discover_and_send(
                                        OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                        NULL,
                                        amf_npcf_am_policy_control_build_delete,
                                        amf_ue, state, NULL);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);
                            } else {
                                r = nas_5gs_send_de_registration_accept(amf_ue);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);
                            }
                        } else {
                            ogs_fatal("Invalid state [%d]", state);
                            ogs_assert_if_reached();
                        }
                        break;
                    DEFAULT
                        ogs_warn("Ignoring invalid resource name [%s]",
                                 sbi_message->h.resource.component[2]);
                    END
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

        CASE(OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_POLICIES)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    ogs_warn("[%s] Ignore SBI message", amf_ue->suci);
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    /*
                     * - AMF_UE_INITIATED_DE_REGISTERED
                     * 1. PDU session establishment request
                     * 2. PDUSessionResourceSetupRequest +
                     *    PDU session establishment accept
                     * 3. PDUSessionResourceSetupResponse
                     * 4. Deregistration request
                     * 5. UEContextReleaseCommand
                     * 6. UEContextReleaseComplete
                     *
                     * - AMF_RELEASE_SM_CONTEXT_NO_STATE
                     * 1. PDU session release request
                     * 2. PDUSessionResourceReleaseCommand +
                     *    PDU session release command
                     * 3. PDUSessionResourceReleaseREsponse
                     * 4. PDU session release complete
                     * 5. Deregistration request
                     * 6. UEContextReleaseCommand
                     * 7. UEContextReleaseComplete
                     */
                    if (state == AMF_RELEASE_SM_CONTEXT_NO_STATE ||
                        state == AMF_UE_INITIATED_DE_REGISTERED) {
                        r = nas_5gs_send_de_registration_accept(amf_ue);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        PCF_AM_POLICY_CLEAR(amf_ue);
                    } else {
                        ogs_fatal("Invalid state [%d]", state);
                        ogs_assert_if_reached();
                    }
                    break;

                DEFAULT
                    ogs_error("Unknown method [%s]", sbi_message->h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_TRANSFER)

                        r = OGS_ERROR;

                        if (sbi_message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                            r = amf_namf_comm_handle_ue_context_transfer_response(
                                    sbi_message, amf_ue);
                            if (r != OGS_OK) {
                                ogs_error("failed to handle "
                                        "UE_CONTEXT_TRANSFER response");
                            }
                        } else {
                            ogs_error("[%s] HTTP response error [%d]",
                                amf_ue->suci, sbi_message->res_status);
                        }

                        if (r != OGS_OK) {
                            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                                CLEAR_AMF_UE_TIMER(amf_ue->t3570);
                                r = nas_5gs_send_identity_request(amf_ue);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);
                                break;
                            }
                        }

                        xact_count = amf_sess_xact_count(amf_ue);
                        amf_sbi_send_release_all_sessions(
                                amf_ue->ran_ue, amf_ue,
                                AMF_RELEASE_SM_CONTEXT_NO_STATE);

                        if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
                            amf_sess_xact_count(amf_ue) == xact_count) {
                            r = amf_ue_sbi_discover_and_send(
                                    OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                                    amf_nausf_auth_build_authenticate,
                                    amf_ue, 0, NULL);
                            ogs_expect(r == OGS_OK);
                            ogs_assert(r != OGS_ERROR);
                        }

                        OGS_FSM_TRAN(s, &gmm_state_authentication);
                        break;

                    DEFAULT
                        ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
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
    }
}

void gmm_state_registered(ogs_fsm_t *s, amf_event_t *e)
{
    int i, r, state, xact_count = 0;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

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

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_assert(amf_ue->num_of_slice <= OGS_MAX_NUM_OF_SLICE);
        for (i = 0; i < amf_ue->num_of_slice; i++) {
            amf_metrics_inst_by_slice_add(&amf_ue->nr_tai.plmn_id,
                    &amf_ue->slice[i].s_nssai,
                    AMF_METR_GAUGE_RM_REGISTERED_SUB_NBR, 1);
        }
        break;
    case OGS_FSM_EXIT_SIG:
        ogs_assert(amf_ue->num_of_slice <= OGS_MAX_NUM_OF_SLICE);
        for (i = 0; i < amf_ue->num_of_slice; i++) {
            amf_metrics_inst_by_slice_add(&amf_ue->nr_tai.plmn_id,
                    &amf_ue->slice[i].s_nssai,
                    AMF_METR_GAUGE_RM_REGISTERED_SUB_NBR, -1);
        }
        break;

    case AMF_EVENT_5GMM_MESSAGE:
        common_register_state(s, e, GMM_COMMON_STATE_REGISTERED);
        break;

    case AMF_EVENT_5GMM_TIMER:
        switch (e->h.timer_id) {
        case AMF_TIMER_T3513:
            if (amf_ue->t3513.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3513)->max_count) {
                amf_sess_t *sess = NULL;

                /* Paging failed */
                ogs_warn("[%s] Paging failed. Stop", amf_ue->supi);

                ogs_list_for_each(&amf_ue->sess_list, sess) {
                    if (sess->paging.ongoing == true &&
                        sess->paging.n1n2_failure_txf_notif_uri != NULL) {
                        ogs_assert(true ==
                            amf_sbi_send_n1_n2_failure_notify(
                                sess,
                                OpenAPI_n1_n2_message_transfer_cause_UE_NOT_RESPONDING));
                    }
                }

                AMF_UE_CLEAR_PAGING_INFO(amf_ue);
                AMF_UE_CLEAR_N2_TRANSFER(
                        amf_ue, pdu_session_resource_setup_request);
                AMF_UE_CLEAR_5GSM_MESSAGE(amf_ue);
                CLEAR_AMF_UE_TIMER(amf_ue->t3513);

            } else {
                amf_ue->t3513.retry_count++;
                /* If t3513 is timeout, the saved pkbuf is used.  */
                r = ngap_send_paging(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;

        case AMF_TIMER_T3522:
            if (amf_ue->t3522.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3522)->max_count) {
                ogs_warn("Retransmission of Deregistration-Request failed. "
                        "Stop retransmission");
                CLEAR_AMF_UE_TIMER(amf_ue->t3522);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3522.retry_count++;
                r = nas_5gs_send_de_registration_request(amf_ue,
                        OpenAPI_deregistration_reason_NULL, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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
                r = nas_5gs_send_configuration_update_command(amf_ue, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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
                r = nas_5gs_send_identity_request(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;

        case AMF_TIMER_MOBILE_REACHABLE:

            /*
             * TS 24.501
             * 5.3.7 Handling of the periodic registration update timer and
             *
             * Upon expiry of the mobile reachable timer the network shall
             * start the implicit de-registration timer over 3GPP access.
             * The default value of the implicit de-registration timer over
             * 3GPP access is 4 minutes greater than the value of timer T3512.
             */

            ogs_warn("[%s] Mobile Reachable Timer Expired", amf_ue->supi);

            ogs_list_for_each(&amf_ue->sess_list, sess) {
                if (sess->paging.ongoing == true &&
                    sess->paging.n1n2_failure_txf_notif_uri != NULL) {
                    ogs_assert(true ==
                        amf_sbi_send_n1_n2_failure_notify(
                            sess,
                            OpenAPI_n1_n2_message_transfer_cause_UE_NOT_REACHABLE_FOR_SESSION));
                }
            }

            /* Stop Paging */
            AMF_UE_CLEAR_PAGING_INFO(amf_ue);
            AMF_UE_CLEAR_N2_TRANSFER(
                    amf_ue, pdu_session_resource_setup_request);
            AMF_UE_CLEAR_5GSM_MESSAGE(amf_ue);
            CLEAR_AMF_UE_TIMER(amf_ue->t3513);

            ogs_timer_start(amf_ue->implicit_deregistration.timer,
                    ogs_time_from_sec(amf_self()->time.t3512.value + 240));
            break;

        case AMF_TIMER_IMPLICIT_DEREGISTRATION:

            /*
             * TS 24.501
             * 5.3.7 Handling of the periodic registration update timer and
             *
             * If the implicit de-registration timer expires before the UE
             * contacts the network, the network shall implicitly de-register
             * the UE.
             *
             * TS 23.502
             * 4.2.2.3.3 Network-initiated Deregistration
             *
             * The AMF does not send the Deregistration Request message
             * to the UE for Implicit Deregistration.
             */

            ogs_info("[%s] Do Network-initiated De-register UE", amf_ue->supi);

            state = AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED;

            if (UDM_SDM_SUBSCRIBED(amf_ue)) {
                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                        amf_nudm_sdm_build_subscription_delete,
                        amf_ue, state, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                    r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                        NULL,
                        amf_npcf_am_policy_control_build_delete,
                        amf_ue, state, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);
        state = e->h.sbi.state;

        xact_count = amf_sess_xact_count(amf_ue);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NAUSF_AUTH)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS)

                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                    if (sbi_message->res_status ==
                            OGS_SBI_HTTP_STATUS_NOT_FOUND) {
                        ogs_warn("[%s] Cannot find SUCI [%d]",
                            amf_ue->suci, sbi_message->res_status);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                            amf_ue->suci, sbi_message->res_status);
                    }
                }

                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    ogs_warn("[%s] Ignore SBI message", amf_ue->suci);
                    break;
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    ogs_warn("[%s] Ignore SBI message", amf_ue->suci);
                    break;
                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    CLEAR_5G_AKA_CONFIRMATION(amf_ue);

                    if (state == AMF_RELEASE_SM_CONTEXT_NO_STATE ||
                        state == AMF_UE_INITIATED_DE_REGISTERED) {

                        if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                            r = amf_ue_sbi_discover_and_send(
                                    OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                    NULL,
                                    amf_npcf_am_policy_control_build_delete,
                                    amf_ue, state, NULL);
                            ogs_expect(r == OGS_OK);
                            ogs_assert(r != OGS_ERROR);
                        } else {
                            r = nas_5gs_send_de_registration_accept(amf_ue);
                            ogs_expect(r == OGS_OK);
                            ogs_assert(r != OGS_ERROR);
                        }

                    } else if (state ==
                            AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED ||
                               state ==
                            AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED) {

                        int xact_count = amf_sess_xact_count(amf_ue);
                        amf_sbi_send_release_all_sessions(NULL, amf_ue, state);

                        if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
                            amf_sess_xact_count(amf_ue) == xact_count) {

                            if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                                r = amf_ue_sbi_discover_and_send(
                                        OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                        NULL,
                                        amf_npcf_am_policy_control_build_delete,
                                        amf_ue, state, NULL);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);
                            }
                        }
                    }
                    break;
                DEFAULT
                    ogs_error("[%s] Invalid HTTP method [%s]",
                            amf_ue->suci, sbi_message->h.method);
                    ogs_assert_if_reached();
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_5G_AKA)
            CASE(OGS_SBI_RESOURCE_NAME_5G_AKA_CONFIRMATION)
            CASE(OGS_SBI_RESOURCE_NAME_EAP_SESSION)
                ogs_warn("[%s] Ignore SBI message", amf_ue->supi);
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            if ((sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) &&
                (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED) &&
                (sbi_message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT)) {
                ogs_error("[%s] HTTP response error [%d]",
                          amf_ue->supi, sbi_message->res_status);
            }

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
            CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                ogs_warn("[%s] Ignore SBI message", amf_ue->supi);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    /*
                     * - AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED
                     * 1. Implicit Timer Expiration
                     * 2. UDM_SDM_Unsubscribe
                     * 3. UDM_UECM_Deregisration
                     * 4. PDU session release request
                     * 5. PDUSessionResourceReleaseCommand +
                     *    PDU session release command
                     * 6. PDUSessionResourceReleaseResponse
                     * 7. AM_Policy_Association_Termination
                     *
                     * - AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED
                     * 1. UDM_UECM_DeregistrationNotification
                     * 2. Deregistration request
                     * 3. UDM_SDM_Unsubscribe
                     * 4. UDM_UECM_Deregisration
                     * 5. PDU session release request
                     * 6. PDUSessionResourceReleaseCommand +
                     *    PDU session release command
                     * 7. PDUSessionResourceReleaseResponse
                     * 8. AM_Policy_Association_Termination
                     * 9.  Deregistration accept
                     * 10. Signalling Connecion Release
                     */
                    if (state ==
                            AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED ||
                        state ==
                            AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED) {
                        UDM_SDM_CLEAR(amf_ue);

                        r = amf_ue_sbi_discover_and_send(
                                OGS_SBI_SERVICE_TYPE_NUDM_UECM, NULL,
                                amf_nudm_uecm_build_registration_delete,
                                amf_ue, state, NULL);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                    } else {
                        ogs_fatal("Invalid state [%d]", state);
                        ogs_assert_if_reached();
                    }
                    break;
                DEFAULT
                    ogs_warn("[%s] Ignore invalid HTTP method [%s]",
                            amf_ue->suci, sbi_message->h.method);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[1]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)
            if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                sbi_message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT &&
                sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                ogs_error("[%s] HTTP response error [%d]",
                        amf_ue->supi, sbi_message->res_status);
            }

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    ogs_warn("[%s] Ignore SBI message", amf_ue->supi);
                    break;
                CASE(OGS_SBI_HTTP_METHOD_PATCH)
                    SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS)
                        /*
                         * - AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED
                         * 1. Implicit Timer Expiration
                         * 2. UDM_SDM_Unsubscribe
                         * 3. UDM_UECM_Deregisration
                         * 4. Authentication Result Removal
                         * 5. PDU session release request
                         * 6. PDUSessionResourceReleaseCommand +
                         *    PDU session release command
                         * 7. PDUSessionResourceReleaseResponse
                         * 8. AM_Policy_Association_Termination
                         *
                         * - AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED
                         * 1. UDM_UECM_DeregistrationNotification
                         * 2. Deregistration request
                         * 3. UDM_SDM_Unsubscribe
                         * 4. UDM_UECM_Deregisration
                         * 5. Authentication Result Removal
                         * 6. PDU session release request
                         * 7. PDUSessionResourceReleaseCommand +
                         *    PDU session release command
                         * 8. PDUSessionResourceReleaseResponse
                         * 9. AM_Policy_Association_Termination
                         * 10. Deregistration accept
                         * 11. Signalling Connecion Release
                         */
                        if (state ==
                                AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED ||
                            state ==
                                AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED) {

                            if (amf_ue->confirmation_for_5g_aka.resource_uri) {
                                r = amf_ue_sbi_discover_and_send(
                                        OGS_SBI_SERVICE_TYPE_NAUSF_AUTH,
                                        NULL,
                                        amf_nausf_auth_build_authenticate_delete,
                                        amf_ue, state, NULL);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);

                            } else {

                                amf_sbi_send_release_all_sessions(
                                        NULL, amf_ue, state);

                                if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
                                    amf_sess_xact_count(amf_ue) == xact_count) {

                                    if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                                        r = amf_ue_sbi_discover_and_send(
                                                OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                                NULL,
                                                amf_npcf_am_policy_control_build_delete,
                                                amf_ue, state, NULL);
                                        ogs_expect(r == OGS_OK);
                                        ogs_assert(r != OGS_ERROR);
                                    }
                                }
                            }
                        } else {
                            ogs_fatal("Invalid state [%d]", state);
                            ogs_assert_if_reached();
                        }
                        break;
                    DEFAULT
                        ogs_warn("Ignoring invalid resource name [%s]",
                                 sbi_message->h.resource.component[2]);
                    END
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

        CASE(OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_POLICIES)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    ogs_warn("[%s] Ignore SBI message", amf_ue->suci);
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    /*
                     * - AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED
                     * 1. Implicit Timer Expiration
                     * 2. UDM_SDM_Unsubscribe
                     * 3. UDM_UECM_Deregisration
                     * 4. PDU session release request
                     * 5. PDUSessionResourceReleaseCommand +
                     *    PDU session release command
                     * 6. PDUSessionResourceReleaseResponse
                     * 7. AM_Policy_Association_Termination
                     *
                     * - AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED
                     * 1. UDM_UECM_DeregistrationNotification
                     * 2. Deregistration request
                     * 3. UDM_SDM_Unsubscribe
                     * 4. UDM_UECM_Deregisration
                     * 5. PDU session release request
                     * 6. PDUSessionResourceReleaseCommand +
                     *    PDU session release command
                     * 7. PDUSessionResourceReleaseResponse
                     * 8. AM_Policy_Association_Termination
                     * 9. Deregistration accept
                     * 10.Signalling Connecion Release
                     */
                    if (state == AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED) {
                        ogs_warn("[%s] Implicit De-registered", amf_ue->supi);
                        OGS_FSM_TRAN(&amf_ue->sm,
                                &gmm_state_ue_context_will_remove);

                    } else if (state ==
                            AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED) {
                        ogs_warn("[%s] Explicit De-registered", amf_ue->supi);

                        amf_ue->explict_de_registered.sbi_done = true;

                        if (amf_ue->explict_de_registered.n1_done == true) {
                            r = ngap_send_ran_ue_context_release_command(
                                    amf_ue->ran_ue,
                                    NGAP_Cause_PR_misc,
                                    NGAP_CauseMisc_om_intervention,
                                    NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                            ogs_expect(r == OGS_OK);
                            ogs_assert(r != OGS_ERROR);
                        }

                    } else {
                        ogs_fatal("Invalid state [%d]", state);
                        ogs_assert_if_reached();
                    }
                    break;

                DEFAULT
                    ogs_error("Unknown method [%s]", sbi_message->h.method);
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
    }
}

static void common_register_state(ogs_fsm_t *s, amf_event_t *e,
        gmm_common_state_e state)
{
    int r, xact_count = 0;
    ogs_nas_5gmm_cause_t gmm_cause;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;
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

    switch (e->h.id) {
    case AMF_EVENT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        ran_ue = ran_ue_cycle(amf_ue->ran_ue);
        ogs_assert(ran_ue);

        h.type = e->nas.type;

        xact_count = amf_sess_xact_count(amf_ue);

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_info("Registration request");
            gmm_cause = gmm_handle_registration_request(
                    amf_ue, h, e->ngap.code,
                    &nas_message->gmm.registration_request);

            switch (amf_ue->nas.registration.value) {
            case OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL:
                amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_RM_REG_INIT_REQ);
                break;
            case OGS_NAS_5GS_REGISTRATION_TYPE_MOBILITY_UPDATING:
                amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_RM_REG_MOB_REQ);
                break;
            case OGS_NAS_5GS_REGISTRATION_TYPE_PERIODIC_UPDATING:
                amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_RM_REG_PERIOD_REQ);
                break;
            case OGS_NAS_5GS_REGISTRATION_TYPE_EMERGENCY:
                amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_RM_REG_EMERG_REQ);
                break;
            default:
                ogs_error("Unknown reg_type[%d]", amf_ue->nas.registration.value);
            }

            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("gmm_handle_registration_request() failed [%d]",
                            gmm_cause);
                r = nas_5gs_send_registration_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (gmm_registration_request_from_old_amf(amf_ue,
                        &nas_message->gmm.registration_request) == true) {
                ogs_sbi_discovery_option_t *discovery_option = NULL;
                ogs_guami_t guami;

                discovery_option = ogs_sbi_discovery_option_new();
                ogs_assert(discovery_option);

                /* Configure Home PLMN ID */
                ogs_nas_to_plmn_id(
                        &amf_ue->home_plmn_id, &amf_ue->old_guti.nas_plmn_id);

                memcpy(&guami.plmn_id, &amf_ue->home_plmn_id,
                        sizeof(ogs_plmn_id_t));
                memcpy(&guami.amf_id, &amf_ue->old_guti.amf_id,
                        sizeof(ogs_amf_id_t));

                ogs_sbi_discovery_option_set_guami(discovery_option, &guami);

                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NAMF_COMM, discovery_option,
                        amf_namf_comm_build_ue_context_transfer,
                        amf_ue, state, nas_message);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                CLEAR_AMF_UE_TIMER(amf_ue->t3570);
                r = nas_5gs_send_identity_request(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                break;
            }

            if (h.integrity_protected && SECURITY_CONTEXT_IS_VALID(amf_ue)) {

                /*
                 * If the OLD RAN_UE is being maintained in AMF-UE Context,
                 * it deletes the NG Context after exchanging
                 * the UEContextReleaseCommand/Complete with the gNB
                 */
                CLEAR_NG_CONTEXT(amf_ue);

                gmm_cause = gmm_handle_registration_update(
                        ran_ue, amf_ue, &nas_message->gmm.registration_request);
                if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                    ogs_error("[%s] gmm_handle_registration_update() "
                                "failed [%d]", amf_ue->suci, gmm_cause);
                    r = nas_5gs_send_registration_reject(
                            ran_ue, amf_ue, gmm_cause);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    OGS_FSM_TRAN(s, gmm_state_exception);
                    break;
                }

                if (amf_sess_xact_count(amf_ue) == xact_count) {

                    if (amf_update_allowed_nssai(amf_ue) == false) {
                        ogs_error("No Allowed-NSSAI");
                        r = nas_5gs_send_gmm_reject(
                                ran_ue, amf_ue,
                                OGS_5GMM_CAUSE_NO_NETWORK_SLICES_AVAILABLE);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        OGS_FSM_TRAN(s, gmm_state_exception);
                        break;
                    }

                    if (!UDM_SDM_SUBSCRIBED(amf_ue)) {
                        r = amf_ue_sbi_discover_and_send(
                                OGS_SBI_SERVICE_TYPE_NUDM_UECM, NULL,
                                amf_nudm_uecm_build_registration, amf_ue, 0, NULL);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
                        break;
                    } else if (!PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                        r = amf_ue_sbi_discover_and_send(
                                OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                NULL,
                                amf_npcf_am_policy_control_build_create,
                                amf_ue, 0, NULL);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
                        break;
                    }

                    CLEAR_AMF_UE_TIMER(amf_ue->t3550);
                    r = nas_5gs_send_registration_accept(amf_ue);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                }

                if (amf_ue->next.m_tmsi)
                    OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
                else
                    OGS_FSM_TRAN(s, &gmm_state_registered);

            } else {

                amf_sbi_send_release_all_sessions(
                        ran_ue, amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);

                if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
                    amf_sess_xact_count(amf_ue) == xact_count) {
                    r = amf_ue_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                            amf_nausf_auth_build_authenticate,
                            amf_ue, 0, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                }

                OGS_FSM_TRAN(s, &gmm_state_authentication);
            }
            break;

        case OGS_NAS_5GS_SERVICE_REQUEST:
            ogs_info("Service request");

            if (state != GMM_COMMON_STATE_REGISTERED) {
                ogs_info("[%s] Handling service request failed [Not registered]",
                            amf_ue->suci);
                r = nas_5gs_send_service_reject(ran_ue, amf_ue,
                    OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            gmm_cause = gmm_handle_service_request(
                    amf_ue, h, e->ngap.code, &nas_message->gmm.service_request);
            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("[%s] gmm_handle_service_request() failed [%d]",
                            amf_ue->suci, gmm_cause);
                r = nas_5gs_send_service_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                ogs_info("Service request : Unknown UE");
                r = nas_5gs_send_service_reject(ran_ue, amf_ue,
                    OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!h.integrity_protected || !SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_error("No Security Context");
                r = nas_5gs_send_service_reject(ran_ue, amf_ue,
                    OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            /*
             * If the OLD RAN_UE is being maintained in AMF-UE Context,
             * it deletes the NG Context after exchanging
             * the UEContextReleaseCommand/Complete with the gNB
             */
            CLEAR_NG_CONTEXT(amf_ue);

            gmm_cause = gmm_handle_service_update(
                    ran_ue, amf_ue, &nas_message->gmm.service_request);
            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("[%s] gmm_handle_service_update() failed [%d]",
                            amf_ue->suci, gmm_cause);
                r = nas_5gs_send_service_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            OGS_FSM_TRAN(s, gmm_state_registered);
            break;

        case OGS_NAS_5GS_IDENTITY_RESPONSE:
            if (amf_ue->nas.message_type == 0) {
                ogs_warn("No Received NAS message");
                r = ngap_send_error_indication2(
                        ran_ue,
                        NGAP_Cause_PR_protocol,
                        NGAP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            CLEAR_AMF_UE_TIMER(amf_ue->t3570);

            ogs_info("Identity response");
            gmm_cause = gmm_handle_identity_response(amf_ue,
                    &nas_message->gmm.identity_response);
            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("gmm_handle_identity_response() "
                            "failed [%d] in type [%d]",
                            gmm_cause, amf_ue->nas.message_type);
                r = nas_5gs_send_gmm_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                ogs_error("No SUCI");
                r = nas_5gs_send_gmm_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_sbi_send_release_all_sessions(
                    ran_ue, amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);

            if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
                amf_sess_xact_count(amf_ue) == xact_count) {
                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                        amf_nausf_auth_build_authenticate,
                        amf_ue, 0, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }

            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->suci, nas_message->gmm.gmm_status.gmm_cause);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE:
            ogs_info("[%s] Deregistration request", amf_ue->supi);

            if (!h.integrity_protected || !SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_error("No Security Context");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            /*
             * If the OLD RAN_UE is being maintained in AMF-UE Context,
             * it deletes the NG Context after exchanging
             * the UEContextReleaseCommand/Complete with the gNB
             */
            CLEAR_NG_CONTEXT(amf_ue);

            gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_ACCEPT_TO_UE:
            ogs_info("[%s] Deregistration accept", amf_ue->supi);
            CLEAR_AMF_UE_TIMER(amf_ue->t3522);

            amf_ue->explict_de_registered.n1_done = true;

            if (amf_ue->explict_de_registered.sbi_done == true) {
                r = ngap_send_ran_ue_context_release_command(ran_ue,
                        NGAP_Cause_PR_misc, NGAP_CauseMisc_om_intervention,
                        NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;

        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMPLETE:
            ogs_debug("[%s] Configuration update complete", amf_ue->supi);

            amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_MM_CONF_UPDATE_SUCC);

            /*
             * TS24.501
             * 5.3.3 Temporary identities
             *
             * The AMF shall assign a new 5G-GUTI for a particular UE:
             *
             * a) during a successful initial registration procedure;
             * b) during a successful registration procedure
             *    for mobility registration update; and
             * c) after a successful service request procedure invoked
             *    as a response to a paging request from the network and
             *    before the release of the N1 NAS signalling connection
             *    as specified in subclause 5.4.4.1.
             *
             * The AMF should assign a new 5G-GUTI for a particular UE
             * during a successful registration procedure
             * for periodic registration update.
             *
             * The AMF may assign a new 5G-GUTI at any time for a particular UE
             * by performing the generic UE configuration update procedure.
             */
            if (amf_ue->next.m_tmsi) {
                amf_ue_confirm_guti(amf_ue);
            } else {
                ogs_info("[%s] No GUTI allocated", amf_ue->supi);
            }

            CLEAR_AMF_UE_TIMER(amf_ue->t3555);
            break;

        case OGS_NAS_5GS_UL_NAS_TRANSPORT:
            if (!h.integrity_protected || !SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_error("No Security Context");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            gmm_handle_ul_nas_transport(
                    ran_ue, amf_ue, &nas_message->gmm.ul_nas_transport);
            break;

        case OGS_NAS_5GS_REGISTRATION_COMPLETE:
            ogs_error("[%s] Registration complete in INVALID-STATE",
                        amf_ue->supi);
            break;

        default:
            ogs_error("Unknown message [%d]", nas_message->gmm.h.message_type);
        }
        break;

    default:
        ogs_fatal("Unknown event[%s]", amf_event_get_name(e));
        ogs_assert_if_reached();
    }
}

void gmm_state_authentication(ogs_fsm_t *s, amf_event_t *e)
{
    int r, rv;
    ogs_nas_5gmm_cause_t gmm_cause;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;

    ogs_nas_5gs_message_t *nas_message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_nas_5gs_authentication_failure_t *authentication_failure = NULL;
    ogs_nas_authentication_failure_parameter_t
        *authentication_failure_parameter = NULL;

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

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case AMF_EVENT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        ran_ue = ran_ue_cycle(amf_ue->ran_ue);
        ogs_assert(ran_ue);

        h.type = e->nas.type;

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_AUTHENTICATION_RESPONSE:
            rv = gmm_handle_authentication_response(
                    amf_ue, &nas_message->gmm.authentication_response);

            if (rv != OGS_OK) {
                r = nas_5gs_send_authentication_reject(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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

            amf_metrics_inst_by_cause_add(authentication_failure->gmm_cause,
                    AMF_METR_CTR_AMF_AUTH_FAIL, 1);

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
                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                        amf_nausf_auth_build_authenticate,
                        amf_ue, 0, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;

            case OGS_5GMM_CAUSE_SYNCH_FAILURE:
                ogs_warn("Authentication failure(Synch failure)");
                if (authentication_failure_parameter->length != OGS_AUTS_LEN) {
                    ogs_error("Invalid AUTS Length [%d]",
                            authentication_failure_parameter->length);
                    break;
                }
                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                        amf_nausf_auth_build_authenticate,
                        amf_ue, 0, authentication_failure_parameter->auts);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;

            default:
                ogs_error("Unknown GMM_CAUSE{%d] in Authentication"
                        " failure",
                        authentication_failure->gmm_cause);
                break;
            }

            r = nas_5gs_send_authentication_reject(amf_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);

            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_warn("Registration request");
            gmm_cause = gmm_handle_registration_request(
                    amf_ue, h, e->ngap.code,
                    &nas_message->gmm.registration_request);
            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("[%s] gmm_handle_registration_request() failed [%d]",
                            amf_ue->suci, gmm_cause);
                r = nas_5gs_send_registration_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            r = amf_ue_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                    amf_nausf_auth_build_authenticate, amf_ue, 0, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            break;

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->suci, nas_message->gmm.gmm_status.gmm_cause);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE:
            ogs_warn("[%s] Deregistration request", amf_ue->supi);

            gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;

        default:
            ogs_error("Unknown message[%d]", nas_message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVENT_5GMM_TIMER:
        switch (e->h.timer_id) {
        case AMF_TIMER_T3560:
            if (amf_ue->t3560.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3560)->max_count) {
                ogs_warn("[%s] Retransmission failed. Stop retransmission",
                        amf_ue->suci);
                r = nas_5gs_send_authentication_reject(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3560.retry_count++;
                r = nas_5gs_send_authentication_request(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;
        default:
            ogs_error("[%s] Unknown timer[%s:%d]", amf_ue->suci,
                    amf_timer_get_name(e->h.timer_id), e->h.timer_id);
            break;
        }
        break;
    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
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
                    r = nas_5gs_send_gmm_reject_from_sbi(
                            amf_ue, sbi_message->res_status);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
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
                        r = nas_5gs_send_authentication_reject(amf_ue);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                    }
                    break;
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    rv = amf_nausf_auth_handle_authenticate_confirmation(
                            amf_ue, sbi_message);
                    if (rv != OGS_OK) {
                        ogs_error("[%s] Cannot handle SBI message",
                                amf_ue->suci);
                        r = nas_5gs_send_authentication_reject(amf_ue);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
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

        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)
            if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                sbi_message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT &&
                sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                ogs_error("[%s] HTTP response error [%d]",
                        amf_ue->supi, sbi_message->res_status);
            }

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    /*
                     * Issue #2733
                     *
                     * We need to ignore this message in this state.
                     */
                    ogs_error("[%s] Ignore SBI message", amf_ue->supi);
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
    int r;
    ogs_nas_5gmm_cause_t gmm_cause;
    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_AMF_UE_TIMER(amf_ue->t3560);
        r = nas_5gs_send_security_mode_command(amf_ue);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case AMF_EVENT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        ran_ue = ran_ue_cycle(amf_ue->ran_ue);
        ogs_assert(ran_ue);

        h.type = e->nas.type;

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_SECURITY_MODE_COMPLETE:
            ogs_debug("[%s] Security mode complete", amf_ue->supi);

        /*
         * TS24.501
         * Section 4.4.4.3
         * Integrity checking of NAS signalling messages in the AMF
         *
         * Once the secure exchange of NAS messages has been established
         * for the NAS signalling connection, the receiving 5GMM entity
         * in the AMF shall not process any NAS signalling messages
         * unless they have been successfully integrity checked by the NAS.
         * If any NAS signalling message, having not successfully passed
         * the integrity check, is received, then the NAS in the AMF shall
         * discard that message. If any NAS signalling message is received,
         * as not integrity protected even though the secure exchange
         * of NAS messages has been established, then the NAS shall discard
         * this message.
         */
            if (h.integrity_protected == 0) {
                ogs_error("[%s] Security-mode : No Integrity Protected",
                        amf_ue->supi);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_error("[%s] No Security Context", amf_ue->supi);
                break;
            }

            /*
             * If the OLD RAN_UE is being maintained in AMF-UE Context,
             * it deletes the NG Context after exchanging
             * the UEContextReleaseCommand/Complete with the gNB
             */
            CLEAR_NG_CONTEXT(amf_ue);

            CLEAR_AMF_UE_TIMER(amf_ue->t3560);

            gmm_cause = gmm_handle_security_mode_complete(
                    amf_ue, &nas_message->gmm.security_mode_complete);
            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("[%s] gmm_handle_security_mode_complete() "
                            "failed [%d] in type [%d]",
                            amf_ue->suci, gmm_cause, amf_ue->nas.message_type);
                r = nas_5gs_send_gmm_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            ogs_kdf_kgnb_and_kn3iwf(
                    amf_ue->kamf, amf_ue->ul_count.i32,
                    amf_ue->nas.access_type, amf_ue->kgnb);
            ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->kgnb, amf_ue->nh);
            amf_ue->nhcc = 1;

            r = amf_ue_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NUDM_UECM, NULL,
                    amf_nudm_uecm_build_registration, amf_ue, 0, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            if (amf_ue->nas.message_type == OGS_NAS_5GS_REGISTRATION_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
            } else if (amf_ue->nas.message_type ==
                        OGS_NAS_5GS_SERVICE_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_registered);
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
            gmm_cause = gmm_handle_registration_request(
                    amf_ue, h, e->ngap.code,
                    &nas_message->gmm.registration_request);
            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("[%s] gmm_handle_registration_request() failed [%d]",
                            amf_ue->suci, gmm_cause);
                r = nas_5gs_send_registration_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            r = amf_ue_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                    amf_nausf_auth_build_authenticate, amf_ue, 0, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;

        case OGS_NAS_5GS_SERVICE_REQUEST:
            ogs_info("[%s] Service request", amf_ue->supi);
            r = nas_5gs_send_service_reject(ran_ue, amf_ue,
                    OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->supi, nas_message->gmm.gmm_status.gmm_cause);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE:
            ogs_warn("[%s] Deregistration request", amf_ue->supi);

            gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;

        default:
            ogs_error("Unknown message[%d]", nas_message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVENT_5GMM_TIMER:
        switch (e->h.timer_id) {
        case AMF_TIMER_T3560:
            if (amf_ue->t3560.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3560)->max_count) {
                ogs_warn("[%s] Retransmission failed. Stop", amf_ue->supi);
                r = nas_5gs_send_gmm_reject(amf_ue->ran_ue, amf_ue,
                        OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3560.retry_count++;
                r = nas_5gs_send_security_mode_command(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->h.timer_id), e->h.timer_id);
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
    int rv, r, state, xact_count = 0;
    ogs_nas_5gmm_cause_t gmm_cause;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;
    ogs_nas_security_header_type_t h;

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

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);
        state = e->h.sbi.state;

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                    ogs_error("[%s] HTTP response error [%d]",
                            amf_ue->supi, sbi_message->res_status);
                    r = nas_5gs_send_gmm_reject(
                            amf_ue->ran_ue, amf_ue,
                            OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                    break;
                }

                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    r = amf_ue_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                            amf_nudm_sdm_build_get,
                            amf_ue, state,
                            (char *)OGS_SBI_RESOURCE_NAME_AM_DATA);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
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
            CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                if ((sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) &&
                    (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED)) {
                    ogs_error("[%s] HTTP response error [%d]",
                            amf_ue->supi, sbi_message->res_status);
                    r = nas_5gs_send_gmm_reject(
                            amf_ue->ran_ue, amf_ue,
                            OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                    break;
                }

                rv = amf_nudm_sdm_handle_provisioned(
                        amf_ue, state, sbi_message);
                if (rv != OGS_OK) {
                    ogs_error("[%s] amf_nudm_sdm_handle_provisioned(%s) failed",
                            amf_ue->supi, sbi_message->h.resource.component[1]);
                    OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                    break;
                }
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
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    rv = amf_npcf_am_policy_control_handle_create(
                            amf_ue, sbi_message);
                    if (rv != OGS_OK) {
                        ogs_error("[%s] amf_npcf_am_policy_control"
                                "_handle_create() failed", amf_ue->supi);
                        OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
                        break;
                    }

                    ogs_assert(amf_ue->nas.message_type ==
                            OGS_NAS_5GS_REGISTRATION_REQUEST);
                    CLEAR_AMF_UE_TIMER(amf_ue->t3550);
                    r = nas_5gs_send_registration_accept(amf_ue);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    /* In nsmf-handler.c
                     *
                     * 1. AMF_SESS_STORE_N2_TRANSFER
                     * 2. if PCF_AM_POLICY is NOT associated
                     * 3. AMF sends npcf-am-policy-control/create to PCF
                     *
                     * In gmm-sm.c
                     * 4. Send Registration Accept
                     * 5. We should clear N2 transfer
                     *    (PDUSessionResourceSetupRequest)
                     */
                    AMF_UE_CLEAR_N2_TRANSFER(
                            amf_ue, pdu_session_resource_setup_request);

                    if (!amf_ue->next.m_tmsi)
                        OGS_FSM_TRAN(s, &gmm_state_registered);
                    break;

                DEFAULT
                    ogs_error("Unknown method [%s]", sbi_message->h.method);
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

    case AMF_EVENT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        ran_ue = ran_ue_cycle(amf_ue->ran_ue);
        ogs_assert(ran_ue);

        h.type = e->nas.type;

        xact_count = amf_sess_xact_count(amf_ue);

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_COMPLETE:
            ogs_info("[%s] Registration complete", amf_ue->supi);

            CLEAR_AMF_UE_TIMER(amf_ue->t3550);

            /*
             * TS24.501
             * 5.3.3 Temporary identities
             *
             * The AMF shall assign a new 5G-GUTI for a particular UE:
             *
             * a) during a successful initial registration procedure;
             * b) during a successful registration procedure
             *    for mobility registration update; and
             * c) after a successful service request procedure invoked
             *    as a response to a paging request from the network and
             *    before the release of the N1 NAS signalling connection
             *    as specified in subclause 5.4.4.1.
             *
             * The AMF should assign a new 5G-GUTI for a particular UE
             * during a successful registration procedure
             * for periodic registration update.
             *
             * The AMF may assign a new 5G-GUTI at any time for a particular UE
             * by performing the generic UE configuration update procedure.
             */
            if (amf_ue->next.m_tmsi) {
                amf_ue_confirm_guti(amf_ue);
            } else {
                ogs_error("[%s] No GUTI allocated", amf_ue->supi);
            }

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
            r = nas_5gs_send_configuration_update_command(amf_ue, &param);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            switch (amf_ue->nas.registration.value) {
            case OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL:
                amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_RM_REG_INIT_SUCC);
                break;
            case OGS_NAS_5GS_REGISTRATION_TYPE_MOBILITY_UPDATING:
                amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_RM_REG_MOB_SUCC);
                break;
            case OGS_NAS_5GS_REGISTRATION_TYPE_PERIODIC_UPDATING:
                amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_RM_REG_PERIOD_SUCC);
                break;
            case OGS_NAS_5GS_REGISTRATION_TYPE_EMERGENCY:
                amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_RM_REG_EMERG_SUCC);
                break;
            default:
                ogs_error("Unknown reg_type[%d]",
                        amf_ue->nas.registration.value);
            }
            OGS_FSM_TRAN(s, &gmm_state_registered);
            break;

        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_warn("Registration request");
            gmm_cause = gmm_handle_registration_request(
                    amf_ue, h, e->ngap.code,
                    &nas_message->gmm.registration_request);
            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("[%s] gmm_handle_registration_request() failed [%d]",
                            amf_ue->suci, gmm_cause);
                r = nas_5gs_send_registration_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_sbi_send_release_all_sessions(
                    ran_ue, amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);

            if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
                amf_sess_xact_count(amf_ue) == xact_count) {
                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                        amf_nausf_auth_build_authenticate,
                        amf_ue, 0, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;

        case OGS_NAS_5GS_SERVICE_REQUEST:
            ogs_info("[%s] Service request", amf_ue->supi);
            r = nas_5gs_send_service_reject(ran_ue, amf_ue,
                OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;

        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("[%s] 5GMM STATUS : Cause[%d]",
                    amf_ue->supi, nas_message->gmm.gmm_status.gmm_cause);
            break;

        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE:
            ogs_warn("[%s] Deregistration request", amf_ue->supi);

            gmm_handle_deregistration_request(
                    amf_ue, &nas_message->gmm.deregistration_request_from_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;

        default:
            ogs_error("Unknown message [%d]", nas_message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVENT_5GMM_TIMER:
        switch (e->h.timer_id) {
        case AMF_TIMER_T3550:
            if (amf_ue->t3550.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3550)->max_count) {
                ogs_warn("[%s] Retransmission failed. Stop retransmission",
                        amf_ue->suci);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3550.retry_count++;
                r = nas_5gs_send_registration_accept(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;
        default:
            ogs_error("[%s] Unknown timer[%s:%d]", amf_ue->suci,
                    amf_timer_get_name(e->h.timer_id), e->h.timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}

void gmm_state_ue_context_will_remove(ogs_fsm_t *s, amf_event_t *e)
{
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

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

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        amf_ue_remove(amf_ue);
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
    }
}

void gmm_state_exception(ogs_fsm_t *s, amf_event_t *e)
{
    int xact_count = 0, r;
    ogs_nas_5gmm_cause_t gmm_cause;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_message_t *nas_message = NULL;
    ogs_nas_security_header_type_t h;

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

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        AMF_UE_CLEAR_PAGING_INFO(amf_ue);
        AMF_UE_CLEAR_N2_TRANSFER(amf_ue, pdu_session_resource_setup_request);
        AMF_UE_CLEAR_5GSM_MESSAGE(amf_ue);
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

        xact_count = amf_sess_xact_count(amf_ue);

        amf_sbi_send_release_all_sessions(
                amf_ue->ran_ue, amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);

        if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
            amf_sess_xact_count(amf_ue) == xact_count) {
            r = ngap_send_amf_ue_context_release_command(amf_ue,
                    NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                    NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case AMF_EVENT_5GMM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        ran_ue = ran_ue_cycle(amf_ue->ran_ue);
        ogs_assert(ran_ue);

        h.type = e->nas.type;

        xact_count = amf_sess_xact_count(amf_ue);

        switch (nas_message->gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_info("Registration request");
            gmm_cause = gmm_handle_registration_request(
                    amf_ue, h, e->ngap.code,
                    &nas_message->gmm.registration_request);
            if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                ogs_error("gmm_handle_registration_request() failed [%d]",
                            gmm_cause);
                r = nas_5gs_send_registration_reject(ran_ue, amf_ue, gmm_cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            if (!AMF_UE_HAVE_SUCI(amf_ue)) {
                CLEAR_AMF_UE_TIMER(amf_ue->t3570);
                r = nas_5gs_send_identity_request(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                OGS_FSM_TRAN(s, &gmm_state_de_registered);
                break;
            }

            if (h.integrity_protected && SECURITY_CONTEXT_IS_VALID(amf_ue)) {

                /*
                 * If the OLD RAN_UE is being maintained in AMF-UE Context,
                 * it deletes the NG Context after exchanging
                 * the UEContextReleaseCommand/Complete with the gNB
                 */
                CLEAR_NG_CONTEXT(amf_ue);

                gmm_cause = gmm_handle_registration_update(
                        ran_ue, amf_ue, &nas_message->gmm.registration_request);
                if (gmm_cause != OGS_5GMM_CAUSE_REQUEST_ACCEPTED) {
                    ogs_error("[%s] gmm_handle_registration_update() "
                                "failed [%d]", amf_ue->suci, gmm_cause);
                    r = nas_5gs_send_registration_reject(
                            ran_ue, amf_ue, gmm_cause);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    OGS_FSM_TRAN(s, gmm_state_exception);
                    break;
                }

                if (amf_sess_xact_count(amf_ue) == xact_count) {

                    if (amf_update_allowed_nssai(amf_ue) == false) {
                        ogs_error("No Allowed-NSSAI");
                        r = nas_5gs_send_gmm_reject(
                                ran_ue, amf_ue,
                                OGS_5GMM_CAUSE_NO_NETWORK_SLICES_AVAILABLE);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        OGS_FSM_TRAN(s, gmm_state_exception);
                        break;
                    }

                    if (!PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                        r = amf_ue_sbi_discover_and_send(
                                OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                NULL,
                                amf_npcf_am_policy_control_build_create,
                                amf_ue, 0, NULL);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
                        break;
                    }

                    CLEAR_AMF_UE_TIMER(amf_ue->t3550);
                    r = nas_5gs_send_registration_accept(amf_ue);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                }

                if (amf_ue->next.m_tmsi)
                    OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
                else
                    OGS_FSM_TRAN(s, &gmm_state_registered);

            } else {

                amf_sbi_send_release_all_sessions(
                        ran_ue, amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);

                if (!AMF_SESSION_RELEASE_PENDING(amf_ue) &&
                    amf_sess_xact_count(amf_ue) == xact_count) {
                    r = amf_ue_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                            amf_nausf_auth_build_authenticate,
                            amf_ue, 0, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
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
