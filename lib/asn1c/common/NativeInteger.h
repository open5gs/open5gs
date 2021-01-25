/*-
 * Copyright (c) 2004-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
/*
 * This type differs from the standard INTEGER in that it is modelled using
 * the fixed machine type (long, int, short), so it can hold only values of
 * limited length. There is no type (i.e., NativeInteger_t, any integer type
 * will do).
 * This type may be used when integer range is limited by subtype constraints.
 */
#ifndef	_NativeInteger_H_
#define	_NativeInteger_H_

#include <asn_application.h>
#include <INTEGER.h>

#ifdef __cplusplus
extern "C" {
#endif

extern asn_TYPE_descriptor_t asn_DEF_NativeInteger;
extern asn_TYPE_operation_t asn_OP_NativeInteger;

asn_struct_free_f NativeInteger_free;

#if !defined(ASN_DISABLE_PRINT_SUPPORT)
asn_struct_print_f NativeInteger_print;
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */

asn_struct_compare_f NativeInteger_compare;

#define NativeInteger_constraint asn_generic_no_constraint

#if !defined(ASN_DISABLE_BER_SUPPORT)
ber_type_decoder_f NativeInteger_decode_ber;
der_type_encoder_f NativeInteger_encode_der;
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */

#if !defined(ASN_DISABLE_XER_SUPPORT)
xer_type_decoder_f NativeInteger_decode_xer;
xer_type_encoder_f NativeInteger_encode_xer;
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */

#if !defined(ASN_DISABLE_OER_SUPPORT)
oer_type_decoder_f NativeInteger_decode_oer;
oer_type_encoder_f NativeInteger_encode_oer;
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */

#if !defined(ASN_DISABLE_UPER_SUPPORT)
per_type_decoder_f NativeInteger_decode_uper;
per_type_encoder_f NativeInteger_encode_uper;
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
per_type_decoder_f NativeInteger_decode_aper;
per_type_encoder_f NativeInteger_encode_aper;
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */

#if !defined(ASN_DISABLE_RFILL_SUPPORT)
asn_random_fill_f NativeInteger_random_fill;
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */

#ifdef __cplusplus
}
#endif

#endif	/* _NativeInteger_H_ */
