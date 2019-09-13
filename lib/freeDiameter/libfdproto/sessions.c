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

/* Sessions module.
 * 
 * Basic functionalities to help implementing User sessions state machines from RFC3588.
 */

#include "fdproto-internal.h"

/*********************** Parameters **********************/

/* Size of the hash table containing the session objects (pow of 2. ex: 6 => 2^6 = 64). must be between 0 and 31. */
#ifndef SESS_HASH_SIZE
#define SESS_HASH_SIZE	6
#endif /* SESS_HASH_SIZE */

/* Default lifetime of a session, in seconds. (31 days = 2678400 seconds) */
#ifndef SESS_DEFAULT_LIFETIME
#define SESS_DEFAULT_LIFETIME	2678400
#endif /* SESS_DEFAULT_LIFETIME */

/********************** /Parameters **********************/

/* Eyescatchers definitions */
#define SH_EYEC 0x53554AD1
#define SD_EYEC 0x5355D474
#define SI_EYEC 0x53551D

/* Macro to check an object is valid */
#define VALIDATE_SH( _obj ) ( ((_obj) != NULL) && ( ((struct session_handler *)(_obj))->eyec == SH_EYEC) )
#define VALIDATE_SI( _obj ) ( ((_obj) != NULL) && ( ((struct session         *)(_obj))->eyec == SI_EYEC) )


/* Handlers registered by users of the session module */
struct session_handler {
	int		  eyec;	/* An eye catcher also used to ensure the object is valid, must be SH_EYEC */
	int		  id;	/* A unique integer to identify this handler */
	void 		(*cleanup)(struct sess_state *, os0_t, void *); /* The cleanup function to be called for cleaning a state */
	session_state_dump state_dump; /* dumper function */
	void             *opaque; /* a value that is passed as is to the cleanup callback */
};

static int 		hdl_id = 0;				/* A global counter to initialize the id field */
static pthread_mutex_t	hdl_lock = PTHREAD_MUTEX_INITIALIZER;	/* lock to protect hdl_id; we could use atomic operations otherwise (less portable) */


/* Data structures linked from the sessions, containing the applications states */
struct state {
	int			 eyec;	/* Must be SD_EYEC */
	struct sess_state	*state;	/* The state registered by the application, never NULL (or the whole object is deleted) */
	struct fd_list		 chain;	/* Chaining in the list of session's states ordered by hdl->id */
	union {
		struct session_handler	*hdl;	/* The handler for which this state was registered */
		os0_t 			 sid;	/* For deleted state, the sid of the session it belong to */
	};
};

/* Session object, one for each value of Session-Id AVP */
struct session {
	int 		eyec;	/* Eyecatcher, SI_EYEC */
	
	os0_t		sid;	/* The \0-terminated Session-Id */
	size_t		sidlen; /* cached length of sid */
	uint32_t	hash;	/* computed hash of sid */
	struct fd_list	chain_h;/* chaining in the hash table of sessions. */
	
	struct timespec	timeout;/* Timeout date for the session */
	struct fd_list	expire;	/* List of expiring sessions, ordered by timeouts. */
	
	pthread_mutex_t stlock;	/* A lock to protect the list of states associated with this session */
	struct fd_list	states;	/* Sentinel for the list of states of this session. */
	int		msg_cnt;/* Reference counter for the messages pointing to this session */
	int		is_destroyed; /* boolean telling if fd_sess_detroy has been called on this */
};

/* Sessions hash table, to allow fast sid to session retrieval */
static struct {
	struct fd_list	sentinel;	/* sentinel element for this sublist. The sublist is ordered by hash value, then fd_os_cmp(sid). */
	pthread_mutex_t lock;		/* the mutex for this sublist -- we might probably change it to rwlock for a little optimization */
} sess_hash [ 1 << SESS_HASH_SIZE ] ;
#define H_MASK( __hash ) ((__hash) & (( 1 << SESS_HASH_SIZE ) - 1))
#define H_LIST( _hash ) (&(sess_hash[H_MASK(_hash)].sentinel))
#define H_LOCK( _hash ) (&(sess_hash[H_MASK(_hash)].lock    ))

static uint32_t		sess_cnt = 0; /* counts all active session (that are in the expiry list) */

/* The following are used to generate sid values that are eternaly unique */
static uint32_t   	sid_h;	/* initialized to the current time in fd_sess_init */
static uint32_t   	sid_l;	/* incremented each time a session id is created */
static pthread_mutex_t 	sid_lock = PTHREAD_MUTEX_INITIALIZER;

