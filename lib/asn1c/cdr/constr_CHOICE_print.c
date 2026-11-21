/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_CHOICE.h>

int
CHOICE_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
             asn_app_consume_bytes_f *cb, void *app_key) {
    const asn_CHOICE_specifics_t *specs = (const asn_CHOICE_specifics_t *)td->specifics;
    unsigned present;

    if(!sptr) return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;

    /*
     * Figure out which CHOICE element is encoded.
     */
    present = _fetch_present_idx(sptr, specs->pres_offset,specs->pres_size);

    /*
     * Print that element.
     */
    if(present > 0 && present <= td->elements_count) {
        asn_TYPE_member_t *elm = &td->elements[present-1];
        const void *memb_ptr;

        if(elm->flags & ATF_POINTER) {
            memb_ptr = *(const void * const *)((const char *)sptr + elm->memb_offset);
            if(!memb_ptr) return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
        }

        /* Print member's name and stuff */
        if(0) {
            if(cb(elm->name, strlen(elm->name), app_key) < 0
            || cb(": ", 2, app_key) < 0)
                return -1;
        }

        return elm->type->op->print_struct(elm->type, memb_ptr, ilevel,
                                           cb, app_key);
    } else {
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
    }
}
