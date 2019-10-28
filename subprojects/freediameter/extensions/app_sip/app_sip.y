/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Alexandre Westfahl <awestfahl@freediameter.net>						 *
*													 *
* Copyright (c) 2010, Alexandre Westfahl, Teraoka Laboratory (Keio University), and the WIDE Project. 	 *		
*													 *
* All rights reserved.											 *
* Based on ta_conf.y  (Sebastien Decugis <sdecugis@freediameter.net>)					 *
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
* * Neither the name of the Teraoka Laboratory nor the 							 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of Teraoka Laboratory 						 *
*   													 *
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



/* For development only : */
%debug 
%error-verbose

/* The parser receives the configuration file filename as parameter */
%parse-param {char * conffile}

/* Keep track of location */
%locations 
%pure-parser

%{
#include "app_sip.h"
#include "app_sip.tab.h"	/* bison is not smart enough to define the YYLTYPE before including this code, so... */

#include <string.h>
#include <errno.h>

/* Forward declaration */
int yyparse(char * conffile);

/* Parse the configuration file */
int as_conf_handle(char * conffile)
{
	extern FILE * app_sipin;
	int ret;
	
	TRACE_ENTRY("%p", conffile);
	
	TRACE_DEBUG (FULL, "Parsing configuration file: %s...", conffile);
	
	app_sipin = fopen(conffile, "r");
	if (app_sipin == NULL) {
		ret = errno;
		fd_log_debug("Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		TRACE_DEBUG (INFO, "Error occurred, message logged -- configuration file.");
		return ret;
	}

	ret = yyparse(conffile);

	fclose(app_sipin);

	if (ret != 0) {
		TRACE_DEBUG (INFO, "Unable to parse the configuration file.");
		return EINVAL;
	}
	
	return 0;
}

/* The Lex parser prototype */
int app_siplex(YYSTYPE *lvalp, YYLTYPE *llocp);

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
	int		 integer;		/* Store integer values */
}

/* In case of error in the lexical analysis */
%token 		LEX_ERROR

/* Key words */
%token 		MODE
%token 		DATASOURCE
%token 		ASMYSQL_LOGIN
%token 		ASMYSQL_PASSWORD
%token 		ASMYSQL_DATABASE
%token 		ASMYSQL_SERVER
%token 		ASMYSQL_PORT
%token 		RTR_PORT
%token 		PPR_PORT

/* Tokens and types for routing table definition */
/* A (de)quoted string (malloc'd in lex parser; it must be freed after use) */
%token <string>	QSTRING

/* An integer value */
%token <integer> INTEGER



/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty grammar is OK */
			| conffile mode
			| conffile datasource
			| conffile mysql_login
			| conffile mysql_password
			| conffile mysql_database
			| conffile mysql_server
			| conffile mysql_port
			| conffile rtr_port
			| conffile ppr_port
			;

mode:			MODE '=' INTEGER ';'
			{
				as_conf->mode = $3;
			}
			;

datasource:			DATASOURCE '=' INTEGER ';'
			{
				as_conf->datasource = $3;
			}
			;

mysql_login:			ASMYSQL_LOGIN '=' QSTRING ';'
			{
				free(as_conf->mysql_login);
				as_conf->mysql_login = $3;
			}
			;

mysql_password:			ASMYSQL_PASSWORD '=' QSTRING ';'
			{
				free(as_conf->mysql_password);
				as_conf->mysql_password = $3;
			}
			;

mysql_database:			ASMYSQL_DATABASE '=' QSTRING ';'
			{
				free(as_conf->mysql_database);
				as_conf->mysql_database = $3;
			}
			;

mysql_server:		ASMYSQL_SERVER '=' QSTRING ';'
			{
				free(as_conf->mysql_server);
				as_conf->mysql_server = $3;
			}
			;

mysql_port:		ASMYSQL_PORT '=' INTEGER ';'
			{
				as_conf->mysql_port = (uint16_t)$3;
			}
			;
rtr_port:		RTR_PORT '=' INTEGER ';'
			{
				as_conf->rtr_port = (uint16_t)$3;
			}
			;
ppr_port:		PPR_PORT '=' INTEGER ';'
			{
				as_conf->ppr_port = (uint16_t)$3;
			}
			;
