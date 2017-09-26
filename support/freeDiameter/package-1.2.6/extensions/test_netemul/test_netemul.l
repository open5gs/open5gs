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

/* Tokenizer */

%{
#include "test_netemul.h"
/* Include yacc tokens definitions */
#include "test_netemul.tab.h"

/* Update the column information */
#define YY_USER_ACTION { 						\
	yylloc->first_column = yylloc->last_column + 1; 		\
	yylloc->last_column = yylloc->first_column + yyleng - 1;	\
}

/* Avoid warning with newer flex */
#define YY_NO_INPUT

%}

%option bison-bridge bison-locations
%option noyywrap
%option nounput

%%

	/* Update the line count */
\n			{
				yylloc->first_line++; 
				yylloc->last_line++; 
				yylloc->last_column=0; 
			}
	 
	/* Eat all spaces but not new lines */
([[:space:]]{-}[\n])+	;
	/* Eat all comments */
#.*$			;


	/* Recognize floats */
[[:digit:]]+"."[[:digit:]]*	{
				/* Convert this to an integer value */
				int ret=0;
				ret = sscanf(yytext, "%f", &yylval->decimal);
				if (ret != 1) {
					/* No matching: an error occurred */
					fd_log_debug("Unable to convert the value '%s' to a valid float: %s", yytext, strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
					/* Maybe we could REJECT instead of failing here? */
				}
				return FLOAT;
			}
			
	/* Recognize simple integers */
[[:digit:]]+		{
				/* Convert this to an integer value */
				int ret=0;
				ret = sscanf(yytext, "%lu", &yylval->ulong);
				if (ret != 1) {
					/* No matching: an error occurred */
					fd_log_debug("Unable to convert the value '%s' to a valid long: %s", yytext, strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
					/* Maybe we could REJECT instead of failing here? */
				}
				return ULONG;
			}
			
	
	
	/* The key words */	
(?i:"latency_average")	 	{	return LATENCY_AVERAGE;		}
(?i:"latency_deviation")	{	return LATENCY_DEVIATION;	}
(?i:"dupl_proba")	 	{	return DUPL_PROBA;		}
(?i:"ms")	 		{	return UNIT_MSEC;		}
(?i:"s")	 		{	return UNIT_SEC;		}
			
	/* Valid single characters for yyparse */
[=;%/]			{ return yytext[0]; }

	/* Unrecognized sequence, if it did not match any previous pattern */
[^[:space:][:digit:]=;%/\n]+	{ 
				fd_log_debug("Unrecognized text on line %d col %d: '%s'.", yylloc->first_line, yylloc->first_column, yytext);
			 	return LEX_ERROR; 
			}

%%
