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

#include "s1ap-path.h"
#include "nas-path.h"
#include "sgsap-path.h"
#include "mme-gtp-path.h"
#include "mme-path.h"
#include "mme-fd-path.h"
#include "mme-sm.h"

void mme_send_delete_session_or_detach(enb_ue_t *enb_ue, mme_ue_t *mme_ue)
{
    int r, xact_count;
    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    xact_count = mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR);

    switch (mme_ue->detach_type) {
    case MME_DETACH_TYPE_REQUEST_FROM_UE:
        ogs_debug("Detach Request from UE");
        mme_gtp_send_delete_all_sessions(
                enb_ue, mme_ue, OGS_GTP_DELETE_SEND_DETACH_ACCEPT);

        if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
            mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
                xact_count) {
            r = nas_eps_send_detach_accept(mme_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        break;

    /* MME Explicit Detach, ie: O&M Procedures */
    case MME_DETACH_TYPE_MME_EXPLICIT:
        ogs_fatal("Not Implemented : MME_DETACH_TYPE_MME_EXPLICIT");
        ogs_assert_if_reached();
        break;

    /* HSS Explicit Detach, ie: Subscription Withdrawl Cancel Location
     *
     * TS23.401 - V16.10.0
     * Ch 5.3.8 Detach procedure
     * Ch 5.3.8.4 HSS-initiated Detach procedure
     */
    case MME_DETACH_TYPE_HSS_EXPLICIT:
        ogs_debug("Explicit HSS Detach");
        mme_gtp_send_delete_all_sessions(
                enb_ue, mme_ue, OGS_GTP_DELETE_NO_ACTION);
        break;

    /* MME Implicit Detach, ie: Lost Communication
     * TS23.401 - V16.10.0
     * Ch 5.3.8.3 MME-initiated Detach procedure (Without Step 1)
     */
    case MME_DETACH_TYPE_MME_IMPLICIT:
        ogs_warn("[%s] Implicit MME Detach", mme_ue->imsi_bcd);
        mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
            OGS_GTP_DELETE_SEND_RELEASE_WITH_UE_CONTEXT_REMOVE);

        if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
            mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
                xact_count) {
            enb_ue_t *enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
            if (enb_ue) {
                ogs_warn("[%s] UEContextReleaseCommand Sent", mme_ue->imsi_bcd);
                ogs_assert(OGS_OK ==
                    s1ap_send_ue_context_release_command(enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0));
            } else {
                ogs_warn("[%s] MME-UE Context Removed", mme_ue->imsi_bcd);
                mme_ue_remove(mme_ue);
            }
        }
        break;

    /* HSS Implicit Detach, ie: MME-UPDATE-PROCEDURE
     *
     * TS23.401 - V16.10.0
     * Ch 5.3.2 Attach procedure
     * Ch 5.3.2.1 E-UTRAN Initial Attach
     *
     * 9. The HSS sends Cancel Location (IMSI, Cancellation Type)
     * to the old MME. The old MME acknowledges with Cancel Location Ack (IMSI)
     * and removes the MM and bearer contexts. If the ULR-Flags indicates
     * "Initial-Attach-Indicator" and the HSS has the SGSN registration,
     * then the HSS sends Cancel Location (IMSI, Cancellation Type)
     * to the old SGSN. The Cancellation Type indicates the old MME/SGSN
     * to release the old Serving GW resource.
     */
    case MME_DETACH_TYPE_HSS_IMPLICIT:
        ogs_debug("Implicit HSS Detach");
        mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
            OGS_GTP_DELETE_SEND_RELEASE_WITH_UE_CONTEXT_REMOVE);
        break;

    default:
        ogs_fatal("    Invalid OGS_NAS_EPS TYPE[%d]", mme_ue->detach_type);
        ogs_assert_if_reached();
    }
}

void mme_send_delete_session_or_mme_ue_context_release(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue)
{
    int r, xact_count = 0;

    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    xact_count = mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR);

    mme_gtp_send_delete_all_sessions(enb_ue, mme_ue,
            OGS_GTP_DELETE_SEND_RELEASE_WITH_UE_CONTEXT_REMOVE);

    if (!MME_SESSION_RELEASE_PENDING(mme_ue) &&
        mme_ue_xact_count(mme_ue, OGS_GTP_LOCAL_ORIGINATOR) ==
            xact_count) {
        if (enb_ue) {
            r = s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else {
            ogs_warn("[%s] No S1 Context", mme_ue->imsi_bcd);
        }
    }
}

void mme_send_release_access_bearer_or_ue_context_release(enb_ue_t *enb_ue)
{
    int r;
    mme_ue_t *mme_ue = NULL;
    ogs_assert(enb_ue);

    mme_ue = mme_ue_find_by_id(enb_ue->mme_ue_id);
    if (mme_ue) {
        ogs_debug("[%s] Release access bearer request", mme_ue->imsi_bcd);
        ogs_assert(OGS_OK ==
            mme_gtp_send_release_access_bearers_request(
                enb_ue, mme_ue,
                OGS_GTP_RELEASE_SEND_UE_CONTEXT_RELEASE_COMMAND));
    } else {
        ogs_debug("No UE Context");
        ogs_assert(enb_ue->relcause.group);
        r = s1ap_send_ue_context_release_command(enb_ue,
                enb_ue->relcause.group, enb_ue->relcause.cause,
                S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE, 0);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }
}

