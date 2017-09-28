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

/* This file contains code for TLS over multi-stream SCTP wrapper implementation (GnuTLS does not support this) */
/* See http://aaa.koganei.wide.ad.jp/blogs/index.php/waaad/2008/08/18/tls-over-sctp for history */

#include "fdcore-internal.h"
#include "cnxctx.h"

#include <netinet/sctp.h>
#include <sys/uio.h>

/*

Architecture of this wrapper:
 - we have several fifo queues (1 per stream pairs).
 GnuTLS is configured to use custom push / pull functions:
 - the pull function retrieves the data from the fifo queue corresponding to a stream #.
 - the push function sends the data on a certain stream.
 We also have a demux thread that reads the socket and store received data in the appropriate fifo
 
 We have one gnutls_session per stream pair, and as many threads that read the gnutls records and save incoming data to the target queue.
 
This complexity is required because we cannot read a socket for a given stream only; we can only get the next message and find its stream.
*/

/* Note that this mechanism is replaced by DTLS in RFC6733 */

/*************************************************************/
/*                      threads                              */
/*************************************************************/

/* Demux received data and store in the appropriate fifo */
static void * demuxer(void * arg)
{
	struct cnxctx * conn = arg;
	uint8_t * buf;
	size_t    bufsz;
	int	  event;
	uint16_t  strid;
	
	TRACE_ENTRY("%p", arg);
	CHECK_PARAMS_DO(conn && (conn->cc_socket > 0), goto out);
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "Demuxer (%d:%s)", conn->cc_socket, conn->cc_remid);
		fd_log_threadname ( buf );
	}
	
	ASSERT( conn->cc_proto == IPPROTO_SCTP );
	ASSERT( fd_cnx_target_queue(conn) );
	ASSERT( conn->cc_sctp3436_data.array );
	
	do {
		CHECK_FCT_DO( fd_sctp_recvmeta(conn, &strid, &buf, &bufsz, &event), goto fatal );
		switch (event) {
			case FDEVP_CNX_MSG_RECV:
				/* Demux this message to the appropriate fifo, another thread will pull, gnutls process, and send to target queue */
				if (strid < conn->cc_sctp_para.pairs) {
					CHECK_FCT_DO(fd_event_send(conn->cc_sctp3436_data.array[strid].raw_recv, event, bufsz, buf), goto fatal );
				} else {
					TRACE_DEBUG(INFO, "Received packet (%zd bytes) on out-of-range stream #%d from %s, discarded.", bufsz, strid, conn->cc_remid);
					free(buf);
				}
				break;
				
			case FDEVP_CNX_EP_CHANGE:
				/* Send this event to the target queue */
				CHECK_FCT_DO( fd_event_send( fd_cnx_target_queue(conn), event, bufsz, buf), goto fatal );
				break;
			
			case FDEVP_CNX_ERROR:
				goto out;
				
			case FDEVP_CNX_SHUTDOWN:
				/* Just ignore the notification for now, we will get another error later anyway */
				continue;
				
			default:
				goto fatal;
		}
		
	} while (conn->cc_loop);
	
out:
	/* Signal termination of the connection to all decipher threads */
	for (strid = 0; strid < conn->cc_sctp_para.pairs; strid++) {
		if (conn->cc_sctp3436_data.array[strid].raw_recv) {
			CHECK_FCT_DO(fd_event_send(conn->cc_sctp3436_data.array[strid].raw_recv, FDEVP_CNX_ERROR, 0, NULL), goto fatal );
		}
	}
	fd_cnx_markerror(conn);
	TRACE_DEBUG(FULL, "Thread terminated");	
	return NULL;
	
fatal:
	/* An unrecoverable error occurred, stop the daemon */
	CHECK_FCT_DO(fd_core_shutdown(), );
	goto out;
}

