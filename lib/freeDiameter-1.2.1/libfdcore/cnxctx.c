/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2015, WIDE Project and NICT								 *
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
#include "cnxctx.h"

#include <net/if.h>
#include <ifaddrs.h> /* for getifaddrs */
#include <sys/uio.h> /* writev */

/* The maximum size of Diameter message we accept to receive (<= 2^24) to avoid too big mallocs in case of trashed headers */
#ifndef DIAMETER_MSG_SIZE_MAX
#define DIAMETER_MSG_SIZE_MAX	65535	/* in bytes */
#endif /* DIAMETER_MSG_SIZE_MAX */


/* Connections contexts (cnxctx) in freeDiameter are wrappers around the sockets and TLS operations .
 * They are used to hide the details of the processing to the higher layers of the daemon.
 * They are always oriented on connections (TCP or SCTP), connectionless modes (UDP or SCTP) are not supported.
 */

/* Lifetime of a cnxctx object:
 * 1) Creation
 *    a) a server socket:
 *       - create the object with fd_cnx_serv_tcp or fd_cnx_serv_sctp
 *       - start listening incoming connections: fd_cnx_serv_listen
 *       - accept new clients with fd_cnx_serv_accept.
 *    b) a client socket:
 *       - connect to a remote server with fd_cnx_cli_connect
 *
 * 2) Initialization
 *    - if TLS is started first, call fd_cnx_handshake
 *    - otherwise to receive clear messages, call fd_cnx_start_clear. fd_cnx_handshake can be called later.
 *
 * 3) Usage
 *    - fd_cnx_receive, fd_cnx_send : exchange messages on this connection (send is synchronous, receive is not, but blocking).
 *    - fd_cnx_recv_setaltfifo : when a message is received, the event is sent to an external fifo list. fd_cnx_receive does not work when the alt_fifo is set.
 *    - fd_cnx_getid : retrieve a descriptive string for the connection (for debug)
 *    - fd_cnx_getremoteid : identification of the remote peer (IP address or fqdn)
 *    - fd_cnx_getcred : get the remote peer TLS credentials, after handshake
 *
 * 4) End
 *    - fd_cnx_destroy
 */

/*******************************************/
/*     Creation of a connection object     */
/*******************************************/

/* Initialize a context structure */
static struct cnxctx * fd_cnx_init(int full)
{
	struct cnxctx * conn = NULL;

	TRACE_ENTRY("%d", full);

	CHECK_MALLOC_DO( conn = malloc(sizeof(struct cnxctx)), return NULL );
	memset(conn, 0, sizeof(struct cnxctx));

	if (full) {
		CHECK_FCT_DO( fd_fifo_new ( &conn->cc_incoming, 5 ), return NULL );
	}

	return conn;
}

#define CC_ID_HDR "{----} "

/* Create and bind a server socket to the given endpoint and port */
struct cnxctx * fd_cnx_serv_tcp(uint16_t port, int family, struct fd_endpoint * ep)
{
	struct cnxctx * cnx = NULL;
	sSS dummy;
	sSA * sa = (sSA *) &dummy;

	TRACE_ENTRY("%hu %d %p", port, family, ep);

	CHECK_PARAMS_DO( port, return NULL );
	CHECK_PARAMS_DO( ep || family, return NULL );
	CHECK_PARAMS_DO( (! family) || (family == AF_INET) || (family == AF_INET6), return NULL );
	CHECK_PARAMS_DO( (! ep) || (ep->ss.ss_family == AF_INET) || (ep->ss.ss_family == AF_INET6), return NULL );
	CHECK_PARAMS_DO( (! ep) || (!family) || (ep->ss.ss_family == family), return NULL );

	/* The connection object */
	CHECK_MALLOC_DO( cnx = fd_cnx_init(0), return NULL );

	/* Prepare the socket address information */
	if (ep) {
		memcpy(sa, &ep->ss, sizeof(sSS));
	} else {
		memset(&dummy, 0, sizeof(dummy));
		sa->sa_family = family;
	}
	if (sa->sa_family == AF_INET) {
		((sSA4 *)sa)->sin_port = htons(port);
		cnx->cc_family = AF_INET;
	} else {
		((sSA6 *)sa)->sin6_port = htons(port);
		cnx->cc_family = AF_INET6;
	}

	/* Create the socket */
	CHECK_FCT_DO( fd_tcp_create_bind_server( &cnx->cc_socket, sa, sSAlen(sa) ), goto error );

	/* Generate the name for the connection object */
	{
		char addrbuf[INET6_ADDRSTRLEN];
		int  rc;
		rc = getnameinfo(sa, sSAlen(sa), addrbuf, sizeof(addrbuf), NULL, 0, NI_NUMERICHOST);
		if (rc)
			snprintf(addrbuf, sizeof(addrbuf), "[err:%s]", gai_strerror(rc));
		snprintf(cnx->cc_id, sizeof(cnx->cc_id), CC_ID_HDR "TCP srv [%s]:%hu (%d)", addrbuf, port, cnx->cc_socket);
	}

	cnx->cc_proto = IPPROTO_TCP;

	return cnx;

error:
	fd_cnx_destroy(cnx);
	return NULL;
}

/* Same function for SCTP, with a list of local endpoints to bind to */
struct cnxctx * fd_cnx_serv_sctp(uint16_t port, struct fd_list * ep_list)
{
#ifdef DISABLE_SCTP
	TRACE_DEBUG(INFO, "This function should never been called when SCTP is disabled...");
	ASSERT(0);
	CHECK_FCT_DO( ENOTSUP, );
	return NULL;
#else /* DISABLE_SCTP */
	struct cnxctx * cnx = NULL;

	TRACE_ENTRY("%hu %p", port, ep_list);

	CHECK_PARAMS_DO( port, return NULL );

	/* The connection object */
	CHECK_MALLOC_DO( cnx = fd_cnx_init(0), return NULL );

	if (fd_g_config->cnf_flags.no_ip6) {
		cnx->cc_family = AF_INET;
	} else {
		cnx->cc_family = AF_INET6; /* can create socket for both IP and IPv6 */
	}
	
	/* Create the socket */
	CHECK_FCT_DO( fd_sctp_create_bind_server( &cnx->cc_socket, cnx->cc_family, ep_list, port ), goto error );

	/* Generate the name for the connection object */
	snprintf(cnx->cc_id, sizeof(cnx->cc_id), CC_ID_HDR "SCTP srv :%hu (%d)", port, cnx->cc_socket);

	cnx->cc_proto = IPPROTO_SCTP;

	return cnx;

error:
	fd_cnx_destroy(cnx);
	return NULL;
#endif /* DISABLE_SCTP */
}

/* Allow clients to connect on the server socket */
int fd_cnx_serv_listen(struct cnxctx * conn)
{
	CHECK_PARAMS( conn );

	switch (conn->cc_proto) {
		case IPPROTO_TCP:
			CHECK_FCT(fd_tcp_listen(conn->cc_socket));
			break;

#ifndef DISABLE_SCTP
		case IPPROTO_SCTP:
			CHECK_FCT(fd_sctp_listen(conn->cc_socket));
			break;
#endif /* DISABLE_SCTP */

		default:
			CHECK_PARAMS(0);
	}

	return 0;
}

/* Accept a client (blocking until a new client connects) -- cancelable */
struct cnxctx * fd_cnx_serv_accept(struct cnxctx * serv)
{
	struct cnxctx * cli = NULL;
	sSS ss;
	socklen_t ss_len = sizeof(ss);
	int cli_sock = 0;

	TRACE_ENTRY("%p", serv);
	CHECK_PARAMS_DO(serv, return NULL);
	
	/* Accept the new connection -- this is blocking until new client enters or until cancellation */
	CHECK_SYS_DO( cli_sock = accept(serv->cc_socket, (sSA *)&ss, &ss_len), return NULL );
	
	CHECK_MALLOC_DO( cli = fd_cnx_init(1), { shutdown(cli_sock, SHUT_RDWR); close(cli_sock); return NULL; } );
	cli->cc_socket = cli_sock;
	cli->cc_family = serv->cc_family;
	cli->cc_proto = serv->cc_proto;
	
	/* Set the timeout */
	fd_cnx_s_setto(cli->cc_socket);
	
	/* Generate the name for the connection object */
	{
		char addrbuf[INET6_ADDRSTRLEN];
		char portbuf[10];
		int  rc;
		
		rc = getnameinfo((sSA *)&ss, ss_len, addrbuf, sizeof(addrbuf), portbuf, sizeof(portbuf), NI_NUMERICHOST | NI_NUMERICSERV);
		if (rc) {
			snprintf(addrbuf, sizeof(addrbuf), "[err:%s]", gai_strerror(rc));
			portbuf[0] = '\0';
		}
		
		/* Numeric values for debug... */
		snprintf(cli->cc_id, sizeof(cli->cc_id), CC_ID_HDR "%s from [%s]:%s (%d<-%d)", 
				IPPROTO_NAME(cli->cc_proto), addrbuf, portbuf, serv->cc_socket, cli->cc_socket);
		
		
		/* ...Name for log messages */
		rc = getnameinfo((sSA *)&ss, ss_len, cli->cc_remid, sizeof(cli->cc_remid), NULL, 0, NI_NUMERICHOST);
		if (rc)
			snprintf(cli->cc_remid, sizeof(cli->cc_remid), "[err:%s]", gai_strerror(rc));
	}
	
	LOG_D("Incoming connection: '%s' <- '%s'   {%s}", fd_cnx_getid(serv), cli->cc_remid, cli->cc_id);

#ifndef DISABLE_SCTP
	/* SCTP-specific handlings */
	if (cli->cc_proto == IPPROTO_SCTP) {
		/* Retrieve the number of streams */
		CHECK_FCT_DO( fd_sctp_get_str_info( cli->cc_socket, &cli->cc_sctp_para.str_in, &cli->cc_sctp_para.str_out, NULL ), {fd_cnx_destroy(cli); return NULL;} );
		if (cli->cc_sctp_para.str_out < cli->cc_sctp_para.str_in)
			cli->cc_sctp_para.pairs = cli->cc_sctp_para.str_out;
		else
			cli->cc_sctp_para.pairs = cli->cc_sctp_para.str_in;
		
		LOG_A( "%s : client '%s' (SCTP:%d, %d/%d streams)", fd_cnx_getid(serv), fd_cnx_getid(cli), cli->cc_socket, cli->cc_sctp_para.str_in, cli->cc_sctp_para.str_out);
	}
#endif /* DISABLE_SCTP */

