/*****************************************************************************************************
 * Software License Agreement (BSD License)
 * Author : Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>
 *
 * Copyright (c) 2009-2010, Souheil Ben Ayed, Teraoka Laboratory of Keio University and the WIDE Project
 * All rights reserved.
 *
 * Redistribution and use of this software in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>.
 *
 * 4. Neither the name of Souheil Ben Ayed, Teraoka Laboratory of Keio University or the WIDE Project nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************************/

%{
#include <stdio.h>
#include "eap_tls.h"
#include "eaptls.tab.h"

void yyerror (YYLTYPE *llocp, struct tls_config * config, const char *s);

extern int yywrap();
extern int yylex();


/* The Lex parser prototype */
int eaptlslex(YYSTYPE *lvalp, YYLTYPE *llocp);


%}

%locations
%parse-param {struct tls_config * config} 
%pure_parser
/*%defines */
%error-verbose
%debug 

%union {
	char *str;
	int val;
	char byte;
}



/* In case of error in the lexical analysis */
%token 		LEX_ERROR

%token 	<val>	NUM

%token 	<str>	iSTRING

%token  <byte>	iBYTE

%token 		CERTS
%token 		CAPATH
%token 		CRLPATH
%token		CHECK_CN_USERNAME

%%	

confparams : 	/* empty */
		| confparams CERTS_files
		| confparams CA_file
		| confparams CRL_file
		| confparams CHECK_CN_USERNAME_param
		| confparams errors
		{
			return EINVAL;
		}
		;

errors :	LEX_ERROR
		| error
		;


CERTS_files :	CERTS '=' iSTRING ':' iSTRING ';'
		{
		char * certfile, *keyfile;
		FILE * fl;

		certfile = $3;
		keyfile = $5;
		if(certfile == NULL){
			TRACE_DEBUG(INFO,"%s[EAP TLS plugin] certificate file missing in configuration file",DIAMEAP_EXTENSION);
			yyerror (&yylloc, config, "cert file missing"); 
			YYERROR;
		}
		if(keyfile == NULL){
			TRACE_DEBUG(INFO,"%s[EAP TLS plugin] privateKey file missing in configuration file",DIAMEAP_EXTENSION);
			yyerror (&yylloc, config, "privateKey file missing"); 
			YYERROR;
		}
		
		fl = fopen(certfile, "r");
		if ((fl == NULL) && (*certfile != '/')) {
			char * tmpfile=certfile;
			CHECK_MALLOC_DO( certfile = malloc( strlen(tmpfile) + strlen(DEFAULT_EXTENSIONS_PATH) + 2 ),
				{ yyerror (&yylloc, config, "Not enough memory"); YYERROR; } );
			sprintf(certfile, DEFAULT_EXTENSIONS_PATH "/%s", tmpfile);
			free(tmpfile);
			fl = fopen(certfile, "r");
		}
		if (fl == NULL) {
			int ret = errno;
			TRACE_DEBUG(INFO,"%s[EAP TLS plugin] Unable to open certificate file %s for reading: %s",DIAMEAP_EXTENSION,certfile,strerror(ret));
			yyerror (&yylloc, config, "Error configuring certificate for EAP-TLS"); 
			YYERROR;
		}
		fclose(fl);
		
		fl = fopen(keyfile, "r");
		if ((fl == NULL) && (*keyfile != '/')) {
			char * tmpfile=keyfile;
			CHECK_MALLOC_DO( keyfile = malloc( strlen(tmpfile) + strlen(DEFAULT_EXTENSIONS_PATH) + 2 ),
				{ yyerror (&yylloc, config, "Not enough memory"); YYERROR; } );
			sprintf(keyfile, DEFAULT_EXTENSIONS_PATH "/%s", tmpfile);
			free(tmpfile);
			fl = fopen(keyfile, "r");
		}
		if (fl == NULL) {
			int ret = errno;
			TRACE_DEBUG(INFO,"%s[EAP TLS plugin] Unable to open privateKey file %s for reading: %s",DIAMEAP_EXTENSION,keyfile,strerror(ret));
			yyerror (&yylloc, config, "Error configuring privateKey for EAP-TLS"); 
			YYERROR;
		}
		fclose(fl);
		
		config->certfile = certfile;
		config->keyfile = keyfile;
		}
		;
		
CA_file :	CAPATH '=' iSTRING ';'
		{
		char * cafile;
		FILE * fl;

		cafile = $3;

		if(cafile == NULL){
			TRACE_DEBUG(INFO,"%s[EAP TLS plugin] CA file missing in configuration file",DIAMEAP_EXTENSION);
			yyerror (&yylloc, config, "cert file missing"); 
			YYERROR;
		}
		
		fl = fopen(cafile, "r");
		if ((fl == NULL) && (*cafile != '/')) {
			char * tmpfile=cafile;
			CHECK_MALLOC_DO( cafile = malloc( strlen(tmpfile) + strlen(DEFAULT_EXTENSIONS_PATH) + 2 ),
				{ yyerror (&yylloc, config, "Not enough memory"); YYERROR; } );
			sprintf(cafile, DEFAULT_EXTENSIONS_PATH "/%s", tmpfile);
			free(tmpfile);
			fl = fopen(cafile, "r");
		}
		if (fl == NULL) {
			int ret = errno;
			TRACE_DEBUG(INFO,"%s[EAP TLS plugin] Unable to open CA file %s for reading: %s",DIAMEAP_EXTENSION,cafile,strerror(ret));
			yyerror (&yylloc, config, "Error configuring CA file for EAP-TLS"); 
			YYERROR;
		}
		fclose(fl);
			config->cafile=$3;
		}
		;

CRL_file :	CRLPATH '=' iSTRING ';'
		{
		char * crlfile;
		FILE * fl;

		crlfile = $3;

		if(crlfile == NULL){
			TRACE_DEBUG(FULL+1,"%s[EAP TLS plugin] CRL file missing in configuration file",DIAMEAP_EXTENSION);

		}else{
		
			fl = fopen(crlfile, "r");
			if ((fl == NULL) && (*crlfile != '/')) {
				char * tmpfile=crlfile;
				CHECK_MALLOC_DO( crlfile = malloc( strlen(tmpfile) + strlen(DEFAULT_EXTENSIONS_PATH) + 2 ),
					{ yyerror (&yylloc, config, "Not enough memory"); YYERROR; } );
				sprintf(crlfile, DEFAULT_EXTENSIONS_PATH "/%s", tmpfile);
				free(tmpfile);
				fl = fopen(crlfile, "r");
			}
			if (fl == NULL) {
				int ret = errno;
				TRACE_DEBUG(INFO,"%s[EAP TLS plugin] Unable to open CRL file %s for reading: %s",DIAMEAP_EXTENSION,crlfile,strerror(ret));
				yyerror (&yylloc, config, "Error configuring CRL file for EAP-TLS"); 
				YYERROR;
			}
			fclose(fl);	
		}
			config->crlfile=$3;
		}
		;

CHECK_CN_USERNAME_param :	
		CHECK_CN_USERNAME '=' NUM ';'
		{
			if((int)$3 == 0){
				config->check_cert_cn_username = FALSE;
			}
			else
			{
				config->check_cert_cn_username = TRUE;			
			}
		}
		;
		
%%

void yyerror(YYLTYPE *llocp, struct tls_config * config,const char *str)
{
         fprintf(stderr,"Error in %s ( on line %i column %i -> line %i column %i) : %s\n",config->conffile, llocp->first_line, llocp->first_column, llocp->last_line, llocp->last_column, str);
}
