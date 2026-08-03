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

#include "nas-security.h"

#define NAS_SECURITY_BEARER 0
#define NAS_SECURITY_MAC_SIZE 4

ogs_pkbuf_t *nas_eps_security_encode(
        mme_ue_t *mme_ue, ogs_nas_eps_message_t *message)
{
    int integrity_protected = 0;
    int new_security_context = 0;
    int ciphered = 0;
    ogs_nas_eps_security_header_t h;
    ogs_pkbuf_t *new = NULL;

    ogs_assert(mme_ue);
    ogs_assert(message);

    switch (message->h.security_header_type) {
    case OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
        return ogs_nas_eps_plain_encode(message);
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
        ogs_error("Not implemented(securiry header type:0x%x)", 
                message->h.security_header_type);
        return NULL;
    }

    if (new_security_context) {
        mme_ue->dl_count = 0;
        mme_ue->ul_count.i32 = 0;
        mme_ue->ul_count_accepted = false;
    }

    if (mme_ue->selected_enc_algorithm == 0)
        ciphered = 0;
    if (mme_ue->selected_int_algorithm == 0)
        integrity_protected = 0;

    memset(&h, 0, sizeof(h));
    h.security_header_type = message->h.security_header_type;
    h.protocol_discriminator = message->h.protocol_discriminator;
    h.sequence_number = (mme_ue->dl_count & 0xff);

    new = ogs_nas_eps_plain_encode(message);
    if (!new) {
        ogs_error("ogs_nas_eps_plain_encode() failed");
        return NULL;
    }

    if (ciphered) {
        /* encrypt NAS message */
        ogs_nas_encrypt(mme_ue->selected_enc_algorithm,
            mme_ue->knas_enc, mme_ue->dl_count, NAS_SECURITY_BEARER,
            OGS_NAS_SECURITY_DOWNLINK_DIRECTION, new);
    }

    /* encode sequence number */
    ogs_assert(ogs_pkbuf_push(new, 1));
    *(uint8_t *)(new->data) = h.sequence_number;

    if (integrity_protected) {
        uint8_t mac[NAS_SECURITY_MAC_SIZE];

        /* calculate NAS MAC(message authentication code) */
        ogs_nas_mac_calculate(mme_ue->selected_int_algorithm,
            mme_ue->knas_int, mme_ue->dl_count, NAS_SECURITY_BEARER, 
            OGS_NAS_SECURITY_DOWNLINK_DIRECTION, new, mac);
        memcpy(&h.message_authentication_code, mac, sizeof(mac));
    }

    /* increase dl_count */
    mme_ue->dl_count = (mme_ue->dl_count + 1) & 0xffffff; /* Use 24bit */

    /* encode all security header */
    ogs_assert(ogs_pkbuf_push(new, 5));
    memcpy(new->data, &h, sizeof(ogs_nas_eps_security_header_t));

    mme_ue->security_context_available = 1;

    return new;
}

