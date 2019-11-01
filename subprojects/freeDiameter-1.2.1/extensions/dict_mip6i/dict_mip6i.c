/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Francois Bard <francois@tera.ics.keio.ac.jp>							 *
*													 *
* Copyright (c) 2010, Teraoka Laboratory, Keio University						 *
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

/*

The following table complete the one in RFC 5778, page 18. The AVPs are implemented below following the order of this table.
We try to keep the structure of the grouped AVP by declaring the contained AVPs just before the grouped AVP they depend on.
The number of '+' indicates the depth of the contained AVP.

DEPTH	NAME					AVP	RFC		TYPE			NOTES
	
	MIP6-Feature-Vector			124	5447		Unsigned64
	MIP-Mobile-Node-Address			333	4004		Address
+	MIP-Home-Agent-Address			334	4004		Address
++	Destination-Host			293	3588		DiameterIdentity
++	Destination-Realm			283	3588		DiameterIdentity
+	MIP-Home-Agent-Host			348	4004		Grouped
+	MIP6-Home-Link-Prefix			125	5447		OctetString
	MIP6-Agent-Info				486	5447		Grouped
	User-Name				1	3588		UTF8String
	Service-Selection			493	5778		UTF8String
+	MIP-Replay-Mode				346	4004		Enumerated
+	MIP-Algorithm-Type			345	4004		Enumerated
+	MIP-MN-HA-SPI				491	5778		Unsigned32
+	MIP-MSA-Lifetime			367	4004		Unsigned32
+	MIP-Session-Key				343	4004		OctetString
	MIP-MN-HA-MSA				492	5778		Grouped			different from MIP-MN-to-HA-MSA (331)
	Chargeable-User-Identity		89	5778		OctetString

QoS AVPs (RFC 5777 - implemented in the corresponding dictionary)

	QoS-Capability				578	5777		Grouped
	QoS-Resources				508	5777		Grouped

ACCOUNTING AVPs (section 6.21)

	Accounting-Input-Octets			363	4004, 4005	Unsigned64
	Accounting-Output-Octets		364	4004, 4005	Unsigned64
	Accounting-Input-Packets		365	4004, 4005	Unsigned64
	Accounting-Output-Packets		366	4004, 4005	Unsigned64
	Acct-Multi-Session-Id			50	3588		UTF8String
	Acct-Session-Time			46	2866, 4004	Unsigned32
	MIP6-Feature-Vector			----------------------------------
	MIP-Mobile-Node-Address			----------------------------------
	MIP6-Agent-Info				----------------------------------
	Chargeable-User-Identity		----------------------------------
	Service-Selection			----------------------------------
	QoS-Resources				----------------------------------
	QoS-Capability				----------------------------------
	MIP-Careof-Address			487	5778		Address			needed (appears in MIR/MIA)

REST OF THE AVPs IN THE MIR & MIA EXCLUDING *[AVP] (as written on page 19 of RFC 5778)

MIP6-Request

	Session-ID				263	3588
	Auth-Application-Id			258	3588
	User-Name				1	3588
	Destination-Realm			283	3588
	Origin-Host				264	3588
	Origin-Realm				296	3588
	Auth-Request-Type			274	3588
	Origin-State-Id				278	3588
	NAS-Identifier				32	2865		radius (see avp)        needed (radius)
	NAS-IP-Address				4	2865            radius (see avp)        needed (radius)
	NAS-IPv6-Address			95	3162            radius (see avp)        needed (radius)
	NAS-Port-Type				61	2865            radius (see avp) 	needed (radius)
	Called-Station-Id			30	2865            radius (see avp) 	needed (radius)
	Calling-Station-Id			31	2865            radius (see avp)  	needed (radius)
	MIP6-Feature-Vector			------------
	MIP6-Auth-Mode				494	5778		Enumerated		needed (mip6a)
	MIP-MN-AAA-SPI				341	5778		Unsigned32		needed (mip6a)
	MIP-MN-HA-SPI				------------
	MIP-Mobile-Node-Address			------------
	MIP6-Agent-Info				------------
	MIP-Careof-Address			------------
	MIP-Authenticator			488	5778		OctetString		needed (mip6a)
	MIP-MAC-Mobility-Data			489	5778		OctetString		needed (mip6a)
	MIP-Timestamp				490	5778		OctetString		needed (mip6a)
	QoS-Capability				------------
	QoS-Resources				------------
	Chargeable-User-Identity		------------
	Service-Selection			------------
	Authorization-Lifetime			291	3588
	Auth-Session-State			277	3588
	Proxy-Info				284	3588
	Route-Record				282	3588

MIP6-Answer - All of them are already implemented as base protocol AVPs or implemented earlier.

*/

