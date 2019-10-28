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

/* Manage the list of RADIUS clients, along with their shared secrets. */

/* Probably some changes are needed to support RADIUS Proxies */

#include "rgw.h"

#define REVERSE_DNS_SIZE_MAX	512 /* length of our buffer for reverse DNS */
#define DUPLICATE_CHECK_LIFETIME 60 /* number of seconds that the received RADIUS records are kept for duplicate checking . TODO: make it configurable if needed */

/* Ordered lists of clients. The order relationship is a memcmp on the address zone. 
   For same addresses, the port is compared.
   The same address cannot be added twice, once with a 0-port and once with another port value.
 */
static struct fd_list cli_ip = FD_LIST_INITIALIZER(cli_ip);
static struct fd_list cli_ip6 = FD_LIST_INITIALIZER(cli_ip6);

/* Lock to protect the previous lists. We use a rwlock because this list is mostly static, to allow parallel reading */
static pthread_rwlock_t cli_rwl = PTHREAD_RWLOCK_INITIALIZER;

/* Structure describing one received RADIUS message, for duplicate checks purpose. */
struct req_info {
	uint16_t	port; 	/* UDP source port of the request */
	uint8_t		id;	/* The identifier in the request header */
	uint8_t		auth[16]; /* Request authenticator, since some RADIUS clients do not implement the id mechanism properly. */
	struct radius_msg *ans; /* The replied answer if any, in case the previous answer got lost. */
	
	int		nbdup;  /* Number of times this request was received as a duplicate */
	struct fd_list	by_id;	  /* The list of requests ordered by their id, port, and auth */
	time_t 		received; /* When was the last duplicate received? */
	struct fd_list  by_time;  /* The list of requests ordered by the 'received' value . */
};

static pthread_t dbt_expire = (pthread_t)NULL; /* The thread that will remove old requests information from all clients (one thread for all) */

/* Structure describing one client */
struct rgw_client {
	/* Link information in global list (cli_ip or cli_ip6) */
	struct fd_list		chain;
	
	/* Reference count */
	int			refcount;
	
	/* The address and optional port (alloc'd during configuration file parsing). */
	union {
		struct sockaddr		*sa; /* generic pointer */
		struct sockaddr_in	*sin;
		struct sockaddr_in6	*sin6;
	};
	
	/* The FQDN, realm, and optional aliases */
	int			 is_local; /* true if the RADIUS client runs on the same host -- we use Diameter Identity in that case */
	enum rgw_cli_type 	 type; /* is it a proxy ? */
	DiamId_t		 fqdn; /* malloc'd here */
	size_t			 fqdn_len;
	DiamId_t		 realm; /* references another string, do not free */
	size_t			 realm_len;
	struct {
		os0_t		 name;
		size_t		 len;
	}			*aliases; /* Received aliases */
	size_t			 aliases_nb;
	
	/* The secret key data. */
	struct {
		unsigned char * data;
		size_t		len;
	} 			key;
	
	/* information of previous msg received, for duplicate checks. */
	struct {
		pthread_mutex_t dupl_lock;    /* The mutex protecting the following lists */
		struct fd_list 	dupl_by_id;   /* The list of req_info structures ordered by their id, port, and auth */
		struct fd_list 	dupl_by_time; /* The list of req_info structures ordered by their time (approximative) */
	} dupl_info[2]; /*[0] for auth, [1] for acct. */
};


/* Create a new req_info structure and initialize its data from a RADIUS request message */
static struct req_info * dupl_new_req_info(struct rgw_radius_msg_meta *msg) {
	struct req_info * ret = NULL;
	CHECK_MALLOC_DO( ret = malloc(sizeof(struct req_info)), return NULL );
	memset(ret, 0, sizeof(struct req_info));
	ret->port = msg->port;
	ret->id   = msg->radius.hdr->identifier;
	memcpy(&ret->auth[0], &msg->radius.hdr->authenticator[0], 16);
	fd_list_init(&ret->by_id, ret);
	fd_list_init(&ret->by_time, ret);
	ret->received = time(NULL);
	return ret;
}

/* Destroy a req_info structure, after it has been unlinked */
static void dupl_free_req_info(struct req_info * r) {
	CHECK_PARAMS_DO( r && FD_IS_LIST_EMPTY(&r->by_id) && FD_IS_LIST_EMPTY(&r->by_time), return );
	if (r->ans) {
		/* Free this RADIUS message */
		radius_msg_free(r->ans);
		free(r->ans);
	}
	
	/* Use r->nbdup for some purpose? */
	
	free(r);
}

/* The core of the purge thread */
static int dupl_purge_list(struct fd_list * clients) {

	struct fd_list *li = NULL;
	
	for (li = clients->next; li != clients; li = li->next) {
		struct rgw_client * client = (struct rgw_client *)li;
		int p;
		
		for (p=0; p<=1; p++) {
		
			/* Lock this list */
			time_t now;
			CHECK_POSIX( pthread_mutex_lock(&client->dupl_info[p].dupl_lock) );
			
			now = time(NULL);
			
			while (!FD_IS_LIST_EMPTY(&client->dupl_info[p].dupl_by_time)) {
			
				/* Check the first item in the list */
				struct req_info * r = (struct req_info *)(client->dupl_info[p].dupl_by_time.next->o);
				
				if (now - r->received > DUPLICATE_CHECK_LIFETIME) {
				
					TRACE_DEBUG(ANNOYING + 1, "Purging RADIUS request (id: %02hhx, port: %hu, dup #%d, age %ld secs)", r->id, ntohs(r->port), r->nbdup, (long)(now - r->received));
					
					/* Remove this record */
					fd_list_unlink(&r->by_time);
					fd_list_unlink(&r->by_id);
					dupl_free_req_info(r);
				} else {
					/* We are done for this list */
					break;
				}
			}
			
			CHECK_POSIX( pthread_mutex_unlock(&client->dupl_info[p].dupl_lock) );
		}
	}
	return 0;
}

