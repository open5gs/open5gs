#include "core_lib.h"
#include "core_debug.h"
#include "core_sha2_hmac.h"
#include "core_aes_cmac.h"
#include "core_pkbuf.h"

#include "milenage.h"
#include "hss/kdf.h"
#include "snow_3g.h"
#include "zuc.h"

#include "testutil.h"

static void security_test1(abts_case *tc, void *data)
{
    char *_k = "465b5ce8 b199b49f aa5f0a2e e238a6bc";
    char *_rand = "23553cbe 9637a89d 218ae64d ae47bf35";
    char *_sqn = "ff9bb4d0 b607";
    char *_amf = "b9b9";
    char *_op = "cdc202d5 123e20f6 2b6d676a c72cb318";
    char *_opc =    "cd63cb71 954a9f4e 48a5994e 37a02baf";
    char *_mac_a = "4a9ffac3 54dfafb3";
    char *_mac_s = "01cfaf9e c4e871e9";
    char *_res = "a54211d5 e3ba50bf";
    char *_ck = "b40ba9a3 c58b2a05 bbf0d987 b21bf8cb";
    char *_ik = "f769bcd7 51044604 12767271 1c6d3441";
    char *_ak = "aa689c64 8370";
    char *_akstar = "451e8bec a43b";

    c_uint8_t k[16];
    c_uint8_t rand[16];
    c_uint8_t op[16];
    c_uint8_t opc[16];
    c_uint8_t sqn[6];
    c_uint8_t amf[2];
    c_uint8_t mac_a[8];
    c_uint8_t mac_s[8];
    c_uint8_t res[8];
    c_uint8_t ck[16];
    c_uint8_t ik[16];
    c_uint8_t ak[6];
    c_uint8_t akstar[6];

    c_uint8_t tmp[16];

    milenage_opc(
        core_ascii_to_hex(_k, strlen(_k), k, sizeof(k)), 
        core_ascii_to_hex(_op, strlen(_op), op, sizeof(op)),
        opc);
    ABTS_TRUE(tc, memcmp(opc, 
        core_ascii_to_hex(_opc, strlen(_opc), tmp, 16), 16) == 0);

    milenage_f1(opc, k, 
        core_ascii_to_hex(_rand, strlen(_rand), rand, sizeof(rand)),
        core_ascii_to_hex(_sqn, strlen(_sqn), sqn, sizeof(sqn)),
        core_ascii_to_hex(_amf, strlen(_amf), amf, sizeof(amf)),
        mac_a, mac_s);
    ABTS_TRUE(tc, memcmp(mac_a, 
        core_ascii_to_hex(_mac_a, strlen(_mac_a), tmp, 8), 8) == 0);
    ABTS_TRUE(tc, memcmp(mac_s, 
        core_ascii_to_hex(_mac_s, strlen(_mac_s), tmp, 8), 8) == 0);

    milenage_f2345(opc, k, rand, res, ck, ik, ak, akstar);
    ABTS_TRUE(tc, memcmp(res, 
        core_ascii_to_hex(_res, strlen(_res), tmp, 8), 8) == 0);
    ABTS_TRUE(tc, memcmp(ck, 
        core_ascii_to_hex(_ck, strlen(_ck), tmp, 16), 16) == 0);
    ABTS_TRUE(tc, memcmp(ik, 
        core_ascii_to_hex(_ik, strlen(_ik), tmp, 16), 16) == 0);
    ABTS_TRUE(tc, memcmp(ak, 
        core_ascii_to_hex(_ak, strlen(_ak), tmp, 6), 6) == 0);
    ABTS_TRUE(tc, memcmp(akstar, 
        core_ascii_to_hex(_akstar, strlen(_akstar), tmp, 6), 6) == 0);
}

