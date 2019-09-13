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

/* Server (listening) part of the framework */

static struct fd_list	FD_SERVERS = FD_LIST_INITIALIZER(FD_SERVERS);	/* The list of all server objects */
/* We don't need to protect this list, it is only accessed from the main framework thread. */

enum s_state {
	NOT_CREATED=0,
	RUNNING,
	TERMINATED,
	ERROR	/* an error occurred, this is not a valid status */
};

/* Servers information */
struct server {
	struct fd_list	chain;		/* link in the FD_SERVERS list */

	struct cnxctx *	conn;		/* server connection context (listening socket) */
	int 		proto;		/* IPPROTO_TCP or IPPROTO_SCTP */
	int 		secur;		/* TLS is started immediatly after connection ? 0: no; 1: RFU; 2: yes (TLS/TCP or TLS/SCTP) */
	
	pthread_t	thr;		/* The thread waiting for new connections (will store the data in the clients fifo) */
	enum s_state	state;		/* state of the thread */
	
	struct fifo	*pending;	/* FIFO of struct cnxctx */
	struct pool_workers {
		struct server * s;	/* pointer to the parent server structure */
		int		id;	/* The worker id for logs */
		pthread_t	worker; /* The thread */
	}		*workers;	/* array of cnf_thr_srv items */
};


/* Micro functions to read/change the status thread-safely */
static pthread_mutex_t s_lock = PTHREAD_MUTEX_INITIALIZER;
static enum s_state get_status(struct server * s)
{
	enum s_state r;
	CHECK_POSIX_DO( pthread_mutex_lock(&s_lock), return ERROR );
	r = s->state;
	CHECK_POSIX_DO( pthread_mutex_unlock(&s_lock), return ERROR );
	return r;
}
static void set_status(struct server * s, enum s_state st)
{
	CHECK_POSIX_DO( pthread_mutex_lock(&s_lock), return );
	s->state = st;
	CHECK_POSIX_DO( pthread_mutex_unlock(&s_lock), return );
}
	

/* dump one item of the server->pending fifo */
static DECLARE_FD_DUMP_PROTOTYPE(dump_cnx, void * item) {
	struct cnxctx * c = item;
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, " '%s'", fd_cnx_getid(c)), return NULL);
	return *buf;
}

/* Dump all servers information */
DECLARE_FD_DUMP_PROTOTYPE(fd_servers_dump, int details)
{
	struct fd_list * li;
	
	FD_DUMP_HANDLE_OFFSET();
	
	for (li = FD_SERVERS.next; li != &FD_SERVERS; li = li->next) {
		struct server * s = (struct server *)li;
		enum s_state st = get_status(s);
		
		if (details) {
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "{server}(@%p)'%s': %s, %s(%d), %s", s, fd_cnx_getid(s->conn), 
					IPPROTO_NAME( s->proto ),
					s->secur ? "Secur" : "NotSecur", s->secur,
					(st == NOT_CREATED) ? "Thread not created" :
					((st == RUNNING) ? "Thread running" :
					((st == TERMINATED) ? "Thread terminated" :
								  "Thread status unknown"))), return NULL);
			/* Dump the client list of this server */
			CHECK_MALLOC_DO( fd_fifo_dump(FD_DUMP_STD_PARAMS, "pending connections", s->pending, dump_cnx), return NULL );

			if (li->next != &FD_SERVERS) {
				CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "\n"), return NULL);
			}
		} else {
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "'%s'(%s,%s)  ", fd_cnx_getid(s->conn), 
					IPPROTO_NAME( s->proto ), s->secur ? "Secur" : "NotSecur"), return NULL);
		}
	}
	
	return *buf;
}


/* The thread in the pool for handling new clients connecting to a server */
static void * client_worker(void * arg)
{
	struct pool_workers * pw = arg;
	struct server * s = pw->s;
	struct cnxctx * c = NULL;
	int fatal = 0;
	struct timespec ts;
	struct fd_cnx_rcvdata rcv_data;
	struct fd_msg_pmdl * pmdl = NULL;
	struct msg    * msg = NULL;
	struct msg_hdr *hdr = NULL;
	struct fd_pei pei;
	
	TRACE_ENTRY("%p", arg);
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "Worker#%d[%s%s]", pw->id, IPPROTO_NAME(s->proto), s->secur?", Sec" : "");
		fd_log_threadname ( buf );
	}
	
	/* Loop until canceled / error */
