/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-sbi.h"
#include "core/abts.h"

static void proto_message_test1(abts_case *tc, void *data)
{
    char *fqdn = NULL;
    ogs_plmn_id_t plmn_id1, plmn_id2;

    ogs_plmn_id_build(&plmn_id1, 456, 123, 3);
    fqdn = ogs_home_network_domain_from_plmn_id(&plmn_id1);
    ABTS_STR_EQUAL(tc, "5gc.mnc123.mcc456.3gppnetwork.org", fqdn);
    ABTS_INT_EQUAL(tc, 456, ogs_plmn_id_mcc_from_fqdn(fqdn));
    ABTS_INT_EQUAL(tc, 123, ogs_plmn_id_mnc_from_fqdn(fqdn));
    ogs_free(fqdn);

    ogs_plmn_id_build(&plmn_id1, 1, 1, 2);
    fqdn = ogs_home_network_domain_from_plmn_id(&plmn_id1);
    ABTS_STR_EQUAL(tc, "5gc.mnc001.mcc001.3gppnetwork.org", fqdn);
    ABTS_INT_EQUAL(tc, 1, ogs_plmn_id_mcc_from_fqdn(fqdn));
    ABTS_INT_EQUAL(tc, 1, ogs_plmn_id_mnc_from_fqdn(fqdn));
    ogs_free(fqdn);

    ogs_plmn_id_build(&plmn_id1, 310, 14, 3);
    fqdn = ogs_home_network_domain_from_plmn_id(&plmn_id1);
    ABTS_STR_EQUAL(tc, "5gc.mnc014.mcc310.3gppnetwork.org", fqdn);
    ABTS_INT_EQUAL(tc, 310, ogs_plmn_id_mcc_from_fqdn(fqdn));
    ABTS_INT_EQUAL(tc, 14, ogs_plmn_id_mnc_from_fqdn(fqdn));
    ogs_free(fqdn);
}

static void proto_message_test2(abts_case *tc, void *data)
{
    char *dnn_oi = NULL;
    char *full_dnn = NULL;
    char dnn_ni[OGS_MAX_DNN_LEN+1];
    ogs_plmn_id_t plmn_id1, plmn_id2;

    ogs_plmn_id_build(&plmn_id1, 456, 123, 3);
    dnn_oi = ogs_dnn_oi_from_plmn_id(&plmn_id1);
    ABTS_STR_EQUAL(tc,
            "mnc123.mcc456.gprs", dnn_oi);
    full_dnn = ogs_msprintf("internet.realm.%s", dnn_oi);
    ABTS_STR_EQUAL(tc,
            "internet.realm.mnc123.mcc456.gprs", full_dnn);
    ABTS_STR_EQUAL(tc,
            dnn_oi, ogs_dnn_oi_from_fqdn(full_dnn));

    ogs_cpystrn(dnn_ni, full_dnn,
            ogs_min(OGS_MAX_DNN_LEN,
                ogs_dnn_oi_from_fqdn(full_dnn) - full_dnn));
    ABTS_STR_EQUAL(tc, "internet.realm", dnn_ni);

    ABTS_INT_EQUAL(tc, 456, ogs_plmn_id_mcc_from_fqdn(full_dnn));
    ABTS_INT_EQUAL(tc, 123, ogs_plmn_id_mnc_from_fqdn(full_dnn));
    ogs_free(dnn_oi);
    ogs_free(full_dnn);
}

typedef struct identity_case_s {
    const char *value;
    const char *type;
    int min_len;
    int max_len;
    bool valid;
} identity_case_t;

#define IMEI_CASE(value, valid) \
    {value, OGS_ID_PEI_TYPE_IMEI, \
        OGS_MAX_IMEI_BCD_LEN, OGS_MAX_IMEI_BCD_LEN, valid}
#define IMEISV_CASE(value, valid) \
    {value, OGS_ID_SUPI_TYPE_IMEISV, \
        OGS_MAX_IMEISV_BCD_LEN, OGS_MAX_IMEISV_BCD_LEN, valid}
#define IMSI_CASE(value, minimum, valid) \
    {value, OGS_ID_SUPI_TYPE_IMSI, minimum, OGS_MAX_IMSI_BCD_LEN, valid}

