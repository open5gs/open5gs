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

/* Manage incoming RADIUS messages. */

#include "rgw.h"

/* How many threads to handle messages in parallel ? */
#define NB_WORKERS	2

static pthread_t workers[NB_WORKERS];
static struct fifo * work_stack = NULL;

/* Data that is stacked */
struct work_item {
	struct rgw_radius_msg_meta * msg;
	struct rgw_client 	   * cli;
};

/* Data stored in freeDiameter while pending Diameter answer */
struct pending_answer {
	struct rgw_radius_msg_meta * rad;  /* the RADIUS message that was received and translated */
	struct rgw_client          * cli;  /* the client it was received from */
};

/* Callback when a Diameter answer is received */
static void receive_diam_answer(void * paback, struct msg **ans);

/* Worker thread, processing incoming RADIUS messages (after parsing) */
static void * work_th(void * arg)
{
	TRACE_ENTRY("%p", arg);
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "radgw/worker #%ld", (long)arg);
		fd_log_threadname ( buf );
	}
	
	while (1) { /* The thread will be cancelled */
		
		struct rgw_radius_msg_meta * msg;
		struct rgw_client * cli;
		struct msg * diam_msg;
		int pb, a;
		struct pending_answer * pa;
	
		/* Get the next incoming RADIUS message */
		{
			struct work_item * wi = NULL;
			
			CHECK_FCT_DO( fd_fifo_get(work_stack, &wi), break );

			msg = wi->msg;
			cli = wi->cli;
			free(wi);
		}
		
		TRACE_DEBUG(ANNOYING, "Processing next RADIUS message: %p received on client: %p", msg, cli);
	
		/* process the data */
		
		/* Check authenticator, if any */
		CHECK_FCT_DO( rgw_clients_auth_check(msg, cli, NULL),
			{
				/* An error occurred, discard message */
				rgw_msg_free(&msg);
				rgw_clients_dispose(&cli);
				continue;
			}  );
		
		/* Check duplicate */
		CHECK_FCT_DO( rgw_clients_check_dup(&msg, cli),
			{
				/* An error occurred, discard message */
				rgw_msg_free(&msg);
				rgw_clients_dispose(&cli);
				continue;
			}  );
		if (msg == NULL) {
			rgw_clients_dispose(&cli);
			continue; /* the message was a duplicate */
		}
		
		diam_msg = NULL;
		/* Note: after this point, the radius message buffer may not be consistent with the array of attributes anymore. */
	
		/* Check that IP is coherent with the identity in the message, and create an empty message with only Origin information */
		CHECK_FCT_DO( rgw_clients_create_origin(msg, cli, &diam_msg),
			{
				/* An error occurred, discard message */
				if (diam_msg) {
					CHECK_FCT_DO( fd_msg_free(diam_msg), );
				}
				rgw_msg_free(&msg);
				rgw_clients_dispose(&cli);
				continue;
			}  );
		
		/* Pass the message to the list of registered plugins */
		CHECK_FCT_DO( rgw_plg_loop_req(&msg, &diam_msg, cli), 
			{
				/* An error occurred, discard message */
				if (diam_msg) {
					CHECK_FCT_DO( fd_msg_free(diam_msg), );
					diam_msg = NULL;
				}
				rgw_msg_free(&msg);
				rgw_clients_dispose(&cli);
				continue;
			}  );
		if (msg == NULL) { /* Error or RADIUS answer locally generated */
			rgw_clients_dispose(&cli);
			if (diam_msg) {
				CHECK_FCT_DO( fd_msg_free(diam_msg), );
				diam_msg = NULL;
			}
			continue; /* the message was handled already */
		}
		
		pb = 0;
		
		/* Check the created Diameter message -- it will be invalid if no callback has handled the RADIUS message */
		if ((diam_msg == NULL) || ( fd_msg_parse_rules(diam_msg, fd_g_config->cnf_dict, NULL) ) ) {
			fd_log_debug("[radgw] No or invalid Diameter message was generated after processing the RADIUS command %hhd (%s)."
					" It may indicate a gateway configuration problem, or implementation issue in a plugin.",
					msg->radius.hdr->code, rgw_msg_code_str(msg->radius.hdr->code));
			/* We should also dump the conflicting rule here to help debug? */
			pb++;
		}
		
		/* Check if the full content of the RADIUS message was handled */
		for (a = 0; a < msg->radius.attr_used; a++) {
			struct radius_attr_hdr *attr = (struct radius_attr_hdr *)(msg->radius.buf + msg->radius.attr_pos[a]);
			pb++;
			fd_log_debug("[radgw] No plugin available to handle attribute %hhd (%s) in command %hhd (%s)! Translation aborted.",
					attr->type, rgw_msg_attrtype_str(attr->type),
					msg->radius.hdr->code, rgw_msg_code_str(msg->radius.hdr->code));
		}
		
		if (pb) {
			/* Something went wrong during the conversion */
			if (diam_msg) {
				CHECK_FCT_DO( fd_msg_free(diam_msg), );
				diam_msg = NULL;
			}
			
			rgw_msg_free(&msg);
			rgw_clients_dispose(&cli);
			
			TRACE_DEBUG(INFO, "%d problem(s) occurred while translating a RADIUS message, data discarded.", pb);
			continue;
		}
		
		/* Send the Diameter message and register for receiving the answer */
		CHECK_MALLOC_DO( pa = malloc(sizeof(struct pending_answer)), break );
		memset(pa, 0, sizeof(*pa));
		pa->rad = msg;
		pa->cli = cli;
		
		CHECK_FCT_DO( fd_msg_send( &diam_msg, receive_diam_answer, pa), 
			{
				/* If an error occurs, log and destroy the data */
				fd_log_debug("An error occurred while sending Diameter message, please turn Debug on for detail.");

				if (diam_msg) {
					CHECK_FCT_DO( fd_msg_free(diam_msg), );
					diam_msg = NULL;
				}

				rgw_msg_free(&msg);
				rgw_clients_dispose(&cli);
				
				free(pa);
				
				continue;
			} );
		
		/* Done! */
	}
	
	TRACE_DEBUG(INFO, "Thread terminated!");
	return NULL;
}

