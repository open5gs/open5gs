/*
 * Copyright (C) 2026 by DNL
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

#include "ogs-cdr.h"
#include "core/abts.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TEST_EPOCH      1785600000      /* fixed for reproducibility */
#define TEST_TZ_MIN     (-240)          /* UTC-4 */

static void fill_record(ogs_cdr_pgw_record_t *r)
{
    static const uint8_t imsi[] =
        { 0x03, 0x32, 0x33, 0x10, 0x32, 0x54, 0x76, 0x98 };
    static const uint8_t pgw[4] = { 10, 99, 0, 10 };
    static const uint8_t sgw[4] = { 10, 99, 0, 11 };
    static const uint8_t ue[4] = { 10, 45, 0, 2 };

    memset(r, 0, sizeof(*r));
    memcpy(r->imsi, imsi, sizeof(imsi));
    r->imsi_len = sizeof(imsi);
    strcpy(r->apn_ni, "internet");
    r->charging_id = 1234567;
    memcpy(r->pgw_addr, pgw, 4);
    memcpy(r->sgw_addr, sgw, 4);
    r->pdn_type = OGS_CDR_PDN_TYPE_IPV4;
    r->has_pdn_addr = 1;
    memcpy(r->pdn_addr, ue, 4);
    r->dynamic_address = 1;
    r->rat_type = OGS_CDR_RAT_TYPE_EUTRAN;
    strcpy(r->node_id, "test-smf");
    r->opening_time = TEST_EPOCH;
    r->start_time = TEST_EPOCH;
    r->tz_offset_min = TEST_TZ_MIN;
    r->duration = 60;
    r->cause = OGS_CDR_CAUSE_NORMAL_RELEASE;
    r->local_sequence_number = 1;
    r->rating_group = 1;
    r->time_of_report = TEST_EPOCH + 60;
    r->service_condition = OGS_CDR_SCC_RECORD_CLOSURE;
}

static void cdr_encode_basic(abts_case *tc, void *data)
{
    ogs_cdr_pgw_record_t r;
    uint8_t buf[OGS_CDR_MAX_RECORD_LEN];
    int len;

    fill_record(&r);
    len = ogs_cdr_encode_pgw_record(&r, buf, sizeof(buf));
    ABTS_TRUE(tc, len > 0);

    /* GPRSRecord CHOICE pGWRecord [79]: high-tag 0xbf 0x4f */
    ABTS_INT_EQUAL(tc, 0xbf, buf[0]);
    ABTS_INT_EQUAL(tc, 0x4f, buf[1]);

    /* recordType [0] INTEGER 85 is the first SET member */
    {
        int off = 2;
        /* skip length octets */
        if (buf[off] & 0x80)
            off += 1 + (buf[off] & 0x7f);
        else
            off += 1;
        ABTS_INT_EQUAL(tc, 0x80, buf[off]);
        ABTS_INT_EQUAL(tc, 0x01, buf[off + 1]);
        ABTS_INT_EQUAL(tc, 85, buf[off + 2]);
    }
}

static void cdr_encode_deterministic(abts_case *tc, void *data)
{
    ogs_cdr_pgw_record_t r;
    uint8_t buf1[OGS_CDR_MAX_RECORD_LEN], buf2[OGS_CDR_MAX_RECORD_LEN];
    int len1, len2;

    fill_record(&r);
    len1 = ogs_cdr_encode_pgw_record(&r, buf1, sizeof(buf1));
    len2 = ogs_cdr_encode_pgw_record(&r, buf2, sizeof(buf2));
    ABTS_INT_EQUAL(tc, len1, len2);
    ABTS_TRUE(tc, memcmp(buf1, buf2, (size_t)len1) == 0);
}

static void cdr_encode_optional_fields(abts_case *tc, void *data)
{
    ogs_cdr_pgw_record_t r;
    uint8_t full[OGS_CDR_MAX_RECORD_LEN], min[OGS_CDR_MAX_RECORD_LEN];
    int len_full, len_min;

    fill_record(&r);
    r.msisdn_len = 6;
    r.imeisv_len = 8;
    r.has_serving_plmn = 1;
    r.has_pgw_plmn = 1;
    r.has_ms_timezone = 1;
    r.uli_len = 13;
    r.has_stop_time = 1;
    r.stop_time = TEST_EPOCH + 60;
    r.has_usage_times = 1;
    r.time_of_first_usage = TEST_EPOCH;
    r.time_of_last_usage = TEST_EPOCH + 30;
    r.record_sequence_number = 3;
    len_full = ogs_cdr_encode_pgw_record(&r, full, sizeof(full));
    ABTS_TRUE(tc, len_full > 0);

    fill_record(&r);
    len_min = ogs_cdr_encode_pgw_record(&r, min, sizeof(min));
    ABTS_TRUE(tc, len_min > 0);
    ABTS_TRUE(tc, len_full > len_min);
}

