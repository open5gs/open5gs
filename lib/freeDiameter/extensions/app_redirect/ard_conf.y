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
 * See doc/app_redirect.conf.sample for configuration file format
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
#include "app_redir.h"
#include "ard_conf.tab.h"	/* bison is not smart enough to define the YYLTYPE before including this code, so... */

/* Forward declaration */
int yyparse(char * conffile);

static int rules_added = 0;

/* We initialize statically the config */
static struct ard_config local_conf = { .default_rct = 86400, .rules = FD_LIST_INITIALIZER(local_conf.rules) };
struct ard_config * ard_conf = &local_conf;

/* We use these lists in the rules parsing */
static struct fd_list temp_list_criteria = FD_LIST_INITIALIZER(temp_list_criteria);
static struct fd_list temp_list_target = FD_LIST_INITIALIZER(temp_list_target);

/* Local variable */
static struct ard_criteria * c;

/* Dump the configuration */
static void ard_conf_dump()
{
	struct fd_list * li;
	if (!TRACE_BOOL(FULL))
		return;
		
	fd_log_debug("app_redirect: configuration dump:");
	fd_log_debug("   default_redirect_cache_time : %u sec", ard_conf->default_rct);
	for (li = ard_conf->rules.next; li != &ard_conf->rules; li = li->next) {
		ard_rule_dump(li->o);
	}
	fd_log_debug("app_redirect: end of configuration dump");
}

/* Parse the configuration file */
int ard_conf_handle(char * conffile)
{
	extern FILE * ard_confin;
	int ret;
	
	TRACE_ENTRY("%p", conffile);
	
	TRACE_DEBUG (FULL, "Parsing configuration file: %s...", conffile);
	
	ard_confin = fopen(conffile, "r");
	if (ard_confin == NULL) {
		ret = errno;
		TRACE_DEBUG(INFO, "Unable to open extension configuration file %s for reading: %s", conffile, strerror(ret));
		return ret;
	}

	ret = yyparse(conffile);

	fclose(ard_confin);

	if (ret != 0) {
		TRACE_DEBUG (INFO, "Unable to parse the configuration file.");
		return EINVAL;
	} else {
		TRACE_DEBUG(FULL, "Added %d Redirect RULES successfully.", rules_added);
		ard_conf_dump();
	}
	
	return 0;
}

/* The Lex parser prototype */
int ard_conflex(YYSTYPE *lvalp, YYLTYPE *llocp);

/* Function to report the errors */
void yyerror (YYLTYPE *ploc, char * conffile, char const *s)
{
	TRACE_DEBUG(INFO, "Error in configuration parsing");
	
	if (ploc->first_line != ploc->last_line) {
		TRACE_DEBUG (INFO, "%s:%d.%d-%d.%d : %s", conffile, ploc->first_line, ploc->first_column, ploc->last_line, ploc->last_column, s);
	} else if (ploc->first_column != ploc->last_column) {
		TRACE_DEBUG (INFO, "%s:%d.%d-%d : %s", conffile, ploc->first_line, ploc->first_column, ploc->last_column, s);
	} else {
		TRACE_DEBUG (INFO, "%s:%d.%d : %s", conffile, ploc->first_line, ploc->first_column, s);
	}
}

/* Compile a regular expression pattern */
static int compile_regex( regex_t  * preg, char * str )
{
	int err;
	
	/* Compile the regular expression */
	err = regcomp(preg, str, REG_EXTENDED | REG_NOSUB);
	if (err != 0) {
		char * buf;
		size_t bl;
		
		/* Error while compiling the regex */
		TRACE_DEBUG(INFO, "Error while compiling the regular expression '%s':", str);
		
		/* Get the error message size */
		bl = regerror(err, preg, NULL, 0);
		
		/* Alloc the buffer for error message */
		CHECK_MALLOC( buf = malloc(bl) );
		
		/* Get the error message content */
		regerror(err, preg, buf, bl);
		TRACE_DEBUG(INFO, "\t%s", buf);
		
		/* Free the buffer, return the error */
		free(buf);
		return EINVAL;
	}

	return 0;
}


%}

/* Values returned by lex for token */
%union {
	/* returned by lex */
	uint32_t	 u32;	/* Store integer values */
	struct {
		char * str;
		int regex; /* true or false */
	}	 tstring;	/* typed string */
}

