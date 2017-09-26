/*****************************************************************************************************
 * Software License Agreement (BSD License)
 * Author : Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>
 *
 * Copyright (c) 2009-2010, Souheil Ben Ayed, Teraoka Laboratory of Keio University, and the WIDE Project
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
#include "diameap_common.h"
#include "diameap.tab.h"

void yyerror (YYLTYPE *llocp, struct diameap_conf * config, const char *s);

extern int yywrap();
extern int yylex();


/* The Lex parser prototype */
int diameaplex(YYSTYPE *lvalp, YYLTYPE *llocp);


%}

%locations
%parse-param {struct diameap_conf * config} 
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

%token 		EAPMETHOD
%token 		AUTHORIZE
%token 		MODE
%token 		DIAMEAP_MYSQL
%token		MAX_INVALID_EAP_PACKET
%token		MULTI_ROUND_TIMEOUT
%token		CHECK_USER_IDENTITY

%%	

confparams : 	/* empty */
		| confparams EAPmethod
		| confparams Authorize
		| confparams DiamEAP_MySQL
		| confparams MAX_Invalid_EAP_Packet
		| confparams Multi_Round_Timeout
		| confparams Check_User_Identity
		| confparams errors
		{
			yyerror(&yylloc, config, "Unrecognized configuration parameter.");
			return EINVAL;
		}
		;

errors :	LEX_ERROR
		| error
		;


EAPmethod :	EAPMETHOD '=' iSTRING ':' NUM ':' NUM ':' iSTRING ':' iSTRING ';'
		{
		char * infile, *cfile;
		FILE * fl;

		infile = $9;
		fl = fopen(infile, "r");
		if ((fl == NULL) && (*infile != '/')) {
			char * tmpfile=infile;
			CHECK_MALLOC_DO( infile = malloc( strlen(tmpfile) + strlen(DEFAULT_EXTENSIONS_PATH) + 2 ),
				{ yyerror (&yylloc, config, "Not enough memory"); YYERROR; } );
			sprintf(infile, DEFAULT_EXTENSIONS_PATH "/%s", tmpfile);
			free(tmpfile);
			fl = fopen(infile, "r");
		}
		if (fl == NULL) {
			int ret = errno;
			TRACE_DEBUG(INFO, "Unable to open %s plugin file %s for reading: %s", $3, infile, strerror(ret));
			yyerror (&yylloc, config, "Error adding EAP Plugin"); 
			YYERROR;
		}
		fclose(fl);
		
		cfile = $11;

		if(strlen(cfile)>0){
			fl = fopen(cfile, "r");
			if ((fl == NULL) && (*cfile != '/')) {
				char * tmp = cfile;
				CHECK_MALLOC_DO( cfile = malloc( strlen(tmp) + strlen(DEFAULT_CONF_PATH) + 2 ),
					{ yyerror (&yylloc, config, "Not enough memory"); YYERROR; } );
				sprintf(cfile, DEFAULT_CONF_PATH "/%s", tmp);
				free(tmp);
				fl = fopen(cfile, "r");
			}
			if (fl == NULL) {
					
			}
			if(fl)
				fclose(fl);
		}		
		
		diameap_plugin_add($3,(char)$5,(char)$7,infile,cfile);
		}
		;

Authorize :	AUTHORIZE '=' NUM ';'
		{
		if((int)$3)
			config->authorize=1;
		}
		;
			

DiamEAP_MySQL :	DIAMEAP_MYSQL '=' iSTRING ',' iSTRING ',' iSTRING ',' iSTRING ';'
		{
		diameap_set_mysql_param($3,$5,$7,$9);
		}
		;

MAX_Invalid_EAP_Packet : MAX_INVALID_EAP_PACKET '=' NUM ';'
		{
		config->max_invalid_eap_packet=(int)$3;
		};

Multi_Round_Timeout : MULTI_ROUND_TIMEOUT '=' NUM ';'
		{
		config->multi_round_time_out=(unsigned int)$3;
		};

Check_User_Identity: CHECK_USER_IDENTITY '=' NUM ';'
		{
			if((int)$3){
				check_user_identity = TRUE;
			}else{
				check_user_identity = FALSE;
			}
		};
		
	
%%

void yyerror(YYLTYPE *llocp, struct diameap_conf * config,const char *str)
{
         fprintf(stderr,"Error in %s ( on line %i column %i -> line %i column %i) : %s\n",config->conffile, llocp->first_line, llocp->first_column, llocp->last_line, llocp->last_column, str);
}
