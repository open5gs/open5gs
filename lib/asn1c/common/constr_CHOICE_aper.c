/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_CHOICE.h>
#include <aper_opentype.h>

asn_dec_rval_t
CHOICE_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                   const asn_TYPE_descriptor_t *td,
                   const asn_per_constraints_t *constraints, void **sptr, asn_per_data_t *pd) {
    const asn_CHOICE_specifics_t *specs = (const asn_CHOICE_specifics_t *)td->specifics;
    asn_dec_rval_t rv;
    const asn_per_constraint_t *ct = NULL;
    asn_TYPE_member_t *elm = NULL;  /* CHOICE's element */
    void *memb_ptr = NULL;
    void **memb_ptr2 = NULL;
    void *st = *sptr;
    int value = 0;

    if(ASN__STACK_OVERFLOW_CHECK(opt_codec_ctx))
        ASN__DECODE_FAILED;

    /*
     * Create the target structure if it is not present already.
     */
    if(!st) {
        st = *sptr = CALLOC(1, specs->struct_size);
        if(!st) ASN__DECODE_FAILED;
    }

    if(constraints) ct = &constraints->value;
    else if(td->encoding_constraints.per_constraints)
        ct = &td->encoding_constraints.per_constraints->value;
    else ct = 0;

    if(ct && ct->flags & APC_EXTENSIBLE) {
        value = per_get_few_bits(pd, 1);
        if(value < 0) ASN__DECODE_STARVED;
        if(value) ct = 0;  /* Not restricted */
        if((unsigned)value >= td->elements_count)
            ASN__DECODE_FAILED;
    }

    if(ct && ct->range_bits >= 0) {
        value = per_get_few_bits(pd, ct->range_bits);
        if(value < 0) ASN__DECODE_STARVED;
        ASN_DEBUG("CHOICE %s got index %d in range %d",
                  td->name, value, ct->range_bits);
        if(value > ct->upper_bound)
            ASN__DECODE_FAILED;
    } else {
        if(specs->ext_start == -1)
            ASN__DECODE_FAILED;

        if(specs && specs->tag2el_count > (unsigned)specs->ext_start) {
            value = aper_get_nsnnwn(pd); /* extension elements range */
            if(value < 0) ASN__DECODE_STARVED;
            value += specs->ext_start;
            if((unsigned)value >= td->elements_count)
                ASN__DECODE_FAILED;
        }
    }

    /* Adjust if canonical order is different from natural order */
    if(specs->from_canonical_order)
        value = specs->from_canonical_order[value];

    /* Set presence to be able to free it later */
    _set_present_idx(st, specs->pres_offset, specs->pres_size, value + 1);

    elm = &td->elements[value];
    if(elm->flags & ATF_POINTER) {
        /* Member is a pointer to another structure */
        memb_ptr2 = (void **)((char *)st + elm->memb_offset);
    } else {
        memb_ptr = (char *)st + elm->memb_offset;
        memb_ptr2 = &memb_ptr;
    }
    ASN_DEBUG("Discovered CHOICE %s encodes %s", td->name, elm->name);

    if(ct && ct->range_bits >= 0) {
        rv = elm->type->op->aper_decoder(opt_codec_ctx, elm->type,
                                         elm->encoding_constraints.per_constraints, memb_ptr2, pd);
    } else {
        rv = aper_open_type_get(opt_codec_ctx, elm->type,
                                elm->encoding_constraints.per_constraints, memb_ptr2, pd);
    }

    if(rv.code != RC_OK)
        ASN_DEBUG("Failed to decode %s in %s (CHOICE) %d",
                  elm->name, td->name, rv.code);
    return rv;
}

asn_enc_rval_t
CHOICE_encode_aper(const asn_TYPE_descriptor_t *td,
                   const asn_per_constraints_t *constraints,
                   const void *sptr, asn_per_outp_t *po) {
    const asn_CHOICE_specifics_t *specs = (const asn_CHOICE_specifics_t *)td->specifics;
    const asn_TYPE_member_t *elm;  /* CHOICE's element */
    const asn_per_constraint_t *ct;
    const void *memb_ptr;
    int present;

    if(!sptr) ASN__ENCODE_FAILED;

    ASN_DEBUG("Encoding %s as CHOICE using ALIGNED PER", td->name);

    if(constraints) ct = &constraints->value;
    else if(td->encoding_constraints.per_constraints)
        ct = &td->encoding_constraints.per_constraints->value;
    else ct = 0;

    present = _fetch_present_idx(sptr,
                                 specs->pres_offset, specs->pres_size);

    /*
     * If the structure was not initialized properly, it cannot be encoded:
     * can't deduce what to encode in the choice type.
     */
    if(present <= 0 || (unsigned)present > td->elements_count)
        ASN__ENCODE_FAILED;
    else
        present--;

    /* Adjust if canonical order is different from natural order */
    if(specs->to_canonical_order)
        present = specs->to_canonical_order[present];

    ASN_DEBUG("Encoding %s CHOICE element %d", td->name, present);

    if(ct && ct->range_bits >= 0) {
        if(present < ct->lower_bound
                || present > ct->upper_bound) {
            if(ct->flags & APC_EXTENSIBLE) {
                if(per_put_few_bits(po, 1, 1))
                    ASN__ENCODE_FAILED;
            } else {
                ASN__ENCODE_FAILED;
            }
            ct = 0;
        }
    }
    if(ct && ct->flags & APC_EXTENSIBLE) {
        if(per_put_few_bits(po, 0, 1))
            ASN__ENCODE_FAILED;
    }

    elm = &td->elements[present];
    if(elm->flags & ATF_POINTER) {
        /* Member is a pointer to another structure */
        memb_ptr = *(const void *const *)((const char *)sptr + elm->memb_offset);
        if(!memb_ptr) ASN__ENCODE_FAILED;
    } else {
        memb_ptr = (const char *)sptr + elm->memb_offset;
    }

    if(ct && ct->range_bits >= 0) {
        if(per_put_few_bits(po, present, ct->range_bits))
            ASN__ENCODE_FAILED;

        return elm->type->op->aper_encoder(elm->type, elm->encoding_constraints.per_constraints,
                                           memb_ptr, po);
    } else {
        asn_enc_rval_t rval = {0,0,0};
        if(specs->ext_start == -1)
            ASN__ENCODE_FAILED;
        if(aper_put_nsnnwn(po, present - specs->ext_start))
            ASN__ENCODE_FAILED;
        if(aper_open_type_put(elm->type, elm->encoding_constraints.per_constraints,
                              memb_ptr, po))
            ASN__ENCODE_FAILED;
        rval.encoded = 0;
        ASN__ENCODED_OK(rval);
    }
}
