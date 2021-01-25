/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NULL.h>

int
NULL_print(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
           asn_app_consume_bytes_f *cb, void *app_key) {
    (void)td;  /* Unused argument */
    (void)ilevel;  /* Unused argument */

    if(sptr) {
        return (cb("<present>", 9, app_key) < 0) ? -1 : 0;
    } else {
        return (cb("<absent>", 8, app_key) < 0) ? -1 : 0;
    }
}
