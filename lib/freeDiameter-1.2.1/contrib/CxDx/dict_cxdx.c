/*********************************************************************************************************
 * Software License Agreement (BSD License)                                                               *
 * Author: Norberto R. de Goes Jr. 						 *
 *													 *
 * Copyright (c) 2011, Norberto R. de Goes Jr..                                                      	 *
 *										 *
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


/*********************************************************************************************************

 === CpqD/DRC  -  Projeto ADRIMS  -  Mar/2011 ===
 === Dicionario Dx/Cx ===
 Baseado no "dict_sip" do FreeDiameter (www.freediameter.net) 
                                                                                 Norberto R Goes Jr
*********************************************************************************************************/


#include <freeDiameter/extension.h>



/* The content of this file follows the same structure as dict_base_proto.c */

#define CHECK_dict_new( _type, _data, _parent, _ref )			\
  CHECK_FCT(  fd_dict_new( fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref))  );

#define CHECK_dict_search( _type, _criteria, _what, _result )		\
  CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );


struct local_rules_definition 
{
  char 			*avp_name;
  enum rule_position	 position;
  int 			 min;
  int			 max;
};

/*==================================================================*/

#define RULE_ORDER( _position ) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0 )

/*==================================================================*/

#define PARSE_loc_rules( _rulearray, _parent, _avp_search_flag) {	\
    int __ar;								\
    for (__ar=0; __ar < sizeof(_rulearray) / sizeof((_rulearray)[0]); __ar++) {	\
      struct dict_rule_data __data = { NULL,				\
				       (_rulearray)[__ar].position,	\
				       0,				\
				       (_rulearray)[__ar].min,		\
				       (_rulearray)[__ar].max};		\
      __data.rule_order = RULE_ORDER(__data.rule_position);		\
                                                                        \
      CHECK_FCT(  fd_dict_search(					\
				 fd_g_config->cnf_dict,			\
				 DICT_AVP,				\
				 _avp_search_flag,			\
				 (_rulearray)[__ar].avp_name,		\
				 &__data.rule_avp, 0 ) );		\
      if ( !__data.rule_avp ) {						\
	TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_name );	\
	return ENOENT;							\
      }									\
                                                                        \
      CHECK_FCT_DO( fd_dict_new( fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL), \
		    {							\
		      TRACE_DEBUG(INFO, "Error on rule with AVP '%s'",	\
				  (_rulearray)[__ar].avp_name );	\
		      return EINVAL;					\
		    } );						\
    }									\
  }

#define enumval_def_u32( _val_, _str_ )		\
  { _str_, 		{ .u32 = _val_ }}

#define enumval_def_os( _len_, _val_, _str_ )				\
  { _str_, 		{ .os = { .data = (unsigned char *)_val_, .len = _len_ }}}


/*==================================================================*/
/*==================================================================*/
/*==================================================================*/
/*==================================================================*/

