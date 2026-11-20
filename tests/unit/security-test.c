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

#include "ogs-nas-common.h"
#include "core/abts.h"

static void security_test1(abts_case *tc, void *data)
{
    const char *_k = "465b5ce8 b199b49f aa5f0a2e e238a6bc";
    const char *_rand = "23553cbe 9637a89d 218ae64d ae47bf35";
    const char *_sqn = "ff9bb4d0 b607";
    const char *_amf = "b9b9";
    const char *_op = "cdc202d5 123e20f6 2b6d676a c72cb318";
    const char *_opc =    "cd63cb71 954a9f4e 48a5994e 37a02baf";
    const char *_mac_a = "4a9ffac3 54dfafb3";
    const char *_mac_s = "01cfaf9e c4e871e9";
    const char *_res = "a54211d5 e3ba50bf";
    const char *_ck = "b40ba9a3 c58b2a05 bbf0d987 b21bf8cb";
    const char *_ik = "f769bcd7 51044604 12767271 1c6d3441";
    const char *_ak = "aa689c64 8370";
    const char *_akstar = "451e8bec a43b";

    uint8_t k[16];
    uint8_t rand[16];
    uint8_t op[16];
    uint8_t opc[16];
    uint8_t sqn[6];
    uint8_t amf[2];
    uint8_t mac_a[8];
    uint8_t mac_s[8];
    uint8_t res[8];
    uint8_t ck[16];
    uint8_t ik[16];
    uint8_t ak[6];
    uint8_t akstar[6];

    uint8_t tmp[16];

    milenage_opc(ogs_hex_from_string(_k, k, sizeof(k)),
            ogs_hex_from_string(_op, op, sizeof(op)), opc);
    ABTS_TRUE(tc, memcmp(opc,
                ogs_hex_from_string(_opc, tmp, sizeof(tmp)), 16) == 0);

    milenage_f1(opc, k, 
        ogs_hex_from_string(_rand, rand, sizeof(rand)),
        ogs_hex_from_string(_sqn, sqn, sizeof(sqn)),
        ogs_hex_from_string(_amf, amf, sizeof(amf)),
        mac_a, mac_s);
    ABTS_TRUE(tc, memcmp(mac_a, 
        ogs_hex_from_string(_mac_a, tmp, sizeof(tmp)), 8) == 0);
    ABTS_TRUE(tc, memcmp(mac_s, 
        ogs_hex_from_string(_mac_s, tmp, sizeof(tmp)), 8) == 0);

    milenage_f2345(opc, k, rand, res, ck, ik, ak, akstar);
    ABTS_TRUE(tc, memcmp(res,
                ogs_hex_from_string(_res, tmp, sizeof(tmp)), 8) == 0);
    ABTS_TRUE(tc, memcmp(ck,
                ogs_hex_from_string(_ck, tmp, sizeof(tmp)), 16) == 0);
    ABTS_TRUE(tc, memcmp(ik,
                ogs_hex_from_string(_ik, tmp, sizeof(tmp)), 16) == 0);
    ABTS_TRUE(tc, memcmp(ak,
                ogs_hex_from_string(_ak, tmp, sizeof(tmp)), 6) == 0);
    ABTS_TRUE(tc, memcmp(akstar, 
        ogs_hex_from_string(_akstar, tmp, sizeof(tmp)), 6) == 0);
}

static void security_test2(abts_case *tc, void *data)
{
    const char *_key = "4a656665";
    const char *_message = "7768617420646f2079612077616e7420666f72206e6f7468696e673f";
    const char *_hmac = "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843";

    uint8_t key[4];
    uint8_t message[28];
    uint8_t hmac[32];
    uint8_t tmp[32];

    ogs_hmac_sha256(ogs_hex_from_string(_key, key, sizeof(key)), 4,
        ogs_hex_from_string(_message, message, sizeof(message)), 28,
        hmac, 32);

    ABTS_TRUE(tc, memcmp(hmac,
                ogs_hex_from_string(_hmac, tmp, sizeof(tmp)), 32) == 0);
}

