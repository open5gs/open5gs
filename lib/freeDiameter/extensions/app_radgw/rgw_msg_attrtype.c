/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2011, WIDE Project and NICT								 *
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
#include "rgw_common.h"

/* The content of this file was semi-automatically generated from the IANA registry. */

/*  Name of RADIUS attribute from its code */
const char * rgw_msg_attrtype_str(unsigned char c) {
		/* 1         User-Name                                 */
	if ( c == 1) return "User-Name ";
		/* 2         User-Password                             */
	if ( c == 2) return "User-Password ";
		/* 3         CHAP-Password                             */
	if ( c == 3) return "CHAP-Password ";
		/* 4         NAS-IP-Address                            */
	if ( c == 4) return "NAS-IP-Address ";
		/* 5         NAS-Port                                  */
	if ( c == 5) return "NAS-Port ";
		/* 6         Service-Type                              */
	if ( c == 6) return "Service-Type ";
		/* 7         Framed-Protocol                           */
	if ( c == 7) return "Framed-Protocol ";
		/* 8         Framed-IP-Address                         */
	if ( c == 8) return "Framed-IP-Address ";
		/* 9         Framed-IP-Netmask                         */
	if ( c == 9) return "Framed-IP-Netmask ";
		/* 10        Framed-Routing                            */
	if ( c == 10) return "Framed-Routing ";
		/* 11        Filter-Id                                 */
	if ( c == 11) return "Filter-Id ";
		/* 12        Framed-MTU                                */
	if ( c == 12) return "Framed-MTU ";
		/* 13        Framed-Compression                        */
	if ( c == 13) return "Framed-Compression ";
		/* 14        Login-IP-Host                             */
	if ( c == 14) return "Login-IP-Host ";
		/* 15        Login-Service                             */
	if ( c == 15) return "Login-Service ";
		/* 16        Login-TCP-Port                            */
	if ( c == 16) return "Login-TCP-Port ";
		/* 17        Unassigned */
	if ( c == 17) return "Unassigned ";
		/* 18        Reply-Message                             */
	if ( c == 18) return "Reply-Message ";
		/* 19        Callback-Number                           */
	if ( c == 19) return "Callback-Number ";
		/* 20        Callback-Id                               */
	if ( c == 20) return "Callback-Id ";
		/* 21        Unassigned */
	if ( c == 21) return "Unassigned ";
		/* 22        Framed-Route                              */
	if ( c == 22) return "Framed-Route ";
		/* 23        Framed-IPX-Network                        */
	if ( c == 23) return "Framed-IPX-Network ";
		/* 24        State                                     */
	if ( c == 24) return "State ";
		/* 25        Class                                     */
	if ( c == 25) return "Class ";
		/* 26        Vendor-Specific                           */
	if ( c == 26) return "Vendor-Specific ";
		/* 27        Session-Timeout                           */
	if ( c == 27) return "Session-Timeout ";
		/* 28        Idle-Timeout                              */
	if ( c == 28) return "Idle-Timeout ";
		/* 29        Termination-Action                        */
	if ( c == 29) return "Termination-Action ";
		/* 30        Called-Station-Id                         */
	if ( c == 30) return "Called-Station-Id ";
		/* 31        Calling-Station-Id                        */
	if ( c == 31) return "Calling-Station-Id ";
		/* 32        NAS-Identifier                            */
	if ( c == 32) return "NAS-Identifier ";
		/* 33        Proxy-State                               */
	if ( c == 33) return "Proxy-State ";
		/* 34        Login-LAT-Service                         */
	if ( c == 34) return "Login-LAT-Service ";
		/* 35        Login-LAT-Node                            */
	if ( c == 35) return "Login-LAT-Node ";
		/* 36        Login-LAT-Group                           */
	if ( c == 36) return "Login-LAT-Group ";
		/* 37        Framed-AppleTalk-Link                     */
	if ( c == 37) return "Framed-AppleTalk-Link ";
		/* 38        Framed-AppleTalk-Network                  */
	if ( c == 38) return "Framed-AppleTalk-Network ";
		/* 39        Framed-AppleTalk-Zone                     */
	if ( c == 39) return "Framed-AppleTalk-Zone ";
		/* 40        Acct-Status-Type                         [RFC2866] */
	if ( c == 40) return "Acct-Status-Type [RFC2866]";
		/* 41        Acct-Delay-Time                          [RFC2866] */
	if ( c == 41) return "Acct-Delay-Time [RFC2866]";
		/* 42        Acct-Input-Octets                        [RFC2866] */
	if ( c == 42) return "Acct-Input-Octets [RFC2866]";
		/* 43        Acct-Output-Octets                       [RFC2866] */
	if ( c == 43) return "Acct-Output-Octets [RFC2866]";
		/* 44        Acct-Session-Id                          [RFC2866] */
	if ( c == 44) return "Acct-Session-Id [RFC2866]";
		/* 45        Acct-Authentic                           [RFC2866] */
	if ( c == 45) return "Acct-Authentic [RFC2866]";
		/* 46        Acct-Session-Time                        [RFC2866] */
	if ( c == 46) return "Acct-Session-Time [RFC2866]";
		/* 47        Acct-Input-Packets                       [RFC2866] */
	if ( c == 47) return "Acct-Input-Packets [RFC2866]";
		/* 48        Acct-Output-Packets                      [RFC2866] */
	if ( c == 48) return "Acct-Output-Packets [RFC2866]";
		/* 49        Acct-Terminate-Cause                     [RFC2866] */
	if ( c == 49) return "Acct-Terminate-Cause [RFC2866]";
		/* 50        Acct-Multi-Session-Id                    [RFC2866] */
	if ( c == 50) return "Acct-Multi-Session-Id [RFC2866]";
		/* 51        Acct-Link-Count                          [RFC2866] */
	if ( c == 51) return "Acct-Link-Count [RFC2866]";
		/* 52        Acct-Input-Gigawords                     [RFC2869] */
	if ( c == 52) return "Acct-Input-Gigawords [RFC2869]";
		/* 53        Acct-Output-Gigawords                    [RFC2869] */
	if ( c == 53) return "Acct-Output-Gigawords [RFC2869]";
		/* 54        Unassigned */
	if ( c == 54) return "Unassigned ";
		/* 55        Event-Timestamp                          [RFC2869] */
	if ( c == 55) return "Event-Timestamp [RFC2869]";
		/* 56        Egress-VLANID                            [RFC4675] */
	if ( c == 56) return "Egress-VLANID [RFC4675]";
		/* 57        Ingress-Filters                          [RFC4675] */
	if ( c == 57) return "Ingress-Filters [RFC4675]";
		/* 58        Egress-VLAN-Name                         [RFC4675] */
	if ( c == 58) return "Egress-VLAN-Name [RFC4675]";
		/* 59        User-Priority-Table                      [RFC4675] */
	if ( c == 59) return "User-Priority-Table [RFC4675]";
		/* 60        CHAP-Challenge */
	if ( c == 60) return "CHAP-Challenge ";
		/* 61        NAS-Port-Type */
	if ( c == 61) return "NAS-Port-Type ";
		/* 62        Port-Limit */
	if ( c == 62) return "Port-Limit ";
		/* 63        Login-LAT-Port */
	if ( c == 63) return "Login-LAT-Port ";
		/* 64        Tunnel-Type                              [RFC2868] */
	if ( c == 64) return "Tunnel-Type [RFC2868]";
		/* 65        Tunnel-Medium-Type                       [RFC2868] */
	if ( c == 65) return "Tunnel-Medium-Type [RFC2868]";
		/* 66        Tunnel-Client-Endpoint                   [RFC2868] */
	if ( c == 66) return "Tunnel-Client-Endpoint [RFC2868]";
		/* 67        Tunnel-Server-Endpoint                   [RFC2868] */
	if ( c == 67) return "Tunnel-Server-Endpoint [RFC2868]";
		/* 68        Acct-Tunnel-Connection                   [RFC2867] */
	if ( c == 68) return "Acct-Tunnel-Connection [RFC2867]";
		/* 69        Tunnel-Password                          [RFC2868] */
	if ( c == 69) return "Tunnel-Password [RFC2868]";
		/* 70        ARAP-Password                            [RFC2869] */
	if ( c == 70) return "ARAP-Password [RFC2869]";
		/* 71        ARAP-Features                            [RFC2869] */
	if ( c == 71) return "ARAP-Features [RFC2869]";
		/* 72        ARAP-Zone-Access                         [RFC2869] */
	if ( c == 72) return "ARAP-Zone-Access [RFC2869]";
		/* 73        ARAP-Security                            [RFC2869] */
	if ( c == 73) return "ARAP-Security [RFC2869]";
		/* 74        ARAP-Security-Data                       [RFC2869] */
	if ( c == 74) return "ARAP-Security-Data [RFC2869]";
		/* 75        Password-Retry                           [RFC2869] */
	if ( c == 75) return "Password-Retry [RFC2869]";
		/* 76        Prompt                                   [RFC2869] */
	if ( c == 76) return "Prompt [RFC2869]";
		/* 77        Connect-Info                             [RFC2869] */
	if ( c == 77) return "Connect-Info [RFC2869]";
		/* 78        Configuration-Token                      [RFC2869] */
	if ( c == 78) return "Configuration-Token [RFC2869]";
		/* 79        EAP-Message                              [RFC2869] */
	if ( c == 79) return "EAP-Message [RFC2869]";
		/* 80        Message-Authenticator                    [RFC2869] */
	if ( c == 80) return "Message-Authenticator [RFC2869]";
		/* 81        Tunnel-Private-Group-ID                  [RFC2868] */
	if ( c == 81) return "Tunnel-Private-Group-ID [RFC2868]";
		/* 82        Tunnel-Assignment-ID                     [RFC2868] */
	if ( c == 82) return "Tunnel-Assignment-ID [RFC2868]";
		/* 83        Tunnel-Preference                        [RFC2868] */
	if ( c == 83) return "Tunnel-Preference [RFC2868]";
		/* 84        ARAP-Challenge-Response                  [RFC2869] */
	if ( c == 84) return "ARAP-Challenge-Response [RFC2869]";
		/* 85        Acct-Interim-Interval                    [RFC2869] */
	if ( c == 85) return "Acct-Interim-Interval [RFC2869]";
		/* 86        Acct-Tunnel-Packets-Lost                 [RFC2867] */
	if ( c == 86) return "Acct-Tunnel-Packets-Lost [RFC2867]";
		/* 87        NAS-Port-Id                              [RFC2869] */
	if ( c == 87) return "NAS-Port-Id [RFC2869]";
		/* 88        Framed-Pool                              [RFC2869] */
	if ( c == 88) return "Framed-Pool [RFC2869]";
		/* 89        CUI                                      [RFC4372] */
	if ( c == 89) return "CUI [RFC4372]";
		/* 90        Tunnel-Client-Auth-ID                    [RFC2868] */
	if ( c == 90) return "Tunnel-Client-Auth-ID [RFC2868]";
		/* 91        Tunnel-Server-Auth-ID                    [RFC2868] */
	if ( c == 91) return "Tunnel-Server-Auth-ID [RFC2868]";
		/* 92        NAS-Filter-Rule                          [RFC4849] */
	if ( c == 92) return "NAS-Filter-Rule [RFC4849]";
		/* 93        Unassigned */
	if ( c == 93) return "Unassigned ";
		/* 94        Originating-Line-Info                    [RFC4005] */
	if ( c == 94) return "Originating-Line-Info [RFC4005]";
		/* 95        NAS-IPv6-Address                         [RFC3162] */
	if ( c == 95) return "NAS-IPv6-Address [RFC3162]";
		/* 96        Framed-Interface-Id                      [RFC3162] */
	if ( c == 96) return "Framed-Interface-Id [RFC3162]";
		/* 97        Framed-IPv6-Prefix                       [RFC3162] */
	if ( c == 97) return "Framed-IPv6-Prefix [RFC3162]";
		/* 98        Login-IPv6-Host                          [RFC3162] */
	if ( c == 98) return "Login-IPv6-Host [RFC3162]";
		/* 99        Framed-IPv6-Route                        [RFC3162] */
	if ( c == 99) return "Framed-IPv6-Route [RFC3162]";
		/* 100       Framed-IPv6-Pool                         [RFC3162] */
	if ( c == 100) return "Framed-IPv6-Pool [RFC3162]";
		/* 101       Error-Cause Attribute                    [RFC3576] */
	if ( c == 101) return "Error-Cause Attribute[RFC3576]";
		/* 102       EAP-Key-Name                             [RFC4072] */
	if ( c == 102) return "EAP-Key-Name [RFC4072]";
		/* 103       Digest-Response                          [RFC5090] */
	if ( c == 103) return "Digest-Response [RFC5090]";
		/* 104       Digest-Realm                             [RFC5090] */
	if ( c == 104) return "Digest-Realm [RFC5090]";
		/* 105       Digest-Nonce                             [RFC5090]   */
	if ( c == 105) return "Digest-Nonce [RFC5090]";
		/* 106       Digest-Response-Auth                     [RFC5090] */
	if ( c == 106) return "Digest-Response-Auth [RFC5090]";
		/* 107       Digest-Nextnonce                         [RFC5090] */
	if ( c == 107) return "Digest-Nextnonce [RFC5090]";
		/* 108       Digest-Method                            [RFC5090] */
	if ( c == 108) return "Digest-Method [RFC5090]";
		/* 109       Digest-URI                               [RFC5090]  */
	if ( c == 109) return "Digest-URI [RFC5090]";
		/* 110       Digest-Qop                               [RFC5090]  */
	if ( c == 110) return "Digest-Qop [RFC5090]";
		/* 111       Digest-Algorithm                         [RFC5090]  */
	if ( c == 111) return "Digest-Algorithm [RFC5090]";
		/* 112       Digest-Entity-Body-Hash                  [RFC5090]  */
	if ( c == 112) return "Digest-Entity-Body-Hash [RFC5090]";
		/* 113       Digest-CNonce                            [RFC5090]  */
	if ( c == 113) return "Digest-CNonce [RFC5090]";
		/* 114       Digest-Nonce-Count                       [RFC5090]  */
	if ( c == 114) return "Digest-Nonce-Count [RFC5090]";
		/* 115       Digest-Username                          [RFC5090]  */
	if ( c == 115) return "Digest-Username [RFC5090]";
		/* 116       Digest-Opaque                            [RFC5090]  */
	if ( c == 116) return "Digest-Opaque [RFC5090]";
		/* 117       Digest-Auth-Param                        [RFC5090]  */
	if ( c == 117) return "Digest-Auth-Param [RFC5090]";
		/* 118       Digest-AKA-Auts                          [RFC5090]  */
	if ( c == 118) return "Digest-AKA-Auts [RFC5090]";
		/* 119       Digest-Domain                            [RFC5090]  */
	if ( c == 119) return "Digest-Domain [RFC5090]";
		/* 120       Digest-Stale                             [RFC5090]  */
	if ( c == 120) return "Digest-Stale [RFC5090]";
		/* 121       Digest-HA1                               [RFC5090]  */
	if ( c == 121) return "Digest-HA1 [RFC5090]";
		/* 122       SIP-AOR                                  [RFC5090]  */
	if ( c == 122) return "SIP-AOR [RFC5090]";
		/* 123       Delegated-IPv6-Prefix                    [RFC4818] */
	if ( c == 123) return "Delegated-IPv6-Prefix [RFC4818]";
		/* 124       MIP6-Feature-Vector                      [RFC5447] */
	if ( c == 124) return "MIP6-Feature-Vector [RFC5447]";
		/* 125       MIP6-Home-Link-Prefix                    [RFC5447] */
	if ( c == 125) return "MIP6-Home-Link-Prefix [RFC5447]";
		/* 126-191   Unassigned */
	if ((c >= 126) && (c <= 191)) return "Unassigned ";
		/* 192-223   Experimental Use                         [RFC3575] */
	if ((c >= 192) && (c <= 223)) return "Experimental Use[RFC3575]";
		/* 224-240   Implementation Specific                  [RFC3575] */
	if ((c >= 224) && (c <= 240)) return "Implementation Specific[RFC3575]";
		/* 241-255   Reserved                                 [RFC3575]    */
	if ((c >= 241)) return "Reserved [RFC3575]";
	/* fallback */ return "[Unknown]";
}
