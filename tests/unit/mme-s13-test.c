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
 * Isolated tests for the EIR (S13) decision logic and the IMEISV cache.
 *
 * Nothing here touches freeDiameter: mme_s13_*_cause() are pure functions
 * of their inputs, mme_s13_handle_eca() / mme_s13_precheck() only read
 * mme_self()->eir and the cache, and the cache API works on the MME context
 * alone. The Diameter wire path (mme_s13_send_ecr / mme_s13_eca_cb /
 * mme_s13_ecr_expire_cb) needs a peer and is covered by scenario tests.
 */

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

    /* ...and never enters the cache */
    ABTS_TRUE(tc, mme_s13_status_is_verdict(OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    ABTS_TRUE(tc, mme_s13_status_is_verdict(OGS_DIAM_S13_EQUIPMENT_GREYLIST));
    ABTS_TRUE(tc, mme_s13_status_is_verdict(OGS_DIAM_S13_EQUIPMENT_BLACKLIST));
    ABTS_TRUE(tc, !mme_s13_status_is_verdict(3));
    ABTS_TRUE(tc, !mme_s13_status_is_verdict(0xffffffff));
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

/*
 * mme_context_init() is needed by every test here, not only the cache
 * ones: it installs the "mme" log domain that mme-s13-handler.c logs to
 * (an ogs_warn() on an uninstalled domain is FATAL). It sizes its pools
 * from the static app and global configuration; the unit binary never
 * parses a config file, so give it small but non-zero sizes first.
 */
#define S13_TEST_MAX_UE 8

static struct {
    int max_ue, max_peer, nf, csmap, emerg, sess, bearer;
} saved_conf;

static struct {
    ogs_log_level_e mme, diam;
} saved_log;

static void s13_context_setup(void)
{
    saved_conf.max_ue = ogs_global_conf()->max.ue;
    saved_conf.max_peer = ogs_global_conf()->max.peer;
    saved_conf.nf = ogs_app()->pool.nf;
    saved_conf.csmap = ogs_app()->pool.csmap;
    saved_conf.emerg = ogs_app()->pool.emerg;
    saved_conf.sess = ogs_app()->pool.sess;
    saved_conf.bearer = ogs_app()->pool.bearer;

    ogs_global_conf()->max.ue = S13_TEST_MAX_UE;
    ogs_global_conf()->max.peer = 2;
    ogs_app()->pool.nf = 2;
    ogs_app()->pool.csmap = 2;
    ogs_app()->pool.emerg = 2;
    ogs_app()->pool.sess = S13_TEST_MAX_UE;
    ogs_app()->pool.bearer = S13_TEST_MAX_UE;
    mme_context_init();

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
    /* Restore before mme_context_final() removes the "mme" domain */
    ogs_log_set_domain_level(__mme_log_domain, saved_log.mme);
    ogs_log_set_domain_level(__ogs_diam_domain, saved_log.diam);

    mme_context_final();

    /* Leave the process-wide configuration as we found it for the
     * suites that run after this one */
    ogs_global_conf()->max.ue = saved_conf.max_ue;
    ogs_global_conf()->max.peer = saved_conf.max_peer;
    ogs_app()->pool.nf = saved_conf.nf;
    ogs_app()->pool.csmap = saved_conf.csmap;
    ogs_app()->pool.emerg = saved_conf.emerg;
    ogs_app()->pool.sess = saved_conf.sess;
    ogs_app()->pool.bearer = saved_conf.bearer;
}

static void s13_test_cache_basic(abts_case *tc, void *data)
{
    mme_eir_cache_entry_t *e1, *e2, *again;
    ogs_time_t before;

    mme_eir_cache_remove_all();

    ABTS_TRUE(tc, mme_eir_cache_find("3512345678901201") == NULL);

    before = ogs_get_monotonic_time();
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", "3512345678901201",
            OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    e1 = mme_eir_cache_find("3512345678901201");
    ABTS_PTR_NOTNULL(tc, e1);
    ABTS_INT_EQUAL(tc, OGS_DIAM_S13_EQUIPMENT_WHITELIST, e1->status);
    ABTS_STR_EQUAL(tc, "3512345678901201", e1->imeisv_bcd);
    ABTS_STR_EQUAL(tc, "001010123456789", e1->imsi_bcd);
    ABTS_TRUE(tc, e1->checked_at >= before);

    /* Same IMEISV again: updated in place, no second entry */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010999999999", "3512345678901201",
            OGS_DIAM_S13_EQUIPMENT_BLACKLIST));
    again = mme_eir_cache_find("3512345678901201");
    ABTS_TRUE(tc, again == e1);
    ABTS_INT_EQUAL(tc, OGS_DIAM_S13_EQUIPMENT_BLACKLIST, again->status);
    ABTS_TRUE(tc, again->checked_at >= before);
    /* IMSI is informational, not the key: first writer's value is kept */
    ABTS_STR_EQUAL(tc, "001010123456789", again->imsi_bcd);

    /* A different IMEISV is a different entry */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", "3512345678901202",
            OGS_DIAM_S13_EQUIPMENT_GREYLIST));
    e2 = mme_eir_cache_find("3512345678901202");
    ABTS_PTR_NOTNULL(tc, e2);
    ABTS_TRUE(tc, e2 != e1);
    ABTS_INT_EQUAL(tc, OGS_DIAM_S13_EQUIPMENT_GREYLIST, e2->status);
    ABTS_TRUE(tc, mme_eir_cache_find("3512345678901201") == e1);

    mme_eir_cache_remove_all();
    ABTS_TRUE(tc, mme_eir_cache_find("3512345678901201") == NULL);
    ABTS_TRUE(tc, mme_eir_cache_find("3512345678901202") == NULL);
}

