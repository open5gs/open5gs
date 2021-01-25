/*
 * Copyright (c) 2007 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <per_opentype.h>

/*
 * Internal functions.
 */

asn_dec_rval_t
uper_sot_suck(const asn_codec_ctx_t *ctx, const asn_TYPE_descriptor_t *td,
              const asn_per_constraints_t *constraints, void **sptr,
              asn_per_data_t *pd) {
    asn_dec_rval_t rv;

	(void)ctx;
	(void)td;
	(void)constraints;
	(void)sptr;

	while(per_get_few_bits(pd, 1) >= 0);

	rv.code = RC_OK;
	rv.consumed = pd->moved;

	return rv;
}
