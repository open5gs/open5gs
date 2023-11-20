/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BIT_STRING.h>

asn_enc_rval_t
BIT_STRING_encode_jer(const asn_TYPE_descriptor_t *td, const void *sptr,
                      int ilevel, enum jer_encoder_flags_e flags,
                      asn_app_consume_bytes_f *cb, void *app_key) {
    asn_enc_rval_t er = {0, 0, 0};
    const char * const h2c = "0123456789ABCDEF";
    char scratch[16 * 3 + 4];
    char *p = scratch;
    const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
    uint8_t *buf;
    uint8_t *end;

    if(!st || !st->buf)
        ASN__ENCODE_FAILED;

    er.encoded = 0;

    buf = st->buf;
    end = buf + st->size - 1;  /* Last byte is special */

    /*
     * Hex dump
     */
    *p++ = '"';
    for(int i = 0; buf < end; buf++, i++) {
        if(!(i % 16) && (i || st->size > 16)) {
            ASN__CALLBACK(scratch, p-scratch);
            p = scratch;
        }
        *p++ = h2c[*buf >> 4];
        *p++ = h2c[*buf & 0x0F];
    }

    ASN__CALLBACK(scratch, p - scratch);
    p = scratch;

    if(buf == end) {
        int ubits = st->bits_unused;
        uint8_t v = *buf & (0xff << ubits);
        *p++ = h2c[v >> 4];
        *p++ = h2c[v & 0x0F];
        ASN__CALLBACK(scratch, p - scratch);
        p = scratch;
    }
    *p++ = '"';
    ASN__CALLBACK(scratch, p - scratch);
    ASN__TEXT_INDENT(1, ilevel - 1);

    ASN__ENCODED_OK(er);
cb_failed:
    ASN__ENCODE_FAILED;
}
