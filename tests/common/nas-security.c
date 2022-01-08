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

#include "test-common.h"

#define NAS_SECURITY_BEARER 0
#define NAS_SECURITY_MAC_SIZE 4

ogs_pkbuf_t *test_nas_5gs_security_encode(
        test_ue_t *test_ue, ogs_nas_5gs_message_t *message)
{
    int integrity_protected = 0;
    int new_security_context = 0;
    int ciphered = 0;
    ogs_nas_5gs_security_header_t h;
    ogs_pkbuf_t *new = NULL;

    ogs_assert(test_ue);
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
        ogs_error("Not implemented(securiry header type:0x%x)",
                message->h.security_header_type);
        return NULL;
    }

    if (new_security_context) {
        test_ue->ul_count = 0;
        test_ue->dl_count.i32 = 0;
    }

    if (test_ue->selected_enc_algorithm == 0)
        ciphered = 0;
    if (test_ue->selected_int_algorithm == 0)
        integrity_protected = 0;

    memset(&h, 0, sizeof(h));
    h.security_header_type = message->h.security_header_type;
    h.extended_protocol_discriminator =
        message->h.extended_protocol_discriminator;
    h.sequence_number = (test_ue->ul_count & 0xff);

    if (message->gmm.h.message_type == OGS_NAS_5GS_DEREGISTRATION_REQUEST)
        new = test_nas_5gs_plain_encode(message);
    else
        new = ogs_nas_5gs_plain_encode(message);
    if (!new) {
        ogs_error("ogs_nas_5gs_plain_encode() failed");
        return NULL;
    }

    if (ciphered) {
        /* encrypt NAS message */
        ogs_nas_encrypt(test_ue->selected_enc_algorithm,
            test_ue->knas_enc, test_ue->ul_count,
            test_ue->nas.access_type,
            OGS_NAS_SECURITY_UPLINK_DIRECTION, new);
    }

    /* encode sequence number */
    ogs_assert(ogs_pkbuf_push(new, 1));
    *(uint8_t *)(new->data) = h.sequence_number;

    if (integrity_protected) {
        uint8_t mac[NAS_SECURITY_MAC_SIZE];

        /* calculate NAS MAC(message authentication code) */
        ogs_nas_mac_calculate(test_ue->selected_int_algorithm,
            test_ue->knas_int, test_ue->ul_count,
            test_ue->nas.access_type,
            OGS_NAS_SECURITY_UPLINK_DIRECTION, new, mac);
        memcpy(&h.message_authentication_code, mac, sizeof(mac));
    }

    /* increase ul_count */
    test_ue->ul_count = (test_ue->ul_count + 1) & 0xffffff; /* Use 24bit */

    /* encode all security header */
    ogs_assert(ogs_pkbuf_push(new, 6));
    memcpy(new->data, &h, sizeof(ogs_nas_5gs_security_header_t));

    return new;
}

int test_nas_5gs_security_decode(test_ue_t *test_ue,
    ogs_nas_security_header_type_t security_header_type, ogs_pkbuf_t *pkbuf)
{
    ogs_assert(test_ue);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);

    if (!test_ue->security_context_available) {
        security_header_type.integrity_protected = 0;
        security_header_type.new_security_context = 0;
        security_header_type.ciphered = 0;
    }

    if (security_header_type.new_security_context) {
        test_ue->dl_count.i32 = 0;
    }

    if (test_ue->selected_enc_algorithm == 0)
        security_header_type.ciphered = 0;
    if (test_ue->selected_int_algorithm == 0)
        security_header_type.integrity_protected = 0;

    if (security_header_type.ciphered ||
        security_header_type.integrity_protected) {
        ogs_nas_5gs_security_header_t *h = NULL;

        /* NAS Security Header */
        ogs_assert(ogs_pkbuf_push(pkbuf, 7));
        h = (ogs_nas_5gs_security_header_t *)pkbuf->data;

        /* NAS Security Header.Sequence_Number */
        ogs_assert(ogs_pkbuf_pull(pkbuf, 6));

        /* calculate dl_count */
        if (test_ue->dl_count.sqn > h->sequence_number)
            test_ue->dl_count.overflow++;
        test_ue->dl_count.sqn = h->sequence_number;

        if (security_header_type.integrity_protected) {
            uint8_t mac[NAS_SECURITY_MAC_SIZE];
            uint32_t mac32;
            uint32_t original_mac = h->message_authentication_code;

            /* calculate NAS MAC(message authentication code) */
            ogs_nas_mac_calculate(test_ue->selected_int_algorithm,
                test_ue->knas_int, test_ue->dl_count.i32,
                test_ue->nas.access_type,
                OGS_NAS_SECURITY_DOWNLINK_DIRECTION, pkbuf, mac);
            h->message_authentication_code = original_mac;

            memcpy(&mac32, mac, NAS_SECURITY_MAC_SIZE);
            if (h->message_authentication_code != mac32) {
                ogs_error("NAS MAC verification failed(0x%x != 0x%x)",
                        ntohl(h->message_authentication_code), ntohl(mac32));
                test_ue->mac_failed = 1;
            }
        }

        /* NAS EMM Header or ESM Header */
        ogs_assert(ogs_pkbuf_pull(pkbuf, 1));

        if (security_header_type.ciphered) {
            /* decrypt NAS message */
            ogs_nas_encrypt(test_ue->selected_enc_algorithm,
                test_ue->knas_enc, test_ue->dl_count.i32,
                test_ue->nas.access_type,
                OGS_NAS_SECURITY_DOWNLINK_DIRECTION, pkbuf);
        }
    }

    return OGS_OK;
}

