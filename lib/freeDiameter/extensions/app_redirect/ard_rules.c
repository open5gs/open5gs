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

#include "app_redir.h"

static const char * redir_type_str[] = {
	"DONT_CACHE",
	"ALL_SESSION",
	"ALL_REALM",
	"REALM_AND_APPLICATION",
	"ALL_APPLICATION",
	"ALL_HOST",
	"ALL_USER"
};

struct dict_object * avp_Redirect_Host = NULL;
struct dict_object * avp_Redirect_Host_Usage = NULL;
struct dict_object * avp_Redirect_Max_Cache_Time = NULL;
	

void ard_rule_dump(struct ard_rule * r)
{
	struct fd_list * li;
	fd_log_debug("   rule @%p: %s, %us", r, redir_type_str[r->type], r->rct);
	for (li = r->criteria.next; li != &r->criteria; li = li->next) {
		struct ard_criteria * c = li->o;
		switch (c->type) {
			case FROM_ID:
				fd_log_debug("      Criteria: received from peer %s'%s'", c->is_regex?"REGEX":"", c->s);
				break;
			case FROM_REALM:
				fd_log_debug("      Criteria: received from realm %s'%s'", c->is_regex?"REGEX":"", c->s);
				break;
			case APP_ID:
				fd_log_debug("      Criteria: application id is %u", c->i);
				break;
			case AVP_INT:
				fd_log_debug("      Criteria: contains '%s' AVP with value '%d'", c->avp_info.avp_name, c->i);
				break;
			case AVP_STR:
				fd_log_debug("      Criteria: contains '%s' AVP with value %s'%s'", c->avp_info.avp_name, c->is_regex?"REGEX":"", c->s);
				break;
		
			default:
				fd_log_debug("      Criteria: invalid (%d)!", c->type);
		}
	}
	for (li = r->targets.next; li != &r->targets; li = li->next) {
		struct ard_target * t = li->o;
		fd_log_debug("      Redirect to: '%s'", t->s);
	}
}

/* Tells if the string in s (is0term or not) matches the string in the criteria (regex or not) */
static int str_match(struct ard_criteria * c, uint8_t *s, size_t l, int is0term, int * match)
{
	TRACE_ENTRY("%p %p %zd %d %p", c, s, l, is0term, match);
	
	*match = 0;
	
	if (c->is_regex == 0) {
		if ( ! fd_os_almostcasesrch(c->s, c->sl, s, l, NULL) )
			*match = 1;
	} else {
		int err;
#ifdef HAVE_REG_STARTEND
		regmatch_t pmatch[1];
		memset(pmatch, 0, sizeof(pmatch));
		pmatch[0].rm_so = 0;
		pmatch[0].rm_eo = l;
		err = regexec(&c->preg, (char *)s, 0, pmatch, REG_STARTEND);
#else /* HAVE_REG_STARTEND */
		if (!is0term) {
			/* We have to create a copy of the string in this case */
			char *mystrcpy;
			CHECK_MALLOC( mystrcpy = (char *)os0dup(s, l) );
			err = regexec(&c->preg, mystrcpy, 0, NULL, 0);
			free(mystrcpy);
		} else {
			err = regexec(&c->preg, (char *)s, 0, NULL, 0);
		}
#endif /* HAVE_REG_STARTEND */
		
		/* Now check the result */
		if (err == 0) {
			/* We have a match */
			*match = 1;
		} else if (err != REG_NOMATCH) {
			/* An error occurred */
			char * buf;
			size_t bl;

			/* Error while compiling the regex */
			TRACE_DEBUG(INFO, "Error while executing the regular expression '%s':", c->s);

			/* Get the error message size */
			bl = regerror(err, &c->preg, NULL, 0);

			/* Alloc the buffer for error message */
			CHECK_MALLOC( buf = malloc(bl) );

			/* Get the error message content */
			regerror(err, &c->preg, buf, bl);
			TRACE_DEBUG(INFO, "\t%s", buf);

			/* Free the buffer, return the error */
			free(buf);
			return (err == REG_ESPACE) ? ENOMEM : EINVAL;
		}
	}
	return 0;
}