static void security_test3(abts_case *tc, void *data)
{
    const char *_ck = "b40ba9a3c58b2a05 bbf0d987b21bf8cb";
    const char *_ik = "f769bcd751044604 127672711c6d3441";
    const char *_plmn_id = "024830";
    const char *_sqn = "fd8eef40df7d";
    const char *_ak = "aa689c648370";
    const char *_kasme =
        "238e457e0f758bad bca8d34bb2612c10 428d426757cb5553 b2b184fa64bfc549";
    uint8_t ck[16];
    uint8_t ik[16];
    uint8_t plmn_id[3];
    uint8_t sqn[6];
    uint8_t ak[6];
    uint8_t kasme[32];
    uint8_t tmp[32];

    ogs_auc_kasme(
        ogs_hex_from_string(_ck, ck, sizeof(ck)),
        ogs_hex_from_string(_ik, ik, sizeof(ik)),
        ogs_hex_from_string(_plmn_id, plmn_id, sizeof(plmn_id)),
        ogs_hex_from_string(_sqn, sqn, sizeof(sqn)),
        ogs_hex_from_string(_ak, ak, sizeof(ak)),
        kasme);

    ABTS_TRUE(tc, 
        memcmp(kasme, ogs_hex_from_string(_kasme, tmp, sizeof(tmp)), 32) == 0);
}

static void security_test4(abts_case *tc, void *data)
{
#define SECURITY_TEST4_BIT_LEN 88
#define SECURITY_TEST4_LEN ((SECURITY_TEST4_BIT_LEN+7)/8)
    const char *_ik = "2bd6459f 82c5b300 952c4910 4881ff48";
    const char *_message = "33323462 63393861 37347900 00000000";
    const char *_mact = "731f1165";
    uint8_t ik[16];
    uint8_t message[SECURITY_TEST4_LEN];
    uint8_t mact[4];
    uint8_t tmp[4];
    uint8_t mac[4];
    ogs_pkbuf_t *pkbuf = NULL;

    snow_3g_f9(ogs_hex_from_string(_ik, ik, sizeof(ik)),
        0x38a6f056, ((int64_t)0x1f << 27), 0, 
        ogs_hex_from_string(_message, message, sizeof(message)),
        SECURITY_TEST4_BIT_LEN, mact);
    ABTS_TRUE(tc, memcmp(mact,
                ogs_hex_from_string(_mact, tmp, sizeof(tmp)), 4) == 0);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+SECURITY_TEST4_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(pkbuf, message, SECURITY_TEST4_LEN);

    ogs_nas_mac_calculate(OGS_NAS_SECURITY_ALGORITHMS_128_EIA1, 
            ik, 0x38a6f056, 0x1f, 0, pkbuf, mac);
    ABTS_TRUE(tc, memcmp(mac, tmp, 4) == 0);
    ogs_pkbuf_free(pkbuf);
}

