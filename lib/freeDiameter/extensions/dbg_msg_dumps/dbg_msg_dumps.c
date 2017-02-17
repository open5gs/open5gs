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

/* This extension uses the hooks mechanism to display the full content of received and sent messages, for
   learning & debugging purpose.
   Do NOT use this extension in production environment because it will slow down all operation. */
   
/* You can add a configuration parameter on the LoadExtension line, e.g. 
LoadExtension="dbg_msg_dump.fdx":"0x149";
The value is an hexadecimal value with the following bits meaning: */
#define HK_ERRORS_QUIET    0x0001	 /* errors are not dumped -- removes the default handling as well */
#define HK_ERRORS_COMPACT  0x0002	 /* errors in compact mode */
#define HK_ERRORS_FULL     0x0004  	 /* errors in full mode (1 line with all the data) */
#define HK_ERRORS_TREE     0x0008  	 /* errors in treeview mode (message split over multiple lines) */

#define HK_SNDRCV_QUIET    0x0010	 /* send+rcv are not dumped -- removes the default handling as well */
#define HK_SNDRCV_COMPACT  0x0020	 /* send+rcv in compact mode */
#define HK_SNDRCV_FULL     0x0040  	 /* send+rcv in full mode */
#define HK_SNDRCV_TREE     0x0080  	 /* send+rcv in tree mode */

#define HK_ROUTING_QUIET   0x0100  	 /* routing decisions are not dumped -- removes the default handling as well */
#define HK_ROUTING_COMPACT 0x0200  	 /* routing decisions in compact mode */
#define HK_ROUTING_FULL    0x0400  	 /* routing decisions in full mode */
#define HK_ROUTING_TREE    0x0800  	 /* routing decisions in tree mode */

#define HK_PEERS_QUIET     0x1000  	 /* peers connections events are not dumped -- removes the default handling as well */
#define HK_PEERS_COMPACT   0x2000  	 /* peers connections events in compact mode */
#define HK_PEERS_FULL      0x4000  	 /* peers connections events in full mode */
#define HK_PEERS_TREE      0x8000  	 /* peers connections events in tree mode */
/*
Default value is HK_ERRORS_TREE + HK_SNDRCV_TREE + HK_PEERS_TREE
*/

#include <freeDiameter/extension.h>

