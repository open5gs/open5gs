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

/* This file contains definitions for both app_radgw extension and its plugins. */

#ifndef _RGW_COMMON_H
#define _RGW_COMMON_H

/* Include definitions from the freeDiameter framework */
#include <freeDiameter/extension.h>

/* Include hostap files for RADIUS processings */
#include "hostap_compat.h"
#include "md5.h"
#include "radius.h"


/**************************************************************/
/*              Interface with gateway's plug-ins             */
/**************************************************************/
/* This structure is private for each plugin */
struct rgwp_config;

/* This structure points to a RADIUS client description, the definition is not known to plugins */
struct rgw_client;

int    rgw_clients_getkey(struct rgw_client * cli, unsigned char **key, size_t *key_len);
char * rgw_clients_id(struct rgw_client *cli);
int    rgw_clients_get_origin(struct rgw_client *cli, DiamId_t *fqdn, size_t *fqdnlen, DiamId_t *realm, size_t *realmlen);

/* Each plugin must provide the following structure. */
extern struct rgw_api {
	/* The name of the plugin */
	const char * rgwp_name;

	/* Parse the configuration file. It may be called several times with different configurations.
	    Called even if no configuration file is passed (with NULL conf_file parameter then) */
	int (*rgwp_conf_parse) ( char * conf_file, struct rgwp_config ** state );
	
	/* Cleanup the configuration state when the daemon is exiting (called even if state is NULL). */
	void (*rgwp_conf_free) (struct rgwp_config * state);

	/* handle an incoming RADIUS message */
	int	(*rgwp_rad_req) ( struct rgwp_config * conf, struct radius_msg * rad_req, struct radius_msg ** rad_ans, struct msg ** diam_fw, struct rgw_client * cli );
	/* ret >0: critical error (errno), log and exit.
	   ret 0: continue; 
	   ret -1: stop processing this message
	   ret -2: reply the content of rad_ans to the RADIUS client immediatly
	 */
	
	/* handle the corresponding Diameter answer */
	int	(*rgwp_diam_ans) ( struct rgwp_config * conf, struct msg ** diam_ans, struct radius_msg ** rad_fw, struct rgw_client * cli );
	/* ret 0: continue; ret >0: error; ret: -1 ... (tbd) */
	
} rgwp_descriptor;



/**************************************************************/
/*              Additional definitions                        */
/**************************************************************/
/* Type of message / server */
#define RGW_PLG_TYPE_AUTH	1
#define RGW_PLG_TYPE_ACCT	2

/* Class attribute prefix to store the Auth Application Id (required to send STR) */
#define CLASS_AAI_PREFIX 	"fD/rgwx/aai:"