static void security_test5(abts_case *tc, void *data)
{
#define SECURITY_TEST5_BIT_LEN 800
#define SECURITY_TEST5_LEN ((SECURITY_TEST5_BIT_LEN+7)/8)
    const char *_ck = "2bd6459f 82c5b300 952c4910 4881ff48";
    const char *_plain = 
        "7ec61272 743bf161 4726446a 6c38ced1"
        "66f6ca76 eb543004 4286346c ef130f92"
        "922b0345 0d3a9975 e5bd2ea0 eb55ad8e"
        "1b199e3e c4316020 e9a1b285 e7627953"
        "59b7bdfd 39bef4b2 484583d5 afe082ae"
        "e638bf5f d5a60619 3901a08f 4ab41aab"
        "9b134880";
    const char *_cipher = 
        "8ceba629 43dced3a 0990b06e a1b0a2c4"
        "fb3cedc7 1b369f42 ba64c1eb 6665e72a"
        "a1c9bb0d eaa20fe8 6058b8ba ee2c2e7f"
        "0becce48 b52932a5 3c9d5f93 1a3a7c53"
        "2259af43 25e2a65e 3084ad5f 6a513b7b"
        "ddc1b65f 0aa0d97a 053db55a 88c4c4f9"
        "605e4143";
    uint8_t ck[16];
    uint8_t plain[SECURITY_TEST5_LEN];
    uint8_t tmp[SECURITY_TEST5_LEN];
    ogs_pkbuf_t *pkbuf = NULL;

    SNOW_CTX ctx;

    ogs_hex_from_string(_plain, plain, sizeof(plain));
    SNOW_init(0x72a4f20f, 0x0c, 1,
        ogs_hex_from_string(_ck, ck, sizeof(ck)), &ctx);
    SNOW(SECURITY_TEST5_LEN, plain, plain, &ctx);
    ABTS_TRUE(tc, memcmp(plain, 
        ogs_hex_from_string(_cipher, tmp, sizeof(tmp)),
        SECURITY_TEST5_LEN) == 0);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+SECURITY_TEST5_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(pkbuf, plain, SECURITY_TEST5_LEN);

    ogs_nas_encrypt(OGS_NAS_SECURITY_ALGORITHMS_128_EEA1,
        ck, 0x72a4f20f, 0x0c, 1, pkbuf);
    ABTS_TRUE(tc, memcmp(pkbuf->data,
                ogs_hex_from_string(_plain, tmp, sizeof(tmp)),
        SECURITY_TEST5_LEN) == 0);
    ogs_pkbuf_free(pkbuf);
}

static void security_test6(abts_case *tc, void *data)
{
#define SECURITY_TEST6_BIT_LEN 64
#define SECURITY_TEST6_LEN ((SECURITY_TEST6_BIT_LEN+7)/8)
    const char *_ik = "d3c5d592 327fb11c 4035c668 0af8c6d1";
    const char *_message = "484583d5 afe082ae";
    const char *_mact = "b93787e6";
    uint8_t ik[16];
    uint8_t message[SECURITY_TEST6_LEN];
    uint8_t mact[16];
    uint8_t tmp[4];
    uint8_t *m = NULL;
    uint32_t count = htonl(0x398a59b4);
    int msg_len = SECURITY_TEST6_LEN;
    int m_len = 8+msg_len;
    uint8_t mac[4];
    ogs_pkbuf_t *pkbuf = NULL;

    m = ogs_calloc(m_len, sizeof(uint8_t));
    ABTS_PTR_NOTNULL(tc, m);
    memcpy(m, &count, sizeof(uint32_t));
    m[4] = ((0x1a << 3) | (1 << 2));
    memcpy(m+8,
            ogs_hex_from_string(_message, message, sizeof(message)), msg_len);

    ogs_aes_cmac_calculate(mact,
            ogs_hex_from_string(_ik, ik, sizeof(ik)), m, m_len);

    ogs_free(m);

    ABTS_TRUE(tc, memcmp(mact,
                ogs_hex_from_string(_mact, tmp, sizeof(tmp)), 4) == 0);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+SECURITY_TEST6_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(pkbuf, message, SECURITY_TEST6_LEN);

    ogs_nas_mac_calculate(OGS_NAS_SECURITY_ALGORITHMS_128_EIA2, 
            ik, 0x398a59b4, 0x1a, 1, pkbuf, mac);
    ABTS_TRUE(tc, memcmp(mac, tmp, 4) == 0);
    ogs_pkbuf_free(pkbuf);
}

