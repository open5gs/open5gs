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

/* 
 * Dictionary definitions of objects specified in Diameter EAP application (RFC4072).
 */
#include <freeDiameter/extension.h>

/* The content of this file follows the same structure as dict_base_proto.c */

#define CHECK_dict_new( _type, _data, _parent, _ref )	\
	CHECK_FCT(  fd_dict_new( fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref))  );

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct local_rules_definition {
	char 			*avp_name;
	enum rule_position	position;
	int 			min;
	int			max;
};

#define RULE_ORDER( _position ) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0 )

#define PARSE_loc_rules( _rulearray, _parent) {								\
	int __ar;											\
	for (__ar=0; __ar < sizeof(_rulearray) / sizeof((_rulearray)[0]); __ar++) {			\
		struct dict_rule_data __data = { NULL, 							\
			(_rulearray)[__ar].position,							\
			0, 										\
			(_rulearray)[__ar].min,								\
			(_rulearray)[__ar].max};							\
		__data.rule_order = RULE_ORDER(__data.rule_position);					\
		CHECK_FCT(  fd_dict_search( 								\
			fd_g_config->cnf_dict,								\
			DICT_AVP, 									\
			AVP_BY_NAME, 									\
			(_rulearray)[__ar].avp_name, 							\
			&__data.rule_avp, 0 ) );							\
		if ( !__data.rule_avp ) {								\
			TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_name );		\
			return ENOENT;									\
		}											\
		CHECK_FCT_DO( fd_dict_new( fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL),	\
			{							        		\
				TRACE_DEBUG(INFO, "Error on rule with AVP '%s'",      			\
					 (_rulearray)[__ar].avp_name );		      			\
				return EINVAL;					      			\
			} );							      			\
	}									      			\
}

#define enumval_def_u32( _val_, _str_ ) \
		{ _str_, 		{ .u32 = _val_ }}

#define enumval_def_os( _len_, _val_, _str_ ) \
		{ _str_, 		{ .os = { .data = (unsigned char *)_val_, .len = _len_ }}}


