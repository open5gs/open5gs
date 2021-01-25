/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_CHOICE.h>

asn_random_fill_result_t
CHOICE_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                   const asn_encoding_constraints_t *constr,
                   size_t max_length) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    asn_random_fill_result_t res;
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    const asn_TYPE_member_t *elm;
    unsigned present;
    void *memb_ptr;    /* Pointer to the member */
    void **memb_ptr2;  /* Pointer to that pointer */
    void *st = *sptr;

    if(max_length == 0) return result_skipped;

    (void)constr;

    if(st == NULL) {
        st = CALLOC(1, specs->struct_size);
        if(st == NULL) {
            return result_failed;
        }
    }

    present = asn_random_between(1, td->elements_count);
    elm = &td->elements[present - 1];

    if(elm->flags & ATF_POINTER) {
        /* Member is a pointer to another structure */
        memb_ptr2 = (void **)((char *)st + elm->memb_offset);
    } else {
        memb_ptr = (char *)st + elm->memb_offset;
        memb_ptr2 = &memb_ptr;
    }

    res = elm->type->op->random_fill(elm->type, memb_ptr2,
                                    &elm->encoding_constraints, max_length);
    _set_present_idx(st, specs->pres_offset, specs->pres_size, present);
    if(res.code == ARFILL_OK) {
        *sptr = st;
    } else {
        if(st == *sptr) {
            ASN_STRUCT_RESET(*td, st);
        } else {
            ASN_STRUCT_FREE(*td, st);
        }
    }

    return res;
}