/* Expiring sessions management */
static struct fd_list	exp_sentinel = FD_LIST_INITIALIZER(exp_sentinel);	/* list of sessions ordered by their timeout date */
static pthread_mutex_t	exp_lock = PTHREAD_MUTEX_INITIALIZER;	/* lock protecting the list. */
static pthread_cond_t	exp_cond = PTHREAD_COND_INITIALIZER;	/* condvar used by the expiry mecahinsm. */
static pthread_t	exp_thr = (pthread_t)NULL; 	/* The expiry thread that handles cleanup of expired sessions */

/* Hierarchy of the locks, to avoid deadlocks:
 *  hash lock > state lock > expiry lock
 * i.e. state lock can be taken while holding the hash lock, but not while holding the expiry lock.
 * As well, the hash lock cannot be taken while holding a state lock.
 */

/********************************************************************************************************/

/* Initialize a session object. It is not linked now. sid must be already malloc'ed. The hash has already been computed. */
static struct session * new_session(os0_t sid, size_t sidlen, uint32_t hash)
{
	struct session * sess;
	
	TRACE_ENTRY("%p %zd", sid, sidlen);
	CHECK_PARAMS_DO( sid && sidlen, return NULL );
	
	CHECK_MALLOC_DO( sess = malloc(sizeof(struct session)), return NULL );
	memset(sess, 0, sizeof(struct session));
	
	sess->eyec = SI_EYEC;
	
	sess->sid  = sid;
	sess->sidlen = sidlen;
	sess->hash = hash;
	fd_list_init(&sess->chain_h, sess);
	
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &sess->timeout), return NULL );
	sess->timeout.tv_sec += SESS_DEFAULT_LIFETIME;
	fd_list_init(&sess->expire, sess);
	
	CHECK_POSIX_DO( pthread_mutex_init(&sess->stlock, NULL), return NULL );
	fd_list_init(&sess->states, sess);
	
	return sess;
}

/* destroy the session object. It should really be already unlinked... */
static void del_session(struct session * s)
{
	ASSERT(FD_IS_LIST_EMPTY(&s->states));
	free(s->sid);
	fd_list_unlink(&s->chain_h);
	fd_list_unlink(&s->expire);
	CHECK_POSIX_DO( pthread_mutex_destroy(&s->stlock), /* continue */ );
	free(s);
}
	
/* The expiry thread */
static void * exp_fct(void * arg)
{
	fd_log_threadname ( "Session/expire" );
	TRACE_ENTRY( "" );
	
	
	do {
		struct timespec	now;
		struct session * first;
		
		CHECK_POSIX_DO( pthread_mutex_lock(&exp_lock),  break );
		pthread_cleanup_push( fd_cleanup_mutex, &exp_lock );
again:		
		/* Check if there are expiring sessions available */
		if (FD_IS_LIST_EMPTY(&exp_sentinel)) {
			/* Just wait for a change or cancelation */
			CHECK_POSIX_DO( pthread_cond_wait( &exp_cond, &exp_lock ), break /* this might not pop the cleanup handler, but since we ASSERT(0), it is not the big issue... */ );
			/* Restart the loop on wakeup */
			goto again;
		}
		
		/* Get the pointer to the session that expires first */
		first = (struct session *)(exp_sentinel.next->o);
		ASSERT( VALIDATE_SI(first) );
		
		/* Get the current time */
		CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &now),  break  );

		/* If first session is not expired, we just wait until it happens */
		if ( TS_IS_INFERIOR( &now, &first->timeout ) ) {
			
			CHECK_POSIX_DO2(  pthread_cond_timedwait( &exp_cond, &exp_lock, &first->timeout ),  
					ETIMEDOUT, /* ETIMEDOUT is a normal error, continue */,
					/* on other error, */ break );
	
			/* on wakeup, loop */
			goto again;
		}
		
		/* Now, the first session in the list is expired; destroy it */
		pthread_cleanup_pop( 0 );
		CHECK_POSIX_DO( pthread_mutex_unlock(&exp_lock),  break );
		
		CHECK_FCT_DO( fd_sess_destroy( &first ), break );
		
	} while (1);
	
	TRACE_DEBUG(INFO, "A system error occurred in session module! Expiry thread is terminating...");
	ASSERT(0);
	return NULL;
}
	
	

/********************************************************************************************************/