static void security_test7(abts_case *tc, void *data)
{
#define SECURITY_TEST7_BIT_LEN 800
#define SECURITY_TEST7_LEN ((SECURITY_TEST7_BIT_LEN+7)/8)
    const char *_ck = "2bd6459f 82c440e0 952c4910 4805ff48";
    const char *_plain = 
        "7ec61272 743bf161 4726446a 6c38ced1 66f6ca76 eb543004 4286346c ef130f92"
        "922b0345 0d3a9975 e5bd2ea0 eb55ad8e 1b199e3e c4316020 e9a1b285 e7627953" 
        "59b7bdfd 39bef4b2 484583d5 afe082ae e638bf5f d5a60619 3901a08f 4ab41aab" 
        "9b134880";
    const char *_cipher = 
        "59616053 53c64bdc a15b195e 288553a9 10632506 d6200aa7 90c4c806 c99904cf"
        "2445cc50 bb1cf168 a4967373 4e081b57 e324ce52 59c0e78d 4cd97b87 0976503c"
        "0943f2cb 5ae8f052 c7b7d392 239587b8 956086bc ab188360 42e2e6ce 42432a17"
        "105c53d3";
    uint8_t ck[16];
    uint8_t plain[SECURITY_TEST7_LEN+100];
    uint8_t cipher[SECURITY_TEST7_LEN+100];
    uint8_t tmp[SECURITY_TEST7_LEN+100];
    ogs_pkbuf_t *pkbuf = NULL;

    uint8_t ivec[16];
    uint32_t count = htonl(0xc675a64b);
    memset(ivec, 0, sizeof(ivec));
    memcpy(ivec+0, &count, sizeof(count));
    ivec[4] = (0x0c << 3) | (1 << 2);

    ogs_aes_ctr128_encrypt(
        ogs_hex_from_string(_ck, ck, sizeof(ck)), ivec,
        ogs_hex_from_string(_plain, plain, sizeof(plain)), SECURITY_TEST7_LEN,
        cipher);

    ABTS_TRUE(tc,
            memcmp(cipher, ogs_hex_from_string(_cipher, tmp, sizeof(tmp)),
                        SECURITY_TEST7_LEN) == 0);

    memset(ivec, 0, sizeof(ivec));
    memcpy(ivec+0, &count, sizeof(count));
    ivec[4] = (0x0c << 3) | (1 << 2);

    ogs_aes_ctr128_encrypt(ogs_hex_from_string(_ck, ck, sizeof(ck)),
        ivec, cipher, SECURITY_TEST7_LEN, cipher);

    ABTS_TRUE(tc, memcmp(cipher, plain, SECURITY_TEST7_LEN) == 0);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+SECURITY_TEST7_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(pkbuf, plain, SECURITY_TEST7_LEN);

    ogs_nas_encrypt(OGS_NAS_SECURITY_ALGORITHMS_128_EEA2,
        ck, 0xc675a64b, 0x0c, 1, pkbuf);
    ABTS_TRUE(tc,
            memcmp(pkbuf->data, ogs_hex_from_string(_cipher, tmp, sizeof(tmp)),
                SECURITY_TEST7_LEN) == 0);
    ogs_pkbuf_free(pkbuf);
}

static void security_test8(abts_case *tc, void *data)
{
#define SECURITY_TEST8_BIT_LEN 584
#define SECURITY_TEST8_LEN ((SECURITY_TEST8_BIT_LEN+7)/8)
    const char *_ik = "c9 e6 ce c4 60 7c 72 db 00 0a ef a8 83 85 ab 0a";
    const char *_message = 
    "983b41d4 7d780c9e 1ad11d7e b70391b1 de0b35da 2dc62f83 e7b78d63 06ca0ea0"
    "7e941b7b e91348f9 fcb170e2 217fecd9 7f9f68ad b16e5d7d 21e569d2 80ed775c"
    "ebde3f40 93c53881 00000000";
    const char *_mact = "24a842b3";
    uint8_t ik[16];
    uint8_t message[SECURITY_TEST8_LEN];
    uint8_t mact[4];
    uint32_t mac32;
    ogs_pkbuf_t *pkbuf = NULL;
    uint8_t mac[4];

    zuc_eia3(
        ogs_hex_from_string(_ik, ik, sizeof(ik)),
        0xa94059da,
        0xa,
        1,
        SECURITY_TEST8_BIT_LEN,
        ogs_hex_from_string(_message, message, sizeof(message)),
        &mac32);
    mac32 = ntohl(mac32);

    ABTS_TRUE(tc, memcmp(&mac32, 
        ogs_hex_from_string(_mact, mact, sizeof(mact)), 4) == 0);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+SECURITY_TEST8_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(pkbuf, message, SECURITY_TEST8_LEN);

    ogs_nas_mac_calculate(OGS_NAS_SECURITY_ALGORITHMS_128_EIA3, 
            ik, 0xa94059da, 0xa, 1, pkbuf, mac);
    ABTS_TRUE(tc, memcmp(mac, mact, 4) == 0);
    ogs_pkbuf_free(pkbuf);

    /* TS 35.223, 128-EIA3 test set 3. Only the high bit of byte 73 counts. */
    zuc_eia3(ik, 0xa94059da, 0xa, 1, 577, message, &mac32);
    ABTS_TRUE(tc, mac32 == 0xfae8ff0b);

    message[sizeof(message) - 1] ^= 0x7f;
    zuc_eia3(ik, 0xa94059da, 0xa, 1, 577, message, &mac32);
    ABTS_TRUE(tc, mac32 == 0xfae8ff0b);
}

