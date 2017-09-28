/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2011, WIDE Project and NICT								 *
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

/* The simple Accounting server for freeDiameter */

#include "app_acct.h"

/* Mandatory AVPs for the Accounting-Answer (any value in adding all the other AVPs?) */
static struct {
	struct dict_object * Accounting_Record_Number;
	struct dict_object * Accounting_Record_Type;
} acct_dict;


/* Callback for incoming Base Accounting Accounting-Request messages */
static int acct_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act)
{
	struct msg * m;
	struct avp * a = NULL;
	struct avp_hdr * art=NULL, *arn=NULL; /* We keep a pointer on the Accounting-Record-{Type, Number} AVPs from the query */
	struct acct_record_list rl;
	
	TRACE_ENTRY("%p %p %p %p", msg, avp, sess, act);
	if (msg == NULL)
		return EINVAL;
	
	m = *msg;
	
	/* Prepare a new record list */
	CHECK_FCT( acct_rec_prepare( &rl ) );
	
	/* Maps the AVPs from the query with this record list */
	CHECK_FCT( acct_rec_map( &rl, m ) );
	
	/* Check that at least one AVP was mapped */
	CHECK_FCT( acct_rec_validate( &rl ) );
	
	/* Now, save these mapped AVPs in the database */
	CHECK_FCT( acct_db_insert( &rl ) );
	
	acct_rec_empty( &rl );
	
	/* OK, we can send a positive reply now */
	
	/* Get Accounting-Record-{Number,Type} values */
	CHECK_FCT( fd_msg_search_avp ( m, acct_dict.Accounting_Record_Type, &a) );
	if (a) {
		CHECK_FCT( fd_msg_avp_hdr( a, &art )  );
	}
	CHECK_FCT( fd_msg_search_avp ( m, acct_dict.Accounting_Record_Number, &a) );
	if (a) {
		CHECK_FCT( fd_msg_avp_hdr( a, &arn )  );
	}
	
	/* Create the answer message */
	CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
	m = *msg;

	/* Set the Origin-Host, Origin-Realm, Result-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set( m, "DIAMETER_SUCCESS", NULL, NULL, 1 ) );
	
	/* Add the mandatory AVPs in the ACA */
	if (art) {
		CHECK_FCT( fd_msg_avp_new ( acct_dict.Accounting_Record_Type, 0, &a ) );
		CHECK_FCT( fd_msg_avp_setvalue( a, art->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( m, MSG_BRW_LAST_CHILD, a ) );
	}
	if (arn) {
		CHECK_FCT( fd_msg_avp_new ( acct_dict.Accounting_Record_Number, 0, &a ) );
		CHECK_FCT( fd_msg_avp_setvalue( a, arn->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( m, MSG_BRW_LAST_CHILD, a ) );
	}
	
	/* Send the answer */
	*act = DISP_ACT_SEND;
	return 0;
}
	

/* entry point */
static int acct_entry(char * conffile)
{
	struct disp_when data;
	
	TRACE_ENTRY("%p", conffile);
	
#ifndef TEST_DEBUG /* We do this differently in the test scenario */
	/* Initialize the configuration and parse the file */
	CHECK_FCT( acct_conf_init() );
	CHECK_FCT( acct_conf_parse(conffile) );
	CHECK_FCT( acct_conf_check(conffile) );
#endif /* TEST_DEBUG */
	
	/* Now initialize the database module */
	CHECK_FCT( acct_db_init() );
	
	/* Search the AVPs we will need in this file */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Record-Number", &acct_dict.Accounting_Record_Number, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Record-Type", &acct_dict.Accounting_Record_Type, ENOENT) );
	
	/* Register the dispatch callbacks */
	memset(&data, 0, sizeof(data));
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Diameter Base Accounting", &data.app, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Accounting-Request", &data.command, ENOENT) );
	CHECK_FCT( fd_disp_register( acct_cb, DISP_HOW_CC, &data, NULL, NULL ) );
	
	/* Advertise the support for the Diameter Base Accounting application in the peer */
	CHECK_FCT( fd_disp_app_support ( data.app, NULL, 0, 1 ) );
	
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	/* Close the db connection */
	acct_db_free();
	
	/* Destroy the configuration */
	acct_conf_free();
}

EXTENSION_ENTRY("app_acct", acct_entry);