/* Thread that purges old RADIUS requests */
static void * dupl_th(void * arg) {
	/* Set the thread name */
	fd_log_threadname ( "app_radgw:duplicate_purge" );
	
	/* The thread will be canceled */
	while (1) {
		
		/* We don't use a cond var, we simply wake up every 5 seconds. If the size of the duplicate cache is critical, it might be changed */
		sleep(5);
		
		/* When we wake up, we will check all clients duplicate lists one by one */
		CHECK_POSIX_DO( pthread_rwlock_rdlock(&cli_rwl), break );
		
		CHECK_FCT_DO( dupl_purge_list(&cli_ip), break );
		CHECK_FCT_DO( dupl_purge_list(&cli_ip6), break );
		
		CHECK_POSIX_DO( pthread_rwlock_unlock(&cli_rwl), break );
	
		/* Loop */
	}
	
	/* If we reach this part, some fatal error was encountered */
	CHECK_FCT_DO(fd_core_shutdown(), );
	TRACE_DEBUG(FULL, "Thread terminated");	
	return NULL;
}


/* create a new rgw_client. the arguments are MOVED into the structure (to limit malloc & free calls). */
static int client_create(struct rgw_client ** res, struct sockaddr ** ip_port, unsigned char ** key, size_t keylen, enum rgw_cli_type type )
{
	struct rgw_client *tmp = NULL;
	DiamId_t fqdn;
	size_t fqdn_len = 0;
	int ret, i;
	int loc = 0;
	
	/* Check if the IP address is local */
	if (   ( ((*ip_port)->sa_family == AF_INET ) && (   IN_IS_ADDR_LOOPBACK( &((struct sockaddr_in  *)(*ip_port))->sin_addr ) ) )
	     ||( ((*ip_port)->sa_family == AF_INET6) && (  IN6_IS_ADDR_LOOPBACK( &((struct sockaddr_in6 *)(*ip_port))->sin6_addr) ) )) {
		/* The client is local */
		loc = 1;
	} else {
		char buf[255];
	
		/* Search FQDN for the client */
		ret = getnameinfo( *ip_port, sizeof(struct sockaddr_storage), &buf[0], sizeof(buf), NULL, 0, 0 );
		if (ret) {
			TRACE_DEBUG(INFO, "Unable to resolve peer name: %s", gai_strerror(ret));
			return EINVAL;
		}
		fqdn = &buf[0];
		CHECK_FCT_DO( ret = fd_os_validate_DiameterIdentity(&fqdn, &fqdn_len, 1),
			{
				TRACE_DEBUG(INFO, "Unable to use resolved peer name '%s' as DiameterIdentity: %s", buf, strerror(ret));
				return ret;
			} );
	}
	
	/* Create the new object */
	CHECK_MALLOC( tmp = malloc(sizeof (struct rgw_client)) );
	memset(tmp, 0, sizeof(struct rgw_client));
	fd_list_init(&tmp->chain, NULL);
	
	/* Initialize the duplicate list info */
	for (i=0; i<=1; i++) {
		CHECK_POSIX( pthread_mutex_init(&tmp->dupl_info[i].dupl_lock, NULL) );
		fd_list_init(&tmp->dupl_info[i].dupl_by_id, NULL);
		fd_list_init(&tmp->dupl_info[i].dupl_by_time, NULL);
	}
	tmp->type = type;
	
	if (loc) {
		tmp->is_local = 1;
	} else {
		/* Copy the fqdn */
		tmp->fqdn = fqdn;
		tmp->fqdn_len = fqdn_len;

		/* Find an appropriate realm */
		tmp->realm = strchr(fqdn, '.');
		if (tmp->realm) {
			tmp->realm += 1;
			tmp->realm_len = tmp->fqdn_len - (tmp->realm - fqdn);
		}
		if ((!tmp->realm) || (*tmp->realm == '\0')) { /* in case the fqdn was "localhost." for example, if it is possible... */
			tmp->realm = fd_g_config->cnf_diamrlm;
			tmp->realm_len = fd_g_config->cnf_diamrlm_len;
		}
	}
	
	/* move the sa info reference */
	tmp->sa = *ip_port;
	*ip_port = NULL;
	
	/* move the key material */
	tmp->key.data = *key;
	tmp->key.len = keylen;
	*key = NULL;
	
	/* Done! */
	*res = tmp;
	return 0;
}

/* Decrease refcount on a client; the lock must be held when this function is called. */
static void client_unlink(struct rgw_client * client)
{
	client->refcount -= 1;
	
	if (client->refcount <= 0) {
		int idx;
		/* to be sure: the refcount should be 0 only when client_fini is called */
		ASSERT( FD_IS_LIST_EMPTY(&client->chain) );
		
		/* Free the data */
		for (idx = 0; idx < client->aliases_nb; idx++)
			free(client->aliases[idx].name);
		free(client->aliases);
		free(client->fqdn);
		free(client->sa);
		free(client->key.data);
		
		/* Free the duplicate info */
		for (idx=0; idx <= 1; idx++){
			CHECK_POSIX_DO( pthread_mutex_lock( &client->dupl_info[idx].dupl_lock ), /* continue */ );
			
			while (!FD_IS_LIST_EMPTY(&client->dupl_info[idx].dupl_by_id)) {
				struct req_info * r = (struct req_info *)(client->dupl_info[idx].dupl_by_id.next->o);
				fd_list_unlink( &r->by_id );
				fd_list_unlink( &r->by_time );
				dupl_free_req_info(r);
			}
			
			CHECK_POSIX_DO( pthread_mutex_unlock( &client->dupl_info[idx].dupl_lock ), /* continue */ );

		}
		
		free(client);
	}
}