/* Decrypt the data received in this stream pair and store it in the target queue */
static void * decipher(void * arg)
{
	struct sctp3436_ctx * ctx = arg;
	struct cnxctx 	 *cnx;
	
	TRACE_ENTRY("%p", arg);
	CHECK_PARAMS_DO(ctx && ctx->raw_recv && ctx->parent, goto error);
	cnx = ctx->parent;
	ASSERT( fd_cnx_target_queue(cnx) );
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "Decipher (%hu@%d:%s)", ctx->strid, cnx->cc_socket, cnx->cc_remid);
		fd_log_threadname ( buf );
	}
	
	/* The next function loops while there is no error */
	CHECK_FCT_DO(fd_tls_rcvthr_core(cnx, ctx->strid ? ctx->session : cnx->cc_tls_para.session), /* continue */);
error:
	fd_cnx_markerror(cnx);
	TRACE_DEBUG(FULL, "Thread terminated");	
	return NULL;
}

/*************************************************************/
/*                     push / pull                           */
/*************************************************************/

#ifdef GNUTLS_VERSION_300
/* Check if data is available for gnutls on a given context */
static int sctp3436_pull_timeout(gnutls_transport_ptr_t tr, unsigned int ms)
{
	struct sctp3436_ctx * ctx = (struct sctp3436_ctx *) tr;
	struct timespec tsstore, *ts = NULL;
	int ret;
	
	TRACE_ENTRY("%p %d", tr, ms);
	
	if (ctx->partial.buf)
		return 1; /* data is already available for pull */
	
	if (ms) {
		CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &tsstore),  return -1  );
		tsstore.tv_nsec += (long)ms * 1000000;
		tsstore.tv_sec += tsstore.tv_nsec / 1000000000L;
		tsstore.tv_nsec %= 1000000000L;
		ts = &tsstore;
	}
	
	ret = fd_fifo_select ( ctx->raw_recv, ts );
	if (ret < 0) {
		errno = -ret;
		ret = -1;
	}
		
	return ret;
}
#endif /* GNUTLS_VERSION_300 */

/* Send data over the connection, called by gnutls */
#ifndef GNUTLS_VERSION_212
static ssize_t sctp3436_push(gnutls_transport_ptr_t tr, const void * data, size_t len)
{
	struct sctp3436_ctx * ctx = (struct sctp3436_ctx *) tr;
	struct iovec iov;
	
	TRACE_ENTRY("%p %p %zd", tr, data, len);
	CHECK_PARAMS_DO( tr && data, { errno = EINVAL; return -1; } );
	
	iov.iov_base = (void *)data;
	iov.iov_len  = len;
	
	return fd_sctp_sendstrv(ctx->parent, ctx->strid, &iov, 1);
}
#else /*  GNUTLS_VERSION_212 */
static ssize_t sctp3436_pushv(gnutls_transport_ptr_t tr, const giovec_t * iov, int iovcnt)
{
	struct sctp3436_ctx * ctx = (struct sctp3436_ctx *) tr;
	
	TRACE_ENTRY("%p %p %d", tr, iov, iovcnt);
	CHECK_PARAMS_DO( tr && iov, { errno = EINVAL; return -1; } );
	
	return fd_sctp_sendstrv(ctx->parent, ctx->strid, (const struct iovec *)iov, iovcnt);
}
#endif /*  GNUTLS_VERSION_212 */

