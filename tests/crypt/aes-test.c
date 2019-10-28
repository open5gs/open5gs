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
#include "ogs-crypt.h"
#include "core/abts.h"

typedef struct {
    unsigned char *key;
    unsigned int *rk;
    int key_bits;
    unsigned char input[16];
    unsigned char cipher_output[16];
    unsigned char decipher_output[16];
} aes_test_vector_t;

static void aes_test1(abts_case *tc, void *data)
{
    const int key_bits = 128;
    unsigned int rk[OGS_AES_RKLENGTH(128)];
    unsigned char key[16] =
            "\x00\x01\x02\x03\x05\x06\x07\x08\x0A\x0B\x0C\x0D\x0F\x10\x11\x12";
    unsigned char pt[16] = "\x50\x68\x12\xA4\x5F\x08\xC8\x89\xB9\x7F\x59\x80\x03\x8B\x83\x59";
    unsigned char ct[16];
    unsigned char expected[16] =
        "\xD8\xF5\x32\x53\x82\x89\xEF\x7D\x06\xB5\x06\xA4\xFD\x5B\xE9\xC9";
    int nrounds;
    int rc;

    nrounds = ogs_aes_setup_enc(rk, key, key_bits);
    ogs_aes_encrypt(rk ,nrounds, pt, ct);

    rc = memcmp(ct, expected, 16);

    ABTS_INT_EQUAL(tc, 0, rc);
}

static void aes_test2(abts_case *tc, void *data)
{
    unsigned char tmp[16];
    int nrounds;
    int rc;
    aes_test_vector_t test_vector[3];
    int i;


    test_vector[0].key_bits = 128;
    test_vector[0].key =
        ogs_malloc(sizeof(char)*OGS_AES_KEYLENGTH(test_vector[0].key_bits));
    test_vector[0].rk =
        ogs_malloc(sizeof(unsigned int)*OGS_AES_RKLENGTH(test_vector[0].key_bits));
    memcpy(test_vector[0].key,
            "\x95\xA8\xEE\x8E\x89\x97\x9B\x9E"
            "\xFD\xCB\xC6\xEB\x97\x97\x52\x8D",
            OGS_AES_KEYLENGTH(test_vector[0].key_bits));
    memcpy(test_vector[0].input,
            "\x4E\xC1\x37\xA4\x26\xDA\xBF\x8A"
            "\xA0\xBE\xB8\xBC\x0C\x2B\x89\xD6",
            16);
    memcpy(test_vector[0].cipher_output,
            "\xD9\xB6\x5D\x12\x32\xBA\x01\x99"
            "\xCD\xBD\x48\x7B\x2A\x1F\xD6\x46",
            16);
    memcpy(test_vector[0].decipher_output,
            "\x95\x70\xC3\x43\x63\x56\x5B\x39"
            "\x35\x03\xA0\x01\xC0\xE2\x3B\x65",
            16);

    test_vector[1].key_bits = 192;
    test_vector[1].key =
        ogs_malloc(sizeof(char)*OGS_AES_KEYLENGTH(test_vector[1].key_bits));
    test_vector[1].rk =
        ogs_malloc(sizeof(unsigned int)*OGS_AES_RKLENGTH(test_vector[1].key_bits));
    memcpy(test_vector[1].key,
            "\x95\xA8\xEE\x8E\x89\x97\x9B\x9E"
            "\xFD\xCB\xC6\xEB\x97\x97\x52\x8D"
            "\x43\x2D\xC2\x60\x61\x55\x38\x18",
            OGS_AES_KEYLENGTH(test_vector[1].key_bits));
    memcpy(test_vector[1].input ,
            "\x4E\xC1\x37\xA4\x26\xDA\xBF\x8A"
            "\xA0\xBE\xB8\xBC\x0C\x2B\x89\xD6",
            16);
    memcpy(test_vector[1].cipher_output,
            "\xB1\x8B\xB3\xE7\xE1\x07\x32\xBE"
            "\x13\x58\x44\x3A\x50\x4D\xBB\x49",
            16);
    memcpy(test_vector[1].decipher_output,
            "\x29\xDF\xD7\x5B\x85\xCE\xE4\xDE"
            "\x6E\x26\xA8\x08\xCD\xC2\xC9\xC3",
            16);

    test_vector[2].key_bits = 256;
    test_vector[2].key =
        ogs_malloc(sizeof(char)*OGS_AES_KEYLENGTH(test_vector[2].key_bits));
    test_vector[2].rk =
        ogs_malloc(sizeof(unsigned int)*OGS_AES_RKLENGTH(test_vector[2].key_bits));
    memcpy(test_vector[2].key,
            "\x95\xA8\xEE\x8E\x89\x97\x9B\x9E"
            "\xFD\xCB\xC6\xEB\x97\x97\x52\x8D"
            "\x43\x2D\xC2\x60\x61\x55\x38\x18"
            "\xEA\x63\x5E\xC5\xD5\xA7\x72\x7E",
            OGS_AES_KEYLENGTH(test_vector[2].key_bits));
    memcpy(test_vector[2].input ,
            "\x4E\xC1\x37\xA4\x26\xDA\xBF\x8A"
            "\xA0\xBE\xB8\xBC\x0C\x2B\x89\xD6",
            16);
    memcpy(test_vector[2].cipher_output,
            "\x2F\x9C\xFD\xDB\xFF\xCD\xE6\xB9"
            "\xF3\x7E\xF8\xE4\x0D\x51\x2C\xF4",
            16);
    memcpy(test_vector[2].decipher_output,
            "\x11\x0A\x35\x45\xCE\x49\xB8\x4B"
            "\xBB\x7B\x35\x23\x61\x08\xFA\x6E",
            16);

    for (i=0; i<3; i++)
    {
        nrounds = ogs_aes_setup_enc(test_vector[i].rk, test_vector[i].key,
                                      test_vector[i].key_bits);
        ogs_aes_encrypt(test_vector[i].rk ,nrounds, test_vector[i].input, tmp);

        rc = memcmp(tmp, test_vector[i].cipher_output, 16);
        ABTS_INT_EQUAL(tc, 0, rc);

        nrounds = ogs_aes_setup_dec(test_vector[i].rk, test_vector[i].key,
                                      test_vector[i].key_bits);
        ogs_aes_decrypt(test_vector[i].rk ,nrounds, test_vector[i].input, tmp);
        rc = memcmp(tmp, test_vector[i].decipher_output, 16);
        ABTS_INT_EQUAL(tc, 0, rc);

        ogs_free(test_vector[i].key);
        ogs_free(test_vector[i].rk);
    }
}