/* Attributes missing from radius.h (not used in EAP) */
enum { RADIUS_ATTR_CHAP_PASSWORD = 3,
	RADIUS_ATTR_SERVICE_TYPE = 6,
	RADIUS_ATTR_FRAMED_PROTOCOL = 7,
	RADIUS_ATTR_FRAMED_IP_ADDRESS = 8,
	RADIUS_ATTR_FRAMED_IP_NETMASK = 9,
	RADIUS_ATTR_FRAMED_ROUTING = 10,
	RADIUS_ATTR_FILTER_ID = 11,
	RADIUS_ATTR_FRAMED_COMPRESSION = 13,
	RADIUS_ATTR_LOGIN_IP_HOST = 14,
	RADIUS_ATTR_LOGIN_SERVICE = 15,
	RADIUS_ATTR_LOGIN_TCP_PORT = 16,
	RADIUS_ATTR_CALLBACK_NUMBER = 19,
	RADIUS_ATTR_CALLBACK_ID = 20,
	RADIUS_ATTR_FRAMED_ROUTE = 22,
	RADIUS_ATTR_FRAMED_IPX_NETWORK = 23,
	RADIUS_ATTR_LOGIN_LAT_SERVICE = 34,
	RADIUS_ATTR_LOGIN_LAT_NODE = 35,
	RADIUS_ATTR_LOGIN_LAT_GROUP = 36,
	RADIUS_ATTR_FRAMED_APPLETALK_LINK = 37,
	RADIUS_ATTR_FRAMED_APPLETALK_NETWORK = 38,
	RADIUS_ATTR_FRAMED_APPLETALK_ZONE = 39,
	RADIUS_ATTR_CHAP_CHALLENGE = 60,
	RADIUS_ATTR_PORT_LIMIT = 62,
	RADIUS_ATTR_LOGIN_LAT_PORT = 63,
	RADIUS_ATTR_TUNNEL_CLIENT_ENDPOINT = 66,
	RADIUS_ATTR_TUNNEL_SERVER_ENDPOINT = 67,
	RADIUS_ATTR_TUNNEL_PASSWORD = 69,
	RADIUS_ATTR_ARAP_PASSWORD = 70,
	RADIUS_ATTR_ARAP_FEATURES = 71,
	RADIUS_ATTR_ARAP_ZONE_ACCESS = 72,
	RADIUS_ATTR_ARAP_SECURITY = 73,
	RADIUS_ATTR_ARAP_SECURITY_DATA = 74,
	RADIUS_ATTR_PASSWORD_RETRY = 75,
	RADIUS_ATTR_PROMPT = 76,
	RADIUS_ATTR_CONFIGURATION_TOKEN = 78,
	RADIUS_ATTR_TUNNEL_ASSIGNMENT_ID = 82,
	RADIUS_ATTR_TUNNEL_PREFERENCE = 83,
	RADIUS_ATTR_ARAP_CHALLENGE_RESPONSE = 84,
	RADIUS_ATTR_NAS_PORT_ID = 87,
	RADIUS_ATTR_FRAMED_POOL = 88,
	RADIUS_ATTR_TUNNEL_CLIENT_AUTH_ID = 90,
	RADIUS_ATTR_TUNNEL_SERVER_AUTH_ID = 91,
	RADIUS_ATTR_ORIGINATING_LINE_INFO = 94,
	RADIUS_ATTR_FRAMED_INTERFACE_ID = 96,
	RADIUS_ATTR_FRAMED_IPV6_PREFIX = 97,
	RADIUS_ATTR_LOGIN_IPV6_HOST = 98,
	RADIUS_ATTR_FRAMED_IPV6_ROUTE = 99,
	RADIUS_ATTR_FRAMED_IPV6_POOL = 100,
	RADIUS_ATTR_ERROR_CAUSE = 101,
	RADIUS_ATTR_EAP_KEY_NAME = 102,
	RADIUS_ATTR_DIGEST_RESPONSE = 103,
	RADIUS_ATTR_DIGEST_REALM = 104,
	RADIUS_ATTR_DIGEST_NONCE = 105,
	RADIUS_ATTR_DIGEST_RESPONSE_AUTH=106,
	RADIUS_ATTR_DIGEST_NEXTNONCE = 107,
	RADIUS_ATTR_DIGEST_METHOD = 108,
	RADIUS_ATTR_DIGEST_URI = 109,
	RADIUS_ATTR_DIGEST_QOP = 110,
	RADIUS_ATTR_DIGEST_ALGORITHM = 111,
	RADIUS_ATTR_DIGEST_ENTITY_BODY_HASH=112,
	RADIUS_ATTR_DIGEST_CNONCE = 113,
	RADIUS_ATTR_DIGEST_NONCE_COUNT = 114,
	RADIUS_ATTR_DIGEST_USERNAME = 115,
	RADIUS_ATTR_DIGEST_HA1 = 121,
	RADIUS_ATTR_SIP_AOR = 122
};

