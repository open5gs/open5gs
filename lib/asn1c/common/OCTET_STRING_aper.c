/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OCTET_STRING.h>
#include <BIT_STRING.h>  /* for .bits_unused member */

#undef RETURN
#define RETURN(_code) do {\
        asn_dec_rval_t tmprval;\
        tmprval.code = _code;\
        tmprval.consumed = consumed_myself;\
        return tmprval;\
    } while(0)

static asn_per_constraints_t asn_DEF_OCTET_STRING_constraints = {
    { APC_CONSTRAINED, 8, 8, 0, 255 },
    { APC_SEMI_CONSTRAINED, -1, -1, 0, 0 },
    0, 0
};

asn_dec_rval_t
OCTET_STRING_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                         const asn_TYPE_descriptor_t *td,
                         const asn_per_constraints_t *constraints,
                         void **sptr, asn_per_data_t *pd) {

    const asn_OCTET_STRING_specifics_t *specs = td->specifics
        ? (const asn_OCTET_STRING_specifics_t *)td->specifics
        : &asn_SPC_OCTET_STRING_specs;
    const asn_per_constraints_t *pc = constraints
        ? constraints
        : td->encoding_constraints.per_constraints;
    const asn_per_constraint_t *cval;
    const asn_per_constraint_t *csiz;
    asn_dec_rval_t rval = { RC_OK, 0 };
    BIT_STRING_t *st = (BIT_STRING_t *)*sptr;
    ssize_t consumed_myself = 0;
    int repeat;
    enum {
        OS__BPC_BIT = 0,
        OS__BPC_CHAR = 1,
        OS__BPC_U16 = 2,
        OS__BPC_U32 = 4
    } bpc;  /* Bytes per character */
    unsigned int unit_bits;
    unsigned int canonical_unit_bits;

    (void)opt_codec_ctx;

    if(pc) {
        cval = &pc->value;
        csiz = &pc->size;
    } else {
        cval = &asn_DEF_OCTET_STRING_constraints.value;
        csiz = &asn_DEF_OCTET_STRING_constraints.size;
    }

    switch(specs->subvariant) {
    default:
/*
    case ASN_OSUBV_ANY:
        ASN_DEBUG("Unrecognized subvariant %d", specs->subvariant);
        RETURN(RC_FAIL);
*/
    case ASN_OSUBV_BIT:
        canonical_unit_bits = unit_bits = 1;
        bpc = OS__BPC_BIT;
        break;
    case ASN_OSUBV_ANY:
    case ASN_OSUBV_STR:
        canonical_unit_bits = unit_bits = 8;
/*
        if(cval->flags & APC_CONSTRAINED)
            unit_bits = cval->range_bits;
*/
        bpc = OS__BPC_CHAR;
        break;
    case ASN_OSUBV_U16:
        canonical_unit_bits = unit_bits = 16;
        if(cval->flags & APC_CONSTRAINED)
            unit_bits = cval->range_bits;
        bpc = OS__BPC_U16;
        break;
    case ASN_OSUBV_U32:
        canonical_unit_bits = unit_bits = 32;
        if(cval->flags & APC_CONSTRAINED)
            unit_bits = cval->range_bits;
        bpc = OS__BPC_U32;
        break;
    }

    /*
     * Allocate the string.
     */
    if(!st) {
        st = (BIT_STRING_t *)(*sptr = CALLOC(1, specs->struct_size));
        if(!st) RETURN(RC_FAIL);
    }

    ASN_DEBUG("PER Decoding %s size %lld .. %lld bits %d",
              csiz->flags & APC_EXTENSIBLE ? "extensible" : "non-extensible",
              (long long int)csiz->lower_bound, (long long int)csiz->upper_bound,
              csiz->effective_bits);

    if(csiz->flags & APC_EXTENSIBLE) {
        int inext = per_get_few_bits(pd, 1);
        if(inext < 0) RETURN(RC_WMORE);
        if(inext) {
            csiz = &asn_DEF_OCTET_STRING_constraints.size;
            cval = &asn_DEF_OCTET_STRING_constraints.value;
            unit_bits = canonical_unit_bits;
        }
    }

    if(csiz->effective_bits >= 0) {
        FREEMEM(st->buf);
        if(bpc) {
            st->size = csiz->upper_bound * bpc;
        } else {
            st->size = (csiz->upper_bound + 7) >> 3;
        }
        st->buf = (uint8_t *)MALLOC(st->size + 1);
        if(!st->buf) { st->size = 0; RETURN(RC_FAIL); }
    }

    /* X.691, #16.5: zero-length encoding */
    /* X.691, #16.6: short fixed length encoding (up to 2 octets) */
    /* X.691, #16.7: long fixed length encoding (up to 64K octets) */
    if(csiz->effective_bits == 0) {
        int ret;
        /* X.691 #16 NOTE 1 for fixed length (<= 16 bits) strings */
        if (st->size > 2 || csiz->range_bits != 0) {
            if (aper_get_align(pd) < 0)
                RETURN(RC_FAIL);
        }
        if(bpc) {
            ASN_DEBUG("Decoding OCTET STRING size %lld",
                      (long long int)csiz->upper_bound);
            ret = OCTET_STRING_per_get_characters(pd, st->buf,
                                                  csiz->upper_bound,
                                                  bpc, unit_bits,
                                                  cval->lower_bound,
                                                  cval->upper_bound,
                                                  pc);
            if(ret > 0) RETURN(RC_FAIL);
        } else {
            ASN_DEBUG("Decoding BIT STRING size %lld",
                      (long long int)csiz->upper_bound);
            ret = per_get_many_bits(pd, st->buf, 0,
                                    unit_bits * csiz->upper_bound);
        }
        if(ret < 0) RETURN(RC_WMORE);
        consumed_myself += unit_bits * csiz->upper_bound;
        st->buf[st->size] = 0;
        if(bpc == 0) {
            int ubs = (csiz->upper_bound & 0x7);
            st->bits_unused = ubs ? 8 - ubs : 0;
        }
        RETURN(RC_OK);
    }

    st->size = 0;
    do {
        ssize_t raw_len;
        ssize_t len_bytes;
        ssize_t len_bits;
        void *p;
        int ret;

        repeat = 0;
        /* Get the PER length */
        if (csiz->upper_bound - csiz->lower_bound == 0)
            /* Indefinite length case */
            raw_len = aper_get_length(pd, -1, -1, csiz->effective_bits, &repeat);
        else
            raw_len = aper_get_length(pd, csiz->lower_bound, csiz->upper_bound,
                                      csiz->effective_bits, &repeat);
        if(raw_len < 0) RETURN(RC_WMORE);
        raw_len += csiz->lower_bound;

        ASN_DEBUG("Got PER length eb %ld, len %ld, %s (%s)",
                  (long)csiz->effective_bits, (long)raw_len,
                  repeat ? "repeat" : "once", td->name);

        /* X.691 #16 NOTE 1  for fixed length (<=16 bits) strings */
        if ((raw_len > 2) || (csiz->upper_bound > 2) || (csiz->range_bits != 0))
        {
            if (aper_get_align(pd) < 0)
                RETURN(RC_FAIL);
        }

        if(bpc) {
            len_bytes = raw_len * bpc;
            len_bits = len_bytes * unit_bits;
        } else {
            len_bits = raw_len;
            len_bytes = (len_bits + 7) >> 3;
            if(len_bits & 0x7)
                st->bits_unused = 8 - (len_bits & 0x7);
            /* len_bits be multiple of 16K if repeat is set */
        }
        p = REALLOC(st->buf, st->size + len_bytes + 1);
        if(!p) RETURN(RC_FAIL);
        st->buf = (uint8_t *)p;

        if(bpc) {
            ret = OCTET_STRING_per_get_characters(pd,
                                                  &st->buf[st->size],
                                                  raw_len, bpc,
                                                  unit_bits,
                                                  cval->lower_bound,
                                                  cval->upper_bound,
                                                  pc);
            if(ret > 0) RETURN(RC_FAIL);
        } else {
            ret = per_get_many_bits(pd, &st->buf[st->size],
                                    0, len_bits);
        }
        if(ret < 0) RETURN(RC_WMORE);
        st->size += len_bytes;
    } while(repeat);
    st->buf[st->size] = 0;  /* nul-terminate */

    return rval;
}

