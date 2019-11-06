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

/* Sample extension to test extensions mechanism in freeDiameter */
#include <freeDiameter/extension.h>

/* from sample.cpp */
void mycppfunc();

static int sample_main(char * conffile);

/* Define the entry point. A convenience macro is provided */
EXTENSION_ENTRY("sample", sample_main);

/* The extension-specific initialization code */
static int sample_main(char * conffile)
{
	/* The debug macro from main tree can be used the same way */
	TRACE_ENTRY("%p", conffile);
	
	/* This is how we access daemon's global vars */
	fprintf(stdout, "I am extension " __FILE__ " running on host %s.", fd_g_config->cnf_diamid);
	
	/* The configuration file name is received in the conffile var. It's up to extension to parse it */
	if (conffile) {
		fprintf(stdout, "I should parse my configuration file there: %s\n", conffile);
	} else {
		fprintf(stdout, "I received no configuration file to parse\n");
	}
	
	/* Functions from the libfreediameter can also be used as demonstrated here: */
	TRACE_DEBUG(INFO, "Let's create that 'Example-AVP'...");
	{
		struct dict_object * origin_host_avp = NULL;
		struct dict_object * session_id_avp = NULL;
		struct dict_object * example_avp_avp = NULL;
		struct dict_rule_data rule_data = { NULL, RULE_REQUIRED, 0, -1, 1 };
		struct dict_avp_data example_avp_data = { 999999, 0, "Example-AVP", AVP_FLAG_VENDOR , 0, AVP_TYPE_GROUPED };

		CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Host", &origin_host_avp, ENOENT));
		CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Session-Id", &session_id_avp, ENOENT));
		
		CHECK_FCT( fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &example_avp_data , NULL, &example_avp_avp ));
		
		rule_data.rule_avp = origin_host_avp;
		rule_data.rule_min = 1;
		rule_data.rule_max = 1;
		CHECK_FCT( fd_dict_new ( fd_g_config->cnf_dict, DICT_RULE, &rule_data, example_avp_avp, NULL ));
		
		rule_data.rule_avp = session_id_avp;
		rule_data.rule_min = 1;
		rule_data.rule_max = -1;
		CHECK_FCT( fd_dict_new ( fd_g_config->cnf_dict, DICT_RULE, &rule_data, example_avp_avp, NULL ));
		
	}
	TRACE_DEBUG(INFO, "'Example-AVP' created without error");
	
	/* Call the c++ function */
	mycppfunc();
	
	/* The initialization function returns an error code with the standard POSIX meaning (ENOMEM, and so on) */
	return 0;
}

/* See file fini.c for an example of destructor */