	return cli;
}

/* Client side: connect to a remote server -- cancelable */
struct cnxctx * fd_cnx_cli_connect_tcp(sSA * sa /* contains the port already */, socklen_t addrlen)
{
	int sock = 0;
	struct cnxctx * cnx = NULL;
	char sa_buf[sSA_DUMP_STRLEN];
	
	TRACE_ENTRY("%p %d", sa, addrlen);
	CHECK_PARAMS_DO( sa && addrlen, return NULL );
	
	fd_sa_sdump_numeric(sa_buf, sa);
	
	LOG_D("Connecting to TCP %s...", sa_buf);
	
	/* Create the socket and connect, which can take some time and/or fail */
	{
		int ret = fd_tcp_client( &sock, sa, addrlen );
		if (ret != 0) {
			LOG_D("TCP connection to %s failed: %s", sa_buf, strerror(ret));
			return NULL;
		}
	}
	
	/* Once the socket is created successfuly, prepare the remaining of the cnx */
	CHECK_MALLOC_DO( cnx = fd_cnx_init(1), { shutdown(sock, SHUT_RDWR); close(sock); return NULL; } );
	
	cnx->cc_socket = sock;
	cnx->cc_family = sa->sa_family;
	cnx->cc_proto  = IPPROTO_TCP;
	
	/* Set the timeout */
	fd_cnx_s_setto(cnx->cc_socket);
	
	/* Generate the names for the object */
	{
		int  rc;
		
		snprintf(cnx->cc_id, sizeof(cnx->cc_id), CC_ID_HDR "TCP,#%d->%s", cnx->cc_socket, sa_buf);
		
		/* ...Name for log messages */
		rc = getnameinfo(sa, addrlen, cnx->cc_remid, sizeof(cnx->cc_remid), NULL, 0, NI_NUMERICHOST);
		if (rc)
			snprintf(cnx->cc_remid, sizeof(cnx->cc_remid), "[err:%s]", gai_strerror(rc));
	}
	
	LOG_A("TCP connection to %s succeed (socket:%d).", sa_buf, sock);
	
	return cnx;
}

/* Same for SCTP, accepts a list of remote addresses to connect to (see sctp_connectx for how they are used) */
struct cnxctx * fd_cnx_cli_connect_sctp(int no_ip6, uint16_t port, struct fd_list * list)
{
#ifdef DISABLE_SCTP
	TRACE_DEBUG(INFO, "This function should never be called when SCTP is disabled...");
	ASSERT(0);
	CHECK_FCT_DO( ENOTSUP, );
	return NULL;
#else /* DISABLE_SCTP */
	int sock = 0;
	struct cnxctx * cnx = NULL;
	char sa_buf[sSA_DUMP_STRLEN];
	sSS primary;
	
	TRACE_ENTRY("%p", list);
	CHECK_PARAMS_DO( list && !FD_IS_LIST_EMPTY(list), return NULL );
	
	fd_sa_sdump_numeric(sa_buf, &((struct fd_endpoint *)(list->next))->sa);
	
	LOG_D("Connecting to SCTP %s:%hu...", sa_buf, port);
	
	{
		int ret = fd_sctp_client( &sock, no_ip6, port, list );
		if (ret != 0) {
			LOG_D("SCTP connection to [%s,...] failed: %s", sa_buf, strerror(ret));
			return NULL;
		}
	}
	
	/* Once the socket is created successfuly, prepare the remaining of the cnx */
	CHECK_MALLOC_DO( cnx = fd_cnx_init(1), { shutdown(sock, SHUT_RDWR); close(sock); return NULL; } );
	
	cnx->cc_socket = sock;
	cnx->cc_family = no_ip6 ? AF_INET : AF_INET6;
	cnx->cc_proto  = IPPROTO_SCTP;
	
	/* Set the timeout */
	fd_cnx_s_setto(cnx->cc_socket);
	
	/* Retrieve the number of streams and primary address */
	CHECK_FCT_DO( fd_sctp_get_str_info( sock, &cnx->cc_sctp_para.str_in, &cnx->cc_sctp_para.str_out, &primary ), goto error );
	if (cnx->cc_sctp_para.str_out < cnx->cc_sctp_para.str_in)
		cnx->cc_sctp_para.pairs = cnx->cc_sctp_para.str_out;
	else
		cnx->cc_sctp_para.pairs = cnx->cc_sctp_para.str_in;
	
	fd_sa_sdump_numeric(sa_buf, (sSA *)&primary);
	
	/* Generate the names for the object */
	{
		int  rc;
		
		snprintf(cnx->cc_id, sizeof(cnx->cc_id), CC_ID_HDR "SCTP,#%d->%s", cnx->cc_socket, sa_buf);
		
		/* ...Name for log messages */
		rc = getnameinfo((sSA *)&primary, sSAlen(&primary), cnx->cc_remid, sizeof(cnx->cc_remid), NULL, 0, NI_NUMERICHOST);
		if (rc)
			snprintf(cnx->cc_remid, sizeof(cnx->cc_remid), "[err:%s]", gai_strerror(rc));
	}
	
	LOG_A("SCTP connection to %s succeed (socket:%d, %d/%d streams).", sa_buf, sock, cnx->cc_sctp_para.str_in, cnx->cc_sctp_para.str_out);
	
	return cnx;

error:
	fd_cnx_destroy(cnx);
	return NULL;
#endif /* DISABLE_SCTP */
}

/* Return a string describing the connection, for debug */
char * fd_cnx_getid(struct cnxctx * conn)
{
	CHECK_PARAMS_DO( conn, return "" );
	return conn->cc_id;
}

/* Return the protocol of a connection */
int fd_cnx_getproto(struct cnxctx * conn)
{
	CHECK_PARAMS_DO( conn, return 0 );
	return conn->cc_proto;
}

/* Set the hostname to check during handshake */
void fd_cnx_sethostname(struct cnxctx * conn, DiamId_t hn)
{
	CHECK_PARAMS_DO( conn, return );
	conn->cc_tls_para.cn = hn;
}

/* We share a lock with many threads but we hold it only very short time so it is OK */
static pthread_mutex_t state_lock = PTHREAD_MUTEX_INITIALIZER;
uint32_t fd_cnx_getstate(struct cnxctx * conn)
{
	uint32_t st;
	CHECK_POSIX_DO( pthread_mutex_lock(&state_lock), { ASSERT(0); } );
	st = conn->cc_state;
	CHECK_POSIX_DO( pthread_mutex_unlock(&state_lock), { ASSERT(0); } );
	return st;
}
int  fd_cnx_teststate(struct cnxctx * conn, uint32_t flag)
{
	uint32_t st;
	CHECK_POSIX_DO( pthread_mutex_lock(&state_lock), { ASSERT(0); } );
	st = conn->cc_state;
	CHECK_POSIX_DO( pthread_mutex_unlock(&state_lock), { ASSERT(0); } );
	return st & flag;
}
void fd_cnx_update_id(struct cnxctx * conn) {
	if (conn->cc_state & CC_STATUS_CLOSING)
		conn->cc_id[1] = 'C';
	else
		conn->cc_id[1] = '-';
	
	if (conn->cc_state & CC_STATUS_ERROR)
		conn->cc_id[2] = 'E';
	else
		conn->cc_id[2] = '-';
	
	if (conn->cc_state & CC_STATUS_SIGNALED)
		conn->cc_id[3] = 'S';
	else
		conn->cc_id[3] = '-';
	
	if (conn->cc_state & CC_STATUS_TLS)
		conn->cc_id[4] = 'T';
	else
		conn->cc_id[4] = '-';
}
void fd_cnx_addstate(struct cnxctx * conn, uint32_t orstate)
{
	CHECK_POSIX_DO( pthread_mutex_lock(&state_lock), { ASSERT(0); } );
	conn->cc_state |= orstate;
	fd_cnx_update_id(conn);
	CHECK_POSIX_DO( pthread_mutex_unlock(&state_lock), { ASSERT(0); } );
}
void fd_cnx_setstate(struct cnxctx * conn, uint32_t abstate)
{
	CHECK_POSIX_DO( pthread_mutex_lock(&state_lock), { ASSERT(0); } );
	conn->cc_state = abstate;
	fd_cnx_update_id(conn);
	CHECK_POSIX_DO( pthread_mutex_unlock(&state_lock), { ASSERT(0); } );
}


/* Return the TLS state of a connection */
int fd_cnx_getTLS(struct cnxctx * conn)
{
	CHECK_PARAMS_DO( conn, return 0 );
	return fd_cnx_teststate(conn, CC_STATUS_TLS);
}

/* Mark the connection to tell if OOO delivery is permitted (only for SCTP) */
int fd_cnx_unordered_delivery(struct cnxctx * conn, int is_allowed)
{
	CHECK_PARAMS( conn );
	conn->cc_sctp_para.unordered = is_allowed;
	return 0;
}

/* Return true if the connection supports unordered delivery of messages */
int fd_cnx_is_unordered_delivery_supported(struct cnxctx * conn)
{
	CHECK_PARAMS_DO( conn, return 0 );
	#ifndef DISABLE_SCTP
	if (conn->cc_proto == IPPROTO_SCTP)
		return (conn->cc_sctp_para.str_out > 1);
	#endif /* DISABLE_SCTP */
	return 0;
}


/* Get the list of endpoints (IP addresses) of the local and remote peers on this connection */
int fd_cnx_getremoteeps(struct cnxctx * conn, struct fd_list * eps)
{
	TRACE_ENTRY("%p %p", conn, eps);
	CHECK_PARAMS(conn && eps);
	
	/* Check we have a full connection object, not a listening socket (with no remote) */
	CHECK_PARAMS( conn->cc_incoming );

	/* Retrieve the peer endpoint(s) of the connection */
	switch (conn->cc_proto) {
		case IPPROTO_TCP: {
			sSS ss;
			socklen_t sl;
			CHECK_FCT(fd_tcp_get_remote_ep(conn->cc_socket, &ss, &sl));
			CHECK_FCT(fd_ep_add_merge( eps, (sSA *)&ss, sl, EP_FL_LL | EP_FL_PRIMARY ));
		}
		break;

		#ifndef DISABLE_SCTP
		case IPPROTO_SCTP: {
			CHECK_FCT(fd_sctp_get_remote_ep(conn->cc_socket, eps));
		}
		break;
		#endif /* DISABLE_SCTP */

		default:
			CHECK_PARAMS(0);
	}

	return 0;
}

