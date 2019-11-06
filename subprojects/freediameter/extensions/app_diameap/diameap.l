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



/*Declarations and option settings*/
%{
#include <stdio.h>
#include "diameap_common.h"
#include "diameap.tab.h"
 
#define YY_USER_ACTION { 						\
	yylloc->first_line = yylloc->last_line = yylineno; \
	yylloc->first_column = yylloc->last_column + 1; 		\
	yylloc->last_column += yyleng +1;	\
	}	
 
#define YY_NO_INPUT
%}



%option noyywrap
%option yylineno
%option bison-bridge 
%option bison-locations
%option nounput

%%
	/* List of patterns and actions */

<*>\n			{				
				yylloc->last_column=0; 
				}

<*>#.*$			{ }

[ \t\n]+		{ 	
			yylloc->last_column=0; 
				/* ignore whitespace */ 
			}

\"([^\n\"]*)\"		{
				yylval->str = strdup(yytext+1); /* Quoted String */
				yylval->str[yyleng-2]='\0';
				return iSTRING;
			}


[[:digit:]]+		{ 	/* Digital number */
				yylval->val = atoi(yytext);
				return NUM; /* Numeric value */ 
			}

(?i:"Load_Plugin")	{
				return EAPMETHOD;
			}

(?i:"Authorization")		{ 
				return AUTHORIZE;
			}

(?i:"DiamEAP_MySQL")		{ 
				return DIAMEAP_MYSQL;
			}
			
(?i:"MAX_Invalid_EAP_Packets")		{
				return MAX_INVALID_EAP_PACKET;
			}

(?i:"Multi_Round_Time_Out")		{
				return MULTI_ROUND_TIMEOUT;
			}

(?i:"Check_user_identity")		{
				return CHECK_USER_IDENTITY;
			}

"="|";"|":"|","		{	
				return yytext[0]; 
			}

	/* Unrecognized token or text */
	
<*>[[:alnum:]]+	 |					 
<*>.	{ 
				fprintf(stderr,"Unrecognized input text '%s'( on line %i column %i )\n", yytext, yylloc->first_line, yylloc->first_column); 
			 	return LEX_ERROR; 
			}

%%
/* Routines */


