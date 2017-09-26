/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Alexandre Westfahl <awestfahl@freediameter.net>						 *
*													 *												 *
* Copyright (c) 2013, WIDE Project and NICT								 *
* Copyright (c) 2010, Alexandre Westfahl, Teraoka Laboratory (Keio University), and the WIDE Project. 	 *		
*													 *
* All rights reserved.											 *
* Based on rgwx_auth plugin (Sebastien Decugis <sdecugis@freediameter.net>)					 *
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
* * Neither the name of the Teraoka Laboratory nor the 							 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of Teraoka Laboratory 						 *
*   													 *
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


/* RADIUS Access-Request messages translation plugin */

#include "rgw_common.h"
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

/* Other constants we use */
#define AI_SIP				6	/* Diameter SIP application */
#define CC_MULTIMEDIA_AUTH_REQUEST	286	/* MAR */
#define CC_MULTIMEDIA_AUTH_ANSWER	286	/* MAA */
#define ACV_ASS_STATE_MAINTAINED	0	/* STATE_MAINTAINED */
#define ACV_ASS_NO_STATE_MAINTAINED	1	/* NO_STATE_MAINTAINED */
#define ER_DIAMETER_SUCCESS_AUTH_SENT_SERVER_NOT_STORED	2008
#define ER_DIAMETER_SUCCESS_SERVER_NAME_NOT_STORED	2006



/* This macro converts a RADIUS attribute to a Diameter AVP of type OctetString */
#define CONV2DIAM_STR( _dictobj_ )	\
	CHECK_PARAMS( attr->length >= sizeof(struct radius_attr_hdr) );			\
	/* Create the AVP with the specified dictionary model */			\
	CHECK_FCT( fd_msg_avp_new ( cs->dict._dictobj_, 0, &avp ) );			\
	value.os.len = attr->length - sizeof(struct radius_attr_hdr);			\
	value.os.data = (os0_t)(attr + 1);						\
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );				\
	/* Add the AVP in the Diameter message. */					\
	CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );		\

#define CONV2DIAM_STR_AUTH( _dictobj_ )	\
	CHECK_PARAMS( attr->length >= sizeof(struct radius_attr_hdr) );			\
	/* Create the AVP with the specified dictionary model */			\
	CHECK_FCT( fd_msg_avp_new ( cs->dict._dictobj_, 0, &avp ) );			\
	value.os.len = attr->length - sizeof(struct radius_attr_hdr);			\
	value.os.data = (os0_t)(attr + 1);						\
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );				\
	/* Add the AVP in the Diameter message. */					\
	CHECK_FCT( fd_msg_avp_add ( auth, MSG_BRW_LAST_CHILD, avp) );			\
		
/* Same thing, for scalar AVPs of 32 bits */
#define CONV2DIAM_32B( _dictobj_ )	\
	CHECK_PARAMS( attr->length == sizeof(struct radius_attr_hdr)+sizeof(uint32_t) );\
	CHECK_FCT( fd_msg_avp_new ( cs->dict._dictobj_, 0, &avp ) );			\
	{										\
		uint8_t * v = (uint8_t *)(attr + 1);					\
		value.u32  = (v[0] << 24)						\
			   | (v[1] << 16)						\
			   | (v[2] <<  8)						\
			   |  v[3] ;							\
	}										\
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );				\
	CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );		\
				
				
				
				
				
