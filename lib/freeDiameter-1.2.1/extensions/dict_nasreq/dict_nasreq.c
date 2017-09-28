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
 * Dictionary definitions of objects specified in Diameter NASREQ (RFC4005).
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


static int dnr_entry(char * conffile)
{
	struct dict_object * nasreq;
	TRACE_ENTRY("%p", conffile);
	
	/* No Vendors definitions */
	
	/* Applications section */
	{
		/* NASREQ (RFC 4005) */
		{
			struct dict_application_data data = {        1, "Diameter Network Access Server Application" 			};
			CHECK_dict_new( DICT_APPLICATION, &data, NULL, &nasreq);
		}
	}
	
	/* Derived AVP types section */
	{
		/* QoSFilterRule */
		{
			/*
				The QosFilterRule format is derived from the OctetString AVP Base
				Format.  It uses the ASCII charset.  Packets may be marked or
				metered based on the following information:

				 Direction                          (in or out)
				 Source and destination IP address  (possibly masked)
				 Protocol
				 Source and destination port        (lists or ranges)
				 DSCP values                        (no mask or range)

				Rules for the appropriate direction are evaluated in order; the
				first matched rule terminates the evaluation.  Each packet is
				evaluated once.  If no rule matches, the packet is treated as best
				effort.  An access device unable to interpret or apply a QoS rule
				SHOULD NOT terminate the session.

				QoSFilterRule filters MUST follow the following format:

				action dir proto from src to dst [options]

        				tag    - Mark packet with a specific DSCP
                				 [DIFFSERV].  The DSCP option MUST be
                				 included.
        				meter  - Meter traffic.  The metering options
                				 MUST be included.

				dir           The format is as described under IPFilterRule.

				proto         The format is as described under IPFilterRule.

				src and dst   The format is as described under IPFilterRule.

				 options:

				 DSCP <color>
				       Color values as defined in [DIFFSERV].  Exact
				       matching of DSCP values is required (no masks or
				       ranges).

				 metering <rate> <color_under> <color_over>
				       The metering option provides Assured Forwarding,
				       as defined in [DIFFSERVAF], and MUST be present
				       if the action is set to meter.  The rate option is
				       the throughput, in bits per second, used
				       by the access device to mark packets.  Traffic
				       over the rate is marked with the color_over
				       codepoint, and traffic under the rate is marked
				       with the color_under codepoint.  The color_under
				       and color_over options contain the drop
				       preferences and MUST conform to the recommended
				       codepoint keywords described in [DIFFSERVAF]
				       (e.g., AF13).

				       The metering option also supports the strict
				       limit on traffic required by Expedited
				       Forwarding, as defined in [DIFFSERVEF].  The
				       color_over option may contain the keyword "drop"
				       to prevent forwarding of traffic that exceeds the
				       rate parameter.

				 The rule syntax is a modified subset of ipfw(8) from FreeBSD,
				 and the ipfw.c code may provide a useful base for
				 implementations.
			*/
			struct dict_type_data data = { AVP_TYPE_OCTETSTRING,	"QoSFilterRule"		, NULL			, NULL			};
			CHECK_dict_new( DICT_TYPE, &data , NULL, NULL);
		}
		
	}
	
	/* AVP section */
	{
		struct dict_object * UTF8String_type;
		struct dict_object * IPFilterRule_type;
		struct dict_object * QoSFilterRule_type;
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "IPFilterRule", &IPFilterRule_type);
		CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "QoSFilterRule", &QoSFilterRule_type);

