/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OBJECT_IDENTIFIER.h>

/*
 * Generate values from the list of interesting values, or just a random
 * value up to the upper limit.
 */
static asn_oid_arc_t
OBJECT_IDENTIFIER__biased_random_arc(asn_oid_arc_t upper_bound) {
    const asn_oid_arc_t values[] = {0, 1, 127, 128, 129, 254, 255, 256};
    size_t idx;

    switch(asn_random_between(0, 2)) {
    case 0:
        idx = asn_random_between(0, sizeof(values) / sizeof(values[0]) - 1);
        if(values[idx] < upper_bound) {
            return values[idx];
        }
        /* Fall through */
    case 1:
        return asn_random_between(0, upper_bound);
    case 2:
    default:
        return upper_bound;
    }
}

asn_random_fill_result_t
OBJECT_IDENTIFIER_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                              const asn_encoding_constraints_t *constraints,
                              size_t max_length) {
    asn_random_fill_result_t result_ok = {ARFILL_OK, 1};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    OBJECT_IDENTIFIER_t *st;
    asn_oid_arc_t arcs[5];
    size_t arcs_len = asn_random_between(2, 5);
    size_t i;

    (void)constraints;

    if(max_length < arcs_len) return result_skipped;

    if(*sptr) {
        st = *sptr;
    } else {
        st = CALLOC(1, sizeof(*st));
    }

    arcs[0] = asn_random_between(0, 2);
    arcs[1] = OBJECT_IDENTIFIER__biased_random_arc(
        arcs[0] <= 1 ? 39 : (ASN_OID_ARC_MAX - 80));
    for(i = 2; i < arcs_len; i++) {
        arcs[i] = OBJECT_IDENTIFIER__biased_random_arc(ASN_OID_ARC_MAX);
    }

    if(OBJECT_IDENTIFIER_set_arcs(st, arcs, arcs_len)) {
        if(st != *sptr) {
            ASN_STRUCT_FREE(*td, st);
        }
        return result_failed;
    }

    *sptr = st;

    result_ok.length = st->size;
    return result_ok;
}