static const identity_case_t identity_cases[] = {
    IMEI_CASE("imei-490154203237518", true),
    IMEISV_CASE("imeisv-4901542032375186", true),
    IMEI_CASE(NULL, false),
    IMEI_CASE("", false),
    IMEI_CASE("imei", false),
    IMEI_CASE("imei-", false),
    IMEI_CASE("imei-49015420323751", false),
    IMEI_CASE("imei-4901542032375186", false),
    IMEISV_CASE("imeisv-490154203237518", false),
    IMEISV_CASE("imeisv-49015420323751860", false),
    IMEI_CASE("imei-49015420323751x", false),
    IMEISV_CASE("imeisv-490154203237518x", false),
    IMEI_CASE("imsi-490154203237518", false),
    IMEI_CASE("IMEI-490154203237518", false),
    IMEI_CASE("imei-490154203237518-1", false),
    IMEI_CASE(" imei-490154203237518", false),
    IMEI_CASE("imei-490154203237518 ", false),
    IMEI_CASE("imei-+90154203237518", false),
    IMSI_CASE("imsi-00101", 6, false),
    IMSI_CASE("imsi-001010", 6, true),
    IMSI_CASE("imsi-001010123456789", 6, true),
    IMSI_CASE(NULL, 6, false),
    IMSI_CASE("", 6, false),
    IMSI_CASE("imsi", 6, false),
    IMSI_CASE("imsi-", 6, false),
    IMSI_CASE("imsi-0010", 6, false),
    IMSI_CASE("imsi-0010101234567890", 6, false),
    IMSI_CASE("imsi-00101012345678x", 6, false),
    IMSI_CASE("nsi-001010123456789", 6, false),
    IMSI_CASE("IMSI-001010123456789", 6, false),
    IMSI_CASE("imsi-001010123456789-1", 6, false),
    IMSI_CASE(" imsi-001010123456789", 6, false),
    IMSI_CASE("imsi-001010123456789 ", 6, false),
    /* Bounds belong to the caller, not a universal SUPI validation rule. */
    IMSI_CASE("imsi-00101", 5, true),
    IMSI_CASE("imsi-1", 1, true),
    IMSI_CASE("imsi-\xff", 1, false),
    {"custom-001", "custom", 1, 3, true},
    {"custom-0001", "custom", 1, 3, false},
};

static void identity_format(abts_case *tc, void *data)
{
    const identity_case_t *test = data;

    ABTS_ASSERT(tc, test->value ? test->value : "NULL identity",
            ogs_id_bcd_is_valid(test->value, test->type,
                test->min_len, test->max_len) == test->valid);
}

static void legacy_identity_bounds(abts_case *tc, void *data)
{
    /* Keep the existing BCD APIs' nonempty, maximum-length-only contract. */
    ABTS_TRUE(tc, ogs_imsi_bcd_is_valid("1"));
    ABTS_TRUE(tc, ogs_imsi_bcd_is_valid("001010123456789"));
    ABTS_TRUE(tc, ogs_imeisv_bcd_is_valid("1"));
    ABTS_TRUE(tc, ogs_imeisv_bcd_is_valid("4901542032375186"));

    /* All numeric APIs reject invalid input and log a warning. */
    ABTS_TRUE(tc, ogs_bcd_string_is_valid("001", 3));
    ABTS_TRUE(tc, !ogs_bcd_string_is_valid("", 3));
    ABTS_TRUE(tc, !ogs_bcd_string_is_valid("0001", 3));
    ABTS_TRUE(tc, !ogs_bcd_string_is_valid("+01", 3));
    ABTS_TRUE(tc, !ogs_bcd_string_is_valid("1 ", 3));
    ABTS_TRUE(tc, !ogs_bcd_string_is_valid("\xff", 3));
    ABTS_TRUE(tc, !ogs_imsi_bcd_is_valid(""));
    ABTS_TRUE(tc, !ogs_imsi_bcd_is_valid("0010101234567890"));
    ABTS_TRUE(tc, !ogs_imsi_bcd_is_valid("00101012345678x"));
    ABTS_TRUE(tc, !ogs_imeisv_bcd_is_valid(""));
    ABTS_TRUE(tc, !ogs_imeisv_bcd_is_valid("49015420323751860"));
    ABTS_TRUE(tc, !ogs_imeisv_bcd_is_valid("490154203237518x"));
}

static void framed_route_format(abts_case *tc, void *data)
{
    static const struct {
        const char *prefix;
        const char *wire;
    } cases[] = {
        { "192.168.100.0/24", "192.168.100.0/24 0.0.0.0 1" },
        { "2001:db8:100::/64", "2001:db8:100::/64 :: 1" },
        { "192.0.2.1/32", "192.0.2.1/32 0.0.0.0 1" },
        { "2001:db8::1/128", "2001:db8::1/128 :: 1" },
    };
    size_t i;
    char *s = NULL;

    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++) {
        s = ogs_framed_route_build(cases[i].prefix);
        ABTS_STR_EQUAL(tc, cases[i].wire, s);
        ogs_free(s);

        s = ogs_framed_route_parse(cases[i].wire, strlen(cases[i].wire));
        ABTS_STR_EQUAL(tc, cases[i].prefix, s);
        ogs_free(s);

        /* Bare prefixes from older Open5GS peers remain accepted. */
        s = ogs_framed_route_parse(cases[i].prefix, strlen(cases[i].prefix));
        ABTS_STR_EQUAL(tc, cases[i].prefix, s);
        ogs_free(s);
    }

    /* Gateway and all metrics are ignored by the prefix parser. */
    s = ogs_framed_route_parse("192.0.2.0/24 192.0.2.1 1 2 -1 3 400",
            strlen("192.0.2.0/24 192.0.2.1 1 2 -1 3 400"));
    ABTS_STR_EQUAL(tc, "192.0.2.0/24", s);
    ogs_free(s);
}

