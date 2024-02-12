/*-
 * Copyright (c) 2003, 2004 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BIT_STRING.h>

/*
 * BIT STRING basic type description.
 */
static const ber_tlv_tag_t asn_DEF_BIT_STRING_tags[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (3 << 2))
};
asn_OCTET_STRING_specifics_t asn_SPC_BIT_STRING_specs = {
    sizeof(BIT_STRING_t),
    offsetof(BIT_STRING_t, _asn_ctx),
    ASN_OSUBV_BIT
};
asn_TYPE_operation_t asn_OP_BIT_STRING = {
    OCTET_STRING_free,         /* Implemented in terms of OCTET STRING */
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    BIT_STRING_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    BIT_STRING_compare,
    BIT_STRING_copy,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    OCTET_STRING_decode_ber,   /* Implemented in terms of OCTET STRING */
    OCTET_STRING_encode_der,   /* Implemented in terms of OCTET STRING */
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    OCTET_STRING_decode_xer_binary,
    BIT_STRING_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    OCTET_STRING_decode_jer_hex,
    BIT_STRING_encode_jer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    BIT_STRING_decode_oer,
    BIT_STRING_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    BIT_STRING_decode_uper,  /* Unaligned PER decoder */
    BIT_STRING_encode_uper,  /* Unaligned PER encoder */
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
    BIT_STRING_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    0  /* Use generic outmost tag fetcher */
};
asn_TYPE_descriptor_t asn_DEF_BIT_STRING = {
    "BIT STRING",
    "BIT_STRING",
    &asn_OP_BIT_STRING,
    asn_DEF_BIT_STRING_tags,
    sizeof(asn_DEF_BIT_STRING_tags)
      / sizeof(asn_DEF_BIT_STRING_tags[0]),
    asn_DEF_BIT_STRING_tags,  /* Same as above */
    sizeof(asn_DEF_BIT_STRING_tags)
      / sizeof(asn_DEF_BIT_STRING_tags[0]),
    {
#if !defined(ASN_DISABLE_OER_SUPPORT)
        0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
        0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
        BIT_STRING_constraint
    },
    0, 0,  /* No members */
    &asn_SPC_BIT_STRING_specs
};

/*
 * BIT STRING generic constraint.
 */
int
BIT_STRING_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
                      asn_app_constraint_failed_f *ctfailcb, void *app_key) {
    const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;

	if(st && st->buf) {
		if((st->size == 0 && st->bits_unused)
		|| st->bits_unused < 0 || st->bits_unused > 7) {
			ASN__CTFAIL(app_key, td, sptr,
				"%s: invalid padding byte (%s:%d)",
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

/*
 * Non-destructively remove the trailing 0-bits from the given bit string.
 */
const BIT_STRING_t *
BIT_STRING__compactify(const BIT_STRING_t *st, BIT_STRING_t *tmp) {
    const uint8_t *b;
    union {
        const uint8_t *c_buf;
        uint8_t *nc_buf;
    } unconst;

    if(st->size == 0) {
        assert(st->bits_unused == 0);
        return st;
    } else {
        for(b = &st->buf[st->size - 1]; b > st->buf && *b == 0; b--) {
            ;
        }
        /* b points to the last byte which may contain data */
        if(*b) {
            int unused = 7;
            uint8_t v = *b;
            v &= -(int8_t)v;
            if(v & 0x0F) unused -= 4;
            if(v & 0x33) unused -= 2;
            if(v & 0x55) unused -= 1;
            tmp->size = b-st->buf + 1;
            tmp->bits_unused = unused;
        } else {
            tmp->size = b-st->buf;
            tmp->bits_unused = 0;
        }

        assert(b >= st->buf);
    }

    unconst.c_buf = st->buf;
    tmp->buf = unconst.nc_buf;
    return tmp;
}

/*
 * Lexicographically compare the common prefix of both strings,
 * and if it is the same return -1 for the smallest string.
 */
int
BIT_STRING_compare(const asn_TYPE_descriptor_t *td, const void *aptr,
                   const void *bptr) {
    /*
     * Remove information about trailing bits, since
     * X.680 (08/2015) #22.7 "ensure that different semantics are not"
     * "associated with [values that differ only in] the trailing 0 bits."
     */
    BIT_STRING_t compact_a, compact_b;
    const BIT_STRING_t *a = BIT_STRING__compactify(aptr, &compact_a);
    const BIT_STRING_t *b = BIT_STRING__compactify(bptr, &compact_b);
    const asn_OCTET_STRING_specifics_t *specs = td->specifics;

    (void)specs;
    assert(specs && specs->subvariant == ASN_OSUBV_BIT);

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
                /* Figure out how many unused bits */
                if(a->bits_unused > b->bits_unused) {
                    return -1;
                } else if(a->bits_unused < b->bits_unused) {
                    return 1;
                } else {
                    return 0;
                }
            }
        } else {
            return ret;
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
BIT_STRING_copy(const asn_TYPE_descriptor_t *td, void **aptr,
                const void *bptr) {
    const asn_OCTET_STRING_specifics_t *specs = td->specifics;
    BIT_STRING_t *a = (BIT_STRING_t *)*aptr;
    const BIT_STRING_t *b = (const BIT_STRING_t *)bptr;

    if(!b) {
        if(a) {
            FREEMEM(a->buf);
            FREEMEM(a);
            *aptr = 0;
        }
        return 0;
    }

    if(!a) {
        a = *aptr = CALLOC(1, specs->struct_size);
        if(!a) return -1;
    }

    uint8_t* buf = MALLOC(b->size + 1);
    if(!buf) return -1;
    memcpy(buf, b->buf, b->size);
    buf[b->size] = 0;

    FREEMEM(a->buf);
    a->buf = buf;
    a->size = b->size;
    a->bits_unused = b->bits_unused;

    return 0;
}
