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
#include <time.h>

/* This file contains helpers functions to be reused as callbacks in the struct dict_type_data structure.
There are three callbacks there:

 - type_encode    :
 - type_interpret :
 	  Those two callbacks allow to manipulate more natural structures of data in the code, and to 
	map transparently these natural structures with the AVP-encoded format by calling the functions
	msg_avp_value_encode or msg_avp_value_interpret.
 - type_dump : 
 	  This callback if provided gives a more human-readable debug information.

 */

/****************************/
/*    Address  AVP  type    */
/****************************/

/* The interpret and encode functions work with a "struct sockaddr_storage" pointer for mapping 
the contents of the AVP */

int fd_dictfct_Address_encode(void * data, union avp_value * avp_value)
{
	sSS * ss = (sSS *) data;
	uint16_t AddressType = 0;
	size_t	size = 0;
	unsigned char * buf = NULL;
	
	TRACE_ENTRY("%p %p", data, avp_value);
	CHECK_PARAMS( data && avp_value  );
	
	switch (ss->ss_family) {
		case AF_INET:
			{
				/* We are encoding an IP address */
				sSA4 * sin = (sSA4 *)ss;
				
				AddressType = 1;/* see http://www.iana.org/assignments/address-family-numbers/ */
				size = 6;	/* 2 for AddressType + 4 for data */
				
				CHECK_MALLOC(  buf = malloc(size)  );
				
				/* may not work because of alignment: *(uint32_t *)(buf+2) = htonl(sin->sin_addr.s_addr); */
				memcpy(buf + 2, &sin->sin_addr.s_addr, 4);
			}
			break;
				
		case AF_INET6:
			{
				/* We are encoding an IPv6 address */
				sSA6 * sin6 = (sSA6 *)ss;
				
				AddressType = 2;/* see http://www.iana.org/assignments/address-family-numbers/ */
				size = 18;	/* 2 for AddressType + 16 for data */
				
				CHECK_MALLOC(  buf = malloc(size)  );
				
				/* The order is already good here */
				memcpy(buf + 2, &sin6->sin6_addr.s6_addr, 16);
				
			}
			break;
				
		default:
			CHECK_PARAMS( AddressType = 0 );
	}
	
	*(uint16_t *)buf = htons(AddressType);

	avp_value->os.len = size;
	avp_value->os.data = buf;
	
	return 0;
}

int fd_dictfct_Address_interpret(union avp_value * avp_value, void * interpreted)
{
	uint16_t AddressType = 0;
	unsigned char * buf;
	
	TRACE_ENTRY("%p %p", avp_value, interpreted);
	
	CHECK_PARAMS( avp_value && interpreted && (avp_value->os.len >= 2)  );
	
	AddressType = ntohs(*(uint16_t *)avp_value->os.data);
	buf = &avp_value->os.data[2];
	
	switch (AddressType) {
		case 1 /* IP */:
			{
				sSA4 * sin = (sSA4 *)interpreted;
				
				CHECK_PARAMS(  avp_value->os.len == 6  );
				
				sin->sin_family = AF_INET;
				/* sin->sin_addr.s_addr = ntohl( * (uint32_t *) buf); -- may not work because of bad alignment */
				memcpy(&sin->sin_addr.s_addr, buf, 4);
			}
			break;
				
		case 2 /* IP6 */:
			{
				sSA6 * sin6 = (sSA6 *)interpreted;
				
				CHECK_PARAMS(  avp_value->os.len == 18  );
				
				sin6->sin6_family = AF_INET6;
				memcpy(&sin6->sin6_addr.s6_addr, buf, 16);
				
			}
			break;
				
		default:
			CHECK_PARAMS( AddressType = 0 );
	}
	
	return 0;
}

