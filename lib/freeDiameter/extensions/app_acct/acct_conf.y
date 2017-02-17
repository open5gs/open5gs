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
 * See doc/app_acct.conf.sample for configuration file format
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
#include "app_acct.h"
#include "acct_conf.tab.h"

#include <string.h>
#include <errno.h>

/* Forward declaration */
int yyparse(char * conffile);

/* The Lex parser prototype */
int acct_conflex(YYSTYPE *lvalp, YYLTYPE *llocp);

/* the global configuration */
struct acct_conf * acct_config = NULL;

/* Initialize the blank configuration structure */
int acct_conf_init(void)
{
	TRACE_ENTRY();
	
	CHECK_MALLOC( acct_config = malloc(sizeof(struct acct_conf)) );
	memset(acct_config, 0, sizeof(struct acct_conf) );
	fd_list_init(&acct_config->avps, NULL);
	
	return 0;
}

/* Validate and eventually display the content of the configuration file for debug */
int acct_conf_check(char * conffile)
{
	CHECK_PARAMS(acct_config);
	
	if ( ! acct_config->conninfo) {
		fd_log_debug("[app_acct] ERROR: 'ConnInfo' is missing in file '%s'."
			" You can specify 'ConnInfo=\"\";' to use default parameters.", conffile);
		return EINVAL;
	}
	if ( ! acct_config->tablename) {
		fd_log_debug("[app_acct] ERROR: 'Table' value is missing in file '%s'.", conffile);
		return EINVAL;
	}

	if (!TRACE_BOOL(FULL))
		return 0;
	
	struct fd_list * li;
		
	fd_log_debug("[app_acct] Configuration dump:");
	fd_log_debug(" Database:");
	fd_log_debug("   ConnInfo ...... : '%s'", acct_config->conninfo ?: "<null>");
	fd_log_debug("   Table name .... : '%s'", acct_config->tablename ?: "<null>");
	fd_log_debug("   Timestamp field : '%s'", acct_config->tsfield ?: "<null>");
	fd_log_debug("   Server name fld : '%s'", acct_config->srvnfield ?: "<null>");
	fd_log_debug(" AVPs that will be saved to the database:");
	for (li = acct_config->avps.next; li != &acct_config->avps; li = li->next) {
		struct acct_conf_avp * a = (struct acct_conf_avp *)li;
		fd_log_debug("   %-*s AVP%s saved in ", 30, a->avpname, a->required ? " [required]":"" );
		if (a->multi) {
			fd_log_debug("fields '%s[1..%d]' ", a->field?:a->avpname, a->multi);
		} else {
			fd_log_debug("field '%s' ", a->field?:a->avpname);
		}
		fd_log_debug("as ::%s", diam2db_types_mapping[a->avptype]);
	}
	fd_log_debug("[app_acct] Complete.");
	return 0;
}

void acct_conf_free(void)
{
	TRACE_ENTRY();
	
	if (!acct_config)
		return;
		
	/* Destroy the list */
	while (!FD_IS_LIST_EMPTY(&acct_config->avps)) {
		struct acct_conf_avp * a = (struct acct_conf_avp *)(acct_config->avps.next);
		fd_list_unlink(&a->chain);
		free(a->avpname);
		free(a->field);
		free(a);
	}
	
	/* destroy other data */
	free(acct_config->conninfo);
	free(acct_config->tablename);
	free(acct_config->tsfield);
	free(acct_config->srvnfield);
	
	/* Done */
	free(acct_config);
	acct_config = NULL;
}

/* Parse the configuration file */
int acct_conf_parse(char * conffile)
{
	extern FILE * acct_confin;
	int ret;
	
	TRACE_ENTRY("%p", conffile);
	
	TRACE_DEBUG (FULL, "Parsing configuration file: %s...", conffile);
	
	acct_confin = fopen(conffile, "r");
	if (acct_confin == NULL) {
		ret = errno;
		fd_log_debug("Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		return ret;
	}

	ret = yyparse(conffile);

	fclose(acct_confin);

	if (ret != 0) {
		TRACE_DEBUG (INFO, "Unable to parse the configuration file.");
		return EINVAL;
	}
	
	return 0;
}

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

static struct acct_conf_avp avpdata;

%}

