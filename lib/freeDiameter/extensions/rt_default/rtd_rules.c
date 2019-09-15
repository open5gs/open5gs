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

#include "rt_default.h"

/* The regular expressions header */
#include <regex.h>

/* We will search for each candidate peer all the rules that are defined, and check which one applies to the message
 * Therefore our repository is organized hierarchicaly.
 *  At the top level, we have two lists of TARGETS (one for IDENTITY, one for REALM), ordered as follow:
 *   - first, the TARGETS defined with a regular expression. We will try matching all regexp to all candidates in the list.
 *   - then, the TARGETS defined by a plain text. We don't have to compare the whole list to each candidate since the list is ordered.
 *
 *  Under each TARGET element, we have the list of RULES that are defined for this target, ordered by CRITERIA type, then is_regex, then string value.
 *
 * Note: Except during configuration parsing and module termination, the lists are only ever accessed read-only, so we do not need a lock.
 */

/* Structure to hold the data that is used for matching. */
struct match_data {
	int	 is_regex;	/* determines how the string is matched */
	char 	*plain;		/* match this value with strcasecmp if is_regex is false. The string is allocated by the lex tokenizer, must be freed at the end. */
	regex_t  preg;		/* match with regexec if is_regex is true. regfree must be called at the end. A copy of the original string is anyway saved in plain. */
};

/* The sentinels for the TARGET lists */
static struct fd_list	TARGETS[RTD_TAR_MAX];

/* Structure of a TARGET element */
struct target {
	struct fd_list		chain;			/* link in the top-level list */
	struct match_data	md;			/* the data to determine if the current candidate matches this element */
	struct fd_list		rules[RTD_CRI_MAX];	/* Sentinels for the lists of rules applying to this target. One list per rtd_crit_type */
	/* note : we do not need the rtd_targ_type here, it is implied by the root of the list this target element is attached to */
};

/* Structure of a RULE element */
struct rule {
	struct fd_list		chain;	/* link in the parent target list */
	struct match_data	md;	/* the data that the criteria must match, -- ignored for RTD_CRI_ALL */
	int			score;	/* The score added to the candidate, if the message matches this criteria */
	/* The type of rule depends on the sentinel */
};

/*********************************************************************/

/* Compile a regular expression pattern */
static int compile_regex( regex_t  * preg, char * str )
{
	int err;
	
	/* Compile the regular expression */
	err = regcomp(preg, str, REG_EXTENDED | REG_NOSUB);
	if (err != 0) {
		char * buf;
		size_t bl;
		
		/* Error while compiling the regex */
		TRACE_DEBUG(INFO, "Error while compiling the regular expression '%s':", str);
		
		/* Get the error message size */
		bl = regerror(err, preg, NULL, 0);
		
		/* Alloc the buffer for error message */
		CHECK_MALLOC( buf = malloc(bl) );
		
		/* Get the error message content */
		regerror(err, preg, buf, bl);
		TRACE_DEBUG(INFO, "\t%s", buf);
		
		/* Free the buffer, return the error */
		free(buf);
		return EINVAL;
	}

	return 0;
}

/* Create a target item and initialize its content */
static struct target * new_target(char * str, int regex)
{
	int i;
	struct target *new = NULL;
	CHECK_MALLOC_DO( new = malloc(sizeof(struct target)), return NULL );
	memset(new, 0, sizeof(struct target));
	
	fd_list_init(&new->chain, new);
	new->md.plain = str;
	new->md.is_regex = regex;
	if (regex) {
		CHECK_FCT_DO( compile_regex(&new->md.preg, str),
			{
				free(new);
				return NULL;
			} );
	}
	for (i = 0; i < RTD_CRI_MAX; i++) {
		fd_list_init(&new->rules[i], new);
	}
	return new;
}

/* Create a rule item and initialize its content; return NULL in case of error */
static struct rule * new_rule(char * str, int regex, int score)
{
	struct rule *new = NULL;
	CHECK_MALLOC_DO( new = malloc(sizeof(struct rule)), return NULL );
	memset(new, 0, sizeof(struct rule));
	
	fd_list_init(&new->chain, new);
	new->md.plain = str;
	new->md.is_regex = regex;
	if (regex) {
		CHECK_FCT_DO( compile_regex(&new->md.preg, str),
			{
				free(new);
				return NULL;
			} );
	}
	new->score = score;
	return new;
}

