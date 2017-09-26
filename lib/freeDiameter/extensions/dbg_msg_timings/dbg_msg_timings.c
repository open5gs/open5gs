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

/* This extension uses the hooks mechanism to compute some timing information related to messages */

#include <freeDiameter/extension.h>

struct fd_hook_permsgdata {
	struct timespec received_on;
	struct timespec sent_on;
};

static struct fd_hook_data_hdl *mt_data_hdl = NULL;
static struct fd_hook_hdl *mt_hdl = NULL;

/* The callback called when messages are received and sent */
static void mt_hook_cb(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata)
{
	struct msg_hdr * hdr;
	char * buf = NULL;
	size_t len;
	
	ASSERT(pmd);
	
	if (type == HOOK_DATA_RECEIVED) {
		/* We just store the timestamp it was received on */
		(void)clock_gettime(CLOCK_REALTIME, &pmd->received_on);
		return;
	}
	
	ASSERT(msg);
	
	/* Check if this message is request or answer */
	CHECK_FCT_DO( fd_msg_hdr(msg, &hdr), return);
	
	if (type == HOOK_MESSAGE_RECEIVED) {
		ASSERT(pmd->received_on.tv_sec); /* otherwise it means the HOOK_DATA_RECEIVED hook was not trigged for this message */
		if (hdr->msg_flags & CMD_FLAG_REQUEST) {
			/* We have received a new request, nothing special to do */
		} else {
			/* This is an answer, check how long it took to get it */
			struct fd_hook_permsgdata *qpmd = fd_hook_get_request_pmd(mt_data_hdl, msg);
			struct timespec delay;
			ASSERT(qpmd); /* If we do not have it, we must find out why */
			ASSERT(qpmd->sent_on.tv_sec); /* same, would mean the HOOK_MESSAGE_SENT hook was not trigged */
			TS_DIFFERENCE( &delay, &qpmd->sent_on, &pmd->received_on );
			CHECK_MALLOC_DO( fd_msg_dump_summary(&buf, &len, NULL, msg, NULL, 0, 1), return );
			LOG_N("[TIMING] RCV ANS %ld.%06ld sec <-'%s': %s", (long)delay.tv_sec, delay.tv_nsec / 1000, peer ? peer->info.pi_diamid : "<unidentified>", buf);
		}
	} else if (type == HOOK_MESSAGE_SENT) {
		DiamId_t source = NULL;
		
		(void)clock_gettime(CLOCK_REALTIME, &pmd->sent_on);
		
		/* Is this a forwarded message ? */
		CHECK_FCT_DO( fd_msg_source_get(msg, &source, NULL), return );
		if (source) {
			struct timespec delay;
			ASSERT(pmd->received_on.tv_sec);
			TS_DIFFERENCE( &delay, &pmd->received_on, &pmd->sent_on );
			CHECK_MALLOC_DO( fd_msg_dump_summary(&buf, &len, NULL, msg, NULL, 0, 1), return );
			LOG_N("[TIMING] FWD %ld.%06ld sec '%s'->'%s': %s", (long)delay.tv_sec, delay.tv_nsec / 1000, source, peer ? peer->info.pi_diamid : "<unidentified>", buf);
		} else if (hdr->msg_flags & CMD_FLAG_REQUEST) {
			/* We are sending a request issued locally, nothing special to log */
		} else {
			/* We have generated an anwer, log the time it took since the corresponding request was received */
			struct fd_hook_permsgdata *qpmd = fd_hook_get_request_pmd(mt_data_hdl, msg);
			if (qpmd->received_on.tv_sec) {
				struct timespec delay;
				TS_DIFFERENCE( &delay, &qpmd->received_on, &pmd->sent_on );
				CHECK_MALLOC_DO( fd_msg_dump_summary(&buf, &len, NULL, msg, NULL, 0, 1), return );
				LOG_N("[TIMING] ANS %ld.%06ld sec ->'%s': %s", (long)delay.tv_sec, delay.tv_nsec / 1000, peer ? peer->info.pi_diamid : "<unidentified>", buf);
			}
		}
	}
		
	free(buf);
}

/* Entry point */
static int mt_main(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	CHECK_FCT( fd_hook_data_register( sizeof(struct fd_hook_permsgdata), NULL, NULL, &mt_data_hdl ) );
	
	CHECK_FCT( fd_hook_register( HOOK_MASK( HOOK_MESSAGE_RECEIVED, HOOK_MESSAGE_SENT, HOOK_DATA_RECEIVED ), 
					mt_hook_cb, NULL, mt_data_hdl, &mt_hdl) );
	
	return 0;
}

/* Cleanup */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	CHECK_FCT_DO( fd_hook_unregister( mt_hdl ), );
	return ;
}

EXTENSION_ENTRY("dbg_msg_timing", mt_main);
