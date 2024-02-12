/*
 * Copyright (c) 2003-2019 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <INTEGER.h>
#include <errno.h>
#include <inttypes.h>

/*
 * INTEGER basic type description.
 */
static const ber_tlv_tag_t asn_DEF_INTEGER_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (2 << 2))
};
asn_TYPE_operation_t asn_OP_INTEGER = {
    INTEGER_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    INTEGER_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    INTEGER_compare,
    INTEGER_copy,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    ber_decode_primitive,
    INTEGER_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    INTEGER_decode_xer,
    INTEGER_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    INTEGER_decode_jer,
    INTEGER_encode_jer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    INTEGER_decode_oer,  /* OER decoder */
    INTEGER_encode_oer,  /* Canonical OER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    INTEGER_decode_uper,  /* Unaligned PER decoder */
    INTEGER_encode_uper,  /* Unaligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    INTEGER_decode_aper,  /* Aligned PER decoder */
    INTEGER_encode_aper,  /* Aligned PER encoder */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    INTEGER_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_INTEGER = {
    "INTEGER",
    "INTEGER",
    &asn_OP_INTEGER,
    asn_DEF_INTEGER_tags,
    sizeof(asn_DEF_INTEGER_tags) / sizeof(asn_DEF_INTEGER_tags[0]),
    asn_DEF_INTEGER_tags,	/* Same as above */
    sizeof(asn_DEF_INTEGER_tags) / sizeof(asn_DEF_INTEGER_tags[0]),
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
    0  /* No specifics */
};

/*
 * INTEGER specific human-readable output.
 */
ssize_t
INTEGER__dump(const asn_TYPE_descriptor_t *td, const INTEGER_t *st, asn_app_consume_bytes_f *cb, void *app_key, int plainOrXER) {
    const asn_INTEGER_specifics_t *specs =
        (const asn_INTEGER_specifics_t *)td->specifics;
	char scratch[32];
	uint8_t *buf = st->buf;
	uint8_t *buf_end = st->buf + st->size;
	intmax_t value;
	ssize_t wrote = 0;
	char *p;
	int ret;

	if(specs && specs->field_unsigned)
		ret = asn_INTEGER2umax(st, (uintmax_t *)&value);
	else
		ret = asn_INTEGER2imax(st, &value);

	/* Simple case: the integer size is small */
	if(ret == 0) {
		const asn_INTEGER_enum_map_t *el;
		el = (value >= 0 || !specs || !specs->field_unsigned)
			? INTEGER_map_value2enum(specs, value) : 0;
		if(el) {
			if(plainOrXER == 0)
				return asn__format_to_callback(cb, app_key,
					"%" ASN_PRIdMAX " (%s)", value, el->enum_name);
			else
				return asn__format_to_callback(cb, app_key,
					"<%s/>", el->enum_name);
		} else if(plainOrXER && specs && specs->strict_enumeration) {
			ASN_DEBUG("ASN.1 forbids dealing with "
				"unknown value of ENUMERATED type");
			errno = EPERM;
			return -1;
		} else {
            return asn__format_to_callback(cb, app_key,
                                           (specs && specs->field_unsigned)
                                               ? "%" ASN_PRIuMAX
                                               : "%" ASN_PRIdMAX,
                                           value);
        }
	} else if(plainOrXER && specs && specs->strict_enumeration) {
		/*
		 * Here and earlier, we cannot encode the ENUMERATED values
		 * if there is no corresponding identifier.
		 */
		ASN_DEBUG("ASN.1 forbids dealing with "
			"unknown value of ENUMERATED type");
		errno = EPERM;
		return -1;
	}

	/* Output in the long xx:yy:zz... format */
	/* TODO: replace with generic algorithm (Knuth TAOCP Vol 2, 4.3.1) */
	for(p = scratch; buf < buf_end; buf++) {
		const char * const h2c = "0123456789ABCDEF";
		if((p - scratch) >= (ssize_t)(sizeof(scratch) - 4)) {
			/* Flush buffer */
			if(cb(scratch, p - scratch, app_key) < 0)
				return -1;
			wrote += p - scratch;
			p = scratch;
		}
		*p++ = h2c[*buf >> 4];
		*p++ = h2c[*buf & 0x0F];
		*p++ = 0x3a;	/* ":" */
	}
	if(p != scratch)
		p--;	/* Remove the last ":" */

	wrote += p - scratch;
	return (cb(scratch, p - scratch, app_key) < 0) ? -1 : wrote;
}

static int
INTEGER__compar_value2enum(const void *kp, const void *am) {
	long a = *(const long *)kp;
	const asn_INTEGER_enum_map_t *el = (const asn_INTEGER_enum_map_t *)am;
	long b = el->nat_value;
	if(a < b) return -1;
	else if(a == b) return 0;
	else return 1;
}

const asn_INTEGER_enum_map_t *
INTEGER_map_value2enum(const asn_INTEGER_specifics_t *specs, long value) {
	int count = specs ? specs->map_count : 0;
	if(!count) return 0;
	return (asn_INTEGER_enum_map_t *)bsearch(&value, specs->value2enum,
		count, sizeof(specs->value2enum[0]),
		INTEGER__compar_value2enum);
}

static intmax_t
asn__integer_convert(const uint8_t *b, const uint8_t *end) {
    uintmax_t value;

    /* Perform the sign initialization */
    /* Actually value = -(*b >> 7); gains nothing, yet unreadable! */
    if((*b >> 7)) {
        value = (uintmax_t)(-1);
    } else {
        value = 0;
    }

    /* Conversion engine */
    for(; b < end; b++) {
        value = (value << 8) | *b;
    }

    return value;
}

int
asn_INTEGER2imax(const INTEGER_t *iptr, intmax_t *lptr) {
	uint8_t *b, *end;
	size_t size;

	/* Sanity checking */
	if(!iptr || !iptr->buf || !lptr) {
		errno = EINVAL;
		return -1;
	}

	/* Cache the begin/end of the buffer */
	b = iptr->buf;	/* Start of the INTEGER buffer */
	size = iptr->size;
	end = b + size;	/* Where to stop */

	if(size > sizeof(intmax_t)) {
		uint8_t *end1 = end - 1;
		/*
		 * Slightly more advanced processing,
		 * able to process INTEGERs with >sizeof(intmax_t) bytes
		 * when the actual value is small, e.g. for intmax_t == int32_t
		 * (0x0000000000abcdef INTEGER would yield a fine 0x00abcdef int32_t)
		 */
		/* Skip out the insignificant leading bytes */
		for(; b < end1; b++) {
			switch(*b) {
				case 0x00: if((b[1] & 0x80) == 0) continue; break;
				case 0xff: if((b[1] & 0x80) != 0) continue; break;
			}
			break;
		}

		size = end - b;
		if(size > sizeof(intmax_t)) {
			/* Still cannot fit the sizeof(intmax_t) */
			errno = ERANGE;
			return -1;
		}
	}

	/* Shortcut processing of a corner case */
	if(end == b) {
		*lptr = 0;
		return 0;
	}

	*lptr = asn__integer_convert(b, end);
	return 0;
}

/* FIXME: negative INTEGER values are silently interpreted as large unsigned ones. */
int
asn_INTEGER2umax(const INTEGER_t *iptr, uintmax_t *lptr) {
	uint8_t *b, *end;
	uintmax_t value;
	size_t size;

	if(!iptr || !iptr->buf || !lptr) {
		errno = EINVAL;
		return -1;
	}

	b = iptr->buf;
	size = iptr->size;
	end = b + size;

	/* If all extra leading bytes are zeroes, ignore them */
	for(; size > sizeof(value); b++, size--) {
		if(*b) {
			/* Value won't fit into uintmax_t */
			errno = ERANGE;
			return -1;
		}
	}

	/* Conversion engine */
	for(value = 0; b < end; b++)
		value = (value << 8) | *b;

	*lptr = value;
	return 0;
}

int
asn_umax2INTEGER(INTEGER_t *st, uintmax_t value) {
    uint8_t *buf;
    uint8_t *end;
    uint8_t *b;
    int shr;

    if(value <= ((~(uintmax_t)0) >> 1)) {
        return asn_imax2INTEGER(st, value);
    }

    buf = (uint8_t *)MALLOC(1 + sizeof(value));
    if(!buf) return -1;

    end = buf + (sizeof(value) + 1);
    buf[0] = 0; /* INTEGERs are signed. 0-byte indicates positive. */
    for(b = buf + 1, shr = (sizeof(value) - 1) * 8; b < end; shr -= 8, b++)
        *b = (uint8_t)(value >> shr);

    if(st->buf) FREEMEM(st->buf);
    st->buf = buf;
    st->size = 1 + sizeof(value);

	return 0;
}

int
asn_imax2INTEGER(INTEGER_t *st, intmax_t value) {
	uint8_t *buf, *bp;
	uint8_t *p;
	uint8_t *pstart;
	uint8_t *pend1;
	int littleEndian = 1;	/* Run-time detection */
	int add;

	if(!st) {
		errno = EINVAL;
		return -1;
	}

	buf = (uint8_t *)(long *)MALLOC(sizeof(value));
	if(!buf) return -1;

	if(*(char *)&littleEndian) {
		pstart = (uint8_t *)&value + sizeof(value) - 1;
		pend1 = (uint8_t *)&value;
		add = -1;
	} else {
		pstart = (uint8_t *)&value;
		pend1 = pstart + sizeof(value) - 1;
		add = 1;
	}

	/*
	 * If the contents octet consists of more than one octet,
	 * then bits of the first octet and bit 8 of the second octet:
	 * a) shall not all be ones; and
	 * b) shall not all be zero.
	 */
	for(p = pstart; p != pend1; p += add) {
		switch(*p) {
		case 0x00: if((*(p+add) & 0x80) == 0)
				continue;
			break;
		case 0xff: if((*(p+add) & 0x80))
				continue;
			break;
		}
		break;
	}
	/* Copy the integer body */
	for(bp = buf, pend1 += add; p != pend1; p += add)
		*bp++ = *p;

	if(st->buf) FREEMEM(st->buf);
	st->buf = buf;
	st->size = bp - buf;

	return 0;
}

int
asn_INTEGER2long(const INTEGER_t *iptr, long *l) {
    intmax_t v;
    if(asn_INTEGER2imax(iptr, &v) == 0) {
        if(v < LONG_MIN || v > LONG_MAX) {
            errno = ERANGE;
            return -1;
        }
        *l = v;
        return 0;
    } else {
        return -1;
    }
}

int
asn_INTEGER2ulong(const INTEGER_t *iptr, unsigned long *l) {
    uintmax_t v;
    if(asn_INTEGER2umax(iptr, &v) == 0) {
        if(v > ULONG_MAX) {
            errno = ERANGE;
            return -1;
        }
        *l = v;
        return 0;
    } else {
        return -1;
    }
}

int
asn_long2INTEGER(INTEGER_t *st, long value) {
    return asn_imax2INTEGER(st, value);
}

int
asn_ulong2INTEGER(INTEGER_t *st, unsigned long value) {
    return asn_imax2INTEGER(st, value);
}

int asn_INTEGER2int64(const INTEGER_t *st, int64_t *value) {
    intmax_t v;
    if(asn_INTEGER2imax(st, &v) == 0) {
        if(v < INT64_MIN || v > INT64_MAX) {
            errno = ERANGE;
            return -1;
        }
        *value = v;
        return 0;
    } else {
        return -1;
    }
}

int asn_INTEGER2uint64(const INTEGER_t *st, uint64_t *value) {
    uintmax_t v;
    if(asn_INTEGER2umax(st, &v) == 0) {
        if(v > UINT64_MAX) {
            errno = ERANGE;
            return -1;
        }
        *value = v;
        return 0;
    } else {
        return -1;
    }
}

int
asn_uint642INTEGER(INTEGER_t *st, uint64_t value) {
	uint8_t *buf;
	uint8_t *end;
	uint8_t *b;
	int shr;

	if(value <= INT64_MAX)
		return asn_int642INTEGER(st, value);

	buf = (uint8_t *)MALLOC(1 + sizeof(value));
	if(!buf) return -1;

	end = buf + (sizeof(value) + 1);
	buf[0] = 0;
	for(b = buf + 1, shr = (sizeof(value)-1)*8; b < end; shr -= 8, b++)
		*b = (uint8_t)(value >> shr);

	if(st->buf) FREEMEM(st->buf);
	st->buf = buf;
	st->size = 1 + sizeof(value);

	return 0;
}

int
asn_int642INTEGER(INTEGER_t *st, int64_t value) {
	uint8_t *buf, *bp;
	uint8_t *p;
	uint8_t *pstart;
	uint8_t *pend1;
	int littleEndian = 1;	/* Run-time detection */
	int add;

	if(!st) {
		errno = EINVAL;
		return -1;
	}

	buf = (uint8_t *)MALLOC(sizeof(value));
	if(!buf) return -1;

	if(*(char *)&littleEndian) {
		pstart = (uint8_t *)&value + sizeof(value) - 1;
		pend1 = (uint8_t *)&value;
		add = -1;
	} else {
		pstart = (uint8_t *)&value;
		pend1 = pstart + sizeof(value) - 1;
		add = 1;
	}

	/*
	 * If the contents octet consists of more than one octet,
	 * then bits of the first octet and bit 8 of the second octet:
	 * a) shall not all be ones; and
	 * b) shall not all be zero.
	 */
	for(p = pstart; p != pend1; p += add) {
		switch(*p) {
		case 0x00: if((*(p+add) & 0x80) == 0)
				continue;
			break;
		case 0xff: if((*(p+add) & 0x80))
				continue;
			break;
		}
		break;
	}
	/* Copy the integer body */
	for(pstart = p, bp = buf, pend1 += add; p != pend1; p += add)
		*bp++ = *p;

	if(st->buf) FREEMEM(st->buf);
	st->buf = buf;
	st->size = bp - buf;

	return 0;
}

/*
 * Parse the number in the given string until the given *end position,
 * returning the position after the last parsed character back using the
 * same (*end) pointer.
 * WARNING: This behavior is different from the standard strtol/strtoimax(3).
 */
enum asn_strtox_result_e
asn_strtoimax_lim(const char *str, const char **end, intmax_t *intp) {
    int sign = 1;
    intmax_t value;

    const intmax_t asn1_intmax_max = ((~(uintmax_t)0) >> 1);
    const intmax_t upper_boundary = asn1_intmax_max / 10;
    intmax_t last_digit_max = asn1_intmax_max % 10;

    if(str >= *end) return ASN_STRTOX_ERROR_INVAL;

    switch(*str) {
    case '-':
        last_digit_max++;
        sign = -1;
        /* FALL THROUGH */
    case '+':
        str++;
        if(str >= *end) {
            *end = str;
            return ASN_STRTOX_EXPECT_MORE;
        }
    }

    for(value = 0; str < (*end); str++) {
        if(*str >= 0x30 && *str <= 0x39) {
            int d = *str - '0';
            if(value < upper_boundary) {
                value = value * 10 + d;
            } else if(value == upper_boundary) {
                if(d <= last_digit_max) {
                    if(sign > 0) {
                        value = value * 10 + d;
                    } else {
                        sign = 1;
                        value = -value * 10 - d;
                    }
                    str += 1;
                    if(str < *end) {
                        // If digits continue, we're guaranteed out of range.
                        *end = str;
                        if(*str >= 0x30 && *str <= 0x39) {
                            return ASN_STRTOX_ERROR_RANGE;
                        } else {
                            *intp = sign * value;
                            return ASN_STRTOX_EXTRA_DATA;
                        }
                    }
                    break;
                } else {
                    *end = str;
                    return ASN_STRTOX_ERROR_RANGE;
                }
            } else {
                *end = str;
                return ASN_STRTOX_ERROR_RANGE;
            }
        } else {
            *end = str;
            *intp = sign * value;
            return ASN_STRTOX_EXTRA_DATA;
        }
    }

    *end = str;
    *intp = sign * value;
    return ASN_STRTOX_OK;
}

/*
 * Parse the number in the given string until the given *end position,
 * returning the position after the last parsed character back using the
 * same (*end) pointer.
 * WARNING: This behavior is different from the standard strtoul/strtoumax(3).
 */
enum asn_strtox_result_e
asn_strtoumax_lim(const char *str, const char **end, uintmax_t *uintp) {
    uintmax_t value;

    const uintmax_t asn1_uintmax_max = ((~(uintmax_t)0));
    const uintmax_t upper_boundary = asn1_uintmax_max / 10;
    uintmax_t last_digit_max = asn1_uintmax_max % 10;

    if(str >= *end) return ASN_STRTOX_ERROR_INVAL;

    switch(*str) {
    case '-':
        return ASN_STRTOX_ERROR_INVAL;
    case '+':
        str++;
        if(str >= *end) {
            *end = str;
            return ASN_STRTOX_EXPECT_MORE;
        }
    }

    for(value = 0; str < (*end); str++) {
        if(*str >= 0x30 && *str <= 0x39) {
            unsigned int d = *str - '0';
            if(value < upper_boundary) {
                value = value * 10 + d;
            } else if(value == upper_boundary) {
                if(d <= last_digit_max) {
                    value = value * 10 + d;
                    str += 1;
                    if(str < *end) {
                        // If digits continue, we're guaranteed out of range.
                        *end = str;
                        if(*str >= 0x30 && *str <= 0x39) {
                            return ASN_STRTOX_ERROR_RANGE;
                        } else {
                            *uintp = value;
                            return ASN_STRTOX_EXTRA_DATA;
                        }
                    }
                    break;
                } else {
                    *end = str;
                    return ASN_STRTOX_ERROR_RANGE;
                }
            } else {
                *end = str;
                return ASN_STRTOX_ERROR_RANGE;
            }
        } else {
            *end = str;
            *uintp = value;
            return ASN_STRTOX_EXTRA_DATA;
        }
    }

    *end = str;
    *uintp = value;
    return ASN_STRTOX_OK;
}

enum asn_strtox_result_e
asn_strtol_lim(const char *str, const char **end, long *lp) {
    intmax_t value;
    switch(asn_strtoimax_lim(str, end, &value)) {
    case ASN_STRTOX_ERROR_RANGE:
        return ASN_STRTOX_ERROR_RANGE;
    case ASN_STRTOX_ERROR_INVAL:
        return ASN_STRTOX_ERROR_INVAL;
    case ASN_STRTOX_EXPECT_MORE:
        return ASN_STRTOX_EXPECT_MORE;
    case ASN_STRTOX_OK:
        if(value >= LONG_MIN && value <= LONG_MAX) {
            *lp = value;
            return ASN_STRTOX_OK;
        } else {
            return ASN_STRTOX_ERROR_RANGE;
        }
    case ASN_STRTOX_EXTRA_DATA:
        if(value >= LONG_MIN && value <= LONG_MAX) {
            *lp = value;
            return ASN_STRTOX_EXTRA_DATA;
        } else {
            return ASN_STRTOX_ERROR_RANGE;
        }
    }

    assert(!"Unreachable");
    return ASN_STRTOX_ERROR_INVAL;
}

enum asn_strtox_result_e
asn_strtoul_lim(const char *str, const char **end, unsigned long *ulp) {
    uintmax_t value;
    switch(asn_strtoumax_lim(str, end, &value)) {
    case ASN_STRTOX_ERROR_RANGE:
        return ASN_STRTOX_ERROR_RANGE;
    case ASN_STRTOX_ERROR_INVAL:
        return ASN_STRTOX_ERROR_INVAL;
    case ASN_STRTOX_EXPECT_MORE:
        return ASN_STRTOX_EXPECT_MORE;
    case ASN_STRTOX_OK:
        if(value <= ULONG_MAX) {
            *ulp = value;
            return ASN_STRTOX_OK;
        } else {
            return ASN_STRTOX_ERROR_RANGE;
        }
    case ASN_STRTOX_EXTRA_DATA:
        if(value <= ULONG_MAX) {
            *ulp = value;
            return ASN_STRTOX_EXTRA_DATA;
        } else {
            return ASN_STRTOX_ERROR_RANGE;
        }
    }

    assert(!"Unreachable");
    return ASN_STRTOX_ERROR_INVAL;
}

int
INTEGER_compare(const asn_TYPE_descriptor_t *td, const void *aptr,
                     const void *bptr) {
    const INTEGER_t *a = aptr;
    const INTEGER_t *b = bptr;

    (void)td;

    if(a && b) {
        if(a->size && b->size) {
            int sign_a = (a->buf[0] & 0x80) ? -1 : 1;
            int sign_b = (b->buf[0] & 0x80) ? -1 : 1;

            if(sign_a < sign_b) return -1;
            if(sign_a > sign_b) return 1;

            /* The shortest integer wins, unless comparing negatives */
            if(a->size < b->size) {
                return -1 * sign_a;
            } else if(a->size > b->size) {
                return 1 * sign_b;
            }

            return sign_a * memcmp(a->buf, b->buf, a->size);
        } else if(a->size) {
            int sign = (a->buf[0] & 0x80) ? -1 : 1;
            return (1) * sign;
        } else if(b->size) {
            int sign = (a->buf[0] & 0x80) ? -1 : 1;
            return (-1) * sign;
        } else {
            return 0;
        }
    } else if(!a && !b) {
        return 0;
    } else if(!a) {
        return -1;
    } else {
        return 1;
    }

}

int
INTEGER_copy(const asn_TYPE_descriptor_t *td, void **aptr,
                     const void *bptr) {
    (void)td;
    INTEGER_t *a = *aptr;
    const INTEGER_t *b = bptr;

    if(!b) {
        if(a) {
            FREEMEM(a->buf);
            FREEMEM(a);
            *aptr = 0;
        }
        return 0;
    }

    if(!a) {
        a = *aptr = CALLOC(1, sizeof(*a));
        if(!a) return -1;
    }

    if(b->size) {
        uint8_t* buf = MALLOC(b->size);
        if(!buf) return -1;
        memcpy(buf, b->buf, b->size);
        FREEMEM(a->buf);
        a->buf = buf;
        a->size = b->size;
    } else {
        FREEMEM(a->buf);
        a->buf = 0;
        a->size = 0;
    }

    return 0;
}
