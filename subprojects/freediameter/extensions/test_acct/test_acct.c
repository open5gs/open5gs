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

/* This extension simply receives ACR and sends positive ACA after displaying the content, 
but does not commit the data to any storage */

#include <freeDiameter/extension.h>

static struct {
	struct dict_object * Accounting_Record_Number;
	struct dict_object * Accounting_Record_Type;
} tac_dict;
	
/* Callback for incoming Base Accounting application messages */
static int tac_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * data, enum disp_action * act)
{
	struct msg_hdr *hdr = NULL;
	
	TRACE_ENTRY("%p %p %p %p", msg, avp, sess, act);
	
	if (msg == NULL)
		return EINVAL;
	
	/* Check what we received */
	CHECK_FCT( fd_msg_hdr(*msg, &hdr) );
	
	if (hdr->msg_flags & CMD_FLAG_REQUEST) {
		/* It was a request, create an answer */
		struct msg *ans, *qry;
		struct avp * a = NULL;
		struct avp_hdr * h = NULL;
		os0_t s;
		size_t sl;
	
		qry = *msg;
		/* Create the answer message, including the Session-Id AVP */
		CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
		ans = *msg;

		/* Set the Origin-Host, Origin-Realm, Result-Code AVPs */
		CHECK_FCT( fd_msg_rescode_set( ans, "DIAMETER_SUCCESS", NULL, NULL, 1 ) );
		
		fd_log_debug("--------------Received the following Accounting message:--------------");
		
		CHECK_FCT( fd_sess_getsid ( sess, &s, &sl ) );
		fd_log_debug("Session: %.*s", (int)sl, s);
		
		/* The AVPs that we copy in the answer */
		CHECK_FCT( fd_msg_search_avp ( qry, tac_dict.Accounting_Record_Type, &a) );
		if (a) {
			CHECK_FCT( fd_msg_avp_hdr( a, &h )  );
			fd_log_debug("Accounting-Record-Type: %d (%s)", h->avp_value->u32, 
						/* it would be better to search this in the dictionary, but it is only for debug, so ok */
						(h->avp_value->u32 == 1) ? "EVENT_RECORD" : 
						(h->avp_value->u32 == 2) ? "START_RECORD" : 
						(h->avp_value->u32 == 3) ? "INTERIM_RECORD" : 
						(h->avp_value->u32 == 4) ? "STOP_RECORD" : 
						"<unknown value>"
					);
			CHECK_FCT( fd_msg_avp_new ( tac_dict.Accounting_Record_Type, 0, &a ) );
			CHECK_FCT( fd_msg_avp_setvalue( a, h->avp_value ) );
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, a ) );
		}
		CHECK_FCT( fd_msg_search_avp ( qry, tac_dict.Accounting_Record_Number, &a) );
		if (a) {
			CHECK_FCT( fd_msg_avp_hdr( a, &h )  );
			fd_log_debug("Accounting-Record-Number: %d", h->avp_value->u32);
			CHECK_FCT( fd_msg_avp_new ( tac_dict.Accounting_Record_Number, 0, &a ) );
			CHECK_FCT( fd_msg_avp_setvalue( a, h->avp_value ) );
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, a ) );
		}
		
		/* We may also dump other data from the message, such as Accounting session Id, number of packets, ...  */

		fd_log_debug("----------------------------------------------------------------------");
		
		/* Send the answer */
		CHECK_FCT( fd_msg_send( msg, NULL, NULL ) );
		
	} else {
		/* We received an answer message, just discard it */
		CHECK_FCT( fd_msg_free( *msg ) );
		*msg = NULL;
	}
	
	return 0;
}

/* entry point: register handler for Base Accounting messages in the daemon */
static int tac_entry(char * conffile)
{
	struct disp_when data;
	
	TRACE_ENTRY("%p", conffile);
	
	memset(&tac_dict, 0, sizeof(tac_dict));
	memset(&data, 0, sizeof(data));
	
	/* Initialize the dictionary objects we use */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Diameter Base Accounting", &data.app, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Record-Number", &tac_dict.Accounting_Record_Number, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Record-Type", &tac_dict.Accounting_Record_Type, ENOENT) );
	
	/* Register the dispatch callback */
	CHECK_FCT( fd_disp_register( tac_cb, DISP_HOW_APPID, &data, NULL, NULL ) );
	
	/* Advertise the support for the Diameter Base Accounting application in the peer */
	CHECK_FCT( fd_disp_app_support ( data.app, NULL, 0, 1 ) );
	
	return 0;
}

EXTENSION_ENTRY("test_acct", tac_entry);
