/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>

asn_TYPE_operation_t asn_OP_OPEN_TYPE = {
    OPEN_TYPE_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    OPEN_TYPE_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    OPEN_TYPE_compare,
    OPEN_TYPE_copy,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    OPEN_TYPE_decode_ber,
    OPEN_TYPE_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    OPEN_TYPE_decode_xer,
    OPEN_TYPE_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    OPEN_TYPE_decode_jer,
    OPEN_TYPE_encode_jer,
#else
    0,
    0,
#endif /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    OPEN_TYPE_decode_oer,
    OPEN_TYPE_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    OPEN_TYPE_decode_uper,
    OPEN_TYPE_encode_uper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    OPEN_TYPE_decode_aper,
    OPEN_TYPE_encode_aper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    0,  /* Random fill is not supported for open type */
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