static void security_test9(abts_case *tc, void *data)
{
#define SECURITY_TEST9_BIT_LEN 200
#define SECURITY_TEST9_LEN ((SECURITY_TEST9_BIT_LEN+7)/8)
    const char *_ck = "17 3d 14 ba 50 03 73 1d 7a 60 04 94 70 f0 0a 29";
    const char *_plain = 
        "6cf65340 735552ab 0c9752fa 6f9025fe 0bd675d9 005875b2 00000000";
    const char *_cipher = 
        "a6c85fc6 6afb8533 aafc2518 dfe78494 0ee1e4b0 30238cc8 10000000";
    uint8_t ck[16];
    uint8_t plain[SECURITY_TEST9_LEN];
    uint8_t cipher[SECURITY_TEST9_LEN];
    uint8_t tmp[SECURITY_TEST9_LEN];
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_hex_from_string(_plain, plain, sizeof(plain));
    zuc_eea3(
        ogs_hex_from_string(_ck, ck, sizeof(ck)),
        0x66035492, 0xf, 0,
        SECURITY_TEST9_BIT_LEN, plain, plain);

    ABTS_TRUE(tc, memcmp(plain, ogs_hex_from_string(_cipher, tmp, sizeof(tmp)),
        SECURITY_TEST9_LEN) == 0);

    zuc_eea3(
        ogs_hex_from_string(_ck, ck, sizeof(ck)),
        0x66035492, 0xf, 0,
        SECURITY_TEST9_BIT_LEN, 
        ogs_hex_from_string(_plain, plain, sizeof(plain)),
        cipher);

    ABTS_TRUE(tc, memcmp(cipher, ogs_hex_from_string(_cipher, tmp, sizeof(tmp)),
        SECURITY_TEST9_LEN) == 0);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+SECURITY_TEST9_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(pkbuf, plain, SECURITY_TEST9_LEN);

    ogs_nas_encrypt(OGS_NAS_SECURITY_ALGORITHMS_128_EEA3,
        ck, 0x66035492, 0xf, 0, pkbuf);
    ABTS_TRUE(tc,
        memcmp(pkbuf->data, ogs_hex_from_string(_cipher, tmp, sizeof(tmp)),
            SECURITY_TEST9_LEN) == 0);
    ogs_pkbuf_free(pkbuf);

    /* TS 35.223, 128-EEA3 test set 1. Clear unused bits of the ciphertext. */
    ogs_hex_from_string(_plain, plain, sizeof(plain));
    plain[sizeof(plain) - 1] ^= 0x7f;
    tmp[sizeof(tmp) - 1] = 0;
    zuc_eea3(ck, 0x66035492, 0xf, 0, 193, plain, cipher);
    ABTS_TRUE(tc, memcmp(cipher, tmp, sizeof(cipher)) == 0);
    zuc_eea3(ck, 0x66035492, 0xf, 0, 193, plain, plain);
    ABTS_TRUE(tc, memcmp(plain, tmp, sizeof(plain)) == 0);
}

