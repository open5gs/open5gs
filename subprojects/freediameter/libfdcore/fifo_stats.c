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

/* See include/freeDiameter/libfdcore.h for more information */
int fd_stat_getstats(enum fd_stat_type stat, struct peer_hdr * peer, 
			int * current_count, int * limit_count, int * highest_count, long long * total_count, 
			struct timespec * total, struct timespec * blocking, struct timespec * last)
{
	struct fd_peer * p = (struct fd_peer *)peer;
	TRACE_ENTRY( "%d %p %p %p %p %p %p %p %p", stat, peer, current_count, limit_count, highest_count, total_count, total, blocking, last);
	
	switch (stat) {
		case STAT_G_LOCAL: {
			CHECK_FCT( fd_fifo_getstats(fd_g_local, current_count, limit_count, highest_count, total_count, total, blocking, last) );
		}
		break;

		case STAT_G_INCOMING: {
			CHECK_FCT( fd_fifo_getstats(fd_g_incoming, current_count, limit_count, highest_count, total_count, total, blocking, last) );
		}
		break;

		case STAT_G_OUTGOING: {
			CHECK_FCT( fd_fifo_getstats(fd_g_outgoing, current_count, limit_count, highest_count, total_count, total, blocking, last) );
		}
		break;

		case STAT_P_PSM: {
			CHECK_PARAMS( CHECK_PEER( peer ) );
			CHECK_FCT( fd_fifo_getstats(p->p_events, current_count, limit_count, highest_count, total_count, total, blocking, last) );
		}
		break;

		case STAT_P_TOSEND: {
			CHECK_PARAMS( CHECK_PEER( peer ) );
			CHECK_FCT( fd_fifo_getstats(p->p_tosend, current_count, limit_count, highest_count, total_count, total, blocking, last) );
		}
		break;

		default:
			return EINVAL;
	}
	
	return 0;
}
