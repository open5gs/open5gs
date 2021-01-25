/*
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SET_OF.h>

asn_TYPE_operation_t asn_OP_SET_OF = {
    SET_OF_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    SET_OF_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    SET_OF_compare,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    SET_OF_decode_ber,
    SET_OF_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    SET_OF_decode_xer,
    SET_OF_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    SET_OF_decode_oer,
    SET_OF_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    SET_OF_decode_uper,
    SET_OF_encode_uper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    SET_OF_decode_aper,
    SET_OF_encode_aper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    SET_OF_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};

/* Append bytes to the above structure */
static int _el_addbytes(const void *buffer, size_t size, void *el_buf_ptr) {
    struct _el_buffer *el_buf = (struct _el_buffer *)el_buf_ptr;

    if(el_buf->length + size > el_buf->allocated_size) {
        size_t new_size = el_buf->allocated_size ? el_buf->allocated_size : 8;
        void *p;

        do {
            new_size <<= 2;
        } while(el_buf->length + size > new_size);

        p = REALLOC(el_buf->buf, new_size);
        if(p) {
            el_buf->buf = p;
            el_buf->allocated_size = new_size;
        } else {
            return -1;
        }
    }

    memcpy(el_buf->buf + el_buf->length, buffer, size);

    el_buf->length += size;
    return 0;
}

static void assert_unused_bits(const struct _el_buffer* p) {
    if(p->length) {
        assert((p->buf[p->length-1] & ~(0xff << p->bits_unused)) == 0);
    } else {
        assert(p->bits_unused == 0);
    }
}

static int _el_buf_cmp(const void *ap, const void *bp) {
    const struct _el_buffer *a = (const struct _el_buffer *)ap;
    const struct _el_buffer *b = (const struct _el_buffer *)bp;
    size_t common_len;
    int ret = 0;

    if(a->length < b->length)
        common_len = a->length;
    else
        common_len = b->length;

    if (a->buf && b->buf) {
        ret = memcmp(a->buf, b->buf, common_len);
    }
    if(ret == 0) {
        if(a->length < b->length)
            ret = -1;
        else if(a->length > b->length)
            ret = 1;
        /* Ignore unused bits. */
        assert_unused_bits(a);
        assert_unused_bits(b);
    }

    return ret;
}

void
SET_OF__encode_sorted_free(struct _el_buffer *el_buf, size_t count) {
    size_t i;

    for(i = 0; i < count; i++) {
        FREEMEM(el_buf[i].buf);
    }

    FREEMEM(el_buf);
}

struct _el_buffer *
SET_OF__encode_sorted(const asn_TYPE_member_t *elm,
                      const asn_anonymous_set_ *list,
                      enum SET_OF__encode_method method) {
    struct _el_buffer *encoded_els;
    int edx;

    encoded_els =
        (struct _el_buffer *)CALLOC(list->count, sizeof(encoded_els[0]));
    if(encoded_els == NULL) {
        return NULL;
    }

	/*
	 * Encode all members.
	 */
    for(edx = 0; edx < list->count; edx++) {
        const void *memb_ptr = list->array[edx];
        struct _el_buffer *encoding_el = &encoded_els[edx];
        asn_enc_rval_t erval = {0,0,0};

        if(!memb_ptr) break;

        /*
		 * Encode the member into the prepared space.
		 */
        switch(method) {
#if !defined(ASN_DISABLE_BER_SUPPORT)
        case SOES_DER:
            erval = elm->type->op->der_encoder(elm->type, memb_ptr, 0, elm->tag,
                                               _el_addbytes, encoding_el);
            break;
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
        case SOES_CUPER:
            erval = uper_encode(elm->type,
                                elm->encoding_constraints.per_constraints,
                                memb_ptr, _el_addbytes, encoding_el);
            if(erval.encoded != -1) {
                size_t extra_bits = erval.encoded % 8;
                assert(encoding_el->length == (size_t)(erval.encoded + 7) / 8);
                encoding_el->bits_unused = (8 - extra_bits) & 0x7;
            }
            break;
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
        case SOES_CAPER:
            erval = aper_encode(elm->type,
                                elm->encoding_constraints.per_constraints,
                                memb_ptr, _el_addbytes, encoding_el);
            if(erval.encoded != -1) {
                size_t extra_bits = erval.encoded % 8;
                assert(encoding_el->length == (size_t)(erval.encoded + 7) / 8);
                encoding_el->bits_unused = (8 - extra_bits) & 0x7;
            }
            break;
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */

        default:
            assert(!"Unreachable");
            break;
        }
        if(erval.encoded < 0) break;
	}

    if(edx == list->count) {
        /*
         * Sort the encoded elements according to their encoding.
         */
        qsort(encoded_els, list->count, sizeof(encoded_els[0]), _el_buf_cmp);

        return encoded_els;
    } else {
        SET_OF__encode_sorted_free(encoded_els, edx);
        return NULL;
    }
}

