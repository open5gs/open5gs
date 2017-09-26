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

The following table complete the one in RFC 5777. The AVPs are implemented in the order of the table.
We try to keep the structure of the grouped AVP by declaring the contained AVPs just before the grouped AVP they depend on.
The number of '+' indicates the depth of the contained AVP.

DEPTH	NAME					AVP	RFC	TYPE			NOTES

++	Vendor-Id				266	3588	Unsigned32
++	QoS-Profile-Id				573	5777	Unsigned32
+	QoS-Profile-Template			574	5777	Grouped
	QoS-Capability				578	5777	Grouped
+++	QoS-Parameters				576	5777	Grouped			no specific AVPs? see RFC 5624
+++	QoS-Profile-Template			--------------------------------
+++	Treatment-Action			572	5777	Enumerated		Type is Enumerated. See Errata 2334 for RFC5777
++	Excess-Treatment			577	5777	Grouped
++	QoS-Parameters				--------------------------------
++	QoS-Profile-Template			--------------------------------
++	QoS-Semantics				575	5777	Enumerated
++	Treatment-Action			--------------------------------
+++	Timezone-Offset				571	5777	Integer32
+++	Timezone-Flag				570	5777	Enumerated
+++	Absolute-End-Fractional-Seconds		569	5777	Unsigned32
+++	Absolute-End-Time			568	5777	Time
+++	Absolute-Start-Fractional-Seconds	567	5777	Unsigned32
+++	Absolute-Start-Time			566	5777	Time
+++	Month-Of-Year-Mask			565	5777	Unsigned32
+++	Day-Of-Month-Mask			564	5777	Unsigned32
+++	Day-Of-Week-Mask			563	5777	Unsigned32
+++	Time-Of-Day-End				562	5777	Unsigned32
+++	Time-Of-Day-Start			561	5777	Unsigned32
++	Time-Of-Day-Condition			560	5777	Grouped			Some AVPs were omitted. See Errata 2333 for RFC5777
+++++	High-User-Priority			559	5777	Unsigned32
+++++	Low-User-Priority			558	5777	Unsigned32
++++	User-Priority-Range			557	5777	Grouped
+++++	S-VID-Start				553	5777	Unsigned32
+++++	S-VID-End				554	5777	Unsigned32
+++++	C-VID-Start				555	5777	Unsigned32
+++++	C-VID-End				556	5777	Unsigned32
++++	VLAN-ID-Range				552	5777	Grouped
+++++	ETH-SAP					551	5777	OctetString
+++++	ETH-Ether-Type				550	5777	OctetString
++++	ETH-Proto-Type				549	5777	Grouped
+++	ETH-Option				548	5777	Grouped
++++	Negated					517	5777	Enumerated
++++	ICMP-Code				547	5777	Integer32		Changed from Enumerated for implementation reason. See AVP for details.
++++	ICMP-Type-Number			546	5777	Enumerated
+++	ICMP-Type				545	5777	Grouped
++++	Negated					--------------------------------
++++	TCP-Flag-Type				544	5777	Unsigned32
+++	TCP-Flags				543	5777	Grouped
++++	TCP-Option-Type				541	5777	Enumerated
++++	TCP-Option-Value			542	5777	OctetString
++++	Negated					--------------------------------
+++	TCP-Option				540	5777	Grouped
++++	IP-Option-Type				538	5777	Enumerated
++++	IP-Option-Value				539	5777	OctetString
++++	Negated					--------------------------------
+++	IP-Option				537	5777	Grouped
+++	Fragmentation-Flag			536	5777	Enumerated
+++	Diffserv-Code-Point			535	5777	Enumerated
++++	IP-Address				518	5777	Address
+++++	IP-Address-Start			520	5777	Address
+++++	IP-Address-End				521	5777	Address
++++	IP-Address-Range			519	5777	Grouped
+++++	IP-Address				--------------------------------
+++++	IP-Mask-Bit-Mask-Width			523	5777	Unsigned32		Name is IP-Mask-Bit-Mask-Width. See Errata 2335 for RFC5777
++++	IP-Address-Mask				522	5777	Grouped
++++	MAC-Address				524	5777	OctetString
+++++	MAC-Address				--------------------------------
+++++	MAC-Address-Mask-Pattern		526	5777	OctetString
++++	MAC-Address-Mask			525	5777	Grouped
++++	EUI64-Address				527	5777	OctetString
+++++	EUI64-Address				--------------------------------
+++++	EUI64-Address-Mask-Pattern		529	5777	OctetString
++++	EUI64-Address-Mask			528	5777	Grouped
++++	Port					530	5777	Integer32
+++++	Port-Start				532	5777	Integer32
+++++	Port-End				533	5777	Integer32
++++	Port-Range				531	5777	Grouped
++++	Negated					--------------------------------
++++	Use-Assigned-Address			534	5777	Enumerated
+++	From-Spec				515	5777	Grouped
++++	------------------------------------------------------------------------
++++	-----------------same dependencies as From-Spec-------------------------
++++	------------------------------------------------------------------------
+++	To-Spec					516	5777	Grouped
+++	Direction				514	5777	Enumerated
+++	Protocol				513	5777	Enumerated
+++	Classifier-ID				512	5777	OctetString
++	Classifier				511	5777	Grouped
++	Filter-Rule-Precedence			510	5777	Unsigned32
+	Filter-Rule				509	5777	Grouped			careful if we have to change vendor specifics or such.
	QoS-Resources				508	5777	Grouped