#if 0
    Case #1: Encrypting 16 bytes (1 block) using AES-CBC with 128-bit key
    Key       : 0x06a9214036b8a15b512e03d534120006
    IV        : 0x3dafba429d9eb430b422da802c9fac41
    Plaintext : "Single block msg"
    Ciphertext: 0xe353779c1079aeb82708942dbe77181a

    Case #2: Encrypting 32 bytes (2 blocks) using AES-CBC with 128-bit key
    Key       : 0xc286696d887c9aa0611bbb3e2025a45a
    IV        : 0x562e17996d093d28ddb3ba695a2e6f58
    Plaintext : 0x000102030405060708090a0b0c0d0e0f
                101112131415161718191a1b1c1d1e1f
    Ciphertext: 0xd296cd94c2cccf8a3a863028b5e1dc0a
                7586602d253cfff91b8266bea6d61ab1

    Case #3: Encrypting 48 bytes (3 blocks) using AES-CBC with 128-bit key
    Key       : 0x6c3ea0477630ce21a2ce334aa746c2cd
    IV        : 0xc782dc4c098c66cbd9cd27d825682c81
    Plaintext : "This is a 48-byte message (exactly 3 AES blocks)"
    Ciphertext: 0xd0a02b3836451753d493665d33f0e886
                2dea54cdb293abc7506939276772f8d5
                021c19216bad525c8579695d83ba2684

    Case #4: Encrypting 64 bytes (4 blocks) using AES-CBC with 128-bit key
    Key       : 0x56e47a38c5598974bc46903dba290349
    IV        : 0x8ce82eefbea0da3c44699ed7db51b7d9
    Plaintext : 0xa0a1a2a3a4a5a6a7a8a9aaabacadaeaf
                b0b1b2b3b4b5b6b7b8b9babbbcbdbebf
                c0c1c2c3c4c5c6c7c8c9cacbcccdcecf
                d0d1d2d3d4d5d6d7d8d9dadbdcdddedf
    Ciphertext: 0xc30e32ffedc0774e6aff6af0869f71aa
                0f3af07a9a31a9c684db207eb0ef8e4e
                35907aa632c3ffdf868bb7b29d3d46ad
                83ce9f9a102ee99d49a53e87f4c3da55
#endif

typedef struct {
    unsigned char key[33];
    int key_bits;
    unsigned char ivec[33];
    unsigned char plain[65];
    unsigned int plainlen;
    unsigned char cipher[65];
    unsigned int cipherlen;
} aes_cbc_test_vector_t;