/* Initialize the session module */
int fd_sess_init(void)
{
	int i;
	
	TRACE_ENTRY( "" );
	
	/* Initialize the global counters */
	sid_h = (uint32_t) time(NULL);
	sid_l = 0;
	
	/* Initialize the hash table */
	for (i = 0; i < sizeof(sess_hash) / sizeof(sess_hash[0]); i++) {
		fd_list_init( &sess_hash[i].sentinel, NULL );
		CHECK_POSIX(  pthread_mutex_init(&sess_hash[i].lock, NULL)  );
	}
	
	return 0;
}

/* Run this when initializations are complete. */
int fd_sess_start(void)
{
	/* Start session garbage collector (expiry) */
	CHECK_POSIX(  pthread_create(&exp_thr, NULL, exp_fct, NULL)  );
	
	return 0;
}

/* Terminate */
void fd_sess_fini(void)
{
	TRACE_ENTRY("");
	CHECK_FCT_DO( fd_thr_term(&exp_thr), /* continue */ );
	
	/* Destroy all sessions in the hash table, and the hash table itself? -- How to do it without a race condition ? */
	
	return;
}

/* Create a new handler */
int fd_sess_handler_create ( struct session_handler ** handler, void (*cleanup)(struct sess_state *, os0_t, void *), session_state_dump dumper, void * opaque )
{
	struct session_handler *new;
	
	TRACE_ENTRY("%p %p", handler, cleanup);
	
	CHECK_PARAMS( handler && cleanup );
	
	CHECK_MALLOC( new = malloc(sizeof(struct session_handler)) );
	memset(new, 0, sizeof(struct session_handler));
	
	CHECK_POSIX( pthread_mutex_lock(&hdl_lock) );
	new->id = ++hdl_id;
	CHECK_POSIX( pthread_mutex_unlock(&hdl_lock) );
	
	new->eyec = SH_EYEC;
	new->cleanup = cleanup;
	new->state_dump = dumper;
	new->opaque = opaque;
	
	*handler = new;
	return 0;
}

/* Destroy a handler, and all states attached to this handler. This operation is very slow but we don't care since it's rarely used. 
 * Note that it's better to call this function after all sessions have been deleted... */
int fd_sess_handler_destroy ( struct session_handler ** handler, void ** opaque )
{
	struct session_handler * del;
	/* place to save the list of states to be cleaned up. We do it after finding them to avoid deadlocks. the "o" field becomes a copy of the sid. */
	struct fd_list deleted_states = FD_LIST_INITIALIZER( deleted_states );
	int i;
	
	TRACE_ENTRY("%p", handler);
	CHECK_PARAMS( handler && VALIDATE_SH(*handler) );
	
	del = *handler;
	*handler = NULL;
	
	del->eyec = 0xdead; /* The handler is not valid anymore for any other operation */
	
	/* Now find all sessions with data registered for this handler, and move this data to the deleted_states list. */
	for (i = 0; i < sizeof(sess_hash) / sizeof(sess_hash[0]); i++) {
		struct fd_list * li_si;
		CHECK_POSIX(  pthread_mutex_lock(&sess_hash[i].lock)  );
		
		for (li_si = sess_hash[i].sentinel.next; li_si != &sess_hash[i].sentinel; li_si = li_si->next) { /* for each session in the hash line */
			struct fd_list * li_st;
			struct session * sess = (struct session *)(li_si->o);
			CHECK_POSIX(  pthread_mutex_lock(&sess->stlock)  );
			for (li_st = sess->states.next; li_st != &sess->states; li_st = li_st->next) { /* for each state in this session */
				struct state * st = (struct state *)(li_st->o);
				/* The list is ordered */
				if (st->hdl->id < del->id)
					continue;
				if (st->hdl->id == del->id) {
					/* This state belongs to the handler we are deleting, move the item to the deleted_states list */
					fd_list_unlink(&st->chain);
					st->sid = sess->sid;
					fd_list_insert_before(&deleted_states, &st->chain);
				}
				break;
			}
			CHECK_POSIX(  pthread_mutex_unlock(&sess->stlock)  );
		}
		CHECK_POSIX(  pthread_mutex_unlock(&sess_hash[i].lock)  );
	}
	
	/* Now, delete all states after calling their cleanup handler */
	while (!FD_IS_LIST_EMPTY(&deleted_states)) {
		struct state * st = (struct state *)(deleted_states.next->o);
		TRACE_DEBUG(FULL, "Calling cleanup handler for session '%s' and data %p", st->sid, st->state);
		(*del->cleanup)(st->state, st->sid, del->opaque);
		fd_list_unlink(&st->chain);
		free(st);
	}
	
	if (opaque)
		*opaque = del->opaque;
	
	/* Free the handler */
	free(del);
	
	return 0;
}



