/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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
 * CHF CDR writer unit tests (Phase 1e).
 *
 * These tests verify:
 *   1. CDR file is created at the correct date-based path
 *      {base}/YYYY/MM/DD/cdr-{charging_data_ref}.json
 *   2. CDR JSON contains all mandatory TS 23.255 §7 / TS 32.298 fields
 *   3. causeForRecordClosing is "normalRelease" vs "volumeLimit" vs
 *      "managementIntervention" correctly
 *   4. CDR writing is skipped when cdr.enable is false
 *   5. UL/DL volume values written to CDR JSON match session counters exactly
 *   6. Zero-duration session produces a valid CDR (start == end time)
 *
 * All tests use a local temp directory (/tmp/chf-unit-test-cdr/) to
 * avoid touching the system log path.  The directory is cleaned up at
 * the end of each test case.
 *
 * NOTE on make_test_sess():
 *   chf_sess_add() already allocates sess->supi via ogs_strdup().
 *   Only gpsi/dnn and numeric fields are set here — do NOT reassign
 *   sess->supi or it will leak the allocation made by chf_sess_add().
 */

#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "ogs-sbi.h"
#include "chf/context.h"
#include "chf/cdr-path.h"
#include "core/abts.h"

#define CDR_TEST_BASE   "/tmp/chf-unit-test-cdr"

/* Helper: recursively remove a directory tree (like rm -rf) */
static void rmrf(const char *path)
{
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "rm -rf '%s'", path);
    (void)system(cmd);
}

/* Helper: populate a minimal chf_sess_t suitable for CDR writing.
 *
 * chf_sess_add() already sets sess->supi via ogs_strdup(supi).
 * We must NOT overwrite sess->supi here — doing so would orphan the
 * first allocation and leak memory (visible in the talloc report). */
static chf_sess_t *make_test_sess(
        const char *supi, const char *ref,
        uint64_t ul, uint64_t dl, uint32_t dur, bool final)
{
    chf_sess_t *sess = chf_sess_add(supi, ref);
    if (!sess) return NULL;

    /* supi is already ogs_strdup'd by chf_sess_add() — do not reassign */
    sess->gpsi               = ogs_strdup("msisdn-447700900001");
    sess->dnn                = ogs_strdup("internet");
    sess->psi                = 5;
    sess->rating_group       = 1;
    sess->ul_octets          = ul;
    sess->dl_octets          = dl;
    sess->duration           = dur;
    sess->granted_quota_total = 10 * 1024 * 1024;
    sess->final_unit         = final;
    sess->final_unit_action  = 0; /* TERMINATE */

    return sess;
}

static void free_test_sess(chf_sess_t *sess)
{
    if (!sess) return;
    /* chf_sess_remove() frees supi/gpsi/dnn — do not free them separately */
    chf_sess_remove(sess);
}

/*
 * The CDR mirrors the TS 32.298 record, so a field sits where the ASN.1
 * puts it rather than at the top level.  These walk the nesting the
 * tests need.
 */
static cJSON *cdr_record(cJSON *root)
{
    return cJSON_GetObjectItemCaseSensitive(root, "chargingFunctionRecord");
}

static cJSON *cdr_pdu(cJSON *root)
{
    return cJSON_GetObjectItemCaseSensitive(cdr_record(root),
            "pDUSessionChargingInformation");
}

/* listOfMultipleUnitUsage[0].usedUnitContainers[0], where the volumes are. */
static cJSON *cdr_container(cJSON *root)
{
    cJSON *list = cJSON_GetObjectItemCaseSensitive(cdr_record(root),
            "listOfMultipleUnitUsage");
    cJSON *usage = cJSON_GetArrayItem(list, 0);

    return cJSON_GetArrayItem(
            cJSON_GetObjectItemCaseSensitive(usage, "usedUnitContainers"), 0);
}

/* The causeForRecClosing name the record carries, or NULL. */
static const char *cdr_cause(cJSON *root)
{
    cJSON *item = cJSON_GetObjectItemCaseSensitive(cdr_record(root),
            "causeForRecClosing");

    return cJSON_IsString(item) ? cJSON_GetStringValue(item) : NULL;
}

