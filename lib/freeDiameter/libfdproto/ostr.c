/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2013, WIDE Project and NICT								 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*    													 *
* * Redistributions in binary form must reproduce the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer in the documentation and/or other						 *
*   materials provided with the distribution.								 *
* 													 *
* * Neither the name of the WIDE Project or NICT nor the 						 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of WIDE Project and 						 *
*   NICT.												 *
* 													 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 	 *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 	 *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/

#include "fdproto-internal.h"

#if (!defined(DIAMID_IDNA_IGNORE) && !defined(DIAMID_IDNA_REJECT))
/* Process IDNA with stringprep -- See RFC5890 -- and libidn documentation... */
#include <idna.h> /* idna_to_ascii_8z() */
#endif /* !defined(DIAMID_IDNA_IGNORE) && !defined(DIAMID_IDNA_REJECT) */

/* Similar to strdup with (must have been verified) os0_t */
os0_t os0dup_int(os0_t s, size_t l) {
	os0_t r;
	CHECK_MALLOC_DO( r = malloc(l+1), return NULL );
	if (l)
		memcpy(r, s, l); /* this might be faster than a strcpy or strdup because it can work with 32 or 64b blocks */
	r[l] = '\0';
	return r;
}

/* case sensitive comparison, fast */
int fd_os_cmp_int(uint8_t * os1, size_t os1sz, uint8_t * os2, size_t os2sz)
{
	ASSERT( os1 && os2);
	if (os1sz < os2sz)
		return -1;
	if (os1sz > os2sz)
		return 1;
	return os1sz ? memcmp(os1, os2, os1sz) : 0;
}

/* a local version of tolower() that does not depend on LC_CTYPE locale */
static inline uint8_t asciitolower(uint8_t a)
{
	if ((a >= 'A') && (a <= 'Z'))
		return a + 32 /* == 'a' - 'A' */;
	return a;
}

/* less sensitive to case, slower. */
/* the semantics of "maybefurther" assume you are searching for os1 in a list of elements ordered, each element passed as os2 */
int fd_os_almostcasesrch_int(uint8_t * os1, size_t os1sz, uint8_t * os2, size_t os2sz, int *maybefurther)
{
	int i;
	int res = 0;
	
	ASSERT( os1 && os2);
	if (maybefurther)
		*maybefurther = 0;
	
	if (os1sz < os2sz)
		return -1;
	
	if (maybefurther)
		*maybefurther = 1;
	
	if (os1sz > os2sz)
		return 1;
	
	for (i = 0; i < os1sz; i++) {
		if (os1[i] == os2[i])
			continue;
		
		if (!res) 
			res = os1[i] < os2[i] ? -1 : 1;
		
		if (asciitolower(os1[i]) == asciitolower(os2[i])) 
			continue;
		
		return res;
	}
	
	return 0;
}

/* Check if the string contains only ASCII */
int fd_os_is_valid_DiameterIdentity(uint8_t * os, size_t ossz)
{
#ifdef DIAMID_IDNA_IGNORE
	
	/* Allow anything */
	
#else /* DIAMID_IDNA_IGNORE */
	
	int i;
	
	/* Allow only letters, digits, hyphen, dot */
	for (i=0; i < ossz; i++) {
		if (os[i] > 'z')
			break;
		if (os[i] >= 'a')
			continue;
		if ((os[i] >= 'A') && (os[i] <= 'Z'))
			continue;
		if ((os[i] == '-') || (os[i] == '.'))
			continue;
		if ((os[i] >= '0') && (os[i] <= '9'))
			continue;
		break;
	}
	if (i < ossz) {
		int nb = 1;
		/* To get a better display, check if the invalid char is UTF-8 */
		if ((os[i] & 0xE0) == 0xC0 /* 110xxxxx */) {
			if ((i < ossz - 1) && ((os[i + 1] & 0xC0) == 0x80 /* 10xxxxxx */))
				nb = 2;
			goto disp;
		}
		if ((os[i] & 0xF0) == 0xE0 /* 1110xxxx */) {
			if ((i < ossz - 2) && ((os[i + 1] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 2] & 0xC0) == 0x80 /* 10xxxxxx */))
				nb = 3;
			goto disp;
		}
		if ((os[i] & 0xF8) == 0xF0 /* 11110xxx */) {
			if ((i < ossz - 3) && ((os[i + 1] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 2] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 3] & 0xC0) == 0x80 /* 10xxxxxx */))
				nb = 4;
			goto disp;
		}
		if ((os[i] & 0xFC) == 0xF8 /* 111110xx */) {
			if ((i < ossz - 4) && ((os[i + 1] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 2] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 3] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 4] & 0xC0) == 0x80 /* 10xxxxxx */))
				nb = 5;
			goto disp;
		}
		if ((os[i] & 0xFE) == 0xFC /* 1111110x */) {
			if ((i < ossz - 5) && ((os[i + 1] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 2] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 3] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 4] & 0xC0) == 0x80 /* 10xxxxxx */)
					   && ((os[i + 5] & 0xC0) == 0x80 /* 10xxxxxx */))
				nb = 6;
			goto disp;
		}
		/* otherwise, we just display the hex code */
		TRACE_DEBUG(INFO, "Invalid character (0x%hhX) at offset %d in DiameterIdentity '%.*s'", os[i], i+1, (int)ossz, os);
		return 0;