asn_enc_rval_t
OCTET_STRING_encode_aper(const asn_TYPE_descriptor_t *td,
                         const asn_per_constraints_t *constraints,
                         const void *sptr, asn_per_outp_t *po) {

    const asn_OCTET_STRING_specifics_t *specs = td->specifics
        ? (const asn_OCTET_STRING_specifics_t *)td->specifics
        : &asn_SPC_OCTET_STRING_specs;
    const asn_per_constraints_t *pc = constraints
        ? constraints
        : td->encoding_constraints.per_constraints;
    const asn_per_constraint_t *cval;
    const asn_per_constraint_t *csiz;
    const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
    asn_enc_rval_t er = { 0, 0, 0 };
    int inext = 0;  /* Lies not within extension root */
    unsigned int unit_bits;
    unsigned int canonical_unit_bits;
    unsigned int sizeinunits;
    const uint8_t *buf;
    int ret;
    enum {
        OS__BPC_BIT  = 0,
        OS__BPC_CHAR = 1,
        OS__BPC_U16  = 2,
        OS__BPC_U32  = 4
    } bpc;  /* Bytes per character */
    int ct_extensible;

    if(!st || (!st->buf && st->size))
        ASN__ENCODE_FAILED;

    if(pc) {
        cval = &pc->value;
        csiz = &pc->size;
    } else {
        cval = &asn_DEF_OCTET_STRING_constraints.value;
        csiz = &asn_DEF_OCTET_STRING_constraints.size;
    }
    ct_extensible = csiz->flags & APC_EXTENSIBLE;

    switch(specs->subvariant) {
        default:
/*
        case ASN_OSUBV_ANY:
            ASN__ENCODE_FAILED;
*/
        case ASN_OSUBV_BIT:
            canonical_unit_bits = unit_bits = 1;
            bpc = OS__BPC_BIT;
            sizeinunits = st->size * 8 - (st->bits_unused & 0x07);
            ASN_DEBUG("BIT STRING of %d bytes",
                      sizeinunits);
        break;
        case ASN_OSUBV_ANY:
    case ASN_OSUBV_STR:
        canonical_unit_bits = unit_bits = 8;
/*
        if(cval->flags & APC_CONSTRAINED)
            unit_bits = 8;
*/
        bpc = OS__BPC_CHAR;
        sizeinunits = st->size;
        break;
    case ASN_OSUBV_U16:
        canonical_unit_bits = unit_bits = 16;
        if(cval->flags & APC_CONSTRAINED)
            unit_bits = cval->range_bits;
        bpc = OS__BPC_U16;
        sizeinunits = st->size / 2;
        break;
    case ASN_OSUBV_U32:
        canonical_unit_bits = unit_bits = 32;
        if(cval->flags & APC_CONSTRAINED)
            unit_bits = cval->range_bits;
        bpc = OS__BPC_U32;
        sizeinunits = st->size / 4;
        break;
    }

    ASN_DEBUG("Encoding %s into %d units of %d bits"
              " (%lld..%lld, effective %d)%s",
              td->name, sizeinunits, unit_bits,
              (long long int)csiz->lower_bound,
              (long long int)csiz->upper_bound,
              csiz->effective_bits, ct_extensible ? " EXT" : "");

    /* Figure out wheter size lies within PER visible constraint */

    if(csiz->effective_bits >= 0) {
        if((int)sizeinunits < csiz->lower_bound
        || (int)sizeinunits > csiz->upper_bound) {
            if(ct_extensible) {
                cval = &asn_DEF_OCTET_STRING_constraints.value;
                csiz = &asn_DEF_OCTET_STRING_constraints.size;
                unit_bits = canonical_unit_bits;
                inext = 1;
            } else
                ASN__ENCODE_FAILED;
        }
    } else {
        inext = 0;
    }

    if(ct_extensible) {
        /* Declare whether length is [not] within extension root */
        if(per_put_few_bits(po, inext, 1))
            ASN__ENCODE_FAILED;
    }

    /* X.691, #16.5: zero-length encoding */
    /* X.691, #16.6: short fixed length encoding (up to 2 octets) */
    /* X.691, #16.7: long fixed length encoding (up to 64K octets) */
    if(csiz->effective_bits >= 0) {
        ASN_DEBUG("Encoding %zu bytes (%lld), length in %d bits",
                  st->size, (long long int)(sizeinunits - csiz->lower_bound),
                  csiz->effective_bits);
        if (csiz->effective_bits > 0) {
                ret = aper_put_length(po, csiz->lower_bound, csiz->upper_bound,
                                      sizeinunits - csiz->lower_bound, NULL);
                if(ret < 0) ASN__ENCODE_FAILED;
        }
        if (csiz->effective_bits > 0 || (st->size > 2)
            || (csiz->upper_bound > (2 * 8 / unit_bits))
            || (csiz->range_bits != 0))
        { /* X.691 #16 NOTE 1 for fixed length (<=16 bits) strings*/
            if (aper_put_align(po) < 0)
                ASN__ENCODE_FAILED;
        }
        if(bpc) {
            ret = OCTET_STRING_per_put_characters(po, st->buf,
                                                  sizeinunits,
                                                  bpc, unit_bits,
                                                  cval->lower_bound,
                                                  cval->upper_bound,
                                                  pc);
        } else {
            ret = per_put_many_bits(po, st->buf,
                                    sizeinunits * unit_bits);
        }
        if(ret) ASN__ENCODE_FAILED;
        ASN__ENCODED_OK(er);
    }

    ASN_DEBUG("Encoding %zu bytes", st->size);

    if(sizeinunits == 0) {
        if(aper_put_length(po, -1, -1, 0, NULL) < 0)
            ASN__ENCODE_FAILED;
        ASN__ENCODED_OK(er);
    }

    buf = st->buf;
    while(sizeinunits) {
        int need_eom = 0;
        ssize_t maySave = aper_put_length(po, -1, -1, sizeinunits, &need_eom);

        if(maySave < 0) ASN__ENCODE_FAILED;

        ASN_DEBUG("Encoding %ld of %ld",
                  (long)maySave, (long)sizeinunits);

        if(bpc) {
            ret = OCTET_STRING_per_put_characters(po, buf, maySave,
                                                  bpc, unit_bits,
                                                  cval->lower_bound,
                                                  cval->upper_bound,
                                                  pc);
        } else {
            ret = per_put_many_bits(po, buf, maySave * unit_bits);
        }
        if(ret) ASN__ENCODE_FAILED;

        if(bpc)
            buf += maySave * bpc;
        else
            buf += maySave >> 3;
        sizeinunits -= maySave;
        assert(!(maySave & 0x07) || !sizeinunits);
        if(need_eom && (aper_put_length(po, -1, -1, 0, NULL) < 0))
            ASN__ENCODE_FAILED; /* End of Message length */
    }

    ASN__ENCODED_OK(er);
}
