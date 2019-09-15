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

/* freeDiameter author note:
 *  The content from this file comes for the main part from the hostap project.
 * It is redistributed under the terms of the BSD license, as allowed
 * by the original copyright reproduced below.
 * The modifications to this file are placed under the copyright of the freeDiameter project.
 */

/*
 * hostapd / RADIUS message processing
 * Copyright (c) 2002-2008, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

/*********************************************************************************/
#include "rgw.h"

static struct radius_attr_hdr *
radius_get_attr_hdr(struct radius_msg *msg, int idx)
{
	return (struct radius_attr_hdr *) (msg->buf + msg->attr_pos[idx]);
}


struct radius_msg *radius_msg_new(u8 code, u8 identifier)
{
	struct radius_msg *msg;

	msg = os_malloc(sizeof(*msg));
	if (msg == NULL)
		return NULL;

	if (radius_msg_initialize(msg, RADIUS_DEFAULT_MSG_SIZE)) {
		os_free(msg);
		return NULL;
	}

	radius_msg_set_hdr(msg, code, identifier);

	return msg;
}


int radius_msg_initialize(struct radius_msg *msg, size_t init_len)
{
	if (msg == NULL || init_len < sizeof(struct radius_hdr))
		return -1;

	os_memset(msg, 0, sizeof(*msg));
	msg->buf = os_zalloc(init_len);
	if (msg->buf == NULL)
		return -1;

	msg->buf_size = init_len;
	msg->hdr = (struct radius_hdr *) msg->buf;
	msg->buf_used = sizeof(*msg->hdr);

	msg->attr_pos =
		os_zalloc(RADIUS_DEFAULT_ATTR_COUNT * sizeof(*msg->attr_pos));
	if (msg->attr_pos == NULL) {
		os_free(msg->buf);
		msg->buf = NULL;
		msg->hdr = NULL;
		return -1;
	}

	msg->attr_size = RADIUS_DEFAULT_ATTR_COUNT;
	msg->attr_used = 0;

	return 0;
}


void radius_msg_set_hdr(struct radius_msg *msg, u8 code, u8 identifier)
{
	msg->hdr->code = code;
	msg->hdr->identifier = identifier;
}


void radius_msg_free(struct radius_msg *msg)
{
	os_free(msg->buf);
	msg->buf = NULL;
	msg->hdr = NULL;
	msg->buf_size = msg->buf_used = 0;

	os_free(msg->attr_pos);
	msg->attr_pos = NULL;
	msg->attr_size = msg->attr_used = 0;
}

/* Destroy a message */
void rgw_msg_free(struct rgw_radius_msg_meta ** msg)
{
	if (!msg || !*msg)
		return;
	
	radius_msg_free(&(*msg)->radius);
	free(*msg);
	*msg = NULL;
}



struct radius_attr_type {
	u8 type;
	char *name;
	enum {
		RADIUS_ATTR_UNDIST, RADIUS_ATTR_TEXT, RADIUS_ATTR_IP,
		RADIUS_ATTR_HEXDUMP, RADIUS_ATTR_INT32, RADIUS_ATTR_IPV6
	} data_type;
};

static struct radius_attr_type radius_attrs[] =
{
	{ RADIUS_ATTR_USER_NAME, "User-Name", RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_USER_PASSWORD, "User-Password", RADIUS_ATTR_UNDIST },
	{ RADIUS_ATTR_NAS_IP_ADDRESS, "NAS-IP-Address", RADIUS_ATTR_IP },
	{ RADIUS_ATTR_NAS_PORT, "NAS-Port", RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_FRAMED_MTU, "Framed-MTU", RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_REPLY_MESSAGE, "Reply-Message", RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_STATE, "State", RADIUS_ATTR_UNDIST },
	{ RADIUS_ATTR_CLASS, "Class", RADIUS_ATTR_UNDIST },
	{ RADIUS_ATTR_VENDOR_SPECIFIC, "Vendor-Specific", RADIUS_ATTR_UNDIST },
	{ RADIUS_ATTR_SESSION_TIMEOUT, "Session-Timeout", RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_IDLE_TIMEOUT, "Idle-Timeout", RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_TERMINATION_ACTION, "Termination-Action",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_CALLED_STATION_ID, "Called-Station-Id",
	  RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_CALLING_STATION_ID, "Calling-Station-Id",
	  RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_NAS_IDENTIFIER, "NAS-Identifier", RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_PROXY_STATE, "Proxy-State", RADIUS_ATTR_UNDIST },
	{ RADIUS_ATTR_ACCT_STATUS_TYPE, "Acct-Status-Type",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_DELAY_TIME, "Acct-Delay-Time", RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_INPUT_OCTETS, "Acct-Input-Octets",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_OUTPUT_OCTETS, "Acct-Output-Octets",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_SESSION_ID, "Acct-Session-Id", RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_ACCT_AUTHENTIC, "Acct-Authentic", RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_SESSION_TIME, "Acct-Session-Time",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_INPUT_PACKETS, "Acct-Input-Packets",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_OUTPUT_PACKETS, "Acct-Output-Packets",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_TERMINATE_CAUSE, "Acct-Terminate-Cause",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_MULTI_SESSION_ID, "Acct-Multi-Session-Id",
	  RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_ACCT_LINK_COUNT, "Acct-Link-Count", RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_INPUT_GIGAWORDS, "Acct-Input-Gigawords", 
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_ACCT_OUTPUT_GIGAWORDS, "Acct-Output-Gigawords",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_EVENT_TIMESTAMP, "Event-Timestamp",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_NAS_PORT_TYPE, "NAS-Port-Type", RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_TUNNEL_TYPE, "Tunnel-Type", RADIUS_ATTR_HEXDUMP },
	{ RADIUS_ATTR_TUNNEL_MEDIUM_TYPE, "Tunnel-Medium-Type",
	  RADIUS_ATTR_HEXDUMP },
	{ RADIUS_ATTR_CONNECT_INFO, "Connect-Info", RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_EAP_MESSAGE, "EAP-Message", RADIUS_ATTR_UNDIST },
	{ RADIUS_ATTR_MESSAGE_AUTHENTICATOR, "Message-Authenticator",
	  RADIUS_ATTR_UNDIST },
	{ RADIUS_ATTR_TUNNEL_PRIVATE_GROUP_ID, "Tunnel-Private-Group-Id",
	  RADIUS_ATTR_HEXDUMP },
	{ RADIUS_ATTR_ACCT_INTERIM_INTERVAL, "Acct-Interim-Interval",
	  RADIUS_ATTR_INT32 },
	{ RADIUS_ATTR_CHARGEABLE_USER_IDENTITY, "Chargeable-User-Identity",
	  RADIUS_ATTR_TEXT },
	{ RADIUS_ATTR_NAS_IPV6_ADDRESS, "NAS-IPv6-Address", RADIUS_ATTR_IPV6 },
};
#define RADIUS_ATTRS (sizeof(radius_attrs) / sizeof(radius_attrs[0]))


