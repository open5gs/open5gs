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

#ifndef SBC_MESSAGE_H
#define SBC_MESSAGE_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* SBc-AP messages:
 * After the CBC integration, the encoding/decoding of 
 * SBc-AP messages will be fully supported.
 */ 

/* SBc-AP interface data definitions */

/* SBc-AP WriteReplaceWarning data */
typedef struct _sbc_pws_data_t {
    uint16_t message_id;
    uint16_t serial_number;
    // uint8_t warning_type;
    uint32_t no_of_tai;
    ogs_eps_tai_t tai[16];              /* TODO: max 65535 */
    uint32_t repetition_period;
    uint32_t number_of_broadcast;
    uint8_t data_coding_scheme;
    uint32_t message_length;
    uint8_t message_contents[1024]; /* TODO: max 9600 */
} sbc_pws_data_t;

#ifdef __cplusplus
}
#endif

#endif /* SBC_MESSAGE_H */
