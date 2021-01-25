/*
 * Copyright (c) 2005-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_PER_SUPPORT_H_
#define	_PER_SUPPORT_H_

#include <asn_system.h>		/* Platform-specific types */
#include <asn_bit_data.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Pre-computed PER constraints.
 */
typedef struct asn_per_constraint_s {
	enum asn_per_constraint_flags {
		APC_UNCONSTRAINED	= 0x0,	/* No PER visible constraints */
		APC_SEMI_CONSTRAINED	= 0x1,	/* Constrained at "lb" */
		APC_CONSTRAINED		= 0x2,	/* Fully constrained */
		APC_EXTENSIBLE		= 0x4	/* May have extension */
	} flags;
	int  range_bits;		/* Full number of bits in the range */
	int  effective_bits;		/* Effective bits */
	intmax_t lower_bound;		/* "lb" value */
	intmax_t upper_bound;		/* "ub" value */
} asn_per_constraint_t;
typedef struct asn_per_constraints_s {
	asn_per_constraint_t value;
	asn_per_constraint_t size;
	int (*value2code)(unsigned int value);
	int (*code2value)(unsigned int code);
} asn_per_constraints_t;

/* Temporary compatibility layer. Will get removed. */
typedef struct asn_bit_data_s asn_per_data_t;
#define per_get_few_bits(data, bits)   asn_get_few_bits(data, bits)
#define per_get_undo(data, bits)   asn_get_undo(data, bits)
#define per_get_many_bits(data, dst, align, bits) \
    asn_get_many_bits(data, dst, align, bits)

/* Temporary compatibility layer. Will get removed. */
typedef struct asn_bit_outp_s asn_per_outp_t;
#define per_put_few_bits(out, bits, obits) asn_put_few_bits(out, bits, obits)
#define per_put_many_bits(out, src, nbits) asn_put_many_bits(out, src, nbits)
#define per_put_aligned_flush(out) asn_put_aligned_flush(out)

#ifdef __cplusplus
}
#endif

#endif	/* _PER_SUPPORT_H_ */
