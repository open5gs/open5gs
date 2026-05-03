/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>

int
OPEN_TYPE_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
                asn_app_consume_bytes_f *cb, void *app_key) {
    
    if(!sptr) {
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
    }

    /* Check if this is direct type mode (no CHOICE wrapper) */
    if(td->elements_count == 0) {
        /* Direct type mode: cannot determine actual type without element context */
        /* This function should not be called directly; SEQUENCE should use */
        /* OPEN_TYPE_print_member instead */
        return (cb("<unknown>", 9, app_key) < 0) ? -1 : 0;
    }

    /* CHOICE wrapper mode: use CHOICE printer */
    return CHOICE_print(td, sptr, ilevel, cb, app_key);
}

int
OPEN_TYPE_print_member(const asn_TYPE_descriptor_t *td, const void *sptr,
                       const asn_TYPE_member_t *elm, int ilevel,
                       asn_app_consume_bytes_f *cb, void *app_key) {
    asn_type_selector_result_t selected;
    const void *memb_ptr;

    if(!sptr || !(elm->flags & ATF_OPEN_TYPE)) {
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
    }

    /* Validate elm->type before accessing its members */
    if(!elm->type) {
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
    }

    /* Get the member pointer */
    if(elm->flags & ATF_POINTER) {
        memb_ptr = *(const void * const *)((const char *)sptr + elm->memb_offset);
        if(!memb_ptr) {
            return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
        }
    } else {
        memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
    }

    /* Check if this is direct type mode (no CHOICE wrapper) */
    if(elm->type->elements_count == 0) {
        /* Direct type mode: use type selector to find the actual type */
        if(!elm->type_selector) {
            return (cb("<unknown>", 9, app_key) < 0) ? -1 : 0;
        }

        selected = elm->type_selector(td, sptr);
        if(!selected.presence_index || !selected.type_descriptor) {
            return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
        }

        /* Print using the actual type descriptor */
        return selected.type_descriptor->op->print_struct(
            selected.type_descriptor, memb_ptr, ilevel, cb, app_key);
    }

    /* CHOICE wrapper mode: use OPEN_TYPE printer (which calls CHOICE printer) */
    return OPEN_TYPE_print(elm->type, memb_ptr, ilevel, cb, app_key);
}
