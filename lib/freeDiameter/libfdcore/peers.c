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

/* Global list of peers */
struct fd_list   fd_g_peers = FD_LIST_INITIALIZER(fd_g_peers);
pthread_rwlock_t fd_g_peers_rw = PTHREAD_RWLOCK_INITIALIZER;

/* List of active peers */
struct fd_list   fd_g_activ_peers = FD_LIST_INITIALIZER(fd_g_activ_peers);	/* peers linked by their p_actives oredered by p_diamid */
pthread_rwlock_t fd_g_activ_peers_rw = PTHREAD_RWLOCK_INITIALIZER;

/* List of validation callbacks (registered with fd_peer_validate_register) */
static struct fd_list validators = FD_LIST_INITIALIZER(validators);	/* list items are simple fd_list with "o" pointing to the callback */
static pthread_rwlock_t validators_rw = PTHREAD_RWLOCK_INITIALIZER;


/* Alloc / reinit a peer structure. if *ptr is not NULL, it must already point to a valid struct fd_peer. */
int fd_peer_alloc(struct fd_peer ** ptr)
{
	struct fd_peer *p;
	
	TRACE_ENTRY("%p", ptr);
	CHECK_PARAMS(ptr);
	
	if (*ptr) {
		p = *ptr;
	} else {
		CHECK_MALLOC( p = malloc(sizeof(struct fd_peer)) );
		*ptr = p;
	}
	
	/* Now initialize the content */
	memset(p, 0, sizeof(struct fd_peer));
	
	fd_list_init(&p->p_hdr.chain, p);
	
	fd_list_init(&p->p_hdr.info.pi_endpoints, p);
	fd_list_init(&p->p_hdr.info.runtime.pir_apps, p);
	
	p->p_eyec = EYEC_PEER;
	CHECK_POSIX( pthread_mutex_init(&p->p_state_mtx, NULL) );
	
	fd_list_init(&p->p_actives, p);
	fd_list_init(&p->p_expiry, p);
	CHECK_FCT( fd_fifo_new(&p->p_tosend, 5) );
	CHECK_FCT( fd_fifo_new(&p->p_tofailover, 0) );
	p->p_hbh = lrand48();
	
	fd_list_init(&p->p_sr.srs, p);
	fd_list_init(&p->p_sr.exp, p);
	CHECK_POSIX( pthread_mutex_init(&p->p_sr.mtx, NULL) );
	CHECK_POSIX( pthread_cond_init(&p->p_sr.cnd, NULL) );
	
	fd_list_init(&p->p_connparams, p);
	
	return 0;
}

