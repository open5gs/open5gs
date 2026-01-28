/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BOOLEAN.h>

asn_dec_rval_t
BOOLEAN_decode_aper(const asn_codec_ctx_t *opt_codec_ctx, const asn_TYPE_descriptor_t *td,
                    const asn_per_constraints_t *constraints, void **sptr, asn_per_data_t *pd) {
    asn_dec_rval_t rv;
    BOOLEAN_t *st = (BOOLEAN_t *)*sptr;

    (void)opt_codec_ctx;
    (void)constraints;
    (void)td;

    if(!st) {
        st = (BOOLEAN_t *)(*sptr = MALLOC(sizeof(*st)));
        if(!st) ASN__DECODE_FAILED;
    }

    /*
     * Extract a single bit
     */
    switch(per_get_few_bits(pd, 1)) {
    case 1:
        *st = 1;
        break;
    case 0:
        *st = 0;
        break;
    case -1:
    default:
        ASN__DECODE_STARVED;
    }

    ASN_DEBUG("%s decoded as %s", td->name, *st ? "TRUE" : "FALSE");

    rv.code = RC_OK;
    rv.consumed = 1;
    return rv;
}

asn_enc_rval_t
BOOLEAN_encode_aper(const asn_TYPE_descriptor_t *td,
                    const asn_per_constraints_t *constraints,
                    const void *sptr, asn_per_outp_t *po) {
    const BOOLEAN_t *st = (const BOOLEAN_t *)sptr;
    asn_enc_rval_t er = { 0, 0, 0 };

    (void)constraints;

    if(!st) ASN__ENCODE_FAILED;

    if(per_put_few_bits(po, *st ? 1 : 0, 1))
        ASN__ENCODE_FAILED;

    ASN__ENCODED_OK(er);
}