static struct radius_attr_type *radius_get_attr_type(u8 type)
{
	size_t i;

	for (i = 0; i < RADIUS_ATTRS; i++) {
		if (type == radius_attrs[i].type)
			return &radius_attrs[i];
	}

	return NULL;
}

static char print_char_buf[5];

static char * print_char(char c)
{
	if (c >= 32 && c < 127)
		sprintf(print_char_buf, "%c", c);
	else
		sprintf(print_char_buf, "<%02x>", c);
	return print_char_buf;
}


static char * radius_msg_dump_attr_val(struct radius_attr_hdr *hdr, char * outbuf, size_t buflen)
{
	struct radius_attr_type *attr;
	int i, len;
	unsigned char *pos;
	u8 attrtype;
	
	memset(outbuf, 0, buflen);

	attr = radius_get_attr_type(hdr->type);

	if (attr == NULL)
		attrtype = RADIUS_ATTR_HEXDUMP;
	else
		attrtype = attr->data_type;

	len = hdr->length - sizeof(struct radius_attr_hdr);
	pos = (unsigned char *) (hdr + 1);

	switch (attrtype) {
	case RADIUS_ATTR_TEXT:
		snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "      Value: '");
		for (i = 0; i < len; i++)
			snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "%s", print_char(pos[i]));
		snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "'");
		break;

	case RADIUS_ATTR_IP:
		if (len == 4) {
			struct in_addr addr;
			os_memcpy(&addr, pos, 4);
			snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "      Value: %s", inet_ntoa(addr));
		} else
			snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "      Invalid IP address length %d", len);
		break;

	case RADIUS_ATTR_IPV6:
		if (len == 16) {
			char buf[128];
			const char *atxt;
			struct in6_addr *addr = (struct in6_addr *) pos;
			atxt = inet_ntop(AF_INET6, addr, buf, sizeof(buf));
			snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "      Value: %s", atxt ? atxt : "?");
		} else
			snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "      Invalid IPv6 address length %d", len);
		break;

	case RADIUS_ATTR_INT32:
		if (len == 4)
			snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "      Value: %u", WPA_GET_BE32(pos));
		else
			snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "      Invalid INT32 length %d", len);
		break;

	case RADIUS_ATTR_HEXDUMP:
	case RADIUS_ATTR_UNDIST:
	default:
		snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), "      Value:");
		for (i = 0; i < len; i++)
			snprintf(outbuf + strlen(outbuf), buflen - strlen(outbuf), " %02x", pos[i]);
		break;
	}
	
	return outbuf;
}

/* Dump a message  */
void rgw_msg_dump(struct rgw_radius_msg_meta * msg, int has_meta)
{
	unsigned char *auth;
	char buf[256];
	size_t i;
	if (! TRACE_BOOL(FULL) )
		return;
	
	auth =  &(msg->radius.hdr->authenticator[0]);
	
	fd_log_debug("------ RADIUS msg dump -------");
	fd_log_debug(" id  : 0x%02hhx, code : %hhd (%s), length : %d", msg->radius.hdr->identifier, msg->radius.hdr->code, rgw_msg_code_str(msg->radius.hdr->code), ntohs(msg->radius.hdr->length));
	fd_log_debug(" auth: %02hhx %02hhx %02hhx %02hhx  %02hhx %02hhx %02hhx %02hhx",
			auth[0], auth[1], auth[2], auth[3], auth[4], auth[5], auth[6], auth[7]);
	fd_log_debug("       %02hhx %02hhx %02hhx %02hhx  %02hhx %02hhx %02hhx %02hhx",
			auth[8],  auth[9],  auth[10], auth[11], auth[12], auth[13], auth[14], auth[15]);
	for (i = 0; i < msg->radius.attr_used; i++) {
		struct radius_attr_hdr *attr = (struct radius_attr_hdr *)(msg->radius.buf + msg->radius.attr_pos[i]);
		fd_log_debug("    - Type: 0x%02hhx (%s)       Len: %-3hhu", attr->type, rgw_msg_attrtype_str(attr->type), attr->length);
		fd_log_debug("%s", radius_msg_dump_attr_val(attr, buf, sizeof(buf)));
	}
	if (has_meta && msg->ps_nb) {
		fd_log_debug("---- hidden attributes:");
		for (i = msg->ps_first; i < msg->ps_first + msg->ps_nb; i++) {
			struct radius_attr_hdr *attr = (struct radius_attr_hdr *)(msg->radius.buf + msg->radius.attr_pos[i]);
			fd_log_debug("    - Type: 0x%02hhx (%s)       Len: %-3hhu", attr->type, rgw_msg_attrtype_str(attr->type), attr->length);
			fd_log_debug("%s", radius_msg_dump_attr_val(attr, buf, sizeof(buf)));
		}
	}
	fd_log_debug("-----------------------------");
}


