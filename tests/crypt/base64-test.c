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

#include "ogs-crypt.h"
#include "core/abts.h"

static struct {
   const char *orig;
   const char *enc;
} base64_tbl[] =
{
    {"", ""},
    {"H", "SA=="},
    {"He", "SGU="},
    {"Hel", "SGVs"},
    {"Hell", "SGVsbA=="},
    {"Hello", "SGVsbG8="},
    {"Hello World", "SGVsbG8gV29ybGQ="},
    {"\xff\xff\xff\xff", "/////w=="},
};
static int num_base64 = sizeof(base64_tbl) / sizeof(base64_tbl[0]);

static void base64_test1(abts_case *tc, void *data)
{
    int i;

    for (i = 0; i < num_base64; i++) {
        char *enc;
        int orig_len, enc_len, b64_len, b64_enc_len;

        orig_len = strlen(base64_tbl[i].orig);
        enc_len = strlen(base64_tbl[i].enc);

        /* includes + 1 for term null */
        b64_enc_len = ogs_base64_encode_len(orig_len);
        ABTS_ASSERT(tc, "base 64 exp. length", (enc_len == (b64_enc_len - 1)));

        enc = ogs_malloc(b64_enc_len);

        b64_len = ogs_base64_encode(enc, base64_tbl[i].orig, orig_len);

        ABTS_ASSERT(tc, "base 64 encoded length", (b64_enc_len == b64_len));
        ABTS_ASSERT(tc, "base 64 encoded matches expected output",
                    (memcmp(enc, base64_tbl[i].enc, b64_enc_len) == 0));

        ogs_free(enc);
    }
}

struct decode_tests_st {
    const char *name;
    uint8_t *raw;
    unsigned raw_size;
    const char *pem;
    unsigned pem_size;
    int res;
};

struct decode_tests_st decode_tests[] = {
    {
        .name = "dec-rnd1",
        .pem = "-----BEGIN dec-rnd1-----\n"
            "9ppGioRpeiiD2lLNYC85eA==\n"
            "-----END rnd1-----\n",
        .raw = "\xf6\x9a\x46\x8a\x84\x69\x7a\x28\x83\xda\x52\xcd\x60\x2f\x39\x78",
        .raw_size = 16,
        .res = OGS_OK
    },
    {
        .name = "dec-rnd2",
        .pem = "-----BEGIN dec-rnd2-----\n"
            "LJ/7hUZ3TtPIz2dlc5+YvELe+Q==\n"
            "-----END rnd2-----\n",
        .raw = "\x2c\x9f\xfb\x85\x46\x77\x4e\xd3\xc8\xcf\x67\x65\x73\x9f\x98\xbc\x42\xde\xf9",
        .raw_size = 19,
        .res = OGS_OK
    },
    {
        .name = "dec-extra-chars",
        .pem = "-----BEGIN dec-extra-chars-----   \n\n"
            "\n\n  LJ/7hUZ3TtPIz2dlc5+YvELe+Q==  \n"
            "   -----END rnd2-----  \n  ",
        .raw = "\x2c\x9f\xfb\x85\x46\x77\x4e\xd3\xc8\xcf\x67\x65\x73\x9f\x98\xbc\x42\xde\xf9",
        .raw_size = 19,
        .res = OGS_OK
    },
#if 0
    {
        .name = "dec-invalid-header",
        .pem = "-----BEGIN dec-xxx-----\n"
            "LJ/7hUZ3TtPIz2dlc5+YvELe+Q==\n"
            "-----END rnd2-----\n",
        .raw = "\x2c\x9f\xfb\x85\x46\x77\x4e\xd3\xc8\xcf\x67\x65\x73\x9f\x98\xbc\x42\xde\xf9",
        .raw_size = 19,
        .res = OGS_ERROR
    },
    {
        .name = "leak1",
        .pem = "-----BEGIN leak1-----E-\x00\x00-----END ",
        .pem_size = 34,
        .raw = "",
        .raw_size = 0,
        .res = OGS_ERROR
    },
#endif
};

static void base64_test2(abts_case *tc, void *data)
{
    int i, rv;
    ogs_datum_t result;

    for (i = 0; i < sizeof(decode_tests)/sizeof(decode_tests[0]); i++) {
        if (!decode_tests[i].pem_size)
            decode_tests[i].pem_size = strlen(decode_tests[i].pem);

        rv = ogs_fbase64_decode(
                decode_tests[i].name,
                (const uint8_t *)decode_tests[i].pem, decode_tests[i].pem_size,
                &result);

        ABTS_INT_EQUAL(tc, decode_tests[i].res, rv);

        if (result.size) {
            ABTS_INT_EQUAL(tc, decode_tests[i].raw_size, result.size);
            ABTS_TRUE(tc,
                memcmp(decode_tests[i].raw, result.data, result.size) == 0);
        }

        if (result.data)
            ogs_free(result.data);
    }
}

abts_suite *test_base64(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, base64_test1, NULL);
    abts_run_test(suite, base64_test2, NULL);

    return suite;
}
