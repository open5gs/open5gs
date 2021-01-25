/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE.h>

asn_random_fill_result_t
SEQUENCE_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                   const asn_encoding_constraints_t *constr,
                   size_t max_length) {
    const asn_SEQUENCE_specifics_t *specs =
        (const asn_SEQUENCE_specifics_t *)td->specifics;
    asn_random_fill_result_t result_ok = {ARFILL_OK, 0};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    void *st = *sptr;
    size_t edx;

    if(max_length == 0) return result_skipped;

    (void)constr;

    if(st == NULL) {
        st = CALLOC(1, specs->struct_size);
        if(st == NULL) {
            return result_failed;
        }
    }

    for(edx = 0; edx < td->elements_count; edx++) {
        const asn_TYPE_member_t *elm = &td->elements[edx];
        void *memb_ptr;    /* Pointer to the member */
        void **memb_ptr2;  /* Pointer to that pointer */
        asn_random_fill_result_t tmpres;

        if(elm->optional && asn_random_between(0, 4) == 2) {
            /* Sometimes decide not to fill the optional value */
            continue;
        }

        if(elm->flags & ATF_POINTER) {
            /* Member is a pointer to another structure */
            memb_ptr2 = (void **)((char *)st + elm->memb_offset);
        } else {
            memb_ptr = (char *)st + elm->memb_offset;
            memb_ptr2 = &memb_ptr;
        }

        tmpres = elm->type->op->random_fill(
            elm->type, memb_ptr2, &elm->encoding_constraints,
            max_length > result_ok.length ? max_length - result_ok.length : 0);
        switch(tmpres.code) {
        case ARFILL_OK:
            result_ok.length += tmpres.length;
            continue;
        case ARFILL_SKIPPED:
            assert(!(elm->flags & ATF_POINTER) || *memb_ptr2 == NULL);
            continue;
        case ARFILL_FAILED:
            if(st == *sptr) {
                ASN_STRUCT_RESET(*td, st);
            } else {
                ASN_STRUCT_FREE(*td, st);
            }
            return tmpres;
        }
    }

    *sptr = st;

    return result_ok;
}
