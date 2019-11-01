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
 * See doc/app_radgw.conf.sample for configuration file format
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
#include "rgw.h"
#include "rgw_conf.tab.h"	/* bison is not smart enough to define the YYLTYPE before including this code, so... */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/* Forward declaration */
int yyparse(char * conffile);

/* Parse the configuration file */
int rgw_conf_handle(char * conffile)
{
	extern FILE * rgw_confin;
	int ret;
	
	rgw_confin = fopen(conffile, "r");
	if (rgw_confin == NULL) {
		ret = errno;
		fd_log_debug("Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		return ret;
	}

	ret = rgw_confparse(conffile);

	fclose(rgw_confin);

	if (ret != 0) {
		return EINVAL;
	}
	
	return 0;
}

/* The Lex parser prototype */
int rgw_conflex(YYSTYPE *lvalp, YYLTYPE *llocp);

/* Function to report the errors */
void yyerror (YYLTYPE *ploc, char * conffile, char const *s)
{
	if (ploc->first_line != ploc->last_line)
		fd_log_debug("%s:%d.%d-%d.%d : %s", conffile, ploc->first_line, ploc->first_column, ploc->last_line, ploc->last_column, s);
	else if (ploc->first_column != ploc->last_column)
		fd_log_debug("%s:%d.%d-%d : %s", conffile, ploc->first_line, ploc->first_column, ploc->last_column, s);
	else
		fd_log_debug("%s:%d.%d : %s", conffile, ploc->first_line, ploc->first_column, s);
}

/* Very simple byte stack management */
static unsigned char * buf = NULL;
static size_t buf_sz = 0;
static size_t buf_rsz = 0;
static inline int buf_add(unsigned char val) /* add a value in the array */
{
	buf_sz++;
	
	if (buf_sz > buf_rsz) {
		void * rez=NULL;
		buf_rsz += 256;
		rez = realloc(buf, buf_rsz);
		if (rez == NULL) {
			fd_log_debug("Error on memory allocation: %s", strerror(errno));
			return 0;
		}
		buf = (unsigned char *)rez;
	}
	buf[buf_sz - 1] = val;
	return 1;   
}
static inline void buf_reinit(void)
{
	free(buf);
	buf = NULL;
	buf_sz = 0;
	buf_rsz = 0;
}

static int port = 0;
static char * plgconffile = NULL;

%}

/* Values returned by lex for token */
%union {
	char 		*string;	/* The string is allocated by strdup in lex.*/
	int		 integer;	/* Store integer values */
	struct sockaddr *ss;		/* sockaddr to free after use (alloc in lex) */
}

/* typed data */
%token <string>	QSTRING
%token <integer> INTEGER
%token <ss>	IP

%type <string>	FINDFILEEXT

%token <integer> NAS_OR_PXY

/* simple tokens */
%token		DISABLED
%token		AUTH
%token		ACCT

%token 		PLG_PREFIX

%token		AUTH_ENABLE
%token		AUTH_PORT
%token		AUTH_IP4
%token		AUTH_IP6
%token		ACCT_ENABLE
%token		ACCT_PORT
%token		ACCT_IP4
%token		ACCT_IP6

/* In case of error in the lexical analysis */
%token 		LEX_ERROR


/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty grammar is OK */
			| conffile plugin
			| conffile clientdef
			| conffile authserv
			| conffile acctserv
			;

				
/* -------------------------------------- */
FINDFILEEXT:		QSTRING
			{
				char * fname = $1;
				FILE * fd;
				
				/* First, check if the file exists */
				fd = fopen(fname, "r");
				if ((fd == NULL) && (*fname != '/')) {
					char * bkp = fname;
					CHECK_MALLOC_DO( fname = malloc( strlen(bkp) + strlen(DEFAULT_EXTENSIONS_PATH) + 2 ),
						{ yyerror (&yylloc, conffile, "Not enough memory"); YYERROR; } );
					sprintf(fname, DEFAULT_EXTENSIONS_PATH "/%s", bkp);
					free(bkp);
					fd = fopen(fname, "r");
				}
				if (fd == NULL) {
					int ret = errno;
					TRACE_DEBUG(INFO, "Unable to open file %s for reading: %s", fname, strerror(ret));
					yyerror (&yylloc, conffile, "Error adding plugin"); 
					YYERROR;
				}
				fclose(fd);
				
				$$ = fname;
			}
			;
/* -------------------------------------- */
plugin:			{
				/* Reset the parameters */
				buf_reinit();
				port = RGW_PLG_TYPE_AUTH | RGW_PLG_TYPE_ACCT ;
				free(plgconffile); plgconffile = NULL;
			}
			PLG_PREFIX '=' FINDFILEEXT plg_attributes ';'
			{
				/* Add this extension in the list */
				if ( rgw_plg_add( $4, plgconffile, port, &buf, buf_sz ) ) {
					yyerror (&yylloc, conffile, "Error parsing / adding extension !");
					YYERROR;
				}
				
				/* Free the array */
				buf_reinit();
				
				/* stop conffile from being freed here */
				plgconffile = NULL;
			}
			;

plg_attributes:		/* empty */
			| plg_attributes ':' QSTRING
			{
				plgconffile = $3;
			}
			| plg_attributes ':' AUTH
			{
				port = RGW_PLG_TYPE_AUTH;
			}
			| plg_attributes ':' ACCT
			{
				port = RGW_PLG_TYPE_ACCT;
			}
			| plg_attributes ':' extcodes_list
			;

