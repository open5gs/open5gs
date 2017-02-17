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

/* This file contains code to handle Device Watchdog messages (DWR and DWA) */

/* Check the value of Origin-State-Id is consistent in a DWR or DWA -- we return an error otherwise */
static int check_state_id(struct msg * msg, struct fd_peer * peer)
{
	struct avp * osi;
	
	/* Check if the request contains the Origin-State-Id */
	CHECK_FCT( fd_msg_search_avp ( msg, fd_dict_avp_OSI, &osi ) );
	if (osi) {
		/* Check the value is consistent with the saved one */
		struct avp_hdr * hdr;
		CHECK_FCT(  fd_msg_avp_hdr( osi, &hdr )  );
		if (hdr->avp_value == NULL) {
			/* This is a sanity check */
			LOG_F("Ignored an Origin-State-Id AVP with unset value in DWR/DWA");
			ASSERT(0); /* To check if this really happens, and understand why... */
		}

		if (! peer->p_hdr.info.runtime.pir_orstate) {
			/* It was not already received in CER/CEA, save it now */
			peer->p_hdr.info.runtime.pir_orstate = hdr->avp_value->u32;
		}

		if (peer->p_hdr.info.runtime.pir_orstate != hdr->avp_value->u32) {
			TRACE_DEBUG(INFO, "Received a new Origin-State-Id from peer '%s'! (%x -> %x); resetting the connection.", 
				peer->p_hdr.info.pi_diamid, 
				peer->p_hdr.info.runtime.pir_orstate,
				hdr->avp_value->u32 );
			return EINVAL;
		}
	}
	return 0;
}

/* Create and send a DWR */
static int send_DWR(struct fd_peer * peer)
{
	struct msg * msg = NULL;
	
	/* Create a new DWR instance */
	CHECK_FCT( fd_msg_new ( fd_dict_cmd_DWR, MSGFL_ALLOC_ETEID, &msg ) );
	
	/* Add the content of the message (only the origin) */
	CHECK_FCT( fd_msg_add_origin ( msg, 1 ) );
	
	/* Now send this message */
	CHECK_FCT( fd_out_send(&msg, NULL, peer, 0) );
	
	/* And mark the pending DW */
	peer->p_flags.pf_dw_pending = 1;
	
	return 0;
}

/* Handle an incoming message */
int fd_p_dw_handle(struct msg ** msg, int req, struct fd_peer * peer)
{
	int reset_tmr = 0;
	
	TRACE_ENTRY("%p %d %p", msg, req, peer);
	
	/* Check the value of OSI for information */
	CHECK_FCT( check_state_id(*msg, peer) );
	
	if (req) {
		/* If we receive a DWR, send back a DWA */
		CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
		CHECK_FCT( fd_msg_rescode_set( *msg, "DIAMETER_SUCCESS", NULL, NULL, 0 ) );
		CHECK_FCT( fd_msg_add_origin ( *msg, 1 ) );
		CHECK_FCT( fd_out_send( msg, peer->p_cnxctx, peer, 0) );
		
	} else {
		/* Discard the DWA */
		CHECK_FCT_DO( fd_msg_free(*msg), /* continue */ );
		*msg = NULL;
		
		/* And clear the pending DW flag */
		peer->p_flags.pf_dw_pending = 0;
	}
	
	/* Now update timeout */
	if (req) {
		/* Update timeout only if we did not already send a DWR ourselves */
		reset_tmr = !peer->p_flags.pf_dw_pending;
	} else {
		/* Reset the timer */
		reset_tmr = 1;
	}
	if (reset_tmr) {
		fd_psm_next_timeout(peer, 1, peer->p_hdr.info.config.pic_twtimer ?: fd_g_config->cnf_timer_tw);
	}
	
	/* If we are in REOPEN state, increment the counter */
	if (fd_peer_getstate(peer) == STATE_REOPEN) {
		peer->p_flags.pf_reopen_cnt += 1;
		
		if (peer->p_flags.pf_reopen_cnt) {
			/* Send a new DWR */
			CHECK_FCT( send_DWR(peer) );
		} else {
			/* Move to OPEN state */
			CHECK_FCT( fd_psm_change_state(peer, STATE_OPEN) );
		}
	}
		
	return 0;
}

/* Handle a timeout in the PSM (OPEN or REOPEN state only) */
int fd_p_dw_timeout(struct fd_peer * peer)
{
	TRACE_ENTRY("%p", peer);

	if (peer->p_flags.pf_dw_pending) {
		/* We have sent a DWR and received no answer during TwTimer */
		CHECK_FCT( fd_psm_change_state(peer, STATE_SUSPECT) );
		fd_psm_next_timeout(peer, 0, 2 * (peer->p_hdr.info.config.pic_twtimer ?: fd_g_config->cnf_timer_tw) );
	} else {
		/* The timeout has expired, send a DWR */
		CHECK_FCT( send_DWR(peer) );
		fd_psm_next_timeout(peer, 0, peer->p_hdr.info.config.pic_twtimer ?: fd_g_config->cnf_timer_tw );
	}
	
	return 0;
}

/* Handle DW exchanges after the peer has come alive again */
int fd_p_dw_reopen(struct fd_peer * peer)
{
	TRACE_ENTRY("%p", peer);

	peer->p_flags.pf_reopen_cnt = 1;
	peer->p_flags.pf_cnx_pb = 0;
	CHECK_FCT( send_DWR(peer) );
	
	return 0;
}