/* Macro to avoid duplicating the code in the next function */
#define client_search_family( _family_ )												\
		case AF_INET##_family_: {												\
			struct sockaddr_in##_family_ * sin##_family_ = (struct sockaddr_in##_family_ *)ip_port;				\
			for (ref = cli_ip##_family_.next; ref != &cli_ip##_family_; ref = ref->next) {					\
				cmp = memcmp(&sin##_family_->sin##_family_##_addr, 							\
					     &((struct rgw_client *)ref)->sin##_family_->sin##_family_##_addr, 				\
					     sizeof(struct in##_family_##_addr));							\
				if (cmp > 0) continue; /* search further in the list */							\
				if (cmp < 0) break; /* this IP is not in the list */							\
				/* Now compare the ports as follow: */									\
				     /* If the ip_port we are searching does not contain a port, just return the first match result */	\
				if ( (sin##_family_->sin##_family_##_port == 0) 							\
				     /* If the entry in the list does not contain a port, return it as a match */			\
				  || (((struct rgw_client *)ref)->sin##_family_->sin##_family_##_port == 0) 				\
				     /* If both ports are equal, it is a match */							\
				  || (sin##_family_->sin##_family_##_port == 								\
				  		((struct rgw_client *)ref)->sin##_family_->sin##_family_##_port)) {			\
					*res = (struct rgw_client *)ref;								\
					return EEXIST;											\
				}													\
				/* Otherwise, the list is ordered by port value (byte order does not matter */				\
				if (sin##_family_->sin##_family_##_port 								\
					> ((struct rgw_client *)ref)->sin##_family_->sin##_family_##_port) continue;			\
				else break;												\
			}														\
			*res = (struct rgw_client *)(ref->prev);									\
			return ENOENT;													\
		}
/* Function to look for an existing rgw_client, or the previous element. 
   The cli_rwl must be held for reading (at least) when calling this function. 
   Returns ENOENT if the matching client does not exist, and res points to the previous element in the list. 
   Returns EEXIST if the matching client is found, and res points to this element. 
   Returns other error code on other error. */
static int client_search(struct rgw_client ** res, struct sockaddr * ip_port )
{
	int cmp;
	struct fd_list *ref = NULL;
	
	CHECK_PARAMS(res && ip_port);
	
	switch (ip_port->sa_family) {
		client_search_family()
				break;
		
		client_search_family( 6 )
				break;
	}
	
	/* We're never supposed to reach this point */
	ASSERT(0);
	return EINVAL;
}

int rgw_clients_getkey(struct rgw_client * cli, unsigned char **key, size_t *key_len)
{
	CHECK_PARAMS( cli && key && key_len );
	*key = cli->key.data;
	*key_len = cli->key.len;
	return 0;
}

int rgw_clients_gettype(struct rgw_client * cli, enum rgw_cli_type *type)
{
	CHECK_PARAMS( cli && type );
	*type = cli->type;
	return 0;
}


int rgw_clients_search(struct sockaddr * ip_port, struct rgw_client ** ref)
{
	int ret = 0;
	
	TRACE_ENTRY("%p %p", ip_port, ref);
	
	CHECK_PARAMS(ip_port && ref);
	
	CHECK_POSIX( pthread_rwlock_rdlock(&cli_rwl) );

	ret = client_search(ref, ip_port);
	if (ret == EEXIST) {
		(*ref)->refcount ++;
		ret = 0;
	} else {
		*ref = NULL;
	}
	
	CHECK_POSIX( pthread_rwlock_unlock(&cli_rwl) );
	
	return ret;
}

int rgw_clients_check_dup(struct rgw_radius_msg_meta **msg, struct rgw_client *cli)
{
	int p, dup = 0;
	struct fd_list * li;
	struct req_info * r;
	
	TRACE_ENTRY("%p %p", msg, cli);
	
	CHECK_PARAMS( msg && cli );
	
	if ((*msg)->serv_type == RGW_PLG_TYPE_AUTH)
		p = 0;
	else
		p = 1;
	
	CHECK_POSIX( pthread_mutex_lock( &cli->dupl_info[p].dupl_lock ) );
	
	/* Search if we have this message in our list */
	for (li = cli->dupl_info[p].dupl_by_id.next; li != &cli->dupl_info[p].dupl_by_id; li = li->next) {
		int cmp = 0;
		r = (struct req_info *)(li->o);
		if (r->id < (*msg)->radius.hdr->identifier)
			continue;
		if (r->id > (*msg)->radius.hdr->identifier)
			break;
		if (r->port < (*msg)->port)
			continue;
		if (r->port > (*msg)->port)
			break;
		cmp = memcmp(&r->auth[0], &(*msg)->radius.hdr->authenticator[0], 16);
		if (cmp < 0)
			continue;
		if (cmp > 0)
			break;
		dup = 1;
		break;
	}
	
	if (dup) {
		time_t now = time(NULL);
		r->nbdup += 1;
		TRACE_DEBUG(INFO, "Received duplicated RADIUS message (id: %02hhx, port: %hu, dup #%d, previously seen %ld secs ago).", 
				r->id, ntohs(r->port), r->nbdup, (long)(now - r->received));
		
		if (r->ans) {
			/* Resend the answer */
			CHECK_FCT_DO( rgw_servers_send((*msg)->serv_type, r->ans->buf, r->ans->buf_used, cli->sa, r->port),  );
			
			/* Should we delete 'r' so that a further duplicate will again be converted to Diameter? */
		}
		
		/* Update the timestamp */
		r->received = now;
		fd_list_unlink(&r->by_time);
		fd_list_insert_before(&cli->dupl_info[p].dupl_by_time, &r->by_time); /* Move as last entry, since it is the most recent */
		
		/* Delete the request message */
		rgw_msg_free(msg);
		
	} else {
		/* The message was not a duplicate, we save it */
		/* li currently points the the next entry in list_by_id */
		CHECK_MALLOC_DO( r= dupl_new_req_info(*msg), { CHECK_POSIX_DO(pthread_mutex_unlock( &cli->dupl_info[p].dupl_lock ), ); return ENOMEM; } );
		fd_list_insert_before(li, &r->by_id);
		fd_list_insert_before(&cli->dupl_info[p].dupl_by_time, &r->by_time); /* it is the most recent */
	}
		
	CHECK_POSIX( pthread_mutex_unlock( &cli->dupl_info[p].dupl_lock ) );
	
	return 0;
}

/* Check if the message has a valid authenticator, and update the meta-data accordingly */
int rgw_clients_auth_check(struct rgw_radius_msg_meta * msg, struct rgw_client * cli, uint8_t * req_auth)
{
	unsigned char * key;
	size_t keylen;
	int count;
	
	TRACE_ENTRY("%p %p %p", msg, cli, req_auth);
	
	CHECK_PARAMS(msg && cli);
	
	CHECK_FCT(rgw_clients_getkey(cli, &key, &keylen));
	
	count = radius_msg_count_attr(&msg->radius, RADIUS_ATTR_MESSAGE_AUTHENTICATOR, 0);
	if (count > 1) {
		TRACE_DEBUG(INFO, "Too many Message-Authenticator attributes (%d), discarding message.", count);
		return EINVAL;
	}
	if (count == 0) {
		TRACE_DEBUG(FULL, "Message does not contain a Message-Authenticator attributes.");
		msg->valid_mac = 0;
	} else {
		if (radius_msg_verify_msg_auth( &msg->radius, key, keylen, req_auth )) {
			TRACE_DEBUG(INFO, "Invalid Message-Authenticator received, discarding message.");
			return EINVAL;
		}
		msg->valid_mac = 1;
	}
	
	return 0;
}

static struct dict_object * cache_orig_host = NULL;
static struct dict_object * cache_orig_realm = NULL;
static struct dict_object * cache_route_record = NULL;

int rgw_clients_init(void)
{
	TRACE_ENTRY();
	CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Host", &cache_orig_host, ENOENT) );
	CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Realm", &cache_orig_realm, ENOENT) );
	CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Route-Record", &cache_route_record, ENOENT) );
	
	/* Create the thread that will purge old RADIUS duplicates */
	CHECK_POSIX( pthread_create( &dbt_expire, NULL, dupl_th, NULL) );
	
	return 0;
}


/* The following function checks if a RADIUS message contains a valid NAS identifier, and initializes an empty Diameter
 message with the appropriate routing information */
/* Check that the NAS-IP-Adress or NAS-Identifier is coherent with the IP the packet was received from */
/* Also update the client list of aliases if needed */
int rgw_clients_create_origin(struct rgw_radius_msg_meta *msg, struct rgw_client * cli, struct msg ** diam)
{
	int idx;
	int valid_nas_info = 0;
	struct radius_attr_hdr *nas_ip = NULL, *nas_ip6 = NULL, *nas_id = NULL;
	size_t nas_id_len;
	char * oh_str = NULL; size_t oh_strlen = 0; int oh_free = 0;
	char * or_str = NULL; size_t or_strlen = 0;
	char * rr_str = NULL; size_t rr_strlen = 0;
	char buf[REVERSE_DNS_SIZE_MAX]; /* to store DNS lookups results */
	
	struct avp *avp = NULL;
	union avp_value avp_val;
	
	TRACE_ENTRY("%p %p %p", msg, cli, diam);
	CHECK_PARAMS(msg && cli && diam && (*diam == NULL));
	
	/* Find the relevant attributes, if any */
	for (idx = 0; idx < msg->radius.attr_used; idx++) {
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(msg->radius.buf + msg->radius.attr_pos[idx]);
		size_t attr_len = attr->length - sizeof(struct radius_attr_hdr);
		
		if ((attr->type == RADIUS_ATTR_NAS_IP_ADDRESS) && (attr_len = 4)) {
			nas_ip = attr;
			continue;
		}
			
		if ((attr->type == RADIUS_ATTR_NAS_IDENTIFIER) && (attr_len > 0)) {
			nas_id = attr;
			nas_id_len = attr_len;
			continue;
		}
			
		if ((attr->type == RADIUS_ATTR_NAS_IPV6_ADDRESS) && (attr_len = 16)) {
			nas_ip6 = attr;
			continue;
		}
	}
		
	if (!nas_ip && !nas_ip6 && !nas_id) {
		TRACE_DEBUG(FULL, "The message does not contain any NAS identification attribute.");
		
		/* Get information on this peer */
		CHECK_FCT( rgw_clients_get_origin(cli, &oh_str, &oh_strlen, &or_str, &or_strlen) );
		
		goto diameter;
	}
	
	/* Check if the message was received from the IP in NAS-IP-Address attribute */
	if (nas_ip && (cli->sa->sa_family == AF_INET) && !memcmp(nas_ip+1, &cli->sin->sin_addr, sizeof(struct in_addr))) {
		TRACE_DEBUG(FULL, "NAS-IP-Address contains the same address as the message was received from.");
		valid_nas_info |= 1;
	}
	if (nas_ip6 && (cli->sa->sa_family == AF_INET6) && !memcmp(nas_ip6+1, &cli->sin6->sin6_addr, sizeof(struct in6_addr))) {
		TRACE_DEBUG(FULL, "NAS-IPv6-Address contains the same address as the message was received from.");
		valid_nas_info |= 2;
	}
	
	
	/*
			In RADIUS it would be possible for a rogue NAS to forge the NAS-IP-
			Address attribute value.  Diameter/RADIUS translation agents MUST
			check a received NAS-IP-Address or NAS-IPv6-Address attribute against
			the source address of the RADIUS packet.  If they do not match and
			the Diameter/RADIUS translation agent does not know whether the
			packet was sent by a RADIUS proxy or NAS (e.g., no Proxy-State
			attribute), then by default it is assumed that the source address
			corresponds to a RADIUS proxy, and that the NAS Address is behind
			that proxy, potentially with some additional RADIUS proxies in
			between.  The Diameter/RADIUS translation agent MUST insert entries
			in the Route-Record AVP corresponding to the apparent route.  This
			implies doing a reverse lookup on the source address and NAS-IP-
			Address or NAS-IPv6-Address attributes to determine the corresponding
			FQDNs.

			If the source address and the NAS-IP-Address or NAS-IPv6-Address do
			not match, and the Diameter/RADIUS translation agent knows that it is
			talking directly to the NAS (e.g., there are no RADIUS proxies
			between it and the NAS), then the error should be logged, and the
			packet MUST be discarded.

			Diameter agents and servers MUST check whether the NAS-IP-Address AVP
			corresponds to an entry in the Route-Record AVP.  This is done by
			doing a reverse lookup (PTR RR) for the NAS-IP-Address to retrieve
			the corresponding FQDN, and by checking for a match with the Route-
			Record AVP.  If no match is found, then an error is logged, but no
			other action is taken.
	*/
	if (nas_ip || nas_ip6) {
		if (!valid_nas_info) {
			if ((!cli->is_local) && (cli->type == RGW_CLI_NAS)) {
				TRACE_DEBUG(INFO, "Message received with a NAS-IP-Address or NAS-IPv6-Address different from the sender's. Please configure as Proxy if this is expected. Message discarded.");
				return EINVAL;
			} else {
				int ret;
				sSS ss;
				/* the peer is configured as a proxy, or running on localhost, so accept the message */
				
				/* In that case, the cli will be stored as Route-Record and the NAS-IP-Address as origin */
				if (!cli->is_local) {
					rr_str = cli->fqdn;
					rr_strlen = cli->fqdn_len;
				}
				
				/* We must DNS-reverse the NAS-IP*-Address */
				memset(&ss, 0 , sizeof(sSS));
				if (nas_ip) {
					sSA4 * sin = (sSA4 *)&ss;
					sin->sin_family = AF_INET;
					memcpy(&sin->sin_addr, nas_ip + 1, sizeof(struct in_addr));
				} else {
					sSA6 * sin6 = (sSA6 *)&ss;
					sin6->sin6_family = AF_INET6;
					memcpy(&sin6->sin6_addr, nas_ip6 + 1, sizeof(struct in6_addr));
				}
				CHECK_SYS_DO( getnameinfo( (sSA *)&ss, sSAlen(&ss), &buf[0], sizeof(buf), NULL, 0, NI_NAMEREQD),
					{
						if (cli->is_local) {
							CHECK_FCT( rgw_clients_get_origin(cli, &oh_str, &oh_strlen, &or_str, &or_strlen) );
							goto diameter;
						}
						
						TRACE_DEBUG(INFO, "The NAS-IP*-Address cannot be DNS reversed in order to create the Origin-Host AVP; rejecting the message (translation is impossible).");
						return EINVAL;
					} );
					
				oh_str = &buf[0];
				CHECK_FCT_DO( ret = fd_os_validate_DiameterIdentity(&oh_str, &oh_strlen, 1),
					{
						if (cli->is_local) {
							CHECK_FCT( rgw_clients_get_origin(cli, &oh_str, &oh_strlen, &or_str, &or_strlen) );
							goto diameter;
						}
						
						TRACE_DEBUG(INFO, "Unable to use resolved client name '%s' as DiameterIdentity: %s", buf, strerror(ret));
						return ret;
					} );
				oh_free = 1;
				
				or_str = strchr(oh_str, '.');
				if (or_str) {
					or_str ++; /* move after the first dot */
					if (*or_str == '\0')
						or_str = NULL; /* Discard this realm, we will use the local realm later */
					else
						or_strlen = oh_strlen - (or_str - oh_str);
				}
			}
		} else {
			/* The attribute matches the source address, just use this in origin-host */
			CHECK_FCT( rgw_clients_get_origin(cli, &oh_str, &oh_strlen, &or_str, &or_strlen) );
		}
		
		goto diameter; /* we ignore the nas_id in that case */
	}
	
	/* We don't have a NAS-IP*-Address attribute if we are here */
	if (cli->is_local) {
		/* Simple: we use our own configuration */
		CHECK_FCT( rgw_clients_get_origin(cli, &oh_str, &oh_strlen, &or_str, &or_strlen) );
		goto diameter;
	}
	
	/* At this point, we only have nas_id, and the client is not local */
	ASSERT(nas_id);
	
	{
		int found, ret;
		struct addrinfo hint, *res, *ptr;
		
		/*
			In RADIUS it would be possible for a rogue NAS to forge the NAS-
			Identifier attribute.  Diameter/RADIUS translation agents SHOULD
			attempt to check a received NAS-Identifier attribute against the
			source address of the RADIUS packet, by doing an A/AAAA RR query.  If
			the NAS-Identifier attribute contains an FQDN, then such a query
			would resolve to an IP address matching the source address.  However,
			the NAS-Identifier attribute is not required to contain an FQDN, so
			such a query could fail.  If it fails, an error should be logged, but
			no action should be taken, other than a reverse lookup on the source
			address and insert the resulting FQDN into the Route-Record AVP.

			Diameter agents and servers SHOULD check whether a NAS-Identifier AVP
			corresponds to an entry in the Route-Record AVP.  If no match is
			found, then an error is logged, but no other action is taken.
		*/
		
		/* first, check if the nas_id is the fqdn of the peer or a known alias */
		if (!fd_os_almostcasesrch(nas_id + 1, nas_id_len, 
						cli->fqdn, cli->fqdn_len, NULL)) {
			TRACE_DEBUG(FULL, "NAS-Identifier contains the fqdn of the client");
			found = 1;
		} else {
			for (idx = 0; idx < cli->aliases_nb; idx++) {
				if (!fd_os_cmp(nas_id + 1, nas_id_len, 
						cli->aliases[idx].name, cli->aliases[idx].len)) {
					TRACE_DEBUG(FULL, "NAS-Identifier valid value found in the cache");
					found = 1;
					break;
				}
			}
		}
		
		if (found) {
			/* The NAS-Identifier matches the source IP */
			CHECK_FCT( rgw_clients_get_origin(cli, &oh_str, &oh_strlen, &or_str, &or_strlen) );

			goto diameter;
		}
		
		/* Attempt DNS resolution of the identifier */
		ASSERT( nas_id_len < sizeof(buf) );
		memcpy(buf, nas_id + 1, nas_id_len);
		buf[nas_id->length - sizeof(struct radius_attr_hdr)] = '\0';
		
		/* Now check if this alias is valid for this peer */
		memset(&hint, 0, sizeof(hint));
		hint.ai_flags  = AI_CANONNAME;
		ret = getaddrinfo(buf, NULL, &hint, &res);
		if (ret == 0) {
			strncpy(buf, res->ai_canonname, sizeof(buf));
			/* The name was resolved correctly, does it match the IP of the client? */
			for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
				if (cli->sa->sa_family != ptr->ai_family)
					continue;
				if (memcmp(cli->sa, ptr->ai_addr, sSAlen(cli->sa)))
					continue;
				
				found = 1;
				break;
			}
			freeaddrinfo(res);
			
			if (!found) {
				if (cli->type == RGW_CLI_NAS) {
					TRACE_DEBUG(INFO, "The NAS-Identifier value '%.*s' resolves to a different IP than the client's, discarding the message. Configure this client as a Proxy if this message should be valid.", 
						(int)nas_id_len, (char *)(nas_id + 1));
					return EINVAL;
				} else {
					/* This identifier matches a different IP, assume it is a proxied message */
					if (!cli->is_local) {
						rr_str = cli->fqdn;
						rr_strlen = cli->fqdn_len;
					}
					oh_str = &buf[0]; /* The canonname resolved */
					oh_strlen = 0;
					CHECK_FCT_DO( ret = fd_os_validate_DiameterIdentity(&oh_str, &oh_strlen, 1),
						{
							TRACE_DEBUG(INFO, "Unable to use resolved client name '%s' as DiameterIdentity: %s", buf, strerror(ret));
							return ret;
						} );
					oh_free = 1;
					or_str = strchr(oh_str, '.');
					if (or_str) {
						or_str ++; /* move after the first dot */
						if (*or_str == '\0')
							or_str = NULL; /* Discard this realm, we will use the local realm later */
						else
							or_strlen = oh_strlen - (or_str - oh_str);
					}
				}
			} else {
				/* It is a valid alias, save it */
				CHECK_MALLOC( cli->aliases = realloc(cli->aliases, (cli->aliases_nb + 1) * sizeof(cli->aliases[0])) );
				
				CHECK_MALLOC( cli->aliases[cli->aliases_nb + 1].name = os0dup(nas_id + 1, nas_id_len ) );
				cli->aliases[cli->aliases_nb + 1].len = nas_id_len;

				cli->aliases_nb ++;
				TRACE_DEBUG(FULL, "Saved valid alias for client: '%.*s' -> '%s'", (int)nas_id_len, (char *)(nas_id + 1), cli->fqdn);
				CHECK_FCT( rgw_clients_get_origin(cli, &oh_str, &oh_strlen, &or_str, &or_strlen) );
			}
		} else {
			/* Error resolving the name */
			TRACE_DEBUG(INFO, "NAS-Identifier '%s' cannot be resolved: %s. Ignoring...", buf, gai_strerror(ret));
			/* Assume this is a valid identifier for the client */
			CHECK_FCT( rgw_clients_get_origin(cli, &oh_str, &oh_strlen, &or_str, &or_strlen) );
		}
	}
	
	/* Now, let's create the empty Diameter message with Origin-Host, -Realm, and Route-Record if needed. */
diameter:
	ASSERT(oh_str); /* If it is not defined here, there is a bug... */
	if (!or_str) {
		or_str = fd_g_config->cnf_diamrlm; /* Use local realm in that case */
		or_strlen = fd_g_config->cnf_diamrlm_len;
	}
	
	/* Create an empty Diameter message so that extensions can store their AVPs */
	CHECK_FCT(  fd_msg_new ( NULL, MSGFL_ALLOC_ETEID, diam )  );
	
	/* Add the Origin-Host as next AVP */
	CHECK_FCT( fd_msg_avp_new ( cache_orig_host, 0, &avp ) );
	memset(&avp_val, 0, sizeof(avp_val));
	avp_val.os.data = (unsigned char *)oh_str;
	avp_val.os.len = oh_strlen;
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &avp_val ) );
	CHECK_FCT( fd_msg_avp_add ( *diam, MSG_BRW_LAST_CHILD, avp) );
	
	/* Add the Origin-Realm as next AVP */
	CHECK_FCT( fd_msg_avp_new ( cache_orig_realm, 0, &avp ) );
	memset(&avp_val, 0, sizeof(avp_val));
	avp_val.os.data = (unsigned char *)or_str;
	avp_val.os.len = or_strlen;
	CHECK_FCT( fd_msg_avp_setvalue ( avp, &avp_val ) );
	CHECK_FCT( fd_msg_avp_add ( *diam, MSG_BRW_LAST_CHILD, avp) );
	
	if (rr_str) {
		CHECK_FCT( fd_msg_avp_new ( cache_route_record, 0, &avp ) );
		memset(&avp_val, 0, sizeof(avp_val));
		avp_val.os.data = (unsigned char *)rr_str;
		avp_val.os.len = rr_strlen;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &avp_val ) );
		CHECK_FCT( fd_msg_avp_add ( *diam, MSG_BRW_LAST_CHILD, avp) );
	}
	
	if (oh_free)
		free(oh_str);
	
	/* Done! */
	return 0;
}

