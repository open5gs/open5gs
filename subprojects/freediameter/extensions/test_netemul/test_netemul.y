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
#include "test_netemul.h"
#include "test_netemul.tab.h"	/* YYLTYPE */

/* Forward declaration */
int yyparse(char * conffile);

/* Parse the configuration file */
int tne_conf_handle(char * conffile)
{
	extern FILE * test_netemulin;
	int ret;
	
	TRACE_ENTRY("%p", conffile);
	
	TRACE_DEBUG (FULL, "Parsing configuration file: %s...", conffile);
	
	test_netemulin = fopen(conffile, "r");
	if (test_netemulin == NULL) {
		ret = errno;
		fd_log_debug("Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		TRACE_DEBUG (INFO, "Error occurred, message logged -- configuration file.");
		return ret;
	}

	ret = yyparse(conffile);

	fclose(test_netemulin);

	if (ret != 0) {
		TRACE_DEBUG (INFO, "Unable to parse the configuration file.");
		return EINVAL;
	} else {
		TRACE_DEBUG(FULL, "[test_netemul]  latency: %lu ms (var:%u%%)  duplicates: %G probability.", tne_conf.lat_avg, tne_conf.lat_dev, tne_conf.dupl_proba);
	}
	
	return 0;
}

/* The Lex parser prototype */
int test_netemullex(YYSTYPE *lvalp, YYLTYPE *llocp);

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
	unsigned long	ulong;
	float	decimal;
}

/* In case of error in the lexical analysis */
%token 		LEX_ERROR

%token <ulong>   ULONG
%token <decimal> FLOAT

/* Tokens */
%token 		LATENCY_AVERAGE
%token 		LATENCY_DEVIATION
%token 		DUPL_PROBA
%token 		UNIT_SEC
%token 		UNIT_MSEC


/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty */
			| conffile latency_average
			| conffile latency_deviation
			| conffile dupl_proba
			;
			
	/* a server entry */
latency_average:	LATENCY_AVERAGE '=' ULONG UNIT_SEC ';'
			{
				tne_conf.lat_avg = $3 * 1000;
			}
			|
			LATENCY_AVERAGE '=' ULONG UNIT_MSEC ';'
			{
				tne_conf.lat_avg = $3;
			}
			;

latency_deviation:	LATENCY_DEVIATION '=' ULONG '%' ';'
			{
				tne_conf.lat_dev = (int)$3;
				if ((tne_conf.lat_dev < 0) || (tne_conf.lat_dev > 100)) {
					yyerror (&yylloc, conffile, "Latency_Deviation must be comprised between 0 and 100.");
					YYERROR;
				}
			}
			;

dupl_proba:		DUPL_PROBA '=' ULONG ';'
			{
				tne_conf.dupl_proba = (float) $3;
			}
			| DUPL_PROBA '=' FLOAT ';'
			{
				tne_conf.dupl_proba = $3;
			}
			| DUPL_PROBA '=' ULONG '/' ULONG ';'
			{
				tne_conf.dupl_proba = ((float)$3) / ((float)$5) ;
			}
			;
