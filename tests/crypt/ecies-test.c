/*
 * Copyright (C) 2022 by Sukchan Lee <acetcom@gmail.com>
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

static void ecies_profile_a(abts_case *tc, void *data)
{
    const char *_e[] = {
        "c53c22208b61860b06c62e5406a7b330c2b577aa5558981510d128247d38bd1d",
        "c80949f13ebe61af4ebdbd293ea4f942696b9e815d7e8f0096bbf6ed7de62256",
        "C53C22208B61860B06C62E5406A7B330C2B577AA5558981510D128247D38BD1D",
        "BE9EFF3E9F22A4B42A3D236E7A6C500B3F2E7E0C7449988BA800D664BF4FCD97",

        /* Private Key in curve25519-1.key */
        "10c9c67e861a5625a1db8f684123896d9b3506199d3df1968e07b6c8448bb147",
        /* Private Key in TS33.501 C4.3.1 */
        "c80949f13ebe61af4ebdbd293ea4f942696b9e815d7e8f0096bbf6ed7de62256",

    };
    const char *_k[] = {
        "b2e92f836055a255837debf850b528997ce0201cb82adfe4be1f587d07d8457d",
        "5a8d38864820197c3394b92613b20b91633cbd897119273bf8e4a6f4eec0a650",
        "977D8B2FDAA7B64AA700D04227D5B440630EA4EC50F9082273A26BB678C92222",
        "5A8D38864820197C3394B92613B20B91633CBD897119273BF8e4A6f4EEC0A650",

        /* Public Key in TS33.501 C4.3.1 */
        "b2e92f836055a255837debf850b528997ce0201cb82adfe4be1f587d07d8457d",
        /* Public Key in curve25519-1.key */
        "e421686f6fb2d70e3fa28d940494095686c3179fef53514667a6ed106b8a7d3d",

    };
    const char *_ek[] = {
        "028ddf890ec83cdf163947ce45f6ec1a0e3070ea5fe57e2b1f05139f3e82422a",
        "028ddf890ec83cdf163947ce45f6ec1a0e3070ea5fe57e2b1f05139f3e82422a",
        "511C1DF473BB88317F923501F8BA944FD3B667D25699DCB552DBCEF60BBDC56D",
        "511C1DF473BB88317F923501F8BA944FD3B667D25699DCB552DBCEF60BBDC56D",

        /* Share Key from curve25519-1.key + TS33.501 C4.3.1 */
        "514bacfdc28039187eec8196339d3ef2665691cd13abcc2a5df15561d9348c60",
        /* Share Key from curve25519-1.key + TS33.501 C4.3.1 */
        "514bacfdc28039187eec8196339d3ef2665691cd13abcc2a5df15561d9348c60",
    };
    int i, num;

    uint8_t e[OGS_ECCKEY_LEN];
    uint8_t k[OGS_ECCKEY_LEN];
    uint8_t ek[OGS_ECCKEY_LEN];
    uint8_t tmp[OGS_ECCKEY_LEN];

    num = 6;
    for (i = 0; i < num; i++) {
        curve25519_donna(ek,
            ogs_hex_from_string(_e[i], e, sizeof(e)),
            ogs_hex_from_string(_k[i], k, sizeof(k)));

        ABTS_TRUE(tc, memcmp(ek,
            ogs_hex_from_string(_ek[i], tmp, sizeof(tmp)), OGS_ECCKEY_LEN) == 0);
    }
}