/* Retrieve data received on a stream and already demultiplexed */
static ssize_t sctp3436_pull(gnutls_transport_ptr_t tr, void * buf, size_t len)
{
	struct sctp3436_ctx * ctx = (struct sctp3436_ctx *) tr;
	size_t pulled = 0;
	int emptied;
	
	TRACE_ENTRY("%p %p %zd", tr, buf, len);
	CHECK_PARAMS_DO( tr && buf, { errno = EINVAL; goto error; } );
	
	/* If we don't have data available now, pull new message from the fifo -- this is blocking (until the queue is destroyed) */
	if (!ctx->partial.buf) {
		int ev;
		CHECK_FCT_DO( errno = fd_event_get(ctx->raw_recv, &ev, &ctx->partial.bufsz, (void *)&ctx->partial.buf), goto error );
		if (ev == FDEVP_CNX_ERROR) {
			/* Documentations says to return 0 on connection closed, but it does hang within gnutls_handshake */
			return -1;
		}
	}
		
	pulled = ctx->partial.bufsz - ctx->partial.offset;
	if (pulled <= len) {
		emptied = 1;
	} else {
		/* limit to the capacity of destination buffer */
		emptied = 0;
		pulled = len;
	}

	/* Store the data in the destination buffer */
	memcpy(buf, ctx->partial.buf + ctx->partial.offset, pulled);

	/* Free the buffer if we read all its content, and reset the partial structure */
	if (emptied) {
		free(ctx->partial.buf);
		memset(&ctx->partial, 0, sizeof(ctx->partial));
	} else {
		ctx->partial.offset += pulled;
	}

	/* We are done */
	return pulled;
	
error:
	gnutls_transport_set_errno (ctx->session, errno);
	return -1;
}

/* Set the parameters of a session to use the appropriate fifo and stream information */
#ifndef GNUTLS_VERSION_300
GCC_DIAG_OFF("-Wdeprecated-declarations")
#endif /* !GNUTLS_VERSION_300 */
static void set_sess_transport(gnutls_session_t session, struct sctp3436_ctx *ctx)
{
	/* Set the transport pointer passed to push & pull callbacks */
	GNUTLS_TRACE( gnutls_transport_set_ptr( session, (gnutls_transport_ptr_t) ctx ) );
	
	/* Reset the low water value, since we don't use sockets */
#ifndef GNUTLS_VERSION_300
	/* starting version 2.12, this call is not needed */
	GNUTLS_TRACE( gnutls_transport_set_lowat( session, 0 ) );
#else  /* GNUTLS_VERSION_300 */
	/* but in 3.0 we have to provide the pull_timeout callback */
	GNUTLS_TRACE( gnutls_transport_set_pull_timeout_function( session, sctp3436_pull_timeout ) );
#endif /* GNUTLS_VERSION_300 */
	
	/* Set the push and pull callbacks */
	GNUTLS_TRACE( gnutls_transport_set_pull_function(session, sctp3436_pull) );
#ifndef GNUTLS_VERSION_212
	GNUTLS_TRACE( gnutls_transport_set_push_function(session, sctp3436_push) );
#else /* GNUTLS_VERSION_212 */
	GNUTLS_TRACE( gnutls_transport_set_vec_push_function(session, sctp3436_pushv) );
#endif /* GNUTLS_VERSION_212 */

	return;
}
#ifndef GNUTLS_VERSION_300
GCC_DIAG_ON("-Wdeprecated-declarations")
#endif /* !GNUTLS_VERSION_300 */

/*************************************************************/
/*               Session resuming support                    */
/*************************************************************/

struct sr_store {
	struct fd_list	 list;	/* list of sr_data, ordered by key.size then key.data */
	pthread_rwlock_t lock;
	struct cnxctx   *parent;
	/* Add another list to chain in a global list to implement a garbage collector on sessions -- TODO if needed */
};

/* Saved master session data for resuming sessions */
struct sr_data {
	struct fd_list	chain;
	gnutls_datum_t	key;
	gnutls_datum_t 	data;
};

/* Initialize the store area for a connection */
static int store_init(struct cnxctx * conn)
{
	TRACE_ENTRY("%p", conn);
	CHECK_PARAMS( conn && !conn->cc_sctp3436_data.sess_store );
	
	CHECK_MALLOC( conn->cc_sctp3436_data.sess_store = malloc(sizeof(struct sr_store)) );
	memset(conn->cc_sctp3436_data.sess_store, 0, sizeof(struct sr_store));
	
	fd_list_init(&conn->cc_sctp3436_data.sess_store->list, NULL);
	CHECK_POSIX( pthread_rwlock_init(&conn->cc_sctp3436_data.sess_store->lock, NULL) );
	conn->cc_sctp3436_data.sess_store->parent = conn;
	
	return 0;
}

