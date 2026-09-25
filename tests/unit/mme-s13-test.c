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

/*
 * Isolated tests for the EIR (S13) decision logic.
 *
 * Nothing here touches freeDiameter: mme_s13_*_cause() are pure functions
 * of their inputs, mme_s13_handle_eca() only reads mme_self()->eir and
 * mme_s13_eca_is_current() only reads the UE/S1 contexts. The Diameter wire
 * path (mme_s13_send_ecr / mme_s13_eca_cb / mme_s13_ecr_expire_cb) needs a
 * peer and is covered by scenario tests.
 */

#include "mme/mme-sm.h"
#include "mme/mme-s13-handler.h"
#include "core/abts.h"

/*
 * Build a message the way mme_s13_eca_cb does: err/exp_err point into the
 * message's own result_code and exactly one of them is set.
 */
static void msg_result(ogs_diam_s13_message_t *m, uint32_t code)
{
    memset(m, 0, sizeof(*m));
    m->cmd_code = OGS_DIAM_S13_CMD_CODE_ME_IDENTITY_CHECK;
    m->result_code = code;
    m->err = &m->result_code;
}

static void msg_exp_result(ogs_diam_s13_message_t *m, uint32_t code)
{
    memset(m, 0, sizeof(*m));
    m->cmd_code = OGS_DIAM_S13_CMD_CODE_ME_IDENTITY_CHECK;
    m->result_code = code;
    m->exp_err = &m->result_code;
}

static void s13_test_check_wanted(abts_case *tc, void *data)
{
    mme_ue_t *mme_ue;
    bool saved_enabled = mme_self()->eir.enabled;

    mme_ue = ogs_calloc(1, sizeof(*mme_ue));
    ABTS_PTR_NOTNULL(tc, mme_ue);

    mme_self()->eir.enabled = true;
    mme_ue->nas_eps.type = MME_EPS_TYPE_ATTACH_REQUEST;
    mme_ue->nas_eps.attach.value = OGS_NAS_ATTACH_TYPE_EPS_ATTACH;
    ABTS_TRUE(tc, mme_s13_check_wanted(mme_ue));
    mme_ue->nas_eps.attach.value =
        OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;
    ABTS_TRUE(tc, mme_s13_check_wanted(mme_ue));
    mme_ue->nas_eps.attach.value = OGS_NAS_ATTACH_TYPE_EPS_EMERGENCY_ATTACH;
    ABTS_TRUE(tc, !mme_s13_check_wanted(mme_ue));

    mme_ue->nas_eps.attach.value = OGS_NAS_ATTACH_TYPE_EPS_ATTACH;
    mme_ue->nas_eps.type = MME_EPS_TYPE_TAU_REQUEST;
    ABTS_TRUE(tc, !mme_s13_check_wanted(mme_ue));
    mme_ue->nas_eps.type = MME_EPS_TYPE_SERVICE_REQUEST;
    ABTS_TRUE(tc, !mme_s13_check_wanted(mme_ue));

    mme_ue->nas_eps.type = MME_EPS_TYPE_ATTACH_REQUEST;
    mme_self()->eir.enabled = false;
    ABTS_TRUE(tc, !mme_s13_check_wanted(mme_ue));

    mme_self()->eir.enabled = saved_enabled;
    ogs_free(mme_ue);
}

/* Equipment-Status -> EMM cause, no knob: the AMF's table */
static void s13_test_equipment_status(abts_case *tc, void *data)
{
    mme_eir_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_equipment_status_cause(
                OGS_DIAM_S13_EQUIPMENT_WHITELIST, &cfg));
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_equipment_status_cause(
                OGS_DIAM_S13_EQUIPMENT_GREYLIST, &cfg));
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_ILLEGAL_ME,
            mme_s13_equipment_status_cause(
                OGS_DIAM_S13_EQUIPMENT_BLACKLIST, &cfg));

    /* Unrecognized status is not a verdict: failure_action decides */
    cfg.failure_action = MME_EIR_ALLOW;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_equipment_status_cause(3, &cfg));
    cfg.failure_action = MME_EIR_REJECT;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_equipment_status_cause(0xffffffff, &cfg));
}