enum {  DIAM_ATTR_USER_NAME = 1,
	DIAM_ATTR_USER_PASSWORD = 2,
	DIAM_ATTR_NAS_IP_ADDRESS = 4,
	DIAM_ATTR_NAS_PORT = 5,
	DIAM_ATTR_SERVICE_TYPE = 6,
	DIAM_ATTR_FRAMED_PROTOCOL = 7,
	DIAM_ATTR_FRAMED_IP_ADDRESS = 8,
	DIAM_ATTR_FRAMED_IP_NETMASK = 9,
	DIAM_ATTR_FRAMED_ROUTING = 10,
	DIAM_ATTR_FILTER_ID = 11,
	DIAM_ATTR_FRAMED_MTU = 12,
	DIAM_ATTR_FRAMED_COMPRESSION = 13,
	DIAM_ATTR_LOGIN_IP_HOST = 14,
	DIAM_ATTR_LOGIN_SERVICE = 15,
	DIAM_ATTR_LOGIN_TCP_PORT = 16,
	DIAM_ATTR_REPLY_MESSAGE = 18,
	DIAM_ATTR_CALLBACK_NUMBER = 19,
	DIAM_ATTR_CALLBACK_ID = 20,
	DIAM_ATTR_FRAMED_ROUTE = 22,
	DIAM_ATTR_FRAMED_IPX_NETWORK = 23,
	DIAM_ATTR_STATE = 24,
	DIAM_ATTR_CLASS = 25,
	DIAM_ATTR_IDLE_TIMEOUT = 28,
	DIAM_ATTR_NAS_IDENTIFIER = 32,
	DIAM_ATTR_LOGIN_LAT_SERVICE = 34,
	DIAM_ATTR_LOGIN_LAT_NODE = 35,
	DIAM_ATTR_LOGIN_LAT_GROUP = 36,
	DIAM_ATTR_FRAMED_APPLETALK_LINK = 37,
	DIAM_ATTR_FRAMED_APPLETALK_NETWORK = 38,
	DIAM_ATTR_FRAMED_APPLETALK_ZONE = 39,
	DIAM_ATTR_ACCT_SESSION_ID = 44,
	DIAM_ATTR_ACCT_MULTI_SESSION_ID = 50,
	DIAM_ATTR_EVENT_TIMESTAMP = 55,
	DIAM_ATTR_NAS_PORT_TYPE = 61,
	DIAM_ATTR_PORT_LIMIT = 62,
	DIAM_ATTR_LOGIN_LAT_PORT = 63,
	DIAM_ATTR_TUNNEL_TYPE = 64,
	DIAM_ATTR_TUNNEL_MEDIUM_TYPE = 65,
	DIAM_ATTR_TUNNEL_CLIENT_ENDPOINT = 66,
	DIAM_ATTR_TUNNEL_SERVER_ENDPOINT = 67,
	DIAM_ATTR_TUNNEL_PASSWORD = 69,
	DIAM_ATTR_ARAP_FEATURES = 71,
	DIAM_ATTR_ARAP_ZONE_ACCESS = 72,
	DIAM_ATTR_ARAP_SECURITY = 73,
	DIAM_ATTR_ARAP_SECURITY_DATA = 74,
	DIAM_ATTR_PASSWORD_RETRY = 75,
	DIAM_ATTR_PROMPT = 76,
	DIAM_ATTR_CONFIGURATION_TOKEN = 78,
	DIAM_ATTR_TUNNEL_PRIVATE_GROUP_ID = 81,
	DIAM_ATTR_TUNNEL_ASSIGNMENT_ID = 82,
	DIAM_ATTR_TUNNEL_PREFERENCE = 83,
	DIAM_ATTR_ARAP_CHALLENGE_RESPONSE = 84,
	DIAM_ATTR_ACCT_INTERIM_INTERVAL = 85,
	DIAM_ATTR_NAS_PORT_ID = 87,
	DIAM_ATTR_FRAMED_POOL = 88,
	DIAM_ATTR_TUNNEL_CLIENT_AUTH_ID = 90,
	DIAM_ATTR_TUNNEL_SERVER_AUTH_ID = 91,
	DIAM_ATTR_NAS_IPV6_ADDRESS = 95,
	DIAM_ATTR_FRAMED_INTERFACE_ID = 96,
	DIAM_ATTR_FRAMED_IPV6_PREFIX = 97,
	DIAM_ATTR_LOGIN_IPV6_HOST = 98,
	DIAM_ATTR_FRAMED_IPV6_ROUTE = 99,
	DIAM_ATTR_FRAMED_IPV6_POOL = 100,
	DIAM_ATTR_EAP_KEY_NAME = 102,
	DIAM_ATTR_DIGEST_RESPONSE = 103,
	DIAM_ATTR_DIGEST_REALM = 104,
	DIAM_ATTR_DIGEST_NONCE = 105,
	DIAM_ATTR_DIGEST_RESPONSE_AUTH=106,
	DIAM_ATTR_DIGEST_NEXTNONCE = 107,
	DIAM_ATTR_DIGEST_METHOD = 108,
	DIAM_ATTR_DIGEST_URI = 109,
	DIAM_ATTR_DIGEST_QOP = 110,
	DIAM_ATTR_DIGEST_ALGORITHM = 111,
	DIAM_ATTR_DIGEST_ENTITY_BODY_HASH=112,
	DIAM_ATTR_DIGEST_CNONCE = 113,
	DIAM_ATTR_DIGEST_NONCE_COUNT = 114,
	DIAM_ATTR_DIGEST_USERNAME = 115,
	DIAM_ATTR_DIGEST_HA1 = 121,
	DIAM_ATTR_SIP_AOR = 122,
	DIAM_ATTR_AUTH_APPLICATION_ID = 258,
	DIAM_ATTR_ACCT_APPLICATION_ID = 259,
	DIAM_ATTR_VENDOR_SPECIFIC_APPLICATION_ID = 260,
	DIAM_ATTR_SESSION_ID = 263,
	DIAM_ATTR_ORIGIN_HOST = 264,
	DIAM_ATTR_MULTI_ROUND_TIMEOUT = 272,
	DIAM_ATTR_AUTH_REQUEST_TYPE = 274,
	DIAM_ATTR_AUTH_GRACE_PERIOD = 276,
	DIAM_ATTR_AUTH_SESSION_STATE = 277,
	DIAM_ATTR_ORIGIN_STATE_ID = 278,
	DIAM_ATTR_FAILED_AVP = 279,
	DIAM_ATTR_ERROR_MESSAGE = 281,
	DIAM_ATTR_ROUTE_RECORD = 282,
	DIAM_ATTR_PROXY_INFO = 284,
	DIAM_ATTR_ACCOUNTING_SUB_SESSION_ID = 287,
	DIAM_ATTR_ERROR_REPORTING_HOST = 294,
	DIAM_ATTR_TERMINATION_CAUSE = 295,
	DIAM_ATTR_ORIGIN_REALM = 296,
	DIAM_ATTR_SIP_AUTH_DATA_ITEM = 376,
	DIAM_ATTR_SIP_AUTHENTICATION_SCHEME = 377,
	DIAM_ATTR_SIP_AUTHENTICATE = 379,
	DIAM_ATTR_SIP_NUMBER_AUTH_ITEMS = 382,
	DIAM_ATTR_NAS_FILTER_RULE = 400,
	DIAM_ATTR_TUNNELING = 401,
	DIAM_ATTR_QOS_FILTER_RULE = 407,
	DIAM_ATTR_ORIGIN_AAA_PROTOCOL = 408,
	DIAM_ATTR_EAP_PAYLOAD = 462,
	DIAM_ATTR_EAP_REISSUED_PAYLOAD = 463,
	DIAM_ATTR_EAP_MASTER_SESSION_KEY = 464,
	DIAM_ATTR_ACCOUNTING_EAP_AUTH_METHOD = 465,
	DIAM_ATTR_ACCOUNTING_RECORD_TYPE = 480,
	DIAM_ATTR_ACCOUNTING_REALTIME_REQUIRED = 483,
	DIAM_ATTR_ACCOUNTING_RECORD_NUMBER = 485
};

const char * rgw_msg_attrtype_str(unsigned char c);
const char * rgw_msg_code_str(unsigned char c);

#endif /* _RGW_COMMON_H */
  
