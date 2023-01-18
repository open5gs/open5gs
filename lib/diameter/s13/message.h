/*
 * Copyright (C) 2023 by Ryan Dimsey <ryan@omnitouch.com.au>
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

#ifndef OGS_DIAM_S13_MESSAGE_H
#define OGS_DIAM_S13_MESSAGE_H

#include "ogs-crypt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_DIAM_S13_APPLICATION_ID                     16777252

#define OGS_DIAM_S13_AVP_CODE_TERMINAL_INFORMATION      (1401)
#define OGS_DIAM_S13_AVP_CODE_IMEI                      (1402)
#define OGS_DIAM_S13_AVP_CODE_SOFTWARE_VERSION          (1403)
#define OGS_DIAM_S13_AVP_CODE_EQUIPMENT_STATUS          (1445)

#define OGS_DIAM_S13_EQUIPMENT_WHITELIST                (0)
#define OGS_DIAM_S13_EQUIPMENT_BLACKLIST                (1)
#define OGS_DIAM_S13_EQUIPMENT_GREYLIST                 (2)

extern struct dict_object *ogs_diam_s13_application;

extern struct dict_object *ogs_diam_s13_cmd_ecr;
extern struct dict_object *ogs_diam_s13_cmd_eca;

extern struct dict_object *ogs_diam_s13_terminal_information;
extern struct dict_object *ogs_diam_s13_imei;
extern struct dict_object *ogs_diam_s13_software_version;
extern struct dict_object *ogs_diam_s13_equipment_status;

typedef struct ogs_diam_s13_eca_message_s {
    uint32_t equipment_status_code;
} ogs_diam_s13_eca_message_t;

typedef struct ogs_diam_s13_message_s {
#define OGS_DIAM_S13_CMD_CODE_ME_IDENTITY_CHECK             324

    /* Experimental Result Code */
#define OGS_DIAM_S13_AUTHENTICATION_DATA_UNAVAILABLE        4181
#define OGS_DIAM_S13_ERROR_USER_UNKNOWN                     5001
#define OGS_DIAM_S13_ERROR_ROAMING_NOT_ALLOWED              5004
#define OGS_DIAM_S13_ERROR_UNKNOWN_EPS_SUBSCRIPTION         5420
#define OGS_DIAM_S13_ERROR_RAT_NOT_ALLOWED                  5421
#define OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN                5422
#define OGS_DIAM_S13_ERROR_UNKOWN_SERVING_NODE              5423
    uint16_t                   cmd_code;
    uint32_t                   result_code;
    uint32_t                   *err;
    uint32_t                   *exp_err;

    ogs_diam_s13_eca_message_t eca_message;
} ogs_diam_s13_message_t;

int ogs_diam_s13_init(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_S13_MESSAGE_H */
