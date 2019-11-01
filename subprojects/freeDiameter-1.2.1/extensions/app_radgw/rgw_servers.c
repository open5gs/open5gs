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

/* Manage the RADIUS server(s): opening sockets, receiving messages, ... */

#include "rgw.h"

#define RADIUS_MAX_MSG_LEN 	3000
#define RADIUS_AUTH_PORT	1812
#define RADIUS_ACCT_PORT	1813

/* Declare the rgw_servers */
struct rgw_servs rgw_servers;

void rgw_servers_dump(void)
{
	char ipstr[INET6_ADDRSTRLEN];
	
	LOG_D(" auth server:");
	LOG_D("    disabled..... : %s", rgw_servers.auth_serv.disabled ? "TRUE":"false");
	LOG_D("    IP disabled.. : %s", rgw_servers.auth_serv.ip_disabled ? "TRUE":"false");
	LOG_D("    IPv6 disabled : %s", rgw_servers.auth_serv.ip6_disabled ? "TRUE":"false");
	LOG_D("    port......... : %hu", ntohs(rgw_servers.auth_serv.port));
	inet_ntop(AF_INET, &rgw_servers.auth_serv.ip_endpoint,ipstr,sizeof(ipstr));
	LOG_D("    IP bind...... : %s", ipstr);
	inet_ntop(AF_INET6, &rgw_servers.auth_serv.ip6_endpoint,ipstr,sizeof(ipstr));
	LOG_D("    IPv6 bind.... : %s", ipstr);

	LOG_D(" acct server:");
	LOG_D("    disabled..... : %s", rgw_servers.acct_serv.disabled ? "TRUE":"false");
	LOG_D("    IP disabled.. : %s", rgw_servers.acct_serv.ip_disabled ? "TRUE":"false");
	LOG_D("    IPv6 disabled : %s", rgw_servers.acct_serv.ip6_disabled ? "TRUE":"false");
	LOG_D("    port......... : %hu", ntohs(rgw_servers.acct_serv.port));
	inet_ntop(AF_INET, &rgw_servers.acct_serv.ip_endpoint,ipstr,sizeof(ipstr));
	LOG_D("    IP bind...... : %s", ipstr);
	inet_ntop(AF_INET6, &rgw_servers.acct_serv.ip6_endpoint,ipstr,sizeof(ipstr));
	LOG_D("    IPv6 bind.... : %s", ipstr);

}

static struct servers_data {
	int	type; /* auth or acct */
	int	family; /* AF_INET or AF_INET6 */
	int	sock; /* the socket number */
	pthread_t th; /* the running server thread, or NULL */
	char    name[10];
} SERVERS[4];

int rgw_servers_init(void)
{
	memset(&rgw_servers, 0, sizeof(rgw_servers));
	memset(&SERVERS[0], 0, sizeof(SERVERS));

	rgw_servers.auth_serv.port = htons(RADIUS_AUTH_PORT);
	rgw_servers.acct_serv.port = htons(RADIUS_ACCT_PORT);
	
	return 0;
}

