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

#ifndef OGS_NAS_COMMON_H
#define OGS_NAS_COMMON_H

#include "crypt/ogs-crypt.h"

#define OGS_NAS_INSIDE

#include "nas/common/types.h"
#include "nas/common/conv.h"
#include "nas/common/security.h"

#undef OGS_NAS_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

extern int __ogs_nas_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_nas_domain

/*
 * TS 24.007 11.2.4: Skip unknown optional NAS IEs using their length.
 * The caller has already consumed the IEI octet. The TLV-E IEI range
 * differs between 5GS and EPS.
 */
#define OGS_NAS_5GS_TLV_E_IEI_MIN 0x70
#define OGS_NAS_EPS_TLV_E_IEI_MIN 0x78

static ogs_inline int ogs_nas_skip_unknown_ie(
        uint8_t type, ogs_pkbuf_t *pkbuf, uint8_t tlv_e_iei_min)
{
    int size = 0;

    if (type >= 0x80)
        return 0;

    if (type >= tlv_e_iei_min) {
        uint16_t length = 0;

        if (pkbuf->len < sizeof(length)) {
           ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
           return OGS_ERROR;
        }

        memcpy(&length, pkbuf->data, sizeof(length));
        size = sizeof(length) + be16toh(length);
    } else {
        uint8_t length = 0;

        if (pkbuf->len < sizeof(length)) {
           ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
           return OGS_ERROR;
        }

        length = *(uint8_t *)pkbuf->data;
        size = sizeof(length) + length;
    }

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", size);
       return OGS_ERROR;
    }

    return size;
}

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_COMMON_H */
