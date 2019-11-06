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

/* This file contains code to handle Disconnect Peer messages (DPR and DPA) */

/* Delay to use before next reconnect attempt */
int fd_p_dp_newdelay(struct fd_peer * peer) 
{
	int delay = peer->p_hdr.info.config.pic_tctimer ?: fd_g_config->cnf_timer_tc;
	
	switch (peer->p_hdr.info.runtime.pir_lastDC) {
		case ACV_DC_REBOOTING:
		default:
			/* We use TcTimer to attempt reconnection */
			break;
		case ACV_DC_BUSY:
			/* No need to hammer the overloaded peer */
			delay *= 10;
			break;
		case ACV_DC_NOT_FRIEND:
			/* He does not want to speak to us... let's retry a *lot* later maybe */
			delay *= 200;
			break;
	}
	return delay;
}

/* Handle a received message */
int fd_p_dp_handle(struct msg ** msg, int req, struct fd_peer * peer)
{
	long to_receive, to_send;
	TRACE_ENTRY("%p %d %p", msg, req, peer);
	
	if (req) {
		/* We received a DPR, save the Disconnect-Cause and go to CLOSING_GRACE or terminate the connection */
		struct avp * dc;
		
		CHECK_FCT( fd_msg_search_avp ( *msg, fd_dict_avp_DC, &dc ));
		if (dc) {
			struct avp_hdr * hdr;
			CHECK_FCT(  fd_msg_avp_hdr( dc, &hdr )  );
			if (hdr->avp_value == NULL) {
				/* This is a sanity check */
				LOG_F("BUG: Unset value in Disconnect-Cause in DPR");
				ASSERT(0); /* To check if this really happens, and understand why... */
			}

			/* save the cause */
			peer->p_hdr.info.runtime.pir_lastDC = hdr->avp_value->u32;
		}
		if (TRACE_BOOL(INFO)) {
			if (dc) {
				struct dict_object * dictobj;
				struct dict_enumval_request er;
				memset(&er, 0, sizeof(er));
				
				/* prepare the request */
				CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_TYPE, TYPE_OF_AVP, fd_dict_avp_DC, &er.type_obj, ENOENT )  );
				er.search.enum_value.u32 = peer->p_hdr.info.runtime.pir_lastDC;
				
				/* Search the enum value */
				CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &er, &dictobj, 0 )  );
				if (dictobj) {
					CHECK_FCT( fd_dict_getval( dictobj, &er.search ) );
					TRACE_DEBUG(INFO, "Peer '%s' sent a DPR with cause: %s", peer->p_hdr.info.pi_diamid, er.search.enum_name);
				} else {
					TRACE_DEBUG(INFO, "Peer '%s' sent a DPR with unknown cause: %u", peer->p_hdr.info.pi_diamid, peer->p_hdr.info.runtime.pir_lastDC);
				}
			} else {
				TRACE_DEBUG(INFO, "Peer '%s' sent a DPR without Disconnect-Cause AVP", peer->p_hdr.info.pi_diamid);
			}
		}
		
		/* Now reply with a DPA */
		CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
		CHECK_FCT( fd_msg_rescode_set( *msg, "DIAMETER_SUCCESS", NULL, NULL, 1 ) );
		
		/* Do we have pending exchanges with this peer? */
		CHECK_FCT( fd_peer_get_load_pending(&peer->p_hdr, &to_receive, &to_send) );
		
		if ((to_receive == 0) && (to_send == 0)) {
			/* No pending exchange, move to CLOSING directly */
			CHECK_FCT( fd_psm_change_state(peer, STATE_CLOSING) );
		
			/* Now send the DPA */
			CHECK_FCT( fd_out_send( msg, NULL, peer, 0) );
			
			/* and move to CLOSED */
			fd_psm_cleanup(peer, 0);

			/* Reset the timer for next connection attempt -- we'll retry sooner or later depending on the disconnection cause */
			fd_psm_next_timeout(peer, 1, fd_p_dp_newdelay(peer));
		} else {
			/* We have pending exchanges, we move to CLOSING_GRACE which allows exchanges of answers but
			not new requests */
			CHECK_FCT( fd_psm_change_state(peer, STATE_CLOSING_GRACE) );
			fd_psm_next_timeout(peer, 0, GRACE_TIMEOUT);
			
			/* Now send the DPA */
			CHECK_FCT( fd_out_send( msg, NULL, peer, 0) );
		}
	} else {
		/* We received a DPA */
		int curstate = fd_peer_getstate(peer);
		if (curstate != STATE_CLOSING_GRACE) {
			TRACE_DEBUG(INFO, "Ignoring DPA received in state %s", STATE_STR(curstate));
		}
			
		/* In theory, we should control the Result-Code AVP. But since we will not go back to OPEN state here anyway, let's skip it */
		
		/* TODO("Control Result-Code in the DPA") */
		CHECK_FCT_DO( fd_msg_free( *msg ), /* continue */ );
		*msg = NULL;
		
		/* Do we still have pending exchanges with this peer? */
		CHECK_FCT( fd_peer_get_load_pending(&peer->p_hdr, &to_receive, &to_send) );
		if ((to_receive != 0) || (to_send != 0)) {
			TRACE_DEBUG(INFO, "Received DPA but pending load: [%ld, %ld], giving grace delay before closing", to_receive, to_send);
			fd_psm_next_timeout(peer, 0, GRACE_TIMEOUT);
			peer->p_flags.pf_localterm = 1;
		} else {
			/* otherwise, go to CLOSING state, the psm will handle terminating the connection */
			CHECK_FCT( fd_psm_change_state(peer, STATE_CLOSING) );
		}
	}
	
	return 0;
}

/* Start disconnection of a peer: send DPR */
int fd_p_dp_initiate(struct fd_peer * peer, char * reason)
{
	struct msg * msg = NULL;
	struct dict_object * dictobj = NULL;
	struct avp * avp = NULL;
	struct dict_enumval_request er;
	union avp_value val;
	
	TRACE_ENTRY("%p %p", peer, reason);
	
	/* Create a new DWR instance */
	CHECK_FCT( fd_msg_new ( fd_dict_cmd_DPR, MSGFL_ALLOC_ETEID, &msg ) );
	
	/* Add the Origin information */
	CHECK_FCT( fd_msg_add_origin ( msg, 0 ) );
	
	/* Add the Disconnect-Cause */
	CHECK_FCT( fd_msg_avp_new ( fd_dict_avp_DC, 0, &avp ) );
	
	/* Search the value in the dictionary */
	memset(&er, 0, sizeof(er));
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_TYPE, TYPE_OF_AVP, fd_dict_avp_DC, &er.type_obj, ENOENT )  );
	er.search.enum_name = reason ?: "REBOOTING";
	CHECK_FCT_DO( fd_dict_search( fd_g_config->cnf_dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &er, &dictobj, ENOENT ), { ASSERT(0); /* internal error: unknown reason */ }  );
	CHECK_FCT( fd_dict_getval( dictobj, &er.search ) );
	
	/* Set the value in the AVP */
	val.u32 = er.search.enum_value.u32;
	CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
	
	/* Save the value also in the peer */
	peer->p_hdr.info.runtime.pir_lastDC = val.u32;
	
	/* Update the peer state and timer */
	CHECK_FCT( fd_psm_change_state(peer, STATE_CLOSING_GRACE) );
	fd_psm_next_timeout(peer, 0, DPR_TIMEOUT);
	
	/* Now send the DPR message */
	CHECK_FCT_DO( fd_out_send(&msg, NULL, peer, 0), /* ignore since we are on timeout anyway */ );
	
	return 0;
}
