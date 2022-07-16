/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <asn_codecs_prim.h>
#include <NULL.h>

asn_enc_rval_t
NULL_encode_jer(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
                enum jer_encoder_flags_e flags, asn_app_consume_bytes_f *cb,
                void *app_key) {
    asn_enc_rval_t er = {0,0,0};

    (void)td;
    (void)sptr;
    (void)ilevel;
    (void)flags;
    (void)cb;
    (void)app_key;

    /* XMLNullValue is empty */
    er.encoded = 0;
    ASN__ENCODED_OK(er);
}
