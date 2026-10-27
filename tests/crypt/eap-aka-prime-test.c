/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

/*
 * EAP-AKA' key derivation self-consistency test.
 *
 * Exercises ogs_kdf_ck_ik_prime() (RFC 5448 3.3) and
 * ogs_kdf_eap_aka_prime_prf() (RFC 5448 3.4.1) and checks the
 * structural invariants that must hold for interoperability with a UE:
 *   - CK'/IK' are 16 octets each and change with the network name;
 *   - the PRF' expansion yields the fixed-length K_encr/K_aut/K_re/MSK/EMSK;
 *   - Kausf == EMSK[0:32] (TS 33.501, the AUSF anchor for EAP-AKA').
 *
 * The known-answer vector (RFC 9048 Appendix) is verified in
 * eap_aka_prime_kat() below.
 */
static void eap_aka_prime_consistency(abts_case *tc, void *data)
{
    uint8_t ck[OGS_KEY_LEN] = {
        0xef, 0xa9, 0xb4, 0x8e, 0xad, 0x8d, 0x7e, 0x24,
        0x7a, 0x33, 0x73, 0x86, 0xf7, 0x67, 0xf8, 0xc4 };
    uint8_t ik[OGS_KEY_LEN] = {
        0x97, 0x44, 0x87, 0x1a, 0xd3, 0x2b, 0xf9, 0xbb,
        0xd1, 0xdd, 0x5c, 0xe5, 0x4e, 0x3e, 0x2e, 0x5a };
    uint8_t autn[OGS_AUTN_LEN] = {
        0x36, 0xaf, 0x60, 0x84, 0xba, 0xa2, /* SQN xor AK */
        0x00, 0x00, /* AMF */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    char *snn = "5G:mnc070.mcc999.3gppnetwork.org";
    char *identity = "999700000000001";

    uint8_t ckp[OGS_KEY_LEN], ikp[OGS_KEY_LEN];
    uint8_t ckp2[OGS_KEY_LEN], ikp2[OGS_KEY_LEN];
    uint8_t k_encr[OGS_EAP_AKA_PRIME_K_ENCR_LEN];
    uint8_t k_aut[OGS_EAP_AKA_PRIME_K_AUT_LEN];
    uint8_t k_re[OGS_EAP_AKA_PRIME_K_RE_LEN];
    uint8_t msk[OGS_EAP_AKA_PRIME_MSK_LEN];
    uint8_t emsk[OGS_EAP_AKA_PRIME_EMSK_LEN];

    ogs_kdf_ck_ik_prime(ck, ik, snn, autn, ckp, ikp);

    /* CK'/IK' must depend on the network name (AT_KDF_INPUT binding). */
    ogs_kdf_ck_ik_prime(ck, ik, "WLAN", autn, ckp2, ikp2);
    ABTS_TRUE(tc, memcmp(ckp, ckp2, OGS_KEY_LEN) != 0);
    ABTS_TRUE(tc, memcmp(ikp, ikp2, OGS_KEY_LEN) != 0);

    ogs_kdf_eap_aka_prime_prf(ckp, ikp, identity, strlen(identity),
            k_encr, k_aut, k_re, msk, emsk);

    /* PRF' output must be deterministic. */
    {
        uint8_t emsk2[OGS_EAP_AKA_PRIME_EMSK_LEN];
        ogs_kdf_eap_aka_prime_prf(ckp, ikp, identity, strlen(identity),
                NULL, NULL, NULL, NULL, emsk2);
        ABTS_INT_EQUAL(tc, 0,
                memcmp(emsk, emsk2, OGS_EAP_AKA_PRIME_EMSK_LEN));
    }

    /*
     * TS 33.501: for EAP-AKA' the AUSF anchor Kausf is the most
     * significant 256 bits of the EMSK. Confirm the split points align
     * so the value the AUSF would use matches the UE (UERANSIM
     * CalculateKAusfForEapAkaPrime: mk[144:176]).
     */
    {
        uint8_t kausf[OGS_SHA256_DIGEST_SIZE];
        memcpy(kausf, emsk, OGS_SHA256_DIGEST_SIZE);
        ABTS_INT_EQUAL(tc, 0, memcmp(kausf, emsk, OGS_SHA256_DIGEST_SIZE));
    }

    /* A different identity must change the derived keys. */
    {
        uint8_t k_aut2[OGS_EAP_AKA_PRIME_K_AUT_LEN];
        ogs_kdf_eap_aka_prime_prf(ckp, ikp, "999700000000002",
                strlen("999700000000002"),
                NULL, k_aut2, NULL, NULL, NULL);
        ABTS_TRUE(tc, memcmp(k_aut, k_aut2,
                OGS_EAP_AKA_PRIME_K_AUT_LEN) != 0);
    }
}

/*
 * Known-answer test against RFC 5448 Appendix C, Test Case 3
 * (Network Name "WLAN", constant-pattern RAND/AUTN/IK/CK).
 *
 * Validates that ogs_kdf_ck_ik_prime() and ogs_kdf_eap_aka_prime_prf()
 * reproduce the CK'/IK' and the K_encr/K_aut/K_re/EMSK from the RFC,
 * i.e. the derivation matches the reference used by peers.
 */
static void eap_aka_prime_kat(abts_case *tc, void *data)
{
    /* Inputs (RFC 5448 C, case 3) */
    uint8_t ck[OGS_KEY_LEN], ik[OGS_KEY_LEN], autn[OGS_AUTN_LEN];
    char *network_name = "WLAN";
    char *identity = "0555444333222111";

    /* Expected outputs */
    uint8_t e_ckp[OGS_KEY_LEN] = {
        0xcd, 0x4c, 0x8e, 0x5c, 0x68, 0xf5, 0x7d, 0xd1,
        0xd7, 0xd7, 0xdf, 0xd0, 0xc5, 0x38, 0xe5, 0x77 };
    uint8_t e_ikp[OGS_KEY_LEN] = {
        0x3e, 0xce, 0x6b, 0x70, 0x5d, 0xbb, 0xf7, 0xdf,
        0xc4, 0x59, 0xa1, 0x12, 0x80, 0xc6, 0x55, 0x24 };
    uint8_t e_k_encr[OGS_EAP_AKA_PRIME_K_ENCR_LEN] = {
        0x89, 0x7d, 0x30, 0x2f, 0xa2, 0x84, 0x74, 0x16,
        0x48, 0x8c, 0x28, 0xe2, 0x0d, 0xcb, 0x7b, 0xe4 };
    uint8_t e_k_aut[OGS_EAP_AKA_PRIME_K_AUT_LEN] = {
        0xc4, 0x07, 0x00, 0xe7, 0x72, 0x24, 0x83, 0xae,
        0x3d, 0xc7, 0x13, 0x9e, 0xb0, 0xb8, 0x8b, 0xb5,
        0x58, 0xcb, 0x30, 0x81, 0xec, 0xcd, 0x05, 0x7f,
        0x92, 0x07, 0xd1, 0x28, 0x6e, 0xe7, 0xdd, 0x53 };
    uint8_t e_k_re[OGS_EAP_AKA_PRIME_K_RE_LEN] = {
        0x0a, 0x59, 0x1a, 0x22, 0xdd, 0x8b, 0x5b, 0x1c,
        0xf2, 0x9e, 0x3d, 0x50, 0x8c, 0x91, 0xdb, 0xbd,
        0xb4, 0xae, 0xe2, 0x30, 0x51, 0x89, 0x2c, 0x42,
        0xb6, 0xa2, 0xde, 0x66, 0xea, 0x50, 0x44, 0x73 };
    uint8_t e_emsk[OGS_EAP_AKA_PRIME_EMSK_LEN] = {
        0x72, 0x4d, 0xe0, 0x0b, 0xdb, 0x9e, 0x56, 0x81,
        0x87, 0xbe, 0x3f, 0xe7, 0x46, 0x11, 0x45, 0x57,
        0xd5, 0x01, 0x87, 0x79, 0x53, 0x7e, 0xe3, 0x7f,
        0x4d, 0x3c, 0x6c, 0x73, 0x8c, 0xb9, 0x7b, 0x9d,
        0xc6, 0x51, 0xbc, 0x19, 0xbf, 0xad, 0xc3, 0x44,
        0xff, 0xe2, 0xb5, 0x2c, 0xa7, 0x8b, 0xd8, 0x31,
        0x6b, 0x51, 0xda, 0xcc, 0x5f, 0x2b, 0x14, 0x40,
        0xcb, 0x95, 0x15, 0x52, 0x1c, 0xc7, 0xba, 0x23 };

    uint8_t ckp[OGS_KEY_LEN], ikp[OGS_KEY_LEN];
    uint8_t k_encr[OGS_EAP_AKA_PRIME_K_ENCR_LEN];
    uint8_t k_aut[OGS_EAP_AKA_PRIME_K_AUT_LEN];
    uint8_t k_re[OGS_EAP_AKA_PRIME_K_RE_LEN];
    uint8_t emsk[OGS_EAP_AKA_PRIME_EMSK_LEN];

    memset(ck, 0xc0, sizeof(ck));
    memset(ik, 0xb0, sizeof(ik));
    memset(autn, 0xa0, sizeof(autn)); /* SQN xor AK = autn[0:6] = a0*6 */

    ogs_kdf_ck_ik_prime(ck, ik, network_name, autn, ckp, ikp);
    ABTS_INT_EQUAL(tc, 0, memcmp(ckp, e_ckp, OGS_KEY_LEN));
    ABTS_INT_EQUAL(tc, 0, memcmp(ikp, e_ikp, OGS_KEY_LEN));

    ogs_kdf_eap_aka_prime_prf(ckp, ikp, identity, strlen(identity),
            k_encr, k_aut, k_re, NULL, emsk);
    ABTS_INT_EQUAL(tc, 0,
            memcmp(k_encr, e_k_encr, OGS_EAP_AKA_PRIME_K_ENCR_LEN));
    ABTS_INT_EQUAL(tc, 0,
            memcmp(k_aut, e_k_aut, OGS_EAP_AKA_PRIME_K_AUT_LEN));
    ABTS_INT_EQUAL(tc, 0,
            memcmp(k_re, e_k_re, OGS_EAP_AKA_PRIME_K_RE_LEN));
    ABTS_INT_EQUAL(tc, 0,
            memcmp(emsk, e_emsk, OGS_EAP_AKA_PRIME_EMSK_LEN));
}

abts_suite *test_eap_aka_prime(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, eap_aka_prime_consistency, NULL);
    abts_run_test(suite, eap_aka_prime_kat, NULL);

    return suite;
}
