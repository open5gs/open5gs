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


/* Add an endpoint information in a list */
int fd_ep_add_merge( struct fd_list * list, sSA * sa, socklen_t sl, uint32_t flags )
{
	struct fd_endpoint * ep;
	struct fd_list * li;
	union {
		sSA * sa;
		sSA4 *sin;
		sSA6 *sin6;
	} ptr;
	in_port_t * port;
	int cmp = -1;
	
	TRACE_ENTRY("%p %p %u %x", list, sa, sl, flags);
	CHECK_PARAMS( list && sa && (sl <= sizeof(sSS)) );
	
	if (list->next == NULL) {
		/* the list is not initialized yet, do it */
		fd_list_init(list, NULL);
	}
	
	ptr.sa = sa;
	
	/* Filter out a bunch of invalid addresses */
	switch (sa->sa_family) {
		case AF_INET:
			if (! (flags & EP_ACCEPTALL)) {
				if (IN_IS_ADDR_UNSPECIFIED(&ptr.sin->sin_addr) 
				 || IN_IS_ADDR_LOOPBACK(&ptr.sin->sin_addr)
				    /* the next one filters both EXPERIMENTAL, BADCLASS and MULTICAST. */
				 || ((ntohl(ptr.sin->sin_addr.s_addr) & 0xe0000000) == 0xe0000000)
				 || (ptr.sin->sin_addr.s_addr == INADDR_BROADCAST)) {
					LOG_A("  DEBUG:fd_ep_add_merge  Address was ignored, not added.");
					return 0;
				}
			}
			port = &ptr.sin->sin_port;
			break;

		case AF_INET6:
			if (! (flags & EP_ACCEPTALL)) {
				if (IN6_IS_ADDR_UNSPECIFIED(&ptr.sin6->sin6_addr) 
				 || IN6_IS_ADDR_LOOPBACK(&ptr.sin6->sin6_addr)
				 || IN6_IS_ADDR_MULTICAST(&ptr.sin6->sin6_addr)
				 || IN6_IS_ADDR_LINKLOCAL(&ptr.sin6->sin6_addr)
				 || IN6_IS_ADDR_SITELOCAL(&ptr.sin6->sin6_addr)) {
					LOG_A("  DEBUG:fd_ep_add_merge  Address was ignored, not added.");
					return 0;
				}
			}
			port = &ptr.sin6->sin6_port;
			break;

		default:
			LOG_A("  DEBUG:fd_ep_add_merge  Address family was unknown, not added.");
			return 0;
	}
	
	/* remove the ACCEPTALL flag */
	flags &= ~EP_ACCEPTALL;
	
	/* Search place in the list */
	for (li = list->next; li != list; li = li->next) {
		ep = (struct fd_endpoint *)li;
		in_port_t * ep_port;
		
		/* First, compare the address family */
		if (ep->sa.sa_family < sa->sa_family)
			continue;
		if (ep->sa.sa_family > sa->sa_family)
			break;
		
		/* Then compare the address field */
		switch (sa->sa_family) {
			case AF_INET:
				cmp = memcmp(&ep->sin.sin_addr, &ptr.sin->sin_addr, sizeof(struct in_addr));
				ep_port = &ep->sin.sin_port;
				break;
			case AF_INET6:
				cmp = memcmp(&ep->sin6.sin6_addr, &ptr.sin6->sin6_addr, sizeof(struct in6_addr));
				ep_port = &ep->sin6.sin6_port;
				break;
			default:
				ASSERT( 0 ); /* we got a different value previously in this same function */
		}
		if (cmp < 0)
			continue;
		if (cmp > 0)
			break;
		
		/* Finally compare the port, only if not 0 */
		if (*port == 0)
			break;
		if (*ep_port == 0) {
			/* save the port information in the list, and break */
			*ep_port = *port;
			break;
		}
		if (*ep_port < *port) {
			cmp = -1;
			continue;
		}
		if (*ep_port > *port)
			cmp = 1;
		break;
	}
	
	if (cmp) {
		/* new item to be added */
		CHECK_MALLOC( ep = malloc(sizeof(struct fd_endpoint)) );
		memset(ep, 0, sizeof(struct fd_endpoint));
		fd_list_init(&ep->chain, NULL);
		memcpy(&ep->ss, sa, sl);
		
		/* Insert in the list */
		fd_list_insert_before(li, &ep->chain);
	}
	
	/* Merge the flags */
	ep->flags |= flags;
	
	return 0;
}

