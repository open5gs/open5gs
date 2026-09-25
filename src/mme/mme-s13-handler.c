/*
 * Copyright (C) 2026 by LetMeConnect
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

#include "mme-sm.h"
#include "mme-s13-handler.h"
#include "mme-fd-path.h"
#include "nas-path.h"
#include "s1ap-path.h"

bool mme_s13_check_wanted(const mme_ue_t *mme_ue)
{
    ogs_assert(mme_ue);

    /*
     * Attach only (v1). Never for an emergency attach: TS 23.401 5.3.2.1
     * lets the MME skip the check there and forbids rejecting on it.
     */
    return mme_self()->eir.enabled &&
        mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST &&
        mme_ue->nas_eps.attach.value !=
            OGS_NAS_ATTACH_TYPE_EPS_EMERGENCY_ATTACH;
}

bool mme_s13_imeisv_is_usable(const char *imeisv_bcd)
{
    /*
     * Terminal-Information needs the full 16 digits: IMEI (14, no check
     * digit) + SVN (2), TS 23.003 6.2.2. Anything else cannot be split.
     */
    return imeisv_bcd &&
        ogs_bcd_string_is_valid_n(imeisv_bcd, strlen(imeisv_bcd),
                OGS_MAX_IMEISV_BCD_LEN, OGS_MAX_IMEISV_BCD_LEN);
}

void mme_s13_start_check(enb_ue_t *enb_ue, mme_ue_t *mme_ue)
{
    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    if (!mme_s13_imeisv_is_usable(mme_ue->imeisv_bcd)) {
        mme_s13_complete_check(enb_ue, mme_ue,
                mme_s13_missing_pei_cause(&mme_self()->eir));
        return;
    }

    mme_s13_send_ecr(enb_ue, mme_ue);
}

bool mme_s13_eca_is_current(const mme_ue_t *mme_ue,
        const enb_ue_t *enb_ue, uint32_t eir_check_id)
{
    ogs_assert(mme_ue);

    /*
     * The ECR is sent on Security Mode Complete, after which the attach
     * waits in emm_state_initial_context_setup. Leaving that state, a new
     * Attach Request or a new S1 connection ends the procedure the ECR
     * was sent for, and a later ECR carries a new id.
     */
    if (!mme_ue->eir_check_pending) {
        ogs_error("[%s] Ignore unexpected ME-Identity-Check-Answer",
                mme_ue->imsi_bcd);
        return false;
    }
    if (mme_ue->eir_check_id != eir_check_id) {
        ogs_error("[%s] Ignore stale ME-Identity-Check-Answer [%u:%u]",
                mme_ue->imsi_bcd, eir_check_id, mme_ue->eir_check_id);
        return false;
    }
    if (mme_ue->nas_eps.type != MME_EPS_TYPE_ATTACH_REQUEST) {
        ogs_error("[%s] Ignore ME-Identity-Check-Answer "
                "after attach cancellation",
                mme_ue->imsi_bcd);
        return false;
    }
    if (!enb_ue || !ENB_UE_IS_SERVING(mme_ue, enb_ue) ||
            enb_ue->mme_ue_id != mme_ue->id) {
        ogs_error("[%s] Ignore ME-Identity-Check-Answer of a released "
                "S1 context [%d:%d]",
                mme_ue->imsi_bcd,
                enb_ue ? enb_ue->id : OGS_INVALID_POOL_ID,
                mme_ue->enb_ue_id);
        return false;
    }
    if (!OGS_FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup)) {
        ogs_error("[%s] Ignore ME-Identity-Check-Answer outside the attach",
                mme_ue->imsi_bcd);
        return false;
    }

    return true;
}

ogs_nas_emm_cause_t mme_s13_failure_cause(const mme_eir_t *eir_config)
{
    ogs_assert(eir_config);

    ogs_warn("Applying EIR failure_action[%s]",
            eir_config->failure_action == OGS_EIR_ACTION_REJECT ?
                "reject" : "allow");
    return eir_config->failure_action == OGS_EIR_ACTION_REJECT ?
        OGS_NAS_EMM_CAUSE_NETWORK_FAILURE :
        OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
}

