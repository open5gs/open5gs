/*-
 * Copyright (c) 2003, 2004 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <INTEGER.h>
#include <OBJECT_IDENTIFIER.h>
#include <asn_codecs_prim.h>
#include <limits.h>	/* for CHAR_BIT */
#include <errno.h>
#include <inttypes.h>

/*
 * OBJECT IDENTIFIER basic type description.
 */
static const ber_tlv_tag_t asn_DEF_OBJECT_IDENTIFIER_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (6 << 2))
};
asn_TYPE_operation_t asn_OP_OBJECT_IDENTIFIER = {
    ASN__PRIMITIVE_TYPE_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    OBJECT_IDENTIFIER_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    OCTET_STRING_compare,   /* Implemented in terms of a string comparison */
    OCTET_STRING_copy,      /* Implemented in terms of a string copy */
#if !defined(ASN_DISABLE_BER_SUPPORT)
    ber_decode_primitive,
    der_encode_primitive,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    OBJECT_IDENTIFIER_decode_xer,
    OBJECT_IDENTIFIER_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    OBJECT_IDENTIFIER_decode_jer,
    OBJECT_IDENTIFIER_encode_jer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    OBJECT_IDENTIFIER_decode_oer,
    OBJECT_IDENTIFIER_encode_oer,
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
    OBJECT_IDENTIFIER_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_OBJECT_IDENTIFIER = {
    "OBJECT IDENTIFIER",
    "OBJECT_IDENTIFIER",
    &asn_OP_OBJECT_IDENTIFIER,
    asn_DEF_OBJECT_IDENTIFIER_tags,
    sizeof(asn_DEF_OBJECT_IDENTIFIER_tags)
        / sizeof(asn_DEF_OBJECT_IDENTIFIER_tags[0]),
    asn_DEF_OBJECT_IDENTIFIER_tags,  /* Same as above */
    sizeof(asn_DEF_OBJECT_IDENTIFIER_tags)
        / sizeof(asn_DEF_OBJECT_IDENTIFIER_tags[0]),
    {
#if !defined(ASN_DISABLE_OER_SUPPORT)
        0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
        0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
        OBJECT_IDENTIFIER_constraint
    },
    0, 0,  /* No members */
    0  /* No specifics */
};

int
OBJECT_IDENTIFIER_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
                             asn_app_constraint_failed_f *ctfailcb,
                             void *app_key) {
    const OBJECT_IDENTIFIER_t *st = (const OBJECT_IDENTIFIER_t *)sptr;

	if(st && st->buf) {
		if(st->size < 1) {
			ASN__CTFAIL(app_key, td, sptr,
				"%s: at least one numerical value "
				"expected (%s:%d)",
				td->name, __FILE__, __LINE__);
			return -1;
		}
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}

	return 0;
}

static ssize_t
OBJECT_IDENTIFIER_get_first_arcs(const uint8_t *arcbuf, size_t arcbuf_len,
                                 asn_oid_arc_t *arc0, asn_oid_arc_t *arc1) {
    asn_oid_arc_t value;

    ssize_t rd = OBJECT_IDENTIFIER_get_single_arc(arcbuf, arcbuf_len, &value);
    if(rd <= 0) return rd;

    if(value >= 80) {
        *arc0 = 2;
        *arc1 = value - 80;
    } else if(value >= 40) {
        *arc0 = 1;
        *arc1 = value - 40;
    } else {
        *arc0 = 0;
        *arc1 = value;
    }

    return rd;
}

