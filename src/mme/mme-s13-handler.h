#ifndef MME_S13_HANDLER_H
#define MME_S13_HANDLER_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Decision table, mirrored on src/amf/n5geir-handler.c. Every entry point
 * returns the EMM cause to apply; OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED means
 * "continue with the ULR".
 *
 *   WHITELISTED / GREYLISTED         -> accepted (greylist is logged)
 *   BLACKLISTED                      -> #6  Illegal ME, always
 *   DIAMETER_ERROR_EQUIPMENT_UNKNOWN -> unknown_action:     #7  EPS services not allowed
 *   no usable IMEISV                 -> missing_pei_action: #7  EPS services not allowed
 *   no verdict (other error, timeout,
 *   unrecognized Equipment-Status)   -> failure_action:     #17 Network failure
 *
 * #17 is the EPS translation of the AMF's #90 (the UE retries after T3411).
 * #6 and #7 both make the UE consider the USIM invalid for EPS until
 * power-off or UICC removal (TS 24.301 5.5.1.2.5), as in the AMF.
 */
ogs_nas_emm_cause_t mme_s13_failure_cause(const mme_eir_t *eir_config);
ogs_nas_emm_cause_t mme_s13_missing_pei_cause(const mme_eir_t *eir_config);
ogs_nas_emm_cause_t mme_s13_equipment_status_cause(
        uint32_t equipment_status_code, const mme_eir_t *eir_config);
ogs_nas_emm_cause_t mme_s13_message_cause(
        const ogs_diam_s13_message_t *s13_message, const mme_eir_t *eir_config);

ogs_nas_emm_cause_t mme_s13_handle_eca(
        mme_ue_t *mme_ue, ogs_diam_s13_message_t *s13_message);

/* Only a real verdict (white/grey/black) is worth caching */
bool mme_s13_status_is_verdict(uint32_t equipment_status_code);

/* ULR when accepted, Attach/TAU Reject + release otherwise: the MME's
 * gmm_complete_equipment_identity_check() */
void mme_s13_complete_check(enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_emm_cause_t emm_cause);

/* True when this procedure must be checked against the EIR:
 * eir.enabled, an attach (v1: attach only), and not an emergency attach. */
bool mme_s13_check_wanted(const mme_ue_t *mme_ue);

/* 16 decimal digits, i.e. splittable into IMEI (14) + SVN (2) */
bool mme_s13_imeisv_is_usable(const char *imeisv_bcd);

/*
 * Pre-check for the attach paths that skip authentication and SMC because
 * the UE already has a valid security context. Those paths would otherwise
 * never reach the ME identity check that hangs off SMC complete.
 */
typedef enum {
    MME_S13_PRECHECK_CONTINUE = 0, /* no check wanted, or a fresh cached
                                    * verdict allows: keep the fast path */
    MME_S13_PRECHECK_REJECT,       /* fresh cached verdict denies: reject
                                    * with #6 Illegal ME */
    MME_S13_PRECHECK_NEED_CHECK,   /* no fresh verdict: take the
                                    * authentication path so the SMC
                                    * collects the IMEISV and the ECR runs */
} mme_s13_precheck_e;

mme_s13_precheck_e mme_s13_precheck(mme_ue_t *mme_ue);

/* Shared reject path: Attach/TAU Reject + UE context release, with the
 * cause chosen by the decision table above. */
void mme_s13_reject_ue(enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_emm_cause_t emm_cause);

#ifdef __cplusplus
}
#endif

#endif /* MME_S13_HANDLER_H */