/* Destroy the store area for a connection, and all its content */
static void store_destroy(struct cnxctx * conn)
{
	/* Del all list entries */
	TRACE_ENTRY("%p", conn);
	CHECK_PARAMS_DO( conn, return );
	
	if (!conn->cc_sctp3436_data.sess_store)
		return;
	
	CHECK_POSIX_DO( pthread_rwlock_destroy(&conn->cc_sctp3436_data.sess_store->lock), /* continue */ );
	
	while (!FD_IS_LIST_EMPTY(&conn->cc_sctp3436_data.sess_store->list)) {
		struct sr_data * sr = (struct sr_data *) conn->cc_sctp3436_data.sess_store->list.next;
		fd_list_unlink( &sr->chain );
		free(sr->key.data);
		free(sr->data.data);
		free(sr);
	}
	
	free(conn->cc_sctp3436_data.sess_store);
	conn->cc_sctp3436_data.sess_store = NULL;
	return;
}

/* Search the position (or next if not found) of a key in a store */
static struct fd_list * find_or_next(struct sr_store * sto, gnutls_datum_t key, int * match)
{
	struct fd_list * ret;
	*match = 0;
	
	for (ret = sto->list.next; ret != &sto->list; ret = ret->next) {
		int cmp = 0;
		struct sr_data * sr = (struct sr_data *)ret;
		
		cmp = fd_os_cmp(key.data, key.size, sr->key.data, sr->key.size);
		if (cmp > 0)
			continue;
		
		if (cmp == 0)
			*match = 1;
		
		break;
	}
	
	return ret;
}

/* Callbacks for the TLS server side of the connection, called during gnutls_handshake */
static int sr_store (void *dbf, gnutls_datum_t key, gnutls_datum_t data)
{
	struct sr_store * sto = (struct sr_store *)dbf;
	struct fd_list * li;
	struct sr_data * sr;
	int match = 0;
	int ret = 0;
	
	TRACE_DEBUG( GNUTLS_DBG_LEVEL, "GNUTLS Callback: %s", __PRETTY_FUNCTION__ );
	CHECK_PARAMS_DO( sto && key.data && data.data, return -1 );
	
	CHECK_POSIX_DO( pthread_rwlock_wrlock(&sto->lock), return -1 );
	TRACE_BUFFER(FD_LOG_DEBUG, GNUTLS_DBG_LEVEL, "Session store [key ", key.data, key.size, "]");
	
	li = find_or_next(sto, key, &match);
	if (match) {
		sr = (struct sr_data *)li;
		
		/* Check the data is the same */
		if ((data.size != sr->data.size) || memcmp(data.data, sr->data.data, data.size)) {
			TRACE_DEBUG(INFO, "GnuTLS tried to store a session with same key and different data!");
			TRACE_BUFFER(FD_LOG_DEBUG, INFO, "Session store [key ", key.data, key.size, "]");
			TRACE_BUFFER(FD_LOG_DEBUG, INFO, "  -- old data [", sr->data.data, sr->data.size, "]");
			TRACE_BUFFER(FD_LOG_DEBUG, INFO, "  -- new data [", data.data, data.size, "]");
			
			ret = -1;
		} else {
			TRACE_DEBUG(GNUTLS_DBG_LEVEL, "GnuTLS tried to store a session with same key and same data, skipped.");
		}
		goto out;
	}
	
	/* Create a new entry */
	CHECK_MALLOC_DO( sr = malloc(sizeof(struct sr_data)), { ret = -1; goto out; } );
	memset(sr, 0, sizeof(struct sr_data));

	fd_list_init(&sr->chain, sr);

	CHECK_MALLOC_DO( sr->key.data = malloc(key.size), { ret = -1; goto out; } );
	sr->key.size = key.size;
	memcpy(sr->key.data, key.data, key.size);

	CHECK_MALLOC_DO( sr->data.data = malloc(data.size), { ret = -1; goto out; } );
	sr->data.size = data.size;
	memcpy(sr->data.data, data.data, data.size);
	
	/* Save this new entry in the list, we are done */
	fd_list_insert_before(li, &sr->chain);

out:	
	CHECK_POSIX_DO( pthread_rwlock_unlock(&sto->lock), return -1 );
	return ret;
}