static void ecies_profile_b(abts_case *tc, void *data)
{
    const char *_e[] = {
        "F1AB1074477EBCC7F554EA1C5FC368B1616730155E0041AC447D6301975FECDA",
        "99798858A1DC6A2C68637149A4B1DBFD1FDFF5ADDD62A2142F06699ED7602529",
        "F1AB1074477EBCC7F554EA1C5FC368B1616730155E0041AC447D6301975FECDA",
        "90A5898BD29FFA3F261E00E980067C70A2B1B992A21F5B4FEF6D4DF69FE804AD",

        /* Private Key in secp256r1-2.key */
        "74a9f918471f56f3befda5c51d738a3f94f5a52d4bc9db9799f5225fbccdde41",
        /* Private Key in TS33.501 C4.4.1 */
        "99798858A1DC6A2C68637149A4B1DBFD1FDFF5ADDD62A2142F06699ED7602529",
    };
    const char *_k[] = {
        "039AAB8376597021E855679A9778EA0B67396E68C66DF32C0F41E9ACCA2DA9B9D1",
        "0272DA71976234CE833A6907425867B82E074D44EF907DFB4B3E21C1C2256EBCD1",
        "03759BB22C563D9F4A6B3C1419E543FC2F39D6823F02A9D71162B39399218B244B",
        "0272DA71976234CE833A6907425867B82E074D44EF907DFB4B3E21C1C2256EBCD1",

        /* Public Key in TS33.501 C4.4.1 */
        "039AAB8376597021E855679A9778EA0B67396E68C66DF32C0F41E9ACCA2DA9B9D1",
        /* Public Key in secp256r1-2.key */
        "03adefcd1317d1ce8562ec25b91b4800120e1236d6e2661ea4235a84e3c85da244",
    };
    const char *_ek[] = {
        "6C7E6518980025B982FBB2FF746E3C2E85A196D252099A7AD23EA7B4C0959CAE",
        "6C7E6518980025B982FBB2FF746E3C2E85A196D252099A7AD23EA7B4C0959CAE",
        "BC3529ED79541CF8C007CE9806330F4A5FF15064D7CF4B16943EF8F007597872",
        "BC3529ED79541CF8C007CE9806330F4A5FF15064D7CF4B16943EF8F007597872",

        /* Share Key from secp256r1-2.key + TS33.501 C4.4.1 */
        "4072b8e3989b8695b35b99c923069508726fa3e65551aca1ec9560c501c190ce",
        /* Share Key from secp256r1-2.key + TS33.501 C4.4.1 */
        "4072b8e3989b8695b35b99c923069508726fa3e65551aca1ec9560c501c190ce",
    };

    uint8_t e[OGS_ECCKEY_LEN];
    uint8_t k[OGS_ECCKEY_LEN+1];
    uint8_t ek[OGS_ECCKEY_LEN];
    uint8_t tmp[OGS_ECCKEY_LEN];

    int i, r, num;

    num = 6;
    for (i = 0; i < num; i++) {
        r = ecdh_shared_secret(
                ogs_hex_from_string(_k[i], k, sizeof(k)),
                ogs_hex_from_string(_e[i], e, sizeof(e)),
                ek);
        ABTS_INT_EQUAL(tc, 1, r);

        ABTS_TRUE(tc, memcmp(ek,
            ogs_hex_from_string(_ek[i], tmp, sizeof(tmp)), OGS_ECCKEY_LEN) == 0);
    }
}