static void security_test10(abts_case *tc, void *data)
{
    typedef void (*hmac_func_t)(const uint8_t *key, uint32_t key_size,
            const uint8_t *message, uint32_t message_len,
            uint8_t *mac, uint32_t mac_size);
    struct hmac_test_vector {
        hmac_func_t hmac;
        uint32_t digest_size;
        uint32_t block_size;
        const char *empty_key_mac;
        const char *digest_plus_one_key_mac;
        const char *block_size_key_mac;
    } test_vector[] = {
        {
            ogs_hmac_sha224, OGS_SHA224_DIGEST_SIZE, OGS_SHA224_BLOCK_SIZE,
            "c4c4d3cfadfee804dba5ef6839f22ae7779d9f99f81e73fdb3272d56",
            "c1d53c767acc64f8dd96a5fb4512c2973d675dfc8dc9461871919373",
            "928065aec02388cf28bd336fecd26a8c81a042553bcf4f00bbbeeacf",
        },
        {
            ogs_hmac_sha256, OGS_SHA256_DIGEST_SIZE, OGS_SHA256_BLOCK_SIZE,
            "e8d1b47fcaaf6553f3f2c662f0ce0f3d178f8f1d050337b174060e5900bc8bc0",
            "1aee398a590adb7f7feb757de9e340c4406a68dddeb537361350b9695263058c",
            "070f665404b179c4eb5c8f67d43d72ab7a4aea4b9f98a275c9d8c6215d6bed3c",
        },
        {
            ogs_hmac_sha384, OGS_SHA384_DIGEST_SIZE, OGS_SHA384_BLOCK_SIZE,
            "ba252d647d257da0c4877ba608b9ac8ee5dd5f7ce4453415ef6de2b452f54c32"
            "4982365861dcd4a58dd415b498ec36f9",
            "492b30320e6420f3c3d5f32d315d428d999c01afd53eb1b3d31f785945edf54a"
            "7862d4dc474c2fc359d890c10a6f3391",
            "b76fe3e48157856a9bae416508f305a24de87f5d790d70ae0bb8b3c0cf2432a0"
            "f8b06d954435257845db7aaf7e3a6770",
        },
        {
            ogs_hmac_sha512, OGS_SHA512_DIGEST_SIZE, OGS_SHA512_BLOCK_SIZE,
            "146ea76e8638ac4810323d94a08a8d4d04e6c4a5eeb193eaf20b8f17c1c43803"
            "d1b481774bd0aa353616175e85b438248c8ef762d23a73e351cac777b4696559",
            "b6389389f70bc7ee58e8a60d477d912d145435140f100d5c7ae9084d54a04fc0"
            "e7f8e158a9647d917164de011f6a340d0401805392ee75ca207cd7e7d7dd8241",
            "d091afeea09a476620b932bbb7ae463a606cabf48a7718366d16f99bb648fc577"
            "0a0f28ec3b9896bd3b7b610975f945c4d682bea5aee8a365069e42bd03b26a6",
        },
    };
    const uint8_t message[] = "Open5GS HMAC boundary test";
    uint8_t key[OGS_SHA512_BLOCK_SIZE];
    uint8_t mac[OGS_SHA512_DIGEST_SIZE];
    uint8_t expected[OGS_SHA512_DIGEST_SIZE];
    int i, j;

    for (i = 0; i < sizeof(key); i++)
        key[i] = i;

    for (i = 0; i < OGS_ARRAY_SIZE(test_vector); i++) {
        test_vector[i].hmac(NULL, 0, message, sizeof(message)-1,
                mac, test_vector[i].digest_size);
        ABTS_TRUE(tc, memcmp(mac, ogs_hex_from_string(
                        test_vector[i].empty_key_mac,
                        expected, sizeof(expected)),
                    test_vector[i].digest_size) == 0);

        j = test_vector[i].digest_size + 1;
        test_vector[i].hmac(key, j, message, sizeof(message)-1,
                mac, test_vector[i].digest_size);
        ABTS_TRUE(tc, memcmp(mac, ogs_hex_from_string(
                        test_vector[i].digest_plus_one_key_mac,
                        expected, sizeof(expected)),
                    test_vector[i].digest_size) == 0);

        j = test_vector[i].block_size;
        test_vector[i].hmac(key, j, message, sizeof(message)-1,
                mac, test_vector[i].digest_size);
        ABTS_TRUE(tc, memcmp(mac, ogs_hex_from_string(
                        test_vector[i].block_size_key_mac,
                        expected, sizeof(expected)),
                    test_vector[i].digest_size) == 0);
    }
}

