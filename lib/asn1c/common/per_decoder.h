/*-
 * Copyright (c) 2005-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_PER_DECODER_H_
#define	_PER_DECODER_H_

#include <asn_application.h>
#include <per_support.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;	/* Forward declaration */

/*
 * Type of the type-specific PER decoder function.
 */
typedef asn_dec_rval_t(per_type_decoder_f)(
    const asn_codec_ctx_t *opt_codec_ctx,
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints, void **struct_ptr,
    asn_per_data_t *per_data);

#ifdef __cplusplus
}
#endif

#endif	/* _PER_DECODER_H_ */
