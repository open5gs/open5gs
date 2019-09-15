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

/* Yacc parser for echo/drop plugin. 
See doc/echodrop.rgwx.conf.sample for description of the parsed format. */


/* For development only : */
%debug 
%error-verbose

/* The parser receives the configuration file filename and the conf structure as parameter */
%parse-param {char * conffile}
%parse-param {struct rgwp_config *cs}

/* Keep track of location */
%locations 
%pure-parser

%{
#include "rgwx_echodrop.h"
#include "rgwx_echodrop.tab.h"	/* bison is not smart enough to define the YYLTYPE before including this code, so... */


/* Forward declaration */
int yyparse(char * conffile, struct rgwp_config *cs);

/* The Lex parser prototype */
int rgwx_echodroplex(YYSTYPE *lvalp, YYLTYPE *llocp);


/* Parse the configuration file */
int ed_conffile_parse(char * conffile, struct rgwp_config *cs)
{
	extern FILE * rgwx_echodropin;
	int ret;
	
	rgwx_echodropin = fopen(conffile, "r");
	if ((rgwx_echodropin == NULL) && (*conffile != '/')) { /* We received a relative path, try adding DEFAULT_CONF_PATH prefix */
		char * fullpath;
		CHECK_MALLOC( fullpath = malloc( strlen(conffile) + strlen(DEFAULT_CONF_PATH) + 2 ) );
		sprintf( fullpath, DEFAULT_CONF_PATH "/%s", conffile );
		rgwx_echodropin = fopen(fullpath, "r");
		free(fullpath);
	}
	if (rgwx_echodropin == NULL) {
		ret = errno;
		fd_log_debug("[echodrop.rgwx] Unable to open plugin configuration file %s for reading: %s", conffile, strerror(ret));
		return ret;
	}

	ret = rgwx_echodropparse(conffile, cs);

	fclose(rgwx_echodropin);

	if (ret != 0) {
		return EINVAL;
	}
	
	return 0;
}

/* Function to report the errors */
void yyerror (YYLTYPE *ploc, char * conffile, struct rgwp_config *cs, char const *s)
{
	if (ploc->first_line != ploc->last_line)
		fd_log_debug("%s:%d.%d-%d.%d : %s", conffile, ploc->first_line, ploc->first_column, ploc->last_line, ploc->last_column, s);
	else if (ploc->first_column != ploc->last_column)
		fd_log_debug("%s:%d.%d-%d : %s", conffile, ploc->first_line, ploc->first_column, ploc->last_column, s);
	else
		fd_log_debug("%s:%d.%d : %s", conffile, ploc->first_line, ploc->first_column, s);
}

static struct {
	struct {
		unsigned vendor :1;
		unsigned tlv :1;
		unsigned ext :1;
	};
	uint8_t		type;
	uint16_t	extype;
	uint32_t	vendor_id;
} attrinfo;


%}

/* Values returned by lex for tokens */
%union {
	unsigned	integer;	/* Value  */
}

/* typed data */
%token <integer> INTEGER
%type  <integer> action

/* simple tokens */
%token		TOK_ECHO
%token		TOK_DROP
%token		TOK_CODE
%token		TOK_VENDOR
%token		TOK_TLV
%token		TOK_EXT

/* In case of error in the lexical analysis */
%token 		LEX_ERROR


/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty grammar is OK */
			| conffile attrdef
			;

	/* An attribute line */				
attrdef:		{
				memset(&attrinfo, 0, sizeof(attrinfo));
			}
			action TOK_CODE INTEGER vendordef ';'
			{
				struct ed_conf_attribute * new;
				struct fd_list * li;

				if ($4 >= 256) {
					yyerror (&yylloc, conffile, cs, "Too big value for attribute CODE");
					YYERROR;
				}
				
				/* Create a new list item */
				CHECK_MALLOC_DO( new = malloc(sizeof(struct ed_conf_attribute)), 
					{
						yyerror (&yylloc, conffile, cs, "Memory allocation error");
						YYERROR;
					} );
				memset(new, 0, sizeof(struct ed_conf_attribute));
				
				fd_list_init(&new->chain, NULL);
				
				new->action = $2;
				new->vsa = attrinfo.vendor;
				new->tlv = attrinfo.tlv;
				new->ext = attrinfo.ext;
				
				if (new->vsa)
					new->vendor_id = attrinfo.vendor_id;
				if (new->tlv)
					new->type = attrinfo.type;
				if (new->ext)
					new->extype = attrinfo.extype;
				
				new->code = $4;
				
				/* Now place this attribute in the list */
				for (li = cs->attributes.next; li != &cs->attributes; li = li->next) {
					struct ed_conf_attribute *eca = (struct ed_conf_attribute *)li;
					/* Order first by attribute code */
					if (eca->code > new->code)
						break;
					if (eca->code < new->code)
						continue;
					
					/* Then by VSA flag */
					if (! new->vsa)
						break;
					if (! eca->vsa)
						continue;
					
					/* Then by vendor value */
					if (eca->vendor_id >= new->vendor_id)
						break;
				}
				
				fd_list_insert_before(li, &new->chain);
			}
			;
	
	/* What to do with the specified attribute */
action:			TOK_ECHO
			{
				$$ = ACT_ECHO;
			}
			|
			TOK_DROP
			{
				$$ = ACT_DROP;
			}
			;

	/* Vendor specifics, if any */
vendordef:		/* empty OK */
			| TOK_VENDOR INTEGER specif
			{
				attrinfo.vendor_id = $2;
				attrinfo.vendor = 1;
			}
			;
			
	/* Any additional specification ? */
specif:			/* empty OK */
			| TOK_TLV INTEGER
			{
				if ($2 >= (1 << 8)) {
					yyerror (&yylloc, conffile, cs, "Too big value for TLV type");
					YYERROR;
				}
				attrinfo.type = $2;
				attrinfo.tlv = 1;
			}
			| TOK_EXT INTEGER
			{
				if ($2 >= (1 << 16)) {
					yyerror (&yylloc, conffile, cs, "Too big value for Ext-Type");
					YYERROR;
				}
				attrinfo.extype = $2;
				attrinfo.ext = 1;
				yyerror (&yylloc, conffile, cs, "The EXT option is not supported in this version.");
				YYERROR;
			}
			;	

