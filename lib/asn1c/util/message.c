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

#include "message.h"

#include "constr_SEQUENCE.h"
#include "constr_SET.h"
#include "constr_CHOICE.h"
#include "constr_SET_OF.h"

void *ogs_asn_calloc_constructed(const asn_TYPE_descriptor_t *td)
{
    size_t size = 0;
    void *ptr = NULL;

    ogs_assert(td);
    ogs_assert(td->op);

    switch (td->op->kind) {
    case ASN_KIND_SEQUENCE:
        ogs_assert(td->specifics);
        size = ((const asn_SEQUENCE_specifics_t *)td->specifics)->struct_size;
        break;
    case ASN_KIND_SET:
        ogs_assert(td->specifics);
        size = ((const asn_SET_specifics_t *)td->specifics)->struct_size;
        break;
    case ASN_KIND_CHOICE:
        ogs_assert(td->specifics);
        size = ((const asn_CHOICE_specifics_t *)td->specifics)->struct_size;
        break;
    case ASN_KIND_SEQUENCE_OF:
    case ASN_KIND_SET_OF:
        ogs_assert(td->specifics);
        size = ((const asn_SET_OF_specifics_t *)td->specifics)->struct_size;
        break;
    default:
        ogs_fatal("ASN.1 descriptor [%s] kind:%d is not constructed; "
                "use CALLOC() when the concrete pointer type is available",
                td->name ? td->name : "unknown", td->op->kind);
        ogs_assert_if_reached();
    }

    ogs_assert(size);

    ptr = CALLOC(1, size);
    ogs_assert(ptr);

    return ptr;
}

void *ogs_asn_calloc_protocol_ies(const asn_TYPE_descriptor_t *parent_td)
{
    ogs_assert(parent_td);
    ogs_assert(parent_td->elements);
    ogs_assert(parent_td->elements_count > 0);
    ogs_assert(parent_td->elements[0].name);
    ogs_assert(strcmp(parent_td->elements[0].name, "protocolIEs") == 0);
    ogs_assert(parent_td->elements[0].type);

    return ogs_asn_calloc_constructed(parent_td->elements[0].type);
}

ogs_pkbuf_t *ogs_asn_encode(const asn_TYPE_descriptor_t *td, void *sptr)
{
    asn_enc_rval_t enc_ret = {0};
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(td);
    ogs_assert(sptr);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);

    enc_ret = aper_encode_to_buffer(td, NULL,
                    sptr, pkbuf->data, OGS_MAX_SDU_LEN);
    ogs_asn_free(td, sptr);

    if (enc_ret.encoded < 0) {
        ogs_error("Failed to encode ASN-PDU [%d]", (int)enc_ret.encoded);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_pkbuf_trim(pkbuf, ((enc_ret.encoded + 7) >> 3));

    return pkbuf;
}

int ogs_asn_decode(const asn_TYPE_descriptor_t *td,
        void *struct_ptr, size_t struct_size, ogs_pkbuf_t *pkbuf)
{
    asn_dec_rval_t dec_ret = {0};

    ogs_assert(td);
    ogs_assert(struct_ptr);
    ogs_assert(struct_size);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    memset(struct_ptr, 0, struct_size);
    dec_ret = aper_decode(NULL, td, (void **)&struct_ptr,
            pkbuf->data, pkbuf->len, 0, 0);

    if (dec_ret.code != RC_OK) {
        ogs_warn("Failed to decode ASN-PDU [code:%d,consumed:%d]",
                dec_ret.code, (int)dec_ret.consumed);
        return OGS_ERROR;
    }

    return OGS_OK;
}

void ogs_asn_free(const asn_TYPE_descriptor_t *td, void *sptr)
{
    ogs_assert(td);
    ogs_assert(sptr);

    ASN_STRUCT_FREE_CONTENTS_ONLY(*td, sptr);
}
