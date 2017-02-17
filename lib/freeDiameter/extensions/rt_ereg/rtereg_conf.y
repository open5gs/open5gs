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
#include "rtereg.h"
#include "rtereg_conf.tab.h"	/* bison is not smart enough to define the YYLTYPE before including this code, so... */

/* Forward declaration */
int yyparse(char * conffile);

/* Parse the configuration file */
int rtereg_conf_handle(char * conffile)
{
	extern FILE * rtereg_confin;
	int ret;
	
	TRACE_ENTRY("%p", conffile);
	
	TRACE_DEBUG (FULL, "Parsing configuration file: %s...", conffile);
	
	rtereg_confin = fopen(conffile, "r");
	if (rtereg_confin == NULL) {
		ret = errno;
		fd_log_debug("Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		TRACE_DEBUG (INFO, "Error occurred, message logged -- configuration file.");
		return ret;
	}

	ret = yyparse(conffile);

	fclose(rtereg_confin);

	if (ret != 0) {
		TRACE_DEBUG (INFO, "Unable to parse the configuration file.");
		return EINVAL;
	} else {
		TRACE_DEBUG(FULL, "[rt-ereg] Added %d rules successfully.", rtereg_conf.rules_nb);
	}
	
	return 0;
}

/* The Lex parser prototype */
int rtereg_conflex(YYSTYPE *lvalp, YYLTYPE *llocp);

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
	int		integer;
}

/* In case of error in the lexical analysis */
%token 		LEX_ERROR

/* A (de)quoted string (malloc'd in lex parser; it must be freed after use) */
%token <string>	 QSTRING
%token <integer> INTEGER

/* Tokens */
%token 		AVP


/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		rules avp rules
			;
			
	/* a server entry */
avp:			AVP '=' QSTRING ';'
			{
				if (rtereg_conf.avp != NULL) {
					yyerror(&yylloc, conffile, "Only one AVP can be specified");
					YYERROR;
				}
				
				CHECK_FCT_DO( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, $3, &rtereg_conf.avp, ENOENT ),
					{
						TRACE_DEBUG(INFO, "Unable to find '%s' AVP in the loaded dictionaries.", $3);
						yyerror (&yylloc, conffile, "Invalid AVP value.");
						YYERROR;
					} );
				
				/* Now check the type */
				{
					struct dict_avp_data data;
					CHECK_FCT( fd_dict_getval( rtereg_conf.avp, &data) );
					CHECK_PARAMS_DO (data.avp_basetype == AVP_TYPE_OCTETSTRING, 
						{
							TRACE_DEBUG(INFO, "'%s' AVP in not an OCTETSTRING AVP (%d).", $3, data.avp_basetype);
							yyerror (&yylloc, conffile, "AVP in not an OCTETSTRING type.");
							YYERROR;
						} );
				}
			}
			;
			
rules:			/* empty OK */
			| rules rule
			;
			
rule:			QSTRING ':' QSTRING '+' '=' INTEGER ';'
			{
				struct rtereg_rule * new;
				int err;
				
				/* Add new rule in the array */
				rtereg_conf.rules_nb += 1;
				CHECK_MALLOC_DO(rtereg_conf.rules = realloc(rtereg_conf.rules, rtereg_conf.rules_nb * sizeof(struct rtereg_rule)),
					{
						yyerror (&yylloc, conffile, "Not enough memory to store the configuration...");
						YYERROR;
					} );
				
				new = &rtereg_conf.rules[rtereg_conf.rules_nb - 1];
				
				new->pattern = $1;
				new->server  = $3;
				new->score   = $6;
				
				/* Attempt to compile the regex */
				CHECK_FCT_DO( err=regcomp(&new->preg, new->pattern, REG_EXTENDED | REG_NOSUB),
					{
						char * buf;
						size_t bl;

						/* Error while compiling the regex */
						TRACE_DEBUG(INFO, "Error while compiling the regular expression '%s':", new->pattern);

						/* Get the error message size */
						bl = regerror(err, &new->preg, NULL, 0);

						/* Alloc the buffer for error message */
						CHECK_MALLOC( buf = malloc(bl) );

						/* Get the error message content */
						regerror(err, &new->preg, buf, bl);
						TRACE_DEBUG(INFO, "\t%s", buf);
						
						/* Free the buffer, return the error */
						free(buf);
						
						yyerror (&yylloc, conffile, "Invalid regular expression.");
						YYERROR;
					} );
			}
			;
