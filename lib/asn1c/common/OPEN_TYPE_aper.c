/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <aper_opentype.h>

asn_dec_rval_t
OPEN_TYPE_aper_get(const asn_codec_ctx_t *opt_codec_ctx,
                   const asn_TYPE_descriptor_t *td, void *sptr,
                   const asn_TYPE_member_t *elm, asn_per_data_t *pd) {
    asn_type_selector_result_t selected;
    void *memb_ptr;   /* Pointer to the member */
    void **memb_ptr2; /* Pointer to that pointer */
    void *inner_value;
    asn_dec_rval_t rv;

    if(!(elm->flags & ATF_OPEN_TYPE)) {
        ASN__DECODE_FAILED;
    }

    if(!elm->type_selector) {
        ASN_DEBUG("Type selector is not defined for Open Type %s->%s->%s",
                  td->name, elm->name, elm->type->name);
        ASN__DECODE_FAILED;
    }

    selected = elm->type_selector(td, sptr);
    if(!selected.presence_index) {
        ASN__DECODE_FAILED;
    }

    /* Fetch the pointer to this member */
    assert(elm->flags == ATF_OPEN_TYPE);
    if(elm->flags & ATF_POINTER) {
        memb_ptr2 = (void **)((char *)sptr + elm->memb_offset);
    } else {
        memb_ptr = (char *)sptr + elm->memb_offset;
        memb_ptr2 = &memb_ptr;
    }
    if(*memb_ptr2 != NULL) {
        /* Make sure we reset the structure first before encoding */
        if(CHOICE_variant_set_presence(elm->type, *memb_ptr2, 0)
           != 0) {
            ASN__DECODE_FAILED;
        }
    }

    inner_value =
        (char *)*memb_ptr2
        + elm->type->elements[selected.presence_index - 1].memb_offset;

    rv = aper_open_type_get(opt_codec_ctx, selected.type_descriptor, NULL,
                            &inner_value, pd);
    switch(rv.code) {
    case RC_OK:
        if(CHOICE_variant_set_presence(elm->type, *memb_ptr2,
                                       selected.presence_index)
           == 0) {
            break;
        } else {
            rv.code = RC_FAIL;
        }
        /* Fall through */
    case RC_WMORE:
    case RC_FAIL:
        if(*memb_ptr2) {
            if(elm->flags & ATF_POINTER) {
                ASN_STRUCT_FREE(*selected.type_descriptor, inner_value);
                *memb_ptr2 = NULL;
            } else {
                ASN_STRUCT_RESET(*selected.type_descriptor,
                                              inner_value);
            }
        }
    }
    return rv;
}

asn_enc_rval_t
OPEN_TYPE_encode_aper(const asn_TYPE_descriptor_t *td,
                      const asn_per_constraints_t *constraints,
                      const void *sptr, asn_per_outp_t *po) {
    const void *memb_ptr;   /* Pointer to the member */
    asn_TYPE_member_t *elm; /* CHOICE's element */
    asn_enc_rval_t er = {0,0,0};
    unsigned present;

    (void)constraints;

    present = CHOICE_variant_get_presence(td, sptr);
    if(present == 0 || present > td->elements_count) {
        ASN__ENCODE_FAILED;
    } else {
        present--;
    }

    ASN_DEBUG("Encoding %s OPEN TYPE element %d", td->name, present);

    elm = &td->elements[present];
    if(elm->flags & ATF_POINTER) {
        /* Member is a pointer to another structure */
        memb_ptr =
            *(const void *const *)((const char *)sptr + elm->memb_offset);
        if(!memb_ptr) ASN__ENCODE_FAILED;
    } else {
        memb_ptr = (const char *)sptr + elm->memb_offset;
    }

    if(aper_open_type_put(elm->type, NULL, memb_ptr, po) < 0) {
        ASN__ENCODE_FAILED;
    }

    er.encoded = 0;
    ASN__ENCODED_OK(er);
}