static void security_test2(abts_case *tc, void *data)
{
    char *_key = "4a656665";
    char *_message = "7768617420646f2079612077616e7420666f72206e6f7468696e673f";
    char *_hmac = "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843";

    c_uint8_t key[4];
    c_uint8_t message[28];
    c_uint8_t hmac[32];
    c_uint8_t tmp[32];

    hmac_sha256(
        core_ascii_to_hex(_key, strlen(_key), key, sizeof(key)),
        4,
        core_ascii_to_hex(_message, strlen(_message), message, sizeof(message)),
        28,
        hmac, 32);

    ABTS_TRUE(tc, memcmp(hmac, 
        core_ascii_to_hex(_hmac, strlen(_hmac), tmp, 32), 32) == 0);
}

static void security_test3(abts_case *tc, void *data)
{
    char *_ck = "B40BA9A3C58B2A05BBF0D987B21BF8CB";
    char *_ik = "F769BCD751044604127672711C6D3441";
    char *_plmn_id = "024830";
    char *_sqn = "FD8EEF40DF7D";
    char *_ak = "AA689C648370";
    char *_kasme = "238E457E0F758BADBCA8D34BB2612C10428D426757CB5553B2B184FA64BFC549";
    c_uint8_t ck[16];
    c_uint8_t ik[16];
    c_uint8_t plmn_id[3];
    c_uint8_t sqn[6];
    c_uint8_t ak[6];
    c_uint8_t kasme[32];
    c_uint8_t tmp[32];

    hss_kdf_kasme(
        core_ascii_to_hex(_ck, strlen(_ck), ck, sizeof(ck)),
        core_ascii_to_hex(_ik, strlen(_ik), ik, sizeof(ik)),
        core_ascii_to_hex(_plmn_id, strlen(_plmn_id), plmn_id, sizeof(plmn_id)),
        core_ascii_to_hex(_sqn, strlen(_sqn), sqn, sizeof(sqn)),
        core_ascii_to_hex(_ak, strlen(_ak), ak, sizeof(ak)),
        kasme);

    ABTS_TRUE(tc, memcmp(kasme, 
        core_ascii_to_hex(_kasme, strlen(_kasme), tmp, 32), 32) == 0);
}

static void security_test4(abts_case *tc, void *data)
{
    char *_ik = "2bd6459f 82c5b300 952c4910 4881ff48";
    char *_message = "33323462 63393861 37347900 00000000";
    char *_mact = "731f1165";
    c_uint8_t ik[16];
    c_uint8_t message[88/8];
    c_uint8_t mact[4];
    c_uint8_t tmp[4];

    snow_3g_f9(core_ascii_to_hex(_ik, strlen(_ik), ik, sizeof(ik)),
        0x38a6f056, (0x1f << 27), 0, 
        core_ascii_to_hex(_message, strlen(_message), message, sizeof(message)),
        88, mact);
    ABTS_TRUE(tc, memcmp(mact, 
        core_ascii_to_hex(_mact, strlen(_mact), tmp, 4), 4) == 0);
}

static void security_test5(abts_case *tc, void *data)
{
    char *_ck = "2bd6459f 82c5b300 952c4910 4881ff48";
    char *_message = 
        "7ec61272 743bf161 4726446a 6c38ced1"
        "66f6ca76 eb543004 4286346c ef130f92"
        "922b0345 0d3a9975 e5bd2ea0 eb55ad8e"
        "1b199e3e c4316020 e9a1b285 e7627953"
        "59b7bdfd 39bef4b2 484583d5 afe082ae"
        "e638bf5f d5a60619 3901a08f 4ab41aab"
        "9b134880";
    char *_cmessage = 
        "8ceba629 43dced3a 0990b06e a1b0a2c4"
        "fb3cedc7 1b369f42 ba64c1eb 6665e72a"
        "a1c9bb0d eaa20fe8 6058b8ba ee2c2e7f"
        "0becce48 b52932a5 3c9d5f93 1a3a7c53"
        "2259af43 25e2a65e 3084ad5f 6a513b7b"
        "ddc1b65f 0aa0d97a 053db55a 88c4c4f9"
        "605e4140";
    c_uint8_t ck[16];
    c_uint8_t message[798/8+1];
    c_uint8_t tmp[798/8+1];

    snow_3g_f8(
        core_ascii_to_hex(_ck, strlen(_ck), ck, sizeof(ck)),
        0x72a4f20f, 0x0c, 1,
        core_ascii_to_hex(_message, strlen(_message), message, sizeof(message)),
        798);
    ABTS_TRUE(tc, memcmp(message, core_ascii_to_hex(
            _cmessage, strlen(_cmessage), tmp, 798/8+1), 798/8+1) == 0);
}