/* In case of error in the lexical analysis */
%token 		TOK_LEX_ERROR

/* A string (malloc'd in lex parser; it must be freed after use):*/
%token <tstring> TOK_TSTRING

/* An integer value */
%token <u32> 	TOK_U32VAL

%type <u32>	rule_type
%type <u32>	rule_duration

/* Tokens */
%token 		TOK_DEFAULT_RCT

%token 		TOK_TO

%token 		TOK_DONT_CACHE
%token 		TOK_ALL_SESSION
%token 		TOK_ALL_REALM
%token 		TOK_REALM_AND_APPLICATION
%token 		TOK_ALL_APPLICATION
%token 		TOK_ALL_HOST
%token 		TOK_ALL_USER

%token 		TOK_FROM_ID
%token 		TOK_FROM_REALM

%token 		TOK_APP





/* -------------------------------------- */
%%

	/* The grammar definition */
conffile:		/* empty grammar is OK */
			| conffile def_rct
			| conffile rule
			;
			
	/* Overwrite default cache time value */
def_rct:		TOK_DEFAULT_RCT '=' TOK_U32VAL ';'
			{
				ard_conf->default_rct = $3;
			}
			;
			
	/* a RULE entry */
rule:			rule_type rule_duration ':' criteria_list TOK_TO target_list ';'
			{
				struct ard_rule * r;
				/* Create the new rule with data in file */
				CHECK_MALLOC_DO( r = malloc(sizeof(struct ard_rule)),
					{
						yyerror (&yylloc, conffile, "Error while allocating new memory...");
						YYERROR;
					} );
				memset(r, 0, sizeof(struct ard_rule));
				fd_list_init(&r->chain, r);
				r->type = $1;
				r->rct = $2;
				fd_list_init(&r->criteria, NULL);
				fd_list_move_end(&r->criteria, &temp_list_criteria);
				fd_list_init(&r->targets, NULL);
				fd_list_move_end(&r->targets, &temp_list_target);
				
				/* Add the new rule in config */
				fd_list_insert_before(&ard_conf->rules, &r->chain);
				rules_added++;
			}
			;

rule_type:		TOK_DONT_CACHE
			{
				$$ = DONT_CACHE;
			}
			| TOK_ALL_SESSION
			{
				$$ = ALL_SESSION;
			}
			| TOK_ALL_REALM
			{
				$$ = ALL_REALM;
			}
			| TOK_REALM_AND_APPLICATION
			{
				$$ = REALM_AND_APPLICATION;
			}
			| TOK_ALL_APPLICATION
			{
				$$ = ALL_APPLICATION;
			}
			| TOK_ALL_HOST
			{
				$$ = ALL_HOST;
			}
			| TOK_ALL_USER
			{
				$$ = ALL_USER;
			}
			;
			
rule_duration:		/* empty */
			{
				$$ = 0;
			}
			|
			TOK_U32VAL
			{
				$$ = $1;
			}
			;

criteria_list:		/* empty is OK */
			| criteria_list criteria_item;
			;

criteria_item:		{
				/* Create the new criteria */
				CHECK_MALLOC_DO( c = malloc(sizeof(struct ard_criteria)),
					{
						yyerror (&yylloc, conffile, "Error while allocating new memory...");
						YYERROR;
					} );
				memset(c, 0, sizeof(struct ard_criteria));
				fd_list_init(&c->chain, c);
			}
			criteria_item_inside
			{
				struct fd_list * li;
				/* If there is a string, save its length */
				if (c->s)
					c->sl = strlen(c->s);
				/* If the criteria contains a regex, parse it */
				if (c->is_regex) {
					CHECK_FCT_DO( compile_regex( &c->preg, c->s ),
						{
							yyerror (&yylloc, conffile, "Error parsing a regular expression...");
							YYERROR;
						} );
				}
				
				/* Now link this new criteria in the list. Order by criteria type to accelerate the search */
				for (li = temp_list_criteria.next; li != &temp_list_criteria; li = li->next) {
					struct ard_criteria * nc = li->o;
					if (nc->type >= c->type)
						break;
				}
				fd_list_insert_before(li, &c->chain);
			}
			;