int rgw_clients_get_origin(struct rgw_client *cli, DiamId_t *fqdn, size_t *fqdnlen, DiamId_t *realm, size_t *realmlen)
{
	TRACE_ENTRY("%p %p %p %p %p", cli, fqdn, fqdnlen, realm, realmlen);
	CHECK_PARAMS(cli && fqdn && fqdnlen);
	
	if (cli->is_local) {
		*fqdn = fd_g_config->cnf_diamid;
		*fqdnlen = fd_g_config->cnf_diamid_len;
		if (realm)
			*realm= fd_g_config->cnf_diamrlm;
		if (realmlen)
			*realmlen= fd_g_config->cnf_diamrlm_len;
	} else {
		*fqdn = cli->fqdn;
		*fqdnlen = cli->fqdn_len;
		if (realm)
			*realm= cli->realm;
		if (realmlen)
			*realmlen= cli->realm_len;
	}
		
	return 0;
}

char * rgw_clients_id(struct rgw_client *cli)
{
	return cli->is_local ? "(local)" : cli->fqdn;
}


void rgw_clients_dispose(struct rgw_client ** ref)
{
	TRACE_ENTRY("%p", ref);
	CHECK_PARAMS_DO(ref, return);
	
	CHECK_POSIX_DO( pthread_rwlock_wrlock(&cli_rwl),  );
	client_unlink(*ref);
	*ref = NULL;
	CHECK_POSIX_DO( pthread_rwlock_unlock(&cli_rwl), );
}

