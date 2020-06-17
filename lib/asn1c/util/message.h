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

#ifndef OGS_ASN_MESSAGE_H
#define OGS_ASN_MESSAGE_H

#include "ogs-core.h"

#include "asn_internal.h"
#include "constr_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *ogs_asn_encode(const asn_TYPE_descriptor_t *td, void *sptr);
int ogs_asn_decode(const asn_TYPE_descriptor_t *td,
        void *struct_ptr, size_t struct_size, ogs_pkbuf_t *pkbuf);
void ogs_asn_free(const asn_TYPE_descriptor_t *td, void *sptr);

#ifdef __cplusplus
}
#endif

#endif

