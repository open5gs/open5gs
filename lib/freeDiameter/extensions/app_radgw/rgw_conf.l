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
/* Lex configuration parser for radius_gw extension.
 *
 */

%{
#include "rgw.h"
#include "rgw_conf.tab.h"

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

/* Use the following start condition to parse an URI */
%x	IN_PLG
%x	IN_CLI1
%x	IN_CLI2
%x	EXPECT_IP4
%x	EXPECT_IP6
%x	EXPECT_DECINT

/* Quoted string. Multilines do not match. */
qstring		\"[^\"\n]*\"

/* Used to match IP, IP6, and port */
IP4		[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}
IP6		[[:xdigit:]]*:[[:xdigit:]]*:[[:xdigit:].:]*
BR_PORT		[[][0-9]+[]]


%%

	/* All sections */
<*>\n			{ 
				/* Update the line count */
				yylloc->first_line++; 
				yylloc->last_line++; 
				yylloc->last_column=0; 
			} 

<*>([[:space:]]{-}[\n])+	; /* Eat all spaces, not new lines */
<*>#.*$			; /* Eat all comments */

<*>{qstring}		{
				/* First copy the string without the quotes for use in the yacc parser */
				yylval->string = strdup(yytext+1);
				if (yylval->string == NULL) {
					fd_log_debug("Unable to allocate memory: %s", strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
				}

				yylval->string[yyleng-2] = '\0';
				
				/* the yacc parser will check the string is valid */
				return QSTRING;
			}


	/* Extension section */			
(?i:"RGWX")		{ BEGIN(IN_PLG); return PLG_PREFIX; 		}

<IN_PLG>(?i:"auth")	{ return AUTH; }
<IN_PLG>(?i:"acct")	{ return ACCT; }

<IN_PLG,IN_CLI2>[[:xdigit:]]+	{
				/* Convert this to an integer value */
				int ret = sscanf(yytext, "%x", &yylval->integer);
				if (ret != 1) {
					/* No matching: an error occurred */
					fd_log_debug("Unable to convert the value '%s' to a valid number: %s", yytext, strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
					/* Maybe we could REJECT instead of failing here? */
				}
				return INTEGER;
			}

<IN_PLG>[:]		{ return yytext[0]; }


	/* Client section */
(?i:"nas"|"cli")	{ BEGIN(IN_CLI1); yylval->integer=RGW_CLI_NAS; return NAS_OR_PXY; 		}
(?i:"pxy")		{ BEGIN(IN_CLI1); yylval->integer=RGW_CLI_PXY; return NAS_OR_PXY; 		}

	/* Match an IP (4 or 6) and optional port */
<IN_CLI1>({IP4}|{IP6}){BR_PORT}?	{
				char * work;
				char * port;
				unsigned short p = 0;
				
				work = strdup(yytext);
				if ( work == NULL ) {
					fd_log_debug("Unable to allocate memory: %s", strerror(errno));
					return LEX_ERROR; /* trig an error in yacc parser */
				}
				
				if ((port = strchr(work, '[')) != NULL) {
					*port = '\0';
					port++;
					if (sscanf(port, "%hu]", &p) != 1) {
						fd_log_debug("'%s' is not a valid port: %s", port, strerror(errno));
						free(work);
						return LEX_ERROR; /* trig an error in yacc parser */
					}
				}
				
				/* Do we have an IP or IPv6? Let's check if we have ':' char somewhere in the beginning */
				if (memchr(work, ':', 5) != NULL) {
					struct sockaddr_in6 * sin6 = NULL;
				
					sin6 = malloc(sizeof(struct sockaddr_in6));
					if (sin6 == NULL) {
						fd_log_debug("Unable to allocate memory: %s", strerror(errno));
						free(work);
						return LEX_ERROR; /* trig an error in yacc parser */
					}
					
					memset(sin6, 0, sizeof(struct sockaddr_in6));
					sin6->sin6_family = AF_INET6;
					if (inet_pton(AF_INET6, work, &sin6->sin6_addr) != 1) {
						fd_log_debug("'%s' is not a valid IPv6 address: %s", work, strerror(errno));
						free(work);
						free(sin6);
						return LEX_ERROR; /* trig an error in yacc parser */
					}
					sin6->sin6_port = htons(p);
					yylval->ss = (struct sockaddr *)sin6;
				} else {
					struct sockaddr_in * sin = NULL;
				
					sin = malloc(sizeof(struct sockaddr_in));
					if (sin == NULL) {
						fd_log_debug("Unable to allocate memory: %s", strerror(errno));
						free(work);
						return LEX_ERROR; /* trig an error in yacc parser */
					}
					
					memset(sin, 0, sizeof(struct sockaddr_in));
					sin->sin_family = AF_INET;
					if (inet_pton(AF_INET, work, &sin->sin_addr) != 1) {
						fd_log_debug("'%s' is not a valid IP address: %s", work, strerror(errno));
						free(work);
						free(sin);
						return LEX_ERROR; /* trig an error in yacc parser */
					}
					
					sin->sin_port = htons(p);
					yylval->ss = (struct sockaddr *)sin;
				}
				free(work);
				return IP;
			}


<IN_CLI1>"/"		{ BEGIN(IN_CLI2); return '/';	 		}


	/* Servers section */
(?i:"auth_server_enable")	{ BEGIN(EXPECT_DECINT); return AUTH_ENABLE; 	}
(?i:"auth_server_port")		{ BEGIN(EXPECT_DECINT); return AUTH_PORT; 	}
(?i:"auth_server_ip4")		{ BEGIN(EXPECT_IP4); return AUTH_IP4; 		}
(?i:"auth_server_ip6")		{ BEGIN(EXPECT_IP6); return AUTH_IP6; 		}
(?i:"acct_server_enable")	{ BEGIN(EXPECT_DECINT); return ACCT_ENABLE; 	}
(?i:"acct_server_port")		{ BEGIN(EXPECT_DECINT); return ACCT_PORT; 	}
(?i:"acct_server_ip4")		{ BEGIN(EXPECT_IP4); return ACCT_IP4; 		}
(?i:"acct_server_ip6")		{ BEGIN(EXPECT_IP6); return ACCT_IP6; 		}

<EXPECT_DECINT>[[:digit:]]+	{
					/* Match an integer (not hexa) */
					int ret = sscanf(yytext, "%d", &yylval->integer);
					if (ret != 1) {
						/* No matching: an error occurred */
						fd_log_debug("Unable to convert the value '%s' to a valid number: %s", yytext, strerror(errno));
						return LEX_ERROR; /* trig an error in yacc parser */
						/* Maybe we could REJECT instead of failing here? */
					}
					return INTEGER;
				}

<EXPECT_IP4,EXPECT_IP6>(?i:"disable")	{ return DISABLED; 				}
				
<EXPECT_IP4>{IP4}		{
					struct sockaddr_in * sin = NULL;
				
					sin = malloc(sizeof(struct sockaddr_in));
					if (sin == NULL) {
						fd_log_debug("Unable to allocate memory: %s", strerror(errno));
						return LEX_ERROR; /* trig an error in yacc parser */
					}
					
					memset(sin, 0, sizeof(struct sockaddr_in));
					sin->sin_family = AF_INET;
					if (inet_pton(AF_INET, yytext, &sin->sin_addr) != 1) {
						fd_log_debug("'%s' is not a valid IP address: %s", yytext, strerror(errno));
						free(sin);
						return LEX_ERROR; /* trig an error in yacc parser */
					}
					yylval->ss = (struct sockaddr *)sin;
					return IP;
				}

<EXPECT_IP6>{IP6}		{
					struct sockaddr_in6 * sin6 = NULL;
				
					sin6 = malloc(sizeof(struct sockaddr_in6));
					if (sin6 == NULL) {
						fd_log_debug("Unable to allocate memory: %s", strerror(errno));
						return LEX_ERROR; /* trig an error in yacc parser */
					}
					
					memset(sin6, 0, sizeof(struct sockaddr_in6));
					sin6->sin6_family = AF_INET6;
					if (inet_pton(AF_INET6, yytext, &sin6->sin6_addr) != 1) {
						fd_log_debug("'%s' is not a valid IPv6 address: %s", yytext, strerror(errno));
						free(sin6);
						return LEX_ERROR; /* trig an error in yacc parser */
					}
					yylval->ss = (struct sockaddr *)sin6;
					return IP;
				}

	
	/* Valid single characters for yyparse in all contexts */
<*>[=]			{ return yytext[0]; }
<*>[;]			{ BEGIN(INITIAL); return yytext[0]; }

	/* Unrecognized token */
<*>[[:alnum:]]+		|	/* This rule is only useful to print a complete token in error messages */
	/* Unrecognized character */
<*>.			{ 
				fd_log_debug("Unrecognized text on line %d col %d: '%s'.", yylloc->first_line, yylloc->first_column, yytext);
			 	return LEX_ERROR; 
			}

%%
