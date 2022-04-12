/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#include "context.h"
#include "gtp-path.h"
#include "pfcp-path.h"
#include "gy-handler.h"
#include "binding.h"

static void urr_enable_total_volume_threshold(smf_sess_t *sess, ogs_pfcp_urr_t *urr,
                                              uint64_t total_volume_threshold)
{
    ogs_debug("Adding CC Grant total_octets=%" PRIu64, total_volume_threshold);
    urr->meas_method |= OGS_PFCP_MEASUREMENT_METHOD_VOLUME;
    urr->rep_triggers.volume_threshold = 1;
    urr->vol_threshold.tovol = 1;
    urr->vol_threshold.total_volume = total_volume_threshold;
    ogs_assert(sess->pfcp_node);
    if (sess->pfcp_node->up_function_features.mnop)
        urr->meas_info.mnop = 1;
}

static void urr_disable_total_volume_threshold(ogs_pfcp_urr_t *urr)
{
    urr->meas_method &= ~OGS_PFCP_MEASUREMENT_METHOD_VOLUME;
    urr->rep_triggers.volume_threshold = 0;
    urr->vol_threshold.tovol = 0;
    urr->vol_threshold.total_volume = 0;
}

static void urr_update_total_volume_threshold(smf_sess_t *sess, ogs_pfcp_urr_t *urr, ogs_diam_gy_message_t *gy_message)
{
    if (gy_message->cca.granted.cc_total_octets_present)
        urr_enable_total_volume_threshold(sess, urr, gy_message->cca.granted.cc_total_octets);
    else
        urr_disable_total_volume_threshold(urr);
}

static void urr_update_time_threshold(ogs_pfcp_urr_t *urr, ogs_diam_gy_message_t *gy_message)
{
    uint32_t time_threshold;
    if (gy_message->cca.validity_time && (gy_message->cca.granted.cc_time_present && gy_message->cca.granted.cc_time > 0))
        time_threshold = (gy_message->cca.validity_time <= gy_message->cca.granted.cc_time) ?
                         gy_message->cca.validity_time : gy_message->cca.granted.cc_time;
    else if (gy_message->cca.validity_time)
        time_threshold = gy_message->cca.validity_time;
    else if (gy_message->cca.granted.cc_time_present && gy_message->cca.granted.cc_time > 0)
        time_threshold = gy_message->cca.granted.cc_time;
    else
        time_threshold = 0;
    if (time_threshold) {
        ogs_debug("Adding CC Grant time=%" PRIu32, time_threshold);
        urr->meas_method |= OGS_PFCP_MEASUREMENT_METHOD_DURATION;
        urr->rep_triggers.time_threshold = 1;
        urr->time_threshold = time_threshold;
        urr->meas_info.istm  = 1;
    } else {
        urr->meas_method &= ~OGS_PFCP_MEASUREMENT_METHOD_DURATION;
        urr->rep_triggers.time_threshold = 0;
        urr->time_threshold = 0;
    }
}

/* Returns ER_DIAMETER_SUCCESS on success, Diameter error code on failue. */
uint32_t smf_gy_handle_cca_initial_request(
        smf_sess_t *sess, ogs_diam_gy_message_t *gy_message,
        ogs_gtp_xact_t *gtp_xact)
{
    smf_bearer_t *bearer;

    ogs_assert(sess);
    ogs_assert(gy_message);
    ogs_assert(gtp_xact);

    ogs_debug("[Gy CCA Initial]");
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (gy_message->result_code != ER_DIAMETER_SUCCESS)
        return gy_message->err ? *gy_message->err :
                                 ER_DIAMETER_AUTHENTICATION_REJECTED;

    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    if (!bearer->urr)
        bearer->urr = ogs_pfcp_urr_add(&sess->pfcp);
    ogs_assert(bearer->urr);

    /* Configure based on what we received from OCS: */
    urr_update_time_threshold(bearer->urr, gy_message);
    urr_update_total_volume_threshold(sess, bearer->urr, gy_message);

    /* Associate acconting URR each direction PDR: */
    ogs_pfcp_pdr_associate_urr(bearer->ul_pdr, bearer->urr);
    ogs_pfcp_pdr_associate_urr(bearer->dl_pdr, bearer->urr);
    return ER_DIAMETER_SUCCESS;
}

