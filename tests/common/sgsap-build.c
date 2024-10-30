/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "test-common.h"

#define TEST_SGSAP_MAX_MESSAGE 64

ogs_pkbuf_t *test_sgsap_location_update_accept(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SGSAP_MAX_MESSAGE] = {
        "0a01089999073746 000006040509f107 09260e05f49ee88e 64",
        "0a01089999073746 000006040509f107 09260e05f49ee88e 65",
        "0a01089999073746 000006040509f107 09260e05f49ee88e 66",

        "0a01087942120000 000030040527f412 c9580e05f437ab9c c5",
        "",
        "",

    };
    uint16_t len[TEST_SGSAP_MAX_MESSAGE] = {
        25,
        25,
        25,

        25,
        0,
        0,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_sgsap_location_update_reject(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SGSAP_MAX_MESSAGE] = {
        "0b01089999073746 0000060f01030405 09f1070926",
        "",
        "",

    };
    uint16_t len[TEST_SGSAP_MAX_MESSAGE] = {
        21,
        0,
        0,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_sgsap_imsi_detach_ack(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SGSAP_MAX_MESSAGE] = {
        "1401089999073746 000006",
        "",
        "",

    };
    uint16_t len[TEST_SGSAP_MAX_MESSAGE] = {
        11,
        0,
        0,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_sgsap_paging_request(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SGSAP_MAX_MESSAGE] = {
        "0101089999073746 000006021003766c 72076578616d706c 65036e6574200101"
        "040509f1070926",
        "0101089999073746 000005021003766c 72076578616d706c 65036e6574200101"
        "040509f1070926",  /* Paging-Reject */
        "0101089999073746 000006021003766c 72076578616d706c 65036e6574200102"
        "040509f1070926",  /* Paging-Request with SMS */

        /* Paging-Request for crash-test */
        "0101089999073746 000006021003766c 72076578616d706c 65036e6574200101"
        "040527f412c958",
        "",
        "",
    };
    uint16_t len[TEST_SGSAP_MAX_MESSAGE] = {
        39,
        39,
        39,

        39,
        0,
        0,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_sgsap_reset_indication(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SGSAP_MAX_MESSAGE] = {
        "15021003766c7207 6578616d706c6503 6e6574",
        "",
        "",

    };
    uint16_t len[TEST_SGSAP_MAX_MESSAGE] = {
        19,
        0,
        0,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_sgsap_release_request(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SGSAP_MAX_MESSAGE] = {
        "1b01089999073746 000006",
        "",
        "",

    };
    uint16_t len[TEST_SGSAP_MAX_MESSAGE] = {
        11,
        0,
        0,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_sgsap_downlink_unitdata(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SGSAP_MAX_MESSAGE] = {
        "0701089999073746 0000061626090123 0100079144775810 0650001700048032"
        "2400009160404044 150009c8329bfd06 4d9b53",

    };
    uint16_t len[TEST_SGSAP_MAX_MESSAGE] = {
        51,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_sgsap_mm_information_request(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SGSAP_MAX_MESSAGE] = {
        "1a01087942120000 0000301714430483 d46413450483d464 1347917071028401"
        "29",

    };
    uint16_t len[TEST_SGSAP_MAX_MESSAGE] = {
        33,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}
