/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <INTEGER.h>

/* Return ceil(log2(v)) for positive v. */
static unsigned
aper_log2_ceil_size(size_t v) {
    unsigned bits = 0;
    size_t power = 1;

    while(power < v) {
        power <<= 1;
        bits++;
    }

    return bits;
}

asn_dec_rval_t
INTEGER_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                    const asn_TYPE_descriptor_t *td,
                    const asn_per_constraints_t *constraints, void **sptr, asn_per_data_t *pd) {
    const asn_INTEGER_specifics_t *specs = (const asn_INTEGER_specifics_t *)td->specifics;
    asn_dec_rval_t rval = { RC_OK, 0 };
    INTEGER_t *st = (INTEGER_t *)*sptr;
    const asn_per_constraint_t *ct;
    asn_per_constraint_t ct_ext_copy;  /* Local copy for extension case */
    int repeat;
    int inext = 0;  /* Track if value is in extension part (outside constraint range) */

    (void)opt_codec_ctx;

    if(!st) {
        st = (INTEGER_t *)(*sptr = CALLOC(1, sizeof(*st)));
        if(!st) ASN__DECODE_FAILED;
    }

    if(!constraints) constraints = td->encoding_constraints.per_constraints;
    ct = constraints ? &constraints->value : 0;

    if(ct && ct->flags & APC_EXTENSIBLE) {
        inext = per_get_few_bits(pd, 1);
        if(inext < 0) ASN__DECODE_STARVED;
        if(inext) {
            /*
             * Value encoded in extension root's extension container.
             *
             * If the original constraint was semi-constrained (only lower_bound
             * present), treat the extension value as semi-constrained too:
             * preserve lower_bound while allowing unconstrained length (range_bits=-1).
             *
             * If the original constraint was fully constrained or unconstrained,
             * extension values MUST be treated as fully unconstrained here.
             */
            if(ct->flags & APC_SEMI_CONSTRAINED) {
                ct_ext_copy = *ct;
                ct_ext_copy.flags = APC_SEMI_CONSTRAINED;
                ct_ext_copy.range_bits = -1;
                ct_ext_copy.effective_bits = -1;
                ct = &ct_ext_copy;
            } else {
                /* keep previous behavior for other cases */
                ct = 0;
            }
        }
    }

    FREEMEM(st->buf);
    st->buf = 0;
    st->size = 0;
    if(ct) {
        if(ct->flags & APC_SEMI_CONSTRAINED) {
            st->buf = (uint8_t *)CALLOC(1, 2);
            if(!st->buf) ASN__DECODE_FAILED;
            st->size = 1;
        } else if(ct->flags & APC_CONSTRAINED && ct->range_bits >= 0) {
            size_t size = (ct->range_bits + 7) >> 3;
            st->buf = (uint8_t *)MALLOC(1 + size + 1);
            if(!st->buf) ASN__DECODE_FAILED;
            st->size = size;
        }
    }

    /* X.691, #12.2.2 */
    if(ct && ct->flags != APC_UNCONSTRAINED) {
        /* #10.5.6 */
        ASN_DEBUG("Integer with range %d bits", ct->range_bits);
        if(ct->range_bits >= 0) {
            if (ct->range_bits > 16) {
                /* X.691 13.2.6: constrained whole number with range > 65536. */
                size_t max_range_bytes = ((size_t)ct->range_bits + 7) >> 3;
                /* Length determinant is (len - 1) in ceil(log2(max_bytes)) bits. */
                unsigned length_bits = aper_log2_ceil_size(max_range_bytes);
                int len_minus_one;
                size_t len;
                intmax_t value = 0;

                len_minus_one = per_get_few_bits(pd, length_bits);
                if(len_minus_one < 0) ASN__DECODE_STARVED;
                len = (size_t)len_minus_one + 1;
                if(len > max_range_bytes || len > sizeof(value)) ASN__DECODE_FAILED;
                ASN_DEBUG("Constrained INTEGER>16 decode: range_bits=%d max_bytes=%" ASN_PRI_SIZE " len_bits=%u len=%" ASN_PRI_SIZE,
                          ct->range_bits, max_range_bytes, length_bits, len);

                if(aper_get_align(pd) < 0) ASN__DECODE_FAILED;

                while(len > 0) {
                    int buf = per_get_few_bits(pd, 8);
                    if(buf < 0) ASN__DECODE_STARVED;
                    if(value > (INTMAX_MAX >> 8)) ASN__DECODE_FAILED;
                    value = (value << 8) | buf;
                    len--;
                }

                if(ct->upper_bound < ct->lower_bound) {
                    ASN__DECODE_FAILED;
                }
                if(value < 0) {
                    /* Offset must not be negative */
                    ASN__DECODE_FAILED;
                } else {
                    intmax_t range = ct->upper_bound - ct->lower_bound;
                    if(range < 0) ASN__DECODE_FAILED;
                    if((uintmax_t)value > (uintmax_t)range) {
                        ASN__DECODE_FAILED;
                    }
                }

                /* Add lower bound; safe after the above checks */
                value += ct->lower_bound;
                
                /* Validate the decoded value is within the constraint bounds */
                if(specs && specs->field_unsigned) {
                    if((uintmax_t)value > (uintmax_t)ct->upper_bound)
                        ASN__DECODE_FAILED;
                    if(asn_umax2INTEGER(st, (uintmax_t)value))
                        ASN__DECODE_FAILED;
                } else {
                    if(value < ct->lower_bound || value > ct->upper_bound)
                        ASN__DECODE_FAILED;
                    if(asn_imax2INTEGER(st, value))
                        ASN__DECODE_FAILED;
                }
                
                ASN_DEBUG("Got value %"ASN_PRIdMAX" + low %"ASN_PRIdMAX"",
                          value, (intmax_t)ct->lower_bound);
            } else {
                intmax_t value = 0;
                if (ct->range_bits < 8) {
                    value = per_get_few_bits(pd, ct->range_bits);
                    if(value < 0) ASN__DECODE_STARVED;
                } else if (ct->range_bits == 8) {
                    if (aper_get_align(pd) < 0)
                        ASN__DECODE_FAILED;
                    value = per_get_few_bits(pd, ct->range_bits);
                    if(value < 0) ASN__DECODE_STARVED;
                } else {
                    /* Align */
                    if (aper_get_align(pd) < 0)
                        ASN__DECODE_FAILED;
                    value = per_get_few_bits(pd, 16);
                    if(value < 0) ASN__DECODE_STARVED;
                }
                value += ct->lower_bound;
                /* Validate the decoded value is within the constraint bounds */
                if(specs && specs->field_unsigned) {
                    if((uintmax_t)value > (uintmax_t)ct->upper_bound)
                        ASN__DECODE_FAILED;
                    if(asn_umax2INTEGER(st, (uintmax_t)value))
                        ASN__DECODE_FAILED;
                } else {
                    if(value < ct->lower_bound || value > ct->upper_bound)
                        ASN__DECODE_FAILED;
                    if(asn_imax2INTEGER(st, value))
                        ASN__DECODE_FAILED;
                }
                ASN_DEBUG("Got value %"ASN_PRIdMAX" + low %"ASN_PRIdMAX"",
                          value, (intmax_t)ct->lower_bound);
            }
            return rval;
        }
        /* Semi-constrained (range_bits < 0): fall through to unconstrained decoding */
    } else {
        ASN_DEBUG("Decoding unconstrained integer %s", td->name);
    }

    /* X.691, #12.2.3, #12.2.4 */
    do {
        ssize_t len;
        void *p;
        int ret;

        /* Get the PER length */
        len = aper_get_length(pd, -1, -1, -1, &repeat);
        if(len < 0) ASN__DECODE_STARVED;

        p = REALLOC(st->buf, st->size + len + 1);
        if(!p) ASN__DECODE_FAILED;
        st->buf = (uint8_t *)p;

        ret = per_get_many_bits(pd, &st->buf[st->size], 0, 8 * len);
        if(ret < 0) ASN__DECODE_STARVED;
        st->size += len;
    } while(repeat);
    st->buf[st->size] = 0;  /* JIC */

    /* #12.2.3 */
    if(ct && ct->lower_bound) {
        /*
         * TODO: replace by in-place arithmetics.
         */
        long value;
        if(asn_INTEGER2long(st, &value))
            ASN__DECODE_FAILED;
        if(asn_long2INTEGER(st, value + ct->lower_bound))
            ASN__DECODE_FAILED;
    }

    return rval;
}

