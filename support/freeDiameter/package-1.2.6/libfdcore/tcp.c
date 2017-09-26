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
#include "cnxctx.h"

#include <netinet/tcp.h>
#include <netinet/ip6.h>
#include <sys/socket.h>

/* Set the socket options for TCP sockets, before bind is called */
static int fd_tcp_setsockopt(int family, int sk)
{
	int ret = 0;
	int opt;
	
	/* Clear the NODELAY option in case it was set, as requested by rfc3539#section-3.2 */
	/* Note that this is supposed to be the default, so we could probably remove this call ... */
	opt = 0;
	ret = setsockopt(sk, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	if (ret != 0) {
		ret = errno;
		TRACE_DEBUG(INFO, "Unable to set the socket TCP_NODELAY option: %s", strerror(ret));
		return ret;
	}
	
	/* Under Linux, we may also set the TCP_CONGESTION option to one of the following strings:
	    - reno (default)
	    - bic
	    - cubic
	    - highspeed
	    - htcp
	    - hybla
	    - illinois
	    - lp
	    - scalable
	    - vegas
	    - veno
	    - westwood
	    - yeah
	*/
	
	/* In case of v6 address, force the v6only option, we use a different socket for v4 */
	#ifdef IPV6_V6ONLY
	if (family == AF_INET6) {
		opt = 1;
		CHECK_SYS(setsockopt(sk, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)));
	}
	#endif /* IPV6_V6ONLY */
	
	{
		opt = 1;
		CHECK_SYS(  setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))  );
	}
	
	/* There are also others sockopt that can be set, but nothing useful for us AFAICT */
	
	return 0;
}

/* Create a socket server and bind it */
int fd_tcp_create_bind_server( int * sock, sSA * sa, socklen_t salen )
{
	TRACE_ENTRY("%p %p %d", sock, sa, salen);
	
	CHECK_PARAMS(  sock && sa  );
	
	/* Create the socket */
	CHECK_SYS(  *sock = socket(sa->sa_family, SOCK_STREAM, IPPROTO_TCP)  );

	/* Set the socket options */
	CHECK_FCT(  fd_tcp_setsockopt(sa->sa_family, *sock)  );
	
	/* Bind the socket */
	CHECK_SYS(  bind( *sock, sa, salen )  );
			
	/* We're done */
	return 0;
}

/* Allow clients connections on server sockets */
int fd_tcp_listen( int sock )
{
	TRACE_ENTRY("%d", sock);
	CHECK_SYS( listen(sock, 5) );
	return 0;
}

/* Create a client socket and connect to remote server */
int fd_tcp_client( int *sock, sSA * sa, socklen_t salen )
{
	int ret = 0;
	int s;
	
	TRACE_ENTRY("%p %p %d", sock, sa, salen);
	CHECK_PARAMS( sock && (*sock <= 0) && sa && salen );
	
	/* Create the socket */
	CHECK_SYS(  s = socket(sa->sa_family, SOCK_STREAM, IPPROTO_TCP)  );
	
	/* Set the socket options */
	CHECK_FCT(  fd_tcp_setsockopt(sa->sa_family, s)  );
	
	/* Cleanup if we are cancelled */
	pthread_cleanup_push(fd_cleanup_socket, &s);
	
	/* Try connecting to the remote address */
	ret = connect(s, sa, salen);
	
	pthread_cleanup_pop(0);
	
	if (ret < 0) {
		ret = errno;
		LOG_A( "connect returned an error: %s", strerror(ret));
		CHECK_SYS_DO( close(s), /* continue */ );
		*sock = -1;
		return ret;
	}
	
	/* Done! */
	*sock = s;
	return ret;
}

/* Get the remote name of a TCP socket */
int fd_tcp_get_remote_ep(int sock, sSS * ss, socklen_t *sl)
{
	TRACE_ENTRY("%d %p %p", sock, ss, sl);
	CHECK_PARAMS( ss && sl );
	
	*sl = sizeof(sSS);
	CHECK_SYS(getpeername(sock, (sSA *)ss, sl));
	
	return 0;
}

