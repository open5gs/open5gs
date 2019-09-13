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

/* This file contains the definitions for the app_radgw internal use. */
  
#ifndef _RGW_H
#define _RGW_H

/* include the common definitions */
#include "rgw_common.h"

/* RADIUS messages + metadata */
struct rgw_radius_msg_meta {

	/* The RADIUS message */
	struct radius_msg 	radius;
	
	/* Metadata */
	struct {
		/* The port it was sent from, in network byte order */
		unsigned	port :16;
		
		/* received on ACCT or AUTH port? */
		unsigned	serv_type :2;
		
		/* The message has a valid Message-Authenticator attribute */
		unsigned	valid_mac :1;
	};
	
	/* For Proxy-State attributes: */
	int	ps_first;	/* The index of the first Proxy-State attribute in radius.attr_pos. It is always >= radius.attr_used */
	int	ps_nb;		/* The number of Proxy-State attributes. The real radius.attr_pos size is attr_used + ps_nb */
};
void rgw_msg_free(struct rgw_radius_msg_meta ** msg);
int rgw_msg_parse(unsigned char * buf, size_t len, struct rgw_radius_msg_meta ** msg);
void rgw_msg_dump(struct rgw_radius_msg_meta * msg, int has_meta);

/* Local RADIUS server(s) configuration */
struct rgw_serv {
	unsigned	disabled	:1;
	unsigned	ip_disabled	:1;
	unsigned	ip6_disabled	:1;
	unsigned	:13; /* padding */
	
	uint16_t	port;	/* stored in network byte order */
	
	struct in_addr	ip_endpoint;
	struct in6_addr	ip6_endpoint;
};

extern struct rgw_servs {
	struct rgw_serv	auth_serv;
	struct rgw_serv	acct_serv;
} rgw_servers;

int rgw_servers_init(void);
int rgw_servers_start(void);
void rgw_servers_dump(void);
int rgw_servers_send(int type, unsigned char *buf, size_t buflen, struct sockaddr *to, uint16_t to_port);
void rgw_servers_fini(void);


/* Clients management */
enum rgw_cli_type { RGW_CLI_NAS, RGW_CLI_PXY };
int rgw_clients_auth_check(struct rgw_radius_msg_meta * msg, struct rgw_client * cli, uint8_t * req_auth);
int rgw_clients_add( struct sockaddr * ip_port, unsigned char ** key, size_t keylen, enum rgw_cli_type type );
int rgw_clients_getkey(struct rgw_client * cli, unsigned char **key, size_t *key_len);
int rgw_clients_gettype(struct rgw_client * cli, enum rgw_cli_type *type);
int rgw_clients_search(struct sockaddr * ip_port, struct rgw_client ** ref);
int rgw_clients_check_dup(struct rgw_radius_msg_meta **msg, struct rgw_client *cli);
int rgw_clients_create_origin(struct rgw_radius_msg_meta *msg, struct rgw_client * cli, struct msg ** diam);
int rgw_client_finish_send(struct radius_msg ** msg, struct rgw_radius_msg_meta * req, struct rgw_client * cli);
int rgw_client_finish_nosend(struct rgw_radius_msg_meta * req, struct rgw_client * cli);
void rgw_clients_dispose(struct rgw_client ** ref);
void rgw_clients_dump(void);
int rgw_clients_init(void);
void rgw_clients_fini(void);
int rgw_client_session_add(struct rgw_client * cli, struct session *sess, char * dest_realm, char * dest_host, application_id_t appid);
int rgw_client_session_stop(struct rgw_client * cli, struct session * sess, int32_t reason);


/* Management of plugins */
int rgw_plg_add( char * plgfile, char * conffile, int port, unsigned char ** codes_array, size_t codes_sz );
void rgw_plg_dump(void);
void rgw_plg_start_cache(void);
int rgw_plg_loop_req(struct rgw_radius_msg_meta **rad, struct msg **diam_msg, struct rgw_client * cli);
int rgw_plg_loop_ans(struct rgw_radius_msg_meta *req, struct msg **diam_ans, struct radius_msg ** rad_ans, struct rgw_client * cli);
void rgw_plg_fini(void);


/* Parse configuration file */
int rgw_conf_handle(char * conffile);


/* Worker module, process incoming RADIUS messages (in separated threads) */
int rgw_work_start(void);
int rgw_work_add(struct rgw_radius_msg_meta * msg, struct rgw_client * client);
void rgw_work_fini(void);


#endif /* _RGW_H */
  
