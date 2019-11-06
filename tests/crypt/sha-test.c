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

static void sha1_test1(abts_case *tc, void *data)
{
    uint8_t msg1[] = "abc";
    uint8_t msg2[] = "abcdbcdecdefdefgefghfghighij"
                       "hijkijkljklmklmnlmnomnopnopq";

    uint8_t digest1[] = {
        0xa9,0x99,0x3e,0x36,0x47,0x06,0x81,0x6a,
        0xba,0x3e,0x25,0x71,0x78,0x50,0xc2,0x6c,
        0x9c,0xd0,0xd8,0x9d
    };
    uint8_t digest2[] = {
        0x84,0x98,0x3e,0x44,0x1c,0x3b,0xd2,0x6e,
        0xba,0xae,0x4a,0xa1,0xf9,0x51,0x29,0xe5,
        0xe5,0x46,0x70,0xf1
    };
    uint8_t digest3[] = {
        0x34,0xaa,0x97,0x3c,0xd4,0xc4,0xda,0xa4,
        0xf6,0x1e,0xeb,0x2b,0xdb,0xad,0x27,0x31,
        0x65,0x34,0x01,0x6f,
    };

    uint8_t digest[20];
    int i;

    ogs_sha1_ctx ctx;

    ogs_sha1_init(&ctx);
    ogs_sha1_update(&ctx, msg1, sizeof(msg1) - 1);
    ogs_sha1_final(&ctx, digest);
    ABTS_INT_EQUAL(tc, 0, memcmp(digest1, digest, 20));

    ogs_sha1_init(&ctx);
    ogs_sha1_update(&ctx, msg2, sizeof(msg2) - 1);
    ogs_sha1_final(&ctx, digest);
    ABTS_INT_EQUAL(tc, 0, memcmp(digest2, digest, 20));

    ogs_sha1_init(&ctx);
    for (i = 0; i < 1000000; i++)
        ogs_sha1_update(&ctx, (uint8_t*)"a", 1);
    ogs_sha1_final(&ctx, digest);
    ABTS_INT_EQUAL(tc, 0, memcmp(digest3, digest, 20));

}

