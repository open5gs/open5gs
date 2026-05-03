/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <UTF8String.h>

/*
 * Biased function for randomizing UTF-8 sequences.
 */
static size_t
UTF8String__random_char(uint8_t *b, size_t size) {
    static const struct rnd_value {
        const char *value;
        size_t size;
    } values[] = {{"\0", 1},
                  {"\x01", 1},
                  {"\x7f", 1},
                  {"\xc2\xa2", 2},
                  {"\xe2\x82\xac", 3},
                  {"\xf0\x90\x8d\x88", 4},
                  {"\xf4\x8f\xbf\xbf", 4}};

    const struct rnd_value *v;
    size_t max_idx = 0;

    switch(size) {
    case 0:
        assert(size != 0);
        return 0;
    case 1:
        max_idx = 2;
        break;
    case 2:
        max_idx = 3;
        break;
    default:
    case 4:
        max_idx = sizeof(values) / sizeof(values[0]) - 1;
        break;
    }

    v = &values[asn_random_between(0, max_idx)];
    memcpy(b, v->value, v->size);
    return v->size;
}

static size_t
UTF8String__encode_codepoint(uint8_t *b, uint32_t code) {
    if(code <= 0x7f) {
        b[0] = code;
        return 1;
    } else if(code <= 0x7ff) {
        b[0] = 0xc0 | (code >> 6);
        b[1] = 0x80 | (code & 0x3f);
        return 2;
    } else if(code >= 0xd800 && code <= 0xdfff) {
        /* Surrogate code points are not valid Unicode scalar values. */
        return UTF8String__encode_codepoint(b, 0xfffd);
    } else if(code <= 0xffff) {
        b[0] = 0xe0 | (code >> 12);
        b[1] = 0x80 | ((code >> 6) & 0x3f);
        b[2] = 0x80 | (code & 0x3f);
        return 3;
    } else if(code <= 0x10ffff) {
        b[0] = 0xf0 | (code >> 18);
        b[1] = 0x80 | ((code >> 12) & 0x3f);
        b[2] = 0x80 | ((code >> 6) & 0x3f);
        b[3] = 0x80 | (code & 0x3f);
        return 4;
    } else {
        return UTF8String__encode_codepoint(b, 0x10ffff);
    }
}

static const asn_per_constraints_t *
UTF8String__per_constraints(const asn_TYPE_descriptor_t *td,
                            const asn_encoding_constraints_t *constraints) {
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
    if(constraints && constraints->per_constraints)
        return constraints->per_constraints;
    if(td->encoding_constraints.per_constraints)
        return td->encoding_constraints.per_constraints;
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
    return 0;
}

asn_random_fill_result_t
UTF8String_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                       const asn_encoding_constraints_t *constraints,
                       size_t max_length) {
    asn_random_fill_result_t result_ok = {ARFILL_OK, 1};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    uint8_t *buf;
    uint8_t *bend;
    uint8_t *b;
    size_t rnd_len;
    size_t idx;
    UTF8String_t *st;
    const asn_per_constraints_t *pc;
    uint32_t value_lb = 0;
    uint32_t value_ub = 0x7f;
    int has_value_constraint = 0;
    int force_single_octets = 0;

    if(max_length == 0 && !*sptr) return result_skipped;

    pc = UTF8String__per_constraints(td, constraints);
    if(pc) {
        if((pc->value.flags & APC_CONSTRAINED)
                && pc->value.lower_bound >= 0
                && pc->value.lower_bound <= pc->value.upper_bound
                && pc->value.lower_bound <= 0x10ffff) {
            value_lb = (uint32_t)pc->value.lower_bound;
            value_ub = pc->value.upper_bound > 0x10ffff
                     ? 0x10ffff : (uint32_t)pc->value.upper_bound;
            has_value_constraint = 1;
        }

        /*
         * UTF8String is encoded through OCTET STRING helpers.  When a PER
         * SIZE constraint is present, the encoder checks the number of
         * octets.  Keep randomized values single-octet unless a permitted
         * alphabet constraint explicitly asks for another range.
         */
        if(pc->size.flags & APC_CONSTRAINED)
            force_single_octets = 1;
    }

    /*
     * When both a value constraint and a SIZE constraint are present,
     * restrict generation to code points in the value range that also
     * fit in a single octet (0..0x7f).  If the value range does not
     * intersect 0..0x7f at all, lift the single-octet restriction and
     * generate from the full value range.
     */
    if(has_value_constraint && force_single_octets) {
        if(value_lb > 0x7f) {
            /* Value range is entirely outside ASCII; allow multi-byte. */
            force_single_octets = 0;
        } else {
            /* Clamp value range to single-octet region. */
            if(value_ub > 0x7f)
                value_ub = 0x7f;
        }
    }

    /* Figure out how far we should go */
    rnd_len = OCTET_STRING_random_length_constrained(td, constraints,
                                                     max_length / 4);

    buf = CALLOC(4, rnd_len + 1);
    if(!buf) return result_failed;

    bend = &buf[4 * rnd_len];

    for(b = buf, idx = 0; idx < rnd_len; idx++) {
        if(has_value_constraint) {
            uint32_t code = (uint32_t)asn_random_between(value_lb, value_ub);
            b += UTF8String__encode_codepoint(b, code);
        } else if(force_single_octets) {
            b += UTF8String__random_char(b, 1);
        } else {
            b += UTF8String__random_char(b, (bend - b));
        }
    }
    *(uint8_t *)b = 0;

    if(*sptr) {
        st = *sptr;
        FREEMEM(st->buf);
    } else {
        st = (OCTET_STRING_t *)(*sptr = CALLOC(1, sizeof(UTF8String_t)));
        if(!st) {
            FREEMEM(buf);
            return result_failed;
        }
    }

    st->buf = buf;
    st->size = b - buf;

    assert(UTF8String_length(st) == (ssize_t)rnd_len);

    return result_ok;
}
