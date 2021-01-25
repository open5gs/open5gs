/*-
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OCTET_STRING.h>
#include <errno.h>

/*
 * OCTET STRING basic type description.
 */
static const ber_tlv_tag_t asn_DEF_OCTET_STRING_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (4 << 2))
};
asn_OCTET_STRING_specifics_t asn_SPC_OCTET_STRING_specs = {
    sizeof(OCTET_STRING_t),
    offsetof(OCTET_STRING_t, _asn_ctx),
    ASN_OSUBV_STR
};
asn_TYPE_operation_t asn_OP_OCTET_STRING = {
    OCTET_STRING_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    OCTET_STRING_print,  /* OCTET STRING generally means a non-ascii sequence */
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    OCTET_STRING_compare,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    OCTET_STRING_decode_ber,
    OCTET_STRING_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    OCTET_STRING_decode_xer_hex,
    OCTET_STRING_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    OCTET_STRING_decode_oer,
    OCTET_STRING_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    OCTET_STRING_decode_uper,  /* Unaligned PER decoder */
    OCTET_STRING_encode_uper,  /* Unaligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    OCTET_STRING_decode_aper,  /* Aligned PER decoder */
    OCTET_STRING_encode_aper,  /* Aligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    OCTET_STRING_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_OCTET_STRING = {
    "OCTET STRING",  /* Canonical name */
    "OCTET_STRING",  /* XML tag name */
    &asn_OP_OCTET_STRING,
    asn_DEF_OCTET_STRING_tags,
    sizeof(asn_DEF_OCTET_STRING_tags)
        / sizeof(asn_DEF_OCTET_STRING_tags[0]),
    asn_DEF_OCTET_STRING_tags,	/* Same as above */
    sizeof(asn_DEF_OCTET_STRING_tags)
        / sizeof(asn_DEF_OCTET_STRING_tags[0]),
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
    &asn_SPC_OCTET_STRING_specs
};

void
OCTET_STRING_free(const asn_TYPE_descriptor_t *td, void *sptr,
                  enum asn_struct_free_method method) {
	OCTET_STRING_t *st = (OCTET_STRING_t *)sptr;

	if(!td || !st)
		return;

	ASN_DEBUG("Freeing %s as OCTET STRING", td->name);

	if(st->buf) {
		FREEMEM(st->buf);
		st->buf = 0;
	}

#if !defined(ASN_DISABLE_BER_SUPPORT)
    const asn_OCTET_STRING_specifics_t *specs;
    asn_struct_ctx_t *ctx;

    specs = td->specifics
            ? (const asn_OCTET_STRING_specifics_t *)td->specifics
            : &asn_SPC_OCTET_STRING_specs;
    ctx = (asn_struct_ctx_t *)((char *)st + specs->ctx_offset);

    /*
     * Remove decode-time stack.
     */
    struct _stack *stck;
    stck = (struct _stack *)ctx->ptr;
    if(stck) {
        while(stck->tail) {
            struct _stack_el *sel = stck->tail;
            stck->tail = sel->prev;
            FREEMEM(sel);
        }
        FREEMEM(stck);
    }
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */

    switch(method) {
    case ASFM_FREE_EVERYTHING:
        FREEMEM(sptr);
        break;
    case ASFM_FREE_UNDERLYING:
        break;
    case ASFM_FREE_UNDERLYING_AND_RESET:
        memset(sptr, 0,
               td->specifics
                   ? ((const asn_OCTET_STRING_specifics_t *)(td->specifics))
                         ->struct_size
                   : sizeof(OCTET_STRING_t));
        break;
    }
}

/*
 * Conversion routines.
 */
int
OCTET_STRING_fromBuf(OCTET_STRING_t *st, const char *str, int len) {
	void *buf;

	if(st == 0 || (str == 0 && len)) {
		errno = EINVAL;
		return -1;
	}

	/*
	 * Clear the OCTET STRING.
	 */
	if(str == NULL) {
		FREEMEM(st->buf);
		st->buf = 0;
		st->size = 0;
		return 0;
	}

	/* Determine the original string size, if not explicitly given */
	if(len < 0)
		len = strlen(str);

	/* Allocate and fill the memory */
	buf = MALLOC(len + 1);
	if(buf == NULL)
		return -1;

	memcpy(buf, str, len);
	((uint8_t *)buf)[len] = '\0';	/* Couldn't use memcpy(len+1)! */
	FREEMEM(st->buf);
	st->buf = (uint8_t *)buf;
	st->size = len;

	return 0;
}

OCTET_STRING_t *
OCTET_STRING_new_fromBuf(const asn_TYPE_descriptor_t *td, const char *str,
                         int len) {
    const asn_OCTET_STRING_specifics_t *specs =
        td->specifics ? (const asn_OCTET_STRING_specifics_t *)td->specifics
                      : &asn_SPC_OCTET_STRING_specs;
    OCTET_STRING_t *st;

	st = (OCTET_STRING_t *)CALLOC(1, specs->struct_size);
	if(st && str && OCTET_STRING_fromBuf(st, str, len)) {
		FREEMEM(st);
		st = NULL;
	}

	return st;
}

/*
 * Lexicographically compare the common prefix of both strings,
 * and if it is the same return -1 for the smallest string.
 */
