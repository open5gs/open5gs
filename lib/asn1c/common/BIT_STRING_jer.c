/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BIT_STRING.h>

static const char *_bit_pattern[16] = {
    "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

asn_enc_rval_t
BIT_STRING_encode_jer(const asn_TYPE_descriptor_t *td, const void *sptr,
                      int ilevel, enum jer_encoder_flags_e flags,
                      asn_app_consume_bytes_f *cb, void *app_key) {
    asn_enc_rval_t er = {0, 0, 0};
    char scratch[128];
    char *p = scratch;
    char *scend = scratch + (sizeof(scratch) - 10);
    const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
    int xcan = 0;
    uint8_t *buf;
    uint8_t *end;

    if(!st || !st->buf)
        ASN__ENCODE_FAILED;

    er.encoded = 0;

    buf = st->buf;
    end = buf + st->size - 1;  /* Last byte is special */

    /*
     * Binary dump
     */
    for(; buf < end; buf++) {
        int v = *buf;
        int nline = xcan?0:(((buf - st->buf) % 8) == 0);
        if(p >= scend || nline) {
            ASN__CALLBACK(scratch, p - scratch);
            p = scratch;
            if(nline) ASN__TEXT_INDENT(1, ilevel);
        }
        memcpy(p + 0, _bit_pattern[v >> 4], 4);
        memcpy(p + 4, _bit_pattern[v & 0x0f], 4);
        p += 8;
    }

    if(!xcan && ((buf - st->buf) % 8) == 0)
        ASN__TEXT_INDENT(1, ilevel);
    ASN__CALLBACK(scratch, p - scratch);
    p = scratch;

    if(buf == end) {
        int v = *buf;
        int ubits = st->bits_unused;
        int i;
        for(i = 7; i >= ubits; i--)
            *p++ = (v & (1 << i)) ? 0x31 : 0x30;
        ASN__CALLBACK(scratch, p - scratch);
    }

    if(!xcan) ASN__TEXT_INDENT(1, ilevel - 1);

    ASN__ENCODED_OK(er);
cb_failed:
    ASN__ENCODE_FAILED;
}
