/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_CHOICE.h>

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
 * Tags are canonically sorted in the tag to member table.
 */
static int
_search4tag(const void *ap, const void *bp) {
    const asn_TYPE_tag2member_t *a = (const asn_TYPE_tag2member_t *)ap;
    const asn_TYPE_tag2member_t *b = (const asn_TYPE_tag2member_t *)bp;

    int a_class = BER_TAG_CLASS(a->el_tag);
    int b_class = BER_TAG_CLASS(b->el_tag);

    if(a_class == b_class) {
        ber_tlv_tag_t a_value = BER_TAG_VALUE(a->el_tag);
        ber_tlv_tag_t b_value = BER_TAG_VALUE(b->el_tag);

        if(a_value == b_value)
            return 0;
        else if(a_value < b_value)
            return -1;
        else
            return 1;
    } else if(a_class < b_class) {
        return -1;
    } else {
        return 1;
    }
}

/*
 * The decoder of the CHOICE type.
 */
asn_dec_rval_t
CHOICE_decode_ber(const asn_codec_ctx_t *opt_codec_ctx,
                  const asn_TYPE_descriptor_t *td, void **struct_ptr,
                  const void *ptr, size_t size, int tag_mode) {
    /*
     * Bring closer parts of structure description.
     */
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    asn_TYPE_member_t *elements = td->elements;

    /*
     * Parts of the structure being constructed.
     */
    void *st = *struct_ptr;  /* Target structure. */
    asn_struct_ctx_t *ctx;   /* Decoder context */

    ber_tlv_tag_t tlv_tag;  /* T from TLV */
    ssize_t tag_len;        /* Length of TLV's T */
    asn_dec_rval_t rval;    /* Return code from subparsers */

    ssize_t consumed_myself = 0;  /* Consumed bytes from ptr */

    ASN_DEBUG("Decoding %s as CHOICE", td->name);

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

        if(tag_mode || td->tags_count) {
            rval = ber_check_tags(opt_codec_ctx, td, ctx, ptr, size,
                                  tag_mode, -1, &ctx->left, 0);
            if(rval.code != RC_OK) {
                ASN_DEBUG("%s tagging check failed: %d",
                          td->name, rval.code);
                return rval;
            }

            if(ctx->left >= 0) {
                /* ?Subtracted below! */
                ctx->left += rval.consumed;
            }
            ADVANCE(rval.consumed);
        } else {
            ctx->left = -1;
        }

        NEXT_PHASE(ctx);

        ASN_DEBUG("Structure consumes %ld bytes, buffer %ld",
                  (long)ctx->left, (long)size);

        /* Fall through */
    case 1:
        /*
         * Fetch the T from TLV.
         */
        tag_len = ber_fetch_tag(ptr, LEFT, &tlv_tag);
        ASN_DEBUG("In %s CHOICE tag length %d", td->name, (int)tag_len);
        switch(tag_len) {
        case 0: if(!SIZE_VIOLATION) RETURN(RC_WMORE);
            /* Fall through */
        case -1: RETURN(RC_FAIL);
        }

        do {
            const asn_TYPE_tag2member_t *t2m;
            asn_TYPE_tag2member_t key;

            key.el_tag = tlv_tag;
            t2m = (const asn_TYPE_tag2member_t *)bsearch(&key,
                    specs->tag2el, specs->tag2el_count,
                    sizeof(specs->tag2el[0]), _search4tag);
            if(t2m) {
                /*
                 * Found the element corresponding to the tag.
                 */
                NEXT_PHASE(ctx);
                ctx->step = t2m->el_no;
                break;
            } else if(specs->ext_start == -1) {
                ASN_DEBUG("Unexpected tag %s "
                          "in non-extensible CHOICE %s",
                          ber_tlv_tag_string(tlv_tag), td->name);
                RETURN(RC_FAIL);
            } else {
                /* Skip this tag */
                ssize_t skip;

                ASN_DEBUG("Skipping unknown tag %s",
                          ber_tlv_tag_string(tlv_tag));

                skip = ber_skip_length(opt_codec_ctx,
                                       BER_TLV_CONSTRUCTED(ptr),
                                       (const char *)ptr + tag_len,
                                       LEFT - tag_len);

                switch(skip) {
                case 0: if(!SIZE_VIOLATION) RETURN(RC_WMORE);
                    /* Fall through */
                case -1: RETURN(RC_FAIL);
                }

                ADVANCE(skip + tag_len);
                RETURN(RC_OK);
            }
        } while(0);

    case 2:
        /*
         * PHASE 2.
         * Read in the element.
         */
        do {
            asn_TYPE_member_t *elm;/* CHOICE's element */
            void *memb_ptr;    /* Pointer to the member */
            void **memb_ptr2;  /* Pointer to that pointer */

            elm = &elements[ctx->step];

            /*
             * Compute the position of the member inside a structure,
             * and also a type of containment (it may be contained
             * as pointer or using inline inclusion).
             */
            if(elm->flags & ATF_POINTER) {
                /* Member is a pointer to another structure */
                memb_ptr2 = (void **)((char *)st + elm->memb_offset);
            } else {
                /*
                 * A pointer to a pointer
                 * holding the start of the structure
                 */
                memb_ptr = (char *)st + elm->memb_offset;
                memb_ptr2 = &memb_ptr;
            }
            /* Set presence to be able to free it properly at any time */
            _set_present_idx(st, specs->pres_offset,
                             specs->pres_size, ctx->step + 1);
            /*
             * Invoke the member fetch routine according to member's type
             */
            rval = elm->type->op->ber_decoder(opt_codec_ctx, elm->type,
                                              memb_ptr2, ptr, LEFT,
                                              elm->tag_mode);
            switch(rval.code) {
            case RC_OK:
                break;
            case RC_WMORE: /* More data expected */
                if(!SIZE_VIOLATION) {
                    ADVANCE(rval.consumed);
                    RETURN(RC_WMORE);
                }
                RETURN(RC_FAIL);
            case RC_FAIL: /* Fatal error */
                RETURN(rval.code);
            } /* switch(rval) */

            ADVANCE(rval.consumed);
        } while(0);

        NEXT_PHASE(ctx);

        /* Fall through */
    case 3:
        ASN_DEBUG("CHOICE %s Leftover: %ld, size = %ld, tm=%d, tc=%d",
                  td->name, (long)ctx->left, (long)size,
                  tag_mode, td->tags_count);

        if(ctx->left > 0) {
            /*
             * The type must be fully decoded
             * by the CHOICE member-specific decoder.
             */
            RETURN(RC_FAIL);
        }

        if(ctx->left == -1
        && !(tag_mode || td->tags_count)) {
            /*
             * This is an untagged CHOICE.
             * It doesn't contain nothing
             * except for the member itself, including all its tags.
             * The decoding is completed.
             */
            NEXT_PHASE(ctx);
            break;
        }

        /*
         * Read in the "end of data chunks"'s.
         */
        while(ctx->left < 0) {
            ssize_t tl;

            tl = ber_fetch_tag(ptr, LEFT, &tlv_tag);
            switch(tl) {
            case 0: if(!SIZE_VIOLATION) RETURN(RC_WMORE);
                /* Fall through */
            case -1: RETURN(RC_FAIL);
            }

            /*
             * Expected <0><0>...
             */
            if(((const uint8_t *)ptr)[0] == 0) {
                if(LEFT < 2) {
                    if(SIZE_VIOLATION)
                        RETURN(RC_FAIL);
                    else
                        RETURN(RC_WMORE);
                } else if(((const uint8_t *)ptr)[1] == 0) {
                    /*
                     * Correctly finished with <0><0>.
                     */
                    ADVANCE(2);
                    ctx->left++;
                    continue;
                }
            } else {
                ASN_DEBUG("Unexpected continuation in %s",
                          td->name);
                RETURN(RC_FAIL);
            }

            /* UNREACHABLE */
        }

        NEXT_PHASE(ctx);
    case 4:
        /* No meaningful work here */
        break;
    }

    RETURN(RC_OK);
}

