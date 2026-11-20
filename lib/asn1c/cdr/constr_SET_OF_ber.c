/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SET_OF.h>
#include <asn_SET_OF.h>

/*
 * Number of bytes left for this structure.
 * (ctx->left) indicates the number of bytes _transferred_ for the structure.
 * (size) contains the number of bytes in the buffer passed.
 */
#define LEFT ((size<(size_t)ctx->left)?size:(size_t)ctx->left)

/*
 * If the subprocessor function returns with an indication that it wants
 * more data, it may well be a fatal decoding problem, because the
 * size is constrained by the <TLV>'s L, even if the buffer size allows
 * reading more data.
 * For example, consider the buffer containing the following TLVs:
 * <T:5><L:1><V> <T:6>...
 * The TLV length clearly indicates that one byte is expected in V, but
 * if the V processor returns with "want more data" even if the buffer
 * contains way more data than the V processor have seen.
 */
#define SIZE_VIOLATION (ctx->left >= 0 && (size_t)ctx->left <= size)

/*
 * This macro "eats" the part of the buffer which is definitely "consumed",
 * i.e. was correctly converted into local representation or rightfully skipped.
 */
#undef ADVANCE
#define ADVANCE(num_bytes)                \
    do {                                  \
        size_t num = num_bytes;           \
        ptr = ((const char *)ptr) + num;  \
        size -= num;                      \
        if(ctx->left >= 0)                \
            ctx->left -= num;             \
        consumed_myself += num;           \
    } while(0)

/*
 * Switch to the next phase of parsing.
 */
#undef NEXT_PHASE
#define NEXT_PHASE(ctx)  \
    do {                 \
        ctx->phase++;    \
        ctx->step = 0;   \
    } while(0)
#undef PHASE_OUT
#define PHASE_OUT(ctx)    \
    do {                  \
        ctx->phase = 10;  \
    } while(0)

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

/*
 * The decoder of the SET OF type.
 */
asn_dec_rval_t
SET_OF_decode_ber(const asn_codec_ctx_t *opt_codec_ctx,
                  const asn_TYPE_descriptor_t *td, void **struct_ptr,
                  const void *ptr, size_t size, int tag_mode) {
    /*
     * Bring closer parts of structure description.
     */
    const asn_SET_OF_specifics_t *specs = (const asn_SET_OF_specifics_t *)td->specifics;
    const asn_TYPE_member_t *elm = td->elements; /* Single one */

    /*
     * Parts of the structure being constructed.
     */
    void *st = *struct_ptr;  /* Target structure. */
    asn_struct_ctx_t *ctx;   /* Decoder context */

    ber_tlv_tag_t tlv_tag;  /* T from TLV */
    asn_dec_rval_t rval;    /* Return code from subparsers */

    ssize_t consumed_myself = 0;  /* Consumed bytes from ptr */

    ASN_DEBUG("Decoding %s as SET OF", td->name);

    /*
     * Create the target structure if it is not present already.
     */
    if(st == 0) {
        st = *struct_ptr = CALLOC(1, specs->struct_size);
        if(st == 0) {
            RETURN(RC_FAIL);
        }
    }

    /*
     * Restore parsing context.
     */
    ctx = (asn_struct_ctx_t *)((char *)st + specs->ctx_offset);

    /* Check recursion depth to prevent stack overflow */
    ASN__DECODER_RECURSION_DEPTH_CHECK(opt_codec_ctx);

    /*
     * Start to parse where left previously
     */
    switch(ctx->phase) {
    case 0:
        /*
         * PHASE 0.
         * Check that the set of tags associated with given structure
         * perfectly fits our expectations.
         */

        rval = ber_check_tags(opt_codec_ctx, td, ctx, ptr, size,
                              tag_mode, 1, &ctx->left, 0);
        if(rval.code != RC_OK) {
            ASN_DEBUG("%s tagging check failed: %d",
                      td->name, rval.code);
            return rval;
        }

        if(ctx->left >= 0)
            ctx->left += rval.consumed;  /* ?Subtracted below! */
        ADVANCE(rval.consumed);

        ASN_DEBUG("Structure consumes %ld bytes, "
                  "buffer %ld", (long)ctx->left, (long)size);

        NEXT_PHASE(ctx);
        /* Fall through */
    case 1:
        /*
         * PHASE 1.
         * From the place where we've left it previously,
         * try to decode the next item.
         */
        for(;; ctx->step = 0) {
            ssize_t tag_len;  /* Length of TLV's T */

            if(ctx->step & 1)
                goto microphase2;

            /*
             * MICROPHASE 1: Synchronize decoding.
             */

            if(ctx->left == 0) {
                ASN_DEBUG("End of SET OF %s", td->name);
                /*
                 * No more things to decode.
                 * Exit out of here.
                 */
                PHASE_OUT(ctx);
                RETURN(RC_OK);
            }

            /*
             * Fetch the T from TLV.
             */
            tag_len = ber_fetch_tag(ptr, LEFT, &tlv_tag);
            switch(tag_len) {
            case 0: if(!SIZE_VIOLATION) RETURN(RC_WMORE);
                /* Fall through */
            case -1: RETURN(RC_FAIL);
            }

            if(ctx->left < 0 && ((const uint8_t *)ptr)[0] == 0) {
                if(LEFT < 2) {
                    if(SIZE_VIOLATION)
                        RETURN(RC_FAIL);
                    else
                        RETURN(RC_WMORE);
                } else if(((const uint8_t *)ptr)[1] == 0) {
                    /*
                     * Found the terminator of the
                     * indefinite length structure.
                     */
                    break;
                }
            }

            /* Outmost tag may be unknown and cannot be fetched/compared */
            if(elm->tag != (ber_tlv_tag_t)-1) {
                if(BER_TAGS_EQUAL(tlv_tag, elm->tag)) {
                /*
                 * The new list member of expected type has arrived.
                 */
                } else {
                    ASN_DEBUG("Unexpected tag %s fixed SET OF %s",
                              ber_tlv_tag_string(tlv_tag), td->name);
                    ASN_DEBUG("%s SET OF has tag %s",
                              td->name, ber_tlv_tag_string(elm->tag));
                    RETURN(RC_FAIL);
                }
            }

            /*
             * MICROPHASE 2: Invoke the member-specific decoder.
             */
            ctx->step |= 1;  /* Confirm entering next microphase */
        microphase2:

            /*
             * Invoke the member fetch routine according to member's type
             */
            rval = elm->type->op->ber_decoder(opt_codec_ctx,
                                              elm->type, &ctx->ptr,
                                              ptr, LEFT, elm->tag_mode);
            ASN_DEBUG("In %s SET OF %s code %d consumed %d",
                      td->name, elm->type->name,
                      rval.code, (int)rval.consumed);
            switch(rval.code) {
            case RC_OK:
                {
                    asn_anonymous_set_ *list = _A_SET_FROM_VOID(st);
                    if(ASN_SET_ADD(list, ctx->ptr) != 0)
                        RETURN(RC_FAIL);
                    else
                        ctx->ptr = 0;
                }
                break;
            case RC_WMORE:  /* More data expected */
                if(!SIZE_VIOLATION) {
                    ADVANCE(rval.consumed);
                    RETURN(RC_WMORE);
                }
                /* Fall through */
            case RC_FAIL:  /* Fatal error */
                ASN_STRUCT_FREE(*elm->type, ctx->ptr);
                ctx->ptr = 0;
                RETURN(RC_FAIL);
            }  /* switch(rval) */

            ADVANCE(rval.consumed);
        }  /* for(all list members) */

        NEXT_PHASE(ctx);
    case 2:
        /*
         * Read in all "end of content" TLVs.
         */
        while(ctx->left < 0) {
            if(LEFT < 2) {
                if(LEFT > 0 && ((const char *)ptr)[0] != 0) {
                    /* Unexpected tag */
                    RETURN(RC_FAIL);
                } else {
                    RETURN(RC_WMORE);
                }
            }
            if(((const char *)ptr)[0] == 0
            && ((const char *)ptr)[1] == 0) {
                ADVANCE(2);
                ctx->left++;
            } else {
                RETURN(RC_FAIL);
            }
        }

        PHASE_OUT(ctx);
    }

    RETURN(RC_OK);
}

