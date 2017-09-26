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

/* Manage the list of plugins that provide handlers for RADIUS messages and attributes */

#include "rgw.h"
#include <dlfcn.h>
#include <libgen.h>

/* List of plugins, in the order they appear in the configuration file. */
static struct fd_list plg_list = FD_LIST_INITIALIZER(plg_list);

/* A plugin entry */
struct plg_descr {
	struct fd_list		 chain; 	/* chaining in plg_list */
	
	void 			*dlo;		/* pointer returned by dlopen for the extension, to use with dlclose later */
	struct rgw_api		*descriptor;	/* Points to the resolved plugin's rgwp_descriptor */
	struct rgwp_config 	*cs;		/* the (private) state returned by rgwp_conf_parse */
	
	int			 type;		/* this extension is called for messages received on this(these) server port(s) only */
	unsigned char 		*cc;		/* array of command codes, or NULL for all cc */
	size_t			 cc_len; 	/* size of the previous array */
};

/* Accelerators for each command code (one for each port). These accelerators are built on-demand, as a cache, after start_cache function has been called.  */
static struct fd_list plg_accel_auth = FD_LIST_INITIALIZER(plg_accel_auth);
static struct fd_list plg_accel_acct = FD_LIST_INITIALIZER(plg_accel_acct);

/* Accelerator list item, one per command code value (only the ones actually used) */
struct plg_accel {
	struct fd_list		chain;	/* link in the plg_accel_* list. List ordered by ccode. */
	unsigned char		ccode;	/* the command code for this accelerator. We don't handle extended CC yet */
	struct fd_list		plugins;/* head for the list of plg_accel_item, corresponding to the extensions to be called for this command code. */
};

/* Accelerator item */
struct plg_accel_item {
	struct fd_list		chain; 	/* link in the plg_accel "plugins" list */
	struct plg_descr *	plg;	/* pointer to the plugin */
	/* Note: we can further optimize by caching the location of plg->descriptor->rgwp_rad_req etc... at this level. */
};

/* RWlock to protect all the previous lists */
static pthread_rwlock_t plg_lock = PTHREAD_RWLOCK_INITIALIZER;

/* Has start_cache been called? */
static int cache_started = 0;


/* The read lock must be held before calling this function */
static int get_accelerator(struct fd_list ** list, unsigned char ccode, int type)
{
	struct fd_list *refer, *search;
	struct plg_accel * accel = NULL;
	struct plg_accel_item * item = NULL;
	int upgraded = 0;
	
	TRACE_ENTRY("%p %hhu %i", list, ccode, type);
	
	CHECK_PARAMS( cache_started && list && ((type == RGW_PLG_TYPE_AUTH) || (type == RGW_PLG_TYPE_ACCT)) );
	
	if (type == RGW_PLG_TYPE_AUTH)
		refer = &plg_accel_auth;
	else
		refer = &plg_accel_acct;
restart:	
	/* Check if we have already an accelerator for this ccode */
	for (search = refer->next; search != refer; search = search->next) {
		struct plg_accel * loc = (struct plg_accel *)search;
		
		if (loc->ccode < ccode)
			continue;
		
		if (loc->ccode > ccode)
			break; /* we don't have an accelerator for this value yet */
		
		/* We found the matching accelerator, just return this list */
		*list = &loc->plugins;
		return 0;
	}
	
	/* We must create the accelerator list, then save it just before "search" */
	
	/* First , upgrade the lock to write lock, and restart the search. This is the only robust solution to avoid deadlocks */
	if (! upgraded) {
		CHECK_POSIX( pthread_rwlock_unlock(&plg_lock) );
		CHECK_POSIX( pthread_rwlock_wrlock(&plg_lock) );
		upgraded = 1;
		goto restart;
	}
	
	/* Now create the new element */
	CHECK_MALLOC( accel = malloc(sizeof(struct plg_accel)) );
	memset(accel, 0, sizeof(struct plg_accel) );
	fd_list_init(&accel->chain, NULL);
	fd_list_init(&accel->plugins, accel);
	accel->ccode = ccode;
	
	/* Check each extension from the global list for this port and ccode */
	for (refer = plg_list.next; refer != &plg_list; refer = refer->next) {
		struct plg_descr * loc = (struct plg_descr *)refer;
		
		/* Skip if this extension is not registered for this port */
		if (! (loc->type & type) )
			continue;
		
		/* Check if the ccode is there */
		if (loc->cc) {
			int i;
			int match = 0;
			for (i=0; i< loc->cc_len; i++) {
				if (loc->cc[i] < ccode)
					continue;
				if (loc->cc[i] == ccode)
					match = 1;
				break;
			}
			if (!match)
				continue;
		}
		
		/* Ok, this extension must be called for this port / ccode, add to the accelerator */
		CHECK_MALLOC( item = malloc(sizeof(struct plg_accel_item)) );
		memset(item, 0, sizeof(struct plg_accel_item));
		fd_list_init(&item->chain, NULL);
		item->plg = loc;
		/* Add as last element of the accelerator */
		fd_list_insert_before(&accel->plugins, &item->chain);
	}
	
	/* Now, save this accelerator entry in the global list */
	fd_list_insert_before(search, &accel->chain);
	*list = &accel->plugins;
	
	return 0;
}


