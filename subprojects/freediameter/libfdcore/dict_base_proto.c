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

/* Diameter Base protocol definitions.
 */

#include "fdcore-internal.h"

#include <netinet/in.h>
#include <sys/socket.h>

/* The pointer for the global dictionary (initialized from main) */
struct dictionary * fd_g_dict = NULL;



#define CHECK_dict_new( _type, _data, _parent, _ref )				\
	CHECK_FCT(  fd_dict_new( dict, (_type), (_data), (_parent), (_ref))  );

#define CHECK_dict_search( _type, _criteria, _what, _result )					\
	CHECK_FCT(  fd_dict_search( dict, (_type), (_criteria), (_what), (_result), ENOENT)  );

struct local_rules_definition {
	char 			*avp_name;
	enum rule_position	position;
	int 			min;
	int			max;
};

#define RULE_ORDER( _position ) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0 )

#define PARSE_loc_rules( _rulearray, _parent) {						\
	int __ar;									\
	for (__ar=0; __ar < sizeof(_rulearray) / sizeof((_rulearray)[0]); __ar++) {	\
		struct dict_rule_data __data = { NULL, 					\
			(_rulearray)[__ar].position,					\
			0, 								\
			(_rulearray)[__ar].min,						\
			(_rulearray)[__ar].max};					\
		__data.rule_order = RULE_ORDER(__data.rule_position);			\
		CHECK_FCT(  fd_dict_search( 						\
			dict,								\
			DICT_AVP, 							\
			AVP_BY_NAME, 							\
			(_rulearray)[__ar].avp_name, 					\
			&__data.rule_avp, 0 ) );					\
		if ( !__data.rule_avp ) {						\
			TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_name );	\
			return ENOENT;							\
		}									\
		CHECK_FCT_DO( fd_dict_new( dict, DICT_RULE, &__data, _parent, NULL),	\
			{								\
				TRACE_DEBUG(INFO, "Error on rule with AVP '%s'",	\
					 (_rulearray)[__ar].avp_name );			\
				return EINVAL;						\
			} );								\
	}										\
}