/* Debug functions */
static void dump_rule(int indent, struct rule * rule)
{
	fd_log_debug("%*s%s%s%s += %d",
		indent, "",
		rule->md.is_regex ? "[" : "'",
		rule->md.plain,
		rule->md.is_regex ? "]" : "'",
		rule->score);
}
static void dump_target(int indent, struct target * target)
{
	int i;
	fd_log_debug("%*s%s%s%s :",
		indent, "",
		target->md.is_regex ? "[" : "'",
		target->md.plain ?: "(empty)",
		target->md.is_regex ? "]" : "'");
	for (i = 0; i < RTD_CRI_MAX; i++) {
		if (! FD_IS_LIST_EMPTY(&target->rules[i])) {
			struct fd_list * li;
			fd_log_debug("%*s  rules[%d]:",
				indent, "", i);
			for (li = target->rules[i].next; li != &target->rules[i]; li = li->next) {
				dump_rule(indent + 3, (struct rule *)li);
			}
		}
	}
}

static void clear_md(struct match_data * md)
{
	/* delete the string */
	if (md->plain) {
		free(md->plain);
		md->plain = NULL;
	}
	
	/* delete the preg if needed */
	if (md->is_regex) {
		regfree(&md->preg);
		md->is_regex = 0;
	}
}

/* Destroy a rule item */
static void del_rule(struct rule * del)
{
	/* Unlink this rule */
	fd_list_unlink(&del->chain);
	
	/* Delete the match data */
	clear_md(&del->md);
	
	free(del);
}

/* Destroy a target item, and all its rules */
static void del_target(struct target * del)
{
	int i;
	
	/* Unlink this target */
	fd_list_unlink(&del->chain);
	
	/* Delete the match data */
	clear_md(&del->md);
	
	/* Delete the children rules */
	for (i = 0; i < RTD_CRI_MAX; i++) {
		while (! FD_IS_LIST_EMPTY(&del->rules[i]) ) {
			del_rule((struct rule *)(del->rules[i].next));
		}
	}
	
	free(del);
}


/* Compare a string with a match_data value. *res contains the result of the comparison (always >0 for regex non-match situations) */
static int compare_match(char * str, size_t len, struct match_data * md, int * res)
{
	int err;
	
	CHECK_PARAMS( str && md && res );
	
	/* Plain strings: we compare with strncasecmp */
	if (md->is_regex == 0) {
		*res = strncasecmp(str, md->plain, len);
		return 0;
	}

	/* Regexp */
	*res = 1;
	
#ifdef HAVE_REG_STARTEND
	{
		regmatch_t pmatch[1];
		memset(pmatch, 0, sizeof(pmatch));
		pmatch[0].rm_so = 0;
		pmatch[0].rm_eo = len;
		err = regexec(&md->preg, str, 0, pmatch, REG_STARTEND);
	}
#else /* HAVE_REG_STARTEND */
	{
		/* We have to create a copy of the string in this case */
		char *mystrcpy;
		CHECK_MALLOC( mystrcpy = os0dup(str, len) );
		err = regexec(&md->preg, mystrcpy, 0, NULL, 0);
		free(mystrcpy);
	}
#endif /* HAVE_REG_STARTEND */
	
	/* Now check the result */
	if (err == 0) {
		/* We have a match */
		*res = 0;
		return 0;
	}
	
	if (err == REG_NOMATCH) {
		*res = 1;
		return 0;
	}
	
	/* In other cases, we have an error */
	{
		char * buf;
		size_t bl;
		
		/* Error while compiling the regex */
		TRACE_DEBUG(INFO, "Error while executing the regular expression '%s':", md->plain);
		
		/* Get the error message size */
		bl = regerror(err, &md->preg, NULL, 0);
		
		/* Alloc the buffer for error message */
		CHECK_MALLOC( buf = malloc(bl) );
		
		/* Get the error message content */
		regerror(err, &md->preg, buf, bl);
		TRACE_DEBUG(INFO, "\t%s", buf);
		
		/* Free the buffer, return the error */
		free(buf);
	}

	return (err == REG_ESPACE) ? ENOMEM : EINVAL;
}

/* Search in list (targets or rules) the next matching item for octet string str(len). Returned in next_match, or *next_match == NULL if no more match. Re-enter with same next_match for the next one. */
static int get_next_match(struct fd_list * list, char * str, size_t len, struct fd_list ** next_match)
{
	struct fd_list * li;
	
	TRACE_ENTRY("%p %p %zd %p", list, str, len, next_match);
	CHECK_PARAMS(list && str && len && next_match);
	
	if (*next_match)
		li = (*next_match)->next;
	else
		li = list->next;
	
	/* Initialize the return value */
	*next_match = NULL;
	
	for ( ; li != list; li = li->next) {
		int cmp;
		struct {
			struct fd_list 		chain;
			struct match_data 	md;
		} * next_item = (void *)li;
		
		/* Check if the string matches this next item */
		CHECK_FCT( compare_match(str, len, &next_item->md, &cmp) );
		
		if (cmp == 0) {
			/* matched! */
			*next_match = li;
			return 0;
		}
		
		if (cmp < 0) /* we can stop searching */
			break;
	}
	
	/* We're done with the list */
	return 0;
}

