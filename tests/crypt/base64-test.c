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
        b64_enc_len = ogs_base64_encoded_size(orig_len);
        ABTS_ASSERT(tc, "base 64 exp. length", (enc_len == (b64_enc_len - 1)));

        enc = ogs_malloc(b64_enc_len);

        b64_len = ogs_base64_encode_from_buffer(
                enc, b64_enc_len,
                (const uint8_t *)base64_tbl[i].orig, orig_len);

        ABTS_ASSERT(tc, "base 64 encoded length", (b64_enc_len == b64_len));
        ABTS_ASSERT(tc, "base 64 encoded matches expected output",
                    (memcmp(enc, base64_tbl[i].enc, b64_enc_len) == 0));

        ogs_free(enc);
    }
}

#define PEM_CASE(description, input, output) \
    { description, input, sizeof(input) - 1, output, sizeof(output) - 1 }
#define PEM_ERROR_CASE(description, input) \
    { description, input, sizeof(input) - 1, NULL, 0 }

static void base64_test2(abts_case *tc, void *data)
{
    static const char label[] = "TEST KEY";
    static const char expected[] = "Hello";
    static const struct {
        const char *description;
        const char *input;
        size_t size;
        const char *output;
        size_t output_size;
    } cases[] = {
        PEM_CASE("matching labels",
            "-----BEGIN TEST KEY-----\nSGVsbG8=\n-----END TEST KEY-----\n",
            expected),
        PEM_CASE("binary payload with two padding characters",
            "-----BEGIN TEST KEY-----\n"
            "AAECAwQFBgcICQoLDA0ODw==\n-----END TEST KEY-----\n",
            "\x00\x01\x02\x03\x04\x05\x06\x07"
            "\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"),
        PEM_CASE("different END label, whitespace and CRLF",
            "-----BEGIN TEST KEY----- \t\r\n"
            "\r\n SGVs\t bG8= \r\n \t-----END OTHER KEY-----", expected),
        PEM_CASE("skip a block whose label has the same prefix",
            "-----BEGIN TEST KEY EXTRA-----\nAA==\n"
            "-----END TEST KEY EXTRA-----\n"
            "-----BEGIN TEST KEY-----\nSGVsbG8=\n-----END TEST KEY-----\n",
            expected),
        PEM_ERROR_CASE("missing requested BEGIN label",
            "-----BEGIN TEST KEY EXTRA-----\nSGVsbG8=\n"
            "-----END TEST KEY EXTRA-----\n"),
        PEM_ERROR_CASE("missing END line",
            "-----BEGIN TEST KEY-----\nSGVsbG8=\n"),
        PEM_ERROR_CASE("END marker on the body line",
            "-----BEGIN TEST KEY-----\nSGVsbG8=-----END TEST KEY-----\n"),
        PEM_ERROR_CASE("embedded NUL after a valid Base64 prefix",
            "-----BEGIN TEST KEY-----\nSGVs\0bG8=\n-----END TEST KEY-----\n"),
        PEM_ERROR_CASE("invalid Base64 character",
            "-----BEGIN TEST KEY-----\nSGVsbG8-\n-----END TEST KEY-----\n"),
    };
    size_t i;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        ogs_datum_t result = { 0 };
        ogs_log_level_e saved_level = ogs_log_get_domain_level(OGS_LOG_DOMAIN);
        int rv, expected_rv = cases[i].output ? OGS_OK : OGS_ERROR;

        if (expected_rv == OGS_ERROR)
            ogs_log_set_domain_level(OGS_LOG_DOMAIN, OGS_LOG_FATAL);

        rv = ogs_fbase64_decode(label, (const uint8_t *)cases[i].input,
                cases[i].size, &result);

        if (expected_rv == OGS_ERROR)
            ogs_log_set_domain_level(OGS_LOG_DOMAIN, saved_level);

        ABTS_ASSERT(tc, cases[i].description, rv == expected_rv);
        if (expected_rv == OGS_OK) {
            ABTS_INT_EQUAL(tc, cases[i].output_size, result.size);
            ABTS_PTR_NOTNULL(tc, result.data);
            if (result.data && result.size == cases[i].output_size)
                ABTS_TRUE(tc,
                    memcmp(result.data, cases[i].output, result.size) == 0);
        } else {
            ABTS_INT_EQUAL(tc, 0, result.size);
            ABTS_PTR_EQUAL(tc, NULL, result.data);
        }

        if (result.data)
            ogs_free(result.data);
    }
}

#undef PEM_CASE
#undef PEM_ERROR_CASE

abts_suite *test_base64(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, base64_test1, NULL);
    abts_run_test(suite, base64_test2, NULL);

    return suite;
}
