/*********************************************************************************************************
 * Software License Agreement (BSD License)                                                               *
 * Author: Thomas Klausner <tk@giga.or.at>                                                                *
 *                                                                                                        *
 * Copyright (c) 2013, Thomas Klausner                                                                    *
 * All rights reserved.                                                                                   *
 *                                                                                                        *
 * Written under contract by nfotex IT GmbH, http://nfotex.com/                                           *
 *                                                                                                        *
 * Redistribution and use of this software in source and binary forms, with or without modification, are  *
 * permitted provided that the following conditions are met:                                              *
 *                                                                                                        *
 * * Redistributions of source code must retain the above                                                 *
 *   copyright notice, this list of conditions and the                                                    *
 *   following disclaimer.                                                                                *
 *                                                                                                        *
 * * Redistributions in binary form must reproduce the above                                              *
 *   copyright notice, this list of conditions and the                                                    *
 *   following disclaimer in the documentation and/or other                                               *
 *   materials provided with the distribution.                                                            *
 *                                                                                                        *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT     *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                             *
 *********************************************************************************************************/

/* 
 * Dictionary definitions for objects specified for DCCA by 3GPP.
 *
 * This extensions contains a lot of AVPs from various 3GPP standards
 * documents, and some rules for the grouped AVPs described therein.
 *
 * This extension does not contain ALL AVPs described by 3GPP, but
 * quite a big number of them.
 *
 * When extending the AVPs, please edit dict_dcca_3gpp.org instead and
 * create pastable code with contrib/tools/org_to_fd.pl.
 *
 * Some points of consideration:
 * 1. This dictionary could be split up per document.
 *
 * + pro: you can only load the AVPs/Rules you're interested in ->
 * smaller memory size
 *
 * - con: the documents use AVPs from each other A LOT, so setting the
 * dependencies correctly will be annoying
 *
 * - con: you need to load all of them as extensions
 *
 * 2. This dictionary contains ONE AVP in the "3GPP2" vendor space,
 * since I found it wasteful to write a separate dictionary just for
 * one AVP. Also, it is defined in a 3GPP document.
 *
 * 3. While there are quite a number of rules here already, many more
 * are missing. I've only added rules for those grouped AVPs or
 * commands in which I was concretely interested so far; many more
 * will need to be added to make this complete.
 *
 * That being said, I hope this will be useful for you.
 *
 */


/*
 * Some comments on the 3GPP Standards documents themselves:
 *
 * 1. It would be good if 29.061 was reviewed to check for each AVP if
 * it is Mandatory or not. The data currently in the document does not
 * match what was in the previous version of the freeDiameter
 * extension (the one that existedbefore I rewrote it) or what I saw
 * so far. IIRC, even the table and the document contradict each
 * other. The AVP table is also missing an entry for
 * "External-Identifier", 28.
 *
 * 2. 29.140 has conflicting AVP names with other documents:
 *   - Sequence-Number is also in 32.329
 *   - Recipient-Address is also in 32.299
 *   - Status is also in 32.299
 *
 * 3. 29.229 has name conflict with 29.329 about User-Data (different
 * AVP code 702, instead of 606) -- the weird thing is, the latter
 * uses some AVPs from the former, but not this one.
*/
#include <freeDiameter/extension.h>


/* The content of this file follows the same structure as dict_base_proto.c */

#define CHECK_dict_new( _type, _data, _parent, _ref )	\
	CHECK_FCT(  fd_dict_new( fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref))  );

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct local_rules_definition {
	struct dict_avp_request avp_vendor_plus_name;
	enum rule_position	position;
	int 			min;
	int			max;
};

#define RULE_ORDER( _position ) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0 )

/* Attention! This version of the macro uses AVP_BY_NAME_AND_VENDOR, in contrast to most other copies! */
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
			AVP_BY_NAME_AND_VENDOR, 							\
			&(_rulearray)[__ar].avp_vendor_plus_name,					\
			&__data.rule_avp, 0 ) );							\
		if ( !__data.rule_avp ) {								\
			TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_vendor_plus_name.avp_name);		\
			return ENOENT;									\
		}											\
		CHECK_FCT_DO( fd_dict_new( fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL),	\
			{							        		\
				TRACE_DEBUG(INFO, "Error on rule with AVP '%s'",      			\
					    (_rulearray)[__ar].avp_vendor_plus_name.avp_name);		\
				return EINVAL;					      			\
			} );							      			\
	}									      			\
}

#define enumval_def_u32( _val_, _str_ ) \
		{ _str_, 		{ .u32 = _val_ }}

#define enumval_def_os( _len_, _val_, _str_ ) \
		{ _str_, 		{ .os = { .data = (unsigned char *)_val_, .len = _len_ }}}