void
SET_OF_free(const asn_TYPE_descriptor_t *td, void *ptr,
            enum asn_struct_free_method method) {
    if(td && ptr) {
		const asn_SET_OF_specifics_t *specs;
		asn_TYPE_member_t *elm = td->elements;
		asn_anonymous_set_ *list = _A_SET_FROM_VOID(ptr);
		asn_struct_ctx_t *ctx;	/* Decoder context */
		int i;

		/*
		 * Could not use set_of_empty() because of (*free)
		 * incompatibility.
		 */
		for(i = 0; i < list->count; i++) {
			void *memb_ptr = list->array[i];
			if(memb_ptr)
			ASN_STRUCT_FREE(*elm->type, memb_ptr);
		}
		list->count = 0;	/* No meaningful elements left */

		asn_set_empty(list);	/* Remove (list->array) */

		specs = (const asn_SET_OF_specifics_t *)td->specifics;
		ctx = (asn_struct_ctx_t *)((char *)ptr + specs->ctx_offset);
		if(ctx->ptr) {
			ASN_STRUCT_FREE(*elm->type, ctx->ptr);
			ctx->ptr = 0;
		}

        switch(method) {
        case ASFM_FREE_EVERYTHING:
            FREEMEM(ptr);
            break;
        case ASFM_FREE_UNDERLYING:
            break;
        case ASFM_FREE_UNDERLYING_AND_RESET:
            memset(ptr, 0, specs->struct_size);
            break;
        }
    }
}

int
SET_OF_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
                  asn_app_constraint_failed_f *ctfailcb, void *app_key) {
    const asn_TYPE_member_t *elm = td->elements;
	asn_constr_check_f *constr;
	const asn_anonymous_set_ *list = _A_CSET_FROM_VOID(sptr);
	int i;

	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}

	constr = elm->encoding_constraints.general_constraints;
	if(!constr) constr = elm->type->encoding_constraints.general_constraints;

	/*
	 * Iterate over the members of an array.
	 * Validate each in turn, until one fails.
	 */
	for(i = 0; i < list->count; i++) {
		const void *memb_ptr = list->array[i];
		int ret;

		if(!memb_ptr) continue;

		ret = constr(elm->type, memb_ptr, ctfailcb, app_key);
		if(ret) return ret;
	}

	return 0;
}

struct comparable_ptr {
    const asn_TYPE_descriptor_t *td;
    const void *sptr;
};

static int
SET_OF__compare_cb(const void *aptr, const void *bptr) {
    const struct comparable_ptr *a = aptr;
    const struct comparable_ptr *b = bptr;
    assert(a->td == b->td);
    return a->td->op->compare_struct(a->td, a->sptr, b->sptr);
}

int
SET_OF_compare(const asn_TYPE_descriptor_t *td, const void *aptr,
               const void *bptr) {
    const asn_anonymous_set_ *a = _A_CSET_FROM_VOID(aptr);
    const asn_anonymous_set_ *b = _A_CSET_FROM_VOID(bptr);

    if(a && b) {
        struct comparable_ptr *asorted;
        struct comparable_ptr *bsorted;
        ssize_t common_length;
        ssize_t idx;

        if(a->count == 0) {
            if(b->count) return -1;
            return 0;
        } else if(b->count == 0) {
            return 1;
        }

        asorted = MALLOC(a->count * sizeof(asorted[0]));
        bsorted = MALLOC(b->count * sizeof(bsorted[0]));
        if(!asorted || !bsorted) {
            FREEMEM(asorted);
            FREEMEM(bsorted);
            return -1;
        }

        for(idx = 0; idx < a->count; idx++) {
            asorted[idx].td = td->elements->type;
            asorted[idx].sptr = a->array[idx];
        }

        for(idx = 0; idx < b->count; idx++) {
            bsorted[idx].td = td->elements->type;
            bsorted[idx].sptr = b->array[idx];
        }

        qsort(asorted, a->count, sizeof(asorted[0]), SET_OF__compare_cb);
        qsort(bsorted, b->count, sizeof(bsorted[0]), SET_OF__compare_cb);

        common_length = (a->count < b->count ? a->count : b->count);
        for(idx = 0; idx < common_length; idx++) {
            int ret = td->elements->type->op->compare_struct(
                td->elements->type, asorted[idx].sptr, bsorted[idx].sptr);
            if(ret) {
                FREEMEM(asorted);
                FREEMEM(bsorted);
                return ret;
            }
        }

        FREEMEM(asorted);
        FREEMEM(bsorted);

        if(idx < b->count) /* more elements in b */
            return -1;     /* a is shorter, so put it first */
        if(idx < a->count) return 1;
    } else if(!a) {
        return -1;
    } else if(!b) {
        return 1;
    }

	return 0;
}
