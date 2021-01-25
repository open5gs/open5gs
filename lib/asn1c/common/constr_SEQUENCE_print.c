/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE.h>

int
SEQUENCE_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
               asn_app_consume_bytes_f *cb, void *app_key) {
    size_t edx;
    int ret;

    if(!sptr) return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;

    /* Dump preamble */
    if(cb(td->name, strlen(td->name), app_key) < 0
    || cb(" ::= {", 6, app_key) < 0)
        return -1;

    for(edx = 0; edx < td->elements_count; edx++) {
        asn_TYPE_member_t *elm = &td->elements[edx];
        const void *memb_ptr;

        if(elm->flags & ATF_POINTER) {
            memb_ptr = *(const void * const *)((const char *)sptr + elm->memb_offset);
            if(!memb_ptr) {
                if(elm->optional) continue;
                /* Print <absent> line */
                /* Fall through */
            }
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
        }

        /* Indentation */
        _i_INDENT(1);

        /* Print the member's name and stuff */
        if(cb(elm->name, strlen(elm->name), app_key) < 0
        || cb(": ", 2, app_key) < 0)
            return -1;

        /* Print the member itself */
        ret = elm->type->op->print_struct(elm->type, memb_ptr, ilevel + 1,
                                          cb, app_key);
        if(ret) return ret;
    }

    ilevel--;
    _i_INDENT(1);

    return (cb("}", 1, app_key) < 0) ? -1 : 0;
}
