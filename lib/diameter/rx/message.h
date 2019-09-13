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

#if !defined(OGS_DIAMETER_INSIDE) && !defined(OGS_DIAMETER_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DIAM_RX_MESSAGE_H
#define OGS_DIAM_RX_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_diam_rx_media_sub_component_s {
    uint32_t            flow_number;
#define OGS_DIAM_RX_FLOW_USAGE_NO_INFORMATION       0
#define OGS_DIAM_RX_FLOW_USAGE_RTCP                 1
#define OGS_DIAM_RX_FLOW_USAGE_AF_SIGNALLING        2
    uint32_t            flow_usage;
    ogs_flow_t          flow[OGS_MAX_NUM_OF_FLOW];
    int                 num_of_flow;
} ogs_diam_rx_media_sub_component_t;

typedef struct ogs_diam_rx_media_component_s {
    uint32_t            media_component_number;
#define OGS_DIAM_RX_MEDIA_TYPE_AUDIO                0
#define OGS_DIAM_RX_MEDIA_TYPE_VIDEO                1
#define OGS_DIAM_RX_MEDIA_TYPE_DATA                 2
#define OGS_DIAM_RX_MEDIA_TYPE_APPLICATION          3
#define OGS_DIAM_RX_MEDIA_TYPE_CONTROL              4
#define OGS_DIAM_RX_MEDIA_TYPE_TEXT                 5
#define OGS_DIAM_RX_MEDIA_TYPE_MESSAGE              6
#define OGS_DIAM_RX_MEDIA_TYPE_OTHER                0xFFFFFFFF
    uint32_t            media_type;

    uint64_t            max_requested_bandwidth_dl; 
    uint64_t            max_requested_bandwidth_ul;
    uint64_t            min_requested_bandwidth_dl; 
    uint64_t            min_requested_bandwidth_ul;
    uint64_t            rr_bandwidth;
    uint64_t            rs_bandwidth;

#define OGS_DIAM_MAX_NUM_OF_MEDIA_SUB_COMPONENT     8
    ogs_diam_rx_media_sub_component_t
        sub[OGS_DIAM_MAX_NUM_OF_MEDIA_SUB_COMPONENT];
    int                 num_of_sub;
} ogs_diam_rx_media_component_t;

typedef struct ogs_diam_rx_message_s {
#define OGS_DIAM_RX_CMD_CODE_AA                     265
#define OGS_DIAM_RX_CMD_CODE_SESSION_TERMINATION    275
    uint16_t          cmd_code;

/* Experimental-Result-Codes */
#define OGS_DIAM_RX_DIAMETER_INVALID_SERVICE_INFORMATION        5061
#define OGS_DIAM_RX_DIAMETER_FILTER_RESTRICTIONS                5062
#define OGS_DIAM_RX_DIAMETER_REQUESTED_SERVICE_NOT_AUTHORIZED   5063
#define OGS_DIAM_RX_DIAMETER_DUPLICATED_AF_SESSION              5064
#define OGS_DIAM_RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE       5065
#define OGS_DIAM_RX_DIAMETER_UNAUTHORIZED_NON_EMERGENCY_SESSION 5066
#define OGS_DIAM_RX_DIAMETER_UNAUTHORIZED_SPONSORED_DATA_CONNECTIVITY 5067
#define OGS_DIAM_RX_DIAMETER_TEMPORARY_NETWORK_FAILURE          5068
    uint32_t          result_code;

#define OGS_DIAM_MAX_NUM_OF_MEDIA_COMPONENT 16
    ogs_diam_rx_media_component_t
        media_component[OGS_DIAM_MAX_NUM_OF_MEDIA_COMPONENT];
    int num_of_media_component;
} ogs_diam_rx_message_t;

void ogs_diam_rx_message_free(ogs_diam_rx_message_t *rx_message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_OGS_DIAM_RX_MESSAGE_H */
