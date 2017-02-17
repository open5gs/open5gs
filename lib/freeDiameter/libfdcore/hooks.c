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

#include "fdcore-internal.h"

/* Structures for the fd_hook_data_hdl management */
static struct fd_hook_data_hdl {
	size_t	pmd_size;
	void  (*pmd_init_cb)(struct fd_hook_permsgdata *);
	void  (*pmd_fini_cb)(struct fd_hook_permsgdata *);
} HDH_array[FD_HOOK_HANDLE_LIMIT];
static int max_index = 0;
static pthread_mutex_t HDH_lock = PTHREAD_MUTEX_INITIALIZER;

/* The structure linked from the msg structure list */
struct pmd_list_item {
	struct fd_list	chain;		/* this list is ordered by hdl */
	struct fd_hook_data_hdl * hdl; 
	struct fd_hook_permsgdata { } pmd; /* this data belongs to the extension; we only know the size of it */
};

#define sizeof_pmd(hdl)	(((size_t)&((struct pmd_list_item *)0)->pmd) + hdl->pmd_size)

/* Now a hook registered by an extension */
struct fd_hook_hdl {
	struct fd_list chain[HOOK_LAST+1];
	void (*fd_hook_cb)(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata);
	void  *regdata;
	struct fd_hook_data_hdl *data_hdl;
};

/* Array of those hooks */
struct {
	struct fd_list sentinel;
	pthread_rwlock_t rwlock;
} HS_array[HOOK_LAST+1];

/* Initialize the array of sentinels for the hooks */
int fd_hooks_init(void)
{
	int i;
	for (i=0; i <= HOOK_LAST; i++) {
		fd_list_init(&HS_array[i].sentinel, NULL);
		CHECK_POSIX( pthread_rwlock_init(&HS_array[i].rwlock, NULL) );
	}
	return 0;
}

/* Get a slot in the array */
int fd_hook_data_register(
	size_t permsgdata_size,
	void (*permsgdata_init_cb) (struct fd_hook_permsgdata *),
        void (*permsgdata_fini_cb) (struct fd_hook_permsgdata *),
        struct fd_hook_data_hdl **new_handle)
{
	int ret = ENOSPC, idx;
	TRACE_ENTRY("%zd %p %p %p", permsgdata_size, permsgdata_init_cb, permsgdata_fini_cb, new_handle);
	
	CHECK_PARAMS( permsgdata_size && new_handle );
	
	CHECK_POSIX( pthread_mutex_lock(&HDH_lock) );
	if (max_index < FD_HOOK_HANDLE_LIMIT) {
		idx = max_index++;
		ret = 0;
	}
	CHECK_POSIX( pthread_mutex_unlock(&HDH_lock) );
	
	if (ret == 0) {
		HDH_array[idx].pmd_size = permsgdata_size;
		HDH_array[idx].pmd_init_cb = permsgdata_init_cb;
		HDH_array[idx].pmd_fini_cb = permsgdata_fini_cb;
		*new_handle = &HDH_array[idx];
	}
	
	return ret;
}

/* Register a new hook callback */
int fd_hook_register (  uint32_t type_mask, 
			void (*fd_hook_cb)(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata), 
			void  *regdata, 
			struct fd_hook_data_hdl *data_hdl,
			struct fd_hook_hdl ** handler )
{
	struct fd_hook_hdl * newhdl = NULL;
	int i;
	
	TRACE_ENTRY("%x %p %p %p %p", type_mask, fd_hook_cb, regdata, data_hdl, handler);
	
	CHECK_PARAMS( fd_hook_cb && handler );
	
	CHECK_MALLOC( newhdl = malloc(sizeof(struct fd_hook_hdl)) );
	memset(newhdl, 0, sizeof(struct fd_hook_hdl));
	
	newhdl->fd_hook_cb = fd_hook_cb;
	newhdl->regdata = regdata;
	newhdl->data_hdl = data_hdl;
	
	for (i=0; i <= HOOK_LAST; i++) {
		fd_list_init(&newhdl->chain[i], newhdl);
		if (type_mask & (1<<i)) {
			CHECK_POSIX( pthread_rwlock_wrlock(&HS_array[i].rwlock) );
			fd_list_insert_before( &HS_array[i].sentinel, &newhdl->chain[i]);
			CHECK_POSIX( pthread_rwlock_unlock(&HS_array[i].rwlock) );
		}
	}
	
	*handler = newhdl;
	return 0;
}

