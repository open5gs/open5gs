/*-
 * Copyright (c) 2006-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_PER_ENCODER_H_
#define	_PER_ENCODER_H_

#include <asn_application.h>
#include <per_support.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;	/* Forward declaration */

/*
 * Type of the generic PER encoder function.
 */
typedef asn_enc_rval_t(per_type_encoder_f)(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints, const void *struct_ptr,
    asn_per_outp_t *per_output);

int ignore_output(const void *data, size_t size, void *app_key);

typedef struct enc_dyn_arg {
    void *buffer;
    size_t length;
    size_t allocated;
} enc_dyn_arg;
int encode_dyn_cb(const void *buffer, size_t size, void *key);

#ifdef __cplusplus
}
#endif

#endif	/* _PER_ENCODER_H_ */
