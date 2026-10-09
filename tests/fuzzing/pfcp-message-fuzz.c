/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
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
#include "ogs-pfcp.h"

#define kMinInputLength 5
#define kMaxInputLength 2048

static void parse_pdr_ies(ogs_pfcp_tlv_create_pdr_t *pdr)
{
    size_t index;

    if (!pdr->presence || !pdr->pdi.presence)
        return;

    for (index = 0; index < OGS_ARRAY_SIZE(pdr->pdi.sdf_filter); index++) {
        ogs_tlv_octet_t *filter = &pdr->pdi.sdf_filter[index];
        if (filter->presence && filter->len && filter->data) {
            ogs_pfcp_sdf_filter_t decoded;
            ogs_pfcp_parse_sdf_filter(&decoded, filter);
        }
    }
}

static void parse_qer_ies(ogs_pfcp_tlv_create_qer_t *qer)
{
    ogs_tlv_octet_t *rates[] = {
        &qer->maximum_bitrate, &qer->guaranteed_bitrate
    };
    size_t index;

    if (!qer->presence)
        return;

    for (index = 0; index < OGS_ARRAY_SIZE(rates); index++) {
        ogs_tlv_octet_t *rate = rates[index];
        if (rate->presence && rate->len && rate->data) {
            ogs_pfcp_bitrate_t decoded;
            ogs_pfcp_parse_bitrate(&decoded, rate);
        }
    }
}

static void parse_urr_ies(ogs_pfcp_tlv_create_urr_t *urr)
{
    ogs_tlv_octet_t *volumes[] = {
        &urr->volume_threshold, &urr->volume_quota,
        &urr->subsequent_volume_threshold, &urr->subsequent_volume_quota
    };
    ogs_tlv_octet_t *dropped = &urr->dropped_dl_traffic_threshold;
    size_t index;

    if (!urr->presence)
        return;

    for (index = 0; index < OGS_ARRAY_SIZE(volumes); index++) {
        ogs_tlv_octet_t *volume = volumes[index];
        if (volume->presence && volume->len && volume->data) {
            ogs_pfcp_volume_threshold_t decoded;
            ogs_pfcp_parse_volume(&decoded, volume);
        }
    }
    if (dropped->presence && dropped->len && dropped->data) {
        ogs_pfcp_dropped_dl_traffic_threshold_t decoded;
        ogs_pfcp_parse_dropped_dl_traffic_threshold(&decoded, dropped);
    }
}

static void parse_pfcp_ies(ogs_pfcp_message_t *message)
{
    ogs_pfcp_tlv_create_pdr_t *pdr = NULL;
    ogs_pfcp_tlv_create_qer_t *qer = NULL;
    ogs_pfcp_tlv_create_urr_t *urr = NULL;
    size_t pdr_count = 0, qer_count = 0, urr_count = 0;
    size_t index;

    switch (message->h.type) {
    case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE: {
        ogs_pfcp_session_establishment_request_t *request =
            &message->pfcp_session_establishment_request;
        pdr = request->create_pdr;
        pdr_count = OGS_ARRAY_SIZE(request->create_pdr);
        qer = request->create_qer;
        qer_count = OGS_ARRAY_SIZE(request->create_qer);
        urr = request->create_urr;
        urr_count = OGS_ARRAY_SIZE(request->create_urr);
        break;
    }
    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE: {
        ogs_pfcp_session_modification_request_t *request =
            &message->pfcp_session_modification_request;
        pdr = request->create_pdr;
        pdr_count = OGS_ARRAY_SIZE(request->create_pdr);
        qer = request->create_qer;
        qer_count = OGS_ARRAY_SIZE(request->create_qer);
        urr = request->create_urr;
        urr_count = OGS_ARRAY_SIZE(request->create_urr);
        break;
    }
    case OGS_PFCP_SESSION_REPORT_REQUEST_TYPE: {
        ogs_pfcp_session_report_request_t *request =
            &message->pfcp_session_report_request;
        for (index = 0; index < OGS_ARRAY_SIZE(request->usage_report); index++) {
            ogs_pfcp_tlv_usage_report_session_report_request_t *report =
                &request->usage_report[index];
            ogs_tlv_octet_t *volume = &report->volume_measurement;
            if (report->presence && volume->presence && volume->len &&
                    volume->data) {
                ogs_pfcp_volume_measurement_t decoded;
                ogs_pfcp_parse_volume_measurement(&decoded, volume);
            }
        }
        break;
    }
    default:
        break;
    }

    for (index = 0; index < pdr_count; index++)
        parse_pdr_ies(&pdr[index]);
    for (index = 0; index < qer_count; index++)
        parse_qer_ies(&qer[index]);
    for (index = 0; index < urr_count; index++)
        parse_urr_ies(&urr[index]);
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 1;
    }

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_pfcp_domain, "pfcp", OGS_LOG_NONE);
        ogs_log_install_domain(&__ogs_tlv_domain, "tlv", OGS_LOG_NONE);
    }

    ogs_pkbuf_t *pkbuf;
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    if (pkbuf == NULL) {
        return 1;
    }
    ogs_pkbuf_put_data(pkbuf, Data, Size);

    ogs_pfcp_message_t *message = ogs_pfcp_parse_msg(pkbuf);
    if (message) {
        parse_pfcp_ies(message);
        ogs_pfcp_message_free(message);
    }

    ogs_pkbuf_free(pkbuf);

    return 0;
}
