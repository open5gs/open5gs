/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_CHOICE.h>

/*
 * Return a standardized complex structure.
 */
#undef RETURN
#define RETURN(_code)                     \
    do {                                  \
        rval.code = _code;                \
        rval.consumed = consumed_myself;  \
        return rval;                      \
    } while(0)

#undef JER_ADVANCE
#define JER_ADVANCE(num_bytes)                                    \
    do {                                                          \
        size_t num = num_bytes;                                   \
        buf_ptr = (const void *)(((const char *)buf_ptr) + num);  \
        size -= num;                                              \
        consumed_myself += num;                                   \
    } while(0)

asn_enc_rval_t
CHOICE_encode_jer(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
                  enum jer_encoder_flags_e flags, asn_app_consume_bytes_f *cb,
                  void *app_key) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    asn_enc_rval_t er = {0,0,0};
    unsigned present = 0;

    if(!sptr)
        ASN__ENCODE_FAILED;

    /*
     * Figure out which CHOICE element is encoded.
     */
    present = _fetch_present_idx(sptr, specs->pres_offset,specs->pres_size);

    if(present == 0 || present > td->elements_count) {
        ASN__ENCODE_FAILED;
    } else {
        asn_enc_rval_t tmper = {0,0,0};
        asn_TYPE_member_t *elm = &td->elements[present-1];
        const void *memb_ptr = NULL;
        const char *mname = elm->name;
        unsigned int mlen = strlen(mname);

        if(elm->flags & ATF_POINTER) {
            memb_ptr =
                *(const void *const *)((const char *)sptr + elm->memb_offset);
            if(!memb_ptr) ASN__ENCODE_FAILED;
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
        }

        er.encoded = 0;

        ASN__CALLBACK3("{\n\"", 3, mname, mlen, "\": ", 2);

        tmper = elm->type->op->jer_encoder(elm->type, memb_ptr,
                                           ilevel + 1, flags, cb, app_key);
        if(tmper.encoded == -1) return tmper;
        er.encoded += tmper.encoded;

        ASN__CALLBACK("}", 1);
        //        ASN__CALLBACK3("</", 2, mname, mlen, ">", 1);
    }

    ASN__ENCODED_OK(er);
cb_failed:
    ASN__ENCODE_FAILED;
}