/* Add a new peer entry */
int fd_peer_add ( struct peer_info * info, const char * orig_dbg, void (*cb)(struct peer_info *, void *), void * cb_data )
{
	struct fd_peer *p = NULL;
	struct fd_list * li, *li_inf;
	int ret = 0;
	
	TRACE_ENTRY("%p %p %p %p", info, orig_dbg, cb, cb_data);
	CHECK_PARAMS(info && info->pi_diamid);
	
	if (info->config.pic_realm) {
		if (!fd_os_is_valid_DiameterIdentity((os0_t)info->config.pic_realm, strlen(info->config.pic_realm))) {
			TRACE_DEBUG(INFO, "'%s' is not a valid DiameterIdentity.", info->config.pic_realm);
			return EINVAL;
		}
	}
	
	/* Create a structure to contain the new peer information */
	CHECK_FCT( fd_peer_alloc(&p) );
	
	/* Copy the informations from the parameters received */
	p->p_hdr.info.pi_diamid = info->pi_diamid;
	CHECK_FCT( fd_os_validate_DiameterIdentity(&p->p_hdr.info.pi_diamid, &p->p_hdr.info.pi_diamidlen, 1) );
	
	memcpy( &p->p_hdr.info.config, &info->config, sizeof(p->p_hdr.info.config) );
	
	/* Duplicate the strings if provided */
	if (info->config.pic_realm) {
		CHECK_MALLOC( p->p_hdr.info.config.pic_realm = strdup(info->config.pic_realm) );
	}
	if (info->config.pic_priority) {
		CHECK_MALLOC( p->p_hdr.info.config.pic_priority = strdup(info->config.pic_priority) );
	}
	
	/* Move the list of endpoints into the peer */
	if (info->pi_endpoints.next)
		while (!FD_IS_LIST_EMPTY( &info->pi_endpoints ) ) {
			li = info->pi_endpoints.next;
			fd_list_unlink(li);
			fd_list_insert_before(&p->p_hdr.info.pi_endpoints, li);
		}
	
	/* The internal data */
	if (orig_dbg) {
		CHECK_MALLOC( p->p_dbgorig = strdup(orig_dbg) );
	} else {
		CHECK_MALLOC( p->p_dbgorig = strdup("unspecified") );
	}
	p->p_cb = cb;
	p->p_cb_data = cb_data;
	
	/* Ok, now check if we don't already have an entry with the same Diameter Id, and insert this one */
	CHECK_POSIX( pthread_rwlock_wrlock(&fd_g_peers_rw) );
	li_inf = &fd_g_peers;
	for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
		struct fd_peer * next = (struct fd_peer *)li;
		int cont;
		int cmp = fd_os_almostcasesrch( p->p_hdr.info.pi_diamid, p->p_hdr.info.pi_diamidlen, 
						next->p_hdr.info.pi_diamid, next->p_hdr.info.pi_diamidlen,
						&cont );
		if (cmp > 0)
			li_inf = li; /* it will come after this element, for sure */
		
		if (cmp == 0) {
			if (fd_peer_getstate(li) == STATE_ZOMBIE) {
				/* If the existing peer is in zombie state then we can add it again, so
				 * continue as if we hadn't found it */
				li_inf = li;
			} else {
				ret = EEXIST; /* we have a duplicate */
				break;
			}
		}

		if (!cont)
			break;
	}
	
	/* We can insert the new peer object */
	if (! ret)
		do {
			/* Update expiry list */
			CHECK_FCT_DO( ret = fd_p_expi_update( p ), break );

			/* Insert the new element in the list */
			fd_list_insert_after( li_inf, &p->p_hdr.chain );
		} while (0);

	CHECK_POSIX( pthread_rwlock_unlock(&fd_g_peers_rw) );
	if (ret) {
		CHECK_FCT( fd_peer_free(&p) );
	} else {
		char* peers_str = NULL;
		size_t peers_str_len;
		fd_peer_dump_list(&peers_str, &peers_str_len, NULL, 0);
		TRACE_DEBUG(FULL, "Diameter peer %.*s added", (int)info->pi_diamidlen, info->pi_diamid);
		TRACE_DEBUG(FULL, "New global list of peers:\n%.*s", (int)peers_str_len, peers_str);
		free(peers_str); peers_str = NULL;

		CHECK_FCT( fd_psm_begin(p) );
	}
	return ret;
}

// Remove a peer entry.
int fd_peer_remove ( DiamId_t diamid, size_t diamidlen )
{
	struct fd_list * li;
	TRACE_DEBUG(FULL, "Remove diameter peer %.*s", (int)diamidlen, diamid);
	int found = 0;

	// Find the peer in the peer list from its pi_diamid.
	CHECK_POSIX( pthread_rwlock_wrlock(&fd_g_peers_rw) );
	for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
		struct fd_peer * peer = (struct fd_peer *)li;
		int cmp = fd_os_cmp( diamid, diamidlen, peer->p_hdr.info.pi_diamid, peer->p_hdr.info.pi_diamidlen );

		if ((cmp == 0) && (fd_peer_getstate(peer) != STATE_ZOMBIE)) {
			/* update the peer lifetime, set the expiry flag and call fd_p_expi_update so that the
			 * p_exp_timer value is updated and the peer expires immediately. */
			peer->p_hdr.info.config.pic_flags.exp = PI_EXP_INACTIVE;
			peer->p_hdr.info.config.pic_lft = 0;
			CHECK_FCT( fd_p_expi_update(peer) );
			found = 1;
			break;
		}
	}

	if (!found)
	{
		TRACE_ERROR("Diameter peer %*s not valid - caller is out of sync", (int)diamidlen, diamid);
	}

	CHECK_POSIX( pthread_rwlock_unlock(&fd_g_peers_rw) );

	return 0;
}

