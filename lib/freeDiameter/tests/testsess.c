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

#include "tests.h"

#define TEST_DIAM_ID 	"testsess.myid"
#define TEST_OPT_IN	"suffix"
#define TEST_OPT	(os0_t)TEST_OPT_IN
#define TEST_SID_IN	TEST_DIAM_ID ";1234;5678;" TEST_OPT_IN
#define TEST_SID	(os0_t)TEST_SID_IN

#define TEST_EYEC	0x7e57e1ec
struct sess_state {
	int	eyec;	/* TEST_EYEC */
	os0_t   sid; 	/* the session with which the data was registered */
	int  *  freed;	/* location where to write the freed status */
	void *  opaque; /* if opaque was provided, this is the value we expect */
};

static void mycleanup( struct sess_state * data, os0_t sid, void * opaque )
{
	/* sanity */
	CHECK( 1, sid ? 1 : 0 );
	CHECK( 1, data? 1 : 0 );
	CHECK( TEST_EYEC, data->eyec );
	CHECK( 0, strcmp((char *)sid, (char *)data->sid) );
	if (data->freed)
		*(data->freed) += 1;
	if (data->opaque) {
		CHECK( 1, opaque == data->opaque ? 1 : 0 );  
	}
	/* Now, free the data */
	free(data->sid);
	free(data);
}

static __inline__ struct sess_state * new_state(os0_t sid, int *freed) 
{
	struct sess_state *new;
	new = malloc(sizeof(struct sess_state));
	CHECK( 1, new ? 1 : 0 );
	memset(new, 0, sizeof(struct sess_state));
	new->eyec = TEST_EYEC;
	new->sid = os0dup(sid, strlen((char *)sid));
	CHECK( 1, new->sid ? 1 : 0 );
	new->freed = freed;
	return new;
}

void * g_opaque = (void *)"test";

/* Avoid a lot of casts */
#undef strlen
#define strlen(s) strlen((char *)s)
#undef strncmp
#define strncmp(s1,s2,l) strncmp((char *)s1, (char *)s2, l)
#undef strcmp
#define strcmp(s1,s2) strcmp((char *)s1, (char *)s2)
	