disp:
		TRACE_DEBUG(INFO, "Invalid character '%.*s' at offset %d in DiameterIdentity '%.*s'", nb, os + i, i+1, (int)ossz, os);
		return 0;
	}
	
#endif /* DIAMID_IDNA_IGNORE */
	
	return 1;
}

/* The following function validates a string as a Diameter Identity or applies the IDNA transformation on it 
 if *inoutsz is != 0 on entry, *id may not be \0-terminated.
 memory has the following meaning: 0: *id can be realloc'd. 1: *id must be malloc'd on output (was static)
*/
int fd_os_validate_DiameterIdentity(char ** id, size_t * inoutsz, int memory)
{
#if !defined(DIAMID_IDNA_IGNORE) && !defined(DIAMID_IDNA_REJECT)
	int gotsize = 0;
#endif /* defined(DIAMID_IDNA_IGNORE) || defined(DIAMID_IDNA_REJECT) */
	
	TRACE_ENTRY("%p %p", id, inoutsz);
	CHECK_PARAMS( id && *id && inoutsz );
	
	if (!*inoutsz)
		*inoutsz = strlen(*id);
#if !defined(DIAMID_IDNA_IGNORE) && !defined(DIAMID_IDNA_REJECT)
	else
		gotsize = 1;
#endif /* defined(DIAMID_IDNA_IGNORE) || defined(DIAMID_IDNA_REJECT) */
	
#ifndef DIAMID_IDNA_IGNORE
	
	if (!fd_os_is_valid_DiameterIdentity((os0_t)*id, *inoutsz)) {
	
#ifdef DIAMID_IDNA_REJECT
		
		TRACE_DEBUG(INFO, "The string '%s' is not a valid DiameterIdentity!", *id);
		TRACE_DEBUG(INFO, "Returning EINVAL since fD is compiled with option DIAMID_IDNA_REJECT.");
		return EINVAL;
	
#else /* DIAMID_IDNA_REJECT */
	
		char *processed;
		int ret;
		
		if (gotsize) { /* make it \0-terminated */
			if (memory) {
				CHECK_MALLOC( *id = os0dup(*id, *inoutsz) );
				memory = 0;
			} else {
				CHECK_MALLOC( *id = realloc(*id, *inoutsz + 1) );
				(*id)[*inoutsz] = '0';
			}
		}
		
		ret = idna_to_ascii_8z ( *id, &processed, IDNA_USE_STD3_ASCII_RULES );
		if (ret == IDNA_SUCCESS) {
			TRACE_DEBUG(INFO, "The string '%s' is not a valid DiameterIdentity, it was changed to '%s'", *id, processed);
			if (memory == 0)
				free(*id);
			*id = processed;
			*inoutsz = strlen(processed);
			/* Done! */
		} else {
			TRACE_DEBUG(INFO, "The string '%s' is not a valid DiameterIdentity and cannot be sanitanized: %s", *id, idna_strerror (ret));
			return EINVAL;
		}
	
#endif /* DIAMID_IDNA_REJECT */
	} else
#endif /* ! DIAMID_IDNA_IGNORE */
	{
		if (memory == 1) {
			CHECK_MALLOC( *id = os0dup(*id, *inoutsz) );
		}
	}
	return 0;
}

/* Analyze a DiameterURI and return its components. 
  Return EINVAL if the URI is not valid. 
  *diamid is malloc'd on function return and must be freed (it is processed by fd_os_validate_DiameterIdentity).
  *secure is 0 (no security) or 1 (security enabled) on return.
  *port is 0 (default) or a value in host byte order on return.
  *transport is 0 (default) or IPPROTO_* on return.
  *proto is 0 (default) or 'd' (diameter), 'r' (radius), or 't' (tacacs+) on return.
  */
