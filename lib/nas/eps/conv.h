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

#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_EPS_CONV_H
#define OGS_NAS_EPS_CONV_H

#include "ogs-nas-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void ogs_nas_imsi_to_buffer(
    const ogs_nas_mobile_identity_imsi_t *imsi, uint8_t imsi_len,
    uint8_t *buf, uint8_t *buf_len);

void ogs_nas_eps_imsi_to_bcd(
    const ogs_nas_mobile_identity_imsi_t *imsi, uint8_t imsi_len, char *bcd);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_EPS_CONV_H */