/* Get a string describing the remote peer address (ip address or fqdn) */
char * fd_cnx_getremoteid(struct cnxctx * conn)
{
	CHECK_PARAMS_DO( conn, return "" );
	return conn->cc_remid;
}

static int fd_cnx_may_dtls(struct cnxctx * conn);

/* Get a short string representing the connection */
int fd_cnx_proto_info(struct cnxctx * conn, char * buf, size_t len) 
{
	CHECK_PARAMS( conn );
	
	if (fd_cnx_teststate(conn, CC_STATUS_TLS)) {
		snprintf(buf, len, "%s,%s,soc#%d", IPPROTO_NAME(conn->cc_proto), fd_cnx_may_dtls(conn) ? "DTLS" : "TLS", conn->cc_socket);
	} else {
		snprintf(buf, len, "%s,soc#%d", IPPROTO_NAME(conn->cc_proto), conn->cc_socket);
	}
	
	return 0;
}

/* Retrieve a list of all IP addresses of the local system from the kernel, using getifaddrs */
int fd_cnx_get_local_eps(struct fd_list * list)
{
	struct ifaddrs *iflist, *cur;
	
	CHECK_SYS(getifaddrs(&iflist));
	
	for (cur = iflist; cur != NULL; cur = cur->ifa_next) {
		if (cur->ifa_flags & IFF_LOOPBACK)
			continue;
		
		if (cur->ifa_addr == NULL) /* may happen with ppp interfaces */
			continue;
		
		if (fd_g_config->cnf_flags.no_ip4 && (cur->ifa_addr->sa_family == AF_INET))
			continue;
		
		if (fd_g_config->cnf_flags.no_ip6 && (cur->ifa_addr->sa_family == AF_INET6))
			continue;
		
		CHECK_FCT(fd_ep_add_merge( list, cur->ifa_addr, sSAlen(cur->ifa_addr), EP_FL_LL ));
	}
	
	freeifaddrs(iflist);
	
	return 0;
}


/**************************************/
/*     Use of a connection object     */
/**************************************/

/* An error occurred on the socket */
void fd_cnx_markerror(struct cnxctx * conn)
{
	TRACE_ENTRY("%p", conn);
	CHECK_PARAMS_DO( conn, goto fatal );
	
	TRACE_DEBUG(FULL, "Error flag set for socket %d (%s, %s)", conn->cc_socket, conn->cc_id, conn->cc_remid);

	/* Mark the error */
	fd_cnx_addstate(conn, CC_STATUS_ERROR);
	
	/* Report the error if not reported yet, and not closing */
	if (!fd_cnx_teststate(conn, CC_STATUS_CLOSING | CC_STATUS_SIGNALED ))  {
		TRACE_DEBUG(FULL, "Sending FDEVP_CNX_ERROR event");
		CHECK_FCT_DO( fd_event_send( fd_cnx_target_queue(conn), FDEVP_CNX_ERROR, 0, NULL), goto fatal);
		fd_cnx_addstate(conn, CC_STATUS_SIGNALED);
	}
	
	return;
fatal:
	/* An unrecoverable error occurred, stop the daemon */
	ASSERT(0);
	CHECK_FCT_DO(fd_core_shutdown(), );	
}

/* Set the timeout option on the socket */
void fd_cnx_s_setto(int sock) 
{
	struct timeval tv;
	
	/* Set a timeout on the socket so that in any case we are not stuck waiting for something */
	memset(&tv, 0, sizeof(tv));
	tv.tv_usec = 100000L;	/* 100ms, to react quickly to head-of-the-line blocking. */
	CHECK_SYS_DO( setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)),  );
	CHECK_SYS_DO( setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)),  );
}


#ifdef GNUTLS_VERSION_300
/* The pull_timeout function for gnutls */
static int fd_cnx_s_select (struct cnxctx * conn, unsigned int ms)
{
	fd_set rfds;
	struct timeval tv;
	
	FD_ZERO (&rfds);
	FD_SET (conn->cc_socket, &rfds);
	
	tv.tv_sec = ms / 1000;
	tv.tv_usec = (ms * 1000) % 1000000;
	
	return select (conn->cc_socket + 1, &rfds, NULL, NULL, &tv);
}		
#endif /* GNUTLS_VERSION_300 */

/* A recv-like function, taking a cnxctx object instead of socket as entry. We use it to quickly react to timeouts without traversing GNUTLS wrapper each time */
ssize_t fd_cnx_s_recv(struct cnxctx * conn, void *buffer, size_t length)
{
	ssize_t ret = 0;
	int timedout = 0;
again:
	ret = recv(conn->cc_socket, buffer, length, 0);
	/* Handle special case of timeout / interrupts */
	if ((ret < 0) && ((errno == EAGAIN) || (errno == EINTR))) {
		pthread_testcancel();
		if (! fd_cnx_teststate(conn, CC_STATUS_CLOSING ))
			goto again; /* don't care, just ignore */
		if (!timedout) {
			timedout ++; /* allow for one timeout while closing */
			goto again;
		}
	}
	
	/* Mark the error */
	if (ret <= 0) {
		CHECK_SYS_DO(ret, /* continue, this is only used to log the error here */);
		fd_cnx_markerror(conn);
	}
	
	return ret;
}

/* Send */
static ssize_t fd_cnx_s_sendv(struct cnxctx * conn, const struct iovec * iov, int iovcnt)
{
	ssize_t ret = 0;
	struct timespec ts, now;
	CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &ts), return -1 );
again:
	ret = writev(conn->cc_socket, iov, iovcnt);
	/* Handle special case of timeout */
	if ((ret < 0) && ((errno == EAGAIN) || (errno == EINTR))) {
		ret = -errno;
		pthread_testcancel();
		
		/* Check how much time we were blocked for this sending. */
		CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &now), return -1 );
		if ( ((now.tv_sec - ts.tv_sec) * 1000 + ((now.tv_nsec - ts.tv_nsec) / 1000000L)) > MAX_HOTL_BLOCKING_TIME) {
			LOG_D("Unable to send any data for %dms, closing the connection", MAX_HOTL_BLOCKING_TIME);
		} else if (! fd_cnx_teststate(conn, CC_STATUS_CLOSING )) {
			goto again; /* don't care, just ignore */
		}
		
		/* propagate the error */
		errno = -ret;
		ret = -1;
		CHECK_SYS_DO(ret, /* continue */);
	}
	
	/* Mark the error */
	if (ret <= 0)
		fd_cnx_markerror(conn);
	
	return ret;
}

/* Send, for older GNUTLS */
#ifndef GNUTLS_VERSION_212
static ssize_t fd_cnx_s_send(struct cnxctx * conn, const void *buffer, size_t length)
{
	struct iovec iov;
	iov.iov_base = (void *)buffer;
	iov.iov_len  = length;
	return fd_cnx_s_sendv(conn, &iov, 1);
}
#endif /* GNUTLS_VERSION_212 */

#define ALIGNOF(t) ((char *)(&((struct { char c; t _h; } *)0)->_h) - (char *)0)  /* Could use __alignof__(t) on some systems but this is more portable probably */
#define PMDL_PADDED(len) ( ((len) + ALIGNOF(struct fd_msg_pmdl) - 1) & ~(ALIGNOF(struct fd_msg_pmdl) - 1) )

size_t fd_msg_pmdl_sizewithoverhead(size_t datalen)
{
	return PMDL_PADDED(datalen) + sizeof(struct fd_msg_pmdl);
}

struct fd_msg_pmdl * fd_msg_pmdl_get_inbuf(uint8_t * buf, size_t datalen)
{
	return (struct fd_msg_pmdl *)(buf + PMDL_PADDED(datalen));
} 

static int fd_cnx_init_msg_buffer(uint8_t * buffer, size_t expected_len, struct fd_msg_pmdl ** pmdl)
{
	*pmdl = fd_msg_pmdl_get_inbuf(buffer, expected_len);
	fd_list_init(&(*pmdl)->sentinel, NULL);
	CHECK_POSIX(pthread_mutex_init(&(*pmdl)->lock, NULL) );
	return 0;
}

static uint8_t * fd_cnx_alloc_msg_buffer(size_t expected_len, struct fd_msg_pmdl ** pmdl)
{
	uint8_t * ret = NULL;
	
	CHECK_MALLOC_DO(  ret = malloc( fd_msg_pmdl_sizewithoverhead(expected_len) ), return NULL );
	CHECK_FCT_DO( fd_cnx_init_msg_buffer(ret, expected_len, pmdl), {free(ret); return NULL;} );
	return ret;
}

#ifndef DISABLE_SCTP /* WE use this function only in SCTP code */
static uint8_t * fd_cnx_realloc_msg_buffer(uint8_t * buffer, size_t expected_len, struct fd_msg_pmdl ** pmdl)
{
	uint8_t * ret = NULL;
	
	CHECK_MALLOC_DO(  ret = realloc( buffer, fd_msg_pmdl_sizewithoverhead(expected_len) ), return NULL );
	CHECK_FCT_DO( fd_cnx_init_msg_buffer(ret, expected_len, pmdl), {free(ret); return NULL;} );
	return ret;
}
#endif /* DISABLE_SCTP */

static void free_rcvdata(void * arg) 
{
	struct fd_cnx_rcvdata * data = arg;
	struct fd_msg_pmdl * pmdl = fd_msg_pmdl_get_inbuf(data->buffer, data->length);
	(void) pthread_mutex_destroy(&pmdl->lock);
	free(data->buffer);
}