static int dict_dcca_3gpp_entry(char * conffile)
{
	/* Applications section */
	{		
                /* Create the vendors */
                {
                        struct dict_vendor_data vendor_data = { 10415, "3GPP" };
                        CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_VENDOR, &vendor_data, NULL, NULL));
                }
                {
                        struct dict_vendor_data vendor_data = { 5535, "3GPP2" };
                        CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_VENDOR, &vendor_data, NULL, NULL));
                }

	}
	

	struct dict_object * Address_type;
	struct dict_object * DiameterIdentity_type;
	struct dict_object * DiameterURI_type;
	struct dict_object * IPFilterRule_type;
	struct dict_object * Time_type;
	struct dict_object * UTF8String_type;

	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Address", &Address_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterIdentity", &DiameterIdentity_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterURI", &DiameterURI_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "IPFilterRule", &IPFilterRule_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Time", &Time_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);
	

	/* The following is created automatically. Do not modify. */
	/* Changes will be lost during the next update. Modify the source org file instead. */

	/* 3GPP 29.061-c00 (12.0.0 2012.12.20)                          */
	/* 3GPP 29.061 is not very clear and self-inconsistent about M  */
	/* for this reason, other sources are assumed more trustworthy  */
	/* M inconsistently specified                                   */
	/* 3GPP-IMSI */
	{
		struct dict_avp_data data = {
			1,	/* Code */
			10415,	/* Vendor */
			"3GPP-IMSI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* 29.061 says OctetString; dumps say UInt32; manually changed  */
	/* 29.061 says MUST NOT M; dumps say MUST                       */
	/* 3GPP-Charging-Id */
	{
		struct dict_avp_data data = {
			2,	/* Code */
			10415,	/* Vendor */
			"3GPP-Charging-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 29.061 says MUST NOT M; dumps say MUST                       */
	/* 3GPP-PDP-Type */
	{
		struct dict_avp_data data = {
			3,	/* Code */
			10415,	/* Vendor */
			"3GPP-PDP-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(3GPP-PDP-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* 3GPP-CG-Address */
	{
		struct dict_avp_data data = {
			4,	/* Code */
			10415,	/* Vendor */
			"3GPP-CG-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-GPRS-Negotiated-QoS-Profile */
	{
		struct dict_avp_data data = {
			5,	/* Code */
			10415,	/* Vendor */
			"3GPP-GPRS-Negotiated-QoS-Profile",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* M inconsistently specified; old contrib/3gg says MUST NOT    */
	/* 3GPP-SGSN-Address */
	{
		struct dict_avp_data data = {
			6,	/* Code */
			10415,	/* Vendor */
			"3GPP-SGSN-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 3GPP-GGSN-Address */
	{
		struct dict_avp_data data = {
			7,	/* Code */
			10415,	/* Vendor */
			"3GPP-GGSN-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 29.061 says MUST NOT M; dumps say MUST                       */
	/* 3GPP-IMSI-MCC-MNC */
	{
		struct dict_avp_data data = {
			8,	/* Code */
			10415,	/* Vendor */
			"3GPP-IMSI-MCC-MNC",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-GGSN-MCC-MNC */
	{
		struct dict_avp_data data = {
			9,	/* Code */
			10415,	/* Vendor */
			"3GPP-GGSN-MCC-MNC",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-NSAPI */
	{
		struct dict_avp_data data = {
			10,	/* Code */
			10415,	/* Vendor */
			"3GPP-NSAPI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* added manually, missing in AVP table                         */
	/* 3GPP-Session-Stop-Indicator */
	{
		struct dict_avp_data data = {
			11,	/* Code */
			10415,	/* Vendor */
			"3GPP-Session-Stop-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-Selection-Mode */
	{
		struct dict_avp_data data = {
			12,	/* Code */
			10415,	/* Vendor */
			"3GPP-Selection-Mode",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-Charging-Characteristics */
	{
		struct dict_avp_data data = {
			13,	/* Code */
			10415,	/* Vendor */
			"3GPP-Charging-Characteristics",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-CG-IPv6-Address */
	{
		struct dict_avp_data data = {
			14,	/* Code */
			10415,	/* Vendor */
			"3GPP-CG-IPv6-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* M inconsistently specified                                   */
	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-SGSN-IPv6-Address */
	{
		struct dict_avp_data data = {
			15,	/* Code */
			10415,	/* Vendor */
			"3GPP-SGSN-IPv6-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-GGSN-IPv6-Address */
	{
		struct dict_avp_data data = {
			16,	/* Code */
			10415,	/* Vendor */
			"3GPP-GGSN-IPv6-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-IPv6-DNS-Servers */
	{
		struct dict_avp_data data = {
			17,	/* Code */
			10415,	/* Vendor */
			"3GPP-IPv6-DNS-Servers",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 29.061 says MUST NOT M; old contrib/3gpp says MUST           */
	/* 3GPP-SGSN-MCC-MNC */
	{
		struct dict_avp_data data = {
			18,	/* Code */
			10415,	/* Vendor */
			"3GPP-SGSN-MCC-MNC",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* M inconsistently specified                                   */
	/* 3GPP-IMEISV */
	{
		struct dict_avp_data data = {
			20,	/* Code */
			10415,	/* Vendor */
			"3GPP-IMEISV",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* M inconsistently specified                                   */
	/* 3GPP-RAT-Type */
	{
		struct dict_avp_data data = {
			21,	/* Code */
			10415,	/* Vendor */
			"3GPP-RAT-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* M inconsistently specified                                   */
	/* 3GPP-User-Location-Info */
	{
		struct dict_avp_data data = {
			22,	/* Code */
			10415,	/* Vendor */
			"3GPP-User-Location-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* M inconsistently specified                                   */
	/* 3GPP-MS-TimeZone */
	{
		struct dict_avp_data data = {
			23,	/* Code */
			10415,	/* Vendor */
			"3GPP-MS-TimeZone",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 3GPP-CAMEL-Charging-Info */
	{
		struct dict_avp_data data = {
			24,	/* Code */
			10415,	/* Vendor */
			"3GPP-CAMEL-Charging-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 3GPP-Packet-Filter */
	{
		struct dict_avp_data data = {
			25,	/* Code */
			10415,	/* Vendor */
			"3GPP-Packet-Filter",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 3GPP-Negotiated-DSCP */
	{
		struct dict_avp_data data = {
			26,	/* Code */
			10415,	/* Vendor */
			"3GPP-Negotiated-DSCP",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 3GPP-Allocate-IP-Type */
	{
		struct dict_avp_data data = {
			27,	/* Code */
			10415,	/* Vendor */
			"3GPP-Allocate-IP-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* added manually, missing in AVP table                         */
	/* External-Identifier */
	{
		struct dict_avp_data data = {
			28,	/* Code */
			10415,	/* Vendor */
			"External-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* TMGI */
	{
		struct dict_avp_data data = {
			900,	/* Code */
			10415,	/* Vendor */
			"TMGI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Required-MBMS-Bearer-Capabilities */
	{
		struct dict_avp_data data = {
			901,	/* Code */
			10415,	/* Vendor */
			"Required-MBMS-Bearer-Capabilities",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* MBMS-StartStop-Indication */
	{
		struct dict_avp_data data = {
			902,	/* Code */
			10415,	/* Vendor */
			"MBMS-StartStop-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MBMS-StartStop-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MBMS-Service-Area */
	{
		struct dict_avp_data data = {
			903,	/* Code */
			10415,	/* Vendor */
			"MBMS-Service-Area",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-Session-Duration */
	{
		struct dict_avp_data data = {
			904,	/* Code */
			10415,	/* Vendor */
			"MBMS-Session-Duration",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Alternative-APN */
	{
		struct dict_avp_data data = {
			905,	/* Code */
			10415,	/* Vendor */
			"Alternative-APN",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* MBMS-Service-Type */
	{
		struct dict_avp_data data = {
			906,	/* Code */
			10415,	/* Vendor */
			"MBMS-Service-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MBMS-Service-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MBMS-2G-3G-Indicator */
	{
		struct dict_avp_data data = {
			907,	/* Code */
			10415,	/* Vendor */
			"MBMS-2G-3G-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MBMS-2G-3G-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MBMS-Session-Identity */
	{
		struct dict_avp_data data = {
			908,	/* Code */
			10415,	/* Vendor */
			"MBMS-Session-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* RAI */
	{
		struct dict_avp_data data = {
			909,	/* Code */
			10415,	/* Vendor */
			"RAI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Additional-MBMS-Trace-Info */
	{
		struct dict_avp_data data = {
			910,	/* Code */
			10415,	/* Vendor */
			"Additional-MBMS-Trace-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-Time-To-Data-Transfer */
	{
		struct dict_avp_data data = {
			911,	/* Code */
			10415,	/* Vendor */
			"MBMS-Time-To-Data-Transfer",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-Session-Repetition-Number */
	{
		struct dict_avp_data data = {
			912,	/* Code */
			10415,	/* Vendor */
			"MBMS-Session-Repetition-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-Required-QoS */
	{
		struct dict_avp_data data = {
			913,	/* Code */
			10415,	/* Vendor */
			"MBMS-Required-QoS",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* MBMS-Counting-Information */
	{
		struct dict_avp_data data = {
			914,	/* Code */
			10415,	/* Vendor */
			"MBMS-Counting-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MBMS-Counting-Information)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MBMS-User-Data-Mode-Indication */
	{
		struct dict_avp_data data = {
			915,	/* Code */
			10415,	/* Vendor */
			"MBMS-User-Data-Mode-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MBMS-User-Data-Mode-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MBMS-GGSN-Address */
	{
		struct dict_avp_data data = {
			916,	/* Code */
			10415,	/* Vendor */
			"MBMS-GGSN-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-GGSN-IPv6-Address */
	{
		struct dict_avp_data data = {
			917,	/* Code */
			10415,	/* Vendor */
			"MBMS-GGSN-IPv6-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-BMSC-SSM-IP-Address */
	{
		struct dict_avp_data data = {
			918,	/* Code */
			10415,	/* Vendor */
			"MBMS-BMSC-SSM-IP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-BMSC-SSM-IPv6-Address */
	{
		struct dict_avp_data data = {
			919,	/* Code */
			10415,	/* Vendor */
			"MBMS-BMSC-SSM-IPv6-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-Flow-Identifier */
	{
		struct dict_avp_data data = {
			920,	/* Code */
			10415,	/* Vendor */
			"MBMS-Flow-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* CN-IP-Multicast-Distribution */
	{
		struct dict_avp_data data = {
			921,	/* Code */
			10415,	/* Vendor */
			"CN-IP-Multicast-Distribution",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(CN-IP-Multicast-Distribution)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MBMS-HC-Indicator */
	{
		struct dict_avp_data data = {
			922,	/* Code */
			10415,	/* Vendor */
			"MBMS-HC-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MBMS-HC-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* 3GPP 29.140-700 (7.0.0 2007.07.05)                           */
	/* Served-User-Identity */
	{
		struct dict_avp_data data = {
			1100,	/* Code */
			10415,	/* Vendor */
			"Served-User-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* reuses: MSISDN                                               */
	/* VASP-ID */
	{
		struct dict_avp_data data = {
			1101,	/* Code */
			10415,	/* Vendor */
			"VASP-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* VAS-ID */
	{
		struct dict_avp_data data = {
			1102,	/* Code */
			10415,	/* Vendor */
			"VAS-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Trigger-Event */
	{
		struct dict_avp_data data = {
			1103,	/* Code */
			10415,	/* Vendor */
			"Trigger-Event",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Trigger-Event)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* reuses: 3GPP-IMSI                                            */
	/* Sender-Address */
	{
		struct dict_avp_data data = {
			1104,	/* Code */
			10415,	/* Vendor */
			"Sender-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Initial-Recipient-Address */
	{
		struct dict_avp_data data = {
			1105,	/* Code */
			10415,	/* Vendor */
			"Initial-Recipient-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Result-Recipient-Address */
	{
		struct dict_avp_data data = {
			1106,	/* Code */
			10415,	/* Vendor */
			"Result-Recipient-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* conflicts with one in (more common) 32.329                   */
	/* Sequence-Number-29.140 */
	{
		struct dict_avp_data data = {
			1107,	/* Code */
			10415,	/* Vendor */
			"Sequence-Number-29.140",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* conflicts with one in (more common) 32.299                   */
	/* Recipient-Address-29.140 */
	{
		struct dict_avp_data data = {
			1108,	/* Code */
			10415,	/* Vendor */
			"Recipient-Address-29.140",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Routeing-Address */
	{
		struct dict_avp_data data = {
			1109,	/* Code */
			10415,	/* Vendor */
			"Routeing-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Originating-Interface */
	{
		struct dict_avp_data data = {
			1110,	/* Code */
			10415,	/* Vendor */
			"Originating-Interface",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Originating-Interface)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Delivery-Report */
	{
		struct dict_avp_data data = {
			1111,	/* Code */
			10415,	/* Vendor */
			"Delivery-Report",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Delivery-Report)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Read-Reply */
	{
		struct dict_avp_data data = {
			1112,	/* Code */
			10415,	/* Vendor */
			"Read-Reply",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Read-Reply)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Sender-Visibility */
	{
		struct dict_avp_data data = {
			1113,	/* Code */
			10415,	/* Vendor */
			"Sender-Visibility",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Sender-Visibility)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Service-Key */
	{
		struct dict_avp_data data = {
			1114,	/* Code */
			10415,	/* Vendor */
			"Service-Key",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Billing-Information */
	{
		struct dict_avp_data data = {
			1115,	/* Code */
			10415,	/* Vendor */
			"Billing-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* conflicts with one in (more common) 32.299                   */
	/* Status-29.140 */
	{
		struct dict_avp_data data = {
			1116,	/* Code */
			10415,	/* Vendor */
			"Status-29.140",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Status-Code */
	{
		struct dict_avp_data data = {
			1117,	/* Code */
			10415,	/* Vendor */
			"Status-Code",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Status-Text */
	{
		struct dict_avp_data data = {
			1118,	/* Code */
			10415,	/* Vendor */
			"Status-Text",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Routeing-Address-Resolution */
	{
		struct dict_avp_data data = {
			1119,	/* Code */
			10415,	/* Vendor */
			"Routeing-Address-Resolution",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Routeing-Address-Resolution)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* 3GPP 29.173-c00 (12.0.0 2013.03.13)                          */
	/* LMSI */
	{
		struct dict_avp_data data = {
			2400,	/* Code */
			10415,	/* Vendor */
			"LMSI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Serving-Node */
	{
		struct dict_avp_data data = {
			2401,	/* Code */
			10415,	/* Vendor */
			"Serving-Node",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MME-Name */
	{
		struct dict_avp_data data = {
			2402,	/* Code */
			10415,	/* Vendor */
			"MME-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterIdentity_type, NULL);
	};

	/* MSC-Number */
	{
		struct dict_avp_data data = {
			2403,	/* Code */
			10415,	/* Vendor */
			"MSC-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* LCS-Capabilities-Sets */
	{
		struct dict_avp_data data = {
			2404,	/* Code */
			10415,	/* Vendor */
			"LCS-Capabilities-Sets",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* GMLC-Address */
	{
		struct dict_avp_data data = {
			2405,	/* Code */
			10415,	/* Vendor */
			"GMLC-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Additional-Serving-Node */
	{
		struct dict_avp_data data = {
			2406,	/* Code */
			10415,	/* Vendor */
			"Additional-Serving-Node",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PPR-Address */
	{
		struct dict_avp_data data = {
			2407,	/* Code */
			10415,	/* Vendor */
			"PPR-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* MME-Realm */
	{
		struct dict_avp_data data = {
			2408,	/* Code */
			10415,	/* Vendor */
			"MME-Realm",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterIdentity_type, NULL);
	};

	/* SGSN-Name */
	{
		struct dict_avp_data data = {
			2409,	/* Code */
			10415,	/* Vendor */
			"SGSN-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterIdentity_type, NULL);
	};

	/* SGSN-Realm */
	{
		struct dict_avp_data data = {
			2410,	/* Code */
			10415,	/* Vendor */
			"SGSN-Realm",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterIdentity_type, NULL);
	};

	/* 3GPP 29.210-670 (6.7.0 2006-12-18)                           */
	/* PDP-Session-Operation */
	{
		struct dict_avp_data data = {
			1015,	/* Code */
			10415,	/* Vendor */
			"PDP-Session-Operation",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PDP-Session-Operation)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* 3GPP 29.212-c00 (12.0.0 2013.03.15)                          */
	/* Gx-specific                                                  */
	/* ADC-Revalidation-Time */
	{
		struct dict_avp_data data = {
			2801,	/* Code */
			10415,	/* Vendor */
			"ADC-Revalidation-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* ADC-Rule-Install */
	{
		struct dict_avp_data data = {
			1092,	/* Code */
			10415,	/* Vendor */
			"ADC-Rule-Install",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ADC-Rule-Remove */
	{
		struct dict_avp_data data = {
			1093,	/* Code */
			10415,	/* Vendor */
			"ADC-Rule-Remove",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ADC-Rule-Definition */
	{
		struct dict_avp_data data = {
			1094,	/* Code */
			10415,	/* Vendor */
			"ADC-Rule-Definition",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ADC-Rule-Base-Name */
	{
		struct dict_avp_data data = {
			1095,	/* Code */
			10415,	/* Vendor */
			"ADC-Rule-Base-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* ADC-Rule-Name */
	{
		struct dict_avp_data data = {
			1096,	/* Code */
			10415,	/* Vendor */
			"ADC-Rule-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ADC-Rule-Report */
	{
		struct dict_avp_data data = {
			1097,	/* Code */
			10415,	/* Vendor */
			"ADC-Rule-Report",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Application-Detection-Information */
	{
		struct dict_avp_data data = {
			1098,	/* Code */
			10415,	/* Vendor */
			"Application-Detection-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Access-Network-Charging-Identifier-Gx */
	{
		struct dict_avp_data data = {
			1022,	/* Code */
			10415,	/* Vendor */
			"Access-Network-Charging-Identifier-Gx",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Allocation-Retention-Priority */
	{
		struct dict_avp_data data = {
			1034,	/* Code */
			10415,	/* Vendor */
			"Allocation-Retention-Priority",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* AN-GW-Address */
	{
		struct dict_avp_data data = {
			1050,	/* Code */
			10415,	/* Vendor */
			"AN-GW-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* APN-Aggregate-Max-Bitrate-DL */
	{
		struct dict_avp_data data = {
			1040,	/* Code */
			10415,	/* Vendor */
			"APN-Aggregate-Max-Bitrate-DL",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* APN-Aggregate-Max-Bitrate-UL */
	{
		struct dict_avp_data data = {
			1041,	/* Code */
			10415,	/* Vendor */
			"APN-Aggregate-Max-Bitrate-UL",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Bearer-Control-Mode */
	{
		struct dict_avp_data data = {
			1023,	/* Code */
			10415,	/* Vendor */
			"Bearer-Control-Mode",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Bearer-Control-Mode)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Bearer-Identifier */
	{
		struct dict_avp_data data = {
			1020,	/* Code */
			10415,	/* Vendor */
			"Bearer-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Bearer-Operation */
	{
		struct dict_avp_data data = {
			1021,	/* Code */
			10415,	/* Vendor */
			"Bearer-Operation",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Bearer-Operation)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Bearer-Usage */
	{
		struct dict_avp_data data = {
			1000,	/* Code */
			10415,	/* Vendor */
			"Bearer-Usage",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Bearer-Usage)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Charging-Rule-Install */
	{
		struct dict_avp_data data = {
			1001,	/* Code */
			10415,	/* Vendor */
			"Charging-Rule-Install",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Charging-Rule-Remove */
	{
		struct dict_avp_data data = {
			1002,	/* Code */
			10415,	/* Vendor */
			"Charging-Rule-Remove",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Charging-Rule-Definition */
	{
		struct dict_avp_data data = {
			1003,	/* Code */
			10415,	/* Vendor */
			"Charging-Rule-Definition",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Charging-Rule-Base-Name */
	{
		struct dict_avp_data data = {
			1004,	/* Code */
			10415,	/* Vendor */
			"Charging-Rule-Base-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Charging-Rule-Name */
	{
		struct dict_avp_data data = {
			1005,	/* Code */
			10415,	/* Vendor */
			"Charging-Rule-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Charging-Rule-Report */
	{
		struct dict_avp_data data = {
			1018,	/* Code */
			10415,	/* Vendor */
			"Charging-Rule-Report",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Charging-Correlation-Indicator */
	{
		struct dict_avp_data data = {
			1073,	/* Code */
			10415,	/* Vendor */
			"Charging-Correlation-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Charging-Correlation-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* CoA-IP-Address */
	{
		struct dict_avp_data data = {
			1035,	/* Code */
			10415,	/* Vendor */
			"CoA-IP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* CoA-Information */
	{
		struct dict_avp_data data = {
			1039,	/* Code */
			10415,	/* Vendor */
			"CoA-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* CSG-Information-Reporting */
	{
		struct dict_avp_data data = {
			1071,	/* Code */
			10415,	/* Vendor */
			"CSG-Information-Reporting",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(CSG-Information-Reporting)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Default-EPS-Bearer-QoS */
	{
		struct dict_avp_data data = {
			1049,	/* Code */
			10415,	/* Vendor */
			"Default-EPS-Bearer-QoS",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Event-Report-Indication */
	{
		struct dict_avp_data data = {
			1033,	/* Code */
			10415,	/* Vendor */
			"Event-Report-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Event-Trigger */
	{
		struct dict_avp_data data = {
			1006,	/* Code */
			10415,	/* Vendor */
			"Event-Trigger",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Event-Trigger)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Flow-Direction */
	{
		struct dict_avp_data data = {
			1080,	/* Code */
			10415,	/* Vendor */
			"Flow-Direction",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Flow-Direction)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Flow-Information */
	{
		struct dict_avp_data data = {
			1058,	/* Code */
			10415,	/* Vendor */
			"Flow-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Flow-Label */
	{
		struct dict_avp_data data = {
			1057,	/* Code */
			10415,	/* Vendor */
			"Flow-Label",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* IP-CAN-Type */
	{
		struct dict_avp_data data = {
			1027,	/* Code */
			10415,	/* Vendor */
			"IP-CAN-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(IP-CAN-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Guaranteed-Bitrate-DL */
	{
		struct dict_avp_data data = {
			1025,	/* Code */
			10415,	/* Vendor */
			"Guaranteed-Bitrate-DL",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Guaranteed-Bitrate-UL */
	{
		struct dict_avp_data data = {
			1026,	/* Code */
			10415,	/* Vendor */
			"Guaranteed-Bitrate-UL",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* HeNB-Local-IP-Address */
	{
		struct dict_avp_data data = {
			2804,	/* Code */
			10415,	/* Vendor */
			"HeNB-Local-IP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Metering-Method */
	{
		struct dict_avp_data data = {
			1007,	/* Code */
			10415,	/* Vendor */
			"Metering-Method",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Metering-Method)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Monitoring-Key */
	{
		struct dict_avp_data data = {
			1066,	/* Code */
			10415,	/* Vendor */
			"Monitoring-Key",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Mute-Notification */
	{
		struct dict_avp_data data = {
			2809,	/* Code */
			10415,	/* Vendor */
			"Mute-Notification",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Mute-Notification)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Monitoring-Time */
	{
		struct dict_avp_data data = {
			2810,	/* Code */
			10415,	/* Vendor */
			"Monitoring-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Network-Request-Support */
	{
		struct dict_avp_data data = {
			1024,	/* Code */
			10415,	/* Vendor */
			"Network-Request-Support",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Network-Request-Support)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Offline */
	{
		struct dict_avp_data data = {
			1008,	/* Code */
			10415,	/* Vendor */
			"Offline",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Offline)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Online */
	{
		struct dict_avp_data data = {
			1009,	/* Code */
			10415,	/* Vendor */
			"Online",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Online)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Packet-Filter-Content */
	{
		struct dict_avp_data data = {
			1059,	/* Code */
			10415,	/* Vendor */
			"Packet-Filter-Content",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
	};

	/* Packet-Filter-Identifier */
	{
		struct dict_avp_data data = {
			1060,	/* Code */
			10415,	/* Vendor */
			"Packet-Filter-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Packet-Filter-Information */
	{
		struct dict_avp_data data = {
			1061,	/* Code */
			10415,	/* Vendor */
			"Packet-Filter-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Packet-Filter-Operation */
	{
		struct dict_avp_data data = {
			1062,	/* Code */
			10415,	/* Vendor */
			"Packet-Filter-Operation",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Packet-Filter-Operation)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Packet-Filter-Usage */
	{
		struct dict_avp_data data = {
			1072,	/* Code */
			10415,	/* Vendor */
			"Packet-Filter-Usage",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Packet-Filter-Usage)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PDN-Connection-ID */
	{
		struct dict_avp_data data = {
			1065,	/* Code */
			10415,	/* Vendor */
			"PDN-Connection-ID",	/* Name */
			AVP_FLAG_VENDOR,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Precedence */
	{
		struct dict_avp_data data = {
			1010,	/* Code */
			10415,	/* Vendor */
			"Precedence",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Pre-emption-Capability */
	{
		struct dict_avp_data data = {
			1047,	/* Code */
			10415,	/* Vendor */
			"Pre-emption-Capability",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Pre-emption-Capability)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Pre-emption-Vulnerability */
	{
		struct dict_avp_data data = {
			1048,	/* Code */
			10415,	/* Vendor */
			"Pre-emption-Vulnerability",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Pre-emption-Vulnerability)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Priority-Level */
	{
		struct dict_avp_data data = {
			1046,	/* Code */
			10415,	/* Vendor */
			"Priority-Level",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Redirect-Information */
	{
		struct dict_avp_data data = {
			1085,	/* Code */
			10415,	/* Vendor */
			"Redirect-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Redirect-Support */
	{
		struct dict_avp_data data = {
			1086,	/* Code */
			10415,	/* Vendor */
			"Redirect-Support",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Redirect-Support)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Reporting-Level */
	{
		struct dict_avp_data data = {
			1011,	/* Code */
			10415,	/* Vendor */
			"Reporting-Level",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Reporting-Level)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Routing-Filter */
	{
		struct dict_avp_data data = {
			1078,	/* Code */
			10415,	/* Vendor */
			"Routing-Filter",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Routing-IP-Address */
	{
		struct dict_avp_data data = {
			1079,	/* Code */
			10415,	/* Vendor */
			"Routing-IP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Routing-Rule-Definition */
	{
		struct dict_avp_data data = {
			1076,	/* Code */
			10415,	/* Vendor */
			"Routing-Rule-Definition",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Routing-Rule-Identifier */
	{
		struct dict_avp_data data = {
			1077,	/* Code */
			10415,	/* Vendor */
			"Routing-Rule-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Routing-Rule-Install */
	{
		struct dict_avp_data data = {
			1081,	/* Code */
			10415,	/* Vendor */
			"Routing-Rule-Install",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Routing-Rule-Remove */
	{
		struct dict_avp_data data = {
			1075,	/* Code */
			10415,	/* Vendor */
			"Routing-Rule-Remove",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PCC-Rule-Status */
	{
		struct dict_avp_data data = {
			1019,	/* Code */
			10415,	/* Vendor */
			"PCC-Rule-Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PCC-Rule-Status)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Session-Release-Cause */
	{
		struct dict_avp_data data = {
			1045,	/* Code */
			10415,	/* Vendor */
			"Session-Release-Cause",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Session-Release-Cause)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* TDF-Information */
	{
		struct dict_avp_data data = {
			1087,	/* Code */
			10415,	/* Vendor */
			"TDF-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* TDF-Application-Identifier */
	{
		struct dict_avp_data data = {
			1088,	/* Code */
			10415,	/* Vendor */
			"TDF-Application-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* TDF-Application-Instance-Identifier */
	{
		struct dict_avp_data data = {
			2802,	/* Code */
			10415,	/* Vendor */
			"TDF-Application-Instance-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* TDF-Destination-Host */
	{
		struct dict_avp_data data = {
			1089,	/* Code */
			10415,	/* Vendor */
			"TDF-Destination-Host",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterIdentity_type, NULL);
	};

	/* TDF-Destination-Realm */
	{
		struct dict_avp_data data = {
			1090,	/* Code */
			10415,	/* Vendor */
			"TDF-Destination-Realm",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterIdentity_type, NULL);
	};

	/* TDF-IP-Address */
	{
		struct dict_avp_data data = {
			1091,	/* Code */
			10415,	/* Vendor */
			"TDF-IP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* QoS-Class-Identifier */
	{
		struct dict_avp_data data = {
			1028,	/* Code */
			10415,	/* Vendor */
			"QoS-Class-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(QoS-Class-Identifier)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* QoS-Information */
	{
		struct dict_avp_data data = {
			1016,	/* Code */
			10415,	/* Vendor */
			"QoS-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* QoS-Negotiation */
	{
		struct dict_avp_data data = {
			1029,	/* Code */
			10415,	/* Vendor */
			"QoS-Negotiation",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(QoS-Negotiation)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* QoS-Upgrade */
	{
		struct dict_avp_data data = {
			1030,	/* Code */
			10415,	/* Vendor */
			"QoS-Upgrade",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(QoS-Upgrade)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PS-to-CS-Session-Continuity */
	{
		struct dict_avp_data data = {
			1099,	/* Code */
			10415,	/* Vendor */
			"PS-to-CS-Session-Continuity",	/* Name */
			AVP_FLAG_VENDOR,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PS-to-CS-Session-Continuity)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Resource-Allocation-Notification */
	{
		struct dict_avp_data data = {
			1063,	/* Code */
			10415,	/* Vendor */
			"Resource-Allocation-Notification",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Resource-Allocation-Notification)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Rule-Failure-Code */
	{
		struct dict_avp_data data = {
			1031,	/* Code */
			10415,	/* Vendor */
			"Rule-Failure-Code",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Rule-Failure-Code)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Security-Parameter-Index */
	{
		struct dict_avp_data data = {
			1056,	/* Code */
			10415,	/* Vendor */
			"Security-Parameter-Index",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* TFT-Filter */
	{
		struct dict_avp_data data = {
			1012,	/* Code */
			10415,	/* Vendor */
			"TFT-Filter",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
	};

	/* TFT-Packet-Filter-Information */
	{
		struct dict_avp_data data = {
			1013,	/* Code */
			10415,	/* Vendor */
			"TFT-Packet-Filter-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ToS-Traffic-Class */
	{
		struct dict_avp_data data = {
			1014,	/* Code */
			10415,	/* Vendor */
			"ToS-Traffic-Class",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Tunnel-Header-Filter */
	{
		struct dict_avp_data data = {
			1036,	/* Code */
			10415,	/* Vendor */
			"Tunnel-Header-Filter",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
	};

	/* Tunnel-Header-Length */
	{
		struct dict_avp_data data = {
			1037,	/* Code */
			10415,	/* Vendor */
			"Tunnel-Header-Length",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Tunnel-Information */
	{
		struct dict_avp_data data = {
			1038,	/* Code */
			10415,	/* Vendor */
			"Tunnel-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* RAT-Type */
	{
		struct dict_avp_data data = {
			1032,	/* Code */
			10415,	/* Vendor */
			"RAT-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(RAT-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Revalidation-Time */
	{
		struct dict_avp_data data = {
			1042,	/* Code */
			10415,	/* Vendor */
			"Revalidation-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Rule-Activation-Time */
	{
		struct dict_avp_data data = {
			1043,	/* Code */
			10415,	/* Vendor */
			"Rule-Activation-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* UDP-Source-Port */
	{
		struct dict_avp_data data = {
			2806,	/* Code */
			10415,	/* Vendor */
			"UDP-Source-Port",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* UE-Local-IP-Address */
	{
		struct dict_avp_data data = {
			2805,	/* Code */
			10415,	/* Vendor */
			"UE-Local-IP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Usage-Monitoring-Information */
	{
		struct dict_avp_data data = {
			1067,	/* Code */
			10415,	/* Vendor */
			"Usage-Monitoring-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Rule-Deactivation-Time */
	{
		struct dict_avp_data data = {
			1044,	/* Code */
			10415,	/* Vendor */
			"Rule-Deactivation-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Usage-Monitoring-Level */
	{
		struct dict_avp_data data = {
			1068,	/* Code */
			10415,	/* Vendor */
			"Usage-Monitoring-Level",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Usage-Monitoring-Level)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Usage-Monitoring-Report */
	{
		struct dict_avp_data data = {
			1069,	/* Code */
			10415,	/* Vendor */
			"Usage-Monitoring-Report",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Usage-Monitoring-Report)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Usage-Monitoring-Support */
	{
		struct dict_avp_data data = {
			1070,	/* Code */
			10415,	/* Vendor */
			"Usage-Monitoring-Support",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Usage-Monitoring-Support)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Gxx-specific                                                 */
	/* QoS-Rule-Install */
	{
		struct dict_avp_data data = {
			1051,	/* Code */
			10415,	/* Vendor */
			"QoS-Rule-Install",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* QoS-Rule-Remove */
	{
		struct dict_avp_data data = {
			1052,	/* Code */
			10415,	/* Vendor */
			"QoS-Rule-Remove",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* QoS-Rule-Definition */
	{
		struct dict_avp_data data = {
			1053,	/* Code */
			10415,	/* Vendor */
			"QoS-Rule-Definition",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* QoS-Rule-Name */
	{
		struct dict_avp_data data = {
			1054,	/* Code */
			10415,	/* Vendor */
			"QoS-Rule-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* QoS-Rule-Base-Name */
	{
		struct dict_avp_data data = {
			1074,	/* Code */
			10415,	/* Vendor */
			"QoS-Rule-Base-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* QoS-Rule-Report */
	{
		struct dict_avp_data data = {
			1055,	/* Code */
			10415,	/* Vendor */
			"QoS-Rule-Report",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Session-Linking-Indicator */
	{
		struct dict_avp_data data = {
			1064,	/* Code */
			10415,	/* Vendor */
			"Session-Linking-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Session-Linking-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* S15-specific                                                 */
	/* CS-Service-Qos-Request-Identifier */
	{
		struct dict_avp_data data = {
			2807,	/* Code */
			10415,	/* Vendor */
			"CS-Service-Qos-Request-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* CS-Service-QoS-Request-Operation */
	{
		struct dict_avp_data data = {
			2808,	/* Code */
			10415,	/* Vendor */
			"CS-Service-QoS-Request-Operation",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(CS-Service-QoS-Request-Operation)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* 3GPP 29.214-b80 (11.8.0 2013.03.15)                          */
	/* Abort-Cause */
	{
		struct dict_avp_data data = {
			500,	/* Code */
			10415,	/* Vendor */
			"Abort-Cause",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Abort-Cause)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Access-Network-Charging-Address */
	{
		struct dict_avp_data data = {
			501,	/* Code */
			10415,	/* Vendor */
			"Access-Network-Charging-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Access-Network-Charging-Identifier */
	{
		struct dict_avp_data data = {
			502,	/* Code */
			10415,	/* Vendor */
			"Access-Network-Charging-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Access-Network-Charging-Identifier-Value */
	{
		struct dict_avp_data data = {
			503,	/* Code */
			10415,	/* Vendor */
			"Access-Network-Charging-Identifier-Value",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Acceptable-Service-Info */
	{
		struct dict_avp_data data = {
			526,	/* Code */
			10415,	/* Vendor */
			"Acceptable-Service-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* AF-Application-Identifier */
	{
		struct dict_avp_data data = {
			504,	/* Code */
			10415,	/* Vendor */
			"AF-Application-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* AF-Charging-Identifier */
	{
		struct dict_avp_data data = {
			505,	/* Code */
			10415,	/* Vendor */
			"AF-Charging-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Application-Service-Provider-Identity */
	{
		struct dict_avp_data data = {
			532,	/* Code */
			10415,	/* Vendor */
			"Application-Service-Provider-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Codec-Data */
	{
		struct dict_avp_data data = {
			524,	/* Code */
			10415,	/* Vendor */
			"Codec-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Flow-Description */
	{
		struct dict_avp_data data = {
			507,	/* Code */
			10415,	/* Vendor */
			"Flow-Description",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
	};

	/* Flow-Number */
	{
		struct dict_avp_data data = {
			509,	/* Code */
			10415,	/* Vendor */
			"Flow-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Flows */
	{
		struct dict_avp_data data = {
			510,	/* Code */
			10415,	/* Vendor */
			"Flows",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Flow-Status */
	{
		struct dict_avp_data data = {
			511,	/* Code */
			10415,	/* Vendor */
			"Flow-Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Flow-Status)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Flow-Usage */
	{
		struct dict_avp_data data = {
			512,	/* Code */
			10415,	/* Vendor */
			"Flow-Usage",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Flow-Usage)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Service-URN */
	{
		struct dict_avp_data data = {
			525,	/* Code */
			10415,	/* Vendor */
			"Service-URN",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Specific-Action */
	{
		struct dict_avp_data data = {
			513,	/* Code */
			10415,	/* Vendor */
			"Specific-Action",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Specific-Action)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Max-Requested-Bandwidth-DL */
	{
		struct dict_avp_data data = {
			515,	/* Code */
			10415,	/* Vendor */
			"Max-Requested-Bandwidth-DL",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Max-Requested-Bandwidth-UL */
	{
		struct dict_avp_data data = {
			516,	/* Code */
			10415,	/* Vendor */
			"Max-Requested-Bandwidth-UL",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Media-Component-Description */
	{
		struct dict_avp_data data = {
			517,	/* Code */
			10415,	/* Vendor */
			"Media-Component-Description",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Media-Component-Number */
	{
		struct dict_avp_data data = {
			518,	/* Code */
			10415,	/* Vendor */
			"Media-Component-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Media-Sub-Component */
	{
		struct dict_avp_data data = {
			519,	/* Code */
			10415,	/* Vendor */
			"Media-Sub-Component",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Media-Type */
	{
		struct dict_avp_data data = {
			520,	/* Code */
			10415,	/* Vendor */
			"Media-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Media-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MPS-Identifier */
	{
		struct dict_avp_data data = {
			528,	/* Code */
			10415,	/* Vendor */
			"MPS-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Min-Requested-Bandwidth-DL */
	{
		struct dict_avp_data data = {
			534,	/* Code */
			10415,	/* Vendor */
			"Min-Requested-Bandwidth-DL",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Min-Requested-Bandwidth-UL */
	{
		struct dict_avp_data data = {
			535,	/* Code */
			10415,	/* Vendor */
			"Min-Requested-Bandwidth-UL",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* RR-Bandwidth */
	{
		struct dict_avp_data data = {
			521,	/* Code */
			10415,	/* Vendor */
			"RR-Bandwidth",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* RS-Bandwidth */
	{
		struct dict_avp_data data = {
			522,	/* Code */
			10415,	/* Vendor */
			"RS-Bandwidth",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Service-Info-Status */
	{
		struct dict_avp_data data = {
			527,	/* Code */
			10415,	/* Vendor */
			"Service-Info-Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Service-Info-Status)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* SIP-Forking-Indication */
	{
		struct dict_avp_data data = {
			523,	/* Code */
			10415,	/* Vendor */
			"SIP-Forking-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(SIP-Forking-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Sponsor-Identity */
	{
		struct dict_avp_data data = {
			531,	/* Code */
			10415,	/* Vendor */
			"Sponsor-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Sponsored-Connectivity-Data */
	{
		struct dict_avp_data data = {
			530,	/* Code */
			10415,	/* Vendor */
			"Sponsored-Connectivity-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* AF-Signalling-Protocol */
	{
		struct dict_avp_data data = {
			529,	/* Code */
			10415,	/* Vendor */
			"AF-Signalling-Protocol",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(AF-Signalling-Protocol)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Required-Access-Info */
	{
		struct dict_avp_data data = {
			536,	/* Code */
			10415,	/* Vendor */
			"Required-Access-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Required-Access-Info)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Rx-Request-Type */
	{
		struct dict_avp_data data = {
			533,	/* Code */
			10415,	/* Vendor */
			"Rx-Request-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Rx-Request-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* IP-Domain-Id */
	{
		struct dict_avp_data data = {
			537,	/* Code */
			10415,	/* Vendor */
			"IP-Domain-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 3GPP 29.229-b20 (11.2.0 2012.12.21)                          */
	/* Associated-Identities */
	{
		struct dict_avp_data data = {
			632,	/* Code */
			10415,	/* Vendor */
			"Associated-Identities",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Associated-Registered-Identities */
	{
		struct dict_avp_data data = {
			647,	/* Code */
			10415,	/* Vendor */
			"Associated-Registered-Identities",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Call-ID-SIP-Header */
	{
		struct dict_avp_data data = {
			643,	/* Code */
			10415,	/* Vendor */
			"Call-ID-SIP-Header",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Charging-Information */
	{
		struct dict_avp_data data = {
			618,	/* Code */
			10415,	/* Vendor */
			"Charging-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Confidentiality-Key */
	{
		struct dict_avp_data data = {
			625,	/* Code */
			10415,	/* Vendor */
			"Confidentiality-Key",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Contact */
	{
		struct dict_avp_data data = {
			641,	/* Code */
			10415,	/* Vendor */
			"Contact",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Deregistration-Reason */
	{
		struct dict_avp_data data = {
			615,	/* Code */
			10415,	/* Vendor */
			"Deregistration-Reason",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Digest-Algorithm */
	{
		struct dict_avp_data data = {
			111,	/* Code */
			10415,	/* Vendor */
			"Digest-Algorithm",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Digest-HA1 */
	{
		struct dict_avp_data data = {
			121,	/* Code */
			10415,	/* Vendor */
			"Digest-HA1",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Digest-QoP */
	{
		struct dict_avp_data data = {
			110,	/* Code */
			10415,	/* Vendor */
			"Digest-QoP",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Digest-Realm */
	{
		struct dict_avp_data data = {
			104,	/* Code */
			10415,	/* Vendor */
			"Digest-Realm",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Feature-List */
	{
		struct dict_avp_data data = {
			630,	/* Code */
			10415,	/* Vendor */
			"Feature-List",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Feature-List-ID */
	{
		struct dict_avp_data data = {
			629,	/* Code */
			10415,	/* Vendor */
			"Feature-List-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* From-SIP-Header */
	{
		struct dict_avp_data data = {
			644,	/* Code */
			10415,	/* Vendor */
			"From-SIP-Header",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Identity-with-Emergency-Registration */
	{
		struct dict_avp_data data = {
			651,	/* Code */
			10415,	/* Vendor */
			"Identity-with-Emergency-Registration",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Integrity-Key */
	{
		struct dict_avp_data data = {
			626,	/* Code */
			10415,	/* Vendor */
			"Integrity-Key",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* LIA-Flags */
	{
		struct dict_avp_data data = {
			653,	/* Code */
			10415,	/* Vendor */
			"LIA-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Loose-Route-Indication */
	{
		struct dict_avp_data data = {
			638,	/* Code */
			10415,	/* Vendor */
			"Loose-Route-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Loose-Route-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Mandatory-Capability */
	{
		struct dict_avp_data data = {
			604,	/* Code */
			10415,	/* Vendor */
			"Mandatory-Capability",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Multiple-Registration-Indication */
	{
		struct dict_avp_data data = {
			648,	/* Code */
			10415,	/* Vendor */
			"Multiple-Registration-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Multiple-Registration-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Optional-Capability */
	{
		struct dict_avp_data data = {
			605,	/* Code */
			10415,	/* Vendor */
			"Optional-Capability",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Originating-Request */
	{
		struct dict_avp_data data = {
			633,	/* Code */
			10415,	/* Vendor */
			"Originating-Request",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Originating-Request)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Path */
	{
		struct dict_avp_data data = {
			640,	/* Code */
			10415,	/* Vendor */
			"Path",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Primary-Charging-Collection-Function-Name */
	{
		struct dict_avp_data data = {
			621,	/* Code */
			10415,	/* Vendor */
			"Primary-Charging-Collection-Function-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterURI_type, NULL);
	};

	/* Primary-Event-Charging-Function-Name */
	{
		struct dict_avp_data data = {
			619,	/* Code */
			10415,	/* Vendor */
			"Primary-Event-Charging-Function-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterURI_type, NULL);
	};

	/* Priviledged-Sender-Indication */
	{
		struct dict_avp_data data = {
			652,	/* Code */
			10415,	/* Vendor */
			"Priviledged-Sender-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Priviledged-Sender-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Public-Identity */
	{
		struct dict_avp_data data = {
			601,	/* Code */
			10415,	/* Vendor */
			"Public-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Reason-Code */
	{
		struct dict_avp_data data = {
			616,	/* Code */
			10415,	/* Vendor */
			"Reason-Code",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Reason-Code)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Reason-Info */
	{
		struct dict_avp_data data = {
			617,	/* Code */
			10415,	/* Vendor */
			"Reason-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Record-Route */
	{
		struct dict_avp_data data = {
			646,	/* Code */
			10415,	/* Vendor */
			"Record-Route",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Restoration-Info */
	{
		struct dict_avp_data data = {
			649,	/* Code */
			10415,	/* Vendor */
			"Restoration-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SCSCF-Restoration-Info */
	{
		struct dict_avp_data data = {
			639,	/* Code */
			10415,	/* Vendor */
			"SCSCF-Restoration-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIP-Auth-Data-Item */
	{
		struct dict_avp_data data = {
			612,	/* Code */
			10415,	/* Vendor */
			"SIP-Auth-Data-Item",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIP-Authenticate */
	{
		struct dict_avp_data data = {
			609,	/* Code */
			10415,	/* Vendor */
			"SIP-Authenticate",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIP-Authentication-Context */
	{
		struct dict_avp_data data = {
			611,	/* Code */
			10415,	/* Vendor */
			"SIP-Authentication-Context",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIP-Authentication-Scheme */
	{
		struct dict_avp_data data = {
			608,	/* Code */
			10415,	/* Vendor */
			"SIP-Authentication-Scheme",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* SIP-Authorization */
	{
		struct dict_avp_data data = {
			610,	/* Code */
			10415,	/* Vendor */
			"SIP-Authorization",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIP-Digest-Authenticate */
	{
		struct dict_avp_data data = {
			635,	/* Code */
			10415,	/* Vendor */
			"SIP-Digest-Authenticate",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIP-Item-Number */
	{
		struct dict_avp_data data = {
			613,	/* Code */
			10415,	/* Vendor */
			"SIP-Item-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIP-Number-Auth-Items */
	{
		struct dict_avp_data data = {
			607,	/* Code */
			10415,	/* Vendor */
			"SIP-Number-Auth-Items",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Secondary-Charging-Collection-Function-Name */
	{
		struct dict_avp_data data = {
			622,	/* Code */
			10415,	/* Vendor */
			"Secondary-Charging-Collection-Function-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterURI_type, NULL);
	};

	/* Secondary-Event-Charging-Function-Name */
	{
		struct dict_avp_data data = {
			620,	/* Code */
			10415,	/* Vendor */
			"Secondary-Event-Charging-Function-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, DiameterURI_type, NULL);
	};

	/* Server-Assignment-Type */
	{
		struct dict_avp_data data = {
			614,	/* Code */
			10415,	/* Vendor */
			"Server-Assignment-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Server-Assignment-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Server-Capabilities */
	{
		struct dict_avp_data data = {
			603,	/* Code */
			10415,	/* Vendor */
			"Server-Capabilities",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Server-Name */
	{
		struct dict_avp_data data = {
			602,	/* Code */
			10415,	/* Vendor */
			"Server-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Session-Priority */
	{
		struct dict_avp_data data = {
			650,	/* Code */
			10415,	/* Vendor */
			"Session-Priority",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Session-Priority)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Subscription-Info */
	{
		struct dict_avp_data data = {
			642,	/* Code */
			10415,	/* Vendor */
			"Subscription-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Supported-Applications */
	{
		struct dict_avp_data data = {
			631,	/* Code */
			10415,	/* Vendor */
			"Supported-Applications",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Supported-Features */
	{
		struct dict_avp_data data = {
			628,	/* Code */
			10415,	/* Vendor */
			"Supported-Features",	/* Name */
			AVP_FLAG_VENDOR,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* To-SIP-Header */
	{
		struct dict_avp_data data = {
			645,	/* Code */
			10415,	/* Vendor */
			"To-SIP-Header",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* UAR-Flags */
	{
		struct dict_avp_data data = {
			637,	/* Code */
			10415,	/* Vendor */
			"UAR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* User-Authorization-Type */
	{
		struct dict_avp_data data = {
			623,	/* Code */
			10415,	/* Vendor */
			"User-Authorization-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(User-Authorization-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* User-Data */
	{
		struct dict_avp_data data = {
			606,	/* Code */
			10415,	/* Vendor */
			"User-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* User-Data-Already-Available */
	{
		struct dict_avp_data data = {
			624,	/* Code */
			10415,	/* Vendor */
			"User-Data-Already-Available",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(User-Data-Already-Available)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Visited-Network-Identifier */
	{
		struct dict_avp_data data = {
			600,	/* Code */
			10415,	/* Vendor */
			"Visited-Network-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Wildcarded-Public-Identity */
	{
		struct dict_avp_data data = {
			634,	/* Code */
			10415,	/* Vendor */
			"Wildcarded-Public-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* 3GPP 32.299-b60 (11.6.0 2012.12.21)                          */
	/* AF-Correlation-Information */
	{
		struct dict_avp_data data = {
			1276,	/* Code */
			10415,	/* Vendor */
			"AF-Correlation-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Access-Network-Information */
	{
		struct dict_avp_data data = {
			1263,	/* Code */
			10415,	/* Vendor */
			"Access-Network-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Access-Transfer-Information */
	{
		struct dict_avp_data data = {
			2709,	/* Code */
			10415,	/* Vendor */
			"Access-Transfer-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Access-Transfer-Type */
	{
		struct dict_avp_data data = {
			2710,	/* Code */
			10415,	/* Vendor */
			"Access-Transfer-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Access-Transfer-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Account-Expiration */
	{
		struct dict_avp_data data = {
			2309,	/* Code */
			10415,	/* Vendor */
			"Account-Expiration",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Accumulated-Cost */
	{
		struct dict_avp_data data = {
			2052,	/* Code */
			10415,	/* Vendor */
			"Accumulated-Cost",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Adaptations */
	{
		struct dict_avp_data data = {
			1217,	/* Code */
			10415,	/* Vendor */
			"Adaptations",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Adaptations)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Additional-Content-Information */
	{
		struct dict_avp_data data = {
			1207,	/* Code */
			10415,	/* Vendor */
			"Additional-Content-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Additional-Type-Information */
	{
		struct dict_avp_data data = {
			1205,	/* Code */
			10415,	/* Vendor */
			"Additional-Type-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Address-Data */
	{
		struct dict_avp_data data = {
			897,	/* Code */
			10415,	/* Vendor */
			"Address-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Address-Domain */
	{
		struct dict_avp_data data = {
			898,	/* Code */
			10415,	/* Vendor */
			"Address-Domain",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Address-Type */
	{
		struct dict_avp_data data = {
			899,	/* Code */
			10415,	/* Vendor */
			"Address-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Address-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Addressee-Type */
	{
		struct dict_avp_data data = {
			1208,	/* Code */
			10415,	/* Vendor */
			"Addressee-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Addressee-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Alternate-Charged-Party-Address */
	{
		struct dict_avp_data data = {
			1280,	/* Code */
			10415,	/* Vendor */
			"Alternate-Charged-Party-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* AoC-Cost-Information */
	{
		struct dict_avp_data data = {
			2053,	/* Code */
			10415,	/* Vendor */
			"AoC-Cost-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* AoC-Format */
	{
		struct dict_avp_data data = {
			2310,	/* Code */
			10415,	/* Vendor */
			"AoC-Format",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(AoC-Format)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* AoC-Information */
	{
		struct dict_avp_data data = {
			2054,	/* Code */
			10415,	/* Vendor */
			"AoC-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* AoC-Request-Type */
	{
		struct dict_avp_data data = {
			2055,	/* Code */
			10415,	/* Vendor */
			"AoC-Request-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(AoC-Request-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* AoC-Service */
	{
		struct dict_avp_data data = {
			2311,	/* Code */
			10415,	/* Vendor */
			"AoC-Service",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* AoC-Service-Obligatory-Type */
	{
		struct dict_avp_data data = {
			2312,	/* Code */
			10415,	/* Vendor */
			"AoC-Service-Obligatory-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(AoC-Service-Obligatory-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* AoC-Service-Type */
	{
		struct dict_avp_data data = {
			2313,	/* Code */
			10415,	/* Vendor */
			"AoC-Service-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(AoC-Service-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* AoC-Subscription-Information */
	{
		struct dict_avp_data data = {
			2314,	/* Code */
			10415,	/* Vendor */
			"AoC-Subscription-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Applic-ID */
	{
		struct dict_avp_data data = {
			1218,	/* Code */
			10415,	/* Vendor */
			"Applic-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Application-Server */
	{
		struct dict_avp_data data = {
			836,	/* Code */
			10415,	/* Vendor */
			"Application-Server",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Application-Server-Information */
	{
		struct dict_avp_data data = {
			850,	/* Code */
			10415,	/* Vendor */
			"Application-Server-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Application-Provided-Called-Party-Address */
	{
		struct dict_avp_data data = {
			837,	/* Code */
			10415,	/* Vendor */
			"Application-Provided-Called-Party-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Associated-Party-Address */
	{
		struct dict_avp_data data = {
			2035,	/* Code */
			10415,	/* Vendor */
			"Associated-Party-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Associated-URI */
	{
		struct dict_avp_data data = {
			856,	/* Code */
			10415,	/* Vendor */
			"Associated-URI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Aux-Applic-Info */
	{
		struct dict_avp_data data = {
			1219,	/* Code */
			10415,	/* Vendor */
			"Aux-Applic-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Base-Time-Interval */
	{
		struct dict_avp_data data = {
			1265,	/* Code */
			10415,	/* Vendor */
			"Base-Time-Interval",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Bearer-Service */
	{
		struct dict_avp_data data = {
			854,	/* Code */
			10415,	/* Vendor */
			"Bearer-Service",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* CG-Address */
	{
		struct dict_avp_data data = {
			846,	/* Code */
			10415,	/* Vendor */
			"CG-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* CSG-Access-Mode */
	{
		struct dict_avp_data data = {
			2317,	/* Code */
			10415,	/* Vendor */
			"CSG-Access-Mode",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(CSG-Access-Mode)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* CSG-Membership-Indication */
	{
		struct dict_avp_data data = {
			2318,	/* Code */
			10415,	/* Vendor */
			"CSG-Membership-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(CSG-Membership-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* CUG-Information */
	{
		struct dict_avp_data data = {
			2304,	/* Code */
			10415,	/* Vendor */
			"CUG-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Called-Asserted-Identity */
	{
		struct dict_avp_data data = {
			1250,	/* Code */
			10415,	/* Vendor */
			"Called-Asserted-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Called-Party-Address */
	{
		struct dict_avp_data data = {
			832,	/* Code */
			10415,	/* Vendor */
			"Called-Party-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Calling-Party-Address */
	{
		struct dict_avp_data data = {
			831,	/* Code */
			10415,	/* Vendor */
			"Calling-Party-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Carrier-Select-Routing-Information */
	{
		struct dict_avp_data data = {
			2023,	/* Code */
			10415,	/* Vendor */
			"Carrier-Select-Routing-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Cause-Code */
	{
		struct dict_avp_data data = {
			861,	/* Code */
			10415,	/* Vendor */
			"Cause-Code",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Change-Condition */
	{
		struct dict_avp_data data = {
			2037,	/* Code */
			10415,	/* Vendor */
			"Change-Condition",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Change-Time */
	{
		struct dict_avp_data data = {
			2038,	/* Code */
			10415,	/* Vendor */
			"Change-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Charge-Reason-Code */
	{
		struct dict_avp_data data = {
			2118,	/* Code */
			10415,	/* Vendor */
			"Charge-Reason-Code",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Charge-Reason-Code)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Charged-Party */
	{
		struct dict_avp_data data = {
			857,	/* Code */
			10415,	/* Vendor */
			"Charged-Party",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Charging-Characteristics-Selection-Mode */
	{
		struct dict_avp_data data = {
			2066,	/* Code */
			10415,	/* Vendor */
			"Charging-Characteristics-Selection-Mode",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Charging-Characteristics-Selection-Mode)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Class-Identifier */
	{
		struct dict_avp_data data = {
			1214,	/* Code */
			10415,	/* Vendor */
			"Class-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Class-Identifier)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Client-Address */
	{
		struct dict_avp_data data = {
			2018,	/* Code */
			10415,	/* Vendor */
			"Client-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Content-Class */
	{
		struct dict_avp_data data = {
			1220,	/* Code */
			10415,	/* Vendor */
			"Content-Class",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Content-Class)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Content-Disposition */
	{
		struct dict_avp_data data = {
			828,	/* Code */
			10415,	/* Vendor */
			"Content-Disposition",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Content-Length */
	{
		struct dict_avp_data data = {
			827,	/* Code */
			10415,	/* Vendor */
			"Content-Length",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Content-Size */
	{
		struct dict_avp_data data = {
			1206,	/* Code */
			10415,	/* Vendor */
			"Content-Size",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Content-Type */
	{
		struct dict_avp_data data = {
			826,	/* Code */
			10415,	/* Vendor */
			"Content-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Current-Tariff */
	{
		struct dict_avp_data data = {
			2056,	/* Code */
			10415,	/* Vendor */
			"Current-Tariff",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* DRM-Content */
	{
		struct dict_avp_data data = {
			1221,	/* Code */
			10415,	/* Vendor */
			"DRM-Content",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(DRM-Content)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Data-Coding-Scheme */
	{
		struct dict_avp_data data = {
			2001,	/* Code */
			10415,	/* Vendor */
			"Data-Coding-Scheme",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Deferred-Location-Event-Type */
	{
		struct dict_avp_data data = {
			1230,	/* Code */
			10415,	/* Vendor */
			"Deferred-Location-Event-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Delivery-Report-Requested */
	{
		struct dict_avp_data data = {
			1216,	/* Code */
			10415,	/* Vendor */
			"Delivery-Report-Requested",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Delivery-Report-Requested)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Destination-Interface */
	{
		struct dict_avp_data data = {
			2002,	/* Code */
			10415,	/* Vendor */
			"Destination-Interface",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Diagnostics */
	{
		struct dict_avp_data data = {
			2039,	/* Code */
			10415,	/* Vendor */
			"Diagnostics",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Domain-Name */
	{
		struct dict_avp_data data = {
			1200,	/* Code */
			10415,	/* Vendor */
			"Domain-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Dynamic-Address-Flag */
	{
		struct dict_avp_data data = {
			2051,	/* Code */
			10415,	/* Vendor */
			"Dynamic-Address-Flag",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Dynamic-Address-Flag)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Dynamic-Address-Flag-Extension */
	{
		struct dict_avp_data data = {
			2068,	/* Code */
			10415,	/* Vendor */
			"Dynamic-Address-Flag-Extension",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Dynamic-Address-Flag-Extension)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Early-Media-Description */
	{
		struct dict_avp_data data = {
			1272,	/* Code */
			10415,	/* Vendor */
			"Early-Media-Description",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Envelope */
	{
		struct dict_avp_data data = {
			1266,	/* Code */
			10415,	/* Vendor */
			"Envelope",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Envelope-End-Time */
	{
		struct dict_avp_data data = {
			1267,	/* Code */
			10415,	/* Vendor */
			"Envelope-End-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Envelope-Reporting */
	{
		struct dict_avp_data data = {
			1268,	/* Code */
			10415,	/* Vendor */
			"Envelope-Reporting",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Envelope-Reporting)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Envelope-Start-Time */
	{
		struct dict_avp_data data = {
			1269,	/* Code */
			10415,	/* Vendor */
			"Envelope-Start-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Event */
	{
		struct dict_avp_data data = {
			825,	/* Code */
			10415,	/* Vendor */
			"Event",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Event-Charging-TimeStamp */
	{
		struct dict_avp_data data = {
			1258,	/* Code */
			10415,	/* Vendor */
			"Event-Charging-TimeStamp",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Event-Type */
	{
		struct dict_avp_data data = {
			823,	/* Code */
			10415,	/* Vendor */
			"Event-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Expires */
	{
		struct dict_avp_data data = {
			888,	/* Code */
			10415,	/* Vendor */
			"Expires",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* File-Repair-Supported */
	{
		struct dict_avp_data data = {
			1224,	/* Code */
			10415,	/* Vendor */
			"File-Repair-Supported",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(File-Repair-Supported)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* From-Address */
	{
		struct dict_avp_data data = {
			2708,	/* Code */
			10415,	/* Vendor */
			"From-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* GGSN-Address */
	{
		struct dict_avp_data data = {
			847,	/* Code */
			10415,	/* Vendor */
			"GGSN-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* IMS-Application-Reference-Identifier */
	{
		struct dict_avp_data data = {
			2601,	/* Code */
			10415,	/* Vendor */
			"IMS-Application-Reference-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* IMS-Charging-Identifier */
	{
		struct dict_avp_data data = {
			841,	/* Code */
			10415,	/* Vendor */
			"IMS-Charging-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* IMS-Communication-Service-Identifier */
	{
		struct dict_avp_data data = {
			1281,	/* Code */
			10415,	/* Vendor */
			"IMS-Communication-Service-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* IMS-Emergency-Indicator */
	{
		struct dict_avp_data data = {
			2322,	/* Code */
			10415,	/* Vendor */
			"IMS-Emergency-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(IMS-Emergency-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* IMS-Information */
	{
		struct dict_avp_data data = {
			876,	/* Code */
			10415,	/* Vendor */
			"IMS-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* IMSI-Unauthenticated-Flag */
	{
		struct dict_avp_data data = {
			2308,	/* Code */
			10415,	/* Vendor */
			"IMSI-Unauthenticated-Flag",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(IMSI-Unauthenticated-Flag)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* IP-Realm-Default-Indication */
	{
		struct dict_avp_data data = {
			2603,	/* Code */
			10415,	/* Vendor */
			"IP-Realm-Default-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(IP-Realm-Default-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Incoming-Trunk-Group-Id */
	{
		struct dict_avp_data data = {
			852,	/* Code */
			10415,	/* Vendor */
			"Incoming-Trunk-Group-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Incremental-Cost */
	{
		struct dict_avp_data data = {
			2062,	/* Code */
			10415,	/* Vendor */
			"Incremental-Cost",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Initial-IMS-Charging-Identifier */
	{
		struct dict_avp_data data = {
			2321,	/* Code */
			10415,	/* Vendor */
			"Initial-IMS-Charging-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Inter-Operator-Identifier */
	{
		struct dict_avp_data data = {
			838,	/* Code */
			10415,	/* Vendor */
			"Inter-Operator-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Interface-Id */
	{
		struct dict_avp_data data = {
			2003,	/* Code */
			10415,	/* Vendor */
			"Interface-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Interface-Port */
	{
		struct dict_avp_data data = {
			2004,	/* Code */
			10415,	/* Vendor */
			"Interface-Port",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Interface-Text */
	{
		struct dict_avp_data data = {
			2005,	/* Code */
			10415,	/* Vendor */
			"Interface-Text",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Interface-Type */
	{
		struct dict_avp_data data = {
			2006,	/* Code */
			10415,	/* Vendor */
			"Interface-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Interface-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* LCS-APN */
	{
		struct dict_avp_data data = {
			1231,	/* Code */
			10415,	/* Vendor */
			"LCS-APN",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* LCS-Client-Dialed-By-MS */
	{
		struct dict_avp_data data = {
			1233,	/* Code */
			10415,	/* Vendor */
			"LCS-Client-Dialed-By-MS",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* LCS-Client-External-ID */
	{
		struct dict_avp_data data = {
			1234,	/* Code */
			10415,	/* Vendor */
			"LCS-Client-External-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* LCS-Client-Id */
	{
		struct dict_avp_data data = {
			1232,	/* Code */
			10415,	/* Vendor */
			"LCS-Client-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* LCS-Client-Name */
	{
		struct dict_avp_data data = {
			1235,	/* Code */
			10415,	/* Vendor */
			"LCS-Client-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* LCS-Client-Type */
	{
		struct dict_avp_data data = {
			1241,	/* Code */
			10415,	/* Vendor */
			"LCS-Client-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(LCS-Client-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* LCS-Data-Coding-Scheme */
	{
		struct dict_avp_data data = {
			1236,	/* Code */
			10415,	/* Vendor */
			"LCS-Data-Coding-Scheme",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* LCS-Format-Indicator */
	{
		struct dict_avp_data data = {
			1237,	/* Code */
			10415,	/* Vendor */
			"LCS-Format-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(LCS-Format-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* LCS-Information */
	{
		struct dict_avp_data data = {
			878,	/* Code */
			10415,	/* Vendor */
			"LCS-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* LCS-Name-String */
	{
		struct dict_avp_data data = {
			1238,	/* Code */
			10415,	/* Vendor */
			"LCS-Name-String",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* LCS-Requestor-Id */
	{
		struct dict_avp_data data = {
			1239,	/* Code */
			10415,	/* Vendor */
			"LCS-Requestor-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* LCS-Requestor-Id-String */
	{
		struct dict_avp_data data = {
			1240,	/* Code */
			10415,	/* Vendor */
			"LCS-Requestor-Id-String",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Local-GW-Inserted-Indication */
	{
		struct dict_avp_data data = {
			2604,	/* Code */
			10415,	/* Vendor */
			"Local-GW-Inserted-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Local-GW-Inserted-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Local-Sequence-Number */
	{
		struct dict_avp_data data = {
			2063,	/* Code */
			10415,	/* Vendor */
			"Local-Sequence-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Location-Estimate */
	{
		struct dict_avp_data data = {
			1242,	/* Code */
			10415,	/* Vendor */
			"Location-Estimate",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Location-Estimate-Type */
	{
		struct dict_avp_data data = {
			1243,	/* Code */
			10415,	/* Vendor */
			"Location-Estimate-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Location-Estimate-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Location-Type */
	{
		struct dict_avp_data data = {
			1244,	/* Code */
			10415,	/* Vendor */
			"Location-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Low-Balance-Indication */
	{
		struct dict_avp_data data = {
			2020,	/* Code */
			10415,	/* Vendor */
			"Low-Balance-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Low-Balance-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Low-Priority-Indicator */
	{
		struct dict_avp_data data = {
			2602,	/* Code */
			10415,	/* Vendor */
			"Low-Priority-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Low-Priority-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MBMSGW-Address */
	{
		struct dict_avp_data data = {
			2307,	/* Code */
			10415,	/* Vendor */
			"MBMSGW-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* MBMS-Information */
	{
		struct dict_avp_data data = {
			880,	/* Code */
			10415,	/* Vendor */
			"MBMS-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MBMS-User-Service-Type */
	{
		struct dict_avp_data data = {
			1225,	/* Code */
			10415,	/* Vendor */
			"MBMS-User-Service-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MBMS-User-Service-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MM-Content-Type */
	{
		struct dict_avp_data data = {
			1203,	/* Code */
			10415,	/* Vendor */
			"MM-Content-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MMBox-Storage-Requested */
	{
		struct dict_avp_data data = {
			1248,	/* Code */
			10415,	/* Vendor */
			"MMBox-Storage-Requested",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MMBox-Storage-Requested)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MMS-Information */
	{
		struct dict_avp_data data = {
			877,	/* Code */
			10415,	/* Vendor */
			"MMS-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MMTel-Information */
	{
		struct dict_avp_data data = {
			2030,	/* Code */
			10415,	/* Vendor */
			"MMTel-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MMTel-SService-Type */
	{
		struct dict_avp_data data = {
			2031,	/* Code */
			10415,	/* Vendor */
			"MMTel-SService-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Media-Initiator-Flag */
	{
		struct dict_avp_data data = {
			882,	/* Code */
			10415,	/* Vendor */
			"Media-Initiator-Flag",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Media-Initiator-Flag)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Media-Initiator-Party */
	{
		struct dict_avp_data data = {
			1288,	/* Code */
			10415,	/* Vendor */
			"Media-Initiator-Party",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Message-Body */
	{
		struct dict_avp_data data = {
			889,	/* Code */
			10415,	/* Vendor */
			"Message-Body",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Message-Class */
	{
		struct dict_avp_data data = {
			1213,	/* Code */
			10415,	/* Vendor */
			"Message-Class",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Message-ID */
	{
		struct dict_avp_data data = {
			1210,	/* Code */
			10415,	/* Vendor */
			"Message-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Message-Size */
	{
		struct dict_avp_data data = {
			1212,	/* Code */
			10415,	/* Vendor */
			"Message-Size",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Message-Type */
	{
		struct dict_avp_data data = {
			1211,	/* Code */
			10415,	/* Vendor */
			"Message-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Message-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* NNI-Information */
	{
		struct dict_avp_data data = {
			2703,	/* Code */
			10415,	/* Vendor */
			"NNI-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* NNI-Type */
	{
		struct dict_avp_data data = {
			2704,	/* Code */
			10415,	/* Vendor */
			"NNI-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(NNI-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Neighbour-Node-Address */
	{
		struct dict_avp_data data = {
			2705,	/* Code */
			10415,	/* Vendor */
			"Neighbour-Node-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Next-Tariff */
	{
		struct dict_avp_data data = {
			2057,	/* Code */
			10415,	/* Vendor */
			"Next-Tariff",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Node-Functionality */
	{
		struct dict_avp_data data = {
			862,	/* Code */
			10415,	/* Vendor */
			"Node-Functionality",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Node-Functionality)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Node-Id */
	{
		struct dict_avp_data data = {
			2064,	/* Code */
			10415,	/* Vendor */
			"Node-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Number-Of-Diversions */
	{
		struct dict_avp_data data = {
			2034,	/* Code */
			10415,	/* Vendor */
			"Number-Of-Diversions",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Number-Of-Messages-Sent */
	{
		struct dict_avp_data data = {
			2019,	/* Code */
			10415,	/* Vendor */
			"Number-Of-Messages-Sent",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Number-Of-Participants */
	{
		struct dict_avp_data data = {
			885,	/* Code */
			10415,	/* Vendor */
			"Number-Of-Participants",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Number-Of-Received-Talk-Bursts */
	{
		struct dict_avp_data data = {
			1282,	/* Code */
			10415,	/* Vendor */
			"Number-Of-Received-Talk-Bursts",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Number-Of-Talk-Bursts */
	{
		struct dict_avp_data data = {
			1283,	/* Code */
			10415,	/* Vendor */
			"Number-Of-Talk-Bursts",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Number-Portability-Routing-Information */
	{
		struct dict_avp_data data = {
			2024,	/* Code */
			10415,	/* Vendor */
			"Number-Portability-Routing-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Offline-Charging */
	{
		struct dict_avp_data data = {
			1278,	/* Code */
			10415,	/* Vendor */
			"Offline-Charging",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Online-Charging-Flag */
	{
		struct dict_avp_data data = {
			2303,	/* Code */
			10415,	/* Vendor */
			"Online-Charging-Flag",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Online-Charging-Flag)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Originating-IOI */
	{
		struct dict_avp_data data = {
			839,	/* Code */
			10415,	/* Vendor */
			"Originating-IOI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Originator */
	{
		struct dict_avp_data data = {
			864,	/* Code */
			10415,	/* Vendor */
			"Originator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Originator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Originator-Address */
	{
		struct dict_avp_data data = {
			886,	/* Code */
			10415,	/* Vendor */
			"Originator-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Originator-Interface */
	{
		struct dict_avp_data data = {
			2009,	/* Code */
			10415,	/* Vendor */
			"Originator-Interface",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Originator-Received-Address */
	{
		struct dict_avp_data data = {
			2027,	/* Code */
			10415,	/* Vendor */
			"Originator-Received-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Originator-SCCP-Address */
	{
		struct dict_avp_data data = {
			2008,	/* Code */
			10415,	/* Vendor */
			"Originator-SCCP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Outgoing-Session-Id */
	{
		struct dict_avp_data data = {
			2320,	/* Code */
			10415,	/* Vendor */
			"Outgoing-Session-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Outgoing-Trunk-Group-Id */
	{
		struct dict_avp_data data = {
			853,	/* Code */
			10415,	/* Vendor */
			"Outgoing-Trunk-Group-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* PDG-Address */
	{
		struct dict_avp_data data = {
			895,	/* Code */
			10415,	/* Vendor */
			"PDG-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* PDG-Charging-Id */
	{
		struct dict_avp_data data = {
			896,	/* Code */
			10415,	/* Vendor */
			"PDG-Charging-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PDN-Connection-Charging-ID */
	{
		struct dict_avp_data data = {
			2050,	/* Code */
			10415,	/* Vendor */
			"PDN-Connection-Charging-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PDP-Address */
	{
		struct dict_avp_data data = {
			1227,	/* Code */
			10415,	/* Vendor */
			"PDP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* PDP-Address-Prefix-Length */
	{
		struct dict_avp_data data = {
			2606,	/* Code */
			10415,	/* Vendor */
			"PDP-Address-Prefix-Length",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PDP-Context-Type */
	{
		struct dict_avp_data data = {
			1247,	/* Code */
			10415,	/* Vendor */
			"PDP-Context-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PDP-Context-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PS-Append-Free-Format-Data */
	{
		struct dict_avp_data data = {
			867,	/* Code */
			10415,	/* Vendor */
			"PS-Append-Free-Format-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PS-Append-Free-Format-Data)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PS-Free-Format-Data */
	{
		struct dict_avp_data data = {
			866,	/* Code */
			10415,	/* Vendor */
			"PS-Free-Format-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PS-Furnish-Charging-Information */
	{
		struct dict_avp_data data = {
			865,	/* Code */
			10415,	/* Vendor */
			"PS-Furnish-Charging-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PS-Information */
	{
		struct dict_avp_data data = {
			874,	/* Code */
			10415,	/* Vendor */
			"PS-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Participant-Access-Priority */
	{
		struct dict_avp_data data = {
			1259,	/* Code */
			10415,	/* Vendor */
			"Participant-Access-Priority",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Participant-Access-Priority)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Participant-Action-Type */
	{
		struct dict_avp_data data = {
			2049,	/* Code */
			10415,	/* Vendor */
			"Participant-Action-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Participant-Action-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Participant-Group */
	{
		struct dict_avp_data data = {
			1260,	/* Code */
			10415,	/* Vendor */
			"Participant-Group",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Participants-Involved */
	{
		struct dict_avp_data data = {
			887,	/* Code */
			10415,	/* Vendor */
			"Participants-Involved",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* PoC-Change-Condition */
	{
		struct dict_avp_data data = {
			1261,	/* Code */
			10415,	/* Vendor */
			"PoC-Change-Condition",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PoC-Change-Condition)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PoC-Change-Time */
	{
		struct dict_avp_data data = {
			1262,	/* Code */
			10415,	/* Vendor */
			"PoC-Change-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* PoC-Controlling-Address */
	{
		struct dict_avp_data data = {
			858,	/* Code */
			10415,	/* Vendor */
			"PoC-Controlling-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* PoC-Event-Type */
	{
		struct dict_avp_data data = {
			2025,	/* Code */
			10415,	/* Vendor */
			"PoC-Event-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PoC-Event-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PoC-Group-Name */
	{
		struct dict_avp_data data = {
			859,	/* Code */
			10415,	/* Vendor */
			"PoC-Group-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* PoC-Information */
	{
		struct dict_avp_data data = {
			879,	/* Code */
			10415,	/* Vendor */
			"PoC-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PoC-Server-Role */
	{
		struct dict_avp_data data = {
			883,	/* Code */
			10415,	/* Vendor */
			"PoC-Server-Role",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PoC-Server-Role)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PoC-Session-Id */
	{
		struct dict_avp_data data = {
			1229,	/* Code */
			10415,	/* Vendor */
			"PoC-Session-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* PoC-Session-Initiation-type */
	{
		struct dict_avp_data data = {
			1277,	/* Code */
			10415,	/* Vendor */
			"PoC-Session-Initiation-type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PoC-Session-Initiation-type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PoC-Session-Type */
	{
		struct dict_avp_data data = {
			884,	/* Code */
			10415,	/* Vendor */
			"PoC-Session-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PoC-Session-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PoC-User-Role */
	{
		struct dict_avp_data data = {
			1252,	/* Code */
			10415,	/* Vendor */
			"PoC-User-Role",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PoC-User-Role-IDs */
	{
		struct dict_avp_data data = {
			1253,	/* Code */
			10415,	/* Vendor */
			"PoC-User-Role-IDs",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* PoC-User-Role-info-Units */
	{
		struct dict_avp_data data = {
			1254,	/* Code */
			10415,	/* Vendor */
			"PoC-User-Role-info-Units",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PoC-User-Role-info-Units)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Positioning-Data */
	{
		struct dict_avp_data data = {
			1245,	/* Code */
			10415,	/* Vendor */
			"Positioning-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Preferred-AoC-Currency */
	{
		struct dict_avp_data data = {
			2315,	/* Code */
			10415,	/* Vendor */
			"Preferred-AoC-Currency",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Priority */
	{
		struct dict_avp_data data = {
			1209,	/* Code */
			10415,	/* Vendor */
			"Priority",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Priority)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Quota-Consumption-Time */
	{
		struct dict_avp_data data = {
			881,	/* Code */
			10415,	/* Vendor */
			"Quota-Consumption-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Quota-Holding-Time */
	{
		struct dict_avp_data data = {
			871,	/* Code */
			10415,	/* Vendor */
			"Quota-Holding-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Rate-Element */
	{
		struct dict_avp_data data = {
			2058,	/* Code */
			10415,	/* Vendor */
			"Rate-Element",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Read-Reply-Report-Requested */
	{
		struct dict_avp_data data = {
			1222,	/* Code */
			10415,	/* Vendor */
			"Read-Reply-Report-Requested",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Read-Reply-Report-Requested)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Real-Time-Tariff-Information */
	{
		struct dict_avp_data data = {
			2305,	/* Code */
			10415,	/* Vendor */
			"Real-Time-Tariff-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Received-Talk-Burst-Time */
	{
		struct dict_avp_data data = {
			1284,	/* Code */
			10415,	/* Vendor */
			"Received-Talk-Burst-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Received-Talk-Burst-Volume */
	{
		struct dict_avp_data data = {
			1285,	/* Code */
			10415,	/* Vendor */
			"Received-Talk-Burst-Volume",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Recipient-Address */
	{
		struct dict_avp_data data = {
			1201,	/* Code */
			10415,	/* Vendor */
			"Recipient-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Recipient-Info */
	{
		struct dict_avp_data data = {
			2026,	/* Code */
			10415,	/* Vendor */
			"Recipient-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Recipient-Received-Address */
	{
		struct dict_avp_data data = {
			2028,	/* Code */
			10415,	/* Vendor */
			"Recipient-Received-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Recipient-SCCP-Address */
	{
		struct dict_avp_data data = {
			2010,	/* Code */
			10415,	/* Vendor */
			"Recipient-SCCP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Refund-Information */
	{
		struct dict_avp_data data = {
			2022,	/* Code */
			10415,	/* Vendor */
			"Refund-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Relationship-Mode */
	{
		struct dict_avp_data data = {
			2706,	/* Code */
			10415,	/* Vendor */
			"Relationship-Mode",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Relationship-Mode)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Remaining-Balance */
	{
		struct dict_avp_data data = {
			2021,	/* Code */
			10415,	/* Vendor */
			"Remaining-Balance",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Reply-Applic-ID */
	{
		struct dict_avp_data data = {
			1223,	/* Code */
			10415,	/* Vendor */
			"Reply-Applic-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Reply-Path-Requested */
	{
		struct dict_avp_data data = {
			2011,	/* Code */
			10415,	/* Vendor */
			"Reply-Path-Requested",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Reply-Path-Requested)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Reporting-Reason */
	{
		struct dict_avp_data data = {
			872,	/* Code */
			10415,	/* Vendor */
			"Reporting-Reason",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Reporting-Reason)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Requested-Party-Address */
	{
		struct dict_avp_data data = {
			1251,	/* Code */
			10415,	/* Vendor */
			"Requested-Party-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Role-Of-Node */
	{
		struct dict_avp_data data = {
			829,	/* Code */
			10415,	/* Vendor */
			"Role-Of-Node",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Role-Of-Node)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* SDP-Answer-Timestamp */
	{
		struct dict_avp_data data = {
			1275,	/* Code */
			10415,	/* Vendor */
			"SDP-Answer-Timestamp",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* SDP-Media-Component */
	{
		struct dict_avp_data data = {
			843,	/* Code */
			10415,	/* Vendor */
			"SDP-Media-Component",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SDP-Media-Description */
	{
		struct dict_avp_data data = {
			845,	/* Code */
			10415,	/* Vendor */
			"SDP-Media-Description",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* SDP-Media-Name */
	{
		struct dict_avp_data data = {
			844,	/* Code */
			10415,	/* Vendor */
			"SDP-Media-Name",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* SDP-Offer-Timestamp */
	{
		struct dict_avp_data data = {
			1274,	/* Code */
			10415,	/* Vendor */
			"SDP-Offer-Timestamp",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* SDP-Session-Description */
	{
		struct dict_avp_data data = {
			842,	/* Code */
			10415,	/* Vendor */
			"SDP-Session-Description",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* SDP-TimeStamps */
	{
		struct dict_avp_data data = {
			1273,	/* Code */
			10415,	/* Vendor */
			"SDP-TimeStamps",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SDP-Type */
	{
		struct dict_avp_data data = {
			2036,	/* Code */
			10415,	/* Vendor */
			"SDP-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(SDP-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* SGSN-Address */
	{
		struct dict_avp_data data = {
			1228,	/* Code */
			10415,	/* Vendor */
			"SGSN-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* SGW-Address */
	{
		struct dict_avp_data data = {
			2067,	/* Code */
			10415,	/* Vendor */
			"SGW-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* SGW-Change */
	{
		struct dict_avp_data data = {
			2065,	/* Code */
			10415,	/* Vendor */
			"SGW-Change",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(SGW-Change)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* SIP-Method */
	{
		struct dict_avp_data data = {
			824,	/* Code */
			10415,	/* Vendor */
			"SIP-Method",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* SIP-Request-Timestamp */
	{
		struct dict_avp_data data = {
			834,	/* Code */
			10415,	/* Vendor */
			"SIP-Request-Timestamp",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* SIP-Request-Timestamp-Fraction */
	{
		struct dict_avp_data data = {
			2301,	/* Code */
			10415,	/* Vendor */
			"SIP-Request-Timestamp-Fraction",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIP-Response-Timestamp */
	{
		struct dict_avp_data data = {
			835,	/* Code */
			10415,	/* Vendor */
			"SIP-Response-Timestamp",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* SIP-Response-Timestamp-Fraction */
	{
		struct dict_avp_data data = {
			2302,	/* Code */
			10415,	/* Vendor */
			"SIP-Response-Timestamp-Fraction",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SM-Discharge-Time */
	{
		struct dict_avp_data data = {
			2012,	/* Code */
			10415,	/* Vendor */
			"SM-Discharge-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* SM-Message-Type */
	{
		struct dict_avp_data data = {
			2007,	/* Code */
			10415,	/* Vendor */
			"SM-Message-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(SM-Message-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* SM-Protocol-ID */
	{
		struct dict_avp_data data = {
			2013,	/* Code */
			10415,	/* Vendor */
			"SM-Protocol-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SM-Service-Type */
	{
		struct dict_avp_data data = {
			2029,	/* Code */
			10415,	/* Vendor */
			"SM-Service-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(SM-Service-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* SM-Status */
	{
		struct dict_avp_data data = {
			2014,	/* Code */
			10415,	/* Vendor */
			"SM-Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SM-User-Data-Header */
	{
		struct dict_avp_data data = {
			2015,	/* Code */
			10415,	/* Vendor */
			"SM-User-Data-Header",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SMS-Information */
	{
		struct dict_avp_data data = {
			2000,	/* Code */
			10415,	/* Vendor */
			"SMS-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SMS-Node */
	{
		struct dict_avp_data data = {
			2016,	/* Code */
			10415,	/* Vendor */
			"SMS-Node",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(SMS-Node)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* SMSC-Address */
	{
		struct dict_avp_data data = {
			2017,	/* Code */
			10415,	/* Vendor */
			"SMSC-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Scale-Factor */
	{
		struct dict_avp_data data = {
			2059,	/* Code */
			10415,	/* Vendor */
			"Scale-Factor",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Served-Party-IP-Address */
	{
		struct dict_avp_data data = {
			848,	/* Code */
			10415,	/* Vendor */
			"Served-Party-IP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Service-Data-Container */
	{
		struct dict_avp_data data = {
			2040,	/* Code */
			10415,	/* Vendor */
			"Service-Data-Container",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Service-Id */
	{
		struct dict_avp_data data = {
			855,	/* Code */
			10415,	/* Vendor */
			"Service-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Service-Information */
	{
		struct dict_avp_data data = {
			873,	/* Code */
			10415,	/* Vendor */
			"Service-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Service-Mode */
	{
		struct dict_avp_data data = {
			2032,	/* Code */
			10415,	/* Vendor */
			"Service-Mode",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Service-Specific-Data */
	{
		struct dict_avp_data data = {
			863,	/* Code */
			10415,	/* Vendor */
			"Service-Specific-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Service-Specific-Info */
	{
		struct dict_avp_data data = {
			1249,	/* Code */
			10415,	/* Vendor */
			"Service-Specific-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Service-Specific-Type */
	{
		struct dict_avp_data data = {
			1257,	/* Code */
			10415,	/* Vendor */
			"Service-Specific-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Serving-Node-Type */
	{
		struct dict_avp_data data = {
			2047,	/* Code */
			10415,	/* Vendor */
			"Serving-Node-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Serving-Node-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Session-Direction */
	{
		struct dict_avp_data data = {
			2707,	/* Code */
			10415,	/* Vendor */
			"Session-Direction",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Session-Direction)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Start-Time */
	{
		struct dict_avp_data data = {
			2041,	/* Code */
			10415,	/* Vendor */
			"Start-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Status */
	{
		struct dict_avp_data data = {
			2702,	/* Code */
			10415,	/* Vendor */
			"Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Status)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Stop-Time */
	{
		struct dict_avp_data data = {
			2042,	/* Code */
			10415,	/* Vendor */
			"Stop-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Submission-Time */
	{
		struct dict_avp_data data = {
			1202,	/* Code */
			10415,	/* Vendor */
			"Submission-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Subscriber-Role */
	{
		struct dict_avp_data data = {
			2033,	/* Code */
			10415,	/* Vendor */
			"Subscriber-Role",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Subscriber-Role)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Supplementary-Service */
	{
		struct dict_avp_data data = {
			2048,	/* Code */
			10415,	/* Vendor */
			"Supplementary-Service",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Talk-Burst-Exchange */
	{
		struct dict_avp_data data = {
			1255,	/* Code */
			10415,	/* Vendor */
			"Talk-Burst-Exchange",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Talk-Burst-Time */
	{
		struct dict_avp_data data = {
			1286,	/* Code */
			10415,	/* Vendor */
			"Talk-Burst-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Talk-Burst-Volume */
	{
		struct dict_avp_data data = {
			1287,	/* Code */
			10415,	/* Vendor */
			"Talk-Burst-Volume",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Tariff-Information */
	{
		struct dict_avp_data data = {
			2060,	/* Code */
			10415,	/* Vendor */
			"Tariff-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Tariff-XML */
	{
		struct dict_avp_data data = {
			2306,	/* Code */
			10415,	/* Vendor */
			"Tariff-XML",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Terminating-IOI */
	{
		struct dict_avp_data data = {
			840,	/* Code */
			10415,	/* Vendor */
			"Terminating-IOI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Time-First-Usage */
	{
		struct dict_avp_data data = {
			2043,	/* Code */
			10415,	/* Vendor */
			"Time-First-Usage",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Time-Last-Usage */
	{
		struct dict_avp_data data = {
			2044,	/* Code */
			10415,	/* Vendor */
			"Time-Last-Usage",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Time-Quota-Mechanism */
	{
		struct dict_avp_data data = {
			1270,	/* Code */
			10415,	/* Vendor */
			"Time-Quota-Mechanism",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Time-Quota-Threshold */
	{
		struct dict_avp_data data = {
			868,	/* Code */
			10415,	/* Vendor */
			"Time-Quota-Threshold",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Time-Quota-Type */
	{
		struct dict_avp_data data = {
			1271,	/* Code */
			10415,	/* Vendor */
			"Time-Quota-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Time-Quota-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Time-Stamps */
	{
		struct dict_avp_data data = {
			833,	/* Code */
			10415,	/* Vendor */
			"Time-Stamps",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Time-Usage */
	{
		struct dict_avp_data data = {
			2045,	/* Code */
			10415,	/* Vendor */
			"Time-Usage",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Token-Text */
	{
		struct dict_avp_data data = {
			1215,	/* Code */
			10415,	/* Vendor */
			"Token-Text",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Traffic-Data-Volumes */
	{
		struct dict_avp_data data = {
			2046,	/* Code */
			10415,	/* Vendor */
			"Traffic-Data-Volumes",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Transcoder-Inserted-Indication */
	{
		struct dict_avp_data data = {
			2605,	/* Code */
			10415,	/* Vendor */
			"Transcoder-Inserted-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Transcoder-Inserted-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Transit-IOI-List */
	{
		struct dict_avp_data data = {
			2701,	/* Code */
			10415,	/* Vendor */
			"Transit-IOI-List",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Trigger */
	{
		struct dict_avp_data data = {
			1264,	/* Code */
			10415,	/* Vendor */
			"Trigger",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Trigger-Type */
	{
		struct dict_avp_data data = {
			870,	/* Code */
			10415,	/* Vendor */
			"Trigger-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Trigger-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Trunk-Group-Id */
	{
		struct dict_avp_data data = {
			851,	/* Code */
			10415,	/* Vendor */
			"Trunk-Group-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Type-Number */
	{
		struct dict_avp_data data = {
			1204,	/* Code */
			10415,	/* Vendor */
			"Type-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Type-Number)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Unit-Cost */
	{
		struct dict_avp_data data = {
			2061,	/* Code */
			10415,	/* Vendor */
			"Unit-Cost",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Unit-Quota-Threshold */
	{
		struct dict_avp_data data = {
			1226,	/* Code */
			10415,	/* Vendor */
			"Unit-Quota-Threshold",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* User-CSG-Information */
	{
		struct dict_avp_data data = {
			2319,	/* Code */
			10415,	/* Vendor */
			"User-CSG-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* User-Participating-Type */
	{
		struct dict_avp_data data = {
			1279,	/* Code */
			10415,	/* Vendor */
			"User-Participating-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(User-Participating-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* User-Session-Id */
	{
		struct dict_avp_data data = {
			830,	/* Code */
			10415,	/* Vendor */
			"User-Session-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Volume-Quota-Threshold */
	{
		struct dict_avp_data data = {
			869,	/* Code */
			10415,	/* Vendor */
			"Volume-Quota-Threshold",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* WAG-Address */
	{
		struct dict_avp_data data = {
			890,	/* Code */
			10415,	/* Vendor */
			"WAG-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* WAG-PLMN-Id */
	{
		struct dict_avp_data data = {
			891,	/* Code */
			10415,	/* Vendor */
			"WAG-PLMN-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* WLAN-Information */
	{
		struct dict_avp_data data = {
			875,	/* Code */
			10415,	/* Vendor */
			"WLAN-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* WLAN-Radio-Container */
	{
		struct dict_avp_data data = {
			892,	/* Code */
			10415,	/* Vendor */
			"WLAN-Radio-Container",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* WLAN-Session-Id */
	{
		struct dict_avp_data data = {
			1246,	/* Code */
			10415,	/* Vendor */
			"WLAN-Session-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* WLAN-Technology */
	{
		struct dict_avp_data data = {
			893,	/* Code */
			10415,	/* Vendor */
			"WLAN-Technology",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* WLAN-UE-Local-IPAddress */
	{
		struct dict_avp_data data = {
			894,	/* Code */
			10415,	/* Vendor */
			"WLAN-UE-Local-IPAddress",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* 3GPP 29.272-c00 (12.0.0 2013.03.13)                          */
	/* Subscription-Data */
	{
		struct dict_avp_data data = {
			1400,	/* Code */
			10415,	/* Vendor */
			"Subscription-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Terminal-Information */
	{
		struct dict_avp_data data = {
			1401,	/* Code */
			10415,	/* Vendor */
			"Terminal-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* IMEI */
	{
		struct dict_avp_data data = {
			1402,	/* Code */
			10415,	/* Vendor */
			"IMEI",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Software-Version */
	{
		struct dict_avp_data data = {
			1403,	/* Code */
			10415,	/* Vendor */
			"Software-Version",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* QoS-Subscribed */
	{
		struct dict_avp_data data = {
			1404,	/* Code */
			10415,	/* Vendor */
			"QoS-Subscribed",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ULR-Flags */
	{
		struct dict_avp_data data = {
			1405,	/* Code */
			10415,	/* Vendor */
			"ULR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ULA-Flags */
	{
		struct dict_avp_data data = {
			1406,	/* Code */
			10415,	/* Vendor */
			"ULA-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Visited-PLMN-Id */
	{
		struct dict_avp_data data = {
			1407,	/* Code */
			10415,	/* Vendor */
			"Visited-PLMN-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Requested-EUTRAN-Authentication-Info */
	{
		struct dict_avp_data data = {
			1408,	/* Code */
			10415,	/* Vendor */
			"Requested-EUTRAN-Authentication-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Requested-UTRAN-GERAN-Authentication-Info */
	{
		struct dict_avp_data data = {
			1409,	/* Code */
			10415,	/* Vendor */
			"Requested-UTRAN-GERAN-Authentication-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Number-Of-Requested-Vectors */
	{
		struct dict_avp_data data = {
			1410,	/* Code */
			10415,	/* Vendor */
			"Number-Of-Requested-Vectors",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Re-Synchronization-Info */
	{
		struct dict_avp_data data = {
			1411,	/* Code */
			10415,	/* Vendor */
			"Re-Synchronization-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Immediate-Response-Preferred */
	{
		struct dict_avp_data data = {
			1412,	/* Code */
			10415,	/* Vendor */
			"Immediate-Response-Preferred",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Authentication-Info */
	{
		struct dict_avp_data data = {
			1413,	/* Code */
			10415,	/* Vendor */
			"Authentication-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* E-UTRAN-Vector */
	{
		struct dict_avp_data data = {
			1414,	/* Code */
			10415,	/* Vendor */
			"E-UTRAN-Vector",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* UTRAN-Vector */
	{
		struct dict_avp_data data = {
			1415,	/* Code */
			10415,	/* Vendor */
			"UTRAN-Vector",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* GERAN-Vector */
	{
		struct dict_avp_data data = {
			1416,	/* Code */
			10415,	/* Vendor */
			"GERAN-Vector",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Network-Access-Mode */
	{
		struct dict_avp_data data = {
			1417,	/* Code */
			10415,	/* Vendor */
			"Network-Access-Mode",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Network-Access-Mode)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* HPLMN-ODB */
	{
		struct dict_avp_data data = {
			1418,	/* Code */
			10415,	/* Vendor */
			"HPLMN-ODB",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Item-Number */
	{
		struct dict_avp_data data = {
			1419,	/* Code */
			10415,	/* Vendor */
			"Item-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Cancellation-Type */
	{
		struct dict_avp_data data = {
			1420,	/* Code */
			10415,	/* Vendor */
			"Cancellation-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Cancellation-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* DSR-Flags */
	{
		struct dict_avp_data data = {
			1421,	/* Code */
			10415,	/* Vendor */
			"DSR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* DSA-Flags */
	{
		struct dict_avp_data data = {
			1422,	/* Code */
			10415,	/* Vendor */
			"DSA-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Context-Identifier */
	{
		struct dict_avp_data data = {
			1423,	/* Code */
			10415,	/* Vendor */
			"Context-Identifier",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Subscriber-Status */
	{
		struct dict_avp_data data = {
			1424,	/* Code */
			10415,	/* Vendor */
			"Subscriber-Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Subscriber-Status)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Operator-Determined-Barring */
	{
		struct dict_avp_data data = {
			1425,	/* Code */
			10415,	/* Vendor */
			"Operator-Determined-Barring",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Access-Restriction-Data */
	{
		struct dict_avp_data data = {
			1426,	/* Code */
			10415,	/* Vendor */
			"Access-Restriction-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* APN-OI-Replacement */
	{
		struct dict_avp_data data = {
			1427,	/* Code */
			10415,	/* Vendor */
			"APN-OI-Replacement",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* All-APN-Configurations-Included-Indicator */
	{
		struct dict_avp_data data = {
			1428,	/* Code */
			10415,	/* Vendor */
			"All-APN-Configurations-Included-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(All-APN-Configurations-Included-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* APN-Configuration-Profile */
	{
		struct dict_avp_data data = {
			1429,	/* Code */
			10415,	/* Vendor */
			"APN-Configuration-Profile",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* APN-Configuration */
	{
		struct dict_avp_data data = {
			1430,	/* Code */
			10415,	/* Vendor */
			"APN-Configuration",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* EPS-Subscribed-QoS-Profile */
	{
		struct dict_avp_data data = {
			1431,	/* Code */
			10415,	/* Vendor */
			"EPS-Subscribed-QoS-Profile",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* VPLMN-Dynamic-Address-Allowed */
	{
		struct dict_avp_data data = {
			1432,	/* Code */
			10415,	/* Vendor */
			"VPLMN-Dynamic-Address-Allowed",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(VPLMN-Dynamic-Address-Allowed)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* STN-SR */
	{
		struct dict_avp_data data = {
			1433,	/* Code */
			10415,	/* Vendor */
			"STN-SR",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Alert-Reason */
	{
		struct dict_avp_data data = {
			1434,	/* Code */
			10415,	/* Vendor */
			"Alert-Reason",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Alert-Reason)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* AMBR */
	{
		struct dict_avp_data data = {
			1435,	/* Code */
			10415,	/* Vendor */
			"AMBR",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* CSG-Subscription-Data */
	{
		struct dict_avp_data data = {
			1436,	/* Code */
			10415,	/* Vendor */
			"CSG-Subscription-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* CSG-Id */
	{
		struct dict_avp_data data = {
			1437,	/* Code */
			10415,	/* Vendor */
			"CSG-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PDN-GW-Allocation-Type */
	{
		struct dict_avp_data data = {
			1438,	/* Code */
			10415,	/* Vendor */
			"PDN-GW-Allocation-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PDN-GW-Allocation-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Expiration-Date */
	{
		struct dict_avp_data data = {
			1439,	/* Code */
			10415,	/* Vendor */
			"Expiration-Date",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* RAT-Frequency-Selection-Priority-ID */
	{
		struct dict_avp_data data = {
			1440,	/* Code */
			10415,	/* Vendor */
			"RAT-Frequency-Selection-Priority-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* IDA-Flags */
	{
		struct dict_avp_data data = {
			1441,	/* Code */
			10415,	/* Vendor */
			"IDA-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PUA-Flags */
	{
		struct dict_avp_data data = {
			1442,	/* Code */
			10415,	/* Vendor */
			"PUA-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* NOR-Flags */
	{
		struct dict_avp_data data = {
			1443,	/* Code */
			10415,	/* Vendor */
			"NOR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* User-Id */
	{
		struct dict_avp_data data = {
			1444,	/* Code */
			10415,	/* Vendor */
			"User-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Equipment-Status */
	{
		struct dict_avp_data data = {
			1445,	/* Code */
			10415,	/* Vendor */
			"Equipment-Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Equipment-Status)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Regional-Subscription-Zone-Code */
	{
		struct dict_avp_data data = {
			1446,	/* Code */
			10415,	/* Vendor */
			"Regional-Subscription-Zone-Code",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* RAND */
	{
		struct dict_avp_data data = {
			1447,	/* Code */
			10415,	/* Vendor */
			"RAND",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* XRES */
	{
		struct dict_avp_data data = {
			1448,	/* Code */
			10415,	/* Vendor */
			"XRES",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* AUTN */
	{
		struct dict_avp_data data = {
			1449,	/* Code */
			10415,	/* Vendor */
			"AUTN",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* KASME */
	{
		struct dict_avp_data data = {
			1450,	/* Code */
			10415,	/* Vendor */
			"KASME",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Trace-Collection-Entity */
	{
		struct dict_avp_data data = {
			1452,	/* Code */
			10415,	/* Vendor */
			"Trace-Collection-Entity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* Kc */
	{
		struct dict_avp_data data = {
			1453,	/* Code */
			10415,	/* Vendor */
			"Kc",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SRES */
	{
		struct dict_avp_data data = {
			1454,	/* Code */
			10415,	/* Vendor */
			"SRES",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PDN-Type */
	{
		struct dict_avp_data data = {
			1456,	/* Code */
			10415,	/* Vendor */
			"PDN-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PDN-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Roaming-Restricted-Due-To-Unsupported-Feature */
	{
		struct dict_avp_data data = {
			1457,	/* Code */
			10415,	/* Vendor */
			"Roaming-Restricted-Due-To-Unsupported-Feature",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Roaming-Restricted-Due-To-Unsupported-Feature)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Trace-Data */
	{
		struct dict_avp_data data = {
			1458,	/* Code */
			10415,	/* Vendor */
			"Trace-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Trace-Reference */
	{
		struct dict_avp_data data = {
			1459,	/* Code */
			10415,	/* Vendor */
			"Trace-Reference",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Trace-Depth */
	{
		struct dict_avp_data data = {
			1462,	/* Code */
			10415,	/* Vendor */
			"Trace-Depth",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Trace-Depth)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Trace-NE-Type-List */
	{
		struct dict_avp_data data = {
			1463,	/* Code */
			10415,	/* Vendor */
			"Trace-NE-Type-List",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Trace-Interface-List */
	{
		struct dict_avp_data data = {
			1464,	/* Code */
			10415,	/* Vendor */
			"Trace-Interface-List",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Trace-Event-List */
	{
		struct dict_avp_data data = {
			1465,	/* Code */
			10415,	/* Vendor */
			"Trace-Event-List",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* OMC-Id */
	{
		struct dict_avp_data data = {
			1466,	/* Code */
			10415,	/* Vendor */
			"OMC-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* GPRS-Subscription-Data */
	{
		struct dict_avp_data data = {
			1467,	/* Code */
			10415,	/* Vendor */
			"GPRS-Subscription-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Complete-Data-List-Included-Indicator */
	{
		struct dict_avp_data data = {
			1468,	/* Code */
			10415,	/* Vendor */
			"Complete-Data-List-Included-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Complete-Data-List-Included-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PDP-Context */
	{
		struct dict_avp_data data = {
			1469,	/* Code */
			10415,	/* Vendor */
			"PDP-Context",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* PDP-Type */
	{
		struct dict_avp_data data = {
			1470,	/* Code */
			10415,	/* Vendor */
			"PDP-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 3GPP2-MEID */
	{
		struct dict_avp_data data = {
			1471,	/* Code */
			10415,	/* Vendor */
			"3GPP2-MEID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Specific-APN-Info */
	{
		struct dict_avp_data data = {
			1472,	/* Code */
			10415,	/* Vendor */
			"Specific-APN-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* LCS-Info */
	{
		struct dict_avp_data data = {
			1473,	/* Code */
			10415,	/* Vendor */
			"LCS-Info",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* GMLC-Number */
	{
		struct dict_avp_data data = {
			1474,	/* Code */
			10415,	/* Vendor */
			"GMLC-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* LCS-PrivacyException */
	{
		struct dict_avp_data data = {
			1475,	/* Code */
			10415,	/* Vendor */
			"LCS-PrivacyException",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SS-Code */
	{
		struct dict_avp_data data = {
			1476,	/* Code */
			10415,	/* Vendor */
			"SS-Code",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SS-Status */
	{
		struct dict_avp_data data = {
			1477,	/* Code */
			10415,	/* Vendor */
			"SS-Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Notification-To-UE-User */
	{
		struct dict_avp_data data = {
			1478,	/* Code */
			10415,	/* Vendor */
			"Notification-To-UE-User",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Notification-To-UE-User)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* External-Client */
	{
		struct dict_avp_data data = {
			1479,	/* Code */
			10415,	/* Vendor */
			"External-Client",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Client-Identity */
	{
		struct dict_avp_data data = {
			1480,	/* Code */
			10415,	/* Vendor */
			"Client-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* GMLC-Restriction */
	{
		struct dict_avp_data data = {
			1481,	/* Code */
			10415,	/* Vendor */
			"GMLC-Restriction",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(GMLC-Restriction)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PLMN-Client */
	{
		struct dict_avp_data data = {
			1482,	/* Code */
			10415,	/* Vendor */
			"PLMN-Client",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(PLMN-Client)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Service-Type */
	{
		struct dict_avp_data data = {
			1483,	/* Code */
			10415,	/* Vendor */
			"Service-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ServiceTypeIdentity */
	{
		struct dict_avp_data data = {
			1484,	/* Code */
			10415,	/* Vendor */
			"ServiceTypeIdentity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MO-LR */
	{
		struct dict_avp_data data = {
			1485,	/* Code */
			10415,	/* Vendor */
			"MO-LR",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Teleservice-List */
	{
		struct dict_avp_data data = {
			1486,	/* Code */
			10415,	/* Vendor */
			"Teleservice-List",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* TS-Code */
	{
		struct dict_avp_data data = {
			1487,	/* Code */
			10415,	/* Vendor */
			"TS-Code",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Call-Barring-Infor-List */
	{
		struct dict_avp_data data = {
			1488,	/* Code */
			10415,	/* Vendor */
			"Call-Barring-Infor-List",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SGSN-Number */
	{
		struct dict_avp_data data = {
			1489,	/* Code */
			10415,	/* Vendor */
			"SGSN-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* IDR-Flags */
	{
		struct dict_avp_data data = {
			1490,	/* Code */
			10415,	/* Vendor */
			"IDR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* ICS-Indicator */
	{
		struct dict_avp_data data = {
			1491,	/* Code */
			10415,	/* Vendor */
			"ICS-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(ICS-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* IMS-Voice-Over-PS-Sessions-Supported */
	{
		struct dict_avp_data data = {
			1492,	/* Code */
			10415,	/* Vendor */
			"IMS-Voice-Over-PS-Sessions-Supported",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(IMS-Voice-Over-PS-Sessions-Supported)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Homogeneous-Support-of-IMS-Voice-Over-PS-Sessions */
	{
		struct dict_avp_data data = {
			1493,	/* Code */
			10415,	/* Vendor */
			"Homogeneous-Support-of-IMS-Voice-Over-PS-Sessions",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Homogeneous-Support-of-IMS-Voice-Over-PS-Sessions)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Last-UE-Activity-Time */
	{
		struct dict_avp_data data = {
			1494,	/* Code */
			10415,	/* Vendor */
			"Last-UE-Activity-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* EPS-User-State */
	{
		struct dict_avp_data data = {
			1495,	/* Code */
			10415,	/* Vendor */
			"EPS-User-State",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* EPS-Location-Information */
	{
		struct dict_avp_data data = {
			1496,	/* Code */
			10415,	/* Vendor */
			"EPS-Location-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MME-User-State */
	{
		struct dict_avp_data data = {
			1497,	/* Code */
			10415,	/* Vendor */
			"MME-User-State",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SGSN-User-State */
	{
		struct dict_avp_data data = {
			1498,	/* Code */
			10415,	/* Vendor */
			"SGSN-User-State",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* User-State */
	{
		struct dict_avp_data data = {
			1499,	/* Code */
			10415,	/* Vendor */
			"User-State",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(User-State)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MME-LocationInformation */
	{
		struct dict_avp_data data = {
			1600,	/* Code */
			10415,	/* Vendor */
			"MME-LocationInformation",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SGSN-Location-Information */
	{
		struct dict_avp_data data = {
			1601,	/* Code */
			10415,	/* Vendor */
			"SGSN-Location-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* E-UTRAN-Cell-Global-Identity */
	{
		struct dict_avp_data data = {
			1602,	/* Code */
			10415,	/* Vendor */
			"E-UTRAN-Cell-Global-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Tracking-Area-Identity */
	{
		struct dict_avp_data data = {
			1603,	/* Code */
			10415,	/* Vendor */
			"Tracking-Area-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Cell-Global-Identity */
	{
		struct dict_avp_data data = {
			1604,	/* Code */
			10415,	/* Vendor */
			"Cell-Global-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Routing-Area-Identity */
	{
		struct dict_avp_data data = {
			1605,	/* Code */
			10415,	/* Vendor */
			"Routing-Area-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Location-Area-Identity */
	{
		struct dict_avp_data data = {
			1606,	/* Code */
			10415,	/* Vendor */
			"Location-Area-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Service-Area-Identity */
	{
		struct dict_avp_data data = {
			1607,	/* Code */
			10415,	/* Vendor */
			"Service-Area-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Geographical-Information */
	{
		struct dict_avp_data data = {
			1608,	/* Code */
			10415,	/* Vendor */
			"Geographical-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Geodetic-Information */
	{
		struct dict_avp_data data = {
			1609,	/* Code */
			10415,	/* Vendor */
			"Geodetic-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Current-Location-Retrieved */
	{
		struct dict_avp_data data = {
			1610,	/* Code */
			10415,	/* Vendor */
			"Current-Location-Retrieved",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Current-Location-Retrieved)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Age-Of-Location-Information */
	{
		struct dict_avp_data data = {
			1611,	/* Code */
			10415,	/* Vendor */
			"Age-Of-Location-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Active-APN */
	{
		struct dict_avp_data data = {
			1612,	/* Code */
			10415,	/* Vendor */
			"Active-APN",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Error-Diagnostic */
	{
		struct dict_avp_data data = {
			1614,	/* Code */
			10415,	/* Vendor */
			"Error-Diagnostic",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Error-Diagnostic)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Ext-PDP-Address */
	{
		struct dict_avp_data data = {
			1621,	/* Code */
			10415,	/* Vendor */
			"Ext-PDP-Address",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
	};

	/* UE-SRVCC-Capability */
	{
		struct dict_avp_data data = {
			1615,	/* Code */
			10415,	/* Vendor */
			"UE-SRVCC-Capability",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(UE-SRVCC-Capability)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MPS-Priority */
	{
		struct dict_avp_data data = {
			1616,	/* Code */
			10415,	/* Vendor */
			"MPS-Priority",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* VPLMN-LIPA-Allowed */
	{
		struct dict_avp_data data = {
			1617,	/* Code */
			10415,	/* Vendor */
			"VPLMN-LIPA-Allowed",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(VPLMN-LIPA-Allowed)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* LIPA-Permission */
	{
		struct dict_avp_data data = {
			1618,	/* Code */
			10415,	/* Vendor */
			"LIPA-Permission",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(LIPA-Permission)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Subscribed-Periodic-RAU-TAU-Timer */
	{
		struct dict_avp_data data = {
			1619,	/* Code */
			10415,	/* Vendor */
			"Subscribed-Periodic-RAU-TAU-Timer",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Ext-PDP-Type */
	{
		struct dict_avp_data data = {
			1620,	/* Code */
			10415,	/* Vendor */
			"Ext-PDP-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SIPTO-Permission */
	{
		struct dict_avp_data data = {
			1613,	/* Code */
			10415,	/* Vendor */
			"SIPTO-Permission",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(SIPTO-Permission)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MDT-Configuration */
	{
		struct dict_avp_data data = {
			1622,	/* Code */
			10415,	/* Vendor */
			"MDT-Configuration",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Job-Type */
	{
		struct dict_avp_data data = {
			1623,	/* Code */
			10415,	/* Vendor */
			"Job-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Job-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Area-Scope */
	{
		struct dict_avp_data data = {
			1624,	/* Code */
			10415,	/* Vendor */
			"Area-Scope",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* List-Of-Measurements */
	{
		struct dict_avp_data data = {
			1625,	/* Code */
			10415,	/* Vendor */
			"List-Of-Measurements",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Reporting-Trigger */
	{
		struct dict_avp_data data = {
			1626,	/* Code */
			10415,	/* Vendor */
			"Reporting-Trigger",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Report-Interval */
	{
		struct dict_avp_data data = {
			1627,	/* Code */
			10415,	/* Vendor */
			"Report-Interval",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Report-Interval)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Report-Amount */
	{
		struct dict_avp_data data = {
			1628,	/* Code */
			10415,	/* Vendor */
			"Report-Amount",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Report-Amount)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Event-Threshold-RSRP */
	{
		struct dict_avp_data data = {
			1629,	/* Code */
			10415,	/* Vendor */
			"Event-Threshold-RSRP",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Event-Threshold-RSRQ */
	{
		struct dict_avp_data data = {
			1630,	/* Code */
			10415,	/* Vendor */
			"Event-Threshold-RSRQ",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Logging-Interval */
	{
		struct dict_avp_data data = {
			1631,	/* Code */
			10415,	/* Vendor */
			"Logging-Interval",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Logging-Interval)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Logging-Duration */
	{
		struct dict_avp_data data = {
			1632,	/* Code */
			10415,	/* Vendor */
			"Logging-Duration",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Logging-Duration)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Relay-Node-Indicator */
	{
		struct dict_avp_data data = {
			1633,	/* Code */
			10415,	/* Vendor */
			"Relay-Node-Indicator",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Relay-Node-Indicator)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* MDT-User-Consent */
	{
		struct dict_avp_data data = {
			1634,	/* Code */
			10415,	/* Vendor */
			"MDT-User-Consent",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(MDT-User-Consent)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* PUR-Flags */
	{
		struct dict_avp_data data = {
			1635,	/* Code */
			10415,	/* Vendor */
			"PUR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Subscribed-VSRVCC */
	{
		struct dict_avp_data data = {
			1636,	/* Code */
			10415,	/* Vendor */
			"Subscribed-VSRVCC",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Subscribed-VSRVCC)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Equivalent-PLMN-List */
	{
		struct dict_avp_data data = {
			1637,	/* Code */
			10415,	/* Vendor */
			"Equivalent-PLMN-List",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* CLR-Flags */
	{
		struct dict_avp_data data = {
			1638,	/* Code */
			10415,	/* Vendor */
			"CLR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* UVR-Flags */
	{
		struct dict_avp_data data = {
			1639,	/* Code */
			10415,	/* Vendor */
			"UVR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* UVA-Flags */
	{
		struct dict_avp_data data = {
			1640,	/* Code */
			10415,	/* Vendor */
			"UVA-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* VPLMN-CSG-Subscription-Data */
	{
		struct dict_avp_data data = {
			1641,	/* Code */
			10415,	/* Vendor */
			"VPLMN-CSG-Subscription-Data",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Time-Zone */
	{
		struct dict_avp_data data = {
			1642,	/* Code */
			10415,	/* Vendor */
			"Time-Zone",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* A-MSISDN */
	{
		struct dict_avp_data data = {
			1643,	/* Code */
			10415,	/* Vendor */
			"A-MSISDN",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MME-Number-for-MT-SMS */
	{
		struct dict_avp_data data = {
			1645,	/* Code */
			10415,	/* Vendor */
			"MME-Number-for-MT-SMS",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* SMS-Register-Request */
	{
		struct dict_avp_data data = {
			1648,	/* Code */
			10415,	/* Vendor */
			"SMS-Register-Request",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(SMS-Register-Request)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Local-Time-Zone */
	{
		struct dict_avp_data data = {
			1649,	/* Code */
			10415,	/* Vendor */
			"Local-Time-Zone",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Daylight-Saving-Time */
	{
		struct dict_avp_data data = {
			1650,	/* Code */
			10415,	/* Vendor */
			"Daylight-Saving-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Daylight-Saving-Time)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Subscription-Data-Flags */
	{
		struct dict_avp_data data = {
			1654,	/* Code */
			10415,	/* Vendor */
			"Subscription-Data-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Measurement-Period-UMTS */
	{
		struct dict_avp_data data = {
			1655,	/* Code */
			10415,	/* Vendor */
			"Measurement-Period-UMTS",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Measurement-Period-UMTS)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Measurement-Period-LTE */
	{
		struct dict_avp_data data = {
			1656,	/* Code */
			10415,	/* Vendor */
			"Measurement-Period-LTE",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Measurement-Period-LTE)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Collection-Period-RRM-LTE */
	{
		struct dict_avp_data data = {
			1657,	/* Code */
			10415,	/* Vendor */
			"Collection-Period-RRM-LTE",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Collection-Period-RRM-LTE)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Collection-Period-RRM-UMTS */
	{
		struct dict_avp_data data = {
			1658,	/* Code */
			10415,	/* Vendor */
			"Collection-Period-RRM-UMTS",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Collection-Period-RRM-UMTS)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Positioning-Method */
	{
		struct dict_avp_data data = {
			1659,	/* Code */
			10415,	/* Vendor */
			"Positioning-Method",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Measurement-Quantity */
	{
		struct dict_avp_data data = {
			1660,	/* Code */
			10415,	/* Vendor */
			"Measurement-Quantity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Event-Threshold-Event-1F */
	{
		struct dict_avp_data data = {
			1661,	/* Code */
			10415,	/* Vendor */
			"Event-Threshold-Event-1F",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Event-Threshold-Event-1I */
	{
		struct dict_avp_data data = {
			1662,	/* Code */
			10415,	/* Vendor */
			"Event-Threshold-Event-1I",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 3GPP 29.329-b50 (11.5.0 2012.12.21)                          */
	/* User-Identity */
	{
		struct dict_avp_data data = {
			700,	/* Code */
			10415,	/* Vendor */
			"User-Identity",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* MSISDN */
	{
		struct dict_avp_data data = {
			701,	/* Code */
			10415,	/* Vendor */
			"MSISDN",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Name conflict with 29.229 User-Data (606), renamed           */
	/* User-Data-29.329 */
	{
		struct dict_avp_data data = {
			702,	/* Code */
			10415,	/* Vendor */
			"User-Data-29.329",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Data-Reference */
	{
		struct dict_avp_data data = {
			703,	/* Code */
			10415,	/* Vendor */
			"Data-Reference",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Data-Reference)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Service-Indication */
	{
		struct dict_avp_data data = {
			704,	/* Code */
			10415,	/* Vendor */
			"Service-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Subs-Req-Type */
	{
		struct dict_avp_data data = {
			705,	/* Code */
			10415,	/* Vendor */
			"Subs-Req-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Subs-Req-Type)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Requested-Domain */
	{
		struct dict_avp_data data = {
			706,	/* Code */
			10415,	/* Vendor */
			"Requested-Domain",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Requested-Domain)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Current-Location */
	{
		struct dict_avp_data data = {
			707,	/* Code */
			10415,	/* Vendor */
			"Current-Location",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Current-Location)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Identity-Set */
	{
		struct dict_avp_data data = {
			708,	/* Code */
			10415,	/* Vendor */
			"Identity-Set",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Identity-Set)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Expiry-Time */
	{
		struct dict_avp_data data = {
			709,	/* Code */
			10415,	/* Vendor */
			"Expiry-Time",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
	};

	/* Send-Data-Indication */
	{
		struct dict_avp_data data = {
			710,	/* Code */
			10415,	/* Vendor */
			"Send-Data-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Send-Data-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* in 29.229                                                    */
	/* Server-Name                                                  */
	/* Supported-Features                                           */
	/* Feature-List-ID                                              */
	/* Feature-List                                                 */
	/* Supported-Applications                                       */
	/* Public-Identity                                              */
	/* DSAI-Tag */
	{
		struct dict_avp_data data = {
			711,	/* Code */
			10415,	/* Vendor */
			"DSAI-Tag",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* in 29.229                                                    */
	/* Wildcarded-Public-Identity                                   */
	/* Wildcarded-IMPU */
	{
		struct dict_avp_data data = {
			636,	/* Code */
			10415,	/* Vendor */
			"Wildcarded-IMPU",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* in 29.229                                                    */
	/* Session-Priority                                             */
	/* One-Time-Notification */
	{
		struct dict_avp_data data = {
			712,	/* Code */
			10415,	/* Vendor */
			"One-Time-Notification",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(One-Time-Notification)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Requested-Nodes */
	{
		struct dict_avp_data data = {
			713,	/* Code */
			10415,	/* Vendor */
			"Requested-Nodes",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Serving-Node-Indication */
	{
		struct dict_avp_data data = {
			714,	/* Code */
			10415,	/* Vendor */
			"Serving-Node-Indication",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_INTEGER32	/* base type of data */
		};
		struct dict_object		*type;
		struct dict_type_data	 tdata = { AVP_TYPE_INTEGER32, "3GPP/Enumerated(Serving-Node-Indication)", NULL, NULL, NULL };
		CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
		CHECK_dict_new(DICT_AVP, &data, type, NULL);
	};

	/* Repository-Data-ID */
	{
		struct dict_avp_data data = {
			715,	/* Code */
			10415,	/* Vendor */
			"Repository-Data-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Sequence-Number */
	{
		struct dict_avp_data data = {
			716,	/* Code */
			10415,	/* Vendor */
			"Sequence-Number",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* UDR-Flags */
	{
		struct dict_avp_data data = {
			719,	/* Code */
			10415,	/* Vendor */
			"UDR-Flags",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* OMA DDS Charging_Data V1.0 20110201-A                        */
	/* Application-Server-Id */
	{
		struct dict_avp_data data = {
			2101,	/* Code */
			10415,	/* Vendor */
			"Application-Server-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Application-Service-Type */
	{
		struct dict_avp_data data = {
			2102,	/* Code */
			10415,	/* Vendor */
			"Application-Service-Type",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Application-Session-Id */
	{
		struct dict_avp_data data = {
			2103,	/* Code */
			10415,	/* Vendor */
			"Application-Session-Id",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Content-ID */
	{
		struct dict_avp_data data = {
			2116,	/* Code */
			10415,	/* Vendor */
			"Content-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* Content-provider-ID */
	{
		struct dict_avp_data data = {
			2117,	/* Code */
			10415,	/* Vendor */
			"Content-provider-ID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* DCD-Information */
	{
		struct dict_avp_data data = {
			2115,	/* Code */
			10415,	/* Vendor */
			"DCD-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Delivery-Status */
	{
		struct dict_avp_data data = {
			2104,	/* Code */
			10415,	/* Vendor */
			"Delivery-Status",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
	};

	/* IM-Information */
	{
		struct dict_avp_data data = {
			2110,	/* Code */
			10415,	/* Vendor */
			"IM-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Number-Of-Messages-Successfully-Exploded */
	{
		struct dict_avp_data data = {
			2111,	/* Code */
			10415,	/* Vendor */
			"Number-Of-Messages-Successfully-Exploded",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Number-Of-Messages-Successfully-Sent */
	{
		struct dict_avp_data data = {
			2112,	/* Code */
			10415,	/* Vendor */
			"Number-Of-Messages-Successfully-Sent",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Service-Generic-Information */
	{
		struct dict_avp_data data = {
			1256,	/* Code */
			10415,	/* Vendor */
			"Service-Generic-Information",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_GROUPED	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Total-Number-Of-Messages-Exploded */
	{
		struct dict_avp_data data = {
			2113,	/* Code */
			10415,	/* Vendor */
			"Total-Number-Of-Messages-Exploded",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* Total-Number-Of-Messages-Sent */
	{
		struct dict_avp_data data = {
			2114,	/* Code */
			10415,	/* Vendor */
			"Total-Number-Of-Messages-Sent",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flag values */
			AVP_TYPE_UNSIGNED32	/* base type of data */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};




	/*****************************************************/
	/* end of generated data, rules below added manually */
	/*****************************************************/


	
	/* 3GPP2-BSID */
	{
		struct dict_avp_data data = {
			9010,	/* Code */
			5535,	/* Vendor */
			"3GPP2-BSID",	/* Name */
			AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,	/* Fixed flags */
			AVP_FLAG_VENDOR,	/* Fixed flag values */
			AVP_TYPE_OCTETSTRING	/* base type of data */ /* XXX: guessed */
		};
		CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
	};

	/* 29.212 */
	
	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Allocation-Retention-Priority";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Priority-Level" }, RULE_REQUIRED, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Pre-emption-Capability" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Pre-emption-Vulnerability" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "QoS-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "QoS-Class-Identifier" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Max-Requested-Bandwidth-UL" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Max-Requested-Bandwidth-DL" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Guaranteed-Bitrate-UL" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Guaranteed-Bitrate-DL" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Bearer-Identifier" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Allocation-Retention-Priority" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "APN-Aggregate-Max-Bitrate-UL" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "APN-Aggregate-Max-Bitrate-DL" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	/* 32.299 */

	{
		/* additional allowed AVPs */
		struct dict_object *ccr;
		CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Request", &ccr);
		struct local_rules_definition rules[] = 
			{
				{ { .avp_vendor = 10415, .avp_name = "AoC-Request-Type"}, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Service-Information"}, RULE_OPTIONAL, -1, 1 },
			};
		PARSE_loc_rules(rules, ccr);
        }

	{
		/* additional allowed AVPs */
		struct dict_object *ccr;
		CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Answer", &ccr);
		struct local_rules_definition rules[] = 
			{
				{ { .avp_vendor = 10415, .avp_name = "Low-Balance-Indication"}, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Remaining-Balance"}, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Service-Information"}, RULE_OPTIONAL, -1, 1 },
			};
		PARSE_loc_rules(rules, ccr);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Application-Server-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Application-Server" }, RULE_REQUIRED, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Application-Provided-Called-Party-Address" }, RULE_OPTIONAL, -1, -1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Envelope";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Envelope-Start-Time" }, RULE_REQUIRED, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Envelope-End-Time" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 0,     .avp_name = "CC-Total-Octets" }, RULE_REQUIRED, -1, 1 },
				{ { .avp_vendor = 0,     .avp_name = "CC-Input-Octets" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 0,     .avp_name = "CC-Output-Octets" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 0,     .avp_name = "CC-Service-Specific-Units" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Event-Type";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "SIP-Method" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Event" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Expires" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "IMS-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Event-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Role-Of-Node" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Node-Functionality" }, RULE_REQUIRED, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "User-Session-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Outgoing-Session-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Session-Priority" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Calling-Party-Address" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Called-Party-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Called-Asserted-Identity" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Number-Portability-Routing-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Carrier-Select-Routing-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Alternate-Charged-Party-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Requested-Party-Address" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Associated-URI" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Time-Stamps" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Application-Server-Information" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Inter-Operator-Identifier" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Transit-IOI-List" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "IMS-Charging-Identifier" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SDP-Session-Description" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "SDP-Media-Component" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Served-Party-IP-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Server-Capabilities" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Trunk-Group-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Bearer-Service" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Service-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Service-Specific-Info" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Message-Body" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Cause-Code" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Access-Network-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Early-Media-Description" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "IMS-Communication-Service-Identifier" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "IMS-Application-Reference-Identifier" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Online-Charging-Flag" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Real-Time-Tariff-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Account-Expiration" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Initial-IMS-Charging-Identifier" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "NNI-Information" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "From-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "IMS-Emergency-Indicator" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Access-Transfer-Information" }, RULE_OPTIONAL, -1, -1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "MMS-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Originator-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Recipient-Address" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Submission-Time" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "MM-Content-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Priority" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Message-ID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Message-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Message-Size" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Message-Class" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Delivery-Report-Requested" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Read-Reply-Report-Requested" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "MMBox-Storage-Requested" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Applic-ID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Reply-Applic-ID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Aux-Applic-Info" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Content-Class" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "DRM-Content" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Adaptations" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "VASP-ID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "VAS-ID" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

        {
		/* Multiple-Services-Credit-Control */
		/* additional allowed AVPs */
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 0;
		vpa.avp_name = "Multiple-Services-Credit-Control";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Time-Quota-Threshold" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Volume-Quota-Threshold" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Unit-Quota-Threshold" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Quota-Holding-Time" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Quota-Consumption-Time" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Reporting-Reason" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Trigger" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PS-Furnish-Charging-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Refund-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "AF-Correlation-Information" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Envelope" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Envelope-Reporting" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Time-Quota-Mechanism" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Service-Specific-Info" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "QoS-Information" }, RULE_OPTIONAL, -1, 1 },
			};
		PARSE_loc_rules(rules, rule_avp);
        }

	{
		/* Used-Service-Unit */
		/* additional allowed AVPs */
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 0;
		vpa.avp_name = "Used-Service-Unit";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Reporting-Reason" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Event-Charging-TimeStamp" }, RULE_OPTIONAL, -1, -1 },
			};
		PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Offline-Charging";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Quota-Consumption-Time" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Time-Quota-Mechanism" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Envelope-Reporting" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 0,     .avp_name = "Multiple-Services-Credit-Control" }, RULE_OPTIONAL, -1, -1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Originator-Address";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Address-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Address-Data" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Address-Domain" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "PS-Furnish-Charging-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "3GPP-Charging-Id" }, RULE_REQUIRED, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PS-Free-Format-Data" }, RULE_REQUIRED, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PS-Append-Free-Format-Data" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "PS-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "3GPP-Charging-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PDN-Connection-Charging-ID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Node-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-PDP-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PDP-Address" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "PDP-Address-Prefix-Length" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Dynamic-Address-Flag" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Dynamic-Address-Flag-Extension" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "QoS-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SGSN-Address" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "GGSN-Address" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "SGW-Address" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "CG-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Serving-Node-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SGW-Change" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-IMSI-MCC-MNC" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "IMSI-Unauthenticated-Flag" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-GGSN-MCC-MNC" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-NSAPI" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 0,     .avp_name = "Called-Station-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-Session-Stop-Indicator" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-Selection-Mode" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-Charging-Characteristics" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Charging-Characteristics-Selection-Mode" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-SGSN-MCC-MNC" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-MS-TimeZone" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Charging-Rule-Base-Name" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-User-Location-Info" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "User-CSG-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 5535,  .avp_name = "3GPP2-BSID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-RAT-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PS-Furnish-Charging-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PDP-Context-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Offline-Charging" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Traffic-Data-Volumes" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Service-Data-Container" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 0,     .avp_name = "User-Equipment-Info" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Terminal-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Start-Time" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Stop-Time" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Change-Condition" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Diagnostics" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Low-Priority-Indicator" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "MME-Number-for-MT-SMS" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "MME-Name" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "MME-Realm" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Recipient-Address";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Address-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Address-Data" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Address-Domain" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Addressee-Type" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "SDP-Media-Component";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "SDP-Media-Name" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SDP-Media-Description" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Local-GW-Inserted-Indication" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "IP-Realm-Default-Indication" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Transcoder-Inserted-Indication" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Media-Initiator-Flag" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Media-Initiator-Party" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP-Charging-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Access-Network-Charging-Identifier-Value" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SDP-Type" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Service-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 0,     .avp_name = "Subscription-Id" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "AoC-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PS-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "WLAN-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "IMS-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "MMS-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "LCS-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "PoC-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "MBMS-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SMS-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "MMTel-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Service-Generic-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "IM-Information" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "DCD-Information" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "SMS-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "SMS-Node" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Client-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Originator-SCCP-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SMSC-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Data-Coding-Scheme" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SM-Discharge-Time" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SM-Message-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Originator-Interface" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SM-Protocol-ID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Reply-Path-Requested" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SM-Status" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SM-User-Data-Header" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Number-Of-Messages-Sent" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Recipient-Info" }, RULE_OPTIONAL, -1, -1 },
				{ { .avp_vendor = 10415, .avp_name = "Originator-Received-Address" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SM-Service-Type" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Time-Quota-Mechanism";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Time-Quota-Type" }, RULE_REQUIRED, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Base-Time-Interval" }, RULE_REQUIRED, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Time-Stamps";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "SIP-Request-Timestamp" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SIP-Response-Timestamp" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SIP-Request-Timestamp-Fraction" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "SIP-Response-Timestamp-Fraction" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }

	/* OMA */
	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "DCD-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Content-ID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Content-provider-ID" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }
	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "IM-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Total-Number-Of-Messages-Sent" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Total-Number-Of-Messages-Exploded" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Number-Of-Messages-Successfully-Sent" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Number-Of-Messages-Successfully-Exploded" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }
	{
		struct dict_object *rule_avp;
		struct dict_avp_request vpa;
		vpa.avp_vendor = 10415;
		vpa.avp_name = "Service-Generic-Information";
		CHECK_dict_search(DICT_AVP, AVP_BY_NAME_AND_VENDOR, &vpa, &rule_avp);
		struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Application-Server-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Application-Service-Type" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Application-Session-Id" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Delivery-Status" }, RULE_OPTIONAL, -1, 1 },
			};
			PARSE_loc_rules(rules, rule_avp);
        }
	
	LOG_D( "Extension 'Dictionary definitions for DCCA 3GPP' initialized");
	return 0;
}

EXTENSION_ENTRY("dict_dcca_3gpp", dict_dcca_3gpp_entry, "dict_dcca");