int rgw_clients_add( struct sockaddr * ip_port, unsigned char ** key, size_t keylen, enum rgw_cli_type type )
{
	struct rgw_client * prev = NULL, *new = NULL;
	int ret;
	
	TRACE_ENTRY("%p %p %zu", ip_port, key, keylen);
	
	CHECK_PARAMS( ip_port && key && *key && keylen );
	CHECK_PARAMS( (ip_port->sa_family == AF_INET) || (ip_port->sa_family == AF_INET6) );
	CHECK_PARAMS( (type == RGW_CLI_NAS) || (type == RGW_CLI_PXY) );
	
	/* Dump the entry in debug mode */
	if (TRACE_BOOL(FULL + 1 )) {
		char sa_buf[sSA_DUMP_STRLEN];
		fd_sa_sdump_numeric(sa_buf, ip_port);
		TRACE_DEBUG(FULL, "Adding %s:", (type == RGW_CLI_NAS) ? "NAS" : "PROXY"  );
		TRACE_DEBUG(FULL, "\tIP : %s", sa_buf );
		TRACE_BUFFER(FD_LOG_DEBUG, FULL, "\tKey: [", *key, keylen, "]" );
	}
	
	/* Lock the lists */
	CHECK_POSIX( pthread_rwlock_wrlock(&cli_rwl) );
	
	/* Check if the same entry does not already exist */
	ret = client_search(&prev, ip_port );
	if (ret == ENOENT) {
		/* No duplicate found, Ok to add */
		CHECK_FCT_DO( ret = client_create( &new, &ip_port, key, keylen, type ), goto end );
		fd_list_insert_after(&prev->chain, &new->chain);
		new->refcount++;
		ret = 0;
		goto end;
	}
	
	if (ret == EEXIST) {
		char sa_buf[sSA_DUMP_STRLEN];
		/* Check if the key is the same, then skip or return an error */
		if ((keylen == prev->key.len ) && ( ! memcmp(*key, prev->key.data, keylen) ) && (type == prev->type)) {
			TRACE_DEBUG(INFO, "Skipping duplicate client description");
			ret = 0;
			goto end;
		}
		
		fd_log_error("ERROR: Conflicting RADIUS clients descriptions!");
		TRACE_ERROR("Previous entry: %s", (prev->type == RGW_CLI_NAS) ? "NAS" : "PROXY");
		fd_sa_sdump_numeric(sa_buf, prev->sa);
		TRACE_ERROR("\tIP : %s", sa_buf);
		TRACE_BUFFER(FD_LOG_ERROR, NONE, "\tKey: [", prev->key.data, prev->key.len, "]" );
		TRACE_ERROR("Conflicting entry: %s", (type == RGW_CLI_NAS) ? "NAS" : "PROXY");
		fd_sa_sdump_numeric(sa_buf, ip_port);
		TRACE_ERROR("\tIP : %s", sa_buf);
		TRACE_BUFFER(FD_LOG_ERROR, NONE, "\tKey: [", *key, keylen, "]" );
	}
end:
	/* release the lists */
	CHECK_POSIX( pthread_rwlock_unlock(&cli_rwl) );
	
	return ret;
}

