/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#define MAX_NUM_OF_KDF_PARAM                    16

#define FC_FOR_CK_PRIME_IK_PRIME_DERIVATION     0x20
#define FC_FOR_5GS_ALGORITHM_KEY_DERIVATION     0x69
#define FC_FOR_KAUSF_DERIVATION                 0x6A
#define FC_FOR_RES_STAR_XRES_STAR_DERIVATION    0x6B
#define FC_FOR_KSEAF_DERIVATION                 0x6C
#define FC_FOR_KAMF_DERIVATION                  0x6D
#define FC_FOR_KGNB_KN3IWF_DERIVATION           0x6E
#define FC_FOR_NH_GNB_DERIVATION                0x6F

#define FC_FOR_KASME                            0x10
#define FC_FOR_KENB_DERIVATION                  0x11
#define FC_FOR_NH_ENB_DERIVATION                0x12
#define FC_FOR_EPS_ALGORITHM_KEY_DERIVATION     0x15
#define FC_FOR_CK_IK_DERIVATION_HANDOVER        0x16
#define FC_FOR_NAS_TOKEN_DERIVATION             0x17
#define FC_FOR_KASME_DERIVATION_IDLE_MOBILITY   0x19
#define FC_FOR_CK_IK_DERIVATION_IDLE_MOBILITY   0x1B

typedef struct kdf_param_s {
    const uint8_t *buf;
    uint16_t len;
} kdf_param_t[MAX_NUM_OF_KDF_PARAM];

/* KDF function : TS.33220 cluase B.2.0 */
static void ogs_kdf_common(const uint8_t *key, uint32_t key_size,
        uint8_t fc, kdf_param_t param, uint8_t *output)
{
    int i = 0, pos;
    uint8_t *s = NULL;

    ogs_assert(key);
    ogs_assert(key_size);
    ogs_assert(fc);
    ogs_assert(param[0].buf);
    ogs_assert(param[0].len);
    ogs_assert(output);

    pos = 1; /* FC Value */

    /* Calculate buffer length */
    for (i = 0; i < MAX_NUM_OF_KDF_PARAM && param[i].buf && param[i].len; i++) {
        pos += (param[i].len + 2);
    }

    s = ogs_calloc(1, pos);
    ogs_assert(s);

    /* Copy buffer from param */
    pos = 0;
    s[pos++] = fc;
    for (i = 0; i < MAX_NUM_OF_KDF_PARAM && param[i].buf && param[i].len; i++) {
        uint16_t len;

        memcpy(&s[pos], param[i].buf, param[i].len);
        pos += param[i].len;
        len = htobe16(param[i].len);
        memcpy(&s[pos], &len, sizeof(len));
        pos += 2;
    }

    ogs_hmac_sha256(key, key_size, s, pos, output, OGS_SHA256_DIGEST_SIZE);

    ogs_free(s);
}

/* TS33.501 Annex A.2 : Kausf derviation function */
void ogs_kdf_kausf(
        uint8_t *ck, uint8_t *ik,
        char *serving_network_name, uint8_t *autn,
        uint8_t *kausf)
{
    kdf_param_t param;
    uint8_t key[OGS_KEY_LEN*2];

    ogs_assert(ck);
    ogs_assert(ik);
    ogs_assert(serving_network_name);
    ogs_assert(autn);
    ogs_assert(kausf);

    memcpy(key, ck, OGS_KEY_LEN);
    memcpy(key+OGS_KEY_LEN, ik, OGS_KEY_LEN);

    memset(param, 0, sizeof(param));
    param[0].buf = (uint8_t *)serving_network_name;
    param[0].len = strlen(serving_network_name);
    param[1].buf = autn;
    param[1].len = OGS_SQN_XOR_AK_LEN;

    ogs_kdf_common(key, OGS_KEY_LEN*2,
            FC_FOR_KAUSF_DERIVATION, param, kausf);
}

/* TS33.501 Annex A.4 : RES* and XRES* derivation function */
void ogs_kdf_xres_star(
        uint8_t *ck, uint8_t *ik,
        char *serving_network_name, uint8_t *rand,
        uint8_t *xres, size_t xres_len,
        uint8_t *xres_star)
{
    kdf_param_t param;
    uint8_t key[OGS_KEY_LEN*2];
    uint8_t output[OGS_SHA256_DIGEST_SIZE];

    ogs_assert(ck);
    ogs_assert(ik);
    ogs_assert(serving_network_name);
    ogs_assert(rand);
    ogs_assert(xres);
    ogs_assert(xres_len);

    memcpy(key, ck, OGS_KEY_LEN);
    memcpy(key+OGS_KEY_LEN, ik, OGS_KEY_LEN);

    memset(param, 0, sizeof(param));
    param[0].buf = (uint8_t *)serving_network_name;
    param[0].len = strlen(serving_network_name);
    param[1].buf = rand;
    param[1].len = OGS_RAND_LEN;
    param[2].buf = xres;
    param[2].len = xres_len;

    ogs_kdf_common(key, OGS_KEY_LEN*2,
            FC_FOR_RES_STAR_XRES_STAR_DERIVATION, param, output);

    memcpy(xres_star, output+OGS_KEY_LEN, OGS_KEY_LEN);
}

