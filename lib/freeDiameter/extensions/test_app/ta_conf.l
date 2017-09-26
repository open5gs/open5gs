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

/* Lex extension's configuration parser.
 *
 * The configuration file contains a default priority, and a list of peers with optional overwite priority.
 * -- see the app_test.conf.sample file for more detail.
 */

%{
#include "test_app.h"
/* Include yacc tokens definitions */
#include "ta_conf.tab.h"

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

	/* Recognize any integer */
[-]?[[:digit:]]+		{
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

	/* Recognize quoted strings -- we do not support escaped \" in the string currently. */
\"[^\"]+\"		{
				/* Match a quoted string. Let's be very permissive. */
				yylval->string = strdup(yytext+1);
				if (!yylval->string) {
					fd_log_debug("Unable to copy the string '%s': %s", yytext, strerror(errno));
					TRACE_DEBUG(INFO, "strdup failed");
					return LEX_ERROR; /* trig an error in yacc parser */
				}
				yylval->string[strlen(yytext) - 2] = '\0';
				return QSTRING;
			}
			
	/* Recognize the tokens */	
(?i:"vendor-id")	{
				return VENDOR_ID;
			}

(?i:"appli-id")		{
				return APPLI_ID;
			}

(?i:"cmd-id")		{
				return CMD_ID;
			}

(?i:"avp-id")		{
				return AVP_ID;
			}

(?i:"long-avp-id")	{
				return LONG_AVP_ID;
			}

(?i:"long-avp-len")	{
				return LONG_AVP_LEN;
			}

(?i:"mode")		{
				return MODE;
			}

(?i:"server")		{
				yylval->integer = MODE_SERV;
				return INTEGER;
			}

(?i:"client")		{
				yylval->integer = MODE_CLI;
				return INTEGER;
			}

(?i:"both")		{
				yylval->integer = MODE_SERV | MODE_CLI;
				return INTEGER;
			}

(?i:"dest-realm")	{
				return DEST_REALM;
			}

(?i:"dest-host")	{
				return DEST_HOST;
			}

(?i:"user-name")	{
				return USER_NAME;
			}

(?i:"Signal")		{
				return SIGNAL;
			}

(?i:"Benchmark")	{
				return BENCH;
			}

			
	/* Valid single characters for yyparse */
[=;]			{ return yytext[0]; }

	/* Unrecognized sequence, if it did not match any previous pattern */
[^[:space:]"*=>;\n]+	{ 
				fd_log_debug("Unrecognized text on line %d col %d: '%s'.", yylloc->first_line, yylloc->first_column, yytext);
			 	return LEX_ERROR; 
			}

%%