static void aes_test3(abts_case *tc, void *data)
{
    aes_cbc_test_vector_t tv[5] = {
        {
            "\x06\xa9\x21\x40\x36\xb8\xa1\x5b\x51\x2e\x03\xd5\x34\x12\x00\x06",
            128,
            "\x3d\xaf\xba\x42\x9d\x9e\xb4\x30\xb4\x22\xda\x80\x2c\x9f\xac\x41",
            "Single block msg",
            16,
            "\xe3\x53\x77\x9c\x10\x79\xae\xb8\x27\x08\x94\x2d\xbe\x77\x18\x1a",
            16
        }, {
            "\xc2\x86\x69\x6d\x88\x7c\x9a\xa0\x61\x1b\xbb\x3e\x20\x25\xa4\x5a",
            128,
            "\x56\x2e\x17\x99\x6d\x09\x3d\x28\xdd\xb3\xba\x69\x5a\x2e\x6f\x58",
            "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
            "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f",
            32,
            "\xd2\x96\xcd\x94\xc2\xcc\xcf\x8a\x3a\x86\x30\x28\xb5\xe1\xdc\x0a"
            "\x75\x86\x60\x2d\x25\x3c\xff\xf9\x1b\x82\x66\xbe\xa6\xd6\x1a\xb1",
            32
        }, {

            "\x6c\x3e\xa0\x47\x76\x30\xce\x21\xa2\xce\x33\x4a\xa7\x46\xc2\xcd",
            128,
            "\xc7\x82\xdc\x4c\x09\x8c\x66\xcb\xd9\xcd\x27\xd8\x25\x68\x2c\x81",
            "This is a 48-byte message (exactly 3 AES blocks)",
            48,
            "\xd0\xa0\x2b\x38\x36\x45\x17\x53\xd4\x93\x66\x5d\x33\xf0\xe8\x86"
            "\x2d\xea\x54\xcd\xb2\x93\xab\xc7\x50\x69\x39\x27\x67\x72\xf8\xd5"
            "\x02\x1c\x19\x21\x6b\xad\x52\x5c\x85\x79\x69\x5d\x83\xba\x26\x84",
            48
        }, {
            "\x56\xe4\x7a\x38\xc5\x59\x89\x74\xbc\x46\x90\x3d\xba\x29\x03\x49",
            128,
            "\x8c\xe8\x2e\xef\xbe\xa0\xda\x3c\x44\x69\x9e\xd7\xdb\x51\xb7\xd9",
            "\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf"
            "\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
            "\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf"
            "\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf",
            64,
            "\xc3\x0e\x32\xff\xed\xc0\x77\x4e\x6a\xff\x6a\xf0\x86\x9f\x71\xaa"
            "\x0f\x3a\xf0\x7a\x9a\x31\xa9\xc6\x84\xdb\x20\x7e\xb0\xef\x8e\x4e"
            "\x35\x90\x7a\xa6\x32\xc3\xff\xdf\x86\x8b\xb7\xb2\x9d\x3d\x46\xad"
            "\x83\xce\x9f\x9a\x10\x2e\xe9\x9d\x49\xa5\x3e\x87\xf4\xc3\xda\x55",
            64
        }, {
            "\x56\xe4\x7a\x38\xc5\x59\x89\x74\xbc\x46\x90\x3d\xba\x29\x03\x49",
            128,
            "\x8c\xe8\x2e\xef\xbe\xa0\xda\x3c\x44\x69\x9e\xd7\xdb\x51\xb7\xd9",
            "\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf"
            "\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
            "\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf"
            "\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd",
            62,
            "\xc3\x0e\x32\xff\xed\xc0\x77\x4e\x6a\xff\x6a\xf0\x86\x9f\x71\xaa"
            "\x0f\x3a\xf0\x7a\x9a\x31\xa9\xc6\x84\xdb\x20\x7e\xb0\xef\x8e\x4e"
            "\x35\x90\x7a\xa6\x32\xc3\xff\xdf\x86\x8b\xb7\xb2\x9d\x3d\x46\xad"
            "\x5d\x96\x74\x07\xe9\x86\x0c\x0b\x50\x80\x4a\xfb\x69\xc1\x13\xaa",
            64
        }
    };

    unsigned char out[128];
    unsigned int i, rc, outlen;
    uint8_t ivec[32];

    for (i = 0; i < 5; i++)
    {
        outlen = sizeof(out);
        memcpy(ivec, tv[i].ivec, 16);
        ogs_aes_cbc_encrypt(tv[i].key, tv[i].key_bits, ivec,
                tv[i].plain, tv[i].plainlen, out, &outlen);
        ABTS_INT_EQUAL(tc, tv[i].cipherlen, outlen);
        rc = memcmp(tv[i].cipher, out, tv[i].cipherlen);
        ABTS_INT_EQUAL(tc, 0, rc);

        outlen = sizeof(out);
        memcpy(ivec, tv[i].ivec, 16);
        ogs_aes_cbc_decrypt(tv[i].key, tv[i].key_bits, ivec,
                tv[i].cipher, tv[i].cipherlen, out, &outlen);
        ABTS_INT_EQUAL(tc, tv[i].cipherlen, outlen);
        rc = memcmp(tv[i].plain, out, tv[i].plainlen);
        ABTS_INT_EQUAL(tc, 0, rc);
    }
}

