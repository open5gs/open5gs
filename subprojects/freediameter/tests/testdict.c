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

#include "tests.h"

/* Test for the dict_iterate_rules function */
int iter_test(void * data, struct dict_rule_data * rule)
{
	struct dict_avp_data avpdata;
	(*(int *)data)++;
	
	CHECK( 0, fd_dict_getval ( rule->rule_avp, &avpdata ) );
	TRACE_DEBUG(FULL, "rule #%d: avp '%s'", *(int *)data, avpdata.avp_name);
	return 0;
}

/* Main test routine */
int main(int argc, char *argv[])
{
	/* First, initialize the daemon modules */
	INIT_FD();
	
	/* Test creating and searching all types of objects */
	{
		struct dict_object * obj1 = NULL;
		struct dict_object * obj2 = NULL;
		struct dict_object * obj3 = NULL;

		vendor_id_t vendor_id = 735671;
		struct dict_vendor_data vendor1_data = { 735671, "Vendor test 1" };
		struct dict_vendor_data vendor2_data = { 735672, "Vendor test 2" };
		struct dict_application_data app1_data = { 735674, "Application test 1" };
		
		
		/* Create two vendors */
		CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_VENDOR, &vendor1_data , NULL, &obj1 ) );
		CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_VENDOR, &vendor2_data , NULL, NULL ) );
		
		/* Check we always retrieve the correct vendor object */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_ID, &vendor_id, &obj2, ENOENT ) );
		CHECK( obj1, obj2);
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_NAME, "Vendor test 1", &obj2, ENOENT ) );
		CHECK( obj1, obj2);
		
		/* Check the error conditions */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_ID, &vendor_id, NULL, ENOENT ) );
		
		vendor_id = 735673; /* Not defined */
		CHECK( ENOENT, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_ID, &vendor_id, NULL, ENOENT ) );
		CHECK( ENOENT, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_NAME, "Vendor test 3", NULL, ENOENT ) );
		CHECK( ENOENT, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_ID, &vendor_id, &obj2, ENOENT ) );
		CHECK( ENOENT, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_NAME, "Vendor test 3", &obj2, ENOENT ) );
		CHECK( ENOTSUP, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_NAME, "Vendor test 3", &obj2, ENOTSUP ) );
		
		/* Check the get_* functions */
		CHECK( 0, fd_dict_getval ( obj1, &vendor1_data ) );
		CHECK( 735671, vendor1_data.vendor_id );
		CHECK( 0, strcmp(vendor1_data.vendor_name, "Vendor test 1") );
		/* error conditions */
		CHECK( EINVAL, fd_dict_getval ( (struct dict_object *)"not an object", &vendor1_data ) );
		
		/* Create the application with vendor1 as parent */
		CHECK( EINVAL, fd_dict_new ( fd_g_config->cnf_dict, DICT_APPLICATION, &app1_data , (struct dict_object *)"bad object", &obj2 ) );
		CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_APPLICATION, &app1_data , obj1, &obj2 ) );
		
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_OF_APPLICATION, obj2, &obj3, ENOENT ) );
		CHECK( obj1, obj3);
		
		/* Creating and searching the other objects is already done in dictionary initialization */
	}

	/* Test creation of the "Example-AVP" grouped AVP from the RFC */
	{
		int nbr = 0;
		struct dict_object * origin_host_avp = NULL;
		struct dict_object * session_id_avp = NULL;
		struct dict_object * example_avp_avp = NULL;
		struct dict_rule_data rule_data = { NULL, RULE_REQUIRED, -1, -1 };
		struct dict_avp_data example_avp_data = { 999999, 0, "Example-AVP", AVP_FLAG_VENDOR , 0, AVP_TYPE_GROUPED };

		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Host", &origin_host_avp, ENOENT ) );
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Session-Id", &session_id_avp, ENOENT ) );
		
		CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &example_avp_data , NULL, &example_avp_avp ) );
		
		rule_data.rule_avp = origin_host_avp;
		rule_data.rule_min = 1;
		rule_data.rule_max = 1;
		CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_RULE, &rule_data, example_avp_avp, NULL ) );
		
		rule_data.rule_avp = session_id_avp;
		rule_data.rule_min = 1;
		rule_data.rule_max = -1;
		CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_RULE, &rule_data, example_avp_avp, NULL ) );
		
		CHECK( 0, fd_dict_iterate_rules ( example_avp_avp, &nbr, iter_test) );
		CHECK( 2, nbr );
	}
	
	/* Test list function */
	{
		struct fd_list * li = NULL;
		struct fd_list * sentinel = NULL;
		enum dict_object_type	type;
		struct dict_object * defvnd=NULL;
		vendor_id_t vid = 0;
		int first = 1;
		
		CHECK( 0, fd_dict_getlistof(VENDOR_BY_ID, fd_g_config->cnf_dict, &sentinel));
		
		for (li = sentinel; (li != sentinel) || (first != 0); li = li->next) {
			first = 0;
			CHECK(0, fd_dict_gettype(li->o, &type));
			CHECK(DICT_VENDOR, type);
#if 0
			struct dict_vendor_data data;
			CHECK( 0, fd_dict_getval(li->o, &data) );
			printf("%d : %s\n", data.vendor_id, data.vendor_name);
#endif
		}
		
		CHECK( 0, fd_dict_search(fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_ID, &vid, &defvnd, ENOENT) );
		
		CHECK( 0, fd_dict_getlistof(AVP_BY_NAME, defvnd, &sentinel));
		for (li = sentinel->next; li != sentinel; li = li->next) {
			CHECK(0, fd_dict_gettype(li->o, &type));
			CHECK(DICT_AVP, type);
#if 0
			struct dict_avp_data data;
			CHECK( 0, fd_dict_getval(li->o, &data) );
			printf("%d : %s\n", data.avp_code, data.avp_name);
#endif
		}
	}

	/* Test delete function */
	{
		struct fd_list * li = NULL;
		struct fd_list * sentinel = NULL;
		struct dict_object * obj=NULL;
		vendor_id_t vid = 0;
		int count = 0, cntbkp;
		
		CHECK( 0, fd_dict_search(fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_ID, &vid, &obj, ENOENT) );
		
		CHECK( EINVAL, fd_dict_delete(obj) );
			
		
		CHECK( 0, fd_dict_getlistof(AVP_BY_NAME, obj, &sentinel));
		obj = NULL;
		
		for (li = sentinel->next; li != sentinel; li = li->next) {
			struct dict_avp_data data;
			CHECK( 0, fd_dict_getval(li->o, &data) );
			count++;
			if (data.avp_basetype != AVP_TYPE_GROUPED)
				obj = li->o;
		}
		
		CHECK(1, obj ? 1 : 0 );
#if 1
		fd_log_debug("%s", fd_dict_dump_object(FD_DUMP_TEST_PARAMS, obj));
#endif
		CHECK( 0, fd_dict_delete(obj) );
		cntbkp = count;
		count = 0;
		for (li = sentinel->next; li != sentinel; li = li->next) {
			count++;
		}
		CHECK( 1, cntbkp - count );
		
	}
	
	LOG_D( "Dictionary at the end of %s: %s", __FILE__, fd_dict_dump(FD_DUMP_TEST_PARAMS, fd_g_config->cnf_dict) ?: "error");
	
	/* That's all for the tests yet */
	PASSTEST();
} 
	