int radius_msg_finish(struct radius_msg *msg, const u8 *secret,
		      size_t secret_len)
{
	if (secret) {
		u8 auth[MD5_MAC_LEN];
		struct radius_attr_hdr *attr;

		os_memset(auth, 0, MD5_MAC_LEN);
		attr = radius_msg_add_attr(msg,
					   RADIUS_ATTR_MESSAGE_AUTHENTICATOR,
					   auth, MD5_MAC_LEN);
		if (attr == NULL) {
			fd_log_debug("WARNING: Could not add Message-Authenticator");
			return -1;
		}
		msg->hdr->length = htons(msg->buf_used);
		hmac_md5(secret, secret_len, msg->buf, msg->buf_used,
			 (u8 *) (attr + 1));
	} else
		msg->hdr->length = htons(msg->buf_used);

	if (msg->buf_used > 0xffff) {
		fd_log_debug("WARNING: too long RADIUS message (%lu)",
		       (unsigned long) msg->buf_used);
		return -1;
	}
	return 0;
}


int radius_msg_finish_srv(struct radius_msg *msg, const u8 *secret,
			  size_t secret_len, const u8 *req_authenticator)
{
	u8 auth[MD5_MAC_LEN];
	struct radius_attr_hdr *attr;
	const u8 *addr[4];
	size_t len[4];

	if (msg->hdr->code != RADIUS_CODE_ACCOUNTING_RESPONSE) {
	    os_memset(auth, 0, MD5_MAC_LEN);
	    attr = radius_msg_add_attr(msg, RADIUS_ATTR_MESSAGE_AUTHENTICATOR,
				       auth, MD5_MAC_LEN);
	    if (attr == NULL) {
		    fd_log_debug("WARNING: Could not add Message-Authenticator");
		    return -1;
	    }
	    msg->hdr->length = htons(msg->buf_used);
	    os_memcpy(msg->hdr->authenticator, req_authenticator,
		      sizeof(msg->hdr->authenticator));
	    hmac_md5(secret, secret_len, msg->buf, msg->buf_used,
		     (u8 *) (attr + 1));
	} else {
	    msg->hdr->length = htons(msg->buf_used);
	}

	/* ResponseAuth = MD5(Code+ID+Length+RequestAuth+Attributes+Secret) */
	addr[0] = (u8 *) msg->hdr;
	len[0] = 1 + 1 + 2;
	addr[1] = req_authenticator;
	len[1] = MD5_MAC_LEN;
	addr[2] = (u8 *) (msg->hdr + 1);
	len[2] = msg->buf_used - sizeof(*msg->hdr);
	addr[3] = secret;
	len[3] = secret_len;
	md5_vector(4, addr, len, msg->hdr->authenticator);

	if (msg->buf_used > 0xffff) {
		fd_log_debug("WARNING: too long RADIUS message (%lu)",
		       (unsigned long) msg->buf_used);
		return -1;
	}
	return 0;
}


void radius_msg_finish_acct(struct radius_msg *msg, const u8 *secret,
			    size_t secret_len)
{
	const u8 *addr[2];
	size_t len[2];

	msg->hdr->length = htons(msg->buf_used);
	os_memset(msg->hdr->authenticator, 0, MD5_MAC_LEN);
	addr[0] = msg->buf;
	len[0] = msg->buf_used;
	addr[1] = secret;
	len[1] = secret_len;
	md5_vector(2, addr, len, msg->hdr->authenticator);

	if (msg->buf_used > 0xffff) {
		fd_log_debug("WARNING: too long RADIUS messages (%lu)",
		       (unsigned long) msg->buf_used);
	}
}


int radius_msg_add_attr_to_array(struct radius_msg *msg,
					struct radius_attr_hdr *attr)
{
	if (msg->attr_used >= msg->attr_size) {
		size_t *nattr_pos;
		int nlen = msg->attr_size * 2;

		nattr_pos = os_realloc(msg->attr_pos,
				       nlen * sizeof(*msg->attr_pos));
		if (nattr_pos == NULL)
			return -1;

		msg->attr_pos = nattr_pos;
		msg->attr_size = nlen;
	}

	msg->attr_pos[msg->attr_used++] = (unsigned char *) attr - msg->buf;

	return 0;
}


