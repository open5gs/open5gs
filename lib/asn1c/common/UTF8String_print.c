/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <UTF8String.h>

int
UTF8String_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
                 asn_app_consume_bytes_f *cb, void *app_key) {
    const UTF8String_t *st = (const UTF8String_t *)sptr;

    (void)td;  /* Unused argument */
    (void)ilevel;  /* Unused argument */

    if(st && st->buf) {
        return (cb(st->buf, st->size, app_key) < 0) ? -1 : 0;
    } else {
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
    }
}
