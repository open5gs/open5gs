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

#if 0 /* Issue #2581 : snow_3g_f8 have memory problem */
    snow_3g_f8(
        ogs_hex_from_string(_ck, ck, sizeof(ck)),
        0x72a4f20f, 0x0c, 1,
        ogs_hex_from_string(_plain, plain, sizeof(plain)),
        SECURITY_TEST5_BIT_LEN);
#else
    SNOW_CTX ctx;

    ogs_hex_from_string(_plain, plain, sizeof(plain));
    SNOW_init(0x72a4f20f, 0x0c, 1,
        ogs_hex_from_string(_ck, ck, sizeof(ck)), &ctx);
    SNOW(SECURITY_TEST5_LEN, plain, plain, &ctx);
#endif
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

    return suite;
}
