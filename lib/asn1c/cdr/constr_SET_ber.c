/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SET.h>

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
 * Tags are canonically sorted in the tag2element map.
 */
static int
_t2e_cmp(const void *ap, const void *bp) {
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
 * The decoder of the SET type.
 */
asn_dec_rval_t
SET_decode_ber(const asn_codec_ctx_t *opt_codec_ctx,
               const asn_TYPE_descriptor_t *td, void **struct_ptr,
               const void *ptr, size_t size, int tag_mode) {
    /*
     * Bring closer parts of structure description.
     */
    const asn_SET_specifics_t *specs = (const asn_SET_specifics_t *)td->specifics;
    const asn_TYPE_member_t *elements = td->elements;

    /*
     * Parts of the structure being constructed.
     */
    void *st = *struct_ptr;  /* Target structure. */
    asn_struct_ctx_t *ctx;   /* Decoder context */

    ber_tlv_tag_t tlv_tag;  /* T from TLV */
    asn_dec_rval_t rval;    /* Return code from subparsers */

    ssize_t consumed_myself = 0;  /* Consumed bytes from ptr */
    size_t edx;                   /* SET element's index */

    ASN_DEBUG("Decoding %s as SET", td->name);

    if(ASN__STACK_OVERFLOW_CHECK(opt_codec_ctx))
        ASN__DECODE_FAILED;

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

        NEXT_PHASE(ctx);

        ASN_DEBUG("Structure advertised %ld bytes, "
                  "buffer contains %ld", (long)ctx->left, (long)size);

        /* Fall through */
    case 1:
        /*
         * PHASE 1.
         * From the place where we've left it previously,
         * try to decode the next member from the list of
         * this structure's elements.
         * Note that elements in BER may arrive out of
         * order, yet DER mandates that they shall arrive in the
         * canonical order of their tags. So, there is a room
         * for optimization.
         */
        for(;; ctx->step = 0) {
            const asn_TYPE_tag2member_t *t2m;
            asn_TYPE_tag2member_t key;
            void *memb_ptr;    /* Pointer to the member */
            void **memb_ptr2;  /* Pointer to that pointer */
            ssize_t tag_len;   /* Length of TLV's T */

            if(ctx->step & 1) {
                edx = ctx->step >> 1;
                goto microphase2;
            }

            /*
             * MICROPHASE 1: Synchronize decoding.
             */

            if(ctx->left == 0)
                /*
                 * No more things to decode.
                 * Exit out of here and check whether all mandatory
                 * elements have been received (in the next phase).
                 */
                break;

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
                     * Invoke the generic finalization function.
                     */
                    goto phase3;
                }
            }

            key.el_tag = tlv_tag;
            t2m = (const asn_TYPE_tag2member_t *)bsearch(&key,
                                                         specs->tag2el,
                                                         specs->tag2el_count,
                                                         sizeof(specs->tag2el[0]),
                                                         _t2e_cmp);
            if(t2m) {
                /*
                 * Found the element corresponding to the tag.
                 */
                edx = t2m->el_no;
                ctx->step = (edx << 1) + 1;
                ASN_DEBUG("Got tag %s (%s), edx %" ASN_PRI_SSIZE "",
                          ber_tlv_tag_string(tlv_tag), td->name, edx);
            } else if(specs->extensible == 0) {
                ASN_DEBUG("Unexpected tag %s "
                          "in non-extensible SET %s",
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
                continue;  /* Try again with the next tag */
            }

            /*
             * MICROPHASE 2: Invoke the member-specific decoder.
             */
        microphase2:

            /*
             * Check for duplications: must not overwrite
             * already decoded elements.
             */
            if(ASN_SET_ISPRESENT2((char *)st + specs->pres_offset, edx)) {
                ASN_DEBUG("SET %s: Duplicate element %s (%" ASN_PRI_SSIZE ")",
                          td->name, elements[edx].name, edx);
                RETURN(RC_FAIL);
            }

            /*
             * Compute the position of the member inside a structure,
             * and also a type of containment (it may be contained
             * as pointer or using inline inclusion).
             */
            if(elements[edx].flags & ATF_POINTER) {
                /* Member is a pointer to another structure */
                memb_ptr2 = (void **)((char *)st + elements[edx].memb_offset);
            } else {
                /*
                 * A pointer to a pointer
                 * holding the start of the structure
                 */
                memb_ptr = (char *)st + elements[edx].memb_offset;
                memb_ptr2 = &memb_ptr;
            }
            /*
             * Invoke the member fetch routine according to member's type
             */
            rval = elements[edx].type->op->ber_decoder(opt_codec_ctx,
                                                       elements[edx].type,
                                                       memb_ptr2, ptr, LEFT,
                                                       elements[edx].tag_mode);
            switch(rval.code) {
            case RC_OK:
                ASN_SET_MKPRESENT((char *)st + specs->pres_offset, edx);
                break;
            case RC_WMORE: /* More data expected */
                if(!SIZE_VIOLATION) {
                    ADVANCE(rval.consumed);
                    RETURN(RC_WMORE);
                }
                /* Fall through */
            case RC_FAIL:  /* Fatal error */
                RETURN(RC_FAIL);
            }  /* switch(rval) */

            ADVANCE(rval.consumed);
        }  /* for(all structure members) */

    phase3:
        ctx->phase = 3;
        /* Fall through */
    case 3:
    case 4:  /* Only 00 is expected */
        ASN_DEBUG("SET %s Leftover: %ld, size = %ld",
                  td->name, (long)ctx->left, (long)size);

        /*
         * Skip everything until the end of the SET.
         */
        while(ctx->left) {
            ssize_t tl, ll;

            tl = ber_fetch_tag(ptr, LEFT, &tlv_tag);
            switch(tl) {
            case 0: if(!SIZE_VIOLATION) RETURN(RC_WMORE);
                /* Fall through */
            case -1: RETURN(RC_FAIL);
            }

            /*
             * If expected <0><0>...
             */
            if(ctx->left < 0
                && ((const uint8_t *)ptr)[0] == 0) {
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
                    ctx->phase = 4;
                    continue;
                }
            }

            if(specs->extensible == 0 || ctx->phase == 4) {
                ASN_DEBUG("Unexpected continuation "
                          "of a non-extensible type %s "
                          "(ptr=%02x)",
                          td->name, *(const uint8_t *)ptr);
                RETURN(RC_FAIL);
            }

            ll = ber_skip_length(opt_codec_ctx,
                                 BER_TLV_CONSTRUCTED(ptr),
                                 (const char *)ptr + tl, LEFT - tl);
            switch(ll) {
            case 0: if(!SIZE_VIOLATION) RETURN(RC_WMORE);
                /* Fall through */
            case -1: RETURN(RC_FAIL);
            }

            ADVANCE(tl + ll);
        }

        ctx->phase = 5;
        /* Fall through */
    case 5:
        /* Check that all mandatory elements are present. */
        if(!_SET_is_populated(td, st))
            RETURN(RC_FAIL);

        NEXT_PHASE(ctx);
    }

    RETURN(RC_OK);
}