/* Search for a peer */
int fd_peer_getbyid( DiamId_t diamid, size_t diamidlen, int igncase, struct peer_hdr ** peer )
{
	struct fd_list * li;
	TRACE_ENTRY("%p %zd %d %p", diamid, diamidlen, igncase, peer);
	CHECK_PARAMS( diamid && diamidlen && peer );
	
	*peer = NULL;
	struct peer_hdr* zombie_peer = NULL;
	
	/* Search in the list */
	CHECK_POSIX( pthread_rwlock_rdlock(&fd_g_peers_rw) );
	if (igncase) {
		for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
			struct fd_peer * next = (struct fd_peer *)li;
			int cmp, cont;
			cmp = fd_os_almostcasesrch( diamid, diamidlen, next->p_hdr.info.pi_diamid, next->p_hdr.info.pi_diamidlen, &cont );
			if (cmp == 0) {
				if (fd_peer_getstate(next) != STATE_ZOMBIE) {
					*peer = &next->p_hdr;
					break;
				}
				else if (zombie_peer == NULL) {
					zombie_peer = &next->p_hdr;
				}
			}
			if (!cont)
				break;
		}
	} else {
		for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
			struct fd_peer * next = (struct fd_peer *)li;
			int cmp = fd_os_cmp( diamid, diamidlen, next->p_hdr.info.pi_diamid, next->p_hdr.info.pi_diamidlen );
			if (cmp > 0)
				continue;
			if (cmp == 0) {
				if (fd_peer_getstate(next) != STATE_ZOMBIE) {
					*peer = &next->p_hdr;
					break;
				}
				else if (zombie_peer == NULL) {
					zombie_peer = &next->p_hdr;
				}
			}
		}
	}
	CHECK_POSIX( pthread_rwlock_unlock(&fd_g_peers_rw) );

	if ((peer == NULL) && (zombie_peer != NULL))
	{
		*peer = zombie_peer;
	}
	
	return 0;
}


#define free_null( _v ) 	\
	if (_v) {		\
		free(_v);	\
		(_v) = NULL;	\
	}
	
#define free_list( _l ) 						\
	while (!FD_IS_LIST_EMPTY(_l)) {					\
		struct fd_list * __li = ((struct fd_list *)(_l))->next;	\
		fd_list_unlink(__li);					\
		free(__li);						\
	}

/* Empty the lists of p_tosend, p_failover, and p_sentreq messages */
void fd_peer_failover_msg(struct fd_peer * peer)
{
	struct msg *m;
	TRACE_ENTRY("%p", peer);
	CHECK_PARAMS_DO(CHECK_PEER(peer), return);
	
	/* Requeue all messages in the "out" queue */
	while ( fd_fifo_tryget(peer->p_tosend, &m) == 0 ) {
		/* but only if they are routable */
		if (fd_msg_is_routable(m)) {
			fd_hook_call(HOOK_MESSAGE_FAILOVER, m, peer, NULL, fd_msg_pmdl_get(m));
			CHECK_FCT_DO(fd_fifo_post_noblock(fd_g_outgoing, (void *)&m), 
				{
					/* fallback: destroy the message */
					fd_hook_call(HOOK_MESSAGE_DROPPED, m, NULL, "Internal error: unable to requeue this message during failover process", fd_msg_pmdl_get(m));
					CHECK_FCT_DO(fd_msg_free(m), /* What can we do more? */)
				} );
		} else {
			/* Just free it */
			/* fd_hook_call(HOOK_MESSAGE_DROPPED, m, NULL, "Non-routable message freed during handover", fd_msg_pmdl_get(m)); */
			CHECK_FCT_DO(fd_msg_free(m), /* What can we do more? */)
		}
	}
	
	/* Requeue all messages in the "failover" queue */
	while ( fd_fifo_tryget(peer->p_tofailover, &m) == 0 ) {
		fd_hook_call(HOOK_MESSAGE_FAILOVER, m, peer, NULL, fd_msg_pmdl_get(m));
		CHECK_FCT_DO(fd_fifo_post_noblock(fd_g_outgoing, (void *)&m), 
			{
				/* fallback: destroy the message */
				fd_hook_call(HOOK_MESSAGE_DROPPED, m, NULL, "Internal error: unable to requeue this message during failover process", fd_msg_pmdl_get(m));
				CHECK_FCT_DO(fd_msg_free(m), /* What can we do more? */)
			} );
	}
	
	/* Requeue all routable sent requests */
	fd_p_sr_failover(&peer->p_sr);
	
	/* Done */
	return;
}

