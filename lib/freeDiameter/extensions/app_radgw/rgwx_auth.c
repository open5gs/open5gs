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

/* RADIUS Access-Request messages translation plugin */

#include "rgw_common.h"

/* Attributes missing from radius.h */
#define RADIUS_ATTR_CHAP_PASSWORD	3
#define RADIUS_ATTR_ARAP_PASSWORD	70

/* Other constants we use */
#define AI_NASREQ		1	/* Diameter NASREQ */
#define AI_EAP			5	/* Diameter EAP application */
#define CC_AA			265	/* AAR */
#define CC_DIAMETER_EAP		268	/* DER */
#define ACV_ART_AUTHORIZE_AUTHENTICATE	3	/* AUTHORIZE_AUTHENTICATE */
#define ACV_OAP_RADIUS			1	/* RADIUS */
#define ACV_ASS_STATE_MAINTAINED	0	/* STATE_MAINTAINED */
#define ACV_ASS_NO_STATE_MAINTAINED	1	/* NO_STATE_MAINTAINED */

/* The state we keep for this plugin */
struct rgwp_config {
	struct {
		struct dict_object * ARAP_Password;		/* ARAP-Password */
		struct dict_object * ARAP_Security;		/* ARAP-Security */
		struct dict_object * ARAP_Security_Data;	/* ARAP-Security-Data */
		struct dict_object * Auth_Application_Id;	/* Auth-Application-Id */
		struct dict_object * Auth_Request_Type;		/* Auth-Request-Type */
		struct dict_object * Authorization_Lifetime;	/* Authorization-Lifetime */
		struct dict_object * Callback_Number;		/* Callback-Number */
		struct dict_object * Called_Station_Id;		/* Called-Station-Id */
		struct dict_object * Calling_Station_Id;	/* Calling-Station-Id */
		struct dict_object * CHAP_Algorithm;		/* CHAP-Algorithm */
		struct dict_object * CHAP_Auth;			/* CHAP-Auth */
		struct dict_object * CHAP_Challenge;		/* CHAP-Challenge */
		struct dict_object * CHAP_Ident;		/* CHAP-Ident */
		struct dict_object * CHAP_Response;		/* CHAP-Response */
		struct dict_object * Destination_Host;		/* Destination-Host */
		struct dict_object * Destination_Realm;		/* Destination-Realm */
		struct dict_object * Connect_Info;		/* Connect-Info */
		struct dict_object * EAP_Payload;		/* EAP-Payload */
		struct dict_object * Error_Message;		/* Error-Message */
		struct dict_object * Error_Reporting_Host;	/* Error-Reporting-Host */
		struct dict_object * Failed_AVP;		/* Failed-AVP */
		struct dict_object * Framed_Compression;	/* Framed-Compression */
		struct dict_object * Framed_IP_Address;		/* Framed-IP-Address */
		struct dict_object * Framed_IP_Netmask;		/* Framed-IP-Netmask */
		struct dict_object * Framed_Interface_Id;	/* Framed-Interface-Id */
		struct dict_object * Framed_IPv6_Prefix;	/* Framed-IPv6-Prefix */
		struct dict_object * Framed_MTU;		/* Framed-MTU */
		struct dict_object * Framed_Protocol;		/* Framed-Protocol */
		struct dict_object * Login_IP_Host;		/* Login-IP-Host */
		struct dict_object * Login_IPv6_Host;		/* Login-IPv6-Host */
		struct dict_object * Login_LAT_Group;		/* Login-LAT-Group */
		struct dict_object * Login_LAT_Node;		/* Login-LAT-Node */
		struct dict_object * Login_LAT_Port;		/* Login-LAT-Port */
		struct dict_object * Login_LAT_Service;		/* Login-LAT-Service */
		struct dict_object * NAS_Identifier;		/* NAS-Identifier */
		struct dict_object * NAS_IP_Address;		/* NAS-IP-Address */
		struct dict_object * NAS_IPv6_Address;		/* NAS-IPv6-Address */
		struct dict_object * NAS_Port;			/* NAS-Port */
		struct dict_object * NAS_Port_Id;		/* NAS-Port-Id */
		struct dict_object * NAS_Port_Type;		/* NAS-Port-Type */
		struct dict_object * Origin_AAA_Protocol;	/* Origin-AAA-Protocol */
		struct dict_object * Origin_Host;		/* Origin-Host */
		struct dict_object * Origin_Realm;		/* Origin-Realm */
		struct dict_object * Originating_Line_Info;	/* Originating-Line-Info */
		struct dict_object * Port_Limit;		/* Port-Limit */
		struct dict_object * Re_Auth_Request_Type;	/* Re-Auth-Request-Type */
		struct dict_object * Result_Code;		/* Result-Code */
		struct dict_object * Service_Type;		/* Service-Type */
		struct dict_object * Session_Id;		/* Session-Id */
		struct dict_object * Session_Timeout;		/* Session-Timeout */
		struct dict_object * State;			/* State */
		struct dict_object * Tunneling;			/* Tunneling */
		struct dict_object * Tunnel_Type;		/* Tunnel-Type */
		struct dict_object * Tunnel_Medium_Type;	/* Tunnel-Medium-Type */
		struct dict_object * Tunnel_Client_Endpoint;	/* Tunnel-Client-Endpoint */
		struct dict_object * Tunnel_Server_Endpoint;	/* Tunnel-Server-Endpoint */
		struct dict_object * Tunnel_Private_Group_Id;	/* Tunnel-Private-Group-Id */
		struct dict_object * Tunnel_Preference;		/* Tunnel-Preference */
		struct dict_object * Tunnel_Client_Auth_Id;	/* Tunnel-Client-Auth-Id */
		struct dict_object * Tunnel_Server_Auth_Id;	/* Tunnel-Server-Auth-Id */
		struct dict_object * User_Name;			/* User-Name */
		struct dict_object * User_Password;		/* User-Password */
		
	} dict; /* cache of the dictionary objects we use */
	struct session_handler * sess_hdl; /* We store RADIUS request authenticator information in the session */
	char * confstr;
	
	int ignore_nai;
};

struct sess_state {
	uint8_t req_auth[16];
};


/* Initialize the plugin */
static int auth_conf_parse(char * confstr, struct rgwp_config ** state)
{
	struct rgwp_config * new;
	struct dict_object * app;
	
	TRACE_ENTRY("%p %p", confstr, state);
	CHECK_PARAMS( state );
	
	CHECK_MALLOC( new = malloc(sizeof(struct rgwp_config)) );
	memset(new, 0, sizeof(struct rgwp_config));
	
	CHECK_FCT( fd_sess_handler_create( &new->sess_hdl, (void *)free, NULL, NULL ) );
	new->confstr = confstr;
	
	if (confstr && strstr(confstr, "nonai"))
		new->ignore_nai = 1;
	
	/* Resolve all dictionary objects we use */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "ARAP-Password", &new->dict.ARAP_Password, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "ARAP-Security", &new->dict.ARAP_Security, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "ARAP-Security-Data", &new->dict.ARAP_Security_Data, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Application-Id", &new->dict.Auth_Application_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Request-Type", &new->dict.Auth_Request_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Authorization-Lifetime", &new->dict.Authorization_Lifetime, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Callback-Number", &new->dict.Callback_Number, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Called-Station-Id", &new->dict.Called_Station_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Calling-Station-Id", &new->dict.Calling_Station_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "CHAP-Algorithm", &new->dict.CHAP_Algorithm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "CHAP-Auth", &new->dict.CHAP_Auth, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "CHAP-Challenge", &new->dict.CHAP_Challenge, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "CHAP-Ident", &new->dict.CHAP_Ident, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "CHAP-Response", &new->dict.CHAP_Response, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Connect-Info", &new->dict.Connect_Info, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Host", &new->dict.Destination_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Realm", &new->dict.Destination_Realm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "EAP-Payload", &new->dict.EAP_Payload, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Error-Message", &new->dict.Error_Message, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Error-Reporting-Host", &new->dict.Error_Reporting_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Failed-AVP", &new->dict.Failed_AVP, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Compression", &new->dict.Framed_Compression, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IP-Address", &new->dict.Framed_IP_Address, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IP-Netmask", &new->dict.Framed_IP_Netmask, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Interface-Id", &new->dict.Framed_Interface_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IPv6-Prefix", &new->dict.Framed_IPv6_Prefix, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-MTU", &new->dict.Framed_MTU, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Protocol", &new->dict.Framed_Protocol, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-IP-Host", &new->dict.Login_IP_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-IPv6-Host", &new->dict.Login_IPv6_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-LAT-Group", &new->dict.Login_LAT_Group, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-LAT-Node", &new->dict.Login_LAT_Node, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-LAT-Port", &new->dict.Login_LAT_Port, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-LAT-Service", &new->dict.Login_LAT_Service, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "NAS-Identifier", &new->dict.NAS_Identifier, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "NAS-IP-Address", &new->dict.NAS_IP_Address, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "NAS-IPv6-Address", &new->dict.NAS_IPv6_Address, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "NAS-Port", &new->dict.NAS_Port, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "NAS-Port-Id", &new->dict.NAS_Port_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "NAS-Port-Type", &new->dict.NAS_Port_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-AAA-Protocol", &new->dict.Origin_AAA_Protocol, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Host", &new->dict.Origin_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Realm", &new->dict.Origin_Realm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Originating-Line-Info", &new->dict.Originating_Line_Info, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Port-Limit", &new->dict.Port_Limit, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Re-Auth-Request-Type", &new->dict.Re_Auth_Request_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Result-Code", &new->dict.Result_Code, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Service-Type", &new->dict.Service_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Session-Id", &new->dict.Session_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Session-Timeout", &new->dict.Session_Timeout, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "State", &new->dict.State, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunneling", &new->dict.Tunneling, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Type", &new->dict.Tunnel_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Medium-Type", &new->dict.Tunnel_Medium_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Client-Endpoint", &new->dict.Tunnel_Client_Endpoint, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Server-Endpoint", &new->dict.Tunnel_Server_Endpoint, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Private-Group-Id", &new->dict.Tunnel_Private_Group_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Preference", &new->dict.Tunnel_Preference, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Client-Auth-Id", &new->dict.Tunnel_Client_Auth_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Server-Auth-Id", &new->dict.Tunnel_Server_Auth_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "User-Name", &new->dict.User_Name, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "User-Password", &new->dict.User_Password, ENOENT) );
	
	/* This plugin provides the following Diameter authentication applications support: */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Diameter Network Access Server Application", &app, ENOENT) );
	CHECK_FCT( fd_disp_app_support ( app, NULL, 1, 0 ) );

	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Diameter Extensible Authentication Protocol (EAP) Application", &app, ENOENT) );
	CHECK_FCT( fd_disp_app_support ( app, NULL, 1, 0 ) );
	
	*state = new;
	return 0;
}

