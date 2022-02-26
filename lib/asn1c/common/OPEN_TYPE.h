/*-
 * Copyright (c) 2017-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef ASN_OPEN_TYPE_H
#define ASN_OPEN_TYPE_H

#include <asn_application.h>
///////////#include <per_support.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Decode an Open Type which is potentially constraiend
 * by the other members of the parent structure.
 */

#undef  ADVANCE
#define ADVANCE(num_bytes)               \
    do {                                 \
        size_t num = num_bytes;          \
        ptr = ((const char *)ptr) + num; \
        size -= num;                     \
        consumed_myself += num;          \
    } while(0)

#define OPEN_TYPE_free CHOICE_free

#if !defined(ASN_DISABLE_PRINT_SUPPORT)
#define OPEN_TYPE_print CHOICE_print
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */

#define OPEN_TYPE_compare CHOICE_compare

#define OPEN_TYPE_constraint CHOICE_constraint

#if !defined(ASN_DISABLE_BER_SUPPORT)
asn_dec_rval_t OPEN_TYPE_ber_get(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *parent_type,
    void *parent_structure,
    const asn_TYPE_member_t *element,
    const void *ptr, size_t size);
#define OPEN_TYPE_decode_ber NULL
#define OPEN_TYPE_encode_der CHOICE_encode_der
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */

#if !defined(ASN_DISABLE_XER_SUPPORT)
asn_dec_rval_t OPEN_TYPE_xer_get(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *parent_type,
    void *parent_structure,
    const asn_TYPE_member_t *element,
    const void *ptr, size_t size);
#define OPEN_TYPE_decode_xer NULL
#define OPEN_TYPE_encode_xer CHOICE_encode_xer
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */

#if !defined(ASN_DISABLE_OER_SUPPORT)
asn_dec_rval_t OPEN_TYPE_oer_get(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *parent_type,
    void *parent_structure,
    asn_TYPE_member_t *element, const void *ptr,
    size_t size);
#define OPEN_TYPE_decode_oer NULL
#define OPEN_TYPE_encode_oer CHOICE_encode_oer
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */

#if !defined(ASN_DISABLE_UPER_SUPPORT)
asn_dec_rval_t OPEN_TYPE_uper_get(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *parent_type,
    void *parent_structure,
    const asn_TYPE_member_t *element,
    asn_per_data_t *pd);
#define OPEN_TYPE_decode_uper NULL
asn_enc_rval_t OPEN_TYPE_encode_uper(
    const asn_TYPE_descriptor_t *type_descriptor,
    const asn_per_constraints_t *constraints, const void *struct_ptr,
    asn_per_outp_t *per_output);
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
asn_dec_rval_t OPEN_TYPE_aper_get(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *parent_type,
    void *parent_structure,
    const asn_TYPE_member_t *element,
    asn_per_data_t *pd);
#define OPEN_TYPE_decode_aper NULL
asn_enc_rval_t OPEN_TYPE_encode_aper(
    const asn_TYPE_descriptor_t *type_descriptor,
    const asn_per_constraints_t *constraints, const void *struct_ptr,
    asn_per_outp_t *per_output);

int OPEN_TYPE_aper_is_unknown_type(
    const asn_TYPE_descriptor_t *td,
    void *sptr,
    const asn_TYPE_member_t *elm);

asn_dec_rval_t OPEN_TYPE_aper_unknown_type_discard_bytes(
    asn_per_data_t *pd);
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */

extern asn_TYPE_operation_t asn_OP_OPEN_TYPE;

#ifdef __cplusplus
}
#endif

#endif	/* ASN_OPEN_TYPE_H */