static struct fd_hook_hdl *md_hdl[4] = {NULL,NULL,NULL,NULL};
static uint32_t dump_level = HK_ERRORS_TREE | HK_SNDRCV_TREE | HK_PEERS_TREE; /* default */
static char * buf = NULL;
static size_t len;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/* The callback called when messages are received and sent */
static void md_hook_cb_tree(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata)
{
	char * peer_name = peer ? peer->info.pi_diamid : "<unknown peer>";
	
	CHECK_POSIX_DO( pthread_mutex_lock(&mtx), );
	
	if (msg) {
		CHECK_MALLOC_DO( fd_msg_dump_treeview(&buf, &len, NULL, msg, fd_g_config->cnf_dict, (type == HOOK_MESSAGE_PARSING_ERROR) ? 0 : 1, 1), 
			{ LOG_E("Error while dumping a message"); pthread_mutex_unlock(&mtx); return; } );
	}
	
	switch (type) {
/* errors */
	case HOOK_MESSAGE_FAILOVER:
		LOG_E("FAILOVER from '%s':", peer_name);
		LOG_SPLIT(FD_LOG_ERROR, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_PARSING_ERROR:
		if (msg) {
			DiamId_t id = NULL;
			if (!fd_msg_source_get( msg, &id, NULL ))
				id = (DiamId_t)"<error getting source>";
			if (!id)
				id = (DiamId_t)"<local>";
			LOG_E("PARSING ERROR: '%s' from '%s': ", (char *)other, (char *)id);
			LOG_SPLIT(FD_LOG_ERROR, "     ", buf, NULL);
		} else {
			struct fd_cnx_rcvdata *rcv_data = other;
			CHECK_MALLOC_DO(fd_dump_extend_hexdump(&buf, &len, NULL, rcv_data->buffer, rcv_data->length, 0, 0), break);
			LOG_E("PARSING ERROR: %zdB msg from '%s': %s", rcv_data->length, peer_name, buf);
		}
		break;
	case HOOK_MESSAGE_ROUTING_ERROR:
		LOG_E("ROUTING ERROR '%s' for: ", (char *)other);
		LOG_SPLIT(FD_LOG_ERROR, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_DROPPED:
		LOG_E("DROPPED '%s'", (char *)other);
		LOG_SPLIT(FD_LOG_ERROR, "     ", buf, NULL);
		break;
	
/* send receive */
	case HOOK_MESSAGE_RECEIVED:
		LOG_N("RCV from '%s':", peer_name);
		LOG_SPLIT(FD_LOG_NOTICE, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_SENT:
		LOG_N("SND to '%s':", peer_name);
		LOG_SPLIT(FD_LOG_NOTICE, "     ", buf, NULL);
		break;
	
/* routing */
	case HOOK_MESSAGE_LOCAL:
		LOG_N("ISSUED:");
		LOG_SPLIT(FD_LOG_NOTICE, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_ROUTING_FORWARD:
		LOG_N("FORWARDING: %s", buf);
		LOG_SPLIT(FD_LOG_NOTICE, "     ", buf, NULL);
		break;
	case HOOK_MESSAGE_ROUTING_LOCAL:
		LOG_N("DISPATCHING: %s", buf);
		LOG_SPLIT(FD_LOG_NOTICE, "     ", buf, NULL);
		break;
	
/* peers */
	case HOOK_PEER_CONNECT_FAILED:
		LOG_N("CONNECT FAILED to %s: %s", peer_name, (char *)other);
		break;
	case HOOK_PEER_CONNECT_SUCCESS:
		{
			char protobuf[40];
			if (peer) {
				CHECK_FCT_DO(fd_peer_cnx_proto_info(peer, protobuf, sizeof(protobuf)), break );
			} else {
				protobuf[0] = '-';
				protobuf[1] = '\0';
			}
			LOG_N("CONNECTED TO '%s' (%s):", peer_name, protobuf);
			LOG_SPLIT(FD_LOG_NOTICE, "     ", buf, NULL);
		}
		break;

/* Not handled */
	case HOOK_DATA_RECEIVED:
		break;
	}
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&mtx), );
}

static void md_hook_cb_full(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata)
{
	char * peer_name = peer ? peer->info.pi_diamid : "<unknown peer>";
	
	CHECK_POSIX_DO( pthread_mutex_lock(&mtx), );
	
	if (msg) {
		CHECK_MALLOC_DO( fd_msg_dump_full(&buf, &len, NULL, msg, fd_g_config->cnf_dict, (type == HOOK_MESSAGE_PARSING_ERROR) ? 0 : 1, 1), 
			{ LOG_E("Error while dumping a message"); pthread_mutex_unlock(&mtx); return; } );
	}
	
	switch (type) {
/* errors */
	case HOOK_MESSAGE_FAILOVER:
		LOG_E("FAILOVER from '%s': %s", peer_name, buf);
		break;
	case HOOK_MESSAGE_PARSING_ERROR:
		if (msg) {
			DiamId_t id = NULL;
			if (!fd_msg_source_get( msg, &id, NULL ))
				id = (DiamId_t)"<error getting source>";
			if (!id)
				id = (DiamId_t)"<local>";
			LOG_E("PARSING ERROR: '%s' from '%s': %s", (char *)other, (char *)id, buf);
		} else {
			struct fd_cnx_rcvdata *rcv_data = other;
			CHECK_MALLOC_DO(fd_dump_extend_hexdump(&buf, &len, NULL, rcv_data->buffer, rcv_data->length, 0, 0), break);
			LOG_E("PARSING ERROR: %zdB msg from '%s': %s", rcv_data->length, peer_name, buf);
		}
		break;
	case HOOK_MESSAGE_ROUTING_ERROR:
		LOG_E("ROUTING ERROR '%s' for: %s", (char *)other, buf);
		break;
	case HOOK_MESSAGE_DROPPED:
		LOG_E("DROPPED '%s' %s", (char *)other, buf);
		break;
	
/* send receive */
	case HOOK_MESSAGE_RECEIVED:
		LOG_N("RCV from '%s': %s", peer_name, buf);
		break;
	case HOOK_MESSAGE_SENT:
		LOG_N("SND to '%s': %s", peer_name, buf);
		break;
	
/* routing */
	case HOOK_MESSAGE_LOCAL:
		LOG_N("ISSUED: %s", buf);
		break;
	case HOOK_MESSAGE_ROUTING_FORWARD:
		LOG_N("FORWARDING: %s", buf);
		break;
	case HOOK_MESSAGE_ROUTING_LOCAL:
		LOG_N("DISPATCHING: %s", buf);
		break;
	
/* peers */
	case HOOK_PEER_CONNECT_FAILED:
		LOG_N("CONNECT FAILED to %s: %s", peer_name, (char *)other);
		break;
	case HOOK_PEER_CONNECT_SUCCESS: {
			char protobuf[40];
			if (peer) {
				CHECK_FCT_DO(fd_peer_cnx_proto_info(peer, protobuf, sizeof(protobuf)), break );
			} else {
				protobuf[0] = '-';
				protobuf[1] = '\0';
			}
			LOG_N("CONNECTED TO '%s' (%s): %s", peer_name, protobuf, buf);
		}
		break;
/* Not handled */
	case HOOK_DATA_RECEIVED:
		break;
	}
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&mtx), );
}

