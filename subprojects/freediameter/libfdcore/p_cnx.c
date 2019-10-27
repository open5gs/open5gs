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

#include "fdcore-internal.h"


/* TODO: change the behavior to handle properly forced ordering at beginning & end of OPEN state */

/* This file contains code used by a peer state machine to initiate a connection to remote peer */

struct next_conn {
	struct fd_list	chain;
	int		proto;	/* Protocol of the next attempt */
	union {
		sSS	ss;	/* The address, only for TCP */
		sSA4	sin;
		sSA6	sin6;
	};
	uint16_t	port;	/* The port, for SCTP (included in ss for TCP) */
	int		dotls;	/* Handshake TLS after connection ? */
};

static __inline__ void failed_connection_attempt(struct fd_peer * peer)
{
	/* Simply remove the first item in the list if not empty */
	if (! FD_IS_LIST_EMPTY(&peer->p_connparams) ) {
		struct fd_list * li = peer->p_connparams.next;
		fd_list_unlink(li);
		free(li);
	}
}

static void empty_connection_list(struct fd_peer * peer)
{
	/* Remove all items */
	while (!FD_IS_LIST_EMPTY(&peer->p_connparams)) {
		failed_connection_attempt(peer);
	}
}

static int prepare_connection_list(struct fd_peer * peer)
{
	struct fd_list * li, *last_prio;
	struct next_conn   * new; 
	
	uint16_t	port_no; /* network order */
	int		dotls_immediate;
	int count = 0;
	
	TRACE_ENTRY("%p", peer);
	 
	/* Resolve peer address(es) if needed */
	if (FD_IS_LIST_EMPTY(&peer->p_hdr.info.pi_endpoints)) {
		struct addrinfo hints, *ai, *aip;
		int ret;

		memset(&hints, 0, sizeof(hints));
		hints.ai_flags = AI_ADDRCONFIG;
		ret = getaddrinfo(peer->p_hdr.info.pi_diamid, NULL, &hints, &ai);
		if (ret) {
			TRACE_DEBUG(INFO, "Unable to resolve address for peer '%s' (%s), aborting", peer->p_hdr.info.pi_diamid, gai_strerror(ret));
			if (ret != EAI_AGAIN)
				fd_psm_terminate( peer, NULL );
			return 0;
		}
		
		for (aip = ai; aip != NULL; aip = aip->ai_next) {
			CHECK_FCT( fd_ep_add_merge( &peer->p_hdr.info.pi_endpoints, aip->ai_addr, aip->ai_addrlen, EP_FL_DISC ) );
		}
		freeaddrinfo(ai);
	}
	
	/* Remove addresses from unwanted family */
	if (peer->p_hdr.info.config.pic_flags.pro3) {
		CHECK_FCT( fd_ep_filter_family(
					&peer->p_hdr.info.pi_endpoints, 
					(peer->p_hdr.info.config.pic_flags.pro3 == PI_P3_IP) ? 
						AF_INET 
						: AF_INET6));
	}
	if (fd_g_config->cnf_flags.no_ip4) {
		CHECK_FCT( fd_ep_filter_family(
					&peer->p_hdr.info.pi_endpoints, 
					AF_INET6));
	}
	if (fd_g_config->cnf_flags.no_ip6) {
		CHECK_FCT( fd_ep_filter_family(
					&peer->p_hdr.info.pi_endpoints, 
					AF_INET));
	}
	
	/* We don't use the alternate addresses that were sent by the remote peer */
	CHECK_FCT( fd_ep_clearflags(&peer->p_hdr.info.pi_endpoints, EP_FL_ADV) );
	
	
	/* Now check we have at least one address to attempt */
	if (FD_IS_LIST_EMPTY(&peer->p_hdr.info.pi_endpoints)) {
		TRACE_DEBUG(INFO, "No address %savailable to connect to peer '%s', aborting", 
					peer->p_hdr.info.config.pic_flags.pro3 ? "in the configured family " : "", peer->p_hdr.info.pi_diamid);
		fd_psm_terminate( peer, NULL );
		return 0;
	}
	
	/* Check if we are able to communicate with this peer */
	if (fd_g_config->cnf_sec_data.tls_disabled && ( peer->p_hdr.info.config.pic_flags.sec != PI_SEC_NONE)) {
		LOG_E("Peer '%s' not configured for No_TLS and TLS is locally disabled; giving up connection attempts", 
					peer->p_hdr.info.pi_diamid);
		fd_psm_terminate( peer, NULL );
		return 0;
	}
	
	/* Cleanup any previous list */
	empty_connection_list(peer);
	
	/* Prepare the parameters */
	if ((peer->p_hdr.info.config.pic_flags.sec != PI_SEC_DEFAULT) || (fd_g_config->cnf_flags.tls_alg)) {
		dotls_immediate = 0;
		port_no = htons(peer->p_hdr.info.config.pic_port ?: DIAMETER_PORT);
	} else {
		dotls_immediate = 1;
		port_no = htons(peer->p_hdr.info.config.pic_port ?: DIAMETER_SECURE_PORT);
	}
	
	last_prio = &peer->p_connparams;
	
	/* Create TCP parameters unless specified otherwise */
	if ((!fd_g_config->cnf_flags.no_tcp) && (peer->p_hdr.info.config.pic_flags.pro4 != PI_P4_SCTP)) {
		for (li = peer->p_hdr.info.pi_endpoints.next; li != &peer->p_hdr.info.pi_endpoints; li = li->next) {
			struct fd_endpoint * ep = (struct fd_endpoint *)li;
			
			CHECK_MALLOC( new = malloc(sizeof(struct next_conn)) );
			memset(new, 0, sizeof(struct next_conn));
			fd_list_init(&new->chain, new);
			
			new->proto = IPPROTO_TCP;
			
			memcpy( &new->ss, &ep->ss, sizeof(sSS) );
			switch (new->ss.ss_family) {
				case AF_INET:
					new->sin.sin_port = port_no;
					break;
				case AF_INET6:
					new->sin6.sin6_port = port_no;
					break;
				default:
					free(new);
					continue; /* Move to the next endpoint */
			}
			
			new->dotls = dotls_immediate;
			
			/* Add the new entry to the appropriate position (conf and disc go first) */
			if (ep->flags & (EP_FL_CONF | EP_FL_DISC)) {
				fd_list_insert_after(last_prio, &new->chain);
				last_prio = &new->chain;
			} else {
				fd_list_insert_before(&peer->p_connparams, &new->chain);
			}
			count++;
		}
	}
	
	/* Now, add the SCTP entry, if not disabled */
#ifndef DISABLE_SCTP
	if ((!fd_g_config->cnf_flags.no_sctp) && (peer->p_hdr.info.config.pic_flags.pro4 != PI_P4_TCP)) {
		struct next_conn   * new;
		
		CHECK_MALLOC( new = malloc(sizeof(struct next_conn)) );
		memset(new, 0, sizeof(struct next_conn));
		fd_list_init(&new->chain, new);

		new->proto = IPPROTO_SCTP;
		new->port  = ntohs(port_no); /* back to host byte order... */
		new->dotls = dotls_immediate;

		/* Add the new entry to the appropriate position (depending on preferences) */
		if ((fd_g_config->cnf_flags.pr_tcp) || (peer->p_hdr.info.config.pic_flags.alg == PI_ALGPREF_TCP)) {
			fd_list_insert_after(last_prio, &new->chain);
		} else {
			fd_list_insert_after(&peer->p_connparams, &new->chain); /* very first position */
		}
		count++;
	}
#endif /* DISABLE_SCTP */
	
	LOG_D("Prepared %d sets of connection parameters to peer %s", count, peer->p_hdr.info.pi_diamid);
	
	return 0;
}