static int sr_remove (void *dbf, gnutls_datum_t key)
{
	struct sr_store * sto = (struct sr_store *)dbf;
	struct fd_list * li;
	struct sr_data * sr;
	int match = 0;
	int ret = 0;
	
	TRACE_DEBUG( GNUTLS_DBG_LEVEL, "GNUTLS Callback: %s", __PRETTY_FUNCTION__ );
	CHECK_PARAMS_DO( sto && key.data, return -1 );
	
	CHECK_POSIX_DO( pthread_rwlock_wrlock(&sto->lock), return -1 );
	TRACE_BUFFER(FD_LOG_DEBUG, GNUTLS_DBG_LEVEL, "Session delete [key ", key.data, key.size, "]");
	
	li = find_or_next(sto, key, &match);
	if (match) {
		sr = (struct sr_data *)li;
		
		/* Destroy this data */
		fd_list_unlink(li);
		free(sr->key.data);
		free(sr->data.data);
		free(sr);
	} else {
		/* It was not found */
		ret = -1;
	}
	
	CHECK_POSIX_DO( pthread_rwlock_unlock(&sto->lock), return -1 );
	return ret;
}

static gnutls_datum_t sr_fetch (void *dbf, gnutls_datum_t key)
{
	struct sr_store * sto = (struct sr_store *)dbf;
	struct fd_list * li;
	struct sr_data * sr;
	int match = 0;
	gnutls_datum_t res = { NULL, 0 };
	gnutls_datum_t error = { NULL, 0 };

	TRACE_DEBUG( GNUTLS_DBG_LEVEL, "GNUTLS Callback: %s", __PRETTY_FUNCTION__ );
	CHECK_PARAMS_DO( sto && key.data, return error );

	CHECK_POSIX_DO( pthread_rwlock_rdlock(&sto->lock), return error );
	TRACE_BUFFER(FD_LOG_DEBUG, GNUTLS_DBG_LEVEL, "Session fetch [key ", key.data, key.size, "]");
	
	li = find_or_next(sto, key, &match);
	if (match) {
		sr = (struct sr_data *)li;
		GNUTLS_TRACE( CHECK_MALLOC_DO(res.data = gnutls_malloc(sr->data.size), goto out ) );
		res.size = sr->data.size;
		memcpy(res.data, sr->data.data, res.size);
	}
out:	
	TRACE_DEBUG(GNUTLS_DBG_LEVEL, "Fetched (%p, %d) from store %p", res.data, res.size, sto);
	CHECK_POSIX_DO( pthread_rwlock_unlock(&sto->lock), return error);
	return res;
}

/* Set the session pointer in a session object */
static void set_resume_callbacks(gnutls_session_t session, struct cnxctx * conn)
{
	TRACE_ENTRY("%p", conn);
	
	GNUTLS_TRACE( gnutls_db_set_retrieve_function(session, sr_fetch));
	GNUTLS_TRACE( gnutls_db_set_remove_function  (session, sr_remove));
	GNUTLS_TRACE( gnutls_db_set_store_function   (session, sr_store));
	GNUTLS_TRACE( gnutls_db_set_ptr              (session, conn->cc_sctp3436_data.sess_store));
	
	return;
}