ssize_t
OBJECT_IDENTIFIER_get_single_arc(const uint8_t *arcbuf, size_t arcbuf_len,
                                 asn_oid_arc_t *ret_value) {
    const uint8_t *b = arcbuf;
    const uint8_t *arcend = arcbuf + arcbuf_len; /* End of arc */

    if(arcbuf == arcend) {
        return 0;
    } else {
        asn_oid_arc_t accum;
	asn_oid_arc_t upper_limit = (ASN_OID_ARC_MAX >> 7);
	/* When the value reaches "upper_limit", it can take */
	/* at most one more digit. If it exceeds "upper_limit" */
	/* but there are more digits - it's an Overflow condition */
        /* Gather all bits into the accumulator */
        for(accum = 0; b < arcend; b++) {
            accum = (accum << 7) | (*b & ~0x80);
            if((*b & 0x80) == 0) { // no more digits
                if(accum <= ASN_OID_ARC_MAX) {
                    *ret_value = accum;
                    return 1 + (b - arcbuf);
                } else {
                    errno = ERANGE; /* Overflow */
                    return -1;
                }
            } else { // to make sure we aren't wrapping around
	      if(accum > upper_limit) {
		    errno = ERANGE; /* Overflow */
		    return -1;
	      }
	    }
        }
        errno = EINVAL;
        return -1;
    }

}

ssize_t
OBJECT_IDENTIFIER__dump_body(const OBJECT_IDENTIFIER_t *st,
                             asn_app_consume_bytes_f *cb, void *app_key) {
    char scratch[32];
    asn_oid_arc_t arc0 = 0;
    asn_oid_arc_t arc1 = 0;
    size_t produced = 0;
    size_t off = 0;
    ssize_t rd;
    int ret;

    rd = OBJECT_IDENTIFIER_get_first_arcs(st->buf, st->size, &arc0, &arc1);
    if(rd <= 0) {
        return -1;
    }

    ret = snprintf(scratch, sizeof(scratch), "%"PRIu32".%"PRIu32, arc0, arc1);
    if(ret >= (ssize_t)sizeof(scratch)) {
        return -1;
    }
    produced += ret;
    if(cb(scratch, ret, app_key) < 0)
        return -1;

    for(off = rd; ; ) {
        asn_oid_arc_t arc;
        rd = OBJECT_IDENTIFIER_get_single_arc(st->buf + off, st->size - off,
                                              &arc);
        if(rd < 0) {
            return -1;
        } else if(rd == 0) {
            /* No more arcs. */
            break;
        } else {
            off += rd;
            assert(off <= st->size);
            ret = snprintf(scratch, sizeof(scratch), ".%" PRIu32, arc);
            if(ret >= (ssize_t)sizeof(scratch)) {
                return -1;
            }
            produced += ret;
            if(cb(scratch, ret, app_key) < 0) return -1;
        }
    }

    if(off != st->size) {
        ASN_DEBUG("Could not scan to the end of Object Identifier");
        return -1;
    }

	return produced;
}

ssize_t
OBJECT_IDENTIFIER_get_arcs(const OBJECT_IDENTIFIER_t *st, asn_oid_arc_t *arcs,
                           size_t arc_slots) {
    asn_oid_arc_t arc0 = 0;
    asn_oid_arc_t arc1 = 0;
    size_t num_arcs = 0;
    size_t off;
    ssize_t rd;

    if(!st || !st->buf) {
        errno = EINVAL;
        return -1;
    }

    rd = OBJECT_IDENTIFIER_get_first_arcs(st->buf, st->size, &arc0, &arc1);
    if(rd <= 0) {
        return -1;
    }
    num_arcs = 2;
    switch(arc_slots) {
    default:
    case 2:
        arcs[1] = arc1;
        /* Fall through */
    case 1:
        arcs[0] = arc0;
        /* Fall through */
    case 0:
        break;
    }

    for(off = rd; ; ) {
        asn_oid_arc_t arc;
        rd = OBJECT_IDENTIFIER_get_single_arc(st->buf + off, st->size - off,
                                              &arc);
        if(rd < 0) {
            return -1;
        } else if(rd == 0) {
            /* No more arcs. */
            break;
        } else {
            off += rd;
            if(num_arcs < arc_slots) {
                arcs[num_arcs] = arc;
            }
            num_arcs++;
        }
    }

    if(off != st->size) {
        return -1;
    }

    return num_arcs;
}


