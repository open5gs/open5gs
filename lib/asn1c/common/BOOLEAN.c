/*-
 * Copyright (c) 2003, 2005 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BOOLEAN.h>

/*
 * BOOLEAN basic type description.
 */
static const ber_tlv_tag_t asn_DEF_BOOLEAN_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (1 << 2))
};
asn_TYPE_operation_t asn_OP_BOOLEAN = {
    BOOLEAN_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    BOOLEAN_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    BOOLEAN_compare,
    BOOLEAN_copy,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    BOOLEAN_decode_ber,
    BOOLEAN_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    BOOLEAN_decode_xer,
    BOOLEAN_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    BOOLEAN_decode_jer,
    BOOLEAN_encode_jer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    BOOLEAN_decode_oer,
    BOOLEAN_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    BOOLEAN_decode_uper,  /* Unaligned PER decoder */
    BOOLEAN_encode_uper,  /* Unaligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    BOOLEAN_decode_aper,  /* Aligned PER decoder */
    BOOLEAN_encode_aper,  /* Aligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    BOOLEAN_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_BOOLEAN = {
    "BOOLEAN",
    "BOOLEAN",
    &asn_OP_BOOLEAN,
    asn_DEF_BOOLEAN_tags,
    sizeof(asn_DEF_BOOLEAN_tags) / sizeof(asn_DEF_BOOLEAN_tags[0]),
    asn_DEF_BOOLEAN_tags,  /* Same as above */
    sizeof(asn_DEF_BOOLEAN_tags) / sizeof(asn_DEF_BOOLEAN_tags[0]),
    {
#if !defined(ASN_DISABLE_OER_SUPPORT)
        0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
        0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
        asn_generic_no_constraint
    },
    0, 0,  /* No members */
    0  /* No specifics */
};

void
BOOLEAN_free(const asn_TYPE_descriptor_t *td, void *ptr,
             enum asn_struct_free_method method) {
    if(td && ptr) {
        switch(method) {
        case ASFM_FREE_EVERYTHING:
            FREEMEM(ptr);
            break;
        case ASFM_FREE_UNDERLYING:
            break;
        case ASFM_FREE_UNDERLYING_AND_RESET:
            memset(ptr, 0, sizeof(BOOLEAN_t));
            break;
        }
    }
}

int
BOOLEAN_compare(const asn_TYPE_descriptor_t *td, const void *aptr,
                const void *bptr) {
    const BOOLEAN_t *a = aptr;
    const BOOLEAN_t *b = bptr;

    (void)td;

    if(a && b) {
        if(!*a == !*b) {    /* TRUE can be encoded by any non-zero byte. */
            return 0;
        } else if(!*a) {
            return -1;
        } else {
            return 1;
        }
    } else if(!a) {
        return -1;
    } else {
        return 1;
    }
}

int
BOOLEAN_copy(const asn_TYPE_descriptor_t *td, void **aptr,
             const void *bptr) {
    BOOLEAN_t *a = *aptr;
    const BOOLEAN_t *b = bptr;

    (void)td;

    if(!b) {
        if(a) {
            FREEMEM(a);
            *aptr = 0;
        }
        return 0;
    }

    if(!a) {
        a = *aptr = MALLOC(sizeof(BOOLEAN_t));
        if(!a) return -1;
    }

    *a = *b;

    return 0;
}