int fd_os_parse_DiameterURI(uint8_t * uri, size_t urisz, DiamId_t * diamid, size_t * diamidlen, int * secure, uint16_t * port, int * transport, char *proto)
{
	size_t offset = 0;
	DiamId_t fqdn = NULL;
	size_t   fqdnlen;
	TRACE_ENTRY("%p %zd %p %p %p %p %p %p", uri, urisz, diamid, diamidlen, secure, port, transport, proto);
	CHECK_PARAMS( uri && urisz );
	
	CHECK_PARAMS( urisz > 7 ); /* "aaa" + "://" + something else at least */
	
	/* Initialize values */
	if (secure)
		*secure = 0;
	if (port)
		*port = 0;
	if (transport)
		*transport = 0;
	if (proto)
		*proto = 0;
	
	/* Check the beginning */
	if (memcmp( uri, "aaa", 3)) {
		TRACE_DEBUG(INFO, "Invalid DiameterURI prefix: got '%.*s', expected 'aaa'", 3, uri);
		return EINVAL;
	}
	offset += 3;
	
	/* Secure? */
	if (uri[offset] == (uint8_t)'s') {
		if (secure)
			*secure = 1;
		offset += 1;
	}
	
	/* Remaining of URI marker */
	if (memcmp( uri + offset, "://", 3)) {
		TRACE_DEBUG(INFO, "Invalid DiameterURI prefix: got '%.*s', expected 'aaa://' or 'aaas://'", (int)offset + 3, uri);
		return EINVAL;
	}
	offset += 3;
	
	/* This is the start of the FQDN */
	fqdn = (DiamId_t)uri + offset;
	for ( ; offset < urisz ; offset++ ) {
		/* Stop only when we find ':' or ';' */
		if ((uri[offset] == (uint8_t)':') || (uri[offset] == (uint8_t)';'))
			break;
	}
	fqdnlen = offset - (fqdn - (DiamId_t)uri);
	CHECK_FCT(fd_os_validate_DiameterIdentity(&fqdn, &fqdnlen, 1));
	if (diamid)
		*diamid = fqdn;
	else
		free(fqdn);
	if (diamidlen)
		*diamidlen = fqdnlen;
	
	if (offset == urisz)
		return 0; /* Finished */
	
	/* Is there a port ? */
	if (uri[offset] == ':') {
		uint16_t p = 0;
		do {
			offset++;

			if (offset == urisz)
				break;

			uint32_t t = (uint32_t)((char)uri[offset] - '0');
			if (t > 9)
				break; /* we did not get a digit */

			t += p * 10; /* the port is specified in decimal base */
			
			if (t >= (1<<16)) {
				TRACE_DEBUG(INFO, "Invalid DiameterURI: port value is too big.");
				return EINVAL;
			}

			p = t;
		} while (1);

		if (port)
			*port = p;
	}
	
	if (offset == urisz)
		return 0; /* Finished */
	
	/* Is there a transport? */
	if ( (urisz - offset > CONSTSTRLEN(";transport=")) 
		&& !strncasecmp((char *)uri + offset, ";transport=", CONSTSTRLEN(";transport=")) ) {
	
		offset += CONSTSTRLEN(";transport=");

		if (urisz - offset < 3) {
			TRACE_DEBUG(INFO, "Invalid DiameterURI: transport string is too short, ignored.");
			return 0;
		}		
		if (!strncasecmp((char *)uri + offset, "tcp", CONSTSTRLEN("tcp"))) {
			if (transport)
				*transport = IPPROTO_TCP;
			offset += CONSTSTRLEN("tcp");
			goto after_transport;
		}
		if (!strncasecmp((char *)uri + offset, "udp", CONSTSTRLEN("udp"))) {
			if (transport)
				*transport = IPPROTO_UDP;
			offset += CONSTSTRLEN("udp");
			goto after_transport;
		}
		if ((urisz - offset > 3) && !strncasecmp((char *)uri + offset, "sctp", CONSTSTRLEN("sctp"))) {
			if (transport) {
#ifndef DISABLE_SCTP
				*transport = IPPROTO_SCTP;
#else /* DISABLE_SCTP */
				TRACE_DEBUG(INFO, "Received DiameterURI with 'transport=sctp' but DISABLE_SCTP was selected");
				*transport = 0;
#endif /* DISABLE_SCTP */
			}
			offset += CONSTSTRLEN("sctp");
			goto after_transport;
		}
		
		TRACE_DEBUG(INFO, "Invalid DiameterURI: transport string is not recognized ('%.*s').", (int)(urisz - offset), uri + offset);
		return EINVAL;
	}
after_transport:
	if (offset == urisz)
		return 0; /* Finished */
	
	/* Is there a protocol? */
	if ( ((urisz - offset) > CONSTSTRLEN(";protocol=")) 
		&& (!strncasecmp((char *)uri + offset, ";protocol=", CONSTSTRLEN(";protocol="))) ) {
	
		offset += CONSTSTRLEN(";protocol=");

		if ( ((urisz - offset) >= CONSTSTRLEN("diameter")) 
		    && (!strncasecmp((char *)uri + offset, "diameter", CONSTSTRLEN("diameter"))) ) {
			if (proto)
				*proto = 'd';
			offset += CONSTSTRLEN("diameter");
			goto after_proto;
		}
		
		if ( ((urisz - offset) >= CONSTSTRLEN("radius")) 
		    && (!strncasecmp((char *)uri + offset, "radius", CONSTSTRLEN("radius"))) ) {
			if (proto)
				*proto = 'r';
			offset += CONSTSTRLEN("radius");
			goto after_proto;
		}
		
		if ( ((urisz - offset) >= CONSTSTRLEN("tacacs+")) 
		    && (!strncasecmp((char *)uri + offset, "tacacs+", CONSTSTRLEN("tacacs+"))) ) {
			if (proto)
				*proto = 't';
			offset += CONSTSTRLEN("tacacs+");
			goto after_proto;
		}
		
		TRACE_DEBUG(INFO, "Invalid DiameterURI: protocol string is not recognized ('%.*s').", (int)(urisz - offset), uri + offset);
		return EINVAL;
		
	}
after_proto:
	if (offset == urisz)
		return 0; /* Finished */
	
	TRACE_DEBUG(INFO, "Invalid DiameterURI: final part of string is not recognized ('%.*s').", (int)(urisz - offset), uri + offset);
	return EINVAL;
}


