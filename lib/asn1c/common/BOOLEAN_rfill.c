/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BOOLEAN.h>

asn_random_fill_result_t
BOOLEAN_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                    const asn_encoding_constraints_t *constraints,
                    size_t max_length) {
    asn_random_fill_result_t result_ok = {ARFILL_OK, 1};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    BOOLEAN_t *st = *sptr;

    if(max_length == 0) return result_skipped;

    if(st == NULL) {
        st = (BOOLEAN_t *)(*sptr = CALLOC(1, sizeof(*st)));
        if(st == NULL) {
            return result_failed;
        }
    }

#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
    if(!constraints || !constraints->per_constraints)
        constraints = &td->encoding_constraints;
    if(constraints->per_constraints) {
        const asn_per_constraint_t *pc = &constraints->per_constraints->value;
        if(pc->flags & APC_CONSTRAINED) {
            *st = asn_random_between(pc->lower_bound, pc->upper_bound);
            return result_ok;
        }
    }
#else
    if(!constraints) constraints = &td->encoding_constraints;
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */

    /* Simulate booleans that are sloppily set and biased. */
    switch(asn_random_between(0, 7)) {
    case 0:
    case 1:
    case 2:
        *st = 0; break;
    case 3: *st = -1; break;
    case 4: *st = 1; break;
    case 5: *st = INT_MIN; break;
    case 6: *st = INT_MAX; break;
    default:
        *st = asn_random_between(INT_MIN, INT_MAX);
        break;
    }
    return result_ok;
}
