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

#include "ogs-core.h"
#include "core/abts.h"

static void conv_test2(abts_case *tc, void *data)
{
#define K "4   6  5B5    CE8   B199B49FAA5F0A2EE238A6BC   "
#define OP "5F1D289C  5D354D0A 140C2548   F5F3E3BA"
#define OPc "E8ED2 89D EBA9 52E4 283B 54E8 8E61 83CA"
#define AMF "8000"
    uint8_t k[16] = {
        0x46, 0x5B, 0x5C, 0xE8, 0xB1, 0x99, 0xB4, 0x9F,
        0xAA, 0x5F, 0x0A, 0x2E, 0xE2, 0x38, 0xA6, 0xBC };
    uint8_t op[16] = {
        0x5F, 0x1D, 0x28, 0x9C, 0x5D, 0x35, 0x4D, 0x0A,
        0x14, 0x0C, 0x25, 0x48, 0xF5, 0xF3, 0xE3, 0xBA };
    uint8_t opc[16] = {
        0xE8, 0xED, 0x28, 0x9D, 0xEB, 0xA9, 0x52, 0xE4,
        0x28, 0x3B, 0x54, 0xE8, 0x8E, 0x61, 0x83, 0xCA };
    uint8_t amf[2] = { 0x80, 0x00 };
#define LOWER "    12abcdE   F"
    uint8_t lower[4] = { 0x12, 0xab, 0xcd, 0xef };

    char buffer[16];

    ABTS_TRUE(tc, memcmp(k,
                ogs_hex_from_string(K, buffer, sizeof(buffer)), 16) == 0);

    ABTS_TRUE(tc, memcmp(op,
                ogs_hex_from_string(OP, buffer, sizeof(buffer)), 16) == 0);

    ABTS_TRUE(tc, memcmp(opc,
                ogs_hex_from_string(OPc, buffer, sizeof(buffer)), 16) == 0);

    ABTS_TRUE(tc, memcmp(amf,
                ogs_hex_from_string(AMF, buffer, sizeof(buffer)), 2) == 0);

    ABTS_TRUE(tc, memcmp(lower, 
                ogs_hex_from_string(LOWER, buffer, sizeof(buffer)), 4) == 0);
}

static void conv_test3(abts_case *tc, void *data)
{
    uint8_t k[16] = {
        0x46, 0x5B, 0x5C, 0xE8, 0xB1, 0x99, 0xB4, 0x9F,
        0xAA, 0x5F, 0x0A, 0x2E, 0xE2, 0x38, 0xA6, 0xBC };
    uint8_t op[16] = {
        0x5F, 0x1D, 0x28, 0x9C, 0x5D, 0x35, 0x4D, 0x0A,
        0x14, 0x0C, 0x25, 0x48, 0xF5, 0xF3, 0xE3, 0xBA };
    uint8_t opc[16] = {
        0xE8, 0xED, 0x28, 0x9D, 0xEB, 0xA9, 0x52, 0xE4,
        0x28, 0x3B, 0x54, 0xE8, 0x8E, 0x61, 0x83, 0xCA };
    uint8_t amf[2] = { 0x80, 0x00 };
    uint8_t lower[4] = { 0x12, 0xab, 0xcd, 0xef };
    uint8_t buffer[128];

    ABTS_TRUE(tc, strcmp("465b5ce8b199b49faa5f0a2ee238a6bc",
        ogs_hex_to_ascii(k, sizeof(k), buffer, sizeof(buffer))) == 0);
    ABTS_TRUE(tc, strcmp("5f1d289c5d354d0a140c2548f5f3e3ba",
        ogs_hex_to_ascii(op, sizeof(op), buffer, sizeof(buffer))) == 0);
    ABTS_TRUE(tc, strcmp("e8ed289deba952e4283b54e88e6183ca",
        ogs_hex_to_ascii(opc, sizeof(opc), buffer, sizeof(buffer))) == 0);
    ABTS_TRUE(tc, strcmp("8000",
        ogs_hex_to_ascii(amf, sizeof(amf), buffer, sizeof(buffer))) == 0);
    ABTS_TRUE(tc, strcmp("12abcdef",
        ogs_hex_to_ascii(lower, sizeof(lower), buffer, sizeof(buffer))) == 0);
}

static void conv_test4(abts_case *tc, void *data)
{
#define MAX_SIZE 8
    uint8_t tmp[MAX_SIZE] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };
    uint8_t buf[MAX_SIZE];
    uint64_t num;

    num = 0x0123456789abcdef;
    ABTS_TRUE(tc, memcmp(tmp, ogs_uint64_to_buffer(num, 8, buf), 8) == 0);
    num = 0x0123456789abcd;
    ABTS_TRUE(tc, memcmp(tmp, ogs_uint64_to_buffer(num, 7, buf), 7) == 0);
    num = 0x0123456789ab;
    ABTS_TRUE(tc, memcmp(tmp, ogs_uint64_to_buffer(num, 6, buf), 6) == 0);
    num = 0x0123456789;
    ABTS_TRUE(tc, memcmp(tmp, ogs_uint64_to_buffer(num, 5, buf), 5) == 0);
    num = 0x01234567;
    ABTS_TRUE(tc, memcmp(tmp, ogs_uint64_to_buffer(num, 4, buf), 4) == 0);
    num = 0x012345;
    ABTS_TRUE(tc, memcmp(tmp, ogs_uint64_to_buffer(num, 3, buf), 3) == 0);
    num = 0x0123;
    ABTS_TRUE(tc, memcmp(tmp, ogs_uint64_to_buffer(num, 2, buf), 2) == 0);
    num = 0x01;
    ABTS_TRUE(tc, memcmp(tmp, ogs_uint64_to_buffer(num, 1, buf), 1) == 0);
}

