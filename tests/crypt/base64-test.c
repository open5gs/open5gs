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

abts_suite *test_base64(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, base64_test1, NULL);

    return suite;
}
