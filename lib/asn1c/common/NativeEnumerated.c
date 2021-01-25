/*-
 * Copyright (c) 2004, 2007 Lev Walkin <vlm@lionet.info>. All rights reserved.
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
#include <NativeEnumerated.h>

/*
 * NativeEnumerated basic type description.
 */
static const ber_tlv_tag_t asn_DEF_NativeEnumerated_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_operation_t asn_OP_NativeEnumerated = {
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
    NativeEnumerated_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    NativeEnumerated_decode_oer,
    NativeEnumerated_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    NativeEnumerated_decode_uper,
    NativeEnumerated_encode_uper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    NativeEnumerated_decode_aper,
    NativeEnumerated_encode_aper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    NativeEnumerated_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_NativeEnumerated = {
    "ENUMERATED",  /* The ASN.1 type is still ENUMERATED */
    "ENUMERATED",
    &asn_OP_NativeEnumerated,
    asn_DEF_NativeEnumerated_tags,
    sizeof(asn_DEF_NativeEnumerated_tags) / sizeof(asn_DEF_NativeEnumerated_tags[0]),
    asn_DEF_NativeEnumerated_tags,  /* Same as above */
    sizeof(asn_DEF_NativeEnumerated_tags) / sizeof(asn_DEF_NativeEnumerated_tags[0]),
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

#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
int
NativeEnumerated__compar_value2enum(const void *ap, const void *bp) {
    const asn_INTEGER_enum_map_t *a = ap;
    const asn_INTEGER_enum_map_t *b = bp;
    if(a->nat_value == b->nat_value)
        return 0;
    if(a->nat_value < b->nat_value)
        return -1;
    return 1;
}
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
