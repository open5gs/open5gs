/*-
 * Copyright (c) 2004, 2005, 2006 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
/*
 * Read the NativeInteger.h for the explanation wrt. differences between
 * INTEGER and NativeInteger.
 * Basically, both are decoders and encoders of ASN.1 INTEGER type, but this
 * implementation deals with the standard (machine-specific) representation
 * of them instead of using the platform-independent buffer.
 */
#include <asn_internal.h>
#include <NativeInteger.h>

/*
 * NativeInteger basic type description.
 */
static const ber_tlv_tag_t asn_DEF_NativeInteger_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (2 << 2))
};
asn_TYPE_operation_t asn_OP_NativeInteger = {
    NativeInteger_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    NativeInteger_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    NativeInteger_compare,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    NativeInteger_decode_ber,
    NativeInteger_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    NativeInteger_decode_xer,
    NativeInteger_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    NativeInteger_decode_oer,  /* OER decoder */
    NativeInteger_encode_oer,  /* Canonical OER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    NativeInteger_decode_uper,  /* Unaligned PER decoder */
    NativeInteger_encode_uper,  /* Unaligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    NativeInteger_decode_aper,  /* Aligned PER decoder */
    NativeInteger_encode_aper,  /* Aligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    NativeInteger_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_NativeInteger = {
    "INTEGER",  /* The ASN.1 type is still INTEGER */
    "INTEGER",
    &asn_OP_NativeInteger,
    asn_DEF_NativeInteger_tags,
    sizeof(asn_DEF_NativeInteger_tags) / sizeof(asn_DEF_NativeInteger_tags[0]),
    asn_DEF_NativeInteger_tags,  /* Same as above */
    sizeof(asn_DEF_NativeInteger_tags) / sizeof(asn_DEF_NativeInteger_tags[0]),
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
NativeInteger_free(const asn_TYPE_descriptor_t *td, void *ptr,
                   enum asn_struct_free_method method) {
    if(!td || !ptr)
		return;

	ASN_DEBUG("Freeing %s as INTEGER (%d, %p, Native)",
		td->name, method, ptr);

    switch(method) {
    case ASFM_FREE_EVERYTHING:
        FREEMEM(ptr);
        break;
    case ASFM_FREE_UNDERLYING:
        break;
    case ASFM_FREE_UNDERLYING_AND_RESET:
        memset(ptr, 0, sizeof(long));
        break;
    }
}

int
NativeInteger_compare(const asn_TYPE_descriptor_t *td, const void *aptr, const void *bptr) {
    (void)td;

    if(aptr && bptr) {
        const asn_INTEGER_specifics_t *specs =
            (const asn_INTEGER_specifics_t *)td->specifics;
        if(specs && specs->field_unsigned) {
            const unsigned long *a = aptr;
            const unsigned long *b = bptr;
            if(*a < *b) {
                return -1;
            } else if(*a > *b) {
                return 1;
            } else {
                return 0;
            }
        } else {
            const long *a = aptr;
            const long *b = bptr;
            if(*a < *b) {
                return -1;
            } else if(*a > *b) {
                return 1;
            } else {
                return 0;
            }
        }
    } else if(!aptr) {
        return -1;
    } else {
        return 1;
    }
}
