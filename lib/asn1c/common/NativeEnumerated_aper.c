/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NativeEnumerated.h>

asn_dec_rval_t
NativeEnumerated_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                             const asn_TYPE_descriptor_t *td,
                             const asn_per_constraints_t *constraints,
                             void **sptr, asn_per_data_t *pd) {
    const asn_INTEGER_specifics_t *specs = (const asn_INTEGER_specifics_t *)td->specifics;
    asn_dec_rval_t rval = { RC_OK, 0 };
    long *native = (long *)*sptr;
    const asn_per_constraint_t *ct;
    long value;

    (void)opt_codec_ctx;

    if(constraints) ct = &constraints->value;
    else if(td->encoding_constraints.per_constraints)
        ct = &td->encoding_constraints.per_constraints->value;
    else ASN__DECODE_FAILED;  /* Mandatory! */
    if(!specs) ASN__DECODE_FAILED;

    if(!native) {
        native = (long *)(*sptr = CALLOC(1, sizeof(*native)));
        if(!native) ASN__DECODE_FAILED;
    }

    ASN_DEBUG("Decoding %s as NativeEnumerated", td->name);

    if(ct->flags & APC_EXTENSIBLE) {
        int inext = per_get_few_bits(pd, 1);
        if(inext < 0) ASN__DECODE_STARVED;
        if(inext) ct = 0;
    }

    /* Deal with APER padding */
    if(ct && ct->upper_bound >= 255) {
        int padding = 0;
        padding = (8 - (pd->moved % 8)) % 8;
        ASN_DEBUG("For NativeEnumerated %s,offset = %zu Padding bits = %d",
                  td->name, pd->moved, padding);
        ASN_DEBUG("For NativeEnumerated %s, upper bound = %llu",
                  td->name, (unsigned long long)ct->upper_bound);
        if(padding > 0)
            per_get_few_bits(pd, padding);
    }

    if(ct && ct->range_bits >= 0) {
        value = per_get_few_bits(pd, ct->range_bits);
        if(value < 0) ASN__DECODE_STARVED;
        if(value >= (specs->extension
             ? specs->extension - 1 : specs->map_count))
        ASN__DECODE_FAILED;
    } else {
        if(!specs->extension)
            ASN__DECODE_FAILED;
        /*
         * X.691, #10.6: normally small non-negative whole number;
         */

        /* XXX handle indefinite index length > 64k */
        value = aper_get_nsnnwn(pd);
        if(value < 0) ASN__DECODE_STARVED;
        value += specs->extension - 1;
        //if(value >= specs->map_count)
        //    ASN__DECODE_FAILED;
        if(value >= specs->map_count) {
            ASN_DEBUG("Decoded unknown index value %s = %ld", td->name, value);
            /* unknown index. Workaround => set the first enumeration value */
            *native = specs->value2enum[0].nat_value;
            return rval;
        }
    }

    *native = specs->value2enum[value].nat_value;
    ASN_DEBUG("Decoded %s = %ld", td->name, *native);

    return rval;
}

asn_enc_rval_t
NativeEnumerated_encode_aper(const asn_TYPE_descriptor_t *td,
                             const asn_per_constraints_t *constraints,
                             const void *sptr, asn_per_outp_t *po) {
    const asn_INTEGER_specifics_t *specs = (const asn_INTEGER_specifics_t *)td->specifics;
    asn_enc_rval_t er = {0,0,0};
    long native, value;
    const asn_per_constraint_t *ct;
    int inext = 0, range_bits = 1;
    asn_INTEGER_enum_map_t key;
    asn_INTEGER_enum_map_t *kf;

    if(!sptr) ASN__ENCODE_FAILED;
    if(!specs) ASN__ENCODE_FAILED;

    if(constraints) ct = &constraints->value;
    else if(td->encoding_constraints.per_constraints)
        ct = &td->encoding_constraints.per_constraints->value;
    else ASN__ENCODE_FAILED;  /* Mandatory! */

    ASN_DEBUG("Encoding %s as NativeEnumerated", td->name);

    er.encoded = 0;

    native = *(const long *)sptr;
    if(native < 0) ASN__ENCODE_FAILED;

    key.nat_value = native;
    kf = bsearch(&key, specs->value2enum, specs->map_count,
                 sizeof(key), NativeEnumerated__compar_value2enum);
    if(!kf) {
        ASN_DEBUG("No element corresponds to %ld", native);
        ASN__ENCODE_FAILED;
    }
    value = kf - specs->value2enum;

    if(ct->range_bits >= 0) {
        int cmpWith = specs->extension
              ? specs->extension - 1 : specs->map_count;
        if(value >= cmpWith)
            inext = 1;
    }
    if(ct->flags & APC_EXTENSIBLE) {
        if(per_put_few_bits(po, inext, 1))
            ASN__ENCODE_FAILED;
        if(inext) range_bits = 0;
    } else if(inext) {
        ASN__ENCODE_FAILED;
    }

    if(range_bits && ct && ct->range_bits >= 0) {
        if(per_put_few_bits(po, value, ct->range_bits))
            ASN__ENCODE_FAILED;
        ASN__ENCODED_OK(er);
    }

    if(!specs->extension)
        ASN__ENCODE_FAILED;

    /*
     * X.691, #10.6: normally small non-negative whole number;
     */
    ASN_DEBUG("value = %ld, ext = %d, inext = %d, res = %ld",
              value, specs->extension, inext,
              value - (inext ? (specs->extension - 1) : 0));
    if(aper_put_nsnnwn(po, value - (inext ? (specs->extension - 1) : 0)))
        ASN__ENCODE_FAILED;

    ASN__ENCODED_OK(er);
}