static void cdr_encode_buffer_too_small(abts_case *tc, void *data)
{
    ogs_cdr_pgw_record_t r;
    uint8_t buf[32];

    fill_record(&r);
    ABTS_INT_EQUAL(tc, -1, ogs_cdr_encode_pgw_record(&r, buf, sizeof(buf)));
}

abts_suite *test_cdr_encode(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, cdr_encode_basic, NULL);
    abts_run_test(suite, cdr_encode_deterministic, NULL);
    abts_run_test(suite, cdr_encode_optional_fields, NULL);
    abts_run_test(suite, cdr_encode_buffer_too_small, NULL);

    return suite;
}

static void cdr_file_roundtrip(abts_case *tc, void *data)
{
    static const char *path = "/tmp/ogs-cdr-test.cdr";
    static const uint8_t node_ip[4] = { 10, 99, 0, 10 };
    ogs_cdr_pgw_record_t r;
    uint8_t buf[OGS_CDR_MAX_RECORD_LEN];
    uint8_t hdr[52];
    int len;
    FILE *fp;
    ogs_cdr_file_t *f;
    long fsize;

    unlink(path);

    f = ogs_cdr_file_open(path, 7, node_ip, TEST_EPOCH, TEST_TZ_MIN);
    ABTS_PTR_NOTNULL(tc, f);

    fill_record(&r);
    len = ogs_cdr_encode_pgw_record(&r, buf, sizeof(buf));
    ABTS_TRUE(tc, len > 0);

    ABTS_INT_EQUAL(tc, 0,
            ogs_cdr_file_append(f, buf, (size_t)len, TEST_EPOCH + 60));
    ABTS_INT_EQUAL(tc, 0,
            ogs_cdr_file_append(f, buf, (size_t)len, TEST_EPOCH + 120));
    ABTS_INT_EQUAL(tc, 2, (int)ogs_cdr_file_count(f));
    ABTS_INT_EQUAL(tc, 52 + 2 * (5 + len), (int)ogs_cdr_file_size(f));

    ABTS_INT_EQUAL(tc, 0,
            ogs_cdr_file_close(f, OGS_CDR_FILE_CLOSE_NORMAL));

    fp = fopen(path, "rb");
    ABTS_PTR_NOTNULL(tc, fp);
    ABTS_INT_EQUAL(tc, 52, (int)fread(hdr, 1, sizeof(hdr), fp));

    /* file length */
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    ABTS_INT_EQUAL(tc, (int)fsize, (int)(
            ((uint32_t)hdr[0] << 24) | ((uint32_t)hdr[1] << 16) |
            ((uint32_t)hdr[2] << 8) | hdr[3]));
    /* header length */
    ABTS_INT_EQUAL(tc, 52, (int)hdr[7]);
    /* release/version octets */
    ABTS_INT_EQUAL(tc, 0xe2, hdr[8]);
    ABTS_INT_EQUAL(tc, 0xe2, hdr[9]);
    /* number of CDRs */
    ABTS_INT_EQUAL(tc, 2, (int)hdr[21]);
    /* file sequence number */
    ABTS_INT_EQUAL(tc, 7, (int)hdr[25]);
    /* closure reason */
    ABTS_INT_EQUAL(tc, OGS_CDR_FILE_CLOSE_NORMAL, hdr[26]);
    /* node IPv4 in IPv6-mapped form */
    ABTS_INT_EQUAL(tc, 0xff, hdr[41]);
    ABTS_INT_EQUAL(tc, 0xff, hdr[42]);
    ABTS_INT_EQUAL(tc, 10, hdr[43]);
    ABTS_INT_EQUAL(tc, 99, hdr[44]);
    /* release id extensions */
    ABTS_INT_EQUAL(tc, 8, hdr[50]);
    ABTS_INT_EQUAL(tc, 8, hdr[51]);

    fclose(fp);
    unlink(path);
}

static void cdr_file_empty_is_deleted(abts_case *tc, void *data)
{
    static const char *path = "/tmp/ogs-cdr-empty.cdr";
    static const uint8_t node_ip[4] = { 10, 99, 0, 10 };
    ogs_cdr_file_t *f;

    unlink(path);
    f = ogs_cdr_file_open(path, 1, node_ip, TEST_EPOCH, 0);
    ABTS_PTR_NOTNULL(tc, f);
    ABTS_INT_EQUAL(tc, 0, ogs_cdr_file_close(f, OGS_CDR_FILE_CLOSE_NORMAL));
    ABTS_TRUE(tc, access(path, F_OK) != 0);

    {
        char tmp[600];
        snprintf(tmp, sizeof(tmp), "%s.tmp", path);
        ABTS_TRUE(tc, access(tmp, F_OK) != 0);
    }
}

abts_suite *test_cdr_file(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, cdr_file_roundtrip, NULL);
    abts_run_test(suite, cdr_file_empty_is_deleted, NULL);

    return suite;
}