static void * server_thread(void * param)
{
	struct servers_data * me = (struct servers_data *)param;
	
	TRACE_ENTRY("%p", param);
	
	CHECK_PARAMS_DO(param, return NULL);
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "radgw/%s serv", me->name);
		fd_log_threadname ( buf );
	}
	
	/* Now loop on this socket, parse and queue each message received, until thread is cancelled. */
	while (1) {
		struct sockaddr_storage from;
		char sa_buf[sSA_DUMP_STRLEN];
		socklen_t fromlen = sizeof(from);
		int len;
		struct rgw_client * nas_info = NULL;
		uint16_t port = 0;
		unsigned char buf[RADIUS_MAX_MSG_LEN];
		struct rgw_radius_msg_meta *msg = NULL;
		
		pthread_testcancel();
		
		/* receive the next message */
		CHECK_SYS_DO( len = recvfrom( me->sock, &buf[0], sizeof(buf), 0, (struct sockaddr *) &from, &fromlen),  break );
		
		/* Get the port */
		port = sSAport(&from);
		if (!port) {
			LOG_E("Invalid port (family: %d), discarding received %d bytes...", from.ss_family, len);
			continue;
		}
		
		fd_sa_sdump_numeric(sa_buf, (sSA*)&from);
		LOG_D("RADIUS: RCV %dB from %s", len, sa_buf);
		
		/* Search the associated client definition, if any */
		CHECK_FCT_DO( rgw_clients_search((struct sockaddr *) &from, &nas_info),
			{
				LOG_E("Discarding %d bytes received from unknown IP: %s", len, sa_buf);
				continue;
			} );
				
		
		/* parse the message, loop if message is invalid */
		CHECK_FCT_DO( rgw_msg_parse(&buf[0], len, &msg), 
			{
				DiamId_t cliname = NULL;
				size_t clisz;
				CHECK_FCT_DO( rgw_clients_get_origin(nas_info, &cliname, &clisz, NULL, NULL), );
				LOG_E( "Discarding invalid RADIUS message from '%s'", cliname);
				rgw_clients_dispose(&nas_info);
				continue; 
			} );
		
		msg->serv_type = me->type;
		msg->port = port;
		
		rgw_msg_dump(msg, 1);
		
		/* queue the message for a worker thread */
		CHECK_FCT_DO( rgw_work_add(msg, nas_info), break );
		
		/* Then wait for next incoming message */
	}
	
	TRACE_DEBUG(INFO, "Server thread terminated.");
	return NULL;
}