/* free this hook callback */
int fd_hook_unregister( struct fd_hook_hdl * handler )
{
	int i;
	TRACE_ENTRY("%p", handler);
	CHECK_PARAMS( handler );
	
	for (i=0; i <= HOOK_LAST; i++) {
		if ( ! FD_IS_LIST_EMPTY(&handler->chain[i])) {
			CHECK_POSIX( pthread_rwlock_wrlock(&HS_array[i].rwlock) );
			fd_list_unlink(&handler->chain[i]);
			CHECK_POSIX( pthread_rwlock_unlock(&HS_array[i].rwlock) );
		}
	}
	
	free(handler);
	
	return 0;
}

/* callback for the libfdproto to free the data associated with a message */
static void pmdl_free(struct fd_msg_pmdl *pmdl)
{
	/* destroy all the items in the list */
	while (!FD_IS_LIST_EMPTY(&pmdl->sentinel)) {
		struct pmd_list_item * li = (struct pmd_list_item *)(pmdl->sentinel.next);
		if (li->hdl->pmd_fini_cb) {
			(*li->hdl->pmd_fini_cb)(&li->pmd);
		}
		fd_list_unlink(&li->chain);
		free(li);
	}
	CHECK_POSIX_DO( pthread_mutex_destroy(&pmdl->lock), );
	pmdl->sentinel.o = NULL;
}

/* Save the list of pmd into the message structure, as well as the callback to free this list */
void   fd_hook_associate(struct msg * msg, struct fd_msg_pmdl * pmdl)
{
	struct fd_msg_pmdl * in_msg;
	
	CHECK_PARAMS_DO( msg && pmdl, return );
	in_msg = fd_msg_pmdl_get(msg);
	ASSERT(in_msg && (in_msg->sentinel.o == NULL)); /* error / already initialized ??? */
	in_msg->sentinel.o = pmdl_free;
	/* Now move all items from the pmdl pointer into the initialized list */
	CHECK_POSIX_DO( pthread_mutex_lock(&pmdl->lock), );
	fd_list_move_end(&in_msg->sentinel, &pmdl->sentinel);
	CHECK_POSIX_DO( pthread_mutex_unlock(&pmdl->lock), );
	pmdl_free(pmdl);
	/* We're done */
}

/* Return the location of the permsgdata area corresponding to this handle, after eventually having created it. Return NULL in case of failure */
static struct fd_hook_permsgdata * get_or_create_pmd(struct fd_msg_pmdl *pmdl, struct fd_hook_data_hdl * h) 
{
	struct fd_hook_permsgdata * ret = NULL;
	struct fd_list * li;
	
	CHECK_POSIX_DO( pthread_mutex_lock(&pmdl->lock), );
	
	if (pmdl->sentinel.o == NULL) {
		pmdl->sentinel.o = pmdl_free;
	}
	
	/* Search in the list for an item with the same handle. The list is ordered by this handle */
	for (li=pmdl->sentinel.next; li != &pmdl->sentinel; li = li->next) {
		struct pmd_list_item * pli = (struct pmd_list_item *) li;
		if (pli->hdl == h)
			ret = &pli->pmd;
		if (pli->hdl >= h)
			break;
	}
	if (!ret) {
		/* we need to create a new one and insert before li */
		struct pmd_list_item * pli;
		CHECK_MALLOC_DO( pli = malloc(sizeof_pmd(h)), );
		if (pli) {
			memset(pli, 0, sizeof_pmd(h));
			fd_list_init(&pli->chain, pli);
			pli->hdl = h;
			ret = &pli->pmd;
			if (h->pmd_init_cb) {
				(*h->pmd_init_cb)(ret);
			}
			fd_list_insert_before(li, &pli->chain);
		}
	}
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&pmdl->lock), );
	return ret;
}

struct fd_hook_permsgdata * fd_hook_get_pmd(struct fd_hook_data_hdl *data_hdl, struct msg * msg)
{
	struct fd_msg_pmdl *pmdl;
	struct fd_hook_permsgdata * ret = NULL;
	struct fd_list * li;
	
	pmdl = fd_msg_pmdl_get(msg);
	if (!pmdl)
		return NULL;
	
	ret = get_or_create_pmd(pmdl, data_hdl);
	return ret;
}

struct fd_hook_permsgdata * fd_hook_get_request_pmd(struct fd_hook_data_hdl *data_hdl, struct msg * answer)
{
	struct msg * qry;
	struct fd_msg_pmdl *pmdl;
	struct fd_hook_permsgdata * ret = NULL;
	struct fd_list * li;
	
