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

/* Tokenizer for echo/drop plugin */

%{
#include "rgwx_echodrop.h"
#include "rgwx_echodrop.tab.h"

/* Update the column information */
#define YY_USER_ACTION { 						\
	yylloc->first_column = yylloc->last_column + 1; 		\
	yylloc->last_column = yylloc->first_column + yyleng - 1;	\
}

/* %option noinput ? */
#define YY_NO_INPUT
%}

%option bison-bridge bison-locations
%option noyywrap
%option nounput

%%

<*>\n			{ 
				/* Update the line count */
				yylloc->first_line++; 
				yylloc->last_line++; 
				yylloc->last_column=0; 
			} 

<*>([[:space:]]{-}[\n])+	; /* Eat all spaces, not new lines */
<*>#.*$			; /* Eat all comments */

[[:digit:]]+		{
				/* Match an integer (not hexa) */
				int ret = sscanf(yytext, "%d", &yylval->integer);
				if (ret != 1) {
					/* No matching: an error occurred */
					fd_log_debug("[echodrop.rgwx] Unable to convert the value '%s' to a valid number: %s", yytext, strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
					/* Maybe we could REJECT instead of failing here? */
				}
				return INTEGER;
			}
				
(?i:"echo")		{ return TOK_ECHO; 		}
(?i:"drop")		{ return TOK_DROP; 		}
(?i:"code")		{ return TOK_CODE; 		}
(?i:"vendor")		{ return TOK_VENDOR; 		}
(?i:"tlv")		{ return TOK_TLV; 		}
(?i:"ext")		{ return TOK_EXT; 		}

	/* Valid single characters for yyparse */
<*>[;]			{ return yytext[0]; }


	/* Unrecognized token */
<*>[[:alnum:]]+		|	/* This rule is only useful to print a complete token in error messages */
	/* Unrecognized character */
<*>.			{ 
				fd_log_debug("[echodrop.rgwx] Unrecognized text on line %d col %d: '%s'.", yylloc->first_line, yylloc->first_column, yytext);
			 	return LEX_ERROR; 
			}

%%
