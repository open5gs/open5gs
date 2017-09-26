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

#include "fdproto-internal.h"

/* The dispatch module in the library is quite simple: callbacks are saved in a global list
 * in no particular order. In addition, they are also linked from the dictionary objects they
 * refer to. */

/* Protection for the lists managed in this module. */
pthread_rwlock_t fd_disp_lock = PTHREAD_RWLOCK_INITIALIZER;

/* List of all registered handlers -- useful if we want to cleanup properly at some point... */
static struct fd_list all_handlers = FD_LIST_INITIALIZER( all_handlers );

/* List of handlers registered for DISP_HOW_ANY. Other handlers are stored in the dictionary */
static struct fd_list any_handlers = FD_LIST_INITIALIZER( any_handlers );

/* The structure to store a callback */
struct disp_hdl {
	int		 eyec;	/* Eye catcher, DISP_EYEC */
	struct fd_list	 all;	/* link in the all_handlers list */
	struct fd_list	 parent;/* link in dictionary cb_list or in any_handlers */
	enum disp_how	 how;	/* Copy of registration parameter */
	struct disp_when when;	/* Copy of registration parameter */
	int		(*cb)( struct msg **, struct avp *, struct session *, void *, enum disp_action *);	/* The callback itself */
	void            *opaque; /* opaque data passed back to the callback */
};

#define DISP_EYEC	0xD15241C1
#define VALIDATE_HDL( _hdl ) \
	( ( ( _hdl ) != NULL ) && ( ((struct disp_hdl *)( _hdl ))->eyec == DISP_EYEC ) )

/**************************************************************************************/

/* Call CBs from a given list (any_handlers if cb_list is NULL) -- must have locked fd_disp_lock before */
int fd_disp_call_cb_int( struct fd_list * cb_list, struct msg ** msg, struct avp *avp, struct session *sess, enum disp_action *action, 
			struct dict_object * obj_app, struct dict_object * obj_cmd, struct dict_object * obj_avp, struct dict_object * obj_enu,
			char ** drop_reason, struct msg ** drop_msg)
{
	struct fd_list * senti, *li;
	int r;
	TRACE_ENTRY("%p %p %p %p %p %p %p %p %p", cb_list, msg, avp, sess, action, obj_app, obj_cmd, obj_avp, obj_enu);
	CHECK_PARAMS(msg && action);
	
	senti = cb_list;
	if (!senti)
		senti = &any_handlers;
	
	for (li = senti->next; li != senti; li = li->next) {
		struct disp_hdl * hdl = (struct disp_hdl *)(li->o);
		
		TRACE_DEBUG(ANNOYING, "when: %p %p %p %p", hdl->when.app, hdl->when.command, hdl->when.avp, hdl->when.value);
		
		/* Check this handler matches this message / avp */
		if (hdl->when.app     && (hdl->when.app     != obj_app))
			continue;
		if (hdl->when.command && (hdl->when.command != obj_cmd))
			continue;
		if (hdl->when.avp     && (hdl->when.avp     != obj_avp))
			continue;
		if (hdl->when.value   && (hdl->when.value   != obj_enu))
			continue;
		
		/* We have a match, the cb must be called. */
		CHECK_FCT_DO( (r = (*hdl->cb)(msg, avp, sess, hdl->opaque, action)),
			{
				*drop_reason = "Internal error: a DISPATCH callback returned an error";
				*drop_msg = *msg;
				*msg = NULL;
			}
		 );
		
		if (*action != DISP_ACT_CONT)
			break;
		
		if ( *msg  == NULL )
			break;
	}
	
	/* We're done on this list */
	return 0;
}

/**************************************************************************************/

/* Create a new handler and link it */
int fd_disp_register ( int (*cb)( struct msg **, struct avp *, struct session *, void *, enum disp_action *), 
			enum disp_how how, struct disp_when * when, void * opaque, struct disp_hdl ** handle )
{
	struct fd_list * cb_list = NULL;
	struct disp_hdl * new;
	struct dict_object * type_enum = NULL, * type_avp;
	struct dictionary  * dict = NULL;
	
