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
    int i;

    /*
     * Terminal-Information needs the full 16 digits: IMEI (14, no check
     * digit) + SVN (2), TS 23.003 6.2.2. Anything else cannot be split.
     */
    if (!imeisv_bcd || strlen(imeisv_bcd) != OGS_MAX_IMEISV_BCD_LEN)
        return false;
    for (i = 0; i < OGS_MAX_IMEISV_BCD_LEN; i++)
        if (imeisv_bcd[i] < '0' || imeisv_bcd[i] > '9')
            return false;

    return true;
}

mme_s13_precheck_e mme_s13_precheck(mme_ue_t *mme_ue)
{
    mme_eir_cache_entry_t *cached = NULL;

    ogs_assert(mme_ue);

    if (!mme_s13_check_wanted(mme_ue))
        return MME_S13_PRECHECK_CONTINUE;

    /* No IMEISV yet: the SMC on the authentication path asks for it */
    if (!mme_s13_imeisv_is_usable(mme_ue->imeisv_bcd))
        return MME_S13_PRECHECK_NEED_CHECK;

    cached = mme_eir_cache_lookup(mme_ue->imeisv_bcd);
    if (!cached)
        return MME_S13_PRECHECK_NEED_CHECK;

    ogs_debug("[%s] EIR cache hit on attach fast path", mme_ue->imsi_bcd);

    /* Only verdicts are cached, so a reject here is always a blacklist */
    return mme_s13_equipment_status_cause(cached->status, &mme_self()->eir) ==
            OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED ?
        MME_S13_PRECHECK_CONTINUE : MME_S13_PRECHECK_REJECT;
}

ogs_nas_emm_cause_t mme_s13_failure_cause(const mme_eir_t *eir_config)
{
    ogs_assert(eir_config);

    ogs_warn("Applying EIR failure_action[%s]",
            eir_config->failure_action == MME_EIR_REJECT ? "reject" : "allow");
    return eir_config->failure_action == MME_EIR_REJECT ?
        OGS_NAS_EMM_CAUSE_NETWORK_FAILURE :
        OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
}

ogs_nas_emm_cause_t mme_s13_missing_pei_cause(const mme_eir_t *eir_config)
{
    ogs_assert(eir_config);

    ogs_error("No usable IMEISV for the EIR [missing_pei_action:%s]",
            eir_config->missing_pei_action == MME_EIR_REJECT ?
                "reject" : "allow");
    return eir_config->missing_pei_action == MME_EIR_REJECT ?
        OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED :
        OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
}

bool mme_s13_status_is_verdict(uint32_t equipment_status_code)
{
    return equipment_status_code == OGS_DIAM_S13_EQUIPMENT_WHITELIST ||
           equipment_status_code == OGS_DIAM_S13_EQUIPMENT_GREYLIST ||
           equipment_status_code == OGS_DIAM_S13_EQUIPMENT_BLACKLIST;
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
                eir_config->unknown_action == MME_EIR_REJECT ?
                    "reject" : "allow");
        return eir_config->unknown_action == MME_EIR_REJECT ?
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
        r = nas_eps_send_attach_reject(
                enb_ue, mme_ue, emm_cause,
                OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else if (mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST) {
        ogs_info("[%s] TAU reject [OGS_NAS_EMM_CAUSE:%d]",
                mme_ue->imsi_bcd, emm_cause);
        r = nas_eps_send_tau_reject(enb_ue, mme_ue, emm_cause);
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
