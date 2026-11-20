/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>
#include <OPEN_TYPE.h>

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
 * Check whether we are inside the extensions group.
 */
#define IN_EXTENSION_GROUP(specs, memb_idx)                \
    ((specs)->first_extension >= 0                         \
     && (unsigned)(specs)->first_extension <= (memb_idx))

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

        if(a_value == b_value) {
            if(a->el_no > b->el_no)
                return 1;
            /*
             * Important: we do not check
             * for a->el_no <= b->el_no!
             */
            return 0;
        } else if(a_value < b_value)
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
 * The decoder of the SEQUENCE type.
 */
asn_dec_rval_t
SEQUENCE_decode_ber(const asn_codec_ctx_t *opt_codec_ctx,
                    const asn_TYPE_descriptor_t *td, void **struct_ptr,
                    const void *ptr, size_t size, int tag_mode) {
    /*
     * Bring closer parts of structure description.
     */
    const asn_SEQUENCE_specifics_t *specs = (const asn_SEQUENCE_specifics_t *)td->specifics;
    const asn_TYPE_member_t *elements = td->elements;

    /*
     * Parts of the structure being constructed.
     */
    void *st = *struct_ptr;  /* Target structure. */
    asn_struct_ctx_t *ctx;   /* Decoder context */

    ber_tlv_tag_t tlv_tag;  /* T from TLV */
    asn_dec_rval_t rval;    /* Return code from subparsers */

    ssize_t consumed_myself = 0;  /* Consumed bytes from ptr */
    size_t edx;  /* SEQUENCE element's index */

    ASN_DEBUG("Decoding %s as SEQUENCE", td->name);

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

	/*
     * Check recursion depth to prevent stack overflow from circular references,
     * using ctx->step 
	 */
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

        ASN_DEBUG("Structure consumes %ld bytes, buffer %ld",
                  (long)ctx->left, (long)size);

        /* Fall through */
    case 1:
        /*
         * PHASE 1.
         * From the place where we've left it previously,
         * try to decode the next member from the list of
         * this structure's elements.
         * (ctx->step) stores the member being processed
         * between invocations and the microphase {0,1} of parsing
         * that member:
         *     step = (<member_number> * 2 + <microphase>).
         */
        for(edx = ((size_t)ctx->step >> 1); edx < td->elements_count;
                edx++, ctx->step = (ctx->step & ~1) + 2) {
            void *memb_ptr;      /* Pointer to the member */
            void **memb_ptr2;    /* Pointer to that pointer */
            ssize_t tag_len;     /* Length of TLV's T */
            size_t opt_edx_end;  /* Next non-optional element */
            size_t n;
            int use_bsearch;

            if(ctx->step & 1)
                goto microphase2;

            /*
             * MICROPHASE 1: Synchronize decoding.
             */
            ASN_DEBUG("In %s SEQUENCE left %d, edx=%" ASN_PRI_SIZE " flags=%d"
                      " opt=%d ec=%d",
                      td->name, (int)ctx->left, edx,
                      elements[edx].flags, elements[edx].optional,
                      td->elements_count);

            if(ctx->left == 0 /* No more stuff is expected */
               && (
                      /* Explicit OPTIONAL specification reaches the end */
                      (edx + elements[edx].optional == td->elements_count) ||
                      /* All extensions are optional */
                      IN_EXTENSION_GROUP(specs, edx))) {
                ASN_DEBUG("End of SEQUENCE %s", td->name);
                /*
                 * Found the legitimate end of the structure.
                 */
                PHASE_OUT(ctx);
                RETURN(RC_OK);
            }

            /*
             * Fetch the T from TLV.
             */
            tag_len = ber_fetch_tag(ptr, LEFT, &tlv_tag);
            ASN_DEBUG("Current tag in %s SEQUENCE for element %" ASN_PRI_SIZE " "
                      "(%s) is %s encoded in %d bytes, of frame %ld",
                      td->name, edx, elements[edx].name,
                      ber_tlv_tag_string(tlv_tag), (int)tag_len, (long)LEFT);
            switch(tag_len) {
            case 0: if(!SIZE_VIOLATION) RETURN(RC_WMORE);
                /* Fall through */
            case -1: RETURN(RC_FAIL);
            }

            if(ctx->left < 0 && ((const uint8_t *)ptr)[0] == 0) {
                if(LEFT < 2) {
                    if(SIZE_VIOLATION) {
                        RETURN(RC_FAIL);
                    } else {
                        RETURN(RC_WMORE);
                    }
                } else if(((const uint8_t *)ptr)[1] == 0) {
                    ASN_DEBUG("edx = %" ASN_PRI_SIZE ", opt = %d, ec=%d", edx,
                              elements[edx].optional, td->elements_count);
                    if((edx + elements[edx].optional == td->elements_count)
                       || IN_EXTENSION_GROUP(specs, edx)) {
                        /*
                         * Yeah, baby! Found the terminator
                         * of the indefinite length structure.
                         */
                        /*
                         * Proceed to the canonical
                         * finalization function.
                         * No advancing is necessary.
                         */
                        goto phase3;
                    }
                }
            }

            /*
             * Find the next available type with this tag.
             */
            use_bsearch = 0;
            opt_edx_end = edx + elements[edx].optional + 1;
            if(opt_edx_end > td->elements_count)
                opt_edx_end = td->elements_count;  /* Cap */
            else if(opt_edx_end - edx > 8) {
                /* Limit the scope of linear search... */
                opt_edx_end = edx + 8;
                use_bsearch = 1;
                /* ... and resort to bsearch() */
            }
            for(n = edx; n < opt_edx_end; n++) {
                if(BER_TAGS_EQUAL(tlv_tag, elements[n].tag)) {
                    /*
                     * Found element corresponding to the tag
                     * being looked at.
                     * Reposition over the right element.
                     */
                    edx = n;
                    ctx->step = 1 + 2 * edx;  /* Remember! */
                    goto microphase2;
                } else if(elements[n].flags & ATF_ANY_TYPE) {
                    /*
                     * This is the ANY type, which may bear
                     * any flag whatsoever.
                     */
                    edx = n;
                    ctx->step = 1 + 2 * edx;  /* Remember! */
                    goto microphase2;
                } else if(elements[n].flags & ATF_OPEN_TYPE) {
                    /*
                     * This is the OPEN TYPE, which may bear
                     * any tag whatsoever.
                     */
                    edx = n;
                    ctx->step = 1 + 2 * edx;  /* Remember! */
                    goto microphase2;
                } else if(elements[n].tag == (ber_tlv_tag_t)-1) {
                    use_bsearch = 1;
                    break;
                }
            }
            if(use_bsearch) {
                /*
                 * Resort to a binary search over
                 * sorted array of tags.
                 */
                const asn_TYPE_tag2member_t *t2m;
                asn_TYPE_tag2member_t key = {0, 0, 0, 0};
                key.el_tag = tlv_tag;
                key.el_no = edx;
                t2m = (const asn_TYPE_tag2member_t *)bsearch(&key,
                       specs->tag2el, specs->tag2el_count,
                       sizeof(specs->tag2el[0]), _t2e_cmp);
                if(t2m) {
                    const asn_TYPE_tag2member_t *best = 0;
                    const asn_TYPE_tag2member_t *t2m_f, *t2m_l;
                    size_t edx_max = edx + elements[edx].optional;
                    /*
                     * Rewind to the first element with that tag,
                     * `cause bsearch() does not guarantee order.
                     */
                    t2m_f = t2m + t2m->toff_first;
                    t2m_l = t2m + t2m->toff_last;
                    for(t2m = t2m_f; t2m <= t2m_l; t2m++) {
                        if(t2m->el_no > edx_max) break;
                        if(t2m->el_no < edx) continue;
                        best = t2m;
                    }
                    if(best) {
                        edx = best->el_no;
                        ctx->step = 1 + 2 * edx;
                        goto microphase2;
                    }
                }
                n = opt_edx_end;
            }
            if(n == opt_edx_end) {
                /*
                 * If tag is unknown, it may be either
                 * an unknown (thus, incorrect) tag,
                 * or an extension (...),
                 * or an end of the indefinite-length structure.
                 */
                if(!IN_EXTENSION_GROUP(specs,
                    edx + elements[edx].optional)) {
                    ASN_DEBUG("Unexpected tag %s (at %" ASN_PRI_SIZE ")",
                              ber_tlv_tag_string(tlv_tag), edx);
                    ASN_DEBUG("Expected tag %s (%s)%s",
                              ber_tlv_tag_string(elements[edx].tag),
                              elements[edx].name,
                              elements[edx].optional ?" or alternatives":"");
                    RETURN(RC_FAIL);
                } else {
                    /* Skip this tag */
                    ssize_t skip;
                    edx += elements[edx].optional;

                    ASN_DEBUG("Skipping unexpected %s (at %" ASN_PRI_SIZE ")",
                              ber_tlv_tag_string(tlv_tag), edx);
                    skip = ber_skip_length(opt_codec_ctx,
                                           BER_TLV_CONSTRUCTED(ptr),
                                           (const char *)ptr + tag_len,
                                           LEFT - tag_len);
                    ASN_DEBUG("Skip length %d in %s",
                              (int)skip, td->name);
                    switch(skip) {
                    case 0: if(!SIZE_VIOLATION) RETURN(RC_WMORE);
                        /* Fall through */
                    case -1: RETURN(RC_FAIL);
                    }

                    ADVANCE(skip + tag_len);
                    ctx->step -= 2;
                    edx--;
                    continue;  /* Try again with the next tag */
                }
            }

            /*
             * MICROPHASE 2: Invoke the member-specific decoder.
             */
            ctx->step |= 1;  /* Confirm entering next microphase */
        microphase2:
            ASN_DEBUG("Inside SEQUENCE %s MF2", td->name);

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
            if(elements[edx].flags & ATF_OPEN_TYPE) {
                rval = OPEN_TYPE_ber_get(opt_codec_ctx, td, st, &elements[edx], ptr, LEFT);
            } else {
	            rval = elements[edx].type->op->ber_decoder(opt_codec_ctx, elements[edx].type,
	                                                       memb_ptr2, ptr, LEFT,
	                                                       elements[edx].tag_mode);
            }
            
            ASN_DEBUG("In %s SEQUENCE decoded %" ASN_PRI_SIZE " %s of %d "
                      "in %d bytes rval.code %d, size=%d",
                      td->name, edx, elements[edx].type->name,
                      (int)LEFT, (int)rval.consumed, rval.code, (int)size);
            switch(rval.code) {
            case RC_OK:
                break;
            case RC_WMORE: /* More data expected */
                if(!SIZE_VIOLATION) {
                    ADVANCE(rval.consumed);
                    RETURN(RC_WMORE);
                }
                ASN_DEBUG("Size violation (c->l=%ld <= s=%ld)",
                          (long)ctx->left, (long)size);
                /* Fall through */
            case RC_FAIL: /* Fatal error */
                RETURN(RC_FAIL);
        } /* switch(rval) */

        ADVANCE(rval.consumed);
    }  /* for(all structure members) */

    phase3:
        ctx->phase = 3;
        /* Fall through */
    case 3:  /* 00 and other tags expected */
    case 4:  /* only 00's expected */

        ASN_DEBUG("SEQUENCE %s Leftover: %ld, size = %ld",
                  td->name, (long)ctx->left, (long)size);

        /*
         * Skip everything until the end of the SEQUENCE.
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
            if(ctx->left < 0 && ((const uint8_t *)ptr)[0] == 0) {
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

            if(!IN_EXTENSION_GROUP(specs, td->elements_count)
            || ctx->phase == 4) {
                ASN_DEBUG("Unexpected continuation "
                          "of a non-extensible type "
                          "%s (SEQUENCE): %s",
                          td->name,
                          ber_tlv_tag_string(tlv_tag));
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

        PHASE_OUT(ctx);
    }

    RETURN(RC_OK);
}


/*
 * The DER encoder of the SEQUENCE type.
 */
asn_enc_rval_t
SEQUENCE_encode_der(const asn_TYPE_descriptor_t *td, const void *sptr,
                    int tag_mode, ber_tlv_tag_t tag,
                    asn_app_consume_bytes_f *cb, void *app_key) {
    size_t computed_size = 0;
    asn_enc_rval_t erval = {0,0,0};
    ssize_t ret;
    size_t edx;

    ASN_DEBUG("%s %s as SEQUENCE",
              cb?"Encoding":"Estimating", td->name);

    /* Check encoding recursion depth to prevent stack overflow */
    ASN__ENCODER_RECURSION_DEPTH_INC();

    /*
     * Gather the length of the underlying members sequence.
     */
    for(edx = 0; edx < td->elements_count; edx++) {
        asn_TYPE_member_t *elm = &td->elements[edx];

        const void *memb_ptr;          /* Pointer to the member */
        const void *const *memb_ptr2;  /* Pointer to that pointer */

        if(elm->flags & ATF_POINTER) {
            memb_ptr2 =
                (const void *const *)((const char *)sptr + elm->memb_offset);
            if(!*memb_ptr2) {
                ASN_DEBUG("Element %s %" ASN_PRI_SIZE " not present",
                          elm->name, edx);
                if(elm->optional)
                    continue;
                /* Mandatory element is missing */
                ASN__ENCODE_FAILED;
            }
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
            memb_ptr2 = &memb_ptr;
        }

        /* Eliminate default values */
        if(elm->default_value_cmp && elm->default_value_cmp(*memb_ptr2) == 0)
            continue;

        if(elm->flags & ATF_OPEN_TYPE) {
            erval = OPEN_TYPE_ber_put(td, sptr, elm,
                                      elm->tag_mode, elm->tag,
                                      0, 0); /* NULL callback for size estimation */
        } else {
	        erval = elm->type->op->der_encoder(elm->type, *memb_ptr2,
	                                           elm->tag_mode, elm->tag,
	                                           0, 0);  /* NULL callback for size estimation */
        }

        if(erval.encoded == -1)
            return erval;
        computed_size += erval.encoded;
        ASN_DEBUG("Member %" ASN_PRI_SIZE " %s estimated %ld bytes",
                  edx, elm->name, (long)erval.encoded);
    }

    /*
     * Encode the TLV for the sequence itself.
     */
    ret = der_write_tags(td, computed_size, tag_mode, 1, tag, cb, app_key);
    ASN_DEBUG("Wrote tags: %ld (+%ld)", (long)ret, (long)computed_size);
    if(ret == -1) {
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODE_FAILED;
    }
    erval.encoded = computed_size + ret;

    if(!cb) {
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODED_OK(erval);
    }

    /*
     * Encode all members.
     */
    for(edx = 0; edx < td->elements_count; edx++) {
        asn_TYPE_member_t *elm = &td->elements[edx];
        asn_enc_rval_t tmperval = {0,0,0};
        const void *memb_ptr;          /* Pointer to the member */
        const void *const *memb_ptr2;  /* Pointer to that pointer */

        if(elm->flags & ATF_POINTER) {
            memb_ptr2 =
                (const void *const *)((const char *)sptr + elm->memb_offset);
            if(!*memb_ptr2) continue;
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
            memb_ptr2 = &memb_ptr;
        }

        /* Eliminate default values */
        if(elm->default_value_cmp && elm->default_value_cmp(*memb_ptr2) == 0)
            continue;

        if(elm->flags & ATF_OPEN_TYPE) {
            tmperval = OPEN_TYPE_ber_put(td, sptr, elm,
                                         elm->tag_mode, elm->tag, cb, app_key);
        } else {
            tmperval = elm->type->op->der_encoder(elm->type, *memb_ptr2,
                                                  elm->tag_mode, elm->tag, cb, app_key);
        }

       
        if(tmperval.encoded == -1) {
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            return tmperval;
        }

        if(computed_size < (size_t)tmperval.encoded) {
	        /* This should never happen if estimation and encoding are consistent */
	        ASN_DEBUG("Size mismatch: computed_size=%zu < tmperval.encoded=%zd for element %s",
	                  computed_size, tmperval.encoded, elm->name);
	        ASN__ENCODER_RECURSION_DEPTH_DEC();
	        ASN__ENCODE_FAILED;
        }
        
        computed_size -= tmperval.encoded;
        ASN_DEBUG("Member %" ASN_PRI_SIZE " %s of SEQUENCE %s encoded in %ld bytes",
                  edx, elm->name, td->name, (long)tmperval.encoded);
    }

    if(computed_size != 0) {
        /*
         * Encoded size is not equal to the computed size.
         */
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        ASN__ENCODE_FAILED;
    }

    ASN__ENCODER_RECURSION_DEPTH_DEC();
    ASN__ENCODED_OK(erval);
}
