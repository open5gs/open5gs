/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <aper_opentype.h>

asn_dec_rval_t
OPEN_TYPE_aper_get(const asn_codec_ctx_t *opt_codec_ctx,
                   const asn_TYPE_descriptor_t *td, void *sptr,
                   const asn_TYPE_member_t *elm, asn_per_data_t *pd) {
    asn_type_selector_result_t selected;
    void *memb_ptr;   /* Pointer to the member */
    void **memb_ptr2; /* Pointer to that pointer */
    void *inner_value;
    asn_dec_rval_t rv;

    if(!(elm->flags & ATF_OPEN_TYPE)) {
        ASN__DECODE_FAILED;
    }

    /* Validate elm->type before accessing its members */
    if(!elm->type) {
        ASN_DEBUG("Open Type %s->%s: type descriptor is NULL",
                  td->name, elm->name);
        ASN__DECODE_FAILED;
    }

    if(!elm->type_selector) {
        ASN_DEBUG("Type selector is not defined for Open Type %s->%s->%s",
                  td->name, elm->name, elm->type->name);
        ASN__DECODE_FAILED;
    }

    selected = elm->type_selector(td, sptr);
    if(!selected.presence_index) {
        ASN__DECODE_FAILED;
    }

    ASN_DEBUG("OPEN_TYPE_aper_get: elm->type=%s, elements=%p, elements_count=%u, selected.presence_index=%u, selected.type=%s",
              elm->type->name, (void*)elm->type->elements, elm->type->elements_count,
              selected.presence_index, selected.type_descriptor->name);

    /* Fetch the pointer to this member */
    assert(elm->flags & ATF_OPEN_TYPE);
    if(elm->flags & ATF_POINTER) {
        memb_ptr2 = (void **)((char *)sptr + elm->memb_offset);
    } else {
        memb_ptr = (char *)sptr + elm->memb_offset;
        memb_ptr2 = &memb_ptr;
    }

    /* Check if this OPEN_TYPE uses CHOICE wrapper (elements_count > 0) or direct type */
    if(elm->type->elements_count > 0) {
        /* CHOICE wrapper mode: validate and allocate CHOICE structure */
        
        /* Validate the selected variant */
        if(selected.presence_index > elm->type->elements_count) {
            ASN_DEBUG("Open Type %s->%s: presence index %u out of bounds (max %u)",
                      td->name, elm->name, selected.presence_index,
                      elm->type->elements_count);
            ASN__DECODE_FAILED;
        }
        
        /* Ensure we can access the elements array if needed */
        if(!elm->type->elements) {
            ASN_DEBUG("Open Type %s->%s: elements array is NULL but elements_count is %u",
                      td->name, elm->name, elm->type->elements_count);
            ASN__DECODE_FAILED;
        }

        /* Allocate the CHOICE structure if not already present */
        if(*memb_ptr2 == NULL) {
            const asn_CHOICE_specifics_t *specs = 
                (const asn_CHOICE_specifics_t *)elm->type->specifics;
            if(!specs) {
                ASN_DEBUG("Open Type %s->%s: type specifics is NULL",
                          td->name, elm->name);
                ASN__DECODE_FAILED;
            }
            *memb_ptr2 = CALLOC(1, specs->struct_size);
            if(*memb_ptr2 == NULL) {
                ASN__DECODE_FAILED;
            }
        } else {
            /* Make sure we reset the structure first before decoding */
            if(CHOICE_variant_set_presence(elm->type, *memb_ptr2, 0)
               != 0) {
                ASN__DECODE_FAILED;
            }
        }
    } else {
        /* Direct type mode: no CHOICE wrapper, decode directly into member */
        ASN_DEBUG("Open Type %s->%s: using direct type mode (no CHOICE wrapper)",
                  td->name, elm->name);
    }

    /* Compute target pointer and constraints based on CHOICE wrapper mode or direct type mode */
    unsigned int memb_offset = 0;
    const asn_per_constraints_t *constraints = NULL;
    const asn_TYPE_member_t *variant_elm = NULL;
    
    if(elm->type->elements_count > 0) {
        /* CHOICE wrapper mode: get variant element info */
        if(elm->type->elements && selected.presence_index > 0 
           && selected.presence_index <= elm->type->elements_count) {
            variant_elm = &elm->type->elements[selected.presence_index - 1];
            memb_offset = variant_elm->memb_offset;
            constraints = variant_elm->encoding_constraints.per_constraints;
        }
        
        /*
         * For ATF_POINTER variants (e.g., "PersonInfo *PersonInfo" in CHOICE):
         *   - The field is a pointer itself, freshly CALLOC'd to NULL
         *   - We need to read the pointer value (NULL) from the field
         *   - Decoder will allocate structure and update inner_value
         *   - We'll copy inner_value back to the field after decoding
         * 
         * For non-pointer variants (e.g., "int value" in CHOICE):
         *   - The field is embedded in the CHOICE structure
         *   - We pass the address of the field to the decoder
         *   - Decoder writes directly into the field
         */
        if(variant_elm && (variant_elm->flags & ATF_POINTER)) {
            /* Read the current pointer value from the field */
            inner_value = *(void **)((char *)*memb_ptr2 + memb_offset);
        } else {
            /* Compute address of the embedded value field */
            inner_value = (char *)*memb_ptr2 + memb_offset;
        }
    } else {
        /* Direct type mode: decode directly into the member pointer */
        inner_value = *memb_ptr2;
    }

    rv = aper_open_type_get(opt_codec_ctx, selected.type_descriptor,
                            constraints, &inner_value, pd);
    ASN_DEBUG("aper_open_type_get returned code=%d for %s", rv.code, selected.type_descriptor->name);
    switch(rv.code) {
    case RC_OK:
        if(elm->type->elements_count > 0) {
            /* Set presence indicator FIRST, before copying pointer */
            ASN_DEBUG("Calling CHOICE_variant_set_presence(elm->type=%s, presence_index=%u, elements_count=%u)",
                      elm->type->name, selected.presence_index, elm->type->elements_count);
            if(CHOICE_variant_set_presence(elm->type, *memb_ptr2,
                                           selected.presence_index)
               == 0) {
                /* CHOICE wrapper mode: for pointer variants, copy decoded pointer back to field */
                if(variant_elm && (variant_elm->flags & ATF_POINTER)) {
                    /*
                     * The decoder allocated a structure and stored pointer in inner_value.
                     * Copy it back to the actual field in the CHOICE structure.
                     */
                    void **variant_ptr = (void **)((char *)*memb_ptr2 + memb_offset);
                    *variant_ptr = inner_value;
                }
                ASN_DEBUG("CHOICE_variant_set_presence succeeded");
                break;
            } else {
                ASN_DEBUG("CHOICE_variant_set_presence FAILED");
                rv.code = RC_FAIL;
            }
        } else {
            /* Direct type mode: update member pointer with decoded value if pointer type */
            if(elm->flags & ATF_POINTER) {
                ASN_DEBUG("Direct type mode: updating member pointer");
                *memb_ptr2 = inner_value;
            } else {
                ASN_DEBUG("Direct type mode: decode successful (non-pointer)");
            }
            break;
        }
        /* Fall through */
    case RC_WMORE:
    case RC_FAIL:
        ASN_DEBUG("Cleaning up after failure, code=%d", rv.code);
        if(*memb_ptr2) {
            if(elm->flags & ATF_POINTER) {
                ASN_STRUCT_FREE(*selected.type_descriptor, inner_value);
                *memb_ptr2 = NULL;
            } else {
                ASN_STRUCT_RESET(*selected.type_descriptor,
                                              inner_value);
            }
        }
    }
    return rv;
}