	CHECK_FCT_DO( fd_msg_answ_getq(answer, &qry), return NULL );
	if (!qry)
		return NULL;
	
	pmdl = fd_msg_pmdl_get(qry);
	if (!pmdl)
		return NULL;
	
	CHECK_POSIX_DO( pthread_mutex_lock(&pmdl->lock), );
	/* Search in the list for an item with the same handle. The list is ordered by this handle */
	for (li=pmdl->sentinel.next; li != &pmdl->sentinel; li = li->next) {
		struct pmd_list_item * pli = (struct pmd_list_item *) li;
		if (pli->hdl == data_hdl)
			ret = &pli->pmd;
		if (pli->hdl >= data_hdl)
			break;
	}
	CHECK_POSIX_DO( pthread_mutex_unlock(&pmdl->lock), );
	return ret;
}

/* Create a mask */
uint32_t fd_hook_mask_helper(int dummy, ...)
{
	va_list ap;
	uint32_t ret = 0;
	int next;
	
	va_start(ap, dummy);
	while ((next = va_arg(ap, int)) >= 0) {
		if (next > HOOK_LAST)
			break; /* invalid parameter */
		ret |= (1<<next);
	}
	va_end(ap);

	return ret;
}

static pthread_mutex_t hook_default_mtx = PTHREAD_MUTEX_INITIALIZER;
static char * hook_default_buf = NULL;
static size_t hook_default_len = 0;

