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
 * See doc/rt_default.conf.sample for configuration file format
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
#include "rt_default.h"
#include "rtd_conf.tab.h"	/* bison is not smart enough to define the YYLTYPE before including this code, so... */

/* Forward declaration */
int yyparse(char * conffile);

static int rules_added = 0;

/* Parse the configuration file */
int rtd_conf_handle(char * conffile)
{
	extern FILE * rtd_confin;
	int ret;
	
	TRACE_ENTRY("%p", conffile);
	
	TRACE_DEBUG (FULL, "Parsing configuration file: %s...", conffile);
	
	rtd_confin = fopen(conffile, "r");
	if (rtd_confin == NULL) {
		ret = errno;
		fd_log_debug("Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		TRACE_DEBUG (INFO, "Error occurred, message logged -- configuration file.");
		return ret;
	}

	ret = yyparse(conffile);

	fclose(rtd_confin);

	if (ret != 0) {
		TRACE_DEBUG (INFO, "Unable to parse the configuration file.");
		return EINVAL;
	} else {
		TRACE_DEBUG(FULL, "Added %d RULES routing entries successfully.", rules_added);
	}
	
	return 0;
}

/* The Lex parser prototype */
int rtd_conflex(YYSTYPE *lvalp, YYLTYPE *llocp);

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
	int		 integer;	/* Store integer values */
	char 		*string;	/* The string is allocated by strdup in lex.*/
	struct {
		char * str;
		int regex;
	}		 tstring;	/* typed string */
	struct {
		char * str;
		int    regex;
		enum rtd_crit_type type;
	}		 criteria;
	struct {
		char * str;
		int    regex;
		enum rtd_targ_type type;
	}		 target;
}

/* In case of error in the lexical analysis */
%token 		LEX_ERROR

/* A (de)quoted string (malloc'd in lex parser; it must be freed after use) */
%token <string>	QSTRING
/* A (de)bracket-quoted string (malloc'd in lex parser; it must be freed after use): ["blahblah"] */
%token <string>	BQSTRING

/* An integer value */
%token <integer> INTEGER

/* The types for this gramar */
%type <tstring>  TSTRING
%type <criteria> CRITERIA
%type <target> 	 TARGET
%type <integer>  EXPR_INT

/* Tokens */
%token 		OH
%token 		OR
%token 		DH
%token 		DR
%token 		UN
%token 		SI

%token 		REALM



/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty grammar is OK */
			| conffile rule
			;
			
	/* a RULE entry */
rule:			CRITERIA ':' TARGET '+' '=' EXPR_INT ';'
			{
				int flag = 0;
				if ($1.regex)
					flag |= RTD_CRIT_REG;
				if ($3.regex)
					flag |= RTD_TARG_REG;
					
				/* Add this rule to the repository */
				CHECK_FCT_DO( rtd_add($1.type, $1.str, $3.type, $3.str, $6, flag),
					{
						yyerror (&yylloc, conffile, "An error occurred while adding a rule, aborting...");
						YYERROR;
					} );
				
				rules_added++;
			}
			;
	
	/* QSTRING and BQSTRING are equivalent in the grammar */
TSTRING:		QSTRING
			{
				$$.str = $1;
				$$.regex = 0;
			}
			| BQSTRING
			{
				$$.str = $1;
				$$.regex = 1;
			}
			;

	/* Details of the CRITERIA type */
CRITERIA:		'*'
			{
				$$.str = NULL;
				$$.regex = 0;
				$$.type = RTD_CRI_ALL;
			}
			| OH '=' TSTRING
			{
				$$.str = $3.str;
				$$.regex =$3.regex;
				$$.type = RTD_CRI_OH;
			}
			| OR '=' TSTRING
			{
				$$.str = $3.str;
				$$.regex =$3.regex;
				$$.type = RTD_CRI_OR;
			}
			| DH '=' TSTRING
			{
				$$.str = $3.str;
				$$.regex =$3.regex;
				$$.type = RTD_CRI_DH;
			}
			| DR '=' TSTRING
			{
				$$.str = $3.str;
				$$.regex =$3.regex;
				$$.type = RTD_CRI_DR;
			}
			| UN '=' TSTRING
			{
				$$.str = $3.str;
				$$.regex =$3.regex;
				$$.type = RTD_CRI_UN;
			}
			| SI '=' TSTRING
			{
				$$.str = $3.str;
				$$.regex =$3.regex;
				$$.type = RTD_CRI_SI;
			}
			;

	/* Details of the TARGET type */
TARGET:			TSTRING
			{
				$$.str = $1.str;
				$$.regex =$1.regex;
				$$.type = RTD_TAR_ID;
			}
			| REALM '=' TSTRING
			{
				$$.str = $3.str;
				$$.regex =$3.regex;
				$$.type = RTD_TAR_REALM;
			}
			;
	
	/* An expression that has an integer value; we allow + and - operators cause it is convenient */
EXPR_INT:		INTEGER
			{
				$$ = $1;
			}
			| EXPR_INT '+' INTEGER
			{
				$$ = $1 + $3;
			}
			| EXPR_INT '-' INTEGER
			{
				$$ = $1 - $3;
			}
			;