/*
 * The DER encoder of the SET type.
 */
asn_enc_rval_t
SET_encode_der(const asn_TYPE_descriptor_t *td, const void *sptr, int tag_mode,
               ber_tlv_tag_t tag, asn_app_consume_bytes_f *cb, void *app_key) {
    const asn_SET_specifics_t *specs = (const asn_SET_specifics_t *)td->specifics;
    size_t computed_size = 0;
    asn_enc_rval_t er;
    int t2m_build_own = (specs->tag2el_count != td->elements_count);
    const asn_TYPE_tag2member_t *t2m;
    asn_TYPE_tag2member_t *t2m_build;
    size_t t2m_count;
    ssize_t ret;
    size_t edx;

    /* Check encoding recursion depth to prevent stack overflow */
    ASN__ENCODER_RECURSION_DEPTH_INC();

    /*
     * Use existing, or build our own tags map.
     */
    if(t2m_build_own) {
        t2m_build = (asn_TYPE_tag2member_t *)CALLOC(td->elements_count,
                                                    sizeof(t2m_build[0]));
        if(!t2m_build) {
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            ASN__ENCODE_FAILED;
        }
        t2m_count = 0;
    } else {
        t2m_build = NULL;
        /*
         * There is no untagged CHOICE in this SET.
         * Employ existing table.
         */
    }

    /*
     * Gather the length of the underlying members sequence.
     */
    for(edx = 0; edx < td->elements_count; edx++) {
        asn_TYPE_member_t *elm = &td->elements[edx];
        asn_enc_rval_t tmper;
        const void *memb_ptr_dontuse;  /* Pointer to the member */
        const void *const *memb_ptr2;  /* Pointer to that pointer */

        /*
         * Compute the length of the encoding of this member.
         */
        if(elm->flags & ATF_POINTER) {
            memb_ptr2 =
                (const void *const *)((const char *)sptr + elm->memb_offset);
            if(!*memb_ptr2) {
                if(!elm->optional) {
                    /* Mandatory elements missing */
                    FREEMEM(t2m_build);
                    ASN__ENCODER_RECURSION_DEPTH_DEC();
                    ASN__ENCODE_FAILED;
                }
                if(t2m_build) {
                    t2m_build[t2m_count].el_no = edx;
                    t2m_build[t2m_count].el_tag = 0;
                    t2m_count++;
                }
                continue;
            }
        } else {
            memb_ptr_dontuse =
                (const void *)((const char *)sptr + elm->memb_offset);
            memb_ptr2 = &memb_ptr_dontuse; /* Only use of memb_ptr_dontuse */
        }

        /* Eliminate default values */
        if(elm->default_value_cmp && elm->default_value_cmp(*memb_ptr2) == 0) {
            if(t2m_build) {
                t2m_build[t2m_count].el_no = edx;
                t2m_build[t2m_count].el_tag = 0;
                t2m_count++;
            }
            continue;
        }

        tmper = elm->type->op->der_encoder(elm->type, *memb_ptr2,
                                           elm->tag_mode, elm->tag,
                                           0, 0);
        if(tmper.encoded == -1) {
            FREEMEM(t2m_build);
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            return tmper;
        }
        computed_size += tmper.encoded;

        /*
         * Remember the outmost tag of this member.
         */
        if(t2m_build) {
            t2m_build[t2m_count].el_no = edx;
            t2m_build[t2m_count].el_tag = asn_TYPE_outmost_tag(
                    elm->type, *memb_ptr2, elm->tag_mode, elm->tag);
            t2m_count++;
        } else {
            /*
             * No dynamic sorting is necessary.
             */
        }
    }

    /*
     * Finalize order of the components.
     */
    if(t2m_build) {
        /*
         * Sort the underlying members according to their
         * canonical tags order. DER encoding mandates it.
         */
        qsort(t2m_build, t2m_count, sizeof(specs->tag2el[0]), _t2e_cmp);
        t2m = t2m_build;
    } else {
        /*
         * Tags are already sorted by the compiler.
         */
        t2m = specs->tag2el;
        t2m_count = specs->tag2el_count;
    }
    assert(t2m_count == td->elements_count);

    /*
     * Encode the TLV for the sequence itself.
     */
    ret = der_write_tags(td, computed_size, tag_mode, 1, tag, cb, app_key);
    if(ret == -1) {
        FREEMEM(t2m_build);
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODE_FAILED;
    }
    er.encoded = computed_size + ret;

    if(!cb) {
        FREEMEM(t2m_build);
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODED_OK(er);
    }

    /*
     * Encode all members.
     */
    for(edx = 0; edx < td->elements_count; edx++) {
        asn_TYPE_member_t *elm;
        asn_enc_rval_t tmper;

        const void *memb_ptr_dontuse;  /* Pointer to the member */
        const void *const *memb_ptr2;  /* Pointer to that pointer */

        /* Encode according to the tag order */
        elm = &td->elements[t2m[edx].el_no];

        if(elm->flags & ATF_POINTER) {
            memb_ptr2 =
                (const void *const *)((const char *)sptr + elm->memb_offset);
            if(!*memb_ptr2) continue;
        } else {
            memb_ptr_dontuse =
                (const void *)((const char *)sptr + elm->memb_offset);
            memb_ptr2 = &memb_ptr_dontuse;  /* Only use of memb_ptr_dontuse */
        }

        /* Eliminate default values */
        if(elm->default_value_cmp && elm->default_value_cmp(*memb_ptr2) == 0)
            continue;

        tmper = elm->type->op->der_encoder(elm->type, *memb_ptr2,
                                           elm->tag_mode, elm->tag,
                                           cb, app_key);
        if(tmper.encoded == -1) {
            FREEMEM(t2m_build);
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            return tmper;
        }
        computed_size -= tmper.encoded;
    }

    if(computed_size != 0) {
        /*
         * Encoded size is not equal to the computed size.
         */
        FREEMEM(t2m_build);
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODE_FAILED;
    }

    FREEMEM(t2m_build);
    ASN__ENCODER_RECURSION_DEPTH_DEC();
    ASN__ENCODED_OK(er);
}
