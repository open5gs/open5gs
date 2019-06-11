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

#ifndef RX_MESSAGE_H
#define RX_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "base/types.h"

typedef struct _rx_media_sub_component_t {
    uint32_t          flow_number;
#define RX_FLOW_USAGE_NO_INFORMATION        0
#define RX_FLOW_USAGE_RTCP                  1
#define RX_FLOW_USAGE_AF_SIGNALLING         2
    uint32_t          flow_usage;
    flow_t              flow[MAX_NUM_OF_FLOW];
    int                 num_of_flow;
} rx_media_sub_component_t;

typedef struct _rx_media_component_t {
    uint32_t          media_component_number;
#define RX_MEDIA_TYPE_AUDIO             0
#define RX_MEDIA_TYPE_VIDEO             1
#define RX_MEDIA_TYPE_DATA              2
#define RX_MEDIA_TYPE_APPLICATION       3
#define RX_MEDIA_TYPE_CONTROL           4
#define RX_MEDIA_TYPE_TEXT              5
#define RX_MEDIA_TYPE_MESSAGE           6
#define RX_MEDIA_TYPE_OTHER             0xFFFFFFFF
    uint32_t          media_type;

    uint64_t          max_requested_bandwidth_dl; 
    uint64_t          max_requested_bandwidth_ul;
    uint64_t          min_requested_bandwidth_dl; 
    uint64_t          min_requested_bandwidth_ul;
    uint64_t          rr_bandwidth;
    uint64_t          rs_bandwidth;

#define MAX_NUM_OF_MEDIA_SUB_COMPONENT            8
    rx_media_sub_component_t sub[MAX_NUM_OF_MEDIA_SUB_COMPONENT];
    int                 num_of_sub;
} rx_media_component_t;

typedef struct _rx_message_t {
#define RX_CMD_CODE_AA                                              265
#define RX_CMD_CODE_SESSION_TERMINATION                             275
    uint16_t          cmd_code;

/* Experimental-Result-Codes */
#define RX_DIAMETER_INVALID_SERVICE_INFORMATION                     5061
#define RX_DIAMETER_FILTER_RESTRICTIONS                             5062
#define RX_DIAMETER_REQUESTED_SERVICE_NOT_AUTHORIZED                5063
#define RX_DIAMETER_DUPLICATED_AF_SESSION                           5064
#define RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE                    5065
#define RX_DIAMETER_UNAUTHORIZED_NON_EMERGENCY_SESSION              5066
#define RX_DIAMETER_UNAUTHORIZED_SPONSORED_DATA_CONNECTIVITY        5067
#define RX_DIAMETER_TEMPORARY_NETWORK_FAILURE                       5068
    uint32_t          result_code;

#define MAX_NUM_OF_MEDIA_COMPONENT 16
    rx_media_component_t media_component[MAX_NUM_OF_MEDIA_COMPONENT];
    int num_of_media_component;
} rx_message_t;

void rx_message_free(rx_message_t *rx_message);

#ifdef __cplusplus
}
#endif

#endif /* RX_MESSAGE_H */
