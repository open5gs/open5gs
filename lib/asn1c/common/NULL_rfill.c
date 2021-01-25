/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NULL.h>

asn_random_fill_result_t
NULL_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                    const asn_encoding_constraints_t *constr,
                    size_t max_length) {
    asn_random_fill_result_t result_ok = {ARFILL_OK, 1};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    NULL_t *st = *sptr;

    (void)td;
    (void)constr;

    if(max_length == 0) return result_skipped;

    if(st == NULL) {
        st = (NULL_t *)(*sptr = CALLOC(1, sizeof(*st)));
        if(st == NULL) {
            return result_failed;
        }
    }

    return result_ok;
}
