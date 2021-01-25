/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SET_OF.h>

asn_random_fill_result_t
SET_OF_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                   const asn_encoding_constraints_t *constraints,
                   size_t max_length) {
    const asn_SET_OF_specifics_t *specs =
        (const asn_SET_OF_specifics_t *)td->specifics;
    asn_random_fill_result_t res_ok = {ARFILL_OK, 0};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    const asn_TYPE_member_t *elm = td->elements;
    void *st = *sptr;
    long max_elements = 5;
    long slb = 0;  /* Lower size bound */
    long sub = 0;  /* Upper size bound */
    size_t rnd_len;

    if(max_length == 0) return result_skipped;

    if(st == NULL) {
        st = (*sptr = CALLOC(1, specs->struct_size));
        if(st == NULL) {
            return result_failed;
        }
    }

    switch(asn_random_between(0, 6)) {
    case 0: max_elements = 0; break;
    case 1: max_elements = 1; break;
    case 2: max_elements = 5; break;
    case 3: max_elements = max_length; break;
    case 4: max_elements = max_length / 2; break;
    case 5: max_elements = max_length / 4; break;
    default: break;
    }
    sub = slb + max_elements;

#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
    if(!constraints || !constraints->per_constraints)
        constraints = &td->encoding_constraints;
    if(constraints->per_constraints) {
        const asn_per_constraint_t *pc = &constraints->per_constraints->size;
        if(pc->flags & APC_SEMI_CONSTRAINED) {
            slb = pc->lower_bound;
            sub = pc->lower_bound + max_elements;
        } else if(pc->flags & APC_CONSTRAINED) {
            slb = pc->lower_bound;
            sub = pc->upper_bound;
            if(sub - slb > max_elements) sub = slb + max_elements;
        }
    }
#else
    if(!constraints) constraints = &td->encoding_constraints;
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */

    /* Bias towards edges of allowed space */
    switch(asn_random_between(-1, 4)) {
    default:
    case -1:
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
        /* Prepare lengths somewhat outside of constrained range. */
        if(constraints->per_constraints
           && (constraints->per_constraints->size.flags & APC_EXTENSIBLE)) {
            switch(asn_random_between(0, 5)) {
            default:
            case 0:
                rnd_len = 0;
                break;
            case 1:
                if(slb > 0) {
                    rnd_len = slb - 1;
                } else {
                    rnd_len = 0;
                }
                break;
            case 2:
                rnd_len = asn_random_between(0, slb);
                break;
            case 3:
                if(sub < (ssize_t)max_length) {
                    rnd_len = sub + 1;
                } else {
                    rnd_len = max_length;
                }
                break;
            case 4:
                if(sub < (ssize_t)max_length) {
                    rnd_len = asn_random_between(sub + 1, max_length);
                } else {
                    rnd_len = max_length;
                }
                break;
            case 5:
                rnd_len = max_length;
                break;
            }
            break;
        }
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
        /* Fall through */
    case 0:
        rnd_len = asn_random_between(slb, sub);
        break;
    case 1:
        if(slb < sub) {
            rnd_len = asn_random_between(slb + 1, sub);
            break;
        }
        /* Fall through */
    case 2:
        rnd_len = asn_random_between(slb, slb);
        break;
    case 3:
        if(slb < sub) {
            rnd_len = asn_random_between(slb, sub - 1);
            break;
        }
        /* Fall through */
    case 4:
        rnd_len = asn_random_between(sub, sub);
        break;
    }

    for(; rnd_len > 0; rnd_len--) {
        asn_anonymous_set_ *list = _A_SET_FROM_VOID(st);
        void *ptr = 0;
        asn_random_fill_result_t tmpres = elm->type->op->random_fill(
            elm->type, &ptr, &elm->encoding_constraints,
            (max_length > res_ok.length ? max_length - res_ok.length : 0)
                / rnd_len);
        switch(tmpres.code) {
        case ARFILL_OK:
            ASN_SET_ADD(list, ptr);
            res_ok.length += tmpres.length;
            break;
        case ARFILL_SKIPPED:
            break;
        case ARFILL_FAILED:
            assert(ptr == 0);
            return tmpres;
        }
    }

    return res_ok;
}