/* Search the first matching rule in the config */
static int find_rule(struct msg * msg, struct ard_rule ** found)
{
	struct fd_list * li;
	struct msg_hdr * mhdr = NULL;
	struct peer_hdr * phdr = NULL;
	
	ASSERT(msg && found);
	*found = NULL;
	
	/* Get the message's header */
	CHECK_FCT( fd_msg_hdr(msg, &mhdr) );
	
	/* Get the message's origin */
	{
		DiamId_t id;
		size_t len;
		CHECK_FCT( fd_msg_source_get(msg, &id, &len) );
		CHECK_FCT( fd_peer_getbyid(id, len, 0, &phdr) );
	}
	
	/* Now for each rule check if all criteria match */
	for (li = ard_conf->rules.next; li != &ard_conf->rules; li = li->next) {
		struct fd_list * lic;
		struct ard_rule * r = li->o;
		int is_match = 1;
		
		for (lic = r->criteria.next; is_match && (lic != &r->criteria); lic = lic->next) {
			struct ard_criteria * c = lic->o;
			
			/* Does this criteria match ? */
			switch (c->type) {
				case APP_ID:
					if (c->i != mhdr->msg_appl)
						is_match = 0;
					break;
					
				case FROM_ID:
					CHECK_FCT( str_match(c, (uint8_t *)phdr->info.pi_diamid, phdr->info.pi_diamidlen, 1, &is_match) );
					break;
				
				case FROM_REALM:
					if (phdr->info.runtime.pir_realm) {
						CHECK_FCT( str_match(c, (uint8_t *)phdr->info.runtime.pir_realm, phdr->info.runtime.pir_realmlen, 1, &is_match) );
					} else {
						/* since we don't have the realm it was received from, assume it does not match */
						TRACE_DEBUG(INFO, "Missing realm info for peer '%s', skipping rule %p", phdr->info.pi_diamid, r);
						is_match = 0;
					}
					break;
				
				case AVP_INT:
				case AVP_STR:
					/* We have to search the whole message for the matching AVP */
					{
						is_match = 0;
						struct avp * avp = NULL;
						CHECK_FCT(  fd_msg_browse(msg, MSG_BRW_FIRST_CHILD, &avp, NULL)  );
						while (avp && !is_match) {
							struct avp_hdr * ahdr = NULL;
							CHECK_FCT( fd_msg_avp_hdr(avp, &ahdr) );

							if ( (ahdr->avp_code == c->avp_info.avp_code)
							  && (ahdr->avp_vendor == c->avp_info.avp_vendor) )  /* always 0 if no V flag */
							{
								/* dict-parse this AVP to ensure it has a value */
								CHECK_FCT( fd_msg_parse_dict( avp, fd_g_config->cnf_dict, NULL ) );

								/* Now check if the value matches our criteria */
								if (c->type == AVP_INT) {
									if (ahdr->avp_value->u32 == c->i)
										is_match = 1;
								} else {
									/* it is AVP_STR */
									CHECK_FCT( str_match(c, ahdr->avp_value->os.data, ahdr->avp_value->os.len, 0, &is_match) );
								}

								if (is_match)
									break;
							}

							/* go to next */
							CHECK_FCT( fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL) );
						}

					}
				
					break;
			
			}
		}
		
		if (is_match) {
			/* We found the first rule that matches for this message */
			*found = r;
			break;
		}
	}
	
	return 0;
}

/* The forward callback */
int ard_rule_apply(void * cbdata, struct msg ** msg)
{
	struct ard_rule * rule = NULL;
	
	TRACE_ENTRY("%p %p", cbdata, msg);
	CHECK_PARAMS(msg && *msg);
	
	/* First, check if we have a rule that applies to this message */
	CHECK_FCT( find_rule(*msg, &rule) );
	
	if (rule) {
		struct avp * avp;
		union avp_value val;
		struct fd_list * li;
		
		/* We have to reply a Redirect message in this case */
		CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, MSGFL_ANSW_ERROR) );
		
		CHECK_FCT( fd_msg_rescode_set( *msg, "DIAMETER_REDIRECT_INDICATION", NULL, NULL, 1 ) );
		
		/* Now add the Redirect-* AVPs */
		CHECK_FCT( fd_msg_avp_new( avp_Redirect_Host_Usage, 0, &avp ) );
		val.u32 = rule->type;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
		CHECK_FCT( fd_msg_avp_add( *msg, MSG_BRW_LAST_CHILD, avp ) );
		
		if (rule->type) {
			CHECK_FCT( fd_msg_avp_new( avp_Redirect_Max_Cache_Time, 0, &avp ) );
			val.u32 = rule->rct ?: ard_conf->default_rct;
			CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
			CHECK_FCT( fd_msg_avp_add( *msg, MSG_BRW_LAST_CHILD, avp ) );
		}
		
		for (li = rule->targets.next; li != &rule->targets; li = li->next) {
			struct ard_target * t = li->o;
			
			CHECK_FCT( fd_msg_avp_new( avp_Redirect_Host, 0, &avp ) );
			val.os.data = t->s;
			val.os.len  = t->l;
			CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
			CHECK_FCT( fd_msg_avp_add( *msg, MSG_BRW_LAST_CHILD, avp ) );
		}
		
		/* Send this answer */
		CHECK_FCT( fd_msg_send( msg, NULL, NULL) );
	}
	
	return 0;
}

