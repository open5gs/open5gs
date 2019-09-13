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

/* RADIUS Accounting-Request messages translation plugin */

#include "rgw_common.h"


/* Other constants we use */
#define AI_ACCT			3	/* Diameter Base Accounting application */
#define CC_AC			271	/* ACR/ACA */
#define ACV_ART_START_RECORD		2	/* START_RECORD */
#define ACV_ART_INTERIM_RECORD		3	/* INTERIM_RECORD */
#define ACV_ART_STOP_RECORD		4	/* STOP_RECORD */
#define ACV_ART_AUTHORIZE_AUTHENTICATE	3	/* AUTHORIZE_AUTHENTICATE */


/* The state we keep for this plugin */
struct rgwp_config {
	struct {
		struct dict_object * Accounting_Record_Number;	/* Accounting-Record-Number */
		struct dict_object * Accounting_Record_Type;	/* Accounting-Record-Type */
		struct dict_object * Acct_Application_Id;	/* Acct-Application-Id */
		struct dict_object * Acct_Delay_Time;		/* Acct-Delay-Time */
		struct dict_object * Accounting_Input_Octets;	/* Accounting-Input-Octets */
		struct dict_object * Accounting_Output_Octets;	/* Accounting-Output-Octets */
		struct dict_object * Accounting_Input_Packets;	/* Accounting-Input-Packets */
		struct dict_object * Accounting_Output_Packets;	/* Accounting-Output-Packets */
		struct dict_object * Acct_Link_Count;		/* Acct-Link-Count */
		struct dict_object * Acct_Authentic;		/* Acct-Authentic */
		struct dict_object * Acct_Multi_Session_Id;	/* Acct-Multi-Session-Id */
		struct dict_object * Acct_Session_Id;		/* Acct-Session-Id */
		struct dict_object * Acct_Session_Time;		/* Acct-Session-Time */
		
		struct dict_object * ARAP_Password;		/* ARAP-Password */
		struct dict_object * ARAP_Security;		/* ARAP-Security */
		struct dict_object * ARAP_Security_Data;	/* ARAP-Security-Data */
		struct dict_object * Auth_Application_Id;	/* Auth-Application-Id */
		struct dict_object * Auth_Request_Type;		/* Auth-Request-Type */
		struct dict_object * Authorization_Lifetime;	/* Authorization-Lifetime */
		struct dict_object * Callback_Number;		/* Callback-Number */
		struct dict_object * Callback_Id;		/* Callback-Id */
		struct dict_object * Called_Station_Id;		/* Called-Station-Id */
		struct dict_object * Calling_Station_Id;	/* Calling-Station-Id */
		struct dict_object * Class;			/* Class */
		struct dict_object * CHAP_Algorithm;		/* CHAP-Algorithm */
		struct dict_object * CHAP_Auth;			/* CHAP-Auth */
		struct dict_object * CHAP_Challenge;		/* CHAP-Challenge */
		struct dict_object * CHAP_Ident;		/* CHAP-Ident */
		struct dict_object * CHAP_Response;		/* CHAP-Response */
		struct dict_object * Connect_Info;		/* Connect-Info */
		struct dict_object * Destination_Host;		/* Destination-Host */
		struct dict_object * Destination_Realm;		/* Destination-Realm */
		struct dict_object * EAP_Payload;		/* EAP-Payload */
		struct dict_object * Error_Message;		/* Error-Message */
		struct dict_object * Error_Reporting_Host;	/* Error-Reporting-Host */
		struct dict_object * Event_Timestamp;		/* Event-Timestamp */
		struct dict_object * Failed_AVP;		/* Failed-AVP */
		struct dict_object * Framed_AppleTalk_Link;	/* Framed-AppleTalk-Link */
		struct dict_object * Framed_AppleTalk_Network;	/* Framed-AppleTalk-Network */
		struct dict_object * Framed_AppleTalk_Zone;	/* Framed-AppleTalk-Zone */
		struct dict_object * Framed_Compression;	/* Framed-Compression */
		struct dict_object * Framed_IP_Address;		/* Framed-IP-Address */
		struct dict_object * Framed_IP_Netmask;		/* Framed-IP-Netmask */
		struct dict_object * Framed_Interface_Id;	/* Framed-Interface-Id */
		struct dict_object * Framed_IPv6_Prefix;	/* Framed-IPv6-Prefix */
		struct dict_object * Framed_IPX_Network;	/* Framed-IPX-Network */
		struct dict_object * Framed_MTU;		/* Framed-MTU */
		struct dict_object * Framed_Protocol;		/* Framed-Protocol */
		struct dict_object * Framed_Pool;		/* Framed-Pool */
		struct dict_object * Framed_IPv6_Route;		/* Framed-IPv6-Route */
		struct dict_object * Framed_IPv6_Pool;		/* Framed-IPv6-Pool */
		struct dict_object * Framed_Route;		/* Framed-Route */
		struct dict_object * Framed_Routing;		/* Framed-Routing */
		struct dict_object * Filter_Id;			/* Filter-Id */
		struct dict_object * Idle_Timeout;		/* Idle-Timeout */
		struct dict_object * Login_IP_Host;		/* Login-IP-Host */
		struct dict_object * Login_IPv6_Host;		/* Login-IPv6-Host */
		struct dict_object * Login_LAT_Group;		/* Login-LAT-Group */
		struct dict_object * Login_LAT_Node;		/* Login-LAT-Node */
		struct dict_object * Login_LAT_Port;		/* Login-LAT-Port */
		struct dict_object * Login_LAT_Service;		/* Login-LAT-Service */
		struct dict_object * Login_Service;		/* Login-Service */
		struct dict_object * Login_TCP_Port;		/* Login-TCP-Port */
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
		struct dict_object * Termination_Cause;		/* Termination-Cause */
		struct dict_object * Tunneling;			/* Tunneling */
		struct dict_object * Tunnel_Type;		/* Tunnel-Type */
		struct dict_object * Tunnel_Assignment_Id;	/* Tunnel-Assignment-Id */
		struct dict_object * Tunnel_Medium_Type;	/* Tunnel-Medium-Type */
		struct dict_object * Tunnel_Client_Endpoint;	/* Tunnel-Client-Endpoint */
		struct dict_object * Tunnel_Server_Endpoint;	/* Tunnel-Server-Endpoint */
		struct dict_object * Tunnel_Private_Group_Id;	/* Tunnel-Private-Group-Id */
		struct dict_object * Tunnel_Preference;		/* Tunnel-Preference */
		struct dict_object * Tunnel_Client_Auth_Id;	/* Tunnel-Client-Auth-Id */
		struct dict_object * Tunnel_Server_Auth_Id;	/* Tunnel-Server-Auth-Id */
		struct dict_object * User_Name;			/* User-Name */
		