int cxdx_dict_init(char * conffile)
{

#define VENDOR_3GPP_Id  10415


  struct dict_object * vendor_dict;
  {
    struct dict_vendor_data vendor_data = { VENDOR_3GPP_Id, "3GPP" };
    CHECK_dict_new (DICT_VENDOR, &vendor_data, NULL, &vendor_dict);
  }


  struct dict_object * cxdx_dict;
  {
    struct dict_application_data data  = { 16777216 /* NRGJ */, "Diameter CxDx Application"	};
    CHECK_dict_new (DICT_APPLICATION, &data, vendor_dict, &cxdx_dict);
  }


  /* ##### AVP section #################################### */
  {
    struct dict_object * UTF8String_type;
    struct dict_object * DiameterURI_type;

    CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String",  &UTF8String_type);
    CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterURI", &DiameterURI_type);

    /* Digest AVPs:  */

    /* Visited-Network-Identifier */
    {
      struct dict_avp_data data = 
	{ 
	  600, 					/* Code */
	  VENDOR_3GPP_Id,  			/* Vendor */
	  "Visited-Network-Identifier",         /* Name */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR, /* Fixed flags */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR, /* Fixed flag values */
	  AVP_TYPE_OCTETSTRING 			/* base type of data */
	};
      CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
    }

    /* Public-Identity */
    {
      struct dict_avp_data data = 
	{ 
	  601, 					/* Code */
	  VENDOR_3GPP_Id,			/* Vendor */
	  "Public-Identity", 		        /* Name */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR, /* Fixed flags */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR,	/* Fixed flag values */
	  AVP_TYPE_OCTETSTRING 			/* base type of data */
	};
      CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
    }


    /* Server-Name */
    {
      struct dict_avp_data data = 
	{ 
	  602, 					/* Code */
	  VENDOR_3GPP_Id,			/* Vendor */
	  "Server-Name", 		        /* Name */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR, /* Fixed flags */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR,	/* Fixed flag values */
	  AVP_TYPE_OCTETSTRING 			/* base type of data */
	};
      CHECK_dict_new( DICT_AVP, &data , DiameterURI_type/*UTF8String_type*/, NULL);
    }


    /* Optional-Capability */
    {
      struct dict_avp_data data = 
	{ 
	  605, 					/* Code */
	  VENDOR_3GPP_Id,			/* Vendor */
	  "Optional-Capability",	        /* Name */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR, /* Fixed flags */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR,	/* Fixed flag values */
	  AVP_TYPE_UNSIGNED32 			/* base type of data */
	};
      CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
    }


    /* Feature-List-ID */
    {
      struct dict_avp_data data = 
	{ 
	  629, 					/* Code */
	  VENDOR_3GPP_Id,			/* Vendor */
	  "Feature-List-ID", 		        /* Name */
	  AVP_FLAG_VENDOR,                      /* Fixed flags */
	  AVP_FLAG_VENDOR,	                /* Fixed flag values */
	  AVP_TYPE_UNSIGNED32 			/* base type of data */
	};
      CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
    }


    /* Feature-List */
    {
      struct dict_avp_data data = 
	{ 
	  630, 					/* Code */
	  VENDOR_3GPP_Id,			/* Vendor */
	  "Feature-List", 		        /* Name */
	  AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, /* Fixed flags */
	  AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
	  AVP_TYPE_UNSIGNED32 			/* base type of data */
	};
      CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
    }

    /* Server-Capabilities */
    {
      struct dict_object * avp;
      struct dict_avp_data data = 
	{ 
	  603, 					/* Code */
	  VENDOR_3GPP_Id,			/* Vendor */
	  "Server-Capabilities", 		/* Name */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR, /* Fixed flags */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR,	/* Fixed flag values */
	  AVP_TYPE_GROUPED 			/* base type of data */
	};

      struct local_rules_definition rules[] = 
	{ 
	  {  "Vendor-Id", 	 RULE_REQUIRED, -1, 1 },
	  {  "Feature-List-ID",  RULE_REQUIRED, -1, 1 },
	  {  "Feature-List", 	 RULE_REQUIRED, -1, 1 }
	};

      CHECK_dict_new (DICT_AVP, &data , NULL, &avp);
      PARSE_loc_rules(rules, avp, AVP_BY_NAME_ALL_VENDORS );
    }



    /* User-Authorization-Type */
    {
      struct dict_avp_data data = 
	{ 
	  623, 					/* Code */
	  VENDOR_3GPP_Id, 			/* Vendor */
	  "User-Authorization-Type", 		/* Name */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR, /* Fixed flags */
	  AVP_FLAG_MANDATORY | AVP_FLAG_VENDOR,	/* Fixed flag values */
	  AVP_TYPE_OCTETSTRING 			/* base type of data */
	};
      CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
    }


    /* Supported-Features */
    {
      struct dict_object * avp;
      struct dict_avp_data data = 
	{ 
	  628, 					/* Code */
	  VENDOR_3GPP_Id,			/* Vendor */
	  "Supported-Features", 		/* Name */
	  AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, /* Fixed flags */
	  AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
	  AVP_TYPE_GROUPED 			/* base type of data */
	};

      struct local_rules_definition rules[] = 
	{ 
	  {  "Vendor-Id", 	 RULE_REQUIRED, -1, 1 },
	  {  "Feature-List-ID",  RULE_REQUIRED, -1, 1 },
	  {  "Feature-List", 	 RULE_REQUIRED, -1, 1 }
	};

      CHECK_dict_new (DICT_AVP, &data , NULL, &avp);
      PARSE_loc_rules(rules, avp, AVP_BY_NAME_ALL_VENDORS );
    }
  



  } /* end AVP section */



  /* ### Command section ############################# */
  {
    /* User-Authorization-Request (UAR) Command */
    {
      /*
	The User-Authorization-Request (UAR) is indicated by the Command-Code
	set to 283 and the Command Flags' 'R' bit set.  The Diameter client
	in a SIP server sends this command to the Diameter server to request
	authorization for the SIP User Agent to route a SIP REGISTER request.
	Because the SIP REGISTER request implicitly carries a permission to
	bind an AOR to a contact address, the Diameter client uses the
	Diameter UAR as a first authorization request towards the Diameter
	server to authorize the registration.  For instance, the Diameter
	server can verify that the AOR is a legitimate user of the realm.

	The Diameter client in the SIP server requests authorization for one
	of the possible values defined in the SIP-User-Authorization-Type AVP
	(Section 9.10).

	The user name used for authentication of the user is conveyed in a
	User-Name AVP (defined in the Diameter base protocol, RFC 3588
	[RFC3588]).  The location of the authentication user name in the SIP
	REGISTER request varies depending on the authentication mechanism.
	When the authentication mechanism is HTTP Digest as defined in RFC
	2617 [RFC2617], the authentication user name is found in the
	"username" directive of the SIP Authorization header field value.
	This Diameter SIP application only provides support for HTTP Digest
	authentication in SIP; other authentication mechanisms are not
	currently supported.

	The SIP or SIPS URI to be registered is conveyed in the SIP-AOR AVP
	(Section 9.8).  Typically this SIP or SIPS URI is found in the To
	header field value of the SIP REGISTER request that triggered the
	Diameter UAR message.

	The SIP-Visited-Network-Id AVP indicates the network that is
	providing SIP services (e.g., SIP proxy functionality or any other
	kind of services) to the SIP User Agent.

	The Message Format of the UAR command is as follows:


	< UAR> ::=< Diameter Header: 300, REQ, PXY, 16777216 >
	< Session-Id >
	{ Vendor-Specific-Application-Id }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	[ Destination-Host ]
	{ Destination-Realm }
	{ User-Name }
	*[ Supported-Features ]
	{ Public-Identity }
	{ Visited-Network-Identifier }
	[ User-Authorization-Type ]
	[ UAR-Flags ]
	*[ AVP ]
	*[ Proxy-Info ]
	*[ Route-Record ]
	*/

      struct dict_object   * cmd;
      struct dict_cmd_data   data = 
	{ 
	  300,                   	/* Code */
	  "User-Authorization-Request", /* Name */
	  CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
	  CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE  /* Fixed flag values */
	};

      struct local_rules_definition rules[] = 
	{
	  {  "Session-Id", 			RULE_FIXED_HEAD, -1,  1 },
	  {  "Vendor-Specific-Application-Id",	RULE_REQUIRED,   -1,  1 },
	  {  "Auth-Session-State", 		RULE_REQUIRED,   -1,  1 },
	  {  "Origin-Host", 			RULE_REQUIRED,   -1,  1 },
	  {  "Origin-Realm", 			RULE_REQUIRED,   -1,  1 },
	  //	  {  "Destination-Host", 		RULE_OPTIONAL,   -1,  1 },
	  {  "Destination-Realm", 		RULE_REQUIRED,   -1,  1 },
	  {  "User-Name", 			RULE_REQUIRED,   -1,  1 },
	  //	  {  "Supported-Features", 		RULE_OPTIONAL,   -1, -1 },
	  {  "Public-Identity", 		RULE_REQUIRED,   -1,  1 },
	  {  "Visited-Network-Identifier",      RULE_REQUIRED,   -1,  1 },
	  //	  {  "UAR-Flags",                       RULE_OPTIONAL,   -1,  1 },
	  //	  {  "User-Authorization-Type", 	RULE_OPTIONAL,   -1,  1 },
	  //	  {  "Proxy-Info", 			RULE_OPTIONAL,   -1, -1 },
	  //	  {  "Route-Record", 			RULE_OPTIONAL,   -1, -1 }
	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME_ALL_VENDORS);
    }

    /* User-Authorization-Answer (UAA) Command */
    {
      /*
	The User-Authorization-Answer (UAA) is indicated by the Command-Code
	set to 283 and the Command Flags' 'R' bit cleared.  The Diameter
	server sends this command in response to a previously received
	Diameter User-Authorization-Request (UAR) command.  The Diameter
	server indicates the result of the requested registration
	authorization.  Additionally, the Diameter server may indicate a
	collection of SIP capabilities that assists the Diameter client to
	select a SIP proxy to the AOR under registration.


	In addition to the values already defined in RFC 3588 [RFC3588], the
	Result-Code AVP may contain one of the values defined in
	Section 10.1.

	Whenever the Diameter server fails to process the Diameter UAR
	message, it MUST stop processing and return the relevant error in the
	Diameter UAA message.  When there is success in the process, the
	Diameter server MUST set the code to DIAMETER_SUCCESS in the Diameter
	UAA message.

	If the Diameter server requires a User-Name AVP value to process the
	Diameter UAR request, but the Diameter UAR message did not contain a
	User-Name AVP value, the Diameter server MUST set the Result-Code AVP
	value to DIAMETER_USER_NAME_REQUIRED (see Section 10.1.2) and return
	it in a Diameter UAA message.  Upon reception of this Diameter UAA
	message with the Result-Code AVP value set to
	DIAMETER_USER_NAME_REQUIRED, the SIP server typically requests
	authentication by sending a SIP 401 (Unauthorized) or SIP 407 (Proxy
	Authentication Required) response back to the originator.

	When the authorization procedure succeeds, the Diameter server
	constructs a User-Authorization-Answer (UAA) message that MUST
	include (1) the address of the SIP server already assigned to the
	user name, (2) the capabilities needed by the SIP server (Diameter
	client) to select another SIP server for the user, or (3) a
	combination of the previous two options.

	If the Diameter server is already aware of a SIP server allocated to
	the user, the Diameter UAA message contains the address of that SIP
	server.

	The Diameter UAA message contains the capabilities required by a SIP
	server to trigger and execute services.  It is required that these
	capabilities are present in the Diameter UAA message due to the
	possibility that the Diameter client (in the SIP server) allocates a
	different SIP server to trigger and execute services for that
	particular user.

	If a User-Name AVP is present in the Diameter UAR message, then the
	Diameter server MUST verify the existence of the user in the realm,
	i.e., the User-Name AVP value is a valid user within that realm.  If
	the Diameter server does not recognize the user name received in the
	User-Name AVP, the Diameter server MUST build a Diameter User-
	Authorization-Answer (UAA) message and MUST set the Result-Code AVP
	to DIAMETER_ERROR_USER_UNKNOWN.


	If a User-Name AVP is present in the Diameter UAR message, then the
	Diameter server MUST authorize that User-Name AVP value is able to
	register the SIP or SIPS URI included in the SIP-AOR AVP.  If this
	authorization fails, the Diameter server must set the Result-Code AVP
	to DIAMETER_ERROR_IDENTITIES_DONT_MATCH and send it in a Diameter
	User-Authorization-Answer (UAA) message.

	Note: Correlation between User-Name and SIP-AOR AVP values is
	required in order to avoid registration of a SIP-AOR allocated to
	another user.

	If there is a SIP-Visited-Network-Id AVP in the Diameter UAR message,
	and the SIP-User-Authorization-Type AVP value received in the
	Diameter UAR message is set to REGISTRATION or REGISTRATION&
	CAPABILITIES, then the Diameter server SHOULD verify whether the user
	is allowed to roam into the network specified in the
	SIP-Visited-Network-Id AVP in the Diameter UAR message.  If the user
	is not allowed to roam into that network, the Diameter AAA server
	MUST set the Result-Code AVP value in the Diameter UAA message to
	DIAMETER_ERROR_ROAMING_NOT_ALLOWED.

	If the SIP-User-Authorization-Type AVP value received in the Diameter
	UAR message is set to REGISTRATION or REGISTRATION&CAPABILITIES, then
	the Diameter server SHOULD verify whether the SIP-AOR AVP value is
	authorized to register in the Home Realm.  Where the SIP AOR is not
	authorized to register in the Home Realm, the Diameter server MUST
	set the Result-Code AVP to DIAMETER_AUTHORIZATION_REJECTED and send
	it in a Diameter UAA message.

	When the SIP-User-Authorization-Type AVP is not present in the
	Diameter UAR message, or when it is present and its value is set to
	REGISTRATION, then:

	o  If the Diameter server is not aware of any previous registration
	of the user name (including registrations of other SIP AORs
	allocated to the same user name), then the Diameter server does
	not know of any SIP server allocated to the user.  In this case,
	the Diameter server MUST set the Result-Code AVP value to
	DIAMETER_FIRST_REGISTRATION in the Diameter UAA message, and the
	Diameter server SHOULD include the required SIP server
	capabilities in the SIP-Server-Capabilities AVP value in the
	Diameter UAA message.  The SIP-Server-Capabilities AVP assists the
	Diameter client (SIP server) to select an appropriate SIP server
	for the user, according to the required capabilities.

	o  In some cases, the Diameter server is aware of a previously
	assigned SIP server for the same or different SIP AORs allocated
	to the same user name.  In these cases, re-assignment of a new SIP
	server may or may not be needed, depending on the capabilities of
	the SIP server.  The Diameter server MUST always include the
	allocated SIP server URI in the SIP-Server-URI AVP of the UAA
	message.  If the Diameter server does not return the SIP
	capabilities, the Diameter server MUST set the Result-Code AVP in
	the Diameter UAA message to DIAMETER_SUBSEQUENT_REGISTRATION.
	Otherwise (i.e., if the Diameter server includes a
	SIP-Server-Capabilities AVP), then the Diameter server MUST set
	the Result-Code AVP in the Diameter UAA message to
	DIAMETER_SERVER_SELECTION.  Then the Diameter client determines,
	based on the received information, whether it needs to select a
	new SIP server.

	When the SIP-User-Authorization-Type AVP value received in the
	Diameter UAR message is set to REGISTRATION&CAPABILITIES, then
	Diameter Server MUST return the list of capabilities in the
	SIP-Server-Capabilities AVP value of the Diameter UAA message, it
	MUST set the Result-Code to DIAMETER_SUCCESS, and it MUST NOT return
	a SIP-Server-URI AVP.  The SIP-Server-Capabilities AVP enables the
	SIP server (Diameter client) to select another appropriate SIP server
	for invoking and executing services for the user, depending on the
	required capabilities.  The Diameter server MAY leave the list of
	capabilities empty to indicate that any SIP server can be selected.

	When the SIP-User-Authorization-Type AVP value received in the
	Diameter UAR message is set to DEREGISTRATION, then:

	o  If the Diameter server is aware of a SIP server assigned to the
	SIP AOR under deregistration, the Diameter server MUST set the
	Result-Code AVP to DIAMETER_SUCCESS and MUST set the
	SIP-Server-URI AVP value to the known SIP server, and return them
	in the Diameter UAA message.

	o  If the Diameter server is not aware of a SIP server assigned to
	the SIP AOR under deregistration, then the Diameter server MUST
	set the Result-Code AVP in the Diameter UAA message to
	DIAMETER_ERROR_IDENTITY_NOT_REGISTERED.

	The Message Format of the UAA command is as follows:

	<UAA> ::= < Diameter Header: 283, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Auth-Session-State }
	{ Result-Code }
	{ Origin-Host }
	{ Origin-Realm }
	[ SIP-Server-URI ]
	[ SIP-Server-Capabilities ]
	[ Authorization-Lifetime ]
	[ Auth-Grace-Period ]
	[ Redirect-Host ]
	[ Redirect-Host-Usage ]
	[ Redirect-Max-Cache-Time ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]


	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = 
	{ 
	  300, 					/* Code */
	  "User-Authorization-Answer", 		/* Name */
	  CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
	  CMD_FLAG_PROXIABLE 					  /* Fixed flag values */
	};

      struct local_rules_definition rules[] = 
	{ 	 
	  {  "Session-Id", 		RULE_FIXED_HEAD, -1,  1 },
	  {  "Auth-Application-Id", 	RULE_REQUIRED,   -1,  1 },
	  {  "Auth-Session-State", 	RULE_REQUIRED,   -1,  1 },
	  {  "Result-Code", 		RULE_REQUIRED,   -1,  1 },
	  {  "Origin-Host", 		RULE_REQUIRED,   -1,  1 },
	  {  "Origin-Realm", 		RULE_REQUIRED,   -1,  1 },
	  {  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 },
	  {  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }
	};

      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }





#if 0   /* TODO - NRGJ :   alterar conforme RFC-3GPP : */
		
    /* Multimedia-Auth-Request (MAR) Command */
    {
      /*		
			The Multimedia-Auth-Request (MAR) command is indicated by the
			Command-Code set to 286 and the Command Flags' 'R' bit set.  The
			Diameter client in a SIP server sends this command to the Diameter
			server to request that the Diameter server authenticate and authorize
			a user attempt to use some SIP service (in this context, SIP service
			can be something as simple as a SIP subscription or using the proxy
			services for a SIP request).

			The MAR command may also register the SIP server's own URI to the
			Diameter server, so that future LIR/LIA messages can return this URI.
			If the SIP server is acting as a SIP registrar (see examples in
			Sections 6.2 and 6.3), its Diameter client MUST include a SIP-
			Server-URI AVP in the MAR command.  In any other cases (see example
			in Section 6.4), its Diameter client MUST NOT include a SIP-Server-
			URI AVP in the MAR command.

			The SIP-Method AVP MUST include the SIP method name of the SIP
			request that triggered this Diameter MAR message.  The Diameter
			server can use this AVP to authorize some SIP requests depending on
			the method.

			The Diameter MAR message MUST include a SIP-AOR AVP.  The SIP-AOR AVP
			indicates the target of the SIP request.  The value of the AVP is
			extracted from different places in SIP request, depending on the
			semantics of the SIP request.  For SIP REGISTER messages the SIP-AOR
			AVP value indicates the intended public user identity under
			registration, and it is the SIP or SIPS URI populated in the To
			header field value (addr-spec as per RFC 3261 [RFC3261]) of the SIP
			REGISTER request.  For other types of SIP requests, such as INVITE,
			SUBSCRIBE, MESSAGE, etc., the SIP-AOR AVP value indicates the
			intended destination of the request.  This is typically populated in
			the Request-URI of the SIP request.  Extracting the SIP-AOR AVP value
			from the proper SIP header field is the Diameter client's
			responsibility.  Extensions to SIP (new SIP methods or new semantics)
			may require the SIP-AOR to be extracted from other parts of the
			request.

			If the SIP request includes some sort of authentication information,
			the Diameter client MUST include the user name, extracted from the
			authentication information of the SIP request, in the User-Name AVP
			value.

			The Message Format of the MAR command is as follows:

			<MAR> ::= < Diameter Header: 286, REQ, PXY >
			< Session-Id >
			{ Auth-Application-Id }
			{ Auth-Session-State }
			{ Origin-Host }
			{ Origin-Realm }
			{ Destination-Realm }
			{ SIP-AOR }
			{ SIP-Method }
			[ Destination-Host ]
			[ User-Name ]
			[ SIP-Server-URI ]
			[ SIP-Number-Auth-Items ]
			[ SIP-Auth-Data-Item ]
			* [ Proxy-Info ]
			* [ Route-Record ]
			* [ AVP ]

			*/

      struct dict_object * cmd;
      struct dict_cmd_data data = 
	{ 
	  303, 				/* Code */
	  "Multimedia-Auth-Request", 	/* Name */
	  CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	  CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 			/* Fixed flag values */
	};

      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 		RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Destination-Realm",	RULE_REQUIRED,   -1, 1 }
		 ,{  "SIP-AOR", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "SIP-Method", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Destination-Host", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "User-Name", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Server-URI", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Number-Auth-Items", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Auth-Data-Item", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
    /* Multimedia-Auth-Answer (MAA) Command */
    {
      /*
			
	The Multimedia-Auth-Answer (MAA) is indicated by the Command-Code set
	to 286 and the Command Flags' 'R' bit cleared.  The Diameter server
	sends this command in response to a previously received Diameter
	Multimedia-Auth-Request (MAR) command.

	In addition to the values already defined in RFC 3588 [RFC3588], the
	Result-Code AVP may contain one of the values defined in
	Section 10.1.

	If the Diameter server requires a User-Name AVP value to process the
	Diameter MAR request, but the Diameter MAR message did not contain a
	User-Name AVP value, the Diameter server MUST set the Result-Code AVP
	value to DIAMETER_USER_NAME_REQUIRED (see Section 10.1.2) and return
	it in a Diameter MAA message.  The Diameter server MAY include a
	SIP-Number-Auth-Items AVP and one or more SIP-Auth-Data-Item AVPs
	with authentication information (e.g., a challenge).  Upon reception
	of this Diameter MAA message with the Result-Code AVP value set to
	DIAMETER_USER_NAME_REQUIRED, the SIP server typically requests
	authentication by generating a SIP 401 (Unauthorized) or SIP 407
	(Proxy Authentication Required) response back to the originator.

	If the User-Name AVP is present in the Diameter MAR message, the
	Diameter server MUST verify the existence of the user in the realm,
	i.e., the User-Name AVP value is a valid user within that realm.  If
	the Diameter server does not recognize the user name received in the
	User-Name AVP, the Diameter server MUST build a Diameter
	Multimedia-Auth-Answer (MAA) message and MUST set the Result-Code AVP
	to DIAMETER_ERROR_USER_UNKNOWN.

	If the SIP-Methods AVP value of the Diameter MAR message is set to
	REGISTER and a User-Name AVP is present, then the Diameter server
	MUST authorize that User-Name AVP value is able to use the URI
	included in the SIP-AOR AVP.  If this authorization fails, the
	Diameter server must set the Result-Code AVP to
	DIAMETER_ERROR_IDENTITIES_DONT_MATCH and send it in a Diameter
	Multimedia-Auth-Answer (MAA) message.

	Note: Correlation between User-Name and SIP-AOR AVP values is only
	required for SIP REGISTER request, to prevent a user from
	registering a SIP-AOR allocated to another user.  In other types
	of SIP requests (e.g., INVITE), the SIP-AOR indicates the intended
	destination of the request, rather than the originator of it.

	The Diameter server MUST verify whether the authentication scheme
	(SIP-Authentication-Scheme AVP value) indicated in the grouped
	SIP-Auth-Data-Item AVP is supported or not.  If that authentication
	scheme is not supported, then the Diameter server MUST set the
	Result-Code AVP to DIAMETER_ERROR_AUTH_SCHEME_NOT_SUPPORTED and send
	it in a Diameter Multimedia-Auth-Answer (MAA) message.

	If the SIP-Number-Auth-Items AVP is present in the Diameter MAR
	message, it indicates the number of authentication data items that
	the Diameter client is requesting.  It is RECOMMENDED that the
	Diameter server, when building the Diameter MAA message, includes a
	number of SIP-Auth-Data-Item AVPs that are a subset of the
	authentication data items requested by the Diameter client in the
	SIP-Number-Auth-Items AVP value of the Diameter MAR message.

	If the SIP-Server-URI AVP is present in the Diameter MAR message,
	then the Diameter server MUST compare the stored SIP server (assigned
	to the user) with the SIP-Server-URI AVP value (received in the
	Diameter MAR message).  If they don't match, the Diameter server MUST
	temporarily save the newly received SIP server assigned to the user,
	and MUST set an "authentication pending" flag for the user.  If they
	match, the Diameter server shall clear the "authentication pending"
	flag for the user.

	In any other situation, if there is a success in processing the
	Diameter MAR command and the Diameter server stored the
	SIP-Server-URI, the Diameter server MUST set the Result-Code AVP
	value to DIAMETER_SUCCESS and return it in a Diameter MAA message.

	If there is a success in processing the Diameter MAR command, but the
	Diameter server does not store the SIP-Server-URI because the AVP was
	not present in the Diameter MAR command, then the Diameter server
	MUST set the Result-Code AVP value to either:

	1.  DIAMETER_SUCCESS_AUTH_SENT_SERVER_NOT_STORED, if the Diameter
	server is sending authentication credentials to create a
	challenge.

	2.  DIAMETER_SUCCESS_SERVER_NAME_NOT_STORED, if the Diameter server
	successfully authenticated the user and authorized the SIP server
	to proceed with the SIP request.

	Otherwise, the Diameter server MUST set the Result-Code AVP value to
	DIAMETER_UNABLE_TO_COMPLY, and it MUST NOT include any
	SIP-Auth-Data-Item AVP.

	The Message Format of the MAA command is as follows:

	<MAA> ::= < Diameter Header: 286, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Result-Code }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	[ User-Name ]
	[ SIP-AOR ]
	[ SIP-Number-Auth-Items ]
	* [ SIP-Auth-Data-Item ]
	[ Authorization-Lifetime ]
	[ Auth-Grace-Period ]
	[ Redirect-Host ]
	[ Redirect-Host-Usage ]
	[ Redirect-Max-Cache-Time ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]

	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = { 
	286, 					/* Code */
	"Multimedia-Auth-Answer", 		/* Name */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	CMD_FLAG_PROXIABLE 						/* Fixed flag values */
      };
      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 		RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Result-Code", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "User-Name", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-AOR", 			RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Number-Auth-Items", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Auth-Data-Item", 	RULE_OPTIONAL,   -1, -1 }
		 ,{  "Authorization-Lifetime", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Auth-Grace-Period", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host-Usage", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Max-Cache-Time", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
    /* Server-Assignment-Request (SAR) Command */
    {
      /*
			
	The Server-Assignment-Request (SAR) command is indicated by the
	Command-Code set to 284 and the Command Flags' 'R' bit set.  The
	Diameter client in a SIP server sends this command to the Diameter
	server to indicate the completion of the authentication process and
	to request that the Diameter server store the URI of the SIP server
	that is currently serving the user.  The main functions of the
	Diameter SAR command are to inform the Diameter server of the URI of
	the SIP server allocated to the user, and to store or clear it from
	the Diameter server.  Additionally, the Diameter client can request
	to download the user profile or part of it.

	During the registration procedure, a SIP server becomes assigned to
	the user.  The Diameter client in the assigned SIP server MUST
	include its own URI in the SIP-Server-URI AVP of the
	Server-Assignment-Request (SAR) Diameter message and send it to the
	Diameter server.  The Diameter server then becomes aware of the
	allocation of the SIP server to the user name and the server's URI.

	The Diameter client in the SIP server MAY send a Diameter SAR message
	because of other reasons.  These reasons are identified in the
	SIP-Server-Assignment-Type AVP (Section 9.4) value.  For instance, a
	Diameter client in a SIP server may contact the Diameter server to
	request deregistration of a user, to inform the Diameter server of an
	authentication failure, or just to download the user profile.  For a
	complete description of all the SIP-Server-Assignment-Type AVP
	values, see Section 9.4.

	Typically the reception of a SIP REGISTER request in a SIP server
	will trigger the Diameter client in the SIP server to send the
	Diameter SAR message.  However, if a SIP server is receiving other
	SIP request, such as INVITE, and the SIP server does not have the
	user profile, the Diameter client in the SIP server may send the
	Diameter SAR message to the Diameter server in order to download the
	user profile and make the Diameter server aware of the SIP server
	assigned to the user.
	The user profile is an important piece of information that dictates
	the behavior of the SIP server when triggering or providing services
	for the user.  Typically the user profile is divided into:

	o  Services to be rendered to the user when the user is registered
	and initiates a SIP request.

	o  Services to be rendered to the user when the user is registered
	and a SIP request destined to that user arrives to the SIP proxy.

	o  Services to be rendered to the user when the user is not
	registered and a SIP request destined to that user arrives to the
	SIP proxy.

	The SIP-Server-Assignment-Type AVP indicates the reason why the
	Diameter client (SIP server) contacted the Diameter server.  If the
	Diameter client sets the SIP-Server-Assignment-Type AVP value to
	REGISTRATION, RE_REGISTRATION, UNREGISTERED_USER, NO_ASSIGNMENT,
	AUTHENTICATION_FAILURE or AUTHENTICATION_TIMEOUT, the Diameter client
	MUST include exactly one SIP-AOR AVP in the Diameter SAR message.

	The SAR message MAY contain zero or more SIP-Supported-User-Data-Type
	AVPs.  Each of them contains a type of user data understood by the
	SIP server.  This allows the Diameter client to provide an indication
	to the Diameter server of the different format of user data
	understood by the SIP server.  The Diameter server uses this
	information to select one or more SIP-User-Data AVPs that will be
	included in the SAA message.

	The Message Format of the SAR command is as follows:

	<SAR> ::= < Diameter Header: 284, REQ, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	{ Destination-Realm }
	{ SIP-Server-Assignment-Type }
	{ SIP-User-Data-Already-Available }
	[ Destination-Host ]
	[ User-Name ]
	[ SIP-Server-URI ]
	* [ SIP-Supported-User-Data-Type ]
	* [ SIP-AOR ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]
	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = { 
	284, 					/* Code */
	"Server-Assignment-Request", 		/* Name */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
      };
      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "Destination-Realm",		RULE_REQUIRED,   -1, 1 }
		 ,{  "SIP-Server-Assignment-Type", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "SIP-User-Data-Already-Available", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Destination-Host", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "User-Name", 			RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Server-URI", 			RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Supported-User-Data-Type", 	RULE_OPTIONAL,   -1, -1 }
		 ,{  "SIP-AOR", 				RULE_OPTIONAL,   -1, -1 }
		 ,{  "Proxy-Info", 			RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 			RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
    /* Server-Assignment-Answer (SAA) Command */
    {
      /*
			
	The Server-Assignment-Answer (SAA) is indicated by the Command-Code
	set to 284 and the Command Flags' 'R' bit cleared.  The Diameter
	server sends this command in response to a previously received
	Diameter Server-Assignment-Request (SAR) command.  The response may
	include the user profile or part of it, if requested.

	In addition to the values already defined in RFC 3588 [RFC3588], the
	Result-Code AVP may contain one of the values defined in
	Section 10.1.

	The Result-Code AVP value in the Diameter SAA message may indicate a
	success or an error in the execution of the Diameter SAR command.  If
	Result-Code AVP value in the Diameter SAA message does not contain an
	error code, the SAA message MAY include one or more SIP-User-Data
	AVPs that typically contain the profile of the user, indicating
	services that the SIP server can provide to that user.

	The Diameter server MAY include one or more
	SIP-Supported-User-Data-Type AVPs, each one identifying a type of
	user data format supported in the Diameter server.  If there is not a
	common supported user data type between the Diameter client and the
	Diameter server, the Diameter server SHOULD declare its list of
	supported user data types by including one or more
	SIP-Supported-User-Data-Type AVPs in a Diameter SAA message.  This
	indication is merely for debugging reasons, since there is not a
	fallback mechanism that allows the Diameter client to retrieve the
	profile in a supported format.

	If the Diameter server requires a User-Name AVP value to process the
	Diameter SAR request, but the Diameter SAR message did not contain a
	User-Name AVP value, the Diameter server MUST set the Result-Code AVP
	value to DIAMETER_USER_NAME_REQUIRED (see Section 10.1.2) and return
	it in a Diameter SAA message.  Upon reception of this Diameter SAA
	message with the Result-Code AVP value set to
	DIAMETER_USER_NAME_REQUIRED, the SIP server typically requests
	authentication by generating a SIP 401 (Unauthorized) or SIP 407
	(Proxy Authentication Required) response back to the originator.

	If the User-Name AVP is included in the Diameter SAR message, upon
	reception of the Diameter SAR message, the Diameter server MUST
	verify the existence of the user in the realm, i.e., the User-Name
	AVP value is a valid user within that realm.  If the Diameter server
	does not recognize the user name received in the User-Name AVP, the
	Diameter server MUST build a Diameter Server-Assignment-Answer (SAA)
	message and MUST set the Result-Code AVP to
	DIAMETER_ERROR_USER_UNKNOWN.
	Then the Diameter server MUST authorize that User-Name AVP value is a
	valid authentication name for the SIP or SIPS URI included in the
	SIP-AOR AVP of the Diameter SAR message.  If this authorization
	fails, the Diameter server must set the Result-Code AVP to
	DIAMETER_ERROR_IDENTITIES_DONT_MATCH and send it in a Diameter
	Server-Assignment-Answer (SAA) message.

	After successful execution of the Diameter SAR command, the Diameter
	server MUST clear the "authentication pending" flag and SHOULD move
	the temporarily stored SIP server URI to permanent storage.

	The actions of the Diameter server upon reception of the Diameter SAR
	message depend on the value of the SIP-Server-Assignment-Type:

	o  If the SIP-Server-Assignment-Type AVP value in the Diameter SAR
	message is set to REGISTRATION or RE_REGISTRATION, the Diameter
	server SHOULD verify that there is only one SIP-AOR AVP.
	Otherwise, the Diameter server MUST answer with a Diameter SAA
	message with the Result-Code AVP value set to
	DIAMETER_AVP_OCCURS_TOO_MANY_TIMES and MUST NOT include any
	SIP-User-Data AVP.  If there is only one SIP-AOR AVP and if the
	SIP-User-Data-Already-Available AVP value is set to
	USER_DATA_NOT_AVAILABLE, then the Diameter server SHOULD include
	one or more user profile data with the SIP or SIPS URI (SIP-AOR
	AVP) and all other SIP identities associated with that AVP in the
	SIP-User-Data AVP value of the Diameter SAA message.  On selecting
	the type of user data, the Diameter server SHOULD take into
	account the supported formats at the SIP server
	(SIP-Supported-User-Data-Type AVP in the SAR message) and the
	local policy.  Additionally, the Diameter server MUST set the
	Result-Code AVP value to DIAMETER_SUCCESS in the Diameter SAA
	message.  The Diameter server considers the SIP AOR authenticated
	and registered.

	o  If the SIP-Server-Assignment-Type AVP value in the Diameter SAR
	message is set to UNREGISTERED_USER, then the Diameter server MUST
	store the SIP server address included in the SIP-Server-URI AVP
	value.  The Diameter server will return the SIP server address in
	Diameter Location-Info-Answer (LIA) messages.  If the
	SIP-User-Data-Already-Available AVP value is set to
	USER_DATA_NOT_AVAILABLE, then the Diameter server SHOULD include
	one or more user profile data associated with the SIP or SIPS URI
	(SIP-AOR AVP) and associated identities in the SIP-User-Data AVP
	value of the Diameter SAA message.  On selecting the type of user
	data, the Diameter server SHOULD take into account the supported
	formats at the SIP server (SIP-Supported-User-Data-Type AVP in the
	SAR message) and the local policy.  The Diameter server MUST set
	the Result-Code AVP value to DIAMETER_SUCCESS.  The Diameter
	server considers the SIP AOR UNREGISTERED, but with a SIP server
	allocated to trigger and provide services for unregistered users.
	Note that in case of UNREGISTERED_USER (SIP-Server-Assignment-Type
	AVP), the Diameter server MUST verify that there is only one
	SIP-AOR AVP.  Otherwise, the Diameter server MUST answer the
	Diameter SAR message with a Diameter SAA message, and it MUST set
	the Result-Code AVP value to DIAMETER_AVP_OCCURS_TOO_MANY_TIMES
	and MUST NOT include any SIP-User-Data AVP.
	If the User-Name AVP was not present in the Diameter SAR message
	and the SIP-AOR is not known for the Diameter server, the Diameter
	server MUST NOT include a User-Name AVP in the Diameter SAA
	message and MUST set the Result-Code AVP value to
	DIAMETER_ERROR_USER_UNKNOWN.

	o  If the SIP-Server-Assignment-Type AVP value in the Diameter SAR
	message is set to TIMEOUT_DEREGISTRATION, USER_DEREGISTRATION,
	DEREGISTRATION_TOO_MUCH_DATA, or ADMINISTRATIVE_DEREGISTRATION,
	the Diameter server MUST clear the SIP server address associated
	with all SIP AORs indicated in each of the SIP-AOR AVP values
	included in the Diameter SAR message.  The Diameter server
	considers all of these SIP AORs as not registered.  The Diameter
	server MUST set the Result-Code AVP value to DIAMETER_SUCCESS in
	the Diameter SAA message.

	o  If the SIP-Server-Assignment-Type AVP value in the Diameter SAR
	message is set to TIMEOUT_DEREGISTRATION_STORE_SERVER_NAME or
	USER_DEREGISTRATION_STORE_SERVER_NAME, the Diameter server MAY
	keep the SIP server address associated with the SIP AORs included
	in the SIP-AOR AVP values of the Diameter SAR message, even though
	the SIP AORs become unregistered.  This feature allows a SIP
	server to request that the Diameter server remain an assigned SIP
	server for those SIP AORs (SIP-AOR AVP values) allocated to the
	same user name, and avoid SIP server assignment.  The Diameter
	server MUST consider all these SIP AORs as not registered.  If the
	Diameter server honors the request of the Diameter client (SIP
	server) to remain as an allocated SIP server, then the Diameter
	server MUST keep the SIP server assigned to those SIP AORs
	allocated to the username and MUST set the Result-Code AVP value
	to DIAMETER_SUCCESS in the Diameter SAA message.  Otherwise, when
	the Diameter server does not honor the request of the Diameter
	client (SIP server) to remain as an allocated SIP server, the
	Diameter server MUST clear the SIP server name assigned to those
	SIP AORs and it MUST set the Result-Code AVP value to
	DIAMETER_SUCCESS_SERVER_NAME_NOT_STORED in the Diameter SAA
	message.
	o  If the SIP-Server-Assignment-Type AVP value in the Diameter SAR
	message is set to NO_ASSIGNMENT, the Diameter server SHOULD first
	verify that the SIP-Server-URI AVP value in the Diameter SAR
	message is the same URI as the one assigned to the SIP-AOR AVP
	value.  If they differ, then the Diameter server MUST set the
	Result-Code AVP value to DIAMETER_UNABLE_TO_COMPLY in the Diameter
	SAA message.  Otherwise, if the SIP-User-Data-Already-Available
	AVP value is set to USER_DATA_NOT_AVAILABLE, then the Diameter
	server SHOULD include the user profile data with the SIP or SIPS
	URI (SIP-AOR AVP) and all other SIP identities associated with
	that AVP in the SIP-User-Data AVP value of the Diameter SAA
	message.  On selecting the type of user data, the Diameter server
	SHOULD take into account the supported formats at the SIP server
	(SIP-Supported-User-Data-Type AVP in the SAR message) and the
	local policy.

	o  If the SIP-Server-Assignment-Type AVP value in the Diameter SAR
	message is set to AUTHENTICATION_FAILURE or
	AUTHENTICATION_TIMEOUT, the Diameter server MUST verify that there
	is exactly one SIP-AOR AVP in the Diameter SAR message.  If the
	number of occurrences of the SIP-AOR AVP is not exactly one, the
	Diameter server MUST set the Result-Code AVP value to
	DIAMETER_AVP_OCCURS_TOO_MANY_TIMES in the Diameter SAA message,
	and SHOULD not take further actions.  If there is exactly one
	SIP-AOR AVP in the Diameter SAR message, the Diameter server MUST
	clear the address of the SIP server assigned to the SIP AOR
	allocated to the user name, and the Diameter server MUST set the
	Result-Code AVP value to DIAMETER_SUCCESS in the Diameter SAA
	message.  The Diameter server MUST consider the SIP AOR as not
	registered.

	The Message Format of the SAA command is as follows:

	<SAA> ::= < Diameter Header: 284, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Result-Code }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	* [ SIP-User-Data ]
	[ SIP-Accounting-Information ]
	* [ SIP-Supported-User-Data-Type ]
	[ User-Name ]
	[ Auth-Grace-Period ]
	[ Authorization-Lifetime ]
	[ Redirect-Host ]
	[ Redirect-Host-Usage ]
	[ Redirect-Max-Cache-Time ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]




	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = { 
	284, 					/* Code */
	"Server-Assignment-Answer", 		/* Name */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	CMD_FLAG_PROXIABLE 						/* Fixed flag values */
      };
      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Result-Code", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "SIP-User-Data",			RULE_OPTIONAL,   -1, -1 }
		 ,{  "SIP-Accounting-Information", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Supported-User-Data-Type", 	RULE_OPTIONAL,   -1, -1 }
		 ,{  "User-Name", 			RULE_OPTIONAL,   -1, 1 }
		 ,{  "Auth-Grace-Period", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Authorization-Lifetime", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host", 			RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host-Usage", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Max-Cache-Time", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Proxy-Info", 			RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 			RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
    /* Location-Info-Request (LIR) Command */
    {
      /*
			
	The Location-Info-Request (LIR) is indicated by the Command-Code set
	to 285 and the Command Flags' 'R' bit set.  The Diameter client in a
	SIP server sends this command to the Diameter server to request
	routing information, e.g., the URI of the SIP server assigned to the
	SIP-AOR AVP value allocated to the users.

	The Message Format of the LIR command is as follows:

	<LIR> ::= < Diameter Header: 285, REQ, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	{ Destination-Realm }
	{ SIP-AOR }
	[ Destination-Host ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]
	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = { 
	285, 					/* Code */
	"Location-Info-Request", 		/* Name */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
      };
      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 		RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Destination-Realm",	RULE_REQUIRED,   -1, 1 }
		 ,{  "SIP-AOR", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "Destination-Host", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
    /* Location-Info-Answer (LIA) Command */
    {
      /*
	The Location-Info-Answer (LIA) is indicated by the Command-Code set
	to 285 and the Command Flags' 'R' bit cleared.  The Diameter server
	sends this command in response to a previously received Diameter
	Location-Info-Request (LIR) command.

	In addition to the values already defined in RFC 3588 [RFC3588], the
	Result-Code AVP may contain one of the values defined in
	Section 10.1.  When the Diameter server finds an error in processing
	the Diameter LIR message, the Diameter server MUST stop the process
	of the message and answer with a Diameter LIA message that includes
	the appropriate error code in the Result-Code AVP value.  When there
	is no error, the Diameter server MUST set the Result-Code AVP value
	to DIAMETER_SUCCESS in the Diameter LIA message.

	One of the errors that the Diameter server may find is that the
	SIP-AOR AVP value is not a valid user in the realm.  In such cases,
	the Diameter server MUST set the Result-Code AVP value to
	DIAMETER_ERROR_USER_UNKNOWN and return it in a Diameter LIA message.

	If the Diameter server cannot process the Diameter LIR command, e.g.,
	due to a database error, the Diameter server MUST set the Result-Code
	AVP value to DIAMETER_UNABLE_TO_COMPLY and return it in a Diameter
	LIA message.  The Diameter server MUST NOT include any SIP-Server-URI
	or SIP-Server-Capabilities AVP in the Diameter LIA message.

	The Diameter server may or may not be aware of a SIP server assigned
	to the SIP-AOR AVP value included in the Diameter LIR message.  If
	the Diameter server is aware of a SIP server allocated to that
	particular user, the Diameter server MUST include the URI of such SIP
	server in the SIP-Server-URI AVP and return it in a Diameter LIA
	message.  This is typically the situation when the user is either
	registered, or unregistered but a SIP server is still assigned to the
	user.

	When the Diameter server is not aware of a SIP server allocated to
	the user (typically the case when the user unregistered), the
	Result-Code AVP value in the Diameter LIA message depends on whether
	the Diameter server is aware that the user has services defined for
	unregistered users:

	o  Those users who have services defined for unregistered users may
	require the allocation of a SIP server to trigger and perhaps
	execute those services.  Therefore, when the Diameter server is
	not aware of an assigned SIP server, but the user has services
	defined for unregistered users, the Diameter server MUST set the
	Result-Code AVP value to DIAMETER_UNREGISTERED_SERVICE and return
	it in a Diameter LIA message.  The Diameter server MAY also
	include a SIP-Server-Capabilities AVP to facilitate the SIP server
	(Diameter client) with the selection of an appropriate SIP server
	with the required capabilities.  Absence of the SIP-Server-
	Capabilities AVP indicates to the SIP server (Diameter client)
	that any SIP server is suitable to be allocated for the user.

	o  Those users who do not have service defined for unregistered users
	do not require further processing.  The Diameter server MUST set
	the Result-Code AVP value to
	DIAMETER_ERROR_IDENTITY_NOT_REGISTERED and return it to the
	Diameter client in a Diameter LIA message.  The SIP server
	(Diameter client) may return the appropriate SIP response (e.g.,
	480 (Temporarily unavailable)) to the original SIP request.

	The Message Format of the LIA command is as follows:

	<LIA> ::= < Diameter Header: 285, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Result-Code }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	[ SIP-Server-URI ]
	[ SIP-Server-Capabilities ]
	[ Auth-Grace-Period ]
	[ Authorization-Lifetime ]
	[ Redirect-Host ]
	[ Redirect-Host-Usage ]
	[ Redirect-Max-Cache-Time ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]
	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = { 
	285, 					/* Code */
	"Location-Info-Answer", 		/* Name */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	CMD_FLAG_PROXIABLE 						/* Fixed flag values */
      };
      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 		RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Result-Code", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "SIP-Server-URI",		RULE_OPTIONAL,   -1, 1 }
		 ,{  "SIP-Server-Capabilities", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Auth-Grace-Period", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Authorization-Lifetime", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host-Usage", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Max-Cache-Time", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
    /* Registration-Termination-Request (RTR) Command */
    {
      /*
	The Registration-Termination-Request (RTR) command is indicated by
	the Command-Code set to 287 and the Command Flags' 'R' bit set.  The
	Diameter server sends this command to the Diameter client in a SIP
	server to indicate to the SIP server that one or more SIP AORs have
	to be deregistered.  The command allows an operator to
	administratively cancel the registration of a user from a centralized
	Diameter server.

	The Diameter server has the capability to initiate the deregistration
	of a user and inform the SIP server by means of the Diameter RTR
	command.  The Diameter server can decide whether only one SIP AOR is
	going to be deregistered, a list of SIP AORs, or all the SIP AORs
	allocated to the user.

	The absence of a SIP-AOR AVP in the Diameter RTR message indicates
	that all the SIP AORs allocated to the user identified by the
	User-Name AVP are being deregistered.

	The Diameter server MUST include a SIP-Deregistration-Reason AVP
	value to indicate the reason for the deregistration.

	The Message Format of the RTR command is as follows:

	<RTR> ::= < Diameter Header: 287, REQ, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	{ Destination-Host }
	{ SIP-Deregistration-Reason }
	[ Destination-Realm ]
	[ User-Name ]
	* [ SIP-AOR ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]

	*/

      struct dict_object * cmd;
      struct dict_cmd_data data = 
	{ 
	  287, 					/* Code */
	  "Registration-Termination-Request", 		/* Name */
	  CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	  CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 			/* Fixed flag values */
	};

      struct local_rules_definition rules[] = 
	{  
	  { "Session-Id",               RULE_FIXED_HEAD, -1,  1 },
	  { "Auth-Application-Id", 	RULE_REQUIRED,   -1,  1 },
	  { "Auth-Session-State", 	RULE_REQUIRED,   -1,  1 },
	  { "Origin-Host", 		RULE_REQUIRED,   -1,  1 },
	  { "Origin-Realm", 		RULE_REQUIRED,   -1,  1 },
	  { "Destination-Host", 	RULE_REQUIRED,   -1,  1 },
	  { "SIP-Deregistration-Reason",RULE_REQUIRED,   -1,  1 },	
	  { "Destination-Realm",	RULE_OPTIONAL,   -1,  1 },
	  { "User-Name", 		RULE_OPTIONAL,   -1,  1 },
	  { "SIP-AOR", 		        RULE_REQUIRED,   -1, -1 },
	  { "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 },
	  { "Route-Record", 		RULE_OPTIONAL,   -1, -1 }
	};
      
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
    /* Registration-Termination-Answer (RTA) Command */
    {
      /*
	The Registration-Termination-Answer (RTA) is indicated by the
	Command-Code set to 287 and the Command Flags' 'R' bit cleared.  The
	Diameter client sends this command in response to a previously
	received Diameter Registration-Termination-Request (RTR) command.

	In addition to the values already defined in RFC 3588 [RFC3588], the
	Result-Code AVP may contain one of the values defined in
	Section 10.1.

	If the SIP server (Diameter client) requires a User-Name AVP value to
	process the Diameter RTR request, but the Diameter RTR message did
	not contain a User-Name AVP value, the Diameter client MUST set the
	Result-Code AVP value to DIAMETER_USER_NAME_REQUIRED (see Section
	10.1.2) and return it in a Diameter RTA message.

	The SIP server (Diameter client) applies the administrative
	deregistration to each of the URIs included in each of the SIP-AOR
	AVP values, or, if there is no SIP-AOR AVP present in the Diameter
	RTR request, to all the URIs allocated to the User-Name AVP value.

	The value of the SIP-Deregistration-Reason AVP in the Diameter RTR
	command has an effect on the actions performed at the SIP server
	(Diameter client):

	o  If the value is set to PERMANENT_TERMINATION, then the user has
	terminated his/her registration to the realm.  If informing the
	interested parties (e.g., subscribers to the "reg" event
	[RFC3680]) about the administrative deregistration is supported
	through SIP procedures, the SIP server (Diameter client) will do
	so.  The Diameter Client in the SIP Server SHOULD NOT request a
	new user registration.  The SIP server clears the registration
	state of the deregistered AORs.

	o  If the value is set to NEW_SIP_SERVER_ASSIGNED, the Diameter
	server informs the SIP server (Diameter client) that a new SIP
	server has been allocated to the user, due to some reason.  The
	SIP server, if supported through SIP procedures, will inform the
	interested parties (e.g., subscribers to the "reg" event
	[RFC3680]) about the administrative deregistration at this SIP
	server.  The Diameter client in the SIP server SHOULD NOT request
	a new user registration.  The SIP server clears the registration
	state of the deregistered SIP AORs.

	o  If the value is set to SIP_SERVER_CHANGE, the Diameter server
	informs the SIP server (Diameter client) that a new SIP server has
	to be allocated to the user, e.g., due to user's capabilities
	requiring a new SIP server, or not enough resources in the current
	SIP server.  If informing the interested parties about the
	administrative deregistration is supported through SIP procedures
	(e.g., subscriptions to the "reg" event [RFC3680]), the SIP server
	will do so.  The Diameter client in the SIP Server SHOULD NOT
	request a new user registration.  The SIP server clears the
	registration state of the deregistered SIP AORs.

	o  If the value is set to REMOVE_SIP_SERVER, the Diameter server
	informs the SIP server (Diameter client) that the SIP server will
	no longer be bound in the Diameter server with that user.  The SIP
	server can delete all data related to the user.

	The Message Format of the RTA command is as follows:

	<RTA> ::= < Diameter Header: 287, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Result-Code }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	[ Authorization-Lifetime ]
	[ Auth-Grace-Period ]
	[ Redirect-Host ]
	[ Redirect-Host-Usage ]
	[ Redirect-Max-Cache-Time ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]

	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = { 
	287, 					/* Code */
	"Registration-Termination-Answer", 		/* Name */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	CMD_FLAG_PROXIABLE 						/* Fixed flag values */
      };
      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 		RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Result-Code", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Authorization-Lifetime",	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Auth-Grace-Period", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host-Usage", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Max-Cache-Time", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
		
    /* Push-Profile-Request (PPR) Command */
    {
      /*
	The Push-Profile-Request (PPR) command is indicated by the
	Command-Code set to 288 and the Command Flags' 'R' bit set.  The
	Diameter server sends this command to the Diameter client in a SIP
	server to update either the user profile of an already registered
	user in that SIP server or the SIP accounting information.  This
	allows an operator to modify the data of a user profile or the
	accounting information and push it to the SIP server where the user
	is registered.

	Each user has a user profile associated with him/her and other
	accounting information.  The profile or the accounting information
	may change with time, e.g., due to addition of new services to the
	user.  When the user profile or the accounting information changes,
	the Diameter server sends a Diameter Push-Profile-Request (PPR)
	command to the Diameter client in a SIP server, in order to start
	applying those new services.

	A PPR command MAY contain a SIP-Accounting-Information AVP that
	updates the addresses of the accounting servers.  Changes in the
	addresses of the accounting servers take effect immediately.  The
	Diameter client SHOULD close any existing accounting session with the
	existing server and start providing accounting information to the
	newly acquired accounting server.

	A PPR command MAY contain zero or more SIP-User-Data AVP values
	containing the new user profile.  On selecting the type of user data,
	the Diameter server SHOULD take into account the supported formats at
	the SIP server (SIP-Supported-User-Data-Type AVP sent in a previous
	SAR message) and the local policy.

	The User-Name AVP indicates the user to whom the profile is
	applicable.

	The Message Format of the PPR command is as follows:

	<PPR> ::= < Diameter Header: 288, REQ, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	{ Destination-Realm }
	{ User-Name }
	* [ SIP-User-Data ]
	[ SIP-Accounting-Information ]
	[ Destination-Host ]
	[ Authorization-Lifetime ]
	[ Auth-Grace-Period ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]

	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = { 
	288, 					/* Code */
	"Push-Profile-Request", 		/* Name */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
      };
      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "Destination-Realm",		RULE_REQUIRED,   -1, 1 }
		 ,{  "User-Name", 			RULE_REQUIRED,   -1, 1 }
		 ,{  "SIP-User-Data", 			RULE_OPTIONAL,   -1, -1 }
		 ,{  "SIP-Accounting-Information", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Destination-Host", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Authorization-Lifetime", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Auth-Grace-Period", 		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Proxy-Info", 			RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 			RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }
    /* Push-Profile-Answer (PPA) Command */
    {
      /*
			
			
	The Push-Profile-Answer (PPA) is indicated by the Command-Code set to
	288 and the Command Flags' 'R' bit cleared.  The Diameter client
	sends this command in response to a previously received Diameter
	Push-Profile-Request (PPR) command.

	In addition to the values already defined in RFC 3588 [RFC3588], the
	Result-Code AVP may contain one of the values defined in
	Section 10.1.

	If there is no error when processing the received Diameter PPR
	message, the SIP server (Diameter client) MUST download the received
	user profile from the SIP-User-Data AVP values in the Diameter PPR
	message and store it associated with the user specified in the
	User-Name AVP value.

	If the SIP server does not recognize or does not support some of the
	data transferred in the SIP-User-Data AVP values, the Diameter client
	in the SIP server MUST return a Diameter PPA message that includes a
	Result-Code AVP set to the value DIAMETER_ERROR_NOT_SUPPORTED_USER_DATA.

	If the SIP server (Diameter client) receives a Diameter PPR message
	with a User-Name AVP that is unknown, the Diameter client MUST set
	the Result-Code AVP value to DIAMETER_ERROR_USER_UNKNOWN and MUST
	return it to the Diameter server in a Diameter PPA message.

	If the SIP server (Diameter client) receives in the
	SIP-User-Data-Content AVP value (of the grouped SIP-User-Data AVP)
	more data than it can accept, it MUST set the Result-Code AVP value
	to DIAMETER_ERROR_TOO_MUCH_DATA and MUST return it to the Diameter
	server in a Diameter PPA message.  The SIP server MUST NOT override
	the existing user profile with the one received in the PPR message.

	If the Diameter server receives the Result-Code AVP value set to
	DIAMETER_ERROR_TOO_MUCH_DATA in a Diameter PPA message, it SHOULD
	force a new re-registration of the user by sending to the Diameter
	client a Diameter Registration-Termination-Request (RTR) with the
	SIP-Deregistration-Reason AVP value set to SIP_SERVER_CHANGE.  This
	will force a re-registration of the user and will trigger a selection
	of a new SIP server.

	If the Diameter client is not able to honor the command, for any
	other reason, it MUST set the Result-Code AVP value to
	DIAMETER_UNABLE_TO_COMPLY and it MUST return it in a Diameter PPA
	message.

	The Message Format of the PPA command is as follows:

	<PPA> ::= < Diameter Header: 288, PXY >
	< Session-Id >
	{ Auth-Application-Id }
	{ Result-Code }
	{ Auth-Session-State }
	{ Origin-Host }
	{ Origin-Realm }
	[ Redirect-Host ]
	[ Redirect-Host-Usage ]
	[ Redirect-Max-Cache-Time ]
	* [ Proxy-Info ]
	* [ Route-Record ]
	* [ AVP ]



	*/
      struct dict_object * cmd;
      struct dict_cmd_data data = { 
	288, 					/* Code */
	"Push-Profile-Answer", 		/* Name */
	CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
	CMD_FLAG_PROXIABLE 						/* Fixed flag values */
      };
      struct local_rules_definition rules[] = 
	{ 	 {  "Session-Id", 		RULE_FIXED_HEAD, -1, 1 }
		 ,{  "Auth-Application-Id", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Result-Code", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Auth-Session-State", 	RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Host", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Origin-Realm", 		RULE_REQUIRED,   -1, 1 }
		 ,{  "Redirect-Host",		RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Host-Usage", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Redirect-Max-Cache-Time", 	RULE_OPTIONAL,   -1, 1 }
		 ,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
		 ,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

	};
			
      CHECK_dict_new( DICT_COMMAND, &data , cxdx_dict, &cmd);
      PARSE_loc_rules( rules, cmd, AVP_BY_NAME );
    }

#endif /* TODO - NRGJ */

  }  /* end Command section */


	
  TRACE_DEBUG(INFO, "Extension 'Dictionary CxDx' initialized");
  return 0;
}


EXTENSION_ENTRY("dict_cxdx", cxdx_dict_init);