/* Create a new session object with the default timeout value, and link it. The refcount is increased by 1, whether the session existed or not */
int fd_sess_new ( struct session ** session, DiamId_t diamid, size_t diamidlen, uint8_t * opt, size_t optlen )
{
	os0_t  sid = NULL;
	size_t sidlen;
	uint32_t hash;
	struct session * sess;
	struct fd_list * li;
	int found = 0;
	int ret = 0;
	
	TRACE_ENTRY("%p %p %zd %p %zd", session, diamid, diamidlen, opt, optlen);
	CHECK_PARAMS( session && (diamid || opt) );

	if (diamid) {	
		if (!diamidlen) {
			diamidlen = strlen(diamid);
		} 
		/* We check if the string is a valid DiameterIdentity */
		CHECK_PARAMS( fd_os_is_valid_DiameterIdentity((uint8_t *)diamid, diamidlen) );
	} else {
		diamidlen = 0;
	}
	if (opt) {	
		if (!optlen) {
			optlen = strlen((char *)opt);
		} else {
			CHECK_PARAMS( fd_os_is_valid_os0(opt, optlen) );
		}
	} else {
		optlen = 0;
	}
		
	/* Ok, first create the identifier for the string */
	if (diamid == NULL) {
		/* opt is the full string */
		CHECK_MALLOC( sid = os0dup(opt, optlen) );
		sidlen = optlen;
	} else {
		uint32_t sid_h_cpy;
		uint32_t sid_l_cpy;
		/* "<diamId>;<high32>;<low32>[;opt]" */
		sidlen = diamidlen;
		sidlen += 22; /* max size of ';<high32>;<low32>' */
		if (opt)
			sidlen += 1 + optlen; /* ';opt' */
		sidlen++; /* space for the final \0 also */
		CHECK_MALLOC( sid = malloc(sidlen) );
		
		CHECK_POSIX( pthread_mutex_lock(&sid_lock) );
		if ( ++sid_l == 0 ) /* overflow */
			++sid_h;
		sid_h_cpy = sid_h;
		sid_l_cpy = sid_l;
		CHECK_POSIX( pthread_mutex_unlock(&sid_lock) );
		
		if (opt) {
			sidlen = snprintf((char*)sid, sidlen, "%.*s;%u;%u;%.*s", (int)diamidlen, diamid, sid_h_cpy, sid_l_cpy, (int)optlen, opt);
		} else {
			sidlen = snprintf((char*)sid, sidlen, "%.*s;%u;%u", (int)diamidlen, diamid, sid_h_cpy, sid_l_cpy);
		}
	}
	
	hash = fd_os_hash(sid, sidlen);
	
	/* Now find the place to add this object in the hash table. */
	CHECK_POSIX( pthread_mutex_lock( H_LOCK(hash) ) );
	pthread_cleanup_push( fd_cleanup_mutex, H_LOCK(hash) );
	
	for (li = H_LIST(hash)->next; li != H_LIST(hash); li = li->next) {
		int cmp;
		struct session * s = (struct session *)(li->o);
		
		/* The list is ordered by hash and sid (in case of collisions) */
		if (s->hash < hash)
			continue;
		if (s->hash > hash)
			break;
		
		cmp = fd_os_cmp(s->sid, s->sidlen, sid, sidlen);
		if (cmp < 0)
			continue;
		if (cmp > 0)
			break;
		
		/* A session with the same sid was already in the hash table */
		found = 1;
		*session = s;
		break;
	}
	
	/* If the session did not exist, we can create it & link it in global tables */
	if (!found) {
		CHECK_MALLOC_DO(sess = new_session(sid, sidlen, hash),
			{
				ret = ENOMEM;
				free(sid);
				goto out;
			} );
	
		fd_list_insert_before(li, &sess->chain_h); /* hash table */
		sess->msg_cnt++;
	} else {
		free(sid);
		
		CHECK_POSIX( pthread_mutex_lock(&(*session)->stlock) ); 
		(*session)->msg_cnt++;
		CHECK_POSIX( pthread_mutex_unlock(&(*session)->stlock) ); 
		
		/* it was found: was it previously destroyed? */
		if ((*session)->is_destroyed == 0) {
			ret = EALREADY;
			goto out;
		} else {
			/* the session was marked destroyed, let's re-activate it. */
			sess = *session;
			sess->is_destroyed = 0;
			
			/* update the expiry time */
			CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &sess->timeout), { ASSERT(0); } );
			sess->timeout.tv_sec += SESS_DEFAULT_LIFETIME;
		}
	}
		
	/* We must insert in the expiry list */
	CHECK_POSIX( pthread_mutex_lock( &exp_lock ) );
	pthread_cleanup_push( fd_cleanup_mutex, &exp_lock );

	/* Find the position in that list. We take it in reverse order */
	for (li = exp_sentinel.prev; li != &exp_sentinel; li = li->prev) {
		struct session * s = (struct session *)(li->o);
		if (TS_IS_INFERIOR( &s->timeout, &sess->timeout ) )
			break;
	}
	fd_list_insert_after( li, &sess->expire );
	sess_cnt++;

	/* We added a new expiring element, we must signal */
	if (li == &exp_sentinel) {
		CHECK_POSIX_DO( pthread_cond_signal(&exp_cond), { ASSERT(0); } ); /* if it fails, we might not pop the cleanup handlers, but this should not happen -- and we'd have a serious problem otherwise */
	}

	/* We're done with the locked part */
	pthread_cleanup_pop(0);
	CHECK_POSIX_DO( pthread_mutex_unlock( &exp_lock ), { ASSERT(0); } ); /* if it fails, we might not pop the cleanup handler, but this should not happen -- and we'd have a serious problem otherwise */

