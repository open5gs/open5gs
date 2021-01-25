/*-
 * Copyright (c) 2003, 2004 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <asn_codecs_prim.h>

void
ASN__PRIMITIVE_TYPE_free(const asn_TYPE_descriptor_t *td, void *sptr,
                         enum asn_struct_free_method method) {
    ASN__PRIMITIVE_TYPE_t *st = (ASN__PRIMITIVE_TYPE_t *)sptr;

	if(!td || !sptr)
		return;

	ASN_DEBUG("Freeing %s as a primitive type", td->name);

	if(st->buf)
		FREEMEM(st->buf);

    switch(method) {
    case ASFM_FREE_EVERYTHING:
        FREEMEM(sptr);
        break;
    case ASFM_FREE_UNDERLYING:
        break;
    case ASFM_FREE_UNDERLYING_AND_RESET:
        memset(sptr, 0, sizeof(ASN__PRIMITIVE_TYPE_t));
        break;
    }
}