/* Describe the current connection */
int fd_peer_cnx_proto_info(struct peer_hdr *peer, char * buf, size_t len)
{
	struct fd_peer * p = (struct fd_peer *)peer;
	TRACE_ENTRY("%p %p %zd", peer, buf, len);
	CHECK_PARAMS(CHECK_PEER(peer) && buf && len);
	
	if (p->p_cnxctx) {
		CHECK_FCT(fd_cnx_proto_info(p->p_cnxctx, buf, len));
	} else if (p->p_receiver) {
		CHECK_FCT(fd_cnx_proto_info(p->p_receiver, buf, len));
	} else {
		snprintf(buf, len, "Not Connected");
	}
	
	return 0;
}

/* Output the remote IP address and port of a peer */
int fd_peer_cnx_remote_ip_port(struct peer_hdr *peer, char * ip_buf, size_t ip_len, unsigned short * port)
{
	struct fd_peer * p = (struct fd_peer *)peer;
	if (p->p_cnxctx == NULL) {
		return -1;
	}
	return fd_cnx_remote_ip_port(p->p_cnxctx, ip_buf, ip_len, port);
}

/* Output the local IP address and port used to connect to a peer */
int fd_peer_cnx_local_ip_port(struct peer_hdr *peer, char * ip_buf, size_t ip_len, unsigned short * port)
{
	struct fd_peer * p = (struct fd_peer *)peer;
	if (p->p_cnxctx == NULL) {
		return -1;
	}
	return fd_cnx_local_ip_port(p->p_cnxctx, ip_buf, ip_len, port);
}


/* Return the value of srlist->cnt */
int fd_peer_get_load_pending(struct peer_hdr *peer, long * to_receive, long * to_send)
{
	struct fd_peer * p = (struct fd_peer *)peer;
	TRACE_ENTRY("%p %p %p", peer, to_receive, to_send);
	CHECK_PARAMS(CHECK_PEER(peer));
	
	if (to_receive) {
		CHECK_POSIX( pthread_mutex_lock(&p->p_sr.mtx) );
		*to_receive = p->p_sr.cnt;
		CHECK_POSIX( pthread_mutex_unlock(&p->p_sr.mtx) );
	}
	if (to_send) {
		CHECK_POSIX( pthread_mutex_lock(&p->p_state_mtx) );
		*to_send = p->p_reqin_count;
		CHECK_POSIX( pthread_mutex_unlock(&p->p_state_mtx) );
	}
	
	return 0;
}


/* Destroy a structure once cleanups have been performed (fd_psm_abord, ...) */
int fd_peer_free(struct fd_peer ** ptr)
{
	struct fd_peer *p;
	
	TRACE_ENTRY("%p", ptr);
	CHECK_PARAMS(ptr);
	p = *ptr;
	*ptr = NULL;
	CHECK_PARAMS(p);
	
	CHECK_PARAMS( FD_IS_LIST_EMPTY(&p->p_hdr.chain) );
	
	free_null(p->p_hdr.info.pi_diamid);
	
	free_null(p->p_hdr.info.config.pic_realm); 
	free_null(p->p_hdr.info.config.pic_priority); 
	
	free_null(p->p_hdr.info.runtime.pir_host);
	free_null(p->p_hdr.info.runtime.pir_realm);
	free_null(p->p_hdr.info.runtime.pir_prodname);
	free_list( &p->p_hdr.info.runtime.pir_apps );
	
	free_list( &p->p_hdr.info.pi_endpoints );
	
	free_null(p->p_dbgorig);
	
	fd_list_unlink(&p->p_expiry);
	fd_list_unlink(&p->p_actives);
	
	CHECK_FCT_DO( fd_fifo_del(&p->p_tosend), /* continue */ );
	CHECK_FCT_DO( fd_fifo_del(&p->p_tofailover), /* continue */ );
	CHECK_POSIX_DO( pthread_mutex_destroy(&p->p_state_mtx), /* continue */);
	CHECK_POSIX_DO( pthread_mutex_destroy(&p->p_sr.mtx), /* continue */);
	CHECK_POSIX_DO( pthread_cond_destroy(&p->p_sr.cnd), /* continue */);
	
	/* If the callback is still around... */
	if (p->p_cb)
		(*p->p_cb)(NULL, p->p_cb_data);
	
	/* Free the structure */
	free(p);
	return 0;
}