		struct dict_object * Session_Termination_Request;/* STR */
	} dict; /* cache of the dictionary objects we use */
	struct session_handler * sess_hdl; /* We store RADIUS request authenticator information in the session */
	char * confstr;
	
	int ignore_nai;
};

/* The state we store in the session */
struct sess_state {
	application_id_t auth_appl;	/* Auth-Application-Id used for this session, if available (stored in a Class attribute) */
	int		 send_str;	/* If not 0, we must send a STR when the ACA is received. */
	uint32_t	 term_cause;	/* If not 0, the Termination-Cause to put in the STR. */
};

static DECLARE_FD_DUMP_PROTOTYPE(acct_conf_session_state_dump, struct sess_state * st)
{
	return fd_dump_extend( FD_DUMP_STD_PARAMS, "[rgwx sess_state](@%p): aai:%x str:%d TC:%u", st, st->auth_appl, st->send_str, st->term_cause);
}

/* Initialize the plugin */
static int acct_conf_parse(char * conffile, struct rgwp_config ** state)
{
	struct rgwp_config * new;
	struct dict_object * app;
	
	TRACE_ENTRY("%p %p", conffile, state);
	CHECK_PARAMS( state );
	
	CHECK_MALLOC( new = malloc(sizeof(struct rgwp_config)) );
	memset(new, 0, sizeof(struct rgwp_config));
	
	CHECK_FCT( fd_sess_handler_create( &new->sess_hdl, (void *)free, acct_conf_session_state_dump, NULL ) );
	new->confstr = conffile;
	
	if (conffile && strstr(conffile, "nonai"))
		new->ignore_nai = 1;
	
	/* Resolve all dictionary objects we use */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Record-Number", &new->dict.Accounting_Record_Number, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Record-Type", &new->dict.Accounting_Record_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Acct-Application-Id", &new->dict.Acct_Application_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Acct-Delay-Time", &new->dict.Acct_Delay_Time, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Input-Octets", &new->dict.Accounting_Input_Octets, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Output-Octets", &new->dict.Accounting_Output_Octets, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Input-Packets", &new->dict.Accounting_Input_Packets, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Accounting-Output-Packets", &new->dict.Accounting_Output_Packets, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Acct-Authentic", &new->dict.Acct_Authentic, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Acct-Link-Count", &new->dict.Acct_Link_Count, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Acct-Multi-Session-Id", &new->dict.Acct_Multi_Session_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Acct-Session-Id", &new->dict.Acct_Session_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Acct-Session-Time", &new->dict.Acct_Session_Time, ENOENT) );
	
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "ARAP-Password", &new->dict.ARAP_Password, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "ARAP-Security", &new->dict.ARAP_Security, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "ARAP-Security-Data", &new->dict.ARAP_Security_Data, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Application-Id", &new->dict.Auth_Application_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Request-Type", &new->dict.Auth_Request_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Authorization-Lifetime", &new->dict.Authorization_Lifetime, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Callback-Number", &new->dict.Callback_Number, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Callback-Id", &new->dict.Callback_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Called-Station-Id", &new->dict.Called_Station_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Calling-Station-Id", &new->dict.Calling_Station_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Class", &new->dict.Class, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Connect-Info", &new->dict.Connect_Info, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Host", &new->dict.Destination_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Realm", &new->dict.Destination_Realm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "EAP-Payload", &new->dict.EAP_Payload, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Error-Message", &new->dict.Error_Message, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Error-Reporting-Host", &new->dict.Error_Reporting_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Event-Timestamp", &new->dict.Event_Timestamp, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Failed-AVP", &new->dict.Failed_AVP, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-AppleTalk-Link", &new->dict.Framed_AppleTalk_Link, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-AppleTalk-Network", &new->dict.Framed_AppleTalk_Network, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-AppleTalk-Zone", &new->dict.Framed_AppleTalk_Zone, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Compression", &new->dict.Framed_Compression, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IP-Address", &new->dict.Framed_IP_Address, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IP-Netmask", &new->dict.Framed_IP_Netmask, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Interface-Id", &new->dict.Framed_Interface_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IPv6-Prefix", &new->dict.Framed_IPv6_Prefix, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IPX-Network", &new->dict.Framed_IPX_Network, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-MTU", &new->dict.Framed_MTU, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Protocol", &new->dict.Framed_Protocol, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Pool", &new->dict.Framed_Pool, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Route", &new->dict.Framed_Route, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IPv6-Route", &new->dict.Framed_IPv6_Route, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-IPv6-Pool", &new->dict.Framed_IPv6_Pool, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Framed-Routing", &new->dict.Framed_Routing, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Filter-Id", &new->dict.Filter_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Idle-Timeout", &new->dict.Idle_Timeout, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-IP-Host", &new->dict.Login_IP_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-IPv6-Host", &new->dict.Login_IPv6_Host, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-LAT-Group", &new->dict.Login_LAT_Group, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-LAT-Node", &new->dict.Login_LAT_Node, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-LAT-Port", &new->dict.Login_LAT_Port, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-LAT-Service", &new->dict.Login_LAT_Service, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-Service", &new->dict.Login_Service, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Login-TCP-Port", &new->dict.Login_TCP_Port, ENOENT) );
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
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Termination-Cause", &new->dict.Termination_Cause, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunneling", &new->dict.Tunneling, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Assignment-Id", &new->dict.Tunnel_Assignment_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Type", &new->dict.Tunnel_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Medium-Type", &new->dict.Tunnel_Medium_Type, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Client-Endpoint", &new->dict.Tunnel_Client_Endpoint, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Server-Endpoint", &new->dict.Tunnel_Server_Endpoint, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Private-Group-Id", &new->dict.Tunnel_Private_Group_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Preference", &new->dict.Tunnel_Preference, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Client-Auth-Id", &new->dict.Tunnel_Client_Auth_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Tunnel-Server-Auth-Id", &new->dict.Tunnel_Server_Auth_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "User-Name", &new->dict.User_Name, ENOENT) );
	
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Session-Termination-Request", &new->dict.Session_Termination_Request, ENOENT) );
	
	/* This plugin provides the following Diameter authentication applications support: */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Diameter Base Accounting", &app, ENOENT) );
	CHECK_FCT( fd_disp_app_support ( app, NULL, 0, 1 ) );
	
	*state = new;
	return 0;
}

