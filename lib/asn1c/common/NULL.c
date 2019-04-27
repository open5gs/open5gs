/*-
 * Copyright (c) 2003, 2005 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <asn_codecs_prim.h>
#include <NULL.h>
#include <BOOLEAN.h>	/* Implemented in terms of BOOLEAN type */

/*
 * NULL basic type description.
 */
static const ber_tlv_tag_t asn_DEF_NULL_tags[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (5 << 2))
};
asn_TYPE_operation_t asn_OP_NULL = {
	BOOLEAN_free,
	NULL_print,
	NULL_compare,
	BOOLEAN_decode_ber,	/* Implemented in terms of BOOLEAN */
	NULL_encode_der,	/* Special handling of DER encoding */
	NULL_decode_xer,
	NULL_encode_xer,
#ifdef	ASN_DISABLE_OER_SUPPORT
	0,
	0,
#else
	NULL_decode_oer,
	NULL_encode_oer,
#endif  /* ASN_DISABLE_OER_SUPPORT */
#ifdef	ASN_DISABLE_PER_SUPPORT
	0,
	0,
	0,
	0,
#else
	NULL_decode_uper,	/* Unaligned PER decoder */
	NULL_encode_uper,	/* Unaligned PER encoder */
	NULL_decode_aper,	/* Aligned PER decoder */
	NULL_encode_aper,	/* Aligned PER encoder */
#endif	/* ASN_DISABLE_PER_SUPPORT */
	NULL_random_fill,
	0	/* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_NULL = {
	"NULL",
	"NULL",
	&asn_OP_NULL,
	asn_DEF_NULL_tags,
	sizeof(asn_DEF_NULL_tags) / sizeof(asn_DEF_NULL_tags[0]),
	asn_DEF_NULL_tags,	/* Same as above */
	sizeof(asn_DEF_NULL_tags) / sizeof(asn_DEF_NULL_tags[0]),
	{ 0, 0, asn_generic_no_constraint },
	0, 0,	/* No members */
	0	/* No specifics */
};

asn_enc_rval_t
NULL_encode_der(const asn_TYPE_descriptor_t *td, const void *ptr, int tag_mode,
                ber_tlv_tag_t tag, asn_app_consume_bytes_f *cb, void *app_key) {
    asn_enc_rval_t erval;

	erval.encoded = der_write_tags(td, 0, tag_mode, 0, tag, cb, app_key);
	if(erval.encoded == -1) {
		erval.failed_type = td;
		erval.structure_ptr = ptr;
	}

	ASN__ENCODED_OK(erval);
}

asn_enc_rval_t
NULL_encode_xer(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
                enum xer_encoder_flags_e flags, asn_app_consume_bytes_f *cb,
                void *app_key) {
    asn_enc_rval_t er;

	(void)td;
	(void)sptr;
	(void)ilevel;
	(void)flags;
	(void)cb;
	(void)app_key;

	/* XMLNullValue is empty */
	er.encoded = 0;
	ASN__ENCODED_OK(er);
}


static enum xer_pbd_rval
NULL__xer_body_decode(const asn_TYPE_descriptor_t *td, void *sptr,
                      const void *chunk_buf, size_t chunk_size) {
    (void)td;
	(void)sptr;
	(void)chunk_buf;    /* Going to be empty according to the rules below. */

	/*
	 * There must be no content in self-terminating <NULL/> tag.
	 */
	if(chunk_size)
		return XPBD_BROKEN_ENCODING;
	else
		return XPBD_BODY_CONSUMED;
}

asn_dec_rval_t
NULL_decode_xer(const asn_codec_ctx_t *opt_codec_ctx,
                const asn_TYPE_descriptor_t *td, void **sptr,
                const char *opt_mname, const void *buf_ptr, size_t size) {
    return xer_decode_primitive(opt_codec_ctx, td,
		sptr, sizeof(NULL_t), opt_mname, buf_ptr, size,
		NULL__xer_body_decode);
}

int
NULL_compare(const asn_TYPE_descriptor_t *td, const void *a, const void *b) {
    (void)td;
    (void)a;
    (void)b;
    return 0;
}

int
NULL_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
           asn_app_consume_bytes_f *cb, void *app_key) {
    (void)td;	/* Unused argument */
	(void)ilevel;	/* Unused argument */

	if(sptr) {
		return (cb("<present>", 9, app_key) < 0) ? -1 : 0;
	} else {
		return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
	}
}