/*
 * Save the single value as an object identifier arc.
 */
ssize_t
OBJECT_IDENTIFIER_set_single_arc(uint8_t *arcbuf, size_t arcbuf_len,
                                 asn_oid_arc_t value) {
    /*
	 * The following conditions must hold:
	 * assert(arcbuf);
	 */
    uint8_t scratch[((sizeof(value) * CHAR_BIT + 6) / 7)];
    uint8_t *scratch_end = &scratch[sizeof(scratch)-1];
    uint8_t *b;
    size_t result_len;
    uint8_t mask;

    for(b = scratch_end, mask = 0; ; mask = 0x80, b--) {
        *b = mask | (value & 0x7f);
        value >>= 7;
        if(!value) {
            break;
        }
    }

    result_len = (scratch_end - b) + 1;

    if(result_len > arcbuf_len) {
        return -1;
    }

    memcpy(arcbuf, b, result_len);

	return result_len;
}

int
OBJECT_IDENTIFIER_set_arcs(OBJECT_IDENTIFIER_t *st, const asn_oid_arc_t *arcs,
                           size_t arc_slots) {
    uint8_t *buf;
    uint8_t *bp;
    ssize_t wrote;
    asn_oid_arc_t arc0;
    asn_oid_arc_t arc1;
    size_t size;
    size_t i;

    if(!st || !arcs || arc_slots < 2) {
        errno = EINVAL;
		return -1;
	}

    arc0 = arcs[0];
    arc1 = arcs[1];

	if(arc0 <= 1) {
		if(arc1 >= 40) {
			/* 8.19.4: At most 39 subsequent values (including 0) */
			errno = ERANGE;
			return -1;
		}
    } else if(arc0 == 2) {
        if(arc1 > ASN_OID_ARC_MAX - 80) {
            errno = ERANGE;
            return -1;
        }
    } else if(arc0 > 2) {
        /* 8.19.4: Only three values are allocated from the root node */
        errno = ERANGE;
        return -1;
    }

    /*
	 * After above tests it is known that the value of arc0 is completely
	 * trustworthy (0..2). However, the arc1's value is still meaningless.
	 */

    /*
     * Roughly estimate the maximum size necessary to encode these arcs.
     * This estimation implicitly takes in account the following facts,
     * that cancel each other:
     * 	* the first two arcs are encoded in a single value.
     * 	* the first value may require more space (+1 byte)
     * 	* the value of the first arc which is in range (0..2)
     */
    size = ((sizeof(asn_oid_arc_t) * CHAR_BIT + 6) / 7) * arc_slots;
    bp = buf = (uint8_t *)MALLOC(size + 1);
    if(!buf) {
        /* ENOMEM */
        return -1;
    }

    wrote = OBJECT_IDENTIFIER_set_single_arc(bp, size, arc0 * 40 + arc1);
    if(wrote <= 0) {
        FREEMEM(buf);
        return -1;
    }
    assert((size_t)wrote <= size);
    bp += wrote;
    size -= wrote;

    for(i = 2; i < arc_slots; i++) {
		wrote = OBJECT_IDENTIFIER_set_single_arc(bp, size, arcs[i]);
        if(wrote <= 0) {
            FREEMEM(buf);
            return -1;
        }
        assert((size_t)wrote <= size);
        bp += wrote;
        size -= wrote;
    }

    /*
	 * Replace buffer.
	 */
	st->size = bp - buf;
	bp = st->buf;
	st->buf = buf;
	st->buf[st->size] = '\0';
	if(bp) FREEMEM(bp);

	return 0;
}

