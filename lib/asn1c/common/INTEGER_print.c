/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <INTEGER.h>

/*
 * INTEGER specific human-readable output.
 */
int
INTEGER_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
              asn_app_consume_bytes_f *cb, void *app_key) {
    const INTEGER_t *st = (const INTEGER_t *)sptr;
    ssize_t ret;

    (void)ilevel;

    if(!st || !st->buf)
        ret = cb("<absent>", 8, app_key);
    else
        ret = INTEGER__dump(td, st, cb, app_key, 0);

    return (ret < 0) ? -1 : 0;
}
