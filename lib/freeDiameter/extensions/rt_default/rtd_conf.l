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

/* Tokenizer
 *
 */

%{
#include "rt_default.h"
/* Include yacc tokens definitions */
#include "rtd_conf.tab.h"

/* Update the column information */
#define YY_USER_ACTION { 						\
	yylloc->first_column = yylloc->last_column + 1; 		\
	yylloc->last_column = yylloc->first_column + yyleng - 1;	\
}

/* Avoid warning with newer flex */
#define YY_NO_INPUT

%}

qstring		\"[^\"\n]*\"


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

	/* Recognize any integer */
[-]?[[:digit:]]+	{
				/* Convert this to an integer value */
				int ret=0;
				ret = sscanf(yytext, "%i", &yylval->integer);
				if (ret != 1) {
					/* No matching: an error occurred */
					fd_log_debug("Unable to convert the value '%s' to a valid number: %s", yytext, strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
					/* Maybe we could REJECT instead of failing here? */
				}
				return INTEGER;
			}
			
	/* In addition, recognize some constant names as integers also */
(?i:"NO_DELIVERY")	{	yylval->integer = FD_SCORE_NO_DELIVERY;		return INTEGER;		}
(?i:"DEFAULT")		{	yylval->integer = FD_SCORE_DEFAULT; 		return INTEGER;		}
(?i:"DEFAULT_REALM")	{	yylval->integer = FD_SCORE_DEFAULT_REALM; 	return INTEGER;		}
(?i:"REALM")		{	yylval->integer = FD_SCORE_REALM; 		return INTEGER;		}
(?i:"REDIR_HOST")	{	yylval->integer = FD_SCORE_REDIR_HOST; 		return INTEGER;		}
(?i:"REDIR_APP")	{	yylval->integer = FD_SCORE_REDIR_APP; 		return INTEGER;		}
(?i:"REDIR_REALM")	{	yylval->integer = FD_SCORE_REDIR_REALM; 	return INTEGER;		}
(?i:"REDIR_REALM_APP")	{	yylval->integer = FD_SCORE_REDIR_REALM_APP; 	return INTEGER;		}
(?i:"REDIR_USER")	{	yylval->integer = FD_SCORE_REDIR_USER; 		return INTEGER;		}
(?i:"REDIR_SESSION")	{	yylval->integer = FD_SCORE_REDIR_SESSION; 	return INTEGER;		}
(?i:"FINALDEST")	{	yylval->integer = FD_SCORE_FINALDEST; 		return INTEGER;		}

	/* Recognize bracketed quoted strings */
[[]{qstring}[]] 	{
				/* Match a quoted string containing a regex */
				CHECK_MALLOC_DO( yylval->string = strdup(yytext+2), 
				{
					TRACE_DEBUG(INFO, "Unable to copy the string '%s': %s", yytext, strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
				} );
				yylval->string[strlen(yytext) - 4] = '\0';
				return BQSTRING;
			}
			
	/* Recognize quoted strings */
{qstring}		{
				/* Match a quoted string. */
				CHECK_MALLOC_DO( yylval->string = strdup(yytext+1), 
				{
					TRACE_DEBUG(INFO, "Unable to copy the string '%s': %s", yytext, strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
				} );
				yylval->string[strlen(yytext) - 2] = '\0';
				return QSTRING;
			}
	
	/* The key words */	
(?i:"oh")	 	{	return OH;	}
(?i:"or")	 	{	return OR;	}
(?i:"dh")	 	{	return DH;	}
(?i:"dr")	 	{	return DR;	}
(?i:"un")	 	{	return UN;	}
(?i:"si")	 	{	return SI;	}
(?i:"rlm")	 	{	return REALM;	}
			
	/* Valid single characters for yyparse */
[*:=+-;]		{ return yytext[0]; }

	/* Unrecognized sequence, if it did not match any previous pattern */
[^[:space:]\"*:=+;\n]+	{ 
				fd_log_debug("Unrecognized text on line %d col %d: '%s'.", yylloc->first_line, yylloc->first_column, yytext);
			 	return LEX_ERROR; 
			}

%%