/* The state we keep for this plugin */
struct rgwp_config {
	struct {
		struct dict_object * Session_Id;		
		struct dict_object * Auth_Application_Id;	
		struct dict_object * Auth_Session_State;	
		struct dict_object * Origin_Host;		
		struct dict_object * Origin_Realm;		
		struct dict_object * Destination_Realm;		
		struct dict_object * SIP_AOR;			
		struct dict_object * SIP_Method;			
		struct dict_object * Destination_Host;		
		struct dict_object * User_Name;			
		struct dict_object * SIP_Server_URI;		
		struct dict_object * SIP_Number_Auth_Items;	
		struct dict_object * SIP_Authorization;	
		struct dict_object * SIP_Authentication_Scheme;	
		struct dict_object * SIP_Authentication_Info;	
		struct dict_object * SIP_Auth_Data_Item;	
		struct dict_object * Proxy_Info;		
		struct dict_object * Route_Record;		
		struct dict_object * Service_Type;		
		struct dict_object * Result_Code;		
		struct dict_object * Digest_URI;		
		struct dict_object * Digest_Nonce;
		struct dict_object * Digest_CNonce;
		struct dict_object * Digest_Nonce_Count;				
		struct dict_object * Digest_Realm;		
		struct dict_object * Digest_Response;
		struct dict_object * Digest_Method;
		struct dict_object * Digest_Response_Auth;		
		struct dict_object * Digest_Username;
		struct dict_object * Digest_Algorithm;	
		struct dict_object * Digest_QOP;

		
		
	} dict; /* cache of the dictionary objects we use */
	char * confstr;
	//Chained list of nonce
	struct fd_list listnonce;
	//This will be used to lock access to chained list
	pthread_mutex_t nonce_mutex; 
};

typedef struct noncechain noncechain;
struct noncechain
{
	struct fd_list chain;
    os0_t sid;
    size_t sidlen;
    os0_t nonce;
    size_t noncelen;
    
};

static int nonce_add_element(os0_t nonce, size_t noncelen, os0_t sid, size_t sidlen, struct rgwp_config * state)
{
	CHECK_PARAMS(nonce && state && sid && sidlen && noncelen);
	
	noncechain *newelt;
	CHECK_MALLOC(newelt=malloc(sizeof(noncechain)));
	
	CHECK_MALLOC(newelt->nonce= os0dup(nonce, noncelen));
	newelt->noncelen=noncelen;
	
	CHECK_MALLOC(newelt->sid=os0dup(sid, sidlen));
	newelt->sidlen=sidlen;
	
	fd_list_init(&newelt->chain,NULL);
	
	CHECK_POSIX(pthread_mutex_lock(&state->nonce_mutex));
	fd_list_insert_before(&state->listnonce,&newelt->chain);
	CHECK_POSIX(pthread_mutex_unlock(&state->nonce_mutex));
	
	return 0;
}
/*
static void nonce_del_element(char * nonce, struct rgwp_config *state)
{
	struct fd_list * li;
	
	CHECK_PARAMS_DO(nonce && state, return);
	
	for(li=state->listnonce.next;li!=&state->listnonce;li=li->next)
	{
		noncechain *temp=(noncechain *)li;
		
		if(strcmp(temp->nonce,nonce)==0)
		{
			fd_list_unlink (li);
			free(temp->sid);
			free(temp->nonce);
			free(temp);
			break;
		}
	}
}
*/
//Retrieve sid from nonce
static os0_t nonce_get_sid(os0_t nonce, size_t noncelen, size_t * sidlen, struct rgwp_config *state)
{
	struct fd_list * li;
	os0_t sid=NULL;
	
	CHECK_PARAMS_DO(nonce && state && noncelen && sidlen, return NULL);
	*sidlen=0;
	
	// **Start mutex
	CHECK_POSIX_DO(pthread_mutex_lock(&state->nonce_mutex),); 
	for(li=state->listnonce.next;li!=&state->listnonce;li=li->next)
	{
		noncechain *temp=(noncechain *)li;
		
		if (!fd_os_cmp(temp->nonce, temp->noncelen, nonce, noncelen))
		{
			fd_list_unlink (li);
			sid=temp->sid;
			*sidlen=temp->sidlen;
			free(temp->nonce);
			free(temp);
			break;
		}
		
	}
	CHECK_POSIX_DO(pthread_mutex_unlock(&state->nonce_mutex),); 
	// ***Stop mutex
	return sid;
}

