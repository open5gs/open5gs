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

#ifndef NAS_CONV_H
#define NAS_CONV_H

#include "nas/nas-message.h"

#ifdef __cplusplus
extern "C" {
#endif

void nas_imsi_to_buffer(
    nas_mobile_identity_imsi_t *imsi, uint8_t imsi_len, 
    uint8_t *buf, uint8_t *buf_len);

void nas_imsi_to_bcd(
    nas_mobile_identity_imsi_t *imsi, uint8_t imsi_len, char *bcd);

#ifdef __cplusplus
}
#endif

#endif /* NAS_CONV_H */