asn_enc_rval_t
INTEGER_encode_aper(const asn_TYPE_descriptor_t *td,
                    const asn_per_constraints_t *constraints,
                    const void *sptr, asn_per_outp_t *po) {
    const asn_INTEGER_specifics_t *specs = (const asn_INTEGER_specifics_t *)td->specifics;
    asn_enc_rval_t er = {0,0,0};
    const INTEGER_t *st = (const INTEGER_t *)sptr;
    const uint8_t *buf;
    const uint8_t *end;
    const asn_per_constraint_t *ct;
    asn_per_constraint_t ct_ext_copy;  /* Local copy for extension case */
    intmax_t value = 0;

    if(!st || st->size == 0) ASN__ENCODE_FAILED;

    if(!constraints) constraints = td->encoding_constraints.per_constraints;
    ct = constraints ? &constraints->value : 0;

    er.encoded = 0;

    if(ct) {
        int inext = 0;
        if(specs && specs->field_unsigned) {
            uintmax_t uval;
            if(asn_INTEGER2umax(st, &uval))
                ASN__ENCODE_FAILED;
            /* Check proper range */
            if(ct->flags & APC_SEMI_CONSTRAINED) {
                if(uval < (uintmax_t)ct->lower_bound)
                    inext = 1;
            } else if(ct->range_bits >= 0) {
                if(uval < (uintmax_t)ct->lower_bound
                        || uval > (uintmax_t)ct->upper_bound)
                    inext = 1;
            }
            ASN_DEBUG("Value %"ASN_PRIdMAX" (%02x/%"ASN_PRI_SIZE") lb %"ASN_PRIdMAX" ub %"ASN_PRIdMAX" %s",
                      uval, st->buf[0], st->size,
                      (intmax_t)ct->lower_bound,
                      (intmax_t)ct->upper_bound,
                      inext ? "ext" : "fix");
            value = uval;
        } else {
            if(asn_INTEGER2imax(st, &value)) ASN__ENCODE_FAILED;
            /* Check proper range */
            if(ct->flags & APC_SEMI_CONSTRAINED) {
                if(value < ct->lower_bound)
                    inext = 1;
            } else if(ct->range_bits >= 0) {
                if(value < ct->lower_bound
                        || value > ct->upper_bound)
                    inext = 1;
            }
            ASN_DEBUG("Value %"ASN_PRIdMAX" (%02x/%"ASN_PRI_SIZE") lb %"ASN_PRIdMAX" ub %"ASN_PRIdMAX" %s",
                      value, st->buf[0], st->size,
                      (intmax_t)ct->lower_bound,
                      (intmax_t)ct->upper_bound,
                      inext ? "ext" : "fix");
        }
        if(ct->flags & APC_EXTENSIBLE) {
            if(per_put_few_bits(po, inext, 1))
                ASN__ENCODE_FAILED;
            if(inext) {
                if(ct->flags & APC_SEMI_CONSTRAINED) {
                    ct_ext_copy = *ct;
                    ct_ext_copy.flags = APC_SEMI_CONSTRAINED;
                    ct_ext_copy.range_bits = -1;
                    ct_ext_copy.effective_bits = -1;
                    ct = &ct_ext_copy;
                } else {
                    ct = 0;
                }
            }
        } else if(inext) {
            ASN__ENCODE_FAILED;
        }
    }

    /* X.691, #12.2.2 */
    if(ct && ct->range_bits >= 0) {
        uintmax_t v;

        /* #10.5.6 */
        ASN_DEBUG("Encoding integer %"ASN_PRIdMAX" (%"ASN_PRIdMAX") with range %d bits",
                  value, (intmax_t)(value - ct->lower_bound),
                  ct->range_bits);

        v = value - ct->lower_bound;

        /* #12 <= 8 -> alignment ? */
        int range = ct->upper_bound - ct->lower_bound + 1;
        if (ct->range_bits < 8 || (ct->range_bits == 8 && range < 256)) {
            if(per_put_few_bits(po, 0x00 | v, ct->range_bits))
                ASN__ENCODE_FAILED;
        } else if (ct->range_bits == 8) {
            if(aper_put_align(po) < 0)
                ASN__ENCODE_FAILED;
            if(per_put_few_bits(po, 0x00 | v, ct->range_bits))
                ASN__ENCODE_FAILED;
        } else if (ct->range_bits <= 16) {
            /* Consume the bytes to align on octet */
            if(aper_put_align(po) < 0)
                ASN__ENCODE_FAILED;
            if(per_put_few_bits(po, 0x0000 | v, 16))
                ASN__ENCODE_FAILED;
        } else {
            /* X.691 13.2.6: constrained whole number with range > 65536. */
            size_t max_range_bytes = ((size_t)ct->range_bits + 7) >> 3;
            /* Determinant width for (num_bytes - 1). */
            unsigned length_bits = aper_log2_ceil_size(max_range_bytes);
            size_t num_bytes = 0;
            uint8_t buf[sizeof(v)];
            uintmax_t tmp = v;

            do {
                num_bytes++;
                tmp >>= 8;
            } while(tmp);

            if(num_bytes > max_range_bytes || num_bytes > sizeof(buf))
                ASN__ENCODE_FAILED;
            ASN_DEBUG("Constrained INTEGER>16 encode: range_bits=%d max_bytes=%" ASN_PRI_SIZE " len_bits=%u len=%" ASN_PRI_SIZE " offset=%" ASN_PRIuMAX,
                      ct->range_bits, max_range_bytes, length_bits, num_bytes, v);

            if(per_put_few_bits(po, num_bytes - 1, length_bits))
                ASN__ENCODE_FAILED;

            if(aper_put_align(po) < 0)
                ASN__ENCODE_FAILED;

            tmp = v;
            {
                size_t i;
                for(i = 0; i < num_bytes; i++) {
                    buf[num_bytes - i - 1] = (uint8_t)(tmp & 0xff);
                    tmp >>= 8;
                }
            }

            if(per_put_many_bits(po, buf, 8 * num_bytes))
                ASN__ENCODE_FAILED;
        }
        ASN__ENCODED_OK(er);
    }

    if(ct && ct->lower_bound) {
        ASN_DEBUG("Adjust lower bound to %"ASN_PRIdMAX"", (intmax_t)ct->lower_bound);
        /*
         * Encode semi-constrained integer by subtracting lower bound.
         * Per X.691, the value is encoded as (value - lower_bound),
         * which is always non-negative.
         */
        INTEGER_t adjusted_int;
        memset(&adjusted_int, 0, sizeof(adjusted_int));

        if(specs && specs->field_unsigned) {
            uintmax_t uval;
            if(asn_INTEGER2umax(st, &uval))
                ASN__ENCODE_FAILED;
            /* Check that value is >= lower_bound to avoid underflow */
            if(uval < (uintmax_t)ct->lower_bound)
                ASN__ENCODE_FAILED;
            uval -= (uintmax_t)ct->lower_bound;
            if(asn_umax2INTEGER(&adjusted_int, uval)) {
                ASN_STRUCT_RESET(asn_DEF_INTEGER, &adjusted_int);
                ASN__ENCODE_FAILED;
            }
        } else {
            intmax_t sval;
            if(asn_INTEGER2imax(st, &sval))
                ASN__ENCODE_FAILED;
            /* Check that value is >= lower_bound */
            if(sval < ct->lower_bound)
                ASN__ENCODE_FAILED;
            sval -= ct->lower_bound;
            if(asn_imax2INTEGER(&adjusted_int, sval)) {
                ASN_STRUCT_RESET(asn_DEF_INTEGER, &adjusted_int);
                ASN__ENCODE_FAILED;
            }
        }

        /* Encode the adjusted value using unconstrained encoding */
        buf = adjusted_int.buf;
        end = adjusted_int.buf + adjusted_int.size;
        while(buf < end) {
            int need_eom = 0;
            ssize_t mayEncode = aper_put_length(po, -1, -1, end - buf, &need_eom);
            if(mayEncode < 0) {
                ASN_STRUCT_RESET(asn_DEF_INTEGER, &adjusted_int);
                ASN__ENCODE_FAILED;
            }
            if(per_put_many_bits(po, buf, 8 * mayEncode)) {
                ASN_STRUCT_RESET(asn_DEF_INTEGER, &adjusted_int);
                ASN__ENCODE_FAILED;
            }
            buf += mayEncode;
            if(need_eom && (aper_put_length(po, -1, -1, 0, NULL) < 0)) {
                ASN_STRUCT_RESET(asn_DEF_INTEGER, &adjusted_int);
                ASN__ENCODE_FAILED;
            }
        }

        ASN_STRUCT_RESET(asn_DEF_INTEGER, &adjusted_int);
        ASN__ENCODED_OK(er);
    }

    for(buf = st->buf, end = st->buf + st->size; buf < end;) {
        int need_eom = 0;
        ssize_t mayEncode = aper_put_length(po, -1, -1, end - buf, &need_eom);
        if(mayEncode < 0)
            ASN__ENCODE_FAILED;
        if(per_put_many_bits(po, buf, 8 * mayEncode))
            ASN__ENCODE_FAILED;
        buf += mayEncode;
        if(need_eom && (aper_put_length(po, -1, -1, 0, NULL) < 0))
            ASN__ENCODE_FAILED;
    }

    ASN__ENCODED_OK(er);
}