static int deap_entry(char * conffile)
{
	struct dict_object * eap;
	TRACE_ENTRY("%p", conffile);
	
	/* Applications section */
	{
		/* EAP (RFC 4072) */
		{
			struct dict_application_data data  = { 	5, "Diameter Extensible Authentication Protocol (EAP) Application"	};
			CHECK_dict_new( DICT_APPLICATION, &data , NULL, &eap);
		}
	}
	
	/* AVP section */
	{
		/* EAP-Payload */
		{
			/*
				The EAP-Payload AVP (AVP Code 462) is of type OctetString and is used
				to encapsulate the actual EAP packet that is being exchanged between
				the EAP client and the home Diameter server.
			*/
			struct dict_avp_data data = { 
					462, 					/* Code */
					0, 					/* Vendor */
					"EAP-Payload", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* EAP-Reissued-Payload */
		{
			/*
				The EAP-Reissued-Payload AVP (AVP Code 463) is of type OctetString.
			*/
			struct dict_avp_data data = { 
					463, 					/* Code */
					0, 					/* Vendor */
					"EAP-Reissued-Payload", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* EAP-Master-Session-Key */
		{
			/*
				The EAP-Master-Session-Key AVP (AVP Code 464) is of type OctetString.
				It contains keying material for protecting the communications between
				the user and the NAS.  Exactly how this keying material is used
				depends on the link layer in question, and is beyond the scope of
				this document.
			*/
			struct dict_avp_data data = { 
					464, 					/* Code */
					0, 					/* Vendor */
					"EAP-Master-Session-Key", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* EAP-Key-Name */
		{
			/*
				The EAP-Key-Name AVP (Radius Attribute Type 102) is of type
				OctetString.  It contains an opaque key identifier (name) generated
				by the EAP method.  Exactly how this name is used depends on the link
				layer in question, and is beyond the scope of this document (see
				[EAPKey] for more discussion).

				Note that not all link layers use this name, and currently most EAP
				methods do not generate it.  Since the NAS operates in pass-through
				mode, it cannot know the Key-Name before receiving it from the AAA
				server.  As a result, a Key-Name AVP sent in a Diameter-EAP-Request
				MUST NOT contain any data.  A home Diameter server receiving a
				Diameter-EAP-Request with a Key-Name AVP with non-empty data MUST
				silently discard the AVP.  In addition, the home Diameter server
				SHOULD include this AVP in Diameter-EAP-Response only if an empty
				EAP-Key-Name AVP was present in Diameter-EAP-Request.
			*/
			struct dict_avp_data data = { 
					102, 					/* Code */
					0, 					/* Vendor */
					"EAP-Key-Name", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Accounting-EAP-Auth-Method */
		{
			/*
				The Accounting-EAP-Auth-Method AVP (AVP Code 465) is of type
				Unsigned64.  In case of expanded types [EAP, Section 5.7], this AVP
				contains the value ((Vendor-Id * 2^32) + Vendor-Type).
			*/
			struct dict_avp_data data = { 
					465, 					/* Code */
					0, 					/* Vendor */
					"Accounting-EAP-Auth-Method", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
	}

/********************/		
/* Commands section */
/********************/		
	{
		/* To avoid defining global variables for all the AVP that we use here, we do search the dictionary in each sub-block.
		 * This is far from optimal, but the code is clearer like this, and the time it requires at execution is not noticeable.
		 */
		/* Diameter-EAP-Request (DER) Command */
		{
			/*
				The Diameter-EAP-Request (DER) command, indicated by the Command-Code
				field set to 268 and the 'R' bit set in the Command Flags field, is
				sent by a Diameter client to a Diameter server, and conveys an
				EAP-Response from the EAP client.  The Diameter-EAP-Request MUST
				contain one EAP-Payload AVP containing the actual EAP payload.  An
				EAP-Payload AVP with no data MAY be sent to the Diameter server to
				initiate an EAP authentication session.

				The DER message MAY be the result of a multi-round authentication
				exchange that occurs when the DEA is received with the Result-Code
				AVP set to DIAMETER_MULTI_ROUND_AUTH [BASE].  A subsequent DER
				message MUST include any State AVPs [NASREQ] that were present in the
				DEA.  For re-authentication, it is recommended that the Identity
				request be skipped in order to reduce the number of authentication
				round trips.  This is only possible when the user's identity is
				already known by the home Diameter server.

				Message format

				<Diameter-EAP-Request> ::= < Diameter Header: 268, REQ, PXY >
                        				 < Session-Id >
                        				 { Auth-Application-Id }
                        				 { Origin-Host }
                        				 { Origin-Realm }
                        				 { Destination-Realm }
                        				 { Auth-Request-Type }
                        				 [ Destination-Host ]
                        				 [ NAS-Identifier ]
                        				 [ NAS-IP-Address ]
                        				 [ NAS-IPv6-Address ]
                        				 [ NAS-Port ]
                        				 [ NAS-Port-Id ]
                        				 [ NAS-Port-Type ]
                        				 [ Origin-State-Id ]
                        				 [ Port-Limit ]
                        				 [ User-Name ]
                        				 { EAP-Payload }
                        				 [ EAP-Key-Name ]
                        				 [ Service-Type ]
                        				 [ State ]
                        				 [ Authorization-Lifetime ]
                        				 [ Auth-Grace-Period ]
                        				 [ Auth-Session-State ]
                        				 [ Callback-Number ]
                        				 [ Called-Station-Id ]
                        				 [ Calling-Station-Id ]
                        				 [ Originating-Line-Info ]
                        				 [ Connect-Info ]
                				       * [ Framed-Compression ]
                        				 [ Framed-Interface-Id ]
                        				 [ Framed-IP-Address ]
                				       * [ Framed-IPv6-Prefix ]
                        				 [ Framed-IP-Netmask ]
                        				 [ Framed-MTU ]
                        				 [ Framed-Protocol ]
                				       * [ Tunneling ]
                				       * [ Proxy-Info ]
                				       * [ Route-Record ]
                				       * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					268, 					/* Code */
					"Diameter-EAP-Request", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 			/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Realm",		RULE_REQUIRED,   -1, 1 }
						 	,{  "Auth-Request-Type", 		RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Host",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Identifier",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IP-Address",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IPv6-Address",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port",				RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Port-Limit",			RULE_OPTIONAL,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "EAP-Payload",			RULE_REQUIRED,   -1, 1 }
							,{  "EAP-Key-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Service-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "State",				RULE_OPTIONAL,   -1, 1 }
							,{  "Authorization-Lifetime",		RULE_OPTIONAL,   -1, 1 }
							,{  "Auth-Grace-Period",		RULE_OPTIONAL,   -1, 1 }
							,{  "Auth-Session-State",		RULE_OPTIONAL,   -1, 1 }
							,{  "Callback-Number",			RULE_OPTIONAL,   -1, 1 }
							,{  "Called-Station-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Calling-Station-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Originating-Line-Info",		RULE_OPTIONAL,   -1, 1 }
							,{  "Connect-Info",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Compression",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-Interface-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IP-Address",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPv6-Prefix",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-IP-Netmask",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-MTU",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Protocol",			RULE_OPTIONAL,   -1, 1 }
							,{  "Tunneling",			RULE_OPTIONAL,   -1,-1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
							,{  "Route-Record",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , eap, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Diameter-EAP-Answer (DEA) Command */
		{
			/*
				The Diameter-EAP-Answer (DEA) message, indicated by the Command-Code
				field set to 268 and the 'R' bit cleared in the Command Flags field,
				is sent by the Diameter server to the client for one of the following
				reasons:

				1.  The message is part of a multi-round authentication exchange, and
				the server is expecting a subsequent Diameter-EAP-Request.  This
				is indicated by setting the Result-Code to
				DIAMETER_MULTI_ROUND_AUTH, and MAY include zero or more State
				AVPs.

				2.  The EAP client has been successfully authenticated and
				authorized, in which case the message MUST include the
				Result-Code AVP indicating success, and SHOULD include an
				EAP-Payload of type EAP-Success.  This event MUST cause the
				access device to provide service to the EAP client.

				3.  The EAP client has not been successfully authenticated and/or
				authorized, and the Result-Code AVP is set to indicate failure.
				This message SHOULD include an EAP-Payload, but this AVP is not
				used to determine whether service is to be provided.

				If the message from the Diameter client included a request for
				authorization, a successful response MUST include the authorization
				AVPs that are relevant to the service being provided.

				Message format

				<Diameter-EAP-Answer> ::= < Diameter Header: 268, PXY >
                        				< Session-Id >
                        				{ Auth-Application-Id }
                        				{ Auth-Request-Type }
                        				{ Result-Code }
                        				{ Origin-Host }
                        				{ Origin-Realm }
                        				[ User-Name ]
                        				[ EAP-Payload ]
                        				[ EAP-Reissued-Payload ]
                        				[ EAP-Master-Session-Key ]
                        				[ EAP-Key-Name ]
                        				[ Multi-Round-Time-Out ]
                        				[ Accounting-EAP-Auth-Method ]
                        				[ Service-Type ]
                				      * [ Class ]
                				      * [ Configuration-Token ]
                        				[ Acct-Interim-Interval ]
                        				[ Error-Message ]
                        				[ Error-Reporting-Host ]
                				      * [ Failed-AVP ]
                        				[ Idle-Timeout ]
                        				[ Authorization-Lifetime ]
                        				[ Auth-Grace-Period ]
                        				[ Auth-Session-State ]
                        				[ Re-Auth-Request-Type ]
                        				[ Session-Timeout ]
                        				[ State ]
                				      * [ Reply-Message ]
                        				[ Origin-State-Id ]
                				      * [ Filter-Id ]
                        				[ Port-Limit ]
                        				[ Callback-Id ]
                        				[ Callback-Number ]
                        				[ Framed-Appletalk-Link ]
                				      * [ Framed-Appletalk-Network ]
                        				[ Framed-Appletalk-Zone ]
                				      * [ Framed-Compression ]
                        				[ Framed-Interface-Id ]
                        				[ Framed-IP-Address ]
                				      * [ Framed-IPv6-Prefix ]
                        				[ Framed-IPv6-Pool ]
                				      * [ Framed-IPv6-Route ]
                        				[ Framed-IP-Netmask ]
                				      * [ Framed-Route ]
                        				[ Framed-Pool ]
                        				[ Framed-IPX-Network ]
                        				[ Framed-MTU ]
                        				[ Framed-Protocol ]
                        				[ Framed-Routing ]
                				      * [ NAS-Filter-Rule ]
                				      * [ QoS-Filter-Rule ]
                				      * [ Tunneling ]
                				      * [ Redirect-Host ]
                        				[ Redirect-Host-Usage ]
                        				[ Redirect-Max-Cache-Time ]
                				      * [ Proxy-Info ]
                				      * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					268, 					/* Code */
					"Diameter-EAP-Answer", 			/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE,	/* Fixed flags */
							   CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Auth-Request-Type", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Result-Code", 			RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "EAP-Payload",			RULE_OPTIONAL,   -1, 1 }
							,{  "EAP-Reissued-Payload",		RULE_OPTIONAL,   -1, 1 }
							,{  "EAP-Master-Session-Key",		RULE_OPTIONAL,   -1, 1 }
							,{  "EAP-Key-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Multi-Round-Time-Out",		RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-EAP-Auth-Method",	RULE_OPTIONAL,   -1, 1 }
							,{  "Service-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Class",				RULE_OPTIONAL,   -1,-1 }
							,{  "Configuration-Token",		RULE_OPTIONAL,   -1,-1 }
							,{  "Acct-Interim-Interval",		RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Message",			RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Reporting-Host",		RULE_OPTIONAL,   -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL,   -1,-1 }
							,{  "Idle-Timeout",			RULE_OPTIONAL,   -1, 1 }
							,{  "Authorization-Lifetime",		RULE_OPTIONAL,   -1, 1 }
							,{  "Auth-Grace-Period",		RULE_OPTIONAL,   -1, 1 }
							,{  "Auth-Session-State",		RULE_OPTIONAL,   -1, 1 }
							,{  "Re-Auth-Request-Type",		RULE_OPTIONAL,   -1, 1 }
							,{  "Session-Timeout",			RULE_OPTIONAL,   -1, 1 }
							,{  "State",				RULE_OPTIONAL,   -1, 1 }
							,{  "Reply-Message",			RULE_OPTIONAL,   -1,-1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Filter-Id",			RULE_OPTIONAL,   -1,-1 }
							,{  "Port-Limit",			RULE_OPTIONAL,   -1, 1 }
							,{  "Callback-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Callback-Number",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-AppleTalk-Link",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-AppleTalk-Network",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-AppleTalk-Zone",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Compression",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-Interface-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IP-Address",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPv6-Prefix",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-IPv6-Pool",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPv6-Route",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-IP-Netmask",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Route",			RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-Pool",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPX-Network",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-MTU",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Protocol",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Routing",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Filter-Rule",			RULE_OPTIONAL,   -1,-1 }
							,{  "QoS-Filter-Rule",			RULE_OPTIONAL,   -1,-1 }
							,{  "Tunneling",			RULE_OPTIONAL,   -1,-1 }
							,{  "Redirect-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Redirect-Host-Usage",		RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Max-Cache-Time",		RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , eap, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Accounting-Request */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):

				Attribute Name                         | ACR | ACA |
				---------------------------------------|-----+-----+
				Accounting-EAP-Auth-Method             |  0+ |  0  |
			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Accounting-EAP-Auth-Method",	RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Accounting-Request", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
	}
	
	LOG_D( "Extension 'Dictionary definitions for EAP' initialized");
	return 0;
}

EXTENSION_ENTRY("dict_eap", deap_entry, "dict_nasreq");
