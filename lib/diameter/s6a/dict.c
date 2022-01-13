/*********************************************************************************************************
 * Software License Agreement (BSD License)                                                               *
 * Author: Sukchan Lee <acetcom@gmail.com>>                                                                *
 *                                                                                                        *
 * Copyright (c) 2017, Open5gs Group
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

int ogs_dict_s6a_entry(char *conffile)
{
    struct dict_object *s6a;
	TRACE_ENTRY("%p", conffile);

	/* Applications section */
    {
        struct dict_object * vendor;
        CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_NAME, "3GPP", &vendor, ENOENT));
        struct dict_application_data app_data = { 16777251, "S6A" };
        CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION, &app_data, vendor, &s6a));
    }

	/* AVP section */
	{
        {
            /* AIR-Flags AVP - 3GPP TS 29.272 #7.3.201 */
            struct dict_avp_data data = {
                1679,                                   /* Code */
                10415,                                  /* Vendor */
                "AIR-Flags",                            /* Name */
                AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,    /* Fixed flags */
                AVP_FLAG_VENDOR,                        /* Fixed flag values */
                AVP_TYPE_UNSIGNED32                     /* base type of data */
            };
            CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
        }

		/* Terminal Information AVP - 3GPP TS 29.272 #7.3.3 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "IMEI" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "3GPP2-MEID" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Software-Version" }, RULE_OPTIONAL, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Terminal-Information", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Requested-EUTRAN-Authentication-Info AVP - 3GPP TS 29.272  #7.3.11 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Number-Of-Requested-Vectors" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Immediate-Response-Preferred" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Re-Synchronization-Info" }, RULE_OPTIONAL, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Requested-EUTRAN-Authentication-Info", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Requested-UTRAN-GERAN-Authentication-Info AVP - 3GPP TS 29.272  #7.3.12 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
				{ { .avp_vendor = 10415, .avp_name = "Number-Of-Requested-Vectors" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Immediate-Response-Preferred" }, RULE_OPTIONAL, -1, 1 },
				{ { .avp_vendor = 10415, .avp_name = "Re-Synchronization-Info" }, RULE_OPTIONAL, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Requested-UTRAN-GERAN-Authentication-Info", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* E-UTRAN-Vector - 3GPP TS 29.272  #7.3.18 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "Item-Number" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "RAND" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "XRES" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "AUTN" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "KASME" }, RULE_REQUIRED, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "E-UTRAN-Vector", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* UTRAN-Vector - 3GPP TS 29.272  #7.3.19 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "Item-Number" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "RAND" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "XRES" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "AUTN" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Confidentiality-Key" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Integrity-Key" }, RULE_REQUIRED, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "UTRAN-Vector", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* GERAN-Vector - 3GPP TS 29.272  #7.3.20 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "Item-Number" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "RAND" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "SRES" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Kc" }, RULE_REQUIRED, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "GERAN-Vector", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Authentication-Info - 3GPP TS 29.272  #7.3.17 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "E-UTRAN-Vector" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "UTRAN-Vector" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "GERAN-Vector" }, RULE_OPTIONAL, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Authentication-Info", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* EPS-Subscribed-QoS-Profile AVP - 3GPP TS 29.272 #7.3.37 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "QoS-Class-Identifier" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Allocation-Retention-Priority" }, RULE_REQUIRED, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "EPS-Subscribed-QoS-Profile", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* AMBR AVP - 3GPP TS 29.272 #7.3.41 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "Max-Requested-Bandwidth-UL" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Max-Requested-Bandwidth-DL" }, RULE_REQUIRED, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AMBR", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Specific-APN-Info AVP - 3GPP TS 29.272  #7.3.82 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { {                      .avp_name = "Service-Selection" }, RULE_REQUIRED, -1, 1 },
                { {                      .avp_name = "MIP6-Agent-Info" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Visited-Network-Identifier" }, RULE_OPTIONAL, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Specific-APN-Info", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* APN-Configuration AVP - 3GPP TS 29.272 #7.3.35 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "Context-Identifier" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Served-Party-IP-Address" }, RULE_OPTIONAL, -1, 2 },
                { { .avp_vendor = 10415, .avp_name = "PDN-Type" }, RULE_REQUIRED, -1, 1 },
                { {                      .avp_name = "Service-Selection" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "EPS-Subscribed-QoS-Profile" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "VPLMN-Dynamic-Address-Allowed" }, RULE_OPTIONAL, -1, 1 },
                { {                      .avp_name = "MIP6-Agent-Info" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Visited-Network-Identifier" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "PDN-GW-Allocation-Type" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "3GPP-Charging-Characteristics" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "AMBR" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Specific-APN-Info" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "APN-OI-Replacement" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "SIPTO-Permission" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "LIPA-Permission" }, RULE_OPTIONAL, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Configuration", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* APN-Configuration-Profile AVP - 3GPP TS 29.272 #7.3.34 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "Context-Identifier" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "All-APN-Configurations-Included-Indicator" }, RULE_REQUIRED, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "APN-Configuration" }, RULE_REQUIRED, -1, -1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Configuration-Profile", &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Subscription-Data AVP - 3GPP TS 29.272 #7.3.2 */
		{
			struct dict_object * avp;
			struct local_rules_definition rules[] =
			{
                { { .avp_vendor = 10415, .avp_name = "Subscriber-Status" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "MSISDN" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "STN-SR" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "ICS-Indicator" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Network-Access-Mode" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Operator-Determined-Barring" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "HPLMN-ODB" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Regional-Subscription-Zone-Code" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Access-Restriction-Data" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "APN-OI-Replacement" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "LCS-Info" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Teleservice-List" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Call-Barring-Info" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "3GPP-Charging-Characteristics" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "AMBR" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "APN-Configuration-Profile" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "RAT-Frequency-Selection-Priority-ID" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Trace-Data" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "GPRS-Subscription-Data" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "CSG-Subscription-Data" }, RULE_OPTIONAL, -1, -1 },
                { { .avp_vendor = 10415, .avp_name = "Roaming-Restricted-Due-To-Unsupported-Feature" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Subscribed-Periodic-RAU-TAU-Timer" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "MPS-Priority" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "VPLMN-LIPA-Allowed" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Relay-Node-Indicator" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "MDT-User-Consent" }, RULE_OPTIONAL, -1, 1 },
			};

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Data", &avp);
			PARSE_loc_rules( rules, avp );
		}
	};

    /* Command section */
    {
		/* S6A-Update Location Request - 3GPP TS 29.272 #7.2.3 */
        {
			struct dict_object * cmd;
            struct dict_cmd_data data = {
                316,                                                    /* Code */
                "Update-Location-Request",                              /* Name */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
            };
            struct local_rules_definition rules[] =
            {
                {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
                {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "User-Name" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
                {  { .avp_vendor = 10415, .avp_name = "Terminal-Information" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "ULR-Flags" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Visited-PLMN-Id" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Equivalent-PLMN-List" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "RAT-Type" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "SGSN-Number" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Homogeneous-Support-of-IMS-Voice-Over-PS-Sessions" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Active-APN" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "UE-SRVCC-Capability" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
            };

            CHECK_dict_new(DICT_COMMAND, &data, s6a, &cmd);
            PARSE_loc_rules(rules, cmd);
        }

		/* S6A-Update Location Answer - 3GPP TS 29.272 #7.2.4 */
        {
			struct dict_object * cmd;
            struct dict_cmd_data data = {
                316,                                                    /* Code */
                "Update-Location-Answer",                               /* Name */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
                CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
            };
            struct local_rules_definition rules[] =
            {
                {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
                {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Error-Diagnostic" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "ULA-Flags" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Subscription-Data" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
            };

            CHECK_dict_new(DICT_COMMAND, &data, s6a, &cmd);
            PARSE_loc_rules(rules, cmd);
        }

		/* S6A-Authentication-Information-Request - 3GPP TS 29.272 #7.2.4 */
        {
			struct dict_object * cmd;
            struct dict_cmd_data data = {
                318,                                                    /* Code */
                "Authentication-Information-Request",                   /* Name */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
            };
            struct local_rules_definition rules[] =
            {
                {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
                {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "User-Name" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
                {  { .avp_vendor = 10415, .avp_name = "Requested-EUTRAN-Authentication-Info" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Requested-UTRAN-GERAN-Authentication-Info" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Visited-PLMN-Id" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "AIR-Flags" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
            };

            CHECK_dict_new(DICT_COMMAND, &data, s6a, &cmd);
            PARSE_loc_rules(rules, cmd);
        }

		/* S6A-Authentication-Information-Answer - 3GPP TS 29.272 #7.2.6 */
        {
			struct dict_object * cmd;
            struct dict_cmd_data data = {
                318,                                                    /* Code */
                "Authentication-Information-Answer",                    /* Name */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
                CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
            };
            struct local_rules_definition rules[] =
            {
                {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
                {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Error-Diagnostic" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
                {  { .avp_vendor = 10415, .avp_name = "Authentication-Info" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
            };

            CHECK_dict_new(DICT_COMMAND, &data, s6a, &cmd);
            PARSE_loc_rules(rules, cmd);
        }

		/* Purge-UE-Request (PUR) Command - 3GPP TS 29.272 #7.2.13 */
		{
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				321,                                                        /* Code */
				"Purge-UE-Request",                                         /* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR,     /* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                       /* Fixed flag values */
			};
			struct local_rules_definition rules[] =
			{
                {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
                {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "User-Name" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
#if 0 /* modified by acetcom */
                {  { .avp_vendor = 10415, .avp_name = "PUR-Flags" }, RULE_REQUIRED, -1, 1 },
#else
                {  { .avp_vendor = 10415, .avp_name = "PUR-Flags" }, RULE_OPTIONAL, -1, 1 },
#endif
                {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Purge-UE-Answer (PUA) Command - 3GPP TS 29.272 #7.2.14 */
		{
			struct dict_object * cmd;
			struct dict_cmd_data data = {
				321,                                                    /* Code */
				"Purge-UE-Answer",                                      /* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
				CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
			};
			struct local_rules_definition rules[] =
			{
                {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
                {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Error-Diagnostic" }, RULE_OPTIONAL, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "PUA-Flags" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
			};

			CHECK_dict_new( DICT_COMMAND, &data, s6a, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

    }

    LOG_D( "Extension 'Dictionary definitions for DCCA 3GPP S6A' initialized");
	return 0;
}

#if 0 /* modified by acetcom */
EXTENSION_ENTRY("dict_s6a", ogs_dict_s6a_entry, "dict_dcca_3gpp");
#endif