ssize_t
OBJECT_IDENTIFIER_parse_arcs(const char *oid_text, ssize_t oid_txt_length,
                             asn_oid_arc_t *arcs, size_t arcs_count,
                             const char **opt_oid_text_end) {
    size_t num_arcs = 0;
    const char *oid_end;
	enum {
		ST_LEADSPACE,
		ST_TAILSPACE,
		ST_AFTERVALUE,	/* Next character ought to be '.' or a space */
		ST_WAITDIGITS 	/* Next character is expected to be a digit */
	} state = ST_LEADSPACE;

	if(!oid_text || oid_txt_length < -1 || (arcs_count && !arcs)) {
		if(opt_oid_text_end) *opt_oid_text_end = oid_text;
		errno = EINVAL;
		return -1;
	}

	if(oid_txt_length == -1)
		oid_txt_length = strlen(oid_text);

#define _OID_CAPTURE_ARC(oid_text, oid_end)                       \
    do {                                                          \
        const char *endp = oid_end;                               \
        unsigned long value;                                      \
        switch(asn_strtoul_lim(oid_text, &endp, &value)) {        \
        case ASN_STRTOX_EXTRA_DATA:                               \
        case ASN_STRTOX_OK:                                       \
            if(value <= ASN_OID_ARC_MAX) {                        \
                if(num_arcs < arcs_count) arcs[num_arcs] = value; \
                num_arcs++;                                       \
                oid_text = endp - 1;                              \
                break;                                            \
            }                                                     \
            /* Fall through */                                    \
        case ASN_STRTOX_ERROR_RANGE:                              \
            if(opt_oid_text_end) *opt_oid_text_end = oid_text;    \
            errno = ERANGE;                                       \
            return -1;                                            \
        case ASN_STRTOX_ERROR_INVAL:                              \
        case ASN_STRTOX_EXPECT_MORE:                              \
            if(opt_oid_text_end) *opt_oid_text_end = oid_text;    \
            errno = EINVAL;                                       \
            return -1;                                            \
        }                                                         \
    } while(0)

    for(oid_end = oid_text + oid_txt_length; oid_text<oid_end; oid_text++) {
	    switch(*oid_text) {
	    case 0x09: case 0x0a: case 0x0d: case 0x20:	/* whitespace */
		switch(state) {
		case ST_LEADSPACE:
		case ST_TAILSPACE:
			continue;
		case ST_AFTERVALUE:
			state = ST_TAILSPACE;
			continue;
		case ST_WAITDIGITS:
			break;	/* Digits expected after ".", got whitespace */
		}
		break;
	    case 0x2e:	/* '.' */
		switch(state) {
		case ST_LEADSPACE:
		case ST_TAILSPACE:
		case ST_WAITDIGITS:
			if(opt_oid_text_end)
				*opt_oid_text_end = oid_text;
			errno = EINVAL;	/* Broken OID */
			return -1;
			break;
		case ST_AFTERVALUE:
			state = ST_WAITDIGITS;
			continue;
		}
		break;
	    case 0x30: case 0x31: case 0x32: case 0x33: case 0x34:
	    case 0x35: case 0x36: case 0x37: case 0x38: case 0x39:
		switch(state) {
		case ST_TAILSPACE:
		case ST_AFTERVALUE:
			if(opt_oid_text_end)
				*opt_oid_text_end = oid_text;
			errno = EINVAL;	/* "1. 1" => broken OID */
			return -1;
		case ST_LEADSPACE:
		case ST_WAITDIGITS:
			_OID_CAPTURE_ARC(oid_text, oid_end);
			state = ST_AFTERVALUE;
			continue;
		}
		break;
	    default:
		/* Unexpected symbols */
		state = ST_WAITDIGITS;
		break;
	    } /* switch() */
	    break;
	} /* for() */


	if(opt_oid_text_end) *opt_oid_text_end = oid_text;

	/* Finalize last arc */
	switch(state) {
	case ST_LEADSPACE:
		return 0; /* No OID found in input data */
	case ST_WAITDIGITS:
		errno = EINVAL;	/* Broken OID */
		return -1;
	case ST_AFTERVALUE:
	case ST_TAILSPACE:
		return num_arcs;
	}

	errno = EINVAL;	/* Broken OID */
	return -1;
}
