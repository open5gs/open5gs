/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NativeInteger.h>

asn_enc_rval_t
NativeInteger_encode_jer(const asn_TYPE_descriptor_t *td, const void *sptr,
                         int ilevel, enum jer_encoder_flags_e flags,
                         asn_app_consume_bytes_f *cb, void *app_key) {
    const asn_INTEGER_specifics_t *specs =
        (const asn_INTEGER_specifics_t *)td->specifics;
    char scratch[32];  /* Enough for 64-bit int */
    asn_enc_rval_t er = {0,0,0};
    const long *native = (const long *)sptr;

    (void)ilevel;
    (void)flags;

    if(!native) ASN__ENCODE_FAILED;

    er.encoded = snprintf(scratch, sizeof(scratch),
                          (specs && specs->field_unsigned)
                              ? "%lu" : "%ld", *native);
    if(er.encoded <= 0 || (size_t)er.encoded >= sizeof(scratch)
        || cb(scratch, er.encoded, app_key) < 0)
        ASN__ENCODE_FAILED;

    ASN__ENCODED_OK(er);
}