struct radius_attr_hdr *radius_msg_add_attr(struct radius_msg *msg, u8 type,
					    const u8 *data, size_t data_len)
{
	size_t buf_needed;
	struct radius_attr_hdr *attr;

	if (data_len > RADIUS_MAX_ATTR_LEN) {
		fd_log_debug("radius_msg_add_attr: too long attribute (%lu bytes)",
		       (unsigned long) data_len);
		return NULL;
	}

	buf_needed = msg->buf_used + sizeof(*attr) + data_len;

	if (msg->buf_size < buf_needed) {
		/* allocate more space for message buffer */
		unsigned char *nbuf;
		size_t nlen = msg->buf_size;

		while (nlen < buf_needed)
			nlen *= 2;
		nbuf = os_realloc(msg->buf, nlen);
		if (nbuf == NULL)
			return NULL;
		msg->buf = nbuf;
		msg->hdr = (struct radius_hdr *) msg->buf;
		os_memset(msg->buf + msg->buf_size, 0, nlen - msg->buf_size);
		msg->buf_size = nlen;
	}

	attr = (struct radius_attr_hdr *) (msg->buf + msg->buf_used);
	attr->type = type;
	attr->length = sizeof(*attr) + data_len;
	if (data_len > 0)
		os_memcpy(attr + 1, data, data_len);

	msg->buf_used += sizeof(*attr) + data_len;

	if (radius_msg_add_attr_to_array(msg, attr))
		return NULL;

	return attr;
}


/* Modified version of radius_msg_parse */
int rgw_msg_parse(unsigned char * buf, size_t len, struct rgw_radius_msg_meta ** msg)
{
	struct rgw_radius_msg_meta * temp_msg = NULL;
	struct radius_hdr *hdr;
	struct radius_attr_hdr *attr;
	size_t msg_len;
	unsigned char *pos, *end;
	int ret = 0;
	
	TRACE_ENTRY("%p %zd %p", buf, len, msg);
	
	CHECK_PARAMS( buf && len >= sizeof(*hdr) && msg );
	
	*msg = NULL;
	
	/* Parse the RADIUS message */
	hdr = (struct radius_hdr *) buf;
	msg_len = ntohs(hdr->length);
	if (msg_len < sizeof(*hdr) || msg_len > len) {
		TRACE_DEBUG(INFO, "Invalid RADIUS message length");
		return EINVAL;
	}

	if (msg_len < len) {
		TRACE_DEBUG(INFO, "Ignored %lu extra bytes after RADIUS message",
		       (unsigned long) len - msg_len);
	}

	CHECK_MALLOC( temp_msg = malloc(sizeof(struct rgw_radius_msg_meta)) );
	memset(temp_msg, 0, sizeof(struct rgw_radius_msg_meta));
	
	if (radius_msg_initialize(&temp_msg->radius, msg_len)) {
		TRACE_DEBUG(INFO, "Error in radius_msg_initialize, returning ENOMEM.");
		free(temp_msg);
		return ENOMEM;
	}
	
	/* Store the received data in the alloc'd buffer */
	memcpy(temp_msg->radius.buf, buf, msg_len);
	temp_msg->radius.buf_size = temp_msg->radius.buf_used = msg_len;
	
	/* parse attributes */
	pos = (unsigned char *) (temp_msg->radius.hdr + 1);
	end = temp_msg->radius.buf + temp_msg->radius.buf_used;
	
	while (pos < end) {
		if ((size_t) (end - pos) < sizeof(*attr)) {
			TRACE_DEBUG(INFO, "Trucated attribute found in RADIUS buffer, EINVAL.");
			ret = EINVAL;
			break;
		}
			
		attr = (struct radius_attr_hdr *) pos;
	
		if (pos + attr->length > end || attr->length < sizeof(*attr)) {
			TRACE_DEBUG(INFO, "Trucated attribute found in RADIUS buffer, EINVAL.");
			ret = EINVAL;
			break;
		}

		if (radius_msg_add_attr_to_array(&temp_msg->radius, attr)) {
			TRACE_DEBUG(INFO, "Error in radius_msg_add_attr_to_array, ENOMEM");
			ret = ENOMEM;
			break;
		}
		
		if (attr->type == RADIUS_ATTR_PROXY_STATE)
			temp_msg->ps_nb += 1;

		pos += attr->length;
	}
	
	if (ret != 0) {
		radius_msg_free(&temp_msg->radius);
		free(temp_msg);
		return ret;
	}
	
	/* Now move all the proxy-state attributes at the end of the attr_pos array */
	if (temp_msg->ps_nb) {
		size_t *temp_ps = NULL;
		int n, new_n = 0, p = 0;
		
		CHECK_MALLOC( temp_ps = calloc(temp_msg->ps_nb, sizeof(size_t)) );
		
		/* Move all the Proxy-State attributes into the temp_ps array */
		for (n=0; n < temp_msg->radius.attr_used; n++) {
			struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(temp_msg->radius.buf + temp_msg->radius.attr_pos[n]);
			
			if (attr->type == RADIUS_ATTR_PROXY_STATE) {
				temp_ps[p++] = temp_msg->radius.attr_pos[n];
			} else {
				temp_msg->radius.attr_pos[new_n++] = temp_msg->radius.attr_pos[n];
			}
		}
		temp_msg->radius.attr_used = new_n; /* hide the proxy-state to other modules */
		temp_msg->ps_first = new_n;
		
		/* And back into the array */
		memcpy(temp_msg->radius.attr_pos + new_n, temp_ps, p * sizeof(size_t));
		free(temp_ps);
	}
	
	*msg = temp_msg;
	return 0;
}



int radius_msg_add_eap(struct radius_msg *msg, const u8 *data, size_t data_len)
{
	const u8 *pos = data;
	size_t left = data_len;

	while (left > 0) {
		int len;
		if (left > RADIUS_MAX_ATTR_LEN)
			len = RADIUS_MAX_ATTR_LEN;
		else
			len = left;

		if (!radius_msg_add_attr(msg, RADIUS_ATTR_EAP_MESSAGE,
					 pos, len))
			return 0;

		pos += len;
		left -= len;
	}

	return 1;
}