/* Result-Code / Experimental-Result -> cause, through the three policies */
static void s13_test_diameter_result_mapping(abts_case *tc, void *data)
{
    ogs_diam_s13_message_t m;
    mme_eir_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    /* 2001: the verdict is in Equipment-Status */
    msg_result(&m, ER_DIAMETER_SUCCESS);
    m.eca_message.equipment_status_code = OGS_DIAM_S13_EQUIPMENT_WHITELIST;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_message_cause(&m, &cfg));
    msg_result(&m, ER_DIAMETER_SUCCESS);
    m.eca_message.equipment_status_code = OGS_DIAM_S13_EQUIPMENT_BLACKLIST;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_ILLEGAL_ME,
            mme_s13_message_cause(&m, &cfg));

    /* Transport / protocol / permanent errors carry no verdict:
     * failure_action, #17 when rejecting */
    cfg.failure_action = MME_EIR_ALLOW;
    msg_result(&m, ER_DIAMETER_UNABLE_TO_DELIVER);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_message_cause(&m, &cfg));
    cfg.failure_action = MME_EIR_REJECT;
    msg_result(&m, ER_DIAMETER_UNABLE_TO_DELIVER);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_message_cause(&m, &cfg));
    msg_result(&m, ER_DIAMETER_TOO_BUSY);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_message_cause(&m, &cfg));
    msg_result(&m, ER_DIAMETER_UNABLE_TO_COMPLY);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_message_cause(&m, &cfg));

    /* 5422 in Experimental-Result is a verdict: "unknown to the EIR",
     * unknown_action, #7 when rejecting */
    cfg.unknown_action = MME_EIR_ALLOW;
    msg_exp_result(&m, OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_message_cause(&m, &cfg));
    cfg.unknown_action = MME_EIR_REJECT;
    msg_exp_result(&m, OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED,
            mme_s13_message_cause(&m, &cfg));

    /* ...and only there: 5422 in Result-Code is not a verdict */
    cfg.failure_action = MME_EIR_REJECT;
    msg_result(&m, OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_message_cause(&m, &cfg));

    /* Any other experimental error: no verdict */
    msg_exp_result(&m, OGS_DIAM_S6A_ERROR_UNKNOWN_EPS_SUBSCRIPTION);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_message_cause(&m, &cfg));

    /* Non-success with neither pointer set (defensive): no verdict, no crash */
    memset(&m, 0, sizeof(m));
    m.result_code = ER_DIAMETER_UNABLE_TO_COMPLY;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_message_cause(&m, &cfg));

    /* Missing IMEISV: missing_pei_action, #7 when rejecting */
    cfg.missing_pei_action = MME_EIR_ALLOW;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_missing_pei_cause(&cfg));
    cfg.missing_pei_action = MME_EIR_REJECT;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED,
            mme_s13_missing_pei_cause(&cfg));
}

/* Full handler, on mme_self()->eir: Result-Code gate first, then the list */
static void s13_test_handle_eca(abts_case *tc, void *data)
{
    ogs_diam_s13_message_t m;
    mme_ue_t *mme_ue;

    mme_ue = ogs_calloc(1, sizeof(*mme_ue)); /* only asserted non-NULL */
    ABTS_PTR_NOTNULL(tc, mme_ue);

    mme_self()->eir.unknown_action = MME_EIR_ALLOW;
    mme_self()->eir.failure_action = MME_EIR_ALLOW;

    msg_result(&m, ER_DIAMETER_SUCCESS);
    m.eca_message.equipment_status_code = OGS_DIAM_S13_EQUIPMENT_WHITELIST;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_handle_eca(mme_ue, &m));

    msg_result(&m, ER_DIAMETER_SUCCESS);
    m.eca_message.equipment_status_code = OGS_DIAM_S13_EQUIPMENT_GREYLIST;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_handle_eca(mme_ue, &m));

    /* Blacklisted: #6 whatever the policies say */
    mme_self()->eir.unknown_action = MME_EIR_REJECT;
    mme_self()->eir.failure_action = MME_EIR_REJECT;
    msg_result(&m, ER_DIAMETER_SUCCESS);
    m.eca_message.equipment_status_code = OGS_DIAM_S13_EQUIPMENT_BLACKLIST;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_ILLEGAL_ME,
            mme_s13_handle_eca(mme_ue, &m));
    mme_self()->eir.unknown_action = MME_EIR_ALLOW;
    mme_self()->eir.failure_action = MME_EIR_ALLOW;

    /* An error Result-Code wins over whatever Equipment-Status is left */
    msg_result(&m, ER_DIAMETER_UNABLE_TO_DELIVER);
    m.eca_message.equipment_status_code = OGS_DIAM_S13_EQUIPMENT_WHITELIST;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_handle_eca(mme_ue, &m));
    mme_self()->eir.failure_action = MME_EIR_REJECT;
    msg_result(&m, ER_DIAMETER_UNABLE_TO_DELIVER);
    m.eca_message.equipment_status_code = OGS_DIAM_S13_EQUIPMENT_WHITELIST;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_handle_eca(mme_ue, &m));
    mme_self()->eir.failure_action = MME_EIR_ALLOW;

    /* Unknown equipment follows mme_self()->eir.unknown_action */
    msg_exp_result(&m, OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_handle_eca(mme_ue, &m));
    mme_self()->eir.unknown_action = MME_EIR_REJECT;
    msg_exp_result(&m, OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN);
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED,
            mme_s13_handle_eca(mme_ue, &m));
    mme_self()->eir.unknown_action = MME_EIR_ALLOW;

    ogs_free(mme_ue);
}

