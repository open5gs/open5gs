/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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
#include "asn_SEQUENCE_OF.h"

/*
 * Access generated SEQUENCE OF/SET OF values that are represented as
 * pointers by newer asn1c output. Keep the asn_anonymous_sequence_ cast in
 * one place instead of spreading it across protocol code.
 *
 * These helpers are only for ASN.1 list wrappers whose first member is
 * compatible with asn_anonymous_sequence_. Do not use them for arbitrary
 * ASN.1 SEQUENCE values.
 */
static inline int ogs_asn_list_count(const void *list)
{
    const asn_anonymous_sequence_ *sequence = NULL;

    ogs_assert(list);

    sequence = (const asn_anonymous_sequence_ *)list;

    return sequence->count;
}

static inline void *ogs_asn_list_get(const void *list, int index)
{
    const asn_anonymous_sequence_ *sequence = NULL;

    ogs_assert(list);

    sequence = (const asn_anonymous_sequence_ *)list;

    ogs_assert(index >= 0);
    ogs_assert(index < sequence->count);
    ogs_assert(sequence->array);

    return sequence->array[index];
}

#define OGS_ASN_LIST_COUNT(__list) \
    ogs_asn_list_count(__list)
#define OGS_ASN_LIST_GET(__list, __index) \
    ogs_asn_list_get(__list, __index)

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Allocate an ASN.1 constructed value from a descriptor when the concrete
 * C pointer type is not available at the call site.
 *
 * Prefer CALLOC(1, sizeof(*ptr)) whenever the destination pointer type is
 * known. This helper is mainly used by ogs_asn_calloc_protocol_ies(),
 * where only the generated descriptor is available.
 */
void *ogs_asn_calloc_constructed(const asn_TYPE_descriptor_t *td);

/*
 * Allocate protocolIEs from an ASN.1 procedure message descriptor.
 * The generated procedure SEQUENCE is expected to have protocolIEs
 * as member 0. Assert the member name here so callers do not silently
 * allocate the wrong member if generated layouts change.
 */
void *ogs_asn_calloc_protocol_ies(const asn_TYPE_descriptor_t *parent_td);

ogs_pkbuf_t *ogs_asn_encode(const asn_TYPE_descriptor_t *td, void *sptr);
int ogs_asn_decode(const asn_TYPE_descriptor_t *td,
        void *struct_ptr, size_t struct_size, ogs_pkbuf_t *pkbuf);
void ogs_asn_free(const asn_TYPE_descriptor_t *td, void *sptr);

#ifdef __cplusplus
}
#endif

#endif

