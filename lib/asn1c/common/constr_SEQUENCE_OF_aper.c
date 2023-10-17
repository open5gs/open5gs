/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE_OF.h>
#include <asn_SEQUENCE_OF.h>

asn_enc_rval_t
SEQUENCE_OF_encode_aper(const asn_TYPE_descriptor_t *td,
                        const asn_per_constraints_t *constraints,
                        const void *sptr, asn_per_outp_t *po) {
    const asn_anonymous_sequence_ *list;
    const asn_per_constraint_t *ct;
    asn_enc_rval_t er = {0,0,0};
    asn_TYPE_member_t *elm = td->elements;
    int seq;

    if(!sptr) ASN__ENCODE_FAILED;
    list = _A_CSEQUENCE_FROM_VOID(sptr);

    er.encoded = 0;

    ASN_DEBUG("Encoding %s as SEQUENCE OF size (%d) using ALIGNED PER", td->name, list->count);

    if(constraints) ct = &constraints->size;
    else if(td->encoding_constraints.per_constraints)
        ct = &td->encoding_constraints.per_constraints->size;
    else ct = 0;

    /* If extensible constraint, check if size is in root */
    if(ct) {
        int not_in_root = (list->count < ct->lower_bound
                || list->count > ct->upper_bound);
        ASN_DEBUG("lb %lld ub %lld %s",
                  (long long int)ct->lower_bound,
                  (long long int)ct->upper_bound,
                  ct->flags & APC_EXTENSIBLE ? "ext" : "fix");
        if(ct->flags & APC_EXTENSIBLE) {
            /* Declare whether size is in extension root */
            if(per_put_few_bits(po, not_in_root, 1))
                ASN__ENCODE_FAILED;
            if(not_in_root) ct = 0;
        } else if(not_in_root && ct->effective_bits >= 0)
            ASN__ENCODE_FAILED;
    }

    if(ct && ct->effective_bits >= 0) {
        /* X.691, #19.5: No length determinant */
/*
        if(per_put_few_bits(po, list->count - ct->lower_bound,
                            ct->effective_bits))
            ASN__ENCODE_FAILED;
*/
        if (ct->lower_bound == ct->upper_bound && ct->upper_bound < 65536) {
            /* No length determinant */
    } else if (aper_put_length(po, ct->lower_bound, ct->upper_bound, list->count - ct->lower_bound, 0) < 0)
            ASN__ENCODE_FAILED;
    }

    for(seq = -1; seq < list->count;) {
        ssize_t mayEncode;
        int need_eom = 0;
        if(seq < 0) seq = 0;
        if(ct && ct->effective_bits >= 0) {
            mayEncode = list->count;
        } else {
            mayEncode = aper_put_length(po, -1, -1, list->count - seq, &need_eom);
            if(mayEncode < 0) ASN__ENCODE_FAILED;
        }

        while(mayEncode--) {
            void *memb_ptr = list->array[seq++];
            if(!memb_ptr) ASN__ENCODE_FAILED;
            er = elm->type->op->aper_encoder(elm->type,
                                             elm->encoding_constraints.per_constraints,
                                             memb_ptr, po);
            if(er.encoded == -1)
                ASN__ENCODE_FAILED;
        }

        if(need_eom && (aper_put_length(po, -1, -1, 0, NULL) < 0))
            ASN__ENCODE_FAILED;  /* End of Message length */
    }

    ASN__ENCODED_OK(er);
}