static void nonce_deletelistnonce(struct rgwp_config *state)
{
	// **Start mutex
	CHECK_POSIX_DO(pthread_mutex_lock(&state->nonce_mutex),); 
	while(!(FD_IS_LIST_EMPTY(&state->listnonce)) )
	{
		noncechain *temp=(noncechain *)state->listnonce.next;
		
		fd_list_unlink (&temp->chain);
		free(temp->sid);
		free(temp->nonce);
		free(temp);
		
	}
	CHECK_POSIX_DO(pthread_mutex_unlock(&state->nonce_mutex),); 
	// ***Stop mutex
}

/* Initialize the plugin */
static int sip_conf_parse(char * conffile, struct rgwp_config ** state)
{
	struct rgwp_config * new;
	struct dict_object * app;
	
	
	TRACE_ENTRY("%p %p", conffile, state);
	CHECK_PARAMS( state );
	
	CHECK_MALLOC( new = malloc(sizeof(struct rgwp_config)) );
	memset(new, 0, sizeof(struct rgwp_config));
	
	new->confstr = conffile;
	
	/* Resolve all dictionary objects we use */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Session-Id", &new->dict.Session_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Application-Id", &new->dict.Auth_Application_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Session-State", &new->dict.Auth_Session_State, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Host", &new->dict.Origin_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Realm", &new->dict.Origin_Realm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Realm", &new->dict.Destination_Realm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-AOR", &new->dict.SIP_AOR, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Method", &new->dict.SIP_Method, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Host", &new->dict.Destination_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "User-Name", &new->dict.User_Name, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Server-URI", &new->dict.SIP_Server_URI, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Number-Auth-Items", &new->dict.SIP_Number_Auth_Items, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Authorization", &new->dict.SIP_Authorization, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Auth-Data-Item", &new->dict.SIP_Auth_Data_Item, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Authentication-Scheme", &new->dict.SIP_Authentication_Scheme, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Authentication-Info", &new->dict.SIP_Authentication_Info, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Proxy-Info", &new->dict.Proxy_Info, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Route-Record", &new->dict.Route_Record, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Result-Code", &new->dict.Result_Code, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-URI", &new->dict.Digest_URI, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Nonce", &new->dict.Digest_Nonce, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Method", &new->dict.Digest_Method, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-CNonce", &new->dict.Digest_CNonce, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Nonce-Count", &new->dict.Digest_Nonce_Count, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Realm", &new->dict.Digest_Realm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Response", &new->dict.Digest_Response, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Response-Auth", &new->dict.Digest_Response_Auth, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Username", &new->dict.Digest_Username, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Algorithm", &new->dict.Digest_Algorithm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-QoP", &new->dict.Digest_QOP, ENOENT) );


	
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Diameter Session Initiation Protocol (SIP) Application", &app, ENOENT) );
	CHECK_FCT( fd_disp_app_support ( app, NULL, 1, 0 ) );
	
	//chained list
	fd_list_init(&new->listnonce,NULL);
	CHECK_POSIX(pthread_mutex_init(&new->nonce_mutex,NULL));
	
	*state = new;
	return 0;
}

/* deinitialize */
static void sip_conf_free(struct rgwp_config * state)
{
	TRACE_ENTRY("%p", state);
	CHECK_PARAMS_DO( state, return );
	
	nonce_deletelistnonce(state);
	CHECK_POSIX_DO(pthread_mutex_destroy(&state->nonce_mutex), /*continue*/);
	
	free(state);
	return;
}