/* The hash keys on the entry's own buffer, never on the caller's */
static void s13_test_cache_key_is_copied(abts_case *tc, void *data)
{
    char key[OGS_MAX_IMEISV_BCD_LEN+1];
    mme_eir_cache_entry_t *e;

    mme_eir_cache_remove_all();

    strcpy(key, "3599999999999901");
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", key, OGS_DIAM_S13_EQUIPMENT_WHITELIST));

    /* Clobber the caller's buffer: the entry must still be reachable */
    memset(key, 'x', OGS_MAX_IMEISV_BCD_LEN);
    e = mme_eir_cache_find("3599999999999901");
    ABTS_PTR_NOTNULL(tc, e);
    ABTS_STR_EQUAL(tc, "3599999999999901", e->imeisv_bcd);
    ABTS_TRUE(tc, mme_eir_cache_find(key) == NULL);

    mme_eir_cache_remove_all();
}

/* TTL arithmetic, on a synthetic entry: no clock involved */
static void s13_test_cache_freshness(abts_case *tc, void *data)
{
    mme_eir_cache_entry_t e;
    ogs_time_t t0 = (ogs_time_t)1000 * OGS_USEC_PER_SEC;

    memset(&e, 0, sizeof(e));
    e.checked_at = t0;

    /* max_age 0: never expires */
    ABTS_TRUE(tc, mme_eir_cache_entry_is_fresh(&e, 0,
            t0 + (ogs_time_t)365 * 86400 * OGS_USEC_PER_SEC));

    /* Strictly inside the window */
    ABTS_TRUE(tc, mme_eir_cache_entry_is_fresh(&e, 60, t0));
    ABTS_TRUE(tc, mme_eir_cache_entry_is_fresh(&e, 60,
            t0 + (ogs_time_t)60 * OGS_USEC_PER_SEC - 1));

    /* Exactly max_age old: expired */
    ABTS_TRUE(tc, !mme_eir_cache_entry_is_fresh(&e, 60,
            t0 + (ogs_time_t)60 * OGS_USEC_PER_SEC));
    ABTS_TRUE(tc, !mme_eir_cache_entry_is_fresh(&e, 60,
            t0 + (ogs_time_t)3600 * OGS_USEC_PER_SEC));

    /* Large max_age must not overflow the usec conversion */
    ABTS_TRUE(tc, mme_eir_cache_entry_is_fresh(&e, 0xffffffffu,
            t0 + (ogs_time_t)365 * 86400 * OGS_USEC_PER_SEC));
}

