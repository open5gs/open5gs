/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OCTET_STRING.h>

/*
 * Biased function for randomizing character values around their limits.
 */
static uint32_t
OCTET_STRING__random_char(unsigned long lb, unsigned long ub) {
    assert(lb <= ub);
    switch(asn_random_between(0, 16)) {
    case 0:
        if(lb < ub) return lb + 1;
        /* Fall through */
    case 1:
        return lb;
    case 2:
        if(lb < ub) return ub - 1;
        /* Fall through */
    case 3:
        return ub;
    default:
        return asn_random_between(lb, ub);
    }
}

asn_random_fill_result_t
OCTET_STRING_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                         const asn_encoding_constraints_t *constraints,
                         size_t max_length) {
    const asn_OCTET_STRING_specifics_t *specs = td->specifics
        ? (const asn_OCTET_STRING_specifics_t *)td->specifics
        : &asn_SPC_OCTET_STRING_specs;
    asn_random_fill_result_t result_ok = {ARFILL_OK, 1};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    unsigned int unit_bytes = 1;
    unsigned long clb = 0;  /* Lower bound on char */
    unsigned long cub = 255;  /* Higher bound on char value */
    uint8_t *buf;
    uint8_t *bend;
    uint8_t *b;
    size_t rnd_len;
    OCTET_STRING_t *st;

    if(max_length == 0 && !*sptr) return result_skipped;

    switch(specs->subvariant) {
    default:
    case ASN_OSUBV_ANY:
        return result_failed;
    case ASN_OSUBV_BIT:
        /* Handled by BIT_STRING itself. */
        return result_failed;
    case ASN_OSUBV_STR:
        unit_bytes = 1;
        clb = 0;
        cub = 255;
        break;
    case ASN_OSUBV_U16:
        unit_bytes = 2;
        clb = 0;
        cub = 65535;
        break;
    case ASN_OSUBV_U32:
        unit_bytes = 4;
        clb = 0;
        cub = 0x10FFFF;
        break;
    }

#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
    if(!constraints || !constraints->per_constraints)
        constraints = &td->encoding_constraints;
    if(constraints->per_constraints) {
        const asn_per_constraint_t *pc = &constraints->per_constraints->value;
        if(pc->flags & APC_SEMI_CONSTRAINED) {
            clb = pc->lower_bound;
        } else if(pc->flags & APC_CONSTRAINED) {
            clb = pc->lower_bound;
            cub = pc->upper_bound;
        }
    }
#else
    if(!constraints) constraints = &td->encoding_constraints;
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */

    rnd_len =
        OCTET_STRING_random_length_constrained(td, constraints, max_length);

    buf = CALLOC(unit_bytes, rnd_len + 1);
    if(!buf) return result_failed;

    bend = &buf[unit_bytes * rnd_len];

    switch(unit_bytes) {
    case 1:
        for(b = buf; b < bend; b += unit_bytes) {
            *(uint8_t *)b = OCTET_STRING__random_char(clb, cub);
        }
        *(uint8_t *)b = 0;
        break;
    case 2:
        for(b = buf; b < bend; b += unit_bytes) {
            uint32_t code = OCTET_STRING__random_char(clb, cub);
            b[0] = code >> 8;
            b[1] = code;
        }
        *(uint16_t *)b = 0;
        break;
    case 4:
        for(b = buf; b < bend; b += unit_bytes) {
            uint32_t code = OCTET_STRING__random_char(clb, cub);
            b[0] = code >> 24;
            b[1] = code >> 16;
            b[2] = code >> 8;
            b[3] = code;
        }
        *(uint32_t *)b = 0;
        break;
    }

    if(*sptr) {
        st = *sptr;
        FREEMEM(st->buf);
    } else {
        st = (OCTET_STRING_t *)(*sptr = CALLOC(1, specs->struct_size));
        if(!st) {
            FREEMEM(buf);
            return result_failed;
        }
    }

    st->buf = buf;
    st->size = unit_bytes * rnd_len;

    result_ok.length = st->size;
    return result_ok;
}

size_t
OCTET_STRING_random_length_constrained(
    const asn_TYPE_descriptor_t *td,
    const asn_encoding_constraints_t *constraints, size_t max_length) {
    const unsigned lengths[] = {0,     1,     2,     3,     4,     8,
                                126,   127,   128,   16383, 16384, 16385,
                                65534, 65535, 65536, 65537};
    size_t rnd_len;

    /* Figure out how far we should go */
    rnd_len = lengths[asn_random_between(
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
            if(max_length <= (size_t)pc->lower_bound) {
                return pc->lower_bound;
            }
            if(pc->flags & APC_EXTENSIBLE) {
                switch(asn_random_between(0, 5)) {
                case 0:
                    if(pc->lower_bound > 0) {
                        rnd_len = pc->lower_bound - 1;
                        break;
                    }
                    /* Fall through */
                case 1:
                    rnd_len = pc->upper_bound + 1;
                    break;
                case 2:
                    /* Keep rnd_len from the table */
                    if(rnd_len <= max_length) {
                        break;
                    }
                    /* Fall through */
                default:
                    rnd_len = asn_random_between(pc->lower_bound,
                                                 suggested_upper_bound);
                }
            } else {
                rnd_len =
                    asn_random_between(pc->lower_bound, suggested_upper_bound);
            }
        } else {
            rnd_len = asn_random_between(0, max_length);
        }
    } else {
#else
    if(!constraints) constraints = &td->encoding_constraints;
    {
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
        if(rnd_len > max_length) {
            rnd_len = asn_random_between(0, max_length);
        }
    }

    return rnd_len;
}
