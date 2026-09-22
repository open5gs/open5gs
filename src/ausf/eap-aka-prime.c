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

#include "eap-aka-prime.h"

/* EAP-AKA' attribute types (RFC 4187 10, RFC 5448) */
#define AT_RAND                 1
#define AT_AUTN                 2
#define AT_RES                  3
#define AT_AUTS                 4
#define AT_MAC                  11
#define AT_NOTIFICATION         12
#define AT_KDF_INPUT            23
#define AT_KDF                  24

/* code|id|len(2)|type|subtype|reserved(2) */
#define EAP_AKA_HEADER_LEN      8
#define AT_MAC_LEN              20  /* type|len|reserved(2)|mac(16) */
#define EAP_AKA_MAC_OFFSET_IN_ATTR 4 /* mac value offset inside AT_MAC */

/* Compute AT_MAC = HMAC-SHA-256(k_aut, packet-with-mac-zeroed)[0:16].
 * mac_value_off is the offset of the 16-octet MAC value within pkt. */
static void compute_at_mac(const uint8_t *k_aut,
        uint8_t *pkt, size_t pkt_len, size_t mac_value_off, uint8_t *mac_out)
{
    uint8_t digest[OGS_SHA256_DIGEST_SIZE];
    uint8_t saved[OGS_KEY_LEN];

    memcpy(saved, pkt + mac_value_off, OGS_KEY_LEN);
    memset(pkt + mac_value_off, 0, OGS_KEY_LEN);

    ogs_hmac_sha256(k_aut, OGS_EAP_AKA_PRIME_K_AUT_LEN,
            pkt, pkt_len, digest, sizeof(digest));

    memcpy(pkt + mac_value_off, saved, OGS_KEY_LEN);
    memcpy(mac_out, digest, OGS_KEY_LEN);
}

static char *encode_base64(const uint8_t *buf, size_t len)
{
    char *b64;
    int b64_len;

    b64_len = ogs_base64_encoded_size(len);
    b64 = ogs_calloc(1, b64_len + 1);
    ogs_assert(b64);
    ogs_base64_encode_from_buffer(b64, b64_len + 1, buf, len);

    return b64;
}

char *ausf_eap_aka_prime_build_challenge(ausf_ue_t *ausf_ue,
        const uint8_t *rand, const uint8_t *autn)
{
    uint8_t pkt[256];
    size_t pos = 0, mac_value_off;
    size_t name_len, kdf_input_pad, kdf_input_attr_len;
    uint16_t pkt_len16, name_len16;
    const char *network_name;

    ogs_assert(ausf_ue);
    ogs_assert(rand);
    ogs_assert(autn);

    network_name = ausf_ue->serving_network_name;
    ogs_assert(network_name);
    name_len = strlen(network_name);

    ausf_ue->eap_id++;

    /* EAP / AKA' header */
    pkt[pos++] = AUSF_EAP_CODE_REQUEST;
    pkt[pos++] = ausf_ue->eap_id;
    pos += 2; /* length, filled below */
    pkt[pos++] = AUSF_EAP_TYPE_AKA_PRIME;
    pkt[pos++] = AUSF_EAP_AKA_SUBTYPE_CHALLENGE;
    pkt[pos++] = 0; /* reserved */
    pkt[pos++] = 0;

    /* AT_RAND */
    pkt[pos++] = AT_RAND;
    pkt[pos++] = 5; /* length in 4-octet units (20 octets) */
    pkt[pos++] = 0;
    pkt[pos++] = 0;
    memcpy(pkt + pos, rand, OGS_RAND_LEN);
    pos += OGS_RAND_LEN;

    /* AT_AUTN */
    pkt[pos++] = AT_AUTN;
    pkt[pos++] = 5;
    pkt[pos++] = 0;
    pkt[pos++] = 0;
    memcpy(pkt + pos, autn, OGS_AUTN_LEN);
    pos += OGS_AUTN_LEN;

    /* AT_KDF (value 1 : the default CK'/IK' KDF) */
    pkt[pos++] = AT_KDF;
    pkt[pos++] = 1;
    pkt[pos++] = 0;
    pkt[pos++] = 1;

    /* AT_KDF_INPUT : the serving network name */
    kdf_input_pad = (4 - (name_len % 4)) % 4;
    /* type|len|actual-len(2)|name|pad */
    kdf_input_attr_len = 4 + name_len + kdf_input_pad;
    if (pos + kdf_input_attr_len + AT_MAC_LEN > sizeof(pkt)) {
        ogs_error("[%s] Serving network name too long [%d]",
                ausf_ue->suci, (int)name_len);
        return NULL;
    }
    pkt[pos++] = AT_KDF_INPUT;
    pkt[pos++] = (uint8_t)(kdf_input_attr_len / 4);
    name_len16 = htobe16((uint16_t)name_len);
    memcpy(pkt + pos, &name_len16, 2);
    pos += 2;
    memcpy(pkt + pos, network_name, name_len);
    pos += name_len;
    if (kdf_input_pad) {
        memset(pkt + pos, 0, kdf_input_pad);
        pos += kdf_input_pad;
    }

    /* AT_MAC (value computed over the whole packet with MAC zeroed) */
    pkt[pos++] = AT_MAC;
    pkt[pos++] = 5;
    pkt[pos++] = 0;
    pkt[pos++] = 0;
    mac_value_off = pos;
    memset(pkt + pos, 0, OGS_KEY_LEN);
    pos += OGS_KEY_LEN;

    /* Patch EAP Length (whole packet, big-endian) */
    pkt_len16 = htobe16((uint16_t)pos);
    memcpy(pkt + 2, &pkt_len16, 2);

    compute_at_mac(ausf_ue->k_aut, pkt, pos, mac_value_off,
            pkt + mac_value_off);

    return encode_base64(pkt, pos);
}