int
OCTET_STRING_compare(const asn_TYPE_descriptor_t *td, const void *aptr,
                     const void *bptr) {
    const asn_OCTET_STRING_specifics_t *specs = td->specifics;
    const OCTET_STRING_t *a = aptr;
    const OCTET_STRING_t *b = bptr;

    (void)specs;
    assert(!specs || specs->subvariant != ASN_OSUBV_BIT);

    if(a && b) {
        size_t common_prefix_size = a->size <= b->size ? a->size : b->size;
        int ret = memcmp(a->buf, b->buf, common_prefix_size);
        if(ret == 0) {
            /* Figure out which string with equal prefixes is longer. */
            if(a->size < b->size) {
                return -1;
            } else if(a->size > b->size) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return ret < 0 ? -1 : 1;
        }
    } else if(!a && !b) {
        return 0;
    } else if(!a) {
        return -1;
    } else {
        return 1;
    }

}

#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
int
OCTET_STRING_per_get_characters(asn_per_data_t *po, uint8_t *buf,
                                size_t units, unsigned int bpc, unsigned int unit_bits,
                                long lb, long ub, const asn_per_constraints_t *pc) {
    uint8_t *end = buf + units * bpc;

    ASN_DEBUG("Expanding %d characters into (%ld..%ld):%d",
              (int)units, lb, ub, unit_bits);

    /* X.691: 27.5.4 */
    if((unsigned long)ub <= ((unsigned long)2 << (unit_bits - 1))) {
        /* Decode without translation */
        lb = 0;
    } else if(pc && pc->code2value) {
        if(unit_bits > 16)
            return 1;  /* FATAL: can't have constrained
                        * UniversalString with more than
                        * 16 million code points */
        for(; buf < end; buf += bpc) {
            int value;
            int code = per_get_few_bits(po, unit_bits);
            if(code < 0) return -1;  /* WMORE */
            value = pc->code2value(code);
            if(value < 0) {
                ASN_DEBUG("Code %d (0x%02x) is"
                          " not in map (%ld..%ld)",
                          code, code, lb, ub);
                return 1;  /* FATAL */
            }
            switch(bpc) {
            case 1: *buf = value; break;
            case 2: buf[0] = value >> 8; buf[1] = value; break;
            case 4: buf[0] = value >> 24; buf[1] = value >> 16;
                buf[2] = value >> 8; buf[3] = value; break;
            }
        }
        return 0;
    }

    /* Shortcut the no-op copying to the aligned structure */
    if(lb == 0 && (unit_bits == 8 * bpc)) {
        return per_get_many_bits(po, buf, 0, unit_bits * units);
    }

    for(; buf < end; buf += bpc) {
        int32_t code = per_get_few_bits(po, unit_bits);
        int32_t ch = code + lb;
        if(code < 0) return -1;  /* WMORE */
        if(ch > ub) {
            ASN_DEBUG("Code %d is out of range (%ld..%ld)",
                      ch, lb, ub);
            return 1;  /* FATAL */
        }
        switch(bpc) {
        case 1: *buf = ch; break;
        case 2: buf[0] = ch >> 8; buf[1] = ch; break;
        case 4: buf[0] = ch >> 24; buf[1] = ch >> 16;
            buf[2] = ch >> 8; buf[3] = ch; break;
        }
    }

    return 0;
}

int
OCTET_STRING_per_put_characters(asn_per_outp_t *po, const uint8_t *buf,
                                size_t units, unsigned int bpc, unsigned int unit_bits,
                                long lb, long ub, const asn_per_constraints_t *pc) {
    const uint8_t *end = buf + units * bpc;

    ASN_DEBUG("Squeezing %d characters into (%ld..%ld):%d (%d bpc)",
              (int)units, lb, ub, unit_bits, bpc);

    /* X.691: 27.5.4 */
    if((unsigned long)ub <= ((unsigned long)2 << (unit_bits - 1))) {
        /* Encode as is */
        lb = 0;
    } else if(pc && pc->value2code) {
        for(; buf < end; buf += bpc) {
            int code;
            uint32_t value;
            switch(bpc) {
            case 1: value = *(const uint8_t *)buf; break;
            case 2: value = (buf[0] << 8) | buf[1]; break;
            case 4: value = (buf[0] << 24) | (buf[1] << 16)
                | (buf[2] << 8) | buf[3]; break;
            default: return -1;
            }
            code = pc->value2code(value);
            if(code < 0) {
                ASN_DEBUG("Character %d (0x%02x) is"
                          " not in map (%ld..%ld)",
                          *buf, *buf, lb, ub);
                return -1;
            }
            if(per_put_few_bits(po, code, unit_bits))
    return -1;
        }
    }

    /* Shortcut the no-op copying to the aligned structure */
    if(lb == 0 && (unit_bits == 8 * bpc)) {
        return per_put_many_bits(po, buf, unit_bits * units);
    }

    for(ub -= lb; buf < end; buf += bpc) {
        int ch;
        uint32_t value;
        switch(bpc) {
        case 1:
            value = *(const uint8_t *)buf;
            break;
        case 2:
            value = (buf[0] << 8) | buf[1];
            break;
        case 4:
            value = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
            break;
        default:
            return -1;
        }
        ch = value - lb;
        if(ch < 0 || ch > ub) {
            ASN_DEBUG("Character %d (0x%02x) is out of range (%ld..%ld)", *buf,
                      value, lb, ub + lb);
            return -1;
        }
        if(per_put_few_bits(po, ch, unit_bits)) return -1;
    }

    return 0;
}
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