/* Main test routine */
int main(int argc, char *argv[])
{
	struct session_handler * hdl1, *hdl2;
	struct session *sess1, *sess2, *sess3;
	os0_t str1, str2;
	size_t str1len, str2len;
	int new;
	
	/* First, initialize the daemon modules */
	INIT_FD();
	
	/* Test functions related to handlers (simple situation) */
	{
		void * testptr = NULL;
		CHECK( 0, fd_sess_handler_create ( &hdl1, mycleanup, NULL, NULL ) );
		CHECK( 0, fd_sess_handler_create ( &hdl2, mycleanup, NULL, NULL ) );
		CHECK( 0, fd_sess_handler_destroy( &hdl2, &testptr ) );
		CHECK( 1, testptr == NULL ? 1 : 0 );
		CHECK( 0, fd_sess_handler_create ( &hdl2, mycleanup, NULL, g_opaque ) );
		#if 0
		fd_log_debug("%s", fd_sess_dump_hdl(FD_DUMP_TEST_PARAMS, hdl1));
		fd_log_debug("%s", fd_sess_dump_hdl(FD_DUMP_TEST_PARAMS, hdl2));
		#endif
	}
	
	/* Test Session Id generation (fd_sess_new) */
	{
		/* DiamId is provided, not opt */
		CHECK( 0, fd_sess_new( &sess1, TEST_DIAM_ID, CONSTSTRLEN(TEST_DIAM_ID), NULL, 0 ) );
		CHECK( 0, fd_sess_new( &sess2, TEST_DIAM_ID, CONSTSTRLEN(TEST_DIAM_ID), NULL, 0 ) );
		#if 0
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess1, 1));
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess2, 1));
		#endif
		
		/* Check both string start with the diameter Id, but are different */
		CHECK( 0, fd_sess_getsid(sess1, &str1, &str1len) );
		CHECK( 1, (strlen(str1) == str1len) ? 1 : 0 );
		CHECK( 0, strncmp(str1, TEST_DIAM_ID ";", CONSTSTRLEN(TEST_DIAM_ID) + 1) );
		CHECK( 0, fd_sess_getsid(sess2, &str2, &str2len) );
		CHECK( 0, strncmp(str2, TEST_DIAM_ID ";", CONSTSTRLEN(TEST_DIAM_ID) + 1) );
		CHECK( 1, strcmp(str1, str2) ? 1 : 0 );
		CHECK( 0, fd_sess_destroy( &sess1 ) );
		CHECK( 0, fd_sess_destroy( &sess2 ) );
		
		/* diamId and opt */
		CHECK( 0, fd_sess_new( &sess1, TEST_DIAM_ID, 0, TEST_OPT, 0 ) );
		CHECK( 0, fd_sess_new( &sess2, TEST_DIAM_ID, CONSTSTRLEN(TEST_DIAM_ID), TEST_OPT, CONSTSTRLEN(TEST_OPT_IN) - 1 ) );
		#if 0
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess1, 1));
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess2, 1));
		#endif
		
		CHECK( 0, fd_sess_getsid(sess1, &str1, &str1len) );
		CHECK( 0, strncmp(str1, TEST_DIAM_ID ";", CONSTSTRLEN(TEST_DIAM_ID) + 1) );
		CHECK( 0, strcmp(str1 + str1len - CONSTSTRLEN(TEST_OPT_IN) - 1, ";" TEST_OPT_IN) );
		
		CHECK( 0, fd_sess_getsid(sess2, &str2, &str2len) );
		CHECK( 0, strncmp(str2, TEST_DIAM_ID ";", CONSTSTRLEN(TEST_DIAM_ID) + 1) );
		CHECK( 0, strncmp(str2 + str2len - CONSTSTRLEN(TEST_OPT_IN), ";" TEST_OPT_IN, CONSTSTRLEN(TEST_OPT_IN)) );
		
		CHECK( 1, strcmp(str1, str2) ? 1 : 0 );
		CHECK( 0, fd_sess_destroy( &sess1 ) );
		CHECK( 0, fd_sess_destroy( &sess2 ) );
		
		/* Now, only opt is provided */
		CHECK( 0, fd_sess_new( &sess1, NULL, 0, TEST_SID, 0 ) );
		CHECK( EALREADY, fd_sess_new( &sess2, NULL, 0, TEST_SID, 0 ) );
		CHECK( sess2, sess1 );
		CHECK( EALREADY, fd_sess_new( &sess3, NULL, 0, TEST_SID, CONSTSTRLEN(TEST_SID_IN) ) );
		CHECK( sess3, sess1 );
		CHECK( 0, fd_sess_new( &sess2, NULL, 0, TEST_SID, CONSTSTRLEN(TEST_SID_IN) - 1 ) );
		#if 0
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess1, 1));
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess2, 1));
		#endif
		CHECK( 0, fd_sess_getsid(sess1, &str1, &str1len) );
		CHECK( 0, fd_sess_getsid(sess2, &str2, &str2len) );
		CHECK( 0, strncmp( str1, str2, CONSTSTRLEN(TEST_SID_IN) - 1 ) );
		CHECK( 0, strcmp( str1, TEST_SID ) );
		CHECK( 1, strcmp( str1, str2 ) ? 1 : 0 );
		
		CHECK( 0, fd_sess_destroy( &sess2 ) );
		CHECK( 0, fd_sess_destroy( &sess1 ) );
	}
	
	/* Test fd_sess_getcount */
	{
		uint32_t cnt;
		CHECK( 0, fd_sess_new( &sess1, TEST_DIAM_ID, CONSTSTRLEN(TEST_DIAM_ID), NULL, 0 ) );
		CHECK( 0, fd_sess_new( &sess2, TEST_DIAM_ID, CONSTSTRLEN(TEST_DIAM_ID), NULL, 0 ) );
		CHECK( 0, fd_sess_getcount(&cnt));
		CHECK( 2, cnt);
		CHECK( 0, fd_sess_destroy( &sess2 ) );
		CHECK( 0, fd_sess_getcount(&cnt));
		CHECK( 1, cnt);
		CHECK( 0, fd_sess_destroy( &sess1 ) );
		CHECK( 0, fd_sess_getcount(&cnt));
		CHECK( 0, cnt);
		
	}
		
	/* Test fd_sess_fromsid */
	{
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess1, &new ) );
		CHECK( 1, new ? 1 : 0 );
		
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess2, &new ) );
		CHECK( 0, new );
		CHECK( sess1, sess2 );
		
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess3, NULL ) );
		CHECK( sess1, sess3 );
		
		CHECK( 0, fd_sess_destroy( &sess1 ) );
	}
	
	/* Test fd_sess_reclaim */
	{
		struct sess_state *tms;
		
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess1, &new ) );
		CHECK( 1, new ? 1 : 0 );
		
		CHECK( 0, fd_sess_reclaim( &sess1 ) );
		CHECK( NULL, sess1 );
		
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess1, &new ) );
		CHECK( 1, new ? 1 : 0 );
		
		tms = new_state(TEST_SID, NULL);
		CHECK( 0, fd_sess_state_store ( hdl1, sess1, &tms ) );
		
		CHECK( 0, fd_sess_reclaim( &sess1 ) );
		CHECK( NULL, sess1 );
		
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess1, &new ) );
		CHECK( 0, new );
		
		CHECK( 0, fd_sess_destroy( &sess1 ) );
		
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess1, &new ) );
		CHECK( 1, new ? 1 : 0 );
		
		CHECK( 0, fd_sess_destroy( &sess1 ) );
	}
	
	/* Test timeout function */
	{
		struct timespec timeout;
		
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess1, &new ) );
		CHECK( 1, new ? 1 : 0 );
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &timeout) );
		CHECK( 0, fd_sess_settimeout( sess1, &timeout) ); /* expire now */
		timeout.tv_sec = 0;
		timeout.tv_nsec= 50000000; /* 50 ms */
		CHECK( 0, nanosleep(&timeout, NULL) );
		
		CHECK( 0, fd_sess_fromsid( TEST_SID, CONSTSTRLEN(TEST_SID_IN), &sess1, &new ) );
		CHECK( 1, new ? 1 : 0 );
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &timeout) );
		timeout.tv_sec += 2678500; /* longer that SESS_DEFAULT_LIFETIME */
		CHECK( 0, fd_sess_settimeout( sess1, &timeout) );
		
		/* Create a second session */
		CHECK( 0, fd_sess_new( &sess2, TEST_DIAM_ID, 0, NULL, 0 ) );
		
		/* We don't really have a way to verify the expiry list is in proper order automatically here... */
		
		CHECK( 0, fd_sess_destroy( &sess2 ) );
		CHECK( 0, fd_sess_destroy( &sess1 ) );
	}
	
	
	/* Test states operations */
	{
		struct sess_state * ms[6], *tms;
		int freed[6];
		struct timespec timeout;
		void * testptr = NULL;
		
		/* Create three sessions */
		CHECK( 0, fd_sess_new( &sess1, TEST_DIAM_ID, 0, NULL, 0 ) );
		CHECK( 0, fd_sess_new( &sess2, TEST_DIAM_ID, 0, NULL, 0 ) );
		CHECK( 0, fd_sess_new( &sess3, TEST_DIAM_ID, 0, NULL, 0 ) );
		
		/* Create 2 states */
		CHECK( 0, fd_sess_getsid(sess1, &str1, &str1len) );
		freed[0] = 0;
		ms[0] = new_state(str1, &freed[0]);
		ms[1] = new_state(str1, NULL);

		tms = ms[0]; /* save a copy */
		CHECK( 0, fd_sess_state_store ( hdl1, sess1, &ms[0] ) );
		CHECK( NULL, ms[0] );
		CHECK( EINVAL, fd_sess_state_store ( hdl1, sess1, NULL ) );
		CHECK( EALREADY, fd_sess_state_store ( hdl1, sess1, &ms[1] ) );
		CHECK( 1, ms[1] ? 1 : 0 );
		
		#if 0
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess1, 1));
		#endif
		
		CHECK( 0, fd_sess_state_retrieve( hdl1, sess1, &ms[0] ) );
		CHECK( tms, ms[0] );
		CHECK( 0, freed[0] );
		
		CHECK( 0, fd_sess_state_retrieve( hdl1, sess2, &tms ) );
		CHECK( NULL, tms );
		
		mycleanup(ms[0], str1, NULL);
		mycleanup(ms[1], str1, NULL);
		
		/* Now create 6 states */
		memset(&freed[0], 0, sizeof(freed));
		CHECK( 0, fd_sess_getsid(sess1, &str1, &str1len) );
		ms[0] = new_state(str1, &freed[0]);
		ms[1] = new_state(str1, &freed[1]);
		CHECK( 0, fd_sess_getsid(sess2, &str1, &str1len) );
		ms[2] = new_state(str1, &freed[2]);
		ms[3] = new_state(str1, &freed[3]);
		CHECK( 0, fd_sess_getsid(sess3, &str1, &str1len) );
		ms[4] = new_state(str1, &freed[4]);
		ms[5] = new_state(str1, &freed[5]);
		ms[5]->opaque = g_opaque;
		str2 = os0dup(str1, str1len);
		CHECK( 1, str2 ? 1 : 0 );
		
		/* Store the six states */
		CHECK( 0, fd_sess_state_store ( hdl1, sess1, &ms[0] ) );
		CHECK( 0, fd_sess_state_store ( hdl2, sess1, &ms[1] ) );
		CHECK( 0, fd_sess_state_store ( hdl1, sess2, &ms[2] ) );
		CHECK( 0, fd_sess_state_store ( hdl2, sess2, &ms[3] ) );
		CHECK( 0, fd_sess_state_store ( hdl1, sess3, &ms[4] ) );
		CHECK( 0, fd_sess_state_store ( hdl2, sess3, &ms[5] ) );
		
		#if 0
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess1, 1));
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess2, 1));
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess3, 1));
		#endif
		
		/* Destroy session 3 */
		CHECK( 0, fd_sess_destroy( &sess3 ) );
		CHECK( 0, freed[0] );
		CHECK( 0, freed[1] );
		CHECK( 0, freed[2] );
		CHECK( 0, freed[3] );
		CHECK( 1, freed[4] );
		CHECK( 1, freed[5] );
		
		/* Destroy handler 2 */
		CHECK( 0, fd_sess_handler_destroy( &hdl2, &testptr ) );
		CHECK( 0, freed[0] );
		CHECK( 1, freed[1] );
		CHECK( 0, freed[2] );
		CHECK( 1, freed[3] );
		CHECK( 1, freed[4] );
		CHECK( 1, freed[5] );
		CHECK( 1, testptr == g_opaque ? 1 : 0 );
		
		#if 1
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess1, 1));
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess2, 1));
		#endif
		
		/* Create again session 3, check that no data is associated to it */
		CHECK( 0, fd_sess_fromsid( str2, str1len, &sess3, &new ) );
		CHECK( 1, new ? 1 : 0 );
		CHECK( 0, fd_sess_state_retrieve( hdl1, sess3, &tms ) );
		CHECK( NULL, tms );
		CHECK( 0, fd_sess_destroy( &sess3 ) );
		free(str2);
		
		/* Timeout does call cleanups */
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &timeout) );
		CHECK( 0, fd_sess_settimeout( sess2, &timeout) );
		#if 1
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess1, 1));
		fd_log_debug("%s", fd_sess_dump(FD_DUMP_TEST_PARAMS, sess2, 1));
		#endif
		timeout.tv_sec = 0;
		timeout.tv_nsec= 50000000; /* 50 ms */
		CHECK( 0, nanosleep(&timeout, NULL) );
		CHECK( 0, freed[0] );
		CHECK( 1, freed[1] );
		CHECK( 1, freed[2] );
		CHECK( 1, freed[3] );
		CHECK( 1, freed[4] );
		CHECK( 1, freed[5] );
		
		/* Check the last data can still be retrieved */
		CHECK( 0, fd_sess_state_retrieve( hdl1, sess1, &tms ) );
		CHECK( 1, tms ? 1 : 0);
		CHECK( 0, fd_sess_getsid(sess1, &str1, &str1len) );
		mycleanup(tms, str1, NULL);
	}
	
	/* TODO: add tests on messages referencing sessions */
	
	/* That's all for the tests yet */
	PASSTEST();
} 
	