next_client:
	LOG_A("Ready to process next incoming connection");

	memset(&rcv_data, 0, sizeof(rcv_data));
	
	/* Get the next connection */
	CHECK_FCT_DO( fd_fifo_get( s->pending, &c ), { fatal = 1; goto cleanup; } );

	/* Handshake if we are a secure server port, or start clear otherwise */
	if (s->secur) {
		LOG_D("Starting handshake with %s", fd_cnx_getid(c));

		int ret = fd_cnx_handshake(c, GNUTLS_SERVER, (s->secur == 1) ? ALGO_HANDSHAKE_DEFAULT : ALGO_HANDSHAKE_3436, NULL, NULL);
		if (ret != 0) {
			char buf[1024];
			snprintf(buf, sizeof(buf), "TLS handshake failed for connection '%s', connection closed.", fd_cnx_getid(c));

			fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, NULL, buf, NULL);

			goto cleanup;
		}
	} else {
		CHECK_FCT_DO( fd_cnx_start_clear(c, 0), goto cleanup );
	}
	
	/* Set the timeout to receive the first message */
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), { fatal = 1; goto cleanup; } );
	ts.tv_sec += INCNX_TIMEOUT;
	
	/* Receive the first Diameter message on the connection -- cleanup in case of timeout */
	CHECK_FCT_DO( fd_cnx_receive(c, &ts, &rcv_data.buffer, &rcv_data.length), 
		{
			char buf[1024];
			
			switch (__ret__) {
			case ETIMEDOUT:
				snprintf(buf, sizeof(buf), "Client '%s' did not send CER within %ds, connection aborted.", fd_cnx_getid(c), INCNX_TIMEOUT);
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, NULL, buf, NULL);
				break;
			
			case ENOTCONN:
				snprintf(buf, sizeof(buf), "Connection from '%s' in error before CER was received.", fd_cnx_getid(c));
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, NULL, buf, NULL);
				break;
			
			default:
				snprintf(buf, sizeof(buf), "Connection from '%s': unspecified error, connection aborted.", fd_cnx_getid(c));
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, NULL, buf, NULL);
			}
			goto cleanup;
		} );
	
	TRACE_DEBUG(FULL, "Received %zdb from new client '%s'", rcv_data.length, fd_cnx_getid(c));
	
	pmdl = fd_msg_pmdl_get_inbuf(rcv_data.buffer, rcv_data.length);
	
	/* Try parsing this message */
	CHECK_FCT_DO( fd_msg_parse_buffer( &rcv_data.buffer, rcv_data.length, &msg ), 
		{ 	/* Parsing failed */ 
			fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, NULL, NULL, &rcv_data, pmdl );
			goto cleanup;
		} );
	
	/* Log incoming message */
	fd_hook_associate(msg, pmdl);
	fd_hook_call(HOOK_MESSAGE_RECEIVED, msg, NULL, fd_cnx_getid(c), fd_msg_pmdl_get(msg));
	
	/* We expect a CER, it must parse with our dictionary and rules */
	CHECK_FCT_DO( fd_msg_parse_rules( msg, fd_g_config->cnf_dict, &pei ), 
		{ /* Parsing failed -- trace details */ 
			char buf[1024];
			
			fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msg, NULL, pei.pei_message ?: pei.pei_errcode, fd_msg_pmdl_get(msg));
			
			snprintf(buf, sizeof(buf), "Error parsing CER from '%s', connection aborted.", fd_cnx_getid(c));
			fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, NULL, buf, NULL);
			
			goto cleanup;
		} );
	
	/* Now check we received a CER */
	CHECK_FCT_DO( fd_msg_hdr ( msg, &hdr ), { fatal = 1; goto cleanup; }  );
	CHECK_PARAMS_DO( (hdr->msg_appl == 0) && (hdr->msg_flags & CMD_FLAG_REQUEST) && (hdr->msg_code == CC_CAPABILITIES_EXCHANGE),
		{ /* Parsing failed -- trace details */ 
			char buf[1024];
			snprintf(buf, sizeof(buf), "Expected CER from '%s', received a different message, connection aborted.", fd_cnx_getid(c));
			fd_hook_call(HOOK_PEER_CONNECT_FAILED, msg, NULL, buf, NULL);
			goto cleanup;
		} );
	
	/* Finally, pass the information to the peers module which will handle it in a separate thread */
	pthread_cleanup_push((void *)fd_cnx_destroy, c);
	pthread_cleanup_push((void *)fd_msg_free, msg);
	CHECK_FCT_DO( fd_peer_handle_newCER( &msg, &c ),  );
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

cleanup:
	/* Cleanup the parsed message if any */
	if (msg) {
		CHECK_FCT_DO( fd_msg_free(msg), /* continue */);
		msg = NULL;
	}
	
	/* Close the connection if needed */
	if (c != NULL) {
		fd_cnx_destroy(c);
		c = NULL;
	}
	
	/* Cleanup the received buffer if any */
	free(rcv_data.buffer);
	
	
	if (!fatal)
		goto next_client;

	LOG_E("Worker thread exiting.");
	return NULL;
}	