/*
 * The exact message mme_s13_ecr_expire_cb synthesises when no ECA arrives
 * within eir.timeout must land in failure_action, whatever Equipment-Status
 * value is left in the message.
 */
static void s13_test_timeout_follows_failure_action(abts_case *tc, void *data)
{
    ogs_diam_s13_message_t *m;
    mme_ue_t *mme_ue;

    m = ogs_calloc(1, sizeof(*m));
    ABTS_PTR_NOTNULL(tc, m);
    m->cmd_code = OGS_DIAM_S13_CMD_CODE_ME_IDENTITY_CHECK;
    m->result_code = ER_DIAMETER_UNABLE_TO_COMPLY;
    m->err = &m->result_code;
    m->eca_message.equipment_status_code = OGS_DIAM_S13_EQUIPMENT_WHITELIST;

    mme_ue = ogs_calloc(1, sizeof(*mme_ue));
    ABTS_PTR_NOTNULL(tc, mme_ue);

    mme_self()->eir.failure_action = MME_EIR_REJECT;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
            mme_s13_handle_eca(mme_ue, m));

    mme_self()->eir.failure_action = MME_EIR_ALLOW;
    ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED,
            mme_s13_handle_eca(mme_ue, m));

    ogs_free(mme_ue);
    ogs_free(m);
}

/* Terminal-Information needs exactly 16 decimal digits */
static void s13_test_imeisv_is_usable(abts_case *tc, void *data)
{
    ABTS_TRUE(tc, mme_s13_imeisv_is_usable("3512345678901201"));
    ABTS_TRUE(tc, !mme_s13_imeisv_is_usable(""));
    ABTS_TRUE(tc, !mme_s13_imeisv_is_usable(NULL));
    ABTS_TRUE(tc, !mme_s13_imeisv_is_usable("351234567890120"));   /* 15 */
    ABTS_TRUE(tc, !mme_s13_imeisv_is_usable("35123456789012012")); /* 17 */
    ABTS_TRUE(tc, !mme_s13_imeisv_is_usable("35123456789012a1"));
    ABTS_TRUE(tc, !mme_s13_imeisv_is_usable("3512345678901 01"));
}

/* Terminal-Information (IMEI + Software-Version) -> PEI as stored by the EIR */
static void s13_test_pei_from_terminal_info(abts_case *tc, void *data)
{
    char pei[OGS_DIAM_S13_MAX_PEI_LEN+1];

    /* IMEI + SVN: the IMEISV form the AMF also sends over N5g-eir */
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_diam_s13_pei_from_terminal_info(
            "49015420323751", 14, "86", 2, pei, sizeof(pei)));
    ABTS_STR_EQUAL(tc, "imeisv-4901542032375186", pei);

    /* IMEI alone: Luhn check digit appended */
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_diam_s13_pei_from_terminal_info(
            "49015420323751", 14, NULL, 0, pei, sizeof(pei)));
    ABTS_STR_EQUAL(tc, "imei-490154203237518", pei);
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_diam_s13_pei_from_terminal_info(
            "86650704004053", 14, NULL, 0, pei, sizeof(pei)));
    ABTS_STR_EQUAL(tc, "imei-866507040040534", pei);

    /* Unusable input */
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_diam_s13_pei_from_terminal_info(
            "4901542032375", 13, "86", 2, pei, sizeof(pei)));
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_diam_s13_pei_from_terminal_info(
            "490154203237518", 15, "86", 2, pei, sizeof(pei)));
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_diam_s13_pei_from_terminal_info(
            "4901542032375a", 14, "86", 2, pei, sizeof(pei)));
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_diam_s13_pei_from_terminal_info(
            "49015420323751", 14, "8", 1, pei, sizeof(pei)));
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_diam_s13_pei_from_terminal_info(
            "49015420323751", 14, "8x", 2, pei, sizeof(pei)));
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_diam_s13_pei_from_terminal_info(
            NULL, 0, NULL, 0, pei, sizeof(pei)));
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_diam_s13_pei_from_terminal_info(
            "49015420323751", 14, "86", 2, pei, 10));
    ABTS_STR_EQUAL(tc, "", pei);
}

/* The decision tests need log domains and EIR policy, but no MME pools. */
static mme_eir_t saved_eir;

static struct {
    ogs_log_level_e mme, diam;
} saved_log;