/* Dump the content of an Address AVP */
DECLARE_FD_DUMP_PROTOTYPE(fd_dictfct_Address_dump, union avp_value * avp_value)
{
	union {
		sSA	sa;
		sSS	ss;
		sSA4	sin;
		sSA6	sin6;
	} s;
	uint16_t fam;
	
	FD_DUMP_HANDLE_OFFSET();
	
	memset(&s, 0, sizeof(s));
	
	/* The first two octets represent the address family, http://www.iana.org/assignments/address-family-numbers/ */
	if (avp_value->os.len < 2) {
		CHECK_MALLOC_DO( fd_dump_extend(FD_DUMP_STD_PARAMS, "[invalid length: %zd]", avp_value->os.len), return NULL);
		return *buf;
	}
	
	/* Following octets are the address in network byte order already */
	fam = avp_value->os.data[0] << 8 | avp_value->os.data[1];
	switch (fam) {
		case 1:
			/* IP */
			s.sa.sa_family = AF_INET;
			if ((avp_value->os.len != 6) && (avp_value->os.len != 8)) {
				CHECK_MALLOC_DO( fd_dump_extend(FD_DUMP_STD_PARAMS, "[invalid IP length: %zd]", avp_value->os.len), return NULL);
				return *buf;
			}
			memcpy(&s.sin.sin_addr.s_addr, avp_value->os.data + 2, 4);
			if (avp_value->os.len == 8)
				memcpy(&s.sin.sin_port, avp_value->os.data + 6, 2);
			break;
		case 2:
			/* IP6 */
			s.sa.sa_family = AF_INET6;
			if ((avp_value->os.len != 18) && (avp_value->os.len != 20)) {
				CHECK_MALLOC_DO( fd_dump_extend(FD_DUMP_STD_PARAMS, "[invalid IP6 length: %zd]", avp_value->os.len), return NULL);
				return *buf;
			}
			memcpy(&s.sin6.sin6_addr.s6_addr, avp_value->os.data + 2, 16);
			if (avp_value->os.len == 20)
				memcpy(&s.sin6.sin6_port, avp_value->os.data + 18, 2);
			break;
		default:
			CHECK_MALLOC_DO( fd_dump_extend(FD_DUMP_STD_PARAMS, "[unsupported family: 0x%hx]", fam), return NULL);
			return *buf;
	}
	
	return fd_sa_dump(FD_DUMP_STD_PARAMS, &s.sa, NI_NUMERICHOST);
}



/*******************************/
/*    UTF8String  AVP  type    */
/*******************************/

/* Dump the AVP in a natural human-readable format. This dumps the complete length of the AVP, it is up to the caller to truncate if needed */
DECLARE_FD_DUMP_PROTOTYPE(fd_dictfct_UTF8String_dump, union avp_value * avp_value)
{
	size_t l;
	FD_DUMP_HANDLE_OFFSET();
	
	l = avp_value->os.len;
	/* Just in case the string ends in invalid UTF-8 chars, we shorten it */
	while ((l > 0) && (avp_value->os.data[l - 1] & 0x80)) {
		/* this byte is start or cont. of multibyte sequence, as we do not know the next byte we need to delete it. */
		l--;
		if (avp_value->os.data[l] & 0x40)
			break; /* This was a start byte, we can stop the loop */
	}
	
	CHECK_MALLOC_DO( fd_dump_extend(FD_DUMP_STD_PARAMS, "\"%.*s\"", (int)l, (char *)avp_value->os.data), return NULL);
	
	return *buf;
}


/*******************************/
/*    Time  AVP  type    */
/*******************************/

/* The interpret and encode functions work with a "time_t" pointer for mapping 
the contents of the AVP */

/* Unix Epoch starts 1970-01-01, NTP 0 is at 1900-01-01 */
#define DIFF_EPOCH_TO_NTP ((365*(1970-1900) + 17ul) * 24 * 60 * 60)