static void conv_test5(abts_case *tc, void *data)
{
    ABTS_TRUE(tc, 0x0123456789abcdef ==
            ogs_buffer_to_uint64((void*)"\x01\x23\x45\x67\x89\xab\xcd\xef", 8));
    ABTS_TRUE(tc, 0x0123456789abcd ==
            ogs_buffer_to_uint64((void*)"\x01\x23\x45\x67\x89\xab\xcd", 7));
    ABTS_TRUE(tc, 0x0123456789ab ==
            ogs_buffer_to_uint64((void*)"\x01\x23\x45\x67\x89\xab", 6));
    ABTS_TRUE(tc, 0x0123456789 ==
            ogs_buffer_to_uint64((void*)"\x01\x23\x45\x67\x89", 5));
    ABTS_TRUE(tc, 0x01234567 ==
            ogs_buffer_to_uint64((void*)"\x01\x23\x45\x67", 4));
    ABTS_TRUE(tc, 0x012345 ==
            ogs_buffer_to_uint64((void*)"\x01\x23\x45", 3));
    ABTS_TRUE(tc, 0x0123 ==
            ogs_buffer_to_uint64((void*)"\x01\x23", 2));
    ABTS_TRUE(tc, 0x01 ==
            ogs_buffer_to_uint64((void*)"\x01", 1));
}

static void conv_test6(abts_case *tc, void *data)
{
    char out[16];
    int out_len;
#define MSISDN "491725670014"
    ogs_bcd_to_buffer(MSISDN, out, &out_len);
    ABTS_TRUE(tc, memcmp("\x94\x71\x52\x76\x00\x41", out, out_len) == 0);
#define MEI "3516020019874800"
    ogs_bcd_to_buffer(MEI, out, &out_len);
    ABTS_TRUE(tc, 
            memcmp("\x53\x61\x20\x00\x91\x78\x84\x00", out, out_len) == 0);
#define IMSI "001010123456819"
    ogs_bcd_to_buffer(IMSI, out, &out_len);
    ABTS_TRUE(tc, 
            memcmp("\x00\x01\x01\x21\x43\x65\x18\xf9", out, out_len) == 0);
}

static void conv_test7(abts_case *tc, void *data)
{
    char out[32];
    uint8_t buf1[6] = { 0x94, 0x71, 0x52, 0x76, 0x00, 0x41 };
    int buf1_len = 6;
    uint8_t buf2[8] = { 0x53, 0x61, 0x20, 0x00, 0x91, 0x78, 0x84, 0x00 };
    int buf2_len = 8;
    uint8_t buf3[8] = { 0x00, 0x01, 0x01, 0x21, 0x43, 0x65, 0x18, 0xf9 };
    int buf3_len = 8;

    ogs_buffer_to_bcd(buf1, buf1_len, out);
    ABTS_TRUE(tc, strcmp("491725670014", out) == 0);
    ogs_buffer_to_bcd(buf2, buf2_len, out);
    ABTS_TRUE(tc, strcmp("3516020019874800", out) == 0);
    ogs_buffer_to_bcd(buf3, buf3_len, out);
    ABTS_TRUE(tc, strcmp("001010123456819", out) == 0);
}

static void conv_test8(abts_case *tc, void *data)
{
    char *str = NULL;
    uint64_t x;

    str = ogs_uint64_to_0string(0);
    ABTS_STR_EQUAL(tc, "0000000000000000", str);
    ogs_free(str);

    str = ogs_uint64_to_0string(1);
    ABTS_STR_EQUAL(tc, "0000000000000001", str);
    ogs_free(str);

    x = 0;
    str = ogs_uint64_to_string(0);
    ABTS_STR_EQUAL(tc, "", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal("0"));
    ogs_free(str);

    x = 1;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "1", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x12;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "12", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x1234;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "1234", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x12345;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "12345", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123456;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123456", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x1234567;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "1234567", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x12345678;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "12345678", str);
    ABTS_INT_EQUAL(tc, x, ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123456789;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123456789", str);
    ABTS_TRUE(tc, x == ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123456789a;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123456789a", str);
    ABTS_TRUE(tc, x == ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123456789ab;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123456789ab", str);
    ABTS_TRUE(tc, x == ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123456789abc;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123456789abc", str);
    ABTS_TRUE(tc, x == ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123456789abcd;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123456789abcd", str);
    ABTS_TRUE(tc, x == ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123456789abcde;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123456789abcde", str);
    ABTS_TRUE(tc, x == ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x123456789abcdef;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "123456789abcdef", str);
    ABTS_TRUE(tc, x == ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);

    x = 0x120456789abcdef0;
    str = ogs_uint64_to_string(x);
    ABTS_STR_EQUAL(tc, "120456789abcdef0", str);
    ABTS_TRUE(tc, x == ogs_uint64_from_string_hexadecimal(str));
    ogs_free(str);
}

abts_suite *test_conv(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, conv_test2, NULL);
    abts_run_test(suite, conv_test3, NULL);
    abts_run_test(suite, conv_test4, NULL);
    abts_run_test(suite, conv_test5, NULL);
    abts_run_test(suite, conv_test6, NULL);
    abts_run_test(suite, conv_test7, NULL);
    abts_run_test(suite, conv_test8, NULL);

    return suite;
}