/* TS33.501 Annex A.5 : HRES* and HXRES* derivation function */
void ogs_kdf_hxres_star(uint8_t *rand, uint8_t *xres_star, uint8_t *hxres_star)
{
    uint8_t message[OGS_RAND_LEN + OGS_KEY_LEN];
    uint8_t output[OGS_SHA256_DIGEST_SIZE];

    ogs_assert(rand);
    ogs_assert(xres_star);
    ogs_assert(hxres_star);

    memcpy(message, rand, OGS_RAND_LEN);
    memcpy(message+OGS_RAND_LEN, xres_star, OGS_KEY_LEN);

    ogs_sha256(message, OGS_RAND_LEN+OGS_KEY_LEN, output);

    memcpy(hxres_star, output+OGS_KEY_LEN, OGS_KEY_LEN);
}

/* TS33.501 Annex A.6 : Kseaf derivation function */
void ogs_kdf_kseaf(char *serving_network_name, const uint8_t *kausf, uint8_t *kseaf)
{
    kdf_param_t param;

    ogs_assert(serving_network_name);
    ogs_assert(kausf);
    ogs_assert(kseaf);

    memset(param, 0, sizeof(param));
    param[0].buf = (uint8_t *)serving_network_name;
    param[0].len = strlen(serving_network_name);

    ogs_kdf_common(kausf, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_KSEAF_DERIVATION, param, kseaf);
}

/* TS33.501 Annex A.7 : Kamf derivation function */
void ogs_kdf_kamf(const char *supi, const uint8_t *abba, uint8_t abba_len,
        const uint8_t *kseaf, uint8_t *kamf)
{
    kdf_param_t param;
    char *val;

    ogs_assert(supi);
    ogs_assert(abba);
    ogs_assert(abba_len);
    ogs_assert(kseaf);
    ogs_assert(kamf);

    val = ogs_id_get_value(supi);
    memset(param, 0, sizeof(param));
    param[0].buf = (const uint8_t*) val;
    ogs_assert(param[0].buf);
    param[0].len = strlen(val);
    param[1].buf = abba;
    param[1].len = abba_len;

    ogs_kdf_common(kseaf, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_KAMF_DERIVATION, param, kamf);

    ogs_free(val);
}

/* TS33.501 Annex A.8 : Algorithm key derivation functions */
void ogs_kdf_nas_5gs(uint8_t algorithm_type_distinguishers,
    uint8_t algorithm_identity, const uint8_t *kamf, uint8_t *knas)
{
    kdf_param_t param;
    uint8_t output[OGS_SHA256_DIGEST_SIZE];

    ogs_assert(kamf);
    ogs_assert(knas);

    memset(param, 0, sizeof(param));
    param[0].buf = &algorithm_type_distinguishers;
    param[0].len = 1;
    param[1].buf = &algorithm_identity;
    param[1].len = 1;

    ogs_kdf_common(kamf, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_5GS_ALGORITHM_KEY_DERIVATION, param, output);
    memcpy(knas, output+16, 16);
}

/* TS33.501 Annex A.9 KgNB and Kn3iwf derivation function */
void ogs_kdf_kgnb_and_kn3iwf(const uint8_t *kamf, uint32_t ul_count,
        uint8_t access_type_distinguisher, uint8_t *kgnb)
{
    kdf_param_t param;

    ogs_assert(kamf);
    ogs_assert(kgnb);

    memset(param, 0, sizeof(param));
    ul_count = htobe32(ul_count);
    param[0].buf = (uint8_t *)&ul_count;
    param[0].len = 4;
    param[1].buf = &access_type_distinguisher;
    param[1].len = 1;

    ogs_kdf_common(kamf, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_KGNB_KN3IWF_DERIVATION, param, kgnb);
}

/* TS33.501 Annex A.10 NH derivation function */
void ogs_kdf_nh_gnb(const uint8_t *kamf, uint8_t *sync_input, uint8_t *kgnb)
{
    kdf_param_t param;

    ogs_assert(kamf);
    ogs_assert(kgnb);

    memset(param, 0, sizeof(param));
    param[0].buf = sync_input;
    param[0].len = OGS_SHA256_DIGEST_SIZE;

    ogs_kdf_common(kamf, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_NH_GNB_DERIVATION, param, kgnb);
}