static void sha2_test1(abts_case *tc, void *data)
{
    const char *vectors[4][3] =
    {   /* SHA-224 */
        {
        "\x23\x09\x7d\x22\x34\x05\xd8\x22\x86\x42\xa4\x77\xbd\xa2\x55\xb3\x2a\xad\xbc\xe4\xbd\xa0\xb3\xf7\xe3\x6c\x9d\xa7",
        "\x75\x38\x8b\x16\x51\x27\x76\xcc\x5d\xba\x5d\xa1\xfd\x89\x01\x50\xb0\xc6\x45\x5c\xb4\xf5\x8b\x19\x52\x52\x25\x25",
        "\x20\x79\x46\x55\x98\x0c\x91\xd8\xbb\xb4\xc1\xea\x97\x61\x8a\x4b\xf0\x3f\x42\x58\x19\x48\xb2\xee\x4e\xe7\xad\x67",
        },
        /* SHA-\x25\x6 */
        {
        "\xba\x78\x16\xbf\x8f\x01\xcf\xea\x41\x41\x40\xde\x5d\xae\x22\x23\xb0\x03\x61\xa3\x96\x17\x7a\x9c\xb4\x10\xff\x61\xf2\x00\x15\xad",
        "\x24\x8d\x6a\x61\xd2\x06\x38\xb8\xe5\xc0\x26\x93\x0c\x3e\x60\x39\xa3\x3c\xe4\x59\x64\xff\x21\x67\xf6\xec\xed\xd4\x19\xdb\x06\xc1",
        "\xcd\xc7\x6e\x5c\x99\x14\xfb\x92\x81\xa1\xc7\xe2\x84\xd7\x3e\x67\xf1\x80\x9a\x48\xa4\x97\x20\x0e\x04\x6d\x39\xcc\xc7\x11\x2c\xd0",
        },
        /* SHA-\x38\x4 */
        {
        "\xcb\x00\x75\x3f\x45\xa3\x5e\x8b\xb5\xa0\x3d\x69\x9a\xc6\x50\x07\x27\x2c\x32\xab\x0e\xde\xd1\x63\x1a\x8b\x60\x5a\x43\xff\x5b\xed"
        "\x80\x86\x07\x2b\xa1\xe7\xcc\x23\x58\xba\xec\xa1\x34\xc8\x25\xa7",
        "\x09\x33\x0c\x33\xf7\x11\x47\xe8\x3d\x19\x2f\xc7\x82\xcd\x1b\x47\x53\x11\x1b\x17\x3b\x3b\x05\xd2\x2f\xa0\x80\x86\xe3\xb0\xf7\x12"
        "\xfc\xc7\xc7\x1a\x55\x7e\x2d\xb9\x66\xc3\xe9\xfa\x91\x74\x60\x39",
        "\x9d\x0e\x18\x09\x71\x64\x74\xcb\x08\x6e\x83\x4e\x31\x0a\x4a\x1c\xed\x14\x9e\x9c\x00\xf2\x48\x52\x79\x72\xce\xc5\x70\x4c\x2a\x5b"
        "\x07\xb8\xb3\xdc\x38\xec\xc4\xeb\xae\x97\xdd\xd8\x7f\x3d\x89\x85",
        },
        /* SHA-\x51\x2 */
        {
        "\xdd\xaf\x35\xa1\x93\x61\x7a\xba\xcc\x41\x73\x49\xae\x20\x41\x31\x12\xe6\xfa\x4e\x89\xa9\x7e\xa2\x0a\x9e\xee\xe6\x4b\x55\xd3\x9a"
        "\x21\x92\x99\x2a\x27\x4f\xc1\xa8\x36\xba\x3c\x23\xa3\xfe\xeb\xbd\x45\x4d\x44\x23\x64\x3c\xe8\x0e\x2a\x9a\xc9\x4f\xa5\x4c\xa4\x9f",
        "\x8e\x95\x9b\x75\xda\xe3\x13\xda\x8c\xf4\xf7\x28\x14\xfc\x14\x3f\x8f\x77\x79\xc6\xeb\x9f\x7f\xa1\x72\x99\xae\xad\xb6\x88\x90\x18"
        "\x50\x1d\x28\x9e\x49\x00\xf7\xe4\x33\x1b\x99\xde\xc4\xb5\x43\x3a\xc7\xd3\x29\xee\xb6\xdd\x26\x54\x5e\x96\xe5\x5b\x87\x4b\xe9\x09",
        "\xe7\x18\x48\x3d\x0c\xe7\x69\x64\x4e\x2e\x42\xc7\xbc\x15\xb4\x63\x8e\x1f\x98\xb1\x3b\x20\x44\x28\x56\x32\xa8\x03\xaf\xa9\x73\xeb"
        "\xde\x0f\xf2\x44\x87\x7e\xa6\x0a\x4c\xb0\x43\x2c\xe5\x77\xc3\x1b\xeb\x00\x9c\x5c\x2c\x49\xaa\x2e\x4e\xad\xb2\x17\xad\x8c\xc0\x9b"
        }
    };

    char message1[] = "abc";
    char message2a[] = "abcdbcdecdefdefgefghfghighijhi"
                       "jkijkljklmklmnlmnomnopnopq";
    char message2b[] = "abcdefghbcdefghicdefghijdefghijkefghij"
                       "klfghijklmghijklmnhijklmnoijklmnopjklm"
                       "nopqklmnopqrlmnopqrsmnopqrstnopqrstu";
    unsigned char *message3;
    unsigned int message3_len  = 1000000;
    unsigned char digest[OGS_SHA512_DIGEST_SIZE];
    int rc;

    message3 = malloc(message3_len);
    if (message3 == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        return;
    }
    memset(message3, 'a', message3_len);

    /* OGS_SHA-2 FIPS 180-2 Validation tests.
     * OGS_SHA-224 Test vector */

    ogs_sha224((uint8_t *)message1, strlen((char *) message1), digest);
    rc = memcmp(vectors[0][0], digest, OGS_SHA224_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);
    ogs_sha224((uint8_t *)message2a, strlen((char *) message2a), digest);
    rc = memcmp(vectors[0][1], digest, OGS_SHA224_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);
    ogs_sha224((uint8_t *)message3, message3_len, digest);
    rc = memcmp(vectors[0][2], digest, OGS_SHA224_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);

    /* OGS_SHA-256 Test vector */

    ogs_sha256((uint8_t *)message1, strlen((char *) message1), digest);
    rc = memcmp(vectors[1][0], digest, OGS_SHA256_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);
    ogs_sha256((uint8_t *)message2a, strlen((char *) message2a), digest);
    rc = memcmp(vectors[1][1], digest, OGS_SHA256_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);
    ogs_sha256((uint8_t *)message3, message3_len, digest);
    rc = memcmp(vectors[1][2], digest, OGS_SHA256_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);

    /* OGS_SHA-384 Test vector */

    ogs_sha384((uint8_t *)message1, strlen((char *) message1), digest);
    rc = memcmp(vectors[2][0], digest, OGS_SHA384_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);
    ogs_sha384((uint8_t *)message2b, strlen((char *) message2b), digest);
    rc = memcmp(vectors[2][1], digest, OGS_SHA384_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);
    ogs_sha384((uint8_t *)message3, message3_len, digest);
    rc = memcmp(vectors[2][2], digest, OGS_SHA384_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);

    /* OGS_SHA-512 Test vector */
    ogs_sha512((uint8_t *)message1, strlen((char *) message1), digest);
    rc = memcmp(vectors[3][0], digest, OGS_SHA512_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);
    ogs_sha512((uint8_t *)message2b, strlen((char *) message2b), digest);
    rc = memcmp(vectors[3][1], digest, OGS_SHA512_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);
    ogs_sha512((uint8_t *)message3, message3_len, digest);
    rc = memcmp(vectors[3][2], digest, OGS_SHA512_DIGEST_SIZE);
    ABTS_INT_EQUAL(tc, 0, rc);

    free(message3);
}


abts_suite *test_sha(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, sha1_test1, NULL);
    abts_run_test(suite, sha2_test1, NULL);

    return suite;
}
