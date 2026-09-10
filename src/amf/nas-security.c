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

#include "nas-security.h"

#define NAS_SECURITY_MAC_SIZE 4

ogs_pkbuf_t *nas_5gs_security_encode(
        amf_ue_t *amf_ue, ogs_nas_5gs_message_t *message)
{
    int integrity_protected = 0;
    int new_security_context = 0;
    int ciphered = 0;
    ogs_nas_5gs_security_header_t h;
    ogs_pkbuf_t *new = NULL;

    ogs_assert(amf_ue);
    ogs_assert(message);

    switch (message->h.security_header_type) {
    case OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
        return ogs_nas_5gs_plain_encode(message);
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED:
        integrity_protected = 1;
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED:
        integrity_protected = 1;
        ciphered = 1;
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT:
        integrity_protected = 1;
        new_security_context = 1;
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT:
        integrity_protected = 1;
        new_security_context = 1;
        ciphered = 1;
        break;
    default:
        ogs_error("Not implemented(security header type:0x%x)",
                message->h.security_header_type);
        return NULL;
    }

    if (new_security_context) {
        amf_ue->dl_count = 0;
        amf_ue->ul_count.i32 = 0;
        amf_ue->ul_count_accepted = false;
    }

    if (amf_ue->selected_enc_algorithm == 0)
        ciphered = 0;
    if (amf_ue->selected_int_algorithm == 0)
        integrity_protected = 0;

    memset(&h, 0, sizeof(h));
    h.security_header_type = message->h.security_header_type;
    h.extended_protocol_discriminator =
        message->h.extended_protocol_discriminator;
    h.sequence_number = (amf_ue->dl_count & 0xff);

    new = ogs_nas_5gs_plain_encode(message);
    if (!new) {
        ogs_error("ogs_nas_5gs_plain_encode() failed");
        return NULL;
    }

    if (ciphered) {
        /* encrypt NAS message */
        ogs_nas_encrypt(amf_ue->selected_enc_algorithm,
            amf_ue->knas_enc, amf_ue->dl_count,
            amf_ue->nas.access_type,
            OGS_NAS_SECURITY_DOWNLINK_DIRECTION, new);
    }

    /* encode sequence number */
    ogs_assert(ogs_pkbuf_push(new, 1));
    *(uint8_t *)(new->data) = h.sequence_number;

    if (integrity_protected) {
        uint8_t mac[NAS_SECURITY_MAC_SIZE];

        /* calculate NAS MAC(message authentication code) */
        ogs_nas_mac_calculate(amf_ue->selected_int_algorithm,
            amf_ue->knas_int, amf_ue->dl_count,
            amf_ue->nas.access_type,
            OGS_NAS_SECURITY_DOWNLINK_DIRECTION, new, mac);
        memcpy(&h.message_authentication_code, mac, sizeof(mac));
    }

    /* increase dl_count */
    amf_ue->dl_count = (amf_ue->dl_count + 1) & 0xffffff; /* Use 24bit */

    /* encode all security header */
    ogs_assert(ogs_pkbuf_push(new, 6));
    memcpy(new->data, &h, sizeof(ogs_nas_5gs_security_header_t));

    amf_ue->security_context_available = 1;

    return new;
}

int nas_5gs_security_decode(amf_ue_t *amf_ue,
    ogs_nas_security_header_type_t security_header_type, ogs_pkbuf_t *pkbuf)
{
    ogs_assert(amf_ue);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);

    if (!amf_ue->security_context_available) {
        security_header_type.integrity_protected = 0;
        security_header_type.new_security_context = 0;
        security_header_type.ciphered = 0;
    }

    if (amf_ue->selected_enc_algorithm == 0)
        security_header_type.ciphered = 0;
    if (amf_ue->selected_int_algorithm == 0)
        security_header_type.integrity_protected = 0;

    if (security_header_type.ciphered ||
        security_header_type.integrity_protected) {
        ogs_nas_5gs_security_header_t *h = NULL;
        uint32_t estimated_ul_count = 0;

        /* NAS Security Header */
        ogs_assert(ogs_pkbuf_push(pkbuf, 7));
        h = (ogs_nas_5gs_security_header_t *)pkbuf->data;

        /* NAS Security Header.Sequence_Number */
        ogs_assert(ogs_pkbuf_pull(pkbuf, 6));

        /*
         * TS24.501 4.4.3.1
         *
         * Estimate the uplink NAS COUNT of the received message from the
         * locally stored overflow counter and the received sequence number.
         *
         * The estimate is kept local. amf_ue->ul_count is the NAS COUNT of
         * the last *accepted* uplink NAS message and must never be advanced
         * by a sequence number that has not been authenticated yet.
         */
        estimated_ul_count = amf_ue->ul_count.i32 & 0x00ffff00;
        if (amf_ue->ul_count.sqn > h->sequence_number)
            estimated_ul_count = (estimated_ul_count + 0x100) & 0x00ffff00;
        estimated_ul_count |= h->sequence_number;

        if (security_header_type.integrity_protected) {
            uint8_t mac[NAS_SECURITY_MAC_SIZE];
            uint32_t mac32;
            uint32_t original_mac = h->message_authentication_code;

            /* calculate NAS MAC(message authentication code) */
            ogs_nas_mac_calculate(amf_ue->selected_int_algorithm,
                amf_ue->knas_int, estimated_ul_count,
                amf_ue->nas.access_type,
                OGS_NAS_SECURITY_UPLINK_DIRECTION, pkbuf, mac);
            h->message_authentication_code = original_mac;

            memcpy(&mac32, mac, NAS_SECURITY_MAC_SIZE);
            if (h->message_authentication_code != mac32) {
                ogs_warn("NAS MAC verification failed(0x%x != 0x%x)",
                    be32toh(h->message_authentication_code), be32toh(mac32));
                amf_ue->mac_failed = 1;
            } else {
                /*
                 * TS33.501 6.4.3.1 (NAS integrity and replay protection)
                 *
                 * A valid MAC only proves that the message was produced by a
                 * holder of KNASint at that NAS COUNT. A replayed message
                 * carries a valid MAC as well, so the NAS COUNT must also be
                 * greater than the NAS COUNT of the last accepted uplink NAS
                 * message in this security context.
                 */
                if (amf_ue->ul_count_accepted == true &&
                    estimated_ul_count <= amf_ue->ul_count.i32) {
                    ogs_error("NAS COUNT REPLAY DETECTED - uplink NAS "
                        "message rejected "
                        "[UL NAS COUNT:0x%06x, LAST ACCEPTED:0x%06x]",
                        estimated_ul_count, amf_ue->ul_count.i32);
                    return OGS_ERROR;
                }

                amf_ue->ul_count.i32 = estimated_ul_count;
                amf_ue->ul_count_accepted = true;
            }
        } else {
            /*
             * No integrity protection (NIA0). The sequence number cannot be
             * authenticated, so replay protection is not possible here.
             */
            amf_ue->ul_count.i32 = estimated_ul_count;
        }

        /* NAS EMM Header or ESM Header */
        ogs_assert(ogs_pkbuf_pull(pkbuf, 1));

        if (security_header_type.ciphered) {
            /* decrypt NAS message */
            if (pkbuf->len == 0) {
                ogs_error("Cannot decrypt Malformed NAS Message");
                return OGS_ERROR;
            }
            ogs_nas_encrypt(amf_ue->selected_enc_algorithm,
                amf_ue->knas_enc, estimated_ul_count,
                amf_ue->nas.access_type,
                OGS_NAS_SECURITY_UPLINK_DIRECTION, pkbuf);
        }
    }

    return OGS_OK;
}
