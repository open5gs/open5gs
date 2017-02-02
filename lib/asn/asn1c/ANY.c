/*-
 * Copyright (c) 2004 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <ANY.h>
#include <errno.h>

static asn_OCTET_STRING_specifics_t asn_DEF_ANY_specs = {
	sizeof(ANY_t),
	offsetof(ANY_t, _asn_ctx),
	ASN_OSUBV_ANY
};
asn_TYPE_descriptor_t asn_DEF_ANY = {
	"ANY",
	"ANY",
	OCTET_STRING_free,
	OCTET_STRING_print,
	asn_generic_no_constraint,
	OCTET_STRING_decode_ber,
	OCTET_STRING_encode_der,
	OCTET_STRING_decode_xer_hex,
	ANY_encode_xer,
	OCTET_STRING_decode_uper,
	OCTET_STRING_encode_uper,
	OCTET_STRING_decode_aper,
	OCTET_STRING_encode_aper,
	0, /* Use generic outmost tag fetcher */
	0, 0, 0, 0,
	0,	/* No PER visible constraints */
	0, 0,	/* No members */
	&asn_DEF_ANY_specs,
};


asn_enc_rval_t
ANY_encode_xer(asn_TYPE_descriptor_t *td, void *sptr,
	int ilevel, enum xer_encoder_flags_e flags,
		asn_app_consume_bytes_f *cb, void *app_key) {

	if(flags & XER_F_CANONICAL) {
		/*
		 * Canonical XER-encoding of ANY type is not supported.
		 */
		ASN__ENCODE_FAILED;
	}

	/* Dump as binary */
	return OCTET_STRING_encode_xer(td, sptr, ilevel, flags, cb, app_key);
}

struct _callback_arg {
	uint8_t *buffer;
	size_t offset;
	size_t size;
};

static int ANY__consume_bytes(const void *buffer, size_t size, void *key);

int
ANY_fromType(ANY_t *st, asn_TYPE_descriptor_t *td, void *sptr) {
	struct _callback_arg arg;
	asn_enc_rval_t erval;

	if(!st || !td) {
		errno = EINVAL;
		return -1;
	}

	if(!sptr) {
		if(st->buf) FREEMEM(st->buf);
		st->size = 0;
		return 0;
	}

	arg.offset = arg.size = 0;
	arg.buffer = 0;

	erval = der_encode(td, sptr, ANY__consume_bytes, &arg);
	if(erval.encoded == -1) {
		if(arg.buffer) FREEMEM(arg.buffer);
		return -1;
	}
	assert((size_t)erval.encoded == arg.offset);

	if(st->buf) FREEMEM(st->buf);
	st->buf = arg.buffer;
	st->size = arg.offset;

	return 0;
}

int
ANY_fromType_aper(ANY_t *st, asn_TYPE_descriptor_t *td, void *sptr) {
	uint8_t *buffer = NULL;
	ssize_t erval;

	if(!st || !td) {
		errno = EINVAL;
		return -1;
	}

	if(!sptr) {
		if(st->buf) FREEMEM(st->buf);
		st->size = 0;
		return 0;
	}

	erval = aper_encode_to_new_buffer(td, td->per_constraints, sptr, (void**)&buffer);

	if(erval == -1) {
		if(buffer) FREEMEM(buffer);
		return -1;
	}
	assert((size_t)erval > 0);

	if(st->buf) FREEMEM(st->buf);
	st->buf = buffer;
	st->size = erval;

	return 0;
}

ANY_t *
ANY_new_fromType(asn_TYPE_descriptor_t *td, void *sptr) {
	ANY_t tmp;
	ANY_t *st;

	if(!td || !sptr) {
		errno = EINVAL;
		return 0;
	}

	memset(&tmp, 0, sizeof(tmp));

	if(ANY_fromType(&tmp, td, sptr)) return 0;

	st = (ANY_t *)CALLOC(1, sizeof(ANY_t));
	if(st) {
		*st = tmp;
		return st;
	} else {
		FREEMEM(tmp.buf);
		return 0;
	}
}

ANY_t *
ANY_new_fromType_aper(asn_TYPE_descriptor_t *td, void *sptr) {
	ANY_t tmp;
	ANY_t *st;

	if(!td || !sptr) {
		errno = EINVAL;
		return 0;
	}

	memset(&tmp, 0, sizeof(tmp));

	if(ANY_fromType_aper(&tmp, td, sptr)) return 0;

	st = (ANY_t *)CALLOC(1, sizeof(ANY_t));
	if(st) {
		*st = tmp;
		return st;
	} else {
		FREEMEM(tmp.buf);
		return 0;
	}
}

int
ANY_to_type(ANY_t *st, asn_TYPE_descriptor_t *td, void **struct_ptr) {
	asn_dec_rval_t rval;
	void *newst = 0;

	if(!st || !td || !struct_ptr) {
		errno = EINVAL;
		return -1;
	}

	if(st->buf == 0) {
		/* Nothing to convert, make it empty. */
		*struct_ptr = (void *)0;
		return 0;
	}

	rval = ber_decode(0, td, (void **)&newst, st->buf, st->size);
	if(rval.code == RC_OK) {
		*struct_ptr = newst;
		return 0;
	} else {
		/* Remove possibly partially decoded data. */
		ASN_STRUCT_FREE(*td, newst);
		return -1;
	}
}

int
ANY_to_type_aper(ANY_t *st, asn_TYPE_descriptor_t *td, void **struct_ptr) {
	asn_dec_rval_t rval;
	void *newst = 0;

	if(!st || !td || !struct_ptr) {
		errno = EINVAL;
		return -1;
	}

	if(st->buf == 0) {
		/* Nothing to convert, make it empty. */
		*struct_ptr = (void *)0;
		return 0;
	}

	rval = aper_decode(0, td, (void **)&newst, st->buf, st->size, 0, 0);
	if(rval.code == RC_OK) {
		*struct_ptr = newst;
		return 0;
	} else {
		/* Remove possibly partially decoded data. */
		ASN_STRUCT_FREE(*td, newst);
		return -1;
	}
}

static int ANY__consume_bytes(const void *buffer, size_t size, void *key) {
	struct _callback_arg *arg = (struct _callback_arg *)key;

	if((arg->offset + size) >= arg->size) {
		size_t nsize = (arg->size ? arg->size << 2 : 16) + size;
		void *p = REALLOC(arg->buffer, nsize);
		if(!p) return -1;
		arg->buffer = (uint8_t *)p;
		arg->size = nsize;
	}

	memcpy(arg->buffer + arg->offset, buffer, size);
	arg->offset += size;
	assert(arg->offset < arg->size);

	return 0;
}