asn_enc_rval_t
OPEN_TYPE_encode_aper(const asn_TYPE_descriptor_t *td,
                      const asn_per_constraints_t *constraints,
                      const void *sptr, asn_per_outp_t *po) {
    const void *memb_ptr;   /* Pointer to the member */
    asn_TYPE_member_t *elm; /* CHOICE's element */
    asn_enc_rval_t er = {0,0,0};
    unsigned present;

    (void)constraints;

    if(!sptr)
        ASN__ENCODE_FAILED;

    if(td->elements_count == 0) {
        /* Direct type mode: no CHOICE wrapper, encode directly */
        ASN_DEBUG("Encoding %s OPEN TYPE in direct type mode", td->name);
        
        /* In direct type mode, sptr points directly to the value to encode */
        /* We can't encode it because we don't know the actual type descriptor */
        /* This should not happen - the parent should handle this case */
        ASN_DEBUG("ERROR: OPEN_TYPE_encode_aper called in direct type mode");
        ASN__ENCODE_FAILED;
    }

    /* CHOICE wrapper mode: use presence indicator */
    present = CHOICE_variant_get_presence(td, sptr);
    if(present == 0 || present > td->elements_count) {
        ASN__ENCODE_FAILED;
    } else {
        present--;
    }

    ASN_DEBUG("Encoding %s OPEN TYPE element %d", td->name, present);

    elm = &td->elements[present];
    if(elm->flags & ATF_POINTER) {
        /* Member is a pointer to another structure */
        memb_ptr =
            *(const void *const *)((const char *)sptr + elm->memb_offset);
        if(!memb_ptr) ASN__ENCODE_FAILED;
    } else {
        memb_ptr = (const char *)sptr + elm->memb_offset;
    }

    if(aper_open_type_put(elm->type, elm->encoding_constraints.per_constraints, memb_ptr, po) < 0) {
        ASN__ENCODE_FAILED;
    }

    er.encoded = 0;
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t
OPEN_TYPE_aper_put(const asn_TYPE_descriptor_t *td, const void *sptr,
                   const asn_TYPE_member_t *elm, asn_per_outp_t *po) {
    asn_type_selector_result_t selected;
    const void *memb_ptr;
    asn_enc_rval_t er = {0,0,0};

    if(!(elm->flags & ATF_OPEN_TYPE)) {
        ASN__ENCODE_FAILED;
    }

    /* Validate elm->type before accessing its members */
    if(!elm->type) {
        ASN_DEBUG("Open Type %s->%s: type descriptor is NULL",
                  td->name, elm->name);
        ASN__ENCODE_FAILED;
    }

    if(!elm->type->op || !elm->type->op->aper_encoder) {
        ASN_DEBUG("Open Type %s->%s: APER encoder is not defined",
                  td->name, elm->name);
        ASN__ENCODE_FAILED;
    }

    if(!elm->type_selector) {
        ASN_DEBUG("Type selector is not defined for Open Type %s->%s->%s",
                  td->name, elm->name, elm->type->name);
        ASN__ENCODE_FAILED;
    }

    selected = elm->type_selector(td, sptr);
    if(!selected.presence_index) {
        ASN__ENCODE_FAILED;
    }

    ASN_DEBUG("OPEN_TYPE_aper_put: elm->type=%s, elements=%p, elements_count=%u, selected.presence_index=%u, selected.type=%s",
              elm->type->name, (void*)elm->type->elements, elm->type->elements_count,
              selected.presence_index, selected.type_descriptor->name);

    /* Fetch the pointer to this member */
    assert(elm->flags & ATF_OPEN_TYPE);
    if(elm->flags & ATF_POINTER) {
        memb_ptr = *(const void *const *)((const char *)sptr + elm->memb_offset);
        if(!memb_ptr) ASN__ENCODE_FAILED;
    } else {
        memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
    }

    /* Check if this OPEN_TYPE uses CHOICE wrapper (elements_count > 0) or direct type */
    if(elm->type->elements_count > 0) {
        /* 
         * CHOICE wrapper mode: encode the CHOICE directly.
         * When elements_count > 0, the CHOICE structure is already selected and
         * populated by the type selector, so we encode it directly using its native
         * APER encoder. The encoder will handle the choice index and variant encoding
         * according to its own constraints (constrained vs. extension variants).
         * Wrapping it again with open type framing would be redundant and incorrect.
         */
        er = elm->type->op->aper_encoder(elm->type, 
                                         elm->encoding_constraints.per_constraints,
                                         memb_ptr, po);
        if(er.encoded == -1) {
            ASN__ENCODE_FAILED;
        }
        ASN__ENCODED_OK(er);
    } else {
        /* 
         * Direct type mode: encode using the selected type descriptor.
         * Since this is direct type without CHOICE wrapper, we need the 
         * open type length determinant wrapper.
         */
        ASN_DEBUG("Direct type mode: encoding using %s wrapped in OPEN TYPE", selected.type_descriptor->name);
        /* Use NULL constraints for direct type mode to match decoder behavior */
        if(aper_open_type_put(selected.type_descriptor, NULL, memb_ptr, po) < 0) {
            ASN__ENCODE_FAILED;
        }
        er.encoded = 0;
        ASN__ENCODED_OK(er);
    }
}


int OPEN_TYPE_aper_is_unknown_type(const asn_TYPE_descriptor_t *td, void *sptr, const asn_TYPE_member_t *elm) {
    asn_type_selector_result_t selected;

    if(!elm->type_selector) {
        return 1;
    }
    else {
        selected = elm->type_selector(td, sptr);
        if(!selected.presence_index) {
            return 1;
        }
    }
    return 0;
}

asn_dec_rval_t
OPEN_TYPE_aper_unknown_type_discard_bytes (asn_per_data_t *pd) {
#define ASN_DUMMY_BYTES 256
    unsigned char dummy[ASN_DUMMY_BYTES], *dummy_ptr = NULL;
    ssize_t bytes;
    int repeat;
    asn_dec_rval_t rv;
    size_t initial_consumed = pd->moved;  /* Track initial position */

    rv.consumed = 0;
    rv.code = RC_FAIL;

    do {
        bytes = aper_get_length(pd, -1, -1, -1, &repeat);
        if (bytes < 0) {
            /* Invalid length - return error */
            return rv;
        }
        if (bytes > 10 * ASN_DUMMY_BYTES)
        {
            return rv;
        }
        else if (bytes > ASN_DUMMY_BYTES)
        {
            dummy_ptr = CALLOC(1, bytes);
            if (!dummy_ptr)
                return rv;
        }

        if (per_get_many_bits(pd, (dummy_ptr ? dummy_ptr : dummy), 0, bytes << 3) < 0) {
            /* Error during bit consumption */
            if (dummy_ptr) {
                FREEMEM(dummy_ptr);
            }
            return rv;
        }

        if (dummy_ptr)
        {
            FREEMEM(dummy_ptr);
            dummy_ptr = NULL;
        }
    } while (repeat);

    /* Update consumed to reflect actual bits consumed */
    rv.consumed = pd->moved - initial_consumed;
    rv.code = RC_OK;
    return rv;
#undef ASN_DUMMY_BYTES
}
