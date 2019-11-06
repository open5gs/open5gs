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

/* See rgwx_echodrop.h for details */

#include "rgwx_echodrop.h"

struct sess_state {
	struct fd_list sentinel;
};

/* If a session is destroyed, empty the list of ed_saved_attribute */
static void state_delete(struct sess_state * arg, os0_t sid, void * opaque) {
	while (!FD_IS_LIST_EMPTY(&arg->sentinel)) {
		struct ed_saved_attribute * esa = (struct ed_saved_attribute *)(arg->sentinel.next);
		fd_list_unlink(&esa->chain);
		free(esa);
	}
	free(arg);
}

static DECLARE_FD_DUMP_PROTOTYPE(ed_session_state_dump, struct sess_state * st)
{
	struct fd_list * li;
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "[rgwx sess_state](@%p):\n", st), return NULL);	
	for (li = st->sentinel.next; li != &st->sentinel; li = li->next) {
		struct ed_saved_attribute * esa = (struct ed_saved_attribute *)(li);
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "[rgwx sess_state {esa}] t:%2hhx l:%2hhx d:", esa->attr.type, esa->attr.length), return NULL);
		CHECK_MALLOC_DO( fd_dump_extend_hexdump(FD_DUMP_STD_PARAMS, (&esa->attr.length) + 1, esa->attr.length - 2, 0,0), return NULL);
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "\n"), return NULL);
	}
	return *buf;
}

/* Initialize the plugin and parse the configuration. */
static int ed_conf_parse(char * conffile, struct rgwp_config ** state)
{
	struct rgwp_config * new;
	
	TRACE_ENTRY("%p %p", conffile, state);
	CHECK_PARAMS( state );
	CHECK_PARAMS_DO( conffile, { fd_log_debug("[echodrop.rgwx] The configuration file is not optional for this plugin."); return EINVAL; } );
	
	CHECK_MALLOC( new = malloc(sizeof(struct rgwp_config)) );
	memset(new, 0, sizeof(struct rgwp_config));
	
	/* Initialize the list of attributes to handle */
	fd_list_init(&new->attributes, NULL);
	
	/* Create the session handler */
	CHECK_FCT( fd_sess_handler_create( &new->sess_hdl, state_delete, ed_session_state_dump, NULL ) );
	
	/* Parse the configuration file */
	CHECK_FCT( ed_conffile_parse(conffile, new) );
	
	if (TRACE_BOOL(FULL)) {
		TRACE_DEBUG(INFO, "Echo/Drop plugin configuration ('%s'):", conffile);
		struct fd_list * li;
		
		for (li = new->attributes.next; li != &new->attributes; li = li->next) {
			struct ed_conf_attribute * eca = (struct ed_conf_attribute *)li;
			char * act = (eca->action == ACT_ECHO) ? "ECHO" : "DROP";
			if (eca->ext) {
				fd_log_debug("  %s Code: %hhu, Vendor: %u, Ext-Type: %hu", act, eca->code, eca->vendor_id, eca->extype);
				continue;
			}
			if (eca->tlv) {
				fd_log_debug("  %s Code: %hhu, Vendor: %u, Type: %hhu", act, eca->code, eca->vendor_id, eca->type);
				continue;
			}
			if (eca->vsa) {
				fd_log_debug("  %s Code: %hhu, Vendor: %u", act, eca->code, eca->vendor_id);
				continue;
			}
			fd_log_debug("  %s Code: %hhu", act, eca->code);
		}
	}
	
	/* OK, we are done */
	*state = new;
	return 0;
}

/* Destroy the state */
static void ed_conf_free(struct rgwp_config * state)
{
	TRACE_ENTRY("%p", state);
	CHECK_PARAMS_DO( state, return );
	CHECK_FCT_DO( fd_sess_handler_destroy( &state->sess_hdl, NULL ),  );
	while (! FD_IS_LIST_EMPTY(&state->attributes) ) {
		struct fd_list * li = state->attributes.next;
		fd_list_unlink(li);
		free(li);
	}
	free(state);
	return;
}


