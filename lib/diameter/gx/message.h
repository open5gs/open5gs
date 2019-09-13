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

#ifndef OGS_DIAM_GX_MESSAGE_H
#define OGS_DIAM_GX_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_diam_gx_message_s {
#define OGS_DIAM_GX_CMD_CODE_CREDIT_CONTROL                         272
#define OGS_DIAM_GX_CMD_RE_AUTH                                     258
    uint16_t            cmd_code;

/* Experimental-Result-Codes */
#define OGS_DIAM_GX_DIAMETER_ERROR_LATE_OVERLAPPING_REQUEST         5453
#define OGS_DIAM_GX_DIAMETER_ERROR_TIMED_OUT_REQUEST                5454
#define OGS_DIAM_GX_DIAMETER_ERROR_INITIAL_PARAMETERS               5140
#define OGS_DIAM_GX_DIAMETER_ERROR_TRIGGER_EVENT                    5141
#define OGS_DIAM_GX_DIAMETER_PCC_RULE_EVENT                         5142
#define OGS_DIAM_GX_DIAMETER_ERROR_BEARER_NOT_AUTHORIZED            5143
#define OGS_DIAM_GX_DIAMETER_ERROR_TRAFFIC_MAPPING_INFO_REJECTED    5144
#define OGS_DIAM_GX_DIAMETER_ERROR_CONFLICTING_REQUEST              5147
#define OGS_DIAM_GX_DIAMETER_ADC_RULE_EVENT                         5148
#define OGS_DIAM_GX_DIAMETER_ERROR_NBIFOM_NOT_AUTHORIZED            5149
    uint32_t            result_code;

#define OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST                 1
#define OGS_DIAM_GX_CC_REQUEST_TYPE_UPDATE_REQUEST                  2
#define OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST             3
#define OGS_DIAM_GX_CC_REQUEST_TYPE_EVENT_REQUEST                   4
    uint32_t            cc_request_type;

    ogs_pdn_t           pdn;
    ogs_pcc_rule_t      pcc_rule[OGS_MAX_NUM_OF_PCC_RULE];
    int                 num_of_pcc_rule;
} ogs_diam_gx_message_t;

void ogs_diam_gx_message_free(ogs_diam_gx_message_t *gx_message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_GX_MESSAGE_H */