static void ansi_x963_kdf(abts_case *tc, void *data)
{
    const char *_z[] = {
        "028ddf890ec83cdf163947ce45f6ec1a0e3070ea5fe57e2b1f05139f3e82422a",
        "511C1DF473BB88317F923501F8BA944FD3B667D25699DCB552DBCEF60BBDC56D",
        "6C7E6518980025B982FBB2FF746E3C2E85A196D252099A7AD23EA7B4C0959CAE",
        "BC3529ED79541CF8C007CE9806330F4A5FF15064D7CF4B16943EF8F007597872",

        /* Share Key from curve25519-1.key + TS33.501 C4.3.1 */
        "514bacfdc28039187eec8196339d3ef2665691cd13abcc2a5df15561d9348c60",
        /* Share Key from secp256r1-2.key + TS33.501 C4.4.1 */
        "4072b8e3989b8695b35b99c923069508726fa3e65551aca1ec9560c501c190ce",
    };
    const char *_info[] = {
        "b2e92f836055a255837debf850b528997ce0201cb82adfe4be1f587d07d8457d",
        "977D8B2FDAA7B64AA700D04227D5B440630EA4EC50F9082273A26BB678C92222",
        "039AAB8376597021E855679A9778EA0B67396E68C66DF32C0F41E9ACCA2DA9B9D1",
        "03759BB22C563D9F4A6B3C1419E543FC2F39D6823F02A9D71162B39399218B244B",

        /* Public Key in TS33.501 C4.3.1 */
        "b2e92f836055a255837debf850b528997ce0201cb82adfe4be1f587d07d8457d",
        /* Public Key in TS33.501 C4.4.1 */
        "039AAB8376597021E855679A9778EA0B67396E68C66DF32C0F41E9ACCA2DA9B9D1",
    };
    int info_len[] = {
        32,
        32,
        33,
        33,

        32,
        33,
    };
    const char *_ek[] = {
        "2ba342cabd2b3b1e5e4e890da11b65f6",
        "FE77B87D87F40428EDD71BCA69D79059",
        "8A65C3AED80295C12BD55087E965702A",
        "84F9A78995D39E6968047547ECC12C4F",

        /* Enc Key from curve25519-1.key + TS33.501 C4.3.1 */
        "6a898587f494ed58a1185b01207b6710",
        /* Enc Key from secp256r1-2.key + TS33.501 C4.4.1 */
        "a950bf5f8922b395f9b6bcf8ddfa48c2",
    };
    const char *_icb[] = {
        "e2622cb0cdd08204e721c8ea9b95a7c6",
        "cafa5287de2b20e3df1bd3a858da00ac",
        "EF285B4061C3BAEE858AB6EC68487DAE",
        "a1a6b7f89d422a733675996ebe781eb7",

        /* ICB from curve25519-1.key + TS33.501 C4.3.1 */
        "ebfc734de36bf03d5e94b9f9db800b32",
        /* ICB from secp256r1-2.key + TS33.501 C4.4.1 */
        "ff0b5323557bd5e2b1da731cd6ca1416",
    };
    const char *_mk[] = {
        "d9846966fb7cf5fcf11266c5957dea60b83fff2b7c940690a4bfe57b1eb52bd2",
        "D87B69F4FE8CD6B211264EA5E69F682F151A82252684CDB15A047E6EF0595028",
        "A5EBAC0BC48D9CF7AE5CE39CD840AC6C761AEC04078FAB954D634F923E901C64",
        "39D5517E965F8E1252B61345ED45226C5F1A8C69F03D6C91437591F0B8E48FA0",

        /* MAC Key from curve25519-1.key + TS33.501 C4.3.1 */
        "19045a963a79b4e397f2b81eb0258cbc0a64c9102aaf9e936d5c9f6d44fd8434",
        /* MAC Key from secp256r1-2.key + TS33.501 C4.4.1 */
        "96f5757bcdb3f4c33976afc96cf7883d3dd017feb3ac229fd813d535e23ed58f",
    };

    uint8_t z[OGS_ECCKEY_LEN];
    uint8_t info[OGS_ECCKEY_LEN+1];

    uint8_t ek[OGS_KEY_LEN];
    uint8_t icb[OGS_IVEC_LEN];
    uint8_t mk[OGS_SHA256_DIGEST_SIZE];

    uint8_t tmp[OGS_SHA256_DIGEST_SIZE];

    int i, num;

    num = 6;
    for (i = 0; i < num; i++) {
        ogs_kdf_ansi_x963(
            ogs_hex_from_string(_z[i], z, sizeof(z)), OGS_ECCKEY_LEN,
            ogs_hex_from_string(_info[i], info, sizeof(info)), info_len[i],
            ek, icb, mk);
        ABTS_TRUE(tc, memcmp(ek,
            ogs_hex_from_string(_ek[i], tmp, sizeof(tmp)), OGS_KEY_LEN) == 0);
        ABTS_TRUE(tc, memcmp(icb,
            ogs_hex_from_string(_icb[i], tmp, sizeof(tmp)), OGS_IVEC_LEN) == 0);
        ABTS_TRUE(tc, memcmp(mk,
            ogs_hex_from_string(_mk[i], tmp, sizeof(tmp)), OGS_SHA256_DIGEST_SIZE) == 0);
    }
}

static void aes_128ctr(abts_case *tc, void *data)
{
    const char *_ek[] = {
        "2ba342cabd2b3b1e5e4e890da11b65f6",
        "FE77B87D87F40428EDD71BCA69D79059",
        "8A65C3AED80295C12BD55087E965702A",
        "84F9A78995D39E6968047547ECC12C4F",

        /* Enc Key from curve25519-1.key + TS33.501 C4.3.1 */
        "6a898587f494ed58a1185b01207b6710",
        /* Enc Key from secp256r1-2.key + TS33.501 C4.4.1 */
        "a950bf5f8922b395f9b6bcf8ddfa48c2",
    };
    const char *_icb[] = {
        "e2622cb0cdd08204e721c8ea9b95a7c6",
        "cafa5287de2b20e3df1bd3a858da00ac",
        "EF285B4061C3BAEE858AB6EC68487DAE",
        "a1a6b7f89d422a733675996ebe781eb7",

        /* ICB from curve25519-1.key + TS33.501 C4.3.1 */
        "ebfc734de36bf03d5e94b9f9db800b32",
        /* ICB from secp256r1-2.key + TS33.501 C4.4.1 */
        "ff0b5323557bd5e2b1da731cd6ca1416",
    };
    const char *_plain[] = {
        "00012080f6",
        "766572796C6F6E67757365726E616D6531",
        "00012080f6",
        "766572796C6F6E67757365726E616D6531",

        /* Plain-text from curve25519-1.key + TS33.501 C4.3.1 */
        "00012080f6",
        /* Plain-text from secp256r1-2.key + TS33.501 C4.4.1 */
        "00012080f6",
    };
    const char *_cipher[] = {
        "cb02352410",
        "8E358A1582ADB15322C10E515141D2039A",
        "46A33FC271",
        "BE22D8B9F856A52ED381CD7EAF4CF2D525",

        /* Ciphered-text from curve25519-1.key + TS33.501 C4.3.1 */
        "7ee4435e19",
        /* Ciphered-text from secp256r1-2.key + TS33.501 C4.4.1 */
        "cdd22e3496",
    };
    int len[] = {
        5,
        17,
        5,
        17,

        5,
        5,
    };

    uint8_t ek[OGS_KEY_LEN];
    uint8_t icb[OGS_IVEC_LEN];
    uint8_t plain[17];
    uint8_t cipher[17];
    uint8_t tmp[17];

    int i, num;

    num = 6;
    for (i = 0; i < num; i++) {
        ogs_aes_ctr128_encrypt(
            ogs_hex_from_string(_ek[i], ek, sizeof(ek)),
            ogs_hex_from_string(_icb[i], icb, sizeof(icb)),
            ogs_hex_from_string(_plain[i], plain, sizeof(plain)), len[i],
            cipher);
        ABTS_TRUE(tc, memcmp(cipher,
            ogs_hex_from_string(_cipher[i], tmp, sizeof(tmp)), len[i]) == 0);
        ogs_aes_ctr128_encrypt(
            ogs_hex_from_string(_ek[i], ek, sizeof(ek)),
            ogs_hex_from_string(_icb[i], icb, sizeof(icb)),
            ogs_hex_from_string(_cipher[i], cipher, sizeof(cipher)), len[i],
            plain);
        ABTS_TRUE(tc, memcmp(plain,
            ogs_hex_from_string(_plain[i], tmp, sizeof(tmp)), len[i]) == 0);
    }
}