/* ------------------------------------------------------------------ */
/* Test 1: CDR file created at correct date-structured path            */
/* Expected: /tmp/chf-unit-test-cdr/YYYY/MM/DD/cdr-<ref>.json        */
/* ------------------------------------------------------------------ */
static void test_cdr_file_path(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    char expected_dir[256];
    char expected_file[512];
    struct stat st;
    time_t now;
    struct tm tm_utc;

    rmrf(CDR_TEST_BASE);

    /* Configure CDR path in the context */
    chf_self()->cdr.enable = true;
    if (chf_self()->cdr.path)
        ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_cdr_init();

    sess = make_test_sess("imsi-000000000000001",
                          "cdr-test-uuid-path-0001",
                          1024, 2048, 30, false);
    ABTS_PTR_NOTNULL(tc, sess);

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    /* Build expected path */
    now = time(NULL);
    gmtime_r(&now, &tm_utc);
    snprintf(expected_dir, sizeof(expected_dir),
             CDR_TEST_BASE "/%04d/%02d/%02d",
             tm_utc.tm_year + 1900,
             tm_utc.tm_mon  + 1,
             tm_utc.tm_mday);
    snprintf(expected_file, sizeof(expected_file),
             "%s/cdr-cdr-test-uuid-path-0001.json", expected_dir);

    /* Verify the returned path matches expected */
    ABTS_STR_EQUAL(tc, expected_file, cdr_file);

    /* Verify the file actually exists */
    ABTS_INT_EQUAL(tc, 0, stat(cdr_file, &st));
    ABTS_INT_EQUAL(tc, 1, S_ISREG(st.st_mode) ? 1 : 0);

    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/* ------------------------------------------------------------------ */
/* Test 2: CDR JSON contains all mandatory fields                      */
/* TS 23.255 §7 / TS 32.298 §5.1.1 mandatory fields                  */
/* ------------------------------------------------------------------ */
static void test_cdr_json_fields(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    FILE *fp;
    char buf[4096];
    size_t nread;

    rmrf(CDR_TEST_BASE);
    chf_self()->cdr.enable = true;
    if (chf_self()->cdr.path) ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_cdr_init();

    sess = make_test_sess("imsi-000000000000002",
                          "cdr-test-uuid-fields-0002",
                          512000, 2048000, 120, false);
    ABTS_PTR_NOTNULL(tc, sess);

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    fp = fopen(cdr_file, "r");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    buf[nread] = '\0';

    /* Check mandatory JSON keys are present */
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"recordType\"")             ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"chargingSessionIdentifier\"")        ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"subscriptionIDData\"")                   ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"userIdentifier\"")                   ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"dataNetworkNameIdentifier\"")                    ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"pDUSessionId\"")           ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"ratingGroup\"")            ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"dataVolumeUplink\"")       ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"dataVolumeDownlink\"")     ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"duration\"")               ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"pDUSessionstartTime\"")              ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"pDUSessionstopTime\"")                ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"causeForRecClosing\"")  ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1,
            strstr(buf, "\"recordingNetworkFunctionID\"")             ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1,
            strstr(buf, "\"nFunctionConsumerInformation\"")           ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1,
            strstr(buf, "\"recordOpeningTime\"")                      ? 1 : 0);

    /* Check specific values */
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"chargingFunctionRecord\"")              ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "imsi-000000000000002")       ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "normalRelease")              ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "512000")                     ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "2048000")                    ? 1 : 0);

    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/* ------------------------------------------------------------------ */
/* Test 3: causeForRecordClosing — "normalRelease" vs "volumeLimit"   */
/* TS 32.298 §5.1.1 — cause codes                                     */
/* ------------------------------------------------------------------ */
static void test_cdr_cause_strings(abts_case *tc, void *data)
{
    chf_sess_t *sess_normal, *sess_limit;
    char *cdr_normal, *cdr_limit;
    FILE *fp;
    char buf[4096];
    size_t nread;

    rmrf(CDR_TEST_BASE);
    chf_self()->cdr.enable = true;
    if (chf_self()->cdr.path) ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_cdr_init();

    /* Session 1: normal release */
    sess_normal = make_test_sess("imsi-000000000000003",
                                 "cdr-test-uuid-cause-normal",
                                 100, 200, 10, false);
    ABTS_PTR_NOTNULL(tc, sess_normal);
    cdr_normal = chf_cdr_write(sess_normal, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_normal);

    fp = fopen(cdr_normal, "r");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    buf[nread] = '\0';
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"normalRelease\"") ? 1 : 0);
    ABTS_INT_EQUAL(tc, 0, strstr(buf, "\"volumeLimit\"")   ? 1 : 0);

    /* Session 2: volume limit exhaustion */
    sess_limit = make_test_sess("imsi-000000000000004",
                                "cdr-test-uuid-cause-limit",
                                5*1024*1024, 5*1024*1024, 60, true);
    ABTS_PTR_NOTNULL(tc, sess_limit);
    cdr_limit = chf_cdr_write(sess_limit, "volumeLimit");
    ABTS_PTR_NOTNULL(tc, cdr_limit);

    fp = fopen(cdr_limit, "r");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    buf[nread] = '\0';
    ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"volumeLimit\"")    ? 1 : 0);
    ABTS_INT_EQUAL(tc, 0, strstr(buf, "\"normalRelease\"")  ? 1 : 0);

    /* Session 3: the operator closed it — a ChargingNotify ABORT_CHARGING.
     * TS 32.298 §5.1.1.5.2 has a value for exactly this, and it must not
     * read as an ordinary release: the subscriber still had quota. */
    {
        chf_sess_t *sess_abort = make_test_sess("imsi-000000000000005",
                                    "cdr-test-uuid-cause-abort",
                                    3*1024*1024, 7*1024*1024, 90, false);
        char *cdr_abort;
        ABTS_PTR_NOTNULL(tc, sess_abort);
        cdr_abort = chf_cdr_write(sess_abort, "managementIntervention");
        ABTS_PTR_NOTNULL(tc, cdr_abort);

        fp = fopen(cdr_abort, "r");
        ABTS_PTR_NOTNULL(tc, fp);
        nread = fread(buf, 1, sizeof(buf) - 1, fp);
        fclose(fp);
        buf[nread] = '\0';
        ABTS_INT_EQUAL(tc, 1, strstr(buf, "\"managementIntervention\"") ? 1 : 0);
        ABTS_INT_EQUAL(tc, 0, strstr(buf, "\"normalRelease\"")          ? 1 : 0);
        ABTS_INT_EQUAL(tc, 0, strstr(buf, "\"volumeLimit\"")            ? 1 : 0);

        ogs_free(cdr_abort);
        free_test_sess(sess_abort);
    }

    ogs_free(cdr_normal);
    ogs_free(cdr_limit);
    free_test_sess(sess_normal);
    free_test_sess(sess_limit);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/* ------------------------------------------------------------------ */