/* Receiver thread (TCP & noTLS) : incoming message is directly saved into the target queue */
static void * rcvthr_notls_tcp(void * arg)
{
	struct cnxctx * conn = arg;
	
	TRACE_ENTRY("%p", arg);
	CHECK_PARAMS_DO(conn && (conn->cc_socket > 0), goto out);
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "Receiver (%d) TCP/noTLS)", conn->cc_socket);
		fd_log_threadname ( buf );
	}
	
	ASSERT( conn->cc_proto == IPPROTO_TCP );
	ASSERT( ! fd_cnx_teststate(conn, CC_STATUS_TLS ) );
	ASSERT( fd_cnx_target_queue(conn) );
	
	/* Receive from a TCP connection: we have to rebuild the message boundaries */
	do {
		uint8_t header[4];
		struct fd_cnx_rcvdata rcv_data;
		struct fd_msg_pmdl *pmdl=NULL;
		ssize_t ret = 0;
		size_t	received = 0;

		do {
			ret = fd_cnx_s_recv(conn, &header[received], sizeof(header) - received);
			if (ret <= 0) {
				goto out; /* Stop the thread, the event was already sent */
			}

			received += ret;
			
			if (header[0] != DIAMETER_VERSION)
				break; /* No need to wait for 4 bytes in this case */
		} while (received < sizeof(header));

		rcv_data.length = ((size_t)header[1] << 16) + ((size_t)header[2] << 8) + (size_t)header[3];

		/* Check the received word is a valid begining of a Diameter message */
		if ((header[0] != DIAMETER_VERSION)	/* defined in <libfdproto.h> */
		   || (rcv_data.length > DIAMETER_MSG_SIZE_MAX)) { /* to avoid too big mallocs */
			/* The message is suspect */
			LOG_E( "Received suspect header [ver: %d, size: %zd] from '%s', assuming disconnection", (int)header[0], rcv_data.length, conn->cc_remid);
			fd_cnx_markerror(conn);
			goto out; /* Stop the thread, the recipient of the event will cleanup */
		}

		/* Ok, now we can really receive the data */
		CHECK_MALLOC_DO(  rcv_data.buffer = fd_cnx_alloc_msg_buffer( rcv_data.length, &pmdl ), goto fatal );
		memcpy(rcv_data.buffer, header, sizeof(header));

		while (received < rcv_data.length) {
			pthread_cleanup_push(free_rcvdata, &rcv_data); /* In case we are canceled, clean the partialy built buffer */
			ret = fd_cnx_s_recv(conn, rcv_data.buffer + received, rcv_data.length - received);
			pthread_cleanup_pop(0);

			if (ret <= 0) {
				free_rcvdata(&rcv_data);
				goto out;
			}
			received += ret;
		}
		
		fd_hook_call(HOOK_DATA_RECEIVED, NULL, NULL, &rcv_data, pmdl);
		
		/* We have received a complete message, pass it to the daemon */
		CHECK_FCT_DO( fd_event_send( fd_cnx_target_queue(conn), FDEVP_CNX_MSG_RECV, rcv_data.length, rcv_data.buffer), 
			{ 
				free_rcvdata(&rcv_data);
				goto fatal; 
			} );
		
	} while (conn->cc_loop);
	
out:
	TRACE_DEBUG(FULL, "Thread terminated");	
	return NULL;
	
fatal:
	/* An unrecoverable error occurred, stop the daemon */
	CHECK_FCT_DO(fd_core_shutdown(), );
	goto out;
}

#ifndef DISABLE_SCTP
/* Receiver thread (SCTP & noTLS) : incoming message is directly saved into cc_incoming, no need to care for the stream ID */
static void * rcvthr_notls_sctp(void * arg)
{
	struct cnxctx * conn = arg;
	struct fd_cnx_rcvdata rcv_data;
	int	  event;
	
	TRACE_ENTRY("%p", arg);
	CHECK_PARAMS_DO(conn && (conn->cc_socket > 0), goto fatal);
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "Receiver (%d) SCTP/noTLS)", conn->cc_socket);
		fd_log_threadname ( buf );
	}
	
	ASSERT( conn->cc_proto == IPPROTO_SCTP );
	ASSERT( ! fd_cnx_teststate(conn, CC_STATUS_TLS ) );
	ASSERT( fd_cnx_target_queue(conn) );
	
	do {
		struct fd_msg_pmdl *pmdl=NULL;
		CHECK_FCT_DO( fd_sctp_recvmeta(conn, NULL, &rcv_data.buffer, &rcv_data.length, &event), goto fatal );
		if (event == FDEVP_CNX_ERROR) {
			fd_cnx_markerror(conn);
			goto out;
		}
		
		if (event == FDEVP_CNX_SHUTDOWN) {
			/* Just ignore the notification for now, we will get another error later anyway */
			continue;
		}

		if (event == FDEVP_CNX_MSG_RECV) {
			CHECK_MALLOC_DO( rcv_data.buffer = fd_cnx_realloc_msg_buffer(rcv_data.buffer, rcv_data.length, &pmdl), goto fatal );
			fd_hook_call(HOOK_DATA_RECEIVED, NULL, NULL, &rcv_data, pmdl);
		}
		CHECK_FCT_DO( fd_event_send( fd_cnx_target_queue(conn), event, rcv_data.length, rcv_data.buffer), goto fatal );
		
	} while (conn->cc_loop || (event != FDEVP_CNX_MSG_RECV));
	
out:
	TRACE_DEBUG(FULL, "Thread terminated");	
	return NULL;

fatal:
	/* An unrecoverable error occurred, stop the daemon */
	CHECK_FCT_DO(fd_core_shutdown(), );
	goto out;
}
#endif /* DISABLE_SCTP */

/* Start receving messages in clear (no TLS) on the connection */
int fd_cnx_start_clear(struct cnxctx * conn, int loop)
{
	TRACE_ENTRY("%p %i", conn, loop);
	
	CHECK_PARAMS( conn && fd_cnx_target_queue(conn) && (!fd_cnx_teststate(conn, CC_STATUS_TLS)) && (!conn->cc_loop));
	
	/* Release resources in case of a previous call was already made */
	CHECK_FCT_DO( fd_thr_term(&conn->cc_rcvthr), /* continue */);
	
	/* Save the loop request */
	conn->cc_loop = loop;
	
	switch (conn->cc_proto) {
		case IPPROTO_TCP:
			/* Start the tcp_notls thread */
			CHECK_POSIX( pthread_create( &conn->cc_rcvthr, NULL, rcvthr_notls_tcp, conn ) );
			break;
#ifndef DISABLE_SCTP
		case IPPROTO_SCTP:
			/* Start the tcp_notls thread */
			CHECK_POSIX( pthread_create( &conn->cc_rcvthr, NULL, rcvthr_notls_sctp, conn ) );
			break;
#endif /* DISABLE_SCTP */
		default:
			TRACE_DEBUG(INFO, "Unknown protocol: %d", conn->cc_proto);
			ASSERT(0);
			return ENOTSUP;
	}
			
	return 0;
}




/* Returns 0 on error, received data size otherwise (always >= 0). This is not used for DTLS-protected associations. */
static ssize_t fd_tls_recv_handle_error(struct cnxctx * conn, gnutls_session_t session, void * data, size_t sz)
{
	ssize_t ret;
again:	
	CHECK_GNUTLS_DO( ret = gnutls_record_recv(session, data, sz), 
		{
			switch (ret) {
				case GNUTLS_E_REHANDSHAKE: 
					if (!fd_cnx_teststate(conn, CC_STATUS_CLOSING)) {
						CHECK_GNUTLS_DO( ret = gnutls_handshake(session),
							{
								if (TRACE_BOOL(INFO)) {
									fd_log_debug("TLS re-handshake failed on socket %d (%s) : %s", conn->cc_socket, conn->cc_id, gnutls_strerror(ret));
								}
								goto end;
							} );
					}

				case GNUTLS_E_AGAIN:
				case GNUTLS_E_INTERRUPTED:
					if (!fd_cnx_teststate(conn, CC_STATUS_CLOSING))
						goto again;
					TRACE_DEBUG(FULL, "Connection is closing, so abord gnutls_record_recv now.");
					break;

				case GNUTLS_E_UNEXPECTED_PACKET_LENGTH:
					/* The connection is closed */
					TRACE_DEBUG(FULL, "Got 0 size while reading the socket, probably connection closed...");
					break;
				
				default:
					if (gnutls_error_is_fatal (ret) == 0) {
						LOG_N("Ignoring non-fatal GNU TLS error: %s", gnutls_strerror (ret));
						goto again;
					}
					LOG_E("Fatal GNUTLS error: %s", gnutls_strerror (ret));
			}
		} );
		
	if (ret == 0)
		CHECK_GNUTLS_DO( gnutls_bye(session, GNUTLS_SHUT_RDWR),  );
	
end:	
	if (ret <= 0)
		fd_cnx_markerror(conn);
	return ret;
}

/* Wrapper around gnutls_record_send to handle some error codes. This is also used for DTLS-protected associations */
static ssize_t fd_tls_send_handle_error(struct cnxctx * conn, gnutls_session_t session, void * data, size_t sz)
{
	ssize_t ret;
	struct timespec ts, now;
	CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &ts), return -1 );
again:	
	CHECK_GNUTLS_DO( ret = gnutls_record_send(session, data, sz),
		{
			pthread_testcancel();
			switch (ret) {
				case GNUTLS_E_REHANDSHAKE: 
					if (!fd_cnx_teststate(conn, CC_STATUS_CLOSING)) {
						CHECK_GNUTLS_DO( ret = gnutls_handshake(session),
							{
								if (TRACE_BOOL(INFO)) {
									fd_log_debug("TLS re-handshake failed on socket %d (%s) : %s", conn->cc_socket, conn->cc_id, gnutls_strerror(ret));
								}
								goto end;
							} );
					}

				case GNUTLS_E_AGAIN:
				case GNUTLS_E_INTERRUPTED:
					CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &now), return -1 );
					if ( ((now.tv_sec - ts.tv_sec) * 1000 + ((now.tv_nsec - ts.tv_nsec) / 1000000L)) > MAX_HOTL_BLOCKING_TIME) {
						LOG_D("Unable to send any data for %dms, closing the connection", MAX_HOTL_BLOCKING_TIME);
					} else if (! fd_cnx_teststate(conn, CC_STATUS_CLOSING )) {
						goto again;
					}
					break;

				default:
					if (gnutls_error_is_fatal (ret) == 0) {
						LOG_N("Ignoring non-fatal GNU TLS error: %s", gnutls_strerror (ret));
						goto again;
					}
					LOG_E("Fatal GNUTLS error: %s", gnutls_strerror (ret));
			}
		} );
end:	
	if (ret <= 0)
		fd_cnx_markerror(conn);
		
	return ret;
}


/* The function that receives TLS data and re-builds a Diameter message -- it exits only on error or cancelation */
/* 	   For the case of DTLS, since we are not using SCTP_UNORDERED, the messages over a single stream are ordered.
	   Furthermore, as long as messages are shorter than the MTU [2^14 = 16384 bytes], they are delivered in a single
	   record, as far as I understand. 
	   For larger messages, however, it is possible that pieces of messages coming from different streams can get interleaved. 
	   As a result, we do not use the following function for DTLS reception, because we use the sequence number to rebuild the
	   messages. */