asn_enc_rval_t
CHOICE_encode_der(const asn_TYPE_descriptor_t *td, const void *sptr,
                  int tag_mode, ber_tlv_tag_t tag, asn_app_consume_bytes_f *cb,
                  void *app_key) {
    const asn_CHOICE_specifics_t *specs = (const asn_CHOICE_specifics_t *)td->specifics;
    asn_TYPE_member_t *elm;  /* CHOICE element */
    asn_enc_rval_t erval = {0,0,0};
    const void *memb_ptr;
    size_t computed_size = 0;
    unsigned present;

    if(!sptr) ASN__ENCODE_FAILED;

    ASN_DEBUG("%s %s as CHOICE",
              cb ? "Encoding" : "Estimating", td->name);

    /* Check encoding recursion depth to prevent stack overflow */
    ASN__ENCODER_RECURSION_DEPTH_INC();

    present = _fetch_present_idx(sptr,
        specs->pres_offset, specs->pres_size);

    /*
     * If the structure was not initialized, it cannot be encoded:
     * can't deduce what to encode in the choice type.
     */
    if(present == 0 || present > td->elements_count) {
        if(present == 0 && td->elements_count == 0) {
            /* The CHOICE is empty?! */
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            erval.encoded = 0;
            ASN__ENCODED_OK(erval);
        }
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODE_FAILED;
    }

    /*
     * Seek over the present member of the structure.
     */
    elm = &td->elements[present-1];
    if(elm->flags & ATF_POINTER) {
        memb_ptr =
            *(const void *const *)((const char *)sptr + elm->memb_offset);
        if(memb_ptr == 0) {
            if(elm->optional) {
                ASN__ENCODER_RECURSION_DEPTH_DEC();
                erval.encoded = 0;
                ASN__ENCODED_OK(erval);
            }
            /* Mandatory element absent */
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            ASN__ENCODE_FAILED;
        }
    } else {
        memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
    }

    /*
     * If the CHOICE itself is tagged EXPLICIT:
     * T ::= [2] EXPLICIT CHOICE { ... }
     * Then emit the appropriate tags.
     */
    if(tag_mode == 1 || td->tags_count) {
        /*
         * For this, we need to pre-compute the member.
         */
        ssize_t ret;

        /* Encode member with its tag */
        erval = elm->type->op->der_encoder(elm->type, memb_ptr,
                                           elm->tag_mode,
                                           elm->tag, 0, 0);
        if(erval.encoded == -1) {
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            return erval;
        }

        /* Encode CHOICE with parent or my own tag */
        ret = der_write_tags(td, erval.encoded, tag_mode, 1, tag,
                             cb, app_key);
        if(ret == -1) {
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            ASN__ENCODE_FAILED;
        }
        computed_size += ret;
    }

    /*
     * Encode the single underlying member.
     */
    erval = elm->type->op->der_encoder(elm->type, memb_ptr,
                                       elm->tag_mode, elm->tag,
                                       cb, app_key);
    if(erval.encoded == -1) {
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        return erval;
    }

    ASN_DEBUG("Encoded CHOICE member in %ld bytes (+%ld)",
              (long)erval.encoded, (long)computed_size);

    erval.encoded += computed_size;

    ASN__ENCODER_RECURSION_DEPTH_DEC();
    return erval;
}