/* Set the socket options for UDP sockets, before bind is called */
static int _udp_setsockopt(int family, int sk)
{
	int ret = 0;
	int opt;
	
	/* In case of v6 address, force the v6only option, we use a different socket for v4 */
	#ifdef IPV6_V6ONLY
	if (family == AF_INET6) {
		opt = 1;
		ret = setsockopt(sk, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
		if (ret != 0) {
			ret = errno;
			TRACE_DEBUG(INFO, "Unable to set the socket IPV6_V6ONLY option: %s", strerror(ret));
			return ret;
		}
	}
	#endif /* IPV6_V6ONLY */
	
	return 0;
}

/* We reuse the same logic for all 4 possible servers (IP / IPv6, Auth / Acct ports) */
#define UDPSERV( _type_, _portval_, _family_ ) {								\
	/* Check that this type / family are not disabled by configuration */					\
	if ( (! rgw_servers. _type_ ## _serv.disabled) 								\
		&& ( ! rgw_servers. _type_ ## _serv.ip ## _family_ ## _disabled ) ) {				\
			struct sockaddr_in ## _family_	 sin ## _family_ ;					\
			/* Create the socket */									\
			CHECK_SYS( SERVERS[idx].sock = socket(AF_INET ## _family_, SOCK_DGRAM, 0) );		\
			/* Set the parameters for bind into "sin" or "sin6" */					\
			memset(& sin ## _family_, 0, sizeof(struct sockaddr_in ## _family_));			\
			sin ## _family_ . sin ## _family_ ## _family = AF_INET ## _family_;			\
			sin ## _family_ . sin ## _family_ ## _port = rgw_servers. _type_ ## _serv . port;	\
			memcpy( &sin ## _family_ .sin ## _family_ ## _addr, 					\
					&rgw_servers. _type_ ## _serv . ip ## _family_ ## _endpoint,		\
					sizeof(struct in ## _family_ ## _addr) );				\
			/* This sockopt must be set before binding */						\
			TRACE_DEBUG(ANNOYING, "Setting socket options...");					\
			CHECK_FCT( _udp_setsockopt(AF_INET ## _family_, SERVERS[idx].sock) );			\
			/* OK, now, bind */									\
			TRACE_DEBUG(ANNOYING, "Binding " #_type_ " ip" #_family_ " server...");			\
			CHECK_SYS( bind( SERVERS[idx].sock,							\
					(struct sockaddr *)&sin ## _family_,					\
					sizeof(struct sockaddr_in ## _family_) ) );				\
			/* Save the server information in SERVERS structure */					\
			SERVERS[idx].type = _portval_;								\
			SERVERS[idx].family = AF_INET ## _family_;						\
			snprintf(&SERVERS[idx].name[0], sizeof(SERVERS[idx].name), # _type_ "/ip" #_family_);	\
			/* Create the server thread */								\
			CHECK_POSIX( pthread_create(&SERVERS[idx].th, NULL, server_thread, &SERVERS[idx]) );	\
			idx++;											\
	}													\
}

int rgw_servers_start(void)
{
	int idx = 0;
	
	TRACE_ENTRY();
	
	UDPSERV( auth, RGW_PLG_TYPE_AUTH,  );
	UDPSERV( auth, RGW_PLG_TYPE_AUTH, 6 );
	UDPSERV( acct, RGW_PLG_TYPE_ACCT,  );
	UDPSERV( acct, RGW_PLG_TYPE_ACCT, 6 );
	
	TRACE_DEBUG(FULL, "%d UDP servers started succesfully.", idx);
	return 0;
}

/* Send a RADIUS message */
int rgw_servers_send(int type, unsigned char *buf, size_t buflen, struct sockaddr *to, uint16_t to_port)
{
	int idx = 0;
	int ret = 0;
	struct sockaddr_storage sto;
	char sa_buf[sSA_DUMP_STRLEN];
	
	/* Find the appropriate socket to use (not sure if it is important) */
	for (idx = 0; idx < sizeof(SERVERS) / sizeof(SERVERS[0]); idx++) {
		if ( SERVERS[idx].sock && (type == SERVERS[idx].type) && (to->sa_family == SERVERS[idx].family) ) {
			ret = 1;
			break;
		}
	}
	
	if (!ret) {
		LOG_E( "Trying to send a message from a disabled server: %s / %s", 
				(type == RGW_PLG_TYPE_AUTH) ? "Auth" : "Acct",
				(to->sa_family == AF_INET)  ? "IP (v4)" : "IPv6");
		return EINVAL;
	}
	
	/* Prepare the destination info */
	memset(&sto, 0, sizeof(sto));
	if (to->sa_family == AF_INET) {
		memcpy(&sto, to, sizeof(struct sockaddr_in));
		((struct sockaddr_in *)&sto)->sin_port = to_port;
	} else {
		memcpy(&sto, to, sizeof(struct sockaddr_in6));
		((struct sockaddr_in6 *)&sto)->sin6_port = to_port;
	}
	
	fd_sa_sdump_numeric(sa_buf, (sSA*)&sto);
	LOG_D("RADIUS: SND %zdB   to %s", buflen, sa_buf);
	
	/* Send */
	ret = sendto(SERVERS[idx].sock, buf, buflen, 0, (struct sockaddr *)&sto, sSAlen(&sto));
	if (ret < 0) {
		ret = errno;
		TRACE_DEBUG(INFO, "An error prevented sending of a RADIUS message: %s", strerror(ret));
		return ret;
	}
	if (ret != buflen) {
		TRACE_DEBUG(INFO, "Incomplete send: %d bytes / %zd", ret, buflen);
		return EAGAIN;
	}
	
	/* Done :) */
	return 0;
}

void rgw_servers_fini(void)
{
	int idx = 0;
	
	for (idx = 0; idx < sizeof(SERVERS) / sizeof(SERVERS[0]); idx++) {
		if (SERVERS[idx].sock == 0)
			break;
		
		CHECK_FCT_DO( fd_thr_term(&SERVERS[idx].th), /* continue */ );
		close(SERVERS[idx].sock);
		SERVERS[idx].sock = 0;
	}
	
}