void mme_send_after_paging(mme_ue_t *mme_ue, bool failed)
{
    int r;
    mme_bearer_t *bearer = NULL;

    ogs_assert(mme_ue);

    switch (mme_ue->paging.type) {
    case MME_PAGING_TYPE_DOWNLINK_DATA_NOTIFICATION:
        bearer = mme_bearer_find_by_id(
                OGS_POINTER_TO_UINT(mme_ue->paging.data));
        if (!bearer) {
            ogs_error("No Bearer [%d]", mme_ue->paging.type);
            goto cleanup;
        }

        if (failed == true) {
            ogs_assert(OGS_OK ==
                mme_gtp_send_downlink_data_notification_ack(
                    bearer, OGS_GTP2_CAUSE_UNABLE_TO_PAGE_UE));
        } else {
            ogs_assert(OGS_OK ==
                mme_gtp_send_downlink_data_notification_ack(
                    bearer, OGS_GTP2_CAUSE_REQUEST_ACCEPTED));
        }
        break;
    case MME_PAGING_TYPE_CREATE_BEARER:
        bearer = mme_bearer_find_by_id(
                OGS_POINTER_TO_UINT(mme_ue->paging.data));
        if (!bearer) {
            ogs_error("No Bearer [%d]", mme_ue->paging.type);
            goto cleanup;
        }

        if (failed == true) {
            ogs_assert(OGS_OK ==
                mme_gtp_send_create_bearer_response(
                    bearer, OGS_GTP2_CAUSE_UNABLE_TO_PAGE_UE));
        } else {
            r = nas_eps_send_activate_dedicated_bearer_context_request(bearer);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        break;
    case MME_PAGING_TYPE_UPDATE_BEARER:
        bearer = mme_bearer_find_by_id(
                OGS_POINTER_TO_UINT(mme_ue->paging.data));
        if (!bearer) {
            ogs_error("No Bearer [%d]", mme_ue->paging.type);
            goto cleanup;
        }

        if (failed == true) {
            ogs_assert(OGS_OK ==
                mme_gtp_send_update_bearer_response(
                    bearer, OGS_GTP2_CAUSE_UNABLE_TO_PAGE_UE));
        } else {
            ogs_gtp_xact_t *xact = NULL;

            /* Get the first Entry */
            ogs_list_for_each_entry(
                    &bearer->update.xact_list, xact, to_update_node) {
                break;
            }
            if (!xact) {
                ogs_error("No GTP xact");
                goto cleanup;
            }

            /*
             * MME must wait for Modify Bearer Context Accept
             * before sending Update Bearer Response,
             * To check this, start a peer timer to check it.
             */
            ogs_timer_start(xact->tm_peer,
                    ogs_local_conf()->time.message.gtp.t3_response_duration);

            r = nas_eps_send_modify_bearer_context_request(bearer,
                    (xact->update_flags &
                        OGS_GTP_MODIFY_QOS_UPDATE) ? 1 : 0,
                    (xact->update_flags &
                        OGS_GTP_MODIFY_TFT_UPDATE) ? 1 : 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        break;
    case MME_PAGING_TYPE_DELETE_BEARER:
        bearer = mme_bearer_find_by_id(
                OGS_POINTER_TO_UINT(mme_ue->paging.data));
        if (!bearer) {
            ogs_error("No Bearer [%d]", mme_ue->paging.type);
            goto cleanup;
        }

        if (failed == true) {
            ogs_assert(OGS_OK ==
                mme_gtp_send_delete_bearer_response(
                    bearer, OGS_GTP2_CAUSE_UNABLE_TO_PAGE_UE));
        } else {
            r = nas_eps_send_deactivate_bearer_context_request(bearer);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        break;
    case MME_PAGING_TYPE_CS_CALL_SERVICE:
        if (failed == true) {
            ogs_assert(OGS_OK ==
                sgsap_send_paging_reject(
                    mme_ue, SGSAP_SGS_CAUSE_UE_UNREACHABLE));
        } else {
            /* Nothing */
        }
        break;
    case MME_PAGING_TYPE_SMS_SERVICE:
        if (failed == true) {
            ogs_assert(OGS_OK ==
                sgsap_send_paging_reject(
                    mme_ue, SGSAP_SGS_CAUSE_UE_UNREACHABLE));
        } else {
            ogs_assert(OGS_OK ==
                sgsap_send_service_request(
                    mme_ue, SGSAP_EMM_CONNECTED_MODE));
        }
        break;
    case MME_PAGING_TYPE_DETACH_TO_UE:
        if (failed == true) {
            /* Nothing */
            ogs_warn("MME-initiated Detach cannot be invoked");
        } else {
            r = nas_eps_send_detach_request(mme_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            if (MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
                ogs_assert(OGS_OK == sgsap_send_detach_indication(mme_ue));
            } else {
                enb_ue_t *enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
                if (enb_ue)
                    mme_send_delete_session_or_detach(enb_ue, mme_ue);
                else
                    ogs_error("ENB-S1 Context has already been removed");
            }
        }
        break;
    default:
        ogs_fatal("Invalid Paging Type[%d]", mme_ue->paging.type);
        ogs_assert_if_reached();
    }

cleanup:
    CLEAR_SERVICE_INDICATOR(mme_ue);
    MME_CLEAR_PAGING_INFO(mme_ue);
    /* the above will clear the failure flag, restore it if we failed */
    if (failed)
        mme_ue->paging.failed = true;
}