static void hmac_sha_256(abts_case *tc, void *data)
{
    const char *_mk[] = {
        "d9846966fb7cf5fcf11266c5957dea60b83fff2b7c940690a4bfe57b1eb52bd2",
        "D87B69F4FE8CD6B211264EA5E69F682F151A82252684CDB15A047E6EF0595028",
        "A5EBAC0BC48D9CF7AE5CE39CD840AC6C761AEC04078FAB954D634F923E901C64",
        "39D5517E965F8E1252B61345ED45226C5F1A8C69F03D6C91437591F0B8E48FA0",

        /* MAC Key from curve25519-1.key + TS33.501 C4.3.1 */
        "19045a963a79b4e397f2b81eb0258cbc0a64c9102aaf9e936d5c9f6d44fd8434",
        /* MAC Key from secp256r1-2.key + TS33.501 C4.4.1 */
        "96f5757bcdb3f4c33976afc96cf7883d3dd017feb3ac229fd813d535e23ed58f",
    };
    const char *_cipher[] = {
        "cb02352410",
        "8E358A1582ADB15322C10E515141D2039A",
        "46A33FC271",
        "BE22D8B9F856A52ED381CD7EAF4CF2D525",

        /* Ciphered-text from curve25519-1.key + TS33.501 C4.3.1 */
        "7ee4435e19",
        /* Ciphered-text from secp256r1-2.key + TS33.501 C4.4.1 */
        "cdd22e3496",
    };
    const char *_mac_tag[] = {
        "cddd9e730ef3fa87",
        "12E1D7783A97F1AC",
        "6AC7DAE96AA30A4D",
        "3CDDC61A0A7882EB",

        /* MAG-tag value from curve25519-1.key + TS33.501 C4.3.1 */
        "78dc897bff24129c",
        /* MAC-tag value secp256r1-2.key + TS33.501 C4.4.1 */
        "5500e9242e65f58c",
    };
    int len[] = {
        5,
        17,
        5,
        17,

        5,
        5,
    };

    uint8_t mk[OGS_SHA256_DIGEST_SIZE];
    uint8_t cipher[17];
    uint8_t mac_tag[OGS_MACTAG_LEN];
    uint8_t tmp[OGS_MACTAG_LEN];

    int i, num;

    num = 6;
    for (i = 0; i < num; i++) {
        ogs_hmac_sha256(
            ogs_hex_from_string(_mk[i], mk, sizeof(mk)), OGS_SHA256_DIGEST_SIZE,
            ogs_hex_from_string(_cipher[i], cipher, sizeof(cipher)), len[i],
            mac_tag, OGS_MACTAG_LEN);
        ABTS_TRUE(tc, memcmp(mac_tag,
            ogs_hex_from_string(_mac_tag[i], tmp, sizeof(tmp)),
            OGS_MACTAG_LEN) == 0);
    }
}

abts_suite *test_ecies(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, ecies_profile_a, NULL);
    abts_run_test(suite, ecies_profile_b, NULL);
    abts_run_test(suite, ansi_x963_kdf, NULL);
    abts_run_test(suite, aes_128ctr, NULL);
    abts_run_test(suite, hmac_sha_256, NULL);

    return suite;
}