/* The handshake is made in parallel in several threads to speed up */
static void * handshake_resume_th(void * arg)
{
	struct sctp3436_ctx * ctx = (struct sctp3436_ctx *) arg;
	int resumed;
	
	TRACE_ENTRY("%p", arg);
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "Handshake resume (%hu@%d)", ctx->strid, ctx->parent->cc_socket);
		fd_log_threadname ( buf );
	}
	
	TRACE_DEBUG(FULL, "Starting TLS resumed handshake on stream %hu", ctx->strid);

	CHECK_GNUTLS_DO( gnutls_handshake( ctx->session ), return NULL);
			
	GNUTLS_TRACE( resumed = gnutls_session_is_resumed(ctx->session) );
	#ifndef GNUTLS_VERSION_300
	if (!resumed) {
		/* Check the credentials here also */
		CHECK_FCT_DO( fd_tls_verify_credentials(ctx->session, ctx->parent, 0), return NULL );
	}
	#endif /* GNUTLS_VERSION_300 */
	if (TRACE_BOOL(FULL)) {
		if (resumed) {
			fd_log_debug("Session was resumed successfully on stream %hu (conn: '%s')", ctx->strid, fd_cnx_getid(ctx->parent));
		} else {
			fd_log_debug("Session was NOT resumed on stream %hu  (full handshake) (conn: '%s')", ctx->strid, fd_cnx_getid(ctx->parent));
		}
	}
			
	/* Finished, OK */
	return arg;
}


/*************************************************************/
/*                     Exported functions                    */
/*************************************************************/

/* Initialize the wrapper for the connection */
int fd_sctp3436_init(struct cnxctx * conn)
{
	uint16_t i;
	
	TRACE_ENTRY("%p", conn);
	CHECK_PARAMS( conn && (conn->cc_sctp_para.pairs > 1) && (!conn->cc_sctp3436_data.array) );
	
	/* First, alloc the array and initialize the non-TLS data */
	CHECK_MALLOC( conn->cc_sctp3436_data.array = calloc(conn->cc_sctp_para.pairs, sizeof(struct sctp3436_ctx))  );
	for (i = 0; i < conn->cc_sctp_para.pairs; i++) {
		conn->cc_sctp3436_data.array[i].parent = conn;
		conn->cc_sctp3436_data.array[i].strid  = i;
		CHECK_FCT( fd_fifo_new(&conn->cc_sctp3436_data.array[i].raw_recv, 10) );
	}
	
	/* Set push/pull functions in the master session, using fifo in array[0] */
	set_sess_transport(conn->cc_tls_para.session, &conn->cc_sctp3436_data.array[0]);
	
	/* For server side, we also initialize the resuming capabilities */
	if (conn->cc_tls_para.mode == GNUTLS_SERVER) {
		
		/* Prepare the store for sessions data */
		CHECK_FCT( store_init(conn) );
		
		/* Set the callbacks for resuming in the master session */
		set_resume_callbacks(conn->cc_tls_para.session, conn);
	}

	/* Start the demux thread */
	CHECK_POSIX( pthread_create( &conn->cc_rcvthr, NULL, demuxer, conn ) );
	
	return 0;
}