criteria_item_inside:	TOK_FROM_ID '=' TOK_TSTRING
			{
				c->type = FROM_ID;
				c->s = $3.str;
				c->is_regex = $3.regex;
			}
			|
			TOK_FROM_REALM '=' TOK_TSTRING
			{
				c->type = FROM_REALM;
				c->s = $3.str;
				c->is_regex = $3.regex;
			}
			|
			TOK_APP '=' TOK_U32VAL
			{
				c->type = APP_ID;
				c->i = $3;
			}
			|
			TOK_TSTRING '=' TOK_U32VAL
			{
				struct dict_object * avp = NULL;
				if ($1.regex) {
					yyerror(&yylloc, conffile, "Error: the AVP name cannot be specified as regular expression");
					YYERROR;
				}
				CHECK_FCT_DO( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME_ALL_VENDORS, $1.str, &avp, ENOENT ),
					{
						TRACE_DEBUG(INFO, "Error while searching for AVP '%s'. Did you load the relevant dictionary extensions?", $1.str);
						yyerror(&yylloc, conffile, "Unable to resolve specified AVP name.");
						YYERROR;
					}  );
				CHECK_FCT_DO( fd_dict_getval(avp, &c->avp_info),
					{
						TRACE_DEBUG(INFO, "Error while retrieving the description for AVP '%s'", $1.str);
						yyerror(&yylloc, conffile, "Unable to retrieve specified AVP's data.");
						YYERROR;
					}  );
				if (c->avp_info.avp_basetype != AVP_TYPE_UNSIGNED32) {
					TRACE_DEBUG(INFO, "The AVP '%s' is not of type UNSIGNED32, matching is not supported (yet)", $1.str);
					yyerror(&yylloc, conffile, "Invalid AVP for this operation.");
					YYERROR;
				}
				
				c->type = AVP_INT;
				c->i = $3;
			}
			|
			TOK_TSTRING '=' TOK_TSTRING
			{
				struct dict_object * avp = NULL;
				if ($1.regex) {
					yyerror(&yylloc, conffile, "Error: the AVP name cannot be specified as regular expression");
					YYERROR;
				}
				CHECK_FCT_DO( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME_ALL_VENDORS, $1.str, &avp, ENOENT ),
					{
						TRACE_DEBUG(INFO, "Error while searching for AVP '%s'. Did you load the relevant dictionary extensions?", $1.str);
						yyerror(&yylloc, conffile, "Unable to resolve specified AVP name.");
						YYERROR;
					}  );
				CHECK_FCT_DO( fd_dict_getval(avp, &c->avp_info),
					{
						TRACE_DEBUG(INFO, "Error while retrieving the description for AVP '%s'", $1.str);
						yyerror(&yylloc, conffile, "Unable to retrieve specified AVP's data.");
						YYERROR;
					}  );
				if (c->avp_info.avp_basetype != AVP_TYPE_OCTETSTRING) {
					TRACE_DEBUG(INFO, "The AVP '%s' is not of type OCTETSTRING, matching is not supported (yet)", $1.str);
					yyerror(&yylloc, conffile, "Invalid AVP for this operation.");
					YYERROR;
				}
				
				c->type = AVP_STR;
				c->s = $3.str;
				c->is_regex = $3.regex;
			}
			;

target_list:		/* This list cannot be empty */
			target_item
			| target_list target_item
			;

target_item:		TOK_TSTRING
			{
				struct ard_target * t;
				
				if ($1.regex) {
					yyerror(&yylloc, conffile, "Regular expressions are not allowed in Redirect-Host specification.");
					YYERROR;
				}
				
				/* Check if the format is valid */
				CHECK_FCT_DO( fd_os_parse_DiameterURI((uint8_t *)$1.str, strlen($1.str), NULL, NULL, NULL, NULL, NULL, NULL),
					{
						TRACE_DEBUG(INFO, "Error while parsing DiameterURI '%s'", $1.str);
						yyerror(&yylloc, conffile, "Specified DiameterURI is invalid.");
						YYERROR;
					}  );
				
				/* Ok. we create the new target */
				CHECK_MALLOC_DO( t = malloc(sizeof(struct ard_target)),
					{
						yyerror (&yylloc, conffile, "Error while allocating new memory...");
						YYERROR;
					} );
				memset(t, 0, sizeof(struct ard_target));
				fd_list_init(&t->chain, t);
				
				t->s = (os0_t) $1.str;
				t->l = strlen($1.str);
				
				fd_list_insert_before(&temp_list_target, &t->chain);
			}
			;
