/*-
 * Copyright (c) 2003, 2005, 2006 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <ENUMERATED.h>

/*
 * ENUMERATED basic type description.
 */
static const ber_tlv_tag_t asn_DEF_ENUMERATED_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_operation_t asn_OP_ENUMERATED = {
    ASN__PRIMITIVE_TYPE_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    INTEGER_print,  /* Implemented in terms of INTEGER */
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    INTEGER_compare,  /* Implemented in terms of INTEGER */
    INTEGER_copy,  /* Implemented in terms of INTEGER */
#if !defined(ASN_DISABLE_BER_SUPPORT)
    ber_decode_primitive,
    INTEGER_encode_der,  /* Implemented in terms of INTEGER */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    INTEGER_decode_xer,  /* This is temporary! */
    INTEGER_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    ENUMERATED_decode_jer,
    INTEGER_encode_jer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    ENUMERATED_decode_oer,
    ENUMERATED_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    ENUMERATED_decode_uper,  /* Unaligned PER decoder */
    ENUMERATED_encode_uper,  /* Unaligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    ENUMERATED_decode_aper,  /* Aligned PER decoder */
    ENUMERATED_encode_aper,  /* Aligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    ENUMERATED_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_ENUMERATED = {
    "ENUMERATED",
    "ENUMERATED",
    &asn_OP_ENUMERATED,
    asn_DEF_ENUMERATED_tags,
    sizeof(asn_DEF_ENUMERATED_tags) / sizeof(asn_DEF_ENUMERATED_tags[0]),
    asn_DEF_ENUMERATED_tags,	/* Same as above */
    sizeof(asn_DEF_ENUMERATED_tags) / sizeof(asn_DEF_ENUMERATED_tags[0]),
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