*/





/****************************************************************************************************************************************
*																	*
* This table contains the Enumerated AVPs whose values are managed by the IANA.								*
*																	*
* Note : The ICMP-Code AVP (AVP Code 547) has been changed to type Integer32. See AVP for details					*
*                                                                                                       				*
* AVP	NAME			LAST UPDATED	LAST CHECKED	SOURCE                                                  		*
* 546	ICMP-Type-Number	2010-04-22	2010-07-02	http://www.iana.org/assignments/icmp-parameters         		*
* 541	TCP-Option-Type		2010-06-22	2010-07-02	http://www.iana.org/assignments/tcp-parameters/tcp-parameters.txt	*
* 538	IP-Option-Type		2007-02-15	2010-07-02	http://www.iana.org/assignments/ip-parameters				*
* 535	Diffserv-Code-Point	2010-05-11	2010-07-02	http://www.iana.org/assignments/dscp-registry/dscp-registry.txt		*
* 513   Protocol                2004-01-26      2010-07-02	http://www.iana.org/assignments/ips-protocols                           *
*																	*
****************************************************************************************************************************************/


/*

NOTES

Sections 10.2 and 10.3 of the RFC5777 have been ignored

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

// No result-Code or values to define in RFC5777, we proceed

/* Dictionary */

int dict_rfc5777_init(char * conffile)
{
	struct dict_object * rfc5777;
	{
		struct dict_application_data data = { 5777, "Traffic Classification and Quality of Service (QoS) Attributes for Diameter (RFC 5777)" };
		CHECK_dict_new( DICT_APPLICATION, &data , NULL, &rfc5777);
	}

	/* AVP section */
	{
		/* Loading the derived data formats */

		struct dict_object * Address_type;
		struct dict_object * Time_type;

		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Address", &Address_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Time", &Time_type);

		/* Vendor_Id - RFC 3588 */

			/*
			Implemented in the base protocol
			*/

		/* QoS-Profile-Id - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					573, 					/* Code */
					0, 					/* Vendor */
					"QoS-Profile-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* QoS-Profile-Template - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					574, 					/* Code */
					0, 					/* Vendor */
					"QoS-Profile-Template",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "Vendor-Id",	RULE_REQUIRED,	-1, 1 }
							,{  "QoS-Profile-Id",	RULE_REQUIRED,	-1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* QoS-Capability - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					578, 					/* Code */
					0, 					/* Vendor */
					"QoS-Capability",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "QoS-Profile-Template", RULE_REQUIRED, -1, -1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* QoS-Parameters - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					576, 					/* Code */
					0, 					/* Vendor */
					"QoS-Parameters",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{		//empty??
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* QoS-Profile-Template - RFC 5777 */

			/*
			Implemented higher
			*/


		/* Treatment-Action - RFC 5777 */
		{
			/*

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Treatment-Action)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "drop",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "shape", 	{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "mark",		{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "permit", 	{ .i32 = 3 }};

			struct dict_avp_data 	data = {
					572, 					/* Code */
					0, 					/* Vendor */
					"Treatment-Action",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}


		/* Excess-Treatment - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					577, 					/* Code */
					0, 					/* Vendor */
					"Excess-Treatment",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "Treatment-Action",		RULE_REQUIRED, -1, 1 }
							,{  "QoS-Profile-Template",	RULE_OPTIONAL, -1, 1 }
							,{  "QoS-Parameters",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* QoS-Parameters - RFC 5777 */

			/*
			Implemented higher
			*/

		/* QoS-Profile-Template - RFC 5777 */

			/*
			Implemented higher
			*/

		/* QoS-Semantics - RFC 5777 */
		{
			/*

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(QoS-Semantics)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "QoS-Desired",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "QoS-Available", 	{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "QoS-Delivered",	{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "Minimum-QoS", 		{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "QoS-Authorized", 	{ .i32 = 4 }};

			struct dict_avp_data 	data = {
					575, 					/* Code */
					0, 					/* Vendor */
					"QoS-Semantics",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}



		/* Treatment-Action - RFC 5777 */

			/*
			Implemented higher
			*/

		/* Timezone-Offset - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					571, 					/* Code */
					0, 					/* Vendor */
					"Timezone-Offset", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_INTEGER32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Timezone-Flag - RFC 5777 */
		{
			/*

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Timezone-Flag)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "UTC",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "LOCAL", 	{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "OFFSET",	{ .i32 = 2 }};

			struct dict_avp_data 	data = {
					570, 					/* Code */
					0, 					/* Vendor */
					"Timezone-Flag",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}


		/* Absolute-End-Fractional-Seconds - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					569, 					/* Code */
					0, 					/* Vendor */
					"Absolute-End-Fractional-Seconds", 	/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Absolute-End-Time - RFC 5777 */
		{
			/*

			*/
			struct dict_avp_data data = {
					568, 					/* Code */
					0, 					/* Vendor */
					"Absolute-End-Time", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , Time_type, NULL);
		}

		/* Absolute-Start-Fractional-Seconds - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					567, 					/* Code */
					0, 					/* Vendor */
					"Absolute-Start-Fractional-Seconds", 	/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Absolute-Start-Time - RFC 5777 */
		{
			/*

			*/
			struct dict_avp_data data = {
					566, 					/* Code */
					0, 					/* Vendor */
					"Absolute-Start-Time", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , Time_type, NULL);
		}

		/* Month-Of-Year-Mask - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					565, 					/* Code */
					0, 					/* Vendor */
					"Month-Of-Year-Mask", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Day-Of-Month-Mask - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					564, 					/* Code */
					0, 					/* Vendor */
					"Day-Of-Month-Mask", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Day-Of-Week-Mask - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					563, 					/* Code */
					0, 					/* Vendor */
					"Day-Of-Week-Mask", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Time-Of-Day-End - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					562, 					/* Code */
					0, 					/* Vendor */
					"Time-Of-Day-End", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Time-Of-Day-Start - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					561, 					/* Code */
					0, 					/* Vendor */
					"Time-Of-Day-Start", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Time-Of-Day-Condition - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					560, 					/* Code */
					0, 					/* Vendor */
					"Time-Of-Day-Condition", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] =
						{ 	 {  "Time-Of-Day-Start", 	RULE_OPTIONAL, -1, 1 }
							,{  "Time-Of-Day-End",		RULE_OPTIONAL, -1, 1 }
							,{  "Day-Of-Week-Mask",		RULE_OPTIONAL, -1, 1 }
							,{  "Day-Of-Month-Mask",	RULE_OPTIONAL, -1, 1 }
							,{  "Month-Of-Year-Mask",	RULE_OPTIONAL, -1, 1 }
							,{  "Absolute-Start-Time",	RULE_OPTIONAL, -1, 1 }
							,{  "Absolute-End-Time",	RULE_OPTIONAL, -1, 1 }
							,{  "Timezone-Flag",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* High-User-Priority - RFC 5777*/
		{
			/*

			*/

			struct dict_avp_data data = {
					559, 					/* Code */
					0, 					/* Vendor */
					"High-User-Priority", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Low-User-Priority - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					558, 					/* Code */
					0, 					/* Vendor */
					"Low-User-Priority", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* User-Priority-Range - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					557, 					/* Code */
					0, 					/* Vendor */
					"User-Priority-Range", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] =
						{ 	 {  "Low-User-Priority", 	RULE_OPTIONAL, -1, -1 }
							,{  "High-User-Priority",	RULE_OPTIONAL, -1, -1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		/* S-VID-Start - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					553, 					/* Code */
					0, 					/* Vendor */
					"S-VID-Start", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* S-VID-End - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					554, 					/* Code */
					0, 					/* Vendor */
					"S-VID-End", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* C-VID-Start - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					555, 					/* Code */
					0, 					/* Vendor */
					"C-VID-Start",	 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* C-VID-End - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					556, 					/* Code */
					0, 					/* Vendor */
					"C-VID-End",	 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* VLAN-ID-Range - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					552, 					/* Code */
					0, 					/* Vendor */
					"VLAN-ID-Range",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "S-VID-Start",	RULE_OPTIONAL, -1, 1 }
							,{  "S-VID-End",	RULE_OPTIONAL, -1, 1 }
							,{  "C-VID-Start",	RULE_OPTIONAL, -1, 1 }
							,{  "C-VID-End",	RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* ETH-SAP - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					551, 					/* Code */
					0, 					/* Vendor */
					"ETH-SAP",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* ETH-Ether-Type - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					550, 					/* Code */
					0, 					/* Vendor */
					"ETH-Ether-Type",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* ETH-Proto-Type - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					549, 					/* Code */
					0, 					/* Vendor */
					"ETH-Proto-Type",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "ETH-Ether-Type",	RULE_OPTIONAL, -1, -1 }
							,{  "ETH-SAP",		RULE_OPTIONAL, -1, -1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* ETH-Option - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					548, 					/* Code */
					0, 					/* Vendor */
					"ETH-Option",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "ETH-Proto-Type",		RULE_REQUIRED, -1, 1 }
							,{  "VLAN-ID-Range",		RULE_OPTIONAL, -1, -1 }
							,{  "User-Priority-Range",	RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Negated - RFC 5777 */
		{
			/*

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Negated)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "False",	{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "True", 	{ .i32 = 1 }};

			struct dict_avp_data 	data = {
					517, 					/* Code */
					0, 					/* Vendor */
					"Negated",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* ICMP-Code - RFC 5777 */
		{
			/*

			Here is what the RFC 5777 says :

			The ICMP-Code AVP (AVP Code 547) is of type Enumerated and the values
			are managed by IANA under the ICMP Type Numbers registry as defined
			in [RFC2780].

			The problem is (please refer to http://www.iana.org/assignments/icmp-parameters ) :
			Many of the ICMP types (AVP 456, see next AVP) have a "code" field (defined in this AVP).

			It means that the enum values defined in the current ICMP-Code AVP are not constants.

			We take the liberty of changing the Type of this AVP.

			NEW TYPE : Integer32

			*/

			struct dict_avp_data data = {
					547, 					/* Code */
					0, 					/* Vendor */
					"ICMP-Code", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_INTEGER32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* ICMP-Type-Number - RFC 5777 */
		{
			/*

			The ICMP-Type-Number AVP (AVP Code 546) is of type Enumerated and the
			values are managed by IANA under the ICMP Type Numbers registry as
			defined in [RFC2780].

			The RFC2780 then redirect us to the IANA.

			The Values have been copied from the following page :
			http://www.iana.org/assignments/icmp-parameters

			Last updated : 2010-04-22

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(ICMP-Type-Number)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0  = { "Echo Reply",			{ .i32 = 0 }};
			struct dict_enumval_data 	t_3  = { "Destination Unreachable", 	{ .i32 = 3 }};
			struct dict_enumval_data 	t_4  = { "Source Quench",		{ .i32 = 4 }};
			struct dict_enumval_data 	t_5  = { "Redirect",			{ .i32 = 5 }};
			struct dict_enumval_data 	t_6  = { "Alternate Host Address",	{ .i32 = 6 }};
			struct dict_enumval_data 	t_8  = { "Echo",			{ .i32 = 8 }};
			struct dict_enumval_data 	t_9  = { "Router Advertisement",	{ .i32 = 9 }};
			struct dict_enumval_data 	t_10 = { "Router Solicitation",		{ .i32 = 10 }};
			struct dict_enumval_data 	t_11 = { "Time Exceeded",		{ .i32 = 11 }};
			struct dict_enumval_data 	t_12 = { "Parameter Problem",		{ .i32 = 12 }};
			struct dict_enumval_data 	t_13 = { "Timestamp",			{ .i32 = 13 }};
			struct dict_enumval_data 	t_14 = { "Timestamp Reply",		{ .i32 = 14 }};
			struct dict_enumval_data 	t_15 = { "Information Request",		{ .i32 = 15 }};
			struct dict_enumval_data 	t_16 = { "Information Reply",		{ .i32 = 16 }};
			struct dict_enumval_data 	t_17 = { "Address Mask Request",	{ .i32 = 17 }};
			struct dict_enumval_data 	t_18 = { "Address Mask Reply",		{ .i32 = 18 }};
			struct dict_enumval_data 	t_30 = { "Traceroute",			{ .i32 = 30 }};
			struct dict_enumval_data 	t_31 = { "Datagram Conversion Error",	{ .i32 = 31 }};
			struct dict_enumval_data 	t_32 = { "Mobile Host Redirect",	{ .i32 = 32 }};
			struct dict_enumval_data 	t_33 = { "IPv6 Where-Are-You",		{ .i32 = 33 }};
			struct dict_enumval_data 	t_34 = { "IPv6 I-Am-Here",		{ .i32 = 34 }};
			struct dict_enumval_data 	t_35 = { "Mobile Registration Request",	{ .i32 = 35 }};
			struct dict_enumval_data 	t_36 = { "Mobile Registration Reply  ",	{ .i32 = 36 }};
			struct dict_enumval_data 	t_37 = { "Domain Name Request",		{ .i32 = 37 }};
			struct dict_enumval_data 	t_38 = { "Domain Name Reply",		{ .i32 = 38 }};
			struct dict_enumval_data 	t_39 = { "SKIP",			{ .i32 = 39 }};
			struct dict_enumval_data 	t_40 = { "Photuris",			{ .i32 = 40 }};
			struct dict_enumval_data 	t_41 = { "ICMP messages utilized by experimental mobility protocols such as Seamoby",{ .i32 = 41 }};

			struct dict_avp_data 	data = {
					546, 					/* Code */
					0, 					/* Vendor */
					"ICMP-Type-Number",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_8  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_9  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_10 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_11 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_12 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_13 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_14 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_15 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_16 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_17 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_18 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_30 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_31 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_32 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_33 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_34 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_35 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_36 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_37 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_38 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_39 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_40 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_41 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* ICMP-Type - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					545, 					/* Code */
					0, 					/* Vendor */
					"ICMP-Type",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "ICMP-Type-Number",	RULE_REQUIRED, -1, 1 }
							,{  "ICMP-Code",	RULE_OPTIONAL, -1, -1 }
							,{  "Negated",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Negated - RFC 5777 */

			/*
			Implemented higher
			*/

		/* TCP-Flag-Type - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					544, 					/* Code */
					0, 					/* Vendor */
					"TCP-Flag-Type",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* TCP-Flags - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					543, 					/* Code */
					0, 					/* Vendor */
					"TCP-Flags",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "TCP-Flag-Type",	RULE_REQUIRED, -1, 1 }
							,{  "Negated",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* TCP-Option-Type - RFC 5777 */
		{
			/*
			The TCP-Option-Type AVP (AVP Code 541) is of type Enumerated and the
			values are managed by IANA under the TCP Option Numbers registry as
			defined in [RFC2780].

			The RFC2780 then redirect us to the IANA.

			The Values have been copied from the following page :
			http://www.iana.org/assignments/tcp-parameters/tcp-parameters.txt

			Last updated : 2010-06-22

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(TCP-Option-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0   = { "End of Option List",			{ .i32 = 0  }};
			struct dict_enumval_data 	t_1   = { "No-Operation",			{ .i32 = 1  }};
			struct dict_enumval_data 	t_2   = { "Maximum Segment Size",		{ .i32 = 2  }};
			struct dict_enumval_data 	t_3   = { "WSOPT - Window Scale",		{ .i32 = 3  }};
			struct dict_enumval_data 	t_4   = { "SACK Permitted",			{ .i32 = 4  }};
			struct dict_enumval_data 	t_5   = { "SACK",				{ .i32 = 5  }};
			struct dict_enumval_data 	t_6   = { "Echo",				{ .i32 = 6  }};
			struct dict_enumval_data 	t_7   = { "Echo Reply",				{ .i32 = 7  }};
			struct dict_enumval_data 	t_8   = { "TSOPT - Time Stamp Option",		{ .i32 = 8  }};
			struct dict_enumval_data 	t_9   = { "Partial Order Connection Permitted",	{ .i32 = 9  }};
			struct dict_enumval_data 	t_10  = { "Partial Order Service Profile",	{ .i32 = 10 }};
			struct dict_enumval_data 	t_11  = { "CC",					{ .i32 = 11 }};
			struct dict_enumval_data 	t_12  = { "CC.NEW",				{ .i32 = 12 }};
			struct dict_enumval_data 	t_13  = { "CC.ECHO",				{ .i32 = 13 }};
			struct dict_enumval_data 	t_14  = { "TCP Alternate Checksum Request",	{ .i32 = 14 }};
			struct dict_enumval_data 	t_15  = { "TCP Alternate Checksum Data",	{ .i32 = 15 }};
			struct dict_enumval_data 	t_16  = { "Skeeter",				{ .i32 = 16 }};
			struct dict_enumval_data 	t_17  = { "Bubba",				{ .i32 = 17 }};
			struct dict_enumval_data 	t_18  = { "Trailer Checksum Option",		{ .i32 = 18 }};
			struct dict_enumval_data 	t_19  = { "MD5 Signature Option",		{ .i32 = 19 }};
			struct dict_enumval_data 	t_20  = { "SCPS Capabilities",			{ .i32 = 20 }};
			struct dict_enumval_data 	t_21  = { "Selective Negative Acknowledgements",{ .i32 = 21 }};
			struct dict_enumval_data 	t_22  = { "Record Boundaries",			{ .i32 = 22 }};
			struct dict_enumval_data 	t_23  = { "Corruption experienced",		{ .i32 = 23 }};
			struct dict_enumval_data 	t_24  = { "SNAP",				{ .i32 = 24 }};
			struct dict_enumval_data 	t_26  = { "TCP Compression Filter",		{ .i32 = 26 }};
			struct dict_enumval_data 	t_27  = { "Quick-Start Response",		{ .i32 = 27 }};
			struct dict_enumval_data 	t_28  = { "User Timeout Option",		{ .i32 = 28 }};
			struct dict_enumval_data 	t_29  = { "TCP Authentication Option (TCP-AO)",	{ .i32 = 29 }};

			struct dict_avp_data 	data = {
					541, 					/* Code */
					0, 					/* Vendor */
					"TCP-Option-Type",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_7  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_8  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_9  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_10 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_11 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_12 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_13 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_14 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_15 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_16 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_17 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_18 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_19 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_20 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_21 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_22 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_23 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_24 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_26 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_27 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_28 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_29 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* TCP-Option-Value - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					542, 					/* Code */
					0, 					/* Vendor */
					"TCP-Option-Value",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Negated - RFC 5777 */

			/*
			Implemented higher
			*/

		/* TCP-Option - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					540, 					/* Code */
					0, 					/* Vendor */
					"TCP-Option",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "TCP-Option-Type",	RULE_REQUIRED, -1, 1 }
							,{  "TCP-Option-Value",	RULE_OPTIONAL, -1, -1 }
							,{  "Negated",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* IP-Option-Type - RFC 5777 */
		{
			/*

			The IP-Option-Type AVP (AVP Code 538) is of type Enumerated and the
			values are managed by IANA under the IP Option Numbers registry as
			defined in [RFC2780].

			The RFC2780 then redirect us to the IANA.

			The Values have been copied from the following page :
			http://www.iana.org/assignments/ip-parameters

			Last updated : 2007-02-15

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(IP-Option-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0   = { "End of Options List    [RFC791,JBP]",	{ .i32 = 0   }};
			struct dict_enumval_data 	t_1   = { "No Operation           [RFC791,JBP]", 	{ .i32 = 1   }};
			struct dict_enumval_data 	t_2   = { "Security                  [RFC1108]", 	{ .i32 = 2   }};
			struct dict_enumval_data 	t_3   = { "Loose Source Route     [RFC791,JBP]", 	{ .i32 = 3   }};
			struct dict_enumval_data 	t_4   = { "Time Stamp             [RFC791,JBP]", 	{ .i32 = 4   }};
			struct dict_enumval_data 	t_5   = { "Extended Security         [RFC1108]", 	{ .i32 = 5   }};
			struct dict_enumval_data 	t_6   = { "Commercial Security           [???]", 	{ .i32 = 6   }};
			struct dict_enumval_data 	t_7   = { "Record Route           [RFC791,JBP]", 	{ .i32 = 7   }};
			struct dict_enumval_data 	t_8   = { "Stream ID              [RFC791,JBP]", 	{ .i32 = 8   }};
			struct dict_enumval_data 	t_9   = { "Strict Source Route    [RFC791,JBP]", 	{ .i32 = 9   }};
			struct dict_enumval_data 	t_10  = { "Experimental Measurement      [ZSu]", 	{ .i32 = 10  }};
			struct dict_enumval_data 	t_11  = { "MTU Probe                 [RFC1191]*", 	{ .i32 = 11  }};
			struct dict_enumval_data 	t_12  = { "MTU Reply                 [RFC1191]*", 	{ .i32 = 12  }};
			struct dict_enumval_data 	t_13  = { "Experimental Flow Control    [Finn]", 	{ .i32 = 13  }};
			struct dict_enumval_data 	t_14  = { "Expermental Access Control [Estrin]", 	{ .i32 = 14  }};
			struct dict_enumval_data 	t_15  = { "???                      [VerSteeg]", 	{ .i32 = 15  }};
			struct dict_enumval_data 	t_16  = { "IMI Traffic Descriptor        [Lee]", 	{ .i32 = 16  }};
			struct dict_enumval_data 	t_17  = { "Extended Internet Protocol[RFC1385]", 	{ .i32 = 17  }};
			struct dict_enumval_data 	t_18  = { "Traceroute		   [RFC1393]", 		{ .i32 = 18  }};
			struct dict_enumval_data 	t_19  = { "Address Extension    [Ullmann IPv7]", 	{ .i32 = 19  }};
			struct dict_enumval_data 	t_20  = { "Router Alert              [RFC2113]", 	{ .i32 = 20  }};
			struct dict_enumval_data 	t_21  = { "Selective Directed Broadcast[Graff]", 	{ .i32 = 21  }};
			struct dict_enumval_data 	t_23  = { "Dynamic Packet State        [Malis]", 	{ .i32 = 23  }};
			struct dict_enumval_data 	t_24  = { "Upstream Multicast Pkt. [Farinacci]", 	{ .i32 = 24  }};
			struct dict_enumval_data 	t_25  = { "Quick-Start 		   [RFC4782]", 		{ .i32 = 25  }};

			struct dict_avp_data 	data = {
					538, 					/* Code */
					0, 					/* Vendor */
					"IP-Option-Type",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_7  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_8  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_9  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_10 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_11 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_12 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_13 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_14 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_15 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_16 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_17 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_18 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_19 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_20 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_21 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_23 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_24 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_25 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* IP-Option-Value - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					539, 					/* Code */
					0, 					/* Vendor */
					"IP-Option-Value",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Negated - RFC 5777 */

			/*
			Implemented higher
			*/

		/* IP-Option - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					537, 					/* Code */
					0, 					/* Vendor */
					"IP-Option",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "IP-Option-Type",	RULE_REQUIRED, -1, 1 }
							,{  "IP-Option-Value",	RULE_OPTIONAL, -1, -1 }
							,{  "Negated",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Fragmentation-Flag - RFC 5777 */
		{
			/*

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Fragmentation-Flag)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "DF",	{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "MF", 	{ .i32 = 1 }};

			struct dict_avp_data 	data = {
					536, 					/* Code */
					0, 					/* Vendor */
					"Fragmentation-Flag",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* Diffserv-Code-Point - RFC 5777 */
		{
			/*
			The Diffserv-Code-Point AVP (AVP Code 535) is of type Enumerated and
			specifies the Differentiated Services Field Codepoints to match in
			the IP header.  The values are managed by IANA under the
			Differentiated Services Field Codepoints registry as defined in
			[RFC2474].

			The RFC2474 then redirect us to the IANA.

			The Values have been copied from the following page :
			http://www.iana.org/assignments/dscp-registry/dscp-registry.txt

			Last updated in code : 2010-05-11

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Diffserv-Code-Point)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0  = { "CS0",		{ .i32 = 0 }};
			struct dict_enumval_data 	t_8  = { "CS1",		{ .i32 = 8 }};
			struct dict_enumval_data 	t_16 = { "CS2",		{ .i32 = 16 }};
			struct dict_enumval_data 	t_24 = { "CS3",		{ .i32 = 24 }};
			struct dict_enumval_data 	t_32 = { "CS4",		{ .i32 = 32 }};
			struct dict_enumval_data 	t_40 = { "CS5",		{ .i32 = 40 }};
			struct dict_enumval_data 	t_48 = { "CS6",		{ .i32 = 48 }};
			struct dict_enumval_data 	t_56 = { "CS7",		{ .i32 = 56 }};
			struct dict_enumval_data 	t_10 = { "AF11",	{ .i32 = 10 }};
			struct dict_enumval_data 	t_12 = { "AF12",	{ .i32 = 12 }};
			struct dict_enumval_data 	t_14 = { "AF13",	{ .i32 = 14 }};
			struct dict_enumval_data 	t_18 = { "AF21",	{ .i32 = 18 }};
			struct dict_enumval_data 	t_20 = { "AF22",	{ .i32 = 20 }};
			struct dict_enumval_data 	t_22 = { "AF23",	{ .i32 = 22 }};
			struct dict_enumval_data 	t_26 = { "AF31",	{ .i32 = 26 }};
			struct dict_enumval_data 	t_28 = { "AF32",	{ .i32 = 28 }};
			struct dict_enumval_data 	t_30 = { "AF33",	{ .i32 = 30 }};
			struct dict_enumval_data 	t_34 = { "AF41",	{ .i32 = 34 }};
			struct dict_enumval_data 	t_36 = { "AF42",	{ .i32 = 36 }};
			struct dict_enumval_data 	t_38 = { "AF43",	{ .i32 = 38 }};
			struct dict_enumval_data 	t_46 = { "EF PHB",	{ .i32 = 46 }};
			struct dict_enumval_data 	t_44 = { "VOICE-ADMIT",	{ .i32 = 44 }};

			struct dict_avp_data 	data = {
					535, 					/* Code */
					0, 					/* Vendor */
					"Diffserv-Code-Point",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_8  , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_16 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_24 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_32 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_40 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_48 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_56 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_10 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_12 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_14 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_18 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_20 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_22 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_26 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_28 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_30 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_34 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_36 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_38 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_46 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_44 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* IP-Address - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					518, 					/* Code */
					0, 					/* Vendor */
					"IP-Address",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* IP-Address-Start - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					520, 					/* Code */
					0, 					/* Vendor */
					"IP-Address-Start",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* IP-Address-End - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					521, 					/* Code */
					0, 					/* Vendor */
					"IP-Address-End",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}

		/* IP-Address-Range - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					519, 					/* Code */
					0, 					/* Vendor */
					"IP-Address-Range",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "IP-Address-Start",	RULE_OPTIONAL, -1, 1 }
							,{  "IP-Address-End",	RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* IP-Address - RFC 5777 */

			/*
			Implemented higher
			*/

		/* IP-Mask-Bit-Mask-Width - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					523, 					/* Code */
					0, 					/* Vendor */
					"IP-Mask-Bit-Mask-Width",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* IP-Address-Mask - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					522, 					/* Code */
					0, 					/* Vendor */
					"IP-Address-Mask",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "IP-Address",		RULE_REQUIRED, -1, 1 }
							,{  "IP-Mask-Bit-Mask-Width",	RULE_REQUIRED, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* MAC-Address - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					524, 					/* Code */
					0, 					/* Vendor */
					"MAC-Address",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MAC-Address - RFC 5777 */

			/*
			Implemented higher
			*/

		/* MAC-Address-Mask-Pattern - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					526, 					/* Code */
					0, 					/* Vendor */
					"MAC-Address-Mask-Pattern",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* MAC-Address-Mask - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					525, 					/* Code */
					0, 					/* Vendor */
					"MAC-Address-Mask",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "MAC-Address",		RULE_REQUIRED, -1, 1 }
							,{  "MAC-Address-Mask-Pattern",	RULE_REQUIRED, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* EUI64-Address - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					527, 					/* Code */
					0, 					/* Vendor */
					"EUI64-Address",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* EUI64-Address - RFC 5777 */

			/*
			Implemented higher
			*/

		/* EUI64-Address-Mask-Pattern - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					529, 					/* Code */
					0, 					/* Vendor */
					"EUI64-Address-Mask-Pattern",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* EUI64-Address-Mask - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					528, 					/* Code */
					0, 					/* Vendor */
					"EUI64-Address-Mask",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "EUI64-Address",			RULE_REQUIRED, -1, 1 }
							,{  "EUI64-Address-Mask-Pattern",	RULE_REQUIRED, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Port - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					530, 					/* Code */
					0, 					/* Vendor */
					"Port",					/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_INTEGER32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Port-Start - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					532, 					/* Code */
					0, 					/* Vendor */
					"Port-Start",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_INTEGER32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Port-End - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					533, 					/* Code */
					0, 					/* Vendor */
					"Port-End",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_INTEGER32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Port-Range - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					531, 					/* Code */
					0, 					/* Vendor */
					"Port-Range",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "Port-Start",	RULE_OPTIONAL, -1, 1 }
							,{  "Port-End",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Negated - RFC 5777 */

			/*
			Implemented higher
			*/

		/* Use-Assigned-Address - RFC 5777 */
		{
			/*

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Use-Assigned-Address)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "False",	{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "True", 	{ .i32 = 1 }};

			struct dict_avp_data 	data = {
					534, 					/* Code */
					0, 					/* Vendor */
					"Use-Assigned-Address",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* From-Spec - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					515, 					/* Code */
					0, 					/* Vendor */
					"From-Spec",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "IP-Address",		RULE_OPTIONAL, -1, -1 }
							,{  "IP-Address-Range",		RULE_OPTIONAL, -1, -1 }
							,{  "IP-Address-Mask",		RULE_OPTIONAL, -1, -1 }
							,{  "MAC-Address",		RULE_OPTIONAL, -1, -1 }
							,{  "MAC-Address-Mask",		RULE_OPTIONAL, -1, -1 }
							,{  "EUI64-Address",		RULE_OPTIONAL, -1, -1 }
							,{  "EUI64-Address-Mask",	RULE_OPTIONAL, -1, -1 }
							,{  "Port",			RULE_OPTIONAL, -1, -1 }
							,{  "Port-Range",		RULE_OPTIONAL, -1, -1 }
							,{  "Negated",			RULE_OPTIONAL, -1, 1 }
							,{  "Use-Assigned-Address",	RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* To-Spec - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					516, 					/* Code */
					0, 					/* Vendor */
					"To-Spec",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "IP-Address",		RULE_OPTIONAL, -1, -1 }
							,{  "IP-Address-Range",		RULE_OPTIONAL, -1, -1 }
							,{  "IP-Address-Mask",		RULE_OPTIONAL, -1, -1 }
							,{  "MAC-Address",		RULE_OPTIONAL, -1, -1 }
							,{  "MAC-Address-Mask",		RULE_OPTIONAL, -1, -1 }
							,{  "EUI64-Address",		RULE_OPTIONAL, -1, -1 }
							,{  "EUI64-Address-Mask",	RULE_OPTIONAL, -1, -1 }
							,{  "Port",			RULE_OPTIONAL, -1, -1 }
							,{  "Port-Range",		RULE_OPTIONAL, -1, -1 }
							,{  "Negated",			RULE_OPTIONAL, -1, 1 }
							,{  "Use-Assigned-Address",	RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Direction - RFC 5777 */
		{
			/*

			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Direction)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "IN",	{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "OUT", 	{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "BOTH", { .i32 = 2 }};

			struct dict_avp_data 	data = {
					514, 					/* Code */
					0, 					/* Vendor */
					"Direction",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* Protocol - RFC 5777 */
		{
			/*
			The Protocol AVP (AVP Code 513) is of type Enumerated and specifies
			the protocol being matched.  The attributes included in the
			Classifier AVP MUST be consistent with the value of the Protocol AVP.
			Exactly zero or one Protocol AVP may be contained within a Classifier
			AVP.  If the Protocol AVP is omitted from the classifier, then
			comparison of the protocol of the packet is irrelevant.  The values
			for this AVP are managed by IANA under the Protocol Numbers registry
			as defined in [RFC3643].

			The RFC3643 then redirects us to the IANA.

			The Values have been copied from the following page :
			http://www.iana.org/assignments/ips-protocols

			Last updated : 2004-01-26
			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Protocol)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "Reserved",						{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "FCIP (Fibre Channel Over TCP/IP)", 			{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "iFCP (A Protocol for Internet Fibre Channel)", 	{ .i32 = 2 }};

			struct dict_avp_data 	data = {
					513, 					/* Code */
					0, 					/* Vendor */
					"Protocol",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}

		/* Classifier-ID - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					512, 					/* Code */
					0, 					/* Vendor */
					"Classifier-ID",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Classifier - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					511, 					/* Code */
					0, 					/* Vendor */
					"Classifier",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "Classifier-ID",		RULE_REQUIRED, -1, 1 }
							,{  "Protocol",			RULE_OPTIONAL, -1, 1 }
							,{  "Direction",		RULE_OPTIONAL, -1, 1 }
							,{  "From-Spec",		RULE_OPTIONAL, -1, -1 }
							,{  "To-Spec",			RULE_OPTIONAL, -1, -1 }
							,{  "Diffserv-Code-Point",	RULE_OPTIONAL, -1, -1 }
							,{  "Fragmentation-Flag",	RULE_OPTIONAL, -1, 1 }
							,{  "IP-Option",		RULE_OPTIONAL, -1, -1 }
							,{  "TCP-Option",		RULE_OPTIONAL, -1, -1 }
							,{  "TCP-Flags",		RULE_OPTIONAL, -1, 1 }
							,{  "ICMP-Type",		RULE_OPTIONAL, -1, -1 }
							,{  "ETH-Option",		RULE_OPTIONAL, -1, -1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* Filter-Rule-Precedence - RFC 5777 */
		{
			/*

			*/

			struct dict_avp_data data = {
					510, 					/* Code */
					0, 					/* Vendor */
					"Filter-Rule-Precedence",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32			/* base type of data */
					};

			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}

		/* Filter-Rule - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					509, 					/* Code */
					0, 					/* Vendor */
					"Filter-Rule",				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "Filter-Rule-Precedence",	RULE_OPTIONAL, -1, 1 }
							,{  "Classifier",		RULE_OPTIONAL, -1, 1 }
							,{  "Time-Of-Day-Condition",	RULE_OPTIONAL, -1, -1 }
							,{  "Treatment-Action",		RULE_OPTIONAL, -1, 1 }
							,{  "QoS-Semantics",		RULE_OPTIONAL, -1, 1 }
							,{  "QoS-Profile-Template",	RULE_OPTIONAL, -1, 1 }
							,{  "QoS-Parameters",		RULE_OPTIONAL, -1, 1 }
							,{  "Excess-Treatment",		RULE_OPTIONAL, -1, 1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}

		/* QoS-Resources - RFC 5777 */
		{
			/*

			*/
			struct dict_object * avp;
			struct dict_avp_data data = {
					508, 					/* Code */
					0, 					/* Vendor */
					"QoS-Resources",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};

			struct local_rules_definition rules[] =
						{ 	 {  "Filter-Rule", 		RULE_REQUIRED, -1, -1 }
						};

			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
	}

	LOG_D( "Dictionary Extension 'Traffic Classification and Quality of Service (QoS) Attributes for Diameter (RFC 5777)' initialized");
	return 0;
}
EXTENSION_ENTRY("dict_rfc5777", dict_rfc5777_init);