static void md_hook_cb_compact(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata)
{
	char * peer_name = peer ? peer->info.pi_diamid : "<unknown peer>";
	
	CHECK_POSIX_DO( pthread_mutex_lock(&mtx), );
	
	if (msg) {
		CHECK_MALLOC_DO( fd_msg_dump_summary(&buf, &len, NULL, msg, fd_g_config->cnf_dict, 0, 0), 
			{ LOG_E("Error while dumping a message"); pthread_mutex_unlock(&mtx); return; } );
	}
	
	switch (type) {
/* errors */
	case HOOK_MESSAGE_FAILOVER:
		LOG_E("FAILOVER from '%s': %s", peer_name, buf);
		break;
	case HOOK_MESSAGE_PARSING_ERROR:
		if (msg) {
			DiamId_t id = NULL;
			if (!fd_msg_source_get( msg, &id, NULL ))
				id = (DiamId_t)"<error getting source>";
			if (!id)
				id = (DiamId_t)"<local>";
			LOG_E("PARSING ERROR: '%s' from '%s': %s", (char *)other, (char *)id, buf);
		} else {
			struct fd_cnx_rcvdata *rcv_data = other;
			CHECK_MALLOC_DO(fd_dump_extend_hexdump(&buf, &len, NULL, rcv_data->buffer, rcv_data->length, 0, 0), break);
			LOG_E("PARSING ERROR: %zdB msg from '%s': %s", rcv_data->length, peer_name, buf);
		}
		break;
	case HOOK_MESSAGE_ROUTING_ERROR:
		LOG_E("ROUTING ERROR '%s' for: %s", (char *)other, buf);
		break;
	case HOOK_MESSAGE_DROPPED:
		LOG_E("DROPPED '%s' %s", (char *)other, buf);
		break;
	
/* send receive */
	case HOOK_MESSAGE_RECEIVED:
		LOG_N("RCV from '%s': %s", peer_name, buf);
		break;
	case HOOK_MESSAGE_SENT:
		LOG_N("SND to '%s': %s", peer_name, buf);
		break;
	
/* routing */
	case HOOK_MESSAGE_LOCAL:
		LOG_N("ISSUED: %s", buf);
		break;
	case HOOK_MESSAGE_ROUTING_FORWARD:
		LOG_N("FORWARDING: %s", buf);
		break;
	case HOOK_MESSAGE_ROUTING_LOCAL:
		LOG_N("DISPATCHING: %s", buf);
		break;
	
/* peers */
	case HOOK_PEER_CONNECT_FAILED:
		LOG_N("CONNECT FAILED to %s: %s", peer_name, (char *)other);
		break;
	case HOOK_PEER_CONNECT_SUCCESS: {
			char protobuf[40];
			if (peer) {
				CHECK_FCT_DO(fd_peer_cnx_proto_info(peer, protobuf, sizeof(protobuf)), break );
			} else {
				protobuf[0] = '-';
				protobuf[1] = '\0';
			}
			LOG_N("CONNECTED TO '%s' (%s)", peer_name, protobuf);
		}
		break;
/* Not handled */
	case HOOK_DATA_RECEIVED:
		break;
	}
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&mtx), );
}

static void md_hook_cb_quiet(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata)
{
}

