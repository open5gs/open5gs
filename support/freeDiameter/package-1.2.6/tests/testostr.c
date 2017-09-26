/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2011, WIDE Project and NICT								 *
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

#include "tests.h"

#define TEST_STR (os0_t)"This is my test string (with extra unused data)"

/* The following string contains UTF-8 encoded characters (Chinese characters) */
#define TEST_IDN_UTF8  "freeDiameter.中国"
#define TEST_IDN_CONV  "freeDiameter.xn--fiqs8s"

/* Main test routine */
int main(int argc, char *argv[])
{
	/* First, initialize the daemon modules */
	INIT_FD();
	
	/* Check the hash function */
	{
		uint8_t buf[30];
		
		uint32_t hash = fd_os_hash(TEST_STR, CONSTSTRLEN(TEST_STR)); /* reference value */
		
		/* Check that a hash of a substring / surstring is different */
		CHECK( 1, hash != fd_os_hash(TEST_STR, CONSTSTRLEN(TEST_STR) - 1) ? 1 : 0 );
		CHECK( 1, hash != fd_os_hash(TEST_STR, CONSTSTRLEN(TEST_STR) + 1) ? 1 : 0 );
		
		/* Check alignment of the string is not important */
		memcpy(buf + 4, TEST_STR, CONSTSTRLEN(TEST_STR));
		CHECK( hash, fd_os_hash(buf + 4, CONSTSTRLEN(TEST_STR)) );
		
		memcpy(buf + 3, TEST_STR, CONSTSTRLEN(TEST_STR));
		CHECK( hash, fd_os_hash(buf + 3, CONSTSTRLEN(TEST_STR)) );
		
		memcpy(buf + 2, TEST_STR, CONSTSTRLEN(TEST_STR));
		CHECK( hash, fd_os_hash(buf + 2, CONSTSTRLEN(TEST_STR)) );
		
		memcpy(buf + 1, TEST_STR, CONSTSTRLEN(TEST_STR));
		CHECK( hash, fd_os_hash(buf + 1, CONSTSTRLEN(TEST_STR)) );
	}
	
	/* Check the Diameter Identity functions */
	{
		char * res;
		size_t len=0;
		
		/* A valid ASCII domain name */
		res = TEST_IDN_CONV;
		CHECK( 0, fd_os_validate_DiameterIdentity(&res, &len, 1) );
		CHECK( 0, strcasecmp(res, TEST_IDN_CONV) ); /* the function does not change a valid DN */
		CHECK( 0, fd_os_validate_DiameterIdentity(&res, &len, 0) );
		CHECK( 0, strcasecmp(res, TEST_IDN_CONV) );
		CHECK( CONSTSTRLEN(TEST_IDN_CONV), len );
		free(res);
		
		/* Now, an invalid string */
		res = TEST_IDN_UTF8;
		len = 0;
		
		#ifdef DIAMID_IDNA_IGNORE
		
		/* The UTF-8 chars are considered valid */
		CHECK( 1, fd_os_is_valid_DiameterIdentity((os0_t)TEST_IDN_UTF8, CONSTSTRLEN(TEST_IDN_UTF8) ) );
		
		/* The string should be passed unmodified */
		CHECK( 0, fd_os_validate_DiameterIdentity(&res, &len, 1) );
		CHECK( 0, strcasecmp(res, TEST_IDN_UTF8) );
		CHECK( 0, fd_os_cmp(res, len, TEST_IDN_UTF8, CONSTSTRLEN(TEST_IDN_UTF8)) );
		CHECK( 0, fd_os_almostcasesrch(res, len, TEST_IDN_UTF8, CONSTSTRLEN(TEST_IDN_UTF8), NULL) );
		CHECK( 0, fd_os_validate_DiameterIdentity(&res, &len, 0) );
		CHECK( 0, strcasecmp(res, TEST_IDN_UTF8) );
		CHECK( CONSTSTRLEN(TEST_IDN_UTF8), len );
		free(res);
		
		#else /* DIAMID_IDNA_IGNORE */
		
		/* The UTF-8 chars are recognized as invalid DiameterIdentity */
		CHECK( 0, fd_os_is_valid_DiameterIdentity((os0_t)TEST_IDN_UTF8, CONSTSTRLEN(TEST_IDN_UTF8) ));
		
		# ifdef DIAMID_IDNA_REJECT
		
		/* The string must be rejected */
		CHECK( EINVAL, fd_os_validate_DiameterIdentity(&res, &len, 1) );
		
		# else /* DIAMID_IDNA_REJECT */
		
		/* The string should be transformed into TEST_IDN_CONV */
		CHECK( 0, fd_os_validate_DiameterIdentity(&res, &len, 1) );
		CHECK( 0, strcasecmp(res, TEST_IDN_CONV) );
		CHECK( CONSTSTRLEN(TEST_IDN_CONV), len );
		free(res);
		
		# endif /* DIAMID_IDNA_REJECT */
		#endif /* DIAMID_IDNA_IGNORE */

	}
	
	{
		/* test fd_os_cmp and fd_os_almostcasesrch and that they are compatible */
		char *t1 = "a";
		char *t2 = "b";
		char *t3 = "C";
		char *t4 = "d";
		char *t5 = "aa";
		char *t6 = "aB";
		char *t7 = "Ac";
		char *t8 = "aD";
		char *t9 = "AAA";
		
		char *t5b = "Aa";
		char *t6b = "ab";
		
		/* First, create a list with all the elements in order given by fd_os_cmp */
		char *t[] = { t1, t2, t3, t4, t5, t6,t7, t8, t9 };
		int i;
		struct fd_list *li, l = FD_LIST_INITIALIZER(l);
		for (i = 0; i < sizeof(t) / sizeof(t[0]); i++) {
			/* insert t[i] */
			struct fd_list *n = malloc(sizeof(struct fd_list));
			CHECK( 1, n ? 1 : 0 );
			fd_list_init(n, t[i]);
			for (li = l.next; li != &l; li = li->next) {
				if ( fd_os_cmp(t[i], strlen(t[i]), li->o, strlen(li->o)) < 0 )
					break;
			} 
			fd_list_insert_before(li, n);
		}
		/* in principle the result is: [ "C", "a", "b", "d", "Ac", "aB", "aD", "aa", "AAA" ] */
		
		/* Since there is no equal value in the list (even case-insensitive), check that the order is valid also for the caseinsensitive variant */
		for (li = l.next; li != l.prev; li = li->next) {
			CHECK( 1, fd_os_almostcasesrch(li->o, strlen(li->o), li->next->o, strlen(li->next->o), NULL) < 0 ? 1 : 0 );
		}
		
		/* Now check that we can case-insentively find t5b and t6b to be equal to t5 and t6 resp. (this is how we use it in the daemon) */
		for (li = l.next; li != &l; li = li->next) {
			int cont, cmp;
			cmp = fd_os_almostcasesrch(t5b, strlen(t5b), li->o, strlen(li->o), &cont);
			TRACE_DEBUG(FULL, "Comp '%s' : %d, %d", (char *)li->o, cmp, cont);
			if (cmp == 0)
				break;
			if (!cont)
				break;
		}
		CHECK( li->o, t5 );
		
		for (li = l.next; li != &l; li = li->next) {
			int cont, cmp;
			cmp = fd_os_almostcasesrch(t6b, strlen(t6b), li->o, strlen(li->o), &cont);
			TRACE_DEBUG(FULL, "Comp '%s' : %d, %d", (char *)li->o, cmp, cont);
			if (cmp == 0)
				break;
			if (!cont)
				break;
		}
		CHECK( li->o, t6 );
		
		
		/* done */
		while (!FD_IS_LIST_EMPTY(&l)) {
			li = l.next;
			fd_list_unlink(li);
			free(li);
		}
	}
	
	/* That's all for the tests yet */
	PASSTEST();
} 
	