out:
	;
	pthread_cleanup_pop(0);
	CHECK_POSIX( pthread_mutex_unlock( H_LOCK(hash) ) );
	
	if (ret) /* in case of error */
		return ret;
	
	*session = sess;
	return 0;
}

/* Find or create a session -- the msg refcount is increased */
int fd_sess_fromsid_msg ( uint8_t * sid, size_t len, struct session ** session, int * new)
{
	int ret;
	
	TRACE_ENTRY("%p %zd %p %p", sid, len, session, new);
	CHECK_PARAMS( sid && session );
	
	if (!fd_os_is_valid_os0(sid,len)) {
		TRACE_DEBUG(INFO, "Warning: a Session-Id value contains \\0 chars... (len:%zd, begin:'%.*s') => Debug messages may be truncated.", len, (int)len, sid);
	}
	
	/* All the work is done in sess_new */
	ret = fd_sess_new ( session, NULL, 0, sid, len );
	switch (ret) {
		case 0:
		case EALREADY:
			break;
		
		default:
			CHECK_FCT(ret);
	}
	
	if (new)
		*new = ret ? 0 : 1;
	
	return 0;
}

/* Get the sid of a session */
int fd_sess_getsid ( struct session * session, os0_t * sid, size_t * sidlen )
{
	TRACE_ENTRY("%p %p", session, sid);
	
	CHECK_PARAMS( VALIDATE_SI(session) && sid );
	
	*sid = session->sid;
	if (sidlen)
		*sidlen = session->sidlen;
	
	return 0;
}

/* Change the timeout value of a session */
int fd_sess_settimeout( struct session * session, const struct timespec * timeout )
{
	struct fd_list * li;
	
	TRACE_ENTRY("%p %p", session, timeout);
	CHECK_PARAMS( VALIDATE_SI(session) && timeout );
	
	/* Lock -- do we need to lock the hash table as well? I don't think so... */
	CHECK_POSIX( pthread_mutex_lock( &exp_lock ) );
	pthread_cleanup_push( fd_cleanup_mutex, &exp_lock );
	
	/* Update the timeout */
	fd_list_unlink(&session->expire);
	memcpy(&session->timeout, timeout, sizeof(struct timespec));
	
	/* Find the new position in expire list. We take it in normal order */
	for (li = exp_sentinel.next; li != &exp_sentinel; li = li->next) {
		struct session * s = (struct session *)(li->o);

		if (TS_IS_INFERIOR( &s->timeout, &session->timeout ) )
			continue;

		break;
	}
	fd_list_insert_before( li, &session->expire );

	/* We added a new expiring element, we must signal if it was in first position */
	if (session->expire.prev == &exp_sentinel) {
		CHECK_POSIX_DO( pthread_cond_signal(&exp_cond), { ASSERT(0); /* so that we don't have a pending cancellation handler */ } );
	}

	/* We're done */
	pthread_cleanup_pop(0);
	CHECK_POSIX( pthread_mutex_unlock( &exp_lock ) );
	
	return 0;
}

