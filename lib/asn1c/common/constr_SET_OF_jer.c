/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SET_OF.h>

typedef struct jer_tmp_enc_s {
    void *buffer;
    size_t offset;
    size_t size;
} jer_tmp_enc_t;

static int
SET_OF_encode_jer_callback(const void *buffer, size_t size, void *key) {
    jer_tmp_enc_t *t = (jer_tmp_enc_t *)key;
    if(t->offset + size >= t->size) {
        size_t newsize = (t->size << 2) + size;
        void *p = REALLOC(t->buffer, newsize);
        if(!p) return -1;
        t->buffer = p;
        t->size = newsize;
    }
    memcpy((char *)t->buffer + t->offset, buffer, size);
    t->offset += size;
    return 0;
}

static int
SET_OF_jer_order(const void *aptr, const void *bptr) {
    const jer_tmp_enc_t *a = (const jer_tmp_enc_t *)aptr;
    const jer_tmp_enc_t *b = (const jer_tmp_enc_t *)bptr;
    size_t minlen = a->offset;
    int ret;
    if(b->offset < minlen) minlen = b->offset;
    /* Well-formed UTF-8 has this nice lexicographical property... */
    ret = memcmp(a->buffer, b->buffer, minlen);
    if(ret != 0) return ret;
    if(a->offset == b->offset)
        return 0;
    if(a->offset == minlen)
        return -1;
    return 1;
}

asn_enc_rval_t
SET_OF_encode_jer(const asn_TYPE_descriptor_t *td, const void *sptr, int ilevel,
                  enum jer_encoder_flags_e flags, asn_app_consume_bytes_f *cb,
                  void *app_key) {
    asn_enc_rval_t er = {0,0,0};
    const asn_SET_OF_specifics_t *specs = (const asn_SET_OF_specifics_t *)td->specifics;
    const asn_TYPE_member_t *elm = td->elements;
    const asn_anonymous_set_ *list = _A_CSET_FROM_VOID(sptr);
    const char *mname = specs->as_XMLValueList
        ? 0 : ((*elm->name) ? elm->name : elm->type->xml_tag);
    size_t mlen = mname ? strlen(mname) : 0;
    int xcan = 0;
    jer_tmp_enc_t *encs = 0;
    size_t encs_count = 0;
    void *original_app_key = app_key;
    asn_app_consume_bytes_f *original_cb = cb;
    int i;

    if(!sptr) ASN__ENCODE_FAILED;

    if(xcan) {
        encs = (jer_tmp_enc_t *)MALLOC(list->count * sizeof(encs[0]));
        if(!encs) ASN__ENCODE_FAILED;
        cb = SET_OF_encode_jer_callback;
    }

    er.encoded = 0;

    for(i = 0; i < list->count; i++) {
        asn_enc_rval_t tmper = {0,0,0};

        void *memb_ptr = list->array[i];
        if(!memb_ptr) continue;

        if(encs) {
            memset(&encs[encs_count], 0, sizeof(encs[0]));
            app_key = &encs[encs_count];
            encs_count++;
        }

        if(mname) {
            if(!xcan) ASN__TEXT_INDENT(1, ilevel);
            ASN__CALLBACK3("\"", 1, mname, mlen, "\": ", 3);
        }

        if(!xcan && specs->as_XMLValueList == 1)
            ASN__TEXT_INDENT(1, ilevel + 1);
        tmper = elm->type->op->jer_encoder(elm->type, memb_ptr,
                                           ilevel + (specs->as_XMLValueList != 2),
                                           flags, cb, app_key);
        if(tmper.encoded == -1) return tmper;
        er.encoded += tmper.encoded;
        if(tmper.encoded == 0 && specs->as_XMLValueList) {
            const char *name = elm->type->xml_tag;
            size_t len = strlen(name);
            ASN__CALLBACK3("<", 1, name, len, "/>", 2);
        }

        /* if(mname) { */
        /*     ASN__CALLBACK3("</", 2, mname, mlen, ">", 1); */
        /* } */

    }

    if(!xcan) ASN__TEXT_INDENT(1, ilevel - 1);

    if(encs) {
        jer_tmp_enc_t *enc = encs;
        jer_tmp_enc_t *end = encs + encs_count;
        ssize_t control_size = 0;

        er.encoded = 0;
        cb = original_cb;
        app_key = original_app_key;
        qsort(encs, encs_count, sizeof(encs[0]), SET_OF_jer_order);

        for(; enc < end; enc++) {
            ASN__CALLBACK(enc->buffer, enc->offset);
            FREEMEM(enc->buffer);
            enc->buffer = 0;
            control_size += enc->offset;
        }
        assert(control_size == er.encoded);
    }

    goto cleanup;
cb_failed:
    ASN__ENCODE_FAILED;
cleanup:
    if(encs) {
        size_t n;
        for(n = 0; n < encs_count; n++) {
            FREEMEM(encs[n].buffer);
        }
        FREEMEM(encs);
    }
    ASN__ENCODED_OK(er);
}