/*
 * TS33.501 Annex C.3.4.1 Profile A
 * TS33.501 Annex C.3.4.2 Profile B
 * ANSI-X9.63-KDF
 */
void ogs_kdf_ansi_x963(
        const uint8_t *z, size_t z_len, const uint8_t *info, size_t info_len,
        uint8_t *ek, uint8_t *icb, uint8_t *mk)
{
    uint8_t input[ECC_BYTES+4+ECC_BYTES+1];
    uint8_t output[OGS_KEY_LEN+OGS_IVEC_LEN+OGS_SHA256_DIGEST_SIZE];
    uint32_t counter = 0;
    size_t counter_len = sizeof(counter);

    ogs_assert(z);
    ogs_assert(info);
    ogs_assert(ek);
    ogs_assert(icb);
    ogs_assert(mk);

    ogs_assert((z_len+counter_len+info_len) <= (ECC_BYTES+4+ECC_BYTES+1));

    memcpy(input, z, z_len);
    counter = htobe32(1);
    memcpy(input+z_len, &counter, counter_len);
    memcpy(input+z_len+counter_len, info, info_len);

    ogs_sha256(input, z_len+counter_len+info_len, output);
    memcpy(ek, output, OGS_KEY_LEN);
    memcpy(icb, output+OGS_KEY_LEN, OGS_IVEC_LEN);

    counter = htobe32(2);
    memcpy(input+z_len, &counter, counter_len);

    ogs_sha256(input, z_len+counter_len+info_len, mk);
}

/* TS33.401 Annex A.2 KASME derivation function */
void ogs_auc_kasme(const uint8_t *ck, const uint8_t *ik,
        const uint8_t *plmn_id, const uint8_t *sqn,  const uint8_t *ak,
        uint8_t *kasme)
{
    kdf_param_t param;
    int i;

    uint8_t key[OGS_KEY_LEN*2];
    uint8_t sqn_xor_ak[OGS_SQN_XOR_AK_LEN];

    ogs_assert(ck);
    ogs_assert(ik);
    ogs_assert(plmn_id);
    ogs_assert(sqn);
    ogs_assert(ak);

    memcpy(key, ck, OGS_KEY_LEN);
    memcpy(key + OGS_KEY_LEN, ik, OGS_KEY_LEN);

    memset(param, 0, sizeof(param));
    param[0].buf = (uint8_t *)plmn_id;
    param[0].len = OGS_PLMN_ID_LEN;

    for (i = 0; i < 6; i++)
        sqn_xor_ak[i] = sqn[i] ^ ak[i];

    param[1].buf = sqn_xor_ak;
    param[1].len = OGS_SQN_XOR_AK_LEN;

    ogs_kdf_common(key, OGS_SHA256_DIGEST_SIZE, FC_FOR_KASME, param, kasme);
}

/* TS33.401 Annex A.3 KeNB derivation function */
void ogs_kdf_kenb(const uint8_t *kasme, uint32_t ul_count, uint8_t *kenb)
{
    kdf_param_t param;

    memset(param, 0, sizeof(param));
    ul_count = htobe32(ul_count);
    param[0].buf = (uint8_t *)&ul_count;
    param[0].len = 4;

    ogs_kdf_common(kasme, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_KENB_DERIVATION, param, kenb);
}

/* TS33.401 Annex A.4 NH derivation function */
void ogs_kdf_nh_enb(const uint8_t *kasme, const uint8_t *sync_input, uint8_t *kenb)
{
    kdf_param_t param;

    memset(param, 0, sizeof(param));
    param[0].buf = sync_input;
    param[0].len = OGS_SHA256_DIGEST_SIZE;

    ogs_kdf_common(kasme, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_NH_ENB_DERIVATION, param, kenb);
}

/* TS33.401 Annex A.7 Algorithm key derivation functions */
void ogs_kdf_nas_eps(uint8_t algorithm_type_distinguishers,
    uint8_t algorithm_identity, const uint8_t *kasme, uint8_t *knas)
{
    kdf_param_t param;
    uint8_t output[OGS_SHA256_DIGEST_SIZE];

    memset(param, 0, sizeof(param));
    param[0].buf = &algorithm_type_distinguishers;
    param[0].len = 1;
    param[1].buf = &algorithm_identity;
    param[1].len = 1;

    ogs_kdf_common(kasme, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_EPS_ALGORITHM_KEY_DERIVATION, param, output);
    memcpy(knas, output+16, 16);
}