static void s13_context_setup(void)
{
    saved_eir = mme_self()->eir;

    if (!ogs_log_find_domain("mme"))
        ogs_log_install_domain(
                &__mme_log_domain, "mme", ogs_core()->log.level);
    if (!ogs_log_find_domain("diam"))
        ogs_log_install_domain(
                &__ogs_diam_domain, "diam", ogs_core()->log.level);

    /* Every case below deliberately drives the warn/error paths of
     * mme-s13-handler.c and lib/diameter/s13: silence both domains for
     * the suite, as the other unit suites do, and restore them after. */
    saved_log.mme = ogs_log_get_domain_level(__mme_log_domain);
    saved_log.diam = ogs_log_get_domain_level(__ogs_diam_domain);
    ogs_log_set_domain_level(__mme_log_domain, OGS_LOG_NONE);
    ogs_log_set_domain_level(__ogs_diam_domain, OGS_LOG_NONE);
}

static void s13_context_teardown(void)
{
    ogs_log_set_domain_level(__mme_log_domain, saved_log.mme);
    ogs_log_set_domain_level(__ogs_diam_domain, saved_log.diam);
    mme_self()->eir = saved_eir;
}

/*
 * Only the answer to the ECR the current attach is waiting for resumes it.
 * A late ECA of a cancelled or replaced procedure must neither send the ULR
 * nor reject the UE.
 */
static void s13_test_eca_is_current(abts_case *tc, void *data)
{
    mme_ue_t *mme_ue;
    enb_ue_t enb_ue;

    mme_ue = ogs_calloc(1, sizeof(*mme_ue));
    ABTS_PTR_NOTNULL(tc, mme_ue);
    memset(&enb_ue, 0, sizeof(enb_ue));

    /* Waiting for the answer to ECR #7 on S1 context 3 */
    mme_ue->id = 1;
    mme_ue->enb_ue_id = 3;
    mme_ue->nas_eps.type = MME_EPS_TYPE_ATTACH_REQUEST;
    mme_ue->eir_check_pending = true;
    mme_ue->eir_check_id = 7;
    enb_ue.id = 3;
    enb_ue.mme_ue_id = mme_ue->id;
    OGS_FSM_STATE(&mme_ue->sm) =
        (ogs_fsm_handler_t)emm_state_initial_context_setup;
    ABTS_TRUE(tc, mme_s13_eca_is_current(mme_ue, &enb_ue, 7));

    /* Answer to an earlier ECR, replaced by a new attach */
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, &enb_ue, 6));
    ABTS_TRUE(tc, mme_ue->eir_check_pending);

    /* The S1 context was removed or replaced before event dispatch */
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, NULL, 7));
    enb_ue.id = 4;
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, &enb_ue, 7));
    enb_ue.id = 3;

    /* Both sides of the UE/S1 association must still agree */
    enb_ue.mme_ue_id = 2;
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, &enb_ue, 7));
    enb_ue.mme_ue_id = mme_ue->id;

    /* A release command leaves the IDs and EMM state unchanged */
    enb_ue.ue_ctx_rel_action = S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK;
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, &enb_ue, 7));
    enb_ue.ue_ctx_rel_action = S1AP_UE_CTX_REL_INVALID_ACTION;

    /* A detach procedure must not resume the earlier attach */
    mme_ue->nas_eps.type = MME_EPS_TYPE_DETACH_REQUEST_TO_UE;
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, &enb_ue, 7));
    mme_ue->nas_eps.type = MME_EPS_TYPE_ATTACH_REQUEST;

    /* Clearing pending invalidates the answer even if other fields match */
    mme_ue->eir_check_pending = false;
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, &enb_ue, 7));
    mme_ue->eir_check_pending = true;
    ABTS_TRUE(tc, mme_s13_eca_is_current(mme_ue, &enb_ue, 7));

    /* The attach left emm_state_initial_context_setup */
    OGS_FSM_STATE(&mme_ue->sm) = (ogs_fsm_handler_t)emm_state_exception;
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, &enb_ue, 7));
    OGS_FSM_STATE(&mme_ue->sm) =
        (ogs_fsm_handler_t)emm_state_initial_context_setup;

    /* No check pending: already answered, or the procedure was cancelled */
    mme_ue->eir_check_pending = false;
    ABTS_TRUE(tc, !mme_s13_eca_is_current(mme_ue, &enb_ue, 7));

    ogs_free(mme_ue);
}

abts_suite *test_mme_s13(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    s13_context_setup();

    /* Decision logic */
    abts_run_test(suite, s13_test_check_wanted, NULL);
    abts_run_test(suite, s13_test_equipment_status, NULL);
    abts_run_test(suite, s13_test_diameter_result_mapping, NULL);
    abts_run_test(suite, s13_test_handle_eca, NULL);
    abts_run_test(suite, s13_test_timeout_follows_failure_action, NULL);
    abts_run_test(suite, s13_test_imeisv_is_usable, NULL);
    abts_run_test(suite, s13_test_pei_from_terminal_info, NULL);

    /* Stale answers */
    abts_run_test(suite, s13_test_eca_is_current, NULL);

    s13_context_teardown();

    return suite;
}