int fd_dict_base_protocol(struct dictionary * dict)
{
	TRACE_ENTRY("%p", dict);
	CHECK_PARAMS(dict);
	
	/* Vendors section */
	{
		/* The base RFC has no vendor information */
		;
	}
	
	/* Applications section */
	{
		/* base accounting application */
		{
			struct dict_application_data data = {          3, "Diameter Base Accounting" 	};
			CHECK_dict_new( DICT_APPLICATION, &data, NULL, NULL);
		}
		
		/* relay application */
		{
			struct dict_application_data data  = { 0xffffffff, "Relay" 				};
			#if AI_RELAY != 0xffffffff
			#error "AI_RELAY definition mismatch"
			#endif
			CHECK_dict_new( DICT_APPLICATION, &data , NULL, NULL);
		}
	}
	
	/* Derived AVP types section */
	{
		/* Address */
		{
			/*
				The Address format is derived from the OctetString AVP Base
				Format.  It is a discriminated union, representing, for example a
				32-bit (IPv4) [RFC791] or 128-bit (IPv6) [RFC4291] address, most
				significant octet first.  The first two octets of the Address AVP
				represents the AddressType, which contains an Address Family
				defined in [IANAADFAM].  The AddressType is used to discriminate
				the content and format of the remaining octets.
			*/
			struct dict_type_data data = { AVP_TYPE_OCTETSTRING,	"Address"		, fd_dictfct_Address_interpret	, fd_dictfct_Address_encode,	fd_dictfct_Address_dump	};
			CHECK_dict_new( DICT_TYPE, &data , NULL, NULL);
		}
		
		/* Time */
		{
			/*
				The Time format is derived from the OctetString AVP Base Format.
				The string MUST contain four octets, in the same format as the
				first four bytes are in the NTP timestamp format.  The NTP
				Timestamp format is defined in chapter 3 of [RFC4330].

				This represents the number of seconds since 0h on 1 January 1900
				with respect to the Coordinated Universal Time (UTC).

				On 6h 28m 16s UTC, 7 February 2036 the time value will overflow.
				SNTP [RFC4330] describes a procedure to extend the time to 2104.
				This procedure MUST be supported by all DIAMETER nodes.
			*/
			struct dict_type_data data = { AVP_TYPE_OCTETSTRING,	"Time"			, fd_dictfct_Time_interpret	, fd_dictfct_Time_encode, 	fd_dictfct_Time_dump		};
			CHECK_dict_new( DICT_TYPE, &data , NULL, NULL);
		}
		
		/* UTF8String */
		{
			/*
				The UTF8String format is derived from the OctetString AVP Base
				Format.  This is a human readable string represented using the
				ISO/IEC IS 10646-1 character set, encoded as an OctetString using
				the UTF-8 [RFC3629] transformation format described in RFC 3629.

				Since additional code points are added by amendments to the 10646
				standard from time to time, implementations MUST be prepared to
				encounter any code point from 0x00000001 to 0x7fffffff.  Byte
				sequences that do not correspond to the valid encoding of a code
				point into UTF-8 charset or are outside this range are prohibited.

				The use of control codes SHOULD be avoided.  When it is necessary
				to represent a new line, the control code sequence CR LF SHOULD be
				used.

				The use of leading or trailing white space SHOULD be avoided.

				For code points not directly supported by user interface hardware
				or software, an alternative means of entry and display, such as
				hexadecimal, MAY be provided.

				For information encoded in 7-bit US-ASCII, the UTF-8 charset is
				identical to the US-ASCII charset.

				UTF-8 may require multiple bytes to represent a single character /
				code point; thus the length of an UTF8String in octets may be
				different from the number of characters encoded.

				Note that the AVP Length field of an UTF8String is measured in
				octets, not characters.
			*/
			struct dict_type_data data = { AVP_TYPE_OCTETSTRING,	"UTF8String"		, NULL			, NULL	, fd_dictfct_UTF8String_dump	};
			CHECK_dict_new( DICT_TYPE, &data , NULL, NULL);
		}
		
		/* DiameterIdentity */
		{
			/*
				The DiameterIdentity format is derived from the OctetString AVP
				Base Format.

                				DiameterIdentity  = FQDN


				DiameterIdentity value is used to uniquely identify a Diameter
				node for purposes of duplicate connection and routing loop
				detection.

				The contents of the string MUST be the FQDN of the Diameter node.
				If multiple Diameter nodes run on the same host, each Diameter
				node MUST be assigned a unique DiameterIdentity.  If a Diameter

				node can be identified by several FQDNs, a single FQDN should be
				picked at startup, and used as the only DiameterIdentity for that
				node, whatever the connection it is sent on.  Note that in this
				document, DiameterIdentity is in ASCII form in order to be
				compatible with existing DNS infrastructure.  See Appendix D for
				interactions between the Diameter protocol and Internationalized
				Domain Name (IDNs).
			*/
			struct dict_type_data data = { AVP_TYPE_OCTETSTRING,	"DiameterIdentity"	, NULL			, NULL		, fd_dictfct_UTF8String_dump	};
			CHECK_dict_new( DICT_TYPE, &data , NULL, NULL);
		}
		
		/* DiameterURI */
		{
			/*
				The DiameterURI MUST follow the Uniform Resource Identifiers (URI)
				syntax [RFC3986] rules specified below:

				 "aaa://" FQDN [ port ] [ transport ] [ protocol ]

                				 ; No transport security

				 "aaas://" FQDN [ port ] [ transport ] [ protocol ]

                				 ; Transport security used

				 FQDN               = Fully Qualified Host Name

				 port               = ":" 1*DIGIT

                				 ; One of the ports used to listen for
                				 ; incoming connections.
                				 ; If absent,
                				 ; the default Diameter port (3868) is
                				 ; assumed.

				 transport          = ";transport=" transport-protocol

                				 ; One of the transports used to listen
                				 ; for incoming connections.  If absent,
                				 ; the default SCTP [RFC2960] protocol is
                				 ; assumed. UDP MUST NOT be used when
                				 ; the aaa-protocol field is set to
                				 ; diameter.

				transport-protocol = ( "tcp" / "sctp" / "udp" )

				protocol           = ";protocol=" aaa-protocol

                				 ; If absent, the default AAA protocol
                				 ; is diameter.

				aaa-protocol       = ( "diameter" / "radius" / "tacacs+" )

				The following are examples of valid Diameter host identities:

				aaa://host.example.com;transport=tcp
				aaa://host.example.com:6666;transport=tcp
				aaa://host.example.com;protocol=diameter
				aaa://host.example.com:6666;protocol=diameter
				aaa://host.example.com:6666;transport=tcp;protocol=diameter
				aaa://host.example.com:1813;transport=udp;protocol=radius
			*/
			struct dict_type_data data = { AVP_TYPE_OCTETSTRING,	"DiameterURI"		, NULL			, NULL		, fd_dictfct_UTF8String_dump	};
			CHECK_dict_new( DICT_TYPE, &data , NULL, NULL);
		}
		
		/* Enumerated */
		{
			/*
				Enumerated is derived from the Integer32 AVP Base Format.  The
				definition contains a list of valid values and their
				interpretation and is described in the Diameter application
				introducing the AVP.
			*/
			
			/* We don't use a generic "Enumerated" type in freeDiameter. Instead, we define
			 * types of the form "Enumerated(<avpname>)" where <avpname> is replaced 
			 * by the name of the AVP to which the type applies.
			 *  Example: Enumerated(Disconnect-Cause)
			 */
			;
		}
		
		/* IPFilterRule */
		{
			/*
				The IPFilterRule format is derived from the OctetString AVP Base
				Format and uses the ASCII charset.  The rule syntax is a modified
				subset of ipfw(8) from FreeBSD.  Packets may be filtered based on
				the following information that is associated with it:

				    Direction                          (in or out)
				    Source and destination IP address  (possibly masked)
				    Protocol
				    Source and destination port        (lists or ranges)
				    TCP flags
				    IP fragment flag
				    IP options
				    ICMP types

				Rules for the appropriate direction are evaluated in order, with
				the first matched rule terminating the evaluation.  Each packet is
				evaluated once.  If no rule matches, the packet is dropped if the
				last rule evaluated was a permit, and passed if the last rule was
				a deny.

				IPFilterRule filters MUST follow the format:
				
				    action dir proto from src to dst [options]
				
			(...skipped loooong explanation...)
				
				There is one kind of packet that the access device MUST always
				discard, that is an IP fragment with a fragment offset of one.
				This is a valid packet, but it only has one use, to try to
				circumvent firewalls.

				An access device that is unable to interpret or apply a deny rule
				MUST terminate the session.  An access device that is unable to
				interpret or apply a permit rule MAY apply a more restrictive
				rule.  An access device MAY apply deny rules of its own before the
				supplied rules, for example to protect the access device owner's
				infrastructure.
			*/
			struct dict_type_data data = { AVP_TYPE_OCTETSTRING,	"IPFilterRule"		, NULL			, NULL		, fd_dictfct_UTF8String_dump	};
			CHECK_dict_new( DICT_TYPE, &data , NULL, NULL);
		}
	}
	
	/* AVP section */
	{
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
		
		/* Vendor-Id */
		{
			/*
				The Vendor-Id AVP (AVP Code 266) is of type Unsigned32 and contains
				the IANA "SMI Network Management Private Enterprise Codes" [RFC3232]
				value assigned to the vendor of the Diameter device.  It is
				envisioned that the combination of the Vendor-Id, Product-Name
				(Section 5.3.7) and the Firmware-Revision (Section 5.3.4) AVPs may
				provide useful debugging information.

				A Vendor-Id value of zero in the CER or CEA messages is reserved and
				indicates that this field is ignored.
			*/
			struct dict_avp_data data = { 
					266, 					/* Code */
					#if AC_VENDOR_ID != 266
					#error "AC_VENDOR_ID definition mismatch"
					#endif
					0, 					/* Vendor */
					"Vendor-Id", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY, 			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data, NULL, NULL);
		}
		
		/* Firmware-Revision */
		{
			/*
				The Firmware-Revision AVP (AVP Code 267) is of type Unsigned32 and is
				used to inform a Diameter peer of the firmware revision of the
				issuing device.

				For devices that do not have a firmware revision (general purpose
				computers running Diameter software modules, for instance), the
				revision of the Diameter software module may be reported instead.
			*/
			struct dict_avp_data data = { 
					267, 					/* Code */
					#if AC_FIRMWARE_REVISION != 267
					#error "AC_FIRMWARE_REVISION definition mismatch"
					#endif
					0, 					/* Vendor */
					"Firmware-Revision", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					0,		 			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Host-IP-Address */
		{
			/*
				The Host-IP-Address AVP (AVP Code 257) is of type Address and is used
				to inform a Diameter peer of the sender's IP address.  All source
				addresses that a Diameter node expects to use with SCTP [RFC2960]
				MUST be advertised in the CER and CEA messages by including a
				Host-IP- Address AVP for each address.  This AVP MUST ONLY be used in
				the CER and CEA messages.
			*/
			struct dict_avp_data data = { 
					257, 					/* Code */
					#if AC_HOST_IP_ADDRESS != 257
					#error "AC_HOST_IP_ADDRESS definition mismatch"
					#endif
					0, 					/* Vendor */
					"Host-IP-Address", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
		}
		
		/* Supported-Vendor-Id */
		{
			/*
				The Supported-Vendor-Id AVP (AVP Code 265) is of type Unsigned32 and
				contains the IANA "SMI Network Management Private Enterprise Codes"
				[RFC3232] value assigned to a vendor other than the device vendor but
				including the application vendor.  This is used in the CER and CEA
				messages in order to inform the peer that the sender supports (a
				subset of) the vendor-specific AVPs defined by the vendor identified
				in this AVP.  The value of this AVP SHOULD NOT be set to zero.
				Multiple instances of this AVP containing the same value SHOULD NOT
				be sent.
			*/
			struct dict_avp_data data = { 
					265, 					/* Code */
					#if AC_SUPPORTED_VENDOR_ID != 265
					#error "AC_SUPPORTED_VENDOR_ID definition mismatch"
					#endif
					0, 					/* Vendor */
					"Supported-Vendor-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Product-Name */
		{
			/*
				The Product-Name AVP (AVP Code 269) is of type UTF8String, and
				contains the vendor assigned name for the product.  The Product-Name
				AVP SHOULD remain constant across firmware revisions for the same
				product.
			*/
			struct dict_avp_data data = { 
					269, 					/* Code */
					#if AC_PRODUCT_NAME != 269
					#error "AC_PRODUCT_NAME definition mismatch"
					#endif
					0, 					/* Vendor */
					"Product-Name", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					0,					/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Disconnect-Cause */
		{
			/*
				The Disconnect-Cause AVP (AVP Code 273) is of type Enumerated.  A
				Diameter node MUST include this AVP in the Disconnect-Peer-Request
				message to inform the peer of the reason for its intention to
				shutdown the transport connection.  The following values are
				supported:

				REBOOTING                         0
				A scheduled reboot is imminent. Receiver of DPR with above result
				code MAY attempt reconnection.

				BUSY                              1
				The peer's internal resources are constrained, and it has
				determined that the transport connection needs to be closed.
				Receiver of DPR with above result code SHOULD NOT attempt
				reconnection.

				DO_NOT_WANT_TO_TALK_TO_YOU        2
				The peer has determined that it does not see a need for the
				transport connection to exist, since it does not expect any
				messages to be exchanged in the near future. Receiver of DPR
				with above result code SHOULD NOT attempt reconnection.
			*/
			struct dict_object 	* 	type;
			struct dict_type_data 		tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Disconnect-Cause)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "REBOOTING", 			{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "BUSY", 			{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "DO_NOT_WANT_TO_TALK_TO_YOU", 	{ .i32 = 2 }};
			struct dict_avp_data 		data = { 
					273, 					/* Code */
					#if AC_DISCONNECT_CAUSE != 273
					#error "AC_DISCONNECT_CAUSE definition mismatch"
					#endif
					0, 					/* Vendor */
					"Disconnect-Cause", 			/* Name */
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
		
		/* Origin-Host */
		{
			/*
				The Origin-Host AVP (AVP Code 264) is of type DiameterIdentity, and
				MUST be present in all Diameter messages.  This AVP identifies the
				endpoint that originated the Diameter message.  Relay agents MUST NOT
				modify this AVP.

				The value of the Origin-Host AVP is guaranteed to be unique within a
				single host.

				Note that the Origin-Host AVP may resolve to more than one address as
				the Diameter peer may support more than one address.

				This AVP SHOULD be placed as close to the Diameter header as
				possible.
			*/
			struct dict_avp_data data = { 
					264, 					/* Code */
					#if AC_ORIGIN_HOST != 264
					#error "AC_ORIGIN_HOST definition mismatch"
					#endif
					0, 					/* Vendor */
					"Origin-Host", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterIdentity_type, NULL);
		}
		
		/* Origin-Realm */
		{
			/*
				The Origin-Realm AVP (AVP Code 296) is of type DiameterIdentity.
				This AVP contains the Realm of the originator of any Diameter message
				and MUST be present in all messages.

				This AVP SHOULD be placed as close to the Diameter header as
				possible.
			*/
			struct dict_avp_data data = { 
					296, 					/* Code */
					#if AC_ORIGIN_REALM != 296
					#error "AC_ORIGIN_REALM definition mismatch"
					#endif
					0, 					/* Vendor */
					"Origin-Realm", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterIdentity_type, NULL);
		}
		
		/* Destination-Host */
		{
			/*
				The Destination-Host AVP (AVP Code 293) is of type DiameterIdentity.
				This AVP MUST be present in all unsolicited agent initiated messages,
				MAY be present in request messages, and MUST NOT be present in Answer
				messages.

				The absence of the Destination-Host AVP will cause a message to be
				sent to any Diameter server supporting the application within the
				realm specified in Destination-Realm AVP.

				This AVP SHOULD be placed as close to the Diameter header as
				possible.
			*/
			struct dict_avp_data data = { 
					293, 					/* Code */
					#if AC_DESTINATION_HOST != 293
					#error "AC_DESTINATION_HOST definition mismatch"
					#endif
					0, 					/* Vendor */
					"Destination-Host", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterIdentity_type, NULL);
		}
		
		/* Destination-Realm */
		{
			/*
				The Destination-Realm AVP (AVP Code 283) is of type DiameterIdentity,
				and contains the realm the message is to be routed to.  The
				Destination-Realm AVP MUST NOT be present in Answer messages.
				Diameter Clients insert the realm portion of the User-Name AVP.
				Diameter servers initiating a request message use the value of the
				Origin-Realm AVP from a previous message received from the intended
				target host (unless it is known a priori).  When present, the
				Destination-Realm AVP is used to perform message routing decisions.

				Request messages whose ABNF does not list the Destination-Realm AVP
				as a mandatory AVP are inherently non-routable messages.

				This AVP SHOULD be placed as close to the Diameter header as
				possible.
			*/
			struct dict_avp_data data = { 
					283, 					/* Code */
					#if AC_DESTINATION_REALM != 283
					#error "AC_DESTINATION_REALM definition mismatch"
					#endif
					0, 					/* Vendor */
					"Destination-Realm", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterIdentity_type, NULL);
		}
		
		/* Route-Record */
		{
			/*
				The Route-Record AVP (AVP Code 282) is of type DiameterIdentity.  The
				identity added in this AVP MUST be the same as the one received in
				the Origin-Host of the Capabilities Exchange message.
			*/
			struct dict_avp_data data = { 
					282, 					/* Code */
					#if AC_ROUTE_RECORD != 282
					#error "AC_ROUTE_RECORD definition mismatch"
					#endif
					0, 					/* Vendor */
					"Route-Record", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterIdentity_type, NULL);
		}
		
		/* Proxy-Host */
		{
			/*
				The Proxy-Host AVP (AVP Code 280) is of type DiameterIdentity.  This
				AVP contains the identity of the host that added the Proxy-Info AVP.
			*/
			struct dict_avp_data adata = { 
					280, 					/* Code */
					#if AC_PROXY_HOST != 280
					#error "AC_PROXY_HOST definition mismatch"
					#endif
					0, 					/* Vendor */
					"Proxy-Host", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , DiameterIdentity_type, NULL);
		}
		
		/* Proxy-State */
		{
			/*
				The Proxy-State AVP (AVP Code 33) is of type OctetString, and
				contains state local information, and MUST be treated as opaque data.
			*/
			struct dict_avp_data adata = { 
					33, 					/* Code */
					#if AC_PROXY_STATE != 33
					#error "AC_PROXY_STATE definition mismatch"
					#endif
					0, 					/* Vendor */
					"Proxy-State", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &adata , NULL, NULL);
		}
			
		/* Proxy-Info */
		{
			/*
				The Proxy-Info AVP (AVP Code 284) is of type Grouped.  The Grouped
				Data field has the following ABNF grammar:

				 Proxy-Info ::= < AVP Header: 284 >
                				{ Proxy-Host }
                				{ Proxy-State }
        				      * [ AVP ]
			*/
			struct dict_object * avp;
			struct dict_avp_data data = { 
					284, 					/* Code */
					#if AC_PROXY_INFO != 284
					#error "AC_PROXY_INFO definition mismatch"
					#endif
					0, 					/* Vendor */
					"Proxy-Info", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Proxy-Host", 			RULE_REQUIRED, -1, 1 }
							,{  "Proxy-State",			RULE_REQUIRED, -1, 1 }
						};
			
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
		}
		
		/* Auth-Application-Id */
		{
			/*
				The Auth-Application-Id AVP (AVP Code 258) is of type Unsigned32 and
				is used in order to advertise support of the Authentication and
				Authorization portion of an application (see Section 2.4).  If
				present in a message other than CER and CEA, the value of the Auth-
				Application-Id AVP MUST match the Application Id present in the
				Diameter message header.
			*/
			struct dict_avp_data data = { 
					258, 					/* Code */
					#if AC_AUTH_APPLICATION_ID != 258
					#error "AC_AUTH_APPLICATION_ID definition mismatch"
					#endif
					0, 					/* Vendor */
					"Auth-Application-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Acct-Application-Id */
		{
			/*
				The Acct-Application-Id AVP (AVP Code 259) is of type Unsigned32 and
				is used in order to advertise support of the Accounting portion of an
				application (see Section 2.4).  If present in a message other than
				CER and CEA, the value of the Acct-Application-Id AVP MUST match the
				Application Id present in the Diameter message header.
			*/
			struct dict_avp_data data = { 
					259, 					/* Code */
					#if AC_ACCT_APPLICATION_ID != 259
					#error "AC_ACCT_APPLICATION_ID definition mismatch"
					#endif
					0, 					/* Vendor */
					"Acct-Application-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Inband-Security-Id */
		{
			/*
				The Inband-Security-Id AVP (AVP Code 299) is of type Unsigned32 and
				is used in order to advertise support of the Security portion of the
				application.

				Currently, the following values are supported, but there is ample
				room to add new security Ids.


				NO_INBAND_SECURITY 0

				This peer does not support TLS.  This is the default value, if the
				AVP is omitted.

				TLS 1

				This node supports TLS security, as defined by [RFC4346].
			*/
			
			/* Although the RFC does not specify an "Enumerated" type here, we go forward and create one.
			 * This is the reason for the "*" in the type name
			 */
			
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated*(Inband-Security-Id)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "NO_INBAND_SECURITY", 		{ .u32 = ACV_ISI_NO_INBAND_SECURITY }};
			struct dict_enumval_data 	t_1 = { "TLS", 			{ .u32 = ACV_ISI_TLS }};
			struct dict_avp_data 		data = { 
					299, 					/* Code */
					#if AC_INBAND_SECURITY_ID != 299
					#error "AC_INBAND_SECURITY_ID definition mismatch"
					#endif
					0, 					/* Vendor */
					"Inband-Security-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Vendor-Specific-Application-Id */
		{
			/*
				The Vendor-Specific-Application-Id AVP (AVP Code 260) is of type
				Grouped and is used to advertise support of a vendor-specific
				Diameter Application.  Exactly one instance of either Auth-
				Application-Id or Acct-Application-Id AVP MUST be present.  The
				Application Id carried by either Auth-Application-Id or Acct-
				Application-Id AVP MUST comply with vendor specific Application Id
				assignment described in Sec 11.3.  It MUST also match the Application
				Id present in the diameter header except when used in a CER or CEA
				messages.

				The Vendor-Id AVP is an informational AVP pertaining to the vendor
				who may have authorship of the vendor-specific Diameter application.
				It MUST NOT be used as a means of defining a completely separate
				vendor-specific Application Id space.

				This AVP MUST also be present as the first AVP in all experimental
				commands defined in the vendor-specific application.

				This AVP SHOULD be placed as close to the Diameter header as
				possible.

				AVP Format

				<Vendor-Specific-Application-Id> ::= < AVP Header: 260 >
                                				   { Vendor-Id }
                                				   [ Auth-Application-Id ]
                                				   [ Acct-Application-Id ]

				A Vendor-Specific-Application-Id AVP MUST contain exactly one of
				either Auth-Application-Id or Acct-Application-Id.  If a Vendor-
				Specific-Application-Id is received without any of these two AVPs,
				then the recipient SHOULD issue an answer with a Result-Code set to
				DIAMETER_MISSING_AVP.  The answer SHOULD also include a Failed-AVP
				which MUST contain an example of an Auth-Application-Id AVP and an
				Acct-Application-Id AVP.

				If a Vendor-Specific-Application-Id is received that contains both
				Auth-Application-Id and Acct-Application-Id, then the recipient
				SHOULD issue an answer with Result-Code set to
				DIAMETER_AVP_OCCURS_TOO_MANY_TIMES.  The answer SHOULD also include a
				Failed-AVP which MUST contain the received Auth-Application-Id AVP
				and Acct-Application-Id AVP.
			*/
			struct dict_object 	* avp;
			struct dict_avp_data	  data = { 
					260, 					/* Code */
					#if AC_VENDOR_SPECIFIC_APPLICATION_ID != 260
					#error "AC_VENDOR_SPECIFIC_APPLICATION_ID definition mismatch"
					#endif
					0, 					/* Vendor */
					"Vendor-Specific-Application-Id", 	/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
					
			struct local_rules_definition rules[] = 
						{
#ifndef WORKAROUND_ACCEPT_INVALID_VSAI
							/* ABNF from RFC6733 */
						 	 {  "Vendor-Id", 			RULE_REQUIRED, -1, 1 }
#else /* WORKAROUND_ACCEPT_INVALID_VSAI */
							/* ABNF from RFC3588 (including erratum, because original text is nonsense) */
						 	 {  "Vendor-Id", 			RULE_REQUIRED, -1, -1}
#endif /* WORKAROUND_ACCEPT_INVALID_VSAI */
							,{  "Auth-Application-Id",		RULE_OPTIONAL, -1, 1 }
							,{  "Acct-Application-Id",		RULE_OPTIONAL, -1, 1 }
						};
			
			/* Create the grouped AVP */
			CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
			PARSE_loc_rules( rules, avp );
			
		}
		
		/* Redirect-Host */
		{
			/*
				One or more of instances of this AVP MUST be present if the answer
				message's 'E' bit is set and the Result-Code AVP is set to
				DIAMETER_REDIRECT_INDICATION.

				Upon receiving the above, the receiving Diameter node SHOULD forward
				the request directly to one of the hosts identified in these AVPs.
				The server contained in the selected Redirect-Host AVP SHOULD be used
				for all messages pertaining to this session.
			*/
			struct dict_avp_data data = { 
					292, 					/* Code */
					#if AC_REDIRECT_HOST != 292
					#error "AC_REDIRECT_HOST definition mismatch"
					#endif
					0, 					/* Vendor */
					"Redirect-Host", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterURI_type, NULL);
		}
		
		/* Redirect-Host-Usage */
		{
			/*
				The Redirect-Host-Usage AVP (AVP Code 261) is of type Enumerated.
				This AVP MAY be present in answer messages whose 'E' bit is set and
				the Result-Code AVP is set to DIAMETER_REDIRECT_INDICATION.

				When present, this AVP dictates how the routing entry resulting from
				the Redirect-Host is to be used.  The following values are supported:


				DONT_CACHE 0

				The host specified in the Redirect-Host AVP should not be cached.
				This is the default value.


				ALL_SESSION 1

				All messages within the same session, as defined by the same value
				of the Session-ID AVP MAY be sent to the host specified in the
				Redirect-Host AVP.


				ALL_REALM 2

				All messages destined for the realm requested MAY be sent to the
				host specified in the Redirect-Host AVP.


				REALM_AND_APPLICATION 3

				All messages for the application requested to the realm specified
				MAY be sent to the host specified in the Redirect-Host AVP.

				ALL_APPLICATION 4

				All messages for the application requested MAY be sent to the host
				specified in the Redirect-Host AVP.


				ALL_HOST 5

				All messages that would be sent to the host that generated the
				Redirect-Host MAY be sent to the host specified in the Redirect-
				Host AVP.


				ALL_USER 6

				All messages for the user requested MAY be sent to the host
				specified in the Redirect-Host AVP.


				When multiple cached routes are created by redirect indications and
				they differ only in redirect usage and peers to forward requests to
				(see Section 6.1.8), a precedence rule MUST be applied to the
				redirect usage values of the cached routes during normal routing to
				resolve contentions that may occur.  The precedence rule is the order
				that dictate which redirect usage should be considered before any
				other as they appear.  The order is as follows:


				1.  ALL_SESSION

				2.  ALL_USER

				3.  REALM_AND_APPLICATION

				4.  ALL_REALM

				5.  ALL_APPLICATION

				6.  ALL_HOST
			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Redirect-Host-Usage)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "DONT_CACHE", 			{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "ALL_SESSION", 			{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "ALL_REALM", 			{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "REALM_AND_APPLICATION", 	{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "ALL_APPLICATION", 		{ .i32 = 4 }};
			struct dict_enumval_data 	t_5 = { "ALL_HOST", 			{ .i32 = 5 }};
			struct dict_enumval_data 	t_6 = { "ALL_USER", 			{ .i32 = 6 }};
			struct dict_avp_data 		data = { 
					261, 					/* Code */
					#if AC_REDIRECT_HOST_USAGE != 261
					#error "AC_REDIRECT_HOST_USAGE definition mismatch"
					#endif
					0, 					/* Vendor */
					"Redirect-Host-Usage", 			/* Name */
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
			CHECK_dict_new( DICT_ENUMVAL, &t_5 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Redirect-Max-Cache-Time */
		{
			/*
				The Redirect-Max-Cache-Time AVP (AVP Code 262) is of type Unsigned32.
				This AVP MUST be present in answer messages whose 'E' bit is set, the
				Result-Code AVP is set to DIAMETER_REDIRECT_INDICATION and the
				Redirect-Host-Usage AVP set to a non-zero value.

				This AVP contains the maximum number of seconds the peer and route
				table entries, created as a result of the Redirect-Host, will be
				cached.  Note that once a host created due to a redirect indication
				is no longer reachable, any associated peer and routing table entries
				MUST be deleted.
			*/
			struct dict_avp_data data = { 
					262, 					/* Code */
					#if AC_REDIRECT_MAX_CACHE_TIME != 262
					#error "AC_REDIRECT_MAX_CACHE_TIME definition mismatch"
					#endif
					0, 					/* Vendor */
					"Redirect-Max-Cache-Time", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Result-Code */
		{
			/*
				The Result-Code AVP (AVP Code 268) is of type Unsigned32 and
				indicates whether a particular request was completed successfully or
				whether an error occurred.  All Diameter answer messages defined in
				IETF applications MUST include one Result-Code AVP.  A non-successful
				Result-Code AVP (one containing a non 2xxx value other than
				DIAMETER_REDIRECT_INDICATION) MUST include the Error-Reporting-Host
				AVP if the host setting the Result-Code AVP is different from the
				identity encoded in the Origin-Host AVP.

				The Result-Code data field contains an IANA-managed 32-bit address
				space representing errors (see Section 11.4).  Diameter provides the
				following classes of errors, all identified by the thousands digit in
				the decimal notation:

				o  1xxx (Informational)

				o  2xxx (Success)

				o  3xxx (Protocol Errors)

				o  4xxx (Transient Failures)

				o  5xxx (Permanent Failure)

				A non-recognized class (one whose first digit is not defined in this
				section) MUST be handled as a permanent failure.
			*/
			
			/* Although the RFC does not specify an "Enumerated" type here, we go forward and create one.
			 * This is the reason for the "*" in the type name
			 */
			struct dict_object * 	type;
			struct dict_type_data 	tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated*(Result-Code)"	, NULL, NULL, NULL };
			struct dict_avp_data 	data = { 
					268, 					/* Code */
					#if AC_RESULT_CODE != 268
					#error "AC_RESULT_CODE definition mismatch"
					#endif
					0, 					/* Vendor */
					"Result-Code", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
			
			/* Informational */
			{
				/* 1001 */
				{
					/*
						This informational error is returned by a Diameter server to
						inform the access device that the authentication mechanism being
						used requires multiple round trips, and a subsequent request needs
						to be issued in order for access to be granted.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_MULTI_ROUND_AUTH", 	{ .u32 = ER_DIAMETER_MULTI_ROUND_AUTH }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
			}
			/* Success */
			{
				/* 2001 */
				{
					/*
						The Request was successfully completed.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_SUCCESS", 		{ .u32 = ER_DIAMETER_SUCCESS }};
					#if ER_DIAMETER_SUCCESS != 2001
					#error "ER_DIAMETER_SUCCESS definition mismatch"
					#endif
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 2002 */
				{
					/*
						When returned, the request was successfully completed, but
						additional processing is required by the application in order to
						provide service to the user.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_LIMITED_SUCCESS", 	{ .u32 = ER_DIAMETER_LIMITED_SUCCESS }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
			}
			/* Protocol Errors */
			{
				/* 3001 -- might be changed to 5xxx soon */
				{
					/*
						The Request contained a Command-Code that the receiver did not
						recognize or support.  This MUST be used when a Diameter node
						receives an experimental command that it does not understand.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_COMMAND_UNSUPPORTED", 	{ .u32 = ER_DIAMETER_COMMAND_UNSUPPORTED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3002 */
				{
					/*
						This error is given when Diameter can not deliver the message to
						the destination, either because no host within the realm
						supporting the required application was available to process the
						request, or because Destination-Host AVP was given without the
						associated Destination-Realm AVP.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_UNABLE_TO_DELIVER", 	{ .u32 = ER_DIAMETER_UNABLE_TO_DELIVER }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3003 */
				{
					/*
						The intended realm of the request is not recognized.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_REALM_NOT_SERVED", 	{ .u32 = ER_DIAMETER_REALM_NOT_SERVED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3004 */
				{
					/*
						When returned, a Diameter node SHOULD attempt to send the message
						to an alternate peer.  This error MUST only be used when a
						specific server is requested, and it cannot provide the requested
						service.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_TOO_BUSY", 		{ .u32 = ER_DIAMETER_TOO_BUSY }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3005 */
				{
					/*
						An agent detected a loop while trying to get the message to the
						intended recipient.  The message MAY be sent to an alternate peer,
						if one is available, but the peer reporting the error has
						identified a configuration problem.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_LOOP_DETECTED", 	{ .u32 = ER_DIAMETER_LOOP_DETECTED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3006 */
				{
					/*
						A redirect agent has determined that the request could not be
						satisfied locally and the initiator of the request should direct
						the request directly to the server, whose contact information has
						been added to the response.  When set, the Redirect-Host AVP MUST
						be present.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_REDIRECT_INDICATION", 	{ .u32 = ER_DIAMETER_REDIRECT_INDICATION }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3007 */
				{
					/*
						A request was sent for an application that is not supported.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_APPLICATION_UNSUPPORTED",	{ .u32 = ER_DIAMETER_APPLICATION_UNSUPPORTED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3008 -- will change to 5xxx soon */
				{
					/*
						A request was received whose bits in the Diameter header were
						either set to an invalid combination, or to a value that is
						inconsistent with the command code's definition.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_INVALID_HDR_BITS", 	{ .u32 = ER_DIAMETER_INVALID_HDR_BITS }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3009 -- will change to 5xxx soon */
				{
					/*
						A request was received that included an AVP whose flag bits are
						set to an unrecognized value, or that is inconsistent with the
						AVP's definition.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_INVALID_AVP_BITS", 	{ .u32 = ER_DIAMETER_INVALID_AVP_BITS }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 3010  -- will change to 5xxx soon */
				{
					/*
						 A CER was received from an unknown peer.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_UNKNOWN_PEER", 	{ .u32 = ER_DIAMETER_UNKNOWN_PEER }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
			}
			/* Transient Failures */
			{
				/* 4001 */
				{
					/*
						The authentication process for the user failed, most likely due to
						an invalid password used by the user.  Further attempts MUST only
						be tried after prompting the user for a new password.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_AUTHENTICATION_REJECTED", 	{ .u32 = ER_DIAMETER_AUTHENTICATION_REJECTED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 4002 */
				{
					/*
						A Diameter node received the accounting request but was unable to
						commit it to stable storage due to a temporary lack of space.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_OUT_OF_SPACE", 		{ .u32 = ER_DIAMETER_OUT_OF_SPACE }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 4003 */
				{
					/*
						The peer has determined that it has lost the election process and
						has therefore disconnected the transport connection.
					*/
					struct dict_enumval_data 	error_code = { "ELECTION_LOST", 			{ .u32 = ER_ELECTION_LOST }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
			}
			/* Permanent Failures */
			{
				/* 5001 */
				{
					/*
						The peer received a message that contained an AVP that is not
						recognized or supported and was marked with the Mandatory bit.  A
						Diameter message with this error MUST contain one or more Failed-
						AVP AVP containing the AVPs that caused the failure.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_AVP_UNSUPPORTED", 	{ .u32 = ER_DIAMETER_AVP_UNSUPPORTED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5002 */
				{
					/*
						The request contained an unknown Session-Id.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_UNKNOWN_SESSION_ID", 	{ .u32 = ER_DIAMETER_UNKNOWN_SESSION_ID }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5003 */
				{
					/*
						A request was received for which the user could not be authorized.
						This error could occur if the service requested is not permitted
						to the user.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_AUTHORIZATION_REJECTED",{ .u32 = ER_DIAMETER_AUTHORIZATION_REJECTED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5004 */
				{
					/*
						The request contained an AVP with an invalid value in its data
						portion.  A Diameter message indicating this error MUST include
						the offending AVPs within a Failed-AVP AVP.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_INVALID_AVP_VALUE",	{ .u32 = ER_DIAMETER_INVALID_AVP_VALUE }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5005 */
				{
					/*
						The request did not contain an AVP that is required by the Command
						Code definition.  If this value is sent in the Result-Code AVP, a
						Failed-AVP AVP SHOULD be included in the message.  The Failed-AVP
						AVP MUST contain an example of the missing AVP complete with the
						Vendor-Id if applicable.  The value field of the missing AVP
						should be of correct minimum length and contain zeroes.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_MISSING_AVP",		{ .u32 = ER_DIAMETER_MISSING_AVP }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5006 */
				{
					/*
						A request was received that cannot be authorized because the user
						has already expended allowed resources.  An example of this error
						condition is a user that is restricted to one dial-up PPP port,
						attempts to establish a second PPP connection.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_RESOURCES_EXCEEDED",	{ .u32 = ER_DIAMETER_RESOURCES_EXCEEDED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5007 */
				{
					/*
						The Home Diameter server has detected AVPs in the request that
						contradicted each other, and is not willing to provide service to
						the user.  The Failed-AVP AVPs MUST be present which contains the
						AVPs that contradicted each other.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_CONTRADICTING_AVPS",	{ .u32 = ER_DIAMETER_CONTRADICTING_AVPS }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5008 */
				{
					/*
						A message was received with an AVP that MUST NOT be present.  The
						Failed-AVP AVP MUST be included and contain a copy of the
						offending AVP.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_AVP_NOT_ALLOWED",	{ .u32 = ER_DIAMETER_AVP_NOT_ALLOWED }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5009 */
				{
					/*
						A message was received that included an AVP that appeared more
						often than permitted in the message definition.  The Failed-AVP
						AVP MUST be included and contain a copy of the first instance of
						the offending AVP that exceeded the maximum number of occurrences
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_AVP_OCCURS_TOO_MANY_TIMES",{ .u32 = ER_DIAMETER_AVP_OCCURS_TOO_MANY_TIMES }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5010 */
				{
					/*
						This error is returned by a Diameter node that is not acting as a
						relay when it receives a CER which advertises a set of
						applications that it does not support.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_NO_COMMON_APPLICATION",{ .u32 = ER_DIAMETER_NO_COMMON_APPLICATION }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5011 */
				{
					/*
						This error is returned when a request was received, whose version
						number is unsupported.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_UNSUPPORTED_VERSION",	{ .u32 = ER_DIAMETER_UNSUPPORTED_VERSION }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5012 */
				{
					/*
						This error is returned when a request is rejected for unspecified
						reasons.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_UNABLE_TO_COMPLY",	{ .u32 = ER_DIAMETER_UNABLE_TO_COMPLY }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5013 -- will change to 3xxx */
				{
					/*
						This error is returned when an unrecognized bit in the Diameter
						header is set to one (1).
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_INVALID_BIT_IN_HEADER", 	{ .u32 = ER_DIAMETER_INVALID_BIT_IN_HEADER }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5014 */
				{
					/*
						The request contained an AVP with an invalid length.  A Diameter
						message indicating this error MUST include the offending AVPs
						within a Failed-AVP AVP.  In cases where the erroneous avp length
						value exceeds the message length or is less than the minimum AVP
						header length, it is sufficient to include the offending AVP
						header and a zero filled payload of the minimum required length
						for the payloads data type.  If the AVP is a grouped AVP, the
						grouped AVP header with an empty payload would be sufficient to
						indicate the offending AVP.  In the case where the offending AVP
						header cannot be fully decoded when avp length is less than the
						minimum AVP header length, it is sufficient to include an
						offending AVP header that is formulated by padding the incomplete
						AVP header with zero up to the minimum AVP header length.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_INVALID_AVP_LENGTH",	{ .u32 = ER_DIAMETER_INVALID_AVP_LENGTH }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5015 -- will change to 3xxx */
				{
					/*
						This error is returned when a request is received with an invalid
						message length.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_INVALID_MESSAGE_LENGTH", 	{ .u32 = ER_DIAMETER_INVALID_MESSAGE_LENGTH }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5016 */
				{
					/*
						The request contained an AVP with which is not allowed to have the
						given value in the AVP Flags field.  A Diameter message indicating
						this error MUST include the offending AVPs within a Failed-AVP
						AVP.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_INVALID_AVP_BIT_COMBO", 	{ .u32 = ER_DIAMETER_INVALID_AVP_BIT_COMBO }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
				/* 5017 */
				{
					/*
						This error is returned when a CER message is received, and there
						are no common security mechanisms supported between the peers.  A
						Capabilities-Exchange-Answer (CEA) MUST be returned with the
						Result-Code AVP set to DIAMETER_NO_COMMON_SECURITY.
					*/
					struct dict_enumval_data 	error_code = { "DIAMETER_NO_COMMON_SECURITY",	{ .u32 = ER_DIAMETER_NO_COMMON_SECURITY }};
					CHECK_dict_new( DICT_ENUMVAL, &error_code , type, NULL);
				}
			}
		}
		
		/* Error-Message */
		{
			/*
				The Error-Message AVP (AVP Code 281) is of type UTF8String.  It MAY
				accompany a Result-Code AVP as a human readable error message.  The
				Error-Message AVP is not intended to be useful in real-time, and
				SHOULD NOT be expected to be parsed by network entities.
			*/
			struct dict_avp_data data = { 
					281, 					/* Code */
					#if AC_ERROR_MESSAGE != 281
					#error "AC_ERROR_MESSAGE definition mismatch"
					#endif
					0, 					/* Vendor */
					"Error-Message", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					0,					/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Error-Reporting-Host */
		{
			/*
				The Error-Reporting-Host AVP (AVP Code 294) is of type
				DiameterIdentity.  This AVP contains the identity of the Diameter
				host that sent the Result-Code AVP to a value other than 2001
				(Success), only if the host setting the Result-Code is different from
				the one encoded in the Origin-Host AVP.  This AVP is intended to be
				used for troubleshooting purposes, and MUST be set when the Result-
				Code AVP indicates a failure.
			*/
			struct dict_avp_data data = { 
					294, 					/* Code */
					#if AC_ERROR_REPORTING_HOST != 294
					#error "AC_ERROR_REPORTING_HOST definition mismatch"
					#endif
					0, 					/* Vendor */
					"Error-Reporting-Host", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					0,					/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , DiameterIdentity_type, NULL);
		}
		
		/* Failed-AVP */
		{
			/*
				The Failed-AVP AVP (AVP Code 279) is of type Grouped and provides
				debugging information in cases where a request is rejected or not
				fully processed due to erroneous information in a specific AVP.  The
				value of the Result-Code AVP will provide information on the reason
				for the Failed-AVP AVP.  A Diameter message SHOULD contain only one
				Failed-AVP that corresponds to the error indicated by the Result-Code
				AVP.  For practical purposes, this Failed-AVP would typically refer
				to the first AVP processing error that a Diameter node encounters.

				The possible reasons for this AVP are the presence of an improperly
				constructed AVP, an unsupported or unrecognized AVP, an invalid AVP
				value, the omission of a required AVP, the presence of an explicitly
				excluded AVP (see tables in Section 10), or the presence of two or
				more occurrences of an AVP which is restricted to 0, 1, or 0-1
				occurrences.

				A Diameter message SHOULD contain one Failed-AVP AVP, containing the
				entire AVP that could not be processed successfully.  If the failure
				reason is omission of a required AVP, an AVP with the missing AVP
				code, the missing vendor id, and a zero filled payload of the minimum
				required length for the omitted AVP will be added.  If the failure
				reason is an invalid AVP length where the reported length is less
				than the minimum AVP header length or greater than the reported
				message length, a copy of the offending AVP header and a zero filled
				payload of the minimum required length SHOULD be added.

				In the case where the offending AVP is embedded within a grouped AVP,
				the Failed-AVP MAY contain the grouped AVP which in turn contains the
				single offending AVP.  The same method MAY be employed if the grouped
				AVP itself is embedded in yet another grouped AVP and so on.  In this
				case, the Failed-AVP MAY contain the grouped AVP heirarchy up to the
				single offending AVP.  This enables the recipient to detect the
				location of the offending AVP when embedded in a group.

				AVP Format

				 <Failed-AVP> ::= < AVP Header: 279 >
        				       1* {AVP}
			*/
			struct dict_avp_data data = { 
					279, 					/* Code */
					#if AC_FAILED_AVP != 279
					#error "AC_FAILED_AVP definition mismatch"
					#endif
					0, 					/* Vendor */
					"Failed-AVP", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Experimental-Result */
		{
			/*
				The Experimental-Result AVP (AVP Code 297) is of type Grouped, and
				indicates whether a particular vendor-specific request was completed
				successfully or whether an error occurred.  Its Data field has the
				following ABNF grammar:

				AVP Format

				 Experimental-Result ::= < AVP Header: 297 >
                        				 { Vendor-Id }
                        				 { Experimental-Result-Code }

				The Vendor-Id AVP (see Section 5.3.3) in this grouped AVP identifies
				the vendor responsible for the assignment of the result code which
				follows.  All Diameter answer messages defined in vendor-specific
				applications MUST include either one Result-Code AVP or one
				Experimental-Result AVP.
			*/
			struct dict_avp_data data = { 
					297, 					/* Code */
					0, 					/* Vendor */
					"Experimental-Result", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_GROUPED 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Experimental-Result-Code */
		{
			/*
				The Experimental-Result-Code AVP (AVP Code 298) is of type Unsigned32
				and contains a vendor-assigned value representing the result of
				processing the request.

				It is recommended that vendor-specific result codes follow the same
				conventions given for the Result-Code AVP regarding the different
				types of result codes and the handling of errors (for non 2xxx
				values).
			*/
			/* Although the RFC does not specify an "Enumerated" type here, we go forward and create one.
			 * This is the reason for the "*" in the type name. Vendors will have to define their values.
			 */
			struct dict_object * 	type;
			struct dict_type_data 	tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated*(Experimental-Result-Code)"	, NULL, NULL, NULL };
			struct dict_avp_data 	data = { 
					298, 					/* Code */
					0, 					/* Vendor */
					"Experimental-Result-Code", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Auth-Request-Type */
		{
			/*
				The Auth-Request-Type AVP (AVP Code 274) is of type Enumerated and is
				included in application-specific auth requests to inform the peers
				whether a user is to be authenticated only, authorized only or both.
				Note any value other than both MAY cause RADIUS interoperability
				issues.  The following values are defined:


				AUTHENTICATE_ONLY 1

				The request being sent is for authentication only, and MUST
				contain the relevant application specific authentication AVPs that
				are needed by the Diameter server to authenticate the user.


				AUTHORIZE_ONLY 2

				The request being sent is for authorization only, and MUST contain
				the application specific authorization AVPs that are necessary to
				identify the service being requested/offered.


				AUTHORIZE_AUTHENTICATE 3

				The request contains a request for both authentication and
				authorization.  The request MUST include both the relevant
				application specific authentication information, and authorization
				information necessary to identify the service being requested/
				offered.
			*/
			struct dict_object 	* 	type;
			struct dict_type_data 		tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Auth-Request-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_1 = { "AUTHENTICATE_ONLY", 		{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "AUTHORIZE_ONLY", 		{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "AUTHORIZE_AUTHENTICATE", 	{ .i32 = 3 }};
			struct dict_avp_data 	data = { 
					274, 					/* Code */
					0, 					/* Vendor */
					"Auth-Request-Type", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Session-Id */
		{
			/*
				The Session-Id AVP (AVP Code 263) is of type UTF8String and is used
				to identify a specific session (see Section 8).  All messages
				pertaining to a specific session MUST include only one Session-Id AVP
				and the same value MUST be used throughout the life of a session.
				When present, the Session-Id SHOULD appear immediately following the
				Diameter Header (see Section 3).

				The Session-Id MUST be globally and eternally unique, as it is meant
				to uniquely identify a user session without reference to any other
				information, and may be needed to correlate historical authentication
				information with accounting information.  The Session-Id includes a
				mandatory portion and an implementation-defined portion; a
				recommended format for the implementation-defined portion is outlined
				below.
				
				(skipped, see RFC for detail)
			*/
			struct dict_avp_data data = { 
					263, 					/* Code */
					#if AC_SESSION_ID != 263
					#error "AC_SESSION_ID definition mismatch"
					#endif
					0, 					/* Vendor */
					"Session-Id", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Authorization-Lifetime */
		{
			/*
				The Authorization-Lifetime AVP (AVP Code 291) is of type Unsigned32
				and contains the maximum number of seconds of service to be provided
				to the user before the user is to be re-authenticated and/or re-
				authorized.  Great care should be taken when the Authorization-
				Lifetime value is determined, since a low, non-zero, value could
				create significant Diameter traffic, which could congest both the
				network and the agents.

				A value of zero (0) means that immediate re-auth is necessary by the
				access device.  This is typically used in cases where multiple
				authentication methods are used, and a successful auth response with
				this AVP set to zero is used to signal that the next authentication
				method is to be immediately initiated.  The absence of this AVP, or a
				value of all ones (meaning all bits in the 32 bit field are set to
				one) means no re-auth is expected.

				If both this AVP and the Session-Timeout AVP are present in a
				message, the value of the latter MUST NOT be smaller than the
				Authorization-Lifetime AVP.

				An Authorization-Lifetime AVP MAY be present in re-authorization
				messages, and contains the number of seconds the user is authorized
				to receive service from the time the re-auth answer message is
				received by the access device.

				This AVP MAY be provided by the client as a hint of the maximum
				lifetime that it is willing to accept.  However, the server MAY
				return a value that is equal to, or smaller, than the one provided by
				the client.
			*/
			struct dict_avp_data data = { 
					291, 					/* Code */
					0, 					/* Vendor */
					"Authorization-Lifetime", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Auth-Grace-Period */
		{
			/*
				The Auth-Grace-Period AVP (AVP Code 276) is of type Unsigned32 and
				contains the number of seconds the Diameter server will wait
				following the expiration of the Authorization-Lifetime AVP before
				cleaning up resources for the session.
			*/
			struct dict_avp_data data = { 
					276, 					/* Code */
					0, 					/* Vendor */
					"Auth-Grace-Period", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Auth-Session-State */
		{
			/*
				The Auth-Session-State AVP (AVP Code 277) is of type Enumerated and
				specifies whether state is maintained for a particular session.  The
				client MAY include this AVP in requests as a hint to the server, but
				the value in the server's answer message is binding.  The following
				values are supported:


				STATE_MAINTAINED 0

				This value is used to specify that session state is being
				maintained, and the access device MUST issue a session termination
				message when service to the user is terminated.  This is the
				default value.


				NO_STATE_MAINTAINED 1

				This value is used to specify that no session termination messages
				will be sent by the access device upon expiration of the
				Authorization-Lifetime.
			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Auth-Session-State)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "STATE_MAINTAINED", 		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "NO_STATE_MAINTAINED", 		{ .i32 = 1 }};
			struct dict_avp_data	 	data = { 
					277, 					/* Code */
					0, 					/* Vendor */
					"Auth-Session-State", 			/* Name */
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
		
		/* Re-Auth-Request-Type */
		{
			/*
				The Re-Auth-Request-Type AVP (AVP Code 285) is of type Enumerated and
				is included in application-specific auth answers to inform the client
				of the action expected upon expiration of the Authorization-Lifetime.
				If the answer message contains an Authorization-Lifetime AVP with a
				positive value, the Re-Auth-Request-Type AVP MUST be present in an
				answer message.  The following values are defined:


				AUTHORIZE_ONLY 0

				An authorization only re-auth is expected upon expiration of the
				Authorization-Lifetime.  This is the default value if the AVP is
				not present in answer messages that include the Authorization-
				Lifetime.


				AUTHORIZE_AUTHENTICATE 1

				An authentication and authorization re-auth is expected upon
				expiration of the Authorization-Lifetime.
			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Re-Auth-Request-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "AUTHORIZE_ONLY", 		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "AUTHORIZE_AUTHENTICATE",	{ .i32 = 1 }};
			struct dict_avp_data	 	data = { 
					285, 					/* Code */
					0, 					/* Vendor */
					"Re-Auth-Request-Type",			/* Name */
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
		
		/* Session-Timeout */
		{
			/*
				The Session-Timeout AVP (AVP Code 27) [RFC2865] is of type Unsigned32
				and contains the maximum number of seconds of service to be provided
				to the user before termination of the session.  When both the
				Session-Timeout and the Authorization-Lifetime AVPs are present in an
				answer message, the former MUST be equal to or greater than the value
				of the latter.

				A session that terminates on an access device due to the expiration
				of the Session-Timeout MUST cause an STR to be issued, unless both
				the access device and the home server had previously agreed that no
				session termination messages would be sent (see Section 8.11).

				A Session-Timeout AVP MAY be present in a re-authorization answer
				message, and contains the remaining number of seconds from the
				beginning of the re-auth.

				A value of zero, or the absence of this AVP, means that this session
				has an unlimited number of seconds before termination.

				This AVP MAY be provided by the client as a hint of the maximum
				timeout that it is willing to accept.  However, the server MAY return
				a value that is equal to, or smaller, than the one provided by the
				client.
			*/
			struct dict_avp_data data = { 
					27, 					/* Code */
					0, 					/* Vendor */
					"Session-Timeout", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* User-Name */
		{
			/*
				The User-Name AVP (AVP Code 1) [RFC2865] is of type UTF8String, which
				contains the User-Name, in a format consistent with the NAI
				specification [RFC4282].
			*/
			struct dict_avp_data data = { 
					1, 					/* Code */
					0, 					/* Vendor */
					"User-Name", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Termination-Cause */
		{
			/*
				The Termination-Cause AVP (AVP Code 295) is of type Enumerated, and
				is used to indicate the reason why a session was terminated on the
				access device.  The following values are defined:


				DIAMETER_LOGOUT 1

				The user initiated a disconnect


				DIAMETER_SERVICE_NOT_PROVIDED 2

				This value is used when the user disconnected prior to the receipt
				of the authorization answer message.


				DIAMETER_BAD_ANSWER 3

				This value indicates that the authorization answer received by the
				access device was not processed successfully.


				DIAMETER_ADMINISTRATIVE 4

				The user was not granted access, or was disconnected, due to
				administrative reasons, such as the receipt of a Abort-Session-
				Request message.


				DIAMETER_LINK_BROKEN 5

				The communication to the user was abruptly disconnected.


				DIAMETER_AUTH_EXPIRED 6

				The user's access was terminated since its authorized session time
				has expired.


				DIAMETER_USER_MOVED 7

				The user is receiving services from another access device.


				DIAMETER_SESSION_TIMEOUT 8

				The user's session has timed out, and service has been terminated.
			*/
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Termination-Cause)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_1 = { "DIAMETER_LOGOUT",			{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "DIAMETER_SERVICE_NOT_PROVIDED", 	{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "DIAMETER_BAD_ANSWER",			{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "DIAMETER_ADMINISTRATIVE", 		{ .i32 = 4 }};
			struct dict_enumval_data 	t_5 = { "DIAMETER_LINK_BROKEN",			{ .i32 = 5 }};
			struct dict_enumval_data 	t_6 = { "DIAMETER_AUTH_EXPIRED", 		{ .i32 = 6 }};
			struct dict_enumval_data 	t_7 = { "DIAMETER_USER_MOVED",			{ .i32 = 7 }};
			struct dict_enumval_data 	t_8 = { "DIAMETER_SESSION_TIMEOUT", 		{ .i32 = 8 }};
			struct dict_avp_data 	data = { 
					295, 					/* Code */
					0, 					/* Vendor */
					"Termination-Cause",			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_5 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_6 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_7 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_8 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Origin-State-Id */
		{
			/*
				The Origin-State-Id AVP (AVP Code 278), of type Unsigned32, is a
				monotonically increasing value that is advanced whenever a Diameter
				entity restarts with loss of previous state, for example upon reboot.
				Origin-State-Id MAY be included in any Diameter message, including
				CER.

				A Diameter entity issuing this AVP MUST create a higher value for
				this AVP each time its state is reset.  A Diameter entity MAY set
				Origin-State-Id to the time of startup, or it MAY use an incrementing
				counter retained in non-volatile memory across restarts.

				The Origin-State-Id, if present, MUST reflect the state of the entity
				indicated by Origin-Host.  If a proxy modifies Origin-Host, it MUST
				either remove Origin-State-Id or modify it appropriately as well.
				Typically, Origin-State-Id is used by an access device that always
				starts up with no active sessions; that is, any session active prior
				to restart will have been lost.  By including Origin-State-Id in a
				message, it allows other Diameter entities to infer that sessions
				associated with a lower Origin-State-Id are no longer active.  If an
				access device does not intend for such inferences to be made, it MUST
				either not include Origin-State-Id in any message, or set its value
				to 0.
			*/
			struct dict_avp_data data = { 
					278, 					/* Code */
					0, 					/* Vendor */
					"Origin-State-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Session-Binding */
		{
			/*
				The Session-Binding AVP (AVP Code 270) is of type Unsigned32, and MAY
				be present in application-specific authorization answer messages.  If
				present, this AVP MAY inform the Diameter client that all future
				application-specific re-auth messages for this session MUST be sent
				to the same authorization server.  This AVP MAY also specify that a
				Session-Termination-Request message for this session MUST be sent to
				the same authorizing server.

				This field is a bit mask, and the following bits have been defined:


				RE_AUTH 1

				When set, future re-auth messages for this session MUST NOT
				include the Destination-Host AVP.  When cleared, the default
				value, the Destination-Host AVP MUST be present in all re-auth
				messages for this session.


				STR 2

				When set, the STR message for this session MUST NOT include the
				Destination-Host AVP.  When cleared, the default value, the
				Destination-Host AVP MUST be present in the STR message for this
				session.


				ACCOUNTING 4

				When set, all accounting messages for this session MUST NOT
				include the Destination-Host AVP.  When cleared, the default
				value, the Destination-Host AVP, if known, MUST be present in all
				accounting messages for this session.
			*/
			
			/* Although the RFC does not specify an "Enumerated" type here, we go forward and create one.
			 * This is the reason for the "*" in the type name
			 * The actual values of the AVP will not always be defined here, but at least it can be used in some cases.
			 *  ... maybe the code will be changed later to support bitfields AVP ...
			 */
			
			struct dict_object 	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_UNSIGNED32,	"Enumerated*(Session-Binding)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_1 = { "RE_AUTH", 		{ .u32 = 1 }};
			struct dict_enumval_data 	t_2 = { "STR", 			{ .u32 = 2 }};
			struct dict_enumval_data 	t_4 = { "ACCOUNTING", 		{ .u32 = 4 }};
			struct dict_avp_data 	data = { 
					270, 					/* Code */
					0, 					/* Vendor */
					"Session-Binding", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Session-Server-Failover */
		{
			/*
				The Session-Server-Failover AVP (AVP Code 271) is of type Enumerated,
				and MAY be present in application-specific authorization answer
				messages that either do not include the Session-Binding AVP or
				include the Session-Binding AVP with any of the bits set to a zero
				value.  If present, this AVP MAY inform the Diameter client that if a
				re-auth or STR message fails due to a delivery problem, the Diameter
				client SHOULD issue a subsequent message without the Destination-Host
				AVP.  When absent, the default value is REFUSE_SERVICE.

				The following values are supported:


				REFUSE_SERVICE 0

				If either the re-auth or the STR message delivery fails, terminate
				service with the user, and do not attempt any subsequent attempts.


				TRY_AGAIN 1

				If either the re-auth or the STR message delivery fails, resend
				the failed message without the Destination-Host AVP present.


				ALLOW_SERVICE 2

				If re-auth message delivery fails, assume that re-authorization
				succeeded.  If STR message delivery fails, terminate the session.


				TRY_AGAIN_ALLOW_SERVICE 3

				If either the re-auth or the STR message delivery fails, resend
				the failed message without the Destination-Host AVP present.  If
				the second delivery fails for re-auth, assume re-authorization
				succeeded.  If the second delivery fails for STR, terminate the
				session.
			*/
			struct dict_object  	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Session-Server-Failover)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_0 = { "REFUSE_SERVICE", 		{ .i32 = 0 }};
			struct dict_enumval_data 	t_1 = { "TRY_AGAIN",			{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "ALLOW_SERVICE", 		{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "TRY_AGAIN_ALLOW_SERVICE",	{ .i32 = 3 }};
			struct dict_avp_data	 	data = { 
					271, 					/* Code */
					0, 					/* Vendor */
					"Session-Server-Failover",		/* Name */
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
		
		/* Multi-Round-Time-Out */
		{
			/*
				The Multi-Round-Time-Out AVP (AVP Code 272) is of type Unsigned32,
				and SHOULD be present in application-specific authorization answer
				messages whose Result-Code AVP is set to DIAMETER_MULTI_ROUND_AUTH.
				This AVP contains the maximum number of seconds that the access
				device MUST provide the user in responding to an authentication
				request.
			*/
			struct dict_avp_data data = { 
					272, 					/* Code */
					0, 					/* Vendor */
					"Multi-Round-Time-Out", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Class */
		{
			/*
				The Class AVP (AVP Code 25) is of type OctetString and is used to by
				Diameter servers to return state information to the access device.
				When one or more Class AVPs are present in application-specific
				authorization answer messages, they MUST be present in subsequent re-
				authorization, session termination and accounting messages.  Class
				AVPs found in a re-authorization answer message override the ones
				found in any previous authorization answer message.  Diameter server
				implementations SHOULD NOT return Class AVPs that require more than
				4096 bytes of storage on the Diameter client.  A Diameter client that
				receives Class AVPs whose size exceeds local available storage MUST
				terminate the session.
			*/
			struct dict_avp_data data = { 
					25, 					/* Code */
					0, 					/* Vendor */
					"Class", 				/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Event-Timestamp */
		{
			/*
				The Event-Timestamp (AVP Code 55) is of type Time, and MAY be
				included in an Accounting-Request and Accounting-Answer messages to
				record the time that the reported event occurred, in seconds since
				January 1, 1900 00:00 UTC.
			*/
			struct dict_avp_data data = { 
					55, 					/* Code */
					0, 					/* Vendor */
					"Event-Timestamp", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , Time_type, NULL);
		}

				
		/* Accounting-Record-Type */
		{
			/*
				The Accounting-Record-Type AVP (AVP Code 480) is of type Enumerated
				and contains the type of accounting record being sent.  The following
				values are currently defined for the Accounting-Record-Type AVP:


				EVENT_RECORD 1

				An Accounting Event Record is used to indicate that a one-time
				event has occurred (meaning that the start and end of the event
				are simultaneous).  This record contains all information relevant
				to the service, and is the only record of the service.


				START_RECORD 2

				An Accounting Start, Interim, and Stop Records are used to
				indicate that a service of a measurable length has been given.  An
				Accounting Start Record is used to initiate an accounting session,
				and contains accounting information that is relevant to the
				initiation of the session.


				INTERIM_RECORD 3

				An Interim Accounting Record contains cumulative accounting
				information for an existing accounting session.  Interim
				Accounting Records SHOULD be sent every time a re-authentication
				or re-authorization occurs.  Further, additional interim record
				triggers MAY be defined by application-specific Diameter
				applications.  The selection of whether to use INTERIM_RECORD
				records is done by the Acct-Interim-Interval AVP.


				STOP_RECORD 4

				An Accounting Stop Record is sent to terminate an accounting
				session and contains cumulative accounting information relevant to
				the existing session.
			*/
			struct dict_object 	* 	type;
			struct dict_type_data	  	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Accounting-Record-Type)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_1 = { "EVENT_RECORD",			{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "START_RECORD", 		{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "INTERIM_RECORD",		{ .i32 = 3 }};
			struct dict_enumval_data 	t_4 = { "STOP_RECORD", 			{ .i32 = 4 }};
			struct dict_avp_data 	data = { 
					480, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Record-Type",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
		/* Acct-Interim-Interval */
		{
			/*
				The Acct-Interim-Interval AVP (AVP Code 85) is of type Unsigned32 and
				is sent from the Diameter home authorization server to the Diameter
				client.  The client uses information in this AVP to decide how and
				when to produce accounting records.  With different values in this
				AVP, service sessions can result in one, two, or two+N accounting
				records, based on the needs of the home-organization.  The following
				accounting record production behavior is directed by the inclusion of
				this AVP:


				1.  The omission of the Acct-Interim-Interval AVP or its inclusion
				with Value field set to 0 means that EVENT_RECORD, START_RECORD,
				and STOP_RECORD are produced, as appropriate for the service.


				2.  The inclusion of the AVP with Value field set to a non-zero value
				means that INTERIM_RECORD records MUST be produced between the
				START_RECORD and STOP_RECORD records.  The Value field of this
				AVP is the nominal interval between these records in seconds.

				The Diameter node that originates the accounting information,
				known as the client, MUST produce the first INTERIM_RECORD record
				roughly at the time when this nominal interval has elapsed from
				the START_RECORD, the next one again as the interval has elapsed
				once more, and so on until the session ends and a STOP_RECORD
				record is produced.

				The client MUST ensure that the interim record production times
				are randomized so that large accounting message storms are not
				created either among records or around a common service start
				time.
			*/
			struct dict_avp_data data = { 
					85, 					/* Code */
					0, 					/* Vendor */
					"Acct-Interim-Interval", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Accounting-Record-Number */
		{
			/*
				The Accounting-Record-Number AVP (AVP Code 485) is of type Unsigned32
				and identifies this record within one session.  As Session-Id AVPs
				are globally unique, the combination of Session-Id and Accounting-
				Record-Number AVPs is also globally unique, and can be used in
				matching accounting records with confirmations.  An easy way to
				produce unique numbers is to set the value to 0 for records of type
				EVENT_RECORD and START_RECORD, and set the value to 1 for the first
				INTERIM_RECORD, 2 for the second, and so on until the value for
				STOP_RECORD is one more than for the last INTERIM_RECORD.
			*/
			struct dict_avp_data data = { 
					485, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Record-Number", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED32 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Acct-Session-Id */
		{
			/*
				The Acct-Session-Id AVP (AVP Code 44) is of type OctetString is only
				used when RADIUS/Diameter translation occurs.  This AVP contains the
				contents of the RADIUS Acct-Session-Id attribute.
			*/
			struct dict_avp_data data = { 
					44, 					/* Code */
					0, 					/* Vendor */
					"Acct-Session-Id", 			/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Acct-Multi-Session-Id */
		{
			/*
				The Acct-Multi-Session-Id AVP (AVP Code 50) is of type UTF8String,
				following the format specified in Section 8.8.  The Acct-Multi-
				Session-Id AVP is used to link together multiple related accounting
				sessions, where each session would have a unique Session-Id, but the
				same Acct-Multi-Session-Id AVP.  This AVP MAY be returned by the
				Diameter server in an authorization answer, and MUST be used in all
				accounting messages for the given session.
			*/
			struct dict_avp_data data = { 
					50, 					/* Code */
					0, 					/* Vendor */
					"Acct-Multi-Session-Id", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_OCTETSTRING 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}
		
		/* Accounting-Sub-Session-Id */
		{
			/*
				The Accounting-Sub-Session-Id AVP (AVP Code 287) is of type
				Unsigned64 and contains the accounting sub-session identifier.  The
				combination of the Session-Id and this AVP MUST be unique per sub-
				session, and the value of this AVP MUST be monotonically increased by
				one for all new sub-sessions.  The absence of this AVP implies no
				sub-sessions are in use, with the exception of an Accounting-Request
				whose Accounting-Record-Type is set to STOP_RECORD.  A STOP_RECORD
				message with no Accounting-Sub-Session-Id AVP present will signal the
				termination of all sub-sessions for a given Session-Id.
			*/
			struct dict_avp_data data = { 
					287, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Sub-Session-Id", 		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_UNSIGNED64 			/* base type of data */
					};
			CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
		}
		
		/* Accounting-Realtime-Required */
		{
			/*
				The Accounting-Realtime-Required AVP (AVP Code 483) is of type
				Enumerated and is sent from the Diameter home authorization server to
				the Diameter client or in the Accounting-Answer from the accounting
				server.  The client uses information in this AVP to decide what to do
				if the sending of accounting records to the accounting server has
				been temporarily prevented due to, for instance, a network problem.


				DELIVER_AND_GRANT 1

				The AVP with Value field set to DELIVER_AND_GRANT means that the
				service MUST only be granted as long as there is a connection to
				an accounting server.  Note that the set of alternative accounting
				servers are treated as one server in this sense.  Having to move
				the accounting record stream to a backup server is not a reason to
				discontinue the service to the user.


				GRANT_AND_STORE 2

				The AVP with Value field set to GRANT_AND_STORE means that service
				SHOULD be granted if there is a connection, or as long as records
				can still be stored as described in Section 9.4.

				This is the default behavior if the AVP isn't included in the
				reply from the authorization server.


				GRANT_AND_LOSE 3

				The AVP with Value field set to GRANT_AND_LOSE means that service
				SHOULD be granted even if the records can not be delivered or
				stored.
			*/
			struct dict_object  	* 	type;
			struct dict_type_data	 	tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Accounting-Realtime-Required)"	, NULL, NULL, NULL };
			struct dict_enumval_data 	t_1 = { "DELIVER_AND_GRANT",		{ .i32 = 1 }};
			struct dict_enumval_data 	t_2 = { "GRANT_AND_STORE", 		{ .i32 = 2 }};
			struct dict_enumval_data 	t_3 = { "GRANT_AND_LOSE",		{ .i32 = 3 }};
			struct dict_avp_data 		data = { 
					483, 					/* Code */
					0, 					/* Vendor */
					"Accounting-Realtime-Required",		/* Name */
					AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, 	/* Fixed flags */
					AVP_FLAG_MANDATORY,			/* Fixed flag values */
					AVP_TYPE_INTEGER32 			/* base type of data */
					};
			/* Create the Enumerated type, and then the AVP */
			CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
			CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
			CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
			CHECK_dict_new( DICT_AVP, &data , type, NULL);
		}
		
	}
	
	/* Commands section */
	{
		/* To avoid defining global variables for all the AVP that we use here, we do search the dictionary in each sub-block.
		 * This is far from optimal, but the code is clearer like this, and the time it requires at execution is not noticeable.
		 */
		
		/* Generic message syntax when the 'E' bit is set */
		{
			/*
				The 'E' (Error Bit) in the Diameter header is set when the request
				caused a protocol-related error (see Section 7.1.3).  A message with
				the 'E' bit MUST NOT be sent as a response to an answer message.
				Note that a message with the 'E' bit set is still subjected to the
				processing rules defined in Section 6.2.  When set, the answer
				message will not conform to the ABNF specification for the command,
				and will instead conform to the following ABNF:

				Message Format

				<answer-message> ::= < Diameter Header: code, ERR [PXY] >
                				0*1< Session-Id >
                				   { Origin-Host }
                				   { Origin-Realm }
                				   { Result-Code }
                				   [ Origin-State-Id ]
                				   [ Error-Message ]
                				   [ Error-Reporting-Host ]
                				   [ Failed-AVP ]
                				 * [ Proxy-Info ]
                				 * [ AVP ]

				Note that the code used in the header is the same than the one found
				in the request message, but with the 'R' bit cleared and the 'E' bit
				set.  The 'P' bit in the header is set to the same value as the one
				found in the request message.
			*/
			struct dict_object * cmd_error;
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD,0, 1 }
							,{  "Origin-Host",			RULE_REQUIRED, -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED, -1, 1 }
							,{  "Result-Code",			RULE_REQUIRED, -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL, -1, 1 }
							,{  "Error-Message",			RULE_OPTIONAL, -1, 1 }
							,{  "Error-Reporting-Host",		RULE_OPTIONAL, -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL, -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL, -1,-1 }
						};
			CHECK_FCT( fd_dict_get_error_cmd(dict, &cmd_error) );
			PARSE_loc_rules( rules, cmd_error );
		}		
		
		/* Capabilities-Exchange-Request */
		{
			/*
				The Capabilities-Exchange-Request (CER), indicated by the Command-
				Code set to 257 and the Command Flags' 'R' bit set, is sent to
				exchange local capabilities.  Upon detection of a transport failure,
				this message MUST NOT be sent to an alternate peer.

				When Diameter is run over SCTP [RFC2960], which allows for
				connections to span multiple interfaces and multiple IP addresses,
				the Capabilities-Exchange-Request message MUST contain one Host-IP-
				Address AVP for each potential IP address that MAY be locally used
				when transmitting Diameter messages.

				Message Format

				 <CER> ::= < Diameter Header: 257, REQ >
        				   { Origin-Host }
        				   { Origin-Realm }
        				1* { Host-IP-Address }
        				   { Vendor-Id }
        				   { Product-Name }
        				   [ Origin-State-Id ]
        				 * [ Supported-Vendor-Id ]
        				 * [ Auth-Application-Id ]
        				 * [ Inband-Security-Id ]
        				 * [ Acct-Application-Id ]
        				 * [ Vendor-Specific-Application-Id ]
        				   [ Firmware-Revision ]
        				 * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					257, 					/* Code */
					#if CC_CAPABILITIES_EXCHANGE != 257
					#error "CC_CAPABILITIES_EXCHANGE definition mismatch"
					#endif
					"Capabilities-Exchange-Request", 	/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_RETRANSMIT | CMD_FLAG_ERROR, 	/* Fixed flags */
					CMD_FLAG_REQUEST 			/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-Host", 			RULE_REQUIRED, -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED, -1, 1 }
							,{  "Host-IP-Address",			RULE_REQUIRED, -1,-1 }
							,{  "Vendor-Id",			RULE_REQUIRED, -1, 1 }
							,{  "Product-Name",			RULE_REQUIRED, -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL, -1, 1 }
							,{  "Supported-Vendor-Id",		RULE_OPTIONAL, -1,-1 }
							,{  "Auth-Application-Id",		RULE_OPTIONAL, -1,-1 }
							,{  "Inband-Security-Id",		RULE_OPTIONAL, -1,-1 }
							,{  "Acct-Application-Id",		RULE_OPTIONAL, -1,-1 }
							,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL, -1,-1 }
							,{  "Firmware-Revision",		RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Capabilities-Exchange-Answer */
		{
			/*
				The Capabilities-Exchange-Answer (CEA), indicated by the Command-Code
				set to 257 and the Command Flags' 'R' bit cleared, is sent in
				response to a CER message.

				When Diameter is run over SCTP [RFC2960], which allows connections to
				span multiple interfaces, hence, multiple IP addresses, the
				Capabilities-Exchange-Answer message MUST contain one Host-IP-Address
				AVP for each potential IP address that MAY be locally used when
				transmitting Diameter messages.

				Message Format

				 <CEA> ::= < Diameter Header: 257 >
        				   { Result-Code }
        				   { Origin-Host }
        				   { Origin-Realm }
        				1* { Host-IP-Address }
        				   { Vendor-Id }
        				   { Product-Name }
        				   [ Origin-State-Id ]
        				   [ Error-Message ]
        				   [ Failed-AVP ]
        				 * [ Supported-Vendor-Id ]
        				 * [ Auth-Application-Id ]
        				 * [ Inband-Security-Id ]
        				 * [ Acct-Application-Id ]
        				 * [ Vendor-Specific-Application-Id ]
        				   [ Firmware-Revision ]
        				 * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					257, 					/* Code */
					#if CC_CAPABILITIES_EXCHANGE != 257
					#error "CC_CAPABILITIES_EXCHANGE definition mismatch"
					#endif
					"Capabilities-Exchange-Answer", 	/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_RETRANSMIT, 	/* Fixed flags */
					0 					/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Result-Code", 			RULE_REQUIRED, -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED, -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED, -1, 1 }
							,{  "Host-IP-Address",			RULE_REQUIRED, -1,-1 }
							,{  "Vendor-Id",			RULE_REQUIRED, -1, 1 }
							,{  "Product-Name",			RULE_REQUIRED, -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL, -1, 1 }
							,{  "Error-Message",			RULE_OPTIONAL, -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL, -1, 1 }
							,{  "Supported-Vendor-Id",		RULE_OPTIONAL, -1,-1 }
							,{  "Auth-Application-Id",		RULE_OPTIONAL, -1,-1 }
							,{  "Inband-Security-Id",		RULE_OPTIONAL, -1,-1 }
							,{  "Acct-Application-Id",		RULE_OPTIONAL, -1,-1 }
							,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL, -1,-1 }
							,{  "Firmware-Revision",		RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Disconnect-Peer-Request */
		{
			/*
				The Disconnect-Peer-Request (DPR), indicated by the Command-Code set
				to 282 and the Command Flags' 'R' bit set, is sent to a peer to
				inform its intentions to shutdown the transport connection.  Upon
				detection of a transport failure, this message MUST NOT be sent to an
				alternate peer.

				Message Format

				 <DPR>  ::= < Diameter Header: 282, REQ >
        				    { Origin-Host }
        				    { Origin-Realm }
        				    { Disconnect-Cause }
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					282, 					/* Code */
					#if CC_DISCONNECT_PEER != 282
					#error "CC_DISCONNECT_PEER definition mismatch"
					#endif
					"Disconnect-Peer-Request", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_RETRANSMIT | CMD_FLAG_ERROR, 	/* Fixed flags */
					CMD_FLAG_REQUEST 			/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-Host", 			RULE_REQUIRED, -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED, -1, 1 }
							,{  "Disconnect-Cause",			RULE_REQUIRED, -1, 1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Disconnect-Peer-Answer */
		{
			/*
				The Disconnect-Peer-Answer (DPA), indicated by the Command-Code set
				to 282 and the Command Flags' 'R' bit cleared, is sent as a response
				to the Disconnect-Peer-Request message.  Upon receipt of this
				message, the transport connection is shutdown.

				Message Format

				 <DPA>  ::= < Diameter Header: 282 >
        				    { Result-Code }
        				    { Origin-Host }
        				    { Origin-Realm }
        				    [ Error-Message ]
        				    [ Failed-AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					282, 					/* Code */
					#if CC_DISCONNECT_PEER != 282
					#error "CC_DISCONNECT_PEER definition mismatch"
					#endif
					"Disconnect-Peer-Answer", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_RETRANSMIT, 	/* Fixed flags */
					0 					/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Result-Code", 			RULE_REQUIRED, -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED, -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED, -1, 1 }
							,{  "Error-Message",			RULE_OPTIONAL, -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}

		/* Device-Watchdog-Request */
		{
			/*
				The Device-Watchdog-Request (DWR), indicated by the Command-Code set
				to 280 and the Command Flags' 'R' bit set, is sent to a peer when no
				traffic has been exchanged between two peers (see Section 5.5.3).
				Upon detection of a transport failure, this message MUST NOT be sent
				to an alternate peer.

				Message Format

				 <DWR>  ::= < Diameter Header: 280, REQ >
        				    { Origin-Host }
        				    { Origin-Realm }
        				    [ Origin-State-Id ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					280, 					/* Code */
					#if CC_DEVICE_WATCHDOG != 280
					#error "CC_DEVICE_WATCHDOG definition mismatch"
					#endif
					"Device-Watchdog-Request", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_RETRANSMIT | CMD_FLAG_ERROR, 	/* Fixed flags */
					CMD_FLAG_REQUEST 			/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Origin-Host", 			RULE_REQUIRED, -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED, -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Device-Watchdog-Answer */
		{
			/*
				The Device-Watchdog-Answer (DWA), indicated by the Command-Code set
				to 280 and the Command Flags' 'R' bit cleared, is sent as a response
				to the Device-Watchdog-Request message.

				Message Format

				 <DWA>  ::= < Diameter Header: 280 >
        				    { Result-Code }
        				    { Origin-Host }
        				    { Origin-Realm }
        				    [ Error-Message ]
        				    [ Failed-AVP ]
        				    [ Origin-State-Id ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					280, 					/* Code */
					#if CC_DEVICE_WATCHDOG != 280
					#error "CC_DEVICE_WATCHDOG definition mismatch"
					#endif
					"Device-Watchdog-Answer", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_RETRANSMIT, 	/* Fixed flags */
					0 					/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Result-Code", 			RULE_REQUIRED, -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED, -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED, -1, 1 }
							,{  "Error-Message",			RULE_OPTIONAL, -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL, -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL, -1, 1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Re-Auth-Request */
		{
			/*
				The Re-Auth-Request (RAR), indicated by the Command-Code set to 258
				and the message flags' 'R' bit set, may be sent by any server to the
				access device that is providing session service, to request that the
				user be re-authenticated and/or re-authorized.


				Message Format

				 <RAR>  ::= < Diameter Header: 258, REQ, PXY >
        				    < Session-Id >
        				    { Origin-Host }
        				    { Origin-Realm }
        				    { Destination-Realm }
        				    { Destination-Host }
        				    { Auth-Application-Id }
        				    { Re-Auth-Request-Type }
        				    [ User-Name ]
        				    [ Origin-State-Id ]
        				  * [ Proxy-Info ]
        				  * [ Route-Record ]
        				  * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					258, 					/* Code */
					#if CC_RE_AUTH != 258
					#error "CC_RE_AUTH definition mismatch"
					#endif
					"Re-Auth-Request", 			/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR,	/* Fixed flags */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Realm",		RULE_REQUIRED,   -1, 1 }
						 	,{  "Destination-Host", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Re-Auth-Request-Type", 		RULE_REQUIRED,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
							,{  "Route-Record",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Re-Auth-Answer */
		{
			/*
				The Re-Auth-Answer (RAA), indicated by the Command-Code set to 258
				and the message flags' 'R' bit clear, is sent in response to the RAR.
				The Result-Code AVP MUST be present, and indicates the disposition of
				the request.

				A successful RAA message MUST be followed by an application-specific
				authentication and/or authorization message.


				Message Format

				 <RAA>  ::= < Diameter Header: 258, PXY >
        				    < Session-Id >
        				    { Result-Code }
        				    { Origin-Host }
        				    { Origin-Realm }
        				    [ User-Name ]
        				    [ Origin-State-Id ]
        				    [ Error-Message ]
        				    [ Error-Reporting-Host ]
        				    [ Failed-AVP ]
        				  * [ Redirect-Host ]
        				    [ Redirect-Host-Usage ]
        				    [ Redirect-Max-Cache-Time ]
        				  * [ Proxy-Info ]
        				  * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					258, 					/* Code */
					#if CC_RE_AUTH != 258
					#error "CC_RE_AUTH definition mismatch"
					#endif
					"Re-Auth-Answer", 			/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE,	/* Fixed flags */
							   CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
 							,{  "Result-Code", 			RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Message",			RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Reporting-Host",		RULE_OPTIONAL,   -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Redirect-Host-Usage",		RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Max-Cache-Time",		RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Session-Termination-Request */
		{
			/*
				The Session-Termination-Request (STR), indicated by the Command-Code
				set to 275 and the Command Flags' 'R' bit set, is sent by the access
				device to inform the Diameter Server that an authenticated and/or
				authorized session is being terminated.


        				   Message Format

				 <STR> ::= < Diameter Header: 275, REQ, PXY >
        				   < Session-Id >
        				   { Origin-Host }
        				   { Origin-Realm }
        				   { Destination-Realm }
        				   { Auth-Application-Id }
        				   { Termination-Cause }
        				   [ User-Name ]
        				   [ Destination-Host ]
        				 * [ Class ]
        				   [ Origin-State-Id ]
        				 * [ Proxy-Info ]
        				 * [ Route-Record ]
        				 * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					275, 					/* Code */
					#if CC_SESSION_TERMINATION != 275
					#error "CC_SESSION_TERMINATION definition mismatch"
					#endif
					"Session-Termination-Request", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR,	/* Fixed flags */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Realm",		RULE_REQUIRED,   -1, 1 }
						 	,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
						 	,{  "Termination-Cause", 		RULE_REQUIRED,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Destination-Host",			RULE_OPTIONAL,   -1, 1 }
							,{  "Class",				RULE_OPTIONAL,   -1,-1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
							,{  "Route-Record",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Session-Termination-Answer */
		{
			/*
				The Session-Termination-Answer (STA), indicated by the Command-Code
				set to 275 and the message flags' 'R' bit clear, is sent by the
				Diameter Server to acknowledge the notification that the session has
				been terminated.  The Result-Code AVP MUST be present, and MAY
				contain an indication that an error occurred while servicing the STR.

				Upon sending or receipt of the STA, the Diameter Server MUST release
				all resources for the session indicated by the Session-Id AVP.  Any
				intermediate server in the Proxy-Chain MAY also release any
				resources, if necessary.

        				    Message Format

				 <STA>  ::= < Diameter Header: 275, PXY >
        				    < Session-Id >
        				    { Result-Code }
        				    { Origin-Host }
        				    { Origin-Realm }
        				    [ User-Name ]
        				  * [ Class ]
        				    [ Error-Message ]
        				    [ Error-Reporting-Host ]
        				    [ Failed-AVP ]
        				    [ Origin-State-Id ]
        				  * [ Redirect-Host ]
        				    [ Redirect-Host-Usage ]
        				    [ Redirect-Max-Cache-Time ]
        				  * [ Proxy-Info ]
        				  * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					275, 					/* Code */
					#if CC_SESSION_TERMINATION != 275
					#error "CC_SESSION_TERMINATION definition mismatch"
					#endif
					"Session-Termination-Answer", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE,	/* Fixed flags */
							   CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
 							,{  "Result-Code", 			RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Class",				RULE_OPTIONAL,   -1,-1 }
							,{  "Error-Message",			RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Reporting-Host",		RULE_OPTIONAL,   -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Redirect-Host-Usage",		RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Max-Cache-Time",		RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Abort-Session-Request */
		{
			/*
				The Abort-Session-Request (ASR), indicated by the Command-Code set to
				274 and the message flags' 'R' bit set, may be sent by any server to
				the access device that is providing session service, to request that
				the session identified by the Session-Id be stopped.


        				    Message Format

				 <ASR>  ::= < Diameter Header: 274, REQ, PXY >
        				    < Session-Id >
        				    { Origin-Host }
        				    { Origin-Realm }
        				    { Destination-Realm }
        				    { Destination-Host }
        				    { Auth-Application-Id }
        				    [ User-Name ]
        				    [ Origin-State-Id ]
        				  * [ Proxy-Info ]
        				  * [ Route-Record ]
        				  * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					274, 					/* Code */
					#if CC_ABORT_SESSION != 274
					#error "CC_ABORT_SESSION definition mismatch"
					#endif
					"Abort-Session-Request", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR,	/* Fixed flags */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Realm",		RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Host",			RULE_REQUIRED,   -1, 1 }
						 	,{  "Auth-Application-Id", 		RULE_REQUIRED,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
							,{  "Route-Record",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Abort-Session-Answer */
		{
			/*
				The Abort-Session-Answer (ASA), indicated by the Command-Code set to
				274 and the message flags' 'R' bit clear, is sent in response to the
				ASR.  The Result-Code AVP MUST be present, and indicates the
				disposition of the request.

				If the session identified by Session-Id in the ASR was successfully
				terminated, Result-Code is set to DIAMETER_SUCCESS.  If the session
				is not currently active, Result-Code is set to
				DIAMETER_UNKNOWN_SESSION_ID.  If the access device does not stop the
				session for any other reason, Result-Code is set to
				DIAMETER_UNABLE_TO_COMPLY.

        				    Message Format

				 <ASA>  ::= < Diameter Header: 274, PXY >
        				    < Session-Id >
        				    { Result-Code }
        				    { Origin-Host }
        				    { Origin-Realm }
        				    [ User-Name ]
        				    [ Origin-State-Id ]
        				    [ Error-Message ]
        				    [ Error-Reporting-Host ]
        				    [ Failed-AVP ]
        				  * [ Redirect-Host ]
        				    [ Redirect-Host-Usage ]
        				    [ Redirect-Max-Cache-Time ]
        				  * [ Proxy-Info ]
        				  * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					274, 					/* Code */
					#if CC_ABORT_SESSION != 274
					#error "CC_ABORT_SESSION definition mismatch"
					#endif
					"Abort-Session-Answer", 		/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE,	/* Fixed flags */
							   CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
 							,{  "Result-Code", 			RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Message",			RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Reporting-Host",		RULE_OPTIONAL,   -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Host",			RULE_OPTIONAL,   -1,-1 }
							,{  "Redirect-Host-Usage",		RULE_OPTIONAL,   -1, 1 }
							,{  "Redirect-Max-Cache-Time",		RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Accounting-Request */
		{
			/*
				The Accounting-Request (ACR) command, indicated by the Command-Code
				field set to 271 and the Command Flags' 'R' bit set, is sent by a
				Diameter node, acting as a client, in order to exchange accounting
				information with a peer.

				One of Acct-Application-Id and Vendor-Specific-Application-Id AVPs
				MUST be present.  If the Vendor-Specific-Application-Id grouped AVP
				is present, it MUST include an Acct-Application-Id AVP.

				The AVP listed below SHOULD include service specific accounting AVPs,
				as described in Section 9.3.


				Message Format

				 <ACR> ::= < Diameter Header: 271, REQ, PXY >
        				   < Session-Id >
        				   { Origin-Host }
        				   { Origin-Realm }
        				   { Destination-Realm }
        				   { Accounting-Record-Type }
        				   { Accounting-Record-Number }
        				   [ Acct-Application-Id ]
        				   [ Vendor-Specific-Application-Id ]
        				   [ User-Name ]
        				   [ Destination-Host ]
        				   [ Accounting-Sub-Session-Id ]
        				   [ Acct-Session-Id ]
        				   [ Acct-Multi-Session-Id ]
        				   [ Acct-Interim-Interval ]
        				   [ Accounting-Realtime-Required ]
        				   [ Origin-State-Id ]
        				   [ Event-Timestamp ]
        				 * [ Proxy-Info ]
        				 * [ Route-Record ]
        				 * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					271, 					/* Code */
					#if CC_ACCOUNTING != 271
					#error "CC_ACCOUNTING definition mismatch"
					#endif
					"Accounting-Request", 			/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR,	/* Fixed flags */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "Destination-Realm",		RULE_REQUIRED,   -1, 1 }
							,{  "Accounting-Record-Type",		RULE_REQUIRED,   -1, 1 }
							,{  "Accounting-Record-Number",		RULE_REQUIRED,   -1, 1 }
							,{  "Acct-Application-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Destination-Host",			RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Sub-Session-Id",	RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Session-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Multi-Session-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Interim-Interval",		RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Realtime-Required",	RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Event-Timestamp",			RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
							,{  "Route-Record",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
		
		/* Accounting-Answer */
		{
			/*
				The Accounting-Answer (ACA) command, indicated by the Command-Code
				field set to 271 and the Command Flags' 'R' bit cleared, is used to
				acknowledge an Accounting-Request command.  The Accounting-Answer
				command contains the same Session-Id as the corresponding request.

				Only the target Diameter Server, known as the home Diameter Server,
				SHOULD respond with the Accounting-Answer command.

				One of Acct-Application-Id and Vendor-Specific-Application-Id AVPs
				MUST be present.  If the Vendor-Specific-Application-Id grouped AVP
				is present, it MUST contain an Acct-Application-Id AVP.

				The AVP listed below SHOULD include service specific accounting AVPs,
				as described in Section 9.3.


				Message Format

				 <ACA> ::= < Diameter Header: 271, PXY >
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
        				   [ Error-Message ]
        				   [ Error-Reporting-Host ]
        				   [ Failed-AVP ]
        				   [ Acct-Interim-Interval ]
        				   [ Accounting-Realtime-Required ]
        				   [ Origin-State-Id ]
        				   [ Event-Timestamp ]
        				 * [ Proxy-Info ]
        				 * [ AVP ]
			*/
			struct dict_object * cmd;
			struct dict_cmd_data data = { 
					271, 					/* Code */
					#if CC_ACCOUNTING != 271
					#error "CC_ACCOUNTING definition mismatch"
					#endif
					"Accounting-Answer", 			/* Name */
					CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE,	/* Fixed flags */
							   CMD_FLAG_PROXIABLE	/* Fixed flag values */
					};
			struct local_rules_definition rules[] = 
						{ 	 {  "Session-Id", 			RULE_FIXED_HEAD, -1, 1 }
 							,{  "Result-Code", 			RULE_REQUIRED,   -1, 1 }
						 	,{  "Origin-Host", 			RULE_REQUIRED,   -1, 1 }
							,{  "Origin-Realm",			RULE_REQUIRED,   -1, 1 }
							,{  "Accounting-Record-Type",		RULE_REQUIRED,   -1, 1 }
							,{  "Accounting-Record-Number",		RULE_REQUIRED,   -1, 1 }
							,{  "Acct-Application-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Vendor-Specific-Application-Id",	RULE_OPTIONAL,   -1, 1 }
							,{  "User-Name",			RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Sub-Session-Id",	RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Session-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Multi-Session-Id",		RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Message",			RULE_OPTIONAL,   -1, 1 }
							,{  "Error-Reporting-Host",		RULE_OPTIONAL,   -1, 1 }
							,{  "Failed-AVP",			RULE_OPTIONAL,   -1, 1 }
							,{  "Acct-Interim-Interval",		RULE_OPTIONAL,   -1, 1 }
							,{  "Accounting-Realtime-Required",	RULE_OPTIONAL,   -1, 1 }
							,{  "Origin-State-Id",			RULE_OPTIONAL,   -1, 1 }
							,{  "Event-Timestamp",			RULE_OPTIONAL,   -1, 1 }
							,{  "Proxy-Info",			RULE_OPTIONAL,   -1,-1 }
						};
			
			CHECK_dict_new( DICT_COMMAND, &data , NULL, &cmd);
			PARSE_loc_rules( rules, cmd );
		}
	}

	return 0;
}