/* The thread that attempts the connection */
static void * connect_thr(void * arg)
{
	struct fd_peer * peer = arg;
	struct cnxctx * cnx = NULL;
	struct next_conn * nc = NULL;
	int rebuilt = 0;
	int fatal_error=0;
	
	TRACE_ENTRY("%p", arg);
	CHECK_PARAMS_DO( CHECK_PEER(peer), return NULL );

	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "ConnTo:%s", peer->p_hdr.info.pi_diamid);
		fd_log_threadname ( buf );
	}
	
	do {
		/* Rebuild the list if needed, if it is empty -- but at most once */
		if (FD_IS_LIST_EMPTY(&peer->p_connparams)) {
			if (! rebuilt) {
				CHECK_FCT_DO( fatal_error = prepare_connection_list(peer), goto out );
				rebuilt ++;
			}
			if (FD_IS_LIST_EMPTY(&peer->p_connparams)) {
				/* We encountered an error or we have looped over all the addresses of the peer. */
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "All connection attempts failed, will retry later", NULL);
				
				CHECK_FCT_DO( fatal_error = fd_event_send(peer->p_events, FDEVP_CNX_FAILED, 0, NULL), goto out );
				return NULL;
			}
		}
		
		/* Attempt connection to the first entry */
		nc = (struct next_conn *)(peer->p_connparams.next);
		
		switch (nc->proto) {
			case IPPROTO_TCP:
				cnx = fd_cnx_cli_connect_tcp((sSA *)&nc->ss, sSAlen(&nc->ss));
				break;
#ifndef DISABLE_SCTP			
			case IPPROTO_SCTP:
				cnx = fd_cnx_cli_connect_sctp((peer->p_hdr.info.config.pic_flags.pro3 == PI_P3_IP) ? 1 : fd_g_config->cnf_flags.no_ip6, 
							nc->port, &peer->p_hdr.info.pi_endpoints);
				break;
#endif /* DISABLE_SCTP */
		}
		
		if (cnx)
			break;
		
		/* Pop these parameters and continue */
		failed_connection_attempt(peer);
		
		pthread_testcancel();
		
	} while (!cnx); /* and until cancellation or all addresses attempted without success */
	
	/* Now, we have an established connection in cnx */
	
	pthread_cleanup_push((void *)fd_cnx_destroy, cnx);
	
	/* Set the hostname in the connection, so that handshake verifies the remote identity */
	fd_cnx_sethostname(cnx,peer->p_hdr.info.pi_diamid);
	
	/* Handshake if needed (secure port) */
	if (nc->dotls) {
		CHECK_FCT_DO( fd_cnx_handshake(cnx, GNUTLS_CLIENT, 
						ALGO_HANDSHAKE_3436,
						peer->p_hdr.info.config.pic_priority, NULL),
			{
				/* Handshake failed ...  */
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "TLS Handshake failed", NULL);
				fd_cnx_destroy(cnx);
				empty_connection_list(peer);
				fd_ep_filter(&peer->p_hdr.info.pi_endpoints, EP_FL_CONF);
				goto out_pop;
			} );
		LOG_A("%s: TLS handshake successful.", peer->p_hdr.info.pi_diamid);
	} else {
		/* Prepare to receive the next message */
		CHECK_FCT_DO( fatal_error = fd_cnx_start_clear(cnx, 0), goto out_pop );
	}
	
	/* Upon success, generate FDEVP_CNX_ESTABLISHED */
	CHECK_FCT_DO( fatal_error = fd_event_send(peer->p_events, FDEVP_CNX_ESTABLISHED, 0, cnx),  );
out_pop:
	;	
	pthread_cleanup_pop(0);
	
out:
	
	if (fatal_error) {
	
		/* Cleanup the connection */
		if (cnx)
			fd_cnx_destroy(cnx);

		/* Generate a termination event */
		CHECK_FCT_DO(fd_core_shutdown(), );
	}
	
	return NULL;
}


/* Initiate a connection attempt to a remote peer */
int fd_p_cnx_init(struct fd_peer * peer)
{
	TRACE_ENTRY("%p", peer);
	
	/* Start the connect thread */
	CHECK_FCT( pthread_create(&peer->p_ini_thr, NULL, connect_thr, peer) );
	return 0;
}

/* Cancel a connection attempt */
void fd_p_cnx_abort(struct fd_peer * peer, int cleanup_all)
{
	TRACE_ENTRY("%p %d", peer, cleanup_all);
	CHECK_PARAMS_DO( CHECK_PEER(peer), return );
	
	if (peer->p_ini_thr != (pthread_t)NULL) {
		CHECK_FCT_DO( fd_thr_term(&peer->p_ini_thr), /* continue */);
		failed_connection_attempt(peer);
	}
	
	if (cleanup_all) {
		empty_connection_list(peer);
	}
}