/* Handle an incoming RADIUS request */
static int sip_rad_req( struct rgwp_config * cs, struct radius_msg * rad_req, struct radius_msg ** rad_ans, struct msg ** diam_fw, struct rgw_client * cli )
{
	int idx;
	int got_AOR = 0;
	int got_Dusername = 0;
	int got_Drealm = 0;
	int got_Duri = 0;
	int got_Dmethod = 0;
	int got_Dnonce = 0;
	int got_Dresponse = 0;
	int got_Dalgorithm = 0;
	os0_t sid = NULL;
	size_t sidlen;
	os0_t un=NULL;
	size_t  un_len;
	size_t nattr_used = 0;
	struct avp *auth_data=NULL, *auth=NULL, *avp = NULL;
	union avp_value value;
	struct session * sess;
	
	TRACE_ENTRY("%p %p %p %p %p", cs, rad_req, rad_ans, diam_fw, cli);
	
	CHECK_PARAMS(rad_req && (rad_req->hdr->code == RADIUS_CODE_ACCESS_REQUEST) && rad_ans && diam_fw && *diam_fw);
	
	/*
	   RFC5090 RADIUS Extension Digest Application
	*/
	CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, *diam_fw, &sess, NULL) );
	if (sess != NULL) {
		TRACE_DEBUG(INFO,"INTERNAL ERROR: We are not supposed to receive a session in radSIP plugin.");
		return EINVAL;		
	}
	
	/* Check basic information is there */
	for (idx = 0; idx < rad_req->attr_used; idx++) {
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(rad_req->buf + rad_req->attr_pos[idx]);
		
		
		switch (attr->type) {
			
			case RADIUS_ATTR_USER_NAME:
				if (attr->length>sizeof(struct radius_attr_hdr)) 
				{
					TRACE_DEBUG(ANNOYING, "Found a User-Name attribute: '%.*s'", (int)(attr->length- sizeof(struct radius_attr_hdr)), (char *)(attr+1));
					un = (os0_t)(attr + 1);
					un_len =attr->length - sizeof(struct radius_attr_hdr);
				}
			break;
			case RADIUS_ATTR_DIGEST_USERNAME:
				got_Dusername = 1;
			break;
			case RADIUS_ATTR_DIGEST_REALM:
				got_Drealm = 1;
			break;
			case RADIUS_ATTR_DIGEST_URI:
				got_Duri = 1;
			break;
			case RADIUS_ATTR_DIGEST_METHOD:
				got_Dmethod = 1;
			break;
			// case RADIUS_ATTR_DIGEST_QOP:
			// 	got_Dqop = 1;
			// break;
			// case RADIUS_ATTR_DIGEST_NONCE_COUNT:
			// 	got_Dnonce_count = 1;
			// break;
			case RADIUS_ATTR_DIGEST_NONCE:
				got_Dnonce = 1;
				
				sid=nonce_get_sid((os0_t)(attr+1), attr->length - sizeof(struct radius_attr_hdr), &sidlen, cs);
				if(!sid)
				{
					TRACE_DEBUG(INFO,"We haven't found the session.'");
					return EINVAL;
				}
				CHECK_FCT(fd_sess_fromsid_msg (sid, sidlen, &sess, NULL));
				free(sid);
							
				
			break;
			// case RADIUS_ATTR_DIGEST_CNONCE:
			// 	got_Dcnonce = 1;
			// break;
			case RADIUS_ATTR_DIGEST_RESPONSE:
				got_Dresponse = 1;
			break;			
			case RADIUS_ATTR_DIGEST_ALGORITHM:
				got_Dalgorithm = 1;
			break;
			case RADIUS_ATTR_SIP_AOR:
				got_AOR = 1;
			break;
		}
	}
	if(!un)
	{
		TRACE_DEBUG(INFO,"No Username in request");
		return EINVAL;
	}

	/* Create the session if it is not already done */
	if (!sess) {
		
		DiamId_t fqdn;
		size_t fqdn_len;
		DiamId_t realm;
		size_t realm_len;
		
		/* Get information on the RADIUS client */
		CHECK_FCT( rgw_clients_get_origin(cli, &fqdn, &fqdn_len, &realm, &realm_len) );
		
		/* Create a new Session-Id. The format is: {fqdn;hi32;lo32;username;diamid} */
		CHECK_MALLOC( sid = malloc(un_len + 1 /* ';' */ + fd_g_config->cnf_diamid_len + 1 /* '\0' */) );
		sidlen = sprintf((char *)sid, "%.*s;%s", (int)un_len, un, fd_g_config->cnf_diamid);
		CHECK_FCT( fd_sess_new(&sess, fqdn, fqdn_len, sid, sidlen) );
		free(sid);
	}
	
	/* Now, add the Session-Id AVP at beginning of Diameter message */
	CHECK_FCT( fd_msg_avp_new ( cs->dict.Session_Id, 0, &avp ) );
	value.os.data = sid;
	value.os.len = sidlen;
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
	CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_FIRST_CHILD, avp) );
	
	TRACE_DEBUG(FULL, "[sip.rgwx] Translating new message for session '%s'...", sid);
	
	/* Now add this session in the message */
	CHECK_FCT( fd_msg_sess_set(*diam_fw, sess) );
		
	/* Add the Destination-Realm AVP */
	CHECK_FCT( fd_msg_avp_new ( cs->dict.Destination_Realm, 0, &avp ) );
	
	int i = 0;
	
	/* Is there an '@' in the user name? We don't care for decorated NAI here */
	for (i = un_len - 2; i > 0; i--) {
		if (un[i] == '@') {
			i++;
			break;
		}
	}

	if (i == 0) {
		/* Not found in the User-Name => we use the local domain of this gateway */
		value.os.data = (os0_t)fd_g_config->cnf_diamrlm;
		value.os.len  = fd_g_config->cnf_diamrlm_len;
	} else {
		value.os.data = un + i;
		value.os.len  = un_len - i;
	}
	
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
	CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
	
	/*
	If the RADIUS Access-Request message does not
	contain any Digest-* attribute, then the RADIUS client does not want
	to apply HTTP Digest authentication, in which case, actions at the
	gateway are outside the scope of this document.
	*/
	
	if(!(got_Dmethod && got_Duri))
	{
		TRACE_DEBUG(INFO,"No Digest attributes in request, we drop it...");
		return 1;
	}

	/* Add the appropriate command code & Auth-Application-Id */
	{
		struct msg_hdr * header = NULL;
		CHECK_FCT( fd_msg_hdr ( *diam_fw, &header ) );
		header->msg_flags = CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE;
		header->msg_code = CC_MULTIMEDIA_AUTH_REQUEST;
		header->msg_appl = AI_SIP;
	
	
		/* Add the Auth-Application-Id */
		{
			CHECK_FCT( fd_msg_avp_new ( cs->dict.Auth_Application_Id, 0, &avp ) );
			value.i32 = header->msg_appl;
			CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
			CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
		}
	}
	/*Add Auth_Session_State  AVP */
	{
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Auth_Session_State, 0, &avp ) );
		value.i32 = ACV_ASS_NO_STATE_MAINTAINED;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
	}

	
	/*Add SIP_Number_Auth_Items  AVP */
	{
		CHECK_FCT( fd_msg_avp_new ( cs->dict.SIP_Number_Auth_Items, 0, &avp ) );
		value.i32 = 1; //We just treat one auth per request in gateway
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
	}

	/* Add SIP_Auth_Data_Item AVP */
	{
		CHECK_FCT( fd_msg_avp_new ( cs->dict.SIP_Auth_Data_Item, 0, &auth_data ) );
	}
	/* Add SIP_Authentication_Scheme AVP */
	{
		CHECK_FCT( fd_msg_avp_new ( cs->dict.SIP_Authentication_Scheme, 0, &avp ) );
		value.i32=0; //There is only Digest Auth in RFC for now
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( auth_data, MSG_BRW_LAST_CHILD, avp) );
	
	}

	
	/* Add SIP_Authorization AVP */
	{
		CHECK_FCT( fd_msg_avp_new ( cs->dict.SIP_Authorization, 0, &auth ) );
		CHECK_FCT( fd_msg_avp_add ( auth_data, MSG_BRW_LAST_CHILD, auth) );
	}

	for (idx = 0; idx < rad_req->attr_used; idx++) 
	{
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(rad_req->buf + rad_req->attr_pos[idx]);
		
		switch (attr->type) {

			case RADIUS_ATTR_USER_NAME:
			CONV2DIAM_STR( User_Name );
			
			if(!got_Dusername)
			{
				CONV2DIAM_STR_AUTH(Digest_Username);
				got_Dusername=1;
			}
			
			break;

			case RADIUS_ATTR_DIGEST_URI:
			
			CONV2DIAM_STR_AUTH(Digest_URI);
			
			//All of these attributes are required by Diameter but not defined in RFC5090 so we provide FAKE values (only in first exchange)
			if(!got_AOR)
			{
				CONV2DIAM_STR( SIP_AOR );
				got_AOR=1;
			}
			/*
			We must provide a fake nonce because of RFC4740 problem
			TODO: remove when RFC is updated
			==START of FAKE
			*/
			if(!got_Dresponse)
			{
				CONV2DIAM_STR_AUTH(Digest_Response);
				got_Dresponse=1;
			}
			/*
			==END of FAKE
			*/
			if(!got_Drealm)
			{
				//We extract Realm from Digest_URI
				DiamId_t realm=NULL;
				size_t realm_len = 0;
				os0_t temp;
				
				temp = (os0_t)(attr + 1);
				
				for (i=attr->length - sizeof(struct radius_attr_hdr) - 1; i>=0; i--) {
					if (temp[i] == '@') {
						realm = (DiamId_t)temp + i + 1;
						CHECK_FCT_DO( fd_os_validate_DiameterIdentity(&realm, &realm_len, 1),
							realm = NULL );
						break;
					}
				}
			
				if(realm!=NULL)
				{
					CHECK_FCT( fd_msg_avp_new ( cs->dict.Digest_Realm, 0, &avp ) );
					value.os.data=(os0_t)realm;
					value.os.len=realm_len;
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( auth, MSG_BRW_LAST_CHILD, avp) );
					
					//We add SIP-Server-URI AVP because SIP server is registrar (through gateway)
					CHECK_FCT( fd_msg_avp_new ( cs->dict.SIP_Server_URI, 0, &avp ) );
					value.os.data=(os0_t)realm;
					value.os.len=realm_len;
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
					
					free(realm);
				}
				else
				{
					TRACE_DEBUG(INFO, "Can't extract domain from URI, droping request...");
					return 1;
				}	
				got_Drealm=1;
			}
			break;

			case RADIUS_ATTR_DIGEST_METHOD:
			CONV2DIAM_STR(SIP_Method);
			CONV2DIAM_STR_AUTH(Digest_Method);
			break;
			case RADIUS_ATTR_DIGEST_REALM:
			CONV2DIAM_STR_AUTH(Digest_Realm);
			
			//We add SIP-Server-URI AVP because SIP server is registrar (through gateway)
			CHECK_FCT( fd_msg_avp_new ( cs->dict.SIP_Server_URI, 0, &avp ) );
			os0_t temp;
			#define SIP_PREFIX	"sip:"
			size_t temp_len = attr->length - sizeof(struct radius_attr_hdr) + CONSTSTRLEN(SIP_PREFIX) + 1;
			CHECK_MALLOC( temp = malloc(temp_len) );
			temp_len = snprintf((char *)temp, temp_len, SIP_PREFIX "%.*s", (int)(attr->length - sizeof(struct radius_attr_hdr)), (char *)(attr + 1));
			
			value.os.data=temp;
			value.os.len=temp_len;
			
			free(temp);
			
			CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
			CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
			break;
			
			case RADIUS_ATTR_DIGEST_USERNAME:
			CONV2DIAM_STR_AUTH(Digest_Username);
			break;
				
			case RADIUS_ATTR_DIGEST_QOP:
			CONV2DIAM_STR_AUTH( Digest_QOP );
			break;
			case RADIUS_ATTR_DIGEST_ALGORITHM:		
			CONV2DIAM_STR_AUTH( Digest_Algorithm );
			break;
			case RADIUS_ATTR_DIGEST_CNONCE:
			CONV2DIAM_STR_AUTH( Digest_CNonce );
			break;
			case RADIUS_ATTR_DIGEST_NONCE:
				CONV2DIAM_STR_AUTH( Digest_Nonce );
			break;
			case RADIUS_ATTR_DIGEST_NONCE_COUNT:
			CONV2DIAM_STR_AUTH( Digest_Nonce_Count );
			break;
			case RADIUS_ATTR_DIGEST_RESPONSE:
			CONV2DIAM_STR_AUTH( Digest_Response );
			break;
			case RADIUS_ATTR_SIP_AOR:
			CONV2DIAM_STR( SIP_AOR );
			break;
				
			default:
			if(!got_Dalgorithm)
			{
				//[Note 3] If Digest-Algorithm is missing, 'MD5' is assumed.
				#define DIGEST_ALGO_MD5	"MD5"
										
				CHECK_FCT( fd_msg_avp_new ( cs->dict.Digest_Algorithm, 0, &avp ) );			
										
				value.os.data = (os0_t)DIGEST_ALGO_MD5;
				value.os.len = CONSTSTRLEN(DIGEST_ALGO_MD5) - 1;
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );					
				CHECK_FCT( fd_msg_avp_add ( auth, MSG_BRW_LAST_CHILD, avp) );	
				got_Dalgorithm=1;	
			}
			
			if(!got_Dnonce)
			{
				//We give a fake nonce because it will be calculated at the server.
				CHECK_FCT( fd_msg_avp_new ( cs->dict.Digest_Nonce, 0, &avp ) );
				value.os.data=(unsigned char *)"nonce";
				value.os.len=strlen((const char *)value.os.data);
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add ( auth, MSG_BRW_LAST_CHILD, avp) );	
				got_Dnonce=1;
			}
			break;
	
		}
	}

	
	CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, auth_data) );
	
	/* Update the radius message to remove all handled attributes */
	rad_req->attr_used = nattr_used;

	//fd_msg_dump_walk(1,*diam_fw);
	
	
	return 0;
}