/* The thread managing a server */
static void * serv_th(void * arg)
{
	struct server *s = (struct server *)arg;
	
	CHECK_PARAMS_DO(s, goto error);
	fd_log_threadname ( fd_cnx_getid(s->conn) );
	
	set_status(s, RUNNING);
	
	/* Accept incoming connections */
	CHECK_FCT_DO( fd_cnx_serv_listen(s->conn), goto error );
	
	do {
		struct cnxctx * conn = NULL;
		
		/* Wait for a new client or cancel */
		CHECK_MALLOC_DO( conn = fd_cnx_serv_accept(s->conn), break );
		
		/* Store this connection in the fifo for processing by the worker pool. Will block when the fifo is full */
		pthread_cleanup_push((void *)fd_cnx_destroy, conn);
		CHECK_FCT_DO( fd_fifo_post( s->pending, &conn ), break );
		pthread_cleanup_pop(0);
		
	} while (1);
error:	
	if (s)
		set_status(s, TERMINATED);

	/* Send error signal to the core */
	LOG_F( "An error occurred in server module! Thread is terminating...");
	CHECK_FCT_DO(fd_core_shutdown(), );

	return NULL;
}


/* Create a new server structure */
static struct server * new_serv( int proto, int secur )
{
	struct server * new;
	int i;
	
	/* New server structure */
	CHECK_MALLOC_DO( new = malloc(sizeof(struct server)), return NULL );
	
	memset(new, 0, sizeof(struct server));
	fd_list_init(&new->chain, new);
	new->proto = proto;
	new->secur = secur;
	
	CHECK_FCT_DO( fd_fifo_new(&new->pending, 5), return NULL);
	CHECK_MALLOC_DO( new->workers = calloc( fd_g_config->cnf_thr_srv, sizeof(struct pool_workers) ), return NULL );
	
	for (i = 0; i < fd_g_config->cnf_thr_srv; i++) {
		/* Create the pool */
		new->workers[i].s = new;
		new->workers[i].id = i;
		CHECK_POSIX_DO( pthread_create( &new->workers[i].worker, NULL, client_worker, &new->workers[i]), return NULL );
	}
	
	return new;
}

