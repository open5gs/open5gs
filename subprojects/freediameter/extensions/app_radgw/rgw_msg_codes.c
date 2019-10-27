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

/*  Name of RADIUS command from its command code */
const char * rgw_msg_code_str(unsigned char c) {
		/* 1        Access-Request                           [RFC2865] */
	if ( c == 1) return "Access-Request [RFC2865]";
		/* 2        Access-Accept                            [RFC2865] */
	if ( c == 2) return "Access-Accept [RFC2865]";
		/* 3        Access-Reject                            [RFC2865] */
	if ( c == 3) return "Access-Reject [RFC2865]";
		/* 4        Accounting-Request                       [RFC2865] */
	if ( c == 4) return "Accounting-Request [RFC2865]";
		/* 5        Accounting-Response                      [RFC2865] */
	if ( c == 5) return "Accounting-Response [RFC2865]";
		/* 6        Accounting-Status                        [RFC3575] */
	if ( c == 6) return "Accounting-Status [RFC3575]";
		/* 7        Password-Request                         [RFC3575] */
	if ( c == 7) return "Password-Request [RFC3575]";
		/* 8        Password-Ack                             [RFC3575] */
	if ( c == 8) return "Password-Ack [RFC3575]";
		/* 9        Password-Reject                          [RFC3575] */
	if ( c == 9) return "Password-Reject [RFC3575]";
		/* 10       Accounting-Message                       [RFC3575] */
	if ( c == 10) return "Accounting-Message [RFC3575]";
		/* 11       Access-Challenge                         [RFC2865] */
	if ( c == 11) return "Access-Challenge [RFC2865]";
		/* 12       Status-Server (experimental)             [RFC2865] */
	if ( c == 12) return "Status-Server (experimental)[RFC2865]";
		/* 13       Status-Client (experimental)             [RFC2865] */
	if ( c == 13) return "Status-Client (experimental)[RFC2865]";
		/* 21       Resource-Free-Request                    [RFC3575] */
	if ( c == 21) return "Resource-Free-Request [RFC3575]";
		/* 22       Resource-Free-Response                   [RFC3575] */
	if ( c == 22) return "Resource-Free-Response [RFC3575]";
		/* 23       Resource-Query-Request                   [RFC3575] */
	if ( c == 23) return "Resource-Query-Request [RFC3575]";
		/* 24       Resource-Query-Response                  [RFC3575] */
	if ( c == 24) return "Resource-Query-Response [RFC3575]";
		/* 25       Alternate-Resource-Reclaim-Request       [RFC3575] */
	if ( c == 25) return "Alternate-Resource-Reclaim-Request [RFC3575]";
		/* 26       NAS-Reboot-Request                       [RFC3575] */
	if ( c == 26) return "NAS-Reboot-Request [RFC3575]";
		/* 27       NAS-Reboot-Response                      [RFC3575] */
	if ( c == 27) return "NAS-Reboot-Response [RFC3575]";
		/* 28       Reserved */
	if ( c == 28) return "Reserved ";
		/* 29       Next-Passcode                            [RFC3575] */
	if ( c == 29) return "Next-Passcode [RFC3575]";
		/* 30       New-Pin                                  [RFC3575] */
	if ( c == 30) return "New-Pin [RFC3575]";
		/* 31       Terminate-Session                        [RFC3575] */
	if ( c == 31) return "Terminate-Session [RFC3575]";
		/* 32       Password-Expired                         [RFC3575] */
	if ( c == 32) return "Password-Expired [RFC3575]";
		/* 33       Event-Request                            [RFC3575] */
	if ( c == 33) return "Event-Request [RFC3575]";
		/* 34       Event-Response                           [RFC3575] */
	if ( c == 34) return "Event-Response [RFC3575]";
		/* 40       Disconnect-Request                       [RFC3575][RFC5176] */
	if ( c == 40) return "Disconnect-Request [RFC3575][RFC5176]";
		/* 41       Disconnect-ACK                           [RFC3575][RFC5176] */
	if ( c == 41) return "Disconnect-ACK [RFC3575][RFC5176]";
		/* 42       Disconnect-NAK                           [RFC3575][RFC5176] */
	if ( c == 42) return "Disconnect-NAK [RFC3575][RFC5176]";
		/* 43       CoA-Request                              [RFC3575][RFC5176] */
	if ( c == 43) return "CoA-Request [RFC3575][RFC5176]";
		/* 44       CoA-ACK                                  [RFC3575][RFC5176] */
	if ( c == 44) return "CoA-ACK [RFC3575][RFC5176]";
		/* 45       CoA-NAK                                  [RFC3575][RFC5176] */
	if ( c == 45) return "CoA-NAK [RFC3575][RFC5176]";
		/* 50       IP-Address-Allocate                      [RFC3575] */
	if ( c == 50) return "IP-Address-Allocate [RFC3575]";
		/* 51       IP-Address-Release                       [RFC3575] */
	if ( c == 51) return "IP-Address-Release [RFC3575]";
		/* 52-249   Unassigned */
	if ((c >= 52) && (c <= 249)) return "Unassigned ";
		/* 250-253  Experimental Use                         [RFC3575] */
	if ((c >= 250) && (c <= 253)) return "Experimental Use[RFC3575]";
		/* 254      Reserved                                 [RFC3575] */
	if ( c == 254) return "Reserved [RFC3575]";
		/* 255      Reserved                                 [RFC3575] */
	if ( c == 255) return "Reserved [RFC3575]";
	/* fallback */ return "[Unknown]";
}