/* TS33.401 Annex A.8: KASME to CK', IK' derivation at handover */
void ogs_kdf_ck_ik_handover(
    uint32_t dl_count, const uint8_t *kasme, uint8_t *ck, uint8_t *ik)
{
    kdf_param_t param;
    uint8_t output[OGS_SHA256_DIGEST_SIZE];

    memset(param, 0, sizeof(param));
    param[0].buf = (uint8_t *)&dl_count;
    param[0].len = 4;

    ogs_kdf_common(kasme, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_CK_IK_DERIVATION_HANDOVER, param, output);
    memcpy(ck, output, 16);
    memcpy(ik, output+16, 16);
}

/* TS33.401 Annex A.9: NAS token derivation for inter-RAT mobility */
void ogs_kdf_nas_token(
    uint32_t ul_count, const uint8_t *kasme, uint8_t *nas_token)
{
    kdf_param_t param;
    uint8_t output[OGS_SHA256_DIGEST_SIZE];

    memset(param, 0, sizeof(param));
    param[0].buf = (uint8_t *)&ul_count;
    param[0].len = 4;

    ogs_kdf_common(kasme, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_NAS_TOKEN_DERIVATION, param, output);
    memcpy(nas_token, output, 2);
}

/* TS33.401 Annex A.11 : K’ASME from CK, IK derivation during idle mode mobility */
void ogs_kdf_kasme_idle_mobility(
        const uint8_t *ck, const uint8_t *ik,
        uint32_t nonce_ue, uint32_t nonce_mme,
        uint8_t *kasme)
{
    kdf_param_t param;
    uint8_t key[OGS_KEY_LEN*2];

    ogs_assert(ck);
    ogs_assert(ik);
    ogs_assert(kasme);

    memcpy(key, ck, OGS_KEY_LEN);
    memcpy(key+OGS_KEY_LEN, ik, OGS_KEY_LEN);

    memset(param, 0, sizeof(param));
    param[0].buf = (uint8_t *)&nonce_ue;
    param[0].len = sizeof(nonce_ue);
    param[1].buf = (uint8_t *)&nonce_mme;
    param[1].len = sizeof(nonce_mme);

    ogs_kdf_common(key, OGS_KEY_LEN*2,
            FC_FOR_KASME_DERIVATION_IDLE_MOBILITY, param, kasme);
}

/* TS33.401 Annex A.13: KASME to CK', IK' derivation at idle mobility */
void ogs_kdf_ck_ik_idle_mobility(
    uint32_t ul_count, const uint8_t *kasme, uint8_t *ck, uint8_t *ik)
{
    kdf_param_t param;
    uint8_t output[OGS_SHA256_DIGEST_SIZE];

    memset(param, 0, sizeof(param));
    param[0].buf = (uint8_t *)&ul_count;
    param[0].len = 4;

    ogs_kdf_common(kasme, OGS_SHA256_DIGEST_SIZE,
            FC_FOR_CK_IK_DERIVATION_IDLE_MOBILITY, param, output);
    memcpy(ck, output, 16);
    memcpy(ik, output+16, 16);
}

/*
 * TS33.401 Annex I Hash Functions
 * Use the KDF given in TS33.220
 */
void ogs_kdf_hash_mme(
        const uint8_t *message, uint32_t message_len, uint8_t *hash_mme)
{
    uint8_t key[32];
    uint8_t output[OGS_SHA256_DIGEST_SIZE];

    ogs_assert(message);
    ogs_assert(message_len);
    ogs_assert(hash_mme);

    memset(key, 0, 32);
    ogs_hmac_sha256(key, 32, message, message_len,
            output, OGS_SHA256_DIGEST_SIZE);

    memcpy(hash_mme, output+24, OGS_HASH_MME_LEN);
}

/*
 * TS33.102
 * 6.3.3 Authentication and key agreement
 * Re-use and re-transmission of (RAND, AUTN)
 */
void ogs_auc_sqn(
    const uint8_t *opc, const uint8_t *k,
    const uint8_t *rand, const uint8_t *conc_sqn_ms,
    uint8_t *sqn_ms, uint8_t *mac_s)
{
    int i;
    uint8_t ak[OGS_AK_LEN];
    /*
     * The AMF used to calculate MAC-S assumes a dummy value of
     * all zeros so that it does not need to be transmitted in the clear
     * in the re-synch message.
     */
    uint8_t amf[2] = { 0, 0 };

    ogs_assert(opc);
    ogs_assert(k);
    ogs_assert(rand);
    ogs_assert(conc_sqn_ms);

    milenage_f2345(opc, k, rand, NULL, NULL, NULL, NULL, ak);
    for (i = 0; i < OGS_SQN_LEN; i++)
        sqn_ms[i] = ak[i] ^ conc_sqn_ms[i];
    milenage_f1(opc, k, rand, sqn_ms, amf, NULL, mac_s);
}