/*  RFC 4493

    --------------------------------------------------
    Subkey Generation
    K              2b7e1516 28aed2a6 abf71588 09cf4f3c
    AES-128(key,0) 7df76b0c 1ab899b3 3e42f047 b91b546f
    K1             fbeed618 35713366 7c85e08f 7236a8de
    K2             f7ddac30 6ae266cc f90bc11e e46d513b
    --------------------------------------------------

    --------------------------------------------------
    Example 1: len = 0
    M              <empty string>
    AES-CMAC       bb1d6929 e9593728 7fa37d12 9b756746
    --------------------------------------------------

    Example 2: len = 16
    M              6bc1bee2 2e409f96 e93d7e11 7393172a
    AES-CMAC       070a16b4 6b4d4144 f79bdd9d d04a287c
    --------------------------------------------------

    Example 3: len = 40
    M              6bc1bee2 2e409f96 e93d7e11 7393172a
                   ae2d8a57 1e03ac9c 9eb76fac 45af8e51
                   30c81c46 a35ce411
    AES-CMAC       dfa66747 de9ae630 30ca3261 1497c827
    --------------------------------------------------

    Example 4: len = 64
    M              6bc1bee2 2e409f96 e93d7e11 7393172a
                   ae2d8a57 1e03ac9c 9eb76fac 45af8e51
                   30c81c46 a35ce411 e5fbc119 1a0a52ef
                   f69f2445 df4f9b17 ad2b417b e66c3710
    AES-CMAC       51f0bebf 7e3b9d92 fc497417 79363cfe
    --------------------------------------------------
*/

static void cmac_test(abts_case *tc, void *data)
{
    uint8_t key[16] = {
        0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
    };

    uint8_t msg[4][64] = {
        {
            /* Empty string */
        },
        {
            0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
            0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a
        }, {
            0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
            0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
            0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
            0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
            0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11
        }, {
            0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
            0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
            0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
            0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
            0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,
            0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
            0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,
            0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10
        }
    };

    uint32_t msglen[4] = {0, 16, 40, 64};

    uint8_t cmac_answer[4][16] = {
        {
            0xbb,0x1d,0x69,0x29,0xe9,0x59,0x37,0x28,
            0x7f,0xa3,0x7d,0x12,0x9b,0x75,0x67,0x46
        }, {
            0x07,0x0a,0x16,0xb4,0x6b,0x4d,0x41,0x44,
            0xf7,0x9b,0xdd,0x9d,0xd0,0x4a,0x28,0x7c
        }, {
            0xdf,0xa6,0x67,0x47,0xde,0x9a,0xe6,0x30,
            0x30,0xca,0x32,0x61,0x14,0x97,0xc8,0x27
        }, {
            0x51,0xf0,0xbe,0xbf,0x7e,0x3b,0x9d,0x92,
            0xfc,0x49,0x74,0x17,0x79,0x36,0x3c,0xfe
        }
    };

    uint8_t cmac[16];

    int i, rc;
    int rv;

    for (i = 0; i < 4; i++)
    {
        rv = ogs_aes_cmac_calculate(cmac, key, msg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        rc = memcmp(cmac, cmac_answer[i], 16);
        ABTS_INT_EQUAL(tc, 0, rc);
    }

    for (i = 0; i < 4; i++)
    {
        rv = ogs_aes_cmac_verify(cmac_answer[i], key, msg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    key[0] = 0;
    for (i = 0; i < 4; i++)
    {
        rv = ogs_aes_cmac_verify(cmac_answer[i], key, msg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, OGS_ERR_INVALID_CMAC, rv);
    }
}

abts_suite *test_aes(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, aes_test1, NULL);
    abts_run_test(suite, aes_test2, NULL);
    abts_run_test(suite, aes_test3, NULL);
    abts_run_test(suite, cmac_test, NULL);

    return suite;
}