void smf_gy_handle_cca_update_request(
        smf_sess_t *sess, ogs_diam_gy_message_t *gy_message,
        ogs_pfcp_xact_t *pfcp_xact)
{
    ogs_pfcp_urr_t *urr = NULL;
    smf_bearer_t *bearer;
    int rv;
    uint64_t modify_flags = 0;
    ogs_pfcp_measurement_method_t prev_meas_method;
    ogs_pfcp_reporting_triggers_t prev_rep_triggers;
    ogs_pfcp_volume_threshold_t prev_vol_threshold;
    ogs_pfcp_time_threshold_t prev_time_threshold;

    ogs_assert(sess);
    ogs_assert(gy_message);
    ogs_assert(pfcp_xact);

    ogs_debug("[Gy CCA Update]");
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (gy_message->result_code != ER_DIAMETER_SUCCESS) {
        ogs_warn("Gy CCA Update Diameter failure: res=%u err=%u",
            gy_message->result_code, *gy_message->err);
        // TODO: generate new gtp_xact from sess here? */
        //ogs_assert(OGS_OK ==
        //    smf_epc_pfcp_send_session_deletion_request(sess, gtp_xact));
        return;
    }

    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    urr = bearer->urr;
    ogs_assert(urr);
    prev_meas_method = urr->meas_method;
    prev_rep_triggers = urr->rep_triggers;
    prev_vol_threshold = urr->vol_threshold;
    prev_time_threshold = urr->time_threshold;

    urr_update_time_threshold(urr, gy_message);
    urr_update_total_volume_threshold(sess, urr, gy_message);
    ogs_pfcp_pdr_associate_urr(bearer->ul_pdr, urr);

    if (urr->meas_method != prev_meas_method)
        modify_flags |= OGS_PFCP_MODIFY_URR_MEAS_METHOD;
    if (urr->rep_triggers.time_threshold != prev_rep_triggers.time_threshold ||
        urr->rep_triggers.volume_threshold != prev_rep_triggers.volume_threshold)
        modify_flags |= OGS_PFCP_MODIFY_URR_REPORT_TRIGGER;

    if (urr->time_threshold != prev_time_threshold)
        modify_flags |= OGS_PFCP_MODIFY_URR_TIME_THRESH;

    if (urr->vol_threshold.tovol != prev_vol_threshold.tovol ||
        urr->vol_threshold.total_volume != prev_vol_threshold.total_volume)
        modify_flags |= OGS_PFCP_MODIFY_URR_VOLUME_THRESH;

    /* Send PFCP Session Modification Request if we need to update the params. */
    if (modify_flags) {
        modify_flags |= OGS_PFCP_MODIFY_URR|OGS_PFCP_MODIFY_UL_ONLY;
        rv = smf_epc_pfcp_send_session_modification_request(sess, pfcp_xact,
            modify_flags,
            OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            OGS_GTP1_CAUSE_REACTIACTION_REQUESTED);
        ogs_assert(rv == OGS_OK);
    }
}

void smf_gy_handle_cca_termination_request(
        smf_sess_t *sess, ogs_diam_gy_message_t *gy_message,
        ogs_gtp_xact_t *gtp_xact)
{
    ogs_assert(sess);
    ogs_assert(gy_message);
    ogs_assert(gtp_xact);

    ogs_debug("[SMF] Delete Session Response");
    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

     if (gtp_xact) {
         /*
          * 1. MME sends Delete Session Request to SGW/SMF.
          * 2. SMF sends Delete Session Response to SGW/MME.
          */
         switch (gtp_xact->gtp_version) {
         case 1:
             ogs_assert(OGS_OK == smf_gtp1_send_delete_pdp_context_response(sess, gtp_xact));
             break;
         case 2:
             ogs_assert(OGS_OK == smf_gtp_send_delete_session_response(sess, gtp_xact));
             break;
         }
     } else {
         /*
          * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to SGW/MME.
          * 2. MME sends Delete Bearer Response to SGW/SMF.
          *
          * OR
          *
          * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to ePDG.
          * 2. ePDG sends Delete Bearer Response(DEFAULT BEARER) to SMF.
          *
          * Note that the following messages are not processed here.
          * - Bearer Resource Command
          * - Delete Bearer Request/Response with DEDICATED BEARER.
          */
     }
     SMF_SESS_CLEAR(sess);
     return;
}

void smf_gy_handle_re_auth_request(
        smf_sess_t *sess, ogs_diam_gy_message_t *gy_message)
{
    /* TODO: find out what to do here */
}
