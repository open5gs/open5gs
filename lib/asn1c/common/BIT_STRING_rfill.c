/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BIT_STRING.h>

asn_random_fill_result_t
BIT_STRING_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                       const asn_encoding_constraints_t *constraints,
                       size_t max_length) {
    const asn_OCTET_STRING_specifics_t *specs =
        td->specifics ? (const asn_OCTET_STRING_specifics_t *)td->specifics
                      : &asn_SPC_BIT_STRING_specs;
    asn_random_fill_result_t result_ok = {ARFILL_OK, 1};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    static unsigned lengths[] = {0,     1,     2,     3,     4,     8,
                                 126,   127,   128,   16383, 16384, 16385,
                                 65534, 65535, 65536, 65537};
    uint8_t *buf;
    uint8_t *bend;
    uint8_t *b;
    size_t rnd_bits, rnd_len;
    BIT_STRING_t *st;

    if(max_length == 0) return result_skipped;

    switch(specs->subvariant) {
    case ASN_OSUBV_ANY:
        return result_failed;
    case ASN_OSUBV_BIT:
        break;
    default:
        break;
    }

    /* Figure out how far we should go */
    rnd_bits = lengths[asn_random_between(
        0, sizeof(lengths) / sizeof(lengths[0]) - 1)];
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
    if(!constraints || !constraints->per_constraints)
        constraints = &td->encoding_constraints;
    if(constraints->per_constraints) {
        const asn_per_constraint_t *pc = &constraints->per_constraints->size;
        if(pc->flags & APC_CONSTRAINED) {
            long suggested_upper_bound = pc->upper_bound < (ssize_t)max_length
                                             ? pc->upper_bound
                                             : (ssize_t)max_length;
            if(max_length < (size_t)pc->lower_bound) {
                return result_skipped;
            }
            if(pc->flags & APC_EXTENSIBLE) {
                switch(asn_random_between(0, 5)) {
                case 0:
                    if(pc->lower_bound > 0) {
                        rnd_bits = pc->lower_bound - 1;
                        break;
                    }
                    /* Fall through */
                case 1:
                    rnd_bits = pc->upper_bound + 1;
                    break;
                case 2:
                    /* Keep rnd_bits from the table */
                    if(rnd_bits < max_length) {
                        break;
                    }
                    /* Fall through */
                default:
                    rnd_bits = asn_random_between(pc->lower_bound,
                                                  suggested_upper_bound);
                }
            } else {
                rnd_bits =
                    asn_random_between(pc->lower_bound, suggested_upper_bound);
            }
        } else {
            rnd_bits = asn_random_between(0, max_length - 1);
        }
    } else {
#else
    if(!constraints) constraints = &td->encoding_constraints;
    {
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
        if(rnd_bits >= max_length) {
            rnd_bits = asn_random_between(0, max_length - 1);
        }
    }

    rnd_len = (rnd_bits + 7) / 8;
    buf = CALLOC(1, rnd_len + 1);
    if(!buf) return result_failed;

    bend = &buf[rnd_len];

    for(b = buf; b < bend; b++) {
        *(uint8_t *)b = asn_random_between(0, 255);
    }
    *b = 0; /* Zero-terminate just in case. */

    if(*sptr) {
        st = *sptr;
        FREEMEM(st->buf);
    } else {
        st = (BIT_STRING_t *)(*sptr = CALLOC(1, specs->struct_size));
        if(!st) {
            FREEMEM(buf);
            return result_failed;
        }
    }

    st->buf = buf;
    st->size = rnd_len;
    st->bits_unused = (8 - (rnd_bits & 0x7)) & 0x7;
    if(st->bits_unused) {
        assert(st->size > 0);
        st->buf[st->size-1] &= 0xff << st->bits_unused;
    }

    result_ok.length = st->size;
    return result_ok;
}
