/*-
 * Copyright (c) 2003, 2005 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NULL.h>

/*
 * NULL basic type description.
 */
static const ber_tlv_tag_t asn_DEF_NULL_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (5 << 2))
};
asn_TYPE_operation_t asn_OP_NULL = {
    NULL_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    NULL_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    NULL_compare,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    NULL_decode_ber,
    NULL_encode_der,  /* Special handling of DER encoding */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    NULL_decode_xer,
    NULL_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    NULL_decode_oer,
    NULL_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    NULL_decode_uper,  /* Unaligned PER decoder */
    NULL_encode_uper,  /* Unaligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    NULL_decode_aper,  /* Aligned PER decoder */
    NULL_encode_aper,  /* Aligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    NULL_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_NULL = {
    "NULL",
    "NULL",
    &asn_OP_NULL,
    asn_DEF_NULL_tags,
    sizeof(asn_DEF_NULL_tags) / sizeof(asn_DEF_NULL_tags[0]),
    asn_DEF_NULL_tags,  /* Same as above */
    sizeof(asn_DEF_NULL_tags) / sizeof(asn_DEF_NULL_tags[0]),
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
NULL_free(const asn_TYPE_descriptor_t *td, void *ptr,
          enum asn_struct_free_method method) {
    if(td && ptr) {
        switch(method) {
        case ASFM_FREE_EVERYTHING:
            FREEMEM(ptr);
            break;
        case ASFM_FREE_UNDERLYING:
            break;
        case ASFM_FREE_UNDERLYING_AND_RESET:
            memset(ptr, 0, sizeof(NULL_t));
            break;
        }
    }
}

int
NULL_compare(const asn_TYPE_descriptor_t *td, const void *a, const void *b) {
    (void)td;
    (void)a;
    (void)b;
    return 0;
}
