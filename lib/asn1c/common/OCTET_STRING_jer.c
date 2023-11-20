/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OCTET_STRING.h>
#include <BIT_STRING.h>  /* for .bits_unused member */

asn_enc_rval_t
OCTET_STRING_encode_jer(const asn_TYPE_descriptor_t *td, const void *sptr,
                        int ilevel, enum jer_encoder_flags_e flags,
                        asn_app_consume_bytes_f *cb, void *app_key) {
    const char * const h2c = "0123456789ABCDEF";
    const OCTET_STRING_t *st = (const OCTET_STRING_t *)sptr;
    asn_enc_rval_t er = { 0, 0, 0 };
    char scratch[16 * 3 + 4];
    char *p = scratch;
    uint8_t *buf;
    uint8_t *end;
    size_t i;

    if(!st || (!st->buf && st->size))
        ASN__ENCODE_FAILED;

    er.encoded = 0;

    /*
     * Dump the contents of the buffer in hexadecimal.
     */
    buf = st->buf;
    end = buf + st->size;
    ASN__CALLBACK("\"", 1);
    for(i = 0; buf < end; buf++, i++) {
      if(!(i % 16) && (i || st->size > 16)) {
        ASN__CALLBACK(scratch, p-scratch);
        p = scratch;
      }
      *p++ = h2c[(*buf >> 4) & 0x0F];
      *p++ = h2c[*buf & 0x0F];
    }
    if(p - scratch) {
      ASN__CALLBACK(scratch, p-scratch);  /* Dump the rest */
    }
    ASN__CALLBACK("\"", 1);

    ASN__ENCODED_OK(er);
cb_failed:
    ASN__ENCODE_FAILED;
}

static const struct OCTET_STRING__jer_escape_table_s {
    const char *string;
    int size;
} OCTET_STRING__jer_escape_table[] = {
#define	OSXET(s)	{ s, sizeof(s) - 1 }
    OSXET("\074\156\165\154\057\076"),  /* <nul/> */
    OSXET("\074\163\157\150\057\076"),  /* <soh/> */
    OSXET("\074\163\164\170\057\076"),  /* <stx/> */
    OSXET("\074\145\164\170\057\076"),  /* <etx/> */
    OSXET("\074\145\157\164\057\076"),  /* <eot/> */
    OSXET("\074\145\156\161\057\076"),  /* <enq/> */
    OSXET("\074\141\143\153\057\076"),  /* <ack/> */
    OSXET("\074\142\145\154\057\076"),  /* <bel/> */
    OSXET("\074\142\163\057\076"),      /* <bs/> */
    OSXET("\011"),                      /* \t */
    OSXET("\012"),                      /* \n */
    OSXET("\074\166\164\057\076"),      /* <vt/> */
    OSXET("\074\146\146\057\076"),      /* <ff/> */
    OSXET("\015"),                      /* \r */
    OSXET("\074\163\157\057\076"),      /* <so/> */
    OSXET("\074\163\151\057\076"),      /* <si/> */
    OSXET("\074\144\154\145\057\076"),  /* <dle/> */
    OSXET("\074\144\143\061\057\076"),  /* <de1/> */
    OSXET("\074\144\143\062\057\076"),  /* <de2/> */
    OSXET("\074\144\143\063\057\076"),  /* <de3/> */
    OSXET("\074\144\143\064\057\076"),  /* <de4/> */
    OSXET("\074\156\141\153\057\076"),  /* <nak/> */
    OSXET("\074\163\171\156\057\076"),  /* <syn/> */
    OSXET("\074\145\164\142\057\076"),  /* <etb/> */
    OSXET("\074\143\141\156\057\076"),  /* <can/> */
    OSXET("\074\145\155\057\076"),      /* <em/> */
    OSXET("\074\163\165\142\057\076"),  /* <sub/> */
    OSXET("\074\145\163\143\057\076"),  /* <esc/> */
    OSXET("\074\151\163\064\057\076"),  /* <is4/> */
    OSXET("\074\151\163\063\057\076"),  /* <is3/> */
    OSXET("\074\151\163\062\057\076"),  /* <is2/> */
    OSXET("\074\151\163\061\057\076"),  /* <is1/> */
    { 0, 0 },                           /* " " */
    { 0, 0 },                           /* ! */
    { 0, 0 },                           /* \" */
    { 0, 0 },                           /* # */
    { 0, 0 },                           /* $ */
    { 0, 0 },                           /* % */
    OSXET("\046\141\155\160\073"),      /* &amp; */
    { 0, 0 },                           /* ' */
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},  /* ()*+,-./ */
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},  /* 01234567 */
    {0,0},{0,0},{0,0},{0,0},            /* 89:; */
    OSXET("\046\154\164\073"),          /* &lt; */
    { 0, 0 },                           /* = */
    OSXET("\046\147\164\073"),          /* &gt; */
};

asn_enc_rval_t
OCTET_STRING_encode_jer_utf8(const asn_TYPE_descriptor_t *td, const void *sptr,
                             int ilevel, enum jer_encoder_flags_e flags,
                             asn_app_consume_bytes_f *cb, void *app_key) {
    const OCTET_STRING_t *st = (const OCTET_STRING_t *)sptr;
    asn_enc_rval_t er = { 0, 0, 0 };
    uint8_t *buf, *end;
    uint8_t *ss;  /* Sequence start */
    ssize_t encoded_len = 0;

    (void)ilevel;  /* Unused argument */
    (void)flags;  /* Unused argument */

    if(!st || (!st->buf && st->size))
        ASN__ENCODE_FAILED;

    buf = st->buf;
    end = buf + st->size;
    for(ss = buf; buf < end; buf++) {
        unsigned int ch = *buf;
        int s_len;	/* Special encoding sequence length */

        /*
         * Escape certain characters: X.680/11.15
         */
        if(ch < sizeof(OCTET_STRING__jer_escape_table)
            / sizeof(OCTET_STRING__jer_escape_table[0])
        && (s_len = OCTET_STRING__jer_escape_table[ch].size)) {
            if(((buf - ss) && cb(ss, buf - ss, app_key) < 0)
            || cb(OCTET_STRING__jer_escape_table[ch].string, s_len, app_key) < 0)
                ASN__ENCODE_FAILED;
            encoded_len += (buf - ss) + s_len;
            ss = buf + 1;
        }
    }

    encoded_len += (buf - ss);
    if((buf - ss) && cb(ss, buf - ss, app_key) < 0)
        ASN__ENCODE_FAILED;

    er.encoded = encoded_len;
    ASN__ENCODED_OK(er);
}