/* Test 4: CDR skipped when cdr.enable is false                       */
/* ------------------------------------------------------------------ */
static void test_cdr_disabled(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *result;

    chf_self()->cdr.enable = false;
    if (chf_self()->cdr.path) ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_cdr_init();

    sess = make_test_sess("imsi-000000000000005",
                          "cdr-test-uuid-disabled-0005",
                          0, 0, 0, false);
    ABTS_PTR_NOTNULL(tc, sess);

    result = chf_cdr_write(sess, "normalRelease");
    /* When disabled, chf_cdr_write returns NULL (no file written) */
    ABTS_PTR_EQUAL(tc, NULL, result);

    free_test_sess(sess);
    chf_cdr_final();

    /* Restore for subsequent tests */
    chf_self()->cdr.enable = true;
}

/* ------------------------------------------------------------------ */
/* Test 5: Verify actual UL/DL volume values in CDR JSON              */
/*                                                                      */
/* Ensures cJSON serialises uint64_t volume counters without truncation */
/* and that the values round-trip correctly via cJSON_GetNumberValue(). */
/* TS 32.298 §5.1.1 — dataVolumeUplink / dataVolumeDownlink            */
/* ------------------------------------------------------------------ */
static void test_cdr_volume_values_exact(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    FILE *fp;
    char buf[4096];
    size_t nread;
    cJSON *cdr_json = NULL;
    cJSON *item     = NULL;

    const uint64_t test_ul = 123456789ULL;  /* ~118 MB */
    const uint64_t test_dl = 987654321ULL;  /* ~942 MB */
    const uint32_t test_dur = 3601;         /* just over 1 hour */

    rmrf(CDR_TEST_BASE);
    chf_self()->cdr.enable = true;
    if (chf_self()->cdr.path) ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_cdr_init();

    sess = make_test_sess("imsi-000000000000010",
                          "cdr-test-uuid-volumes-0010",
                          test_ul, test_dl, test_dur, false);
    ABTS_PTR_NOTNULL(tc, sess);

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    fp = fopen(cdr_file, "r");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    buf[nread] = '\0';

    /* Parse CDR JSON and verify numeric fields exactly.
     * cJSON stores all numbers as double, which is precise up to 2^53 —
     * our test volumes are well within that range. */
    cdr_json = cJSON_Parse(buf);
    ABTS_PTR_NOTNULL(tc, cdr_json);

    item = cJSON_GetObjectItemCaseSensitive(cdr_container(cdr_json),
            "dataVolumeUplink");
    ABTS_PTR_NOTNULL(tc, item);
    ABTS_INT_EQUAL(tc, (int)test_ul, (int)cJSON_GetNumberValue(item));

    item = cJSON_GetObjectItemCaseSensitive(cdr_container(cdr_json),
            "dataVolumeDownlink");
    ABTS_PTR_NOTNULL(tc, item);
    ABTS_INT_EQUAL(tc, (int)test_dl, (int)cJSON_GetNumberValue(item));

    item = cJSON_GetObjectItemCaseSensitive(cdr_record(cdr_json), "duration");
    ABTS_PTR_NOTNULL(tc, item);
    ABTS_INT_EQUAL(tc, (int)test_dur, (int)cJSON_GetNumberValue(item));

    /* There is no separate exhausted flag: causeForRecClosing says it. */
    ABTS_PTR_NOTNULL(tc, (void *)cdr_cause(cdr_json));
    ABTS_STR_EQUAL(tc, "normalRelease", cdr_cause(cdr_json));

    cJSON_Delete(cdr_json);
    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/* ------------------------------------------------------------------ */
/* Test 6: Zero-duration session produces a valid CDR                  */
/*                                                                      */
/* A Create→immediate Release (no Update) has duration=0.  The CDR    */
/* writer computes start_time = now - 0 = now, so startTime==endTime. */
/* The file must still be written and parseable.                       */
/* ------------------------------------------------------------------ */
static void test_cdr_zero_duration(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    FILE *fp;
    char buf[4096];
    size_t nread;
    cJSON *cdr_json = NULL;
    cJSON *item     = NULL;

    rmrf(CDR_TEST_BASE);
    chf_self()->cdr.enable = true;
    if (chf_self()->cdr.path) ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_cdr_init();

    /* Create a session with zero usage and zero duration
     * (models a session released immediately after Create). */
    sess = make_test_sess("imsi-000000000000011",
                          "cdr-test-uuid-zerodur-0011",
                          0, 0, 0, false);
    ABTS_PTR_NOTNULL(tc, sess);

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    /* File must exist and contain valid JSON */
    fp = fopen(cdr_file, "r");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    buf[nread] = '\0';

    cdr_json = cJSON_Parse(buf);
    ABTS_PTR_NOTNULL(tc, cdr_json);

    /* Volumes and duration must be zero */
    item = cJSON_GetObjectItemCaseSensitive(cdr_container(cdr_json),
            "dataVolumeUplink");
    ABTS_PTR_NOTNULL(tc, item);
    ABTS_INT_EQUAL(tc, 0, (int)cJSON_GetNumberValue(item));

    item = cJSON_GetObjectItemCaseSensitive(cdr_container(cdr_json),
            "dataVolumeDownlink");
    ABTS_PTR_NOTNULL(tc, item);
    ABTS_INT_EQUAL(tc, 0, (int)cJSON_GetNumberValue(item));

    item = cJSON_GetObjectItemCaseSensitive(cdr_record(cdr_json), "duration");
    ABTS_PTR_NOTNULL(tc, item);
    ABTS_INT_EQUAL(tc, 0, (int)cJSON_GetNumberValue(item));

    /* startTime and endTime must both be present (and identical or very close) */
    ABTS_PTR_NOTNULL(tc,
        cJSON_GetObjectItemCaseSensitive(cdr_pdu(cdr_json),
            "pDUSessionstartTime"));
    ABTS_PTR_NOTNULL(tc,
        cJSON_GetObjectItemCaseSensitive(cdr_pdu(cdr_json),
            "pDUSessionstopTime"));

    cJSON_Delete(cdr_json);
    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/* ------------------------------------------------------------------ */
/* Test 7: Large volumes > 2 GiB do not overflow cJSON serialisation   */
/*                                                                      */
/* Fixes Gap G-6: ul_octets/dl_octets are uint64_t.  cJSON stores     */
/* numbers as double (IEEE 754), which is exact up to 2^53.  Values   */
/* up to ~9 PiB are representable.  The critical boundary is 2^31-1   */
/* (INT32_MAX = 2 147 483 647), which is where a naive int cast breaks. */
/* This test uses 3 GiB UL + 4 GiB DL (both > INT32_MAX) and verifies */
/* the CDR JSON encodes and re-parses them without truncation.          */
/* ------------------------------------------------------------------ */
static void test_cdr_large_volume(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    FILE *fp;
    char buf[4096];
    size_t nread;
    cJSON *cdr_json = NULL;
    cJSON *item     = NULL;

    /* 3 GiB UL, 4 GiB DL — both exceed INT32_MAX (2 147 483 647) */
    const uint64_t ul = 3ULL * 1024 * 1024 * 1024;  /* 3 221 225 472 */
    const uint64_t dl = 4ULL * 1024 * 1024 * 1024;  /* 4 294 967 296 */

    rmrf(CDR_TEST_BASE);
    chf_self()->cdr.enable = true;
    if (chf_self()->cdr.path) ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_cdr_init();

    sess = make_test_sess("imsi-000000000000012",
                          "cdr-test-uuid-largev-0012",
                          ul, dl, 7200, true);
    ABTS_PTR_NOTNULL(tc, sess);

    cdr_file = chf_cdr_write(sess, "volumeLimit");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    fp = fopen(cdr_file, "r");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    buf[nread] = '\0';

    cdr_json = cJSON_Parse(buf);
    ABTS_PTR_NOTNULL(tc, cdr_json);

    /* Use double comparison — cJSON stores numbers as double (exact up to 2^53) */
    item = cJSON_GetObjectItemCaseSensitive(cdr_container(cdr_json),
            "dataVolumeUplink");
    ABTS_PTR_NOTNULL(tc, item);
    ABTS_INT_EQUAL(tc, 1,
        ((uint64_t)cJSON_GetNumberValue(item) == ul) ? 1 : 0);

    item = cJSON_GetObjectItemCaseSensitive(cdr_container(cdr_json),
            "dataVolumeDownlink");
    ABTS_PTR_NOTNULL(tc, item);
    ABTS_INT_EQUAL(tc, 1,
        ((uint64_t)cJSON_GetNumberValue(item) == dl) ? 1 : 0);

    /* There is no separate exhausted flag: causeForRecClosing says it. */
    ABTS_PTR_NOTNULL(tc, (void *)cdr_cause(cdr_json));
    ABTS_STR_EQUAL(tc, "volumeLimit", cdr_cause(cdr_json));

    cJSON_Delete(cdr_json);
    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/* ------------------------------------------------------------------ */
/* Suite registration                                                   */
/* ------------------------------------------------------------------ */
/* Which sessions are worth a CDR                                       */
/*                                                                      */
/* A session refused for an empty allowance was granted nothing and     */
/* used nothing; a stream of those would bury the real records.  Any    */
/* session that carried traffic, or that held a grant at all, keeps its */
/* CDR - including one that attached and used nothing.                  */
/* ------------------------------------------------------------------ */
static void test_cdr_is_billable(abts_case *tc, void *data)
{
    chf_sess_t *sess = chf_sess_add("imsi-999700000009001",
            "cdr-billable-0001");
    ABTS_PTR_NOTNULL(tc, sess);

    /* Refused session: no grant, no traffic */
    sess->granted_quota_total = 0;
    sess->ul_octets = 0;
    sess->dl_octets = 0;
    ABTS_TRUE(tc, chf_cdr_is_billable(sess) == false);

    /* Attached, granted quota, used nothing */
    sess->granted_quota_total = 50 * 1024 * 1024;
    ABTS_TRUE(tc, chf_cdr_is_billable(sess) == true);

    /* Traffic but no grant left (the final report of an exhausted plan) */
    sess->granted_quota_total = 0;
    sess->dl_octets = 1;
    ABTS_TRUE(tc, chf_cdr_is_billable(sess) == true);

    sess->dl_octets = 0;
    sess->ul_octets = 1;
    ABTS_TRUE(tc, chf_cdr_is_billable(sess) == true);

    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* The slice the session ran on appears in the CDR                      */
/*                                                                      */
/* The SMF sends it as pDUSessionChargingInformation.pduSessionInfor-  */
/* mation.networkSlicingInfo.sNSSAI (TS 32.291); the CDR carries it so  */
/* records can be attributed per slice (TS 32.298).  A session whose    */
/* SMF sent no slice has no sNSSAI member rather than an empty one.     */
/* ------------------------------------------------------------------ */
static void test_cdr_snssai_field(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    FILE *fp;
    char buf[4096];
    size_t nread;

    rmrf(CDR_TEST_BASE);
    chf_self()->cdr.enable = true;
    if (chf_self()->cdr.path) ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_cdr_init();

    /* sst with a slice differentiator */
    sess = make_test_sess("imsi-000000000000010",
                          "cdr-test-uuid-snssai-0010", 1024, 2048, 10, false);
    ABTS_PTR_NOTNULL(tc, sess);
    sess->s_nssai.sst = 1;
    sess->s_nssai.sd.v = 0x010203;

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    fp = fopen(cdr_file, "r");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(buf, 1, sizeof(buf) - 1, fp);
    buf[nread] = '\0';
    fclose(fp);

    ABTS_TRUE(tc, strstr(buf, "\"networkSliceInstanceID\"") != NULL);
    ABTS_TRUE(tc, strstr(buf, "\"sST\":1") != NULL);
    ABTS_TRUE(tc, strstr(buf, "\"sD\":\"010203\"") != NULL);

    ogs_free(cdr_file);
    free_test_sess(sess);

    /* No slice: the member is left out entirely */
    sess = make_test_sess("imsi-000000000000011",
                          "cdr-test-uuid-snssai-0011", 1024, 2048, 10, false);
    ABTS_PTR_NOTNULL(tc, sess);
    ABTS_INT_EQUAL(tc, 0, sess->s_nssai.sst);

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    fp = fopen(cdr_file, "r");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(buf, 1, sizeof(buf) - 1, fp);
    buf[nread] = '\0';
    fclose(fp);

    ABTS_TRUE(tc, strstr(buf, "networkSliceInstanceID") == NULL);

    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/* ------------------------------------------------------------------ */
/* Encodings and filing                                                 */
/* ------------------------------------------------------------------ */

/* Today's CDR directory, which is where every test's files land. */
static void today_path(char *out, size_t outsz)
{
    time_t now = time(NULL);
    struct tm tm_utc;

    gmtime_r(&now, &tm_utc);
    snprintf(out, outsz, CDR_TEST_BASE "/%04d/%02d/%02d",
            tm_utc.tm_year + 1900, tm_utc.tm_mon + 1, tm_utc.tm_mday);
}

/* How many files in today's directory end in `suffix`. */
static int count_files(const char *suffix)
{
    char dir[256];
    DIR *d;
    struct dirent *ent;
    size_t slen = strlen(suffix);
    int n = 0;

    today_path(dir, sizeof(dir));

    d = opendir(dir);
    if (!d)
        return 0;

    while ((ent = readdir(d)) != NULL) {
        size_t len = strlen(ent->d_name);
        if (len >= slen && !strcmp(ent->d_name + len - slen, suffix))
            n++;
    }
    closedir(d);

    return n;
}

/* Put the writer in a known state: our temp path, one named format. */
static void setup_formats(int formats)
{
    rmrf(CDR_TEST_BASE);
    chf_self()->cdr.enable = true;
    chf_self()->cdr.formats = formats;
    if (chf_self()->cdr.path) ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(CDR_TEST_BASE);
    chf_self()->cdr.json.aggregate = false;
    chf_self()->cdr.asn1.aggregate = false;
    chf_cdr_init();
}

/*
 * format: asn1 writes a .ber and no .json.
 *
 * The first three bytes are asserted because they are how a reader
 * identifies the record: the context-specific constructed tag 200 that
 * TS 32.298 assigns chargingFunctionRecord, in its multi-byte form.
 */
static void test_cdr_asn1_file(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    FILE *fp;
    unsigned char head[4];
    size_t nread;

    setup_formats(CHF_CDR_FMT_ASN1);

    sess = make_test_sess("imsi-000000000000020",
                          "cdr-test-uuid-asn1-0020",
                          4096, 8192, 60, false);
    ABTS_PTR_NOTNULL(tc, sess);

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    /* The returned path is the .ber, since no JSON was asked for. */
    ABTS_TRUE(tc, strstr(cdr_file, ".ber") != NULL);
    ABTS_INT_EQUAL(tc, 1, count_files(".ber"));
    ABTS_INT_EQUAL(tc, 0, count_files(".json"));

    fp = fopen(cdr_file, "rb");
    ABTS_PTR_NOTNULL(tc, fp);
    nread = fread(head, 1, sizeof(head), fp);
    fclose(fp);

    ABTS_INT_EQUAL(tc, 4, (int)nread);
    ABTS_INT_EQUAL(tc, 0xbf, head[0]);
    ABTS_INT_EQUAL(tc, 0x81, head[1]);
    ABTS_INT_EQUAL(tc, 0x48, head[2]);

    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/* format: both writes one of each, named the same but for the extension. */
static void test_cdr_both_formats(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    char ber_path[512];
    struct stat st;
    char *dot;

    setup_formats(CHF_CDR_FMT_JSON | CHF_CDR_FMT_ASN1);

    sess = make_test_sess("imsi-000000000000021",
                          "cdr-test-uuid-both-0021",
                          1000, 2000, 120, false);
    ABTS_PTR_NOTNULL(tc, sess);

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);

    /* JSON is what the CDR index records when both are written. */
    ABTS_TRUE(tc, strstr(cdr_file, ".json") != NULL);

    ABTS_INT_EQUAL(tc, 1, count_files(".json"));
    ABTS_INT_EQUAL(tc, 1, count_files(".ber"));

    /* The companion .ber sits beside it. */
    ogs_cpystrn(ber_path, cdr_file, sizeof(ber_path));
    dot = strrchr(ber_path, '.');
    ABTS_PTR_NOTNULL(tc, dot);
    if (dot) {
        ogs_cpystrn(dot, ".ber", sizeof(ber_path) - (dot - ber_path));
        ABTS_INT_EQUAL(tc, 0, stat(ber_path, &st));
        ABTS_TRUE(tc, st.st_size > 0);
    }

    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();
    rmrf(CDR_TEST_BASE);
}

/*
 * Aggregation by record count.
 *
 * max_records 3 over 7 sessions must give 3 files - 3, 3 and 1 - and
 * every record must still be in one of them.
 */
static void test_cdr_aggregation_by_records(abts_case *tc, void *data)
{
    int i;
    char first[512] = "";

    setup_formats(CHF_CDR_FMT_JSON);
    chf_self()->cdr.json.aggregate = true;
    chf_self()->cdr.json.max_records = 3;
    chf_self()->cdr.json.max_bytes = 0;      /* size is not the trigger */

    for (i = 0; i < 7; i++) {
        chf_sess_t *sess;
        char ref[64];
        char *cdr_file;

        snprintf(ref, sizeof(ref), "cdr-test-uuid-agg-%04d", i);
        sess = make_test_sess("imsi-000000000000030", ref,
                              100 + i, 200 + i, 10, false);
        ABTS_PTR_NOTNULL(tc, sess);

        cdr_file = chf_cdr_write(sess, "normalRelease");
        ABTS_PTR_NOTNULL(tc, cdr_file);

        /* An aggregated file is not named after the session. */
        ABTS_TRUE(tc, strstr(cdr_file, ref) == NULL);

        if (!first[0])
            ogs_cpystrn(first, cdr_file, sizeof(first));

        ogs_free(cdr_file);
        free_test_sess(sess);
    }

    chf_cdr_final();   /* closes the open file before we count lines */

    ABTS_INT_EQUAL(tc, 3, count_files(".json"));

    /* The first file holds exactly max_records lines. */
    {
        FILE *fp = fopen(first, "r");
        int lines = 0, c;

        ABTS_PTR_NOTNULL(tc, fp);
        if (fp) {
            while ((c = fgetc(fp)) != EOF)
                if (c == '\n') lines++;
            fclose(fp);
        }
        ABTS_INT_EQUAL(tc, 3, lines);
    }

    chf_self()->cdr.json.aggregate = false;
    chf_self()->cdr.json.max_records = 1000;
    rmrf(CDR_TEST_BASE);
}

/*
 * Aggregation by size.
 *
 * The threshold is checked before a record is appended, so a file does
 * not grow past it - which also means the limit must not be so small
 * that one record cannot fit, or nothing could ever be written.
 */
static void test_cdr_aggregation_by_size(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;
    struct stat st;
    off_t one_record;
    int i;

    /*
     * The threshold has to be stated in terms of a real record, not a
     * guess: a CHF CDR is several hundred bytes and its exact size moves
     * with the field set, so a hard-coded limit silently becomes either
     * "one record per file" or "never rolls".  Write one and measure it.
     */
    setup_formats(CHF_CDR_FMT_JSON);
    sess = make_test_sess("imsi-000000000000031", "cdr-test-uuid-aggsz-9999",
                          100, 200, 10, false);
    ABTS_PTR_NOTNULL(tc, sess);
    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_NOTNULL(tc, cdr_file);
    ABTS_INT_EQUAL(tc, 0, stat(cdr_file, &st));
    one_record = st.st_size;
    ABTS_TRUE(tc, one_record > 0);
    ogs_free(cdr_file);
    free_test_sess(sess);
    chf_cdr_final();

    /*
     * Room for two records but not three, so six of them fill three
     * files.  Two and a half rather than two exactly, because records
     * are not all the same length - pDUSessionChargingID is a hash, so
     * its decimal form varies by a few digits - and half a record of
     * slack is far more than that variation.
     */
    setup_formats(CHF_CDR_FMT_JSON);
    chf_self()->cdr.json.aggregate = true;
    chf_self()->cdr.json.max_records = 0;    /* count is not the trigger */
    chf_self()->cdr.json.max_bytes = (uint64_t)one_record * 5 / 2;

    for (i = 0; i < 6; i++) {
        char ref[64];

        snprintf(ref, sizeof(ref), "cdr-test-uuid-aggsz-%04d", i);
        sess = make_test_sess("imsi-000000000000031", ref,
                              100, 200, 10, false);
        ABTS_PTR_NOTNULL(tc, sess);

        cdr_file = chf_cdr_write(sess, "normalRelease");
        ABTS_PTR_NOTNULL(tc, cdr_file);
        ogs_free(cdr_file);
        free_test_sess(sess);
    }

    chf_cdr_final();

    ABTS_INT_EQUAL(tc, 3, count_files(".json"));

    chf_self()->cdr.json.aggregate = false;
    chf_self()->cdr.json.max_records = 1000;
    chf_self()->cdr.json.max_bytes = 16ULL * 1024 * 1024;
    rmrf(CDR_TEST_BASE);
}

/*
 * The two encodings file independently.
 *
 * This is the arrangement the config exists for: an aggregated BER
 * stream for a mediation system, a file per session in JSON for a
 * person.  Three sessions must therefore leave three .json files and
 * one .ber.
 */
static void test_cdr_filing_per_format(abts_case *tc, void *data)
{
    int i;

    setup_formats(CHF_CDR_FMT_JSON | CHF_CDR_FMT_ASN1);
    chf_self()->cdr.json.aggregate = false;
    chf_self()->cdr.asn1.aggregate = true;
    chf_self()->cdr.asn1.max_records = 0;
    chf_self()->cdr.asn1.max_bytes = 0;

    for (i = 0; i < 3; i++) {
        chf_sess_t *sess;
        char ref[64];
        char *cdr_file;

        snprintf(ref, sizeof(ref), "cdr-test-uuid-mixed-%04d", i);
        sess = make_test_sess("imsi-000000000000032", ref,
                              1000, 2000, 30, false);
        ABTS_PTR_NOTNULL(tc, sess);

        cdr_file = chf_cdr_write(sess, "normalRelease");
        ABTS_PTR_NOTNULL(tc, cdr_file);
        ogs_free(cdr_file);
        free_test_sess(sess);
    }

    chf_cdr_final();

    ABTS_INT_EQUAL(tc, 3, count_files(".json"));
    ABTS_INT_EQUAL(tc, 1, count_files(".ber"));

    chf_self()->cdr.asn1.aggregate = false;
    chf_self()->cdr.asn1.max_records = 1000;
    chf_self()->cdr.asn1.max_bytes = 16ULL * 1024 * 1024;
    rmrf(CDR_TEST_BASE);
}

/*
 * The BER sink aggregates too, and its records concatenate.
 *
 * The record-count threshold was only ever tested on the JSON sink, whose
 * records are newline-delimited.  BER has no delimiter — records are
 * self-delimiting TLVs — so this checks the sink rolls on count and that
 * the resulting file is the concatenation of whole records, which is what
 * the shipped decoder relies on.
 */
static void test_cdr_asn1_aggregation_by_records(abts_case *tc, void *data)
{
    int i;
    char first[512] = "";

    setup_formats(CHF_CDR_FMT_ASN1);
    chf_self()->cdr.asn1.aggregate = true;
    chf_self()->cdr.asn1.max_records = 2;
    chf_self()->cdr.asn1.max_bytes = 0;

    for (i = 0; i < 5; i++) {
        chf_sess_t *sess;
        char ref[64];
        char *cdr_file;

        snprintf(ref, sizeof(ref), "cdr-test-uuid-beragg-%04d", i);
        sess = make_test_sess("imsi-000000000000034", ref,
                              1000 + i, 2000 + i, 10, false);
        ABTS_PTR_NOTNULL(tc, sess);

        cdr_file = chf_cdr_write(sess, "normalRelease");
        ABTS_PTR_NOTNULL(tc, cdr_file);
        ABTS_TRUE(tc, strstr(cdr_file, ".ber") != NULL);
        if (!first[0])
            ogs_cpystrn(first, cdr_file, sizeof(first));
        ogs_free(cdr_file);
        free_test_sess(sess);
    }

    chf_cdr_final();

    /* 5 records at 2 per file → 3 files, and no JSON. */
    ABTS_INT_EQUAL(tc, 3, count_files(".ber"));
    ABTS_INT_EQUAL(tc, 0, count_files(".json"));

    /*
     * The first file must be exactly two whole records.  Walking it by
     * tag and length is the check that matters: a short write or a stray
     * byte between records would leave the remainder undecodable.
     */
    {
        FILE *fp = fopen(first, "rb");
        unsigned char buf[8192];
        size_t n, off = 0;
        int records = 0;

        ABTS_PTR_NOTNULL(tc, fp);
        if (fp) {
            n = fread(buf, 1, sizeof(buf), fp);
            fclose(fp);

            while (off + 3 < n) {
                size_t len;

                /* Context-specific constructed tag 200, multi-byte form. */
                if (buf[off] != 0xbf || buf[off + 1] != 0x81 ||
                        buf[off + 2] != 0x48)
                    break;
                off += 3;

                /* Definite length: short form, or long form with a count. */
                if (buf[off] < 0x80) {
                    len = buf[off];
                    off += 1;
                } else {
                    size_t nbytes = buf[off] & 0x7f;
                    size_t k;
                    off += 1;
                    if (!nbytes || off + nbytes > n)
                        break;
                    len = 0;
                    for (k = 0; k < nbytes; k++)
                        len = (len << 8) | buf[off + k];
                    off += nbytes;
                }

                if (off + len > n)
                    break;      /* a record running past the end */
                off += len;
                records++;
            }

            ABTS_INT_EQUAL(tc, 2, records);
            ABTS_INT_EQUAL(tc, 1, off == n ? 1 : 0);
        }
    }

    chf_self()->cdr.asn1.aggregate = false;
    chf_self()->cdr.asn1.max_records = 1000;
    chf_self()->cdr.asn1.max_bytes = 16ULL * 1024 * 1024;
    rmrf(CDR_TEST_BASE);
}

/* format: none writes nothing, even with cdr.enable true. */
static void test_cdr_no_format(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    char *cdr_file;

    setup_formats(0);

    sess = make_test_sess("imsi-000000000000033",
                          "cdr-test-uuid-nofmt-0033",
                          1000, 2000, 30, false);
    ABTS_PTR_NOTNULL(tc, sess);

    cdr_file = chf_cdr_write(sess, "normalRelease");
    ABTS_PTR_EQUAL(tc, NULL, cdr_file);
    ABTS_INT_EQUAL(tc, 0, count_files(".json"));
    ABTS_INT_EQUAL(tc, 0, count_files(".ber"));

    free_test_sess(sess);
    chf_cdr_final();
    chf_self()->cdr.formats = CHF_CDR_FMT_JSON;
    rmrf(CDR_TEST_BASE);
}

/* ------------------------------------------------------------------ */
abts_suite *test_chf_cdr(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, test_cdr_file_path,          NULL);
    abts_run_test(suite, test_cdr_json_fields,         NULL);
    abts_run_test(suite, test_cdr_cause_strings,       NULL);
    abts_run_test(suite, test_cdr_disabled,            NULL);
    abts_run_test(suite, test_cdr_volume_values_exact, NULL);
    abts_run_test(suite, test_cdr_zero_duration,       NULL);
    abts_run_test(suite, test_cdr_large_volume,        NULL);
    abts_run_test(suite, test_cdr_is_billable,         NULL);
    abts_run_test(suite, test_cdr_snssai_field,        NULL);
    abts_run_test(suite, test_cdr_asn1_file,            NULL);
    abts_run_test(suite, test_cdr_both_formats,         NULL);
    abts_run_test(suite, test_cdr_aggregation_by_records, NULL);
    abts_run_test(suite, test_cdr_aggregation_by_size,  NULL);
    abts_run_test(suite, test_cdr_filing_per_format,    NULL);
    abts_run_test(suite, test_cdr_asn1_aggregation_by_records, NULL);
    abts_run_test(suite, test_cdr_no_format,            NULL);

    return suite;
}
