/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <ENUMERATED.h>
#include <NativeEnumerated.h>

asn_dec_rval_t
ENUMERATED_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                       const asn_TYPE_descriptor_t *td,
                       const asn_per_constraints_t *constraints, void **sptr, asn_per_data_t *pd) {
    asn_dec_rval_t rval;
    ENUMERATED_t *st = (ENUMERATED_t *)*sptr;
    long value;
    void *vptr = &value;

    if(!st) {
        st = (ENUMERATED_t *)(*sptr = CALLOC(1, sizeof(*st)));
        if(!st) ASN__DECODE_FAILED;
    }

    rval = NativeEnumerated_decode_aper(opt_codec_ctx, td, constraints,
                                        (void **)&vptr, pd);
    if(rval.code == RC_OK)
        if(asn_long2INTEGER(st, value))
            rval.code = RC_FAIL;
    return rval;
}

asn_enc_rval_t
ENUMERATED_encode_aper(const asn_TYPE_descriptor_t *td,
                       const asn_per_constraints_t *constraints,
                       const void *sptr, asn_per_outp_t *po) {
    const ENUMERATED_t *st = (const ENUMERATED_t *)sptr;
    long value;

    if(asn_INTEGER2long(st, &value))
        ASN__ENCODE_FAILED;

    return NativeEnumerated_encode_aper(td, constraints, &value, po);
}