static struct dict_object * AVP_MODELS[RTD_CRI_MAX];

/*********************************************************************/

/* Prepare the module */
int rtd_init(void)
{
	int i;
	
	TRACE_ENTRY();
	
	for (i = 0; i < RTD_TAR_MAX; i++) {
		fd_list_init(&TARGETS[i], NULL);
	}
	
	for (i = 1; i < RTD_CRI_MAX; i++) {
		switch (i) {
			case RTD_CRI_OH:
				CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Host", &AVP_MODELS[i], ENOENT ));
				break;
			case RTD_CRI_OR:
				CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Realm", &AVP_MODELS[i], ENOENT ));
				break;
			case RTD_CRI_DH:
				CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Host", &AVP_MODELS[i], ENOENT ));
				break;
			case RTD_CRI_DR:
				CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Realm", &AVP_MODELS[i], ENOENT ));
				break;
			case RTD_CRI_UN:
				CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "User-Name", &AVP_MODELS[i], ENOENT ));
				break;
			case RTD_CRI_SI:
				CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Session-Id", &AVP_MODELS[i], ENOENT ));
				break;
			default:
				TRACE_DEBUG(INFO, "Missing definition in extension initializer");
				ASSERT( 0 );
				return EINVAL;
		}
	}
	
	return 0;
}

/* Destroy the module's data */
void rtd_fini(void)
{
	int i;
	
	TRACE_ENTRY();

	for (i = 0; i < RTD_TAR_MAX; i++) {
		while (!FD_IS_LIST_EMPTY(&TARGETS[i])) {
			del_target((struct target *) TARGETS[i].next);
		}
	}
	
}

/* Add a new rule in the repository. this is called when the configuration file is being parsed */
int rtd_add(enum rtd_crit_type ct, char * criteria, enum rtd_targ_type tt, char * target, int score, int flags)
{
	struct fd_list * target_suiv = NULL;
	struct fd_list * rule_suiv = NULL;
	struct target * trg = NULL;
	struct rule * rul = NULL;
	
	TRACE_ENTRY("%d %p %d %p %d %x", ct, criteria, tt, target, score, flags);
	CHECK_PARAMS((ct < RTD_CRI_MAX) && ((ct == RTD_CRI_ALL) || criteria) && (tt < RTD_TAR_MAX) && target);
	
	/* First, search in the TARGET list if we already have this target */
	for (target_suiv = TARGETS[tt].next; target_suiv != &TARGETS[tt]; target_suiv = target_suiv->next) {
		int cmp;
		struct target * cur = (struct target *)target_suiv;
		
		if (flags & RTD_TARG_REG) {
			/* We are adding a regexp, it is saved in the list before the plain expressions */
			if (cur->md.is_regex == 0)
				break;	
		} else {
			/* We are adding a plain expression, save it after all regexps */
			if (cur->md.is_regex != 0)
				continue;
		}
		
		/* At this point, the type is the same, so compare the plain string value */
		cmp = strcmp(cur->md.plain, target);
		if (cmp < 0)
			continue;
		
		if (cmp == 0) /* We already have a target with the same string */
			trg = cur;
		
		break;
	}
	
	if (trg) {
		/* Ok, we can free the target string, we will use the previously allocated one */
		free(target);
	} else {
		CHECK_MALLOC( trg = new_target(target, flags & RTD_TARG_REG) );
		fd_list_insert_before( target_suiv, &trg->chain );
	}
	
	/* Now, search for the rule position in this target's list */
	if (ct == RTD_CRI_ALL) {
		/* Special case: we don't have a criteria -- always create a rule element */
		CHECK_MALLOC( rul = new_rule(NULL, 0, score) );
		fd_list_insert_before( &trg->rules[RTD_CRI_ALL], &rul->chain );
	} else {
		for (rule_suiv = trg->rules[ct].next; rule_suiv != &trg->rules[ct]; rule_suiv = rule_suiv->next) {
			int cmp;
			struct rule * cur = (struct rule *)rule_suiv;

			if (flags & RTD_CRIT_REG) {
				/* We are adding a regexp, it is saved in the list before the plain expressions */
				if (cur->md.is_regex == 0)
					break;	
			} else {
				/* We are adding a plain expression, save it after all regexps */
				if (cur->md.is_regex != 0)
					continue;
			}

			/* At this point, the type is the same, so compare the plain string value */
			cmp = strcmp(cur->md.plain, criteria);
			if (cmp < 0)
				continue;

			if (cmp == 0) /* We already have a target with the same string */
				rul = cur;

			break;
		}
	
		if (rul) {
			/* Ok, we can free the target string, we will use the previously allocated one */
			free(criteria);
			TRACE_DEBUG(INFO, "Warning: duplicate rule (%s : %s) found, merging score...", rul->md.plain, trg->md.plain);
			rul->score += score;
		} else {
			CHECK_MALLOC( rul = new_rule(criteria, flags & RTD_CRIT_REG, score) );
			fd_list_insert_before( rule_suiv, &rul->chain );
		}
	}
	
	return 0;
}