static void dump_cli_list(struct fd_list *senti)
{
	struct rgw_client * client = NULL;
	struct fd_list *ref = NULL;
	
	for (ref = senti->next; ref != senti; ref = ref->next) {
		char sa_buf[sSA_DUMP_STRLEN];
		client = (struct rgw_client *)ref;
		fd_sa_sdump_numeric(sa_buf, client->sa);
		LOG_D("  - %s%s", sa_buf, (client->type == RGW_CLI_NAS) ? "" : " [PROXY]" );
	}
}

void rgw_clients_dump(void)
{
	if ( ! TRACE_BOOL(FULL) )
		return;
	
	CHECK_POSIX_DO( pthread_rwlock_rdlock(&cli_rwl), /* ignore error */ );
	
	if (!FD_IS_LIST_EMPTY(&cli_ip))
		fd_log_debug(" RADIUS IP clients list:");
	dump_cli_list(&cli_ip);
		
	if (!FD_IS_LIST_EMPTY(&cli_ip6))
		fd_log_debug(" RADIUS IPv6 clients list:");
	dump_cli_list(&cli_ip6);
		
	CHECK_POSIX_DO( pthread_rwlock_unlock(&cli_rwl), /* ignore error */ );
}

void rgw_clients_fini(void)
{
	struct fd_list * client;
	
	TRACE_ENTRY();
	
	CHECK_POSIX_DO( pthread_rwlock_wrlock(&cli_rwl), /* ignore error */ );
	
	CHECK_FCT_DO( fd_thr_term(&dbt_expire), /* continue */ );

	/* empty the lists */
	while ( ! FD_IS_LIST_EMPTY(&cli_ip) ) {
		client = cli_ip.next;
		fd_list_unlink(client);
		client_unlink((struct rgw_client *)client);
	}
	while (! FD_IS_LIST_EMPTY(&cli_ip6)) {
		client = cli_ip6.next;
		fd_list_unlink(client);
		client_unlink((struct rgw_client *)client);
	}
	
	CHECK_POSIX_DO( pthread_rwlock_unlock(&cli_rwl), /* ignore error */ );
	
}