/* lookup() applies the TTL and drops a stale entry on the spot */
static void s13_test_cache_lookup_ttl(abts_case *tc, void *data)
{
    mme_eir_cache_entry_t *e;

    mme_eir_cache_remove_all();
    mme_self()->eir.max_age = 60;

    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", "3512345678901201",
            OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    ABTS_PTR_NOTNULL(tc, mme_eir_cache_lookup("3512345678901201"));

    /* Age the entry past max_age */
    e = mme_eir_cache_find("3512345678901201");
    ABTS_PTR_NOTNULL(tc, e);
    e->checked_at -= (ogs_time_t)61 * OGS_USEC_PER_SEC;
    ABTS_TRUE(tc, mme_eir_cache_lookup("3512345678901201") == NULL);
    /* ...and it is gone, not merely hidden */
    ABTS_TRUE(tc, mme_eir_cache_find("3512345678901201") == NULL);

    /* max_age 0: the same aged entry stays valid */
    mme_self()->eir.max_age = 0;
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", "3512345678901201",
            OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    e = mme_eir_cache_find("3512345678901201");
    e->checked_at -= (ogs_time_t)365 * 86400 * OGS_USEC_PER_SEC;
    ABTS_PTR_NOTNULL(tc, mme_eir_cache_lookup("3512345678901201"));

    mme_self()->eir.max_age = 3600;
    mme_eir_cache_remove_all();
}

/*
 * Pool exhaustion. The pool holds max.ue * 2 entries (mme_context_init).
 * A new key on a full pool evicts the least recently used entry; a hit
 * (lookup or update) makes an entry the most recently used.
 */
static void s13_test_cache_lru_eviction(abts_case *tc, void *data)
{
    int i, n = S13_TEST_MAX_UE * 2;
    char imeisv[OGS_MAX_IMEISV_BCD_LEN+1];
    char victim[OGS_MAX_IMEISV_BCD_LEN+1];

    mme_eir_cache_remove_all();
    mme_self()->eir.max_age = 3600;

    for (i = 0; i < n; i++) {
        snprintf(imeisv, sizeof(imeisv), "35%014d", i);
        ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
                "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    }
    for (i = 0; i < n; i++) {
        snprintf(imeisv, sizeof(imeisv), "35%014d", i);
        ABTS_PTR_NOTNULL(tc, mme_eir_cache_find(imeisv));
    }

    /* Touch #0 (the oldest) through a lookup: it is now the newest */
    snprintf(imeisv, sizeof(imeisv), "35%014d", 0);
    ABTS_PTR_NOTNULL(tc, mme_eir_cache_lookup(imeisv));

    /* Full pool, new key: #1 is now the LRU and must go, #0 must stay */
    snprintf(imeisv, sizeof(imeisv), "35%014d", n);
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    ABTS_PTR_NOTNULL(tc, mme_eir_cache_find(imeisv));
    snprintf(victim, sizeof(victim), "35%014d", 1);
    ABTS_TRUE(tc, mme_eir_cache_find(victim) == NULL);
    snprintf(imeisv, sizeof(imeisv), "35%014d", 0);
    ABTS_PTR_NOTNULL(tc, mme_eir_cache_find(imeisv));

    /* Refreshing #2 through an update also protects it: #3 goes next */
    snprintf(imeisv, sizeof(imeisv), "35%014d", 2);
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_GREYLIST));
    snprintf(imeisv, sizeof(imeisv), "35%014d", n + 1);
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    snprintf(victim, sizeof(victim), "35%014d", 3);
    ABTS_TRUE(tc, mme_eir_cache_find(victim) == NULL);
    snprintf(imeisv, sizeof(imeisv), "35%014d", 2);
    ABTS_PTR_NOTNULL(tc, mme_eir_cache_find(imeisv));
    ABTS_INT_EQUAL(tc, OGS_DIAM_S13_EQUIPMENT_GREYLIST,
            mme_eir_cache_find(imeisv)->status);

    /* remove_all gives every slot back */
    mme_eir_cache_remove_all();
    for (i = 0; i < n; i++) {
        snprintf(imeisv, sizeof(imeisv), "35%014d", i);
        ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
                "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    }
    mme_eir_cache_remove_all();
}

/*
 * Pre-check used by the attach paths that skip authentication + SMC:
 * fresh verdict applied inline, otherwise the attach must go through the
 * authentication path so that the SMC collects the IMEISV.
 */