/* deinitialize */
static void acct_conf_free(struct rgwp_config * state)
{
	TRACE_ENTRY("%p", state);
	CHECK_PARAMS_DO( state, return );
	CHECK_FCT_DO( fd_sess_handler_destroy( &state->sess_hdl, NULL ),  );
	free(state);
	return;
}

/* Incoming RADIUS request */
static int acct_rad_req( struct rgwp_config * cs, struct radius_msg * rad_req, struct radius_msg ** rad_ans, struct msg ** diam_fw, struct rgw_client * cli )
{
	int idx;
	int send_str=0;
	uint32_t str_cause=0;
	uint32_t e2eid = 0;
	application_id_t auth_appl=0;
	int got_id = 0;
	uint32_t status_type;
	uint32_t termination_action = 0;
	uint32_t gigawords_in=0, gigawords_out=0;
	size_t nattr_used = 0;
	union avp_value value;
	struct avp ** avp_tun = NULL, *avp = NULL;
	struct session * sess;
	
	const char * prefix = "Diameter/";
	size_t pref_len;
	os0_t si = NULL;
	size_t si_len = 0;
	os0_t un = NULL;
	size_t un_len = 0;
	
	TRACE_ENTRY("%p %p %p %p %p", cs, rad_req, rad_ans, diam_fw, cli);
	CHECK_PARAMS(rad_req && (rad_req->hdr->code == RADIUS_CODE_ACCOUNTING_REQUEST) && rad_ans && diam_fw && *diam_fw);
	
	pref_len = strlen(prefix);
	
	/*
	      Either NAS-IP-Address or NAS-Identifier MUST be present in a
	      RADIUS Accounting-Request.  It SHOULD contain a NAS-Port or NAS-
	      Port-Type attribute or both unless the service does not involve a
	      port or the NAS does not distinguish among its ports.
	*/
	/* We also enforce that the message contains a CLASS attribute with Diameter/ prefix containing the Session-Id. */
	for (idx = 0; idx < rad_req->attr_used; idx++) {
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(rad_req->buf + rad_req->attr_pos[idx]);
		uint8_t * v = (uint8_t *)(attr + 1);
		size_t attr_len = attr->length - sizeof(struct radius_attr_hdr);
		
		switch (attr->type) {
			case RADIUS_ATTR_NAS_IP_ADDRESS:
			case RADIUS_ATTR_NAS_IDENTIFIER:
			case RADIUS_ATTR_NAS_IPV6_ADDRESS:
				got_id = 1;
				break;
				
			case RADIUS_ATTR_TERMINATION_ACTION:
				CHECK_PARAMS( attr->length == 6 );
				termination_action  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
				break;
				
			case RADIUS_ATTR_ACCT_INPUT_GIGAWORDS:
				CHECK_PARAMS( attr->length == 6 );
				gigawords_in  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
				break;
				
			case RADIUS_ATTR_ACCT_OUTPUT_GIGAWORDS:
				CHECK_PARAMS( attr->length == 6 );
				gigawords_out  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
				break;
				
			case RADIUS_ATTR_CLASS:
				if ((attr_len > pref_len ) && ! strncmp((char *)v, prefix, pref_len)) {
					int i;
					si = v + pref_len;
					si_len = attr_len - pref_len;
					TRACE_DEBUG(ANNOYING, "Found Class attribute with '%s' prefix (attr #%d), SI:'%.*s'.", prefix, idx, (int)si_len, si);
					/* Remove from the message */
					for (i = idx + 1; i < rad_req->attr_used; i++)
						rad_req->attr_pos[i - 1] = rad_req->attr_pos[i];
					rad_req->attr_used -= 1;
				}
				break;

			case RADIUS_ATTR_USER_NAME:
				if (attr_len) {
					un = v;
					un_len = attr_len;
					TRACE_DEBUG(ANNOYING, "Found a User-Name attribute: '%.*s'", (int)un_len, un);
				}
				break;
			
		}
	}
	
	/* Check basic information is there */
	if (!got_id || radius_msg_get_attr_int32(rad_req, RADIUS_ATTR_ACCT_STATUS_TYPE, &status_type)) {
		TRACE_DEBUG(INFO, "[acct.rgwx] RADIUS Account-Request from %s did not contain a NAS ip/identifier or Acct-Status-Type attribute, reject.", rgw_clients_id(cli));
		return EINVAL;
	}
	
	/*
	      -- RFC2866:
	      In Accounting-Request Packets, the Authenticator value is a 16
	      octet MD5 [5] checksum, called the Request Authenticator.

	      The NAS and RADIUS accounting server share a secret.  The Request
	      Authenticator field in Accounting-Request packets contains a one-
	      way MD5 hash calculated over a stream of octets consisting of the
	      Code + Identifier + Length + 16 zero octets + request attributes +
	      shared secret (where + indicates concatenation).  The 16 octet MD5
	      hash value is stored in the Authenticator field of the
	      Accounting-Request packet.

	      Note that the Request Authenticator of an Accounting-Request can
	      not be done the same way as the Request Authenticator of a RADIUS
	      Access-Request, because there is no User-Password attribute in an
	      Accounting-Request.
	      
	      
	      -- RFC5080:
	      The Request Authenticator field MUST contain the correct data, as
	      given by the above calculation.  Invalid packets are silently
	      discarded.  Note that some early implementations always set the
	      Request Authenticator to all zeros.  New implementations of RADIUS
	      clients MUST use the above algorithm to calculate the Request
	      Authenticator field.  New RADIUS server implementations MUST
	      silently discard invalid packets.
	      
	*/	
	{
		uint8_t save[MD5_MAC_LEN];
		uint8_t * secret;
		size_t secret_len;
		
		/* Get the shared secret */
		CHECK_FCT(rgw_clients_getkey(cli, &secret, &secret_len));
		
		/* Copy the received Request Authenticator */
		memcpy(&save[0], &rad_req->hdr->authenticator[0], MD5_MAC_LEN);
		
		/* Compute the same authenticator */
		radius_msg_finish_acct(rad_req, secret, secret_len);
		
		/* And now compare with the received value */
		if (memcmp(&save[0], &rad_req->hdr->authenticator[0], MD5_MAC_LEN)) {
			/* Invalid authenticator */
			TRACE_BUFFER(FD_LOG_DEBUG, FULL+1, "Received ReqAuth: ", &save[0], MD5_MAC_LEN, "" );
			TRACE_BUFFER(FD_LOG_DEBUG, FULL+1, "Expected ReqAuth: ", &rad_req->hdr->authenticator[0], MD5_MAC_LEN, "" );
			TRACE_DEBUG(INFO, "[acct.rgwx] Invalid Request Authenticator in Account-Request from %s, discarding the message.", rgw_clients_id(cli));
			return EINVAL;
		}
	}
	
	
	/* Handle the Accounting-On case: nothing to do, just reply OK */
	if (status_type == RADIUS_ACCT_STATUS_TYPE_ACCOUNTING_ON) {
		TRACE_DEBUG(FULL, "[acct.rgwx] Received Accounting-On Acct-Status-Type attribute, replying without translation to Diameter.");
		CHECK_MALLOC( *rad_ans = radius_msg_new(RADIUS_CODE_ACCOUNTING_RESPONSE, rad_req->hdr->identifier) );
		return -2;
	}
	
	if (status_type == RADIUS_ACCT_STATUS_TYPE_ACCOUNTING_OFF) {
		TRACE_DEBUG(FULL, "[acct.rgwx] Received Accounting-Off Acct-Status-Type attribute, we must terminate all active sessions.");
		TODO("RADIUS side is rebooting, send STR on all sessions???");
		return ENOTSUP;
	}
	
	/* Check if we got a valid session information, otherwise the server will not be able to handle the data... */
	if (!si) {
		TRACE_DEBUG(INFO, "[acct.rgwx] RADIUS Account-Request from %s did not contain a CLASS attribute with Diameter session information, reject.", rgw_clients_id(cli));
		return EINVAL;
	}
	
	/* Add the Destination-Realm */
	CHECK_FCT( fd_msg_avp_new ( cs->dict.Destination_Realm, 0, &avp ) );
	idx = 0;
	if (un && ! cs->ignore_nai) {
		/* Is there an '@' in the user name? We don't care for decorated NAI here */
		for (idx = un_len - 2; idx > 0; idx--) {
			if (un[idx] == '@') {
				idx++;
				break;
			}
		}
	}
	if (idx == 0) {
		/* Not found in the User-Name => we use the local domain of this gateway */
		value.os.data = (uint8_t *)fd_g_config->cnf_diamrlm;
		value.os.len  = fd_g_config->cnf_diamrlm_len;
	} else {
		value.os.data = un + idx;
		value.os.len  = un_len - idx;
	}
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
	CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_FIRST_CHILD, avp) );
	
	/* Create the Session-Id AVP */
	{
		CHECK_FCT( fd_sess_fromsid_msg ( si, si_len, &sess, NULL) );
		
		TRACE_DEBUG(FULL, "[acct.rgwx] Translating new accounting message for session '%.*s'...", (int)si_len, si);
		
		/* Add the Session-Id AVP as first AVP */
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Session_Id, 0, &avp ) );
		value.os.data = (unsigned char *)si;
		value.os.len = si_len;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_FIRST_CHILD, avp) );
		CHECK_FCT( fd_msg_sess_set(*diam_fw, sess) );
	}
	
		
	/* Add the command code */
	{
		struct msg_hdr * header = NULL;
		CHECK_FCT( fd_msg_hdr ( *diam_fw, &header ) );
		header->msg_flags = CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE;
		header->msg_code = CC_AC;
		header->msg_appl = AI_ACCT;
		
		/* Add the Acct-Application-Id */
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Acct_Application_Id, 0, &avp ) );
		value.i32 = header->msg_appl;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
		
		/* save the end to end id */
		e2eid = header->msg_eteid;
	}
	
	/* Convert the RADIUS attributes, as they appear in the message */
	for (idx = 0; idx < rad_req->attr_used; idx++) {
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(rad_req->buf + rad_req->attr_pos[idx]);

		switch (attr->type) {
			/*	
			      Any attribute valid in a RADIUS Access-Request or Access-Accept
			      packet is valid in a RADIUS Accounting-Request packet, except that
			      the following attributes MUST NOT be present in an Accounting-
			      Request:  User-Password, CHAP-Password, Reply-Message, State.
			*/
			case RADIUS_ATTR_USER_PASSWORD:
			case RADIUS_ATTR_CHAP_PASSWORD:
			case RADIUS_ATTR_REPLY_MESSAGE:
			case RADIUS_ATTR_STATE:
			case RADIUS_ATTR_MESSAGE_AUTHENTICATOR:
			case RADIUS_ATTR_EAP_MESSAGE:
				TRACE_DEBUG(INFO, "[acct.rgwx] RADIUS Account-Request contains a forbidden attribute (%hhd), reject.", attr->type);
				return EINVAL;
				
			
			/* This macro converts a RADIUS attribute to a Diameter AVP of type OctetString */
			#define CONV2DIAM_STR( _dictobj_ )	\
				CHECK_PARAMS( attr->length >= 2 );						\
				/* Create the AVP with the specified dictionary model */			\
				CHECK_FCT( fd_msg_avp_new ( cs->dict._dictobj_, 0, &avp ) );			\
				value.os.len = attr->length - 2;						\
				value.os.data = (unsigned char *)(attr + 1);					\
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );				\
				/* Add the AVP in the Diameter message. */					\
				CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );		\
				
			/* Same thing, for scalar AVPs of 32 bits */
			#define CONV2DIAM_32B( _dictobj_ )	\
				CHECK_PARAMS( attr->length == 6 );						\
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
				CHECK_PARAMS( attr->length == 10);						\
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
				
			
			/* Attributes as listed in RFC2866, section 5.13  and RFC4005, section 10.2.1 */
			case RADIUS_ATTR_USER_NAME:
				CONV2DIAM_STR( User_Name );
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
				
			case RADIUS_ATTR_FRAMED_ROUTING:
				CONV2DIAM_32B( Framed_Routing );
				break;
				
			case RADIUS_ATTR_FILTER_ID:
				CONV2DIAM_STR( Filter_Id );
				break;
				
			case RADIUS_ATTR_FRAMED_MTU:
				CONV2DIAM_32B( Framed_MTU );
				break;
			
			case RADIUS_ATTR_FRAMED_COMPRESSION:
				CONV2DIAM_32B( Framed_Compression );
				break;
			
			case RADIUS_ATTR_LOGIN_IP_HOST:
				CONV2DIAM_STR( Login_IP_Host );
				break;
				
			case RADIUS_ATTR_LOGIN_SERVICE:
				CONV2DIAM_32B( Login_Service );
				break;
				
			case RADIUS_ATTR_LOGIN_TCP_PORT:
				CONV2DIAM_32B( Login_TCP_Port );
				break;
				
			case RADIUS_ATTR_CALLBACK_NUMBER:
				CONV2DIAM_STR( Callback_Number );
				break;
				
			case RADIUS_ATTR_CALLBACK_ID:
				CONV2DIAM_STR( Callback_Id );
				break;
				
			case RADIUS_ATTR_FRAMED_ROUTE:
				CONV2DIAM_STR( Framed_Route );
				break;
				
			case RADIUS_ATTR_FRAMED_IPX_NETWORK:
				CONV2DIAM_32B( Framed_IPX_Network );
				break;
				
			case RADIUS_ATTR_CLASS:
				CONV2DIAM_STR( Class );
				/* In addition, save the data in the session if it is "our" CLASS_AAI_PREFIX Class attribute */
				{
					char buf[32];
					char * attr_val, *auth_val;
					attr_val = (char *)(attr + 1);
					auth_val = attr_val + CONSTSTRLEN(CLASS_AAI_PREFIX);
					if (	(attr->length > sizeof(struct radius_attr_hdr) + CONSTSTRLEN(CLASS_AAI_PREFIX)  )
						&& (attr->length < sizeof(struct radius_attr_hdr) + CONSTSTRLEN(CLASS_AAI_PREFIX) + sizeof(buf))
						&& ! strncmp(attr_val, CLASS_AAI_PREFIX, CONSTSTRLEN(CLASS_AAI_PREFIX))) {
					
						memset(buf, 0, sizeof(buf));
						memcpy(buf, auth_val, attr->length - sizeof(struct radius_attr_hdr) - CONSTSTRLEN(CLASS_AAI_PREFIX));
						if (sscanf(buf, "%u", &auth_appl) == 1) {
							TRACE_DEBUG(ANNOYING, "Found Class attribute with '%s' prefix (attr #%d), AAI:%u.", CLASS_AAI_PREFIX, idx, auth_appl);
						}
					}
				}
				break;
			
			case RADIUS_ATTR_VENDOR_SPECIFIC:
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
				
			case RADIUS_ATTR_SESSION_TIMEOUT:
				/* Translation depends on Termination-Action : rfc4005#section-9.2.1 */
				if (termination_action != RADIUS_TERMINATION_ACTION_RADIUS_REQUEST) {
					CONV2DIAM_32B( Session_Timeout );
				} else {
					CONV2DIAM_32B( Authorization_Lifetime );
					/* And add this additional AVP */
					CHECK_FCT( fd_msg_avp_new ( cs->dict.Re_Auth_Request_Type, 0, &avp ) );
					value.u32 = ACV_ART_AUTHORIZE_AUTHENTICATE;
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				}
				break;
				
			case RADIUS_ATTR_IDLE_TIMEOUT:
				CONV2DIAM_32B( Idle_Timeout );
				break;
			
			case RADIUS_ATTR_TERMINATION_ACTION:
				/* Just remove */
				break;
			
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
			
			case RADIUS_ATTR_FRAMED_APPLETALK_LINK:
				CONV2DIAM_32B( Framed_AppleTalk_Link );
				break;
				
			case RADIUS_ATTR_FRAMED_APPLETALK_NETWORK:
				CONV2DIAM_32B( Framed_AppleTalk_Network );
				break;
				
			case RADIUS_ATTR_FRAMED_APPLETALK_ZONE:
				CONV2DIAM_STR( Framed_AppleTalk_Zone );
				break;
			
			case RADIUS_ATTR_ACCT_STATUS_TYPE:
				/*
				      -  If the RADIUS message received is an Accounting-Request, the
        				 Acct-Status-Type attribute value must be converted to a
        				 Accounting-Record-Type AVP value.  If the Acct-Status-Type
        				 attribute value is STOP, the local server MUST issue a
        				 Session-Termination-Request message once the Diameter
        				 Accounting-Answer message has been received.
				*/
				switch (status_type) {
					case RADIUS_ACCT_STATUS_TYPE_START:
						value.u32 = ACV_ART_START_RECORD;
						break;
					case RADIUS_ACCT_STATUS_TYPE_STOP:
						value.u32 = ACV_ART_STOP_RECORD;
						send_str = 1; /* Register this info in the session */
						break;
					case RADIUS_ACCT_STATUS_TYPE_INTERIM_UPDATE:
						value.u32 = ACV_ART_INTERIM_RECORD;
						break;
					default:
						TRACE_DEBUG(INFO, "Unknown RADIUS_ATTR_ACCT_STATUS_TYPE value %d, aborting...", status_type);
						return ENOTSUP;
				}
				CHECK_FCT( fd_msg_avp_new ( cs->dict.Accounting_Record_Type, 0, &avp ) );
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				
				/* While here, we also add the Accouting-Record-Number AVP.
					  The Accounting-Record-Number AVP (AVP Code 485) is of type Unsigned32
					   and identifies this record within one session.  As Session-Id AVPs
					   are globally unique, the combination of Session-Id and Accounting-
					   Record-Number AVPs is also globally unique, and can be used in
					   matching accounting records with confirmations.  An easy way to
					   produce unique numbers is to set the value to 0 for records of type
					   EVENT_RECORD and START_RECORD, and set the value to 1 for the first
					   INTERIM_RECORD, 2 for the second, and so on until the value for
					   STOP_RECORD is one more than for the last INTERIM_RECORD.
					   
				  -- we actually use the end-to-end id of the message here, which remains constant
				    if we send a duplicate, so it has the same properties as the suggested algorithm.
				    Anyway, it assumes that we are not converting twice the same RADIUS message.
				   . */
				CHECK_FCT( fd_msg_avp_new ( cs->dict.Accounting_Record_Number, 0, &avp ) );
				value.u32 = e2eid;
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				
				break;
			
			case RADIUS_ATTR_ACCT_DELAY_TIME:
				CONV2DIAM_32B( Acct_Delay_Time );
				break;
				
			/*
			      -  If the RADIUS message contains the Accounting-Input-Octets,
        			 Accounting-Input-Packets, Accounting-Output-Octets, or
        			 Accounting-Output-Packets, these attributes must be converted
        			 to the Diameter equivalents.  Further, if the Acct-Input-
        			 Gigawords or Acct-Output-Gigawords attributes are present,
        			 these must be used to properly compute the Diameter accounting
        			 AVPs.
			*/
			case RADIUS_ATTR_ACCT_INPUT_OCTETS:
				memset(&value, 0, sizeof(value));
				{
					uint8_t * v = (uint8_t *)(attr + 1);
					value.u64  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
				}
				value.u64 += ((uint64_t)gigawords_in << 32);
				CHECK_FCT( fd_msg_avp_new ( cs->dict.Accounting_Input_Octets, 0, &avp ) );
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				break;
				
			case RADIUS_ATTR_ACCT_OUTPUT_OCTETS:
				memset(&value, 0, sizeof(value));
				{
					uint8_t * v = (uint8_t *)(attr + 1);
					value.u64  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
				}
				value.u64 += ((uint64_t)gigawords_out << 32);
				CHECK_FCT( fd_msg_avp_new ( cs->dict.Accounting_Output_Octets, 0, &avp ) );
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				break;
				
			case RADIUS_ATTR_ACCT_SESSION_ID:
				CONV2DIAM_STR( Acct_Session_Id );
				break;
				
			case RADIUS_ATTR_ACCT_AUTHENTIC:
				CONV2DIAM_32B( Acct_Authentic );
				break;
				
			case RADIUS_ATTR_ACCT_SESSION_TIME:
				CONV2DIAM_32B( Acct_Session_Time );
				break;
				
			case RADIUS_ATTR_ACCT_INPUT_PACKETS:
				memset(&value, 0, sizeof(value));
				{
					uint8_t * v = (uint8_t *)(attr + 1);
					value.u64  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
				}
				/* value.u64 += (gigawords_in << 32); */
				CHECK_FCT( fd_msg_avp_new ( cs->dict.Accounting_Input_Packets, 0, &avp ) );
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				break;
				
			case RADIUS_ATTR_ACCT_OUTPUT_PACKETS:
				memset(&value, 0, sizeof(value));
				{
					uint8_t * v = (uint8_t *)(attr + 1);
					value.u64  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
				}
				/* value.u64 += (gigawords_out << 32); */
				CHECK_FCT( fd_msg_avp_new ( cs->dict.Accounting_Output_Packets, 0, &avp ) );
				CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				break;
				
			/*
			      -  If the Accounting message contains an Acct-Termination-Cause
        			 attribute, it should be translated to the equivalent
        			 Termination-Cause AVP value.
			*/
			case RADIUS_ATTR_ACCT_TERMINATE_CAUSE:
				/* rfc4005#section-9.3.5 */
				{
					uint8_t * v = (uint8_t *)(attr + 1);
					str_cause  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
				}
				str_cause += 10; /* This seems to be the rule, we can modify later if needed */
				break;

			case RADIUS_ATTR_ACCT_MULTI_SESSION_ID:
				CONV2DIAM_STR( Acct_Multi_Session_Id );
				break;
				
			case RADIUS_ATTR_ACCT_LINK_COUNT:
				CONV2DIAM_32B( Acct_Link_Count );
				break;
				
			/* CHAP-Challenge is not present in Accounting-Request */
			
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
				
			case RADIUS_ATTR_FRAMED_IPV6_ROUTE:
				CONV2DIAM_STR( Framed_IPv6_Route );
				break;
				
			case RADIUS_ATTR_FRAMED_IPV6_POOL:
				CONV2DIAM_STR( Framed_IPv6_Pool );
				break;
				
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
			
			/* Tunnel-Password never present in an Accounting-Request */

			case RADIUS_ATTR_TUNNEL_PRIVATE_GROUP_ID:
				CONV2DIAM_TUN_STR( Tunnel_Private_Group_Id );
				break;
			
			case RADIUS_ATTR_TUNNEL_ASSIGNMENT_ID:
				CONV2DIAM_TUN_STR( Tunnel_Assignment_Id );
				break;
			
			/* Tunnel-Reference never present in an Accounting-Request */
			
			case RADIUS_ATTR_TUNNEL_CLIENT_AUTH_ID:
				CONV2DIAM_TUN_STR( Tunnel_Client_Auth_Id );
				break;
			
			case RADIUS_ATTR_TUNNEL_SERVER_AUTH_ID:
				CONV2DIAM_TUN_STR( Tunnel_Server_Auth_Id );
				break;
			
		/* RFC 2869 */
			/*
			                                      Acct-Input-Gigawords, Acct-Output-
			   Gigawords, Event-Timestamp, and NAS-Port-Id may have 0-1 instances in
			   an Accounting-Request packet.  Connect-Info may have 0+ instances in
			   an Accounting-Request packet.  The other attributes added in this
			   document must not be present in an Accounting-Request.
			*/
			case RADIUS_ATTR_ACCT_INPUT_GIGAWORDS:
				break; /* we already saved the value in gigawords_in */
				
			case RADIUS_ATTR_ACCT_OUTPUT_GIGAWORDS:
				break; /* we already saved the value in gigawords_out */
				
			case RADIUS_ATTR_EVENT_TIMESTAMP:
				/* RADIUS: 
				      The Value field is four octets encoding an unsigned integer with
				      the number of seconds since January 1, 1970 00:00 UTC.
				   Diameter:
				      The Time format is derived from the OctetString AVP Base Format.
				      The string MUST contain four octets, in the same format as the
				      first four bytes are in the NTP timestamp format.  The NTP
				      Timestamp format is defined in Chapter 3 of [RFC4330].

				      This represents the number of seconds since 0h on 1 January 1900
				      with respect to the Coordinated Universal Time (UTC).
				      
				      -- RFC4330:
					 NTP timestamps are represented as a 64-bit unsigned
					   fixed-point number, in seconds relative to 0h on 1 January 1900.  The
					   integer part is in the first 32 bits, and the fraction part in the
					   last 32 bits.  In the fraction part, the non-significant low-order
					   bits are not specified and are ordinarily set to 0.
				*/
				{
					uint32_t ts;
					
					uint8_t * v = (uint8_t *)(attr + 1);
					/* Read the RADIUS attribute value */
					ts  = (v[0] << 24)
					           | (v[1] << 16)
					           | (v[2] <<  8)
					           |  v[3] ;
					
					/* Add the 70 missing years */
					ts += 2208988800U; /* 60 * 60 * 24 * ( 365 * 70 + 17 ) */
					
					/* Convert to network byte order */
					ts = htonl(ts);
					
					 /* Diameter Time datatype is derived from OctetString */
					value.os.data = (void *) &ts;
					value.os.len = sizeof(uint32_t);
					
					CHECK_FCT( fd_msg_avp_new ( cs->dict.Event_Timestamp, 0, &avp ) );
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( *diam_fw, MSG_BRW_LAST_CHILD, avp) );
				}
				break;
			
			case RADIUS_ATTR_NAS_PORT_ID:
				CONV2DIAM_STR( NAS_Port_Id );
				break;
						
			case RADIUS_ATTR_CONNECT_INFO:
				CONV2DIAM_STR( Connect_Info );
				break;
			
			case RADIUS_ATTR_FRAMED_POOL: /* To follow the IPv6 version */
				CONV2DIAM_STR( Framed_Pool );
				break;
				
			
		/* RFC 3579 */
			/*
			 The EAP-Message and Message-Authenticator attributes specified in
			   this document MUST NOT be present in an Accounting-Request.
			*/				
			case RADIUS_ATTR_ORIGINATING_LINE_INFO:
				CONV2DIAM_STR( Originating_Line_Info );
				break;
				
		/* Default */		
			default: /* unknown attribute */
				/* We just keep the attribute in the RADIUS message */
				rad_req->attr_pos[nattr_used++] = rad_req->attr_pos[idx];
		}
	}
	
	/* Update the radius message to remove all handled attributes */
	rad_req->attr_used = nattr_used;
	
	/* Store useful information in the session */
	{
		struct sess_state * st;
		
		CHECK_MALLOC( st = malloc(sizeof(struct sess_state)) );
		memset(st, 0, sizeof(struct sess_state));
		st->auth_appl = auth_appl;
		if (auth_appl) { /* We use the value 0 for servers which indicated NO STATE MAINTAINED, hence have no need for STR */
			st->send_str = send_str;
		}
		st->term_cause = str_cause;
		CHECK_FCT( fd_sess_state_store( cs->sess_hdl, sess, &st ) );
	}
	
	return 0;
}