static int diameter_string_to_time_t(const char *str, size_t len, time_t *result) {
    time_t time_stamp;
    CHECK_PARAMS(len == 4);
   
    time_stamp = (((unsigned long)(str[0]&0xff))<<24) + ((str[1]&0xff)<<16) + ((str[2]&0xff)<<8) + ((str[3]&0xff));
    time_stamp -= DIFF_EPOCH_TO_NTP;
#ifdef FIX__NEEDED_FOR_YEAR_2036_AND_LATER
/* NTP overflows in 2036; after that, values start at zero again */
#define NTP_OVERFLOW_CORRECTION (0x100000000ull)
    /* XXX: debug and find correct conversion */
    if (str[0] & 0x80 == 0x00) {
        time_stamp += NTP_OVERFLOW_CORRECTION;
    }
#endif
    *result = time_stamp;
    return 0;
}

static int time_t_to_diameter_string(time_t time_stamp, char **result) {
    uint64_t out = time_stamp;
    char *conv;
    /* XXX: 2036 fix */
    out += DIFF_EPOCH_TO_NTP;
    CHECK_PARAMS( (out >> 32) == 0);

    CHECK_MALLOC(conv=(char *)malloc(5));
    
    conv[0] = (out>>24) & 0xff;
    conv[1] = (out>>16) & 0xff;
    conv[2] = (out>> 8) & 0xff;
    conv[3] =  out      & 0xff;
    conv[4] = '\0';
    *result = conv;
    return 0;
}

int fd_dictfct_Time_encode(void * data, union avp_value * avp_value)
{
	char * buf;
	size_t len;
	
	TRACE_ENTRY("%p %p", data, avp_value);
	CHECK_PARAMS( data && avp_value  );
	
	CHECK_FCT( time_t_to_diameter_string( *((time_t *)data), &buf) );
	/* FIXME: return len from the function above? */ len = 4;
	
	avp_value->os.len = len;
	avp_value->os.data = (uint8_t *)buf;
	return 0;
}

int fd_dictfct_Time_interpret(union avp_value * avp_value, void * interpreted)
{
	TRACE_ENTRY("%p %p", avp_value, interpreted);
	
	CHECK_PARAMS( avp_value && interpreted );
	
	return diameter_string_to_time_t((const char *)avp_value->os.data, avp_value->os.len, interpreted);
}

static void _format_offs (long offset, char *buf) {
    int offs_hours, offs_minutes, sgn = 1;
    if (offset < 0) {
        offset = -offset;
        sgn = 1;
    }
    offs_hours = (int)(offset/3600);
    offs_minutes = (offset%3600)/60;

    char* s = buf;

    *(s++) = sgn == 1 ? '+' : '-';
    *(s++) = (char)(offs_hours/10) + '0';
    *(s++) = offs_hours%10 + '0';

    if (offs_minutes == 0) {
        *(s++) = '\0';
    } else {
        *(s++) = (char)(offs_minutes/10) + '0';
        *(s++) = offs_minutes%10 + '0';
        *(s++) = '\0';
    }
}

DECLARE_FD_DUMP_PROTOTYPE(fd_dictfct_Time_dump, union avp_value * avp_value)
{
	time_t val;
	struct tm conv;
	char tz_buf[7];
		
	FD_DUMP_HANDLE_OFFSET();
	
	if (avp_value->os.len != 4) {
		CHECK_MALLOC_DO( fd_dump_extend(FD_DUMP_STD_PARAMS, "[invalid length: %zd]", avp_value->os.len), return NULL);
		return *buf;
	}

	if (diameter_string_to_time_t((char *)avp_value->os.data, avp_value->os.len, &val) != 0) {
		CHECK_MALLOC_DO( fd_dump_extend(FD_DUMP_STD_PARAMS, "[time conversion error]"), return NULL);
		return *buf;
	}
	
	CHECK_MALLOC_DO( localtime_r(&val, &conv), return NULL);
	_format_offs(conv.tm_gmtoff, tz_buf);
	CHECK_MALLOC_DO( fd_dump_extend(FD_DUMP_STD_PARAMS, "%d%02d%02dT%02d%02d%02d%s", conv.tm_year+1900, conv.tm_mon+1, conv.tm_mday, conv.tm_hour, conv.tm_min, conv.tm_sec, tz_buf), return NULL);
	return *buf;
}