static int sip_diam_ans( struct rgwp_config * cs, struct msg ** diam_ans, struct radius_msg ** rad_fw, struct rgw_client * cli )
{
	
	
	struct avp *avp, *next;
	struct avp_hdr *ahdr;
	//char buf[254]; /* to store some attributes values (with final '\0') */
	struct session * sess;
	os0_t sid = NULL;
	size_t sidlen;
	
	TRACE_ENTRY("%p %p %p %p", cs, diam_ans, rad_fw, cli);
	CHECK_PARAMS(cs && diam_ans && *diam_ans && rad_fw && *rad_fw);
	
	
	/* MACROS to help in the process: convert AVP data to RADIUS attributes. */
	/* Control large attributes:  _trunc_ = 0 => error; _trunc_ = 1 => truncate; _trunc = 2 => create several attributes */
	#define CONV2RAD_STR( _attr_, _data_, _len_, _trunc_)	{					\
		size_t __l = (size_t)(_len_);								\
		size_t __off = 0;									\
		TRACE_DEBUG(FULL, "Converting AVP to "#_attr_);						\
		if ((_trunc_) == 0) {									\
			CHECK_PARAMS( __l <= 253 );							\
		}											\
		if ((__l > 253) && (_trunc_ == 1)) {							\
			TRACE_DEBUG(INFO, "[authSIP.rgwx] AVP truncated in "#_attr_);			\
			__l = 253;									\
		}											\
		do {											\
			size_t __w = (__l > 253) ? 253 : __l;						\
			CHECK_MALLOC(radius_msg_add_attr(*rad_fw, (_attr_), (_data_) + __off, __w));	\
			__off += __w;									\
			__l   -= __w;									\
		} while (__l);										\
	}

	#define CONV2RAD_32B( _attr_, _data_)	{							\
		uint32_t __v = htonl((uint32_t)(_data_));						\
		TRACE_DEBUG(FULL, "Converting AVP to "#_attr_);						\
		CHECK_MALLOC(radius_msg_add_attr(*rad_fw, (_attr_), (uint8_t *)&__v, sizeof(__v)));	\
	}

	/* Search the different AVPs we handle here */
	CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, *diam_ans, &sess, NULL) );
	if (sess) {
		CHECK_FCT( fd_sess_getsid(sess, &sid, &sidlen) );
	}


	/* Check the Diameter error code */
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Result_Code, &avp) );
	ASSERT( avp ); /* otherwise the message should have been discarded a lot earlier because of ABNF */
	CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
	switch (ahdr->avp_value->u32) {
		case ER_DIAMETER_MULTI_ROUND_AUTH:
		case ER_DIAMETER_SUCCESS_AUTH_SENT_SERVER_NOT_STORED:		
			(*rad_fw)->hdr->code = RADIUS_CODE_ACCESS_CHALLENGE;
			//struct timespec nowts;
			//CHECK_SYS(clock_gettime(CLOCK_REALTIME, &nowts));
			//nowts.tv_sec+=600;
			//CHECK_FCT(fd_sess_settimeout(session, &nowts ));
			break;
		case ER_DIAMETER_SUCCESS_SERVER_NAME_NOT_STORED:
		case ER_DIAMETER_SUCCESS:
			(*rad_fw)->hdr->code = RADIUS_CODE_ACCESS_ACCEPT;
			// in_success=1;
			break;
		
		default:
			(*rad_fw)->hdr->code = RADIUS_CODE_ACCESS_REJECT;
			fd_log_debug("[sip.rgwx] Received Diameter answer with error code '%d', session %.*s, translating into Access-Reject",
					ahdr->avp_value->u32, (int)sidlen, sid);
			return 0;
	}
	/* Remove this Result-Code avp */
	CHECK_FCT( fd_msg_free( avp ) );
	
	/* Now loop in the list of AVPs and convert those that we know how */
	CHECK_FCT( fd_msg_browse(*diam_ans, MSG_BRW_FIRST_CHILD, &next, NULL) );
	
	while (next) {
		int handled = 1;
		avp = next;
		CHECK_FCT( fd_msg_browse(avp, MSG_BRW_WALK, &next, NULL) );
		
		CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
		
		if (!(ahdr->avp_flags & AVP_FLAG_VENDOR)) {
			switch (ahdr->avp_code) {
				
				
				case DIAM_ATTR_DIGEST_NONCE:
					CONV2RAD_STR(DIAM_ATTR_DIGEST_NONCE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					nonce_add_element(ahdr->avp_value->os.data, ahdr->avp_value->os.len, sid, sidlen, cs);
					break;
				case DIAM_ATTR_DIGEST_REALM:
					CONV2RAD_STR(DIAM_ATTR_DIGEST_REALM, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
				case DIAM_ATTR_DIGEST_QOP:
					CONV2RAD_STR(DIAM_ATTR_DIGEST_QOP, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
				case DIAM_ATTR_DIGEST_ALGORITHM:
					CONV2RAD_STR(DIAM_ATTR_DIGEST_ALGORITHM, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;		
				case DIAM_ATTR_DIGEST_RESPONSE_AUTH:
					CONV2RAD_STR(DIAM_ATTR_DIGEST_RESPONSE_AUTH, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					break;
				default:
					handled=0;
					break;
			}
		} 
		else 
		{
			/* Vendor-specific AVPs */
			switch (ahdr->avp_vendor) {
				
				default: /* unknown vendor */
					handled = 0;
			}
		}
		if (handled) {
			CHECK_FCT( fd_msg_free( avp ) );
		}
	}
	
	return 0;
}

/* The exported symbol */
struct rgw_api rgwp_descriptor = {
	.rgwp_name       = "sip",
	.rgwp_conf_parse = sip_conf_parse,
	.rgwp_conf_free  = sip_conf_free,
	.rgwp_rad_req    = sip_rad_req,
	.rgwp_diam_ans   = sip_diam_ans
};	

