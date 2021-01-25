/*
 * Copyright (c) 2005-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_APER_SUPPORT_H_
#define	_APER_SUPPORT_H_

#include <asn_system.h>		/* Platform-specific types */
#include <per_support.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * X.691 (08/2015) #11.9 "General rules for encoding a length determinant"
 * Get the length "n" from the Aligned PER stream.
 */
ssize_t aper_get_length(asn_per_data_t *pd, int range,
                        int effective_bound_bits, int *repeat);

/*
 * Get the normally small length "n".
 */
ssize_t aper_get_nslength(asn_per_data_t *pd);

/*
 * Get the normally small non-negative whole number.
 */
ssize_t aper_get_nsnnwn(asn_per_data_t *pd, int range);

/*
 * X.691 (08/2015) #11.9 "General rules for encoding a length determinant"
 * Put the length "whole_length" to the Aligned PER stream.
 * If (opt_need_eom) is given, it will be set to 1 if final 0-length is needed.
 * In that case, invoke uper_put_length(po, 0, 0) after encoding the last block.
 * This function returns the number of units which may be flushed
 * in the next units saving iteration.
 */
ssize_t aper_put_length(asn_per_outp_t *po, int range, size_t length,
                        int *opt_need_eom);

/* Align the current bit position to octet bundary */
int aper_put_align(asn_per_outp_t *po);
int32_t aper_get_align(asn_per_data_t *pd);

/*
 * Put the normally small length "n" to the Unaligned PER stream.
 * Returns 0 or -1.
 */
int aper_put_nslength(asn_per_outp_t *po, size_t length);

/*
 * Put the normally small non-negative whole number.
 */
int aper_put_nsnnwn(asn_per_outp_t *po, int range, int number);

#ifdef __cplusplus
}
#endif

#endif	/* _APER_SUPPORT_H_ */
