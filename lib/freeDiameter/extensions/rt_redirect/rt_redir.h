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

/* Diameter Redirect management */
#include <freeDiameter/extension.h>

/* There are 2 locks in this module. The priority is established as follow to avoid deadlocks:
exp_peer mutex > usages rwlock.
(e.g., the rwlock can be taken while holding the mutex, but not the other way) 
*/

/* The types of redirects (from Redirect-Host-Usage AVP value) */
enum redir_h_u {
	DONT_CACHE = 0,
	ALL_SESSION,
	ALL_REALM,
	REALM_AND_APPLICATION,
	ALL_APPLICATION,
	ALL_HOST,
	ALL_USER
};
#define H_U_MAX	ALL_USER

/* Eye catcher */
#define REDIR_ENTRY_EYEC  0x43D14E74

/* Expiration time set for DONT_CACHE tasks, so that the entry is found when the code is called back */
#define DEFAULT_EXPIRE_TIME 10 /* seconds */
	
/* Structure to store a parsed Redirect-Host */
struct redir_host {
	struct fd_list chain; 
	
	DiamId_t id;	/* malloc'd */
	size_t	 len;
	/* We don't use the following yet because we don't support dynamic new connections 
	int 	 secure;
	uint16_t port;
	int	 l4;
	char	 proto;
	*/
};

/* Rule data that is stored depending on Redirect-Host-Usage value */
union matchdata {
	/* DONT_CACHE */
	struct {
		struct msg * msg; /* The query message for which this rule was created */
	} message;

	/* ALL_SESSION */
	struct {
		os0_t s;  /* sid */
		size_t l;
	} session;

	/* ALL_REALM */
	struct {
		os0_t s;  /* Destination-Realm AVP data */
		size_t l;
	} realm;

	/* REALM_AND_APPLICATION */
	struct {
		application_id_t a; /* message's application */
		os0_t s;  /* Destination-Realm AVP data */
		size_t l;
	} realm_app;

	/* ALL_APPLICATION */
	struct {
		application_id_t a; /* message's application */
	} app;

	/* ALL_HOST */
	struct {
		os0_t s;  /* the Origin-Host of the Redirect message (which may be the next hop or not) */
		size_t l;
	} host;

	/* ALL_USER */
	struct {
		os0_t s;  /* User-Name AVP data */
		size_t l;
	} user;
};
	

/* Structure to store a Redirect indication */
struct redir_entry {
	uint32_t eyec; /* must be REDIR_ENTRY_EYEC, used for debug only */
	
	struct {
		os0_t s; /* alloc'd, must be freed */
		size_t l;
	} from; /* whom this rule was received from (next hop) ? */

	struct fd_list	 target_peers_list; /* The list of Redirect-Hosts for this entry */

	struct timespec  timeout;  /* When does this entry expires? */
	struct fd_list   exp_list; /* chain in the expire_list list, ordered by expiration date, protected by exp_peer_lock */
	
	enum redir_h_u type;  /* Type of this entry */
	struct fd_list redir_list; /* link in redirects_usages lists. Lists are ordered by the data value. Protected by rw locks */
	union matchdata	data;	/* The strings are duplicated & must be freed in this structure */
};

/* The array where the redir_entries are stored */
struct redir_line {
	enum fd_rt_out_score 	score;
	pthread_rwlock_t	lock; /* protect the list */
	struct fd_list		sentinel; /* list of redir_entry, the "o" field of the sentinel points to the redir_line entry */
};
extern struct redir_line redirects_usages[];

/* Accelerator to the line lock */
#define RWLOCK_REDIR( _entry ) ( &(redirects_usages[(_entry)->type].lock) )

/* Lock that must be owned before calling some functions */
extern pthread_mutex_t redir_exp_peer_lock;

/* Dictionary cached objects */
extern struct dict_object * redir_dict_dr;
extern struct dict_object * redir_dict_un;

/* Functions on redir_entry */
int redir_entry_init();
int redir_entry_new(struct redir_entry ** e, struct fd_list * targets, uint32_t rhu, struct msg * qry, DiamId_t nh, size_t nhlen, os0_t oh, size_t ohlen);
extern int (*redir_entry_cmp_key[])(union matchdata * , union matchdata *); /* compare functions */
int redir_entry_insert(struct redir_entry * e);
int redir_entry_destroy(struct redir_entry * e);

/* Functions for expiry */
void * redir_exp_thr_fct(void * arg);
int redir_exp_set(struct redir_entry * e, uint32_t duration);

/* Forward cb */
int redir_fwd_cb(void * cbdata, struct msg ** msg);

/* Out callback */
int redir_out_cb(void * cbdata, struct msg ** pmsg, struct fd_list * candidates);