int nas_eps_security_decode(mme_ue_t *mme_ue, 
    ogs_nas_security_header_type_t security_header_type, ogs_pkbuf_t *pkbuf)
{
    ogs_assert(mme_ue);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);

    if (security_header_type.service_request) {
#define SHORT_MAC_SIZE 2
        ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number =
            (ogs_nas_ksi_and_sequence_number_t *)(pkbuf->data + 1);
        uint8_t original_mac[SHORT_MAC_SIZE];
        uint8_t estimated_sequence_number;
        uint8_t sequence_number_high_3bit;
        uint8_t mac[NAS_SECURITY_MAC_SIZE];
        uint32_t estimated_ul_count = 0;

        if (mme_ue->selected_int_algorithm == 0) {
            ogs_warn("integrity algorithm is not defined");
            return OGS_ERROR;
        }

        ogs_assert(ksi_and_sequence_number);
        estimated_sequence_number = 
            ksi_and_sequence_number->sequence_number;

        sequence_number_high_3bit = mme_ue->ul_count.sqn & 0xe0;
        if ((mme_ue->ul_count.sqn & 0x1f) > estimated_sequence_number) {
            sequence_number_high_3bit += 0x20;
        }
        estimated_sequence_number += sequence_number_high_3bit;

        /*
         * TS24.301 4.4.3.1
         *
         * The estimate is kept local. mme_ue->ul_count is the NAS COUNT of
         * the last *accepted* uplink NAS message and must never be advanced
         * by a sequence number that has not been authenticated yet.
         */
        estimated_ul_count = mme_ue->ul_count.i32 & 0x00ffff00;
        if (mme_ue->ul_count.sqn > estimated_sequence_number)
            estimated_ul_count = (estimated_ul_count + 0x100) & 0x00ffff00;
        estimated_ul_count |= estimated_sequence_number;

        memcpy(original_mac, pkbuf->data + 2, SHORT_MAC_SIZE);

        ogs_pkbuf_trim(pkbuf, 2);
        ogs_nas_mac_calculate(mme_ue->selected_int_algorithm,
            mme_ue->knas_int, estimated_ul_count, NAS_SECURITY_BEARER,
            OGS_NAS_SECURITY_UPLINK_DIRECTION, pkbuf, mac);

        ogs_pkbuf_put_data(pkbuf, original_mac, SHORT_MAC_SIZE);
        if (memcmp(mac + 2, pkbuf->data + 2, 2) != 0) {
            ogs_warn("NAS MAC verification failed(%x%x != %x%x)",
                    mac[2], mac[3],
                    ((unsigned char *)pkbuf->data)[2],
                    ((unsigned char *)pkbuf->data)[3]);

            mme_ue->mac_failed = 1;
        } else {
            /* TS33.401 8.1.1 : replay protection for uplink NAS */
            if (mme_ue->ul_count_accepted == true &&
                estimated_ul_count <= mme_ue->ul_count.i32) {
                ogs_error("NAS COUNT REPLAY DETECTED - service request "
                    "rejected "
                    "[UL NAS COUNT:0x%06x, LAST ACCEPTED:0x%06x]",
                    estimated_ul_count, mme_ue->ul_count.i32);
                return OGS_ERROR;
            }

            mme_ue->ul_count.i32 = estimated_ul_count;
            mme_ue->ul_count_accepted = true;
        }

        return OGS_OK;
    }

    if (!mme_ue->security_context_available) {
        security_header_type.integrity_protected = 0;
        security_header_type.new_security_context = 0;
        security_header_type.ciphered = 0;
    }

    if (mme_ue->selected_enc_algorithm == 0)
        security_header_type.ciphered = 0;
    if (mme_ue->selected_int_algorithm == 0)
        security_header_type.integrity_protected = 0;

    if (security_header_type.ciphered || 
        security_header_type.integrity_protected) {
        ogs_nas_eps_security_header_t *h = NULL;
        uint32_t estimated_ul_count = 0;

        /* NAS Security Header */
        ogs_assert(ogs_pkbuf_push(pkbuf, 6));
        h = (ogs_nas_eps_security_header_t *)pkbuf->data;

        /* NAS Security Header.Sequence_Number */
        ogs_assert(ogs_pkbuf_pull(pkbuf, 5));

        /*
         * TS24.301 4.4.3.1
         *
         * Estimate the uplink NAS COUNT of the received message from the
         * locally stored overflow counter and the received sequence number.
         *
         * The estimate is kept local. mme_ue->ul_count is the NAS COUNT of
         * the last *accepted* uplink NAS message and must never be advanced
         * by a sequence number that has not been authenticated yet.
         */
        estimated_ul_count = mme_ue->ul_count.i32 & 0x00ffff00;
        if (mme_ue->ul_count.sqn > h->sequence_number)
            estimated_ul_count = (estimated_ul_count + 0x100) & 0x00ffff00;
        estimated_ul_count |= h->sequence_number;

        if (security_header_type.integrity_protected) {
            uint8_t mac[NAS_SECURITY_MAC_SIZE];
            uint32_t mac32;
            uint32_t original_mac = h->message_authentication_code;

            /* calculate NAS MAC(message authentication code) */
            ogs_nas_mac_calculate(mme_ue->selected_int_algorithm,
                mme_ue->knas_int, estimated_ul_count, NAS_SECURITY_BEARER,
                OGS_NAS_SECURITY_UPLINK_DIRECTION, pkbuf, mac);
            h->message_authentication_code = original_mac;

            memcpy(&mac32, mac, NAS_SECURITY_MAC_SIZE);
            if (h->message_authentication_code != mac32) {
                ogs_warn("NAS MAC verification failed(0x%x != 0x%x)",
                    be32toh(h->message_authentication_code), be32toh(mac32));
                mme_ue->mac_failed = 1;
            } else {
                /*
                 * TS33.401 8.1.1 (NAS integrity and replay protection)
                 *
                 * A valid MAC only proves that the message was produced by a
                 * holder of KNASint at that NAS COUNT. A replayed message
                 * carries a valid MAC as well, so the NAS COUNT must also be
                 * greater than the NAS COUNT of the last accepted uplink NAS
                 * message in this security context.
                 */
                if (mme_ue->ul_count_accepted == true &&
                    estimated_ul_count <= mme_ue->ul_count.i32) {
                    ogs_error("NAS COUNT REPLAY DETECTED - uplink NAS "
                        "message rejected "
                        "[UL NAS COUNT:0x%06x, LAST ACCEPTED:0x%06x]",
                        estimated_ul_count, mme_ue->ul_count.i32);
                    return OGS_ERROR;
                }

                mme_ue->ul_count.i32 = estimated_ul_count;
                mme_ue->ul_count_accepted = true;
            }
        } else {
            /*
             * No integrity protection (EIA0). The sequence number cannot be
             * authenticated, so replay protection is not possible here.
             */
            mme_ue->ul_count.i32 = estimated_ul_count;
        }

        /* NAS EMM Header or ESM Header */
        ogs_assert(ogs_pkbuf_pull(pkbuf, 1));

        if (security_header_type.ciphered) {
            /* decrypt NAS message */
            if (pkbuf->len == 0) {
                ogs_error("Cannot decrypt Malformed NAS Message");
                return OGS_ERROR;
            }
            ogs_nas_encrypt(mme_ue->selected_enc_algorithm,
                mme_ue->knas_enc, estimated_ul_count, NAS_SECURITY_BEARER,
                OGS_NAS_SECURITY_UPLINK_DIRECTION, pkbuf);
        }
    }

    return OGS_OK;
}