/* Callback when an STA is received after having sent an STR. */
static void handle_sta(void * data, struct msg ** answer)
{
	struct rgwp_config * cs = data;
	struct avp *avp;
	struct avp_hdr *ahdr;
	
	CHECK_PARAMS_DO( data && answer && *answer, goto out );
	
	/* Check the Diameter error code */
	CHECK_FCT_DO( fd_msg_search_avp (*answer, cs->dict.Result_Code, &avp), goto out );
	CHECK_PARAMS_DO( avp, goto out );
	CHECK_FCT_DO( fd_msg_avp_hdr ( avp, &ahdr ), goto out );
	if (ahdr->avp_value->u32 != ER_DIAMETER_SUCCESS)
		goto out;
	
	/* OK, discard the message without complaining */
	fd_msg_free(*answer);
	*answer = NULL;
		
out:
	if (answer && *answer) {
		char * buf = NULL; size_t buflen;
		CHECK_MALLOC_DO( fd_msg_dump_treeview(&buf, &buflen, NULL, *answer, NULL, 0, 1), );
		TRACE_DEBUG(INFO, "Received the following problematic STA message, discarding anyway: %s", buf ?: "<error>");
		free(buf);
		
		fd_msg_free(*answer);
		*answer = NULL;
	}
	return;
}