extcodes_list:		/* empty */
			| extcodes_list INTEGER
			{
				if ($2 < 0 || $2 > 255) {
					yyerror (&yylloc, conffile, "Invalid command code value!");
					YYERROR;
				}
				if ( ! buf_add((unsigned char)$2) ) {
					yyerror (&yylloc, conffile, "Error allocating memory!");
					YYERROR;
				}
			}
			;
				
/* -------------------------------------- */

clientdef:		{
				buf_reinit();
			}
			NAS_OR_PXY '=' IP '/' clisecret_key ';'
			{
				/* Add this client */
				if ( rgw_clients_add( $4, &buf, buf_sz, $2 ) ) {
					yyerror (&yylloc, conffile, "Error parsing / adding client !");
					YYERROR;
				}
				
				/* reinit the buffer */
				buf_reinit();
			}
			;

clisecret_key:		/* empty */
			| clisecret_key QSTRING
			{
				int i;
				size_t len = strlen($2);
				for (i = 0; i < len; i++) {
					if ( ! buf_add( $2 [i] ) ) {
						yyerror (&yylloc, conffile, "Memory allocation error.");
						YYERROR;
					}
				}
				
				free($2);
			}
			| clisecret_key INTEGER
			{
				if ( $2 < 0 || $2 > 255 ) {
					yyerror (&yylloc, conffile, "Invalid value in key.");
					YYERROR;
				}
				
				if ( ! buf_add( $2 ) ) {
					yyerror (&yylloc, conffile, "Memory allocation error.");
					YYERROR;
				}
			}
			;

/* -------------------------------------- */

authserv:		AUTH_ENABLE '=' INTEGER ';'
			{
				if ($3 == 0) {
					rgw_servers.auth_serv.disabled = 1;
				} else {
					rgw_servers.auth_serv.disabled = 0;
				}
			}
			| AUTH_PORT '=' INTEGER ';'
			{
				if ($3 <= 0 || $3 > 65535) {
					yyerror (&yylloc, conffile, "Invalid port number !");
					YYERROR;
				}
					
				rgw_servers.auth_serv.port = htons($3);
			}
			| AUTH_IP4 '=' DISABLED ';'
			{
				rgw_servers.auth_serv.ip_disabled = 1;
			}
			| AUTH_IP4 '=' IP ';'
			{
				if (((struct sockaddr *)($3))->sa_family != AF_INET) {
					yyerror (&yylloc, conffile, "Invalid address specification !");
					YYERROR;
				}
				memcpy( & rgw_servers.auth_serv.ip_endpoint, &((struct sockaddr_in *)($3))->sin_addr, sizeof(struct in_addr) );
				free($3);
				rgw_servers.auth_serv.ip_disabled = 0;
			}
			| AUTH_IP6 '=' DISABLED ';'
			{
				rgw_servers.auth_serv.ip6_disabled = 1;
			}
			| AUTH_IP6 '=' IP ';'
			{
				if (((struct sockaddr *)($3)) -> sa_family != AF_INET6) {
					yyerror (&yylloc, conffile, "Invalid address specification !");
					YYERROR;
				}
				memcpy( & rgw_servers.auth_serv.ip6_endpoint, &((struct sockaddr_in6 *)($3))->sin6_addr, sizeof(struct in6_addr) );
				free($3);
				rgw_servers.auth_serv.ip6_disabled = 0;
			}
			;

/* -------------------------------------- */

acctserv:		ACCT_ENABLE '=' INTEGER ';'
			{
				if ($3 == 0) {
					rgw_servers.acct_serv.disabled = 1;
				} else {
					rgw_servers.acct_serv.disabled = 0;
				}
			}
			| ACCT_PORT '=' INTEGER ';'
			{
				if ($3 <= 0 || $3 > 65535) {
					yyerror (&yylloc, conffile, "Invalid port number !");
					YYERROR;
				}
					
				rgw_servers.acct_serv.port = htons($3);
			}
			| ACCT_IP4 '=' DISABLED ';'
			{
				rgw_servers.acct_serv.ip_disabled = 1;
			}
			| ACCT_IP4 '=' IP ';'
			{
				if (((struct sockaddr *)($3)) -> sa_family != AF_INET) {
					yyerror (&yylloc, conffile, "Invalid address specification !");
					YYERROR;
				}
				memcpy( & rgw_servers.auth_serv.ip_endpoint, &((struct sockaddr_in *)($3))->sin_addr, sizeof(struct in_addr) );
				free($3);
				rgw_servers.acct_serv.ip_disabled = 0;
			}
			| ACCT_IP6 '=' DISABLED ';'
			{
				rgw_servers.acct_serv.ip6_disabled = 1;
			}
			| ACCT_IP6 '=' IP ';'
			{
				if (((struct sockaddr *)($3)) -> sa_family != AF_INET6) {
					yyerror (&yylloc, conffile, "Invalid address specification !");
					YYERROR;
				}
				memcpy( & rgw_servers.auth_serv.ip6_endpoint, &((struct sockaddr_in6 *)($3))->sin6_addr, sizeof(struct in6_addr) );
				free($3);
				rgw_servers.acct_serv.ip6_disabled = 0;
			}
			;