/* Handle attributes from a RADIUS request as specified in the configuration */
static int ed_rad_req( struct rgwp_config * cs, struct radius_msg * rad_req, struct radius_msg ** rad_ans, struct msg ** diam_fw, struct rgw_client * cli )
{
	size_t nattr_used = 0;
	int idx;
	struct fd_list echo_list = FD_LIST_INITIALIZER(echo_list);
	struct fd_list *li;
	
	TRACE_ENTRY("%p %p %p %p %p", cs, rad_req, rad_ans, diam_fw, cli);
	CHECK_PARAMS(cs && rad_req);
	
	/* For each attribute in the original message */
	for (idx = 0; idx < rad_req->attr_used; idx++) {
		int action = 0;
		struct radius_attr_hdr * attr = (struct radius_attr_hdr *)(rad_req->buf + rad_req->attr_pos[idx]);
		
		/* Look if we have a matching attribute in our configuration */
		for (li = cs->attributes.next; li != &cs->attributes; li = li->next) {
			struct ed_conf_attribute * eca = (struct ed_conf_attribute *)li;
			uint32_t vid;
			unsigned char * ptr;
			
			if (eca->code < attr->type)
				continue;
			if (eca->code > attr->type)
				break;
			
			/* the code matches one in our configuration, check additional data if needed */
			
			if (! eca->vsa) {
				action = eca->action;
				break;
			}
			
			if (attr->length < 8)
				continue;
			
			ptr = (unsigned char *)(attr + 1);
			/* since attr is not aligned, we may not access *(attr+1) directly */
			memcpy(&vid, ptr, sizeof(uint32_t));
			
			if (eca->vendor_id < ntohl(vid))
				continue;
			if (eca->vendor_id > ntohl(vid))
				break;
			
			/* The vendor matches our configured line... */
			
			if ( ! eca->tlv && ! eca->ext ) {
				action = eca->action;
				break;
			}
			
			if (attr->length < 10)
				continue;
			
			if (eca->tlv) {
				struct radius_attr_vendor * tl = (struct radius_attr_vendor *)(ptr + sizeof(uint32_t));
				if (tl->vendor_type == eca->type) {
					action = eca->action;
					break;
				}
				continue;
			}
			
			if (eca->ext) {
				/* To be done */
				fd_log_debug("Extended attributes are not implemented yet!");
				ASSERT(0);
				continue;
			}
		}
		
		switch (action) {
			case ACT_DROP:
				TRACE_DEBUG(FULL, "Dropping attribute with code %hhd", attr->type);
				break;
				
			case ACT_ECHO:
				{
					struct ed_saved_attribute * esa = NULL;
					TRACE_DEBUG(FULL, "Saving attribute with code %hhd", attr->type);
					CHECK_MALLOC( esa = malloc(sizeof(struct ed_saved_attribute) + attr->length - sizeof(struct radius_attr_hdr)) );
					fd_list_init(&esa->chain, NULL);
					memcpy(&esa->attr, attr, attr->length);
					fd_list_insert_before(&echo_list, &esa->chain);
				}
				break;
			
			default: /* Attribute was not specified in the configuration */
				/* We just keep the attribute in the RADIUS message */
				rad_req->attr_pos[nattr_used++] = rad_req->attr_pos[idx];
		}
	}
	rad_req->attr_used = nattr_used;
	
	/* Save the echoed values in the session, if any */
	if (!FD_IS_LIST_EMPTY(&echo_list)) {
		struct session * sess;
		struct sess_state * st;
		
		CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, *diam_fw, &sess, NULL) );

		CHECK_PARAMS_DO(sess,
			{
				fd_log_debug(	"[echodrop.rgwx] The extension is configured to echo some attributes from this message, but no session object has been created for it (yet)."
						" Please check your configuration file and include a session-generating extension BEFORE calling echodrop.rgwx to echo attributes."
						" Please use debug.rgwx to retrieve more information." );
				return EINVAL;
			} );
		
		/* Move the values in a dynamically allocated list */
		CHECK_MALLOC( st = malloc(sizeof(struct sess_state)) );
		fd_list_init(&st->sentinel, NULL);
		fd_list_move_end(&st->sentinel, &echo_list);
		
		/* Save the list in the session */
		CHECK_FCT( fd_sess_state_store( cs->sess_hdl, sess, &st ) );
	}
	
	return 0;
}

/* Process an answer: add the ECHO attributes back, if any */
static int ed_diam_ans( struct rgwp_config * cs, struct msg ** diam_ans, struct radius_msg ** rad_fw, struct rgw_client * cli )
{
	struct session * sess;
	struct sess_state * st;
	
	TRACE_ENTRY("%p %p %p %p", cs, diam_ans, rad_fw, cli);
	CHECK_PARAMS(cs);
	
	CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, *diam_ans, &sess, NULL) );
	
	/* If there is no session associated, just give up */
	if (! sess ) {
		TRACE_DEBUG(FULL, "No session associated with the message, nothing to do here...");
		return 0;
	}
	
	/* Now try and retrieve any data from the session */
	CHECK_FCT( fd_sess_state_retrieve( cs->sess_hdl, sess, &st ) );
	if (st == NULL) {
		/* No attribute saved in the session, just return */
		return 0;
	}
	
	/* From this point on, we have a list of attributes to add to the radius message */
	
	CHECK_PARAMS( rad_fw && *rad_fw);
	
	while (! FD_IS_LIST_EMPTY(&st->sentinel) ) {
		struct ed_saved_attribute * esa = (struct ed_saved_attribute *)(st->sentinel.next);
		
		fd_list_unlink(&esa->chain);
		
		TRACE_DEBUG(FULL, "Echo attribute in the RADIUS answer: type %hhu, len: %hhu", esa->attr.type, esa->attr.length);
		
		/* Add this attribute in the RADIUS message */
		CHECK_MALLOC( radius_msg_add_attr(*rad_fw, esa->attr.type, (unsigned char *)(esa + 1), esa->attr.length - sizeof(struct radius_attr_hdr)) );
		
		free(esa);
	}
	free(st);

	return 0;
}



/* The exported symbol */
struct rgw_api rgwp_descriptor = {
	.rgwp_name       = "echo/drop",
	.rgwp_conf_parse = ed_conf_parse,
	.rgwp_conf_free  = ed_conf_free,
	.rgwp_rad_req    = ed_rad_req,
	.rgwp_diam_ans   = ed_diam_ans
};	
