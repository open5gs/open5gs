/*-
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_VisibleString_H_
#define	_VisibleString_H_

#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef OCTET_STRING_t VisibleString_t;  /* Implemented via OCTET STRING */

extern asn_TYPE_descriptor_t asn_DEF_VisibleString;
extern asn_TYPE_operation_t asn_OP_VisibleString;

#define VisibleString_free OCTET_STRING_free

#if !defined(ASN_DISABLE_PRINT_SUPPORT)
#define VisibleString_print OCTET_STRING_print
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */

#define VisibleString_compare OCTET_STRING_compare
#define VisibleString_copy    OCTET_STRING_copy   

asn_constr_check_f VisibleString_constraint;

#if !defined(ASN_DISABLE_BER_SUPPORT)
#define VisibleString_decode_ber OCTET_STRING_decode_ber
#define VisibleString_encode_der OCTET_STRING_encode_der
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */

#if !defined(ASN_DISABLE_XER_SUPPORT)
#define VisibleString_decode_xer OCTET_STRING_decode_xer_hex
#define VisibleString_encode_xer OCTET_STRING_encode_xer
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */

#if !defined(ASN_DISABLE_JER_SUPPORT)
#define VisibleString_decode_jer OCTET_STRING_decode_jer_hex
#define VisibleString_encode_jer OCTET_STRING_encode_jer
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */

#if !defined(ASN_DISABLE_UPER_SUPPORT)
#define VisibleString_decode_uper OCTET_STRING_decode_uper
#define VisibleString_encode_uper OCTET_STRING_encode_uper
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
#define VisibleString_decode_aper OCTET_STRING_decode_aper
#define VisibleString_encode_aper OCTET_STRING_encode_aper
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */

#ifdef __cplusplus
}
#endif

#endif	/* _VisibleString_H_ */