static void security_test6(abts_case *tc, void *data)
{
#if 0 /* Not supported */
    char *_ik = "2bd6459f 82c5b300 952c4910 4881ff48";
    char *_message = "33323462 63393840";
    char *_mact = "118c6eb8";
    c_uint8_t ik[16];
    c_uint8_t message[58/8+1];
    c_uint8_t mact[16];
    c_uint8_t tmp[4];
    c_uint8_t *m = NULL;
    c_uint32_t count = htonl(0x38a6f056);
    int msg_len = 58/8+1;
    int m_len = 8+msg_len;

    m = core_calloc(m_len, sizeof(c_uint8_t));
    memcpy(m, &count, sizeof(c_uint32_t));
    m[4] = ((0x18 << 3) | (0 << 2));
    memcpy(m+8, 
        core_ascii_to_hex(_message, strlen(_message), message, sizeof(message)),
        msg_len);

    aes_cmac_calculate(mact, 
            core_ascii_to_hex(_ik, strlen(_ik), ik, sizeof(ik)), m, m_len);
    d_print_hex(mact, 4);

    core_free(m);

    ABTS_TRUE(tc, memcmp(mact, 
        core_ascii_to_hex(_mact, strlen(_mact), tmp, 4), 4) == 0);
#endif
}

static void security_test7(abts_case *tc, void *data)
{
    char *_ik = "d3c5d592 327fb11c 4035c668 0af8c6d1";
    char *_message = "484583d5 afe082ae";
    char *_mact = "b93787e6";
    c_uint8_t ik[16];
    c_uint8_t message[64/8];
    c_uint8_t mact[16];
    c_uint8_t tmp[4];
    c_uint8_t *m = NULL;
    c_uint32_t count = htonl(0x398a59b4);
    int msg_len = 64/8;
    int m_len = 8+msg_len;

    m = core_calloc(m_len, sizeof(c_uint8_t));
    memcpy(m, &count, sizeof(c_uint32_t));
    m[4] = ((0x1a << 3) | (1 << 2));
    memcpy(m+8, 
        core_ascii_to_hex(_message, strlen(_message), message, sizeof(message)),
        msg_len);

    aes_cmac_calculate(mact, 
            core_ascii_to_hex(_ik, strlen(_ik), ik, sizeof(ik)), m, m_len);

    core_free(m);

    ABTS_TRUE(tc, memcmp(mact, 
        core_ascii_to_hex(_mact, strlen(_mact), tmp, 4), 4) == 0);
}

static void security_test8(abts_case *tc, void *data)
{
    char *_ik = "c9 e6 ce c4 60 7c 72 db 00 0a ef a8 83 85 ab 0a";
    char *_message = 
    "983b41d4 7d780c9e 1ad11d7e b70391b1 de0b35da 2dc62f83 e7b78d63 06ca0ea0"
    "7e941b7b e91348f9 fcb170e2 217fecd9 7f9f68ad b16e5d7d 21e569d2 80ed775c"
    "ebde3f40 93c53881 00000000";
    char *_mact = "fae8ff0b";
    c_uint8_t ik[16];
    c_uint8_t message[577/8+1];
    c_uint8_t mact[4];
    c_uint32_t mac;

    zuc_eia3(
        core_ascii_to_hex(_ik, strlen(_ik), ik, sizeof(ik)),
        0xa94059da,
        0xa,
        1,
        577,
        core_ascii_to_hex(_message, strlen(_message), message, sizeof(message)),
        &mac);
    mac = ntohl(mac);

    ABTS_TRUE(tc, memcmp(&mac, 
        core_ascii_to_hex(_mact, strlen(_mact), mact, 4), 4) == 0);
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

    return suite;
}
