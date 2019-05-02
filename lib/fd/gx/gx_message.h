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

#ifndef __GX_MESSAGE_H__
#define __GX_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "base/types.h"

typedef struct _gx_message_t {
#define GX_CMD_CODE_CREDIT_CONTROL                      272
#define GX_CMD_RE_AUTH                                  258
    uint16_t          cmd_code;

/* Experimental-Result-Codes */
#define GX_DIAMETER_ERROR_LATE_OVERLAPPING_REQUEST      5453
#define GX_DIAMETER_ERROR_TIMED_OUT_REQUEST             5454
#define GX_DIAMETER_ERROR_INITIAL_PARAMETERS            5140
#define GX_DIAMETER_ERROR_TRIGGER_EVENT                 5141
#define GX_DIAMETER_PCC_RULE_EVENT                      5142
#define GX_DIAMETER_ERROR_BEARER_NOT_AUTHORIZED         5143
#define GX_DIAMETER_ERROR_TRAFFIC_MAPPING_INFO_REJECTED 5144
#define GX_DIAMETER_ERROR_CONFLICTING_REQUEST           5147
#define GX_DIAMETER_ADC_RULE_EVENT                      5148
#define GX_DIAMETER_ERROR_NBIFOM_NOT_AUTHORIZED         5149
    uint32_t          result_code;

#define GX_CC_REQUEST_TYPE_INITIAL_REQUEST              1
#define GX_CC_REQUEST_TYPE_UPDATE_REQUEST               2
#define GX_CC_REQUEST_TYPE_TERMINATION_REQUEST          3
#define GX_CC_REQUEST_TYPE_EVENT_REQUEST                4
    uint32_t          cc_request_type;

    pdn_t               pdn;
    pcc_rule_t          pcc_rule[MAX_NUM_OF_PCC_RULE];
    int                 num_of_pcc_rule;
} gx_message_t;

void gx_message_free(gx_message_t *gx_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GX_MESSAGE_H__ */