/* Destroy the states associated to a session, and mark it destroyed. */
int fd_sess_destroy ( struct session ** session )
{
	struct session * sess;
	int destroy_now;
	os0_t sid;
	int ret = 0;
	
	/* place to save the list of states to be cleaned up. We do it after finding them to avoid deadlocks. the "o" field becomes a copy of the sid. */
	struct fd_list deleted_states = FD_LIST_INITIALIZER( deleted_states );
	
	TRACE_ENTRY("%p", session);
	CHECK_PARAMS( session && VALIDATE_SI(*session) );
	
	sess = *session;
	*session = NULL;
	
	/* Lock the hash line */
	CHECK_POSIX( pthread_mutex_lock( H_LOCK(sess->hash) ) );
	pthread_cleanup_push( fd_cleanup_mutex, H_LOCK(sess->hash) );
	
	/* Unlink from the expiry list */
	CHECK_POSIX_DO( pthread_mutex_lock( &exp_lock ), { ASSERT(0); /* otherwise cleanup handler is not pop'd */ } );
	if (!FD_IS_LIST_EMPTY(&sess->expire)) {
		sess_cnt--;
		fd_list_unlink( &sess->expire ); /* no need to signal the condition here */
	}
	CHECK_POSIX_DO( pthread_mutex_unlock( &exp_lock ), { ASSERT(0); /* otherwise cleanup handler is not pop'd */ } );
	
	/* Now move all states associated to this session into deleted_states */
	CHECK_POSIX_DO( pthread_mutex_lock( &sess->stlock ), { ASSERT(0); /* otherwise cleanup handler is not pop'd */ } );
	while (!FD_IS_LIST_EMPTY(&sess->states)) {
		struct state * st = (struct state *)(sess->states.next->o);
		fd_list_unlink(&st->chain);
		fd_list_insert_before(&deleted_states, &st->chain);
	}
	CHECK_POSIX_DO( pthread_mutex_unlock( &sess->stlock ), { ASSERT(0); /* otherwise cleanup handler is not pop'd */ } );
	
	/* Mark the session as destroyed */
	destroy_now = (sess->msg_cnt == 0);
	if (destroy_now) {
		fd_list_unlink( &sess->chain_h );
		sid = sess->sid;
	} else {
		sess->is_destroyed = 1;
		CHECK_MALLOC_DO( sid = os0dup(sess->sid, sess->sidlen), ret = ENOMEM );
	}
	pthread_cleanup_pop(0);
	CHECK_POSIX( pthread_mutex_unlock( H_LOCK(sess->hash) ) );
	
	if (ret)
		return ret;
	
	/* Now, really delete the states */
	while (!FD_IS_LIST_EMPTY(&deleted_states)) {
		struct state * st = (struct state *)(deleted_states.next->o);
		fd_list_unlink(&st->chain);
		TRACE_DEBUG(FULL, "Calling handler %p cleanup for state %p registered with session '%s'", st->hdl, st, sid);
		(*st->hdl->cleanup)(st->state, sid, st->hdl->opaque);
		free(st);
	}
	
	/* Finally, destroy the session itself, if it is not referrenced by any message anymore */
	if (destroy_now) {
		del_session(sess);
	} else {
		free(sid);
	}
	
	return 0;
}

/* Destroy a session if it is not used */
int fd_sess_reclaim ( struct session ** session )
{
	struct session * sess;
	uint32_t hash;
	int destroy_now = 0;
	
	TRACE_ENTRY("%p", session);
	CHECK_PARAMS( session && VALIDATE_SI(*session) );
	
	sess = *session;
	hash = sess->hash;
	*session = NULL;
	
	CHECK_POSIX( pthread_mutex_lock( H_LOCK(hash) ) );
	pthread_cleanup_push( fd_cleanup_mutex, H_LOCK(hash) );
	CHECK_POSIX_DO( pthread_mutex_lock( &sess->stlock ), { ASSERT(0); /* otherwise, cleanup not poped on FreeBSD */ } );
	pthread_cleanup_push( fd_cleanup_mutex, &sess->stlock );
	CHECK_POSIX_DO( pthread_mutex_lock( &exp_lock ), { ASSERT(0); /* otherwise, cleanup not poped on FreeBSD */ } );
	
	/* We only do something if the states list is empty */
	if (FD_IS_LIST_EMPTY(&sess->states)) {
		/* In this case, we do as in destroy */
		fd_list_unlink( &sess->expire );
		destroy_now = (sess->msg_cnt == 0);
		if (destroy_now) {
			fd_list_unlink(&sess->chain_h);
		} else {
			/* just mark it as destroyed, it will be freed when the last message stops referencing it */
			sess->is_destroyed = 1;
		}
	}
	
	CHECK_POSIX_DO( pthread_mutex_unlock( &exp_lock ), { ASSERT(0); /* otherwise, cleanup not poped on FreeBSD */ } );
	pthread_cleanup_pop(0);
	CHECK_POSIX_DO( pthread_mutex_unlock( &sess->stlock ), { ASSERT(0); /* otherwise, cleanup not poped on FreeBSD */ } );
	pthread_cleanup_pop(0);
	CHECK_POSIX( pthread_mutex_unlock( H_LOCK(hash) ) );
	
	if (destroy_now)
		del_session(sess);
	
	return 0;
}