int rgw_client_finish_send(struct radius_msg ** msg, struct rgw_radius_msg_meta * req, struct rgw_client * cli)
{
	int p;
	struct fd_list * li;
	
	TRACE_ENTRY("%p %p %p", msg, req, cli);
	CHECK_PARAMS( msg && *msg && cli );
	
	if (!req) {
		/* We don't support this case yet */
		ASSERT(0);
		return ENOTSUP;
	}
	
	/* Add all the Proxy-States back in the message */
	for (p = 0; p < req->ps_nb; p++) {
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(req->radius.buf + req->radius.attr_pos[req->ps_first + p]);
		
		if (radius_msg_add_attr_to_array(*msg, attr)) {
			TRACE_DEBUG(INFO, "Error in radius_msg_add_attr_to_array, ENOMEM");
			radius_msg_free(*msg);
			free(*msg);
			*msg = NULL;
			return ENOMEM;
		}
	}
	
	/* Add the Message-Authenticator if needed, and other final tasks */
	if (radius_msg_finish_srv(*msg, cli->key.data, cli->key.len, req->radius.hdr->authenticator)) {
		TRACE_DEBUG(INFO, "An error occurred while preparing the RADIUS answer");
		radius_msg_free(*msg);
		free(*msg);
		*msg = NULL;
		return EINVAL;
	}
	
	/* Debug */
	TRACE_DEBUG(FULL, "RADIUS message ready for sending:");
	rgw_msg_dump((struct rgw_radius_msg_meta *)*msg, 0);

	/* Send the message */
	CHECK_FCT( rgw_servers_send(req->serv_type, (*msg)->buf, (*msg)->buf_used, cli->sa, req->port) );

	/* update the duplicate cache */
	if (req->serv_type == RGW_PLG_TYPE_AUTH)
		p = 0;
	else
		p = 1;
	
	CHECK_POSIX( pthread_mutex_lock( &cli->dupl_info[p].dupl_lock ) );
	
	/* Search this message in our list */
	for (li = cli->dupl_info[p].dupl_by_id.next; li != &cli->dupl_info[p].dupl_by_id; li = li->next) {
		int cmp = 0;
		struct req_info * r = (struct req_info *)(li->o);
		if (r->id < req->radius.hdr->identifier)
			continue;
		if (r->id > req->radius.hdr->identifier)
			break;
		if (r->port < req->port)
			continue;
		if (r->port > req->port)
			break;
		cmp = memcmp(&r->auth[0], &req->radius.hdr->authenticator[0], 16);
		if (cmp < 0)
			continue;
		if (cmp > 0)
			break;
		
		/* We have the request in our duplicate cache */
		/* This should not happen, but just in case... */
		if (r->ans) {
			radius_msg_free(r->ans);
			free(r->ans);
		}
		
		/* Now save the message */
		r->ans = *msg;
		*msg = NULL;
		
		/* Update the timestamp */
		{
			time_t now = time(NULL);
			r->received = now;
			fd_list_unlink(&r->by_time); /* Move as last entry, since it is the most recent */
			fd_list_insert_before(&cli->dupl_info[p].dupl_by_time, &r->by_time);
		}
		break;
	}
		
	CHECK_POSIX( pthread_mutex_unlock( &cli->dupl_info[p].dupl_lock ) );
	
	/* If we have not found the request in our list, the purge time is probably too small */
	if (*msg) {
		TODO("Augment the purge time...");
		/* If we receive the duplicate request again, it will be converted to Diameter... */
		radius_msg_free(*msg);
		free(*msg);
		*msg = NULL;
	}
	
	/* Finished */
	return 0;
}