static void s13_test_precheck(abts_case *tc, void *data)
{
    mme_ue_t *mme_ue;
    mme_eir_cache_entry_t *e;
    const char *imeisv = "3512345678901201";

    mme_eir_cache_remove_all();
    mme_self()->eir.enabled = true;
    mme_self()->eir.max_age = 3600;

    mme_ue = ogs_calloc(1, sizeof(*mme_ue));
    ABTS_PTR_NOTNULL(tc, mme_ue);
    mme_ue->nas_eps.type = MME_EPS_TYPE_ATTACH_REQUEST;
    mme_ue->nas_eps.attach.value = OGS_NAS_ATTACH_TYPE_EPS_ATTACH;
    strcpy(mme_ue->imeisv_bcd, imeisv);

    ABTS_TRUE(tc, mme_s13_check_wanted(mme_ue));

    /* No cached verdict: authentication path */
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_NEED_CHECK, mme_s13_precheck(mme_ue));

    /* No usable IMEISV: same, the SMC will collect it */
    mme_ue->imeisv_bcd[0] = 0;
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_NEED_CHECK, mme_s13_precheck(mme_ue));
    strcpy(mme_ue->imeisv_bcd, imeisv);

    /* Fresh WHITE: fast path continues */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_WHITELIST));
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_CONTINUE, mme_s13_precheck(mme_ue));

    /* Fresh BLACK: reject inline, no knob */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_BLACKLIST));
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_REJECT, mme_s13_precheck(mme_ue));

    /* Fresh GREY: admitted, like the AMF */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_GREYLIST));
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_CONTINUE, mme_s13_precheck(mme_ue));

    /* Stale verdict: dropped, authentication path */
    e = mme_eir_cache_find(imeisv);
    ABTS_PTR_NOTNULL(tc, e);
    e->checked_at -= (ogs_time_t)3601 * OGS_USEC_PER_SEC;
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_NEED_CHECK, mme_s13_precheck(mme_ue));
    ABTS_TRUE(tc, mme_eir_cache_find(imeisv) == NULL);

    /* Not wanted at all: EIR disabled, emergency attach, TAU (v1) */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_eir_cache_update(
            "001010123456789", imeisv, OGS_DIAM_S13_EQUIPMENT_BLACKLIST));
    mme_self()->eir.enabled = false;
    ABTS_TRUE(tc, !mme_s13_check_wanted(mme_ue));
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_CONTINUE, mme_s13_precheck(mme_ue));
    mme_self()->eir.enabled = true;

    mme_ue->nas_eps.attach.value = OGS_NAS_ATTACH_TYPE_EPS_EMERGENCY_ATTACH;
    ABTS_TRUE(tc, !mme_s13_check_wanted(mme_ue));
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_CONTINUE, mme_s13_precheck(mme_ue));
    mme_ue->nas_eps.attach.value = OGS_NAS_ATTACH_TYPE_EPS_ATTACH;

    mme_ue->nas_eps.type = MME_EPS_TYPE_TAU_REQUEST;
    ABTS_TRUE(tc, !mme_s13_check_wanted(mme_ue));
    ABTS_INT_EQUAL(tc, MME_S13_PRECHECK_CONTINUE, mme_s13_precheck(mme_ue));

    mme_self()->eir.enabled = false;
    mme_eir_cache_remove_all();
    ogs_free(mme_ue);
}

abts_suite *test_mme_s13(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    s13_context_setup();

    /* Decision logic */
    abts_run_test(suite, s13_test_equipment_status, NULL);
    abts_run_test(suite, s13_test_diameter_result_mapping, NULL);
    abts_run_test(suite, s13_test_handle_eca, NULL);
    abts_run_test(suite, s13_test_timeout_follows_failure_action, NULL);
    abts_run_test(suite, s13_test_imeisv_is_usable, NULL);
    abts_run_test(suite, s13_test_pei_from_terminal_info, NULL);

    /* Cache */
    abts_run_test(suite, s13_test_cache_basic, NULL);
    abts_run_test(suite, s13_test_cache_key_is_copied, NULL);
    abts_run_test(suite, s13_test_cache_freshness, NULL);
    abts_run_test(suite, s13_test_cache_lookup_ttl, NULL);
    abts_run_test(suite, s13_test_cache_lru_eviction, NULL);

    /* Attach fast path pre-check */
    abts_run_test(suite, s13_test_precheck, NULL);

    s13_context_teardown();

    return suite;
}