static int acct_diam_ans( struct rgwp_config * cs, struct msg ** diam_ans, struct radius_msg ** rad_fw, struct rgw_client * cli )
{
	struct session * sess;
	struct sess_state * st = NULL, stloc;
	struct avp *avp, *next;
	struct avp_hdr *ahdr, *oh, *or;
	os0_t sid = NULL;
	size_t sidlen;
	
	TRACE_ENTRY("%p %p %p %p", cs, diam_ans, rad_fw, cli);
	CHECK_PARAMS(cs);
	
	CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, *diam_ans, &sess, NULL) );
	if (sess) {
		CHECK_FCT( fd_sess_state_retrieve( cs->sess_hdl, sess, &st ) );
		CHECK_FCT( fd_sess_getsid(sess, &sid, &sidlen) );
	}
	
	if (!st) {
		TRACE_DEBUG(INFO, "Received an ACA without corresponding session information, cannot translate to RADIUS");
		return EINVAL;
	}
	
	/* Free the state */
	memcpy(&stloc, st, sizeof(struct sess_state));
	free(st);
	st = &stloc;
	
	/* Search these AVPs first */
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Origin_Host, &avp) );
	CHECK_FCT( fd_msg_avp_hdr ( avp, &oh ) );

	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Origin_Realm, &avp) );
	CHECK_FCT( fd_msg_avp_hdr ( avp, &or ) );

	/* Check the Diameter error code */
	CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Result_Code, &avp) );
	ASSERT( avp ); /* otherwise the message should have been discarded a lot earlier because of ABNF */
	CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
	switch (ahdr->avp_value->u32) {
		case ER_DIAMETER_SUCCESS:
		case ER_DIAMETER_LIMITED_SUCCESS:
			(*rad_fw)->hdr->code = RADIUS_CODE_ACCOUNTING_RESPONSE;
			break;
		
		default:
			fd_log_debug("[acct.rgwx] Received Diameter answer with error code '%d' from server '%.*s', session %.*s, not translating into Accounting-Response",
					ahdr->avp_value->u32, 
					(int)oh->avp_value->os.len, oh->avp_value->os.data,
					(int)sidlen, sid);
			CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Error_Message, &avp) );
			if (avp) {
				CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
				fd_log_debug("[acct.rgwx]   Error-Message content: '%.*s'",
						(int)ahdr->avp_value->os.len, ahdr->avp_value->os.data);
			}
			CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Error_Reporting_Host, &avp) );
			if (avp) {
				CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
				fd_log_debug("[acct.rgwx]   Error-Reporting-Host: '%.*s'",
						(int)ahdr->avp_value->os.len, ahdr->avp_value->os.data);
			}
			CHECK_FCT( fd_msg_search_avp (*diam_ans, cs->dict.Failed_AVP, &avp) );
			if (avp) {
				fd_log_debug("[acct.rgwx]   Failed-AVP was included in the message.");
				/* Dump its content ? */
			}
			
			/* Now, destroy the Diameter message, since we know it is not converted to RADIUS */
			CHECK_FCT( fd_msg_free(*diam_ans) );
			*diam_ans = NULL;

			return -1;
	}
	/* Remove this Result-Code avp */
	CHECK_FCT( fd_msg_free( avp ) );
	
	/* If it was a response to a STOP record, we must send an STR for this session */
	if (st->send_str) {
		struct msg * str = NULL;
		struct msg_hdr * hdr = NULL;
		DiamId_t fqdn;
		size_t fqdn_len;
		DiamId_t realm;
		size_t realm_len;
		union avp_value avp_val;
		
		/* Create a new STR message */
		CHECK_FCT(  fd_msg_new ( cs->dict.Session_Termination_Request, MSGFL_ALLOC_ETEID, &str )  );
		
		/* Set the application-id to the auth application if available, accounting otherwise (not sure what is actually expected...) */
		CHECK_FCT( fd_msg_hdr ( str, &hdr ) );
		hdr->msg_appl = st->auth_appl ?: AI_ACCT;
		
		/* Add the Session-Id AVP as first AVP */
		CHECK_FCT( fd_msg_avp_new (  cs->dict.Session_Id, 0, &avp ) );
		avp_val.os.data = sid;
		avp_val.os.len = sidlen;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &avp_val ) );
		CHECK_FCT( fd_msg_avp_add ( str, MSG_BRW_FIRST_CHILD, avp) );
		CHECK_FCT( fd_sess_ref_msg(sess) );

		/* Add the Destination-Realm as next AVP */
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Destination_Realm, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_setvalue ( avp, or->avp_value ) );
		CHECK_FCT( fd_msg_avp_add ( str, MSG_BRW_LAST_CHILD, avp) );

		/* Get information on the NAS */
		CHECK_FCT( rgw_clients_get_origin(cli, &fqdn, &fqdn_len, &realm, &realm_len) );

		/* Add the Origin-Host as next AVP */
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Origin_Host, 0, &avp ) );
		memset(&avp_val, 0, sizeof(avp_val));
		avp_val.os.data = (unsigned char *)fqdn;
		avp_val.os.len = fqdn_len;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &avp_val ) );
		CHECK_FCT( fd_msg_avp_add ( str, MSG_BRW_LAST_CHILD, avp) );

		/* Add the Origin-Realm as next AVP */
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Origin_Realm, 0, &avp ) );
		memset(&avp_val, 0, sizeof(avp_val));
		avp_val.os.data = (unsigned char *)realm;
		avp_val.os.len = realm_len;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &avp_val ) );
		CHECK_FCT( fd_msg_avp_add ( str, MSG_BRW_LAST_CHILD, avp) );
		
		/* Auth-Application-Id -- if we did not get it from our Class attribute, we just set "0" */
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Auth_Application_Id, 0, &avp ) );
		avp_val.u32 = st->auth_appl;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &avp_val ) );
		CHECK_FCT( fd_msg_avp_add ( str, MSG_BRW_LAST_CHILD, avp) );
		
		/* Termination-Cause */
		CHECK_FCT( fd_msg_avp_new ( cs->dict.Termination_Cause, 0, &avp ) );
		avp_val.u32 = st->term_cause;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &avp_val ) );
		CHECK_FCT( fd_msg_avp_add ( str, MSG_BRW_LAST_CHILD, avp) );
		
		/* Send this message */
		CHECK_FCT( fd_msg_send ( &str, handle_sta, cs ) );
	}
	
	/* 
		No attributes should be found in
		   Accounting-Response packets except Proxy-State and possibly Vendor-
		   Specific.
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
				/* Based on RFC4005, sec 3.10 */
				case DIAM_ATTR_SESSION_ID:
				case DIAM_ATTR_ORIGIN_HOST:
				case DIAM_ATTR_ORIGIN_REALM:
				case DIAM_ATTR_ACCOUNTING_RECORD_TYPE:
				case DIAM_ATTR_ACCOUNTING_RECORD_NUMBER:
				case DIAM_ATTR_ACCT_APPLICATION_ID:
				case DIAM_ATTR_VENDOR_SPECIFIC_APPLICATION_ID:
				case DIAM_ATTR_USER_NAME:
				case DIAM_ATTR_ACCOUNTING_SUB_SESSION_ID:
				case DIAM_ATTR_ACCT_SESSION_ID:
				case DIAM_ATTR_ACCT_MULTI_SESSION_ID:
				case DIAM_ATTR_EVENT_TIMESTAMP:
				case DIAM_ATTR_ORIGIN_AAA_PROTOCOL:
				case DIAM_ATTR_ORIGIN_STATE_ID:
				case DIAM_ATTR_NAS_IDENTIFIER:
				case DIAM_ATTR_NAS_IP_ADDRESS:
				case DIAM_ATTR_NAS_IPV6_ADDRESS:
				case DIAM_ATTR_NAS_PORT:
				case DIAM_ATTR_NAS_PORT_ID:
				case DIAM_ATTR_NAS_PORT_TYPE:
				case DIAM_ATTR_SERVICE_TYPE:
				case DIAM_ATTR_TERMINATION_CAUSE:
				case DIAM_ATTR_ACCOUNTING_REALTIME_REQUIRED:
				case DIAM_ATTR_ACCT_INTERIM_INTERVAL:
				case DIAM_ATTR_CLASS:
					/* We just remove these AVP, they are not expected in RADIUS client */
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
	
	/*
	      The Authenticator field in an Accounting-Response packet is called
	      the Response Authenticator, and contains a one-way MD5 hash
	      calculated over a stream of octets consisting of the Accounting-
	      Response Code, Identifier, Length, the Request Authenticator field
	      from the Accounting-Request packet being replied to, and the
	      response attributes if any, followed by the shared secret.  The
	      resulting 16 octet MD5 hash value is stored in the Authenticator
	      field of the Accounting-Response packet.
	      
	      -- done in radius_msg_finish_srv 
	*/

	return 0;
}

/* The exported symbol */
struct rgw_api rgwp_descriptor = {
	.rgwp_name       = "acct",
	.rgwp_conf_parse = acct_conf_parse,
	.rgwp_conf_free  = acct_conf_free,
	.rgwp_rad_req    = acct_rad_req,
	.rgwp_diam_ans   = acct_diam_ans
};	
