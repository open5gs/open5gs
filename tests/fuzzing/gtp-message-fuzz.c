/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include <stdio.h>
#include <stdint.h>

#include "fuzzing.h"
#include "ogs-gtp.h"

#define kMinInputLength 5
#define kMaxInputLength 1024
#define GTP_VERSION(byte) (((byte) >> 5) & 0x07)

static void parse_gtp1_ies(ogs_gtp1_message_t *message)
{
    ogs_tlv_octet_t *location = NULL;
    ogs_tlv_octet_t *qos = NULL;

    switch (message->h.type) {
    case OGS_GTP1_CREATE_PDP_CONTEXT_REQUEST_TYPE:
        location = &message->create_pdp_context_request.
            user_location_information;
        qos = &message->create_pdp_context_request.quality_of_service_profile;
        break;
    case OGS_GTP1_UPDATE_PDP_CONTEXT_REQUEST_TYPE:
        location = &message->update_pdp_context_request.
            user_location_information;
        qos = &message->update_pdp_context_request.quality_of_service_profile;
        break;
    case OGS_GTP1_SGSN_CONTEXT_RESPONSE_TYPE: {
        ogs_gtp1_sgsn_context_response_t *response =
            &message->sgsn_context_response;
        if (response->mm_context.presence && response->mm_context.len &&
                response->mm_context.data) {
            ogs_gtp1_mm_context_decoded_t decoded;
            ogs_gtp1_parse_mm_context(&decoded, &response->mm_context);
        }
        if (response->pdp_context.presence && response->pdp_context.len &&
                response->pdp_context.data) {
            ogs_gtp1_pdp_context_decoded_t decoded;
            ogs_gtp1_parse_pdp_context(&decoded, &response->pdp_context);
        }
        break;
    }
    default:
        break;
    }

    if (location && location->presence && location->len && location->data) {
        ogs_gtp1_uli_t decoded;
        ogs_gtp1_parse_uli(&decoded, location);
    }
    if (qos && qos->presence && qos->len && qos->data) {
        ogs_gtp1_qos_profile_decoded_t decoded;
        ogs_gtp1_parse_qos_profile(&decoded, qos);
    }
}

static void parse_bearer_ies(ogs_gtp2_tlv_bearer_context_t *bearer)
{
    if (!bearer->presence)
        return;

    if (bearer->bearer_level_qos.presence && bearer->bearer_level_qos.len &&
            bearer->bearer_level_qos.data) {
        ogs_gtp2_bearer_qos_t decoded;
        ogs_gtp2_parse_bearer_qos(&decoded, &bearer->bearer_level_qos);
    }
    if (bearer->tft.presence && bearer->tft.len && bearer->tft.data) {
        ogs_gtp2_tft_t decoded;
        ogs_gtp2_parse_tft(&decoded, &bearer->tft);
    }
}

static void parse_gtp2_ies(ogs_gtp2_message_t *message)
{
    ogs_tlv_octet_t *location = NULL;

    switch (message->h.type) {
    case OGS_GTP2_CREATE_SESSION_REQUEST_TYPE: {
        ogs_gtp2_create_session_request_t *request =
            &message->create_session_request;
        size_t index;

        location = &request->user_location_information;
        for (index = 0;
                index < OGS_ARRAY_SIZE(request->bearer_contexts_to_be_created);
                index++)
            parse_bearer_ies(&request->bearer_contexts_to_be_created[index]);
        break;
    }
    case OGS_GTP2_MODIFY_BEARER_REQUEST_TYPE:
        location = &message->modify_bearer_request.user_location_information;
        break;
    case OGS_GTP2_CREATE_BEARER_REQUEST_TYPE:
        parse_bearer_ies(&message->create_bearer_request.bearer_contexts);
        break;
    case OGS_GTP2_BEARER_RESOURCE_COMMAND_TYPE: {
        ogs_gtp2_bearer_resource_command_t *command =
            &message->bearer_resource_command;

        location = &command->user_location_information;
        if (command->flow_quality_of_service.presence &&
                command->flow_quality_of_service.len &&
                command->flow_quality_of_service.data) {
            ogs_gtp2_flow_qos_t decoded;
            ogs_gtp2_parse_flow_qos(&decoded, &command->flow_quality_of_service);
        }
        if (command->traffic_aggregate_description.presence &&
                command->traffic_aggregate_description.len &&
                command->traffic_aggregate_description.data) {
            ogs_gtp2_tft_t decoded;
            ogs_gtp2_parse_tft(&decoded, &command->traffic_aggregate_description);
        }
        break;
    }
    default:
        break;
    }

    if (location && location->presence && location->len && location->data) {
        ogs_gtp2_uli_t decoded;
        ogs_gtp2_parse_uli(&decoded, location);
    }
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) 
{ /* open5gs/tests/non3gpp/gtp-path.c */

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 1;
    }

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_gtp_domain, "gtp", OGS_LOG_NONE);
        ogs_log_install_domain(&__ogs_tlv_domain, "tlv", OGS_LOG_NONE);
    }

    ogs_pkbuf_t *pkbuf;
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);

    if (pkbuf == NULL) {
        return 1;
    }
    ogs_pkbuf_put_data(pkbuf, Data, Size);

    if (GTP_VERSION(Data[0]) == OGS_GTP1_VERSION_1) {
        /* GTPv1 */
        ogs_gtp1_message_t gtp1_message;
        if (ogs_gtp1_parse_msg(&gtp1_message, pkbuf) == OGS_OK)
            parse_gtp1_ies(&gtp1_message);
    } else {
        /* GTPv2 */
        ogs_gtp2_message_t gtp2_message;
        if (ogs_gtp2_parse_msg(&gtp2_message, pkbuf) == OGS_OK)
            parse_gtp2_ies(&gtp2_message);
    }

    ogs_pkbuf_free(pkbuf);

    return 0;
}
