/*-
 * Copyright (c) 2003, 2004 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <ObjectDescriptor.h>

/*
 * ObjectDescriptor basic type description.
 */
static const ber_tlv_tag_t asn_DEF_ObjectDescriptor_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (7 << 2)),  /* [UNIVERSAL 7] IMPLICIT ... */
    (ASN_TAG_CLASS_UNIVERSAL | (4 << 2))   /* ... OCTET STRING */
};
asn_TYPE_operation_t asn_OP_ObjectDescriptor = {
    OCTET_STRING_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    OCTET_STRING_print_utf8,  /* Treat as ASCII subset (it's not) */
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    OCTET_STRING_compare,
    OCTET_STRING_copy,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    OCTET_STRING_decode_ber,  /* Implemented in terms of OCTET STRING */
    OCTET_STRING_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    OCTET_STRING_decode_xer_utf8,
    OCTET_STRING_encode_xer_utf8,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    OCTET_STRING_decode_jer_utf8,
    OCTET_STRING_encode_jer_utf8,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    0,
    0,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    OCTET_STRING_decode_uper,
    OCTET_STRING_encode_uper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    OCTET_STRING_decode_aper,
    OCTET_STRING_encode_aper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    0,  /* Not supported for ObjectDescriptor */
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_ObjectDescriptor = {
    "ObjectDescriptor",
    "ObjectDescriptor",
    &asn_OP_ObjectDescriptor,
    asn_DEF_ObjectDescriptor_tags,
    sizeof(asn_DEF_ObjectDescriptor_tags)
      / sizeof(asn_DEF_ObjectDescriptor_tags[0]) - 1,
    asn_DEF_ObjectDescriptor_tags,
    sizeof(asn_DEF_ObjectDescriptor_tags)
      / sizeof(asn_DEF_ObjectDescriptor_tags[0]),
    {
#if !defined(ASN_DISABLE_OER_SUPPORT)
        0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
        0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
        asn_generic_unknown_constraint
    },
    0, 0,  /* No members */
    0  /* No specifics */
};
