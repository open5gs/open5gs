/*
 * Copyright (c) 2005-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_system.h>
#include <asn_internal.h>
#include <aper_support.h>

int32_t
aper_get_align(asn_per_data_t *pd) {

	if(pd->nboff & 0x7) {
		ASN_DEBUG("Aligning %ld bits", 8 - ((unsigned long)pd->nboff & 0x7));
		return per_get_few_bits(pd, 8 - (pd->nboff & 0x7));
	}
	return 0;
}

ssize_t
aper_get_length(asn_per_data_t *pd, ssize_t lb, ssize_t ub,
		int ebits, int *repeat) {
	int constrained = (lb >= 0) && (ub >= 0);
	ssize_t value;

	*repeat = 0;

	if (constrained && ub < 65536) {
		int range = ub - lb + 1;
		return aper_get_nsnnwn(pd, range);
	}

	if (aper_get_align(pd) < 0)
		return -1;

	if(ebits >= 0) return per_get_few_bits(pd, ebits);

	value = per_get_few_bits(pd, 8);
	if(value < 0) return -1;
	if((value & 128) == 0)  /* #11.9.3.6 */
		return (value & 0x7F);
	if((value & 64) == 0) { /* #11.9.3.7 */
		value = ((value & 63) << 8) | per_get_few_bits(pd, 8);
		if(value < 0) return -1;
		return value;
	}
	value &= 63;	/* this is "m" from X.691, #11.9.3.8 */
	if(value < 1 || value > 4)
		return -1;
	*repeat = 1;
	return (16384 * value);
}

ssize_t
aper_get_nslength(asn_per_data_t *pd) {
	ssize_t length;

	ASN_DEBUG("Getting normally small length");

	if(per_get_few_bits(pd, 1) == 0) {
		length = per_get_few_bits(pd, 6) + 1;
		if(length <= 0) return -1;
		ASN_DEBUG("l=%zd", length);
		return length;
	} else {
		int repeat;
		length = aper_get_length(pd, -1, -1, -1, &repeat);
		if(length >= 0 && !repeat) return length;
		return -1; /* Error, or do not support >16K extensions */
	}
}

ssize_t
aper_get_nsnnwn(asn_per_data_t *pd, int range) {
	ssize_t value;
	int bytes = 0;

	ASN_DEBUG("getting nsnnwn with range %d", range);

	if(range <= 255) {
		int i;

		if (range < 0) return -1;
		/* 1 -> 8 bits */
		for (i = 1; i <= 8; i++) {
			int upper = 1 << i;
			if (upper >= range)
				break;
		}
		value = per_get_few_bits(pd, i);
		return value;
	} else if (range == 256){
		/* 1 byte */
		bytes = 1;
	} else if (range <= 65536) {
		/* 2 bytes */
		bytes = 2;
	} else {
		//return -1;
		int length;

		/* handle indefinite range */
		length = per_get_few_bits(pd, 1);
		if (length == 0)
			return per_get_few_bits(pd, 6);

		if (aper_get_align(pd) < 0)
			return -1;

		length = per_get_few_bits(pd, 8);
		/* the length is not likely to be that big */
		if (length > 4)
			return -1;
		value = 0;
		if (per_get_many_bits(pd, (uint8_t *)&value, 0, length * 8) < 0)
			return -1;
		return value;
	}
	if (aper_get_align(pd) < 0)
		return -1;
	value = per_get_few_bits(pd, 8 * bytes);
	return value;
}

int aper_put_align(asn_per_outp_t *po) {

	if(po->nboff & 0x7) {
		ASN_DEBUG("Aligning %ld bits", 8 - ((unsigned long)po->nboff & 0x7));
		if(per_put_few_bits(po, 0x00, (8 - (po->nboff & 0x7))))
			return -1;
	}
	return 0;
}

ssize_t
aper_put_length(asn_per_outp_t *po, ssize_t lb, ssize_t ub, size_t n, int *need_eom) {
	int constrained = (lb >= 0) && (ub >= 0);
	int dummy = 0;
	if(!need_eom) need_eom = &dummy;

	*need_eom = 0;


	ASN_DEBUG("APER put length %zu with range (%zd..%zd)", n, lb, ub);

	/* 11.9 X.691 Note 2 */
	if (constrained && ub < 65536) {
		int range = ub - lb + 1;
		return aper_put_nsnnwn(po, range, n) ? -1 : (ssize_t)n;
	}

	if (aper_put_align(po) < 0)
		return -1;

	if(n <= 127) { /* #11.9.3.6 */
		return per_put_few_bits(po, n, 8)
		? -1 : (ssize_t)n;
	}
	else if(n < 16384) /* #11.9.3.7 */
		return per_put_few_bits(po, n|0x8000, 16)
		? -1 : (ssize_t)n;

	*need_eom = 0 == (n & 16383);
	n >>= 14;
	if(n > 4) {
		*need_eom = 0;
		n = 4;
	}

	return per_put_few_bits(po, 0xC0 | n, 8)
	? -1 : (ssize_t)(n << 14);
}


int
aper_put_nslength(asn_per_outp_t *po, size_t length) {

	if(length <= 64) {
		/* #11.9.3.4 */
		if(length == 0) return -1;
		return per_put_few_bits(po, length-1, 7) ? -1 : 0;
	} else {
		if(aper_put_length(po, -1, -1, length, NULL) != (ssize_t)length) {
			/* This might happen in case of >16K extensions */
			return -1;
		}
	}

	return 0;
}

int
aper_put_nsnnwn(asn_per_outp_t *po, int range, int number) {
	int bytes;

	ASN_DEBUG("aper put nsnnwn %d with range %d", number, range);
	/* 10.5.7.1 X.691 */
	if(range < 0) {
		int i;
		for (i = 1; ; i++) {
			int bits = 1 << (8 * i);
			if (number <= bits)
				break;
		}
		bytes = i;
		assert(i <= 4);
	}
	if(range <= 255) {
		int i;
		for (i = 1; i <= 8; i++) {
			int bits = 1 << i;
			if (range <= bits)
				break;
		}
		return per_put_few_bits(po, number, i);
	} else if(range == 256) {
		if (number >= range)
			return -1;
		bytes = 1;
	} else if(range <= 65536) {
		if (number >= range)
			return -1;
		bytes = 2;
	} else { /* Ranges > 64K */
		int i;
		for (i = 1; ; i++) {
			int bits = 1 << (8 * i);
			if (range <= bits)
				break;
		}
		assert(i <= 4);
		bytes = i;
	}
	if(aper_put_align(po) < 0) /* Aligning on octet */
		return -1;
/* 	if(per_put_few_bits(po, bytes, 8))
		return -1;
*/
	return per_put_few_bits(po, number, 8 * bytes);
}
