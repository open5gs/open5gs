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

/* Do not include this directly, use dbg_interactive.i instead */

/****** ENDPOINTS *********/

%{

#include <sys/socket.h>
#include <netdb.h>

%}

%extend fd_endpoint {
	fd_endpoint() {
		struct fd_endpoint *np = (struct fd_endpoint *)calloc(1, sizeof(struct fd_endpoint));
		if (!np) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		fd_list_init(&np->chain, np);
		return np;
	}
	
	fd_endpoint(const char * endpoint, uint16_t port = 0, uint32_t flags = EP_FL_CONF) {
		struct addrinfo hints;
		struct addrinfo *ai = NULL;
		int ret;
		
		memset(&hints, 0, sizeof(hints));
		hints.ai_family= AF_UNSPEC;
		hints.ai_flags = AI_NUMERICHOST;
		
		ret = getaddrinfo(endpoint, NULL, &hints, &ai);
		if (ret) {
			DI_ERROR(ret, PyExc_ValueError, gai_strerror(ret));
			return NULL;
		}
		
		if (port) {
			switch (ai->ai_family) {
				case AF_INET:
					((sSA4 *)ai->ai_addr)->sin_port = htons(port);
					break;
				case AF_INET6:
					((sSA6 *)ai->ai_addr)->sin6_port = htons(port);
					break;
				default:
					DI_ERROR(EINVAL, PyExc_RuntimeError, "Unknown family returned by getaddrinfo");
					return NULL;
			}
		}
		
		struct fd_endpoint *np = (struct fd_endpoint *)calloc(1, sizeof(struct fd_endpoint));
		if (!np) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		fd_list_init(&np->chain, np);
		
		memcpy(&np->s.sa, ai->ai_addr, ai->ai_addrlen);
		
		freeaddrinfo(ai);
		
		np->flags = flags;
		
		return np;
	}
	
	~fd_endpoint() {
		fd_list_unlink(&$self->chain);
		free($self);
	}
	
	/* Merge to a list */
	%delobject add_merge;
	void add_merge(struct fd_list * eplist) {
		int ret;
		
		if (!eplist) {
			DI_ERROR(EINVAL, NULL, NULL);
			return;
		}
		
		ret = fd_ep_add_merge( eplist, &$self->s.sa, sSAlen(&$self->s.sa), $self->flags );
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
			return;
		}
		
		return;
	}
	
	void dump() {
		char * buf = NULL;
		size_t len;
		printf("%s", fd_ep_dump_one(&buf, &len, NULL, 1, $self));
		free(buf);
	}
}