ogs_nas_emm_cause_t mme_s13_missing_pei_cause(const mme_eir_t *eir_config)
{
    ogs_assert(eir_config);

    ogs_error("No usable IMEISV for the EIR [missing_pei_action:%s]",
            eir_config->missing_pei_action == OGS_EIR_ACTION_REJECT ?
                "reject" : "allow");
    return eir_config->missing_pei_action == OGS_EIR_ACTION_REJECT ?
        OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED :
        OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
}

ogs_nas_emm_cause_t mme_s13_equipment_status_cause(
        uint32_t equipment_status_code, const mme_eir_t *eir_config)
{
    switch (equipment_status_code) {
    case OGS_DIAM_S13_EQUIPMENT_WHITELIST:
        ogs_info("Whitelisted equipment");
        return OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
    case OGS_DIAM_S13_EQUIPMENT_GREYLIST:
        ogs_warn("Greylisted equipment");
        return OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
    case OGS_DIAM_S13_EQUIPMENT_BLACKLIST:
        ogs_warn("Blacklisted equipment");
        return OGS_NAS_EMM_CAUSE_ILLEGAL_ME;
    default:
        /* Not a verdict: handled like a malformed answer, as the AMF does */
        ogs_error("Unknown Equipment-Status [%u]", equipment_status_code);
        return mme_s13_failure_cause(eir_config);
    }
}

ogs_nas_emm_cause_t mme_s13_message_cause(
        const ogs_diam_s13_message_t *s13_message, const mme_eir_t *eir_config)
{
    ogs_assert(s13_message);
    ogs_assert(eir_config);

    /*
     * 3GPP TS 29.272 clause 7.4: DIAMETER_ERROR_EQUIPMENT_UNKNOWN is the
     * only application error the EIR can return over S13. It is a verdict
     * ("not in my lists"), so unknown_action applies, not failure_action.
     */
    if (s13_message->exp_err &&
        *s13_message->exp_err == OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN) {
        ogs_info("Unknown equipment [unknown_action:%s]",
                eir_config->unknown_action == OGS_EIR_ACTION_REJECT ?
                    "reject" : "allow");
        return eir_config->unknown_action == OGS_EIR_ACTION_REJECT ?
            OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED :
            OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
    }

    /* Every other failure means the EIR gave no verdict at all */
    if (s13_message->result_code != ER_DIAMETER_SUCCESS) {
        ogs_warn("ME-Identity-Check answer error "
                "[Result-Code:%d Experimental-Result-Code:%d]",
                s13_message->err ? (int)*s13_message->err : -1,
                s13_message->exp_err ? (int)*s13_message->exp_err : -1);
        return mme_s13_failure_cause(eir_config);
    }

    return mme_s13_equipment_status_cause(
            s13_message->eca_message.equipment_status_code, eir_config);
}

ogs_nas_emm_cause_t mme_s13_handle_eca(
        mme_ue_t *mme_ue, ogs_diam_s13_message_t *s13_message)
{
    ogs_assert(mme_ue);
    ogs_assert(s13_message);

    return mme_s13_message_cause(s13_message, &mme_self()->eir);
}

void mme_s13_complete_check(enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_emm_cause_t emm_cause)
{
    ogs_assert(mme_ue);

    if (emm_cause == OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED) {
        ogs_info("[%s] Continue attach after EIR check", mme_ue->imsi_bcd);
        mme_s6a_send_ulr(enb_ue, mme_ue, 0);
        return;
    }

    ogs_warn("[%s] Attach rejected after EIR check [cause:%d]",
            mme_ue->imsi_bcd, emm_cause);
    mme_s13_reject_ue(enb_ue, mme_ue, emm_cause);
}

void mme_s13_reject_ue(enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_emm_cause_t emm_cause)
{
    int r;

    ogs_assert(mme_ue);

    if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
        ogs_info("[%s] Attach reject [OGS_NAS_EMM_CAUSE:%d]",
                mme_ue->imsi_bcd, emm_cause);
        r = nas_eps_send_attach_reject(enb_ue, mme_ue, emm_cause,
                OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else
        ogs_error("Invalid Type[%d]", mme_ue->nas_eps.type);

    r = s1ap_send_ue_context_release_command(enb_ue,
            S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
            S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);
}
