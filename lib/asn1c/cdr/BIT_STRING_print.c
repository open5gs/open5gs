/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BIT_STRING.h>

/*
 * BIT STRING specific contents printer.
 */
int
BIT_STRING_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
                 asn_app_consume_bytes_f *cb, void *app_key) {
    const char * const h2c = "0123456789ABCDEF";
    char scratch[64];
    const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
    uint8_t *buf;
    uint8_t *end;
    char *p = scratch;

    (void)td;  /* Unused argument */

    if(!st || !st->buf)
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;

    ilevel++;
    buf = st->buf;
    end = buf + st->size;

    /*
     * Hexadecimal dump.
     */
    for(; buf < end; buf++) {
        if((buf - st->buf) % 16 == 0 && (st->size > 16)
                && buf != st->buf) {
            _i_INDENT(1);
            /* Dump the string */
            if(cb(scratch, p - scratch, app_key) < 0) return -1;
            p = scratch;
        }
        *p++ = h2c[*buf >> 4];
        *p++ = h2c[*buf & 0x0F];
        *p++ = 0x20;
    }

    if(p > scratch) {
        p--;  /* Eat the tailing space */

        if((st->size > 16)) {
            _i_INDENT(1);
        }

        /* Dump the incomplete 16-bytes row */
        if(cb(scratch, p - scratch, app_key) < 0)
            return -1;
    }

    if(st->bits_unused) {
        int ret = snprintf(scratch, sizeof(scratch), " (%d bit%s unused)",
                           st->bits_unused, st->bits_unused == 1 ? "" : "s");
        assert(ret > 0 && ret < (ssize_t)sizeof(scratch));
        if(ret > 0 && ret < (ssize_t)sizeof(scratch)
           && cb(scratch, ret, app_key) < 0)
            return -1;
    }

    return 0;
}