/* Handshake other streams, after full handshake on the master session */
int fd_sctp3436_handshake_others(struct cnxctx * conn, char * priority, void * alt_creds)
{
	uint16_t i;
	int errors = 0;
	gnutls_datum_t 	master_data;
	
	TRACE_ENTRY("%p %p", conn, priority);
	CHECK_PARAMS( conn && (conn->cc_sctp_para.pairs > 1) && conn->cc_sctp3436_data.array );

	/* Server side: we set all the parameters, the resume callback will take care of resuming the session */
	/* Client side: we duplicate the parameters of the master session, then set the transport pointer */
	
	/* For client side, retrieve the master session parameters */
	if (conn->cc_tls_para.mode == GNUTLS_CLIENT) {
		CHECK_GNUTLS_DO( gnutls_session_get_data2(conn->cc_tls_para.session, &master_data), return ENOMEM );
		/* For debug: */
		if (TRACE_BOOL(GNUTLS_DBG_LEVEL)) {
			uint8_t  id[256];
			size_t	 ids = sizeof(id);
			CHECK_GNUTLS_DO( gnutls_session_get_id(conn->cc_tls_para.session, id, &ids), /* continue */ );
			TRACE_BUFFER(FD_LOG_DEBUG, GNUTLS_DBG_LEVEL, "Master session id: [", id, ids, "]");
		}
	}
	
	/* Initialize the session objects and start the handshake in a separate thread */
	for (i = 1; i < conn->cc_sctp_para.pairs; i++) {
		/* Set credentials and priority */
		CHECK_FCT( fd_tls_prepare(&conn->cc_sctp3436_data.array[i].session, conn->cc_tls_para.mode, 0, priority, alt_creds) );
		
		/* additional initialization for gnutls 3.x */
		#ifdef GNUTLS_VERSION_300
			/* the verify function has already been set in the global initialization in config.c */

		/* fd_tls_verify_credentials_2 uses the connection */
		gnutls_session_set_ptr (conn->cc_sctp3436_data.array[i].session, (void *) conn);

		if ((conn->cc_tls_para.cn != NULL) && (conn->cc_tls_para.mode == GNUTLS_CLIENT)) {
			/* this might allow virtual hosting on the remote peer */
			CHECK_GNUTLS_DO( gnutls_server_name_set (conn->cc_sctp3436_data.array[i].session, GNUTLS_NAME_DNS, conn->cc_tls_para.cn, strlen(conn->cc_tls_para.cn)), /* ignore failure */);
		}

		#endif /* GNUTLS_VERSION_300 */

		#ifdef GNUTLS_VERSION_310
		GNUTLS_TRACE( gnutls_handshake_set_timeout( conn->cc_sctp3436_data.array[i].session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT));
		#endif /* GNUTLS_VERSION_310 */

		/* For the client, copy data from master session; for the server, set session resuming pointers */
		if (conn->cc_tls_para.mode == GNUTLS_CLIENT) {
			CHECK_GNUTLS_DO( gnutls_session_set_data(conn->cc_sctp3436_data.array[i].session, master_data.data, master_data.size), return ENOMEM );
		} else {
			set_resume_callbacks(conn->cc_sctp3436_data.array[i].session, conn);
		}
		
		/* Set transport parameters */
		set_sess_transport(conn->cc_sctp3436_data.array[i].session, &conn->cc_sctp3436_data.array[i]);
		
		/* Start the handshake thread */
		CHECK_POSIX( pthread_create( &conn->cc_sctp3436_data.array[i].thr, NULL, handshake_resume_th, &conn->cc_sctp3436_data.array[i] ) );
	}
	
	/* We can now release the memory of master session data if any */
	if (conn->cc_tls_para.mode == GNUTLS_CLIENT) {
		GNUTLS_TRACE( gnutls_free(master_data.data) );
	}
	
	/* Now wait for all handshakes to finish */
	for (i = 1; i < conn->cc_sctp_para.pairs; i++) {
		void * ret;
		CHECK_POSIX( pthread_join(conn->cc_sctp3436_data.array[i].thr, &ret) );
		conn->cc_sctp3436_data.array[i].thr = (pthread_t) NULL;
		if (ret == NULL) {
			errors++; /* Handshake failed on this stream */
		}
	}
	
	if (errors) {
		TRACE_DEBUG(INFO, "Handshake failed on %d/%hd stream pairs", errors, conn->cc_sctp_para.pairs);
		fd_cnx_markerror(conn);
		return ENOTCONN;
	}
	
	return 0;
}

/* Receive messages from others ? all other stream pairs : the master pair */
int fd_sctp3436_startthreads(struct cnxctx * conn, int others)
{
	uint16_t i;
	
	TRACE_ENTRY("%p", conn);
	CHECK_PARAMS( conn && conn->cc_sctp3436_data.array );
	
	if (others) {
		for (i = 1; i < conn->cc_sctp_para.pairs; i++) {

			/* Start the decipher thread */
			CHECK_POSIX( pthread_create( &conn->cc_sctp3436_data.array[i].thr, NULL, decipher, &conn->cc_sctp3436_data.array[i] ) );
		}
	} else {
		CHECK_POSIX( pthread_create( &conn->cc_sctp3436_data.array[0].thr, NULL, decipher, &conn->cc_sctp3436_data.array[0] ) );
	}
	return 0;
}

