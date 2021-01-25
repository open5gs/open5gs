/*
 * Copyright (c) 2007-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_PER_OPENTYPE_H_
#define	_PER_OPENTYPE_H_

#include <asn_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

asn_dec_rval_t uper_sot_suck(
        const asn_codec_ctx_t *,
        const asn_TYPE_descriptor_t *td,
        const asn_per_constraints_t *constraints,
        void **sptr, asn_per_data_t *pd);

#ifdef __cplusplus
}
#endif

#endif	/* _PER_OPENTYPE_H_ */
