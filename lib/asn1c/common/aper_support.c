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
		return aper_get_constrained_whole_number(pd, lb, ub);
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
aper_get_nsnnwn(asn_per_data_t *pd) {
	int b;
	int length;

	ASN_DEBUG("getting nsnnwn");

	b = per_get_few_bits(pd, 1);
	if (b == -1)
		return -1;

	/* X.691 2002 10.6.1 */
	if (b == 0)
		return per_get_few_bits(pd, 6);

	if (aper_get_align(pd) < 0)
		return -1;

	/* X.691 2002 10.6.2 */
	/* X.691 2002 10.9.3.5 */
	b = per_get_few_bits(pd, 1);
	if (b == -1)
		return -1;

	if (b == 1) {
		/* other 10.9.3.x cases not handled, it's doubtful we reach them in practice */
		ASN_DEBUG("todo: X.691 2002 10.9.3.x");
		return -1;
	}

	/* X.691 2002 10.9.3.6 */
	length = per_get_few_bits(pd, 7);
	if (length > 4) {
		/* todo */
		ASN_DEBUG("todo: X.691 2002 10.9.3.6 for length > 4");
		return -1;
	}
	ASN_DEBUG("length %d\n", length);

	/* todo: 0xffffffff will be seen as -1 and will lead to decoding failure */
	return per_get_few_bits(pd, length * 8);
}

/* X.691 2002 10.5 - Decoding of a constrained whole number */
long
aper_get_constrained_whole_number(asn_per_data_t *pd, long lb, long ub) {
	assert(ub >= lb);
	long range = ub - lb + 1;
	int range_len;
	int value_len;
	long value;

	ASN_DEBUG("aper get constrained_whole_number with lb %ld and ub %ld", lb, ub);

	/* X.691 2002 10.5.4 */
	if (range == 1)
		return lb;

	/* X.691 2002 10.5.7.1 - The bit-field case. */
	if (range <= 255) {
		int bitfield_size = 8;
		for (bitfield_size = 8; bitfield_size >= 2; bitfield_size--)
			if ((range - 1) & (1 << (bitfield_size-1)))
				break;
		value = per_get_few_bits(pd, bitfield_size);
		if (value < 0 || value >= range)
			return -1;
		return value + lb;
	}

	/* X.691 2002 10.5.7.2 - The one-octet case. */
	if (range == 256) {
		if (aper_get_align(pd))
			return -1;
		value = per_get_few_bits(pd, 8);
		if (value < 0 || value >= range)
			return -1;
		return value + lb;
	}

	/* X.691 2002 10.5.7.3 - The two-octet case. */
	if (range <= 65536) {
		if (aper_get_align(pd))
			return -1;
		value = per_get_few_bits(pd, 16);
		if (value < 0 || value >= range)
			return -1;
		return value + lb;
	}

	/* X.691 2002 10.5.7.4 - The indefinite length case. */
	/* since we limit input to be 'long' we don't handle all numbers */
	/* and so length determinant is retrieved as X.691 2002 10.9.3.3 */
	/* number of bytes to store the range */
	for (range_len = 3; ; range_len++) {
		long bits = ((long)1) << (8 * range_len);
		if (range - 1 < bits)
			break;
	}
	value_len = aper_get_constrained_whole_number(pd, 1, range_len);
	if (value_len == -1)
		return -1;
	if (value_len > 4) {
		ASN_DEBUG("todo: aper_get_constrained_whole_number: value_len > 4");
		return -1;
	}
	if (aper_get_align(pd))
		return -1;
	value = per_get_few_bits(pd, value_len * 8);
	if (value < 0 || value >= range)
		return -1;
	return value + lb;
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

	/* X.691 2002 10.9.3.3 */
	if (constrained && ub < 65536)
		return aper_put_constrained_whole_number(po, lb, ub, n + lb) ? -1 : (ssize_t)n;

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
aper_put_nsnnwn(asn_per_outp_t *po, int number) {
	int len;

	ASN_DEBUG("aper put nsnnwn %d", number);

	if (number <= 63) {
		if (per_put_few_bits(po, 0, 1))
			return -1;
		return per_put_few_bits(po, number, 6);
	}

	if (per_put_few_bits(po, 1, 1))
		return -1;

	if (number < 256) {
		len = 1;
	} else if (number < 65536) {
		len = 2;
	} else { /* number > 64K */
		int i;
		for (i = 3; ; i++) {
			int bits = 1 << (8 * i);
			if (number < bits)
				break;
		}
		len = i;
	}

	if (aper_put_align(po) < 0)
		return -1;

	/* put the length which is a non-constrained whole number */
	if (len <= 127) {
		if(per_put_few_bits(po, 0, 1))
			return -1;
		if(per_put_few_bits(po, len, 7))
			return -1;
	} else {
		/* todo but not big problem, it's very doubtful that the
		 * number of bytes to encode 'number' will be > 127
		 */
		return -1;
	}
	if(aper_put_align(po) < 0) /* Aligning on octet */
		return -1;
	return per_put_few_bits(po, number, 8 * len);
}

/* X.691 2002 10.5 - Encoding of a constrained whole number */
int
aper_put_constrained_whole_number(asn_per_outp_t *po, long lb, long ub, long number) {
	assert(ub >= lb);
	long range = ub - lb + 1;
	long value = number - lb;
	int range_len;
	int value_len;

	ASN_DEBUG("aper put constrained_whole_number %ld with lb %ld and ub %ld", number, lb, ub);

	if (number < lb || number > ub)
		return -1;

	/* X.691 2002 10.5.4 */
	if (range == 1)
		return 0;

	/* X.691 2002 10.5.7.1 - The bit-field case. */
	if (range <= 255) {
		int bitfield_size = 8;
		for (bitfield_size = 8; bitfield_size >= 2; bitfield_size--)
			if ((range - 1) & (1 << (bitfield_size-1)))
				break;
		return per_put_few_bits(po, value, bitfield_size);
	}

	/* X.691 2002 10.5.7.2 - The one-octet case. */
	if (range == 256) {
		if (aper_put_align(po))
			return -1;
		return per_put_few_bits(po, value, 8);
	}

	/* X.691 2002 10.5.7.3 - The two-octet case. */
	if (range <= 65536) {
		if (aper_put_align(po))
			return -1;
		return per_put_few_bits(po, value, 16);
	}

	/* X.691 2002 10.5.7.4 - The indefinite length case. */
	/* since we limit input to be 'long' we don't handle all numbers */
	/* and so length determinant is stored as X.691 2002 10.9.3.3 */
	/* number of bytes to store the range */
	for (range_len = 3; ; range_len++) {
		int bits = 1 << (8 * range_len);
		if (range - 1 < bits)
			break;
	}
	/* number of bytes to store the value */
	for (value_len = 1; ; value_len++) {
		long bits = ((long)1) << (8 * value_len);
		if (value < bits)
			break;
	}
	if (aper_put_constrained_whole_number(po, 1, range_len, value_len))
		return -1;
	if (aper_put_align(po))
		return -1;
	return per_put_few_bits(po, value, value_len * 8);
}
