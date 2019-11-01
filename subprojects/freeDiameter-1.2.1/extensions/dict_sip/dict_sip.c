/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Alexandre Westfahl <awestfahl@freediameter.net>						 *
*													 *
* Copyright (c) 2010, Alexandre Westfahl, Teraoka Laboratory (Keio University), and the WIDE Project. 	 *										 *
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



int ds_dict_init(char * conffile)
{
	struct dict_object * sip;
	{
		struct dict_application_data data  = { 	6, "Diameter Session Initiation Protocol (SIP) Application"	};
		CHECK_dict_new( DICT_APPLICATION, &data , NULL, &sip);
	}

	/* AVP section */
	{
		struct dict_object * UTF8String_type;
		struct dict_object * DiameterURI_type;

		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterURI", &DiameterURI_type);

		/* Digest AVPs (from RADIUS) */
		
		/* Digest-Response */
		{
			/*

			*/

			struct dict_avp_data data = { 
					103, 					/* Code */
					0, 					/* Vendor */
					"Digest-Response", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Realm */
		{
			/*

			*/

			struct dict_avp_data data = { 
					104, 					/* Code */
					0, 					/* Vendor */
					"Digest-Realm", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Nonce */
		{
			/*

			*/

			struct dict_avp_data data = { 
					105, 					/* Code */
					0, 					/* Vendor */
					"Digest-Nonce", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Response-Auth */
		{
			/*

			*/

			struct dict_avp_data data = { 
					106, 					/* Code */
					0, 					/* Vendor */
					"Digest-Response-Auth", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Nextnonce */
		{
			/*

			*/

			struct dict_avp_data data = { 
					107, 					/* Code */
					0, 					/* Vendor */
					"Digest-Nextnonce", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Method */
		{
			/*

			*/

			struct dict_avp_data data = { 
					108, 					/* Code */
					0, 					/* Vendor */
					"Digest-Method", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-URI */
		{
			/*

			*/

			struct dict_avp_data data = { 
					109, 					/* Code */
					0, 					/* Vendor */
					"Digest-URI", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-QoP */
		{
			/*

			*/

			struct dict_avp_data data = { 
					110, 					/* Code */
					0, 					/* Vendor */
					"Digest-QoP", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Algorithm */
		{
			/*

			*/

			struct dict_avp_data data = { 
					111, 					/* Code */
					0, 					/* Vendor */
					"Digest-Algorithm", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Entity-Body-Hash */
		{
			/*

			*/

			struct dict_avp_data data = { 
					112, 					/* Code */
					0, 					/* Vendor */
					"Digest-Entity-Body-Hash", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-CNonce */
		{
			/*

			*/

			struct dict_avp_data data = { 
					113, 					/* Code */
					0, 					/* Vendor */
					"Digest-CNonce", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Nonce-Count */
		{
			/*

			*/

			struct dict_avp_data data = { 
					114, 					/* Code */
					0, 					/* Vendor */
					"Digest-Nonce-Count", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Username */
		{
			/*

			*/

			struct dict_avp_data data = { 
					115, 					/* Code */
					0, 					/* Vendor */
					"Digest-Username", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Opaque */
		{
			/*

			*/

			struct dict_avp_data data = { 
					116, 					/* Code */
					0, 					/* Vendor */
					"Digest-Opaque", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Auth-Param */
		{
			/*

			*/

			struct dict_avp_data data = { 
					117, 					/* Code */
					0, 					/* Vendor */
					"Digest-Auth-Param", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-AKA-Auts */
		{
			/*

			*/

			struct dict_avp_data data = { 
					118, 					/* Code */
					0, 					/* Vendor */
					"Digest-AKA-Auts", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Domain */
		{
			/*

			*/

			struct dict_avp_data data = { 
					119, 					/* Code */
					0, 					/* Vendor */
					"Digest-Domain", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-Stale */
		{
			/*

			*/

			struct dict_avp_data data = { 
					120, 					/* Code */
					0, 					/* Vendor */
					"Digest-Stale", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Digest-HA1 */
		{
			/*

			*/

			struct dict_avp_data data = { 
					121, 					/* Code */
					0, 					/* Vendor */
					"Digest-HA1", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		/* SIP-AOR */
		{
			/*

			*/

			struct dict_avp_data data = { 
					122, 					/* Code */
					0, 					/* Vendor */
					"SIP-AOR", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}


		/* Diameter SIP AVPs*/
		/* SIP-Accounting-Server-URI*/
		{
			/*
				The SIP-Accounting-Server-URI AVP (AVP Code 369) is of type
				DiameterURI.  This AVP contains the address of a Diameter server that
				is able to receive SIP-session-related accounting information.
			*/

			struct dict_avp_data data = { 
					369, 					/* Code */
					0, 					/* Vendor */
					"SIP-Accounting-Server-URI", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterURI_type, NULL);
		}
		/* SIP-Credit-Control-Server-URI */
		{
			/*
			The SIP-Credit-Control-Server-URI AVP (AVP Code 370) is of type
			DiameterURI.  This AVP contains the address of a Diameter server that
			is able to authorize real-time credit control usage.  The Diameter
			Credit-Control Application [RFC4006] may be used for this purpose.


			*/

			struct dict_avp_data data = { 
					370, 					/* Code */
					0, 					/* Vendor */
					"SIP-Credit-Control-Server-URI", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterURI_type, NULL);
		}

		/* SIP-Accounting-Information */
		{
			/*
				The SIP-Accounting-Information (AVP Code 368) is of type Grouped, and
				contains the Diameter addresses of those nodes that are able to
				collect accounting information.

				The SIP-Accounting-Information AVP is defined as follows (per the
				grouped-avp-def of RFC 3588 [RFC3588]):

				SIP-Accounting-Information ::= < AVP Header: 368 >
					* [ SIP-Accounting-Server-URI ]
					* [ SIP-Credit-Control-Server-URI ]
					* [ AVP]

			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					368, 					/* Code */
					0, 					/* Vendor */
					"SIP-Accounting-Information", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "SIP-Accounting-Server-URI", 			RULE_OPTIONAL, -1, -1 }
							,{  "SIP-Credit-Control-Server-URI",			RULE_OPTIONAL, -1, -1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* SIP-Server-URI */
		{
			/*
				The SIP-Server-URI AVP (AVP Code 371) is of type UTF8String.  This
				AVP contains a SIP or SIPS URI (as defined in RFC 3261 [RFC3261])
				that identifies a SIP server.
			*/

			struct dict_avp_data data = { 
					371, 					/* Code */
					0, 					/* Vendor */
					"SIP-Server-URI", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* SIP-Mandatory-Capability */
		{
			/*
				The SIP-Mandatory-Capability AVP (AVP Code 373) is of type
				Unsigned32.  The value represents a certain capability (or set of
				capabilities) that have to be fulfilled by the SIP server allocated
				to the user.

				The semantics of the different values are not standardized, as it is
				a matter of the administrative network to allocate its own semantics
				within its own network.  Each value has to represent a single
				capability within the administrative network.

			*/

			struct dict_avp_data data = { 
					373, 					/* Code */
					0, 					/* Vendor */
					"SIP-Mandatory-Capability", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		/* SIP-Optional-Capability */
		{
			/*
				The SIP-Optional-Capability AVP (AVP Code 374) is of type Unsigned32.
				The value represents a certain capability (or set of capabilities)
				that, optionally, may be fulfilled by the SIP server allocated to the
				user.

				The semantics of the different values are not standardized, as it is
				a matter of the administrative network to allocate its own semantics
				within its own network.  Each value has to represent a single
				capability within the administrative network.

			*/

			struct dict_avp_data data = { 
					374, 					/* Code */
					0, 					/* Vendor */
					"SIP-Optional-Capability", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}


		/* SIP-Server-Capabilities */
		{
			/*
				The SIP-Server-Capabilities AVP (AVP Code 372) is of type Grouped.
				The Diameter indicates in this AVP the requirements for a particular
				SIP capability, so that the Diameter client (SIP server) is able to
				select another appropriate SIP server to serve the user.

				The SIP-Server-Capabilities AVP allows a Diameter client (SIP server)
				to select another SIP server for triggering or executing services to
				the user.  A user may have enabled some services that require the
				implementation of certain capabilities in the SIP server that
				triggers or executes those services.  For example, the SIP server
				that triggers or executes services to this user may need to implement
				SIP servlets [JSR-000116], Call Processing Language (CPL) [RFC3880],
				or any other kind of capability.  Or perhaps that user belongs to a
				premium users group that has a certain stringent quality-of-service
				agreement that requires a fast SIP server.  The capabilities required
				or recommended to a given user are conveyed in the
				SIP-Server-Capabilities AVP.  When it receives them, the Diameter
				client (SIP server) that does the SIP server selection needs to have
				the means to find out available SIP servers that meet the required or
				optional capabilities.  Such means are outside the scope of this
				specification.

				Note that the SIP-Server-Capabilities AVP assists the Diameter client
				(SIP server) to produce a subset of all the available SIP servers to
				be allocated to the user in the Home Realm; this is the subset that
				conforms the requirements of capabilities on a per-user basis.
				Typically this subset will be formed of more than a single SIP
				server, so once the subset of those SIP servers is identified, it is
				possible that several instances of these SIP servers exist, in which
				case the Diameter client (SIP server) should choose one particular
				SIP server to execute and trigger services to this user.  It is
				expected that at this point the SIP server (Diameter client) will
				follow the procedures of RFC 3263 [RFC3263] to allocate one SIP
				server to the user.

				The SIP-Server-Capabilities AVP is defined as follows (per the
				grouped-avp-def of RFC 3588 [RFC3588]):

				SIP-Server-Capabilities ::= < AVP Header: 372 >
					* [ SIP-Mandatory-Capability ]
					* [ SIP-Optional-Capability ]
					* [ SIP-Server-URI ]
					* [ AVP ]
	
			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					372, 					/* Code */
					0, 					/* Vendor */
					"SIP-Server-Capabilities", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "SIP-Mandatory-Capability", 	RULE_OPTIONAL, -1, -1 }
							,{  "SIP-Optional-Capability",		RULE_OPTIONAL, -1, -1 }
							,{  "SIP-Server-URI",			RULE_OPTIONAL, -1, -1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}


		/* SIP-Server-Assignment-Type */
		{
			/*
				The SIP-Server-Assignment-Type AVP (AVP Code 375) is of type
				Enumerated and indicates the type of server update being performed in
				a Diameter Server-Assignment-Request (SAR) operation.  The following
				values are defined:


				o  NO_ASSIGNMENT (0)
				The Diameter client uses this value to request the user profile of
				a SIP AOR, without affecting the registration state of that
				identity.

				o  REGISTRATION (1)
				First SIP registration of a SIP AOR.

				o  RE_REGISTRATION (2)
				Subsequent SIP registration of a SIP AOR.

				o  UNREGISTERED_USER (3)
				The SIP server has received a SIP request (e.g., SIP INVITE)
				addressed for a SIP AOR that is not registered.

				o  TIMEOUT_DEREGISTRATION (4)
				The SIP registration timer of an identity has expired.

				o  USER_DEREGISTRATION (5)
				The SIP server has received a request to deregister a SIP AOR.

				o  TIMEOUT_DEREGISTRATION_STORE_SERVER_NAME (6)
				The SIP registration timer of an identity has expired.  The SIP
				server keeps the user data stored and requests the Diameter server
				to store the SIP server address.

				o  USER_DEREGISTRATION_STORE_SERVER_NAME (7)
				The SIP server has received a user-initiated deregistration
				request.  The SIP server keeps the user data stored and requests
				the Diameter server to store the SIP server address.

				o  ADMINISTRATIVE_DEREGISTRATION (8)
				The SIP server, due to administrative reasons, has deregistered a
				SIP AOR.

				o  AUTHENTICATION_FAILURE (9)
				The authentication of a user has failed.

				o  AUTHENTICATION_TIMEOUT (10)
				The authentication timer has expired.

				o  DEREGISTRATION_TOO_MUCH_DATA (11)
				The SIP server has requested user profile information from the
				Diameter server and has received a volume of data higher than it
				can accept.

			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(SIP-Server-Assignment-Type)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "NO_ASSIGNMENT"),
				enumval_def_u32( 1, "REGISTRATION"),
				enumval_def_u32( 2, "RE_REGISTRATION"),
				enumval_def_u32( 3, "UNREGISTERED_USER"),
				enumval_def_u32( 4, "TIMEOUT_DEREGISTRATION"),
				enumval_def_u32( 5, "USER_DEREGISTRATION"),
				enumval_def_u32( 6, "TIMEOUT_DEREGISTRATION_STORE_SERVER_NAME"),
				enumval_def_u32( 7, "USER_DEREGISTRATION_STORE_SERVER_NAME"),
				enumval_def_u32( 8, "ADMINISTRATIVE_DEREGISTRATION"),
				enumval_def_u32( 9, "AUTHENTICATION_FAILURE"),
				enumval_def_u32( 10, "AUTHENTICATION_TIMEOUT"),
				enumval_def_u32( 11, "DEREGISTRATION_TOO_MUCH_DATA")
			};
			struct dict_avp_data data = { 
					375, 					/* Code */
					0, 					/* Vendor */
					"SIP-Server-Assignment-Type", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
			};
			int i;
			/* Create the Enumerated type, enumerated values, and the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		

		/* SIP-Authenticate */
		{
			/*
				The SIP-Authenticate AVP (AVP Code 379) is of type Grouped and
				contains a reconstruction of either the SIP WWW-Authenticate or
				Proxy-Authentication header fields specified in RFC 2617 [RFC2617]
				for the HTTP Digest authentication scheme.  Additionally, the AVP may
				include a Digest-HA1 AVP that contains H(A1) (as defined in RFC 2617
				[RFC2617]).  H(A1) allows the Diameter client to create an expected
				response and compare it with the Digest response received from the
				SIP UA.
				The SIP-Authenticate AVP is defined as follows (per the
				grouped-avp-def of RFC 3588 [RFC3588]):

				SIP-Authenticate ::= < AVP Header: 379 >
						   { Digest-Realm }
						   { Digest-Nonce }
						   [ Digest-Domain ]
						   [ Digest-Opaque ]
						   [ Digest-Stale ]
						   [ Digest-Algorithm ]
						   [ Digest-QoP ]
						   [ Digest-HA1]
						 * [ Digest-Auth-Param ]
						 * [ AVP ]


			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					379, 					/* Code */
					0, 					/* Vendor */
					"SIP-Authenticate", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Digest-Realm", 		RULE_REQUIRED, -1, 1 }
							,{  "Digest-Nonce",		RULE_REQUIRED, -1, 1 }
							,{  "Digest-Domain",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Opaque",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Stale",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Algorithm",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-QoP",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-HA1",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Auth-Param",	RULE_OPTIONAL, -1, -1 }

						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		/* SIP-Authorization */
		{
			/*
				The SIP-Authorization AVP (AVP Code 380) is of type Grouped and
				contains a reconstruction of either the SIP Authorization or
				Proxy-Authorization header fields specified in RFC 2617 [RFC2617] for
				the HTTP Digest authentication scheme.

				The SIP-Authorization AVP is defined as follows (per the
				grouped-avp-def of RFC 3588 [RFC3588]):

				SIP-Authorization ::= < AVP Header: 380 >
						    { Digest-Username }
						    { Digest-Realm }
						    { Digest-Nonce }
						    { Digest-URI }
						    { Digest-Response }
						    [ Digest-Algorithm ]
						    [ Digest-CNonce ]
						    [ Digest-Opaque ]
						    [ Digest-QoP ]
						    [ Digest-Nonce-Count ]
						    [ Digest-Method]
						    [ Digest-Entity-Body-Hash ]
						  * [ Digest-Auth-Param ]
						  * [ AVP ]

			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					380, 					/* Code */
					0, 					/* Vendor */
					"SIP-Authorization", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Digest-Username", 		RULE_REQUIRED, -1, 1 }
							,{  "Digest-Realm",		RULE_REQUIRED, -1, 1 }
							,{  "Digest-Nonce",		RULE_REQUIRED, -1, 1 }
							,{  "Digest-URI",		RULE_REQUIRED, -1, 1 }
							,{  "Digest-Response",		RULE_REQUIRED, -1, 1 }
							,{  "Digest-Algorithm",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-CNonce",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Opaque",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-QoP",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Nonce-Count",	RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Method",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Entity-Body-Hash",	RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Auth-Param",	RULE_OPTIONAL, -1, -1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		/* SIP-Authentication-Info */
		{
			/*
				The SIP-Authentication-Info AVP (AVP Code 381) is of type Grouped and
				contains a reconstruction of the SIP Authentication-Info header
				specified in RFC 2617 [RFC2617] for the HTTP Digest authentication
				scheme.
				The SIP-Authentication-Info AVP is defined as follows (per the
				grouped-avp-def of RFC 3588 [RFC3588]):

				SIP-Authentication-Info ::= < AVP Header: 381 >
							  [ Digest-Nextnonce ]
							  [ Digest-QoP ]
							  [ Digest-Response-Auth ]
							  [ Digest-CNonce ]
							  [ Digest-Nonce-Count ]
							* [ AVP ]

				Note that, in some cases, the Digest-Response-Auth AVP cannot be
				calculated at the Diameter server, but has to be calculated at the
				Diameter client (SIP server).  For example, if the value of the
				quality of protection (qop) parameter in Digest is set to "auth-int",
				then the response-digest (rspauth parameter value in Digest) is
				calculated with the hash of the body of the SIP response, which is
				not available at the Diameter server.  In this case, the Diameter
				client (SIP server) must calculate the response-digest once the body
				of the SIP response is calculated.

				Therefore, a value of "auth-int" in the Digest-QoP AVP of the
				SIP-Authentication-Info AVP indicates that the Diameter client (SIP
				server) MUST compute the Digest "rspauth" parameter value at the
				Diameter client (SIP server).

			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					381, 					/* Code */
					0, 					/* Vendor */
					"SIP-Authentication-Info", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Digest-Nextnonce", 	RULE_OPTIONAL, -1, 1 }
							,{  "Digest-QoP",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Response-Auth",	RULE_OPTIONAL, -1, 1 }
							,{  "Digest-CNonce",		RULE_OPTIONAL, -1, 1 }
							,{  "Digest-Nonce-Count",	RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		/* SIP-Authentication-Scheme */
		{
			/*
				The SIP-Authentication-Scheme AVP (AVP Code 377) is of type
				Enumerated and indicates the authentication scheme used in the
				authentication of SIP services.  RFC 2617 identifies this value as an
				"auth-scheme" (see Section 1.2 of RFC 2617 [RFC2617]).  The only
				currently defined value is:

				o  DIGEST (0) to indicate HTTP Digest authentication as specified in
				RFC 2617 [RFC2617] Section 3.2.1.  Derivative work is also
				considered Digest authentication scheme, as long as the
				"auth-scheme" is identified as Digest in the SIP headers carrying
				the HTTP authentication.  This includes, e.g., the HTTP Digest
				authentication using AKA [RFC3310].

				Each HTTP Digest directive (parameter) is transported in a
				corresponding AVP, whose name follows the pattern Digest-*.  The
				Digest-* AVPs are RADIUS attributes imported from the RADIUS
				Extension for Digest Authentication [RFC4590] namespace, allowing a
				smooth transition between RADIUS and Diameter applications supporting
				SIP.  The Diameter SIP application goes a step further by grouping
				the Digest-* AVPs into the SIP-Authenticate, SIP-Authorization, and
				SIP-Authentication-Info grouped AVPs that correspond to the SIP WWW-
				Authenticate/Proxy-Authentication, Authorization/Proxy-Authorization,
				and Authentication-Info headers fields, respectively.

				Note: Due to the fact that HTTP Digest authentication [RFC2617] is
				the only mandatory authentication mechanism in SIP, this memo only
				provides support for HTTP Digest authentication and derivative
				work such as HTTP Digest authentication using AKA [RFC3310].
				Extensions to this memo can register new values and new AVPs to
				provide support for other authentication schemes or extensions to
				HTTP Digest authentication.

				Note: Although RFC 2617 [RFC2617] defines the Basic and Digest
				schemes for authenticating HTTP requests, RFC 3261 [RFC3261] only
				imports HTTP Digest as a mechanism to provide authentication in
				SIP.

				Due to syntactic requirements, HTTP Digest authentication has to
				escape quote characters in contents of HTTP Digest directives.  When
				translating directives into Digest-* AVPs, the Diameter client or
				server removes the surrounding quotes where present, as required by
				the syntax of the Digest-* attributes defined in the "RADIUS
				Extension for Digest Authentication" [RFC4590].

			*/

			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(SIP-Authentication-Scheme)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "DIGEST")
			};
			struct dict_avp_data data = { 
					377, 					/* Code */
					0, 					/* Vendor */
					"SIP-Authentication-Scheme", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
			};
			int i;
			/* Create the Enumerated type, enumerated values, and the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		/* SIP-Item-Number */
		{
			/*
				The SIP-Item-Number (AVP Code 378) is of type Unsigned32 and is
				included in a SIP-Auth-Data-Item grouped AVP in circumstances where
				there are multiple occurrences of SIP-Auth-Data-Item AVPs and the
				order of processing is relevant.  The AVP indicates the order in
				which the Grouped SIP-Auth-Data-Item should be processed.  Lower
				values of the SIP-Item-Number AVP indicate that the whole
				SIP-Auth-Data-Item SHOULD be processed before other
				SIP-Auth-Data-Item AVPs that contain higher values in the
				SIP-Item-Number AVP.

			*/

			struct dict_avp_data data = { 
					378, 					/* Code */
					0, 					/* Vendor */
					"SIP-Item-Number", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		/* SIP-Auth-Data-Item */
		{
			/*
				The SIP-Auth-Data-Item (AVP Code 376) is of type Grouped and contains
				the authentication and/or authorization information pertaining to a
				user.

				When the Diameter server uses the grouped SIP-Auth-Data-Item AVP to
				include a SIP-Authenticate AVP, the Diameter server MUST send a
				maximum of one authentication data item (e.g., in case the SIP
				request contained several credentials).  Section 11 contains a
				detailed discussion and normative text of the case when a SIP request
				contains several credentials.

				The SIP-Auth-Data-Item AVP is defined as follows (per the
				grouped-avp-def of RFC 3588 [RFC3588]):

				SIP-Auth-Data-Item ::= < AVP Header: 376 >
				{ SIP-Authentication-Scheme }
					[ SIP-Item-Number ]
					[ SIP-Authenticate ]
					[ SIP-Authorization ]
					[ SIP-Authentication-Info ]
					* [ AVP ]


			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					376, 					/* Code */
					0, 					/* Vendor */
					"SIP-Auth-Data-Item", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "SIP-Authentication-Scheme",RULE_REQUIRED, -1, 1 }
							,{  "SIP-Item-Number", 		RULE_OPTIONAL, -1, 1 }
							,{  "SIP-Authenticate",		RULE_OPTIONAL, -1, 1 }
							,{  "SIP-Authorization",	RULE_OPTIONAL, -1, 1 }
							,{  "SIP-Authentication-Info",	RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		/* SIP-Number-Auth-Items */
		{
			/*
				The SIP-Number-Auth-Items AVP (AVP Code 382) is of type Unsigned32
				and indicates the number of authentication and/or authorization
				credentials that the Diameter server included in a Diameter message.

				When the AVP is present in a request, it indicates the number of
				SIP-Auth-Data-Items the Diameter client is requesting.  This can be
				used, for instance, when the SIP server is requesting several
				pre-calculated authentication credentials.  In the answer message,
				the SIP-Number-Auth-Items AVP indicates the actual number of items
				that the Diameter server included.

			*/

			struct dict_avp_data data = { 
					382, 					/* Code */
					0, 					/* Vendor */
					"SIP-Number-Auth-Items", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* SIP-Reason-Code */
		{
			/*
				The SIP-Reason-Code AVP (AVP Code 384) is of type Enumerated and
				defines the reason for the network initiated deregistration.  The
				following values are defined:

				o  PERMANENT_TERMINATION (0)
				o  NEW_SIP_SERVER_ASSIGNED (1)
				o  SIP_SERVER_CHANGE (2)
				o  REMOVE_SIP_SERVER (3)
			*/

			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(SIP-Reason-Code)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "PERMANENT_TERMINATION"),
				enumval_def_u32( 1, "NEW_SIP_SERVER_ASSIGNED"),
				enumval_def_u32( 2, "SIP_SERVER_CHANGE"),
				enumval_def_u32( 3, "REMOVE_SIP_SERVER")
			};
			struct dict_avp_data data = { 
					384, 					/* Code */
					0, 					/* Vendor */
					"SIP-Reason-Code", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
			};
			int i;
			/* Create the Enumerated type, enumerated values, and the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* SIP-Reason-Info */
		{
			/*
				The SIP-Reason-Info AVP (AVP Code 385) is of type UTF8String and
				contains textual information that can be rendered to the user, about
				the reason for a deregistration.
				
			*/

			struct dict_avp_data data = { 
					385, 					/* Code */
					0, 					/* Vendor */
					"SIP-Reason-Info", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* SIP-Deregistration-Reason */
		{
			/*
				The SIP-Deregistration-Reason AVP (AVP Code 383) is of type Grouped
				and indicates the reason for a deregistration operation.

				The SIP-Deregistration-Reason AVP is defined as follows (per the
				grouped-avp-def of RFC 3588 [RFC3588]):

				SIP-Deregistration-Reason ::= < AVP Header: 383 >
							    { SIP-Reason-Code }
							    [ SIP-Reason-Info ]
							  * [ AVP ]

			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					383, 					/* Code */
					0, 					/* Vendor */
					"SIP-Deregistration-Reason", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "SIP-Reason-Code", 	RULE_REQUIRED, -1, 1 }
							,{  "SIP-Reason-Info",	RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* SIP-Visited-Network-Id */
		{
			/*
				The SIP-Visited-Network-Id AVP (AVP Code 386) is of type UTF8String.
				This AVP contains an identifier that helps the home network identify
				the visited network (e.g., the visited network domain name), in order
				to authorize roaming to that visited network.
				
			*/

			struct dict_avp_data data = { 
					386, 					/* Code */
					0, 					/* Vendor */
					"SIP-Visited-Network-Id", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		/* SIP-User-Authorization-Type */
		{
			/*
				The SIP-User-Authorization-Type AVP (AVP Code 387) is of type
				Enumerated and indicates the type of user authorization being
				performed in a User Authorization operation, i.e., the Diameter
				User-Authorization-Request (UAR) command.  The following values are
				defined:

				o  REGISTRATION (0)
				This value is used for initial registration or re-registration.
				This is the default value.

				o  DEREGISTRATION (1)
				This value is used for deregistration.

				o  REGISTRATION_AND_CAPABILITIES (2)
				This value is used for initial registration or re-registration
				when the SIP server explicitly requests the Diameter server to get
				capability information.  This capability information helps the SIP
				server to allocate another SIP server to serve the user.
				
			*/

			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(SIP-User-Authorization-Type)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "REGISTRATION"),
				enumval_def_u32( 1, "DEREGISTRATION"),
				enumval_def_u32( 2, "REGISTRATION_AND_CAPABILITIES")
			};
			struct dict_avp_data data = { 
					387, 					/* Code */
					0, 					/* Vendor */
					"SIP-User-Authorization-Type", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
			};
			int i;
			/* Create the Enumerated type, enumerated values, and the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		/* SIP-Supported-User-Data-Type */
		{
			/*
				The SIP-Supported-User-Data-Type AVP (AVP Code 388) is of type
				UTF8String and contains a string that identifies the type of
				supported user data (user profile, see SIP-User-Data AVP
				(Section 9.12)) supported in the node.  The AVP can be repeated, if
				the SIP server supports several user data types.  In case of
				repetition, the Diameter client should order the different instances
				of this AVP according to its preferences.

				When the Diameter client inserts this AVP in a SAR message, it allows
				the Diameter client to provide an indication to the Diameter server
				of the types of user data supported by the SIP server.  The Diameter
				server, upon inspection of these AVPs, will return a suitable
				SIP-User-Data AVP (Section 9.12) of the type indicated in the
				SIP-User-Data-Type AVP (Section 9.12.1).
				
			*/

			struct dict_avp_data data = { 
					388, 					/* Code */
					0, 					/* Vendor */
					"SIP-Supported-User-Data-Type", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		/* SIP-User-Data-Type */
		{
			/*
				The SIP-User-Data-Type AVP (AVP Code 390) is of type UTF8String and
				contains a string that identifies the type of user data included in
				the SIP-User-Data-Type AVP (Section 9.12).

				This document does not specify a convention to characterize the type
				of user data contained in the SIP-User-Data-Type AVP (Section 9.12).  It
				is believed that in most cases this feature will be used in
				environments controlled by a network administrator who can configure
				both the client and server to assign the same value type at the
				client and server.  It is also RECOMMENDED that organizations
				developing their own profile of SIP-User-Data-Type AVP (Section 9.12)
				allocate a type based on their canonical DNS name.  For instance,
				organization "example.com" can define several types of SIP-User-Data
				and allocate the types "type1.dsa.example.com",
				"type2.dsa.example.com", and so on.  This convention will avoid a
				clash in the allocation of types of SIP-User-Data-Type AVP (Section 9.12).
				
			*/

			struct dict_avp_data data = { 
					390, 					/* Code */
					0, 					/* Vendor */
					"SIP-User-Data-Type", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		/* SIP-User-Data-Contents */
		{
			/*
				The SIP-User-Data-Contents AVP (AVP Code 391) is of type OctetString.
				The Diameter peers do not need to understand the value of this AVP.

				The AVP contains the user profile data required for a SIP server to
				give service to the user.

				
			*/

			struct dict_avp_data data = { 
					391, 					/* Code */
					0, 					/* Vendor */
					"SIP-User-Data-Contents", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* SIP-User-Data */
		{
			/*
				The SIP-User-Data AVP (AVP Code 389) is of type Grouped.  This AVP
				allows the Diameter server to transport user-specific data, such as a
				user profile, to the SIP server (in the Diameter client).  The
				Diameter server selects a type of user data that is understood by the
				SIP server in the Diameter client, and has been indicated in a
				SIP-Supported-User-Data-Type AVP.  In case the Diameter client
				indicated support for several types of user data, the Diameter server
				SHOULD choose the first type supported by the client.

				The SIP-User-Data grouped AVP contains a SIP-User-Data-Type AVP that
				indicates the type of user data included in the
				SIP-User-Data-Contents-AVP.

				The SIP-User-Data AVP is defined as follows (per the grouped-avp-def
				of RFC 3588 [RFC3588]):


				SIP-User-Data ::= < AVP Header: 389 >
						{ SIP-User-Data-Type }
						{ SIP-User-Data-Contents }
					      * [ AVP ]
				
			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					389, 					/* Code */
					0, 					/* Vendor */
					"SIP-User-Data", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "SIP-User-Data-Type", 	RULE_REQUIRED, -1, 1 }
							,{  "SIP-User-Data-Contents",	RULE_REQUIRED, -1, 1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		/* SIP-User-Data-Already-Available */
		{
			/*
				The SIP-User-Data-Already-Available AVP (AVP Code 392) is of type
				Enumerated and gives an indication to the Diameter server about
				whether the Diameter client (SIP server) already received the portion
				of the user profile needed in order to serve the user.  The following
				values are defined:

				o  USER_DATA_NOT_AVAILABLE (0)
				The Diameter client (SIP server) does not have the data that it
				needs to serve the user.

				o  USER_DATA_ALREADY_AVAILABLE (1)
				The Diameter client (SIP server) already has received the data
				that it needs to serve the user.

	
			*/

			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(SIP-User-Data-Already-Available)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "USER_DATA_NOT_AVAILABLE"),
				enumval_def_u32( 1, "USER_DATA_ALREADY_AVAILABLE")
			};
			struct dict_avp_data data = { 
					392, 					/* Code */
					0, 					/* Vendor */
					"SIP-User-Data-Already-Available", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
			};
			int i;
			/* Create the Enumerated type, enumerated values, and the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		/* SIP-Method */
		{
			/*
				The SIP-Method-AVP (AVP Code 393) is of type UTF8String and contains
				the method of the SIP request that triggered the Diameter message.
				The Diameter server MUST use this AVP solely for authorization of SIP
				requests, and MUST NOT use it to compute the Digest authentication.
				To compute the Digest authentication, the Diameter server MUST use
				the Digest-Method AVP instead.

				
			*/

			struct dict_avp_data data = { 
					393, 					/* Code */
					0, 					/* Vendor */
					"SIP-Method", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		/* Complement of Result-Code AVP values */
		{
			struct dict_object * resultcode_data_type;
			CHECK_dict_search(DICT_TYPE,TYPE_BY_NAME,"Enumerated*(Result-Code)",&resultcode_data_type);
		

			{

				/* Success */
				{
					/* 2003 */
					{
						/*
							The user was not previously registered.  The Diameter server has
							now authorized the registration.
						*/
						struct dict_enumval_data 	error_code = { "DIAMETER_FIRST_REGISTRATION", 	{ .u32 = 2003 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 2004 */
					{
						/*
							The user is already registered.  The Diameter server has now
							authorized the re-registration.

						*/
						struct dict_enumval_data 	error_code = { "DIAMETER_SUBSEQUENT_REGISTRATION", 	{ .u32 = 2004 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 2005 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_UNREGISTERED_SERVICE", 	{ .u32 = 2005 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 2006 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_SUCCESS_SERVER_NAME_NOT_STORED", 	{ .u32 = 2006 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 2007 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_SERVER_SELECTION", 	{ .u32 = 2007 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 2008 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_SUCCESS_AUTH_SENT_SERVER_NOT_STORED", 	{ .u32 = 2008 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
				}
				/* Transient Failures */
				{
					/* 4013 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_USER_NAME_REQUIRED", 	{ .u32 = 4013 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
				}
				/* Permanent Failures */
				{
					/* 5032 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_USER_UNKNOWN", 	{ .u32 = 5032 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 5033 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_IDENTITIES_DONT_MATCH", 	{ .u32 = 5033 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 5034 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_IDENTITY_NOT_REGISTERED", 	{ .u32 = 5034 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 5035 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_ROAMING_NOT_ALLOWED", 	{ .u32 = 5035 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 5036 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_IDENTITY_ALREADY_REGISTERED", 	{ .u32 = 5036 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 5037 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_AUTH_SCHEME_NOT_SUPPORTED", 	{ .u32 = 5037 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 5038 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_IN_ASSIGNMENT_TYPE", 	{ .u32 = 5038 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 5039 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_TOO_MUCH_DATA", 	{ .u32 = 5039 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
					/* 5040 */
					{
						struct dict_enumval_data 	error_code = { "DIAMETER_ERROR_NOT SUPPORTED_USER_DATA", 	{ .u32 = 5040 }};
						CHECK_dict_new( DICT_ENUMVAL, &error_code , resultcode_data_type, NULL);
					}
				}
			}
		

		}   
	}
	/* Command section */
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

			<UAR> ::= < Diameter Header: 283, REQ, PXY >
				 < Session-Id >
				 { Auth-Application-Id }
				 { Auth-Session-State }
				 { Origin-Host }
				 { Origin-Realm }
				 { Destination-Realm }
				 { SIP-AOR }
				 [ Destination-Host ]
				 [ User-Name ]
				 [ SIP-Visited-Network-Id ]
				 [ SIP-User-Authorization-Type ]
			       * [ Proxy-Info ]
			       * [ Route-Record ]
			       * [ AVP ]

			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					283, 					/* Code */
					"User-Authorization-Request", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 			/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Auth-Session-State", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Realm", 			RULE_REQUIRED,   -1, 1 }
						 	,{  "Destination-Realm", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "SIP-AOR", 				RULE_REQUIRED,   -1, 1 }
						 	,{  "Destination-Host", 		RULE_OPTIONAL,   -1, 1 }
						 	,{  "User-Name", 			RULE_OPTIONAL,   -1, 1 }
						 	,{  "SIP-Visited-Network-Id", 		RULE_OPTIONAL,   -1, 1 }
						 	,{  "SIP-User-Authorization-Type", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "Proxy-Info", 			RULE_OPTIONAL,   -1, -1 }
						 	,{  "Route-Record", 			RULE_OPTIONAL,   -1, -1 }

						};
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			struct dict_cmd_data data = { 
					283, 					/* Code */
					"User-Authorization-Answer", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
					CMD_FLAG_PROXIABLE 						/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 		RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Auth-Application-Id", 	RULE_REQUIRED,   -1, 1 }
						 	,{  "Auth-Session-State", 	RULE_REQUIRED,   -1, 1 }
						 	,{  "Result-Code", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Realm", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "SIP-Server-URI", 		RULE_OPTIONAL,   -1, 1 }
						 	,{  "SIP-Server-Capabilities", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "Authorization-Lifetime", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "Auth-Grace-Period", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "Redirect-Host", 		RULE_OPTIONAL,   -1, 1 }
						 	,{  "Redirect-Host-Usage", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "Redirect-Max-Cache-Time", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
						 	,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

						};
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
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
			struct dict_cmd_data data = { 
					286, 					/* Code */
					"Multimedia-Auth-Request", 		/* Name */
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
						 	,{  "SIP-Method", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Destination-Host", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "User-Name", 		RULE_OPTIONAL,   -1, 1 }
						 	,{  "SIP-Server-URI", 		RULE_OPTIONAL,   -1, 1 }
						 	,{  "SIP-Number-Auth-Items", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "SIP-Auth-Data-Item", 	RULE_OPTIONAL,   -1, 1 }
						 	,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
						 	,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

						};
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			struct dict_cmd_data data = { 
					287, 					/* Code */
					"Registration-Termination-Request", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 		RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Auth-Application-Id", 	RULE_REQUIRED,   -1, 1 }
						 	,{  "Auth-Session-State", 	RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Realm", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Destination-Host", 	RULE_REQUIRED,   -1, 1 }
						 	,{  "SIP-Deregistration-Reason",RULE_REQUIRED,   -1, 1 }	
						 	,{  "Destination-Realm",	RULE_OPTIONAL,   -1, 1 }
						 	,{  "User-Name", 		RULE_OPTIONAL,   -1, 1 }
						 	,{  "SIP-AOR", 			RULE_REQUIRED,   -1, -1 }
						 	,{  "Proxy-Info", 		RULE_OPTIONAL,   -1, -1 }
						 	,{  "Route-Record", 		RULE_OPTIONAL,   -1, -1 }

						};
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
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
			
			CHECK_dict_new( DICT_COMMAND, &data , sip, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
	}
	
	LOG_D( "Extension 'Dictionary definitions for SIP' initialized");
	return 0;
}
EXTENSION_ENTRY("dict_sip", ds_dict_init);
