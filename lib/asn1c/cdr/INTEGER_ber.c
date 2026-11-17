/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <INTEGER.h>

/*
 * Encode INTEGER type using DER.
 */
asn_enc_rval_t
INTEGER_encode_der(const asn_TYPE_descriptor_t *td, const void *sptr,
                   int tag_mode, ber_tlv_tag_t tag, asn_app_consume_bytes_f *cb,
                   void *app_key) {
    const INTEGER_t *st = (const INTEGER_t *)sptr;
    asn_enc_rval_t rval;
    INTEGER_t effective_integer;

    ASN_DEBUG("%s %s as INTEGER (tm=%d)",
              cb?"Encoding":"Estimating", td->name, tag_mode);

    /*
     * Canonicalize integer in the buffer.
     * (Remove too long sign extension, remove some first 0x00 bytes)
     */
    if(st->buf) {
        uint8_t *buf = st->buf;
        uint8_t *end1 = buf + st->size - 1;
        int shift;

        /* Compute the number of superfluous leading bytes */
        for(; buf < end1; buf++) {
            /*
             * If the contents octets of an integer value encoding
             * consist of more than one octet, then the bits of the
             * first octet and bit 8 of the second octet:
             * a) shall not all be ones; and
             * b) shall not all be zero.
             */
            switch(*buf) {
            case 0x00: 
	        if((buf[1] & 0x80) == 0) continue;
                break;
            case 0xff: 
            	if((buf[1] & 0x80)) continue;
                break;
            }
            break;
        }

        /* Remove leading superfluous bytes from the integer */
        shift = buf - st->buf;
        if(shift) {
            union {
                const uint8_t *c_buf;
                uint8_t *nc_buf;
            } unconst;
            unconst.c_buf = st->buf;
            effective_integer.buf = unconst.nc_buf + shift;
            effective_integer.size = st->size - shift;

            st = &effective_integer;
        }
    }

    rval = der_encode_primitive(td, st, tag_mode, tag, cb, app_key);
    if(rval.structure_ptr == &effective_integer) {
        rval.structure_ptr = sptr;
    }
    return rval;
}
