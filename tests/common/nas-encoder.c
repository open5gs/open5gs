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

int ogs_nas_5gs_encode_deregistration_request_from_ue(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);

ogs_pkbuf_t *test_nas_5gmm_encode(ogs_nas_5gs_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM.
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_5gmm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));

    memcpy(pkbuf->data - size, &message->gmm.h, size);
    encoded += size;

    switch(message->gmm.h.message_type) {
    case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
        size = ogs_nas_5gs_encode_deregistration_request_from_ue(
                pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented",
                message->gmm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));

    pkbuf->len = encoded;

    return pkbuf;
}

ogs_pkbuf_t *test_nas_5gs_plain_encode(ogs_nas_5gs_message_t *message)
{
    ogs_assert(message);

    ogs_assert(message->gmm.h.extended_protocol_discriminator ==
            message->gsm.h.extended_protocol_discriminator);

    if (message->gmm.h.extended_protocol_discriminator ==
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM)
        return test_nas_5gmm_encode(message);

    ogs_error("Unknown discriminator [%d]",
        message->gmm.h.extended_protocol_discriminator);

    return NULL;
}

int ogs_nas_eps_encode_detach_request_from_ue(
        ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);

ogs_pkbuf_t *test_nas_emm_encode(ogs_nas_eps_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM.
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_emm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));

    memcpy(pkbuf->data - size, &message->emm.h, size);
    encoded += size;

    switch(message->emm.h.message_type) {
    case OGS_NAS_EPS_DETACH_REQUEST:
        size = ogs_nas_eps_encode_detach_request_from_ue(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented",
                message->emm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));

    pkbuf->len = encoded;

    return pkbuf;
}

ogs_pkbuf_t *test_nas_eps_plain_encode(ogs_nas_eps_message_t *message)
{
    ogs_assert(message);

    ogs_assert(message->emm.h.protocol_discriminator ==
            message->emm.h.protocol_discriminator);

    if (message->emm.h.protocol_discriminator ==
            OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM)
        return test_nas_emm_encode(message);

    ogs_error("Unknown discriminator [%d]",
        message->emm.h.protocol_discriminator);

    return NULL;
}