#ifndef ASN_DISABLE_OER_SUPPORT

asn_dec_rval_t
NULL_decode_oer(const asn_codec_ctx_t *opt_codec_ctx,
                const asn_TYPE_descriptor_t *td,
                const asn_oer_constraints_t *constraints, void **sptr,
                const void *ptr, size_t size) {
    asn_dec_rval_t rv = {RC_OK, 0};
    (void)opt_codec_ctx;
    (void)td;
    (void)constraints;
    (void)ptr;
    (void)size;

    if(!*sptr) {
        *sptr = MALLOC(sizeof(NULL_t));
        if(*sptr) {
            *(NULL_t *)*sptr = 0;
        } else {
            ASN__DECODE_FAILED;
        }
    }

    return rv;
}

asn_enc_rval_t
NULL_encode_oer(const asn_TYPE_descriptor_t *td,
                const asn_oer_constraints_t *constraints, const void *sptr,
                asn_app_consume_bytes_f *cb, void *app_key) {
    asn_enc_rval_t er;

    (void)td;
    (void)sptr;
    (void)constraints;
    (void)cb;
    (void)app_key;

    er.encoded = 0; /* Encoding in 0 bytes. */

    ASN__ENCODED_OK(er);
}

#endif /* ASN_DISABLE_OER_SUPPORT */

#ifndef ASN_DISABLE_PER_SUPPORT

asn_dec_rval_t
NULL_decode_uper(const asn_codec_ctx_t *opt_codec_ctx,
                 const asn_TYPE_descriptor_t *td,
                 const asn_per_constraints_t *constraints, void **sptr,
                 asn_per_data_t *pd) {
    asn_dec_rval_t rv;

	(void)opt_codec_ctx;
	(void)td;
	(void)constraints;
	(void)pd;

	if(!*sptr) {
		*sptr = MALLOC(sizeof(NULL_t));
		if(*sptr) {
			*(NULL_t *)*sptr = 0;
		} else {
			ASN__DECODE_FAILED;
		}
	}

	/*
	 * NULL type does not have content octets.
	 */

	rv.code = RC_OK;
	rv.consumed = 0;
	return rv;
}

asn_enc_rval_t
NULL_encode_uper(const asn_TYPE_descriptor_t *td,
                 const asn_per_constraints_t *constraints, const void *sptr,
                 asn_per_outp_t *po) {
    asn_enc_rval_t er;

	(void)td;
	(void)constraints;
	(void)sptr;
	(void)po;

	er.encoded = 0;
	ASN__ENCODED_OK(er);
}

asn_dec_rval_t
NULL_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                 const asn_TYPE_descriptor_t *td,
                 const asn_per_constraints_t *constraints, void **sptr, asn_per_data_t *pd) {
	asn_dec_rval_t rv;

	(void)opt_codec_ctx;
	(void)td;
	(void)constraints;
	(void)pd;

	if(!*sptr) {
		*sptr = MALLOC(sizeof(NULL_t));
		if(*sptr) {
			*(NULL_t *)*sptr = 0;
		} else {
			ASN__DECODE_FAILED;
		}
	}

	/*
	 * NULL type does not have content octets.
	 */

	rv.code = RC_OK;
	rv.consumed = 0;
	return rv;
}


asn_enc_rval_t
NULL_encode_aper(const asn_TYPE_descriptor_t *td,
                 const asn_per_constraints_t *constraints,
                 const void *sptr, asn_per_outp_t *po) {
	asn_enc_rval_t er;

	(void)td;
	(void)constraints;
	(void)sptr;
	(void)po;

	er.encoded = 0;
	ASN__ENCODED_OK(er);
}

#endif  /* ASN_DISABLE_PER_SUPPORT */

asn_random_fill_result_t
NULL_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                    const asn_encoding_constraints_t *constr,
                    size_t max_length) {
    asn_random_fill_result_t result_ok = {ARFILL_OK, 1};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    NULL_t *st = *sptr;

    (void)td;
    (void)constr;

    if(max_length == 0) return result_skipped;

    if(st == NULL) {
        st = (NULL_t *)(*sptr = CALLOC(1, sizeof(*st)));
        if(st == NULL) {
            return result_failed;
        }
    }

    return result_ok;
}

