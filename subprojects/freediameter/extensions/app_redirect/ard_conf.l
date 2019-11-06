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
#include "app_redir.h"
/* Include yacc tokens definitions */
#include "ard_conf.tab.h"

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
[[:digit:]]+		{
				/* Convert this to an u32 value */
				int ret=0;
				ret = sscanf(yytext, "%u", &yylval->u32);
				if (ret != 1) {
					/* No matching: an error occurred */
					fd_log_debug("Unable to convert the value '%s' to a valid number: %s", yytext, strerror(errno));
					return TOK_LEX_ERROR; /* trig an error in yacc parser */
					/* Maybe we could REJECT instead of failing here? */
				}
				return TOK_U32VAL;
			}
			
	/* Recognize bracketed quoted strings */
[[]{qstring}[]] 	{
				/* Match a quoted string containing a regex */
				CHECK_MALLOC_DO( yylval->tstring.str = strdup(yytext+2), 
				{
					TRACE_DEBUG(INFO, "Unable to copy the string '%s': %s", yytext, strerror(errno));
					return TOK_LEX_ERROR; /* trig an error in yacc parser */
				} );
				yylval->tstring.str[strlen(yytext) - 4] = '\0';
				yylval->tstring.regex = 1;
				return TOK_TSTRING;
			}
			
	/* Recognize quoted strings (since it comes after the previous rule, the string should not be quoted) */
{qstring}		{
				/* Match a quoted string. */
				CHECK_MALLOC_DO( yylval->tstring.str = strdup(yytext+1), 
				{
					TRACE_DEBUG(INFO, "Unable to copy the string '%s': %s", yytext, strerror(errno));
					return TOK_LEX_ERROR; /* trig an error in yacc parser */
				} );
				yylval->tstring.str[strlen(yytext) - 2] = '\0';
				yylval->tstring.regex = 0;
				return TOK_TSTRING;
			}
	
	/* The key words */	
(?i:"default_redirect_cache_time")	{ return TOK_DEFAULT_RCT;		}
(?i:"to")	 			{ return TOK_TO;			}
(?i:"DONT_CACHE")	 		{ return TOK_DONT_CACHE;		}
(?i:"ALL_SESSION")	 		{ return TOK_ALL_SESSION;		}
(?i:"ALL_REALM")	 		{ return TOK_ALL_REALM;			}
(?i:"REALM_AND_APPLICATION")	 	{ return TOK_REALM_AND_APPLICATION;	}
(?i:"ALL_APPLICATION")	 		{ return TOK_ALL_APPLICATION;		}
(?i:"ALL_HOST")	 			{ return TOK_ALL_HOST;			}
(?i:"ALL_USER")	 			{ return TOK_ALL_USER;			}
(?i:"from.id")	 			{ return TOK_FROM_ID;			}
(?i:"from.realm")	 		{ return TOK_FROM_REALM;		}
(?i:"app")	 			{ return TOK_APP;			}
			
	/* Valid single characters for yyparse */
[:=;]		{ return yytext[0]; }

	/* Unrecognized sequence, if it did not match any previous pattern */
[^[:space:]\":=;\n]+	{ 
				fd_log_debug("Unrecognized text on line %d col %d: '%s'.", yylloc->first_line, yylloc->first_column, yytext);
			 	return TOK_LEX_ERROR; 
			}

%%
