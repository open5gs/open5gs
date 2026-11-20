/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE_OF.h>
#include <asn_SEQUENCE_OF.h>

/*
 * The DER encoder of the SEQUENCE OF type.
 */
asn_enc_rval_t
SEQUENCE_OF_encode_der(const asn_TYPE_descriptor_t *td, const void *sptr,
                       int tag_mode, ber_tlv_tag_t tag,
                       asn_app_consume_bytes_f *cb, void *app_key) {
    asn_TYPE_member_t *elm = td->elements;
    const asn_anonymous_sequence_ *list = _A_CSEQUENCE_FROM_VOID(sptr);
    size_t computed_size = 0;
    ssize_t encoding_size = 0;
    asn_enc_rval_t erval = {0,0,0};
    int edx;

    ASN_DEBUG("Estimating size of SEQUENCE OF %s", td->name);

    /* Check encoding recursion depth to prevent stack overflow */
    ASN__ENCODER_RECURSION_DEPTH_INC();

    /*
     * Gather the length of the underlying members sequence.
     */
    if(!list) {
	    ASN_DEBUG("SEQUENCE OF list is null");
	    ASN__ENCODER_RECURSION_DEPTH_DEC();
	    ASN__ENCODE_FAILED;
    }

    if((!list->array) && (list->count != 0)) {
	    ASN_DEBUG("SEQUENCE OF list->array is null");
	    ASN__ENCODER_RECURSION_DEPTH_DEC();
	    ASN__ENCODE_FAILED;
    }

    if(list->count > 1000000) { // Arbitrary sanity check for count
	    ASN_DEBUG("SEQUENCE OF list->array (list->count) is invalid or count is nonsensical");
	    ASN__ENCODER_RECURSION_DEPTH_DEC();
	    ASN__ENCODE_FAILED;
    }

    ASN_DEBUG("SEQUENCE OF list pointer: %p, array pointer: %p, count: %d",
              list, list->array, list->count);

    for(edx = 0; edx < list->count; edx++) {
	    void *memb_ptr = list->array[edx];
	    if(!memb_ptr) {
		    ASN_DEBUG("SEQUENCE OF member pointer is null at index %d", edx);
		    continue;
	    }
	    erval = elm->type->op->der_encoder(elm->type, memb_ptr,
	                                       elm->tag_mode, elm->tag,
	                                       0, 0);
	    if(erval.encoded == -1) {
		    ASN__ENCODER_RECURSION_DEPTH_DEC();
		    return erval;
	    }
	    computed_size += erval.encoded;
    }

    /*
     * Encode the TLV for the sequence itself.
     */
    encoding_size = der_write_tags(td, computed_size, tag_mode, 1, tag,
                                   cb, app_key);
    if(encoding_size == -1) {
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        erval.encoded = -1;
        erval.failed_type = td;
        erval.structure_ptr = sptr;
        return erval;
    }

    computed_size += encoding_size;
    if(!cb) {
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        erval.encoded = computed_size;
        ASN__ENCODED_OK(erval);
    }

    ASN_DEBUG("Encoding members of SEQUENCE OF %s", td->name);

    /*
     * Encode all members.
     */
    for(edx = 0; edx < list->count; edx++) {
        void *memb_ptr = list->array[edx];
        if(!memb_ptr) continue;
        erval = elm->type->op->der_encoder(elm->type, memb_ptr,
                                           elm->tag_mode, elm->tag,
                                           cb, app_key);
        if(erval.encoded == -1) {
            ASN__ENCODER_RECURSION_DEPTH_DEC();
            return erval;
        }
        encoding_size += erval.encoded;
    }

    if(computed_size != (size_t)encoding_size) {
        /*
         * Encoded size is not equal to the computed size.
         */
        ASN__ENCODER_RECURSION_DEPTH_DEC();
        erval.encoded = -1;
        erval.failed_type = td;
        erval.structure_ptr = sptr;
    } else {
        erval.encoded = computed_size;
        erval.structure_ptr = 0;
        erval.failed_type = 0;
    }

    ASN__ENCODER_RECURSION_DEPTH_DEC();
    return erval;
}
