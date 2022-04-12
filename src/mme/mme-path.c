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

#include "s1ap-path.h"
#include "nas-path.h"
#include "mme-gtp-path.h"
#include "mme-path.h"

void mme_send_delete_session_or_detach(mme_ue_t *mme_ue)
{
    ogs_assert(mme_ue);

    if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
        mme_gtp_send_delete_all_sessions(mme_ue,
                OGS_GTP_DELETE_SEND_DETACH_ACCEPT);
    } else {
        ogs_assert(OGS_OK ==
            nas_eps_send_detach_accept(mme_ue));
    }
}

void mme_send_delete_session_or_mme_ue_context_release(mme_ue_t *mme_ue)
{
    ogs_assert(mme_ue);

    if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
        mme_gtp_send_delete_all_sessions(mme_ue,
                OGS_GTP_DELETE_SEND_UE_CONTEXT_RELEASE_COMMAND);
    } else {
        enb_ue_t *enb_ue = enb_ue_cycle(mme_ue->enb_ue);
        if (enb_ue) {
            ogs_assert(OGS_OK ==
                s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0));
        } else {
            ogs_warn("[%s] No S1 Context", mme_ue->imsi_bcd);
        }
    }
}

void mme_send_release_access_bearer_or_ue_context_release(enb_ue_t *enb_ue)
{
    mme_ue_t *mme_ue = NULL;
    ogs_assert(enb_ue);

    mme_ue = enb_ue->mme_ue;
    if (mme_ue) {
        ogs_debug("[%s] Release access bearer request", mme_ue->imsi_bcd);
        ogs_assert(OGS_OK ==
            mme_gtp_send_release_access_bearers_request(
                mme_ue, OGS_GTP_RELEASE_SEND_UE_CONTEXT_RELEASE_COMMAND));
    } else {
        ogs_debug("No UE Context");
        ogs_assert(OGS_OK ==
            s1ap_send_ue_context_release_command(enb_ue,
                S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE, 0));
    }
}

void mme_send_after_paging(mme_ue_t *mme_ue, uint8_t cause_value)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_assert(mme_ue);

    ogs_list_for_each(&mme_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            ogs_gtp_xact_t *xact = NULL;
            uint8_t type;

            xact = ogs_gtp_xact_cycle(bearer->current.xact);
            if (xact) {
                /*
                 * It may conflict with GTP transaction already used.
                 * To avoid this, check `xact->step` to see if
                 * the transaction has already been committed.
                 */
                type = xact->seq[xact->step-1].type;

                switch (type) {
                case OGS_GTP2_DOWNLINK_DATA_NOTIFICATION_TYPE:
                    ogs_assert(OGS_OK ==
                        mme_gtp_send_downlink_data_notification_ack(
                            bearer, cause_value));
                    break;
                case OGS_GTP2_CREATE_BEARER_REQUEST_TYPE:
                    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
                        ogs_assert(OGS_OK ==
                        nas_eps_send_activate_dedicated_bearer_context_request(
                                bearer));
                    } else {
                        ogs_assert(OGS_OK ==
                            mme_gtp_send_create_bearer_response(
                                bearer, cause_value));
                    }
                    break;
                case OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE:
                    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
                        ogs_assert(OGS_OK ==
                            nas_eps_send_modify_bearer_context_request(bearer,
                                (xact->update_flags &
                                    OGS_GTP_MODIFY_QOS_UPDATE) ? 1 : 0,
                                (xact->update_flags &
                                    OGS_GTP_MODIFY_TFT_UPDATE) ? 1 : 0));
                    } else {
                        ogs_assert(OGS_OK ==
                            mme_gtp_send_update_bearer_response(
                                bearer, cause_value));
                    }
                    break;
                case OGS_GTP2_DELETE_BEARER_REQUEST_TYPE:
                    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
                        ogs_assert(OGS_OK ==
                        nas_eps_send_deactivate_bearer_context_request(bearer));
                    } else {
                        ogs_assert(OGS_OK ==
                            mme_gtp_send_delete_bearer_response(
                                bearer, cause_value));
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
}