/****************************************************************************************************************************************
*																	*
* This table is a copy of the registry named "MIP6 Authentication Mode Registry" and managed by IANA.					*
* source : http://www.iana.org/assignments/aaa-parameters/aaa-parameters.txt								*
* up to date on october 2010														*
*																	*
*      Value          Token       Reference												*
*        0       Reserved         [RFC5778]												*
*        1       MIP6_AUTH_MN_AAA [RFC5778]												*
*   2-4294967295 Unassigned														*
*																	*
****************************************************************************************************************************************/


/*

NOTES

check for omissions !

*/

#include <freeDiameter/extension.h>

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



/* Defines if there are any */

//New Result-Code for MIP (RFC5778, Section 7.*)
#define DIAMETER_SUCCESS_RELOCATE_HA 2009
#define DIAMETER_ERROR_MIP6_AUTH_MODE 5041

//others
#define MIP6_AUTH_MN_AAA 1

/* Dictionary */

int dict_mip6i_init(char * conffile)
{
	struct dict_object * mip6i;
	{
		struct dict_application_data data  = { 	7, "Diameter Mobile IPv6 IKE (MIP6I) Application" };
		CHECK_dict_new( DICT_APPLICATION, &data , NULL, &mip6i);
	}

	/***************/
	/* AVP section */
	/***************/
	{
		/* Loading all the derived data formats */

		struct dict_object * Address_type;
		struct dict_object * UTF8String_type;
		struct dict_object * DiameterIdentity_type;
		struct dict_object * DiameterURI_type;
		struct dict_object * Time_type;

		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Address", &Address_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterIdentity", &DiameterIdentity_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterURI", &DiameterURI_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Time", &Time_type);

		/* MIP6-Feature-Vector - RFC 5447 */
		{
			/*

			*/

			struct dict_avp_data data = {
					124, 					/* Code */
					0, 					/* Vendor */
					"MIP6-Feature-Vector", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP-Mobile-Node-Address - RFC 4004 */
		{
			/*

			*/

			struct dict_avp_data data = {
					333, 					/* Code */
					0, 					/* Vendor */
					"MIP-Mobile-Node-Address",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* MIP-Home-Agent-Address - RFC 4004 */
		{
			/*

			*/

			struct dict_avp_data data = {
					334, 					/* Code */
					0, 					/* Vendor */
					"MIP-Home-Agent-Address",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* Destination-Host - Base Protocol */

			/*
			Implemented in the base protocol
			*/

		/* Destination-Realm - Base Protocol */

			/*
			Implemented in the base protocol
			*/

		/* MIP-Home-Agent-Host - RFC 4004 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					348, 					/* Code */
					0, 					/* Vendor */
					"MIP-Home-Agent-Host",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "Destination-Realm",	RULE_REQUIRED,	-1, 1 }
							,{  "Destination-Host",		RULE_REQUIRED,	-1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* MIP6-Home-Link-Prefix - RFC 5447 */
		{
			/*

			*/

			struct dict_avp_data data = {
					125, 					/* Code */
					0, 					/* Vendor */
					"MIP6-Home-Link-Prefix", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP6-Agent-Info - RFC 5447 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					486, 					/* Code */
					0, 					/* Vendor */
					"MIP6-Agent-Info",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "MIP-Home-Agent-Address", 		RULE_OPTIONAL, -1, 2 }
							,{  "MIP-Home-Agent-Host",		RULE_OPTIONAL, -1, 1 }
							,{  "MIP6-Home-Link-Prefix",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* User-Name - RFC 3588 */

			/*
			Implemented in the base protocol
			*/

		/* Service-Selection - RFC 5778 */
		{
			/*

			*/

			struct dict_avp_data data = {
					493, 					/* Code */
					0, 					/* Vendor */
					"Service-Selection",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* MIP-Replay-Mode - RFC 5778 */
		{
			/*

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(MIP-Replay-Mode)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_1 = { "None",		{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "Timestamp", 	{ .i32 = 2 }};

			struct dict_avp_data 	data = {
					346, 					/* Code */
					0, 					/* Vendor */
					"MIP-Replay-Mode",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* MIP-Algorithm-Type - RFC 5778 */
		{
			/*
			The MIP-Algorithm-Type AVP (AVP Code 345) is of type Enumerated and
			contains the Algorithm identifier for the associated Mobile IPv6
			MN-HA Authentication Option.  The Diameter server selects the
			algorithm type.  Existing algorithm types are defined in [RFC4004]
			that also fulfill current RFC 4285 requirements. This AVP is reused
			from [RFC4004].
			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(MIP-Algorithm-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_2 = { "HMAC-SHA-1 [HMAC]",	{ .i32 = 2 }};

			struct dict_avp_data 	data = {
					345, 					/* Code */
					0, 					/* Vendor */
					"MIP-Algorithm-Type",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* MIP-MN-HA-SPI - RFC 5778 */
		{
			/*

			*/
			struct dict_avp_data data = {
					491, 					/* Code */
					0, 					/* Vendor */
					"MIP-MN-HA-SPI",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP-MSA-Lifetime - RFC 5778 */
		{
			/*

			*/
			struct dict_avp_data data = {
					367, 					/* Code */
					0, 					/* Vendor */
					"MIP-MSA-Lifetime",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP-Session-Key - RFC 5778 */
		{
			/*

			*/
			struct dict_avp_data data = {
					343, 					/* Code */
					0, 					/* Vendor */
					"MIP-Session-Key",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP-MN-HA-MSA - RFC 5778 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					492, 					/* Code */
					0, 					/* Vendor */
					"MIP-MN-HA-MSA",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "MIP-Session-Key", 		RULE_REQUIRED, -1, 1 }
							,{  "MIP-MSA-Lifetime", 	RULE_REQUIRED, -1, 1 }
							,{  "MIP-MN-HA-SPI", 		RULE_OPTIONAL, -1, 1 }
							,{  "MIP-Algorithm-Type", 	RULE_OPTIONAL, -1, 1 }
							,{  "MIP-Replay-Mode", 		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Chargeable-User-Identity - RFC 5778 */
		{
			/*

			*/
			struct dict_avp_data data = {
					89, 					/* Code */
					0, 					/* Vendor */
					"Chargeable-User-Identity",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

	//////////////
	/* QoS AVPs */
	//////////////

		/* QoS-Capability */

			/*
			Implemented in RFC 5777
			*/

		/* QoS-Resources */

			/*
			Implemented in RFC 5777
			*/

	/////////////////////
	/* Accounting AVPs */
	/////////////////////

		/* Accounting-Input-Octets - RFC 4004 */
		{
			/*

			*/
			struct dict_avp_data data = {
					363, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Input-Octets",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Accounting-Output-Octets - RFC 4004 */
		{
			/*

			*/
			struct dict_avp_data data = {
					364, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Output-Octets",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Accounting-Input-Packets - RFC 4004 */
		{
			/*

			*/
			struct dict_avp_data data = {
					365, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Input-Packets",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Accounting-Output-Packets - RFC 4004 */
		{
			/*

			*/
			struct dict_avp_data data = {
					366, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Output-Packets",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Acct-Multi-Session-Id - RFC 3588 */

			/*
			Implemented in the base protocol
			*/

		/* Acct-Session-Time - RFC 4004 */
		{
			/*

			*/
			struct dict_avp_data data = {
					46, 					/* Code */
					0, 					/* Vendor */
					"Acct-Session-Time",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP-Careof-Address - RFC 5778 */
		{
			/*
			This AVP is needed in MIP6I at least for implementation reasons
			(it appears in a command)
			*/

			struct dict_avp_data data = {
					487, 					/* Code */
					0, 					/* Vendor */
					"MIP-Careof-Address",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

	/////////////////////////////////////
	/* Radius AVPs used in MIR & MIA */
	/////////////////////////////////////

	/*
	We used the following correspondences for determining the type of the Radius AVPs

		Radius		Diameter

		text		UTF8Sting
		string		OctetString
		address		Address
		integer		Unsigned32
		time		Time
	*/


		/* NAS-Identifier				32	3575 */
		{
			/*
			string -> OctetString
			*/

			struct dict_avp_data data = {
					32, 					/* Code */
					0, 					/* Vendor */
					"NAS-Identifier",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* NAS-IP-Address				4	3575 */
		{
			/*
			address -> Address
			*/

			struct dict_avp_data data = {
					4, 					/* Code */
					0, 					/* Vendor */
					"NAS-IP-Address",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* NAS-IPv6-Address			95	3162 */
		{
			/*
			address -> Address
			*/

			struct dict_avp_data data = {
					95, 					/* Code */
					0, 					/* Vendor */
					"NAS-IPv6-Address",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* NAS-Port-Type			61	2865 */
		{
			/*
			integer -> Unsigned32

			   Value

			      The Value field is four octets.  "Virtual" refers to a connection
			      to the NAS via some transport protocol, instead of through a
			      physical port.  For example, if a user telnetted into a NAS to
			      authenticate himself as an Outbound-User, the Access-Request might
			      include NAS-Port-Type = Virtual as a hint to the RADIUS server
			      that the user was not on a physical port.

			      0       Async
			      1       Sync
			      2       ISDN Sync
			      3       ISDN Async V.120
			      4       ISDN Async V.110
			      5       Virtual
			      6       PIAFS
			      7       HDLC Clear Channel
			      8       X.25
			      9       X.75
			      10      G.3 Fax
			      11      SDSL - Symmetric DSL
			      12      ADSL-CAP - Asymmetric DSL, Carrierless Amplitude Phase
				      Modulation
			      13      ADSL-DMT - Asymmetric DSL, Discrete Multi-Tone
			      14      IDSL - ISDN Digital Subscriber Line
			      15      Ethernet
			      16      xDSL - Digital Subscriber Line of unknown type
			      17      Cable
			      18      Wireless - Other
			      19      Wireless - IEEE 802.11

			      PIAFS is a form of wireless ISDN commonly used in Japan, and
			      stands for PHS (Personal Handyphone System) Internet Access Forum
			      Standard (PIAFS).
			*/

			struct dict_avp_data data = {
					61, 					/* Code */
					0, 					/* Vendor */
					"NAS-Port-Type",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Called-Station-Id			30	2865 */
		{
			/*
			string -> OctetString
			*/

			struct dict_avp_data data = {
					30, 					/* Code */
					0, 					/* Vendor */
					"Called-Station-Id",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		/* Calling-Station-Id			31	2865 */
		{
			/*
			string -> OctetString
			*/

			struct dict_avp_data data = {
					31, 					/* Code */
					0, 					/* Vendor */
					"Calling-Station-Id",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

	///////////////////////////////////////////////////////////
	/* REST OF THE AVP IN THE MIR AND MIA EXCLUDING *[AVP]   */
	///////////////////////////////////////////////////////////

		/* MIP6-Auth-Mode - RFC 5778 */
		{
			/*

			*/
			struct dict_object * 	 type;
			struct dict_type_data 	 tdata = { AVP_TYPE_INTEGER32, "Enumerated(MIP6-Auth-Mode)", NULL, NULL, NULL };
			struct dict_enumval_data t_0 = { "None", 	{ .i32 = MIP6_AUTH_MN_AAA }};
			struct dict_avp_data data = {
					494, 					/* Code */
					0, 					/* Vendor */
					"MIP6-Auth-Mode",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* MIP-MN-AAA-SPI - RFC 5778 */
		{
			/*

			*/

			struct dict_avp_data data = {
					341, 					/* Code */
					0, 					/* Vendor */
					"MIP-MN-AAA-SPI",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP-Authenticator - RFC 5778 */
		{
			/*

			*/
			struct dict_avp_data data = {
					488, 					/* Code */
					0, 					/* Vendor */
					"MIP-Authenticator",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP-MAC-Mobility-Data - RFC 5778 */
		{
			/*

			*/
			struct dict_avp_data data = {
					489, 					/* Code */
					0, 					/* Vendor */
					"MIP-MAC-Mobility-Data",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MIP-Timestamp - RFC 5778 */
		{
			/*

			*/
			struct dict_avp_data data = {
					490, 					/* Code */
					0, 					/* Vendor */
					"MIP-Timestamp",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
	}

/*******************/
/* Command section */
/*******************/

	{
		/* Diameter-EAP-Request (DER) */

			/*
			Defined in dict_eap
			*/

		/* Diameter-EAP-Answer (DEA) - COPIED FROM dict_eap.c, SEE IT FOR MORE REFERENCE */

			/*
			Defined in dict_eap
			*/

		/* MIP6-Request (MIR) */
		{
			/*

			The MIP6-Request (MIR), indicated by the Command-Code field set to
			325 and the 'R' bit set in the Command Flags field, is sent by the
			HA, acting as a Diameter client, in order to request the
			authentication and authorization of an MN.

			Although the HA provides the Diameter server with replay protection-
			related information, the HA is responsible for the replay protection.

			The message format is shown below.

			<MIP6-Request> ::= < Diameter Header: 325, REQ, PXY >
					   < Session-ID >
					   { Auth-Application-Id }
					   { User-Name }
					   { Destination-Realm }
					   { Origin-Host }
					   { Origin-Realm }
					   { Auth-Request-Type }
					   [ Destination-Host ]
					   [ Origin-State-Id ]
					   [ NAS-Identifier ]
					   [ NAS-IP-Address ]
					   [ NAS-IPv6-Address ]
					   [ NAS-Port-Type ]
					   [ Called-Station-Id ]
					   [ Calling-Station-Id ]
					   [ MIP6-Feature-Vector ]
					   { MIP6-Auth-Mode }
					   [ MIP-MN-AAA-SPI ]
					   [ MIP-MN-HA-SPI ]
					1*2{ MIP-Mobile-Node-Address }
					   { MIP6-Agent-Info }
					   { MIP-Careof-Address }
					   [ MIP-Authenticator ]
					   [ MIP-MAC-Mobility-Data ]
					   [ MIP-Timestamp ]
					   [ QoS-Capability ]
					 * [ QoS-Resources ]
					   [ Chargeable-User-Identity ]
					   [ Service-Selection ]
					   [ Authorization-Lifetime ]
					   [ Auth-Session-State ]
					 * [ Proxy-Info ]
					 * [ Route-Record ]
					 * [ AVP ]

			If the MN is both authenticated and authorized for the mobility
			service, then the Auth-Request-Type AVP is set to the value
			AUTHORIZE_AUTHENTICATE.  This is the case when the MIP6-Auth-Mode is
			set to the value MIP6_AUTH_MN_AAA.

			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = {
					325, 					/* Code */
					"MIP6-Request", 			/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
					};
			struct local_rules_definition rules[] =
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
							,{  "User-Name", 			RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Realm", 		RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm", 			RULE_REQUIRED,   -1, 1 }
							,{  "Auth-Request-Type", 		RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Host", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",	 		RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Identifier", 			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IP-Address", 			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IPv6-Address", 		RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Type", 			RULE_OPTIONAL,   -1, 1 }
							,{  "Called-Station-Id", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Calling-Station-Id", 		RULE_OPTIONAL,   -1, 1 }
							,{  "MIP6-Feature-Vector", 		RULE_OPTIONAL,   -1, 1 }
							,{  "MIP6-Auth-Mode", 			RULE_OPTIONAL,   -1, 1 }
							,{  "MIP-MN-AAA-SPI", 			RULE_OPTIONAL,   -1, 1 }
							,{  "MIP-MN-HA-SPI", 			RULE_OPTIONAL,   -1, 1 }
							,{  "MIP-Mobile-Node-Address", 		RULE_OPTIONAL,    1, 2 }
							,{  "MIP6-Agent-Info", 			RULE_OPTIONAL,   -1, 1 }
							,{  "MIP-Careof-Address", 		RULE_OPTIONAL,   -1, 1 }
							,{  "MIP-Authenticator", 		RULE_OPTIONAL,   -1, 1 }
							,{  "MIP-MAC-Mobility-Data", 		RULE_OPTIONAL,   -1, 1 }
							,{  "MIP-Timestamp", 			RULE_OPTIONAL,   -1, 1 }
							,{  "QoS-Capability", 			RULE_OPTIONAL,   -1, 1 }
							,{  "QoS-Resources", 			RULE_OPTIONAL,   -1, -1 }
							,{  "Chargeable-User-Identity", 	RULE_OPTIONAL,   -1, 1 }
							,{  "Service-Selection", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Authorization-Lifetime", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Auth-Session-State", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info", 			RULE_OPTIONAL,   -1, -1 }
							,{  "Route-Record", 			RULE_OPTIONAL,   -1, -1 }
						};

			CHECK_dict_new( DICT_COMMAND, &data, mip6i, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* MIP6-Answer (MIA) */
		{
			/*

			The MIP6-Answer (MIA) message, indicated by the Command-Code field
			set to 325 and the 'R' bit cleared in the Command Flags field, is
			sent by the Diameter server in response to the MIP6-Request message.

			The User-Name AVP MAY be included in the MIA if it is present in the
			MIR.  The Result-Code AVP MAY contain one of the values defined in
			Section 7, in addition to the values defined in [RFC3588].

			An MIA message with the Result-Code AVP set to DIAMETER_SUCCESS MUST
			include the MIP-Mobile-Node-Address AVP.

			The message format is shown below.

			<MIP6-Answer> ::= < Diameter Header: 325, PXY >
					  < Session-Id >
					  { Auth-Application-Id }
					  { Result-Code }
					  { Origin-Host }
					  { Origin-Realm }
					  { Auth-Request-Type }
					  [ User-Name ]
					  [ Authorization-Lifetime ]
					  [ Auth-Session-State ]
					  [ Error-Message ]
					  [ Error-Reporting-Host ]
					  [ Re-Auth-Request-Type ]
					  [ MIP6-Feature-Vector ]
					  [ MIP6-Agent-Info ]
					*2[ MIP-Mobile-Node-Address ]
					  [ MIP-MN-HA-MSA ]
					* [ QoS-Resources ]
					  [ Chargeable-User-Identity ]
					  [ Service-Selection ]
					  [ Origin-State-Id ]
					* [ Proxy-Info ]
					* [ Redirect-Host ]
					  [ Redirect-Host-Usage ]
					  [ Redirect-Max-Cache-Time ]
					* [ Failed-AVP ]
					* [ AVP ]

			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = {
					325, 					/* Code */
					"MIP6-Answer", 				/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
					CMD_FLAG_PROXIABLE 			/* Fixed flag values */
					};
			struct local_rules_definition rules[] =
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
							,{  "Result-Code", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm", 			RULE_REQUIRED,   -1, 1 }
							,{  "Auth-Request-Type", 		RULE_REQUIRED,   -1, 1 }
							,{  "User-Name", 			RULE_OPTIONAL,   -1, 1 }
							,{  "Authorization-Lifetime", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Auth-Session-State", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Message", 			RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Reporting-Host", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Re-Auth-Request-Type", 		RULE_OPTIONAL,   -1, 1 }
							,{  "MIP6-Feature-Vector", 		RULE_OPTIONAL,   -1, 1 }
							,{  "MIP6-Agent-Info", 			RULE_OPTIONAL,   -1, 1 }
							,{  "MIP-Mobile-Node-Address", 		RULE_OPTIONAL,   -1, 2 }
							,{  "MIP-MN-HA-MSA", 			RULE_OPTIONAL,   -1, 1 }
							,{  "QoS-Resources", 			RULE_OPTIONAL,   -1, -1 }
							,{  "Chargeable-User-Identity",		RULE_OPTIONAL,   -1, 1 }
							,{  "Service-Selection", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id", 			RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info", 			RULE_OPTIONAL,   -1, -1 }
							,{  "Redirect-Host", 			RULE_OPTIONAL,   -1, -1 }
							,{  "Redirect-Host-Usage", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Max-Cache-Time", 		RULE_OPTIONAL,   -1, 1 }
							,{  "Failed-AVP", 			RULE_OPTIONAL,   -1, -1 }
						};

			CHECK_dict_new( DICT_COMMAND, &data, mip6i, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
	}

	LOG_D( "Dictionary Extension 'Diameter Mobile IPv6 IKE (MIP6I)' initialized");
	return 0;
}
EXTENSION_ENTRY("dict_mip6i", dict_mip6i_init, "dict_rfc5777");