/*
 * The DER encoder of the SET OF type.
 */
asn_enc_rval_t
SET_OF_encode_der(const asn_TYPE_descriptor_t *td, const void *sptr,
                  int tag_mode, ber_tlv_tag_t tag, asn_app_consume_bytes_f *cb,
                  void *app_key) {
    const asn_TYPE_member_t *elm = td->elements;
    const asn_anonymous_set_ *list = _A_CSET_FROM_VOID(sptr);
    size_t computed_size = 0;
    ssize_t encoding_size = 0;
    struct _el_buffer *encoded_els;
    int edx;

    ASN_DEBUG("Estimating size for SET OF %s", td->name);

    /* Check encoding recursion depth to prevent stack overflow */
    ASN__ENCODER_RECURSION_DEPTH_INC();

    /*
     * Gather the length of the underlying members sequence.
     */
    for(edx = 0; edx < list->count; edx++) {
        void *memb_ptr = list->array[edx];
        asn_enc_rval_t erval = {0,0,0};

        if(!memb_ptr) {
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            ASN__ENCODE_FAILED;
        }

        erval =
            elm->type->op->der_encoder(elm->type, memb_ptr, elm->tag_mode, elm->tag, 0, 0);
        if(erval.encoded == -1) {
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            return erval;
        }
        computed_size += erval.encoded;
    }

    /*
     * Encode the TLV for the sequence itself.
     */
    encoding_size =
        der_write_tags(td, computed_size, tag_mode, 1, tag, cb, app_key);
    if(encoding_size < 0) {
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODE_FAILED;
    }
    computed_size += encoding_size;

    if(!cb || list->count == 0) {
        asn_enc_rval_t erval = {0,0,0};
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        erval.encoded = computed_size;
        ASN__ENCODED_OK(erval);
    }

    ASN_DEBUG("Encoding members of %s SET OF", td->name);

    /*
     * DER mandates dynamic sorting of the SET OF elements
     * according to their encodings. Build an array of the
     * encoded elements.
     */
    encoded_els = SET_OF__encode_sorted(elm, list, SOES_DER);

    /*
     * Report encoded elements to the application.
     * Dispose of temporary sorted members table.
     */
    for(edx = 0; edx < list->count; edx++) {
        struct _el_buffer *encoded_el = &encoded_els[edx];
        /* Report encoded chunks to the application */
        if(cb(encoded_el->buf, encoded_el->length, app_key) < 0) {
            break;
        } else {
            encoding_size += encoded_el->length;
        }
    }

    SET_OF__encode_sorted_free(encoded_els, list->count);

    if(edx == list->count) {
        asn_enc_rval_t erval = {0,0,0};
        assert(computed_size == (size_t)encoding_size);
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        erval.encoded = computed_size;
        ASN__ENCODED_OK(erval);
    } else {
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODE_FAILED;
    }
}
