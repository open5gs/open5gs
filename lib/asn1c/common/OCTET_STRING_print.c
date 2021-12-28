/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OCTET_STRING.h>

int
OCTET_STRING_print(const asn_TYPE_descriptor_t *td, const void *sptr,
                   int ilevel, asn_app_consume_bytes_f *cb, void *app_key) {
    const char * const h2c = "0123456789ABCDEF";
    const OCTET_STRING_t *st = (const OCTET_STRING_t *)sptr;
    char scratch[16 * 3 + 4];
    char *p = scratch;
    uint8_t *buf;
    uint8_t *end;
    size_t i;

    (void)td;  /* Unused argument */

    if(!st || (!st->buf && st->size))
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;

    /*
     * Dump the contents of the buffer in hexadecimal.
     */
    buf = st->buf;
    end = (buf == NULL)? NULL : buf + st->size;
    for(i = 0; buf < end; buf++, i++) {
        if(!(i % 16) && (i || st->size > 16)) {
            if(cb(scratch, p - scratch, app_key) < 0)
                return -1;
            _i_INDENT(1);
            p = scratch;
        }
        *p++ = h2c[(*buf >> 4) & 0x0F];
        *p++ = h2c[*buf & 0x0F];
        *p++ = 0x20;
    }

    if(p > scratch) {
        p--;  /* Remove the tail space */
        if(cb(scratch, p - scratch, app_key) < 0)
            return -1;
    }

    return 0;
}

int
OCTET_STRING_print_utf8(const asn_TYPE_descriptor_t *td, const void *sptr,
                        int ilevel, asn_app_consume_bytes_f *cb,
                        void *app_key) {
    const OCTET_STRING_t *st = (const OCTET_STRING_t *)sptr;

    (void)td;  /* Unused argument */
    (void)ilevel;  /* Unused argument */

    if(st && (st->buf || !st->size)) {
        return (cb(st->buf, st->size, app_key) < 0) ? -1 : 0;
    } else {
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
    }
}
