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

/* Yacc extension's configuration parser.
 * See doc/app_test.conf.sample for configuration file format
 */

/* For development only : */
%debug 
%error-verbose

/* The parser receives the configuration file filename as parameter */
%parse-param {char * conffile}

/* Keep track of location */
%locations 
%pure-parser

%{
#include "test_app.h"
#include "ta_conf.tab.h"	/* bison is not smart enough to define the YYLTYPE before including this code, so... */

#include <string.h>
#include <errno.h>

/* Forward declaration */
int yyparse(char * conffile);

/* Parse the configuration file */
int ta_conf_handle(char * conffile)
{
	extern FILE * ta_confin;
	int ret;
	
	TRACE_ENTRY("%p", conffile);
	
	TRACE_DEBUG (FULL, "Parsing configuration file: %s...", conffile);
	
	ta_confin = fopen(conffile, "r");
	if (ta_confin == NULL) {
		ret = errno;
		fd_log_debug("Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		TRACE_DEBUG (INFO, "Error occurred, message logged -- configuration file.");
		return ret;
	}

	ret = yyparse(conffile);

	fclose(ta_confin);

	if (ret != 0) {
		TRACE_DEBUG (INFO, "Unable to parse the configuration file.");
		return EINVAL;
	}
	
	return 0;
}

/* The Lex parser prototype */
int ta_conflex(YYSTYPE *lvalp, YYLTYPE *llocp);

/* Function to report the errors */
void yyerror (YYLTYPE *ploc, char * conffile, char const *s)
{
	TRACE_DEBUG(INFO, "Error in configuration parsing");
	
	if (ploc->first_line != ploc->last_line)
		fd_log_debug("%s:%d.%d-%d.%d : %s", conffile, ploc->first_line, ploc->first_column, ploc->last_line, ploc->last_column, s);
	else if (ploc->first_column != ploc->last_column)
		fd_log_debug("%s:%d.%d-%d : %s", conffile, ploc->first_line, ploc->first_column, ploc->last_column, s);
	else
		fd_log_debug("%s:%d.%d : %s", conffile, ploc->first_line, ploc->first_column, s);
}

%}

/* Values returned by lex for token */
%union {
	char 		*string;	/* The string is allocated by strdup in lex.*/
	int		 integer;	/* Store integer values */
}

/* In case of error in the lexical analysis */
%token 		LEX_ERROR

/* Key words */
%token 		VENDOR_ID
%token 		APPLI_ID
%token 		CMD_ID
%token 		AVP_ID
%token 		LONG_AVP_ID
%token 		LONG_AVP_LEN
%token 		MODE
%token 		DEST_REALM
%token 		DEST_HOST
%token 		USER_NAME
%token 		SIGNAL
%token		BENCH

/* Tokens and types for routing table definition */
/* A (de)quoted string (malloc'd in lex parser; it must be freed after use) */
%token <string>	QSTRING

/* An integer value */
%token <integer> INTEGER



/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty grammar is OK */
			| conffile vendor
			| conffile appli
			| conffile cmd
			| conffile avp
			| conffile long_avp_id
			| conffile long_avp_len
			| conffile mode
			| conffile dstrealm
			| conffile dsthost
			| conffile usrname
			| conffile signal
			| conffile bench
			;

vendor:			VENDOR_ID '=' INTEGER ';'
			{
				ta_conf->vendor_id = $3;
			}
			;

appli:			APPLI_ID '=' INTEGER ';'
			{
				ta_conf->appli_id = $3;
			}
			;

cmd:			CMD_ID '=' INTEGER ';'
			{
				ta_conf->cmd_id = $3;
			}
			;

avp:			AVP_ID '=' INTEGER ';'
			{
				ta_conf->avp_id = $3;
			}
			;

long_avp_id:		LONG_AVP_ID '=' INTEGER ';'
			{
				ta_conf->long_avp_id = $3;
			}
			;

long_avp_len:		LONG_AVP_LEN '=' INTEGER ';'
			{
				ta_conf->long_avp_len = $3;
			}
			;

mode:			MODE '=' INTEGER ';'
			{
				ta_conf->mode = $3 | (ta_conf->mode & ~3); /* overwrite the 2 lsb */
			}
			;

bench:			BENCH ';'
			{
				ta_conf->mode |= MODE_BENCH;
			}
			| BENCH INTEGER INTEGER ';'
			{
				ta_conf->mode |= MODE_BENCH;
				ta_conf->bench_duration = $2;
				ta_conf->bench_concur   = $3;
			}
			;

dstrealm:		DEST_REALM '=' QSTRING ';'
			{
				free(ta_conf->dest_realm);
				ta_conf->dest_realm = $3;
			}
			;

dsthost:		DEST_HOST '=' QSTRING ';'
			{
				free(ta_conf->dest_host);
				ta_conf->dest_host = $3;
			}
			;

usrname:		USER_NAME '=' QSTRING ';'
			{
				free(ta_conf->user_name);
				ta_conf->user_name = $3;
			}
			;

signal:			SIGNAL '=' INTEGER ';'
			{
				ta_conf->signal = $3;
			}
			;