int rgw_plg_add( char * plgfile, char * conffile, int type, unsigned char ** codes_array, size_t codes_sz )
{
	struct plg_descr * new;
	
	TRACE_ENTRY("%p %p %d %p %zi", plgfile, conffile, type, codes_array, codes_sz);
	
	CHECK_PARAMS( plgfile && type && codes_array && (cache_started == 0) );
	
	CHECK_MALLOC( new = malloc(sizeof(struct plg_descr)) );
	memset(new, 0, sizeof(struct plg_descr));
	
	fd_list_init(&new->chain, new);
	
	/* Try and load the plugin */
	TRACE_DEBUG(FULL, "Loading plugin: %s", plgfile);
	new->dlo = dlopen(plgfile, RTLD_NOW | RTLD_GLOBAL);
	if (new->dlo == NULL) {
		/* An error occured */
		fd_log_debug("Loading of plugin '%s' failed: %s", plgfile, dlerror());
		goto error;
	}
	
	/* Resolve the descriptor */
	new->descriptor = dlsym( new->dlo, "rgwp_descriptor" );
	if (new->descriptor == NULL) {
		/* An error occured */
		fd_log_debug("Unable to resolve 'rgwp_descriptor' in plugin '%s': %s", plgfile, dlerror());
		goto error;
	}
	
	TRACE_DEBUG(FULL, "Plugin '%s' found in file '%s'", new->descriptor->rgwp_name, plgfile);
	
	/* Now parse the configuration file, this will initialize all plugin states and store it in the "cs" pointer (the plugin must be re-entrant, so no global state) */
	if (new->descriptor->rgwp_conf_parse) {
		CHECK_FCT_DO( (*(new->descriptor->rgwp_conf_parse))(conffile, &new->cs), 
			{
				fd_log_debug("An error occurred while parsing configuration file '%s' in plugin '%s', aborting...", conffile, plgfile);
				goto error;
			} );
	}
	
	/* Now sort the array (very simple algorithm, but this list is usually small) of command codes and save */
	if (*codes_array && codes_sz) {
		int i;
		
		new->cc = *codes_array;
		*codes_array = NULL;
		
		for (i = 0; i < codes_sz - 1; i++) {
			int j, idx = i, min = new->cc[i];
			
			/* find the smallest remaining element */
			for (j = i + 1; j < codes_sz; j++) {
				if (min > new->cc[j]) {
					min = new->cc[j];
					idx = j;
				}
			}
			
			/* swap if needed */
			if (idx != i) {
				int tmp = new->cc[i];
				new->cc[i] = new->cc[idx];
				new->cc[idx] = tmp;
			}
		}
		new->cc_len = codes_sz;
	}
		
	new->type = type;
	
	/* And save this new extension in the list. We don't need to lock at this point because we are single threaded. */
	fd_list_insert_before(&plg_list, &new->chain);
	
	return 0;
	
	
error:
	if (new && new->dlo)
		dlclose(new->dlo);
	if (new)
		free(new);	
	return EINVAL;
}