/* Values returned by lex for token */
%union {
	char 		*string;	/* The string is allocated by strdup in lex.*/
	int		 integer;	/* Store integer values */
}

/* In case of error in the lexical analysis */
%token 		LEX_ERROR

/* Keywords */
%token 		FIELD
%token 		REQUIRED
%token 		MULTI
%token 		CONNINFO
%token 		TABLE
%token 		TSFIELD
%token 		SRVNFIELD

/* Tokens and types */
/* A (de)quoted string (malloc'd in lex parser; it must be freed after use) */
%token <string>	QSTRING

/* An integer value */
%token <integer> INTEGER



/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty grammar is OK for the parser (will be validated afterwards) */
			| conffile avpline
			| conffile conninfoline
			| conffile tableline
			| conffile tsfieldline
			| conffile srvnfieldline
			| conffile errors
			{
				yyerror(&yylloc, conffile, "An error occurred while parsing the configuration file.");
				return EINVAL;
			}
			;

			/* Catch lexical and syntax errors */
errors:			LEX_ERROR
			| error
			;

	/* The tokens */
avpline:		{
				memset(&avpdata, 0, sizeof(struct acct_conf_avp));
			}
			QSTRING avpcontents ';'
			{
				struct acct_conf_avp *new;
				struct dict_object * dict;
				struct dict_avp_data dictdata;
				
				/* Validate the avp name first */
				CHECK_FCT_DO( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME_ALL_VENDORS, $2, &dict, ENOENT),
					{ yyerror (&yylloc, conffile, "AVP definition not found in the dictionary. Was the appropriate dict_*.fdx extension loaded?"); YYERROR; } );
				CHECK_FCT( fd_dict_getval( dict, &dictdata ));
				
				/* Create a new entry */
				CHECK_MALLOC_DO( new = malloc(sizeof(struct acct_conf_avp)),
					{ yyerror (&yylloc, conffile, "Out of memory"); YYERROR; } );
				
				/* Retrieve all the data from avpcontents parsing (field, required, multi) */	
				memcpy(new, &avpdata, sizeof(struct acct_conf_avp));
				
				/* Initialize the other data */
				fd_list_init(&new->chain, NULL);
				new->avpname = $2;
				new->avpobj = dict;
				new->avptype = dictdata.avp_basetype;
			
				/* Add this new entry at the end of the list */
				fd_list_insert_before( &acct_config->avps, &new->chain );
			}
			;
			
avpcontents:		/* Empty content is fine */
			| '=' '{' avpflagline '}'
			;
			
avpflagline:		/* Empty flags is also fine */
			| avpflagline FIELD '=' QSTRING ';'
			{
				if (avpdata.field) {
					yyerror (&yylloc, conffile, "Duplicate entry");
					YYERROR;
				}
				avpdata.field = $4;
			}
			| avpflagline REQUIRED ';'
			{
				avpdata.required = 1;
			}
			| avpflagline MULTI '=' INTEGER ';'
			{
				avpdata.multi = (unsigned) $4;
			}
			;
	
conninfoline:		CONNINFO '=' QSTRING ';'
			{
				if (acct_config->conninfo) {
					yyerror (&yylloc, conffile, "Duplicate entry");
					YYERROR;
				}
				acct_config->conninfo = $3;
			}
			;

tableline:		TABLE '=' QSTRING ';'
			{
				if (acct_config->tablename) {
					yyerror (&yylloc, conffile, "Duplicate entry");
					YYERROR;
				}
				acct_config->tablename = $3;
			}
			;

tsfieldline:		TSFIELD '=' QSTRING ';'
			{
				if (acct_config->tsfield) {
					yyerror (&yylloc, conffile, "Duplicate entry");
					YYERROR;
				}
				acct_config->tsfield = $3;
			}
			;

srvnfieldline:		SRVNFIELD '=' QSTRING ';'
			{
				if (acct_config->srvnfield) {
					yyerror (&yylloc, conffile, "Duplicate entry");
					YYERROR;
				}
				acct_config->srvnfield = $3;
			}
			;