int fd_tls_rcvthr_core(struct cnxctx * conn, gnutls_session_t session)
{
	/* No guarantee that GnuTLS preserves the message boundaries, so we re-build it as in TCP. */
	do {
		uint8_t header[4];
		struct fd_cnx_rcvdata rcv_data;
		struct fd_msg_pmdl *pmdl=NULL;
		ssize_t ret = 0;
		size_t	received = 0;

		do {
			ret = fd_tls_recv_handle_error(conn, session, &header[received], sizeof(header) - received);
			if (ret <= 0) {
				/* The connection is closed */
				goto out;
			}
			received += ret;
		} while (received < sizeof(header));

		rcv_data.length = ((size_t)header[1] << 16) + ((size_t)header[2] << 8) + (size_t)header[3];

		/* Check the received word is a valid beginning of a Diameter message */
		if ((header[0] != DIAMETER_VERSION)	/* defined in <libfreeDiameter.h> */
		   || (rcv_data.length > DIAMETER_MSG_SIZE_MAX)) { /* to avoid too big mallocs */
			/* The message is suspect */
			LOG_E( "Received suspect header [ver: %d, size: %zd] from '%s', assume disconnection", (int)header[0], rcv_data.length, conn->cc_remid);
			fd_cnx_markerror(conn);
			goto out;
		}

		/* Ok, now we can really receive the data */
		CHECK_MALLOC(  rcv_data.buffer = fd_cnx_alloc_msg_buffer( rcv_data.length, &pmdl ) );
		memcpy(rcv_data.buffer, header, sizeof(header));

		while (received < rcv_data.length) {
			pthread_cleanup_push(free_rcvdata, &rcv_data); /* In case we are canceled, clean the partialy built buffer */
			ret = fd_tls_recv_handle_error(conn, session, rcv_data.buffer + received, rcv_data.length - received);
			pthread_cleanup_pop(0);

			if (ret <= 0) {
				free_rcvdata(&rcv_data);
				goto out;
			}
			received += ret;
		}
		
		fd_hook_call(HOOK_DATA_RECEIVED, NULL, NULL, &rcv_data, pmdl);
		
		/* We have received a complete message, pass it to the daemon */
		CHECK_FCT_DO( ret = fd_event_send( fd_cnx_target_queue(conn), FDEVP_CNX_MSG_RECV, rcv_data.length, rcv_data.buffer), 
			{ 
				free_rcvdata(&rcv_data);
				CHECK_FCT_DO(fd_core_shutdown(), );
				return ret; 
			} );
		
	} while (1);
	
out:
	return ENOTCONN;
}

/* Receiver thread (TLS & 1 stream SCTP or TCP)  */
static void * rcvthr_tls_single(void * arg)
{
	struct cnxctx * conn = arg;
	
	TRACE_ENTRY("%p", arg);
	CHECK_PARAMS_DO(conn && (conn->cc_socket > 0), return NULL );
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "Receiver (%d) TLS/single stream", conn->cc_socket);
		fd_log_threadname ( buf );
	}
	
	ASSERT( fd_cnx_teststate(conn, CC_STATUS_TLS) );
	ASSERT( fd_cnx_target_queue(conn) );

	/* The next function only returns when there is an error on the socket */	
	CHECK_FCT_DO(fd_tls_rcvthr_core(conn, conn->cc_tls_para.session), /* continue */);

	TRACE_DEBUG(FULL, "Thread terminated");	
	return NULL;
}

/* Prepare a gnutls session object for handshake */
int fd_tls_prepare(gnutls_session_t * session, int mode, int dtls, char * priority, void * alt_creds)
{
	if (dtls) {
		LOG_E("DTLS sessions not yet supported");
		return ENOTSUP;
	}

	/* Create the session context */
	CHECK_GNUTLS_DO( gnutls_init (session, mode), return ENOMEM );

	/* Set the algorithm suite */
	if (priority) {
		const char * errorpos;
		CHECK_GNUTLS_DO( gnutls_priority_set_direct( *session, priority, &errorpos ), 
			{ TRACE_DEBUG(INFO, "Error in priority string '%s' at position: '%s'", priority, errorpos); return EINVAL; } );
	} else {
		CHECK_GNUTLS_DO( gnutls_priority_set( *session, fd_g_config->cnf_sec_data.prio_cache ), return EINVAL );
	}

	/* Set the credentials of this side of the connection */
	CHECK_GNUTLS_DO( gnutls_credentials_set (*session, GNUTLS_CRD_CERTIFICATE, alt_creds ?: fd_g_config->cnf_sec_data.credentials), return EINVAL );

	/* Request the remote credentials as well */
	if (mode == GNUTLS_SERVER) {
		gnutls_certificate_server_set_request (*session, GNUTLS_CERT_REQUIRE);
	}
		
	return 0;
}

#ifndef GNUTLS_VERSION_300