	TRACE_ENTRY("%p %d %p %p", cb, how, when, handle);
	CHECK_PARAMS( cb && ( (how == DISP_HOW_ANY) || when ));
	
	switch (how) {
		case DISP_HOW_ANY:
			cb_list = &any_handlers;
			break;
		
		case DISP_HOW_APPID:
			CHECK_FCT( fd_dict_disp_cb(DICT_APPLICATION, when->app, &cb_list) );
			break;
		
		case DISP_HOW_CC:
			CHECK_FCT( fd_dict_disp_cb(DICT_COMMAND, when->command, &cb_list) );
			break;
		
		case DISP_HOW_AVP_ENUMVAL:
			CHECK_FCT( fd_dict_disp_cb(DICT_ENUMVAL, when->value, &cb_list) ); /* cb_list is then overwritten */
			CHECK_FCT( fd_dict_getdict(when->value, &dict) );
			CHECK_FCT( fd_dict_search(dict, DICT_TYPE, TYPE_OF_ENUMVAL, when->value, &type_enum, EINVAL) );
		case DISP_HOW_AVP:
			CHECK_FCT( fd_dict_disp_cb(DICT_AVP, when->avp, &cb_list) );
			if (dict) {
				CHECK_FCT( fd_dict_search(dict, DICT_TYPE, TYPE_OF_AVP, when->avp, &type_avp, EINVAL) );
				if (type_enum) {
					CHECK_PARAMS( type_enum == type_avp );
				}
			}
			break;
		
		default:
			CHECK_PARAMS(how = 0);
	}
	/* We might further check optional fields, but we trust the caller ^^ */
	
	/* Create the new handler */
	CHECK_MALLOC( new = malloc( sizeof(struct disp_hdl) ) );
	memset(new, 0, sizeof(struct disp_hdl));
	new->eyec = DISP_EYEC;
	fd_list_init(&new->all, new);
	fd_list_init(&new->parent, new);
	new->how = how;
	switch (how) {
		case DISP_HOW_ANY:
			/* there is no "when" in that case */
			break;
		case DISP_HOW_AVP_ENUMVAL:
			new->when.value   = when->value;
		case DISP_HOW_AVP:
			new->when.avp     = when->avp;
		case DISP_HOW_CC:
			new->when.command = when->command;
		case DISP_HOW_APPID:
			new->when.app     = when->app;
	}
	new->cb = cb;
	new->opaque = opaque;
	
	/* Now, link this new element in the appropriate lists */
	CHECK_POSIX( pthread_rwlock_wrlock(&fd_disp_lock) );
	fd_list_insert_before(&all_handlers, &new->all);
	fd_list_insert_before(cb_list, &new->parent);
	CHECK_POSIX( pthread_rwlock_unlock(&fd_disp_lock) );
	
	/* We're done */
	if (handle)
		*handle = new;
	
	return 0;
}

/* Delete a handler */
int fd_disp_unregister ( struct disp_hdl ** handle, void ** opaque )
{
	struct disp_hdl * del;
	TRACE_ENTRY("%p", handle);
	CHECK_PARAMS( handle && VALIDATE_HDL(*handle) );
	del = *handle;
	*handle = NULL;
	
	CHECK_POSIX( pthread_rwlock_wrlock(&fd_disp_lock) );
	fd_list_unlink(&del->all);
	fd_list_unlink(&del->parent);
	CHECK_POSIX( pthread_rwlock_unlock(&fd_disp_lock) );
	
	if (opaque)
		*opaque = del->opaque;
	
	free(del);
	return 0;
}

/* Delete all handlers */
void fd_disp_unregister_all ( void )
{
	TRACE_ENTRY("");
	while (!FD_IS_LIST_EMPTY(&all_handlers)) {
		CHECK_FCT_DO( fd_disp_unregister((void *)&(all_handlers.next->o), NULL), /* continue */ );
	}
	return;
}
