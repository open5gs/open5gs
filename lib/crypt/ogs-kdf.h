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

#if !defined(OGS_CRYPT_INSIDE) && !defined(OGS_CRYPT_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_KDF_H
#define OGS_KDF_H

#ifdef __cplusplus
extern "C" {
#endif

/* TS33.501 Annex A.2 : Kausf derviation function */
void ogs_kdf_kausf(
        uint8_t *ck, uint8_t *ik,
        char *serving_network_name, uint8_t *autn,
        uint8_t *kausf);

/* TS33.501 Annex A.4 : RES* and XRES* derivation function */
void ogs_kdf_xres_star(
        uint8_t *ck, uint8_t *ik,
        char *serving_network_name, uint8_t *rand,
        uint8_t *xres, size_t xres_len,
        uint8_t *xres_star);

/* TS33.501 Annex A.5 : HRES* and HXRES* derivation function */
void ogs_kdf_hxres_star(uint8_t *rand, uint8_t *xres_star, uint8_t *hxres_star);

/* TS33.501 Annex A.6 : Kseaf derivation function */
void ogs_kdf_kseaf(char *serving_network_name, uint8_t *kausf, uint8_t *kseaf);

/* TS33.501 Annex A.7 : Kamf derivation function */
void ogs_kdf_kamf(char *supi, uint8_t *abba, uint8_t abba_len,
        uint8_t *kseaf, uint8_t *kamf);

/* Algorithm Type Distinguishers */
#define OGS_KDF_NAS_ENC_ALG 0x01
#define OGS_KDF_NAS_INT_ALG 0x02

/* Algorithm Identity defined in lib/nas/common/types.h
#define OGS_NAS_SECURITY_ALGORITHMS_NIA0 0
#define OGS_NAS_SECURITY_ALGORITHMS_128_NIA1 1
#define OGS_NAS_SECURITY_ALGORITHMS_128_NIA1 2
#define OGS_NAS_SECURITY_ALGORITHMS_128_NIA3 3
#define OGS_NAS_SECURITY_ALGORITHMS_NEA0 0
#define OGS_NAS_SECURITY_ALGORITHMS_128_NEA1 1
#define OGS_NAS_SECURITY_ALGORITHMS_128_NEA2 2
#define OGS_NAS_SECURITY_ALGORITHMS_128_NEA3 3
*/

/* TS33.501 Annex A.8 : Algorithm key derivation functions */
void ogs_kdf_nas_5gs(uint8_t algorithm_type_distinguishers,
    uint8_t algorithm_identity, uint8_t *kamf, uint8_t *knas);

/* TS33.501 Annex A.9 KgNB and Kn3iwf derivation function */
void ogs_kdf_kgnb_and_kn3iwf(uint8_t *kamf, uint32_t ul_count,
        uint8_t access_type_distinguisher, uint8_t *kgnb);

/* TS33.501 Annex A.10 NH derivation function */
void ogs_kdf_nh_gnb(uint8_t *kamf, uint8_t *sync_input, uint8_t *kgnb);

/* TS33.401 Annex A.2 KASME derivation function */
void ogs_auc_kasme(const uint8_t *ck, const uint8_t *ik, 
        const uint8_t *plmn_id, const uint8_t *sqn,  const uint8_t *ak,
        uint8_t *kasme);

/* TS33.401 Annex A.3 KeNB derivation function */
void ogs_kdf_kenb(uint8_t *kasme, uint32_t ul_count, uint8_t *kenb);

/* TS33.401 Annex A.4 NH derivation function */
void ogs_kdf_nh_enb(uint8_t *kasme, uint8_t *sync_input, uint8_t *kenb);

/* TS33.401 Annex A.7 Algorithm key derivation functions */
void ogs_kdf_nas_eps(uint8_t algorithm_type_distinguishers,
    uint8_t algorithm_identity, uint8_t *kasme, uint8_t *knas);

/*
 * TS33.401 Annex I Hash Functions
 * Use the KDF given in TS33.220
 */
void ogs_kdf_hash_mme(uint8_t *message, uint8_t message_len, uint8_t *hash_mme);

/*
 * TS33.102
 * 6.3.3 Authentication and key agreement
 * Re-use and re-transmission of (RAND, AUTN)
 */
void ogs_auc_sqn(
    const uint8_t *opc, const uint8_t *k,
    const uint8_t *rand, const uint8_t *conc_sqn_ms,
    uint8_t *sqn_ms, uint8_t *mac_s);

#ifdef __cplusplus
}
#endif

#endif /* OGS_KDF_H */
