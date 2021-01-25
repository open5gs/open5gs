/*-
 * Copyright (c) 2006-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_APER_ENCODER_H_
#define	_APER_ENCODER_H_

#include <asn_application.h>
#include <aper_support.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;	/* Forward declaration */

/*
 * Aligned PER encoder of any ASN.1 type. May be invoked by the application.
 * WARNING: This function returns the number of encoded bits in the .encoded
 * field of the return value. Use the following formula to convert to bytes:
 * 	bytes = ((.encoded + 7) / 8)
 */
asn_enc_rval_t aper_encode(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints,
    const void *struct_ptr,                     /* Structure to be encoded */
    asn_app_consume_bytes_f *consume_bytes_cb,  /* Data collector */
    void *app_key                               /* Arbitrary callback argument */
);

/*
 * A variant of aper_encode() which encodes data into the existing buffer
 * WARNING: This function returns the number of encoded bits in the .encoded
 * field of the return value.
 */
asn_enc_rval_t aper_encode_to_buffer(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints,
    const void *struct_ptr,  /* Structure to be encoded */
    void *buffer,            /* Pre-allocated buffer */
    size_t buffer_size       /* Initial buffer size (max) */
);

/*
 * A variant of aper_encode_to_buffer() which allocates buffer itself.
 * Returns the number of bytes in the buffer or -1 in case of failure.
 * WARNING: This function produces a "Production of the complete encoding",
 * with length of at least one octet. Contrast this to precise bit-packing
 * encoding of aper_encode() and aper_encode_to_buffer().
 */
ssize_t
aper_encode_to_new_buffer(
    const struct asn_TYPE_descriptor_s *td,
    const asn_per_constraints_t *constraints,
    const void *sptr,
    void **buffer_r
);

#ifdef __cplusplus
}
#endif

#endif	/* _APER_ENCODER_H_ */
