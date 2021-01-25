/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OBJECT_IDENTIFIER.h>

int
OBJECT_IDENTIFIER_print(const asn_TYPE_descriptor_t *td, const void *sptr,
                        int ilevel, asn_app_consume_bytes_f *cb,
                        void *app_key) {
    const OBJECT_IDENTIFIER_t *st = (const OBJECT_IDENTIFIER_t *)sptr;

    (void)td;  /* Unused argument */
    (void)ilevel;  /* Unused argument */

    if(!st || !st->buf)
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;

    /* Dump preamble */
    if(cb("{ ", 2, app_key) < 0)
        return -1;

    if(OBJECT_IDENTIFIER__dump_body(st, cb, app_key) < 0) {
        return -1;
    }

    return (cb(" }", 2, app_key) < 0) ? -1 : 0;
}
