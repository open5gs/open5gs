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

#ifndef OGS_NAS_5GS_CONV_H
#define OGS_NAS_5GS_CONV_H

#include "ogs-nas-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void ogs_nas_5gs_imsi_to_bcd(
    ogs_nas_5gs_mobile_identity_t *mobile_identity, char *bcd);

char *ogs_nas_5gs_suci_from_mobile_identity(
        ogs_nas_5gs_mobile_identity_t *mobile_identity);

void ogs_nas_5gs_mobile_identity_guti_to_nas_guti(
        ogs_nas_5gs_mobile_identity_guti_t *mobile_identity_guti,
        ogs_nas_5gs_guti_t *nas_guti);
void ogs_nas_5gs_nas_guti_to_mobilty_identity_guti(
        ogs_nas_5gs_guti_t *nas_guti,
        ogs_nas_5gs_mobile_identity_guti_t *mobile_identity_guti);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_5GS_CONV_H */