/* Terminate peer module (destroy all peers, first gently, then violently) */
int fd_peer_fini()
{
	struct fd_list * li;
	struct fd_list purge = FD_LIST_INITIALIZER(purge); /* Store zombie peers here */
	int list_empty;
	struct timespec	wait_until, now;
	
	TRACE_ENTRY();
	
	CHECK_FCT_DO(fd_p_expi_fini(), /* continue */);
	
	TRACE_DEBUG(INFO, "Sending terminate signal to all peer connections");
	
	CHECK_FCT_DO( pthread_rwlock_wrlock(&fd_g_peers_rw), /* continue */ );
	for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
		struct fd_peer * peer = (struct fd_peer *)li->o;
		
		if (fd_peer_getstate(peer) != STATE_ZOMBIE) {
			CHECK_FCT_DO( fd_psm_terminate(peer, "REBOOTING"), /* continue */ );
		} else {
			li = li->prev; /* to avoid breaking the loop */
			fd_list_unlink(&peer->p_hdr.chain);
			fd_list_insert_before(&purge, &peer->p_hdr.chain);
		}
	}
	list_empty = FD_IS_LIST_EMPTY(&fd_g_peers);
	CHECK_FCT_DO( pthread_rwlock_unlock(&fd_g_peers_rw), /* continue */ );
	
	if (!list_empty) {
		CHECK_SYS(  clock_gettime(CLOCK_REALTIME, &now)  );
		fd_psm_start(); /* just in case */
		TRACE_DEBUG(INFO, "Waiting for connections shutdown... (%d sec max)", DPR_TIMEOUT + 1);
		wait_until.tv_sec  = now.tv_sec + DPR_TIMEOUT + 1;
		wait_until.tv_nsec = now.tv_nsec;
	}
	
	while ((!list_empty) && (TS_IS_INFERIOR(&now, &wait_until))) {
		
		/* Allow the PSM(s) to execute */
		usleep(100000);
		
		/* Remove zombie peers */
		CHECK_FCT_DO( pthread_rwlock_wrlock(&fd_g_peers_rw), /* continue */ );
		for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
			struct fd_peer * peer = (struct fd_peer *)li->o;
			if (fd_peer_getstate(peer) == STATE_ZOMBIE) {
				li = li->prev; /* to avoid breaking the loop */
				fd_list_unlink(&peer->p_hdr.chain);
				fd_list_insert_before(&purge, &peer->p_hdr.chain);
			}
		}
		list_empty = FD_IS_LIST_EMPTY(&fd_g_peers);
		CHECK_FCT_DO( pthread_rwlock_unlock(&fd_g_peers_rw), /* continue */ );
		CHECK_SYS(  clock_gettime(CLOCK_REALTIME, &now)  );
	}

        fd_peer_fini_force();

        /* Free memory objects of all peers */
        while (!FD_IS_LIST_EMPTY(&purge)) {
                struct fd_peer * peer = (struct fd_peer *)(purge.next->o);
                fd_list_unlink(&peer->p_hdr.chain);
                fd_peer_free(&peer);
        }

        /* Now empty the validators list */
        CHECK_FCT_DO( pthread_rwlock_wrlock(&validators_rw), /* continue */ );
        while (!FD_IS_LIST_EMPTY( &validators )) {
                struct fd_list * v = validators.next;
                fd_list_unlink(v);
                free(v);
        }
        CHECK_FCT_DO( pthread_rwlock_unlock(&validators_rw), /* continue */ );
	
	return 0;
}