void rgw_plg_dump(void)
{
	struct plg_descr * plg;
	struct fd_list * ptr, *ptraccel;
	
	if ( ! TRACE_BOOL(FULL) )
		return;
	
	CHECK_POSIX_DO( pthread_rwlock_rdlock(&plg_lock), );
	
	if ( ! FD_IS_LIST_EMPTY( &plg_list ) )
		fd_log_debug("[app_radgw]  --- List of registered plugins:");
	for (ptr = plg_list.next; ptr != &plg_list; ptr = ptr->next) {
		char buf[1024];
		plg = (struct plg_descr *)ptr;
		
		snprintf(buf, sizeof(buf), "  %-25s ( %p ) - types: %s%s, codes: ", 
			 plg->descriptor->rgwp_name, 
			 plg->cs,
			 plg->type & RGW_PLG_TYPE_AUTH ? "Au" : "  ",
			 plg->type & RGW_PLG_TYPE_ACCT ? "Ac" : "  ");
		
		if (plg->cc) {
			int i;
			
			for (i = 0; i < plg->cc_len; i++) {
				snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), "%02hhx ", plg->cc[i]);
			}
			fd_log_debug("%s", buf);
		} else {
			fd_log_debug("%s*", buf);
		}
	}
	
	CHECK_POSIX_DO( pthread_rwlock_unlock(&plg_lock), );
	
	/* Dump the list of accelerators */
	if ( ! TRACE_BOOL(FULL + 1) )
		return;
	
	CHECK_POSIX_DO( pthread_rwlock_rdlock(&plg_lock), );
	if ( !FD_IS_LIST_EMPTY( &plg_accel_auth ) || !FD_IS_LIST_EMPTY( &plg_accel_acct ))
		fd_log_debug("  --- Accelerators:");
	
	for (ptraccel = plg_accel_auth.next; ptraccel != &plg_accel_auth; ptraccel = ptraccel->next) {
		struct plg_accel * accel = (struct plg_accel *)ptraccel;
		fd_log_debug("  auth, code %02hhu:", accel->ccode);

		for (ptr = accel->plugins.next; ptr != &accel->plugins; ptr = ptr->next) {
			struct plg_accel_item * item = (struct plg_accel_item *)ptr;
			fd_log_debug("     %-15s (%p)", item->plg->descriptor->rgwp_name, item->plg->cs);
		}
	}
	for (ptraccel = plg_accel_acct.next; ptraccel != &plg_accel_acct; ptraccel = ptraccel->next) {
		struct plg_accel * accel = (struct plg_accel *)ptraccel;
		fd_log_debug("  acct, code %02hhu:", accel->ccode);

		for (ptr = accel->plugins.next; ptr != &accel->plugins; ptr = ptr->next) {
			struct plg_accel_item * item = (struct plg_accel_item *)ptr;
			fd_log_debug("     %-15s (%p)", item->plg->descriptor->rgwp_name, item->plg->cs);
		}
	}
	
	
	CHECK_POSIX_DO( pthread_rwlock_unlock(&plg_lock), );
	
}

void rgw_plg_start_cache(void)
{
	cache_started++;
}

int rgw_plg_loop_req(struct rgw_radius_msg_meta **rad, struct msg **diam_msg, struct rgw_client * cli)
{
	int ret = 0;
	struct fd_list * head = NULL, *li;
	struct radius_msg * rad_ans = NULL;
	
	TRACE_ENTRY("%p %p %p", rad, diam_msg, cli);
	CHECK_PARAMS( rad && *rad && diam_msg && *diam_msg && cli);
	
	/* First, get the list of extensions for this message */
	CHECK_POSIX( pthread_rwlock_rdlock( &plg_lock) );
	CHECK_FCT_DO( ret = get_accelerator(&head, (*rad)->radius.hdr->code, (*rad)->serv_type), 
		{ CHECK_POSIX( pthread_rwlock_unlock( &plg_lock) ); return ret; } );
	
	/* Loop in the list of extensions */
	for (li = head->next; li != head; li = li->next) {
		struct plg_descr * plg = ((struct plg_accel_item *) li)->plg;
		
		if (plg->descriptor->rgwp_rad_req) {
			TRACE_DEBUG(ANNOYING, "Calling next plugin: %s", plg->descriptor->rgwp_name);
			ret = (*plg->descriptor->rgwp_rad_req)(plg->cs, &(*rad)->radius, &rad_ans, diam_msg, cli);
			if (ret)
				break;
		} else {
			TRACE_DEBUG(ANNOYING, "Skipping extension '%s' (NULL callback)", plg->descriptor->rgwp_name);
		}					
	}
	
	CHECK_POSIX( pthread_rwlock_unlock( &plg_lock) );
	
	/* If no error encountered, we're done here */
	if (ret == 0)
		return 0;
	
	/* Destroy the Diameter temp message, if any */
	if (*diam_msg) {
		CHECK_FCT_DO( fd_msg_free(*diam_msg), );
		*diam_msg = NULL;
	}
	
	/* Send the radius message back if required */
	if ((ret == -2) && rad_ans && rad) {
		CHECK_FCT_DO( rgw_client_finish_send(&rad_ans, *rad, cli), /* It failed, it can't be helped... */);
	}
	
	if (ret > 0) {
		/* Critical error, log and exit */
		TRACE_DEBUG(NONE, "An error occurred while handling a RADIUS message from '%s': %s", rgw_clients_id(cli), strerror(ret));
		return ret;
	}
	
	/* Now, discard the message and return */
	rgw_msg_free(rad);
	return 0;
}