u8 *radius_msg_get_eap(struct radius_msg *msg, size_t *eap_len)
{
	u8 *eap, *pos;
	size_t len, i;
	struct radius_attr_hdr *attr;

	if (msg == NULL)
		return NULL;

	len = 0;
	for (i = 0; i < msg->attr_used; i++) {
		attr = radius_get_attr_hdr(msg, i);
		if (attr->type == RADIUS_ATTR_EAP_MESSAGE)
			len += attr->length - sizeof(struct radius_attr_hdr);
	}

	if (len == 0)
		return NULL;

	eap = os_malloc(len);
	if (eap == NULL)
		return NULL;

	pos = eap;
	for (i = 0; i < msg->attr_used; i++) {
		attr = radius_get_attr_hdr(msg, i);
		if (attr->type == RADIUS_ATTR_EAP_MESSAGE) {
			int flen = attr->length - sizeof(*attr);
			os_memcpy(pos, attr + 1, flen);
			pos += flen;
		}
	}

	if (eap_len)
		*eap_len = len;

	return eap;
}


int radius_msg_verify_msg_auth(struct radius_msg *msg, const u8 *secret,
			       size_t secret_len, const u8 *req_auth)
{
	u8 auth[MD5_MAC_LEN], orig[MD5_MAC_LEN];
	u8 orig_authenticator[16];
	struct radius_attr_hdr *attr = NULL, *tmp;
	size_t i;

	for (i = 0; i < msg->attr_used; i++) {
		tmp = radius_get_attr_hdr(msg, i);
		if (tmp->type == RADIUS_ATTR_MESSAGE_AUTHENTICATOR) {
			if (attr != NULL) {
				fd_log_debug("Multiple Message-Authenticator attributes in RADIUS message");
				return 1;
			}
			attr = tmp;
		}
	}

	if (attr == NULL) {
		fd_log_debug("No Message-Authenticator attribute found");
		return 1;
	}

	os_memcpy(orig, attr + 1, MD5_MAC_LEN);
	os_memset(attr + 1, 0, MD5_MAC_LEN);
	if (req_auth) {
		os_memcpy(orig_authenticator, msg->hdr->authenticator,
			  sizeof(orig_authenticator));
		os_memcpy(msg->hdr->authenticator, req_auth,
			  sizeof(msg->hdr->authenticator));
	}
	hmac_md5(secret, secret_len, msg->buf, msg->buf_used, auth);
	os_memcpy(attr + 1, orig, MD5_MAC_LEN);
	if (req_auth) {
		os_memcpy(msg->hdr->authenticator, orig_authenticator,
			  sizeof(orig_authenticator));
	}

	if (os_memcmp(orig, auth, MD5_MAC_LEN) != 0) {
		fd_log_debug("Invalid Message-Authenticator!");
		return 1;
	}

	return 0;
}


int radius_msg_verify(struct radius_msg *msg, const u8 *secret,
		      size_t secret_len, struct radius_msg *sent_msg, int auth)
{
	const u8 *addr[4];
	size_t len[4];
	u8 hash[MD5_MAC_LEN];

	if (sent_msg == NULL) {
		fd_log_debug("No matching Access-Request message found");
		return 1;
	}

	if (auth &&
	    radius_msg_verify_msg_auth(msg, secret, secret_len,
				       sent_msg->hdr->authenticator)) {
		return 1;
	}

	/* ResponseAuth = MD5(Code+ID+Length+RequestAuth+Attributes+Secret) */
	addr[0] = (u8 *) msg->hdr;
	len[0] = 1 + 1 + 2;
	addr[1] = sent_msg->hdr->authenticator;
	len[1] = MD5_MAC_LEN;
	addr[2] = (u8 *) (msg->hdr + 1);
	len[2] = msg->buf_used - sizeof(*msg->hdr);
	addr[3] = secret;
	len[3] = secret_len;
	md5_vector(4, addr, len, hash);
	if (os_memcmp(hash, msg->hdr->authenticator, MD5_MAC_LEN) != 0) {
		fd_log_debug("Response Authenticator invalid!");
		return 1;
	}

	return 0;
}


int radius_msg_copy_attr(struct radius_msg *dst, struct radius_msg *src,
			 u8 type)
{
	struct radius_attr_hdr *attr;
	size_t i;
	int count = 0;

	for (i = 0; i < src->attr_used; i++) {
		attr = radius_get_attr_hdr(src, i);
		if (attr->type == type) {
			if (!radius_msg_add_attr(dst, type, (u8 *) (attr + 1),
						 attr->length - sizeof(*attr)))
				return -1;
			count++;
		}
	}

	return count;
}


/* Create Request Authenticator. The value should be unique over the lifetime
 * of the shared secret between authenticator and authentication server.
 * Use one-way MD5 hash calculated from current timestamp and some data given
 * by the caller. */
void radius_msg_make_authenticator(struct radius_msg *msg,
				   const u8 *data, size_t len)
{
	struct os_time tv;
	long int l;
	const u8 *addr[3];
	size_t elen[3];

	os_get_time(&tv);
	l = os_random();
	addr[0] = (u8 *) &tv;
	elen[0] = sizeof(tv);
	addr[1] = data;
	elen[1] = len;
	addr[2] = (u8 *) &l;
	elen[2] = sizeof(l);
	md5_vector(3, addr, elen, msg->hdr->authenticator);
}


/* Get Vendor-specific RADIUS Attribute from a parsed RADIUS message.
 * Returns the Attribute payload and sets alen to indicate the length of the
 * payload if a vendor attribute with subtype is found, otherwise returns NULL.
 * The returned payload is allocated with os_malloc() and caller must free it
 * by calling os_free().
 */