/* Terminate peer module (destroy all peers forcibly) */
int fd_peer_fini_force()
{
        struct fd_list * li;
        struct fd_list purge = FD_LIST_INITIALIZER(purge); /* Store zombie peers here */
        int list_empty;

        CHECK_FCT_DO( pthread_rwlock_wrlock(&fd_g_peers_rw), /* continue */ );
        list_empty = FD_IS_LIST_EMPTY(&fd_g_peers);
        CHECK_FCT_DO( pthread_rwlock_unlock(&fd_g_peers_rw), /* continue */ );

        if (!list_empty) {
                TRACE_DEBUG(INFO, "Forcing connections shutdown");
                CHECK_FCT_DO( pthread_rwlock_wrlock(&fd_g_peers_rw), /* continue */ );
                while (!FD_IS_LIST_EMPTY(&fd_g_peers)) {
                        struct fd_peer * peer = (struct fd_peer *)(fd_g_peers.next->o);
                        fd_psm_abord(peer);
                        fd_list_unlink(&peer->p_hdr.chain);
                        fd_list_insert_before(&purge, &peer->p_hdr.chain);
                }
                CHECK_FCT_DO( pthread_rwlock_unlock(&fd_g_peers_rw), /* continue */ );
        }

        /* Free memory objects of all peers */
        while (!FD_IS_LIST_EMPTY(&purge)) {
                struct fd_peer * peer = (struct fd_peer *)(purge.next->o);
                fd_list_unlink(&peer->p_hdr.chain);
                fd_peer_free(&peer);
        }
}

/* Dump info of one peer */
DECLARE_FD_DUMP_PROTOTYPE(fd_peer_dump, struct peer_hdr * p, int details)
{
	FD_DUMP_HANDLE_OFFSET();
	
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "{peer}(@%p): ", p), return NULL);
	
	if (!CHECK_PEER(p)) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "INVALID/NULL"), return NULL);
	} else {
		struct fd_peer * peer = (struct fd_peer *)p;
		
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "%s [%s, cnt:%ldsr,%ldpa]", peer->p_hdr.info.pi_diamid, STATE_STR(fd_peer_getstate(peer)), peer->p_sr.cnt, peer->p_reqin_count), return NULL);
		if (details > 0) {
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, " hst:%s", peer->p_hdr.info.runtime.pir_host ?: "<unknown>"), return NULL);
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, " rlm:%s", peer->p_hdr.info.runtime.pir_realm ?: "<unknown>"), return NULL);
			if (peer->p_hdr.info.runtime.pir_prodname) {
				CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, " ['%s' %u]", peer->p_hdr.info.runtime.pir_prodname, peer->p_hdr.info.runtime.pir_firmrev), return NULL);
			}
		}
		if (details > 1) {
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, " [from:%s] flags:%s%s%s%s%s%s%s%s lft:%ds", 
				peer->p_dbgorig ?: "unset",
				peer->p_hdr.info.config.pic_flags.pro3 == PI_P3_DEFAULT ? "-" :
					(peer->p_hdr.info.config.pic_flags.pro3 == PI_P3_IP ? "4" : "6"),
				peer->p_hdr.info.config.pic_flags.pro4 == PI_P4_DEFAULT ? "-" :
					(peer->p_hdr.info.config.pic_flags.pro4 == PI_P4_TCP ? "T" : "S"),
				peer->p_hdr.info.config.pic_flags.alg ? "P" : "-",
				peer->p_hdr.info.config.pic_flags.sec & PI_SEC_NONE ? "N" :"-",
				peer->p_hdr.info.config.pic_flags.sec & PI_SEC_TLS_OLD ? "O" :"-",
				peer->p_hdr.info.config.pic_flags.sctpsec & PI_SCTPSEC_3436 ? "3" :"-",
				peer->p_hdr.info.config.pic_flags.exp ? "E" : "-",
				peer->p_hdr.info.config.pic_flags.persist ? "P" : "-",
				peer->p_hdr.info.config.pic_lft), return NULL);
		}
	
	}
	
	return *buf;
}

/* Dump the list of peers */
DECLARE_FD_DUMP_PROTOTYPE(fd_peer_dump_list, int details)
{
	struct fd_list * li;
	FD_DUMP_HANDLE_OFFSET();
	
	CHECK_FCT_DO( pthread_rwlock_rdlock(&fd_g_peers_rw), /* continue */ );
	
	for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
		CHECK_MALLOC_DO( fd_peer_dump(FD_DUMP_STD_PARAMS, (struct peer_hdr *)li->o, details), break);
		if (li->next != &fd_g_peers) {
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "\n"), break);
		}
	}
	
	CHECK_FCT_DO( pthread_rwlock_unlock(&fd_g_peers_rw), /* continue */ );
	return *buf;
}