/* Save a state information with a session */
int fd_sess_state_store ( struct session_handler * handler, struct session * session, struct sess_state ** state )
{
	struct state *new;
	struct fd_list * li;
	int already = 0;
	int ret = 0;
	
	TRACE_ENTRY("%p %p %p", handler, session, state);
	CHECK_PARAMS( handler && VALIDATE_SH(handler) && session && VALIDATE_SI(session) && (!session->is_destroyed) && state );
	
	/* Lock the session state list */
	CHECK_POSIX( pthread_mutex_lock(&session->stlock) );
	pthread_cleanup_push( fd_cleanup_mutex, &session->stlock );
			
	/* Create the new state object */
	CHECK_MALLOC_DO(new = malloc(sizeof(struct state)), { ret = ENOMEM; goto out; } );
	memset(new, 0, sizeof(struct state));
	
	new->eyec = SD_EYEC;
	new->state= *state;
	fd_list_init(&new->chain, new);
	new->hdl = handler;
	
	/* find place for this state in the list */
	for (li = session->states.next; li != &session->states; li = li->next) {
		struct state * st = (struct state *)(li->o);
		/* The list is ordered by handler's id */
		if (st->hdl->id < handler->id)
			continue;
		
		if (st->hdl->id == handler->id) {
			TRACE_DEBUG(INFO, "A state was already stored for session '%s' and handler '%p', at location %p", session->sid, st->hdl, st->state);
			already = EALREADY;
		}
		
		break;
	}
	
	if (!already) {
		fd_list_insert_before(li, &new->chain);
		*state = NULL;
	} else {
		free(new);
	}
out:
	;	
	pthread_cleanup_pop(0);
	CHECK_POSIX( pthread_mutex_unlock(&session->stlock) );
	
	return ret ?: already;
}

/* Get the data back */
int fd_sess_state_retrieve ( struct session_handler * handler, struct session * session, struct sess_state ** state )
{
	struct fd_list * li;
	struct state * st = NULL;
	
	TRACE_ENTRY("%p %p %p", handler, session, state);
	CHECK_PARAMS( handler && VALIDATE_SH(handler) && session && VALIDATE_SI(session) && state );
	
	*state = NULL;
	
	/* Lock the session state list */
	CHECK_POSIX( pthread_mutex_lock(&session->stlock) );
	pthread_cleanup_push( fd_cleanup_mutex, &session->stlock );
	
	/* find the state in the list */
	for (li = session->states.next; li != &session->states; li = li->next) {
		st = (struct state *)(li->o);
		
		/* The list is ordered by handler's id */
		if (st->hdl->id > handler->id)
			break;
	}
	
	/* If we found the state */
	if (st && (st->hdl == handler)) {
		fd_list_unlink(&st->chain);
		*state = st->state;
		free(st);
	}
	
	pthread_cleanup_pop(0);
	CHECK_POSIX( pthread_mutex_unlock(&session->stlock) );
	
	return 0;
}

/* For the messages module */
int fd_sess_fromsid ( uint8_t * sid, size_t len, struct session ** session, int * new)
{
	TRACE_ENTRY("%p %zd %p %p", sid, len, session, new);
	CHECK_PARAMS( sid && len && session );
	
	/* Get the session object */
	CHECK_FCT( fd_sess_fromsid_msg ( sid, len, session, new) );
	
	/* Decrease the refcount */
	CHECK_POSIX( pthread_mutex_lock(&(*session)->stlock) );
	(*session)->msg_cnt--; /* was increased in fd_sess_new */
	CHECK_POSIX( pthread_mutex_unlock(&(*session)->stlock) );
		
	/* Done */
	return 0;
}