/* Start all the servers */
int fd_servers_start()
{
	struct server * s;
	
	int empty_conf_ep = FD_IS_LIST_EMPTY(&fd_g_config->cnf_endpoints);
	
	/* SCTP */
	if (!fd_g_config->cnf_flags.no_sctp) {
#ifdef DISABLE_SCTP
		ASSERT(0);
#else /* DISABLE_SCTP */
		
		/* Create the server on unsecure port */
		if (fd_g_config->cnf_port) {
			CHECK_MALLOC( s = new_serv(IPPROTO_SCTP, 0) );
			CHECK_MALLOC( s->conn = fd_cnx_serv_sctp(fd_g_config->cnf_port, empty_conf_ep ? NULL : &fd_g_config->cnf_endpoints) );
			fd_list_insert_before( &FD_SERVERS, &s->chain );
			CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
		}
		
		/* Create the server on secure port */
		if (fd_g_config->cnf_port_tls) {
			CHECK_MALLOC( s = new_serv(IPPROTO_SCTP, 2 /* Change when DTLS is introduced */) );
			CHECK_MALLOC( s->conn = fd_cnx_serv_sctp(fd_g_config->cnf_port_tls, empty_conf_ep ? NULL : &fd_g_config->cnf_endpoints) );
			fd_list_insert_before( &FD_SERVERS, &s->chain );
			CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
		}
		
		/* Create the other server on 3436 secure port */
		/*if (fd_g_config->cnf_port_3436) {
			CHECK_MALLOC( s = new_serv(IPPROTO_SCTP, 2) );
			CHECK_MALLOC( s->conn = fd_cnx_serv_sctp(fd_g_config->cnf_port_3436, empty_conf_ep ? NULL : &fd_g_config->cnf_endpoints) );
			fd_list_insert_before( &FD_SERVERS, &s->chain );
			CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
		}*/
		
#endif /* DISABLE_SCTP */
	}
	
	/* TCP */
	if (!fd_g_config->cnf_flags.no_tcp) {
		
		if (empty_conf_ep) {
			/* Bind TCP servers on [0.0.0.0] */
			if (!fd_g_config->cnf_flags.no_ip4) {
				
				if (fd_g_config->cnf_port) {
					CHECK_MALLOC( s = new_serv(IPPROTO_TCP, 0) );
					CHECK_MALLOC( s->conn = fd_cnx_serv_tcp(fd_g_config->cnf_port, AF_INET, NULL) );
					fd_list_insert_before( &FD_SERVERS, &s->chain );
					CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
				}

				if (fd_g_config->cnf_port_tls) {
					CHECK_MALLOC( s = new_serv(IPPROTO_TCP, 1) );
					CHECK_MALLOC( s->conn = fd_cnx_serv_tcp(fd_g_config->cnf_port_tls, AF_INET, NULL) );
					fd_list_insert_before( &FD_SERVERS, &s->chain );
					CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
				}
			}
			
			/* Bind TCP servers on [::] */
			if (!fd_g_config->cnf_flags.no_ip6) {

				if (fd_g_config->cnf_port) {
					CHECK_MALLOC( s = new_serv(IPPROTO_TCP, 0) );
					CHECK_MALLOC( s->conn = fd_cnx_serv_tcp(fd_g_config->cnf_port, AF_INET6, NULL) );
					fd_list_insert_before( &FD_SERVERS, &s->chain );
					CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
				}

				if (fd_g_config->cnf_port_tls) {
					CHECK_MALLOC( s = new_serv(IPPROTO_TCP, 1) );
					CHECK_MALLOC( s->conn = fd_cnx_serv_tcp(fd_g_config->cnf_port_tls, AF_INET6, NULL) );
					fd_list_insert_before( &FD_SERVERS, &s->chain );
					CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
				}
			}
		} else {
			/* Create all endpoints -- check flags */
			struct fd_list * li;
			for (li = fd_g_config->cnf_endpoints.next; li != &fd_g_config->cnf_endpoints; li = li->next) {
				struct fd_endpoint * ep = (struct fd_endpoint *)li;
				sSA * sa = (sSA *) &ep->ss;
				if (! (ep->flags & EP_FL_CONF))
					continue;
				if (fd_g_config->cnf_flags.no_ip4 && (sa->sa_family == AF_INET))
					continue;
				if (fd_g_config->cnf_flags.no_ip6 && (sa->sa_family == AF_INET6))
					continue;
				
				if (fd_g_config->cnf_port) {
					CHECK_MALLOC( s = new_serv(IPPROTO_TCP, 0) );
					CHECK_MALLOC( s->conn = fd_cnx_serv_tcp(fd_g_config->cnf_port, sa->sa_family, ep) );
					fd_list_insert_before( &FD_SERVERS, &s->chain );
					CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
				}

				if (fd_g_config->cnf_port_tls) {
					CHECK_MALLOC( s = new_serv(IPPROTO_TCP, 1) );
					CHECK_MALLOC( s->conn = fd_cnx_serv_tcp(fd_g_config->cnf_port_tls, sa->sa_family, ep) );
					fd_list_insert_before( &FD_SERVERS, &s->chain );
					CHECK_POSIX( pthread_create( &s->thr, NULL, serv_th, s ) );
				}
			}
		}
	}
	
	/* Now, if we had an empty list of local adresses (no address configured), try to read the real addresses from the kernel */
	if (empty_conf_ep) {
		CHECK_FCT(fd_cnx_get_local_eps(&fd_g_config->cnf_endpoints));
		if (FD_IS_LIST_EMPTY(&fd_g_config->cnf_endpoints)) {
			TRACE_DEBUG(INFO, "Unable to find the address(es) of the local system. " 
					"Please use \"ListenOn\" parameter in the configuration. "
					"This information is required to generate the CER/CEA messages.");
			return EINVAL;
		}
	}
	
	{
		char * buf = NULL;
		size_t len = 0, offset = 0;
		CHECK_MALLOC_DO( fd_dump_extend( &buf, &len, &offset , "Local server address(es): "), );
		CHECK_MALLOC_DO( fd_ep_dump(  &buf, &len, &offset, 0, 0, &fd_g_config->cnf_endpoints ), );
		LOG_N("%s", buf ?: "Error dumping addresses");
		free(buf);
	}
	return 0;
}

/* Terminate all the servers */
int fd_servers_stop()
{
	TRACE_ENTRY("");
	
	TRACE_DEBUG(INFO, "Shutting down server sockets...");
	
	/* Loop on all servers */
	while (!FD_IS_LIST_EMPTY(&FD_SERVERS)) {
		struct server * s = (struct server *)(FD_SERVERS.next);
		int i;
		struct cnxctx * c;
		
		/* cancel thread */
		CHECK_FCT_DO( fd_thr_term(&s->thr), /* continue */);
		
		/* destroy server connection context */
		fd_cnx_destroy(s->conn);
		
		/* cancel and destroy all worker threads */
		for (i = 0; i < fd_g_config->cnf_thr_srv; i++) {
			/* Destroy worker thread */
			CHECK_FCT_DO( fd_thr_term(&s->workers[i].worker), /* continue */);
		}
		free(s->workers);
		
		/* Close any pending connection */
		while ( fd_fifo_tryget( s->pending, &c ) == 0 ) {
			fd_cnx_destroy(c);
		}
		CHECK_FCT_DO( fd_fifo_del(&s->pending), );
		
		/* Now destroy the server object */
		fd_list_unlink(&s->chain);
		free(s);
	}
	
	/* We're done! */
	return 0;
}