/********************************************************************************************************/
/* Hash function -- credits to Austin Appleby, thank you ^^ */
/* See http://murmurhash.googlepages.com for more information on this function */

/* the strings are NOT always aligned properly (ex: received in RADIUS message), so we use the aligned MurmurHash2 function as needed */
#define _HASH_MIX(h,k,m) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }
uint32_t fd_os_hash ( uint8_t * string, size_t len )
{
	uint32_t hash = len;
	uint8_t * data = string;
	
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	int align = (long)string & 3;
	
	if (!align || (len < 4)) {
		/* In case data is aligned, MurmurHash2 function */
		while(len >= 4)
		{
			/* Mix 4 bytes at a time into the hash */
			uint32_t k = *(uint32_t *)data;	/* We don't care about the byte order */

			_HASH_MIX(hash, k, m);

			data += 4;
			len -= 4;
		}

		/* Handle the last few bytes of the input */
		switch(len) {
			case 3: hash ^= data[2] << 16;
			case 2: hash ^= data[1] << 8;
			case 1: hash ^= data[0];
	        		hash *= m;
		}
		
	} else {
		/* Unaligned data, use alignment-safe slower version */
		
		/* Pre-load the temp registers */
		uint32_t t = 0, d = 0;
		switch(align)
		{
			case 1: t |= data[2] << 16;
			case 2: t |= data[1] << 8;
			case 3: t |= data[0];
		}
		t <<= (8 * align);

		data += 4-align;
		len -= 4-align;
		
		/* From this point, "data" can be read by chunks of 4 bytes */
		
		int sl = 8 * (4-align);
		int sr = 8 * align;

		/* Mix */
		while(len >= 4)
		{
			uint32_t k;
			
			d = *(unsigned int *)data;
			k = (t >> sr) | (d << sl);

			_HASH_MIX(hash, k, m);

			t = d;

			data += 4;
			len -= 4;
		}

		/* Handle leftover data in temp registers */
		d = 0;
		if(len >= align)
		{
			uint32_t k;
			
			switch(align)
			{
			case 3: d |= data[2] << 16;
			case 2: d |= data[1] << 8;
			case 1: d |= data[0];
			}

			k = (t >> sr) | (d << sl);
			_HASH_MIX(hash, k, m);

			data += align;
			len -= align;

			/* Handle tail bytes */

			switch(len)
			{
			case 3: hash ^= data[2] << 16;
			case 2: hash ^= data[1] << 8;
			case 1: hash ^= data[0];
					hash *= m;
			};
		}
		else
		{
			switch(len)
			{
			case 3: d |= data[2] << 16;
			case 2: d |= data[1] << 8;
			case 1: d |= data[0];
			case 0: hash ^= (t >> sr) | (d << sl);
					hash *= m;
			}
		}


	}

	/* Do a few final mixes of the hash to ensure the last few
	   bytes are well-incorporated. */
	hash ^= hash >> 13;
	hash *= m;
	hash ^= hash >> 15;

	return hash;
} 

