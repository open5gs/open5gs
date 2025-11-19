/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <INTEGER.h>

asn_dec_rval_t
INTEGER_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                    const asn_TYPE_descriptor_t *td,
                    const asn_per_constraints_t *constraints, void **sptr, asn_per_data_t *pd) {
    const asn_INTEGER_specifics_t *specs = (const asn_INTEGER_specifics_t *)td->specifics;
    asn_dec_rval_t rval = { RC_OK, 0 };
    INTEGER_t *st = (INTEGER_t *)*sptr;
    const asn_per_constraint_t *ct;
    int repeat;

    (void)opt_codec_ctx;

    if(!st) {
        st = (INTEGER_t *)(*sptr = CALLOC(1, sizeof(*st)));
        if(!st) ASN__DECODE_FAILED;
    }

    if(!constraints) constraints = td->encoding_constraints.per_constraints;
    ct = constraints ? &constraints->value : 0;

    if(ct && ct->flags & APC_EXTENSIBLE) {
        int inext = per_get_few_bits(pd, 1);
        if(inext < 0) ASN__DECODE_STARVED;
        if(inext) ct = 0;
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
                int max_range_bytes = (ct->range_bits >> 3) +
                                      (((ct->range_bits % 8) > 0) ? 1 : 0);
                int length = 0, i;
                intmax_t value = 0;

                for (i = 1; ; i++) {
                    int upper = 1 << i;
                    if (upper >= max_range_bytes)
                        break;
                }
                ASN_DEBUG("Can encode %d (%d bytes) in %d bits", ct->range_bits,
                          max_range_bytes, i);

                if ((length = per_get_few_bits(pd, i)) < 0)
                    ASN__DECODE_FAILED;

                /* X.691 #12.2.6 length determinant + lb (1) */
                length += 1;
                ASN_DEBUG("Got length %d", length);
                if (aper_get_align(pd) != 0)
                    ASN__DECODE_FAILED;
                while (length--) {
                    int buf = per_get_few_bits(pd, 8);
                    if (buf < 0)
                        ASN__DECODE_FAILED;
                    value += (((intmax_t)buf) << (8 * length));
                }

                value += ct->lower_bound;
                if((specs && specs->field_unsigned)
                        ? asn_umax2INTEGER(st, (uintmax_t)value)
                        : asn_imax2INTEGER(st, value))
                    ASN__DECODE_FAILED;
                ASN_DEBUG("Got value %ld + low %lld",
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
                if((specs && specs->field_unsigned)
                        ? asn_umax2INTEGER(st, (uintmax_t)value)
                        : asn_imax2INTEGER(st, value))
                    ASN__DECODE_FAILED;
                ASN_DEBUG("Got value %ld + low %lld",
                          value, (intmax_t)ct->lower_bound);
            }
            return rval;
        } else {
            ASN__DECODE_FAILED;
        }
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
            ASN_DEBUG("Value %lu (%02x/%zu) lb %lld ub %lld %s",
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
            ASN_DEBUG("Value %lu (%02x/%zu) lb %lld ub %lld %s",
                      value, st->buf[0], st->size,
                      (intmax_t)ct->lower_bound,
                      (intmax_t)ct->upper_bound,
                      inext ? "ext" : "fix");
        }
        if(ct->flags & APC_EXTENSIBLE) {
            if(per_put_few_bits(po, inext, 1))
                ASN__ENCODE_FAILED;
            if(inext) ct = 0;
        } else if(inext) {
            ASN__ENCODE_FAILED;
        }
    }

    /* X.691, #12.2.2 */
    if(ct && ct->range_bits >= 0) {
        uintmax_t v;

        /* #10.5.6 */
        ASN_DEBUG("Encoding integer %ld (%lld) with range %d bits",
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
            /* TODO: extend to >64 bits */
            int64_t v64 = v;
            int i, j;
            int max_range_bytes = (ct->range_bits >> 3) +
                                  (((ct->range_bits % 8) > 0) ? 1 : 0);

            for (i = 1; ; i++) {
                int upper = 1 << i;
                if (upper >= max_range_bytes)
                    break;
            }

            for (j = sizeof(int64_t) -1; j != 0; j--) {
                int64_t val;
                val = v64 >> (j * 8);
                if (val != 0)
                    break;
            }

            /* Putting length in the minimum number of bits ex: 5 = 3bits */
            if (per_put_few_bits(po, j, i))
                ASN__ENCODE_FAILED;

            /* Consume the bits to align on octet */
            if (aper_put_align(po) < 0)
                ASN__ENCODE_FAILED;
            /* Put the value */
            for (i = 0; i <= j; i++) {
                if(per_put_few_bits(po, (v64 >> (8 * (j - i))) & 0xff, 8))
                    ASN__ENCODE_FAILED;
            }
        }
        ASN__ENCODED_OK(er);
    }

    if(ct && ct->lower_bound) {
        ASN_DEBUG("Adjust lower bound to %lld", (intmax_t)ct->lower_bound);
        /* TODO: adjust lower bound */
        ASN__ENCODE_FAILED;
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