/* Call this function when a RADIUS request has explicitely no answer (mainly accounting) so 
that we purge the duplicate cache and allow further message to be translated again.
This is useful for example when a temporary error occurred in Diameter (like UNABLE_TO_DELIVER) */
int rgw_client_finish_nosend(struct rgw_radius_msg_meta * req, struct rgw_client * cli)
{
	int p;
	struct fd_list * li;
	
	TRACE_ENTRY("%p %p", req, cli);
	CHECK_PARAMS( req && cli );
	
	/* update the duplicate cache */
	if (req->serv_type == RGW_PLG_TYPE_AUTH)
		p = 0;
	else
		p = 1;
	
	CHECK_POSIX( pthread_mutex_lock( &cli->dupl_info[p].dupl_lock ) );
	
	/* Search this message in our list */
	for (li = cli->dupl_info[p].dupl_by_id.next; li != &cli->dupl_info[p].dupl_by_id; li = li->next) {
		int cmp = 0;
		struct req_info * r = (struct req_info *)(li->o);
		if (r->id < req->radius.hdr->identifier)
			continue;
		if (r->id > req->radius.hdr->identifier)
			break;
		if (r->port < req->port)
			continue;
		if (r->port > req->port)
			break;
		cmp = memcmp(&r->auth[0], &req->radius.hdr->authenticator[0], 16);
		if (cmp < 0)
			continue;
		if (cmp > 0)
			break;
		
		/* We have the request in our duplicate cache, remove it */
		fd_list_unlink(&r->by_id);
		fd_list_unlink(&r->by_time);
		dupl_free_req_info(r);
		break;
	}
		
	CHECK_POSIX( pthread_mutex_unlock( &cli->dupl_info[p].dupl_lock ) );
	
	/* Finished */
	return 0;
}