/* Delete endpoints that do not have a matching flag from a list (0: delete all endpoints) */
int fd_ep_filter( struct fd_list * list, uint32_t flags )
{
	struct fd_list * li;
	
	TRACE_ENTRY("%p %x", list, flags);
	CHECK_PARAMS(list);
	
	for (li = list->next; li != list; li = li->next) {
		struct fd_endpoint * ep = (struct fd_endpoint *)li;
		
		if (! (ep->flags & flags)) {
			li = li->prev;
			fd_list_unlink(&ep->chain);
			free(ep);
		}
	}
	
	return 0;
}

/* Keep only endpoints of the same family as af */
int fd_ep_filter_family( struct fd_list * list, int af )
{
	struct fd_list * li;
	
	TRACE_ENTRY("%p %d", list, af);
	CHECK_PARAMS(list);
	
	for (li = list->next; li != list; li = li->next) {
		struct fd_endpoint * ep = (struct fd_endpoint *)li;
		
		if (ep->sa.sa_family != af) {
			li = li->prev;
			fd_list_unlink(&ep->chain);
			free(ep);
		}
	}
	
	return 0;
}

/* Reset the given flag(s) from all items in the list */
int fd_ep_clearflags( struct fd_list * list, uint32_t flags )
{
	struct fd_list * li;
	
	TRACE_ENTRY("%p %x", list, flags);
	CHECK_PARAMS(list);
	
	for (li = list->next; li != list; li = li->next) {
		struct fd_endpoint * ep = (struct fd_endpoint *)li;
		ep->flags &= ~flags;
		if (ep->flags == 0) {
			li = li->prev;
			fd_list_unlink(&ep->chain);
			free(ep);
		}
	}
	
	return 0;
}

DECLARE_FD_DUMP_PROTOTYPE(fd_ep_dump_one, int preamble, struct fd_endpoint * ep )
{
	FD_DUMP_HANDLE_OFFSET();
	
	if (preamble) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "{ep}(@%p): ", ep), return NULL);
	}
	
	if (!ep) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "INVALID/NULL"), return NULL);
		return *buf;
	}
	
	CHECK_MALLOC_DO( fd_sa_dump( FD_DUMP_STD_PARAMS, &ep->sa, NI_NUMERICHOST | NI_NUMERICSERV ), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "{%s%s%s%s%s}",
				(ep->flags & EP_FL_CONF) 	? "C" : "-",
				(ep->flags & EP_FL_DISC) 	? "D" : "-",
				(ep->flags & EP_FL_ADV) 	? "A" : "-",
				(ep->flags & EP_FL_LL) 		? "L" : "-",
				(ep->flags & EP_FL_PRIMARY) 	? "P" : "-"), return NULL);
	return *buf;
}

DECLARE_FD_DUMP_PROTOTYPE(fd_ep_dump, int preamble, int indent, struct fd_list * eps  )
{
	struct fd_list * li;
	
	FD_DUMP_HANDLE_OFFSET();
	
	if (preamble) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "%*s{eps}(@%p):", indent, "", eps), return NULL);
	}
	if (eps) {
		for (li = eps->next; li != eps; li = li->next) {
			struct fd_endpoint * ep = (struct fd_endpoint *)li;
			if (preamble) {
				CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "\n%*s", indent+1, ""), return NULL);
			} else if (li->prev != eps) {
				CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "\t"), return NULL);
			}
			CHECK_MALLOC_DO( fd_ep_dump_one( FD_DUMP_STD_PARAMS, preamble, ep ), return NULL);
		}
	}
	return *buf;
}
	