static u8 *radius_msg_get_vendor_attr(struct radius_msg *msg, u32 vendor,
				      u8 subtype, size_t *alen)
{
	u8 *data, *pos;
	size_t i, len;

	if (msg == NULL)
		return NULL;

	for (i = 0; i < msg->attr_used; i++) {
		struct radius_attr_hdr *attr = radius_get_attr_hdr(msg, i);
		size_t left;
		u32 vendor_id;
		struct radius_attr_vendor *vhdr;

		if (attr->type != RADIUS_ATTR_VENDOR_SPECIFIC)
			continue;

		left = attr->length - sizeof(*attr);
		if (left < 4)
			continue;

		pos = (u8 *) (attr + 1);

		os_memcpy(&vendor_id, pos, 4);
		pos += 4;
		left -= 4;

		if (ntohl(vendor_id) != vendor)
			continue;

		while (left >= sizeof(*vhdr)) {
			vhdr = (struct radius_attr_vendor *) pos;
			if (vhdr->vendor_length > left ||
			    vhdr->vendor_length < sizeof(*vhdr)) {
				left = 0;
				break;
			}
			if (vhdr->vendor_type != subtype) {
				pos += vhdr->vendor_length;
				left -= vhdr->vendor_length;
				continue;
			}

			len = vhdr->vendor_length - sizeof(*vhdr);
			data = os_malloc(len);
			if (data == NULL)
				return NULL;
			os_memcpy(data, pos + sizeof(*vhdr), len);
			if (alen)
				*alen = len;
			return data;
		}
	}

	return NULL;
}


static u8 * decrypt_ms_key(const u8 *key, size_t len,
			   const u8 *req_authenticator,
			   const u8 *secret, size_t secret_len, size_t *reslen)
{
	u8 *plain, *ppos, *res;
	const u8 *pos;
	size_t left, plen;
	u8 hash[MD5_MAC_LEN];
	int i, first = 1;
	const u8 *addr[3];
	size_t elen[3];

	/* key: 16-bit salt followed by encrypted key info */

	if (len < 2 + 16)
		return NULL;

	pos = key + 2;
	left = len - 2;
	if (left % 16) {
		fd_log_debug("Invalid ms key len %lu", (unsigned long) left);
		return NULL;
	}

	plen = left;
	ppos = plain = os_malloc(plen);
	if (plain == NULL)
		return NULL;
	plain[0] = 0;

	while (left > 0) {
		/* b(1) = MD5(Secret + Request-Authenticator + Salt)
		 * b(i) = MD5(Secret + c(i - 1)) for i > 1 */

		addr[0] = secret;
		elen[0] = secret_len;
		if (first) {
			addr[1] = req_authenticator;
			elen[1] = MD5_MAC_LEN;
			addr[2] = key;
			elen[2] = 2; /* Salt */
		} else {
			addr[1] = pos - MD5_MAC_LEN;
			elen[1] = MD5_MAC_LEN;
		}
		md5_vector(first ? 3 : 2, addr, elen, hash);
		first = 0;

		for (i = 0; i < MD5_MAC_LEN; i++)
			*ppos++ = *pos++ ^ hash[i];
		left -= MD5_MAC_LEN;
	}

	if (plain[0] == 0 || plain[0] > plen - 1) {
		fd_log_debug("Failed to decrypt MPPE key");
		os_free(plain);
		return NULL;
	}

	res = os_malloc(plain[0]);
	if (res == NULL) {
		os_free(plain);
		return NULL;
	}
	os_memcpy(res, plain + 1, plain[0]);
	if (reslen)
		*reslen = plain[0];
	os_free(plain);
	return res;
}


static void encrypt_ms_key(const u8 *key, size_t key_len, u16 salt,
			   const u8 *req_authenticator,
			   const u8 *secret, size_t secret_len,
			   u8 *ebuf, size_t *elen)
{
	int i, len, first = 1;
	u8 hash[MD5_MAC_LEN], saltbuf[2], *pos;
	const u8 *addr[3];
	size_t _len[3];

	WPA_PUT_BE16(saltbuf, salt);

	len = 1 + key_len;
	if (len & 0x0f) {
		len = (len & 0xf0) + 16;
	}
	os_memset(ebuf, 0, len);
	ebuf[0] = key_len;
	os_memcpy(ebuf + 1, key, key_len);

	*elen = len;

	pos = ebuf;
	while (len > 0) {
		/* b(1) = MD5(Secret + Request-Authenticator + Salt)
		 * b(i) = MD5(Secret + c(i - 1)) for i > 1 */
		addr[0] = secret;
		_len[0] = secret_len;
		if (first) {
			addr[1] = req_authenticator;
			_len[1] = MD5_MAC_LEN;
			addr[2] = saltbuf;
			_len[2] = sizeof(saltbuf);
		} else {
			addr[1] = pos - MD5_MAC_LEN;
			_len[1] = MD5_MAC_LEN;
		}
		md5_vector(first ? 3 : 2, addr, _len, hash);
		first = 0;

		for (i = 0; i < MD5_MAC_LEN; i++)
			*pos++ ^= hash[i];

		len -= MD5_MAC_LEN;
	}
}