/* Loop in the extension list (same as req) to convert data from diam_ans to rad_ans */
int rgw_plg_loop_ans(struct rgw_radius_msg_meta *req, struct msg **diam_ans, struct radius_msg ** rad_ans, struct rgw_client * cli)
{
	int ret = 0;
	struct fd_list * head = NULL, *li;
	
	TRACE_ENTRY("%p %p %p %p", req, diam_ans, rad_ans, cli);
	CHECK_PARAMS( req && diam_ans && *diam_ans && rad_ans && *rad_ans && cli);
	
	/* Get the list of extensions of the RADIUS request */
	CHECK_POSIX( pthread_rwlock_rdlock( &plg_lock) );
	CHECK_FCT_DO( ret = get_accelerator(&head, req->radius.hdr->code, req->serv_type), 
		{ CHECK_POSIX( pthread_rwlock_unlock( &plg_lock) ); return ret; } );
	
	/* Loop in the list of extensions */
	for (li = head->next; li != head; li = li->next) {
		struct plg_descr * plg = ((struct plg_accel_item *) li)->plg;
		
		if (plg->descriptor->rgwp_diam_ans) {
			TRACE_DEBUG(ANNOYING, "Calling next plugin: %s", plg->descriptor->rgwp_name);
			ret = (*plg->descriptor->rgwp_diam_ans)(plg->cs, diam_ans, rad_ans, (void *)cli);
			if (ret)
				break;
		} else {
			TRACE_DEBUG(ANNOYING, "Skipping extension '%s' (NULL callback)", plg->descriptor->rgwp_name);
		}					
	}
	
	CHECK_POSIX( pthread_rwlock_unlock( &plg_lock) );
	
	/* If no error encountered, we're done here */
	if (ret == 0)
		return 0;
	
	/* Destroy the temporary RADIUS answer */
	if (*rad_ans) {
		radius_msg_free(*rad_ans);
		free(*rad_ans);
		*rad_ans = NULL;
	}
	
	if (ret > 0) {
		/* Critical error, log and exit */
		fd_log_debug("[app_radgw] An error occurred while handling a DIAMETER answer to a converted RADIUS request, turn on DEBUG for details: %s", strerror(ret));
		return ret;
	}
	
	/* We might define other return values with special meaning here (ret == -1, ...) for example create a new Diameter request */
	
	/* -1: just abord the translation with no more processing. */
	
	return 0;
}

void rgw_plg_fini(void)
{
	struct fd_list * item, *subitem;
	
	TRACE_ENTRY();
	
	CHECK_POSIX_DO( pthread_rwlock_rdlock( &plg_lock), /* continue anyway */ );
	
	/* Remove all elements from all accelerators */
	while ( ! FD_IS_LIST_EMPTY(&plg_accel_auth) ) {
		item = plg_accel_auth.next;
		fd_list_unlink(item);
		{
			struct plg_accel * accel = (struct plg_accel *)item;
			while ( ! FD_IS_LIST_EMPTY(&accel->plugins) ) {
				subitem = accel->plugins.next;
				fd_list_unlink(subitem);
				free(subitem);
			}
		}
		free(item);
	}
	while ( ! FD_IS_LIST_EMPTY(&plg_accel_acct) ) {
		item = plg_accel_acct.next;
		fd_list_unlink(item);
		{
			struct plg_accel * accel = (struct plg_accel *)item;
			while ( ! FD_IS_LIST_EMPTY(&accel->plugins) ) {
				subitem = accel->plugins.next;
				fd_list_unlink(subitem);
				free(subitem);
			}
		}
		free(item);
	}
	
	/* Now destroy all plugins information */
	while ( ! FD_IS_LIST_EMPTY(&plg_list) ) {
		struct plg_descr * plg = (struct plg_descr *) plg_list.next;
		fd_list_unlink(&plg->chain);
		free(plg->cc);
		if (plg->descriptor && plg->descriptor->rgwp_conf_free ) {
			TRACE_DEBUG(INFO, "RADIUS/Diameter gateway plugin '%s' cleaning up...", plg->descriptor->rgwp_name);
			(*plg->descriptor->rgwp_conf_free)(plg->cs);
		}
		if (plg->dlo)
			dlclose(plg->dlo);
		free(plg);
	}
	
	CHECK_POSIX_DO( pthread_rwlock_unlock( &plg_lock), );
}
