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

/*  RADIUS translation plugin, to handle specific RADIUS attributes by either caching them and 
adding them to a corresponding RADIUS reply, or just dropping the attributes (no conversion to Diameter) */
/* This extension is a kind of swiss army-knife for interoperability, must be used with care. 
  All attribute behaviors are specified through the configuration file of the extension
 */

#include "rgw_common.h"

/* Action associated with an attribute */
#define ACT_ECHO 1
#define ACT_DROP 2

/* Result of configuration parsing */
struct rgwp_config {
	/* list of attributes and associated actions */
	struct fd_list attributes;
	
	/* Handler to store the echo'ed attributes values */
	struct session_handler * sess_hdl;
};

/* An item of the attribute list */
struct ed_conf_attribute {
	struct fd_list 	chain;	/* link in the list */
	
	struct {
		unsigned	action 	:2;	/* ACT_ECHO or ACT_DROP */
		unsigned	vsa	:1;	/* Interpret as Vendor-Specific, and match the vendor id in addition to the code */
		unsigned	tlv	:1;	/* Interpret as Vendor-Specific with TLV format, and match the type also */
		unsigned	ext	:1;	/* Interpret as radius extended attribute, and match the ext-type also */
	};
	
	uint32_t	vendor_id; /* vendor id to match when vsa = 1 */
	uint16_t	extype; /* Ext-Type value to match if ext = 1 */
	uint8_t		type; /* Type value to match if tlv = 1; */
	
	uint8_t		code; /* The attribute code, the list is ordered by this value */
};

/* For ECHO items, we save a list of these in the session */
struct ed_saved_attribute {
	struct fd_list		chain;
	struct radius_attr_hdr  attr; /* copy of the attribute */
	/* The data of the attribute follows ... */
};


/* The yacc parser */
int ed_conffile_parse(char * conffile, struct rgwp_config *cs);