struct radius_ms_mppe_keys *
radius_msg_get_ms_keys(struct radius_msg *msg, struct radius_msg *sent_msg,
		       const u8 *secret, size_t secret_len)
{
	u8 *key;
	size_t keylen;
	struct radius_ms_mppe_keys *keys;

	if (msg == NULL || sent_msg == NULL)
		return NULL;

	keys = os_zalloc(sizeof(*keys));
	if (keys == NULL)
		return NULL;

	key = radius_msg_get_vendor_attr(msg, RADIUS_VENDOR_ID_MICROSOFT,
					 RADIUS_VENDOR_ATTR_MS_MPPE_SEND_KEY,
					 &keylen);
	if (key) {
		keys->send = decrypt_ms_key(key, keylen,
					    sent_msg->hdr->authenticator,
					    secret, secret_len,
					    &keys->send_len);
		os_free(key);
	}

	key = radius_msg_get_vendor_attr(msg, RADIUS_VENDOR_ID_MICROSOFT,
					 RADIUS_VENDOR_ATTR_MS_MPPE_RECV_KEY,
					 &keylen);
	if (key) {
		keys->recv = decrypt_ms_key(key, keylen,
					    sent_msg->hdr->authenticator,
					    secret, secret_len,
					    &keys->recv_len);
		os_free(key);
	}

	return keys;
}


struct radius_ms_mppe_keys *
radius_msg_get_cisco_keys(struct radius_msg *msg, struct radius_msg *sent_msg,
			  const u8 *secret, size_t secret_len)
{
	u8 *key;
	size_t keylen;
	struct radius_ms_mppe_keys *keys;

	if (msg == NULL || sent_msg == NULL)
		return NULL;

	keys = os_zalloc(sizeof(*keys));
	if (keys == NULL)
		return NULL;

	key = radius_msg_get_vendor_attr(msg, RADIUS_VENDOR_ID_CISCO,
					 RADIUS_CISCO_AV_PAIR, &keylen);
	if (key && keylen == 51 &&
	    os_memcmp(key, "leap:session-key=", 17) == 0) {
		keys->recv = decrypt_ms_key(key + 17, keylen - 17,
					    sent_msg->hdr->authenticator,
					    secret, secret_len,
					    &keys->recv_len);
	}
	os_free(key);

	return keys;
}


int radius_msg_add_mppe_keys(struct radius_msg *msg,
			     const u8 *req_authenticator,
			     const u8 *secret, size_t secret_len,
			     const u8 *send_key, size_t send_key_len,
			     const u8 *recv_key, size_t recv_key_len)
{
	struct radius_attr_hdr *attr;
	u32 vendor_id = htonl(RADIUS_VENDOR_ID_MICROSOFT);
	u8 *buf;
	struct radius_attr_vendor *vhdr;
	u8 *pos;
	size_t elen;
	int hlen;
	u16 salt;

	hlen = sizeof(vendor_id) + sizeof(*vhdr) + 2;

	/* MS-MPPE-Send-Key */
	buf = os_malloc(hlen + send_key_len + 16);
	if (buf == NULL) {
		return 0;
	}
	pos = buf;
	os_memcpy(pos, &vendor_id, sizeof(vendor_id));
	pos += sizeof(vendor_id);
	vhdr = (struct radius_attr_vendor *) pos;
	vhdr->vendor_type = RADIUS_VENDOR_ATTR_MS_MPPE_SEND_KEY;
	pos = (u8 *) (vhdr + 1);
	salt = os_random() | 0x8000;
	WPA_PUT_BE16(pos, salt);
	pos += 2;
	encrypt_ms_key(send_key, send_key_len, salt, req_authenticator, secret,
		       secret_len, pos, &elen);
	vhdr->vendor_length = hlen + elen - sizeof(vendor_id);

	attr = radius_msg_add_attr(msg, RADIUS_ATTR_VENDOR_SPECIFIC,
				   buf, hlen + elen);
	os_free(buf);
	if (attr == NULL) {
		return 0;
	}

	/* MS-MPPE-Recv-Key */
	buf = os_malloc(hlen + send_key_len + 16);
	if (buf == NULL) {
		return 0;
	}
	pos = buf;
	os_memcpy(pos, &vendor_id, sizeof(vendor_id));
	pos += sizeof(vendor_id);
	vhdr = (struct radius_attr_vendor *) pos;
	vhdr->vendor_type = RADIUS_VENDOR_ATTR_MS_MPPE_RECV_KEY;
	pos = (u8 *) (vhdr + 1);
	salt ^= 1;
	WPA_PUT_BE16(pos, salt);
	pos += 2;
	encrypt_ms_key(recv_key, recv_key_len, salt, req_authenticator, secret,
		       secret_len, pos, &elen);
	vhdr->vendor_length = hlen + elen - sizeof(vendor_id);

	attr = radius_msg_add_attr(msg, RADIUS_ATTR_VENDOR_SPECIFIC,
				   buf, hlen + elen);
	os_free(buf);
	if (attr == NULL) {
		return 0;
	}

	return 1;
}


/* Add User-Password attribute to a RADIUS message and encrypt it as specified
 * in RFC 2865, Chap. 5.2 */
struct radius_attr_hdr *
radius_msg_add_attr_user_password(struct radius_msg *msg,
				  const u8 *data, size_t data_len,
				  const u8 *secret, size_t secret_len)
{
	u8 buf[128];
	int padlen, i;
	size_t buf_len, pos;
	const u8 *addr[2];
	size_t len[2];
	u8 hash[16];

	if (data_len > 128)
		return NULL;

	os_memcpy(buf, data, data_len);
	buf_len = data_len;

	padlen = data_len % 16;
	if (padlen) {
		padlen = 16 - padlen;
		os_memset(buf + data_len, 0, padlen);
		buf_len += padlen;
	}

	addr[0] = secret;
	len[0] = secret_len;
	addr[1] = msg->hdr->authenticator;
	len[1] = 16;
	md5_vector(2, addr, len, hash);

	for (i = 0; i < 16; i++)
		buf[i] ^= hash[i];
	pos = 16;

	while (pos < buf_len) {
		addr[0] = secret;
		len[0] = secret_len;
		addr[1] = &buf[pos - 16];
		len[1] = 16;
		md5_vector(2, addr, len, hash);

		for (i = 0; i < 16; i++)
			buf[pos + i] ^= hash[i];

		pos += 16;
	}

	return radius_msg_add_attr(msg, RADIUS_ATTR_USER_PASSWORD,
				   buf, buf_len);
}


