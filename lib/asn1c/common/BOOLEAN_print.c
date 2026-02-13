/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BOOLEAN.h>

int
BOOLEAN_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
              asn_app_consume_bytes_f *cb, void *app_key) {
    const BOOLEAN_t *st = (const BOOLEAN_t *)sptr;
    const char *buf;
    size_t buflen;

    (void)td;  /* Unused argument */
    (void)ilevel;  /* Unused argument */

    if(st) {
        if(*st) {
            buf = "TRUE";
            buflen = 4;
        } else {
            buf = "FALSE";
            buflen = 5;
        }
    } else {
        buf = "<absent>";
        buflen = 8;
    }

    return (cb(buf, buflen, app_key) < 0) ? -1 : 0;
}
