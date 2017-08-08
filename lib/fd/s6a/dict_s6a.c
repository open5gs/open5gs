/*********************************************************************************************************
* Software License Agreement (BSD License)															 *
* Author: Sebastien ROUX <sebastien.roux@eurecom.fr>							 *
*													 *
* Copyright (c) 2013, Eurecom						 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*   													 *
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
			AVP_BY_NAME_ALL_VENDORS, 									\
			(_rulearray)[__ar].avp_name, 							\
			&__data.rule_avp, 0 ) );							\
		if ( !__data.rule_avp ) {								\
			TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_name );		\
			return ENOENT;									\
		}											\
		CHECK_FCT_DO( fd_dict_new( fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL),	\
			{											\
				TRACE_DEBUG(INFO, "Error on rule with AVP '%s'",	  			\
					 (_rulearray)[__ar].avp_name );			  			\
				return EINVAL;						  			\
			} );								  			\
	}										  			\
}

#define enumval_def_u32( _val_, _str_ ) \
		{ _str_, 		{ .u32 = _val_ }}

#define enumval_def_os( _len_, _val_, _str_ ) \
		{ _str_, 		{ .os = { .data = (unsigned char *)_val_, .len = _len_ }}}

/* Defines if there are any */

/* Dictionary */

int dict_s6a_init(char * conffile)
{
#define VENDOR_3GPP_Id  10415

	TRACE_ENTRY("%p", conffile);

	struct dict_object * s6a_dict;
	{
		struct dict_application_data data = { 16777251, "Diameter S6a 3GPP" };
		CHECK_dict_new( DICT_APPLICATION, &data , NULL, &s6a_dict);
	}

	/* AVP section */
	{
		/* Loading the derived data formats */

		struct dict_object * Time_type;
		struct dict_object * Address_type;
		struct dict_object * UTF8String_type;

		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Time", &Time_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Address", &Address_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);

		/* 3GPP-Charging-Characteristics */
		{
			struct dict_avp_data data = {
				13, 					/* Code */
				0, 					/* Vendor */
				"3GPP-Charging-Characteristics",		/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flag values */
				AVP_TYPE_OCTETSTRING					/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Service-Selection - RFC 5778 */
		{
			struct dict_avp_data data = {
				493, 					/* Code */
				0, 					/* Vendor , EURECOM: with FD 1.2.0 does not work if vendor ID is 0 for this AVP*/
				"Service-Selection",			/* Name */
				AVP_FLAG_MANDATORY, 	/* Fixed flags */
				AVP_FLAG_MANDATORY,	/* Fixed flag values */
				AVP_TYPE_OCTETSTRING 			/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Max-Requested-Bandwidth-DL AVP - 3GPP TS 29.214  #5.3.14 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				515,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Max-Requested-Bandwidth-DL",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Max-Requested-Bandwidth-UL AVP - 3GPP TS 29.214  #5.3.15 */
		{
			struct dict_avp_data data = {
				516,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Max-Requested-Bandwidth-UL",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Visited-Network-Identifier AVP - 3GPP TS 29.229 #6.3.1 */
		{
			struct dict_avp_data data = {
				600,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Visited-Network-Identifier",			/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Confidentiality-Key AVP - 3GPP TS 29.229 #6.3.27 */
		{
			struct dict_avp_data data = {
				625,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Confidentiality-Key",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Integrity-Key AVP - 3GPP TS 29.229 #6.3.28 */
		{
			struct dict_avp_data data = {
				626,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Integrity-Key",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Feature-List-ID AVP - 3GPP TS 29.229 #6.3.29 */
		{
			struct dict_avp_data data = {
				629,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Feature-List-ID",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Feature-List AVP - 3GPP TS 29.229 #6.3.30 */
		{
			struct dict_avp_data data = {
				630,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Feature-List",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MSISDN AVP - 3GPP TS 29.329 #6.3.2 */
		{
			struct dict_avp_data data = {
				701,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"MSISDN",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Served-Party-IP-Address AVP - 3GPP TS 32.299 #7.2.187
		 * The Served-Party-IP-Address AVP (AVP code 848) is of type Address
		 * and holds the IP address of either the calling or called party,
		 * depending on whether the P-CSCF is in touch with the calling or the
		 * called party. This AVP is only provided by the P-CSCF.
		 */
		{
			struct dict_avp_data data = {
				848,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Served-Party-IP-Address",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* QoS-Class-Identifier AVP - 3GPP TS 29.212  #5.3.17 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(QoS-Class-Identifier)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_1		= { "QCI_1",					{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 	= { "QCI_2",					{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 	= { "QCI_3",					{ .i32 = 3 }};
			struct dict_enumval_data 	t_4		= { "QCI_4",					{ .i32 = 4 }};
			struct dict_enumval_data 	t_5 	= { "QCI_5",					{ .i32 = 5 }};
			struct dict_enumval_data 	t_6 	= { "QCI_6",					{ .i32 = 6 }};
			struct dict_enumval_data 	t_7 	= { "QCI_7",					{ .i32 = 7 }};
			struct dict_enumval_data 	t_8 	= { "QCI_8",					{ .i32 = 8 }};
			struct dict_enumval_data 	t_9		= { "QCI_9",					{ .i32 = 9 }};

			struct dict_avp_data data = {
				1028,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"QoS-Class-Identifier",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata, NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_1, avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2, avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3, avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4, avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5, avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6, avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_7, avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_8, avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_9, avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* RAT-Type AVP - 3GPP TS 29.212  #5.3.31 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(RAT-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0		= { "WLAN",						{ .u32 = 0 }};
			struct dict_enumval_data 	t_1		= { "VIRTUAL",					{ .u32 = 1 }};
			struct dict_enumval_data 	t_1000 	= { "UTRAN",					{ .u32 = 1000 }};
			struct dict_enumval_data 	t_1001 	= { "GERAN",					{ .u32 = 1001 }};
			struct dict_enumval_data 	t_1002	= { "GAN",						{ .u32 = 1002 }};
			struct dict_enumval_data 	t_1003 	= { "HSPA_EVOLUTION",			{ .u32 = 1003 }};
			struct dict_enumval_data 	t_1004 	= { "EUTRAN",					{ .u32 = 1004 }};
			struct dict_enumval_data 	t_2000 	= { "CDMA2000_1X",				{ .u32 = 2000 }};
			struct dict_enumval_data 	t_2001 	= { "HRPD",						{ .u32 = 2001 }};
			struct dict_enumval_data 	t_2002	= { "UMB",						{ .u32 = 2002 }};
			struct dict_enumval_data 	t_2003 	= { "EHRPD",					{ .u32 = 2003 }};

			struct dict_avp_data data = {
				1032,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"RAT-Type",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1000 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1001 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1002 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1003 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1004 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2000 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2001 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2002 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2003 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Priority-Level AVP - 3GPP TS 29.272  #5.3.45 */
		{
			struct dict_avp_data data = {
				1046,			  /* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Priority-Level",		  /* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32  /* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Pre-emption-Capability AVP - 3GPP TS 29.212  #5.3.46 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Pre-emption-Capability)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0		= { "PRE-EMPTION_CAPABILITY_ENABLED",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 	= { "PRE-EMPTION_CAPABILITY_DISABLED",		{ .i32 = 1 }};

			struct dict_avp_data data = {
				1047,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Pre-emption-Capability",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Pre-emption-Vulnerability AVP - 3GPP TS 29.212  #5.3.47 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Pre-emption-Vulnerability)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0		= { "PRE-EMPTION_VULNERABILITY_ENABLED",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 	= { "PRE-EMPTION_VULNERABILITY_DISABLED",		{ .i32 = 1 }};

			struct dict_avp_data data = {
				1048,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Pre-emption-Vulnerability",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Allocation-Retention-Priority AVP - 3GPP TS 29.272 #5.3.32 */
		{
			/*

			 Allocation-Retention-Priority ::= < AVP Header: 1034 >
					{ Priority-Level }
					[ Pre-emption-Capability ]
					[ Pre-emption-Vulnerability ]

			 The Allocation-Retention-Priority AVP (AVP code 1034) is of type
			 Grouped, and it is used to indicate the priority of allocation and
			 retention, the pre-emption capability and pre-emption vulnerability
			 for the SDF if provided within the QoS-Information-AVP or for the
			 EPS default bearer if provided within the Default-EPS-Bearer-QoS AVP.
			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
				1034,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Allocation-Retention-Priority",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Priority-Level",				RULE_REQUIRED, -1,  1 }
				,{  "Pre-emption-Capability",		RULE_OPTIONAL, -1,  1 }
				,{  "Pre-emption-Vulnerability",	RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* PDP-Address AVP - 3GPP TS 32.299 #6.3.2 */
		{
			/*
			 * The PDP-Address-Prefix-Length AVP needs not be available for IPv6
			 * typed IP-address prefix length of 64 bits.
			 */
			struct dict_avp_data data = {
				1227,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"PDP-Address",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* Subscriber-Status - 3GPP TS 29.272  #7.3.29 */
		{
			struct dict_object * type;
			struct dict_type_data	 	tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Subscriber-Status)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "SERVICE_GRANTED",					{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "OPERATOR_DETERMINED_BARRING", 		{ .i32 = 1 }};

			struct dict_avp_data data = {
				1424,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Subscriber-Status",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE,	&tdata, NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0,   type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1,   type, NULL);
			CHECK_dict_new( DICT_AVP,	 &data,  NULL, &type);
		}

		/* STN-SR AVP - 3GPP TS 29.272  #7.3.33 */
		{
			struct dict_avp_data data = {
				1433,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"STN-SR",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* ICS-Indicator AVP - 3GPP TS 29.272  #7.3.104 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(ICS-Indicator)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "FALSE",			{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "TRUE",				{ .i32 = 1 }};

			struct dict_avp_data data = {
				1491,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"ICS-Indicator",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* IMEI AVP - 3GPP TS 29.272  #7.3.4 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1402,				/* Code */
                VENDOR_3GPP_Id,					/* Vendor */
				"IMEI",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, &avp);
		}

		/* 3GPP2-MEID AVP - 3GPP TS 29.272  #7.3.6 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1471,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"3GPP2-MEID",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* Software Version - 3GPP TS 29.272  #7.3.5 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1403,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Software-Version",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, &avp);
		}

		/* Terminal Information AVP - 3GPP TS 29.272 #7.3.3 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1401,				/* Code */
                VENDOR_3GPP_Id,					/* Vendor */
				"Terminal-Information",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "IMEI",				RULE_REQUIRED,  1, 1 }
				,{  "3GPP2-MEID",		RULE_REQUIRED,  1, 1 }
				,{  "Software-Version",	RULE_REQUIRED,  1, 1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* ULA-Flags - 3GPP TS 29.272  #7.3.8 */
		{
			struct dict_avp_data data = {
				1406,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"ULA-Flags",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Visited-PLMN-Id AVP - 3GPP TS 29.272  #7.3.9 */
		{
			struct dict_avp_data data = {
				1407,				/* Code */
                VENDOR_3GPP_Id,					/* Vendor */
				"Visited-PLMN-Id",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Number-Of-Requested-Vectors - 3GPP TS 29.272  #7.3.14 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1410,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Number-Of-Requested-Vectors",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Re-Synchronization-Info - 3GPP TS 29.272  #7.3.15 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1411,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Re-Synchronization-Info",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Immediate-Response-Preferred - 3GPP TS 29.272  #7.3.16 */
		{
			struct dict_avp_data data = {
				1412,				/* Code */
				VENDOR_3GPP_Id,     /* Vendor */
				"Immediate-Response-Preferred",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Requested-EUTRAN-Authentication-Info AVP - 3GPP TS 29.272  #7.3.11 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1408,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Requested-EUTRAN-Authentication-Info",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Number-Of-Requested-Vectors", 	RULE_OPTIONAL, -1, 1 }
				,{  "Immediate-Response-Preferred", 	RULE_OPTIONAL, -1, 1 }
				,{  "Re-Synchronization-Info", 		RULE_OPTIONAL, -1, 1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Requested-UTRAN-GERAN-Authentication-Info AVP - 3GPP TS 29.272  #7.3.12 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1409,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Requested-UTRAN-GERAN-Authentication-Info",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Number-Of-Requested-Vectors", 	RULE_OPTIONAL, -1, 1 }
				,{  "Immediate-Response-Preferred", 	RULE_OPTIONAL, -1, 1 }
				,{  "Re-Synchronization-Info", 		RULE_OPTIONAL, -1, 1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Item-Number AVP - 3GPP TS 29.272  #7.3.23 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1419,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Item-Number",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* RAND AVP - 3GPP TS 29.272  #7.3.53 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1447,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"RAND",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* XRES AVP - 3GPP TS 29.272  #7.3.54 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1448,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"XRES",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* AUTN AVP - 3GPP TS 29.272  #7.3.55 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1449,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"AUTN",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* SRES AVP - 3GPP TS 29.272  #7.3.60 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1454,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"SRES",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* KASME AVP - 3GPP TS 29.272  #7.3.56 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1450,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"KASME",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Kc AVP - 3GPP TS 29.272  #7.3.59 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1453,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Kc",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* Trace-Collection-Entity AVP - 3GPP TS 29.272  #7.3.96 */
		{
			struct dict_avp_data data = {
				1452,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Trace-Collection-Entity",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, Address_type, NULL);
		}

		/* E-UTRAN-Vector - 3GPP TS 29.272  #7.3.18 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1414,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"E-UTRAN-Vector",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Item-Number", 		RULE_OPTIONAL, -1,  1 }
				,{  "RAND", 			RULE_REQUIRED, -1,  1 }
				,{  "XRES", 			RULE_REQUIRED, -1,  1 }
				,{  "AUTN", 			RULE_REQUIRED, -1,  1 }
				,{  "KASME", 			RULE_REQUIRED, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* UTRAN-Vector - 3GPP TS 29.272  #7.3.19 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1415,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"UTRAN-Vector",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Item-Number", 			RULE_OPTIONAL, -1,  1 }
				,{  "RAND", 				RULE_REQUIRED, -1,  1 }
				,{  "XRES", 				RULE_REQUIRED, -1,  1 }
				,{  "AUTN", 				RULE_REQUIRED, -1,  1 }
				,{  "Confidentiality-Key", 	RULE_REQUIRED, -1,  1 }
				,{  "Integrity-Key", 		RULE_REQUIRED, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* GERAN-Vector - 3GPP TS 29.272  #7.3.20 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1416,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"GERAN-Vector",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Item-Number", 			RULE_OPTIONAL, -1,  1 }
				,{  "RAND", 				RULE_REQUIRED, -1,  1 }
				,{  "SRES", 				RULE_REQUIRED, -1,  1 }
				,{  "Kc", 					RULE_REQUIRED, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Authentication-Info - 3GPP TS 29.272  #7.3.17 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1413,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Authentication-Info",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "E-UTRAN-Vector", 		RULE_OPTIONAL, -1, -1 }
				,{  "UTRAN-Vector", 		RULE_OPTIONAL, -1, -1 }
				,{  "GERAN-Vector", 		RULE_OPTIONAL, -1, -1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Network-Access-Mode - 3GPP TS 29.272  #7.3.21 */
		{
			struct dict_object * type;
			struct dict_type_data	 	tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Network-Access-Mode)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "PACKET_AND_CIRCUIT",	{ .u32 = 0 }};
			struct dict_enumval_data 	t_1 = { "ONLY_PACKET", 			{ .u32 = 1 }};

			struct dict_avp_data data = {
				1417,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Network-Access-Mode",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* HPLMN-ODB AVP - 3GPP TS 29.272  #7.3.22 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1418,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"HPLMN-ODB",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Cancellation-Type - 3GPP TS 29.272  #7.3.24 */
		{
			struct dict_object * type;
			struct dict_type_data	 	tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Cancellation-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "MME_UPDATE_PROCEDURE",			{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "SGSN_UPDATE_PROCEDURE", 		{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "SUBSCRIPTION_WITHDRAWAL", 		{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "UPDATE_PROCEDURE_IWF", 		{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "INITIAL_ATTACH_PROCEDURE", 	{ .i32 = 4 }};

			struct dict_avp_data data = {
				1420,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Cancellation-Type",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE,	&tdata, NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0,   type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1,   type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2,   type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3,   type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4,   type, NULL);
			CHECK_dict_new( DICT_AVP,	 &data,  NULL, &type);
		}

		/* DSR-Flags AVP - 3GPP TS 29.272  #7.3.25 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1421,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"DSR-Flags",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* DSA-Flags AVP - 3GPP TS 29.272  #7.3.26 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1422,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"DSA-Flags",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Context-Identifier AVP - 3GPP TS 29.272  #7.3.27 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1423,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Context-Identifier",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Operator-Determined-Barring AVP - 3GPP TS 29.272  #7.3.30 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1425,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Operator-Determined-Barring",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Access-Restriction-Data AVP - 3GPP TS 29.272  #7.3.31 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1426,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Access-Restriction-Data",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* APN-OI-Replacement AVP - 3GPP TS 29.272  #7.3.32 */
		{
			struct dict_avp_data data = {
				1427,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"APN-OI-Replacement",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* All-APN-Configurations-Included-Indicator AVP - 3GPP TS 29.272  #7.3.33 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(All-APN-Configurations-Included-Indicator)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "All_APN_CONFIGURATIONS_INCLUDED",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "MODIFIED/ADDED_APN_CONFIGURATIONS_INCLUDED", 	{ .i32 = 1 }};

			struct dict_avp_data data = {
				1428,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"All-APN-Configurations-Included-Indicator",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* PDN-Type AVP - 3GPP TS 29.272  #7.3.62 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(PDN-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "IPv4",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "IPv6",				{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "IPv4v6", 			{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "IPv4_OR_IPv6", 	{ .i32 = 3 }};

			struct dict_avp_data data = {
				1456,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"PDN-Type",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* EPS-Subscribed-QoS-Profile AVP - 3GPP TS 29.272 #7.3.37 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1431,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"EPS-Subscribed-QoS-Profile",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "QoS-Class-Identifier",				RULE_REQUIRED, -1,  1 }
				,{ "Allocation-Retention-Priority",		RULE_REQUIRED, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* VPLMN-Dynamic-Address-Allowed AVP - 3GPP TS 29.272  #7.3.38 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(VPLMN-Dynamic-Address-Allowed)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "NOTALLOWED",	{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "ALLOWED", 		{ .i32 = 1 }};

			struct dict_avp_data data = {
				1432,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"VPLMN-Dynamic-Address-Allowed",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Alert-Reason AVP - 3GPP TS 29.272  #7.3.83 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Alert-Reason)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "UE_PRESENT",			{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "UE_MEMORY_AVAILABLE", 	{ .i32 = 1 }};

			struct dict_avp_data data = {
				1434,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Alert-Reason",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* PDN-GW-Allocation-Type AVP - 3GPP TS 29.272  #7.3.44 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(PDN-GW-Allocation-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "STATIC",	{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "DYNAMIC", 	{ .i32 = 1 }};

			struct dict_avp_data data = {
				1438,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"PDN-GW-Allocation-Type",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Expiration-Date AVP - 3GPP TS 29.272  #7.3.80 */
		{
			struct dict_avp_data data = {
				1439,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Expiration-Date",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , Time_type, NULL);
		}

		/* CSG-Id AVP - 3GPP TS 29.272 #7.3.79 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1437,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"CSG-Id",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* AMBR AVP - 3GPP TS 29.272 #7.3.41 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1435,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"AMBR",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Max-Requested-Bandwidth-UL",		RULE_REQUIRED, -1,  1 }
				,{ "Max-Requested-Bandwidth-DL",		RULE_REQUIRED, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Specific-APN-Info AVP - 3GPP TS 29.272  #7.3.82 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1472,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Specific-APN-Info",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Service-Selection",					RULE_REQUIRED, -1,  1 }
				,{ "MIP6-Agent-Info",					RULE_REQUIRED, -1,  1 }
				,{ "Visited-Network-Identifier",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* SIPTO-Permission AVP - 3GPP TS 29.272  #7.3.135 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(SIPTO-Permission)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "SIPTO_ALLOWED",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "SIPTO_NOTALLOWED",				{ .i32 = 1 }};

			struct dict_avp_data data = {
				1613,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"SIPTO-Permission",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* LIPA-Permission AVP - 3GPP TS 29.272 #7.3.133 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(LIPA-Permission)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0  = { "LIPA-PROHIBITED",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1  = { "LIPA-ONLY",					{ .i32 = 1 }};
			struct dict_enumval_data 	t_2  = { "LIPA-CONDITIONAL",			{ .i32 = 2 }};

			struct dict_avp_data data = {
				1618,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"LIPA-Permission",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);

			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* List-Of-Measurements AVP - 3GPP TS 29.272 #7.3.139 */
		{
			struct dict_avp_data data = {
				1625,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"List-Of-Measurements",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Reporting-Trigger AVP - 3GPP TS 29.272 #7.3.140 */
		{
			struct dict_avp_data data = {
				1626,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Reporting-Trigger",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Report-Interval AVP - 3GPP TS 29.272 #7.3.141 and TS 32.422 #5.10.5 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Report-Interval)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0  = { "250ms",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1  = { "500ms",				{ .i32 = 1 }};
			struct dict_enumval_data 	t_2  = { "1000ms",				{ .i32 = 2 }};
			struct dict_enumval_data 	t_3  = { "2000ms",				{ .i32 = 3 }};
			struct dict_enumval_data 	t_4  = { "3000ms",				{ .i32 = 4 }};
			struct dict_enumval_data 	t_5  = { "4000ms",				{ .i32 = 5 }};
			struct dict_enumval_data 	t_6  = { "6000ms",				{ .i32 = 6 }};
			struct dict_enumval_data 	t_7  = { "8000ms",				{ .i32 = 7 }};
			struct dict_enumval_data 	t_8  = { "12000ms",				{ .i32 = 8 }};
			struct dict_enumval_data 	t_9  = { "16000ms",				{ .i32 = 9 }};
			struct dict_enumval_data 	t_10 = { "20000ms",				{ .i32 = 10 }};
			struct dict_enumval_data 	t_11 = { "24000ms",				{ .i32 = 11 }};
			struct dict_enumval_data 	t_12 = { "28000ms",				{ .i32 = 12 }};
			struct dict_enumval_data 	t_13 = { "32000ms",				{ .i32 = 13 }};
			struct dict_enumval_data 	t_14 = { "64000ms",				{ .i32 = 14 }};
			/* Possible in LTE */
			struct dict_enumval_data 	t_15 = { "120ms",				{ .i32 = 15 }};
			struct dict_enumval_data 	t_16 = { "240ms",				{ .i32 = 16 }};
			struct dict_enumval_data 	t_17 = { "480ms",				{ .i32 = 17 }};
			struct dict_enumval_data 	t_18 = { "640ms",				{ .i32 = 18 }};
			struct dict_enumval_data 	t_19 = { "1024ms",				{ .i32 = 19 }};
			struct dict_enumval_data 	t_20 = { "2048ms",				{ .i32 = 20 }};
			struct dict_enumval_data 	t_21 = { "5120ms",				{ .i32 = 21 }};
			struct dict_enumval_data 	t_22 = { "10240ms",				{ .i32 = 22 }};
			struct dict_enumval_data 	t_23 = { "1min",				{ .i32 = 23 }};
			struct dict_enumval_data 	t_24 = { "6min",				{ .i32 = 24 }};
			struct dict_enumval_data 	t_25 = { "12min",				{ .i32 = 25 }};
			struct dict_enumval_data 	t_26 = { "30min",				{ .i32 = 26 }};
			struct dict_enumval_data 	t_27 = { "60min",				{ .i32 = 27 }};

			struct dict_avp_data data = {
				1627,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Report-Interval",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_7 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_8 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_9 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_10 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_11 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_12 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_13 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_14 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_15 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_16 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_17 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_18 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_19 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_20 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_21 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_22 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_23 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_24 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_25 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_26 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_27 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Report-Amount AVP - 3GPP TS 29.272 #7.3.142 and TS 32.422 #5.10.6 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Report-Amount)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0  = { "1",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1  = { "2",				{ .i32 = 1 }};
			struct dict_enumval_data 	t_2  = { "4",				{ .i32 = 2 }};
			struct dict_enumval_data 	t_3  = { "8",				{ .i32 = 3 }};
			struct dict_enumval_data 	t_4  = { "16",				{ .i32 = 4 }};
			struct dict_enumval_data 	t_5  = { "32",				{ .i32 = 5 }};
			struct dict_enumval_data 	t_6  = { "64",				{ .i32 = 6 }};
			struct dict_enumval_data 	t_7  = { "infinity",		{ .i32 = 7 }};

			struct dict_avp_data data = {
				1628,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Report-Amount",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_7 , avp, NULL);

			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Event-Threshold-RSRP AVP - 3GPP TS 29.272 #7.3.143 */
		{
			struct dict_avp_data data = {
				1629,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Event-Threshold-RSRP",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Event-Threshold-RSRQ AVP - 3GPP TS 29.272 #7.3.144 */
		{
			struct dict_avp_data data = {
				1630,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Event-Threshold-RSRQ",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Logging-Interval AVP - 3GPP TS 29.272 #7.3.145 and TS 32.422 #5.10.8 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Logging-Interval)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0  = { "1.28",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1  = { "2.56",				{ .i32 = 1 }};
			struct dict_enumval_data 	t_2  = { "5.12",				{ .i32 = 2 }};
			struct dict_enumval_data 	t_3  = { "10.24",				{ .i32 = 3 }};
			struct dict_enumval_data 	t_4  = { "20.48",				{ .i32 = 4 }};
			struct dict_enumval_data 	t_5  = { "30.72",				{ .i32 = 5 }};
			struct dict_enumval_data 	t_6  = { "40.96",				{ .i32 = 6 }};
			struct dict_enumval_data 	t_7  = { "61.44",				{ .i32 = 7 }};

			struct dict_avp_data data = {
				1631,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Logging-Interval",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_7 , avp, NULL);

			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Logging-Duration AVP - 3GPP TS 29.272 #7.3.145 and TS 32.422 #5.10.9 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Logging-Duration)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0  = { "600sec",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1  = { "1200sec",				{ .i32 = 1 }};
			struct dict_enumval_data 	t_2  = { "2400sec",				{ .i32 = 2 }};
			struct dict_enumval_data 	t_3  = { "3600sec",				{ .i32 = 3 }};
			struct dict_enumval_data 	t_4  = { "5400sec",				{ .i32 = 4 }};
			struct dict_enumval_data 	t_5  = { "7200sec",				{ .i32 = 5 }};

			struct dict_avp_data data = {
				1632,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Logging-Duration",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5 , avp, NULL);

			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* APN-Configuration AVP - 3GPP TS 29.272 #7.3.35 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1430,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"APN-Configuration",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Context-Identifier",			RULE_REQUIRED, -1,  1 }
				,{ "Served-Party-IP-Address",		RULE_OPTIONAL, -1,  2 }
				,{ "PDN-Type",						RULE_REQUIRED, -1,  1 }
				,{ "Service-Selection",				RULE_REQUIRED, -1,  1 }
				,{ "EPS-Subscribed-QoS-Profile",	RULE_OPTIONAL, -1,  1 }
				,{ "VPLMN-Dynamic-Address-Allowed",	RULE_OPTIONAL, -1,  1 }
				,{ "MIP6-Agent-Info",				RULE_OPTIONAL, -1,  1 }
				,{ "Visited-Network-Identifier",	RULE_OPTIONAL, -1,  1 }
				,{ "PDN-GW-Allocation-Type",		RULE_OPTIONAL, -1,  1 }
				,{ "3GPP-Charging-Characteristics",	RULE_OPTIONAL, -1,  1 }
				,{ "AMBR",							RULE_OPTIONAL, -1,  1 }
				,{ "Specific-APN-Info",				RULE_OPTIONAL, -1, -1 }
				,{ "APN-OI-Replacement",			RULE_OPTIONAL, -1,  1 }
				,{ "SIPTO-Permission",				RULE_OPTIONAL, -1,  1 }
				,{ "LIPA-Permission",				RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* APN-Configuration-Profile AVP - 3GPP TS 29.272 #7.3.34 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1429,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"APN-Configuration-Profile",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Context-Identifier",						RULE_REQUIRED, -1,  1 }
				,{ "All-APN-Configurations-Included-Indicator",	RULE_REQUIRED, -1,  1 }
				,{ "APN-Configuration",							RULE_REQUIRED, -1, -1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* CSG-Subscription-Data AVP - 3GPP TS 29.272 #7.3.78 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1436,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"CSG-Subscription-Data",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "CSG-Id",					RULE_REQUIRED, -1,  1 }
				,{ "Expiration-Date",			RULE_OPTIONAL, -1,  1 }
				,{ "Service-Selection",			RULE_OPTIONAL, -1, -1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* RAT-Frequency-Selection-Priority-ID AVP - 3GPP TS 29.272 #7.3.46 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1440,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"RAT-Frequency-Selection-Priority-ID",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* IDA-Flags AVP - 3GPP TS 29.272 #7.3.47 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1441,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"IDA-Flags",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* PUA-Flags AVP - 3GPP TS 29.272 #7.3.48 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1442,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"PUA-Flags",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* NOR-Flags AVP - 3GPP TS 29.272 #7.3.49 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1443,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"NOR-Flags",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* User-Id AVP - 3GPP TS 29.272 #7.3.50 */
		{
			struct dict_avp_data data = {
				1444,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"User-Id",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Equipment-Status AVP - 3GPP TS 29.272  #7.3.51 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Equipment-Status)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "WHITELISTED",	{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "BLACKLISTED", 	{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "GREYLISTED", 	{ .i32 = 2 }};

			struct dict_avp_data data = {
				1445,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Equipment-Status",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Regional-Subscription-Zone-Code AVP - 3GPP TS 29.272  #7.3.52 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1446,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Regional-Subscription-Zone-Code",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Roaming-Restricted-Due-To-Unsupported-Feature AVP - 3GPP TS 29.272  #7.3.81 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Roaming-Restricted-Due-To-Unsupported-Feature)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "Roaming-Restricted-Due-To-Unsupported-Feature",	{ .i32 = 0 }};

			struct dict_avp_data data = {
				1457,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Roaming-Restricted-Due-To-Unsupported-Feature",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Trace-Reference AVP - 3GPP TS 29.272  #7.3.64 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1459,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Trace-Reference",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* Trace-Depth AVP - 3GPP TS 29.272  #7.3.67 and 3GPP TS 32.422  #5.3 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Trace-Depth)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "Minimum",									{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "Medium",									{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "Maximum",									{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "MinimumWithoutVendorSpecificExtension",	{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "MediumWithoutVendorSpecificExtension",		{ .i32 = 4 }};
			struct dict_enumval_data 	t_5 = { "MaximumWithoutVendorSpecificExtension",	{ .i32 = 5 }};

			struct dict_avp_data data = {
				1462,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Trace-Depth",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Trace-NE-Type-List AVP - 3GPP TS 29.272  #7.3.68 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1463,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Trace-NE-Type-List",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* Trace-Event-List AVP - 3GPP TS 29.272  #7.3.69 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1464,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Trace-Interface-List",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* Trace-Event-List AVP - 3GPP TS 29.272  #7.3.70 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1465,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Trace-Event-List",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* OMC-Id AVP - 3GPP TS 29.272  #7.3.71 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1466,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"OMC-Id",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* Complete-Data-List-Included-Indicator AVP - 3GPP TS 29.272  #7.3.73 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Complete-Data-List-Included-Indicator)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "All_PDP_CONTEXTS_INCLUDED",	{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "MODIFIED/ADDED_PDP CONTEXTS_INCLUDED", 	{ .i32 = 1 }};

			struct dict_avp_data data = {
				1468,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Complete-Data-List-Included-Indicator",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* PDP-Type AVP - 3GPP TS 29.272  #7.3.75 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1470,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"PDP-Type",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* GMLC-Number AVP - 3GPP TS 29.272  #7.3.85 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1474,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"GMLC-Number",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* SS-Code AVP - 3GPP TS 29.272  #7.3.87 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1476,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"SS-Code",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* SS-Status AVP - 3GPP TS 29.272  #7.3.88 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1477,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"SS-Status",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* Notification-To-UE-User AVP - 3GPP TS 29.272  #7.3.89 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Notification-To-UE-User)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "NOTIFY_LOCATION_ALLOWED",								{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "NOTIFYANDVERIFY_LOCATION_ALLOWED_IF_NO_RESPONSE",		{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "NOTIFYANDVERIFY_LOCATION_NOT_ALLOWED_IF_NO_RESPONSE",	{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "LOCATION_NOT_ALLOWED",									{ .i32 = 3 }};

			struct dict_avp_data data = {
				1478,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Notification-To-UE-User",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* Client-Identity AVP - 3GPP TS 29.272  #7.3.91 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1480,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Client-Identity",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* GMLC-Restriction AVP - 3GPP TS 29.272  #7.3.92 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(GMLC-Restriction)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "GMLC_LIST",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "HOME_COUNTRY",				{ .i32 = 1 }};

			struct dict_avp_data data = {
				1481,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"GMLC-Restriction",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* PLMN-Client AVP - 3GPP TS 29.272  #7.3.92 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(PLMN-Client)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "BROADCAST_SERVICE",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "O_AND_M_HPLMN",					{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "O_AND_M_VPLMN",					{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "ANONYMOUS_LOCATION",				{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "TARGET_UE_SUBSCRIBED_SERVICE",		{ .i32 = 4 }};

			struct dict_avp_data data = {
				1482,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"PLMN-Client",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* ServiceTypeIdentity AVP - 3GPP TS 29.272  #7.3.95 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1484,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"ServiceTypeIdentity",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
		}

		/* QoS-Subscribed - 3GPP TS 29.272 #7.3.77 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1404,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"QoS-Subscribed",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* ULR-Flags - 3GPP TS 29.272  #7.3.7 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1405,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"ULR-Flags",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Ext-PDP-Type AVP - 3GPP TS 29.272  #7.3.75A */
		{
			struct dict_avp_data data = {
				1620,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Ext-PDP-Type",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Ext-PDP-Address AVP - 3GPP TS 29.272  #7.3.129 */
		{
			struct dict_avp_data data = {
				1621,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Ext-PDP-Address",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, Address_type, NULL);
		}

		/* TS-Code AVP - 3GPP TS 29.272  #7.3.100 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1487,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"TS-Code",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* External-Client AVP - 3GPP TS 29.272 #7.3.90 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1479,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"External-Client",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Client-Identity",				RULE_REQUIRED, -1,  1 }
				,{ "GMLC-Restriction",				RULE_OPTIONAL, -1,  1 }
				,{ "Notification-To-UE-User",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Service-Type AVP - 3GPP TS 29.272 #7.3.94 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1483,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Service-Type",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "ServiceTypeIdentity",			RULE_REQUIRED, -1,  1 }
				,{ "GMLC-Restriction",				RULE_OPTIONAL, -1,  1 }
				,{ "Notification-To-UE-User",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* LCS-PrivacyException AVP - 3GPP TS 29.272 #7.3.86 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1475,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"LCS-PrivacyException",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "SS-Code",						RULE_REQUIRED, -1,  1 }
				,{ "SS-Status",						RULE_REQUIRED, -1,  1 }
				,{ "Notification-To-UE-User",		RULE_OPTIONAL, -1,  1 }
				,{ "External-Client",				RULE_OPTIONAL, -1, -1 }
				,{ "PLMN-Client",					RULE_OPTIONAL, -1, -1 }
				,{ "Service-Type",					RULE_OPTIONAL, -1, -1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* MO-LR AVP - 3GPP TS 29.272 #7.3.96 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1485,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"MO-LR",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "SS-Code",			RULE_REQUIRED, -1,  1 }
				,{ "SS-Status",			RULE_REQUIRED, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* LCS-Info AVP - 3GPP TS 29.272 #7.3.84 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1473,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"LCS-Info",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "GMLC-Number",				RULE_OPTIONAL, -1, -1 }
				,{ "LCS-PrivacyException",		RULE_OPTIONAL, -1, -1 }
				,{ "MO-LR",						RULE_OPTIONAL, -1, -1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* PDP-Context AVP - 3GPP TS 29.272 #7.3.74 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1469,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"PDP-Context",		/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Context-Identifier",			RULE_REQUIRED, -1,  1 }
				,{ "PDP-Type",						RULE_REQUIRED, -1,  1 }
				,{ "PDP-Address",					RULE_OPTIONAL, -1,  1 }
				,{ "QoS-Subscribed",				RULE_REQUIRED, -1,  1 }
				,{ "VPLMN-Dynamic-Address-Allowed",	RULE_OPTIONAL, -1,  1 }
				,{ "Service-Selection",				RULE_REQUIRED, -1,  1 }
				,{ "3GPP-Charging-Characteristics",	RULE_OPTIONAL, -1,  1 }
				,{ "Ext-PDP-Type",					RULE_OPTIONAL, -1,  1 }
				,{ "Ext-PDP-Address",				RULE_OPTIONAL, -1,  1 }
				,{ "AMBR",							RULE_OPTIONAL, -1,  1 }
				,{ "SIPTO-Permission",				RULE_OPTIONAL, -1,  1 }
				,{ "LIPA-Permission",				RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* GPRS-Subscription-Data AVP - 3GPP TS 29.272  #7.3.72 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1467,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"GPRS-Subscription-Data",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Complete-Data-List-Included-Indicator",		RULE_REQUIRED, -1,  1 }
				,{ "PDP-Context",								RULE_REQUIRED, -1,  50 }
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Teleservice-List AVP - 3GPP TS 29.272 #7.3.99 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1486,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Teleservice-List",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "TS-Code",			RULE_REQUIRED, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Call-Barring-Infor-List AVP - 3GPP TS 29.272 #7.3.101 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1488,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Call-Barring-Infor-List",		/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				{ "SS-Code",		RULE_REQUIRED,  1, -1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* SGSN-Number AVP - 3GPP TS 29.272  #7.3.102 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1489,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"SGSN-Number",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* IDR-Flags AVP - 3GPP TS 29.272  #7.3.103 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				1490,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"IDR-Flags",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* IMS-Voice-Over-PS-Sessions-Supported AVP - 3GPP TS 29.272  #7.3.106 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(IMS-Voice-Over-PS-Sessions-Supported)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "NOT_SUPPORTED",			{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "SUPPORTED",				{ .i32 = 1 }};

			struct dict_avp_data data = {
				1492,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"IMS-Voice-Over-PS-Sessions-Supported",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , avp, NULL);
		}

		/* SGSN-Number AVP - 3GPP TS 29.272  #7.3.102 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Homogeneous-Support-of-IMS-Voice-Over-PSSessions)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "NOT_SUPPORTED",						{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "SUPPORTED", 							{ .i32 = 1 }};

			struct dict_avp_data data = {
				1493,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Homogeneous-Support-of-IMS-Voice-Over-PSSessions",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Last-UE-Activity-Time AVP - 3GPP TS 29.272 #7.3.108 */
		{
			struct dict_avp_data data = {
				1494,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Last-UE-Activity-Time",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , Time_type, NULL);
		}

		/* E-UTRAN-Cell-Global-Identity AVP - 3GPP TS 29.272 #7.3.117 */
		{
			struct dict_avp_data data = {
				1602,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"E-UTRAN-Cell-Global-Identity",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Tracking-Area-Identity AVP - 3GPP TS 29.272 #7.3.118 */
		{
			struct dict_avp_data data = {
				1603,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Tracking-Area-Identity",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Cell-Global-Identity AVP - 3GPP TS 29.272 #7.3.119 */
		{
			struct dict_avp_data data = {
				1604,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Cell-Global-Identity",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Routing-Area-Identity AVP - 3GPP TS 29.272 #7.3.120 */
		{
			struct dict_avp_data data = {
				1605,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Routing-Area-Identity",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Location-Area-Identity AVP - 3GPP TS 29.272 #7.3.121 */
		{
			struct dict_avp_data data = {
				1606,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Location-Area-Identity",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Service-Area-Identity AVP - 3GPP TS 29.272 #7.3.122 */
		{
			struct dict_avp_data data = {
				1607,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Service-Area-Identity",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Geographical-Information AVP - 3GPP TS 29.272 #7.3.123 */
		{
			struct dict_avp_data data = {
				1608,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Geographical-Information",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Geodetic-Information AVP - 3GPP TS 29.272 #7.3.124 */
		{
			struct dict_avp_data data = {
				1609,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Geodetic-Information",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}

		/* Current-Location-Retrieved AVP - 3GPP TS 29.272  #7.3.125 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Current-Location-Retrieved)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "ACTIVE-LOCATION-RETRIEVAL",		{ .i32 = 0 }};

			struct dict_avp_data data = {
				1610,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Current-Location-Retrieved",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Age-Of-Location-Information AVP - 3GPP TS 29.272 #7.3.126 */
		{
			struct dict_avp_data data = {
				1611,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Age-Of-Location-Information",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* UE-SRVCC-Capability AVP - 3GPP TS 29.272  #7.3.130 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(UE-SRVCC-Capability)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "UE-SRVCC-NOT-SUPPORTED",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "UE-SRVCC-SUPPORTED",			{ .i32 = 1 }};

			struct dict_avp_data data = {
				1615,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"UE-SRVCC-Capability",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* MPS-Priority AVP - 3GPP TS 29.272 #7.3.131 */
		{
			struct dict_avp_data data = {
				1616,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"MPS-Priority",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* VPLMN-LIPA-Allowed AVP - 3GPP TS 29.272  #7.3.132 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(VPLMN-LIPA-Allowed)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "LIPA-NOTALLOWED",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "LIPA-ALLOWED",			{ .i32 = 1 }};

			struct dict_avp_data data = {
				1617,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"VPLMN-LIPA-Allowed",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Subscribed-Periodic-RAU-TAU-Timer AVP - 3GPP TS 29.272 #7.3.134 */
		{
			struct dict_avp_data data = {
				1619,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Subscribed-Periodic-RAU-TAU-Timer",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Job-Type AVP - 3GPP TS 29.272 #7.3.132 and 3GPP TS 32.422 #5.10.1 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Job-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "Immediate-MDT-Only",			{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "Logged-MDT-only",				{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "Trace-Only",					{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "Immediate-MDT-and-Trace",		{ .i32 = 3 }};

			struct dict_avp_data data = {
				1623,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Job-Type",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* Relay-Node-Indicator AVP - 3GPP TS 29.272  #7.3.147 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Relay-Node-Indicator)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "NOT_RELAY_NODE",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "RELAY_NODE",			{ .i32 = 1 }};

			struct dict_avp_data data = {
				1633,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Relay-Node-Indicator",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* MDT-User-Consent AVP - 3GPP TS 29.272  #7.3.148 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(MDT-User-Consent)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "CONSENT_NOT_GIVEN",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "CONSENT_GIVEN",			{ .i32 = 1 }};

			struct dict_avp_data data = {
				1634,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"MDT-User-Consent",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* PUR-Flags AVP - 3GPP TS 29.272 #7.3.149 */
		{
			struct dict_avp_data data = {
				1635,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"PUR-Flags",			/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* User-State AVP - 3GPP TS 29.272 #7.3.114 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(User-State)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "DETACHED",									{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "ATTACHED_NOT_REACHABLE_FOR_PAGING", 		{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "ATTACHED_REACHABLE_FOR_PAGING", 			{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "CONNECTED_NOT_REACHABLE_FOR_PAGING", 		{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "CONNECTED_REACHABLE_FOR_PAGING", 			{ .i32 = 4 }};
			struct dict_enumval_data 	t_5 = { "NETWORK_DETERMINED_NOT_REACHABLE", 		{ .i32 = 5 }};

			struct dict_avp_data data = {
				1499,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"User-State",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* MME-User-State AVP - 3GPP TS 29.272 #7.3.112 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1497,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"MME-User-State",		/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				{ "User-State",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* SGSN-User-State AVP - 3GPP TS 29.272 #7.3.113 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1498,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"SGSN-User-State",		/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				{ "User-State",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* EPS-User-State AVP - 3GPP TS 29.272 #7.3.110 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1495,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"EPS-User-State",		/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "MME-User-State",		RULE_OPTIONAL, -1,  1 }
				,{ "SGSN-User-State",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* MME-Location-Information AVP - 3GPP TS 29.272 #7.3.115 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1600,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"MME-Location-Information",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "E-UTRAN-Cell-Global-Identity",				RULE_OPTIONAL, -1,  1 }
				,{ "Tracking-Area-Identity",			RULE_OPTIONAL, -1,  1 }
				,{ "Geographical-Information",			RULE_OPTIONAL, -1,  1 }
				,{ "Geodetic-Information",				RULE_OPTIONAL, -1,  1 }
				,{ "Current-Location-Retrieved",		RULE_OPTIONAL, -1,  1 }
				,{ "Age-Of-Location-Information",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* SGSN-Location-Information AVP - 3GPP TS 29.272 #7.3.116 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1601,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"SGSN-Location-Information",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Cell-Global-Identity",				RULE_OPTIONAL, -1,  1 }
				,{ "Location-Area-Identity",			RULE_OPTIONAL, -1,  1 }
				,{ "Service-Area-Identity",				RULE_OPTIONAL, -1,  1 }
				,{ "Routing-Area-Identity",				RULE_OPTIONAL, -1,  1 }
				,{ "Geographical-Information",			RULE_OPTIONAL, -1,  1 }
				,{ "Geodetic-Information",				RULE_OPTIONAL, -1,  1 }
				,{ "Current-Location-Retrieved",		RULE_OPTIONAL, -1,  1 }
				,{ "Age-Of-Location-Information",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Area-Scope AVP - 3GPP TS 29.272  #7.3.138 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1624,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Area-Scope",			/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Cell-Global-Identity",				RULE_OPTIONAL, -1, -1 }
				,{ "E-UTRAN-Cell-Global-Identity",		RULE_OPTIONAL, -1, -1 }
				,{ "Routing-Area-Identity",				RULE_OPTIONAL, -1, -1 }
				,{ "Location-Area-Identity",			RULE_OPTIONAL, -1, -1 }
				,{ "Tracking-Area-Identity",			RULE_OPTIONAL, -1, -1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* MDT-Configuration AVP - 3GPP TS 29.272 #7.3.136 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1622,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"MDT-Configuration",		/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Job-Type",					RULE_REQUIRED, -1,  1 }
				,{ "Area-Scope",				RULE_OPTIONAL, -1,  1 }
				,{ "List-Of-Measurements",		RULE_OPTIONAL, -1,  1 }
				,{ "Reporting-Trigger",			RULE_OPTIONAL, -1,  1 }
				,{ "Report-Interval",			RULE_OPTIONAL, -1,  1 }
				,{ "Report-Amount",				RULE_OPTIONAL, -1,  1 }
				,{ "Event-Threshold-RSRP",		RULE_OPTIONAL, -1,  1 }
				,{ "Event-Threshold-RSRQ",		RULE_OPTIONAL, -1,  1 }
				,{ "Logging-Interval",			RULE_OPTIONAL, -1,  1 }
				,{ "Logging-Duration",			RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* EPS-Location-Information AVP - 3GPP TS 29.272 #7.3.111 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1496,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"EPS-Location-Information",		/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "MME-Location-Information",		RULE_OPTIONAL, -1,  1 }
				,{ "SGSN-Location-Information",		RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Active-APN AVP - 3GPP TS 29.173  #7.3.127 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1612,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Active-APN",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Context-Identifier",				RULE_REQUIRED, -1,  1 }
				,{ "Service-Selection",					RULE_OPTIONAL, -1,  1 }
				,{ "MIP6-Agent-Info",					RULE_OPTIONAL, -1,  1 }
				,{ "Visited-Network-Identifier",		RULE_OPTIONAL, -1,  1 }
				,{ "Specific-APN-Info",					RULE_OPTIONAL, -1, -1 }
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Error-Diagnostic AVP - 3GPP TS 29.272  #7.3.128 */
		{
			struct dict_object * avp;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Error-Diagnostic)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "GPRS_DATA_SUBSCRIBED",				{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "NO_GPRS_DATA_SUBSCRIBED", 			{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "ODB-ALL-APN", 						{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "ODB-HPLMN-APN", 					{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "ODB-VPLMN-APN", 					{ .i32 = 4 }};

			struct dict_avp_data data = {
				1614,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Error-Diagnostic",			/* Name */
				AVP_FLAG_VENDOR,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_INTEGER32		/* base type of data */
			};

			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &avp);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , avp, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , avp, NULL);
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
		}

		/* GMLC-Address AVP - 3GPP TS 29.173  #6.4.7 */
		{
			struct dict_avp_data data = {
				2405,					/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"GMLC-Address",			/* Name */
				AVP_FLAG_VENDOR,		/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING		/* base type of data */
			};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* Supported Features AVP - 3GPP TS 29.229 */
		{
			struct dict_object * avp;
			struct dict_avp_data data = {
				628,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Supported-Features",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Vendor-Id",			RULE_REQUIRED,  1, 1 }
				,{  "Feature-List-ID",		RULE_REQUIRED,  1, 1 }
				,{  "Feature-List",			RULE_REQUIRED,  1, 1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Trace-Data AVP - 3GPP TS 29.272  #7.3.63 */
		{
			struct dict_object * avp;

			struct dict_avp_data data = {
				1458,				/* Code */
                VENDOR_3GPP_Id,     /* Vendor */
				"Trace-Data",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 { "Trace-Reference",				RULE_REQUIRED, -1,  1 }
				,{ "Trace-Depth",					RULE_REQUIRED, -1,  1 }
				,{ "Trace-NE-Type-List",			RULE_REQUIRED, -1,  1 }
				,{ "Trace-Interface-List",			RULE_OPTIONAL, -1,  1 }
				,{ "Trace-Event-List",				RULE_REQUIRED, -1,  1 }
				,{ "OMC-Id",						RULE_OPTIONAL, -1,  1 }
				,{ "Trace-Collection-Entity",		RULE_REQUIRED, -1,  1 }
				,{ "MDT-Configuration",				RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Subscription-Data AVP - 3GPP TS 29.272 #7.3.2 */
		{
			/*

			 * The Subscription-Data AVP is of type Grouped. It shall contain
			 * the information related to the user profile relevant for
			 * EPS and GERAN/UTRAN.

			 Subscription-Data ::= <AVP header: 1400 10415>
				[ Subscriber-Status ]
				[ MSISDN ]
				[ STN-SR ]
				[ ICS-Indicator ]
				[ Network-Access-Mode ]
				[ Operator-Determined-Barring ]
				[ HPLMN-ODB ]
			 *10[ Regional-Subscription-Zone-Code]
				[ Access-Restriction-Data ]
				[ APN-OI-Replacement ]
				[ LCS-Info ]
				[ Teleservice-List ]
				[ Call-Barring-Infor-List ]
				[ 3GPP-Charging-Characteristics ]
				[ AMBR ]
				[ APN-Configuration-Profile ]
				[ RAT-Frequency-Selection-Priority-ID ]
				[ Trace-Data]
				[ GPRS-Subscription-Data ]
			   *[ CSG-Subscription-Data ]
				[ Roaming-Restricted-Due-To-Unsupported-Feature ]
				[ Subscribed-Periodic-RAU-TAU-Timer ]
				[ MPS-Priority ]
				[ VPLMN-LIPA-Allowed ]
				[ Relay-Node-Indicator ]
				[ MDT-User-Consent ]
			   *[ AVP ]

			 * The AMBR included in this grouped AVP shall include the AMBR
			 * associated to the user’s subscription (UE-AMBR);
			 * Max-Requested-Bandwidth-UL and Max-Requested-Bandwidth-DL within
			 * this AVP shall not both be set to "0".
			 * The APN-OI-Replacement included in this grouped AVP shall include
			 * the UE level APN-OI-Replacement associated to the user’s subscription.
			 */
			struct dict_object * avp;
			struct dict_avp_data data = {
				1400,				/* Code */
				VENDOR_3GPP_Id,					/* Vendor */
				"Subscription-Data",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED		/* base type of data */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Subscriber-Status",				RULE_OPTIONAL, -1,  1 }
				,{  "MSISDN",							RULE_OPTIONAL, -1,  1 }
				,{  "STN-SR",							RULE_OPTIONAL, -1,  1 }
				,{  "ICS-Indicator",					RULE_OPTIONAL, -1,  1 }
				,{  "Network-Access-Mode",				RULE_OPTIONAL, -1,  1 }
				,{  "Operator-Determined-Barring",		RULE_OPTIONAL, -1,  1 }
				,{  "HPLMN-ODB",						RULE_OPTIONAL, -1,  1 }
				,{  "Regional-Subscription-Zone-Code",	RULE_OPTIONAL, -1,  10 }
				,{  "Access-Restriction-Data",			RULE_OPTIONAL, -1,  1 }
				,{  "APN-OI-Replacement",				RULE_OPTIONAL, -1,  1 }
				,{  "LCS-Info",							RULE_OPTIONAL, -1,  1 }
				,{  "Teleservice-List",					RULE_OPTIONAL, -1,  1 }
				,{  "Call-Barring-Infor-List",			RULE_OPTIONAL, -1,  1 }
				,{  "3GPP-Charging-Characteristics",	RULE_OPTIONAL, -1,  1 }
				,{  "AMBR",								RULE_OPTIONAL, -1,  1 }
				,{  "APN-Configuration-Profile",		RULE_OPTIONAL, -1,  1 }
				,{  "RAT-Frequency-Selection-Priority-ID", RULE_OPTIONAL, -1,  1 }
				,{  "Trace-Data",						RULE_OPTIONAL, -1,  1 }
				,{  "GPRS-Subscription-Data",			RULE_OPTIONAL, -1,  1 }
				,{  "CSG-Subscription-Data",			RULE_OPTIONAL, -1, -1 }
				,{  "Roaming-Restricted-Due-To-Unsupported-Feature", RULE_OPTIONAL, -1,  1 }
				,{  "Subscribed-Periodic-RAU-TAU-Timer",	RULE_OPTIONAL, -1,  1 }
				,{  "MPS-Priority",						RULE_OPTIONAL, -1,  1 }
				,{  "VPLMN-LIPA-Allowed",				RULE_OPTIONAL, -1,  1 }
				,{  "Relay-Node-Indicator",				RULE_OPTIONAL, -1,  1 }
				,{  "MDT-User-Consent",					RULE_OPTIONAL, -1,  1 }
			};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* S6A-Update Location Request - 3GPP TS 29.272 #7.2.3 */
		{
			/*

			The Update-Location-Request (ULR) command, indicated by the
			Command-Code field set to 316 and the "R" bit set in
			the Command Flags field, is sent from MME or SGSN to HSS.

			< Update-Location-Request> ::= < Diameter Header: 316, REQ, PXY, 16777251 >
					< Session-Id >
					[ Vendor-Specific-Application-Id ]
					{ Auth-Session-State }
					{ Origin-Host }
					{ Origin-Realm }
					[ Destination-Host ]
					{ Destination-Realm }
					{ User-Name }
				   *[ Supported-Features ]
					[ Terminal-Information ]
					{ RAT-Type }
					{ ULR-Flags }
					[ UE-SRVCC-Capability ]
					{ Visited-PLMN-Id }
					[ SGSN-Number ]
					[ Homogeneous-Support-of-IMS-Voice-Over-PS-Sessions ]
					[ GMLC-Address ]
				   *[ Active-APN ]
				   *[ AVP ]
				   *[ Proxy-Info ]
				   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				316, 					/* Code */
				"Update-Location-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 						RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State",				RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",						RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host",					RULE_OPTIONAL,   -1,  1 }
				,{  "Destination-Realm",				RULE_REQUIRED,   -1,  1 }
				,{  "User-Name",						RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features",				RULE_OPTIONAL,   -1, -1 }
				,{  "Terminal-Information",				RULE_OPTIONAL,   -1,  1 }
				,{  "RAT-Type",							RULE_REQUIRED,	1,  1 }
				,{  "ULR-Flags",						RULE_REQUIRED,	1,  1 }
				,{  "UE-SRVCC-Capability",				RULE_OPTIONAL,   -1,  1 }
				,{  "Visited-PLMN-Id",					RULE_REQUIRED,	1,  1 }
				,{  "SGSN-Number",						RULE_OPTIONAL,   -1,  1 }
				,{  "Homogeneous-Support-of-IMS-Voice-Over-PSSessions", RULE_OPTIONAL, -1, 1 }
				,{  "GMLC-Address", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Active-APN", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 					RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* S6A-Update Location Answer - 3GPP TS 29.272 #7.2.4 */
		{
			/*

			The Update-Location-Answer (ULA) command, indicated by the
			Command-Code field set to 316 and the 'R' bit cleared
			in the Command Flags field, is sent from HSS to MME or SGSN.

			< Update-Location-Answer> ::= < Diameter Header: 316, PXY, 16777251 >
					< Session-Id >
					[ Vendor-Specific-Application-Id ]
					[ Result-Code ]
					[ Experimental-Result ]
					[ Error-Diagnostic ]
					{ Auth-Session-State }
					{ Origin-Host }
					{ Origin-Realm }
				   *[ Supported-Features ]
					[ ULA-Flags ]
					[ Subscription-Data ]
				   *[ AVP ]
				   *[ Failed-AVP ]
				   *[ Proxy-Info ]
				   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				316, 					/* Code */
				"Update-Location-Answer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 						RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1,  1 }
				,{  "Result-Code",						RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result",				RULE_OPTIONAL,   -1,  1 }
				,{  "Error-Diagnostic",					RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State",				RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",						RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features",				RULE_OPTIONAL,   -1, -1 }
				,{  "ULA-Flags",						RULE_OPTIONAL,   -1,  1 }
				,{  "Subscription-Data",				RULE_OPTIONAL,   -1,  1 }
				,{  "Failed-AVP",						RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 					RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* S6A-Cancel-Location-Request - 3GPP TS 29.272 #7.2.7 */
		{
			/*

			The Cancel-Location-Request (CLR) command, indicated by the
			Command-Code field set to 317 and the 'R' bit set in
			the Command Flags field, is sent from HSS to MME or SGSN.

			< Cancel-Location-Request> ::= < Diameter Header: 317, REQ, PXY, 16777251 >
					< Session-Id >
					[ Vendor-Specific-Application-Id ]
					{ Auth-Session-State }
					{ Origin-Host }
					{ Origin-Realm }
					{ Destination-Host }
					{ Destination-Realm }
					{ User-Name }
				   *[Supported-Features ]
					{ Cancellation-Type }
				   *[ AVP ]
				   *[ Proxy-Info ]
				   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				317, 					/* Code */
				"Cancel-Location-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 						RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State",				RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",						RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host",					RULE_OPTIONAL,   -1,  1 }
				,{  "Destination-Realm",				RULE_REQUIRED,   -1,  1 }
				,{  "User-Name",						RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features",				RULE_OPTIONAL,   -1, -1 }
				,{  "Cancellation-Type", 				RULE_REQUIRED,   -1,  1 }
				,{  "Proxy-Info", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 					RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* S6A-Cancel-Location-Answer - 3GPP TS 29.272 #7.2.8 */
		{
			/*

			The Cancel-Location-Answer (CLA) command, indicated by the
			Command-Code field set to 317 and the 'R' bit cleared
			in the Command Flags field, is sent from MME or SGSN to HSS.

			< Cancel-Location-Answer> ::= < Diameter Header: 317, PXY, 16777251 >
					< Session-Id >
					[ Vendor-Specific-Application-Id ]
				   *[ Supported-Features ]
					[ Result-Code ]
					[ Experimental-Result ]
					{ Auth-Session-State }
					{ Origin-Host }
					{ Origin-Realm }
				   *[ AVP ]
				   *[ Failed-AVP ]
				   *[ Proxy-Info ]
				   *[ Route-Record ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				317, 					/* Code */
				"Cancel-Location-Answer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 						RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1,  1 }
				,{  "Supported-Features",				RULE_OPTIONAL,   -1, -1 }
				,{  "Result-Code",						RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result",				RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State",				RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",						RULE_REQUIRED,   -1,  1 }
				,{  "Failed-AVP", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 					RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* S6A-Authentication-Information-Request - 3GPP TS 29.272 #7.2.4 */
		{
			/*

			The Authentication-Information-Request (AIR) command, indicated by
			the Command-Code field set to 318 and the 'R'
			bit set in the Command Flags field, is sent from MME or SGSN to HSS.

			< Authentication-Information-Request> ::= < Diameter Header: 318, REQ, PXY, 16777251 >
					< Session-Id >
					[ Vendor-Specific-Application-Id ]
					{ Auth-Session-State }
					{ Origin-Host }
					{ Origin-Realm }
					[ Destination-Host ]
					{ Destination-Realm }
					{ User-Name }
				   *[ Supported-Features ]
					[ Requested-EUTRAN-Authentication-Info ]
					[ Requested-UTRAN-GERAN-Authentication-Info ]
					{ Visited-PLMN-Id }
				   *[ AVP ]
				   *[ Proxy-Info ]
				   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				318, 					/* Code */
				"Authentication-Information-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 						RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State",				RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",						RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host",					RULE_OPTIONAL,   -1,  1 }
				,{  "Destination-Realm",				RULE_REQUIRED,   -1,  1 }
				,{  "User-Name",						RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features",				RULE_OPTIONAL,   -1, -1 }
				,{  "Requested-EUTRAN-Authentication-Info", 		RULE_OPTIONAL,   -1,  1 }
				,{  "Requested-UTRAN-GERAN-Authentication-Info", 	RULE_OPTIONAL,   -1,  1 }
				,{  "Visited-PLMN-Id", 					RULE_REQUIRED,   -1,  1 }
				,{  "Proxy-Info", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 					RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* S6A-Authentication-Information-Answer - 3GPP TS 29.272 #7.2.6 */
		{
			/*

			The Authentication-Information-Answer (AIA) command, indicated by
			the Command-Code field set to318 and the 'R' bit cleared in the
			Command Flags field, is sent from HSS to MME or SGSN.

			< Authentication-Information-Answer> ::= < Diameter Header: 318, PXY, 16777251 >
					< Session-Id >
					[ Vendor-Specific-Application-Id ]
					[ Result-Code ]
					[ Experimental-Result ]
					[ Error-Diagnostic ]
					{ Auth-Session-State }
					{ Origin-Host }
					{ Origin-Realm }
				   *[ Supported-Features ]
					[ Authentication-Info ]
				   *[ AVP ]
				   *[ Failed-AVP ]
				   *[ Proxy-Info ]
				   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				318, 					/* Code */
				"Authentication-Information-Answer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 						RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1,  1 }
				,{  "Result-Code",						RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result",				RULE_OPTIONAL,   -1,  1 }
				,{  "Error-Diagnostic",					RULE_OPTIONAL,   -1,  1 }
				,{  "Origin-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",						RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features",				RULE_OPTIONAL,   -1, -1 }
				,{  "Authentication-Info", 				RULE_OPTIONAL,   -1,  1 }
				,{  "Failed-AVP", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 					RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Insert-Subscriber-Data-Request IDR Command - 3GPP TS 29.272 #7.2.9 */
		{
			/*

			The Insert-Subscriber-Data-Request (IDR) command, indicated by the
			Command-Code field set to 319 and the 'R' bit set in the Command
			Flags field, is sent from HSS to MME or SGSN.

			< Insert-Subscriber-Data-Request> ::= < Diameter Header: 319, REQ, PXY, 16777251 >
					< Session-Id >
					[ Vendor-Specific-Application-Id ]
					{ Auth-Session-State }
					{ Origin-Host }
					{ Origin-Realm }
					{ Destination-Host }
					{ Destination-Realm }
					{ User-Name }
				   *[ Supported-Features]
					{ Subscription-Data}
					[IDR- Flags ]
				   *[ AVP ]
				   *[ Proxy-Info ]
				   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				319, 					/* Code */
				"Insert-Subscriber-Data-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 						RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State",				RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",						RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host",					RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Realm",				RULE_REQUIRED,   -1,  1 }
				,{  "User-Name",						RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features",				RULE_OPTIONAL,   -1, -1 }
				,{  "Subscription-Data", 				RULE_REQUIRED,   -1,  1 }
				,{  "IDR-Flags", 						RULE_OPTIONAL,   -1,  1 }
				,{  "Proxy-Info", 						RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 					RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Insert-Subscriber-Data-Answer IDA Command - 3GPP TS 29.272 #7.2.10 */
		{
			/*

			The Insert-Subscriber-Data-Request (IDR) command, indicated by the
			Command-Code field set to 319 and the 'R' bit set in the Command
			Flags field, is sent from HSS to MME or SGSN.

			< Insert-Subscriber-Data-Answer> ::= < Diameter Header: 319, PXY, 16777251 >
					< Session-Id >
					[ Vendor-Specific-Application-Id ]
				   *[ Supported-Features ]
					[ Result-Code ]
					[ Experimental-Result ]
					{ Auth-Session-State }
					{ Origin-Host }
					{ Origin-Realm }
					[ IMS-Voice-Over-PS-Sessions-Supported ]
					[ Last-UE-Activity-Time ]
					[ RAT-Type ]
					[ IDA-Flags ]
					[ EPS-User-State ]
					[ EPS-Location-Information ]
				   *[ AVP ]
				   *[ Failed-AVP ]
				   *[ Proxy-Info ]
				   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				319, 					/* Code */
				"Insert-Subscriber-Data-Answer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Supported-Features",					RULE_OPTIONAL,   -1, -1 }
				,{  "Result-Code", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host", 							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm", 						RULE_REQUIRED,   -1,  1 }
				,{  "IMS-Voice-Over-PS-Sessions-Supported", RULE_OPTIONAL,   -1,  1 }
				,{  "Last-UE-Activity-Time", 				RULE_OPTIONAL,   -1,  1 }
				,{  "RAT-Type", 							RULE_OPTIONAL,   -1,  1 }
				,{  "IDA-Flags", 							RULE_OPTIONAL,   -1,  1 }
				,{  "EPS-User-State", 						RULE_OPTIONAL,   -1,  1 }
				,{  "EPS-Location-Information", 			RULE_OPTIONAL,   -1,  1 }
				,{  "Failed-AVP", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 						RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Delete-Subscriber-Data-Request (DSR) Command - 3GPP TS 29.272 #7.2.11 */
		{
			/*

			The Delete-SubscriberData-Request (DSR) command, indicated by
			the Command-Code field set to 320 and the 'R' bit
			set in the Command Flags field, is sent from HSS to MME or SGSN.

			< Delete-Subscriber-Data-Request > ::= < Diameter Header: 320, REQ, PXY, 16777251 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
				{ Destination-Host }
				{ Destination-Realm }
				{ User-Name }
			   *[ Supported-Features ]
				{ DSR-Flags }
			   *[ Context-Identifier ]
				[ Trace-Reference ]
			   *[ TS-Code ]
			   *[ SS-Code ]
			   *[ AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				320, 					/* Code */
				"Delete-SubscriberData-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id",							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",							RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Realm",					RULE_REQUIRED,   -1,  1 }
				,{  "User-Name", 							RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features", 					RULE_OPTIONAL,   -1, -1 }
				,{  "DSR-Flags", 							RULE_REQUIRED,   -1,  1 }
				,{  "Context-Identifier", 					RULE_OPTIONAL,   -1, -1 }
				,{  "Trace-Reference",						RULE_OPTIONAL,   -1,  1 }
				,{  "TS-Code",								RULE_OPTIONAL,   -1, -1 }
				,{  "SS-Code",								RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info",							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record",							RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Delete-Subscriber-Data-Answer (DSA) Command - 3GPP TS 29.272 #7.2.12 */
		{
			/*

			The Delete-SubscriberData-Answer (DSA) command, indicated by the
			Command-Code field set to 320 and the 'R' bit
			cleared in the Command Flags field, is sent from MME or SGSN to HSS.

			< Delete-Subscriber-Data-Answer> ::= < Diameter Header: 320, PXY, 16777251 >
			< Session-Id >
			[ Vendor-Specific-Application-Id ]
			*[ Supported-Features ]
			[ Result-Code ]
			[ Experimental-Result ]
			{ Auth-Session-State }
			{ Origin-Host }
			{ Origin-Realm }
			[ DSA-Flags ]
			*[ AVP ]
			*[ Failed-AVP ]
			*[ Proxy-Info ]
			*[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				320, 					/* Code */
				"Delete-SubscriberData-Answer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id",							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Supported-Features", 					RULE_OPTIONAL,   -1, -1 }
				,{  "Result-Code", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",							RULE_REQUIRED,   -1,  1 }
				,{  "DSA-Flags", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Proxy-Info",							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record",							RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Purge-UE-Request (PUR) Command - 3GPP TS 29.272 #7.2.13 */
		{
			/*

			The Delete-SubscriberData-Request (DSR) command, indicated by
			the Command-Code field set to 320 and the 'R' bit
			set in the Command Flags field, is sent from HSS to MME or SGSN.

			< Purge-UE-Request> ::= < Diameter Header: 321, REQ, PXY, 16777251 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
				[ Destination-Host ]
				{ Destination-Realm }
				{ User-Name }
				[ PUR-Flags ]
			   *[ Supported-Features ]
			   *[ AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				321, 					/* Code */
				"Purge-UE-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id",							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",							RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host",						RULE_OPTIONAL,   -1,  1 }
				,{  "Destination-Realm",					RULE_REQUIRED,   -1,  1 }
				,{  "User-Name", 							RULE_REQUIRED,   -1,  1 }
				,{  "PUR-Flags", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Supported-Features", 					RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info",							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record",							RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Purge-UE-Answer (PUA) Command - 3GPP TS 29.272 #7.2.14 */
		{
			/*

			 The Purge-UE-Answer (PUA) command, indicated by the Command-Code
			 field set to 321 and the 'R' bit cleared in the
			 Command Flags field, is sent from HSS to MME or SGSN.

			 < Purge-UE-Answer> ::= < Diameter Header: 321, PXY, 16777251 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
			   *[ Supported-Features ]
				[ Result-Code ]
				[ Experimental-Result ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
				[ PUA-Flags ]
			   *[ AVP ]
			   *[ Failed-AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				321, 					/* Code */
				"Purge-UE-Answer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id",							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Supported-Features", 					RULE_OPTIONAL,   -1, -1 }
				,{  "Result-Code", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",							RULE_REQUIRED,   -1,  1 }
				,{  "PUA-Flags", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Failed-AVP",							RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info",							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record",							RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Reset-Request (RSR) Command - 3GPP TS 29.272 #7.2.15 */
		{
			/*

			The Reset-Request (RSR) command, indicated by the Command-Code
			field set to 322 and the 'R' bit set in the
			Command Flags field, is sent from HSS to MME or SGSN.

			< Reset-Request> ::= < Diameter Header: 322, REQ, PXY, 16777251 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
				{ Destination-Host }
				{ Destination-Realm }
			   *[ Supported-Features ]
			   *[ User-Id ]
			   *[ AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				322, 					/* Code */
				"Reset-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id",							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",							RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host",						RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Realm",					RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features", 					RULE_OPTIONAL,   -1, -1 }
				,{  "User-Id", 								RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info",							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record",							RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Reset-Answer (RSA) Command - 3GPP TS 29.272 #7.2.16 */
		{
			/*

			The Authentication-Information-Answer (RSA) command, indicated by
			the Command-Code field set to 322 and the 'R'
			bit cleared in the Command Flags field, is sent from MME or SGSN to HSS.

			< Reset-Answer> ::= < Diameter Header: 322, PXY, 16777251 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
			   *[ Supported-Features ]
				[ Result-Code ]
				[ Experimental-Result ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
			   *[ AVP ]
			   *[ Failed-AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				322, 					/* Code */
				"Reset-Anwer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id",							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Supported-Features", 					RULE_OPTIONAL,   -1, -1 }
				,{  "Result-Code", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host",							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm",							RULE_REQUIRED,   -1,  1 }
				,{  "Failed-AVP", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info",							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record",							RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Notify-Request (NOR) Command - 3GPP TS 29.272 #7.2.17 */
		{
			/*

			The Notify-Request (NOR) command, indicated by the Command-Code
			field set to 323 and the 'R' bit set in the Command Flags field,
			is sent from MME or SGSN to HSS.

			< Notify-Request> ::= < Diameter Header: 323, REQ, PXY, 16777251 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
				[ Destination-Host ]
				{ Destination-Realm }
				{ User-Name }
			   *[ Supported-Features ]
				[ Terminal-Information ]
				[ MIP6-Agent-Info ]
				[ Visited-Network-Identifier ]
				[ Context-Identifier ]
				[ Service-Selection ]
				[ Alert-Reason ]
				[ UE-SRVCC-Capability ]
				[ NOR-Flags ]
			   *[ AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				323, 					/* Code */
				"Notify-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host", 							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm", 						RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Destination-Realm", 					RULE_REQUIRED,   -1,  1 }
				,{  "User-Name", 							RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features", 					RULE_OPTIONAL,   -1, -1 }
				,{  "Terminal-Information", 				RULE_OPTIONAL,   -1,  1 }
				,{  "MIP6-Agent-Info", 						RULE_OPTIONAL,   -1,  1 }
				,{  "Visited-Network-Identifier", 			RULE_OPTIONAL,   -1,  1 }
				,{  "Context-Identifier", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Service-Selection", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Alert-Reason", 						RULE_OPTIONAL,   -1,  1 }
				,{  "UE-SRVCC-Capability", 					RULE_OPTIONAL,   -1,  1 }
				,{  "NOR-Flags", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Proxy-Info", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 						RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Notify-Answer (NOA) Command - 3GPP TS 29.272 #7.2.18 */
		{
			/*

			The Notify-Answer (NOA) command, indicated by the Command-Code
			field set to 323 and the 'R' bit cleared in the
			Command Flags field, is sent from HSS to MME or SGSN.

			< Notify-Answer> ::= < Diameter Header: 323, PXY, 16777251 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
				[ Result-Code ]
				[ Experimental-Result ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
			   *[ Supported-Features ]
			   *[ AVP ]
			   *[ Failed-AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				323, 					/* Code */
				"Notify-Answer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Result-Code",							RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result",					RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host", 							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm", 						RULE_REQUIRED,   -1,  1 }
				,{  "Supported-Features", 					RULE_OPTIONAL,   -1, -1 }
				,{  "Failed-AVP", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 						RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* ME-Identity-Check-Request (ECR) Command - 3GPP TS 29.272 #7.2.19 */
		{
			/*

			The ME-Identity-Check-Request (ECR) command, indicated by the
			Command-Code field set to 324 and the 'R' bit set
			in the Command Flags field, is sent from MME or SGSN to EIR.

			< ME-Identity-Check-Request > ::= < Diameter Header: 324, REQ, PXY, 16777252 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
				[ Destination-Host ]
				{ Destination-Realm }
				{ Terminal-Information }
				[ User-Name ]
			   *[ AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				324, 					/* Code */
				"ME-Identity-Check-Request", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host", 							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm", 						RULE_REQUIRED,   -1,  1 }
				,{  "Destination-Host", 					RULE_OPTIONAL,   -1,  1 }
				,{  "Destination-Realm", 					RULE_REQUIRED,   -1,  1 }
				,{  "Terminal-Information",					RULE_REQUIRED,   -1,  1 }
				,{  "User-Name", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Proxy-Info", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 						RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* ME-Identity-Check-Answer (ECA) Command - 3GPP TS 29.272 #7.2.20 */
		{
			/*

			The ME-Identity-Check-Answer (ECA) command, indicated by the
			Command-Code field set to 324 and the 'R' bit
			cleared in the Command Flags field, is sent from EIR to MME or SGSN.

			< ME-Identity-Check-Answer> ::= < Diameter Header: 324, PXY, 16777252 >
				< Session-Id >
				[ Vendor-Specific-Application-Id ]
				[ Result-Code ]
				[ Experimental-Result ]
				{ Auth-Session-State }
				{ Origin-Host }
				{ Origin-Realm }
				[ Equipment-Status ]
			   *[ AVP ]
			   *[ Failed-AVP ]
			   *[ Proxy-Info ]
			   *[ Route-Record ]
			 */
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				324, 					/* Code */
				"ME-Identity-Check-Answer", 			/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, 	/* Fixed flags */
				CMD_FLAG_PROXIABLE 						/* Fixed flag values */
			};

			struct local_rules_definition rules[] =
			{
				 {  "Session-Id", 							RULE_FIXED_HEAD, -1,  1 }
				,{  "Vendor-Specific-Application-Id",		RULE_OPTIONAL,   -1,  1 }
				,{  "Result-Code",							RULE_OPTIONAL,   -1,  1 }
				,{  "Experimental-Result",					RULE_OPTIONAL,   -1,  1 }
				,{  "Auth-Session-State", 					RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Host", 							RULE_REQUIRED,   -1,  1 }
				,{  "Origin-Realm", 						RULE_REQUIRED,   -1,  1 }
				,{  "Equipment-Status",						RULE_OPTIONAL,   -1,  1 }
				,{  "User-Name", 							RULE_OPTIONAL,   -1,  1 }
				,{  "Failed-AVP", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Proxy-Info", 							RULE_OPTIONAL,   -1, -1 }
				,{  "Route-Record", 						RULE_OPTIONAL,   -1, -1 }
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a_dict, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
	}

	LOG_D( "Dictionary Extension 'S6A from 3GPP standard v.10.5' initialized");
	return 0;
}
EXTENSION_ENTRY("dict_s6a", dict_s6a_init, "dict_nas_mipv6");