static void security_test11(abts_case *tc, void *data)
{
    /* TS 35.217 UIA2 sets 1-5 and TS 33.401 Annex C.4 EIA1 sets 2-6. */
    static const struct {
        const char *name;
        uint32_t count;
        uint32_t fresh;
        uint32_t direction;
        uint32_t bit_len;
        const char *ik;
        const char *message;
        const char *mac;
    } vectors[] = {
        {
            "UIA2 test set 1",
            0x38a6f056, 0x05d2ec49, 0, 189,
            "2bd6459f82c5b300952c49104881ff48",
            "6b227737296f393c8079353edc87e2e805d2ec49a4f2d8e0",
            "2bce1820",
        },
        {
            "UIA2 test set 2",
            0x3edc87e2, 0xa4f2d8e2, 1, 254,
            "d42f682428201cafcd9f97945e6de7b7",
            "b5924384328a4ae00b737109f8b6c8dd2b4db63dd533981ceb19aad52a5b2bc0",
            "fc7b18bd",
        },
        {
            "UIA2 test set 3",
            0x36af6144, 0x9838f03a, 1, 319,
            "fdb9cfdf28936cc483a31869d81b8fab",
            "5932bc0ace2b0aba33d8ac188ac54f346fad10bf9dee2920b43bd0c53a915cb7"
            "df6caa72053abff2",
            "02f1faaf",
        },
        {
            "UIA2 test set 4",
            0x14793e41, 0x0397e8fd, 1, 384,
            "c736c6aab22bfff91e2698d2e22ad57e",
            "d0a7d463df9fb2b278833fa02e235aa172bd970c1473e12907fb648b6599aaa0"
            "b24a038665422b20a499276a50427009",
            "38b554c0",
        },
        {
            "UIA2 test set 5",
            0x296f393c, 0x6b227737, 1, 1000,
            "f4ebec69e73eaf2eb2cf6af4b3120ffd",
            "10bfff839e0c71658dbb2d1707e145724f41c16f48bf403c3b18e38fd5d1663b"
            "6f6d900193e3cea8bb4f1b4f5be822032232a78d7d75238d5e6daecd3b4322cf"
            "59bc7ea84ab18811b5bfb7bc553f4fe44478ce287a14879990d18d12ca79d2c8"
            "55149021cd5ce8ca0371ca04fcce143e3d7cfee94585b5885cac46068b",
            "061745ae",
        },
        {
            "128-EIA1 test set 2",
            0x36af6144, 0xc0000000, 1, 254,
            "7e5e94431e11d73828d739cc6ced4573",
            "b3d3c9170a4e1632f60f861013d22d84b726b6a278d802d1eeaf1321ba5929dc",
            "e3259f6f",
        },
        {
            "128-EIA1 test set 3",
            0xc7590ea9, 0xb8000000, 0, 511,
            "d3419be821087acd02123a9248033359",
            "bbb057038809496bcff86d6fbc8ce5b135a06b166054f2d565be8ace75dc851e"
            "0bcdd8f07141c495872fb5d8c0c66a8b6da556663e4e461205d84580bee5bc7e",
            "9a16c77d",
        },
        {
            "128-EIA1 test set 4",
            0x36af6144, 0x78000000, 1, 768,
            "83fd23a244a74cf358da3019f1722635",
            "35c68716633c66fb750c266865d53c11ea05b1e9fa49c8398d48e1efa5909d39"
            "47902837f5ae96d5a05bc8d61ca8dbef1b13a4b4abfe4fb1006045b674bb5472"
            "9304c382be53a5af05556176f6eaa2ef1d05e4b083181ee674cda5a485f74d7a",
            "bba74492",
        },
        {
            "128-EIA1 test set 5",
            0x36af6144, 0xc0000000, 0, 383,
            "6832a65cff4473621ebdd4ba26a921fe",
            "d3c53839626820717765667620323837636240981ba6824c1bfb1ab485472029"
            "b71d808ce33e2cc3c0b5fc1f3de8a6dc",
            "4145e4b0",
        },
        {
            "128-EIA1 test set 6",
            0x7827fab2, 0x28000000, 1, 2558,
            "5d0a80d8134ae19677824b671e838af4",
            "70dedf2dc42c5cbd3a96f8a0b11418b3608d5733604a2cd36aabc70ce3193bb5"
            "153be2d3c06dfdb2d16e9c357158be6a41d6b861e491db3fbfeb518efcf048d7"
            "d58953730ff30c9ec470ffcd663dc34201c36addc0111c35b38afee7cfdb582e"
            "3731f8b4baa8d1a89c06e81199a9716227be344efcb436ddd0f096c064c3b5e2"
            "c399993fc77394f9e09720a811850ef23b2ee05d9e6173609d86e1c0c18ea51a"
            "012a00bb413b9cb8188a703cd6bae31cc67b34b1b00019e6a2b2a690f02671fe"
            "7c9ef8dec0094e533763478d58d2c5f5b827a0148c5948a96931acf84f465a64"
            "e62ce74007e991e37ea823fa0fb21923b79905b733b631e6c7d6860a3831ac35"
            "1a9c730c52ff72d9d308eedbab21fde143a0ea17e23edc1f74cbb3638a2033aa"
            "a15464eaa733385dbbeb6fd73509b857e6a419dca1d8907af977fbac4dfa35ec",
            "0fa2b1ee",
        },
    };
    uint8_t ik[16], message[320], mac[4], expected[4];
    size_t i;

    for (i = 0; i < OGS_ARRAY_SIZE(vectors); i++) {
        size_t bytes = (vectors[i].bit_len + 7) / 8;
        unsigned int tail_bits = vectors[i].bit_len % 8;

        ogs_hex_from_string(vectors[i].ik, ik, sizeof(ik));
        ogs_hex_from_string(vectors[i].message, message, bytes);
        ogs_hex_from_string(vectors[i].mac, expected, sizeof(expected));

        snow_3g_f9(ik, vectors[i].count, vectors[i].fresh,
                vectors[i].direction, message, vectors[i].bit_len, mac);
        ABTS_ASSERT(tc, vectors[i].name,
                memcmp(mac, expected, sizeof(mac)) == 0);

        if (tail_bits) {
            /* Unused low bits of the last byte must not affect the MAC. */
            message[bytes - 1] ^= (1u << (8 - tail_bits)) - 1;
            snow_3g_f9(ik, vectors[i].count, vectors[i].fresh,
                    vectors[i].direction, message, vectors[i].bit_len, mac);
            ABTS_ASSERT(tc, vectors[i].name,
                    memcmp(mac, expected, sizeof(mac)) == 0);
        }
    }
}

abts_suite *test_security(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, security_test1, NULL);
    abts_run_test(suite, security_test2, NULL);
    abts_run_test(suite, security_test3, NULL);
    abts_run_test(suite, security_test4, NULL);
    abts_run_test(suite, security_test5, NULL);
    abts_run_test(suite, security_test6, NULL);
    abts_run_test(suite, security_test7, NULL);
    abts_run_test(suite, security_test8, NULL);
    abts_run_test(suite, security_test9, NULL);
    abts_run_test(suite, security_test10, NULL);
    abts_run_test(suite, security_test11, NULL);

    return suite;
}
