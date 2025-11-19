/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SET_OF.h>

asn_enc_rval_t
SET_OF_encode_aper(const asn_TYPE_descriptor_t *td,
                   const asn_per_constraints_t *constraints, const void *sptr,
                   asn_per_outp_t *po) {
    const asn_anonymous_set_ *list;
    const asn_per_constraint_t *ct;
    const asn_TYPE_member_t *elm = td->elements;
    struct _el_buffer *encoded_els;
    asn_enc_rval_t er = {0,0,0};
    int seq;

    if(!sptr) ASN__ENCODE_FAILED;

    list = _A_CSET_FROM_VOID(sptr);

    er.encoded = 0;

    ASN_DEBUG("Encoding %s as SET OF (%d)", td->name, list->count);

    if(constraints) ct = &constraints->size;
    else if(td->encoding_constraints.per_constraints)
        ct = &td->encoding_constraints.per_constraints->size;
    else ct = 0;

    /* If extensible constraint, check if size is in root */
    if(ct) {
        int not_in_root =
            (list->count < ct->lower_bound || list->count > ct->upper_bound);
        ASN_DEBUG("lb %lld ub %lld %s",
                  (long long int)ct->lower_bound,
                  (long long int)ct->upper_bound,
                  ct->flags & APC_EXTENSIBLE ? "ext" : "fix");
        if(ct->flags & APC_EXTENSIBLE) {
            /* Declare whether size is in extension root */
            if(per_put_few_bits(po, not_in_root, 1)) ASN__ENCODE_FAILED;
            if(not_in_root) ct = 0;
        } else if(not_in_root && ct->effective_bits >= 0) {
            ASN__ENCODE_FAILED;
        }

    }

    if(ct && ct->effective_bits >= 0) {
        /* X.691, #19.5: No length determinant */
        /*if(per_put_few_bits(po, list->count - ct->lower_bound,
                            ct->effective_bits))
            ASN__ENCODE_FAILED;*/

        if (aper_put_length(po, ct->lower_bound, ct->upper_bound, list->count - ct->lower_bound, 0) < 0) {
            ASN__ENCODE_FAILED;
        }
    }

    /*
     * Canonical PER #22.1 mandates dynamic sorting of the SET OF elements
     * according to their encodings. Build an array of the encoded elements.
     */
    encoded_els = SET_OF__encode_sorted(elm, list, SOES_CAPER);

    for(seq = 0; seq < list->count;) {
        ssize_t may_encode;
        int need_eom = 0;
        if(ct && ct->effective_bits >= 0) {
            may_encode = list->count;
        } else {
            may_encode =
                aper_put_length(po, -1, -1, list->count - seq, &need_eom);
            if(may_encode < 0) ASN__ENCODE_FAILED;
        }

        while(may_encode--) {
            const struct _el_buffer *el = &encoded_els[seq++];
            if(asn_put_many_bits(po, el->buf,
                                 (8 * el->length) - el->bits_unused) < 0) {
                break;
            }
        }
        if(need_eom && (aper_put_length(po, -1, -1, 0, NULL) < 0))
            ASN__ENCODE_FAILED;  /* End of Message length */
    }

    SET_OF__encode_sorted_free(encoded_els, list->count);

    ASN__ENCODED_OK(er);
}

asn_dec_rval_t
SET_OF_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                   const asn_TYPE_descriptor_t *td,
                   const asn_per_constraints_t *constraints, void **sptr, asn_per_data_t *pd) {
    asn_dec_rval_t rv = {RC_OK, 0};
    const asn_SET_OF_specifics_t *specs = (const asn_SET_OF_specifics_t *)td->specifics;
    const asn_TYPE_member_t *elm = td->elements;  /* Single one */
    void *st = *sptr;
    asn_anonymous_set_ *list;
    const asn_per_constraint_t *ct;
    int repeat = 0;
    ssize_t nelems;

    if(ASN__STACK_OVERFLOW_CHECK(opt_codec_ctx))
        ASN__DECODE_FAILED;

    /*
     * Create the target structure if it is not present already.
     */
    if(!st) {
        st = *sptr = CALLOC(1, specs->struct_size);
        if(!st) ASN__DECODE_FAILED;
    }
    list = _A_SET_FROM_VOID(st);

    /* Figure out which constraints to use */
    if(constraints) ct = &constraints->size;
    else if(td->encoding_constraints.per_constraints)
        ct = &td->encoding_constraints.per_constraints->size;
    else ct = 0;

    if(ct && ct->flags & APC_EXTENSIBLE) {
        int value = per_get_few_bits(pd, 1);
        if(value < 0) ASN__DECODE_STARVED;
        if(value) ct = 0;  /* Not restricted! */
    }

    if(ct && ct->upper_bound >= 1 && ct->upper_bound <= 65535
       && ct->upper_bound == ct->lower_bound) {
        /* X.691, #19.5: No length determinant */
        nelems = ct->upper_bound;
        ASN_DEBUG("Preparing to fetch %ld elements from %s",
                  (long)nelems, td->name);
    } else {
        nelems = -1;
    }

    do {
        int i;
        if(nelems < 0) {
            if (ct)
                nelems = aper_get_length(pd, ct->lower_bound, ct->upper_bound,
                                         ct->effective_bits, &repeat);
            else
                nelems = aper_get_length(pd, -1, -1, -1, &repeat);
            ASN_DEBUG("Got to decode %d elements (eff %d)",
                      (int)nelems, (int)(ct ? ct->effective_bits : -1));
            if(nelems < 0) ASN__DECODE_STARVED;
        }

        for(i = 0; i < nelems; i++) {
            void *ptr = 0;
            ASN_DEBUG("SET OF %s decoding", elm->type->name);
            rv = elm->type->op->aper_decoder(opt_codec_ctx, elm->type,
                                             elm->encoding_constraints.per_constraints, &ptr, pd);
            ASN_DEBUG("%s SET OF %s decoded %d, %p",
                      td->name, elm->type->name, rv.code, ptr);
            if(rv.code == RC_OK) {
                if(ASN_SET_ADD(list, ptr) == 0)
                    continue;
                ASN_DEBUG("Failed to add element into %s",
                          td->name);
                /* Fall through */
                rv.code = RC_FAIL;
            } else {
                ASN_DEBUG("Failed decoding %s of %s (SET OF)",
                          elm->type->name, td->name);
            }
            if(ptr) ASN_STRUCT_FREE(*elm->type, ptr);
            return rv;
        }

        nelems = -1;  /* Allow uper_get_length() */
    } while(repeat);

    ASN_DEBUG("Decoded %s as SET OF", td->name);

    rv.code = RC_OK;
    rv.consumed = 0;
    return rv;
}