/********************************
 *  NAS Session AVPs            *
 ********************************/		
		/* NAS-Port */
		{
			/*
				The NAS-Port AVP (AVP Code 5) is of type Unsigned32 and contains the
				physical or virtual port number of the NAS which is authenticating
				the user.  Note that "port" is meant in its sense as a service
				connection on the NAS, not as an IP protocol identifier.

				Either NAS-Port or NAS-Port-Id (AVP Code 87) SHOULD be present in
				AA-Request (AAR) commands if the NAS differentiates among its ports.
			*/
			struct dict_avp_data data = { 
					5, 					/* Code */
					0, 					/* Vendor */
					"NAS-Port", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* NAS-Port-Id */
		{
			/*
				The NAS-Port-Id AVP (AVP Code 87) is of type UTF8String and consists
				of ASCII text identifying the port of the NAS authenticating the
				user.  Note that "port" is meant in its sense as a service connection
				on the NAS, not as an IP protocol identifier.

				Either NAS-Port or NAS-Port-Id SHOULD be present in AA-Request (AAR)
				commands if the NAS differentiates among its ports.  NAS-Port-Id is
				intended for use by NASes that cannot conveniently number their
				ports.
			*/
			struct dict_avp_data data = { 
					87, 					/* Code */
					0, 					/* Vendor */
					"NAS-Port-Id", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* NAS-Port-Type */
		{
			/*
				The NAS-Port-Type AVP (AVP Code 61) is of type Enumerated and
				contains the type of the port on which the NAS is authenticating the
				user.  This AVP SHOULD be present if the NAS uses the same NAS-Port
				number ranges for different service types concurrently.

				The supported values are defined in [RADIUSTypes].  The following
				list is informational and subject to change by the IANA.
				
				http://www.iana.org/assignments/radius-types
				Sub-registry: Values for RADIUS Attribute 61, NAS-Port-Type
				Reference: [RFC2865]
				
				Extract on 2009.06.01:
				0      Async                                                               [RFC2865]
				1      Sync                                                                [RFC2865]
				2      ISDN Sync                                                           [RFC2865]
				3      ISDN Async V.120                                                    [RFC2865]
				4      ISDN Async V.110                                                    [RFC2865]
				5      Virtual                                                             [RFC2865]
				6      PIAFS                                                               [RFC2865]
				7      HDLC Clear Channel                                                  [RFC2865]
				8      X.25                                                                [RFC2865]
				9      X.75                                                                [RFC2865]
				10     G.3 Fax                                                             [RFC2865]
				11     SDSL - Symmetric DSL                                                [RFC2865]
				12     ADSL-CAP - Asymmetric DSL, Carrierless Amplitude Phase Modulation   [RFC2865]
				13     ADSL-DMT - Asymmetric DSL, Discrete Multi-Tone                      [RFC2865]
				14     IDSL - ISDN Digital Subscriber Line                                 [RFC2865]
				15     Ethernet                                                            [RFC2865]
				16     xDSL - Digital Subscriber Line of unknown type                      [RFC2865]
				17     Cable                                                               [RFC2865]
				18     Wireless - Other                                                    [RFC2865]
				19     Wireless - IEEE 802.11                                              [RFC2865]
				20     Token-Ring                                                          [RFC3580] 
				21     FDDI                                                                [RFC3580]
				22     Wireless - CDMA2000                                                 [McCann] 
				23     Wireless - UMTS                                                     [McCann]
				24     Wireless - 1X-EV                                                    [McCann]
				25     IAPP                                                                [IEEE 802.11F][Kerry]   
				26     FTTP - Fiber to the Premises                                        [Nyce]
				27     Wireless - IEEE 802.16                                              [IEEE 802.16]          12 December 2006
				28     Wireless - IEEE 802.20                                              [IEEE 802.20]          12 December 2006
				29     Wireless - IEEE 802.22                                              [IEEE 802.22]          12 December 2006		
				30     PPPoA - PPP over ATM                                                [RFC4603]
				31     PPPoEoA - PPP over Ethernet over ATM                                [RFC4603]
				32     PPPoEoE - PPP over Ethernet over Ethernet                           [RFC4603]
				33     PPPoEoVLAN - PPP over Ethernet over VLAN                            [RFC4603]
				34     PPPoEoQinQ - PPP over Ethernet over IEEE 802.1QinQ                  [RFC4603]
				35     xPON - Passive Optical Network                                      [Hublet][Yan]           19 June 2007
			*/
			
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(NAS-Port-Type)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "Async [RFC2865]"),
				enumval_def_u32( 1, "Sync [RFC2865]"),
				enumval_def_u32( 2, "ISDN Sync [RFC2865]"),
				enumval_def_u32( 3, "ISDN Async V.120 [RFC2865]"),
				enumval_def_u32( 4, "ISDN Async V.110 [RFC2865]"),
				enumval_def_u32( 5, "Virtual [RFC2865]"),
				enumval_def_u32( 6, "PIAFS [RFC2865]"),
				enumval_def_u32( 7, "HDLC Clear Channel [RFC2865]"),
				enumval_def_u32( 8, "X.25 [RFC2865]"),
				enumval_def_u32( 9, "X.75 [RFC2865]"),
				enumval_def_u32(10, "G.3 Fax [RFC2865]"),
				enumval_def_u32(11, "SDSL - Symmetric DSL [RFC2865]"),
				enumval_def_u32(12, "ADSL-CAP - Asymmetric DSL, Carrierless Amplitude Phase Modulation [RFC2865]"),
				enumval_def_u32(13, "ADSL-DMT - Asymmetric DSL, Discrete Multi-Tone [RFC2865]"),
				enumval_def_u32(14, "IDSL - ISDN Digital Subscriber Line [RFC2865]"),
				enumval_def_u32(15, "Ethernet [RFC2865]"),
				enumval_def_u32(16, "xDSL - Digital Subscriber Line of unknown type [RFC2865]"),
				enumval_def_u32(17, "Cable [RFC2865]"),
				enumval_def_u32(18, "Wireless - Other [RFC2865]"),
				enumval_def_u32(19, "Wireless - IEEE 802.11 [RFC2865]"),
				enumval_def_u32(20, "Token-Ring [RFC3580]"),
				enumval_def_u32(21, "FDDI [RFC3580]"),
				enumval_def_u32(22, "Wireless - CDMA2000 [McCann]"),
				enumval_def_u32(23, "Wireless - UMTS [McCann]"),
				enumval_def_u32(24, "Wireless - 1X-EV [McCann]"),
				enumval_def_u32(25, "IAPP [IEEE 802.11F][Kerry]"),
				enumval_def_u32(26, "FTTP - Fiber to the Premises [Nyce]"),
				enumval_def_u32(27, "Wireless - IEEE 802.16 [IEEE 802.16]"),
				enumval_def_u32(28, "Wireless - IEEE 802.20 [IEEE 802.20]"),
				enumval_def_u32(29, "Wireless - IEEE 802.22 [IEEE 802.22]"),
				enumval_def_u32(30, "PPPoA - PPP over ATM [RFC4603]"),
				enumval_def_u32(31, "PPPoEoA - PPP over Ethernet over ATM [RFC4603]"),
				enumval_def_u32(32, "PPPoEoE - PPP over Ethernet over Ethernet [RFC4603]"),
				enumval_def_u32(33, "PPPoEoVLAN - PPP over Ethernet over VLAN [RFC4603]"),
				enumval_def_u32(34, "PPPoEoQinQ - PPP over Ethernet over IEEE 802.1QinQ [RFC4603]"),
				enumval_def_u32(35, "xPON - Passive Optical Network [Hublet][Yan]")
			};
			struct dict_avp_data 	data = { 
					61, 					/* Code */
					0, 					/* Vendor */
					"NAS-Port-Type", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
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
		
		/* Called-Station-Id */
		{
			/*
				The Called-Station-Id AVP (AVP Code 30) is of type UTF8String and
				allows the NAS to send the ASCII string describing the layer 2
				address the user contacted in the request.  For dialup access, this
				can be a phone number obtained by using Dialed Number Identification
				(DNIS) or a similar technology.  Note that this may be different from
				the phone number the call comes in on.  For use with IEEE 802 access,
				the Called-Station-Id MAY contain a MAC address formatted as
				described in [RAD802.1X].  It SHOULD only be present in
				authentication and/or authorization requests.

				If the Auth-Request-Type AVP is set to authorization-only and the
				User-Name AVP is absent, the Diameter Server MAY perform
				authorization based on this field.  This can be used by a NAS to
				request whether a call should be answered based on the DNIS.
			*/
			struct dict_avp_data data = { 
					30, 					/* Code */
					0, 					/* Vendor */
					"Called-Station-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Calling-Station-Id */
		{
			/*
				The Calling-Station-Id AVP (AVP Code 31) is of type UTF8String and
				allows the NAS to send the ASCII string describing the layer 2
				address from which the user connected in the request.  For dialup
				access, this is the phone number the call came from, using Automatic
				Number Identification (ANI) or a similar technology.  For use with
				IEEE 802 access, the Calling-Station-Id AVP MAY contain a MAC
				address, formated as described in [RAD802.1X].  It SHOULD only be
				present in authentication and/or authorization requests.

				If the Auth-Request-Type AVP is set to authorization-only and the
				User-Name AVP is absent, the Diameter Server MAY perform
				authorization based on this field.  This can be used by a NAS to
				request whether a call should be answered based on the layer 2
				address (ANI, MAC Address, etc.)
			*/
			struct dict_avp_data data = { 
					31, 					/* Code */
					0, 					/* Vendor */
					"Calling-Station-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Connect-Info */
		{
			/*
				The Connect-Info AVP (AVP Code 77) is of type UTF8String and is sent
				in the AA-Request message or ACR STOP message.  When sent in the
				Access-Request, it indicates the nature of the user's connection.
				The connection speed SHOULD be included at the beginning of the first
				Connect-Info AVP in the message.  If the transmit and receive
				connection speeds differ, both may be included in the first AVP with
				the transmit speed listed first (the speed the NAS modem transmits
				at), then a slash (/), then the receive speed, and then other
				optional information.

				For example: "28800 V42BIS/LAPM" or "52000/31200 V90"

				More than one Connect-Info attribute may be present in an
				Accounting-Request packet to accommodate expected efforts by the ITU
				to have modems report more connection information in a standard
				format that might exceed 252 octets.

				If sent in the ACR STOP, this attribute may summarize statistics
				relating to session quality.  For example, in IEEE 802.11, the
				Connect-Info attribute may contain information on the number of link
				layer retransmissions.  The exact format of this attribute is
				implementation specific.
			*/
			struct dict_avp_data data = { 
					77, 					/* Code */
					0, 					/* Vendor */
					"Connect-Info", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Originating-Line-Info */
		{
			/*
				The Originating-Line-Info AVP (AVP Code 94) is of type OctetString
				and is sent by the NAS system to convey information about the origin
				of the call from an SS7 system.

				The originating line information (OLI) element indicates the nature
				and/or characteristics of the line from which a call originated
				(e.g., pay phone, hotel, cellular).  Telephone companies are starting
				to offer OLI to their customers as an option over Primary Rate
				Interface (PRI).  Internet Service Providers (ISPs) can use OLI in
				addition to Called-Station-Id and Calling-Station-Id attributes to
				differentiate customer calls and to define different services.

				The Value field contains two octets (00 - 99).  ANSI T1.113 and
				BELLCORE 394 can be used for additional information about these
				values and their use.  For more information on current assignment
				values, see [http://www.nanpa.com/number_resource_info/ani_ii_assignments.html].

			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_OCTETSTRING,	"Enumerated*(Originating-Line-Info)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_os( 2, "00", "Plain Old Telephone Service (POTS)"),
				enumval_def_os( 2, "01", "Multiparty Line (more than 2)"),
				enumval_def_os( 2, "02", "ANI Failure"),
				enumval_def_os( 2, "06", "Station Level Rating"),
				enumval_def_os( 2, "07", "Special Operator Handling Required"),
				enumval_def_os( 2, "20", "Automatic Identified Outward Dialing (AIOD)"),
				enumval_def_os( 2, "23", "Coin or Non-Coin"),
				enumval_def_os( 2, "24", "Toll Free Service (Non-Pay Origination)"),
				enumval_def_os( 2, "25", "Toll Free Service (Pay Origination)"),
				enumval_def_os( 2, "27", "Toll Free Service (Coin Control Origination)"),
				enumval_def_os( 2, "29", "Prison/Inmate Service"),
				enumval_def_os( 2, "30", "Intercept (Blank)"),
				enumval_def_os( 2, "31", "Intercept (Trouble)"),
				enumval_def_os( 2, "32", "Intercept (Regular)"),
				enumval_def_os( 2, "34", "Telco Operator Handled Call"),
				enumval_def_os( 2, "52", "Outward Wide Area Telecommunications Service (OUTWATS)"),
				enumval_def_os( 2, "60", "Telecommunications Relay Service (TRS)(Unrestricted)"),
				enumval_def_os( 2, "61", "Cellular/Wireless PCS (Type 1)"),
				enumval_def_os( 2, "62", "Cellular/Wireless PCS (Type 2)"),
				enumval_def_os( 2, "63", "Cellular/Wireless PCS (Roaming)"),
				enumval_def_os( 2, "66", "TRS (Hotel)"),
				enumval_def_os( 2, "67", "TRS (Restricted)"),
				enumval_def_os( 2, "70", "Pay Station, No Coin Control"),
				enumval_def_os( 2, "93", "Access for Private Virtual Network Service")
			};
			struct dict_avp_data data = { 
					94, 					/* Code */
					0, 					/* Vendor */
					"Originating-Line-Info", 		/* Name */
					AVP_FLAG_VENDOR, 			/* Fixed flags */
					0,		 			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			int i;
			/* Create the Enumerated type, enumerated values, and the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Reply-Message */
		{
			/*
				The Reply-Message AVP (AVP Code 18) is of type UTF8String and
				contains text that MAY be displayed to the user.  When used in an
				AA-Answer message with a successful Result-Code AVP, it indicates
				success.  When found in an AAA message with a Result-Code other than
				DIAMETER_SUCCESS, the AVP contains a failure message.

				The Reply-Message AVP MAY indicate dialog text to prompt the user
				before another AA-Request attempt.  When used in an AA-Answer with a
				Result-Code of DIAMETER_MULTI_ROUND_AUTH or in an Re-Auth-Request
				message, it MAY contain a dialog text to prompt the user for a
				response.

				Multiple Reply-Messages MAY be included, and if any are displayed,
				they MUST be displayed in the same order as they appear in the
				Diameter message.
			*/
			struct dict_avp_data data = { 
					18, 					/* Code */
					0, 					/* Vendor */
					"Reply-Message", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
/********************************
 *  NAS Authentication AVPs     *
 ********************************/		
		/* User-Password */
		{
			/*
				The User-Password AVP (AVP Code 2) is of type OctetString and
				contains the password of the user to be authenticated, or the user's
				input in a multi-round authentication exchange.

				The User-Password AVP contains a user password or one-time password
				and therefore represents sensitive information.  As required in
				[BASE], Diameter messages are encrypted by using IPsec or TLS.
				Unless this AVP is used for one-time passwords, the User-Password AVP
				SHOULD NOT be used in untrusted proxy environments without encrypting
				it by using end-to-end security techniques, such as the proposed CMS
				Security [DiamCMS].

				The clear-text password (prior to encryption) MUST NOT be longer than
				128 bytes in length.
			*/
			struct dict_avp_data data = { 
					2, 					/* Code */
					0, 					/* Vendor */
					"User-Password", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Password-Retry */
		{
			/*
				The Password-Retry AVP (AVP Code 75) is of type Unsigned32 and MAY be
				included in the AA-Answer if the Result-Code indicates an
				authentication failure.  The value of this AVP indicates how many
				authentication attempts a user is permitted before being
				disconnected.  This AVP is primarily intended for use when the
				Framed-Protocol AVP (see section 6.10.1) is set to ARAP.
			*/
			struct dict_avp_data data = { 
					75, 					/* Code */
					0, 					/* Vendor */
					"Password-Retry", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Prompt */
		{
			/*
				The Prompt AVP (AVP Code 76) is of type Enumerated and MAY be present
				in the AA-Answer message.  When present, it is used by the NAS to
				determine whether the user's response, when entered, should be
				echoed.

				The supported values are listed in http://www.iana.org/assignments/radius-types
				Sub-registry: Values for RADIUS Attribute 76, Prompt
				Reference: [RFC2869]
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Prompt)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "No Echo [RFC2869]"),
				enumval_def_u32( 1, "Echo [RFC2869]")
			};
			struct dict_avp_data data = { 
					76, 					/* Code */
					0, 					/* Vendor */
					"Prompt", 			/* Name */
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
		
		/* CHAP-Algorithm */
		{
			/*
				The CHAP-Algorithm AVP (AVP Code 403) is of type Enumerated and
				contains the algorithm identifier used in the computation of the CHAP
				response [PPPCHAP].  The following values are currently supported:

				CHAP with MD5       5
				 The CHAP response is computed by using the procedure described
				 in [PPPCHAP].  This algorithm requires that the CHAP-Response
				 AVP MUST be present in the CHAP-Auth AVP.
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(CHAP-Algorithm)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 5, "CHAP with MD5")
			};
			struct dict_avp_data adata = { 
					403, 					/* Code */
					0, 					/* Vendor */
					"CHAP-Algorithm", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			int i;
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &adata , type, NULL);
		}
		
		/* CHAP-Ident */
		{
			/*
				The CHAP-Ident AVP (AVP Code 404) is of type OctetString and contains
				the 1 octet CHAP Identifier used in the computation of the CHAP
				response [PPPCHAP].
			*/
			struct dict_avp_data adata = { 
					404, 					/* Code */
					0, 					/* Vendor */
					"CHAP-Ident", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , NULL, NULL);
		}
			
		/* CHAP-Response */
		{
			/*
				The CHAP-Response AVP (AVP Code 405) is of type OctetString and
				contains the 16 octet authentication data provided by the user in
				response to the CHAP challenge [PPPCHAP].
			*/
			struct dict_avp_data adata = { 
					405, 					/* Code */
					0, 					/* Vendor */
					"CHAP-Response", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , NULL, NULL);
		}
			
		/* CHAP-Auth */
		{
			/*
				The CHAP-Auth AVP (AVP Code 402) is of type Grouped and contains the
				information necessary to authenticate a user using the PPP
				Challenge-Handshake Authentication Protocol (CHAP) [PPPCHAP].  If the
				CHAP-Auth AVP is found in a message, the CHAP-Challenge AVP MUST be
				present as well.  The optional AVPs containing the CHAP response
				depend upon the value of the CHAP-Algorithm AVP.  The grouped AVP has
				the following ABNF grammar:

				CHAP-Auth  ::= < AVP Header: 402 >
        				     { CHAP-Algorithm }
        				     { CHAP-Ident }
        				     [ CHAP-Response ]
        				   * [ AVP ]
			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					402, 					/* Code */
					0, 					/* Vendor */
					"CHAP-Auth", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "CHAP-Algorithm", 			RULE_REQUIRED, -1, 1 }
							,{  "CHAP-Ident",			RULE_REQUIRED, -1, 1 }
							,{  "CHAP-Response",			RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		
		/* CHAP-Challenge */
		{
			/*
				The CHAP-Challenge AVP (AVP Code 60) is of type OctetString and
				contains the CHAP Challenge sent by the NAS to the CHAP peer
				[PPPCHAP].
			*/
			struct dict_avp_data data = { 
					60, 					/* Code */
					0, 					/* Vendor */
					"CHAP-Challenge", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* ARAP-Password */
		{
			/*
				The ARAP-Password AVP (AVP Code 70) is of type OctetString and is
				only present when the Framed-Protocol AVP (see section 6.10.1) is
				included in the message and is set to ARAP.  This AVP MUST NOT be
				present if either the User-Password or the CHAP-Auth AVP is present.
				See [RADIUSExt] for more information on the contents of this AVP.
			*/
			struct dict_avp_data data = { 
					70, 					/* Code */
					0, 					/* Vendor */
					"ARAP-Password", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* ARAP-Challenge-Response */
		{
			/*
				The ARAP-Challenge-Response AVP (AVP Code 84) is of type OctetString
				and is only present when the Framed-Protocol AVP (see section 6.10.1)
				is included in the message and is set to ARAP.  This AVP contains an
				8 octet response to the dial-in client's challenge.  The RADIUS
				server calculates this value by taking the dial-in client's challenge
				from the high-order 8 octets of the ARAP-Password AVP and performing
				DES encryption on this value with the authenticating user's password
				as the key.  If the user's password is fewer than 8 octets in length,
				the password is padded at the end with NULL octets to a length of 8
				before it is used as a key.
			*/
			struct dict_avp_data data = { 
					84, 					/* Code */
					0, 					/* Vendor */
					"ARAP-Challenge-Response", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* ARAP-Security */
		{
			/*
				The ARAP-Security AVP (AVP Code 73) is of type Unsigned32 and MAY be
				present in the AA-Answer message if the Framed-Protocol AVP (see
				section 6.10.1) is set to the value of ARAP, and the Result-Code AVP
				is set to DIAMETER_MULTI_ROUND_AUTH.  See [RADIUSExt] for more
				information on the format of this AVP.
			*/
			struct dict_avp_data data = { 
					73, 					/* Code */
					0, 					/* Vendor */
					"ARAP-Security", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* ARAP-Security-Data */
		{
			/*
				The ARAP-Security-Data AVP (AVP Code 74) is of type OctetString and MAY be
				present in the AA-Request or AA-Answer message if the Framed-Protocol
				AVP is set to the value of ARAP, and the Result-Code AVP is set to
				DIAMETER_MULTI_ROUND_AUTH.  This AVP contains the security module
				challenge or response associated with the ARAP Security Module
				specified in ARAP-Security.
			*/
			struct dict_avp_data data = { 
					74, 					/* Code */
					0, 					/* Vendor */
					"ARAP-Security-Data", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
/********************************
 *  NAS Authorization AVPs      *
 ********************************/		
		/* Service-Type */
		{
			/*
				The Service-Type AVP (AVP Code 6) is of type Enumerated and contains
				the type of service the user has requested or the type of service to
				be provided.  One such AVP MAY be present in an authentication and/or
				authorization request or response.  A NAS is not required to
				implement all of these service types.  It MUST treat unknown or
				unsupported Service-Types received in a response as a failure and end
				the session with a DIAMETER_INVALID_AVP_VALUE Result-Code.

				When used in a request, the Service-Type AVP SHOULD be considered a
				hint to the server that the NAS believes the user would prefer the
				kind of service indicated.  The server is not required to honor the
				hint.  Furthermore, if the service specified by the server is
				supported, but not compatible with the current mode of access, the
				NAS MUST fail to start the session.  The NAS MUST also generate the
				appropriate error message(s).

				The following values have been defined for the Service-Type AVP.  The
				complete list of defined values can be found in [RADIUS] and
				[RADIUSTypes].

				Registry Name: Radius Attribute Values
				Reference: [RFC2865][RFC3575]

				Sub-registry: Values for RADIUS Attribute 6, Service-Type
				Reference: [RFC2865][RFC3575]

				1  Login
				2  Framed
				3  Callback Login
				4  Callback Framed
				5  Outbound
				6  Administrative
				7  NAS Prompt
				8  Authenticate Only
				9  Callback NAS Prompt
				10  Call Check
				11  Callback Administrative
				12  Voice           [Chiba]
				13  Fax             [Chiba]
				14  Modem Relay	    [Chiba]
				15  IAPP-Register   [IEEE 802.11f][Kerry]
				16  IAPP-AP-Check   [IEEE 802.11f][Kerry]
				17  Authorize Only  [RFC3576]

				The following values are further qualified:

				Login               1
				 The user should be connected to a host.  The message MAY
				 include additional AVPs defined in sections 6.16 or 6.17.

				Framed              2
				 A Framed Protocol, such as PPP or SLIP, should be started for
				 the User.  The message MAY include additional AVPs defined in
				 section 6.10, or section 7 for tunneling services.

				Callback Login      3
				 The user should be disconnected and called back, then connected
				 to a host.  The message MAY include additional AVPs defined in
				 this section.

				Callback Framed     4
				 The user should be disconnected and called back, and then a
				 Framed Protocol, such as PPP or SLIP, should be started for the
				 User.  The message MAY include additional AVPs defined in
				 section 6.10, or in section 7 for tunneling services.

			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Service-Type)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 1, "Login"),
				enumval_def_u32( 2, "Framed"),
				enumval_def_u32( 3, "Callback Login"),
				enumval_def_u32( 4, "Callback Framed"),
				enumval_def_u32( 5, "Outbound"),
				enumval_def_u32( 6, "Administrative"),
				enumval_def_u32( 7, "NAS Prompt"),
				enumval_def_u32( 8, "Authenticate Only"),
				enumval_def_u32( 9, "Callback NAS Prompt"),
				enumval_def_u32(10, "Call Check"),
				enumval_def_u32(11, "Callback Administrative"),
				enumval_def_u32(12, "Voice [Chiba]"),
				enumval_def_u32(13, "Fax [Chiba]"),
				enumval_def_u32(14, "Modem Relay [Chiba]"),
				enumval_def_u32(15, "IAPP-Register [IEEE 802.11f][Kerry]"),
				enumval_def_u32(16, "IAPP-AP-Check [IEEE 802.11f][Kerry]"),
				enumval_def_u32(17, "Authorize Only [RFC3576]")
			};
			struct dict_avp_data data = { 
					6, 					/* Code */
					0, 					/* Vendor */
					"Service-Type", 			/* Name */
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
		
		/* Callback-Number */
		{
			/*
				The Callback-Number AVP (AVP Code 19) is of type UTF8String and
				contains a dialing string to be used for callback.  It MAY be used in
				an authentication and/or authorization request as a hint to the
				server that a Callback service is desired, but the server is not
				required to honor the hint in the corresponding response.
			*/
			struct dict_avp_data data = { 
					19, 					/* Code */
					0, 					/* Vendor */
					"Callback-Number", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Callback-Id */
		{
			/*
				The Callback-Id AVP (AVP Code 20) is of type UTF8String and contains
				the name of a place to be called, to be interpreted by the NAS.  This
				AVP MAY be present in an authentication and/or authorization
				response.

				This AVP is not roaming-friendly as it assumes that the Callback-Id
				is configured on the NAS.  Using the Callback-Number AVP therefore
				preferable.
			*/
			struct dict_avp_data data = { 
					20, 					/* Code */
					0, 					/* Vendor */
					"Callback-Id", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Idle-Timeout */
		{
			/*
				The Idle-Timeout AVP (AVP Code 28) is of type Unsigned32 and sets the
				maximum number of consecutive seconds of idle connection allowable to
				the user before termination of the session or before a prompt is
				issued.  The default is none, or system specific.
			*/
			struct dict_avp_data data = { 
					28, 					/* Code */
					0, 					/* Vendor */
					"Idle-Timeout", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Port-Limit */
		{
			/*
				The Port-Limit AVP (AVP Code 62) is of type Unsigned32 and sets the
				maximum number of ports the NAS provides to the user.  It MAY be used
				in an authentication and/or authorization request as a hint to the
				server that multilink PPP [PPPMP] service is desired, but the server
				is not required to honor the hint in the corresponding response.
			*/
			struct dict_avp_data data = { 
					62, 					/* Code */
					0, 					/* Vendor */
					"Port-Limit", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* NAS-Filter-Rule */
		{
			/*
				The NAS-Filter-Rule AVP (AVP Code 400) is of type IPFilterRule and
				provides filter rules that need to be configured on the NAS for the
				user.  One or more of these AVPs MAY be present in an authorization
				response.
			*/
			struct dict_avp_data data = { 
					400, 					/* Code */
					0, 					/* Vendor */
					"NAS-Filter-Rule", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , IPFilterRule_type, NULL);
		}
		
		/* Filter-Id */
		{
			/*
				The Filter-Id AVP (AVP Code 11) is of type UTF8String and contains
				the name of the filter list for this user.  Zero or more Filter-Id
				AVPs MAY be sent in an authorization answer.

				Identifying a filter list by name allows the filter to be used on
				different NASes without regard to filter-list implementation details.
				However, this AVP is not roaming friendly, as filter naming differs
				from one service provider to another.

				In non-RADIUS environments, it is RECOMMENDED that the NAS-Filter-
				Rule AVP be used instead.
			*/
			struct dict_avp_data data = { 
					11, 					/* Code */
					0, 					/* Vendor */
					"Filter-Id", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Configuration-Token */
		{
			/*
				The Configuration-Token AVP (AVP Code 78) is of type OctetString and
				is sent by a Diameter Server to a Diameter Proxy Agent or Translation
				Agent in an AA-Answer command to indicate a type of user profile to
				be used.  It should not be sent to a Diameter Client (NAS).

				The format of the Data field of this AVP is site specific.
			*/
			struct dict_avp_data data = { 
					78, 					/* Code */
					0, 					/* Vendor */
					"Configuration-Token", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* QoS-Filter-Rule */
		{
			/*
				The QoS-Filter-Rule AVP (AVP Code 407) is of type QoSFilterRule and
				provides QoS filter rules that need to be configured on the NAS for
				the user.  One or more such AVPs MAY be present in an authorization
				response.
			*/
			struct dict_avp_data data = { 
					407, 					/* Code */
					0, 					/* Vendor */
					"QoS-Filter-Rule", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , QoSFilterRule_type, NULL);
		}
		
/***  Framed Access Authorization AVPs  ***/
		
		/* Framed-Protocol */
		{
			/*
				The Framed-Protocol AVP (AVP Code 7) is of type Enumerated and
				contains the framing to be used for framed access.  This AVP MAY be
				present in both requests and responses.  The supported values are
				listed in [RADIUSTypes].

				Sub-registry: Values for RADIUS Attribute 7, Framed-Protocol
				Reference: [RFC2865]
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Framed-Protocol)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 1, "PPP"),
				enumval_def_u32( 2, "SLIP"),
				enumval_def_u32( 3, "AppleTalk Remote Access Protocol (ARAP)"),
				enumval_def_u32( 4, "Gandalf proprietary SingleLink/MultiLink protocol"),
				enumval_def_u32( 5, "Xylogics proprietary IPX/SLIP"),
				enumval_def_u32( 6, "X.75 Synchronous"),
				enumval_def_u32( 7, "GPRS PDP Context [Moore]")
			};
			struct dict_avp_data data = { 
					7, 					/* Code */
					0, 					/* Vendor */
					"Framed-Protocol", 			/* Name */
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
		
		/* Framed-Routing */
		{
			/*
				The Framed-Routing AVP (AVP Code 10) is of type Enumerated and
				contains the routing method for the user when the user is a router to
				a network.  This AVP SHOULD only be present in authorization
				responses.  The supported values are listed in [RADIUSTypes].

				Sub-registry: Values for RADIUS Attribute 10, Framed-Routing
				Reference: [RFC2865]
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Framed-Routing)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "None"),
				enumval_def_u32( 1, "Send routing packets"),
				enumval_def_u32( 2, "Listen for routing packets"),
				enumval_def_u32( 3, "Send and Listen")
			};
			struct dict_avp_data data = { 
					10, 					/* Code */
					0, 					/* Vendor */
					"Framed-Routing", 			/* Name */
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
		
		/* Framed-MTU */
		{
			/*
				The Framed-MTU AVP (AVP Code 12) is of type Unsigned32 and contains
				the Maximum Transmission Unit to be configured for the user, when it
				is not negotiated by some other means (such as PPP).  This AVP SHOULD
				only be present in authorization responses.  The MTU value MUST be in
				the range from 64 to 65535.
			*/
			struct dict_avp_data data = { 
					12, 					/* Code */
					0, 					/* Vendor */
					"Framed-MTU", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Framed-Compression */
		{
			/*
				The Framed-Compression AVP (AVP Code 13) is of type Enumerated and
				contains the compression protocol to be used for the link.  It MAY be
				used in an authorization request as a hint to the server that a
				specific compression type is desired, but the server is not required
				to honor the hint in the corresponding response.

				More than one compression protocol AVP MAY be sent.  The NAS is
				responsible for applying the proper compression protocol to the
				appropriate link traffic.

				The supported values are listed in [RADIUSTypes].
				Sub-registry: Values for RADIUS Attribute 13, Framed-Compression
				Reference: [RFC2865]
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Framed-Compression)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "None"),
				enumval_def_u32( 1, "VJ TCP/IP header compression"),
				enumval_def_u32( 2, "IPX header compression"),
				enumval_def_u32( 3, "Stac-LZS compression")
			};
			struct dict_avp_data data = { 
					13, 					/* Code */
					0, 					/* Vendor */
					"Framed-Compression", 			/* Name */
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
		
/***  IP Access Authorization AVPs  ***/
		
		/* Framed-IP-Address */
		{
			/*
				The Framed-IP-Address AVP (AVP Code 8) [RADIUS] is of type
				OctetString and contains an IPv4 address of the type specified in the
				attribute value to be configured for the user.  It MAY be used in an
				authorization request as a hint to the server that a specific address
				is desired, but the server is not required to honor the hint in the
				corresponding response.

				Two values have special significance: 0xFFFFFFFF and 0xFFFFFFFE.  The
				value 0xFFFFFFFF indicates that the NAS should allow the user to
				select an address (i.e., negotiated).  The value 0xFFFFFFFE indicates
				that the NAS should select an address for the user (e.g., assigned
				from a pool of addresses kept by the NAS).
			*/
			struct dict_avp_data data = { 
					8, 					/* Code */
					0, 					/* Vendor */
					"Framed-IP-Address", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Framed-IP-Netmask */
		{
			/*
				The Framed-IP-Netmask AVP (AVP Code 9) is of type OctetString and
				contains the four octets of the IPv4 netmask to be configured for the
				user when the user is a router to a network.  It MAY be used in an
				authorization request as a hint to the server that a specific netmask
				is desired, but the server is not required to honor the hint in the
				corresponding response.  This AVP MUST be present in a response if
				the request included this AVP with a value of 0xFFFFFFFF.
			*/
			struct dict_avp_data data = { 
					9, 					/* Code */
					0, 					/* Vendor */
					"Framed-IP-Netmask", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Framed-Route */
		{
			/*
				The Framed-Route AVP (AVP Code 22) is of type UTF8String and contains
				the ASCII routing information to be configured for the user on the
				NAS.  Zero or more of these AVPs MAY be present in an authorization
				response.

				The string MUST contain a destination prefix in dotted quad form
				optionally followed by a slash and a decimal length specifier stating
				how many high-order bits of the prefix should be used.  This is
				followed by a space, a gateway address in dotted quad form, a space,
				and one or more metrics separated by spaces; for example,

				"192.168.1.0/24 192.168.1.1 1".

				The length specifier may be omitted, in which case it should default
				to 8 bits for class A prefixes, to 16 bits for class B prefixes, and
				to 24 bits for class C prefixes; for example,

				"192.168.1.0 192.168.1.1 1".

				Whenever the gateway address is specified as "0.0.0.0" the IP address
				of the user SHOULD be used as the gateway address.
			*/
			struct dict_avp_data data = { 
					22, 					/* Code */
					0, 					/* Vendor */
					"Framed-Route", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Framed-Pool */
		{
			/*
				The Framed-Pool AVP (AVP Code 88) is of type OctetString and contains
				the name of an assigned address pool that SHOULD be used to assign an
				address for the user.  If a NAS does not support multiple address
				pools, the NAS SHOULD ignore this AVP.  Address pools are usually
				used for IP addresses but can be used for other protocols if the NAS
				supports pools for those protocols.

				Although specified as type OctetString for compatibility with RADIUS
				[RADIUSExt], the encoding of the Data field SHOULD also conform to
				the rules for the UTF8String Data Format.
			*/
			struct dict_avp_data data = { 
					88, 					/* Code */
					0, 					/* Vendor */
					"Framed-Pool", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Framed-Interface-Id */
		{
			/*
				The Framed-Interface-Id AVP (AVP Code 96) is of type Unsigned64 and
				contains the IPv6 interface identifier to be configured for the user.
				It MAY be used in authorization requests as a hint to the server that
				a specific interface id is desired, but the server is not required to
				honor the hint in the corresponding response.
			*/
			struct dict_avp_data data = { 
					96, 					/* Code */
					0, 					/* Vendor */
					"Framed-Interface-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Framed-IPv6-Prefix */
		{
			/*
				The Framed-IPv6-Prefix AVP (AVP Code 97) is of type OctetString and
				contains the IPv6 prefix to be configured for the user.  One or more
				AVPs MAY be used in authorization requests as a hint to the server
				that specific IPv6 prefixes are desired, but the server is not
				required to honor the hint in the corresponding response.
			*/
			struct dict_avp_data data = { 
					97, 					/* Code */
					0, 					/* Vendor */
					"Framed-IPv6-Prefix", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Framed-IPv6-Route */
		{
			/*
				The Framed-IPv6-Route AVP (AVP Code 99) is of type UTF8String and
				contains the ASCII routing information to be configured for the user
				on the NAS.  Zero or more of these AVPs MAY be present in an
				authorization response.

				The string MUST contain an IPv6 address prefix followed by a slash
				and a decimal length specifier stating how many high order bits of
				the prefix should be used.  This is followed by a space, a gateway
				address in hexadecimal notation, a space, and one or more metrics
				separated by spaces; for example,

				"2000:0:0:106::/64 2000::106:a00:20ff:fe99:a998 1".

				Whenever the gateway address is the IPv6 unspecified address, the IP
				address of the user SHOULD be used as the gateway address, such as
				in:

				"2000:0:0:106::/64 :: 1".
			*/
			struct dict_avp_data data = { 
					99, 					/* Code */
					0, 					/* Vendor */
					"Framed-IPv6-Route", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Framed-IPv6-Pool */
		{
			/*
				The Framed-IPv6-Pool AVP (AVP Code 100) is of type OctetString and
				contains the name of an assigned pool that SHOULD be used to assign
				an IPv6 prefix for the user.  If the access device does not support
				multiple prefix pools, it MUST ignore this AVP.

				Although specified as type OctetString for compatibility with RADIUS
				[RADIUSIPv6], the encoding of the Data field SHOULD also conform to
				the rules for the UTF8String Data Format.
			*/
			struct dict_avp_data data = { 
					100, 					/* Code */
					0, 					/* Vendor */
					"Framed-IPv6-Pool", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
/***  IPX Access  ***/
		
		/* Framed-IPX-Network */
		{
			/*
				The Framed-IPX-Network AVP (AVP Code 23) is of type Unsigned32 and
				contains the IPX Network number to be configured for the user.  It
				MAY be used in an authorization request as a hint to the server that
				a specific address is desired, but the server is not required to
				honor the hint in the corresponding response.

				Two addresses have special significance: 0xFFFFFFFF and 0xFFFFFFFE.
				The value 0xFFFFFFFF indicates that the NAS should allow the user to
				select an address (i.e., Negotiated).  The value 0xFFFFFFFE indicates
				that the NAS should select an address for the user (e.g., assign it
				from a pool of one or more IPX networks kept by the NAS).
			*/
			struct dict_avp_data data = { 
					23, 					/* Code */
					0, 					/* Vendor */
					"Framed-IPX-Network", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
/***  AppleTalk Network Access  ***/
		
		/* Framed-AppleTalk-Link */
		{
			/*
				The Framed-AppleTalk-Link AVP (AVP Code 37) is of type Unsigned32 and
				contains the AppleTalk network number that should be used for the
				serial link to the user, which is another AppleTalk router.  This AVP
				MUST only be present in an authorization response and is never used
				when the user is not another router.

				Despite the size of the field, values range from 0 to 65,535.  The
				special value of 0 indicates an unnumbered serial link.  A value of 1
				to 65,535 means that the serial line between the NAS and the user
				should be assigned that value as an AppleTalk network number.
			*/
			struct dict_avp_data data = { 
					37, 					/* Code */
					0, 					/* Vendor */
					"Framed-AppleTalk-Link", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Framed-AppleTalk-Network */
		{
			/*
				The Framed-AppleTalk-Network AVP (AVP Code 38) is of type Unsigned32
				and contains the AppleTalk Network number that the NAS should probe
				to allocate an AppleTalk node for the user.  This AVP MUST only be
				present in an authorization response and is never used when the user
				is not another router.  Multiple instances of this AVP indicate that
				the NAS may probe, using any of the network numbers specified.

				Despite the size of the field, values range from 0 to 65,535.  The
				special value 0 indicates that the NAS should assign a network for
				the user, using its default cable range.  A value between 1 and
				65,535 (inclusive) indicates to the AppleTalk Network that the NAS
				should probe to find an address for the user.
			*/
			struct dict_avp_data data = { 
					38, 					/* Code */
					0, 					/* Vendor */
					"Framed-AppleTalk-Network", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Framed-AppleTalk-Zone */
		{
			/*
				The Framed-AppleTalk-Zone AVP (AVP Code 39) is of type OctetString
				and contains the AppleTalk Default Zone to be used for this user.
				This AVP MUST only be present in an authorization response.  Multiple
				instances of this AVP in the same message are not allowed.

				The codification of this field's allowed range is outside the scope
				of this specification.
			*/
			struct dict_avp_data data = { 
					39, 					/* Code */
					0, 					/* Vendor */
					"Framed-AppleTalk-Zone", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
/***  AppleTalk Remote Access [RFC2869] ***/
		
		/* ARAP-Features */
		{
			/*
				The ARAP-Features AVP (AVP Code 71) is of type OctetString and MAY be
				present in the AA-Accept message if the Framed-Protocol AVP is set to
				the value of ARAP.  See [RADIUSExt] for more information about the
				format of this AVP.
			*/
			struct dict_avp_data data = { 
					71, 					/* Code */
					0, 					/* Vendor */
					"ARAP-Features", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* ARAP-Zone-Access */
		{
			/*
				The ARAP-Zone-Access AVP (AVP Code 72) is of type Enumerated and MAY
				be present in the AA-Accept message if the Framed-Protocol AVP is set
				to the value of ARAP.

				The supported values are listed in [RADIUSTypes] and defined in
				[RADIUSExt].
				Sub-registry: Values for RADIUS Attribute 72, ARAP-Zone-Access 
				Reference: [RFC2869]
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(ARAP-Zone-Access)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 1, "Only allow access to default zone [RFC2869]"),
				enumval_def_u32( 2, "Use zone filter inclusively [RFC2869]"),
				enumval_def_u32( 3, "Not used [RFC2869]"),
				enumval_def_u32( 4, "Use zone filter exclusively [RFC2869]")
			};
			struct dict_avp_data data = { 
					72, 					/* Code */
					0, 					/* Vendor */
					"ARAP-Zone-Access", 			/* Name */
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
		
/***  Non-Framed Access Authorization AVPs ***/
		
		/* Login-IP-Host */
		{
			/*
				The Login-IP-Host AVP (AVP Code 14) [RADIUS] is of type OctetString
				and contains the IPv4 address of a host with which to connect the
				user when the Login-Service AVP is included.  It MAY be used in an
				AA-Request command as a hint to the Diameter Server that a specific
				host is desired, but the Diameter Server is not required to honor the
				hint in the AA-Answer.

				Two addresses have special significance: all ones and 0.  The value
				of all ones indicates that the NAS SHOULD allow the user to select an
				address.  The value 0 indicates that the NAS SHOULD select a host to
				connect the user to.
			*/
			struct dict_object 	*type;
			uint32_t		 allzeros = 0;
			uint32_t		 allones  = (uint32_t) -1;
			struct dict_type_data 	 tdata = { AVP_TYPE_OCTETSTRING,	"Enumerated(Login-IP-Host)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_os( 4, &allzeros, "(0) NAS selects the host"),
				enumval_def_os( 4, &allones , "(1) User selects the address")
			};
			struct dict_avp_data data = { 
					14, 					/* Code */
					0, 					/* Vendor */
					"Login-IP-Host", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			int i;
			/* Create the Enumerated type, enumerated values, and the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Login-IPv6-Host */
		{
			/*
				The Login-IPv6-Host AVP (AVP Code 98) [RADIUSIPv6] is of type
				OctetString and contains the IPv6 address of a host with which to
				connect the user when the Login-Service AVP is included.  It MAY be
				used in an AA-Request command as a hint to the Diameter Server that a
				specific host is desired, but the Diameter Server is not required to
				honor the hint in the AA-Answer.

				Two addresses have special significance:

				0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF and 0.  The value
				0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF indicates that the NAS SHOULD
				allow the user to select an address.  The value 0 indicates that the
				NAS SHOULD select a host to connect the user to.

			*/
			struct dict_object 	*type;
			unsigned char 		 allzeros[16];
			unsigned char		 allones[16];
			struct dict_type_data 	 tdata = { AVP_TYPE_OCTETSTRING,	"Enumerated(Login-IPv6-Host)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_os( 16, &allzeros, "(0) NAS selects the host"),
				enumval_def_os( 16, &allones , "(1) User selects the address")
			};
			struct dict_avp_data data = { 
					98, 					/* Code */
					0, 					/* Vendor */
					"Login-IPv6-Host", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			int i;
			memset(allzeros, 0,   sizeof(allzeros));
			memset(allones, 0xff, sizeof(allones));
			/* Create the Enumerated type, enumerated values, and the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Login-Service */
		{
			/*
				The Login-Service AVP (AVP Code 15) is of type Enumerated and
				contains the service that should be used to connect the user to the
				login host.  This AVP SHOULD only be present in authorization
				responses.

				The supported values are listed in [RADIUSTypes].
				Sub-registry: Values for RADIUS Attribute 15, Login-Service
				Reference: [RFC2865]
   			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Login-Service)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 0, "Telnet"),
				enumval_def_u32( 1, "Rlogin"),
				enumval_def_u32( 2, "TCP Clear"),
				enumval_def_u32( 3, "PortMaster (proprietary)"),
				enumval_def_u32( 4, "LAT"),
				enumval_def_u32( 5, "X25-PAD"),
				enumval_def_u32( 6, "X25-T3POS"),
				enumval_def_u32( 8, "TCP Clear Quiet (suppresses any NAS-generated connect string)")
			};
			struct dict_avp_data data = { 
					15, 					/* Code */
					0, 					/* Vendor */
					"Login-Service", 			/* Name */
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
		
/***  TCP Services  ***/
		
		/* Login-TCP-Port */
		{
			/*
				The Login-TCP-Port AVP (AVP Code 16) is of type Unsigned32 and
				contains the TCP port with which the user is to be connected when the
				Login-Service AVP is also present.  This AVP SHOULD only be present
				in authorization responses.  The value MUST NOT be greater than
				65,535.
			*/
			struct dict_avp_data data = { 
					16, 					/* Code */
					0, 					/* Vendor */
					"Login-TCP-Port", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
/***  LAT Services  ***/
		
		/* Login-LAT-Service */
		{
			/*
				The Login-LAT-Service AVP (AVP Code 34) is of type OctetString and
				contains the system with which the user is to be connected by LAT.
				It MAY be used in an authorization request as a hint to the server
				that a specific service is desired, but the server is not required to
				honor the hint in the corresponding response.  This AVP MUST only be
				present in the response if the Login-Service AVP states that LAT is
				desired.

				Administrators use this service attribute when dealing with clustered
				systems, such as a VAX or Alpha cluster.  In these environments,
				several different time-sharing hosts share the same resources (disks,
				printers, etc.), and administrators often configure each host to
				offer access (service) to each of the shared resources.  In this
				case, each host in the cluster advertises its services through LAT
				broadcasts.

				Sophisticated users often know which service providers (machines) are
				faster and tend to use a node name when initiating a LAT connection.
				Some administrators want particular users to use certain machines as
				a primitive form of load balancing (although LAT knows how to do load
				balancing itself).

				The String field contains the identity of the LAT service to use.
				The LAT Architecture allows this string to contain $ (dollar), -
				(hyphen), . (period), _ (underscore), numerics, upper- and lowercase
				alphabetics, and the ISO Latin-1 character set extension [ISOLatin].
				All LAT string comparisons are case insensitive.
			*/
			struct dict_avp_data data = { 
					34, 					/* Code */
					0, 					/* Vendor */
					"Login-LAT-Service", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Login-LAT-Node */
		{
			/*
				The Login-LAT-Node AVP (AVP Code 35) is of type OctetString and
				contains the Node with which the user is to be automatically
				connected by LAT.  It MAY be used in an authorization request as a
				hint to the server that a specific LAT node is desired, but the
				server is not required to honor the hint in the corresponding
				response.  This AVP MUST only be present in a response if the Login-
				Service-Type AVP is set to LAT.

				The String field contains the identity of the LAT service to use.
				The LAT Architecture allows this string to contain $ (dollar), -
				(hyphen), . (period), _ (underscore), numerics, upper- and lowercase
				alphabetics, and the ISO Latin-1 character set extension [ISOLatin].
				All LAT string comparisons are case insensitive.
			*/
			struct dict_avp_data data = { 
					35, 					/* Code */
					0, 					/* Vendor */
					"Login-LAT-Node", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Login-LAT-Group */
		{
			/*
				The Login-LAT-Group AVP (AVP Code 36) is of type OctetString and
				contains a string identifying the LAT group codes this user is
				authorized to use.  It MAY be used in an authorization request as a
				hint to the server that a specific group is desired, but the server
				is not required to honor the hint in the corresponding response.
				This AVP MUST only be present in a response if the Login-Service-Type
				AVP is set to LAT.

				LAT supports 256 different group codes, which LAT uses as a form of
				access rights.  LAT encodes the group codes as a 256-bit bitmap.

				Administrators can assign one or more of the group code bits at the
				LAT service provider; it will only accept LAT connections that have
				these group codes set in the bitmap.  The administrators assign a
				bitmap of authorized group codes to each user.  LAT gets these from
				the operating system and uses them in its requests to the service
				providers.

				The codification of the range of allowed usage of this field is
				outside the scope of this specification.
			*/
			struct dict_avp_data data = { 
					36, 					/* Code */
					0, 					/* Vendor */
					"Login-LAT-Group", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Login-LAT-Port */
		{
			/*
				The Login-LAT-Port AVP (AVP Code 63) is of type OctetString and
				contains the Port with which the user is to be connected by LAT.  It
				MAY be used in an authorization request as a hint to the server that
				a specific port is desired, but the server is not required to honor
				the hint in the corresponding response.  This AVP MUST only be
				present in a response if the Login-Service-Type AVP is set to LAT.

				The String field contains the identity of the LAT service to use.
				The LAT Architecture allows this string to contain $ (dollar), -
				(hyphen), . (period), _ (underscore), numerics, upper- and lower-case
				alphabetics, and the ISO Latin-1 character set extension [ISOLatin].
				All LAT string comparisons are case insensitive.
			*/
			struct dict_avp_data data = { 
					63, 					/* Code */
					0, 					/* Vendor */
					"Login-LAT-Port", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
/********************************
 *  NAS Tunneling AVPs          *
 ********************************/		
		/* Tunnel-Type */
		{
			/*
				The Tunnel-Type AVP (AVP Code 64) is of type Enumerated and contains
				the tunneling protocol(s) to be used (in the case of a tunnel
				initiator) or in use (in the case of a tunnel terminator).  It MAY be
				used in an authorization request as a hint to the server that a
				specific tunnel type is desired, but the server is not required to
				honor the hint in the corresponding response.

				The Tunnel-Type AVP SHOULD also be included in Accounting-Request
				messages.

				A tunnel initiator is not required to implement any of these tunnel
				types.  If a tunnel initiator receives a response that contains only
				unknown or unsupported Tunnel-Types, the tunnel initiator MUST behave
				as though a response were received with the Result-Code indicating a
				failure.

				The supported values are listed in [RADIUSTypes].
				Sub-registry: Values for RADIUS Attribute 64, Tunnel-Type 
				Reference: [RFC2868]
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Tunnel-Type)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 1, "Point-to-Point Tunneling Protocol (PPTP) [RFC2868]"),
				enumval_def_u32( 2, "Layer Two Forwarding (L2F) [RFC2868]"),
				enumval_def_u32( 3, "Layer Two Tunneling Protocol (L2TP) [RFC2868]"),
				enumval_def_u32( 4, "Ascend Tunnel Management Protocol (ATMP) [RFC2868]"),
				enumval_def_u32( 5, "Virtual Tunneling Protocol (VTP) [RFC2868]"),
				enumval_def_u32( 6, "IP Authentication Header in the Tunnel-mode (AH) [RFC2868]"),
				enumval_def_u32( 7, "IP-in-IP Encapsulation (IP-IP) [RFC2868]"),
				enumval_def_u32( 8, "Minimal IP-in-IP Encapsulation (MIN-IP-IP) [RFC2868]"),
				enumval_def_u32( 9, "IP Encapsulating Security Payload in the Tunnel-mode (ESP) [RFC2868]"),
				enumval_def_u32(10, "Generic Route Encapsulation (GRE) [RFC2868]"),
				enumval_def_u32(11, "Bay Dial Virtual Services (DVS) [RFC2868]"),
				enumval_def_u32(12, "IP-in-IP Tunneling [RFC2868]"),
				enumval_def_u32(13, "Virtual LANs (VLAN) [RFC3580]")
			};
			struct dict_avp_data adata = { 
					64, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Type", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			int i;
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &adata , type, NULL);
		}
		
		/* Tunnel-Medium-Type */
		{
			/*
				The Tunnel-Medium-Type AVP (AVP Code 65) is of type Enumerated and
				contains the transport medium to use when creating a tunnel for
				protocols (such as L2TP) that can operate over multiple transports.
				It MAY be used in an authorization request as a hint to the server
				that a specific medium is desired, but the server is not required to
				honor the hint in the corresponding response.

				The supported values are listed in [RADIUSTypes].
				Sub-registry: Values for RADIUS Attribute 65, Tunnel-Medium-Type
				Reference: [RFC2868]
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Tunnel-Medium-Type)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 1, "IPv4 (IP version 4) [RFC2868]"),
				enumval_def_u32( 2, "IPv6 (IP version 6) [RFC2868]"),
				enumval_def_u32( 3, "NSAP [RFC2868]"),
				enumval_def_u32( 4, "HDLC (8-bit multidrop) [RFC2868]"),
				enumval_def_u32( 5, "BBN 1822 [RFC2868]"),
				enumval_def_u32( 6, "802 (includes all 802 media plus Ethernet \"canonical format\") [RFC2868]"),
				enumval_def_u32( 7, "E.163 (POTS) [RFC2868]"),
				enumval_def_u32( 8, "E.164 (SMDS, Frame Relay, ATM) [RFC2868]"),
				enumval_def_u32( 9, "F.69 (Telex) [RFC2868]"),
				enumval_def_u32(10, "X.121 (X.25, Frame Relay) [RFC2868]"),
				enumval_def_u32(11, "IPX [RFC2868]"),
				enumval_def_u32(12, "Appletalk [RFC2868]"),
				enumval_def_u32(13, "Decnet IV [RFC2868]"),
				enumval_def_u32(14, "Banyan Vines [RFC2868]"),
				enumval_def_u32(15, "E.164 with NSAP format subaddress [RFC2868]")
			};
			struct dict_avp_data adata = { 
					65, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Medium-Type", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			int i;
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			CHECK_dict_new( DICT_AVP, &adata , type, NULL);
		}
		
		/* Tunnel-Client-Endpoint */
		{
			/*
				The Tunnel-Client-Endpoint AVP (AVP Code 66) is of type UTF8String
				and contains the address of the initiator end of the tunnel.  It MAY
				be used in an authorization request as a hint to the server that a
				specific endpoint is desired, but the server is not required to honor
				the hint in the corresponding response.

				This AVP SHOULD be included in the corresponding Accounting-Request
				messages, in which case it indicates the address from which the
				tunnel was initiated.  This AVP, along with the Tunnel-Server-
				Endpoint and Session-Id AVP [BASE], MAY be used to provide a globally
				unique means to identify a tunnel for accounting and auditing
				purposes.

				If Tunnel-Medium-Type is IPv4 (1), then this string is either the
				fully qualified domain name (FQDN) of the tunnel client machine, or a
				"dotted-decimal" IP address.  Implementations MUST support the
				dotted-decimal format and SHOULD support the FQDN format for IP
				addresses.

				If Tunnel-Medium-Type is IPv6 (2), then this string is either the
				FQDN of the tunnel client machine, or a text representation of the
				address in either the preferred or alternate form [IPv6Addr].
				Conforming implementations MUST support the preferred form and SHOULD
				support both the alternate text form and the FQDN format for IPv6
				addresses.

				If Tunnel-Medium-Type is neither IPv4 nor IPv6, then this string is a
				tag referring to configuration data local to the Diameter client that
				describes the interface or medium-specific client address to use.
   			*/
			struct dict_avp_data adata = { 
					66, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Client-Endpoint", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , UTF8String_type, NULL);
		}
		
		/* Tunnel-Server-Endpoint */
		{
			/*
				The Tunnel-Server-Endpoint AVP (AVP Code 67) is of type UTF8String
				and contains the address of the server end of the tunnel.  It MAY be
				used in an authorization request as a hint to the server that a
				specific endpoint is desired, but the server is not required to honor
				the hint in the corresponding response.

				This AVP SHOULD be included in the corresponding Accounting-Request
				messages, in which case it indicates the address from which the
				tunnel was initiated.  This AVP, along with the Tunnel-Client-
				Endpoint and Session-Id AVP [BASE], MAY be used to provide a globally
				unique means to identify a tunnel for accounting and auditing
				purposes.

				If Tunnel-Medium-Type is IPv4 (1), then this string is either the
				fully qualified domain name (FQDN) of the tunnel server machine, or a
				"dotted-decimal" IP address.  Implementations MUST support the
				dotted-decimal format and SHOULD support the FQDN format for IP
				addresses.

				If Tunnel-Medium-Type is IPv6 (2), then this string is either the
				FQDN of the tunnel server machine, or a text representation of the
				address in either the preferred or alternate form [IPv6Addr].
				Implementations MUST support the preferred form and SHOULD support
				both the alternate text form and the FQDN format for IPv6 addresses.

				If Tunnel-Medium-Type is not IPv4 or IPv6, this string is a tag
				referring to configuration data local to the Diameter client that
				describes the interface or medium-specific server address to use.
   			*/
			struct dict_avp_data adata = { 
					67, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Server-Endpoint", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , UTF8String_type, NULL);
		}
		
		/* Tunnel-Password */
		{
			/*
				The Tunnel-Password AVP (AVP Code 69) is of type OctetString and may
				contain a password to be used to authenticate to a remote server.
				The Tunnel-Password AVP contains sensitive information.  This value
				is not protected in the same manner as RADIUS [RADTunnels].

				As required in [BASE], Diameter messages are encrypted by using IPsec
				or TLS.  The Tunnel-Password AVP SHOULD NOT be used in untrusted
				proxy environments without encrypting it by using end-to-end security
				techniques, such as CMS Security [DiamCMS].
   			*/
			struct dict_avp_data adata = { 
					69, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Password", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , NULL, NULL);
		}
		
		/* Tunnel-Private-Group-Id */
		{
			/*
				The Tunnel-Private-Group-Id AVP (AVP Code 81) is of type OctetString
				and contains the group Id for a particular tunneled session.  The
				Tunnel-Private-Group-Id AVP MAY be included in an authorization
				request if the tunnel initiator can predetermine the group resulting
				from a particular connection.  It SHOULD be included in the
				authorization response if this tunnel session is to be treated as
				belonging to a particular private group.  Private groups may be used
				to associate a tunneled session with a particular group of users.
				For example, it MAY be used to facilitate routing of unregistered IP
				addresses through a particular interface.  This AVP SHOULD be
				included in the Accounting-Request messages that pertain to the
				tunneled session.
   			*/
			struct dict_avp_data adata = { 
					81, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Private-Group-Id", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , NULL, NULL);
		}
		
		/* Tunnel-Assignment-Id */
		{
			/*
				The Tunnel-Assignment-Id AVP (AVP Code 82) is of type OctetString and
				is used to indicate to the tunnel initiator the particular tunnel to
				which a session is to be assigned.  Some tunneling protocols, such as
				[PPTP] and [L2TP], allow for sessions between the same two tunnel
				endpoints to be multiplexed over the same tunnel and also for a given
				session to use its own dedicated tunnel.  This attribute provides a
				mechanism for Diameter to inform the tunnel initiator (e.g., PAC,
				LAC) whether to assign the session to a multiplexed tunnel or to a
				separate tunnel.  Furthermore, it allows for sessions sharing
				multiplexed tunnels to be assigned to different multiplexed tunnels.

				A particular tunneling implementation may assign differing
				characteristics to particular tunnels.  For example, different
				tunnels may be assigned different QoS parameters.  Such tunnels may
				be used to carry either individual or multiple sessions.  The
				Tunnel-Assignment-Id attribute thus allows the Diameter server to
				indicate that a particular session is to be assigned to a tunnel
				providing an appropriate level of service.  It is expected that any
				QoS-related Diameter tunneling attributes defined in the future
				accompanying this one will be associated by the tunnel initiator with
				the Id given by this attribute.  In the meantime, any semantic given
				to a particular Id string is a matter left to local configuration in
				the tunnel initiator.

				The Tunnel-Assignment-Id AVP is of significance only to Diameter and
				the tunnel initiator.  The Id it specifies is only intended to be of
				local use to Diameter and the tunnel initiator.  The Id assigned by
				the tunnel initiator is not conveyed to the tunnel peer.

				This attribute MAY be included in authorization responses.  The
				tunnel initiator receiving this attribute MAY choose to ignore it and
				to assign the session to an arbitrary multiplexed or non-multiplexed
				tunnel between the desired endpoints.  This AVP SHOULD also be
				included in the Accounting-Request messages pertaining to the
				tunneled session.

				If a tunnel initiator supports the Tunnel-Assignment-Id AVP, then it
				should assign a session to a tunnel in the following manner:

				-  If this AVP is present and a tunnel exists between the
				 specified endpoints with the specified Id, then the session
				 should be assigned to that tunnel.

				-  If this AVP is present and no tunnel exists between the
				 specified endpoints with the specified Id, then a new tunnel
				 should be established for the session and the specified Id
				 should be associated with the new tunnel.

				-  If this AVP is not present, then the session is assigned to an
				 unnamed tunnel.  If an unnamed tunnel does not yet exist
				 between the specified endpoints, then it is established and
				 used for this session and for subsequent ones established
				 without the Tunnel-Assignment-Id attribute.  A tunnel initiator
				 MUST NOT assign a session for which a Tunnel-Assignment-Id AVP
				 was not specified to a named tunnel (i.e., one that was
				 initiated by a session specifying this AVP).

				Note that the same Id may be used to name different tunnels if these
				tunnels are between different endpoints.

   			*/
			struct dict_avp_data adata = { 
					82, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Assignment-Id", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , NULL, NULL);
		}
		
		/* Tunnel-Preference */
		{
			/*
				The Tunnel-Preference AVP (AVP Code 83) is of type Unsigned32 and is
				used to identify the relative preference assigned to each tunnel when
				more than one set of tunneling AVPs is returned within separate
				Grouped-AVP AVPs.  It MAY be used in an authorization request as a
				hint to the server that a specific preference is desired, but the
				server is not required to honor the hint in the corresponding
				response.

				For example, suppose that AVPs describing two tunnels are returned by
				the server, one with a Tunnel-Type of PPTP and the other with a
				Tunnel-Type of L2TP.  If the tunnel initiator supports only one of
				the Tunnel-Types returned, it will initiate a tunnel of that type.
				If, however, it supports both tunnel protocols, it SHOULD use the
				value of the Tunnel-Preference AVP to decide which tunnel should be
				started.  The tunnel with the lowest numerical value in the Value
				field of this AVP SHOULD be given the highest preference.  The values
				assigned to two or more instances of the Tunnel-Preference AVP within
				a given authorization response MAY be identical.  In this case, the
				tunnel initiator SHOULD use locally configured metrics to decide
				which set of AVPs to use.
   			*/
			struct dict_avp_data adata = { 
					83, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Preference", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , NULL, NULL);
		}
		
		/* Tunnel-Client-Auth-Id */
		{
			/*
				The Tunnel-Client-Auth-Id AVP (AVP Code 90) is of type UTF8String and
				specifies the name used by the tunnel initiator during the
				authentication phase of tunnel establishment.  It MAY be used in an
				authorization request as a hint to the server that a specific
				preference is desired, but the server is not required to honor the
				hint in the corresponding response.  This AVP MUST be present in the
				authorization response if an authentication name other than the
				default is desired.  This AVP SHOULD be included in the Accounting-
				Request messages pertaining to the tunneled session.
   			*/
			struct dict_avp_data adata = { 
					90, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Client-Auth-Id", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , UTF8String_type, NULL);
		}
		
		/* Tunnel-Server-Auth-Id */
		{
			/*
				The Tunnel-Server-Auth-Id AVP (AVP Code 91) is of type UTF8String and
				specifies the name used by the tunnel terminator during the
				authentication phase of tunnel establishment.  It MAY be used in an
				authorization request as a hint to the server that a specific
				preference is desired, but the server is not required to honor the
				hint in the corresponding response.  This AVP MUST be present in the
				authorization response if an authentication name other than the
				default is desired.  This AVP SHOULD be included in the Accounting-
				Request messages pertaining to the tunneled session.
   			*/
			struct dict_avp_data adata = { 
					91, 					/* Code */
					0, 					/* Vendor */
					"Tunnel-Server-Auth-Id", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , UTF8String_type, NULL);
		}
		
		/* Tunneling */
		{
			/*
				The Tunneling AVP (AVP Code 401) is of type Grouped and contains the
				following AVPs, used to describe a compulsory tunnel service:
				[RADTunnels], [RADTunlAcct].  Its data field has the following ABNF
				grammar:

				Tunneling     ::= < AVP Header: 401 >
                				{ Tunnel-Type }
                				{ Tunnel-Medium-Type }
                				{ Tunnel-Client-Endpoint }
                				{ Tunnel-Server-Endpoint }
                				[ Tunnel-Preference ]
                				[ Tunnel-Client-Auth-Id ]
                				[ Tunnel-Server-Auth-Id ]
                				[ Tunnel-Assignment-Id ]
                				[ Tunnel-Password ]
                				[ Tunnel-Private-Group-Id ]
			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					401, 					/* Code */
					0, 					/* Vendor */
					"Tunneling", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Tunnel-Type", 			RULE_REQUIRED, -1, 1 }
							,{  "Tunnel-Medium-Type",		RULE_OPTIONAL, -1, 1 }
							,{  "Tunnel-Client-Endpoint",		RULE_OPTIONAL, -1, 1 }
							,{  "Tunnel-Server-Endpoint",		RULE_OPTIONAL, -1, 1 }
							,{  "Tunnel-Preference",		RULE_OPTIONAL, -1, 1 }
							,{  "Tunnel-Client-Auth-Id",		RULE_OPTIONAL, -1, 1 }
							,{  "Tunnel-Server-Auth-Id",		RULE_OPTIONAL, -1, 1 }
							,{  "Tunnel-Assignment-Id",		RULE_OPTIONAL, -1, 1 }
							,{  "Tunnel-Password",			RULE_OPTIONAL, -1, 1 }
							,{  "Tunnel-Private-Group-Id",		RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		
/********************************
 *  NAS Accounting AVPs         *
 ********************************/		
		/* Accounting-Input-Octets */
		{
			/*
				The Accounting-Input-Octets AVP (AVP Code 363) is of type Unsigned64
				and contains the number of octets received from the user.
				
				For NAS usage, this AVP indicates how many octets have been received
				from the port in the course of this session.  It can only be present
				in ACR messages with an Accounting-Record-Type of INTERIM_RECORD or
				STOP_RECORD.
			*/
			struct dict_avp_data data = { 
					363, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Input-Octets", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Accounting-Output-Octets */
		{
			/*
				The Accounting-Output-Octets AVP (AVP Code 364) is of type Unsigned64
				and contains the number of octets sent to the user.

				For NAS usage, this AVP indicates how many octets have been sent to
				the port in the course of this session.  It can only be present in
				ACR messages with an Accounting-Record-Type of INTERIM_RECORD or
				STOP_RECORD.
			*/
			struct dict_avp_data data = { 
					364, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Output-Octets", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Accounting-Input-Packets */
		{
			/*
				The Accounting-Input-Packets (AVP Code 365) is of type Unsigned64 and
				contains the number of packets received from the user.

				For NAS usage, this AVP indicates how many packets have been received
				from the port over the course of a session being provided to a Framed
				User.  It can only be present in ACR messages with an Accounting-
				Record-Type of INTERIM_RECORD or STOP_RECORD.
			*/
			struct dict_avp_data data = { 
					365, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Input-Packets", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Accounting-Output-Packets */
		{
			/*
				The Accounting-Output-Packets (AVP Code 366) is of type Unsigned64
				and contains the number of IP packets sent to the user.

				For NAS usage, this AVP indicates how many packets have been sent to
				the port over the course of a session being provided to a Framed
				User.  It can only be present in ACR messages with an Accounting-
				Record-Type of INTERIM_RECORD or STOP_RECORD.
			*/
			struct dict_avp_data data = { 
					366, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Output-Packets", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Acct-Session-Time */
		{
			/*
				The Acct-Session-Time AVP (AVP Code 46) is of type Unsigned32 and
				indicates the length of the current session in seconds.  It can only
				be present in ACR messages with an Accounting-Record-Type of
				INTERIM_RECORD or STOP_RECORD.
			*/
			struct dict_avp_data data = { 
					46, 					/* Code */
					0, 					/* Vendor */
					"Acct-Session-Time", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Acct-Authentic */
		{
			/*
				The Acct-Authentic AVP (AVP Code 45) is of type Enumerated and
				specifies how the user was authenticated.  The supported values are
				listed in [RADIUSTypes].
				Sub-registry: Values for RADIUS Attribute 45, Acct-Authentic
				Reference: [RFC2866]
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Acct-Authentic)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 1, "RADIUS"),
				enumval_def_u32( 2, "Local"),
				enumval_def_u32( 3, "Remote"),
				enumval_def_u32( 4, "Diameter")
			};
			struct dict_avp_data data = { 
					45, 					/* Code */
					0, 					/* Vendor */
					"Acct-Authentic", 			/* Name */
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
		
		/* Accounting-Auth-Method */
		{
			/*
				The Accounting-Auth-Method AVP (AVP Code 406) is of type Enumerated.
				A NAS MAY include this AVP in an Accounting-Request message to
				indicate the method used to authenticate the user.  (Note that this
				is equivalent to the RADIUS MS-Acct-Auth-Type VSA attribute).

				The following values are defined:

				1  PAP
				2  CHAP
				3  MS-CHAP-1
				4  MS-CHAP-2
				5  EAP
				7  None
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Accounting-Auth-Method)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 1, "PAP"),
				enumval_def_u32( 2, "CHAP"),
				enumval_def_u32( 3, "MS-CHAP-1"),
				enumval_def_u32( 4, "MS-CHAP-2"),
				enumval_def_u32( 5, "EAP"),
				enumval_def_u32( 7, "None")
			};
			struct dict_avp_data data = { 
					406, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Auth-Method", 		/* Name */
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
		
		/* Acct-Delay-Time */
		{
			/*
				The Acct-Delay-Time AVP (AVP Code 41) is of type Unsigned32 and
				indicates the number of seconds the Diameter client has been trying
				to send the Accounting-Request (ACR).  The accounting server may
				subtract this value from the time when the ACR arrives at the server
				to calculate the approximate time of the event that caused the ACR to
				be generated.

				This AVP is not used for retransmissions at the transport level (TCP
				or SCTP).  Rather, it may be used when an ACR command cannot be
				transmitted because there is no appropriate peer to transmit it to or
				was rejected because it could not be delivered.  In these cases, the
				command MAY be buffered and transmitted later, when an appropriate
				peer-connection is available or after sufficient time has passed that
				the destination-host may be reachable and operational.  If the ACR is
				resent in this way, the Acct-Delay-Time AVP SHOULD be included.  The
				value of this AVP indicates the number of seconds that elapsed
				between the time of the first attempt at transmission and the current
				attempt.
			*/
			struct dict_avp_data data = { 
					41, 					/* Code */
					0, 					/* Vendor */
					"Acct-Delay-Time", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Acct-Link-Count */
		{
			/*
				The Acct-Link-Count AVP (AVP Code 51) is of type Unsigned32 and
				indicates the total number of links that have been active (current or
				closed) in a given multilink session at the time the accounting
				record is generated.  This AVP MAY be included in Accounting-Requests
				for any session that may be part of a multilink service.

				The Acct-Link-Count AVP may be used to make it easier for an
				accounting server to know when it has all the records for a given
				multilink service.  When the number of Accounting-Requests received
				with Accounting-Record-Type = STOP_RECORD and with the same Acct-
				Multi-Session-Id and unique Session-Ids equals the largest value of
				Acct-Link-Count seen in those Accounting-Requests, all STOP_RECORD
				Accounting-Requests for that multilink service have been received.

				The following example, showing eight Accounting-Requests, illustrates
				how the Acct-Link-Count AVP is used.  In the table below, only the
				relevant AVPs are shown, although additional AVPs containing
				accounting information will be present in the Accounting-Requests.

				Acct-Multi-                   Accounting-     Acct-
				Session-Id     Session-Id     Record-Type     Link-Count
				--------------------------------------------------------
				"...10"        "...10"      START_RECORD        1
				"...10"        "...11"      START_RECORD        2
				"...10"        "...11"      STOP_RECORD         2
				"...10"        "...12"      START_RECORD        3
				"...10"        "...13"      START_RECORD        4
				"...10"        "...12"      STOP_RECORD         4
				"...10"        "...13"      STOP_RECORD         4
				"...10"        "...10"      STOP_RECORD         4

			*/
			struct dict_avp_data data = { 
					51, 					/* Code */
					0, 					/* Vendor */
					"Acct-Link-Count", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Acct-Tunnel-Connection */
		{
			/*
				The Acct-Tunnel-Connection AVP (AVP Code 68) is of type OctetString
				and contains the identifier assigned to the tunnel session.  This
				AVP, along with the Tunnel-Client-Endpoint and Tunnel-Server-Endpoint
				AVPs, may be used to provide a means to uniquely identify a tunnel
				session for auditing purposes.

				The format of the identifier in this AVP depends upon the value of
				the Tunnel-Type AVP.  For example, to identify an L2TP tunnel
				connection fully, the L2TP Tunnel Id and Call Id might be encoded in
				this field.  The exact encoding of this field is implementation
				dependent.
			*/
			struct dict_avp_data data = { 
					68, 					/* Code */
					0, 					/* Vendor */
					"Acct-Tunnel-Connection", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Acct-Tunnel-Packets-Lost */
		{
			/*
				The Acct-Tunnel-Packets-Lost AVP (AVP Code 86) is of type Unsigned32
				and contains the number of packets lost on a given link.
			*/
			struct dict_avp_data data = { 
					86, 					/* Code */
					0, 					/* Vendor */
					"Acct-Tunnel-Packets-Lost", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
/***********************************
 *  Compatibility with RADIUS AVPs *
 ***********************************/
		/* NAS-Identifier */
		{
			/*
				The NAS-Identifier AVP (AVP Code 32) [RADIUS] is of type UTF8String
				and contains the identity of the NAS providing service to the user.
				This AVP SHOULD only be added by a RADIUS/Diameter Translation Agent.
				When this AVP is present, the Origin-Host AVP identifies the NAS
				providing service to the user.

				In RADIUS it would be possible for a rogue NAS to forge the NAS-
				Identifier attribute.  Diameter/RADIUS translation agents SHOULD
				attempt to check a received NAS-Identifier attribute against the
				source address of the RADIUS packet, by doing an A/AAAA RR query.  If
				the NAS-Identifier attribute contains an FQDN, then such a query
				would resolve to an IP address matching the source address.  However,
				the NAS-Identifier attribute is not required to contain an FQDN, so
				such a query could fail.  If it fails, an error should be logged, but
				no action should be taken, other than a reverse lookup on the source
				address and insert the resulting FQDN into the Route-Record AVP.

				Diameter agents and servers SHOULD check whether a NAS-Identifier AVP
				corresponds to an entry in the Route-Record AVP.  If no match is
				found, then an error is logged, but no other action is taken.
			*/
			struct dict_avp_data data = { 
					32, 					/* Code */
					0, 					/* Vendor */
					"NAS-Identifier", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* NAS-IP-Address */
		{
			/*
				The NAS-IP-Address AVP (AVP Code 4) [RADIUS] is of type OctetString
				and contains the IP Address of the NAS providing service to the user.
				This AVP SHOULD only be added by a RADIUS/Diameter Translation Agent.
				When this AVP is present, the Origin-Host AVP identifies the NAS
				providing service to the user.

				In RADIUS it would be possible for a rogue NAS to forge the NAS-IP-
				Address attribute value.  Diameter/RADIUS translation agents MUST
				check a received NAS-IP-Address or NAS-IPv6-Address attribute against
				the source address of the RADIUS packet.  If they do not match and
				the Diameter/RADIUS translation agent does not know whether the
				packet was sent by a RADIUS proxy or NAS (e.g., no Proxy-State
				attribute), then by default it is assumed that the source address
				corresponds to a RADIUS proxy, and that the NAS Address is behind
				that proxy, potentially with some additional RADIUS proxies in
				between.  The Diameter/RADIUS translation agent MUST insert entries
				in the Route-Record AVP corresponding to the apparent route.  This
				implies doing a reverse lookup on the source address and NAS-IP-
				Address or NAS-IPv6-Address attributes to determine the corresponding
				FQDNs.

				If the source address and the NAS-IP-Address or NAS-IPv6-Address do
				not match, and the Diameter/RADIUS translation agent knows that it is
				talking directly to the NAS (e.g., there are no RADIUS proxies
				between it and the NAS), then the error should be logged, and the
				packet MUST be discarded.

				Diameter agents and servers MUST check whether the NAS-IP-Address AVP
				corresponds to an entry in the Route-Record AVP.  This is done by
				doing a reverse lookup (PTR RR) for the NAS-IP-Address to retrieve
				the corresponding FQDN, and by checking for a match with the Route-
				Record AVP.  If no match is found, then an error is logged, but no
				other action is taken.
			*/
			struct dict_avp_data data = { 
					4, 					/* Code */
					0, 					/* Vendor */
					"NAS-IP-Address", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* NAS-IPv6-Address */
		{
			/*
				The NAS-IPv6-Address AVP (AVP Code 95) [RADIUSIPv6] is of type
				OctetString and contains the IPv6 Address of the NAS providing
				service to the user.  This AVP SHOULD only be added by a
				RADIUS/Diameter Translation Agent.  When this AVP is present, the
				Origin-Host AVP identifies the NAS providing service to the user.

				In RADIUS it would be possible for a rogue NAS to forge the NAS-
				IPv6-Address attribute.  Diameter/RADIUS translation agents MUST
				check a received NAS-IPv6-Address attribute against the source
				address of the RADIUS packet.  If they do not match and the
				Diameter/RADIUS translation agent does not know whether the packet
				was sent by a RADIUS proxy or NAS (e.g., no Proxy-State attribute),
				then by default it is assumed that the source address corresponds to
				a RADIUS proxy, and that the NAS-IPv6-Address is behind that proxy,
				potentially with some additional RADIUS proxies in between.  The
				Diameter/RADIUS translation agent MUST insert entries in the Route-
				Record AVP corresponding to the apparent route.  This implies doing a
				reverse lookup on the source address and NAS-IPv6-Address attributes
				to determine the corresponding FQDNs.

				If the source address and the NAS-IPv6-Address do not match, and the
				Diameter/RADIUS translation agent knows that it is talking directly
				to the NAS (e.g., there are no RADIUS proxies between it and the
				NAS), then the error should be logged, and the packet MUST be
				discarded.

				Diameter agents and servers MUST check whether the NAS-IPv6-Address
				AVP corresponds to an entry in the Route-Record AVP.  This is done by
				doing a reverse lookup (PTR RR) for the NAS-IPv6-Address to retrieve
				the corresponding FQDN, and by checking for a match with the Record-
				Route AVP.  If no match is found, then an error is logged, but no
				other action is taken.
			*/
			struct dict_avp_data data = { 
					95, 					/* Code */
					0, 					/* Vendor */
					"NAS-IPv6-Address", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* State */
		{
			/*
				The State AVP (AVP Code 24) [RADIUS] is of type OctetString and has
				two uses in the Diameter NAS application.

				The State AVP MAY be sent by a Diameter Server to a NAS in an AA-
				Response command that contains a Result-Code of
				DIAMETER_MULTI_ROUND_AUTH.  If so, the NAS MUST return it unmodified
				in the subsequent AA-Request command.

				The State AVP MAY also be sent by a Diameter Server to a NAS in an
				AA-Response command that also includes a Termination-Action AVP with
				the value of AA-REQUEST.  If the NAS performs the Termination-Action
				by sending a new AA-Request command upon termination of the current
				service, it MUST return the State AVP unmodified in the new request
				command.

				In either usage, the NAS MUST NOT interpret the AVP locally.  Usage
				of the State AVP is implementation dependent.
			*/
			struct dict_avp_data data = { 
					24, 					/* Code */
					0, 					/* Vendor */
					"State", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,		 	/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Termination-Cause mapping */
		{
			
			struct dict_object 	*type;
			struct dict_enumval_data tvals[] = {
				enumval_def_u32(11, "[RADIUS] User Request"),
				enumval_def_u32(12, "[RADIUS] Lost Carrier"),
				enumval_def_u32(13, "[RADIUS] Lost Service"),
				enumval_def_u32(14, "[RADIUS] Idle Timeout"),
				enumval_def_u32(15, "[RADIUS] Session Timeout"),
				enumval_def_u32(16, "[RADIUS] Admin Reset"),
				enumval_def_u32(17, "[RADIUS] Admin Reboot"),
				enumval_def_u32(18, "[RADIUS] Port Error"),
				enumval_def_u32(19, "[RADIUS] NAS Error"),
				enumval_def_u32(20, "[RADIUS] NAS Request"),
				enumval_def_u32(21, "[RADIUS] NAS Reboot"),
				enumval_def_u32(22, "[RADIUS] Port Unneeded"),
				enumval_def_u32(23, "[RADIUS] Port Preempted"),
				enumval_def_u32(24, "[RADIUS] Port Suspended"),
				enumval_def_u32(25, "[RADIUS] Service Unavailable"),
				enumval_def_u32(26, "[RADIUS] Callback"),
				enumval_def_u32(27, "[RADIUS] User Error"),
				enumval_def_u32(28, "[RADIUS] Host Request"),
				enumval_def_u32(29, "[RADIUS] Supplicant Restart"),
				enumval_def_u32(30, "[RADIUS] Reauthentication Failure"),
				enumval_def_u32(31, "[RADIUS] Port Reinit"),
				enumval_def_u32(32, "[RADIUS] Port Disabled")
				};
			int i;

			CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Enumerated(Termination-Cause)", &type);
			for (i = 0; i < sizeof(tvals) / sizeof(tvals[0]); i++) {
				CHECK_dict_new( DICT_ENUMVAL, &tvals[i], type, NULL);
			}
			
		}
		
		/* Origin-AAA-Protocol */
		{
			/*
				The Origin-AAA-Protocol AVP (AVP Code 408) is of the type Enumerated
				and should be inserted in a Diameter message translated by a gateway
				system from another AAA protocol, such as RADIUS.  It identifies the
				source protocol of the message to the Diameter system receiving the
				message.

				The supported values are:

				 1       RADIUS
			*/
			struct dict_object 	*type;
			struct dict_type_data 	 tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated(Origin-AAA-Protocol)"	, NULL, NULL};
			struct dict_enumval_data tvals[] = {
				enumval_def_u32( 1, "RADIUS")
			};
			struct dict_avp_data data = { 
					408, 					/* Code */
					0, 					/* Vendor */
					"Origin-AAA-Protocol", 			/* Name */
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
		
	}

/********************/		
/* Commands section */
/********************/		
	{
		/* To avoid defining global variables for all the AVP that we use here, we do search the dictionary in each sub-block.
		 * This is far from optimal, but the code is clearer like this, and the time it requires at execution is not noticeable.
		 */
		
		/* AA-Request (AAR) Command */
		{
			/*
				The AA-Request (AAR), which is indicated by setting the Command-Code
				field to 265 and the 'R' bit in the Command Flags field, is used to
				request authentication and/or authorization for a given NAS user.
				The type of request is identified through the Auth-Request-Type AVP
				[BASE].  The recommended value for most RADIUS interoperabily
				situations is AUTHORIZE_AUTHENTICATE.

				If Authentication is requested, the User-Name attribute SHOULD be
				present, as well as any additional authentication AVPs that would
				carry the password information.  A request for authorization SHOULD
				only include the information from which the authorization will be
				performed, such as the User-Name, Called-Station-Id, or Calling-
				Station-Id AVPs.  All requests SHOULD contain AVPs uniquely
				identifying the source of the call, such as Origin-Host and NAS-Port.
				Certain networks MAY use different AVPs for authorization purposes.
				A request for authorization will include some AVPs defined in section
				6.

				It is possible for a single session to be authorized first and then
				for an authentication request to follow.

				This AA-Request message MAY be the result of a multi-round
				authentication exchange, which occurs when the AA-Answer message is
				received with the Result-Code AVP set to DIAMETER_MULTI_ROUND_AUTH.
				A subsequent AAR message SHOULD be sent, with the User-Password AVP
				that includes the user's response to the prompt, and MUST include any
				State AVPs that were present in the AAA message.

				Message Format
				<AA-Request> ::= < Diameter Header: 265, REQ, PXY >
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
        				       [ Origin-AAA-Protocol ]
        				       [ Origin-State-Id ]
        				       [ Port-Limit ]
        				       [ User-Name ]
        				       [ User-Password ]
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
        				       [ CHAP-Auth ]
        				       [ CHAP-Challenge ]
        				     * [ Framed-Compression ]
        				       [ Framed-Interface-Id ]
        				       [ Framed-IP-Address ]
        				     * [ Framed-IPv6-Prefix ]
        				       [ Framed-IP-Netmask ]
        				       [ Framed-MTU ]
        				       [ Framed-Protocol ]
        				       [ ARAP-Password ]
        				       [ ARAP-Security ]
        				     * [ ARAP-Security-Data ]
        				     * [ Login-IP-Host ]
        				     * [ Login-IPv6-Host ]
        				       [ Login-LAT-Group ]
        				       [ Login-LAT-Node ]
        				       [ Login-LAT-Port ]
        				       [ Login-LAT-Service ]
        				     * [ Tunneling ]
        				     * [ Proxy-Info ]
        				     * [ Route-Record ]
        				     * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					265, 					/* Code */
					"AA-Request", 				/* Name */
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
							,{  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Port-Limit",			RULE_OPTIONAL,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "User-Password",			RULE_OPTIONAL,   -1, 1 }
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
							,{  "CHAP-Auth",			RULE_OPTIONAL,   -1, 1 }
							,{  "CHAP-Challenge",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Compression",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-Interface-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IP-Address",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPv6-Prefix",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-IP-Netmask",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-MTU",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Protocol",			RULE_OPTIONAL,   -1, 1 }
							,{  "ARAP-Password",			RULE_OPTIONAL,   -1, 1 }
							,{  "ARAP-Security",			RULE_OPTIONAL,   -1, 1 }
							,{  "ARAP-Security-Data",		RULE_OPTIONAL,   -1,-1 }
							,{  "Login-IP-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Login-IPv6-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Login-LAT-Group",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Node",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Port",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Service",		RULE_OPTIONAL,   -1, 1 }
							,{  "Tunneling",			RULE_OPTIONAL,   -1,-1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
							,{  "Route-Record",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , nasreq, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* AA-Answer (AAA) Command */
		{
			/*
				The AA-Answer (AAA) message is indicated by setting the Command-Code
				field to 265 and clearing the 'R' bit in the Command Flags field.  It
				is sent in response to the AA-Request (AAR) message.  If
				authorization was requested, a successful response will include the
				authorization AVPs appropriate for the service being provided, as
				defined in section 6.

				For authentication exchanges requiring more than a single round trip,
				the server MUST set the Result-Code AVP to DIAMETER_MULTI_ROUND_AUTH.
				An AAA message with this result code MAY include one Reply-Message or
				more and MAY include zero or one State AVPs.

				If the Reply-Message AVP was present, the network access server
				SHOULD send the text to the user's client to display to the user,
				instructing the client to prompt the user for a response.  For
				example, this capability can be achieved in PPP via PAP.  If the
				access client is unable to prompt the user for a new response, it
				MUST treat the AA-Answer (AAA) with the Reply-Message AVP as an error
				and deny access.

				Message Format

				<AA-Answer> ::= < Diameter Header: 265, PXY >
        				      < Session-Id >
        				      { Auth-Application-Id }
        				      { Auth-Request-Type }
        				      { Result-Code }
        				      { Origin-Host }
        				      { Origin-Realm }
        				      [ User-Name ]
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
        				      [ Multi-Round-Time-Out ]
        				      [ Session-Timeout ]
        				      [ State ]
        				    * [ Reply-Message ]
        				      [ Origin-AAA-Protocol ]
        				      [ Origin-State-Id ]
        				    * [ Filter-Id ]
        				      [ Password-Retry ]
        				      [ Port-Limit ]
        				      [ Prompt ]
        				      [ ARAP-Challenge-Response ]
        				      [ ARAP-Features ]
        				      [ ARAP-Security ]
        				    * [ ARAP-Security-Data ]
        				      [ ARAP-Zone-Access ]
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
        				    * [ Login-IP-Host ]
        				    * [ Login-IPv6-Host ]
        				      [ Login-LAT-Group ]
        				      [ Login-LAT-Node ]
        				      [ Login-LAT-Port ]
        				      [ Login-LAT-Service ]
        				      [ Login-Service ]
        				      [ Login-TCP-Port ]
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
					265, 					/* Code */
					"AA-Answer", 				/* Name */
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
							,{  "Multi-Round-Time-Out",		RULE_OPTIONAL,   -1, 1 }
							,{  "Session-Timeout",			RULE_OPTIONAL,   -1, 1 }
							,{  "State",				RULE_OPTIONAL,   -1, 1 }
							,{  "Reply-Message",			RULE_OPTIONAL,   -1,-1 }
							,{  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Filter-Id",			RULE_OPTIONAL,   -1,-1 }
							,{  "Password-Retry",			RULE_OPTIONAL,   -1, 1 }
							,{  "Port-Limit",			RULE_OPTIONAL,   -1, 1 }
							,{  "Prompt",				RULE_OPTIONAL,   -1, 1 }
							,{  "ARAP-Challenge-Response",		RULE_OPTIONAL,   -1, 1 }
							,{  "ARAP-Features",			RULE_OPTIONAL,   -1, 1 }
							,{  "ARAP-Security",			RULE_OPTIONAL,   -1, 1 }
							,{  "ARAP-Security-Data",		RULE_OPTIONAL,   -1,-1 }
							,{  "ARAP-Zone-Access",			RULE_OPTIONAL,   -1, 1 }
							,{  "Callback-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Callback-Number",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-AppleTalk-Link",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-AppleTalk-Network",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-AppleTalk-Zone",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Compression",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-Interface-Id",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-IP-Address",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPv6-Prefix",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-IPv6-Pool",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPv6-Route",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-IP-Netmask",		RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-Route",			RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-Pool",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPX-Network",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-MTU",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Protocol",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Routing",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-IP-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Login-IPv6-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Login-LAT-Group",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Node",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Port",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Service",		RULE_OPTIONAL,   -1, 1 }
							,{  "Login-Service",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-TCP-Port",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Filter-Rule",			RULE_OPTIONAL,   -1,-1 }
							,{  "QoS-Filter-Rule",			RULE_OPTIONAL,   -1,-1 }
							,{  "Tunneling",			RULE_OPTIONAL,   -1,-1 }
							,{  "Redirect-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Redirect-Host-Usage",		RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Max-Cache-Time",		RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , nasreq, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Re-Auth-Request */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):
				
			<RA-Request>  ::= < Diameter Header: 258, REQ, PXY >
        			       < Session-Id >
        			       { Origin-Host }
        			       { Origin-Realm }
        			       { Destination-Realm }
        			       { Destination-Host }
        			       { Auth-Application-Id }
        			       { Re-Auth-Request-Type }
        			       [ User-Name ]
        			       [ Origin-AAA-Protocol ]
        			       [ Origin-State-Id ]
        			       [ NAS-Identifier ]
        			       [ NAS-IP-Address ]
        			       [ NAS-IPv6-Address ]
        			       [ NAS-Port ]
        			       [ NAS-Port-Id ]
        			       [ NAS-Port-Type ]
        			       [ Service-Type ]
        			       [ Framed-IP-Address ]
        			       [ Framed-IPv6-Prefix ]
        			       [ Framed-Interface-Id ]
        			       [ Called-Station-Id ]
        			       [ Calling-Station-Id ]
        			       [ Originating-Line-Info ]
        			       [ Acct-Session-Id ]
        			       [ Acct-Multi-Session-Id ]
        			       [ State ]
        			     * [ Class ]
        			       [ Reply-Message ]
        			     * [ Proxy-Info ]
        			     * [ Route-Record ]
        			     * [ AVP ]
			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Identifier",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IP-Address",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IPv6-Address",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port",				RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Service-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IP-Address",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPv6-Prefix",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Interface-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Called-Station-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Calling-Station-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Originating-Line-Info",		RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Session-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Multi-Session-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "State",				RULE_OPTIONAL,   -1, 1 }
							,{  "Class",				RULE_OPTIONAL,   -1,-1 }
							,{  "Reply-Message",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Re-Auth-Request", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Re-Auth-Answer */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):
				

				<RA-Answer>  ::= < Diameter Header: 258, PXY >
        				       < Session-Id >
        				       { Result-Code }
        				       { Origin-Host }
        				       { Origin-Realm }
        				       [ User-Name ]
        				       [ Origin-AAA-Protocol ]
        				       [ Origin-State-Id ]
        				       [ Error-Message ]
        				       [ Error-Reporting-Host ]
        				     * [ Failed-AVP ]
        				     * [ Redirected-Host ]
        				       [ Redirected-Host-Usage ]
        				       [ Redirected-Host-Cache-Time ]
        				       [ Service-Type ]
        				     * [ Configuration-Token ]
        				       [ Idle-Timeout ]
        				       [ Authorization-Lifetime ]
        				       [ Auth-Grace-Period ]
        				       [ Re-Auth-Request-Type ]
        				       [ State ]
        				     * [ Class ]
        				     * [ Reply-Message ]
        				       [ Prompt ]
        				     * [ Proxy-Info ]
        				     * [ AVP ]
			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
							,{  "Service-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Configuration-Token",		RULE_OPTIONAL,   -1,-1 }
							,{  "Idle-Timeout",			RULE_OPTIONAL,   -1, 1 }
							,{  "Authorization-Lifetime",		RULE_OPTIONAL,   -1, 1 }
							,{  "Auth-Grace-Period",		RULE_OPTIONAL,   -1, 1 }
							,{  "Re-Auth-Request-Type",		RULE_OPTIONAL,   -1, 1 }
							,{  "State",				RULE_OPTIONAL,   -1, 1 }
							,{  "Class",				RULE_OPTIONAL,   -1,-1 }
							,{  "Reply-Message",			RULE_OPTIONAL,   -1,-1 }
							,{  "Prompt",				RULE_OPTIONAL,   -1, 1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Re-Auth-Answer", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Session-Termination-Request */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):
				

				<ST-Request> ::= < Diameter Header: 275, REQ, PXY >
        				      < Session-Id >
        				      { Origin-Host }
        				      { Origin-Realm }
        				      { Destination-Realm }
        				      { Auth-Application-Id }
        				      { Termination-Cause }
        				      [ User-Name ]
        				      [ Destination-Host ]
        				    * [ Class ]
        				      [ Origin-AAA-Protocol ]
        				      [ Origin-State-Id ]
        				    * [ Proxy-Info ]
        				    * [ Route-Record ]
        				    * [ AVP ]
			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Session-Termination-Request", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Session-Termination-Answer */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):
				
				<ST-Answer>  ::= < Diameter Header: 275, PXY >
        				       < Session-Id >
        				       { Result-Code }
        				       { Origin-Host }
        				       { Origin-Realm }
        				       [ User-Name ]
        				     * [ Class ]
        				       [ Error-Message ]
        				       [ Error-Reporting-Host ]
        				     * [ Failed-AVP ]
        				       [ Origin-AAA-Protocol ]
        				       [ Origin-State-Id ]
        				     * [ Redirect-Host ]
        				       [ Redirect-Host-Usase ]
        				       [ Redirect-Max-Cache-Time ]
        				     * [ Proxy-Info ]
        				     * [ AVP ]
			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Session-Termination-Answer", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Abort-Session-Request */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):
				
			<AS-Request>  ::= < Diameter Header: 274, REQ, PXY >
        			       < Session-Id >
        			       { Origin-Host }
        			       { Origin-Realm }
        			       { Destination-Realm }
        			       { Destination-Host }
        			       { Auth-Application-Id }
        			       [ User-Name ]
        			       [ Origin-AAA-Protocol ]
        			       [ Origin-State-Id ]
        			       [ NAS-Identifier ]
        			       [ NAS-IP-Address ]
        			       [ NAS-IPv6-Address ]
        			       [ NAS-Port ]
        			       [ NAS-Port-Id ]
        			       [ NAS-Port-Type ]
        			       [ Service-Type ]
        			       [ Framed-IP-Address ]
        			       [ Framed-IPv6-Prefix ]
        			       [ Framed-Interface-Id ]
        			       [ Called-Station-Id ]
        			       [ Calling-Station-Id ]
        			       [ Originating-Line-Info ]
        			       [ Acct-Session-Id ]
        			       [ Acct-Multi-Session-Id ]
        			       [ State ]
        			     * [ Class ]
        			     * [ Reply-Message ]
        			     * [ Proxy-Info ]
        			     * [ Route-Record ]
        			     * [ AVP ]
				     
			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Identifier",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IP-Address",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IPv6-Address",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port",				RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Service-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IP-Address",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-IPv6-Prefix",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Interface-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Called-Station-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Calling-Station-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Originating-Line-Info",		RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Session-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Multi-Session-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "State",				RULE_OPTIONAL,   -1, 1 }
							,{  "Class",				RULE_OPTIONAL,   -1,-1 }
							,{  "Reply-Message",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Abort-Session-Request", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Abort-Session-Answer */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):

				<AS-Answer>  ::= < Diameter Header: 274, PXY >
        				       < Session-Id >
        				       { Result-Code }
        				       { Origin-Host }
        				       { Origin-Realm }
        				       [ User-Name ]
        				       [ Origin-AAA-Protocol ]
        				       [ Origin-State-Id ]
        				       [ State]
        				       [ Error-Message ]
        				       [ Error-Reporting-Host ]
        				     * [ Failed-AVP ]
        				     * [ Redirected-Host ]
        				       [ Redirected-Host-Usage ]
        				       [ Redirected-Max-Cache-Time ]
        				     * [ Proxy-Info ]
        				     * [ AVP ]
			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
							,{  "State", 				RULE_OPTIONAL,   -1, 1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Abort-Session-Answer", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Accounting-Request */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):

				<AC-Request> ::= < Diameter Header: 271, REQ, PXY >
        				      < Session-Id >
        				      { Origin-Host }
        				      { Origin-Realm }
        				      { Destination-Realm }
        				      { Accounting-Record-Type }
        				      { Accounting-Record-Number }
        				      [ Acct-Application-Id ]
        				      [ Vendor-Specific-Application-Id ]
        				      [ User-Name ]
        				      [ Accounting-Sub-Session-Id ]
        				      [ Acct-Session-Id ]
        				      [ Acct-Multi-Session-Id ]
        				      [ Origin-AAA-Protocol ]
        				      [ Origin-State-Id ]
        				      [ Destination-Host ]
        				      [ Event-Timestamp ]
        				      [ Acct-Delay-Time ]
        				      [ NAS-Identifier ]
        				      [ NAS-IP-Address ]
        				      [ NAS-IPv6-Address ]
        				      [ NAS-Port ]
        				      [ NAS-Port-Id ]
        				      [ NAS-Port-Type ]
        				    * [ Class ]
        				      [ Service-Type ]
        				      [ Termination-Cause ]
        				      [ Accounting-Input-Octets ]
        				      [ Accounting-Input-Packets ]
        				      [ Accounting-Output-Octets ]
        				      [ Accounting-Output-Packets ]
        				      [ Acct-Authentic ]
        				      [ Accounting-Auth-Method ]
        				      [ Acct-Link-Count ]
        				      [ Acct-Session-Time ]
        				      [ Acct-Tunnel-Connection ]
        				      [ Acct-Tunnel-Packets-Lost ]
        				      [ Callback-Id ]
        				      [ Callback-Number ]
        				      [ Called-Station-Id ]
        				      [ Calling-Station-Id ]
        				    * [ Connection-Info ]
        				      [ Originating-Line-Info ]
        				      [ Authorization-Lifetime ]
        				      [ Session-Timeout ]
        				      [ Idle-Timeout ]
        				      [ Port-Limit ]
        				      [ Accounting-Realtime-Required ]
        				      [ Acct-Interim-Interval ]
        				    * [ Filter-Id ]
        				    * [ NAS-Filter-Rule ]
        				    * [ Qos-Filter-Rule ]
        				      [ Framed-AppleTalk-Link ]
        				      [ Framed-AppleTalk-Network ]
        				      [ Framed-AppleTalk-Zone ]
        				      [ Framed-Compression ]
        				      [ Framed-Interface-Id ]
        				      [ Framed-IP-Address ]
        				      [ Framed-IP-Netmask ]
        				    * [ Framed-IPv6-Prefix ]
        				      [ Framed-IPv6-Pool ]
        				    * [ Framed-IPv6-Route ]
        				      [ Framed-IPX-Network ]
        				      [ Framed-MTU ]
        				      [ Framed-Pool ]
        				      [ Framed-Protocol ]
        				    * [ Framed-Route ]
        				      [ Framed-Routing ]
        				    * [ Login-IP-Host ]
        				    * [ Login-IPv6-Host ]
        				      [ Login-LAT-Group ]
        				      [ Login-LAT-Node ]
        				      [ Login-LAT-Port ]
        				      [ Login-LAT-Service ]
        				      [ Login-Service ]
        				      [ Login-TCP-Port ]
        				    * [ Tunneling ]
        				    * [ Proxy-Info ]
        				    * [ Route-Record ]
        				    * [ AVP ]
			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Delay-Time",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Identifier",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IP-Address",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IPv6-Address",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port",				RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Class",				RULE_OPTIONAL,   -1,-1 }
							,{  "Service-Type",			RULE_OPTIONAL,   -1, 1 }
							,{  "Termination-Cause",		RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Input-Octets",		RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Input-Packets",		RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Output-Octets",		RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Output-Packets",	RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Authentic",			RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Auth-Method",		RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Link-Count",			RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Session-Time",		RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Tunnel-Connection",		RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Tunnel-Packets-Lost",		RULE_OPTIONAL,   -1, 1 }
							,{  "Callback-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Callback-Number",			RULE_OPTIONAL,   -1, 1 }
							,{  "Called-Station-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Calling-Station-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Connect-Info",			RULE_OPTIONAL,   -1,-1 }
							,{  "Originating-Line-Info",		RULE_OPTIONAL,   -1, 1 }
							,{  "Authorization-Lifetime",		RULE_OPTIONAL,   -1, 1 }
							,{  "Session-Timeout",			RULE_OPTIONAL,   -1, 1 }
							,{  "Idle-Timeout",			RULE_OPTIONAL,   -1, 1 }
							,{  "Port-Limit",			RULE_OPTIONAL,   -1, 1 }
							,{  "Filter-Id",			RULE_OPTIONAL,   -1,-1 }
							,{  "NAS-Filter-Rule",			RULE_OPTIONAL,   -1,-1 }
							,{  "QoS-Filter-Rule",			RULE_OPTIONAL,   -1,-1 }
							,{  "Framed-AppleTalk-Link",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-AppleTalk-Network",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-AppleTalk-Zone",		RULE_OPTIONAL,   -1, 1 }
							,{  "Framed-Compression",		RULE_OPTIONAL,   -1, 1 }
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
							,{  "Login-IP-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Login-IPv6-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Login-LAT-Group",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Node",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Port",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-LAT-Service",		RULE_OPTIONAL,   -1, 1 }
							,{  "Login-Service",			RULE_OPTIONAL,   -1, 1 }
							,{  "Login-TCP-Port",			RULE_OPTIONAL,   -1, 1 }
							,{  "Tunneling",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Accounting-Request", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Accounting-Answer */
		{
			/*
				Add additional rules of the ABNF (compared to Base definition):

				<AC-Answer> ::= < Diameter Header: 271, PXY >
        				      < Session-Id >
        				      { Result-Code }
        				      { Origin-Host }
        				      { Origin-Realm }
        				      { Accounting-Record-Type }
        				      { Accounting-Record-Number }
        				      [ Acct-Application-Id ]
        				      [ Vendor-Specific-Application-Id ]
        				      [ User-Name ]
        				      [ Accounting-Sub-Session-Id ]
        				      [ Acct-Session-Id ]
        				      [ Acct-Multi-Session-Id ]
        				      [ Event-Timestamp ]
        				      [ Error-Message ]
        				      [ Error-Reporting-Host ]
        				    * [ Failed-AVP ]
        				      [ Origin-AAA-Protocol ]
        				      [ Origin-State-Id ]
        				      [ NAS-Identifier ]
        				      [ NAS-IP-Address ]
        				      [ NAS-IPv6-Address ]
        				      [ NAS-Port ]
        				      [ NAS-Port-Id ]
        				      [ NAS-Port-Type ]
        				      [ Service-Type ]
        				      [ Termination-Cause ]
        				      [ Accounting-Realtime-Required ]
        				      [ Acct-Interim-Interval ]
        				    * [ Class ]
        				    * [ Proxy-Info ]
        				    * [ Route-Record ]
        				    * [ AVP ]

			*/
			struct dict_object * cmd;
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-AAA-Protocol",		RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Identifier", 			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IP-Address", 			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-IPv6-Address", 		RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port", 			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Id", 			RULE_OPTIONAL,   -1, 1 }
							,{  "NAS-Port-Type", 			RULE_OPTIONAL,   -1, 1 }
							,{  "Service-Type", 			RULE_OPTIONAL,   -1, 1 }
							,{  "Termination-Cause", 		RULE_OPTIONAL,   -1, 1 }
						};
			
			CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Accounting-Answer", &cmd);
			PARSE_loc_rules( rules, cmd );
		}
	}
	
	LOG_D( "Extension 'Dictionary definitions for NASREQ' initialized");
	return 0;
}

EXTENSION_ENTRY("dict_nasreq", dnr_entry);
