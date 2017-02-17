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
 * See doc/dict_legacy_xml.conf.sample for configuration file format
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
#include "dict_lxml.h"
#include "dict_lxml.tab.h"	/* bison is not smart enough to define the YYLTYPE before including this code, so... */

/* Forward declaration */
int yyparse(char * conffile);

static int nb_files = 0;
static int nb_dict = 0;

/* Parse the configuration file */
int dict_lxml_handle(char * conffile)
{
	extern FILE * dict_lxmlin;
	int ret;
	
	TRACE_ENTRY("%p", conffile);
	
	TRACE_DEBUG (FULL, "Parsing configuration file: %s...", conffile);
	
	dict_lxmlin = fopen(conffile, "r");
	if (dict_lxmlin == NULL) {
		ret = errno;
		fd_log_debug("Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		TRACE_DEBUG (INFO, "Error occurred, message logged -- configuration file.");
		return ret;
	}

	ret = yyparse(conffile);

	fclose(dict_lxmlin);

	if (ret != 0) {
		TRACE_DEBUG (INFO, "Unable to parse the configuration file.");
		return EINVAL;
	} else {
		TRACE_DEBUG(FULL, "%d XML dictionary files parsed successfully, %d dictionary objects added.", nb_files, nb_dict);
	}
	
	return 0;
}

/* The Lex parser prototype */
int dict_lxmllex(YYSTYPE *lvalp, YYLTYPE *llocp);

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
}

/* In case of error in the lexical analysis */
%token 		LEX_ERROR

/* A (de)quoted string (malloc'd in lex parser; it must be freed after use) */
%token <string>	QSTRING

/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty grammar is OK */
			| conffile xmlfile
			;
			
	/* a RULE entry */
xmlfile:		QSTRING ';'
			{
				int ret = dict_lxml_parse($1);
				if (ret < 0) {
					yyerror (&yylloc, conffile, "An error occurred while parsing a file, aborting...");
					YYERROR;
				}
				nb_files++;
				nb_dict += ret;
			}
			;