char *ausf_eap_aka_prime_build_result(uint8_t code, uint8_t eap_id)
{
    uint8_t pkt[4];
    uint16_t len16;

    pkt[0] = code;
    pkt[1] = eap_id;
    len16 = htobe16(4);
    memcpy(pkt + 2, &len16, 2);

    return encode_base64(pkt, sizeof(pkt));
}

int ausf_eap_aka_prime_handle_response(ausf_ue_t *ausf_ue,
        const char *eap_payload_b64, bool *success, uint8_t *auts)
{
    uint8_t *pkt = NULL;
    int decoded_size, pkt_len;
    size_t pos;
    int subtype = -1;

    const uint8_t *at_res = NULL, *at_mac = NULL;
    size_t res_len = 0;
    bool have_auts = false;

    ogs_assert(ausf_ue);
    ogs_assert(eap_payload_b64);
    if (success) *success = false;

    decoded_size = ogs_base64_decoded_size(eap_payload_b64);
    if (decoded_size < EAP_AKA_HEADER_LEN) {
        ogs_error("[%s] EAP payload too short [%d]", ausf_ue->suci,
                decoded_size);
        return -1;
    }
    pkt = ogs_calloc(1, decoded_size + 1);
    ogs_assert(pkt);
    pkt_len = ogs_base64_decode_to_buffer(pkt, decoded_size + 1,
            eap_payload_b64);

    if (pkt_len < EAP_AKA_HEADER_LEN) {
        ogs_error("[%s] EAP decode failed [%d]", ausf_ue->suci, pkt_len);
        goto cleanup;
    }
    if (pkt[0] != AUSF_EAP_CODE_RESPONSE) {
        ogs_error("[%s] Not an EAP Response [%d]", ausf_ue->suci, pkt[0]);
        goto cleanup;
    }
    if (pkt[4] != AUSF_EAP_TYPE_AKA_PRIME) {
        ogs_error("[%s] Not EAP-AKA' [%d]", ausf_ue->suci, pkt[4]);
        goto cleanup;
    }

    /*
     * RFC 3748 4.1: the Identifier of the Response must match the
     * Identifier of the outstanding Request.
     */
    if (pkt[1] != ausf_ue->eap_id) {
        ogs_error("[%s] EAP Identifier mismatch [%d != %d]",
                ausf_ue->suci, pkt[1], ausf_ue->eap_id);
        goto cleanup;
    }

    subtype = pkt[5];

    /* Walk the attributes (each: type|len(4-octet units)|value) */
    pos = EAP_AKA_HEADER_LEN;
    while (pos + 2 <= (size_t)pkt_len) {
        uint8_t atype = pkt[pos];
        size_t alen = (size_t)pkt[pos+1] * 4;

        if (alen == 0 || pos + alen > (size_t)pkt_len) {
            ogs_error("[%s] Bad EAP attribute len", ausf_ue->suci);
            subtype = -1;
            goto cleanup;
        }

        switch (atype) {
        case AT_RES:
            /* value: RES length in bits(2) | RES | pad */
            if (alen >= 4) {
                size_t bits = ((size_t)pkt[pos+2] << 8) | pkt[pos+3];
                res_len = bits / 8;
                if (4 + res_len <= alen)
                    at_res = pkt + pos + 4;
                else
                    res_len = 0;
            }
            break;
        case AT_MAC:
            /* value: reserved(2) | MAC(16) */
            if (alen == AT_MAC_LEN)
                at_mac = pkt + pos + EAP_AKA_MAC_OFFSET_IN_ATTR;
            break;
        case AT_AUTS:
            /* value: AUTS(14) */
            if (alen == 16 && auts) {
                memcpy(auts, pkt + pos + 2, OGS_AUTS_LEN);
                have_auts = true;
            }
            break;
        default:
            break;
        }
        pos += alen;
    }

    if (subtype == AUSF_EAP_AKA_SUBTYPE_SYNCHRONIZATION_FAILURE) {
        if (!have_auts)
            ogs_error("[%s] Sync failure without AT_AUTS", ausf_ue->suci);
        goto cleanup;
    }

    if (subtype == AUSF_EAP_AKA_SUBTYPE_CHALLENGE) {
        uint8_t xmac[OGS_KEY_LEN];

        if (!at_mac) {
            ogs_error("[%s] AKA'-Challenge response without AT_MAC",
                    ausf_ue->suci);
            goto cleanup;
        }
        if (!at_res || res_len == 0) {
            ogs_error("[%s] AKA'-Challenge response without AT_RES",
                    ausf_ue->suci);
            goto cleanup;
        }

        /* Verify AT_MAC over the received packet with the MAC field zeroed */
        compute_at_mac(ausf_ue->k_aut, pkt, pkt_len,
                (at_mac - pkt), xmac);
        if (memcmp(xmac, at_mac, OGS_KEY_LEN) != 0) {
            ogs_error("[%s] EAP-AKA' AT_MAC mismatch", ausf_ue->suci);
            goto cleanup;
        }

        /* Compare AT_RES with the expected XRES from the UDM vector */
        if (res_len != ausf_ue->xres_len ||
            memcmp(at_res, ausf_ue->xres, res_len) != 0) {
            ogs_error("[%s] EAP-AKA' RES mismatch", ausf_ue->suci);
            goto cleanup;
        }

        if (success) *success = true;
    }

cleanup:
    if (pkt) ogs_free(pkt);
    return subtype;
}