static struct dict_object *avp_oh_model = NULL;
static pthread_mutex_t cache_avp_lock = PTHREAD_MUTEX_INITIALIZER;

/* Handle an incoming CER request on a new connection */
int fd_peer_handle_newCER( struct msg ** cer, struct cnxctx ** cnx )
{
	struct msg * msg;
	struct avp *avp_oh;
	struct avp_hdr * avp_hdr;
	struct fd_list * li, *li_inf;
	int found = 0;
	int ret = 0;
	struct fd_peer * peer;
	struct cnx_incoming * ev_data;
	
	TRACE_ENTRY("%p %p", cer, cnx);
	CHECK_PARAMS(cer && *cer && cnx && *cnx);
	
	msg = *cer; 
	
	/* If needed, resolve the dictionary model for Origin-Host */
	CHECK_POSIX( pthread_mutex_lock(&cache_avp_lock) );
	if (!avp_oh_model) {
		avp_code_t code = AC_ORIGIN_HOST;
		CHECK_FCT_DO( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_CODE, &code, &avp_oh_model, ENOENT),
			{ LOG_E("Cannot find Origin-Host AVP definition in the dictionary!"); (void) pthread_mutex_unlock(&cache_avp_lock); return __ret__; } );
	}
	CHECK_POSIX( pthread_mutex_unlock(&cache_avp_lock) );
	
	/* Find the Diameter Identity of the remote peer in the message */
	CHECK_FCT( fd_msg_search_avp ( msg, avp_oh_model, &avp_oh ) );
	ASSERT(avp_oh); /* otherwise it should not have passed rules validation, right? */
	CHECK_FCT( fd_msg_avp_hdr ( avp_oh, &avp_hdr ) );
	
	/* First, check if the Origin-Host value is valid */
	if (!fd_os_is_valid_DiameterIdentity(avp_hdr->avp_value->os.data, avp_hdr->avp_value->os.len)) {
		CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, cer, MSGFL_ANSW_ERROR ) );
		CHECK_FCT( fd_msg_rescode_set(*cer, "DIAMETER_INVALID_AVP_VALUE", 
							"Your Origin-Host contains invalid characters.", avp_oh, 1 ) );
		
		fd_hook_call(HOOK_PEER_CONNECT_FAILED, *cer, NULL, "Received CER with invalid Origin-Host AVP", NULL);
		
		CHECK_FCT( fd_out_send(cer, *cnx, NULL, 0) );
		return EINVAL;
	}
	
	/* Search if we already have this peer id in our list. We take directly the write lock so that we don't need to upgrade if it is a new peer.
	 * There is space for a small optimization here if needed.
	 */
	CHECK_POSIX( pthread_rwlock_wrlock(&fd_g_peers_rw) );
	
	li_inf = &fd_g_peers;
	for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
		int cmp, cont;
		peer = (struct fd_peer *)li;
		cmp = fd_os_almostcasesrch( avp_hdr->avp_value->os.data, avp_hdr->avp_value->os.len, peer->p_hdr.info.pi_diamid, peer->p_hdr.info.pi_diamidlen, &cont );
		if (cmp > 0) {
			li_inf = li;
		}
		if (cmp == 0) {
			if (fd_peer_getstate(li) == STATE_ZOMBIE) {
				/* If the existing peer is in zombie state then we can add it again, so
				 * continue as if we hadn't found it */
				li_inf = li;
			}
			else {
				found = 1;
				break;
			}
		}
		if (!cont)
			break;
	}
	
	if (!found) {
		/* Create a new peer entry for this new remote peer */
		peer = NULL;
		CHECK_FCT_DO( ret = fd_peer_alloc(&peer), goto out );
		
		/* Set the peer Diameter Id and the responder flag parameters */
		CHECK_MALLOC_DO( peer->p_hdr.info.pi_diamid = os0dup(avp_hdr->avp_value->os.data, avp_hdr->avp_value->os.len), 
			{ ret = ENOMEM; goto out; } );
		peer->p_hdr.info.pi_diamidlen = avp_hdr->avp_value->os.len;
		CHECK_MALLOC_DO( peer->p_dbgorig = strdup(fd_cnx_getid(*cnx)), { ret = ENOMEM; goto out; } );
		peer->p_flags.pf_responder = 1;
		peer->p_flags.pf_delete = 1;
		
		LOG_D("Created new peer object for incoming CER: %s", peer->p_hdr.info.pi_diamid);
		
#ifndef DISABLE_PEER_EXPIRY
		/* Set this peer to expire on inactivity */
		peer->p_hdr.info.config.pic_flags.exp 	= PI_EXP_INACTIVE;
		peer->p_hdr.info.config.pic_lft		= 3600;	/* 1 hour without any message 
		-- RFC3539 states that this must not be inferior to BRINGDOWN_INTERVAL = 5 minutes */
		
		CHECK_FCT_DO( ret = fd_p_expi_update( peer ), goto out );
#endif /* DISABLE_PEER_EXPIRY */
		
		/* Insert the new peer in the list (the PSM will take care of setting the expiry after validation) */
		fd_list_insert_after( li_inf, &peer->p_hdr.chain );
		
		/* Start the PSM, which will receive the event below */
		CHECK_FCT_DO( ret = fd_psm_begin(peer), goto out );
	}
	
	/* Send the new connection event to the PSM */
	CHECK_MALLOC_DO( ev_data = malloc(sizeof(struct cnx_incoming)), { ret = ENOMEM; goto out; } );
	memset(ev_data, 0, sizeof(*ev_data));
	
	ev_data->cer = msg;
	ev_data->cnx = *cnx;
	ev_data->validate = !found;
	
	CHECK_FCT_DO( ret = fd_event_send(peer->p_events, FDEVP_CNX_INCOMING, sizeof(*ev_data), ev_data), goto out );
	