/* The function that does the work of calling the extension's callbacks and also managing the permessagedata structures */
void   fd_hook_call(enum fd_hook_type type, struct msg * msg, struct fd_peer * peer, void * other, struct fd_msg_pmdl * pmdl)
{
	struct fd_list * li;
	ASSERT(type <= HOOK_LAST);
	int call_default = 0;
	
	/* lock the list of hooks for this type */
	CHECK_POSIX_DO( pthread_rwlock_rdlock(&HS_array[type].rwlock), );
	
	pthread_cleanup_push( fd_cleanup_rwlock, &HS_array[type].rwlock );
	
	if (FD_IS_LIST_EMPTY(&HS_array[type].sentinel)) {
		call_default = 1;
	} else {
		/* for each registered hook */
		for (li = HS_array[type].sentinel.next; li != &HS_array[type].sentinel; li = li->next) {
			struct fd_hook_hdl * h = (struct fd_hook_hdl *)li->o;
			struct fd_hook_permsgdata * pmd = NULL;

			/* do we need to handle pmd ? */
			if (h->data_hdl && pmdl) {
				pmd = get_or_create_pmd(pmdl, h->data_hdl);
			}

			/* Now, call this callback */
			(*h->fd_hook_cb)(type, msg, &peer->p_hdr, other, pmd, h->regdata);
		}
	}
	
	pthread_cleanup_pop(0);
	
	/* done */
	CHECK_POSIX_DO( pthread_rwlock_unlock(&HS_array[type].rwlock), );
	
	if (call_default) {
		CHECK_POSIX_DO( pthread_mutex_lock(&hook_default_mtx), );
		
		pthread_cleanup_push( fd_cleanup_mutex, &hook_default_mtx );
	
		/* There was no registered handler, default behavior for this hook */
		switch (type) {
			case HOOK_DATA_RECEIVED: {
				struct fd_cnx_rcvdata *rcv_data = other;
				LOG_A("RCV: %zd bytes", rcv_data->length);
				break;
			}
			
			case HOOK_MESSAGE_RECEIVED: {
				CHECK_MALLOC_DO(fd_msg_dump_summary(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_D("RCV from '%s': %s", peer ? peer->p_hdr.info.pi_diamid : "<unknown>", hook_default_buf);
				break;
			}
			
			case HOOK_MESSAGE_LOCAL: {
				CHECK_MALLOC_DO(fd_msg_dump_full(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_A("Handled to framework for sending: %s", hook_default_buf);
				break;
			}
			
			case HOOK_MESSAGE_PRE_SEND: {
				CHECK_MALLOC_DO(fd_msg_dump_summary(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_D("GOING TO SEND TO '%s': %s", peer ? peer->p_hdr.info.pi_diamid : "<unknown>", hook_default_buf);
				break;
			}

			case HOOK_MESSAGE_SENT: {
				CHECK_MALLOC_DO(fd_msg_dump_summary(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_D("SENT to '%s': %s", peer ? peer->p_hdr.info.pi_diamid : "<unknown>", hook_default_buf);
				break;
			}
			
			case HOOK_MESSAGE_FAILOVER: {
				CHECK_MALLOC_DO(fd_msg_dump_summary(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_D("Failing over message sent to '%s': %s", peer ? peer->p_hdr.info.pi_diamid : "<unknown>", hook_default_buf);
				break;
			}
			
			case HOOK_MESSAGE_PARSING_ERROR: {
				if (msg) {
					DiamId_t id = NULL;
					if (!fd_msg_source_get( msg, &id, NULL ))
						id = (DiamId_t)"<error getting source>";
					
					if (!id)
						id = (DiamId_t)"<local>";
					
					CHECK_MALLOC_DO(fd_msg_dump_treeview(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
					
					LOG_E("Parsing error: '%s' for the following message received from '%s':", (char *)other, (char *)id);
					LOG_SPLIT(FD_LOG_ERROR, "   ", hook_default_buf, NULL);
				} else {
					struct fd_cnx_rcvdata *rcv_data = other;
					CHECK_MALLOC_DO(fd_dump_extend_hexdump(&hook_default_buf, &hook_default_len, NULL, rcv_data->buffer, rcv_data->length, 0, 0), break);
					LOG_E("Parsing error: cannot parse %zdB buffer from '%s': %s",  rcv_data->length, peer ? peer->p_hdr.info.pi_diamid : "<unknown>", hook_default_buf);
				}
				break;
			}
			
			case HOOK_MESSAGE_ROUTING_ERROR: {
				CHECK_MALLOC_DO(fd_msg_dump_treeview(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_E("Routing error: '%s' for the following message:", (char *)other);
				LOG_SPLIT(FD_LOG_ERROR, "   ", hook_default_buf, NULL);
				break;
			}
			
			case HOOK_MESSAGE_ROUTING_FORWARD: {
				CHECK_MALLOC_DO(fd_msg_dump_summary(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_D("FORWARDING: %s", hook_default_buf);
				break;
			}
			
			case HOOK_MESSAGE_ROUTING_LOCAL: {
				CHECK_MALLOC_DO(fd_msg_dump_summary(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_D("DISPATCHING: %s", hook_default_buf);
				break;
			}
			
			case HOOK_MESSAGE_DROPPED: {
				CHECK_MALLOC_DO(fd_msg_dump_treeview(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				LOG_E("Message discarded ('%s'):", (char *)other);
				LOG_SPLIT(FD_LOG_ERROR, "   ", hook_default_buf, NULL);
				break;
			}
			
			case HOOK_PEER_CONNECT_FAILED: {
				if (msg) {
					CHECK_MALLOC_DO(fd_msg_dump_full(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
					LOG_N("Connection to '%s' failed: '%s'; CER/CEA dump:", peer ? peer->p_hdr.info.pi_diamid : "<unknown>", (char *)other);
					LOG_SPLIT(FD_LOG_NOTICE, "   ", hook_default_buf, NULL);
				} else {
					LOG_D("Connection to '%s' failed: %s", peer ? peer->p_hdr.info.pi_diamid : "<unknown>", (char *)other);
				}
				break;
			}
		
			case HOOK_PEER_CONNECT_SUCCESS: {
				DiamId_t id = NULL;
				if ((!fd_msg_source_get( msg, &id, NULL )) && (id == NULL)) { /* The CEA is locally issued */
					fd_msg_answ_getq(msg, &msg); /* We dump the CER in that case */
				}
				CHECK_MALLOC_DO(fd_msg_dump_full(&hook_default_buf, &hook_default_len, NULL, msg, NULL, 0, 1), break);
				char protobuf[40];
				if (peer) {
					CHECK_FCT_DO(fd_peer_cnx_proto_info(&peer->p_hdr, protobuf, sizeof(protobuf)), break );
				} else {
					protobuf[0] = '-';
					protobuf[1] = '\0';
				}
				LOG_N("Connected to '%s' (%s), remote capabilities: ", peer ? peer->p_hdr.info.pi_diamid : "<unknown>", protobuf);
				LOG_SPLIT(FD_LOG_NOTICE, "   ", hook_default_buf, NULL);
				break;
			}
			
		}
		
		pthread_cleanup_pop(0);
		
		CHECK_POSIX_DO( pthread_mutex_unlock(&hook_default_mtx), );
	}
}