ogs_pkbuf_t *test_nas_eps_security_encode(
        test_ue_t *test_ue, ogs_nas_eps_message_t *message)
{
    int integrity_protected = 0;
    int new_security_context = 0;
    int ciphered = 0;
    ogs_nas_eps_security_header_t h;
    ogs_pkbuf_t *new = NULL;

    ogs_assert(test_ue);
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
        test_ue->ul_count = 0;
        test_ue->dl_count.i32 = 0;
    }

    if (test_ue->selected_enc_algorithm == 0)
        ciphered = 0;
    if (test_ue->selected_int_algorithm == 0)
        integrity_protected = 0;

    memset(&h, 0, sizeof(h));
    h.security_header_type = message->h.security_header_type;
    h.protocol_discriminator = message->h.protocol_discriminator;
    h.sequence_number = (test_ue->ul_count & 0xff);

    if (message->emm.h.message_type == OGS_NAS_EPS_DETACH_REQUEST)
        new = test_nas_eps_plain_encode(message);
    else
        new = ogs_nas_eps_plain_encode(message);
    if (!new) {
        ogs_error("ogs_nas_eps_plain_encode() failed");
        return NULL;
    }

    if (ciphered) {
        /* encrypt NAS message */
        ogs_nas_encrypt(test_ue->selected_enc_algorithm,
            test_ue->knas_enc, test_ue->ul_count,
            NAS_SECURITY_BEARER, OGS_NAS_SECURITY_UPLINK_DIRECTION, new);
    }

    /* encode sequence number */
    ogs_assert(ogs_pkbuf_push(new, 1));
    *(uint8_t *)(new->data) = h.sequence_number;

    if (integrity_protected) {
        uint8_t mac[NAS_SECURITY_MAC_SIZE];

        /* calculate NAS MAC(message authentication code) */
        ogs_nas_mac_calculate(test_ue->selected_int_algorithm,
            test_ue->knas_int, test_ue->ul_count,
            NAS_SECURITY_BEARER, OGS_NAS_SECURITY_UPLINK_DIRECTION, new, mac);
        memcpy(&h.message_authentication_code, mac, sizeof(mac));
    }

    /* increase ul_count */
    test_ue->ul_count = (test_ue->ul_count + 1) & 0xffffff; /* Use 24bit */

    /* encode all security header */
    ogs_assert(ogs_pkbuf_push(new, 5));
    memcpy(new->data, &h, sizeof(ogs_nas_eps_security_header_t));

    return new;
}

int test_nas_eps_security_decode(test_ue_t *test_ue,
    ogs_nas_security_header_type_t security_header_type, ogs_pkbuf_t *pkbuf)
{
    ogs_assert(test_ue);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);

    if (!test_ue->security_context_available) {
        security_header_type.integrity_protected = 0;
        security_header_type.new_security_context = 0;
        security_header_type.ciphered = 0;
    }

    if (security_header_type.new_security_context) {
        test_ue->dl_count.i32 = 0;
    }

    if (test_ue->selected_enc_algorithm == 0)
        security_header_type.ciphered = 0;
    if (test_ue->selected_int_algorithm == 0)
        security_header_type.integrity_protected = 0;

    if (security_header_type.ciphered ||
        security_header_type.integrity_protected) {
        ogs_nas_eps_security_header_t *h = NULL;

        /* NAS Security Header */
        ogs_assert(ogs_pkbuf_push(pkbuf, 6));
        h = (ogs_nas_eps_security_header_t *)pkbuf->data;

        /* NAS Security Header.Sequence_Number */
        ogs_assert(ogs_pkbuf_pull(pkbuf, 5));

        /* calculate dl_count */
        if (test_ue->dl_count.sqn > h->sequence_number)
            test_ue->dl_count.overflow++;
        test_ue->dl_count.sqn = h->sequence_number;

        if (security_header_type.integrity_protected) {
            uint8_t mac[NAS_SECURITY_MAC_SIZE];
            uint32_t mac32;
            uint32_t original_mac = h->message_authentication_code;

            /* calculate NAS MAC(message authentication code) */
            ogs_nas_mac_calculate(test_ue->selected_int_algorithm,
                test_ue->knas_int, test_ue->dl_count.i32,
                NAS_SECURITY_BEARER,
                OGS_NAS_SECURITY_DOWNLINK_DIRECTION, pkbuf, mac);
            h->message_authentication_code = original_mac;

            memcpy(&mac32, mac, NAS_SECURITY_MAC_SIZE);
            if (h->message_authentication_code != mac32) {
                ogs_error("NAS MAC verification failed(0x%x != 0x%x)",
                        ntohl(h->message_authentication_code), ntohl(mac32));
                test_ue->mac_failed = 1;
            }
        }

        /* NAS EMM Header or ESM Header */
        ogs_assert(ogs_pkbuf_pull(pkbuf, 1));

        if (security_header_type.ciphered) {
            /* decrypt NAS message */
            ogs_nas_encrypt(test_ue->selected_enc_algorithm,
                test_ue->knas_enc, test_ue->dl_count.i32,
                NAS_SECURITY_BEARER,
                OGS_NAS_SECURITY_DOWNLINK_DIRECTION, pkbuf);
        }
    }

    return OGS_OK;
}
