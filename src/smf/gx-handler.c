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

#include "context.h"
#include "gtp-path.h"
#include "pfcp-path.h"
#include "gx-handler.h"
#include "bearer-binding.h"

static uint8_t gtp_cause_from_diameter(
        const uint32_t *dia_err, const uint32_t *dia_exp_err)
{
    if (dia_exp_err) {
    }
    if (dia_err) {
        switch (*dia_err) {
        case OGS_DIAM_UNKNOWN_SESSION_ID:
            return OGS_GTP_CAUSE_APN_ACCESS_DENIED_NO_SUBSCRIPTION;
        }
    }

    ogs_error("Unexpected Diameter Result Code %d/%d, defaulting to severe "
              "network failure",
              dia_err ? *dia_err : -1, dia_exp_err ? *dia_exp_err : -1);
    return OGS_GTP_CAUSE_UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER;
}

void smf_gx_handle_cca_initial_request(
        smf_sess_t *sess, ogs_diam_gx_message_t *gx_message,
        ogs_gtp_xact_t *gtp_xact)
{
    int i;
    ogs_assert(sess);
    ogs_assert(gx_message);
    ogs_assert(gtp_xact);

    ogs_debug("[PGW] Create Session Response");
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    if (gx_message->result_code != ER_DIAMETER_SUCCESS) {
        uint8_t cause_value = gtp_cause_from_diameter(
            gx_message->err, gx_message->exp_err);

        ogs_gtp_send_error_message(gtp_xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    if (sess->pdn.ambr.downlink || sess->pdn.ambr.uplink) {
        smf_bearer_t *bearer = smf_default_bearer_in_sess(sess);
        ogs_pfcp_pdr_t *pdr = NULL;
        ogs_pfcp_qer_t *qer = NULL;
        ogs_assert(bearer);

        /* Only 1 QER is used per bearer */
        qer = ogs_list_first(&bearer->pfcp.qer_list);
        if (!qer) {
            qer = ogs_pfcp_qer_add(&bearer->pfcp);
            ogs_assert(qer);
            qer->id = OGS_NEXT_ID(sess->qer_id, 1, OGS_MAX_NUM_OF_QER+1);
        }

        qer->mbr.uplink = sess->pdn.ambr.uplink;
        qer->mbr.downlink = sess->pdn.ambr.downlink;

        ogs_list_for_each(&bearer->pfcp.pdr_list, pdr)
            ogs_pfcp_pdr_associate_qer(pdr, qer);
    }

    sess->num_of_pcc_rule = gx_message->num_of_pcc_rule;
    for (i = 0; i < gx_message->num_of_pcc_rule; i++)
        OGS_STORE_PCC_RULE(&sess->pcc_rule[i], &gx_message->pcc_rule[i]);

    smf_pfcp_send_session_establishment_request(sess, gtp_xact);
}

void smf_gx_handle_cca_termination_request(
        smf_sess_t *sess, ogs_diam_gx_message_t *gx_message,
        ogs_gtp_xact_t *gtp_xact)
{
    ogs_assert(sess);
    ogs_assert(gx_message);
    ogs_assert(gtp_xact);

    ogs_debug("[SMF] Delete Session Response");
    ogs_debug("    SGW_S5C_TEID[0x%x] SMF_N4_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->smf_n4_teid);

    smf_pfcp_send_session_deletion_request(sess, gtp_xact);
}

void smf_gx_handle_re_auth_request(
        smf_sess_t *sess, ogs_diam_gx_message_t *gx_message)
{
    int i;

    sess->num_of_pcc_rule = gx_message->num_of_pcc_rule;
    for (i = 0; i < gx_message->num_of_pcc_rule; i++)
        OGS_STORE_PCC_RULE(&sess->pcc_rule[i], &gx_message->pcc_rule[i]);

    smf_bearer_binding(sess);
}
