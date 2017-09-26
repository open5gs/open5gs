/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2011, WIDE Project and NICT								 *
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

/* Header file for the app_redirect extension. 
 *
 *  See the app_redirect.conf.sample file for the format of the configuration file.
 */
 
/* FreeDiameter's common include file */
#include <freeDiameter/extension.h>

/* Host configuration for this specific extension */
#include <ard-host.h>
#include <regex.h>

/* Extension's configuration */
struct ard_config {
	uint32_t	default_rct; /* redirect-cache-time to use unless overwritten by a rule */
	struct fd_list  rules;	     /* the list of rules in the order they appear in the conf file. */
};
extern struct ard_config * ard_conf; /* initialized in ard_conf.y */

/* The types of redirects (from Redirect-Host-Usage AVP value) */
enum redir_h_u {
	DONT_CACHE = 0,
	ALL_SESSION,
	ALL_REALM,
	REALM_AND_APPLICATION,
	ALL_APPLICATION,
	ALL_HOST,
	ALL_USER
};
#define H_U_MAX	ALL_USER

/* A rule */
struct ard_rule {
	struct fd_list	chain; /* link in configuration */
	
	enum redir_h_u 	type; /* What kind of rule is this? */
	uint32_t	rct;  /* overwrite default_rct is not 0 */
	
	struct fd_list  criteria; /* list of criteria to match. The rule is applied if all criteria match */
	
	struct fd_list  targets;  /* list of Redirect-Host values to send. */
};

/* What kind of criteria exist */
enum rule_criteria { /* note: the order of the values reflects the complexity of matching -- it should be kept this way */
	APP_ID,
	FROM_ID,
	FROM_REALM,
	AVP_INT,
	AVP_STR
};

/* A criteria in the list */
struct ard_criteria {
	struct fd_list  chain; /* link in ard_rule->criteria */
	enum rule_criteria type; /* What is this rule */
	
	/* the data that must be matched -- everything is not used by all criteria types */
	char * s;
	size_t sl;
	int is_regex;
	regex_t  preg;
	uint32_t i;
	struct dict_avp_data avp_info;
};

/* A target entry in the ard_rule->targets list */
struct ard_target {
	struct fd_list chain;
	os0_t  s; /* must be freed afterwards */
	size_t l;
};

/* The AVPs we use */
extern struct dict_object * avp_Redirect_Host;
extern struct dict_object * avp_Redirect_Host_Usage;
extern struct dict_object * avp_Redirect_Max_Cache_Time;

/* Parse the configuration file */
int ard_conf_handle(char * conffile);

/* Dump a rule (debug) */
void ard_rule_dump(struct ard_rule * r);

/* Check if a rule applies, and if found, create the reply */
int ard_rule_apply(void * cbdata, struct msg ** msg);