/* deinitialize */
static void auth_conf_free(struct rgwp_config * state)
{
	TRACE_ENTRY("%p", state);
	CHECK_PARAMS_DO( state, return );
	CHECK_FCT_DO( fd_sess_handler_destroy( &state->sess_hdl, NULL ),  );
	free(state);
	return;
}

/* Handle an incoming RADIUS request */
static int auth_rad_req( struct rgwp_config * cs, struct radius_msg * rad_req, struct radius_msg ** rad_ans, struct msg ** diam_fw, struct rgw_client * cli )
{
	int idx;
	int got_id = 0;
	int got_mac = 0;
	int got_passwd = 0;
	int got_eap = 0;
	int got_empty_eap = 0;
	const char * prefix = "Diameter/";
	size_t pref_len;
	os0_t dh = NULL;
	size_t dh_len = 0;
	os0_t dr = NULL;
	size_t dr_len = 0;
	os0_t si = NULL;
	size_t si_len = 0;
	os0_t un = NULL;
	size_t un_len = 0;
	size_t nattr_used = 0;
	struct avp ** avp_tun = NULL, *avp = NULL;
	union avp_value value;
	struct session * sess;
	
	TRACE_ENTRY("%p %p %p %p %p", cs, rad_req, rad_ans, diam_fw, cli);
	CHECK_PARAMS(cs && rad_req && (rad_req->hdr->code == RADIUS_CODE_ACCESS_REQUEST) && rad_ans && diam_fw && *diam_fw);
	
	pref_len = strlen(prefix);
	
	/*
	   Guidelines:
	     http://tools.ietf.org/html/rfc4005#section-9.1
	     http://tools.ietf.org/html/rfc4072#section-6.1

	   When a Translation Agent receives a RADIUS message, the following
	   steps should be taken:

	      -  If a Message-Authenticator attribute is present, the value MUST
        	 be checked but not included in the Diameter message.  If it is
        	 incorrect, the RADIUS message should be silently discarded.
        	 The gateway system SHOULD generate and include a Message-
        	 Authenticator in returned RADIUS responses.
	             -> done in rgw_msg_auth_check

	      -  The transport address of the sender MUST be checked against the
        	 NAS identifying attributes.  See the description of NAS-
        	 Identifier and NAS-IP-Address below.
		     -> done in rgw_clients_check_origin

	      -  The Translation Agent must maintain transaction state
        	 information relevant to the RADIUS request, such as the
        	 Identifier field in the RADIUS header, any existing RADIUS
        	 Proxy-State attribute, and the source IP address and port
        	 number of the UDP packet.  These may be maintained locally in a
        	 state table or saved in a Proxy-Info AVP group.  A Diameter
        	 Session-Id AVP value must be created using a session state
        	 mapping mechanism.
		     -> Identifier, source and port are saved along with the request,
		        and associated with the session state.
		     -> sub_echo_drop should handle the Proxy-State attribute (conf issue)

	      -  The Diameter Origin-Host and Origin-Realm AVPs MUST be created
        	 and added by using the information from an FQDN corresponding
        	 to the NAS-IP-Address attribute (preferred if available),
        	 and/or to the NAS-Identifier attribute.  (Note that the RADIUS
        	 NAS-Identifier is not required to be an FQDN.)
		     -> done in rgw_clients_create_origin.

	      -  The response MUST have an Origin-AAA-Protocol AVP added,
        	 indicating the protocol of origin of the message.
		     -> what "response" ??? Added to the AAR / DER in this function.

	      -  The Proxy-Info group SHOULD be added, with the local server's
        	 identity specified in the Proxy-Host AVP.  This should ensure
        	 that the response is returned to this system.
		     -> We don't need this, answer is always routed here anyway.
		     
	      For EAP:
	      
	      o  RADIUS EAP-Message attribute(s) are translated to a Diameter
		 EAP-Payload AVP.  If multiple RADIUS EAP-Message attributes are
		 present, they are concatenated and translated to a single Diameter
		 EAP-Payload AVP.
		     -> concatenation done by radius_msg_get_eap

	      -> the remaining is specific conversion rules
	*/
	
	/* Check basic information is there, and also retrieve some attribute information */
	for (idx = 0; idx < rad_req->attr_used; idx++) {
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(rad_req->buf + rad_req->attr_pos[idx]);
		uint8_t * attr_val = (uint8_t *)(attr + 1);
		size_t attr_len = attr->length - sizeof(struct radius_attr_hdr);
		
		switch (attr->type) {
			case RADIUS_ATTR_NAS_IP_ADDRESS:
			case RADIUS_ATTR_NAS_IDENTIFIER:
			case RADIUS_ATTR_NAS_IPV6_ADDRESS:
				got_id = 1;
				break;
			case RADIUS_ATTR_MESSAGE_AUTHENTICATOR:
				got_mac = 1;
				break;
			case RADIUS_ATTR_EAP_MESSAGE:
				got_eap = 1;
				if (attr->length == 2)
					got_empty_eap = 1;
				break;
			case RADIUS_ATTR_USER_PASSWORD:
			case RADIUS_ATTR_CHAP_PASSWORD:
			case RADIUS_ATTR_ARAP_PASSWORD:
				got_passwd += 1;
				break;
				
			/* Is there a State attribute with prefix "Diameter/" in the message? (in that case: Diameter/Destination-Host/Destination-Realm/Session-Id) */
			/* NOTE: RFC4005 says "Origin-Host" here, but it's not coherent with the rules for answers. Destination-Host makes more sense */
			case RADIUS_ATTR_STATE:
				if ((attr_len > pref_len + 5 /* for the '/'s and non empty strings */ ) 
					&& ! memcmp(attr_val, prefix, pref_len)) {
					int i, start;

					TRACE_DEBUG(ANNOYING, "Found a State attribute with '%s' prefix (attr #%d).", prefix, idx);

					/* Now parse the value and check its content is valid. Unfortunately we cannot use strchr here since strings are not \0-terminated */

					i = start = pref_len;
					dh = attr_val + i;
					for (; (i < attr_len - 2) && (attr_val[i] != '/'); i++) /* loop */;
					if ( i >= attr_len - 2 ) continue; /* the attribute format is not good */
					dh_len = i - start;

					start = ++i;
					dr = attr_val + i;
					for (; (i < attr_len - 1) && (attr_val[i] != '/'); i++) /* loop */;
					if ( i >= attr_len - 1 ) continue; /* the attribute format is not good */
					dr_len = i - start;

					i++;
					si = attr_val + i;
					si_len = attr_len - i;

					TRACE_DEBUG(ANNOYING, "Attribute parsed successfully: DH:'%.*s' DR:'%.*s' SI:'%.*s'.", (int)dh_len, dh, (int)dr_len, dr, (int)si_len, si);
					/* Remove from the message */
					for (i = idx + 1; i < rad_req->attr_used; i++)
						rad_req->attr_pos[i - 1] = rad_req->attr_pos[i];
					rad_req->attr_used -= 1;
					idx--;
				}
				break;
		
			case RADIUS_ATTR_USER_NAME:
				TRACE_DEBUG(ANNOYING, "Found a User-Name attribute: '%.*s'", (int)attr_len, attr_len ? (char *)attr_val : "");
				un = attr_val;
				un_len = attr_len;
				break;
			
		}
	}
	if (!got_id) {
		TRACE_DEBUG(INFO, "RADIUS Access-Request did not contain a NAS IP or Identifier attribute, reject.");
		return EINVAL;
	}
	/* [Note 1] An Access-Request that contains either a User-Password or
	   CHAP-Password or ARAP-Password or one or more EAP-Message attributes
	   MUST NOT contain more than one type of those four attributes.  If it
	   does not contain any of those four attributes, it SHOULD contain a
	   Message-Authenticator.  If any packet type contains an EAP-Message
	   attribute it MUST also contain a Message-Authenticator.  A RADIUS
	   server receiving an Access-Request not containing any of those four
	   attributes and also not containing a Message-Authenticator attribute
	   SHOULD silently discard it.  */
	if (((got_eap + got_passwd) > 1) || (got_eap && !got_mac) || (!got_eap && !got_passwd && !got_mac)) {
		TRACE_DEBUG(INFO, "RADIUS Access-Request not conform to RFC3579 sec 3.3 note 1, discard.");
		return EINVAL;
	}
	
	
	
	/*
	      -  If the RADIUS request contained a State attribute and the
        	 prefix of the data is "Diameter/", the data following the
        	 prefix contains the Diameter Origin-Host/Origin-Realm/Session-
        	 Id.  If no such attributes are present and the RADIUS command
        	 is an Access-Request, a new Session-Id is created.  The
        	 Session-Id is included in the Session-Id AVP.
	*/
	
	/* Add the Destination-Realm AVP */
	CHECK_FCT( fd_msg_avp_new ( cs->dict.Destination_Realm, 0, &avp ) );
	if (dr) {
		value.os.data = (unsigned char *)dr;
		value.os.len = dr_len;
	} else {
		int i = 0;
		if (un && ! cs->ignore_nai) {
			/* Is there an '@' in the user name? We don't care for decorated NAI here */
			for (i = un_len - 2; i > 0; i--) {
				if (un[i] == '@') {
					i++;
					break;
				}
			}
		}
		if (i <= 0) {
			/* Not found in the User-Name => we use the local domain of this gateway */
			value.os.data = (uint8_t *)fd_g_config->cnf_diamrlm;
			value.os.len  = fd_g_config->cnf_diamrlm_len;
		} else {
			value.os.data = un + i;
			value.os.len  = un_len - i;
		}
	}
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
	CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_FIRST_CHILD, avp) );
	
	/* Add the Destination-Host if found */
	if (dh) {
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Destination_Host, 0, &avp ) );
		value.os.data = (unsigned char *)dh;
		value.os.len = dh_len;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_FIRST_CHILD, avp) );
	}
	
	/* Create the session if it is not already done */
	{
		os0_t sess_str = NULL;
		size_t sess_strlen;
		
		if (si_len) {
			/* We already have the Session-Id, just use it */
			CHECK_FCT( fd_sess_fromsid_msg ( si, si_len, &sess, NULL) );
		} else {
			/* Create a new Session-Id string */
			
			DiamId_t fqdn;
			size_t fqdnlen;
			DiamId_t realm;
			size_t realmlen;
			
			/* Get information on the RADIUS client */
			CHECK_FCT( rgw_clients_get_origin(cli, &fqdn, &fqdnlen, &realm, &realmlen) );
			
			/* If we have a user name, create the new session with it */
			if (un) {
				int len;
				/* If not found, create a new Session-Id. Our format is: {fqdn;hi32;lo32;username;diamid} */
				CHECK_MALLOC( sess_str = malloc(un_len + 1 /* ';' */ + fd_g_config->cnf_diamid_len + 1 /* '\0' */) );
				len = sprintf((char *)sess_str, "%.*s;%s", (int)un_len, un, fd_g_config->cnf_diamid);
				CHECK_FCT( fd_sess_new(&sess, fqdn, fqdnlen, sess_str, len) );
				free(sess_str);
			} else {
				/* We don't have enough information to create the Session-Id, the RADIUS message is probably invalid */
				TRACE_DEBUG(INFO, "RADIUS Access-Request does not contain a User-Name attribute, rejecting.");
				return EINVAL;
			}	
		}
		
		/* Now, add the Session-Id AVP at beginning of Diameter message */
		CHECK_FCT( fd_sess_getsid(sess, &sess_str, &sess_strlen) );
		TRACE_DEBUG(FULL, "[auth.rgwx] Translating new message for session '%s'...", sess_str);
		
		/* Now add this session in the message */
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Session_Id, 0, &avp ) );
		value.os.data = sess_str;
		value.os.len = sess_strlen;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_FIRST_CHILD, avp) );
		CHECK_FCT( fd_msg_sess_set(*diam_fw, sess) );
	}
	
	
	/* Add the appropriate command code & Auth-Application-Id */
	{
		struct msg_hdr * header = NULL;
		CHECK_FCT( fd_msg_hdr ( *diam_fw, &header ) );
		header->msg_flags = CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE;
		if (got_eap) {
			header->msg_code = CC_DIAMETER_EAP;
			header->msg_appl = AI_EAP;
		} else {
			header->msg_code = CC_AA;
			header->msg_appl = AI_NASREQ;
		}
		
		/* Add the Auth-Application-Id */
		{
			CHECK_FCT( fd_msg_avp_new ( cs->dict.Auth_Application_Id, 0, &avp ) );
			value.i32 = header->msg_appl;
			CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
			CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
		}
	}
	
	/*  	The type of request is identified through the Auth-Request-Type AVP
		[BASE].  The recommended value for most RADIUS interoperabily
		situations is AUTHORIZE_AUTHENTICATE. */
	
	/* Add Auth-Request-Type AVP */
	{
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Auth_Request_Type, 0, &avp ) );
		value.i32 = ACV_ART_AUTHORIZE_AUTHENTICATE;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
	}
	
	/* Add Origin-AAA-Protocol AVP */
	{
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Origin_AAA_Protocol, 0, &avp ) );
		value.i32 = ACV_OAP_RADIUS;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
	}
	
	/* Convert the EAP payload (concat RADIUS attributes) */
	if (got_eap) {
		CHECK_FCT( fd_msg_avp_new ( cs->dict.EAP_Payload, 0, &avp ) );
		
		/*    o  An empty RADIUS EAP-Message attribute (with length 2) signifies
			 EAP-Start, and it is translated to an empty EAP-Payload AVP. */
		if (got_empty_eap) {
			value.os.len = 0;
			value.os.data = (uint8_t *)"";
		} else {
			CHECK_MALLOC( value.os.data = radius_msg_get_eap(rad_req, &value.os.len) );
		}
		
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
	}
	
	/* Tunnel AVPs need some preparation */
	/* Convert the attributes one by one */
	for (idx = 0; idx < rad_req->attr_used; idx++) {
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(rad_req->buf + rad_req->attr_pos[idx]);

		switch (attr->type) {
			
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
				
			/* And the 64b version */
			#define CONV2DIAM_64B( _dictobj_ )	\
				CHECK_PARAMS( attr->length == sizeof(struct radius_attr_hdr)+sizeof(uint64_t) );\
				CHECK_FCT( fd_msg_avp_new ( cs->dict._dictobj_, 0, &avp ) );			\
				{										\
					uint8_t * v = (uint8_t *)(attr + 1);					\
					value.u64  = ((uint64_t)(v[0]) << 56)					\
					           | ((uint64_t)(v[1]) << 48)					\
					           | ((uint64_t)(v[2]) << 40)					\
					           | ((uint64_t)(v[3]) << 32)					\
					           | ((uint64_t)(v[4]) << 24)					\
					           | ((uint64_t)(v[5]) << 16)					\
					           | ((uint64_t)(v[6]) <<  8)					\
					           |  (uint64_t)(v[7]) ;					\
				}										\
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );				\
				CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );		\
				
		/* RFC 2865 */	
			/*
			      -  The Destination-Realm AVP is created from the information found
        			 in the RADIUS User-Name attribute.
				 	-> done in rgw_clients_create_origin
			*/
			case RADIUS_ATTR_USER_NAME:
				CONV2DIAM_STR( User_Name );
				break;
				
			/*
			      -  If the RADIUS User-Password attribute is present, the password
        			 must be unencrypted by using the link's RADIUS shared secret.
        			 The unencrypted value must be forwarded in a User-Password AVP
        			 using Diameter security.
			*/
			case RADIUS_ATTR_USER_PASSWORD:
				if ((attr->length - 2) % 16) {
					TRACE_DEBUG(INFO, "Invalid length of User-Password attribute: %hhd", attr->length);
					return EINVAL;
				}
				{
					/* Decipher following this logic (refers to rfc2865#section-5.2 )
					   b1 = MD5(S + RA)	p1 = c(1) xor b1
					   b2 = MD5(S + c(1))   p2 = c(2) xor b2
					   ...
					*/
					
					uint8_t *ciph = (uint8_t *)(attr+1); 	/* c(i) */
					size_t ciph_len = attr->length - 2;
					uint8_t deciph[128];			/* pi */
					size_t deciph_len = 0;
					uint8_t * secret;			/* S */
					size_t secret_len;
					uint8_t hash[16];			/* b(i) */
					const uint8_t *addr[2];
					size_t len[2];
					
					/* Retrieve the shared secret */
					CHECK_FCT(rgw_clients_getkey(cli, &secret, &secret_len));
					
					/* Initial b1 = MD5(S + RA) */
					addr[0] = secret;
					len[0] = secret_len;
					addr[1] = rad_req->hdr->authenticator;
					len[1] = 16;
					md5_vector(2, addr, len, hash);
					
					/* loop */
					while (deciph_len < ciph_len) {
						int i;
						/* pi = c(i) xor bi */
						for (i = 0; i < 16; i++)
							deciph[deciph_len + i] = ciph[deciph_len + i] ^ hash[i];
							/* do we have to remove the padding '\0's ? */
						
						/* b(i+1) = MD5(S + c(i) */
						addr[1] = ciph + deciph_len;
						md5_vector(2, addr, len, hash);
						
						deciph_len += 16;
					}
					
					/* Now save this value in the AVP */
					CHECK_FCT( fd_msg_avp_new ( cs->dict.User_Password, 0, &avp ) );
					value.os.data = &deciph[0];
					value.os.len  = deciph_len;
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				}
				break;
				

			/*
			      -  If the RADIUS CHAP-Password attribute is present, the Ident and
        			 Data portion of the attribute are used to create the CHAP-Auth
        			 grouped AVP.
			*/
			case RADIUS_ATTR_CHAP_PASSWORD:
				CHECK_PARAMS( attr->length == 19 /* RFC 2865 */);
				{
					uint8_t * c = (uint8_t *)(attr + 1);
					struct avp * chap_auth;
					CHECK_FCT( fd_msg_avp_new ( cs->dict.CHAP_Auth, 0, &chap_auth ) );
					CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, chap_auth) );

					CHECK_FCT( fd_msg_avp_new ( cs->dict.CHAP_Algorithm, 0, &avp ) );
					value.u32 = 5; /* The only value defined currently... */
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( chap_auth, MSG_BRW_LAST_CHILD, avp) );
					
					CHECK_FCT( fd_msg_avp_new ( cs->dict.CHAP_Ident, 0, &avp ) );
					value.os.data = c;
					value.os.len = 1;
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( chap_auth, MSG_BRW_LAST_CHILD, avp) );
					
					c++;
					
					CHECK_FCT( fd_msg_avp_new ( cs->dict.CHAP_Response, 0, &avp ) );
					value.os.data = c;
					value.os.len = attr->length - 3;
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( chap_auth, MSG_BRW_LAST_CHILD, avp) );
				}
				break;
				
			case RADIUS_ATTR_NAS_IP_ADDRESS:
				CONV2DIAM_STR( NAS_IP_Address );
				break;
				
			case RADIUS_ATTR_NAS_PORT:
				CONV2DIAM_32B( NAS_Port );
				break;
				
			case RADIUS_ATTR_SERVICE_TYPE:
				CONV2DIAM_32B( Service_Type );
				break;
				
			case RADIUS_ATTR_FRAMED_PROTOCOL:
				CONV2DIAM_32B( Framed_Protocol );
				break;
				
			case RADIUS_ATTR_FRAMED_IP_ADDRESS:
				CONV2DIAM_STR( Framed_IP_Address );
				break;
				
			case RADIUS_ATTR_FRAMED_IP_NETMASK:
				CONV2DIAM_STR( Framed_IP_Netmask );
				break;
				
			/* Framed-Routing never present in an Access-Request */
			/* Filter-Id never present in an Access-Request */
				
			case RADIUS_ATTR_FRAMED_MTU:
				CONV2DIAM_32B( Framed_MTU );
				break;
			
			case RADIUS_ATTR_FRAMED_COMPRESSION:
				CONV2DIAM_32B( Framed_Compression );
				break;
			
			case RADIUS_ATTR_LOGIN_IP_HOST:
				CONV2DIAM_STR( Login_IP_Host );
				break;
				
			/* Login-Service never present in an Access-Request */
			/* Login-TCP-Port never present in an Access-Request */
			/* Reply-Message never present in an Access-Request */
			
			case RADIUS_ATTR_CALLBACK_NUMBER:
				CONV2DIAM_STR( Callback_Number );
				break;
				
			/* Callback-Id never present in an Access-Request */
			/* Framed-Route never present in an Access-Request */
			/* Framed-IPX-Network never present in an Access-Request */
				
			case RADIUS_ATTR_STATE:
				CONV2DIAM_STR( State );
				break;
			
			/* Class never present in an Access-Request */
				
			case RADIUS_ATTR_VENDOR_SPECIFIC:
				/* RFC 4005, Section 9.6 : 
					   Systems that don't have vendor format knowledge MAY discard such
					   attributes without knowing a suitable translation.
					   
					   [conversion rule in 9.6.2]
				 */
				if (attr->length >= 6) {
					uint32_t vendor_id;
					uint8_t * c = (uint8_t *)(attr + 1);
					
					vendor_id = c[0] << 24 | c[1] << 16 | c[2] << 8 | c[3];
					c += 4;
					
					switch (vendor_id) {
						
						/* For the vendors we KNOW they follow the VSA recommended format, we convert following the rules of RFC4005 (9.6.2) */
						case RADIUS_VENDOR_ID_MICROSOFT : /* RFC 2548 */
						/* other vendors ? */
						{
							size_t left;
							struct radius_attr_vendor *vtlv;
							
							left = attr->length - 6;
							vtlv = (struct radius_attr_vendor *)c;
						
							while ((left >= 2) && (vtlv->vendor_length <= left)) {
								/* Search our dictionary for corresponding Vendor's AVP */
								struct dict_avp_request req;
								struct dict_object * avp_model = NULL;
								memset(&req, 0, sizeof(struct dict_avp_request));
								req.avp_vendor = vendor_id;
								req.avp_code = vtlv->vendor_type;
								
								CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_CODE_AND_VENDOR, &req, &avp_model, 0) );
								if (!avp_model) {
									TRACE_DEBUG(FULL, "Unknown attribute (vendor 0x%x, code 0x%x) ignored.", req.avp_vendor, req.avp_code);
								} else {
									CHECK_FCT( fd_msg_avp_new ( avp_model, 0, &avp ) );
									value.os.len = vtlv->vendor_length - 2;
									value.os.data = (unsigned char *)(vtlv + 1);
									CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
									CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
								}
								c += vtlv->vendor_length;
								left -= vtlv->vendor_length;
								vtlv = (struct radius_attr_vendor *)c;
							}
						}
						break;
						
						/* Other vendors we KNOw how to convert the attributes would be added here... */
						/* case RADIUS_VENDOR_ID_CISCO :
							break; */
						/* case RADIUS_VENDOR_ID_IETF : (extended RADIUS attributes)
							break; */
						
						/* When we don't know, just discard the attribute... VSA are optional with regards to RADIUS anyway */
						default:
							/* do nothing */
							TRACE_DEBUG(FULL, "VSA attribute from vendor %d discarded", vendor_id);
							
					}
				}
				break;
				
			/* Session-Timeout never present in an Access-Request */
			/* Idle-Timeout never present in an Access-Request */
			/* Termination-Action never present in an Access-Request */
				
			case RADIUS_ATTR_CALLED_STATION_ID:
				CONV2DIAM_STR( Called_Station_Id );
				break;
			
			case RADIUS_ATTR_CALLING_STATION_ID:
				CONV2DIAM_STR( Calling_Station_Id );
				break;
			
			case RADIUS_ATTR_NAS_IDENTIFIER:
				CONV2DIAM_STR( NAS_Identifier );
				break;
			
			/* Proxy-State is handled by echo_drop.rgwx plugin, we ignore it here */
			
			case RADIUS_ATTR_LOGIN_LAT_SERVICE:
				CONV2DIAM_STR( Login_LAT_Service );
				break;
			
			case RADIUS_ATTR_LOGIN_LAT_NODE:
				CONV2DIAM_STR( Login_LAT_Node );
				break;
			
			case RADIUS_ATTR_LOGIN_LAT_GROUP:
				CONV2DIAM_STR( Login_LAT_Group );
				break;
			
			/* Framed-AppleTalk-Link never present in an Access-Request */
			/* Framed-AppleTalk-Network never present in an Access-Request */
			/* Framed-AppleTalk-Zone never present in an Access-Request */
			
			case RADIUS_ATTR_CHAP_CHALLENGE:
				CONV2DIAM_STR( CHAP_Challenge );
				break;
			
			case RADIUS_ATTR_NAS_PORT_TYPE:
				CONV2DIAM_32B( NAS_Port_Type );
				break;
			
			case RADIUS_ATTR_PORT_LIMIT:
				CONV2DIAM_32B( Port_Limit );
				break;
			
			case RADIUS_ATTR_LOGIN_LAT_PORT:
				CONV2DIAM_STR( Login_LAT_Port );
				break;
			
			
		/* RFC 3162 */	
			case RADIUS_ATTR_NAS_IPV6_ADDRESS:
				CONV2DIAM_STR( NAS_IPv6_Address );
				break;
				
			case RADIUS_ATTR_FRAMED_INTERFACE_ID:
				CONV2DIAM_64B( Framed_Interface_Id );
				break;
				
			case RADIUS_ATTR_FRAMED_IPV6_PREFIX:
				CONV2DIAM_STR( Framed_IPv6_Prefix );
				break;
				
			case RADIUS_ATTR_LOGIN_IPV6_HOST:
				CONV2DIAM_STR( Login_IPv6_Host );
				break;
				
			/* Framed-IPv6-Route never present in an Access-Request */
			/* Framed-IPv6-Pool never present in an Access-Request */


		/* RFC 2868 */
			/* Prepare the top-level Tunneling AVP for each tag values, as needed, and add to the Diameter message.
				This macro is called when an AVP is added inside the group, so we will not have empty grouped AVPs */
			#define AVP_TUN_PREPARE() {										\
						if (avp_tun == NULL) {								\
							CHECK_MALLOC( avp_tun = calloc(sizeof(struct avp *), 32 ) );		\
						}										\
						tag = *(uint8_t *)(attr + 1);							\
						if (tag > 0x1F) tag = 0;							\
						if (avp_tun[tag] == NULL) {							\
							CHECK_FCT( fd_msg_avp_new ( cs->dict.Tunneling, 0, &avp_tun[tag] ) );	\
							CHECK_FCT( fd_msg_avp_add (*diam_fw, MSG_BRW_LAST_CHILD, avp_tun[tag]));\
						}										\
					}
			
			/* Convert an attribute to an OctetString AVP and add inside the Tunneling AVP corresponding to the tag */
			#define CONV2DIAM_TUN_STR( _dictobj_ ) {						\
				uint8_t tag;									\
				CHECK_PARAMS( attr->length >= 3);						\
				AVP_TUN_PREPARE();								\
				CHECK_FCT( fd_msg_avp_new ( cs->dict._dictobj_, 0, &avp ) );			\
				value.os.len = attr->length - (tag ? 3 : 2);					\
				value.os.data = ((unsigned char *)(attr + 1)) + (tag ? 1 : 0);			\
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );				\
				CHECK_FCT( fd_msg_avp_add ( avp_tun[tag], MSG_BRW_LAST_CHILD, avp) );		\
				}
				
			/* Convert an attribute to a scalar AVP and add inside the Tunneling AVP corresponding to the tag */
			#define CONV2DIAM_TUN_24B( _dictobj_ ) {						\
				uint8_t tag;									\
				CHECK_PARAMS( attr->length == 6);						\
				AVP_TUN_PREPARE();								\
				CHECK_FCT( fd_msg_avp_new ( cs->dict._dictobj_, 0, &avp ) );			\
				{										\
					uint8_t * v = (uint8_t *)(attr + 1);					\
					value.u32 = (v[1] << 16) | (v[2] <<8) | v[3] ;				\
				}										\
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );				\
				CHECK_FCT( fd_msg_avp_add ( avp_tun[tag], MSG_BRW_LAST_CHILD, avp) );		\
				}

			/*
			      -  If the RADIUS message contains Tunnel information [RADTunnels],
        			 the attributes or tagged groups should each be converted to a
        			 Diameter Tunneling Grouped AVP set.  If the tunnel information
        			 contains a Tunnel-Password attribute, the RADIUS encryption
        			 must be resolved, and the password forwarded, by using Diameter
        			 security methods.
				    -> If the RADIUS message does not use properly the Tag info, result is unpredictable here.. 
			*/
			case RADIUS_ATTR_TUNNEL_TYPE:
				CONV2DIAM_TUN_24B( Tunnel_Type );
				break;
			
			case RADIUS_ATTR_TUNNEL_MEDIUM_TYPE:
				CONV2DIAM_TUN_24B( Tunnel_Medium_Type );
				break;
			
			case RADIUS_ATTR_TUNNEL_CLIENT_ENDPOINT:
				CONV2DIAM_TUN_STR( Tunnel_Client_Endpoint );
				break;
			
			case RADIUS_ATTR_TUNNEL_SERVER_ENDPOINT:
				CONV2DIAM_TUN_STR( Tunnel_Server_Endpoint );
				break;
			
			/* Tunnel-Password never present in an Access-Request */

			case RADIUS_ATTR_TUNNEL_PRIVATE_GROUP_ID:
				CONV2DIAM_TUN_STR( Tunnel_Private_Group_Id );
				break;
			
			/* Tunnel-Assignment-ID never present in an Access-Request */
			
			case RADIUS_ATTR_TUNNEL_PREFERENCE:
				CONV2DIAM_TUN_24B( Tunnel_Preference );
				break;
			
			case RADIUS_ATTR_TUNNEL_CLIENT_AUTH_ID:
				CONV2DIAM_TUN_STR( Tunnel_Client_Auth_Id );
				break;
			
			case RADIUS_ATTR_TUNNEL_SERVER_AUTH_ID:
				CONV2DIAM_TUN_STR( Tunnel_Server_Auth_Id );
				break;
			
			
		/* RFC 2869 */	
			case RADIUS_ATTR_ARAP_PASSWORD:
				CONV2DIAM_STR( ARAP_Password );
				break;
				
			/* ARAP-Features never present in an Access-Request */
			/* ARAP-Zone-Access never present in an Access-Request */
			
			case RADIUS_ATTR_ARAP_SECURITY:
				CONV2DIAM_32B( ARAP_Security );
				break;
			
			case RADIUS_ATTR_ARAP_SECURITY_DATA:
				CONV2DIAM_STR( ARAP_Security_Data );
				break;
			
			/* Password-Retry never present in an Access-Request */
			/* Prompt never present in an Access-Request */
			
			case RADIUS_ATTR_CONNECT_INFO:
				CONV2DIAM_STR( Connect_Info );
				break;
			
			/* Configuration-Token never present in an Access-Request */
			/* ARAP-Challenge-Response never present in an Access-Request */
			/* Acct-Interim-Interval never present in an Access-Request */
			
			case RADIUS_ATTR_NAS_PORT_ID:
				CONV2DIAM_STR( NAS_Port_Id );
				break;
			
			/* Framed-Pool never present in an Access-Request */
			
				
		/* RFC 2869 / 3579 */	
			case RADIUS_ATTR_ORIGINATING_LINE_INFO:
				CONV2DIAM_STR( Originating_Line_Info );
				break;
				
			case RADIUS_ATTR_MESSAGE_AUTHENTICATOR:
			case RADIUS_ATTR_EAP_MESSAGE:
				/* It was already handled, just remove the attribute */
				break;
				
		/* Default */		
			default: /* unknown attribute */
				/* We just keep the attribute in the RADIUS message */
				rad_req->attr_pos[nattr_used++] = rad_req->attr_pos[idx];
		}
	}
	
	/* Destroy tunnel pointers (if we used it) */
	free(avp_tun);
	
	/* Update the radius message to remove all handled attributes */
	rad_req->attr_used = nattr_used;

	/* Store the request identifier in the session (if provided) */
	{
		struct sess_state  *st;
		CHECK_MALLOC(st = malloc(sizeof(struct sess_state)));
		memcpy(st->req_auth, &rad_req->hdr->authenticator[0], 16);
		
		CHECK_FCT( fd_sess_state_store( cs->sess_hdl, sess, &st ) );
	}
	
	return 0;
}

