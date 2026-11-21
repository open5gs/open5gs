/*-
 * Copyright (c) 2003, 2004, 2006 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE_OF.h>
#include <asn_SEQUENCE_OF.h>

/*
 * Generic placeholder descriptor for anonymous SEQUENCE OF.
 * NOTE: not a concrete "OF T" — used only to satisfy references from
 * some codegen paths. Concrete OF types still have their own descriptors.
 */
asn_TYPE_descriptor_t asn_DEF_SEQUENCE_OF = {
    "SEQUENCE OF",
    "SEQUENCE OF",
    &asn_OP_SEQUENCE_OF,
    0, 0,
    0, 0,
    {0},            /* No constraints */
    0, 0,           /* No members */
    0               /* No specifics */
};

asn_TYPE_operation_t asn_OP_SEQUENCE_OF = {
    .kind = ASN_KIND_SEQUENCE_OF,
    SEQUENCE_OF_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    SEQUENCE_OF_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    SEQUENCE_OF_compare,
    SEQUENCE_OF_copy,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    SEQUENCE_OF_decode_ber,
    SEQUENCE_OF_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    SEQUENCE_OF_decode_xer,
    SEQUENCE_OF_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    SEQUENCE_OF_decode_jer,
    SEQUENCE_OF_encode_jer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    SEQUENCE_OF_decode_oer,  /* Same as SET OF decoder. */
    SEQUENCE_OF_encode_oer,  /* Same as SET OF encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    SEQUENCE_OF_decode_uper,  /* Same as SET OF decoder */
    SEQUENCE_OF_encode_uper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    SEQUENCE_OF_decode_aper,
    SEQUENCE_OF_encode_aper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    SEQUENCE_OF_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */,
#if !defined(ASN_DISABLE_CBOR_SUPPORT)
    SEQUENCE_OF_decode_cbor,
    SEQUENCE_OF_encode_cbor,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_CBOR_SUPPORT) */
};

int
SEQUENCE_OF_compare(const asn_TYPE_descriptor_t *td, const void *aptr,
               const void *bptr) {
    const asn_anonymous_sequence_ *a = _A_CSEQUENCE_FROM_VOID(aptr);
    const asn_anonymous_sequence_ *b = _A_CSEQUENCE_FROM_VOID(bptr);
    ssize_t idx;

    if(a && b) {
        ssize_t common_length = (a->count < b->count ? a->count : b->count);
        for(idx = 0; idx < common_length; idx++) {
            int ret = td->elements->type->op->compare_struct(
                td->elements->type, a->array[idx], b->array[idx]);
            if(ret) return ret;
        }

        if(idx < b->count) /* more elements in b */
            return -1;    /* a is shorter, so put it first */
        if(idx < a->count) return 1;

    } else if(!a) {
        return -1;
    } else if(!b) {
        return 1;
    }

    return 0;
}
