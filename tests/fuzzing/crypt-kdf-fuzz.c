/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
 *
 * This file is part of Open5GS.
 *
 * Threat model: air-interface and inter-NF material (RAND, AUTN, SQN, SUPI,
 * serving-network name, NAS COUNT, SUCI keys, base64) reaches the in-tree
 * AKA/NAS crypto. lib/crypt is 0-covered in the 20261114 report. The harness
 * clamps only API preconditions (keybits, non-empty SNN, "imsi-" SUPI,
 * ANSI-X9.63 length) so ogs_assert() is not a fuzzer fault.
 */

#include <stdint.h>
#include <string.h>

#include "fuzzing.h"
#include "ogs-crypt.h"

#define kMinInputLength 48
#define kMaxInputLength 2048

static const uint8_t *take(const uint8_t **p, const uint8_t *end, size_t n)
{
    const uint8_t *cur;
    if ((size_t)(end - *p) < n)
        return NULL;
    cur = *p;
    *p += n;
    return cur;
}

static uint8_t u8at(const uint8_t **p, const uint8_t *end, uint8_t fb)
{
    const uint8_t *b = take(p, end, 1);
    return b ? *b : fb;
}

static void fill_str(char *dst, size_t cap, const uint8_t *src, size_t n)
{
    size_t i;
    if (n >= cap)
        n = cap - 1;
    /* n==0 must not read src: take(0) returns the end pointer. */
    if (n == 0 || src == NULL) {
        dst[0] = 'a';
        dst[1] = '\0';
        return;
    }
    memcpy(dst, src, n);
    for (i = 0; i < n; i++) {
        if (dst[i] == '\0')
            dst[i] = 'x';
    }
    dst[n] = '\0';
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    const uint8_t *p, *end, *chunk;
    uint8_t key[32], iv[16], plain[128], out[256], mac[16], digest[64];
    uint8_t ck[16], ik[16], ak[16], rand_[16], sqn[6], amf[2], autn[16];
    uint8_t opc[16], k[16], res[16], xres_star[16];
    uint8_t kausf[32], kseaf[32], kamf[32], knas[16], kgnb[32];
    uint8_t secret[32], pubkey[33], shared[32], basepoint[32];
    char snn[96], supi[64], b64[128];
    uint8_t decoded[96];
    uint32_t rk[OGS_AES_RKLENGTH(OGS_AES_MAX_KEY_BITS)];
    uint32_t outlen, inlen, keybits, count, bearer, dir;
    size_t res_len, n, i;
    int nrounds;
    u32 snow_k[4], snow_iv[4], snow_z[4], zuc_mac;
    uint8_t snow_data[32], zuc_out[32];

    if (Size < kMinInputLength || Size > kMaxInputLength)
        return 0;

    if (!initialized) {
        initialize();
        ogs_log_set_mask_level("core", OGS_LOG_NONE);
    }

    p = Data;
    end = Data + Size;
    memset(key, 0, sizeof(key));
    memset(iv, 0, sizeof(iv));
    memset(plain, 0, sizeof(plain));
    chunk = take(&p, end, 32);
    if (chunk)
        memcpy(key, chunk, 32);
    chunk = take(&p, end, 16);
    if (chunk)
        memcpy(iv, chunk, 16);

    keybits = 128 + 64 * (u8at(&p, end, 0) % 3);
    inlen = u8at(&p, end, 16);
    if (inlen == 0 || inlen > sizeof(plain))
        inlen = 16;
    chunk = take(&p, end, inlen);
    if (chunk)
        memcpy(plain, chunk, inlen);

    nrounds = ogs_aes_setup_enc(rk, key, (int)keybits);
    if (nrounds > 0)
        ogs_aes_encrypt(rk, nrounds, plain, out);
    nrounds = ogs_aes_setup_dec(rk, key, (int)keybits);
    if (nrounds > 0)
        ogs_aes_decrypt(rk, nrounds, plain, out);
    outlen = sizeof(out);
    ogs_aes_cbc_encrypt(key, keybits, iv, plain, inlen, out, &outlen);
    if ((inlen % 16) == 0) {
        outlen = sizeof(out);
        ogs_aes_cbc_decrypt(key, keybits, iv, plain, inlen, out, &outlen);
    }
    ogs_aes_ctr128_encrypt(key, iv, plain, inlen, out);
    ogs_aes_cmac_calculate(mac, key, plain, inlen);
    ogs_aes_cmac_verify(mac, key, plain, inlen);

    memcpy(ck, key, 16);
    memcpy(ik, key + 16, 16);
    memcpy(k, key, 16);
    memcpy(opc, key + 16, 16);
    memcpy(rand_, iv, 16);
    memcpy(sqn, Data, 6);
    memcpy(amf, Data + 6, 2);
    n = u8at(&p, end, 8);
    chunk = take(&p, end, n > 80 ? 80 : n);
    fill_str(snn, sizeof(snn), chunk, n > 80 ? 80 : n);

    milenage_opc(k, opc, opc);
    res_len = sizeof(res);
    milenage_generate(opc, amf, k, sqn, rand_, autn, ik, ck, ak, res, &res_len);
    milenage_f1(opc, k, rand_, sqn, amf, mac, mac + 8);
    milenage_f2345(opc, k, rand_, res, ck, ik, ak, ak);
    milenage_auts(opc, k, rand_, autn, sqn);
    res_len = sizeof(res);
    milenage_check(opc, k, sqn, rand_, autn, ik, ck, res, &res_len, autn);
    gsm_milenage(opc, k, rand_, mac, mac + 8);

    ogs_kdf_kausf(ck, ik, snn, autn, kausf);
    ogs_kdf_xres_star(ck, ik, snn, rand_, res, 8, xres_star);
    ogs_kdf_hxres_star(rand_, xres_star, mac);
    ogs_kdf_kseaf(snn, kausf, kseaf);

    n = u8at(&p, end, 8);
    if (n == 0)
        n = 1;
    if (n > 40)
        n = 40;
    memcpy(supi, "imsi-", 5);
    memset(supi + 5, '1', n);
    supi[5 + n] = '\0';
    chunk = take(&p, end, n);
    if (chunk) {
        for (i = 0; i < n; i++) {
            unsigned char c = chunk[i];
            if (c < 0x20 || c == '-' || c == '\0')
                c = '1';
            supi[5 + i] = (char)c;
        }
    }
    ogs_kdf_kamf(supi, amf, (uint8_t)sizeof(amf), kseaf, kamf);
    ogs_kdf_nas_5gs(u8at(&p, end, 1), u8at(&p, end, 2), kamf, knas);
    count = 0;
    chunk = take(&p, end, 4);
    if (chunk)
        memcpy(&count, chunk, 4);
    bearer = u8at(&p, end, 0) & 0x1f;
    dir = u8at(&p, end, 0) & 1;
    ogs_kdf_kgnb_and_kn3iwf(kamf, count, u8at(&p, end, 1), kgnb);
    ogs_kdf_nh_gnb(kamf, kgnb, kgnb);
    ogs_auc_kasme(ck, ik, sqn, sqn, ak, kausf);
    ogs_kdf_kenb(kausf, count, kgnb);
    ogs_kdf_nh_enb(kausf, kgnb, kgnb);
    ogs_kdf_nas_eps(u8at(&p, end, 1), u8at(&p, end, 1), kausf, knas);
    ogs_kdf_ck_ik_handover(count, kausf, ck, ik);
    ogs_kdf_nas_token(count, kausf, mac);
    ogs_kdf_kasme_idle_mobility(ck, ik, count, count, kausf);
    ogs_kdf_ck_ik_idle_mobility(count, kausf, ck, ik);
    ogs_kdf_hash_mme(plain, (uint32_t)inlen, mac);

    {
        size_t z_len = (u8at(&p, end, 16) % 32) + 1;
        size_t info_len = (u8at(&p, end, 8) % 16) + 1;
        uint8_t z[32], info[16], ek[16], icb[16], mk[32];
        if (z_len + info_len > 64)
            info_len = 1;
        memset(z, 0, sizeof(z));
        memset(info, 1, sizeof(info));
        ogs_kdf_ansi_x963(z, z_len, info, info_len, ek, icb, mk);
    }

    memcpy(snow_k, key, sizeof(snow_k));
    memcpy(snow_iv, iv, sizeof(snow_iv));
    snow_3g_initialize(snow_k, snow_iv);
    snow_3g_generate_key_stream(4, snow_z);
    memset(snow_data, 0, sizeof(snow_data));
    memcpy(snow_data, plain, 16);
    snow_3g_f8(key, count, bearer, dir, snow_data, 128);
    snow_3g_f9(key, count, count, dir, snow_data, 128, mac);
    zuc_eea3(key, count, bearer, dir, 128, snow_data, zuc_out);
    zuc_eia3(key, count, bearer, dir, 128, snow_data, &zuc_mac);
    kasumi_key_schedule(key);
    kasumi(snow_data);
    kasumi_f8(key, count, bearer, dir, snow_data, 64);
    kasumi_f9(key, count, count, dir, snow_data, 64);

    memcpy(secret, key, 32);
    secret[0] &= 248;
    secret[31] &= 127;
    secret[31] |= 64;
    memset(basepoint, 0, sizeof(basepoint));
    basepoint[0] = 9;
    curve25519_donna(pubkey, secret, basepoint);
    ecc_make_key(pubkey, secret);
    ecdh_shared_secret(pubkey, secret, shared);
    ecdsa_sign(secret, key, out);
    ecdsa_verify(pubkey, key, out);

    ogs_sha256(plain, (uint32_t)inlen, digest);
    ogs_sha512(plain, (uint32_t)inlen, digest);
    n = u8at(&p, end, 8);
    if (n >= sizeof(b64))
        n = sizeof(b64) - 1;
    fill_str(b64, sizeof(b64), take(&p, end, n), n);
    ogs_base64_decode_to_buffer(decoded, sizeof(decoded), b64);
    ogs_base64_encode_from_buffer(b64, sizeof(b64), plain, 16);
    return 0;
}