static int auth_diam_ans( struct rgwp_config * cs, struct msg ** diam_ans, struct radius_msg ** rad_fw, struct rgw_client * cli )
{
	struct msg_hdr * hdr;
	struct avp *avp, *next, *avp_x, *avp_y, *aoh;
	struct avp_hdr *ahdr, *oh;
	uint8_t buf[254]; /* to store some attributes values (with final '\0') */
	size_t sz;
	int ta_set = 0;
	int no_str = 0; /* indicate if an STR is required for this server */
	uint8_t	tuntag = 0;
	struct sess_state  *st;
	int error_cause = 0;
	struct session * sess;
	os0_t sid = NULL;
	size_t sidlen;
	
	TRACE_ENTRY("%p %p %p %p", cs, diam_ans, rad_fw, cli);
	CHECK_PARAMS(cs && diam_ans && *diam_ans && rad_fw && *rad_fw);
	
	/* Retrieve the request identified which was stored in the session */
	CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, *diam_ans, &sess, NULL) );
	if (sess) {
		CHECK_FCT( fd_sess_state_retrieve( cs->sess_hdl, sess, &st ) );
		CHECK_FCT( fd_sess_getsid(sess, &sid, &sidlen) );
	} /* else ? */
	
	/*	
	      -  If the Diameter Command-Code is set to AA-Answer and the
        	 Result-Code AVP is set to DIAMETER_MULTI_ROUND_AUTH, the
        	 gateway must send a RADIUS Access-Challenge.  This must have
        	 the Origin-Host, Origin-Realm, and Diameter Session-Id AVPs
        	 encapsulated in the RADIUS State attribute, with the prefix
        	 "Diameter/", concatenated in the above order separated with "/"
        	 characters, in UTF-8 [UTF-8].  This is necessary to ensure that
        	 the Translation Agent receiving the subsequent RADIUS Access-
        	 Request will have access to the Session Identifier and be able
        	 to set the Destination-Host to the correct value.
		 	-> done here below
		 
	      -  If the Command-Code is set to AA-Answer, the Diameter Session-
        	 Id AVP is saved in a new RADIUS Class attribute whose format
        	 consists of the string "Diameter/" followed by the Diameter
        	 Session Identifier.  This will ensure that the subsequent
        	 Accounting messages, which could be received by any Translation
        	 Agent, would have access to the original Diameter Session
        	 Identifier.
		 	-> done here but only for Access-Accept messages (Result-Code = success)
	 */
	
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
			TRACE_DEBUG(INFO, "[auth.rgwx] AVP truncated in "#_attr_);			\
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

	#define CONV2RAD_64B( _attr_, _data_)	{							\
		uint64_t __v = htonll((uint64_t)(_data_));						\
		TRACE_DEBUG(FULL, "Converting AVP to "#_attr_);						\
		CHECK_MALLOC(radius_msg_add_attr(*rad_fw, (_attr_), (uint8_t *)&__v, sizeof(__v)));	\
	}

	/* Search the different AVPs we handle here */
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Origin_Host, &aoh) );
	CHECK_FCT( fd_msg_avp_hdr ( aoh, &oh ) );

	/* Check the Diameter error code */
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Result_Code, &avp) );
	ASSERT( avp ); /* otherwise the message should have been discarded a lot earlier because of ABNF */
	CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
	switch (ahdr->avp_value->u32) {
		case ER_DIAMETER_MULTI_ROUND_AUTH:
			(*rad_fw)->hdr->code = RADIUS_CODE_ACCESS_CHALLENGE;
			break;
		case ER_DIAMETER_SUCCESS:
		case ER_DIAMETER_LIMITED_SUCCESS:
			(*rad_fw)->hdr->code = RADIUS_CODE_ACCESS_ACCEPT;
			break;
		
		default:
			/* Can we convert the value to a natural Error-Cause ? */
			switch (ahdr->avp_value->u32) {
				case ER_DIAMETER_AVP_UNSUPPORTED:
					error_cause = 401; /* Unsupported Attribute */
					break;
					
				case ER_DIAMETER_MISSING_AVP:
					error_cause = 402; /* Missing Attribute */
					break;
					
				case ER_DIAMETER_UNABLE_TO_COMPLY:
					error_cause = 404; /* Invalid Request */
					break;
					
				case ER_DIAMETER_APPLICATION_UNSUPPORTED:
					error_cause = 405; /* Unsupported Service */
					break;
					
				case ER_DIAMETER_COMMAND_UNSUPPORTED:
					error_cause = 406; /* Unsupported Extension */
					break;
					
				case ER_DIAMETER_INVALID_AVP_VALUE:
					error_cause = 407; /* Invalid Attribute Value */
					break;
					
				case ER_DIAMETER_AVP_NOT_ALLOWED:
					error_cause = 501; /* Administratively Prohibited */
					break;
					
				case ER_DIAMETER_REALM_NOT_SERVED:
				case ER_DIAMETER_LOOP_DETECTED:
				case ER_DIAMETER_UNKNOWN_PEER:
				case ER_DIAMETER_UNABLE_TO_DELIVER:
					error_cause = 502; /* Request Not Routable (Proxy) */
					break;
					
				case ER_DIAMETER_UNKNOWN_SESSION_ID:
					error_cause = 503; /* Session Context Not Found */
					break;
					
				case ER_DIAMETER_TOO_BUSY:
				case ER_DIAMETER_OUT_OF_SPACE:
					error_cause = 506; /* Resources Unavailable */
					break;
					
#if 0
			/* remaining Diameter Result-Code & RADIUS Error-Cause */
				case ER_DIAMETER_REDIRECT_INDICATION:
				case ER_DIAMETER_INVALID_HDR_BITS:
				case ER_DIAMETER_INVALID_AVP_BITS:
				case ER_DIAMETER_AUTHENTICATION_REJECTED:
				case ER_ELECTION_LOST:
				case ER_DIAMETER_AUTHORIZATION_REJECTED:
				case ER_DIAMETER_RESOURCES_EXCEEDED:
				case ER_DIAMETER_CONTRADICTING_AVPS:
				case ER_DIAMETER_AVP_OCCURS_TOO_MANY_TIMES
				case ER_DIAMETER_NO_COMMON_APPLICATION:
				case ER_DIAMETER_UNSUPPORTED_VERSION:
				case ER_DIAMETER_INVALID_BIT_IN_HEADER:
				case ER_DIAMETER_INVALID_AVP_LENGTH:
				case ER_DIAMETER_INVALID_MESSAGE_LENGTH:
				case ER_DIAMETER_INVALID_AVP_BIT_COMBO:
				case ER_DIAMETER_NO_COMMON_SECURITY:
					error_cause = 403; /* NAS Identification Mismatch */
					error_cause = 504; /* Session Context Not Removable */
					error_cause = 505; /* Other Proxy Processing Error */
					error_cause = 507; /* Request Initiated */
					error_cause = 508; /* Multiple Session Selection Unsupported */
#endif /* 0 */
			}
			/* In any case, the following is processed: */
			(*rad_fw)->hdr->code = RADIUS_CODE_ACCESS_REJECT;
			fd_log_debug("[auth.rgwx] Received Diameter answer with error code '%d' from server '%.*s', session %.*s, translating into Access-Reject",
					ahdr->avp_value->u32, 
					(int)oh->avp_value->os.len, oh->avp_value->os.data,
					(int)sidlen, sid);
			CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Error_Message, &avp_x) );
			if (avp_x) {
				CHECK_FCT( fd_msg_avp_hdr ( avp_x, &ahdr ) );
				fd_log_debug("[auth.rgwx]   Error-Message content: '%.*s'",
						(int)ahdr->avp_value->os.len, ahdr->avp_value->os.data);
			}
			CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Error_Reporting_Host, &avp_x) );
			if (avp_x) {
				CHECK_FCT( fd_msg_avp_hdr ( avp_x, &ahdr ) );
				fd_log_debug("[auth.rgwx]   Error-Reporting-Host: '%.*s'",
						(int)ahdr->avp_value->os.len, ahdr->avp_value->os.data);
			}
			CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Failed_AVP, &avp_x) );
			if (avp_x) {
				fd_log_debug("[auth.rgwx]   Failed-AVP was included in the message.");
				/* Dump its content ? */
			}
	}
	/* Remove this Result-Code avp */
	CHECK_FCT( fd_msg_free( avp ) );
	
	/* Creation of the State or Class attribute with session information */
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Origin_Realm, &avp) );
	CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
	
	/* Now, save the session-id and eventually server info in a STATE or CLASS attribute */
	if ((*rad_fw)->hdr->code == RADIUS_CODE_ACCESS_CHALLENGE) {
		if (sizeof(buf) < (sz = snprintf((char *)buf, sizeof(buf), "Diameter/%.*s/%.*s/%.*s", 
				(int)oh->avp_value->os.len,  (char *)oh->avp_value->os.data,
				(int)ahdr->avp_value->os.len,  (char *)ahdr->avp_value->os.data,
				(int)sidlen, (char *)sid))) {
			TRACE_DEBUG(INFO, "Data truncated in State attribute: %s", buf);
		}
		CONV2RAD_STR(RADIUS_ATTR_STATE, buf, sz, 0);
	}

	if ((*rad_fw)->hdr->code == RADIUS_CODE_ACCESS_ACCEPT) {
		/* Add the Session-Id */
		if (sizeof(buf) < (sz = snprintf((char *)buf, sizeof(buf), "Diameter/%.*s", 
				(int)sidlen, sid))) {
			TRACE_DEBUG(INFO, "Data truncated in Class attribute: %s", buf);
		}
		CONV2RAD_STR(RADIUS_ATTR_CLASS, buf, sz, 0);
	}
	
	/* Unlink the Origin-Realm now; the others are unlinked at the end of this function */
	CHECK_FCT( fd_msg_free( avp ) );
	
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Session_Timeout, &avp) );
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Authorization_Lifetime, &avp_x) );
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Re_Auth_Request_Type, &avp_y) );
	/*	
	   When translating a Diameter AA-Answer (with successful result code)
	   to RADIUS Access-Accept that contains a Session-Timeout or
	   Authorization-Lifetime AVP, take the following steps:
	   
	      -  If the Diameter message contains a Session-Timeout AVP but no
        	 Authorization-Lifetime AVP, translate it to a Session-Timeout
        	 attribute (not a Termination-Action).
	*/
	if ((avp != NULL) && (avp_x == NULL)) {
		CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
		CONV2RAD_32B( RADIUS_ATTR_SESSION_TIMEOUT, ahdr->avp_value->u32 );
	}
	
	/*	
	      -  If the Diameter message contains an Authorization-Lifetime AVP
        	 but no Session-Timeout AVP, translate it to a Session-Timeout
        	 attribute and a Termination-Action set to AA-REQUEST.  (Remove
        	 Authorization-Lifetime and Re-Auth-Request-Type.)
	*/
	if ((avp == NULL) && (avp_x != NULL)) {
		CHECK_FCT( fd_msg_avp_hdr ( avp_x, &ahdr ) );
		CONV2RAD_32B( RADIUS_ATTR_SESSION_TIMEOUT, ahdr->avp_value->u32 );
		CONV2RAD_32B( RADIUS_ATTR_TERMINATION_ACTION, RADIUS_TERMINATION_ACTION_RADIUS_REQUEST );
		ta_set = 1;
	}
	
	/*	
	      -  If the Diameter message has both, the Session-Timeout must be
        	 greater than or equal to the Authorization-Lifetime (required
        	 by [BASE]).  Translate it to a Session-Timeout value (with
        	 value from Authorization-Lifetime AVP, the smaller one) and
        	 with the Termination-Action set to AA-REQUEST.  (Remove the
        	 Authorization-Lifetime and Re-Auth-Request-Type.)
	*/
	if ((avp != NULL) && (avp_x != NULL)) {
		CHECK_FCT( fd_msg_avp_hdr ( avp_x, &ahdr ) );
		CONV2RAD_32B( RADIUS_ATTR_SESSION_TIMEOUT, ahdr->avp_value->u32 );
		CONV2RAD_32B( RADIUS_ATTR_TERMINATION_ACTION, RADIUS_TERMINATION_ACTION_RADIUS_REQUEST );
		ta_set = 1;
	}
	
	/*  -> Not too sure about Auth-Grace-Period... we'll just discard it for now */
	
	if (avp) {
		CHECK_FCT( fd_msg_free( avp ) );
	}
	if (avp_x) {
		CHECK_FCT( fd_msg_free( avp_x ) );
	}
	if (avp_y) {
		CHECK_FCT( fd_msg_free( avp_y ) );
	}
	
	
	/*
	      -  If a Proxy-State attribute was present in the RADIUS request,
        	 the same attribute is added in the response.  This information
        	 may be found in the Proxy-Info AVP group, or in a local state
        	 table.
		 	-> handled by sub_echo_drop

	      -  If state information regarding the RADIUS request was saved in
        	 a Proxy-Info AVP or local state table, the RADIUS Identifier
        	 and UDP IP Address and port number are extracted and used in
        	 issuing the RADIUS reply.
		 	-> was saved with the full request
	*/
	
	
	/* Now loop in the list of AVPs and convert those that we know how */
	CHECK_FCT( fd_msg_browse(*diam_ans, MSG_BRW_FIRST_CHILD, &next, NULL) );
	
	while (next) {
		int handled = 1;
		avp = next;
		CHECK_FCT( fd_msg_browse(avp, MSG_BRW_NEXT, &next, NULL) );
		
		CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
		
		if (!(ahdr->avp_flags & AVP_FLAG_VENDOR)) {
			switch (ahdr->avp_code) {
		/* In case of Diameter error, include the Reply-Message attribute */
				case DIAM_ATTR_ERROR_MESSAGE:
					CONV2RAD_STR(RADIUS_ATTR_REPLY_MESSAGE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 2);
					break;
					
				case DIAM_ATTR_ERROR_REPORTING_HOST:
					{
						char buf[254];
						int bsz = snprintf(buf, sizeof(buf), "Error-Reporting-Host: %*s", (int)(ahdr->avp_value->os.len), ahdr->avp_value->os.data);
						CONV2RAD_STR(RADIUS_ATTR_REPLY_MESSAGE, (uint8_t *)buf, bsz, 2);
					}
					break;
				
				case DIAM_ATTR_FAILED_AVP:
					{
						struct avp * favp;
						CHECK_FCT( fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &favp, NULL) );
						if (favp) {
							char buf[254];
							int bsz;
							struct dict_object * favp_model;
							
							CHECK_FCT( fd_msg_model(favp, &favp_model) );
							if (favp_model) {
								struct dict_avp_data fadata;
								CHECK_FCT( fd_dict_getval(favp_model, &fadata) );
								bsz = snprintf(buf, sizeof(buf), "Failed-AVP: %s", fadata.avp_name);
							} else {
								struct avp_hdr * favp_hdr;
								CHECK_FCT( fd_msg_avp_hdr ( favp, &favp_hdr ) );
								bsz = snprintf(buf, sizeof(buf), "Failed-AVP: code %u, vendor %u", favp_hdr->avp_code, favp_hdr->avp_vendor);
							}
							CONV2RAD_STR(RADIUS_ATTR_REPLY_MESSAGE, (uint8_t *)buf, bsz, 2);
						}
					}
					break;
					
		/* RFC 4005 (AVP in the order of the AA-Request/Answer AVP Table) */
				case DIAM_ATTR_ACCT_INTERIM_INTERVAL:
					CONV2RAD_32B(RADIUS_ATTR_ACCT_INTERIM_INTERVAL, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_ARAP_CHALLENGE_RESPONSE:
					CONV2RAD_STR(RADIUS_ATTR_ARAP_CHALLENGE_RESPONSE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					break;
					
				case DIAM_ATTR_ARAP_FEATURES:
					CONV2RAD_STR(RADIUS_ATTR_ARAP_FEATURES, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					break;
					
				/* ARAP-Password is not present in answers */
					
				case DIAM_ATTR_ARAP_SECURITY:
					CONV2RAD_32B(RADIUS_ATTR_ARAP_SECURITY, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_ARAP_SECURITY_DATA:
					CONV2RAD_STR(RADIUS_ATTR_ARAP_SECURITY_DATA, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 2);
					break;
					
				case DIAM_ATTR_ARAP_ZONE_ACCESS:
					CONV2RAD_32B(RADIUS_ATTR_ARAP_ZONE_ACCESS, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_AUTH_APPLICATION_ID:
					/* We just remove this AVP */
					break;
					
				case DIAM_ATTR_AUTH_GRACE_PERIOD:
					/* We just remove this AVP (?) */
					break;
				
				case DIAM_ATTR_AUTH_REQUEST_TYPE:
					/* We only check the value */
					if (ahdr->avp_value->u32 != 3) {
						fd_log_debug("[auth.rgwx] Received Diameter answer with Auth-Request-Type set to %d (%s) from server %.*s, session %.*s."
								"  This may cause interoperability problems with RADIUS.",
								ahdr->avp_value->u32,
								(ahdr->avp_value->u32 == 1) ? "AUTHENTICATE_ONLY" :
									((ahdr->avp_value->u32 == 2) ? "AUTHORIZE_ONLY" : "???"),
								(int)oh->avp_value->os.len, oh->avp_value->os.data, 
								(int)sidlen, sid);
					}
					break;
				
				case DIAM_ATTR_AUTH_SESSION_STATE:
					if ((!ta_set) && (ahdr->avp_value->u32 == ACV_ASS_STATE_MAINTAINED)) {
						CONV2RAD_32B( RADIUS_ATTR_TERMINATION_ACTION, RADIUS_TERMINATION_ACTION_RADIUS_REQUEST );
					}
					
					if (ahdr->avp_value->u32 == ACV_ASS_NO_STATE_MAINTAINED) {
						no_str = 1;
					}
					break;
					
				/* Authorization-Lifetime already handled */
				
				case DIAM_ATTR_CALLBACK_ID:
					CONV2RAD_STR(RADIUS_ATTR_CALLBACK_ID, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
				
				case DIAM_ATTR_CALLBACK_NUMBER:
					CONV2RAD_STR(RADIUS_ATTR_CALLBACK_NUMBER, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
				
				/* Called-Station-Id is not present in answers */
				/* Calling-Station-Id is not present in answers */
				/* CHAP-Auth is not present in answers */
				/* CHAP-Challenge is not present in answers */
					
				case DIAM_ATTR_CLASS:
					CONV2RAD_STR(RADIUS_ATTR_CLASS, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 2);
					break;
				
				case DIAM_ATTR_CONFIGURATION_TOKEN:
					/* We might as well remove it since it's not supposed to be sent to the NAS... */
					CONV2RAD_STR(RADIUS_ATTR_CONFIGURATION_TOKEN, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 2);
					break;
				
				/* Connect-Info is not present in answers */
				
				case DIAM_ATTR_FILTER_ID:
					CONV2RAD_STR(RADIUS_ATTR_FILTER_ID, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 2);
					break;
					
				case DIAM_ATTR_FRAMED_APPLETALK_LINK:
					CONV2RAD_32B(RADIUS_ATTR_FRAMED_APPLETALK_LINK, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_FRAMED_APPLETALK_NETWORK:
					CONV2RAD_32B(RADIUS_ATTR_FRAMED_APPLETALK_NETWORK, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_FRAMED_APPLETALK_ZONE:
					CONV2RAD_STR(RADIUS_ATTR_FRAMED_APPLETALK_ZONE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_FRAMED_COMPRESSION:
					CONV2RAD_32B(RADIUS_ATTR_FRAMED_COMPRESSION,  ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_FRAMED_INTERFACE_ID:
					CONV2RAD_64B(RADIUS_ATTR_FRAMED_INTERFACE_ID,  ahdr->avp_value->u64);
					break;
					
				case DIAM_ATTR_FRAMED_IP_ADDRESS:
					CONV2RAD_STR(RADIUS_ATTR_FRAMED_IP_ADDRESS,  ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					break;
					
				case DIAM_ATTR_FRAMED_IP_NETMASK:
					CONV2RAD_STR(RADIUS_ATTR_FRAMED_IP_NETMASK, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					break;
					
				case DIAM_ATTR_FRAMED_IPV6_PREFIX:
					CONV2RAD_STR(RADIUS_ATTR_FRAMED_IPV6_PREFIX, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					break;
					
				case DIAM_ATTR_FRAMED_IPV6_POOL:
					CONV2RAD_STR(RADIUS_ATTR_FRAMED_IPV6_POOL, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_FRAMED_IPV6_ROUTE:
					CONV2RAD_STR(RADIUS_ATTR_FRAMED_IPV6_ROUTE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_FRAMED_IPX_NETWORK:
					CONV2RAD_32B(RADIUS_ATTR_FRAMED_IPX_NETWORK, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_FRAMED_MTU:
					CONV2RAD_32B(RADIUS_ATTR_FRAMED_MTU, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_FRAMED_POOL:
					CONV2RAD_STR(RADIUS_ATTR_FRAMED_POOL, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_FRAMED_PROTOCOL:
					CONV2RAD_32B(RADIUS_ATTR_FRAMED_PROTOCOL, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_FRAMED_ROUTE:
					CONV2RAD_STR(RADIUS_ATTR_FRAMED_ROUTE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_FRAMED_ROUTING:
					CONV2RAD_32B(RADIUS_ATTR_FRAMED_ROUTING, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_IDLE_TIMEOUT:
					CONV2RAD_32B(RADIUS_ATTR_IDLE_TIMEOUT, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_LOGIN_IP_HOST:
					CONV2RAD_STR(RADIUS_ATTR_LOGIN_IP_HOST, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					break;
					
				case DIAM_ATTR_LOGIN_IPV6_HOST:
					CONV2RAD_STR(RADIUS_ATTR_LOGIN_IPV6_HOST, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0);
					break;
					
				case DIAM_ATTR_LOGIN_LAT_GROUP:
					CONV2RAD_STR(RADIUS_ATTR_LOGIN_LAT_GROUP, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_LOGIN_LAT_NODE:
					CONV2RAD_STR(RADIUS_ATTR_LOGIN_LAT_NODE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_LOGIN_LAT_PORT:
					CONV2RAD_STR(RADIUS_ATTR_LOGIN_LAT_PORT, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_LOGIN_LAT_SERVICE:
					CONV2RAD_STR(RADIUS_ATTR_LOGIN_LAT_SERVICE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
					
				case DIAM_ATTR_LOGIN_SERVICE:
					CONV2RAD_32B(RADIUS_ATTR_LOGIN_SERVICE, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_LOGIN_TCP_PORT:
					CONV2RAD_32B(RADIUS_ATTR_LOGIN_TCP_PORT, ahdr->avp_value->u32);
					break;
					
			/*
			      -							        If the
        			 Multi-Round-Time-Out AVP is present, the value of the AVP MUST
        			 be inserted in the RADIUS Session-Timeout AVP.

			      o  As described in [NASREQ], if the Result-Code AVP set to
				 DIAMETER_MULTI_ROUND_AUTH and the Multi-Round-Time-Out AVP is
				 present, it is translated to the RADIUS Session-Timeout attribute.
			*/
				case DIAM_ATTR_MULTI_ROUND_TIMEOUT:
					CONV2RAD_32B(RADIUS_ATTR_SESSION_TIMEOUT, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_NAS_FILTER_RULE:
					/* This is not translatable to RADIUS */
					fd_log_debug("[auth.rgwx] Received Diameter answer with non-translatable NAS-Filter-Rule AVP from '%.*s' (session: '%.*s'), ignoring.",
							(int)oh->avp_value->os.len, oh->avp_value->os.data,
							(int)sidlen, sid);
					handled = 0;
					break;
					
				/* NAS-Identifier is not present in answers */
				/* NAS-IP-Address is not present in answers */
				/* NAS-IPv6-Address is not present in answers */
				/* NAS-Port is not present in answers */
				/* NAS-Port-Id is not present in answers */
				/* NAS-Port-Type is not present in answers */
				
				case DIAM_ATTR_ORIGIN_AAA_PROTOCOL:
					/* We just remove this AVP */
					break;
					
				/* Originating-Line-Info is not present in answers */
				
				case DIAM_ATTR_PASSWORD_RETRY:
					CONV2RAD_32B(RADIUS_ATTR_PASSWORD_RETRY, ahdr->avp_value->u32);
					break;
				
				case DIAM_ATTR_PORT_LIMIT:
					CONV2RAD_32B(RADIUS_ATTR_PORT_LIMIT, ahdr->avp_value->u32);
					break;
				
				case DIAM_ATTR_PROMPT:
					CONV2RAD_32B(RADIUS_ATTR_PROMPT, ahdr->avp_value->u32);
					break;
					
				case DIAM_ATTR_QOS_FILTER_RULE:
					/* This is not translatable to RADIUS */
					fd_log_debug("[auth.rgwx] Received Diameter answer with non-translatable QoS-Filter-Rule AVP from '%.*s' (session: '%.*s'), ignoring.",
							(int)oh->avp_value->os.len, oh->avp_value->os.data,
							(int)sidlen, sid);
					handled = 0;
					break;
					
				/* Re-Auth-Request-Type already handled */
				
				case DIAM_ATTR_REPLY_MESSAGE:
					CONV2RAD_STR(RADIUS_ATTR_REPLY_MESSAGE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 2);
					break;
					
				case DIAM_ATTR_SERVICE_TYPE:
					CONV2RAD_32B(RADIUS_ATTR_SERVICE_TYPE, ahdr->avp_value->u32);
					break;
				
				case DIAM_ATTR_STATE:
					CONV2RAD_STR(RADIUS_ATTR_STATE, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 2);
					break;
					
				case DIAM_ATTR_TUNNELING:
					{
#define CONV2RAD_TUN_STR( _attr_, _data_, _len_, _trunc_)	{				\
	size_t __l = (size_t)(_len_);								\
	size_t __w = (__l > 252) ? 252 : __l;							\
	size_t __off = 0;									\
	if ((_trunc_) == 0) {									\
		CHECK_PARAMS( __l <= 252 );							\
	}											\
	if ((__l > 252) && (_trunc_ == 1)) {							\
		TRACE_DEBUG(FULL, "Attribute truncated!");					\
		__l = 252;									\
	}											\
	buf[0] = tuntag;									\
	memcpy(&buf[1], (_data_), __w);								\
	CHECK_MALLOC(radius_msg_add_attr(*rad_fw, (_attr_), &buf[0], __w + 1));			\
	while (__l -= __w) {									\
		__off += __w;									\
		__w = (__l > 253) ? 253 : __l;							\
		CHECK_MALLOC(radius_msg_add_attr(*rad_fw, (_attr_), (_data_) + __off, __w));	\
	}											\
}

#define CONV2RAD_TUN_32B( _attr_, _data_)	{						\
	uint32_t __v = htonl((uint32_t)(_data_) | (tuntag << 24));				\
	CHECK_MALLOC(radius_msg_add_attr(*rad_fw, (_attr_), (uint8_t *)&__v, sizeof(__v)));	\
}
						struct avp *inavp, *innext;
						tuntag++;
						CHECK_FCT( fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &innext, NULL) );
						while (innext) {
							inavp = innext;
							CHECK_FCT( fd_msg_browse(inavp, MSG_BRW_NEXT, &innext, NULL) );
							CHECK_FCT( fd_msg_avp_hdr ( inavp, &ahdr ) );
							
							if ( ! (ahdr->avp_flags & AVP_FLAG_VENDOR)) {
								switch (ahdr->avp_code) {
									case DIAM_ATTR_TUNNEL_TYPE:
										CONV2RAD_TUN_32B( RADIUS_ATTR_TUNNEL_TYPE, ahdr->avp_value->u32);
										break;
										
									case DIAM_ATTR_TUNNEL_MEDIUM_TYPE:
										CONV2RAD_TUN_32B( RADIUS_ATTR_TUNNEL_MEDIUM_TYPE, ahdr->avp_value->u32);
										break;
										
									case DIAM_ATTR_TUNNEL_CLIENT_ENDPOINT:
										CONV2RAD_TUN_STR(RADIUS_ATTR_TUNNEL_CLIENT_ENDPOINT, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
										break;
										
									case DIAM_ATTR_TUNNEL_SERVER_ENDPOINT:
										CONV2RAD_TUN_STR(RADIUS_ATTR_TUNNEL_SERVER_ENDPOINT, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
										break;
										
									case DIAM_ATTR_TUNNEL_PREFERENCE:
										CONV2RAD_TUN_32B( RADIUS_ATTR_TUNNEL_PREFERENCE, ahdr->avp_value->u32);
										break;
										
									case DIAM_ATTR_TUNNEL_CLIENT_AUTH_ID:
										CONV2RAD_TUN_STR(RADIUS_ATTR_TUNNEL_CLIENT_AUTH_ID, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
										break;
										
									case DIAM_ATTR_TUNNEL_SERVER_AUTH_ID:
										CONV2RAD_TUN_STR(RADIUS_ATTR_TUNNEL_SERVER_AUTH_ID, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
										break;
										
									case DIAM_ATTR_TUNNEL_ASSIGNMENT_ID:
										CONV2RAD_TUN_STR(RADIUS_ATTR_TUNNEL_ASSIGNMENT_ID, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
										break;
										
									case DIAM_ATTR_TUNNEL_PASSWORD:
										{
											/* This AVP must be encoded for RADIUS (similar to radius_msg_add_attr_user_password)
											    0                   1                   2                   3
											    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
											   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
											   |     Type      |    Length     |     Tag       |   Salt
											   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
											      Salt (cont)  |   String ...
											   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
											*/
											size_t pos;
											int i;
											uint8_t * secret;	/* S */
											size_t secret_len;
											uint8_t hash[16];	/* b(i) */
											const uint8_t *addr[3];
											size_t len[3];
											
											/* We need the request authenticator */
											CHECK_PARAMS(st);

											/* Retrieve the shared secret */
											CHECK_FCT(rgw_clients_getkey(cli, &secret, &secret_len));
											
											/* Beginning of the buffer */
											buf[0] = tuntag;
											buf[1] = (uint8_t)(lrand48()); /* A (hi bits) */
											buf[2] = (uint8_t)(lrand48()); /* A (low bits) */
											
											/* The plain text string P */
											CHECK_PARAMS(ahdr->avp_value->os.len < 240);
											buf[3] = ahdr->avp_value->os.len;
											memcpy(&buf[4], ahdr->avp_value->os.data, ahdr->avp_value->os.len);
											memset(&buf[4 + ahdr->avp_value->os.len], 0, sizeof(buf) - 4 - ahdr->avp_value->os.len);
											
											/* Initial b1 = MD5(S + R + A) */
											addr[0] = secret;
											len[0] = secret_len;
											addr[1] = st->req_auth;
											len[1] = 16;
											addr[2] = &buf[1];
											len[2] = 2;
											md5_vector(3, addr, len, hash);
											
											/* Initial c(1) = p(1) xor b(1) */
											for (i = 0; i < 16; i++) {
												buf[i + 3] ^= hash[i];
											}
											pos = 16;
											
											/* loop */
											while (pos < ahdr->avp_value->os.len + 1) {
												addr[0] = secret;
												len[0] = secret_len;
												addr[1] = &buf[pos - 13];
												len[1] = 16;
												/* b(i) = MD5( S + c(i-1) */
												md5_vector(2, addr, len, hash);
												
												/* c(i) = p(i) xor b(i) */
												for (i = 0; i < 16; i++)
													buf[pos + i + 3] ^= hash[i];

												pos += 16;
											}
											
											CONV2RAD_STR(RADIUS_ATTR_TUNNEL_PASSWORD, &buf[0], pos + 3, 0);
										}
										break;
										
									case DIAM_ATTR_TUNNEL_PRIVATE_GROUP_ID:
										CONV2RAD_TUN_STR(RADIUS_ATTR_TUNNEL_PRIVATE_GROUP_ID, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
										break;
									
									default:
										TRACE_DEBUG(FULL, "Ignored unknown AVP inside Tunneling AVP (%d)", ahdr->avp_code);
								}
							} else {
								TRACE_DEBUG(FULL, "Ignored unknown Vendor AVP inside Tunneling AVP (%d, %d)", ahdr->avp_vendor, ahdr->avp_code);
							}
						}
					}
					break;
					
				case DIAM_ATTR_USER_NAME:
					CONV2RAD_STR(RADIUS_ATTR_USER_NAME, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
				
				/* User-Password never present in answers */
					
		/* RFC 4072 (AVP in the order of the EAP Command AVP Table) */
			/*
			      o  Diameter Accounting-EAP-Auth-Method AVPs, if present, are
				 discarded.
			*/
				case DIAM_ATTR_ACCOUNTING_EAP_AUTH_METHOD:
					break;
					
			/*
			      o  Diameter EAP-Master-Session-Key AVP can be translated to the
				 vendor-specific RADIUS MS-MPPE-Recv-Key and MS-MPPE-Send-Key
				 attributes [RFC2548].  The first up to 32 octets of the key is
				 stored into MS-MPPE-Recv-Key, and the next up to 32 octets (if
				 present) are stored into MS-MPPE-Send-Key.  The encryption of this
				 attribute is described in [RFC2548].
			*/
				case DIAM_ATTR_EAP_MASTER_SESSION_KEY:
					{
						uint8_t * secret;	/* S */
						size_t secret_len;
						size_t recv_len, send_len;

						/* We need the request authenticator */
						CHECK_PARAMS(st);

						/* Retrieve the shared secret */
						CHECK_FCT(rgw_clients_getkey(cli, &secret, &secret_len));
						
						if (ahdr->avp_value->os.len != 64) {
							TRACE_DEBUG(INFO, "Received EAP-Master-Session-Key attribute with length %zd != 64.", ahdr->avp_value->os.len)
						}
						
						CHECK_PARAMS(ahdr->avp_value->os.len <= 64);
						recv_len = ahdr->avp_value->os.len >= 32 ? 32 : ahdr->avp_value->os.len;
						send_len = ahdr->avp_value->os.len - recv_len;
						
						if ( ! radius_msg_add_mppe_keys(*rad_fw, st->req_auth, secret, secret_len, 
								ahdr->avp_value->os.data + recv_len, send_len,
								ahdr->avp_value->os.data, recv_len) ) {
							TRACE_DEBUG(INFO, "Error while converting EAP-Master-Session-Key to RADIUS message");
							return ENOMEM;
						}
					}
					break;
				
				case DIAM_ATTR_EAP_KEY_NAME:
					CONV2RAD_STR(RADIUS_ATTR_EAP_KEY_NAME, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 1);
					break;
				
			/*
			      o  Diameter EAP-Payload AVP is translated to RADIUS EAP-Message
				 attribute(s).  If necessary, the value is split into multiple
				 RADIUS EAP-Message attributes.
			*/
				case DIAM_ATTR_EAP_PAYLOAD:
					if ( ! radius_msg_add_eap(*rad_fw, ahdr->avp_value->os.data, ahdr->avp_value->os.len) ) {
						TRACE_DEBUG(INFO, "Error while converting EAP payload to RADIUS message");
						return ENOMEM;
					}
					break;
					
			/*
			      o  Diameter EAP-Reissued-Payload AVP is translated to a message that
				 contains RADIUS EAP-Message attribute(s), and a RADIUS Error-Cause
				 attribute [RFC3576] with value 202 (decimal), "Invalid EAP Packet
				 (Ignored)" [RFC3579].
			*/
				case DIAM_ATTR_EAP_REISSUED_PAYLOAD:
					if ( ! radius_msg_add_eap(*rad_fw, ahdr->avp_value->os.data, ahdr->avp_value->os.len) ) {
						TRACE_DEBUG(INFO, "Error while converting EAP reissued payload to RADIUS message");
						return ENOMEM;
					}
					
					error_cause = 202; /* Invalid EAP Packet */
					break;
			
				default:
					/* Leave the AVP in the message for further treatment */
					handled = 0;
			}
		} else {
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
	
	CHECK_FCT( fd_msg_free( aoh ) );
	free(st);
	
	if (error_cause) {
		if ( ! radius_msg_add_attr_int32(*rad_fw, RADIUS_ATTR_ERROR_CAUSE, error_cause) ) {
			TRACE_DEBUG(INFO, "Error while adding Error-Cause attribute in RADIUS message");
			return ENOMEM;
		}
	}

	if ((*rad_fw)->hdr->code == RADIUS_CODE_ACCESS_ACCEPT) {
		/* Add the auth-application-id required for STR, or 0 if no STR is required */
		CHECK_FCT( fd_msg_hdr( *diam_ans, &hdr ) );
		if (sizeof(buf) < (sz = snprintf((char *)buf, sizeof(buf), CLASS_AAI_PREFIX "%u", 
				no_str ? 0 : hdr->msg_appl))) {
			TRACE_DEBUG(INFO, "Data truncated in Class attribute: %s", buf);
		}
		CONV2RAD_STR(RADIUS_ATTR_CLASS, buf, sz, 0);
	}
	
	return 0;
}

/* The exported symbol */
struct rgw_api rgwp_descriptor = {
	.rgwp_name       = "auth",
	.rgwp_conf_parse = auth_conf_parse,
	.rgwp_conf_free  = auth_conf_free,
	.rgwp_rad_req    = auth_rad_req,
	.rgwp_diam_ans   = auth_diam_ans
};	
