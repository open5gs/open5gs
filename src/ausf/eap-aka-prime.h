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

#ifndef AUSF_EAP_AKA_PRIME_H
#define AUSF_EAP_AKA_PRIME_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* EAP codes (RFC 3748) */
#define AUSF_EAP_CODE_REQUEST                           1
#define AUSF_EAP_CODE_RESPONSE                          2
#define AUSF_EAP_CODE_SUCCESS                           3
#define AUSF_EAP_CODE_FAILURE                           4

/* EAP method type (IANA) */
#define AUSF_EAP_TYPE_AKA_PRIME                         50

/* EAP-AKA' subtypes (RFC 4187 8.1, RFC 5448) */
#define AUSF_EAP_AKA_SUBTYPE_CHALLENGE                  1
#define AUSF_EAP_AKA_SUBTYPE_AUTHENTICATION_REJECT      2
#define AUSF_EAP_AKA_SUBTYPE_SYNCHRONIZATION_FAILURE    4
#define AUSF_EAP_AKA_SUBTYPE_NOTIFICATION               12
#define AUSF_EAP_AKA_SUBTYPE_CLIENT_ERROR               14

/*
 * Build an EAP-Request/AKA'-Challenge for this UE and return it as a
 * base64-encoded EapPayload string (caller frees with ogs_free()).
 *
 * The packet carries AT_RAND, AT_AUTN, AT_KDF(=1), AT_KDF_INPUT (the
 * serving network name) and AT_MAC keyed by ausf_ue->k_aut. The chosen
 * EAP Identifier is recorded in ausf_ue->eap_id.
 */
char *ausf_eap_aka_prime_build_challenge(ausf_ue_t *ausf_ue,
        const uint8_t *rand, const uint8_t *autn);

/*
 * Build a bare EAP-Success or EAP-Failure packet (code only, no type)
 * and return it base64-encoded (caller frees with ogs_free()).
 */
char *ausf_eap_aka_prime_build_result(uint8_t code, uint8_t eap_id);

/*
 * Parse an EAP-Response EapPayload (base64) from the UE.
 *
 * Returns the EAP-AKA' subtype (>= 0), or a negative value on a
 * malformed packet. For an AKA'-Challenge response, *success is set true
 * only when AT_MAC (verified with ausf_ue->k_aut) and AT_RES (compared
 * against ausf_ue->xres) both match. For a synchronization failure the
 * 14-octet AT_AUTS is copied into auts (when auts != NULL).
 */
int ausf_eap_aka_prime_handle_response(ausf_ue_t *ausf_ue,
        const char *eap_payload_b64, bool *success, uint8_t *auts);

#ifdef __cplusplus
}
#endif

#endif /* AUSF_EAP_AKA_PRIME_H */