/* Verify remote credentials after successful handshake (return 0 if OK, EINVAL otherwise) */
int fd_tls_verify_credentials(gnutls_session_t session, struct cnxctx * conn, int verbose)
{
	int i, ret = 0;
	unsigned int gtret;
	const gnutls_datum_t *cert_list;
	unsigned int cert_list_size;
	gnutls_x509_crt_t cert;
	time_t now;
	
	TRACE_ENTRY("%p %d", conn, verbose);
	CHECK_PARAMS(conn);
	
	/* Trace the session information -- http://www.gnu.org/software/gnutls/manual/gnutls.html#Obtaining-session-information */
	#ifdef DEBUG
	if (verbose) {
		const char *tmp;
		gnutls_kx_algorithm_t kx;
  		gnutls_credentials_type_t cred;
		
		LOG_D("TLS Session information for connection '%s':", conn->cc_id);

		/* print the key exchange's algorithm name */
		GNUTLS_TRACE( kx = gnutls_kx_get (session) );
		GNUTLS_TRACE( tmp = gnutls_kx_get_name (kx) );
		LOG_D("\t - Key Exchange: %s", tmp);

		/* Check the authentication type used and switch
		* to the appropriate. */
		GNUTLS_TRACE( cred = gnutls_auth_get_type (session) );
		switch (cred)
		{
			case GNUTLS_CRD_IA:
				LOG_D("\t - TLS/IA session");
				break;

			case GNUTLS_CRD_PSK:
				/* This returns NULL in server side. */
				if (gnutls_psk_client_get_hint (session) != NULL)
					LOG_D("\t - PSK authentication. PSK hint '%s'",
						gnutls_psk_client_get_hint (session));
				/* This returns NULL in client side. */
				if (gnutls_psk_server_get_username (session) != NULL)
					LOG_D("\t - PSK authentication. Connected as '%s'",
						gnutls_psk_server_get_username (session));
				break;

			case GNUTLS_CRD_ANON:	/* anonymous authentication */
				LOG_D("\t - Anonymous DH using prime of %d bits",
					gnutls_dh_get_prime_bits (session));
				break;

			case GNUTLS_CRD_CERTIFICATE:	/* certificate authentication */
				/* Check if we have been using ephemeral Diffie-Hellman. */
				if (kx == GNUTLS_KX_DHE_RSA || kx == GNUTLS_KX_DHE_DSS) {
					LOG_D("\t - Ephemeral DH using prime of %d bits",
						gnutls_dh_get_prime_bits (session));
				}
				break;
#ifdef ENABLE_SRP				
			case GNUTLS_CRD_SRP:
				LOG_D("\t - SRP session with username %s",
					gnutls_srp_server_get_username (session));
				break;
#endif /* ENABLE_SRP */

			default:
				fd_log_debug("\t - Different type of credentials for the session (%d).", cred);
				break;

		}

		/* print the protocol's name (ie TLS 1.0) */
		tmp = gnutls_protocol_get_name (gnutls_protocol_get_version (session));
		LOG_D("\t - Protocol: %s", tmp);

		/* print the certificate type of the peer. ie X.509 */
		tmp = gnutls_certificate_type_get_name (gnutls_certificate_type_get (session));
		LOG_D("\t - Certificate Type: %s", tmp);

		/* print the compression algorithm (if any) */
		tmp = gnutls_compression_get_name (gnutls_compression_get (session));
		LOG_D("\t - Compression: %s", tmp);

		/* print the name of the cipher used. ie 3DES. */
		tmp = gnutls_cipher_get_name (gnutls_cipher_get (session));
		LOG_D("\t - Cipher: %s", tmp);

		/* Print the MAC algorithms name. ie SHA1 */
		tmp = gnutls_mac_get_name (gnutls_mac_get (session));
		LOG_D("\t - MAC: %s", tmp);
	}
	#endif /* DEBUG */
	
	/* First, use built-in verification */
	CHECK_GNUTLS_DO( gnutls_certificate_verify_peers2 (session, &gtret), return EINVAL );
	if (gtret) {
		LOG_E("TLS: Remote certificate invalid on socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
		if (gtret & GNUTLS_CERT_INVALID)
			LOG_E(" - The certificate is not trusted (unknown CA? expired?)");
		if (gtret & GNUTLS_CERT_REVOKED)
			LOG_E(" - The certificate has been revoked.");
		if (gtret & GNUTLS_CERT_SIGNER_NOT_FOUND)
			LOG_E(" - The certificate hasn't got a known issuer.");
		if (gtret & GNUTLS_CERT_SIGNER_NOT_CA)
			LOG_E(" - The certificate signer is not a CA, or uses version 1, or 3 without basic constraints.");
		if (gtret & GNUTLS_CERT_INSECURE_ALGORITHM)
			LOG_E(" - The certificate signature uses a weak algorithm.");
		return EINVAL;
	}
	
	/* Code from http://www.gnu.org/software/gnutls/manual/gnutls.html#Verifying-peer_0027s-certificate */
	if (gnutls_certificate_type_get (session) != GNUTLS_CRT_X509) {
		LOG_E("TLS: Remote peer did not present a certificate, other mechanisms are not supported yet. socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
		return EINVAL;
	}
	
	GNUTLS_TRACE( cert_list = gnutls_certificate_get_peers (session, &cert_list_size) );
	if (cert_list == NULL)
		return EINVAL;
	
	now = time(NULL);
	
	#ifdef DEBUG
		char serial[40];
		char dn[128];
		size_t size;
		unsigned int algo, bits;
		time_t expiration_time, activation_time;
		
		LOG_D("TLS Certificate information for connection '%s' (%d certs provided):", conn->cc_id, cert_list_size);
		for (i = 0; i < cert_list_size; i++)
		{

			CHECK_GNUTLS_DO( gnutls_x509_crt_init (&cert), return EINVAL);
			CHECK_GNUTLS_DO( gnutls_x509_crt_import (cert, &cert_list[i], GNUTLS_X509_FMT_DER), return EINVAL);
		
			LOG_A(" Certificate %d info:", i);

			GNUTLS_TRACE( expiration_time = gnutls_x509_crt_get_expiration_time (cert) );
			GNUTLS_TRACE( activation_time = gnutls_x509_crt_get_activation_time (cert) );

			LOG( i ? FD_LOG_ANNOYING : FD_LOG_DEBUG, "\t - Certificate is valid since: %.24s", ctime (&activation_time));
			LOG( i ? FD_LOG_ANNOYING : FD_LOG_DEBUG, "\t - Certificate expires: %.24s", ctime (&expiration_time));

			/* Print the serial number of the certificate. */
			size = sizeof (serial);
			gnutls_x509_crt_get_serial (cert, serial, &size);
			
			{
				int j;
				char buf[1024];
				snprintf(buf, sizeof(buf), "\t - Certificate serial number: ");
				for (j = 0; j < size; j++) {
					snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), "%02hhx", serial[j]);
				}
				LOG( i ? FD_LOG_ANNOYING : FD_LOG_DEBUG, "%s", buf);
			}

			/* Extract some of the public key algorithm's parameters */
			GNUTLS_TRACE( algo = gnutls_x509_crt_get_pk_algorithm (cert, &bits) );
			LOG( i ? FD_LOG_ANNOYING : FD_LOG_DEBUG, "\t - Certificate public key: %s",
			      gnutls_pk_algorithm_get_name (algo));

			/* Print the version of the X.509 certificate. */
			LOG( i ? FD_LOG_ANNOYING : FD_LOG_DEBUG, "\t - Certificate version: #%d",
			      gnutls_x509_crt_get_version (cert));

			size = sizeof (dn);
			GNUTLS_TRACE( gnutls_x509_crt_get_dn (cert, dn, &size) );
			LOG( i ? FD_LOG_ANNOYING : FD_LOG_DEBUG, "\t - DN: %s", dn);

			size = sizeof (dn);
			GNUTLS_TRACE( gnutls_x509_crt_get_issuer_dn (cert, dn, &size) );
			LOG( i ? FD_LOG_ANNOYING : FD_LOG_DEBUG, "\t - Issuer's DN: %s", dn);

			GNUTLS_TRACE( gnutls_x509_crt_deinit (cert) );
		}
	#endif /* DEBUG */

	/* Check validity of all the certificates */
	for (i = 0; i < cert_list_size; i++)
	{
		time_t deadline;
		
		CHECK_GNUTLS_DO( gnutls_x509_crt_init (&cert), return EINVAL);
		CHECK_GNUTLS_DO( gnutls_x509_crt_import (cert, &cert_list[i], GNUTLS_X509_FMT_DER), return EINVAL);
		
		GNUTLS_TRACE( deadline = gnutls_x509_crt_get_expiration_time(cert) );
		if ((deadline != (time_t)-1) && (deadline < now)) {
			LOG_E("TLS: Remote certificate invalid on socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
			LOG_E(" - The certificate %d in the chain is expired", i);
			ret = EINVAL;
		}
		
		GNUTLS_TRACE( deadline = gnutls_x509_crt_get_activation_time(cert) );
		if ((deadline != (time_t)-1) && (deadline > now)) {
			LOG_E("TLS: Remote certificate invalid on socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
			LOG_E(" - The certificate %d in the chain is not yet activated", i);
			ret = EINVAL;
		}
		
		if ((i == 0) && (conn->cc_tls_para.cn)) {
			if (!gnutls_x509_crt_check_hostname (cert, conn->cc_tls_para.cn)) {
				LOG_E("TLS: Remote certificate invalid on socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
				LOG_E(" - The certificate hostname does not match '%s'", conn->cc_tls_para.cn);
				ret = EINVAL;
			}
		}
		
		GNUTLS_TRACE( gnutls_x509_crt_deinit (cert) );
	}

	return ret;
}

#else /* GNUTLS_VERSION_300 */

/* Verify remote credentials DURING handshake (return gnutls status) */
int fd_tls_verify_credentials_2(gnutls_session_t session)
{
	/* inspired from gnutls 3.x guidelines */
	unsigned int status;
	const gnutls_datum_t *cert_list = NULL;
	unsigned int cert_list_size;
	gnutls_x509_crt_t cert;
	struct cnxctx * conn;
	int hostname_verified = 0;

	TRACE_ENTRY("%p", session);
	
	/* get the associated connection */
	conn = gnutls_session_get_ptr (session);
	
	/* Trace the session information -- http://www.gnu.org/software/gnutls/manual/gnutls.html#Obtaining-session-information */
#ifdef DEBUG
		const char *tmp;
		gnutls_credentials_type_t cred;
		gnutls_kx_algorithm_t kx;
		int dhe, ecdh;

		dhe = ecdh = 0;

		LOG_A("TLS Session information for connection '%s':", conn->cc_id);
		
		/* print the key exchange's algorithm name
		*/
		GNUTLS_TRACE( kx = gnutls_kx_get (session) );
		GNUTLS_TRACE( tmp = gnutls_kx_get_name (kx) );
		LOG_D("\t- Key Exchange: %s", tmp);

		/* Check the authentication type used and switch
		* to the appropriate.
		*/
		GNUTLS_TRACE( cred = gnutls_auth_get_type (session) );
		switch (cred)
		{
			case GNUTLS_CRD_IA:
				LOG_D("\t - TLS/IA session");
				break;


			#ifdef ENABLE_SRP
			case GNUTLS_CRD_SRP:
				LOG_D("\t - SRP session with username %s",
					gnutls_srp_server_get_username (session));
				break;
			#endif

			case GNUTLS_CRD_PSK:
				/* This returns NULL in server side.
				*/
				if (gnutls_psk_client_get_hint (session) != NULL)
					LOG_D("\t - PSK authentication. PSK hint '%s'",
						gnutls_psk_client_get_hint (session));
				/* This returns NULL in client side.
				*/
				if (gnutls_psk_server_get_username (session) != NULL)
					LOG_D("\t - PSK authentication. Connected as '%s'",
						gnutls_psk_server_get_username (session));

				if (kx == GNUTLS_KX_ECDHE_PSK)
					ecdh = 1;
				else if (kx == GNUTLS_KX_DHE_PSK)
					dhe = 1;
				break;

			case GNUTLS_CRD_ANON:      /* anonymous authentication */
				LOG_D("\t - Anonymous DH using prime of %d bits",
					gnutls_dh_get_prime_bits (session));
				if (kx == GNUTLS_KX_ANON_ECDH)
					ecdh = 1;
				else if (kx == GNUTLS_KX_ANON_DH)
					dhe = 1;
				break;

			case GNUTLS_CRD_CERTIFICATE:       /* certificate authentication */

				/* Check if we have been using ephemeral Diffie-Hellman.
				*/
				if (kx == GNUTLS_KX_DHE_RSA || kx == GNUTLS_KX_DHE_DSS)
					dhe = 1;
				else if (kx == GNUTLS_KX_ECDHE_RSA || kx == GNUTLS_KX_ECDHE_ECDSA)
					ecdh = 1;
				
				/* Now print some info on the remote certificate */
				if (gnutls_certificate_type_get (session) == GNUTLS_CRT_X509) {
					gnutls_datum_t cinfo;

					cert_list = gnutls_certificate_get_peers (session, &cert_list_size);

					LOG_D("\t Peer provided %d certificates.", cert_list_size);

					if (cert_list_size > 0)
					{
						int ret;

						/* we only print information about the first certificate.
						*/
						gnutls_x509_crt_init (&cert);

						gnutls_x509_crt_import (cert, &cert_list[0], GNUTLS_X509_FMT_DER);

												LOG_A("\t Certificate info:");

						/* This is the preferred way of printing short information about
						 a certificate. */

						ret = gnutls_x509_crt_print (cert, GNUTLS_CRT_PRINT_ONELINE, &cinfo);
						if (ret == 0)
						{
						  LOG_D("\t\t%s", cinfo.data);
						  gnutls_free (cinfo.data);
						}
						
						if (conn->cc_tls_para.cn) {
							if (!gnutls_x509_crt_check_hostname (cert, conn->cc_tls_para.cn)) {
								LOG_E("\tTLS: Remote certificate invalid on socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
								LOG_E("\t - The certificate hostname does not match '%s'", conn->cc_tls_para.cn);
								gnutls_x509_crt_deinit (cert);
								return GNUTLS_E_CERTIFICATE_ERROR;
							}
							
						}

						hostname_verified = 1;

						gnutls_x509_crt_deinit (cert);

					}
    				}
				break;

			default:
				LOG_E("\t - unknown session type (%d)", cred);

		}                           /* switch */

		if (ecdh != 0)
			LOG_D("\t - Ephemeral ECDH using curve %s",
				gnutls_ecc_curve_get_name (gnutls_ecc_curve_get (session)));
		else if (dhe != 0)
			LOG_D("\t - Ephemeral DH using prime of %d bits",
				gnutls_dh_get_prime_bits (session));

		/* print the protocol's name (ie TLS 1.0) 
		*/
		tmp = gnutls_protocol_get_name (gnutls_protocol_get_version (session));
		LOG_D("\t - Protocol: %s", tmp);

		/* print the certificate type of the peer.
		* ie X.509
		*/
		tmp = gnutls_certificate_type_get_name (gnutls_certificate_type_get (session));
		LOG_D("\t - Certificate Type: %s", tmp);

		/* print the compression algorithm (if any)
		*/
		tmp = gnutls_compression_get_name (gnutls_compression_get (session));
		LOG_D("\t - Compression: %s", tmp);

		/* print the name of the cipher used.
		* ie 3DES.
		*/
		tmp = gnutls_cipher_get_name (gnutls_cipher_get (session));
		LOG_D("\t - Cipher: %s", tmp);

		/* Print the MAC algorithms name.
		* ie SHA1
		*/
		tmp = gnutls_mac_get_name (gnutls_mac_get (session));
		LOG_D("\t - MAC: %s", tmp);
	
#endif /* DEBUG */		

	/* This verification function uses the trusted CAs in the credentials
	* structure. So you must have installed one or more CA certificates.
	*/
	CHECK_GNUTLS_DO( gnutls_certificate_verify_peers2 (session, &status), return GNUTLS_E_CERTIFICATE_ERROR );
	if (status & GNUTLS_CERT_INVALID) {
		LOG_E("TLS: Remote certificate invalid on socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
		if (status & GNUTLS_CERT_SIGNER_NOT_FOUND)
			LOG_E(" - The certificate hasn't got a known issuer.");

		if (status & GNUTLS_CERT_REVOKED)
			LOG_E(" - The certificate has been revoked.");

		if (status & GNUTLS_CERT_EXPIRED)
			LOG_E(" - The certificate has expired.");

		if (status & GNUTLS_CERT_NOT_ACTIVATED)
			LOG_E(" - The certificate is not yet activated.");
	}	
	if (status & GNUTLS_CERT_INVALID)
	{
		return GNUTLS_E_CERTIFICATE_ERROR;
	}
	
	/* Up to here the process is the same for X.509 certificates and
	* OpenPGP keys. From now on X.509 certificates are assumed. This can
	* be easily extended to work with openpgp keys as well.
	*/
	if ((!hostname_verified) && (conn->cc_tls_para.cn)) {
		if (gnutls_certificate_type_get (session) != GNUTLS_CRT_X509) {
			LOG_E("TLS: Remote credentials are not x509, rejected on socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
			return GNUTLS_E_CERTIFICATE_ERROR;
		}

		CHECK_GNUTLS_DO( gnutls_x509_crt_init (&cert), return GNUTLS_E_CERTIFICATE_ERROR );

		cert_list = gnutls_certificate_get_peers (session, &cert_list_size);
		CHECK_PARAMS_DO( cert_list, return GNUTLS_E_CERTIFICATE_ERROR );

		CHECK_GNUTLS_DO( gnutls_x509_crt_import (cert, &cert_list[0], GNUTLS_X509_FMT_DER), return GNUTLS_E_CERTIFICATE_ERROR );

		if (!gnutls_x509_crt_check_hostname (cert, conn->cc_tls_para.cn)) {
			LOG_E("TLS: Remote certificate invalid on socket %d (Remote: '%s')(Connection: '%s') :", conn->cc_socket, conn->cc_remid, conn->cc_id);
			LOG_E(" - The certificate hostname does not match '%s'", conn->cc_tls_para.cn);
			gnutls_x509_crt_deinit (cert);
			return GNUTLS_E_CERTIFICATE_ERROR;
		}

		gnutls_x509_crt_deinit (cert);
	}

	/* notify gnutls to continue handshake normally */
	return 0;
}

#endif /* GNUTLS_VERSION_300 */

static int fd_cnx_may_dtls(struct cnxctx * conn) {
#ifndef DISABLE_SCTP
	if ((conn->cc_proto == IPPROTO_SCTP) && (conn->cc_tls_para.algo == ALGO_HANDSHAKE_DEFAULT))
		return 1;
#endif /* DISABLE_SCTP */
	return 0;
}

#ifndef DISABLE_SCTP
static int fd_cnx_uses_dtls(struct cnxctx * conn) {
	return fd_cnx_may_dtls(conn) && (fd_cnx_teststate(conn, CC_STATUS_TLS));
}
#endif /* DISABLE_SCTP */

/* TLS handshake a connection; no need to have called start_clear before. Reception is active if handhsake is successful */
int fd_cnx_handshake(struct cnxctx * conn, int mode, int algo, char * priority, void * alt_creds)
{
	int dtls = 0;
	
	TRACE_ENTRY( "%p %d %d %p %p", conn, mode, algo, priority, alt_creds);
	CHECK_PARAMS( conn && (!fd_cnx_teststate(conn, CC_STATUS_TLS)) && ( (mode == GNUTLS_CLIENT) || (mode == GNUTLS_SERVER) ) && (!conn->cc_loop) );

	/* Save the mode */
	conn->cc_tls_para.mode = mode;
	conn->cc_tls_para.algo = algo;
	
	/* Cancel receiving thread if any -- it should already be terminated anyway, we just release the resources */
	CHECK_FCT_DO( fd_thr_term(&conn->cc_rcvthr), /* continue */);
	
	/* Once TLS handshake is done, we don't stop after the first message */
	conn->cc_loop = 1;
	
	dtls = fd_cnx_may_dtls(conn);
	
	/* Prepare the master session credentials and priority */
	CHECK_FCT( fd_tls_prepare(&conn->cc_tls_para.session, mode, dtls, priority, alt_creds) );

	/* Special case: multi-stream TLS is not natively managed in GNU TLS, we use a wrapper library */
	if ((!dtls) && (conn->cc_sctp_para.pairs > 1)) {
#ifdef DISABLE_SCTP
		ASSERT(0);
		CHECK_FCT( ENOTSUP );
#else /* DISABLE_SCTP */
		/* Initialize the wrapper, start the demux thread */
		CHECK_FCT( fd_sctp3436_init(conn) );
#endif /* DISABLE_SCTP */
	} else {
		/* Set the transport pointer passed to push & pull callbacks */
		GNUTLS_TRACE( gnutls_transport_set_ptr( conn->cc_tls_para.session, (gnutls_transport_ptr_t) conn ) );

		/* Set the push and pull callbacks */
		if (!dtls) {
			#ifdef GNUTLS_VERSION_300
			GNUTLS_TRACE( gnutls_transport_set_pull_timeout_function( conn->cc_tls_para.session, (void *)fd_cnx_s_select ) );
			#endif /* GNUTLS_VERSION_300 */
			GNUTLS_TRACE( gnutls_transport_set_pull_function(conn->cc_tls_para.session, (void *)fd_cnx_s_recv) );
			#ifndef GNUTLS_VERSION_212
			GNUTLS_TRACE( gnutls_transport_set_push_function(conn->cc_tls_para.session, (void *)fd_cnx_s_send) );
			#else /* GNUTLS_VERSION_212 */
			GNUTLS_TRACE( gnutls_transport_set_vec_push_function(conn->cc_tls_para.session, (void *)fd_cnx_s_sendv) );
			#endif /* GNUTLS_VERSION_212 */
		} else {
			TODO("DTLS push/pull functions");
			return ENOTSUP;
		}
	}
	
	/* additional initialization for gnutls 3.x */
	#ifdef GNUTLS_VERSION_300
		/* the verify function has already been set in the global initialization in config.c */
	
	/* fd_tls_verify_credentials_2 uses the connection */
	gnutls_session_set_ptr (conn->cc_tls_para.session, (void *) conn);
	
	if ((conn->cc_tls_para.cn != NULL) && (mode == GNUTLS_CLIENT)) {
		/* this might allow virtual hosting on the remote peer */
		CHECK_GNUTLS_DO( gnutls_server_name_set (conn->cc_tls_para.session, GNUTLS_NAME_DNS, conn->cc_tls_para.cn, strlen(conn->cc_tls_para.cn)), /* ignore failure */);
	}
	
	#endif /* GNUTLS_VERSION_300 */

	#ifdef GNUTLS_VERSION_310
	GNUTLS_TRACE( gnutls_handshake_set_timeout( conn->cc_tls_para.session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT));
	#endif /* GNUTLS_VERSION_310 */
	
	/* Mark the connection as protected from here, so that the gnutls credentials will be freed */
	fd_cnx_addstate(conn, CC_STATUS_TLS);
	
	/* Handshake master session */
	{
		int ret;
	
		CHECK_GNUTLS_DO( ret = gnutls_handshake(conn->cc_tls_para.session),
			{
				if (TRACE_BOOL(INFO)) {
					fd_log_debug("TLS Handshake failed on socket %d (%s) : %s", conn->cc_socket, conn->cc_id, gnutls_strerror(ret));
				}
				fd_cnx_markerror(conn);
				return EINVAL;
			} );

		#ifndef GNUTLS_VERSION_300
		/* Now verify the remote credentials are valid -- only simple tests here */
		CHECK_FCT_DO( fd_tls_verify_credentials(conn->cc_tls_para.session, conn, 1), 
			{  
				CHECK_GNUTLS_DO( gnutls_bye(conn->cc_tls_para.session, GNUTLS_SHUT_RDWR),  );
				fd_cnx_markerror(conn);
				return EINVAL;
			});
		#endif /* GNUTLS_VERSION_300 */
	}
	
	/* Multi-stream TLS: handshake other streams as well */
	if ((!dtls) && (conn->cc_sctp_para.pairs > 1)) {
#ifndef DISABLE_SCTP
		/* Start reading the messages from the master session. That way, if the remote peer closed, we are not stuck inside handshake */
		CHECK_FCT(fd_sctp3436_startthreads(conn, 0));

		/* Resume all additional sessions from the master one. */
		CHECK_FCT(fd_sctp3436_handshake_others(conn, priority, alt_creds));

		/* Start decrypting the messages from all threads and queuing them in target queue */
		CHECK_FCT(fd_sctp3436_startthreads(conn, 1));
#endif /* DISABLE_SCTP */
	} else {
		/* Start decrypting the data */
		if (!dtls) {
			CHECK_POSIX( pthread_create( &conn->cc_rcvthr, NULL, rcvthr_tls_single, conn ) );
		} else {
			TODO("Signal the dtls_push function that multiple streams can be used from this point.");
			TODO("Create DTLS rcvthr (must reassembly based on seq numbers & stream id ?)");
			return ENOTSUP;
		}
	}
	
	return 0;
}

/* Retrieve TLS credentials of the remote peer, after handshake */
int fd_cnx_getcred(struct cnxctx * conn, const gnutls_datum_t **cert_list, unsigned int *cert_list_size)
{
	TRACE_ENTRY("%p %p %p", conn, cert_list, cert_list_size);
	CHECK_PARAMS( conn && fd_cnx_teststate(conn, CC_STATUS_TLS) && cert_list && cert_list_size );
	
	/* This function only works for X.509 certificates. */
	CHECK_PARAMS( gnutls_certificate_type_get (conn->cc_tls_para.session) == GNUTLS_CRT_X509 );
	
	GNUTLS_TRACE( *cert_list = gnutls_certificate_get_peers (conn->cc_tls_para.session, cert_list_size) );
	if (*cert_list == NULL) {
		TRACE_DEBUG(INFO, "No certificate was provided by remote peer / an error occurred.");
		return EINVAL;
	}

	TRACE_DEBUG( FULL, "Saved certificate chain (%d certificates) in peer structure.", *cert_list_size);
	
	return 0;
}

/* Receive next message. if timeout is not NULL, wait only until timeout. This function only pulls from a queue, mgr thread is filling that queue aynchrounously. */
/* if the altfifo has been set on this conn object, this function must not be called */
int fd_cnx_receive(struct cnxctx * conn, struct timespec * timeout, unsigned char **buf, size_t * len)
{
	int    ev;
	size_t ev_sz;
	void * ev_data;
	
	TRACE_ENTRY("%p %p %p %p", conn, timeout, buf, len);
	CHECK_PARAMS(conn && (conn->cc_socket > 0) && buf && len);
	CHECK_PARAMS(conn->cc_rcvthr != (pthread_t)NULL);
	CHECK_PARAMS(conn->cc_alt == NULL);

	/* Now, pull the first event */
get_next:
	if (timeout) {
		CHECK_FCT( fd_event_timedget(conn->cc_incoming, timeout, FDEVP_PSM_TIMEOUT, &ev, &ev_sz, &ev_data) );
	} else {
		CHECK_FCT( fd_event_get(conn->cc_incoming, &ev, &ev_sz, &ev_data) );
	}
	
	switch (ev) {
		case FDEVP_CNX_MSG_RECV:
			/* We got one */
			*len = ev_sz;
			*buf = ev_data;
			return 0;
			
		case FDEVP_PSM_TIMEOUT:
			TRACE_DEBUG(FULL, "Timeout event received");
			return ETIMEDOUT;
			
		case FDEVP_CNX_EP_CHANGE:
			/* We ignore this event */
			goto get_next;
			
		case FDEVP_CNX_ERROR:
			TRACE_DEBUG(FULL, "Received ERROR event on the connection");
			return ENOTCONN;
	}
	
	TRACE_DEBUG(INFO, "Received unexpected event %d (%s)", ev, fd_pev_str(ev));
	return EINVAL;
}

/* Where the events are sent */
struct fifo * fd_cnx_target_queue(struct cnxctx * conn)
{
	struct fifo *q;
	CHECK_POSIX_DO( pthread_mutex_lock(&state_lock), { ASSERT(0); } );
	q = conn->cc_alt ?: conn->cc_incoming;
	CHECK_POSIX_DO( pthread_mutex_unlock(&state_lock), { ASSERT(0); } );
	return q;
}

/* Set an alternate FIFO list to send FDEVP_CNX_* events to */
int fd_cnx_recv_setaltfifo(struct cnxctx * conn, struct fifo * alt_fifo)
{
	int ret;
	TRACE_ENTRY( "%p %p", conn, alt_fifo );
	CHECK_PARAMS( conn && alt_fifo && conn->cc_incoming );
	
	/* The magic function does it all */
	CHECK_POSIX_DO( pthread_mutex_lock(&state_lock), { ASSERT(0); } );
	CHECK_FCT_DO( ret = fd_fifo_move( conn->cc_incoming, alt_fifo, &conn->cc_alt ), );
	CHECK_POSIX_DO( pthread_mutex_unlock(&state_lock), { ASSERT(0); } );
	
	return ret;
}

/* Send function when no multi-stream is involved, or sending on stream #0 (send() always use stream 0)*/
static int send_simple(struct cnxctx * conn, unsigned char * buf, size_t len)
{
	ssize_t ret;
	size_t sent = 0;
	TRACE_ENTRY("%p %p %zd", conn, buf, len);
	do {
		if (fd_cnx_teststate(conn, CC_STATUS_TLS)) {
			CHECK_GNUTLS_DO( ret = fd_tls_send_handle_error(conn, conn->cc_tls_para.session, buf + sent, len - sent),  );
		} else {
			struct iovec iov;
			iov.iov_base = buf + sent;
			iov.iov_len  = len - sent;
			CHECK_SYS_DO( ret = fd_cnx_s_sendv(conn, &iov, 1), );
		}
		if (ret <= 0)
			return ENOTCONN;
		
		sent += ret;
	} while ( sent < len );
	return 0;
}

/* Send a message -- this is synchronous -- and we assume it's never called by several threads at the same time (on the same conn), so we don't protect. */
int fd_cnx_send(struct cnxctx * conn, unsigned char * buf, size_t len)
{
	TRACE_ENTRY("%p %p %zd", conn, buf, len);
	
	CHECK_PARAMS(conn && (conn->cc_socket > 0) && (! fd_cnx_teststate(conn, CC_STATUS_ERROR)) && buf && len);

	TRACE_DEBUG(FULL, "Sending %zdb %sdata on connection %s", len, fd_cnx_teststate(conn, CC_STATUS_TLS) ? "TLS-protected ":"", conn->cc_id);
	
	switch (conn->cc_proto) {
		case IPPROTO_TCP:
			CHECK_FCT( send_simple(conn, buf, len) );
			break;
		
#ifndef DISABLE_SCTP
		case IPPROTO_SCTP: {
			int dtls = fd_cnx_uses_dtls(conn);
			if (!dtls) {
				int stream = 0;
				if (conn->cc_sctp_para.unordered) {
					int limit;
					if (fd_cnx_teststate(conn, CC_STATUS_TLS))
						limit = conn->cc_sctp_para.pairs;
					else
						limit = conn->cc_sctp_para.str_out;
					
					if (limit > 1) {
						conn->cc_sctp_para.next += 1;
						conn->cc_sctp_para.next %= limit;
						stream = conn->cc_sctp_para.next;
					}
				}
				
				if (stream == 0) {
					/* We can use default function, it sends over stream #0 */
					CHECK_FCT( send_simple(conn, buf, len) );
				} else {
					if (!fd_cnx_teststate(conn, CC_STATUS_TLS)) {
						struct iovec iov;
						iov.iov_base = buf;
						iov.iov_len  = len;
						
						CHECK_SYS_DO( fd_sctp_sendstrv(conn, stream, &iov, 1), { fd_cnx_markerror(conn); return ENOTCONN; } );
					} else {
						/* push the data to the appropriate session */
						ssize_t ret;
						size_t sent = 0;
						ASSERT(conn->cc_sctp3436_data.array != NULL);
						do {
							CHECK_GNUTLS_DO( ret = fd_tls_send_handle_error(conn, conn->cc_sctp3436_data.array[stream].session, buf + sent, len - sent), );
							if (ret <= 0)
								return ENOTCONN;

							sent += ret;
						} while ( sent < len );
					}
				}
			} else {
				/* DTLS */
				/* Multistream is handled at lower layer in the push/pull function */
				CHECK_FCT( send_simple(conn, buf, len) );
			}
		}
		break;
#endif /* DISABLE_SCTP */
	
		default:
			TRACE_DEBUG(INFO, "Unknown protocol: %d", conn->cc_proto);
			ASSERT(0);
			return ENOTSUP;	/* or EINVAL... */
	}
	
	return 0;
}


/**************************************/
/*     Destruction of connection      */
/**************************************/

/* Destroy a conn structure, and shutdown the socket */
void fd_cnx_destroy(struct cnxctx * conn)
{
	TRACE_ENTRY("%p", conn);
	
	CHECK_PARAMS_DO(conn, return);
	
	fd_cnx_addstate(conn, CC_STATUS_CLOSING);
	
	/* Initiate shutdown of the TLS session(s): call gnutls_bye(WR), then read until error */
	if (fd_cnx_teststate(conn, CC_STATUS_TLS)) {
#ifndef DISABLE_SCTP
		int dtls = fd_cnx_uses_dtls(conn);
		if ((!dtls) && (conn->cc_sctp_para.pairs > 1)) {
			if (! fd_cnx_teststate(conn, CC_STATUS_ERROR )) {
				/* Bye on master session */
				CHECK_GNUTLS_DO( gnutls_bye(conn->cc_tls_para.session, GNUTLS_SHUT_WR), fd_cnx_markerror(conn) );
			}

			if (! fd_cnx_teststate(conn, CC_STATUS_ERROR ) ) {
				/* and other stream pairs */
				fd_sctp3436_bye(conn);
			}

			if (! fd_cnx_teststate(conn, CC_STATUS_ERROR ) ) {
				/* Now wait for all decipher threads to terminate */
				fd_sctp3436_waitthreadsterm(conn);
			} else {
				/* Abord the threads, the connection is dead already */
				fd_sctp3436_stopthreads(conn);
			}

			/* Deinit gnutls resources */
			fd_sctp3436_gnutls_deinit_others(conn);
			if (conn->cc_tls_para.session) {
				GNUTLS_TRACE( gnutls_deinit(conn->cc_tls_para.session) );
				conn->cc_tls_para.session = NULL;
			}
			
			/* Destroy the wrapper (also stops the demux thread) */
			fd_sctp3436_destroy(conn);

		} else {
#endif /* DISABLE_SCTP */
		/* We are TLS, but not using the sctp3436 wrapper layer */
			if (! fd_cnx_teststate(conn, CC_STATUS_ERROR ) ) {
				/* Master session */
				CHECK_GNUTLS_DO( gnutls_bye(conn->cc_tls_para.session, GNUTLS_SHUT_WR), fd_cnx_markerror(conn) );
			}

			if (! fd_cnx_teststate(conn, CC_STATUS_ERROR ) ) {
				/* In this case, just wait for thread rcvthr_tls_single to terminate */
				if (conn->cc_rcvthr != (pthread_t)NULL) {
					CHECK_POSIX_DO(  pthread_join(conn->cc_rcvthr, NULL), /* continue */  );
					conn->cc_rcvthr = (pthread_t)NULL;
				}
			} else {
				/* Cancel the receiver thread in case it did not already terminate */
				CHECK_FCT_DO( fd_thr_term(&conn->cc_rcvthr), /* continue */ );
			}
			
			/* Free the resources of the TLS session */
			if (conn->cc_tls_para.session) {
				GNUTLS_TRACE( gnutls_deinit(conn->cc_tls_para.session) );
				conn->cc_tls_para.session = NULL;
			}
#ifndef DISABLE_SCTP
		}
#endif /* DISABLE_SCTP */
	}
	
	/* Terminate the thread in case it is not done yet -- is there any such case left ?*/
	CHECK_FCT_DO( fd_thr_term(&conn->cc_rcvthr), /* continue */ );
		
	/* Shut the connection down */
	if (conn->cc_socket > 0) {
		shutdown(conn->cc_socket, SHUT_RDWR);
		close(conn->cc_socket);
		conn->cc_socket = -1;
	}
	
	/* Empty and destroy FIFO list */
	if (conn->cc_incoming) {
		fd_event_destroy( &conn->cc_incoming, free );
	}
	
	/* Free the object */
	free(conn);
	
	/* Done! */
	return;
}