int radius_msg_get_attr(struct radius_msg *msg, u8 type, u8 *buf, size_t len)
{
	struct radius_attr_hdr *attr = NULL, *tmp;
	size_t i, dlen;

	for (i = 0; i < msg->attr_used; i++) {
		tmp = radius_get_attr_hdr(msg, i);
		if (tmp->type == type) {
			attr = tmp;
			break;
		}
	}

	if (!attr)
		return -1;

	dlen = attr->length - sizeof(*attr);
	if (buf)
		os_memcpy(buf, (attr + 1), dlen > len ? len : dlen);
	return dlen;
}


int radius_msg_get_attr_ptr(struct radius_msg *msg, u8 type, u8 **buf,
			    size_t *len, const u8 *start)
{
	size_t i;
	struct radius_attr_hdr *attr = NULL, *tmp;

	for (i = 0; i < msg->attr_used; i++) {
		tmp = radius_get_attr_hdr(msg, i);
		if (tmp->type == type &&
		    (start == NULL || (u8 *) tmp > start)) {
			attr = tmp;
			break;
		}
	}

	if (!attr)
		return -1;

	*buf = (u8 *) (attr + 1);
	*len = attr->length - sizeof(*attr);
	return 0;
}


int radius_msg_count_attr(struct radius_msg *msg, u8 type, int min_len)
{
	size_t i;
	int count;

	for (count = 0, i = 0; i < msg->attr_used; i++) {
		struct radius_attr_hdr *attr = radius_get_attr_hdr(msg, i);
		if (attr->type == type &&
		    attr->length >= sizeof(struct radius_attr_hdr) + min_len)
			count++;
	}

	return count;
}


struct radius_tunnel_attrs {
	int tag_used;
	int type; /* Tunnel-Type */
	int medium_type; /* Tunnel-Medium-Type */
	int vlanid;
};


/**
 * radius_msg_get_vlanid - Parse RADIUS attributes for VLAN tunnel information
 * @msg: RADIUS message
 * Returns: VLAN ID for the first tunnel configuration of -1 if none is found
 */
int radius_msg_get_vlanid(struct radius_msg *msg)
{
	struct radius_tunnel_attrs tunnel[RADIUS_TUNNEL_TAGS], *tun;
	size_t i;
	struct radius_attr_hdr *attr = NULL;
	const u8 *data;
	char buf[10];
	size_t dlen;

	os_memset(&tunnel, 0, sizeof(tunnel));

	for (i = 0; i < msg->attr_used; i++) {
		attr = radius_get_attr_hdr(msg, i);
		data = (const u8 *) (attr + 1);
		dlen = attr->length - sizeof(*attr);
		if (attr->length < 3)
			continue;
		if (data[0] >= RADIUS_TUNNEL_TAGS)
			tun = &tunnel[0];
		else
			tun = &tunnel[data[0]];

		switch (attr->type) {
		case RADIUS_ATTR_TUNNEL_TYPE:
			if (attr->length != 6)
				break;
			tun->tag_used++;
			tun->type = WPA_GET_BE24(data + 1);
			break;
		case RADIUS_ATTR_TUNNEL_MEDIUM_TYPE:
			if (attr->length != 6)
				break;
			tun->tag_used++;
			tun->medium_type = WPA_GET_BE24(data + 1);
			break;
		case RADIUS_ATTR_TUNNEL_PRIVATE_GROUP_ID:
			if (data[0] < RADIUS_TUNNEL_TAGS) {
				data++;
				dlen--;
			}
			if (dlen >= sizeof(buf))
				break;
			os_memcpy(buf, data, dlen);
			buf[dlen] = '\0';
			tun->tag_used++;
			tun->vlanid = atoi(buf);
			break;
		}
	}

	for (i = 0; i < RADIUS_TUNNEL_TAGS; i++) {
		tun = &tunnel[i];
		if (tun->tag_used &&
		    tun->type == RADIUS_TUNNEL_TYPE_VLAN &&
		    tun->medium_type == RADIUS_TUNNEL_MEDIUM_TYPE_802 &&
		    tun->vlanid > 0)
			return tun->vlanid;
	}

	return -1;
}


void radius_free_class(struct radius_class_data *c)
{
	size_t i;
	if (c == NULL)
		return;
	for (i = 0; i < c->count; i++)
		os_free(c->attr[i].data);
	os_free(c->attr);
	c->attr = NULL;
	c->count = 0;
}


int radius_copy_class(struct radius_class_data *dst,
		      const struct radius_class_data *src)
{
	size_t i;

	if (src->attr == NULL)
		return 0;

	dst->attr = os_zalloc(src->count * sizeof(struct radius_attr_data));
	if (dst->attr == NULL)
		return -1;

	dst->count = 0;

	for (i = 0; i < src->count; i++) {
		dst->attr[i].data = os_malloc(src->attr[i].len);
		if (dst->attr[i].data == NULL)
			break;
		dst->count++;
		os_memcpy(dst->attr[i].data, src->attr[i].data,
			  src->attr[i].len);
		dst->attr[i].len = src->attr[i].len;
	}

	return 0;
}