static void receive_diam_answer(void * paback, struct msg **ans)
{
	struct pending_answer * pa = (struct pending_answer *)paback;
	struct radius_msg * rad_ans;
	struct avp *avp;
	struct avp_hdr  *ahdr;
	int pb = 0;
	
	TRACE_ENTRY("%p %p", pa, ans);
	CHECK_PARAMS_DO( pa && ans, return );
	
	/* Create an empty RADIUS answer message */
	CHECK_MALLOC_DO( rad_ans = radius_msg_new(0, pa->rad->radius.hdr->identifier), goto out );
	
	/* Pass the Diameter answer to the same extensions as the request */
	CHECK_FCT_DO( rgw_plg_loop_ans(pa->rad, ans, &rad_ans, pa->cli), goto out );

	if (*ans != NULL) {

		/* Now check what AVPs remain in the diameter answer. If AVPs with the 'M' flag are here, we have a problem... */
		CHECK_FCT_DO( fd_msg_browse(*ans, MSG_BRW_FIRST_CHILD, &avp, NULL), { avp = NULL; pb++; } );
		while (avp) {
			CHECK_FCT_DO( fd_msg_avp_hdr ( avp, &ahdr ), { pb++; continue; } );
			if (ahdr->avp_flags & AVP_FLAG_MANDATORY) {
				if (ahdr->avp_flags & AVP_FLAG_VENDOR) {
					TRACE_DEBUG(FULL, "Remaining Mandatory Vendor AVP, code %d", ahdr->avp_code);
					pb++;
				} else {
					switch (ahdr->avp_code) {
						/* A few AVPs can be safely ignored here: */
						case DIAM_ATTR_SESSION_ID:
						case DIAM_ATTR_ROUTE_RECORD:
						case DIAM_ATTR_PROXY_INFO:


							/* just ignore */
							break;

						default:
							LOG_D("Remaining Mandatory AVP, code %d", ahdr->avp_code);
							pb++;
					}
				}
			}
			CHECK_FCT_DO( fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL), { pb++; break; } );
		}

		if (pb) {
			TRACE_DEBUG(INFO, "[radgw] WARNING: %d mandatory AVP in the Diameter answer have not been translated to RADIUS! Please use debug.rgwx for more information.", pb);
		}
	}
	
	
out:
	/* Now try and send the RADIUS answer */
	if (rad_ans) {
		CHECK_FCT_DO( rgw_client_finish_send(&rad_ans, pa->rad, pa->cli), );	
	} else {
		/* Remove the request from the duplicate cache */
		CHECK_FCT_DO( rgw_client_finish_nosend(pa->rad, pa->cli), );
	}

	/* Clear the Diameter message */
	if (*ans) {
		CHECK_FCT_DO( fd_msg_free(*ans),  );
		*ans = NULL;
	}
	
	/* Clear the RADIUS request */
	if (pa->rad) {
		rgw_msg_free(&pa->rad);
	}
	
	/* Release reference on the client */
	rgw_clients_dispose(&pa->cli);
	
	/* Clear the answer data */
	free(pa);
	
	/* Finished */
	return;
}

int rgw_work_start(void)
{
	long i;
	TRACE_ENTRY();
	
	memset(workers, 0, sizeof(workers));
	
	CHECK_FCT( fd_fifo_new ( &work_stack, 30 ) );
	
	/* Create the worker thread(s) */
	for (i = 0; i < NB_WORKERS; i++) {
		CHECK_POSIX( pthread_create(&workers[i], NULL, work_th, (void *)i) );
	}
	
	return 0;
}

int rgw_work_add(struct rgw_radius_msg_meta * msg, struct rgw_client * client)
{
	struct work_item * new;
	
	CHECK_MALLOC( new = malloc(sizeof(struct work_item)) );
	memset(new, 0, sizeof(struct work_item));
	
	new->msg = msg;
	new->cli = client;
	
	CHECK_FCT( fd_fifo_post(work_stack, &new) );
	
	return 0;
}

void rgw_work_fini(void)
{
	int i;
	TRACE_ENTRY();
	
	for (i = 0; i < NB_WORKERS; i++) {
		fd_thr_term(&workers[i]);
	}
	
	TODO("Empty the stack, what to do about the RADIUS messages?");
	
	return;
}