/* Initiate a "bye" on all stream pairs */
void fd_sctp3436_bye(struct cnxctx * conn)
{
	uint16_t i;
	
	CHECK_PARAMS_DO( conn && conn->cc_sctp3436_data.array, return );
	
	/* End all TLS sessions, in series (not as efficient as paralel, but simpler) */
	for (i = 1; i < conn->cc_sctp_para.pairs; i++) {
		if ( ! fd_cnx_teststate(conn, CC_STATUS_ERROR)) {
			CHECK_GNUTLS_DO( gnutls_bye(conn->cc_sctp3436_data.array[i].session, GNUTLS_SHUT_WR), fd_cnx_markerror(conn) );
		}
	}
}

/* After "bye" was sent on all streams, read from sessions until an error is received */
void fd_sctp3436_waitthreadsterm(struct cnxctx * conn)
{
	uint16_t i;
	
	TRACE_ENTRY("%p", conn);
	CHECK_PARAMS_DO( conn && conn->cc_sctp3436_data.array, return );
	
	for (i = 0; i < conn->cc_sctp_para.pairs; i++) {
		if (conn->cc_sctp3436_data.array[i].thr != (pthread_t)NULL) {
			CHECK_POSIX_DO( pthread_join(conn->cc_sctp3436_data.array[i].thr, NULL), /* continue */ );
			conn->cc_sctp3436_data.array[i].thr = (pthread_t)NULL;
		}
	}
	return;
}

/* Free gnutls resources of all sessions */
void fd_sctp3436_gnutls_deinit_others(struct cnxctx * conn)
{
	uint16_t i;
	
	TRACE_ENTRY("%p", conn);
	CHECK_PARAMS_DO( conn && conn->cc_sctp3436_data.array, return );
	
	for (i = 1; i < conn->cc_sctp_para.pairs; i++) {
		if (conn->cc_sctp3436_data.array[i].session) {
			GNUTLS_TRACE( gnutls_deinit(conn->cc_sctp3436_data.array[i].session) );
			conn->cc_sctp3436_data.array[i].session = NULL;
		}
	}
}


/* Stop all receiver threads */
void fd_sctp3436_stopthreads(struct cnxctx * conn)
{
	uint16_t i;
	
	TRACE_ENTRY("%p", conn);
	CHECK_PARAMS_DO( conn && conn->cc_sctp3436_data.array, return );
	
	for (i = 0; i < conn->cc_sctp_para.pairs; i++) {
		CHECK_FCT_DO( fd_thr_term(&conn->cc_sctp3436_data.array[i].thr), /* continue */ );
	}
	return;
}

/* Destroy a wrapper context */
void fd_sctp3436_destroy(struct cnxctx * conn)
{
	uint16_t i;
	
	CHECK_PARAMS_DO( conn && conn->cc_sctp3436_data.array, return );
	
	/* Terminate all receiving threads in case we did not do it yet */
	fd_sctp3436_stopthreads(conn);
	
	/* Now, stop the demux thread */
	CHECK_FCT_DO( fd_thr_term(&conn->cc_rcvthr), /* continue */ );
	
	/* Free remaining data in the array */
	for (i = 0; i < conn->cc_sctp_para.pairs; i++) {
		if (conn->cc_sctp3436_data.array[i].raw_recv)
			fd_event_destroy( &conn->cc_sctp3436_data.array[i].raw_recv, free );
		free(conn->cc_sctp3436_data.array[i].partial.buf);
		if (conn->cc_sctp3436_data.array[i].session) {
			GNUTLS_TRACE( gnutls_deinit(conn->cc_sctp3436_data.array[i].session) );
			conn->cc_sctp3436_data.array[i].session = NULL;
		}
	}
	
	/* Free the array itself now */
	free(conn->cc_sctp3436_data.array);
	conn->cc_sctp3436_data.array = NULL;
	
	/* Delete the store of sessions */
	store_destroy(conn);
	
	return ;
}
