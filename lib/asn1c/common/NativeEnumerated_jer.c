/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NativeEnumerated.h>

asn_enc_rval_t
NativeEnumerated_encode_jer(const asn_TYPE_descriptor_t *td, const void *sptr,
                            int ilevel, enum jer_encoder_flags_e flags,
                            asn_app_consume_bytes_f *cb, void *app_key) {
    const asn_INTEGER_specifics_t *specs =
        (const asn_INTEGER_specifics_t *)td->specifics;
    asn_enc_rval_t er = {0,0,0};
    const long *native = (const long *)sptr;
    const asn_INTEGER_enum_map_t *el;

    (void)ilevel;
    (void)flags;

    if(!native) ASN__ENCODE_FAILED;

    el = INTEGER_map_value2enum(specs, *native);
    if(el) {
        er.encoded =
            asn__format_to_callback(cb, app_key, "\"%s\"", el->enum_name);
        if(er.encoded < 0) ASN__ENCODE_FAILED;
        ASN__ENCODED_OK(er);
    } else {
        ASN_DEBUG(
            "ASN.1 forbids dealing with "
            "unknown value of ENUMERATED type");
        ASN__ENCODE_FAILED;
    }
}
