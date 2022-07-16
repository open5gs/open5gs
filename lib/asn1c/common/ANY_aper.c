/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <ANY.h>
#include <errno.h>

#undef RETURN
#define RETURN(_code)                       \
    do {                                    \
        asn_dec_rval_t tmprval;             \
        tmprval.code = _code;               \
        tmprval.consumed = consumed_myself; \
        return tmprval;                     \
    } while(0)

int
ANY_fromType_aper(ANY_t *st, asn_TYPE_descriptor_t *td, void *sptr) {
    uint8_t *buffer = NULL;
    ssize_t erval;

    if(!st || !td) {
        errno = EINVAL;
        return -1;
    }

    if(!sptr) {
        if(st->buf) FREEMEM(st->buf);
        st->size = 0;
        return 0;
    }

    erval = aper_encode_to_new_buffer(td, td->encoding_constraints.per_constraints, sptr, (void**)&buffer);

    if(erval == -1) {
        if(buffer) FREEMEM(buffer);
        return -1;
    }
    assert((size_t)erval > 0);

    if(st->buf) FREEMEM(st->buf);
    st->buf = buffer;
    st->size = erval;

    return 0;
}

ANY_t *
ANY_new_fromType_aper(asn_TYPE_descriptor_t *td, void *sptr) {
    ANY_t tmp;
    ANY_t *st;

    if(!td || !sptr) {
        errno = EINVAL;
        return 0;
    }

    memset(&tmp, 0, sizeof(tmp));

    if(ANY_fromType_aper(&tmp, td, sptr)) return 0;

    st = (ANY_t *)CALLOC(1, sizeof(ANY_t));
    if(st) {
        *st = tmp;
        return st;
    } else {
        FREEMEM(tmp.buf);
        return 0;
    }
}

int
ANY_to_type_aper(ANY_t *st, asn_TYPE_descriptor_t *td, void **struct_ptr) {
    asn_dec_rval_t rval;
    void *newst = 0;

    if(!st || !td || !struct_ptr) {
        errno = EINVAL;
        return -1;
    }

    if(st->buf == 0) {
        /* Nothing to convert, make it empty. */
        *struct_ptr = (void *)0;
        return 0;
    }

    rval = aper_decode(0, td, (void **)&newst, st->buf, st->size, 0, 0);
    if(rval.code == RC_OK) {
        *struct_ptr = newst;
        return 0;
    } else {
        /* Remove possibly partially decoded data. */
        ASN_STRUCT_FREE(*td, newst);
        return -1;
    }
}

asn_dec_rval_t
ANY_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                const asn_TYPE_descriptor_t *td,
                const asn_per_constraints_t *constraints, void **sptr,
                asn_per_data_t *pd) {
    const asn_OCTET_STRING_specifics_t *specs =
        td->specifics ? (const asn_OCTET_STRING_specifics_t *)td->specifics
                      : &asn_SPC_ANY_specs;
    size_t consumed_myself = 0;
    int repeat;
    ANY_t *st = (ANY_t *)*sptr;

    (void)opt_codec_ctx;
    (void)constraints;

    /*
     * Allocate the structure.
     */
    if(!st) {
        st = (ANY_t *)(*sptr = CALLOC(1, specs->struct_size));
        if(!st) RETURN(RC_FAIL);
    }

    ASN_DEBUG("APER Decoding ANY type");

    st->size = 0;
    do {
        ssize_t raw_len;
        ssize_t len_bytes;
        ssize_t len_bits;
        void *p;
        int ret;

        /* Get the PER length */
        raw_len = aper_get_length(pd, -1, -1, 0, &repeat);
        if(raw_len < 0) RETURN(RC_WMORE);
        if(raw_len == 0 && st->buf) break;

        ASN_DEBUG("Got PER length len %" ASN_PRI_SIZE ", %s (%s)", raw_len,
                  repeat ? "repeat" : "once", td->name);
        len_bytes = raw_len;
        len_bits = len_bytes * 8;

        p = REALLOC(st->buf, st->size + len_bytes + 1);
        if(!p) RETURN(RC_FAIL);
        st->buf = (uint8_t *)p;

        ret = per_get_many_bits(pd, &st->buf[st->size], 0, len_bits);
        if(ret < 0) RETURN(RC_WMORE);
        consumed_myself += len_bits;
        st->size += len_bytes;
    } while(repeat);
    st->buf[st->size] = 0; /* nul-terminate */

    RETURN(RC_OK);
}

asn_enc_rval_t
ANY_encode_aper(const asn_TYPE_descriptor_t *td,
                const asn_per_constraints_t *constraints, const void *sptr,
                asn_per_outp_t *po) {
    const ANY_t *st = (const ANY_t *)sptr;
    asn_enc_rval_t er = {0, 0, 0};
    const uint8_t *buf;
    size_t size;
    int ret;

    (void)constraints;

    if(!st || (!st->buf && st->size)) ASN__ENCODE_FAILED;

    buf = st->buf;
    size = st->size;
    do {
        int need_eom = 0;
        ssize_t may_save = aper_put_length(po, -1, -1, size, &need_eom);
        if(may_save < 0) ASN__ENCODE_FAILED;

        ret = per_put_many_bits(po, buf, may_save * 8);
        if(ret) ASN__ENCODE_FAILED;

        buf += may_save;
        size -= may_save;
        assert(!(may_save & 0x07) || !size);
        if(need_eom && aper_put_length(po, -1, -1, 0, NULL))
            ASN__ENCODE_FAILED; /* End of Message length */
    } while(size);

    ASN__ENCODED_OK(er);
}
