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

The following table lists the AVPs needed for the NAS to HAAA server interaction.
We try to keep the structure of the grouped AVP by declaring the contained AVPs just before the grouped AVP they depend on.
The number of '+' indicates the depth of the contained AVP.

DEPTH	NAME					AVP CODE	RFC	TYPE			IMPLEMENTED	NOTES
	
	MIP6-Feature-Vector			124		5447	Unsigned64		yes
+	MIP-Home-Agent-Address			334		4004	Address			not yet
++	Destination-Host			293		3588	DiameterIdentity	no (Base)
++	Destination-Realm			283		3588	DiameterIdentity	no (Base)
+	MIP-Home-Agent-Host			348		4004	Grouped			not yet
+	MIP6-Home-Link-Prefix			125		5447	OctetString		yes
	MIP6-Agent-Info				486		5447	Grouped			yes

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






/* Defines if there are any */

/* Define Flags for MIP6-Feature-Vector*/

#define MIP6_INTEGRATED 0x0000000000000001
#define LOCAL_HOME_AGENT_ASSIGNMENT 0x0000000000000002

/* Dictionary */

int dict_nas_mipv6_init(char * conffile)
{
	struct dict_object * nas_mipv6;
	{
		struct dict_application_data data = { 5447, "MIPv6 NAS-to-HAAA Interaction" };
		CHECK_dict_new( DICT_APPLICATION, &data , NULL, &nas_mipv6);
	}

	/* AVP section */
	{
		/* Loading the derived data formats */

		struct dict_object * Address_type;
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Address", &Address_type);

		/* MIP6-Feature-Vector */
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

		/* Destination-Realm - Base Protocol */	

		/* MIP-Home-Agent-Host - RFC 4004 */
		{
			/*
			The MIP-Home-Agent-Host AVP (AVP Code 348) is of type Grouped and
			contains the identity of the assigned Home Agent.  If the MIP-Home-
			Agent-Host AVP is present in the AMR, the AAAH MUST copy it into the
			HAR.

			 MIP-Home-Agent-Host ::= < AVP Header: 348 >
						  { Destination-Realm }
						  { Destination-Host }
						* [ AVP ]
			*/

			struct dict_object * avp;
			struct dict_avp_data data = {
					348, 					/* Code */
					0, 					/* Vendor */
					"MIP-Home-Agent-Host", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "Destination-Realm", 	RULE_REQUIRED, -1, 1 }
							,{  "Destination-Host",		RULE_REQUIRED, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* MIP6-Home-Link-Prefix */
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

		/* MIP6-Agent-Info */
		{
			/*
			
			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					486, 					/* Code */
					0, 					/* Vendor */
					"MIP6-Agent-Info", 			/* Name */
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
	}

	LOG_D( "Dictionary Extension 'MIPv6 NAS-to-HAAA Interaction' initialized");
	return 0;
}
EXTENSION_ENTRY("dict_nas_mipv6", dict_nas_mipv6_init);