static void framed_route_classful(abts_case *tc, void *data)
{
    static const struct {
        const char *value;
        const char *prefix;
    } cases[] = {
        { "10.0.0.0 0.0.0.0 1", "10.0.0.0/8" },
        { "172.16.0.0 0.0.0.0 1", "172.16.0.0/16" },
        { "192.0.2.0 0.0.0.0 1", "192.0.2.0/24" },
        { "0.0.0.0", "0.0.0.0/8" },
        { "127.255.255.255", "127.255.255.255/8" },
        { "128.0.0.0", "128.0.0.0/16" },
        { "191.255.255.255", "191.255.255.255/16" },
        { "192.0.0.0", "192.0.0.0/24" },
        { "223.255.255.255", "223.255.255.255/24" },
        /* An omitted IPv6 length is left for the caller to interpret. */
        { "2001:db8::", "2001:db8::" },
        { "2001:db8:: :: 1", "2001:db8::" },
        /* Explicit lengths are preserved, not replaced by a default.
         * These check string conversion, not UPF subnet installation. */
        { "0.0.0.0/0 0.0.0.0 1", "0.0.0.0/0" },
        { "::/0 :: 1", "::/0" },
    };
    size_t i;
    char *s = NULL;

    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++) {
        s = ogs_framed_route_parse(cases[i].value, strlen(cases[i].value));
        ABTS_STR_EQUAL(tc, cases[i].prefix, s);
        ogs_free(s);
    }
}

static void framed_route_invalid(abts_case *tc, void *data)
{
    static const char *values[] = {
        "", " 0.0.0.0 1", "   ", "not-an-ip", "10", "10.0.0",
        "256.0.0.0", "-1.0.0.0", "+1.0.0.0", "1.2.3.4junk",
        "999999999999999999999999.0.0.0",
        "224.0.0.1", "239.255.255.255", "240.0.0.0", "255.255.255.255",
        "224.0.0.1 0.0.0.0 1",
    };
    const char embedded_nul[] = "192.0.2.0\0junk 0.0.0.0 1";
    size_t i;

    for (i = 0; i < OGS_ARRAY_SIZE(values); i++)
        ABTS_PTR_EQUAL(tc, NULL,
                ogs_framed_route_parse(values[i], strlen(values[i])));

    ABTS_PTR_EQUAL(tc, NULL, ogs_framed_route_parse("", -1));
    ABTS_PTR_EQUAL(tc, NULL, ogs_framed_route_parse("192.0.2.0", 0));
    ABTS_PTR_EQUAL(tc, NULL,
            ogs_framed_route_parse(embedded_nul, sizeof(embedded_nul) - 1));
}

static void framed_route_bounds(abts_case *tc, void *data)
{
    static const struct {
        const char *value;
        const char *prefix;
    } cases[] = {
        { "192.0.2.0/24", "192.0.2.0/24" },
        { "2001:db8::/64", "2001:db8::/64" },
        { "192.0.2.0/24 0.0.0.0 1", "192.0.2.0/24" },
        { "2001:db8::/64 :: 1", "2001:db8::/64" },
        { "10.0.0.0", "10.0.0.0/8" },
        { "2001:db8::", "2001:db8::" },
    };
    size_t i, length;
    char *value = NULL;
    char *s = NULL;

    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++) {
        length = strlen(cases[i].value);
        /* Allocate exactly length bytes: there is no trailing NUL. */
        value = ogs_malloc(length);
        ogs_assert(value);
        memcpy(value, cases[i].value, length);

        s = ogs_framed_route_parse(value, length);
        ABTS_STR_EQUAL(tc, cases[i].prefix, s);
        ogs_free(s);
        ogs_free(value);
    }

    /* Bytes beyond the supplied length must not become part of a prefix. */
    s = ogs_framed_route_parse("192.0.2.0/24junk", strlen("192.0.2.0/24"));
    ABTS_STR_EQUAL(tc, "192.0.2.0/24", s);
    ogs_free(s);

    s = ogs_framed_route_parse("10.0.0.0junk", strlen("10.0.0.0"));
    ABTS_STR_EQUAL(tc, "10.0.0.0/8", s);
    ogs_free(s);
}

abts_suite *test_proto_message(abts_suite *suite)
{
    size_t i;

    suite = ADD_SUITE(suite)

    abts_run_test(suite, proto_message_test1, NULL);
    abts_run_test(suite, proto_message_test2, NULL);
    for (i = 0; i < OGS_ARRAY_SIZE(identity_cases); i++)
        abts_run_test(suite, identity_format, (void *)&identity_cases[i]);
    abts_run_test(suite, legacy_identity_bounds, NULL);
    abts_run_test(suite, framed_route_format, NULL);
    abts_run_test(suite, framed_route_classful, NULL);
    abts_run_test(suite, framed_route_invalid, NULL);
    abts_run_test(suite, framed_route_bounds, NULL);

    return suite;
}