/* Entry point */
static int md_main(char * conffile)
{
	uint32_t mask_errors, mask_sndrcv, mask_routing, mask_peers;
	uint32_t mask_quiet, mask_compact, mask_full, mask_tree;
	TRACE_ENTRY("%p", conffile);
	
	if (conffile != NULL) {
		char * endp;
		dump_level = (uint32_t)strtoul(conffile, &endp, 16);
		CHECK_PARAMS_DO( *endp == '\0', {
			LOG_E("Configuration parameter must be in the form \"0xNNNN\"");
			return EINVAL; });
	}
	
	mask_errors = HOOK_MASK( HOOK_MESSAGE_FAILOVER, HOOK_MESSAGE_PARSING_ERROR, HOOK_MESSAGE_ROUTING_ERROR, HOOK_MESSAGE_DROPPED  );
	mask_sndrcv = HOOK_MASK( HOOK_MESSAGE_RECEIVED, HOOK_MESSAGE_SENT );
	mask_routing= HOOK_MASK( HOOK_MESSAGE_LOCAL, HOOK_MESSAGE_ROUTING_FORWARD, HOOK_MESSAGE_ROUTING_LOCAL );
	mask_peers  = HOOK_MASK( HOOK_PEER_CONNECT_FAILED, HOOK_PEER_CONNECT_SUCCESS );
	
	mask_quiet  = (dump_level & HK_ERRORS_QUIET)  ? mask_errors : 0;
	mask_quiet |= (dump_level & HK_SNDRCV_QUIET)  ? mask_sndrcv : 0;
	mask_quiet |= (dump_level & HK_ROUTING_QUIET) ? mask_routing : 0;
	mask_quiet |= (dump_level & HK_PEERS_QUIET)   ? mask_peers : 0;
	
	mask_compact  = (dump_level & HK_ERRORS_COMPACT)  ? mask_errors : 0;
	mask_compact |= (dump_level & HK_SNDRCV_COMPACT)  ? mask_sndrcv : 0;
	mask_compact |= (dump_level & HK_ROUTING_COMPACT) ? mask_routing : 0;
	mask_compact |= (dump_level & HK_PEERS_COMPACT)   ? mask_peers : 0;
	
	mask_full  = (dump_level & HK_ERRORS_FULL)  ? mask_errors : 0;
	mask_full |= (dump_level & HK_SNDRCV_FULL)  ? mask_sndrcv : 0;
	mask_full |= (dump_level & HK_ROUTING_FULL) ? mask_routing : 0;
	mask_full |= (dump_level & HK_PEERS_FULL)   ? mask_peers : 0;
	
	mask_tree  = (dump_level & HK_ERRORS_TREE)  ? mask_errors : 0;
	mask_tree |= (dump_level & HK_SNDRCV_TREE)  ? mask_sndrcv : 0;
	mask_tree |= (dump_level & HK_ROUTING_TREE) ? mask_routing : 0;
	mask_tree |= (dump_level & HK_PEERS_TREE)   ? mask_peers : 0;
	
	if (mask_quiet) {
		CHECK_FCT( fd_hook_register( mask_quiet, md_hook_cb_quiet, NULL, NULL, &md_hdl[0]) );
	}
	if (mask_compact) {
		CHECK_FCT( fd_hook_register( mask_compact, md_hook_cb_compact, NULL, NULL, &md_hdl[1]) );
	}
	if (mask_full) {
		CHECK_FCT( fd_hook_register( mask_full, md_hook_cb_full, NULL, NULL, &md_hdl[2]) );
	}
	if (mask_tree) {
		CHECK_FCT( fd_hook_register( mask_tree, md_hook_cb_tree, NULL, NULL, &md_hdl[3]) );
	}

	return 0;
}

/* Cleanup */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	if (md_hdl[0]) { CHECK_FCT_DO( fd_hook_unregister( md_hdl[0] ), ); }
	if (md_hdl[1]) { CHECK_FCT_DO( fd_hook_unregister( md_hdl[1] ), ); }
	if (md_hdl[2]) { CHECK_FCT_DO( fd_hook_unregister( md_hdl[2] ), ); }
	if (md_hdl[2]) { CHECK_FCT_DO( fd_hook_unregister( md_hdl[3] ), ); }
	return ;
}

EXTENSION_ENTRY("dbg_msg_dumps", md_main);
