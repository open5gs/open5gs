/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NativeInteger.h>
#include <INTEGER.h>

/*
 * Decode INTEGER type.
 */
asn_dec_rval_t
NativeInteger_decode_ber(const asn_codec_ctx_t *opt_codec_ctx,
                         const asn_TYPE_descriptor_t *td, void **nint_ptr,
                         const void *buf_ptr, size_t size, int tag_mode) {
    const asn_INTEGER_specifics_t *specs =
        (const asn_INTEGER_specifics_t *)td->specifics;
    long *native = (long *)*nint_ptr;
    asn_dec_rval_t rval;
    ber_tlv_len_t length;

    /*
     * If the structure is not there, allocate it.
     */
    if(native == NULL) {
        native = (long *)(*nint_ptr = CALLOC(1, sizeof(*native)));
        if(native == NULL) {
            rval.code = RC_FAIL;
            rval.consumed = 0;
            return rval;
        }
    }

    ASN_DEBUG("Decoding %s as INTEGER (tm=%d)",
              td->name, tag_mode);

    /*
     * Check tags.
     */
    rval = ber_check_tags(opt_codec_ctx, td, 0, buf_ptr, size,
                          tag_mode, 0, &length, 0);
    if(rval.code != RC_OK)
        return rval;

    ASN_DEBUG("%s length is %d bytes", td->name, (int)length);

    /*
     * Make sure we have this length.
     */
    buf_ptr = ((const char *)buf_ptr) + rval.consumed;
    size -= rval.consumed;
    if(length > (ber_tlv_len_t)size) {
        rval.code = RC_WMORE;
        rval.consumed = 0;
        return rval;
    }

    /*
     * ASN.1 encoded INTEGER: buf_ptr, length
     * Fill the native, at the same time checking for overflow.
     * If overflow occurred, return with RC_FAIL.
     */
    {
        INTEGER_t tmp;
        union {
            const void *constbuf;
            void *nonconstbuf;
        } unconst_buf;
        long l;

        unconst_buf.constbuf = buf_ptr;
        tmp.buf = (uint8_t *)unconst_buf.nonconstbuf;
        tmp.size = length;

        if((specs&&specs->field_unsigned)
            ? asn_INTEGER2ulong(&tmp, (unsigned long *)&l) /* sic */
            : asn_INTEGER2long(&tmp, &l)) {
            rval.code = RC_FAIL;
            rval.consumed = 0;
            return rval;
        }

        *native = l;
    }

    rval.code = RC_OK;
    rval.consumed += length;

    ASN_DEBUG("Took %ld/%ld bytes to encode %s (%ld)",
              (long)rval.consumed, (long)length, td->name, (long)*native);

    return rval;
}

/*
 * Encode the NativeInteger using the standard INTEGER type DER encoder.
 */
asn_enc_rval_t
NativeInteger_encode_der(const asn_TYPE_descriptor_t *sd, const void *ptr,
                         int tag_mode, ber_tlv_tag_t tag,
                         asn_app_consume_bytes_f *cb, void *app_key) {
    unsigned long native = *(const unsigned long *)ptr; /* Disable sign ext. */
    asn_enc_rval_t erval = {0,0,0};
    INTEGER_t tmp;

#ifdef WORDS_BIGENDIAN  /* Opportunistic optimization */

    tmp.buf = (uint8_t *)&native;
    tmp.size = sizeof(native);

#else  /* Works even if WORDS_BIGENDIAN is not set where should've been */
    uint8_t buf[sizeof(native)];
    uint8_t *p;

    /* Prepare a fake INTEGER */
    for(p = buf + sizeof(buf) - 1; p >= buf; p--, native >>= 8)
        *p = (uint8_t)native;

    tmp.buf = buf;
    tmp.size = sizeof(buf);
#endif  /* WORDS_BIGENDIAN */

    /* Encode fake INTEGER */
    erval = INTEGER_encode_der(sd, &tmp, tag_mode, tag, cb, app_key);
    if(erval.structure_ptr == &tmp) {
        erval.structure_ptr = ptr;
    }
    return erval;
}
