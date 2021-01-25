/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SET_OF.h>

int
SET_OF_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
             asn_app_consume_bytes_f *cb, void *app_key) {
    asn_TYPE_member_t *elm = td->elements;
    const asn_anonymous_set_ *list = _A_CSET_FROM_VOID(sptr);
    int ret;
    int i;

    if(!sptr) return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;

    /* Dump preamble */
    if(cb(td->name, strlen(td->name), app_key) < 0
    || cb(" ::= {", 6, app_key) < 0)
        return -1;

    for(i = 0; i < list->count; i++) {
        const void *memb_ptr = list->array[i];
        if(!memb_ptr) continue;

        _i_INDENT(1);

        ret = elm->type->op->print_struct(elm->type, memb_ptr,
                                          ilevel + 1, cb, app_key);
        if(ret) return ret;
    }

    ilevel--;
    _i_INDENT(1);

    return (cb("}", 1, app_key) < 0) ? -1 : 0;
}