out:	
	CHECK_POSIX( pthread_rwlock_unlock(&fd_g_peers_rw) );

	if (ret == 0) {
		/* Reset the "out" parameters, so that they are not cleanup on function return. */
		*cer = NULL;
		*cnx = NULL;
	} else {
		char buf[1024];
		snprintf(buf, sizeof(buf), "An error occurred while processing new incoming CER: %s", strerror(ret));
		fd_hook_call(HOOK_PEER_CONNECT_FAILED, *cer, NULL, buf, NULL);
	}
	
	return ret;
}

/* Save a callback to accept / reject incoming unknown peers */
int fd_peer_validate_register ( int (*peer_validate)(struct peer_info * /* info */, int * /* auth */, int (**cb2)(struct peer_info *)) )
{
	struct fd_list * v;
	
	TRACE_ENTRY("%p", peer_validate);
	CHECK_PARAMS(peer_validate);
	
	/* Alloc a new entry */
	CHECK_MALLOC( v = malloc(sizeof(struct fd_list)) );
	fd_list_init( v, peer_validate );
	
	/* Add at the beginning of the list */
	CHECK_FCT( pthread_rwlock_wrlock(&validators_rw) );
	fd_list_insert_after(&validators, v);
	CHECK_FCT( pthread_rwlock_unlock(&validators_rw));
	
	/* Done! */
	return 0;
}

/* Validate a peer by calling the callbacks in turn -- return 0 if the peer is validated, ! 0 in case of error (>0) or if the peer is rejected (-1) */
int fd_peer_validate( struct fd_peer * peer )
{
	int ret = 0;
	struct fd_list * v;
	
	CHECK_FCT( pthread_rwlock_rdlock(&validators_rw) );
	for (v = validators.next; v != &validators; v = v->next) {
		int auth = 0;
		pthread_cleanup_push(fd_cleanup_rwlock, &validators_rw);
		CHECK_FCT_DO( ret = ((int(*)(struct peer_info *, int *, int (**)(struct peer_info *)))(v->o)) (&peer->p_hdr.info, &auth, &peer->p_cb2),  );
		pthread_cleanup_pop(0);
		if (ret)
			goto out;
		if (auth) {
			ret = (auth > 0) ? 0 : -1;
			goto out;
		}
		peer->p_cb2 = NULL;
	}
	
	/* No callback has given a firm result, the default is to reject */
	ret = -1;
out:
	CHECK_FCT( pthread_rwlock_unlock(&validators_rw));
	return ret;
}
