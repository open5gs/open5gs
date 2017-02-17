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

/* 
 * This extension allows to perform some pattern-matching on an AVP
 * and send the message to a server accordingly.
 * See rt_ereg.conf.sample file for the format of the configuration file.
 */

#include "rtereg.h"

/* The configuration structure */
struct rtereg_conf rtereg_conf;

#ifndef HAVE_REG_STARTEND
static char * buf = NULL;
static size_t bufsz;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
#endif /* HAVE_REG_STARTEND */

static int proceed(char * value, size_t len, struct fd_list * candidates)
{
	int i;
	
	for (i = 0; i < rtereg_conf.rules_nb; i++) {
		/* Does this pattern match the value? */
		struct rtereg_rule * r = &rtereg_conf.rules[i];
		int err = 0;
		struct fd_list * c;
		
		TRACE_DEBUG(ANNOYING, "Attempt pattern matching of '%.*s' with rule '%s'", (int)len, value, r->pattern);
		
		#ifdef HAVE_REG_STARTEND
		{
			regmatch_t pmatch[1];
			memset(pmatch, 0, sizeof(pmatch));
			pmatch[0].rm_so = 0;
			pmatch[0].rm_eo = len;
			err = regexec(&r->preg, value, 0, pmatch, REG_STARTEND);
		}
		#else /* HAVE_REG_STARTEND */
		{
			/* We have a 0-terminated string */
			err = regexec(&r->preg, value, 0, NULL, 0);
		}
		#endif /* HAVE_REG_STARTEND */
		
		if (err == REG_NOMATCH)
			continue;
			
		if (err != 0) {
			char * errstr;
			size_t bl;

			/* Error while compiling the regex */
			TRACE_DEBUG(INFO, "Error while executing the regular expression '%s':", r->pattern);

			/* Get the error message size */
			bl = regerror(err, &r->preg, NULL, 0);

			/* Alloc the buffer for error message */
			CHECK_MALLOC( errstr = malloc(bl) );

			/* Get the error message content */
			regerror(err, &r->preg, errstr, bl);
			TRACE_DEBUG(INFO, "\t%s", errstr);

			/* Free the buffer, return the error */
			free(errstr);
			
			return (err == REG_ESPACE) ? ENOMEM : EINVAL;
		}
		
		/* From this point, the expression matched the AVP value */
		TRACE_DEBUG(FULL, "[rt_ereg] Match: '%s' to value '%.*s' => '%s' += %d",
					r->pattern,
					(int)len,
					value,
					r->server,
					r->score);
		
		for (c = candidates->next; c != candidates; c = c->next) {
			struct rtd_candidate * cand = (struct rtd_candidate *)c;

			if (strcmp(r->server, cand->diamid) == 0) {
				cand->score += r->score;
				break;
			}
		}
	};
	
	return 0;
}

/* The callback called on new messages */
static int rtereg_out(void * cbdata, struct msg ** pmsg, struct fd_list * candidates)
{
	struct msg * msg = *pmsg;
	struct avp * avp = NULL;
	
	TRACE_ENTRY("%p %p %p", cbdata, msg, candidates);
	
	CHECK_PARAMS(msg && candidates);
	
	/* Check if it is worth processing the message */
	if (FD_IS_LIST_EMPTY(candidates)) {
		return 0;
	}
	
	/* Now search the AVP in the message */
	CHECK_FCT( fd_msg_search_avp ( msg, rtereg_conf.avp, &avp ) );
	if (avp != NULL) {
		struct avp_hdr * ahdr = NULL;
		CHECK_FCT( fd_msg_avp_hdr ( avp, &ahdr ) );
		if (ahdr->avp_value != NULL) {
#ifndef HAVE_REG_STARTEND
			int ret;
		
			/* Lock the buffer */
			CHECK_POSIX( pthread_mutex_lock(&mtx) );
			
			/* Augment the buffer if needed */
			if (ahdr->avp_value->os.len >= bufsz) {
				CHECK_MALLOC_DO( buf = realloc(buf, ahdr->avp_value->os.len + 1), 
					{ pthread_mutex_unlock(&mtx); return ENOMEM; } );
			}
			
			/* Copy the AVP value */
			memcpy(buf, ahdr->avp_value->os.data, ahdr->avp_value->os.len);
			buf[ahdr->avp_value->os.len] = '\0';
			
			/* Now apply the rules */
			ret = proceed(buf, ahdr->avp_value->os.len, candidates);
			
			CHECK_POSIX(pthread_mutex_unlock(&mtx));
			
			CHECK_FCT(ret);
#else /* HAVE_REG_STARTEND */
			CHECK_FCT( proceed((char *) ahdr->avp_value->os.data, ahdr->avp_value->os.len, candidates) );
#endif /* HAVE_REG_STARTEND */
		}
	}
	
	return 0;
}

/* handler */
static struct fd_rt_out_hdl * rtereg_hdl = NULL;

/* entry point */
static int rtereg_entry(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	/* Initialize the configuration */
	memset(&rtereg_conf, 0, sizeof(rtereg_conf));
	
	/* Parse the configuration file */
	CHECK_FCT( rtereg_conf_handle(conffile) );
	
	/* Register the callback */
	CHECK_FCT( fd_rt_out_register( rtereg_out, NULL, 1, &rtereg_hdl ) );
	
	/* We're done */
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	int i;
	TRACE_ENTRY();
	
	/* Unregister the cb */
	CHECK_FCT_DO( fd_rt_out_unregister ( rtereg_hdl, NULL ), /* continue */ );
	
	/* Destroy the data */
	if (rtereg_conf.rules) 
		for (i = 0; i < rtereg_conf.rules_nb; i++) {
			free(rtereg_conf.rules[i].pattern);
			free(rtereg_conf.rules[i].server);
			regfree(&rtereg_conf.rules[i].preg);
		}
	free(rtereg_conf.rules);
#ifndef HAVE_REG_STARTEND
	free(buf);
#endif /* HAVE_REG_STARTEND */
	
	/* Done */
	return ;
}

EXTENSION_ENTRY("rt_ereg", rtereg_entry);
