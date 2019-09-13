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

/* Debug plugin for app_radgw */

#include "rgw_common.h"

/* Store the configuration string in the state */
static int debug_conf_parse ( char * conf_file, struct rgwp_config ** state )
{
	TRACE_ENTRY("%p %p", conf_file, state);
	CHECK_PARAMS(state);
	
	*state = (void *)conf_file;
	
	return 0;
}


/* Function to display the content of a RADIUS message (more friendly way than radius_msg_dump) */
static void debug_dump_radius(struct radius_msg *msg)
{
	unsigned char *auth;
	size_t i;
	
	auth =  &(msg->hdr->authenticator[0]);
	fd_log_debug(" id  : 0x%02hhx, code: %hhd (%s)", msg->hdr->identifier, msg->hdr->code, rgw_msg_code_str(msg->hdr->code));
	fd_log_debug(" auth: %02hhx %02hhx %02hhx %02hhx  %02hhx %02hhx %02hhx %02hhx",
			auth[0], auth[1], auth[2], auth[3], 
			auth[4], auth[5], auth[6], auth[7]);
	fd_log_debug("       %02hhx %02hhx %02hhx %02hhx  %02hhx %02hhx %02hhx %02hhx",
			auth[8],  auth[9],  auth[10], auth[11], 
			auth[12], auth[13], auth[14], auth[15]);
	for (i = 0; i < msg->attr_used; i++) {
		struct radius_attr_hdr *attr = (struct radius_attr_hdr *)(msg->buf + msg->attr_pos[i]);
		fd_log_debug("  - len:%3hhu, type:0x%02hhx (%s)", attr->length, attr->type, rgw_msg_attrtype_str(attr->type));
		/* If we need to dump the value, it's better to call directly radius_msg_dump instead... */
	}
}

/* Function called when a new RADIUS message is being converted to Diameter */
static int debug_rad_req( struct rgwp_config * cs, struct radius_msg * rad_req, struct radius_msg ** rad_ans, struct msg ** diam_fw, struct rgw_client * cli )
{
	TRACE_ENTRY("%p %p %p %p %p", cs, rad_req, rad_ans, diam_fw, cli);
	
	fd_log_debug("------------- RADIUS/Diameter Request Debug%s%s%s -------------", cs ? " [" : "", cs ? (char *)cs : "", cs ? "]" : "");
	
	if (!rad_req) {
		fd_log_debug(" RADIUS request: NULL pointer");
	} else {
		fd_log_debug(" RADIUS request (%p) DUMP:", rad_req);
		debug_dump_radius(rad_req);
	}
	
	if (!rad_ans || ! *rad_ans) {
		fd_log_debug(" RADIUS answer: NULL pointer");
	} else {
		fd_log_debug(" RADIUS answer (%p) DUMP:", *rad_ans);
		debug_dump_radius(*rad_ans);
	}
	
	if (!diam_fw || ! *diam_fw) {
		fd_log_debug(" Diameter message: NULL pointer");
	} else {
		char * buf = NULL; size_t buflen;
		CHECK_MALLOC( fd_msg_dump_treeview(&buf, &buflen, NULL, *diam_fw, NULL, 0, 1) );
		fd_log_debug(" Diameter message (%p) DUMP: %s", *diam_fw, buf);
		free(buf);
	}
	
	fd_log_debug("===========  Debug%s%s%s complete =============", cs ? " [" : "", cs ? (char *)cs : "", cs ? "]" : "");
	
	return 0;
}

/* This one, when Diameter answer is converted to RADIUS */
static int debug_diam_ans( struct rgwp_config * cs, struct msg ** diam_ans, struct radius_msg ** rad_fw, struct rgw_client * cli )
{
	TRACE_ENTRY("%p %p %p %p", cs, diam_ans, rad_fw, cli);

	fd_log_debug("------------- RADIUS/Diameter Answer Debug%s%s%s -------------", cs ? " [" : "", cs ? (char *)cs : "", cs ? "]" : "");
	
	if (!diam_ans || ! *diam_ans) {
		fd_log_debug(" Diameter message: NULL pointer");
	} else {
		char * buf = NULL; size_t buflen;
		CHECK_MALLOC( fd_msg_dump_treeview(&buf, &buflen, NULL, *diam_ans, NULL, 0, 1) );
		fd_log_debug(" Diameter message (%p) DUMP: %s", *diam_ans, buf);
		free(buf);
	}
	
	if (!rad_fw || ! *rad_fw) {
		fd_log_debug(" RADIUS answer: NULL pointer");
	} else {
		fd_log_debug(" RADIUS answer (%p) DUMP:", *rad_fw);
		debug_dump_radius(*rad_fw);
	}
	
	fd_log_debug("===========  Debug%s%s%s complete =============", cs ? " [" : "", cs ? (char *)cs : "", cs ? "]" : "");
	return 0;
}


/* The exported symbol */
struct rgw_api rgwp_descriptor = {
	.rgwp_name       = "debug",
	.rgwp_conf_parse = debug_conf_parse,
	.rgwp_conf_free  = NULL,
	.rgwp_rad_req    = debug_rad_req,
	.rgwp_diam_ans   = debug_diam_ans
};	