/* Check if a message and list of eligible candidate match any of our rules, and update its score according to it. */
int rtd_process( struct msg * msg, struct fd_list * candidates )
{
	struct fd_list * li;
	struct {
		enum { NOT_RESOLVED_YET = 0, NOT_FOUND, FOUND } status;
		union avp_value * avp;
	} parsed_msg_avp[RTD_CRI_MAX];
	
	TRACE_ENTRY("%p %p", msg, candidates);
	CHECK_PARAMS(msg && candidates);
	
	/* We delay looking for the AVPs in the message until we really need them. Another approach would be to parse the message once and save all needed AVPs. */
	memset(parsed_msg_avp, 0, sizeof(parsed_msg_avp));
	
	/* For each candidate in the list */
	for (li = candidates->next; li != candidates; li = li->next) {
		struct rtd_candidate * cand = (struct rtd_candidate *)li;
		int i;
		struct {
			char * str;
			size_t len;
		} cand_data[RTD_TAR_MAX] = {
			{ cand->diamid,  strlen(cand->diamid) },
			{ cand->realm,   strlen(cand->realm)  }
		};
		
		for (i = 0; i < RTD_TAR_MAX; i++) {
			/* Search the next rule matching this candidate in the i-th target list */
			struct target * target = NULL;
			
			do {
				int j;
				struct fd_list * l;
				struct rule * r;
				
				CHECK_FCT ( get_next_match( &TARGETS[i], cand_data[i].str, cand_data[i].len, (void *)&target) );
				if (!target)
					break;
				
				/* First, apply all rules of criteria RTD_CRI_ALL */
				for ( l = target->rules[RTD_CRI_ALL].next; l != &target->rules[RTD_CRI_ALL]; l = l->next ) {
					r = (struct rule *)l;
					cand->score += r->score;
					TRACE_DEBUG(ANNOYING, "Applied rule {'*' : '%s' += %d} to candidate '%s'", target->md.plain, r->score, cand->diamid);
				}
				
				/* The target is matching this candidate, check if there are additional rules criteria matching this message. */
				for ( j = 1; j < RTD_CRI_MAX; j++ ) {
					if ( ! FD_IS_LIST_EMPTY(&target->rules[j]) ) {
						/* if needed, find the required data in the message */
						if (parsed_msg_avp[j].status == NOT_RESOLVED_YET) {
							struct avp * avp = NULL;
							/* Search for the AVP in the message */
							CHECK_FCT( fd_msg_search_avp ( msg, AVP_MODELS[j], &avp ) );
							if (avp == NULL) {
								parsed_msg_avp[j].status = NOT_FOUND;
							} else {
								struct avp_hdr * ahdr = NULL;
								CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
								if (ahdr->avp_value == NULL) {
									/* This should not happen, but anyway let's just ignore it */
									parsed_msg_avp[j].status = NOT_FOUND;
								} else {
									/* OK, we got the AVP */
									parsed_msg_avp[j].status = FOUND;
									parsed_msg_avp[j].avp = ahdr->avp_value;
								}
							}
						}
						
						/* If we did not find the data for these rules in the message, just skip the series */
						if (parsed_msg_avp[j].status == NOT_FOUND) {
							TRACE_DEBUG(ANNOYING, "Skipping series of rules %d of target '%s', criteria absent from the message", j, target->md.plain);
							continue;
						}
						
						/* OK, we can now check if one of our rule's criteria match the message content */
						r = NULL;
						do {
							CHECK_FCT ( get_next_match( &target->rules[j], (char *) /* is this cast safe? */ parsed_msg_avp[j].avp->os.data, parsed_msg_avp[j].avp->os.len, (void *)&r) );
							if (!r)
								break;
							
							cand->score += r->score;
							TRACE_DEBUG(ANNOYING, "Applied rule {'%s' : '%s' += %d} to candidate '%s'", r->md.plain, target->md.plain, r->score, cand->diamid);
						} while (1);
					}
				}
			} while (1);
		}
	}
	
	return 0;
}

void rtd_dump(void)
{
	int i;
	fd_log_debug("[rt_default] Dumping rules repository...");
	for (i = 0; i < RTD_TAR_MAX; i++) {
		if (!FD_IS_LIST_EMPTY( &TARGETS[i] )) {
			struct fd_list * li;
			fd_log_debug("  Targets list %d:", i);
			for (li = TARGETS[i].next; li != &TARGETS[i]; li = li->next) {
				dump_target(4, (struct target *)li);
			}
		}
	}
	
	fd_log_debug("[rt_default] End of dump");
}