int fd_sess_ref_msg ( struct session * session )
{
	TRACE_ENTRY("%p", session);
	CHECK_PARAMS( VALIDATE_SI(session) );

	/* Update the msg refcount */
	CHECK_POSIX( pthread_mutex_lock(&session->stlock) );
	session->msg_cnt++;
	CHECK_POSIX( pthread_mutex_unlock(&session->stlock) );
	
	return 0;
}

int fd_sess_reclaim_msg ( struct session ** session )
{
	int reclaim;
	uint32_t hash;
	
	TRACE_ENTRY("%p", session);
	CHECK_PARAMS( session && VALIDATE_SI(*session) );
	
	/* Lock the hash line to avoid possibility that session is freed while we are reclaiming */
	hash = (*session)->hash;
	CHECK_POSIX( pthread_mutex_lock( H_LOCK(hash)) );
	pthread_cleanup_push( fd_cleanup_mutex, H_LOCK(hash) ); 

	/* Update the msg refcount */
	CHECK_POSIX( pthread_mutex_lock(&(*session)->stlock) );
	reclaim = (*session)->msg_cnt;
	(*session)->msg_cnt = reclaim - 1;
	CHECK_POSIX( pthread_mutex_unlock(&(*session)->stlock) );
	
	/* Ok, now unlock the hash line */
	pthread_cleanup_pop( 0 );
	CHECK_POSIX( pthread_mutex_unlock( H_LOCK(hash) ) );
	
	/* and reclaim if no message references the session anymore */
	if (reclaim == 1) {
		CHECK_FCT(fd_sess_reclaim ( session ));
	} else {
		*session = NULL;
	}
	return 0;
}



/* Dump functions */
DECLARE_FD_DUMP_PROTOTYPE(fd_sess_dump, struct session * session, int with_states)
{
	FD_DUMP_HANDLE_OFFSET();
	
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "{session}(@%p): ", session), return NULL);
	
	if (!VALIDATE_SI(session)) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "INVALID/NULL"), return NULL);
	} else {
		char timebuf[30];
		struct tm tm;

		strftime(timebuf, sizeof(timebuf), "%D,%T", localtime_r( &session->timeout.tv_sec , &tm ));
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "'%s'(%zd) h:%x m:%d d:%d to:%s.%06ld",
							session->sid, session->sidlen, session->hash, session->msg_cnt, session->is_destroyed,
							timebuf, session->timeout.tv_nsec/1000), 
				 return NULL);
		
		if (with_states) {
			struct fd_list * li;
			CHECK_POSIX_DO( pthread_mutex_lock(&session->stlock), /* ignore */ );
			pthread_cleanup_push( fd_cleanup_mutex, &session->stlock );
			
			for (li = session->states.next; li != &session->states; li = li->next) {
				struct state * st = (struct state *)(li->o);
				CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "\n  {state i:%d}(@%p): ", st->hdl->id, st), return NULL);
				if (st->hdl->state_dump) {
					CHECK_MALLOC_DO( (*st->hdl->state_dump)( FD_DUMP_STD_PARAMS, st->state), 
							fd_dump_extend( FD_DUMP_STD_PARAMS, "[dumper error]"));
				} else {
					CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "<%p>", st->state), return NULL);
				}
			}
			
			pthread_cleanup_pop(0);
			CHECK_POSIX_DO( pthread_mutex_unlock(&session->stlock), /* ignore */ );
		}
	}
	
	return *buf;
}

DECLARE_FD_DUMP_PROTOTYPE(fd_sess_dump_hdl, struct session_handler * handler)
{
	FD_DUMP_HANDLE_OFFSET();
	
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "{sesshdl}(@%p): ", handler), return NULL);
	
	if (!VALIDATE_SH(handler)) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "INVALID/NULL"), return NULL);
	} else {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "i:%d cl:%p d:%p o:%p", handler->id, handler->cleanup, handler->state_dump, handler->opaque), return NULL);
	}
	return *buf;
}	

int fd_sess_getcount(uint32_t *cnt)
{
	CHECK_PARAMS(cnt);
	CHECK_POSIX( pthread_mutex_lock( &exp_lock ) );
	*cnt = sess_cnt;
	CHECK_POSIX( pthread_mutex_unlock( &exp_lock ) );
	return 0;
}
