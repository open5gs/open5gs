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

/* Main test routine */
int main(int argc, char *argv[])
{
	struct msg * acr = NULL;
	struct avp * pi = NULL, *avp1, *avp2;
	unsigned char * buf = NULL;
	
	/* First, initialize the daemon modules */
	INIT_FD();
	
	/* Create the message object from model */
	{
		struct dict_object * acr_model = NULL;

		/* Now find the ACR dictionary object */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Accounting-Request", &acr_model, ENOENT ) );

		/* Create the instance, using the templates */
		CHECK( 0, fd_msg_new ( acr_model, 0, &acr ) );

		/* Check there is no child */
		CHECK( ENOENT, fd_msg_browse ( acr, MSG_BRW_FIRST_CHILD, NULL, NULL) );
		
		#if 0
		/* For debug: dump the object */
		fd_log_debug("Dumping Accounting-Request empty message:");
		fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, acr, fd_g_config->cnf_dict, 0, 1));
		#endif
	}
	
	/* Create the Proxy-Info AVP from model */
	{
		struct dict_object * pi_model = NULL;

		/* Now find the ACR dictionary object */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Proxy-Info", &pi_model, ENOENT ) );

		/* Create the instance, using the templates */
		CHECK( 0, fd_msg_avp_new ( pi_model, 0, &pi ) );

		#if 0
		/* For debug: dump the object */
		fd_log_debug("Dumping Proxy-Info AVP");
		fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, pi, fd_g_config->cnf_dict, 0, 1));
		fd_log_debug("Dumping dictionary model");
		fd_log_debug("%s", fd_dict_dump_object(FD_DUMP_TEST_PARAMS, pi_model));
		#endif
		
	}
	
	/* Get a reference to the current last AVP in the message */
	{
		int diff = 0;
		
		CHECK( 0, fd_msg_avp_new ( NULL, 0, &avp1 ) );
		CHECK( 0, fd_msg_avp_add ( acr, MSG_BRW_LAST_CHILD, avp1) );
		
		CHECK( 0, fd_msg_browse ( acr, MSG_BRW_LAST_CHILD, &avp2, &diff) );
		CHECK( 1, diff );
		CHECK( avp1, avp2 );
		
		/* Check that we cannot add this AVP to another object since it is already linked */
		CHECK( EINVAL, fd_msg_avp_add( pi, MSG_BRW_LAST_CHILD, avp1) );
	}

	/* Now add the Proxy-Info AVP at the end of the message */
	{
		CHECK( 0, fd_msg_avp_add( acr, MSG_BRW_LAST_CHILD, pi) );
		#if 0
		/* For debug: dump the object */
		fd_log_debug("Dumping Accounting-Request with Proxy-Info AVP at the end");
		fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, acr, fd_g_config->cnf_dict, 0, 1));
		#endif
	}
	
	/* Check the last child is now the proxy-Info */
	{
		CHECK( 0, fd_msg_browse ( acr, MSG_BRW_LAST_CHILD, &avp2, NULL) );
		CHECK( pi, avp2 );
	}
	
	/* Check that the avp before the proxy-info is the previous last one */
	{
		int diff = 0;
		CHECK( 0, fd_msg_browse ( pi, MSG_BRW_PREV, &avp2, &diff) );
		CHECK( avp1, avp2 );
		CHECK( 0, diff);
	}
	
	/* Check that there are no AVP after the proxy-info */
	CHECK( ENOENT, fd_msg_browse ( pi, MSG_BRW_NEXT, NULL, NULL) );
	
	/* Test the fd_msg_free function unlinks the object properly */
	{
		struct dict_object * rr_model = NULL;

		/* Now find the dictionary object */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Route-Record", &rr_model, ENOENT ) );

		/* Create the instance, using the templates */
		CHECK( 0, fd_msg_avp_new ( rr_model, 0, &avp1 ) );
		
		/* Add the AVP at the end of the message */
		CHECK( 0, fd_msg_avp_add( pi, MSG_BRW_NEXT, avp1) );
		
		/* Check the last AVP of the message is now this one */
		CHECK( 0, fd_msg_browse ( acr, MSG_BRW_LAST_CHILD, &avp2, NULL) );
		CHECK( avp1, avp2 );
		
		/* Now delete it */
		CHECK( 0, fd_msg_free( avp1 ) );
		
		/* Check the last AVP of the message is back to pi */
		CHECK( 0, fd_msg_browse ( acr, MSG_BRW_LAST_CHILD, &avp2, NULL) );
		CHECK( pi, avp2 );
		
		/* Delete the whole message */
		CHECK( 0, fd_msg_free( acr ) );
	}
	
	/* Recreate the message object */
	{
		struct dict_object * acr_model = NULL;

		/* Now find the ACR dictionary object */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Accounting-Request", &acr_model, ENOENT ) );

		/* Create the instance, using the templates */
		CHECK( 0, fd_msg_new ( acr_model, 0, &acr ) );
	}
	
	/* Now let's create some additional Dictionary objects for the test */
	{
		/* The constant values used here are totally arbitrary chosen */
		struct dict_object * vendor;
		{
			struct dict_vendor_data vendor_data = { 73565, "Vendor test" };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_VENDOR, &vendor_data , NULL, &vendor ) );
		}
		
		{
			struct dict_application_data app_data = { 73566, "Application test" };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_APPLICATION, &app_data , vendor, NULL ) );
		}
		
		{
			struct dict_avp_data avp_data = { 73567, 0, "AVP Test - no vendor - f32", 0, 0, AVP_TYPE_FLOAT32 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		
		{
			struct dict_avp_data avp_data = { 139103, 0, "AVP Test - no vendor - f64", 0, 0, AVP_TYPE_FLOAT64 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		
		{ 
			struct dict_object  * type = NULL;
			struct dict_type_data type_data = { AVP_TYPE_INTEGER64, "Int64 test" };
			struct dict_avp_data  avp_data = { 73568, 73565, "AVP Test - i64", AVP_FLAG_VENDOR, AVP_FLAG_VENDOR, AVP_TYPE_INTEGER64 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_TYPE, &type_data , NULL, &type ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , type, NULL ) );
		}
		
		{
			struct dict_object     * type = NULL;
			struct dict_type_data    type_data = { AVP_TYPE_INTEGER32, "Enum32 test" };
			struct dict_enumval_data val1 = { "i32 const test (val 1)", { .i32 = 1 } };
			struct dict_enumval_data val2 = { "i32 const test (val 2)", { .i32 = 2 } };
			struct dict_enumval_data val3 = { "i32 const test (val -5)",{ .i32 = -5 } };
			struct dict_avp_data     avp_data = { 73569, 73565, "AVP Test - enumi32", AVP_FLAG_VENDOR, AVP_FLAG_VENDOR, AVP_TYPE_INTEGER32 };
			
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_TYPE, &type_data , NULL, &type ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , type, NULL ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_ENUMVAL, &val1 , type, NULL ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_ENUMVAL, &val2 , type, NULL ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_ENUMVAL, &val3 , type, NULL ) );
		}
			
		{ 
			struct dict_object  * type = NULL;
			struct dict_type_data type_data = { AVP_TYPE_OCTETSTRING, "OS test" };
			struct dict_avp_data  avp_data = { 73570, 73565, "AVP Test - os", AVP_FLAG_VENDOR, AVP_FLAG_VENDOR, AVP_TYPE_OCTETSTRING };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_TYPE, &type_data , NULL, &type ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , type, NULL ) );
		}
		
		{
			struct dict_object     * type = NULL;
			struct dict_type_data    type_data = { AVP_TYPE_OCTETSTRING, "OS enum test" };
			struct dict_enumval_data val1 = { "os const test (Test)", { .os = { (unsigned char *)"Test", 4 } } };
			struct dict_enumval_data val2 = { "os const test (waaad)", { .os = { (unsigned char *)"waaad", 5 } } };
			struct dict_enumval_data val3 = { "os const test (waa)", { .os = { (unsigned char *)"waaad", 3 } } };
			struct dict_avp_data     avp_data = { 73571, 73565, "AVP Test - enumos", AVP_FLAG_VENDOR, AVP_FLAG_VENDOR, AVP_TYPE_OCTETSTRING };
			
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_TYPE, &type_data , NULL, &type ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , type, NULL ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_ENUMVAL, &val1 , type, NULL ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_ENUMVAL, &val2 , type, NULL ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_ENUMVAL, &val3 , type, NULL ) );
		}
		
		{
			struct dict_object * gavp = NULL;
			struct dict_avp_data avp_data = { 73572, 73565, "AVP Test - grouped", AVP_FLAG_VENDOR, AVP_FLAG_VENDOR, AVP_TYPE_GROUPED };
			
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, &gavp ) );
			
			/* Macro to search AVP and create a rule */		
			#define ADD_RULE( _parent, _vendor, _avpname, _pos, _min, _max, _ord ) {		\
				struct dict_object * _avp = NULL;						\
				struct dict_avp_request _req = { (_vendor), 0, (_avpname) };			\
				struct dict_rule_data _data;							\
				CHECK( 0, fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME_AND_VENDOR, &_req, &_avp, ENOENT));\
				_data.rule_avp = _avp;								\
				_data.rule_position = (_pos);							\
				_data.rule_order = (_ord);							\
				_data.rule_min = (_min);							\
				_data.rule_max = (_max);							\
				CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_RULE, &_data , (_parent), NULL ) );	\
			}
			
			ADD_RULE(gavp, 73565, "AVP Test - os", RULE_OPTIONAL,   -1, -1,  0);
			
		}
			
		{
			struct dict_object  * application = NULL;
			struct dict_object  * command = NULL;
			struct dict_cmd_data  cmd_data = { 73573, "Test-Command-Request", CMD_FLAG_REQUEST, CMD_FLAG_REQUEST };
			
			CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Application test", &application, ENOENT ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_COMMAND, &cmd_data , application, &command ) );
			ADD_RULE(command, 0,     "AVP Test - no vendor - f32", 	RULE_FIXED_HEAD, -1,  1,  1);
			ADD_RULE(command, 73565, "AVP Test - i64",		RULE_REQUIRED,   -1, -1,  0);
			ADD_RULE(command, 73565, "AVP Test - enumi32", 		RULE_OPTIONAL,   -1, -1,  0);
			ADD_RULE(command, 73565, "AVP Test - os", 		RULE_OPTIONAL,   -1, -1,  0);
			ADD_RULE(command, 73565, "AVP Test - enumos", 		RULE_OPTIONAL,   -1, -1,  0);
			ADD_RULE(command, 73565, "AVP Test - grouped", 		RULE_OPTIONAL,   -1, -1,  0);
		}
		
		{
			struct dict_object  * application = NULL;
			struct dict_object  * command = NULL;
			struct dict_cmd_data  cmd_data = { 73573, "Test-Command-Answer", CMD_FLAG_REQUEST, 0 };
			
			CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Application test", &application, ENOENT ) );
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_COMMAND, &cmd_data , application, &command ) );
		}
		
		{
			struct dict_object  * gavp = NULL;
			struct dict_avp_data  avp_data = { 73574, 73565, "AVP Test - rules", AVP_FLAG_VENDOR, AVP_FLAG_VENDOR, AVP_TYPE_GROUPED };
			
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, &gavp ) );
			
			ADD_RULE(gavp,     0, "AVP Test - no vendor - f32", RULE_FIXED_HEAD,   0, 1, 1);
			ADD_RULE(gavp, 73565, "AVP Test - i64", 	    RULE_FIXED_HEAD,  -1, 1, 2);
			ADD_RULE(gavp, 73565, "AVP Test - enumi32", 	    RULE_FIXED_HEAD,  -1, 1, 3);
			ADD_RULE(gavp, 73565, "AVP Test - os", 	    	    RULE_REQUIRED,     2, 3, 0);
			ADD_RULE(gavp, 73565, "AVP Test - enumos",     	    RULE_OPTIONAL,     0, 1, 0);
			ADD_RULE(gavp, 73565, "AVP Test - grouped",         RULE_FIXED_TAIL,  -1, 1, 1);
			/* ABNF : 
				< no vendor - f32 >
				< i64 >
				< enumi32 >
			    2*3 { os }
			     *1 [ enumos ]
				< grouped >
						*/
			#if 0
			fd_log_debug("%s", fd_dict_dump_object(FD_DUMP_TEST_PARAMS, gavp));
			#endif
		}
		#if 0
		{
			fd_log_debug("%s", fd_dict_dump_object(FD_DUMP_TEST_PARAMS, vendor));
		}
		#endif
	}
	
	/* Now create some values and check the length is correctly handled */
	{
		struct dict_object * cmd_model = NULL;
		struct msg         * msg = NULL;
		struct dict_object * avp_model = NULL;
		struct avp         * avp = NULL;
		union avp_value      value;
		
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Test-Command-Request", &cmd_model, ENOENT ) );
		
		/* Check an error is trigged if the AVP has no value set */
		{
			CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP,     AVP_BY_NAME,     "AVP Test - no vendor - f32", &avp_model, ENOENT ) );
			
			CHECK( 0, fd_msg_new ( cmd_model, 0, &msg ) );
			CHECK( 0, fd_msg_avp_new ( avp_model, 0, &avp ) );
			
			CHECK( 0, fd_msg_avp_add ( msg, MSG_BRW_FIRST_CHILD, avp ) );
			
			CHECK( EINVAL, fd_msg_update_length ( avp ) );
			CHECK( EINVAL, fd_msg_update_length ( msg ) );
			
			CHECK( 0, fd_msg_free( msg ) );
		}
		
		/* Check the sizes are handled properly */
		{
			struct avp * avpi = NULL;
			struct avp * avpch = NULL;
			struct avp_hdr * avpdata = NULL;
			struct msg_hdr * msgdata = NULL;
			#define ADD_AVP( _parent, _position, _avpi, _avpvendor, _avpname) {			\
				struct dict_object * _avp = NULL;						\
				struct dict_avp_request _req = { (_avpvendor), 0, (_avpname) };			\
				CHECK( 0, fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME_AND_VENDOR, &_req, &_avp, ENOENT));\
				CHECK( 0, fd_msg_avp_new ( _avp, 0, &_avpi ) );					\
				CHECK( 0, fd_msg_avp_add ( (_parent), (_position), _avpi ) );			\
			}
			/* Create a message with many AVP inside */
			CHECK( 0, fd_msg_new ( cmd_model, 0, &msg ) );
			CHECK( 0, fd_msg_hdr ( msg, &msgdata ) );
			
			/* Avp no vendor, float32 => size = 12 */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,     "AVP Test - no vendor - f32" );
			value.f32 = 3.1415;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
			CHECK( 0, fd_msg_update_length ( avpi ) );
			#if 0
			fd_log_debug("AVP no vendor, value 3.1415:");
			fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpi, fd_g_config->cnf_dict, 0, 0));
			#endif
			CHECK( 0, fd_msg_avp_hdr ( avpi, &avpdata ) );
			CHECK( 12, avpdata->avp_len );
			
			/* Check what happens when we delete the value */
			CHECK( 0, fd_msg_avp_setvalue ( avpi, NULL ) );
			CHECK( EINVAL, fd_msg_update_length ( avpi ) );
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
			
			/* Add a vendor AVP, integer64 => size = 20 */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - i64" );
			value.i64 = 0x123456789abcdeLL;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
			CHECK( 0, fd_msg_update_length ( avpi ) );
			#if 0
			fd_log_debug("AVP vendor, value 0x123456789abcdeL:");
			fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpi, fd_g_config->cnf_dict, 0, 0));
			#endif
			CHECK( 0, fd_msg_avp_hdr ( avpi, &avpdata ) );
			CHECK( 20, avpdata->avp_len );
			
			/* Check the size of the message is 20 (header) + 12 + 20 = 52 */
			CHECK( 0, fd_msg_update_length ( msg ) );
			CHECK( 52, msgdata->msg_length );
			
			/* Add an AVP with an enum value */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - enumi32" );
			{
				struct dict_object * type_model = NULL;
				struct dict_object * value_model = NULL;
				struct dict_enumval_request request;
				
				CHECK( 0, fd_msg_model ( avpi, &avp_model ) );
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_TYPE, TYPE_OF_AVP, avp_model, &type_model, ENOENT ) );
				memset(&request, 0, sizeof(request));
				request.type_obj = type_model;
				request.search.enum_name = "i32 const test (val 2)";
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &request, &value_model, ENOENT ) );
				CHECK( 0, fd_dict_getval ( value_model, &request.search ) );
				CHECK( 0, fd_msg_avp_setvalue ( avpi, &request.search.enum_value ) );
				#if 0
				fd_log_debug("AVP enum i32, value 2 (from const):");
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpi, fd_g_config->cnf_dict, 0, 0));
				#endif
			}
			
			/* Add an AVP with an enum value, negative */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - enumi32" );
			{
				struct dict_object  * type_model = NULL;
				struct dict_object  * value_model = NULL;
				struct dict_enumval_request request;
				
				CHECK( 0, fd_msg_model ( avpi, &avp_model ) );
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_TYPE, TYPE_OF_AVP, avp_model, &type_model, ENOENT ) );
				memset(&request, 0, sizeof(request));
				request.type_obj = type_model;
				request.search.enum_name = "i32 const test (val -5)";
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &request, &value_model, ENOENT ) );
				CHECK( 0, fd_dict_getval ( value_model, &request.search ) );
				CHECK( 0, fd_msg_avp_setvalue ( avpi, &request.search.enum_value ) );
				#if 0
				fd_log_debug("AVP enum i32, value -5 (from const):");
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpi, fd_g_config->cnf_dict, 0, 0));
				#endif
				/* Check the size is correct ( 12 for header + 4 for value ) */
				CHECK( 0, fd_msg_update_length ( avpi ) );
				CHECK( 0, fd_msg_avp_hdr ( avpi, &avpdata ) );
				CHECK( 16, avpdata->avp_len );
			}
			
			/* Now add a value which is not a constant into an enumerated AVP */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - enumi32" );
			value.i32 = -10;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
			CHECK( 0, fd_msg_update_length ( avpi ) );
			#if 0
			fd_log_debug("AVP vendor enum i32, value -10 (not const):");
			fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpi, fd_g_config->cnf_dict, 0, 0));
			#endif
			
			/* Add an octetstring AVP */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - os" );
			{
				unsigned char buf[90];
				memcpy(&buf, "This\0 is a buffer of dat\a. It is not a string so we can have any c\0ntr\0l character here...\0\0", 89);
				value.os.data = buf;
				value.os.len = 89;
				CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
				memset(&buf, 0, sizeof(buf)); /* Test that the OS value is really copied */
				CHECK( 0, fd_msg_update_length ( avpi ) );
				#if 0
				fd_log_debug("AVP octet string, 'This\\0 is a b...'");
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpi, fd_g_config->cnf_dict, 0, 0));
				#endif
				CHECK( 0, fd_msg_avp_hdr ( avpi, &avpdata ) );
				CHECK( 101, avpdata->avp_len );
				CHECK( 'T', avpdata->avp_value->os.data[0] );
				CHECK( 'i', avpdata->avp_value->os.data[6] );
			}

			/* Check the size of the message is 20 (header) + 12 + 20 + 16 * 3 + 101 + 3 (padding) = 204 */
			CHECK( 0, fd_msg_update_length ( msg ) );
			CHECK( 204, msgdata->msg_length );
			
			/* Add an octetstring from an enumerated constant */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - enumos" );
			{
				struct dict_object  * type_model = NULL;
				struct dict_object  * value_model = NULL;
				struct dict_enumval_request request;
				
				CHECK( 0, fd_msg_model ( avpi, &avp_model ) );
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_TYPE, TYPE_OF_AVP, avp_model, &type_model, ENOENT ) );
				memset(&request, 0, sizeof(request));
				request.type_obj = type_model;
				request.search.enum_name = "os const test (waaad)";
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &request, &value_model, ENOENT ) );
				CHECK( 0, fd_dict_getval ( value_model, &request.search ) );
				CHECK( 0, fd_msg_avp_setvalue ( avpi, &request.search.enum_value ) );
				#if 0
				fd_log_debug("AVP Enumuerated OctetString (from const):");
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpi, fd_g_config->cnf_dict, 0, 0));
				#endif
				/* Check the size is correct ( 12 for header + 5 for value ) */
				CHECK( 0, fd_msg_update_length ( avpi ) );
				CHECK( 0, fd_msg_avp_hdr ( avpi, &avpdata ) );
				CHECK( 17, avpdata->avp_len );
			}
				
			/* Add an octetstring from an enumerated constant */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - enumos" );
			{
				struct dict_object  * type_model = NULL;
				struct dict_object  * value_model = NULL;
				struct dict_enumval_request request;
				
				CHECK( 0, fd_msg_model ( avpi, &avp_model ) );
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_TYPE, TYPE_OF_AVP, avp_model, &type_model, ENOENT ) );
				memset(&request, 0, sizeof(request));
				request.type_obj = type_model;
				request.search.enum_name = "os const test (waa)";
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &request, &value_model, ENOENT ) );
				CHECK( 0, fd_dict_getval ( value_model, &request.search ) );
				CHECK( 0, fd_msg_avp_setvalue ( avpi, &request.search.enum_value ) );
				#if 0
				fd_log_debug("AVP Enumuerated OctetString (from const):");
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpi, fd_g_config->cnf_dict, 0, 0));
				#endif
				/* Check the size is correct ( 12 for header + 3 for value ) */
				CHECK( 0, fd_msg_update_length ( avpi ) );
				CHECK( 0, fd_msg_avp_hdr ( avpi, &avpdata ) );
				CHECK( 15, avpdata->avp_len );
			}
				

			/* Check the size of the message is 20 (header) + 12 + 20 + 16 * 3 + (101 + 3) + (17 + 3) + (15 + 1) = 240 */
			CHECK( 0, fd_msg_update_length ( msg ) );
			CHECK( 240, msgdata->msg_length );
			
			/* Now test the grouped AVPs */	
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - grouped" );
			  ADD_AVP( avpi, MSG_BRW_LAST_CHILD, avpch, 73565, "AVP Test - os" );
			  {
				value.os.data = (unsigned char *)"12345678";
				value.os.len = 8;
				CHECK( 0, fd_msg_avp_setvalue ( avpch, &value ) );
				#if 0
				fd_log_debug("AVP octet string, '1234678'");
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpch, fd_g_config->cnf_dict, 0, 0));
				#endif
				CHECK( 0, fd_msg_update_length ( avpch ) );
				CHECK( 0, fd_msg_avp_hdr ( avpch, &avpdata ) );
				CHECK( 20, avpdata->avp_len );
			  }
			  ADD_AVP( avpi, MSG_BRW_LAST_CHILD, avpch, 73565, "AVP Test - os" );
			  {
				value.os.data = (unsigned char *)"123456789";
				value.os.len = 9;
				CHECK( 0, fd_msg_avp_setvalue ( avpch, &value ) );
				#if 0
				fd_log_debug("AVP octet string, '12346789'");
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, avpch, fd_g_config->cnf_dict, 0, 0));
				#endif
			  }
			
			/* Check the size is updated recursively: (gavp hdr: 12) + (avp1: 20) + (avp2: 21 + 3) = 56 */
			CHECK( 0, fd_msg_update_length ( avpi ) );
			CHECK( 0, fd_msg_avp_hdr ( avpi, &avpdata ) );
			CHECK( 56, avpdata->avp_len );
			
			/* Add another similar grouped AVP, to have lot of padding */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - grouped" );
			  ADD_AVP( avpi, MSG_BRW_LAST_CHILD, avpch, 73565, "AVP Test - os" );
			  {
				value.os.data = (unsigned char *)"1";
				value.os.len = 1;
				CHECK( 0, fd_msg_avp_setvalue ( avpch, &value ) );
			  }
			  ADD_AVP( avpi, MSG_BRW_LAST_CHILD, avpch, 73565, "AVP Test - os" );
			  {
				value.os.data = (unsigned char *)"1234567";
				value.os.len = 7;
				CHECK( 0, fd_msg_avp_setvalue ( avpch, &value ) );
			  }
			
			/* Now check the global size of the message, if padding is correctly handled */
			/* size = 20 (header) + 12 + 20 + 16 * 3 + (101 + 3) + (17 + 3) + (15 + 1) 
			 *        + ( 12 + ( 20 + 21) + 3 )         # padding for the grouped AVP = 3
			 *        + ( 12 + ( (13 + 3) + 19 ) + 1 )  # and 1 for this one
			 * size = 240 + 56 + 48 = 344
			 */
			CHECK( 0, fd_msg_update_length ( msg ) );
			#if 0
			fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, msg, fd_g_config->cnf_dict, 0, 1));
			#endif
			CHECK( 344, msgdata->msg_length );
			
			/* Set the application to the test application: 73566 */
			msgdata->msg_appl = 73566;
			
			/* Set the hop-by-hop ID to a random value: 0x4b44b41d */
			msgdata->msg_hbhid = 0x4b44b41d;
			/* Set the end-to-end ID to a random value: 0xe2ee2e1d */
			msgdata->msg_eteid = 0xe2ee2e1d;
		}
		
		/* Test the msg_bufferize function */
		{
			
			CHECK( 0, fd_msg_bufferize( msg, &buf, NULL ) );
			
			/* Test the first bytes */
			CHECK( 0x01, buf[0] ); /* Version */
			CHECK( 0x00, buf[1] ); /* Length: 344 = 0x000158 */
			CHECK( 0x01, buf[2] );
			CHECK( 0x58, buf[3] );
			CHECK( 0x80, buf[4] ); /* flags: only "R" is set. */
			CHECK( 0x01, buf[5] ); /* Command code: 73573 = 0x011F65 */
			CHECK( 0x1F, buf[6] );
			CHECK( 0x65, buf[7] );
			CHECK( 0x00, buf[8] ); /* App ID: 73566 = 0x00011F5E */
			CHECK( 0x01, buf[9] ); 
			CHECK( 0x1F, buf[10] );
			CHECK( 0x5E, buf[11] );
			CHECK( 0x4b, buf[12] ); /* hop-by-hop id: 0x4b44b41d */
			CHECK( 0x44, buf[13] );
			CHECK( 0xb4, buf[14] );
			CHECK( 0x1d, buf[15] );
			CHECK( 0xe2, buf[16] ); /* end-to-end id: 0xe2ee2e1d */
			CHECK( 0xee, buf[17] );
			CHECK( 0x2e, buf[18] );
			CHECK( 0x1d, buf[19] );
			
			CHECK( 0x00, buf[20] ); /* First AVP (AVP Test - no vendor - f32) begin: code 73567 = 0x00011F5F */
			CHECK( 0x01, buf[21] );
			CHECK( 0x1F, buf[22] );
			CHECK( 0x5F, buf[23] );
			CHECK( 0x00, buf[24] ); /* flags: 0 */
			CHECK( 0x00, buf[25] ); /* length: 12 = 0x00000c */
			CHECK( 0x00, buf[26] );
			CHECK( 0x0C, buf[27] );
			CHECK( 0x40, buf[28] ); /* Value: 3.1415:  sign = '+' => most significant bit = 0 */
			CHECK( 0x49, buf[29] ); /* 2 <= 3.1415 < 4 => exponent = 1 => biaised (on 8 bits) = (decimal) 128 = (binary) 100 0000 0 */
			CHECK( 0x0e, buf[30] ); /* significand = (decimal) 1.57075 = (binary) 1.100 1001 0000 1110 0101 0110 */
			CHECK( 0x56, buf[31] ); /* total => 0100 0000 0100 1001 0000 1110 0101 0110 = (hexa) 40 49 0e 56*/
			
			/* The other AVPs will be tested by successful parsing... */
		}
		
		/* Now free the message, we keep only the buffer. */
		CHECK( 0, fd_msg_free( msg ) );
		
	}
	
	/* Test the parsing of buffers and messages */
	{
		unsigned char * buf_cpy = NULL;
		struct msg * msg;
		
		#define CPYBUF() {			\
			buf_cpy = malloc(344);		\
			CHECK( buf_cpy ? 1 : 0, 1);	\
			memcpy(buf_cpy, buf, 344);	\
		}
		
		/* Test the msg_parse_buffer function */
		{
			CPYBUF();
			CHECK( EBADMSG, fd_msg_parse_buffer( &buf_cpy, 340, &msg) );
			
			CPYBUF();
			CHECK( 0, fd_msg_parse_buffer( &buf_cpy, 344, &msg) );
			#if 0
			fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, msg, fd_g_config->cnf_dict, 0, 1));
			#endif
			
			/* reinit the msg */
			CHECK( 0, fd_msg_free ( msg ) );
				
		}
		
		/* Test the fd_msg_search_avp function */
		{
			struct dict_object * avp_model;
			struct avp 	   * found;
			struct avp_hdr     * avpdata = NULL;
			
			/* Now find the ACR dictionary object */
			CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "AVP Test - no vendor - f32", &avp_model, ENOENT ) );
			
			CPYBUF();
			CHECK( 0, fd_msg_parse_buffer( &buf_cpy, 344, &msg) );
			
			/* Search this AVP instance in the msg */
			CHECK( 0, fd_msg_search_avp( msg, avp_model, &found ) );
			
			/* Check the AVP value is 3.1415 */
			CHECK( 0, fd_msg_avp_hdr ( found, &avpdata ) );
			CHECK( 3.1415F, avpdata->avp_value->f32 );
			
			/* reinit the msg */
			CHECK( 0, fd_msg_free ( msg ) );
				
		}
		
		/* Test the msg_parse_dict function */
		{
			/* Test with an unknown command code */
			{
				CPYBUF();
				
				/* Change the command-code */
				buf_cpy[5] = 0x11;
				CHECK( 0, fd_msg_parse_buffer( &buf_cpy, 344, &msg) );
				CHECK( ENOTSUP, fd_msg_parse_dict( msg, fd_g_config->cnf_dict, NULL ) );
				
				/* reset */
				CHECK( 0, fd_msg_free ( msg ) );
			}
			
			/* Test with an unknown Mandatory AVP */
			{
				CPYBUF();
				
				buf_cpy[20] = 0x11;	/* New AVP code = 0x11011F5F, undefined */
				buf_cpy[24] = 0x40; 	/* Add the 'M' flag */
				
				/* Check that we cannot support this message now */
				CHECK( 0, fd_msg_parse_buffer( &buf_cpy, 344, &msg) );
				CHECK( ENOTSUP, fd_msg_parse_dict( msg, fd_g_config->cnf_dict, NULL ) );
				
				/* reset */
				CHECK( 0, fd_msg_free ( msg ) );
			}
			
			/* Test with an unknown optional AVP */
			{
				CPYBUF();
				
				buf_cpy[20] = 0x11;	/* New AVP code = 0x11011F5F, undefined */
				
				/* Check that we can support this message now */
				CHECK( 0, fd_msg_parse_buffer( &buf_cpy, 344, &msg) );
				CHECK( 0, fd_msg_parse_dict( msg, fd_g_config->cnf_dict, NULL ) );
				
				#if 0
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, msg, fd_g_config->cnf_dict, 0, 1));
				#endif
				
				/* reset */
				CHECK( 0, fd_msg_free ( msg ) );
			}
			
			/* Test with an invalid AVP (definition mismatch with the dictionary) */
			{
				CPYBUF();
				
				buf_cpy[21] = 0x02;	/* New AVP code = 0x00021F5F, f64 type in the dictionary */
				
				
				/* Check that we cannot support this message now */
				CHECK( 0, fd_msg_parse_buffer( &buf_cpy, 344, &msg) );
				CHECK( EBADMSG, fd_msg_parse_dict( msg, fd_g_config->cnf_dict, NULL ) );
				
				/* reset */
				CHECK( 0, fd_msg_free ( msg ) );
			}
			
			{
				unsigned char * buftmp = NULL;
				struct msg * error;
				/* Check the parse or error works as expected */
				CPYBUF();
				
				buf_cpy[21] = 0x02;	/* New AVP code = 0x00021F5F, f64 type in the dictionary */
				
				/* Check that we cannot support this message now */
				CHECK( 0, fd_msg_init() );
				CHECK( 0, fd_msg_parse_buffer( &buf_cpy, 344, &msg) );
				CHECK( EBADMSG, fd_msg_parse_or_error( &msg, &error ) );
				CHECK( NULL, msg );
				msg = error;
				
				CHECK( 0, fd_msg_bufferize( msg, &buftmp, NULL ) );				
				
				fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, msg, fd_g_config->cnf_dict, 0, 1));
				
				TODO("Check the Failed-AVP is as expected");
				
				
				/* reset */
				CHECK( 0, fd_msg_free ( msg ) );
				free(buftmp);
			}
			
			
			CHECK( 0, fd_msg_parse_buffer( &buf, 344, &msg) );
			CHECK( 0, fd_msg_parse_dict( msg, fd_g_config->cnf_dict, NULL ) );
			#if 0
			fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, msg, fd_g_config->cnf_dict, 0, 1));
			#endif
		}
		
		/* Now test the msg_parse_rule function */
		{
			struct fd_pei pei;
			
			CHECK( 0, fd_msg_parse_rules( msg, fd_g_config->cnf_dict, &pei ) );
			
			/* Use the "AVP Test - rules" AVP to test the rules */
			{
				struct avp * tavp = NULL;
				struct avp * tempavp = NULL;
				struct avp * childavp = NULL;
				
				ADD_AVP( msg, MSG_BRW_LAST_CHILD, tavp, 73565, "AVP Test - rules" );
				
				/* Create a conforming message first */
				ADD_AVP( tavp, MSG_BRW_LAST_CHILD, childavp,     0, "AVP Test - no vendor - f32" );
				ADD_AVP( tavp, MSG_BRW_LAST_CHILD, childavp, 73565, "AVP Test - i64" );
				ADD_AVP( tavp, MSG_BRW_LAST_CHILD, childavp, 73565, "AVP Test - enumi32" );
				ADD_AVP( tavp, MSG_BRW_LAST_CHILD, childavp, 73565, "AVP Test - os" );
				ADD_AVP( tavp, MSG_BRW_LAST_CHILD, childavp, 73565, "AVP Test - os" );
				ADD_AVP( tavp, MSG_BRW_LAST_CHILD, childavp, 73565, "AVP Test - grouped" );
				
				/* Check the message is still conform */
				CHECK( 0, fd_msg_parse_rules( msg, fd_g_config->cnf_dict, &pei ) );
				
				/* The first avp is optional in fixed position, so remove it and check the message is still OK */
				CHECK( 0, fd_msg_browse ( tavp, MSG_BRW_FIRST_CHILD, &childavp, NULL) );
				CHECK( 0, fd_msg_free ( childavp ) );
				CHECK( 0, fd_msg_parse_rules( msg, fd_g_config->cnf_dict, &pei ) );
				ADD_AVP( tavp, MSG_BRW_FIRST_CHILD, childavp,     0, "AVP Test - no vendor - f32" );
				
				
				/* Now break some rules and check it is detected */
				#define CHECK_CONFLICT( _msg, _error, _conflictavp_name, _conflictavp_vnd )		{	\
					struct fd_pei _pei;									\
					CHECK( EBADMSG,  fd_msg_parse_rules( _msg, fd_g_config->cnf_dict, &_pei ) );		\
					if (_error) {										\
						CHECK( 0, strcmp( _error, _pei.pei_errcode ) );					\
					}											\
					if ((_conflictavp_name) == NULL) {							\
						CHECK( NULL, _pei.pei_avp);							\
					} else {										\
						struct dict_avp_request _req = { (_conflictavp_vnd), 0, (_conflictavp_name) };	\
						struct dict_object *    _avp;							\
						struct dict_object * _conflict;							\
						CHECK( 1, (_pei.pei_avp) ? 1 : 0 );						\
						CHECK( 0, fd_msg_model( _pei.pei_avp, &_conflict ) );				\
						CHECK( 0, fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME_AND_VENDOR, &_req, &_avp, ENOENT));	\
						CHECK( _avp, _conflict );							\
					}											\
				}

			/* ABNF : 
				< no vendor - f32 >
				< i64 >
				< enumi32 >
			    2*3 { os }
			     *1 [ enumos ]
				< grouped >
						*/
				{
					/* Test the FIXED_HEAD rules positions: add another AVP before the third */
					CHECK( 0, fd_msg_browse ( tavp, MSG_BRW_FIRST_CHILD, &tempavp, NULL) ); /* tempavp is the novendor avp */
					CHECK( 0, fd_msg_browse ( tempavp, MSG_BRW_NEXT, &tempavp, NULL) );     /* tempavp is the i64 avp */
					ADD_AVP( tempavp, MSG_BRW_NEXT, childavp, 73565, "AVP Test - os" );
					
					CHECK_CONFLICT( msg, "DIAMETER_MISSING_AVP", "AVP Test - enumi32", 73565 );
					
					/* Now remove this AVP */
					CHECK( 0, fd_msg_free ( childavp ) );
				}
				{
					/* Remove the third AVP, same rule must conflict */
					CHECK( 0, fd_msg_browse ( tempavp, MSG_BRW_NEXT, &childavp, NULL) );     /* childavp is the enumi32 avp */
					CHECK( 0, fd_msg_free ( childavp ) );
					
					CHECK_CONFLICT( msg, "DIAMETER_MISSING_AVP", "AVP Test - enumi32", 73565 );
					
					/* Add the AVP back */
					ADD_AVP( tempavp, MSG_BRW_NEXT, childavp, 73565, "AVP Test - enumi32" );
				}
				
				{
					/* Test the minimum value in the REQUIRED rule: delete one of the os AVPs */
					CHECK( 0, fd_msg_browse ( childavp, MSG_BRW_NEXT, &tempavp, NULL) );     /* tempavp is the os avp */
					CHECK( 0, fd_msg_free ( tempavp ) );
					
					CHECK_CONFLICT( msg, "DIAMETER_MISSING_AVP", "AVP Test - os", 73565 ); /* The rule requires at least 2 AVP, we have only 1 */
					
					/* Now add this AVP */
					ADD_AVP( childavp, MSG_BRW_NEXT, tempavp, 73565, "AVP Test - os" );
				}
				{
					/* Test the maximum value in the REQUIRED rule: add more of the os AVPs */
					ADD_AVP( childavp, MSG_BRW_NEXT, tempavp, 73565, "AVP Test - os" );
					ADD_AVP( childavp, MSG_BRW_NEXT, tempavp, 73565, "AVP Test - os" );
					
					CHECK_CONFLICT( msg, "DIAMETER_AVP_OCCURS_TOO_MANY_TIMES", "AVP Test - os", 73565 ); /* The rule requires at most 3 AVP, we have 4 */
					
					/* Now delete these AVP */
					CHECK( 0, fd_msg_free ( tempavp ) );
					CHECK( 0, fd_msg_browse ( childavp, MSG_BRW_NEXT, &tempavp, NULL) );
					CHECK( 0, fd_msg_free ( tempavp ) );
				}
				
				{
					/* Test the maximum value in the OPTIONAL rule: add 2 enumos AVPs */
					ADD_AVP( childavp, MSG_BRW_NEXT, tempavp, 73565, "AVP Test - enumos" );
					
					/* The message is still conform */
					CHECK( 0, fd_msg_parse_rules( msg, fd_g_config->cnf_dict, &pei ) );
					
					/* Now break the rule */
					ADD_AVP( childavp, MSG_BRW_NEXT, tempavp, 73565, "AVP Test - enumos" );
					
					CHECK_CONFLICT( msg, "DIAMETER_AVP_OCCURS_TOO_MANY_TIMES", "AVP Test - enumos", 73565 );
					
					/* Now delete this AVP */
					CHECK( 0, fd_msg_free ( tempavp ) );
				}
				
				{
					/* Test the RULE_FIXED_TAIL rules positions: add another AVP at the end */
					ADD_AVP( tavp, MSG_BRW_LAST_CHILD, childavp, 73565, "AVP Test - os" );
					
					CHECK_CONFLICT( msg, "DIAMETER_MISSING_AVP", "AVP Test - grouped", 73565 );
					
					/* Now remove this AVP */
					CHECK( 0, fd_msg_free ( childavp ) );
				}
			}
		}
		
		/* Test the fd_msg_new_answer_from_req function */
		{
			struct dict_object * cmd_model = NULL;
			struct msg         * msg = NULL;
			struct avp * pi1, *pi2, *avp;
			char * host1="host1", * host2="host2";
			union avp_value      value;
			struct msg_hdr * msgdata = NULL;
			
			CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Test-Command-Request", &cmd_model, ENOENT ) );
			
			/* Test default behavior without flags */
			{ 
				/* Create a message with some AVPs inside */
				CHECK( 0, fd_msg_new ( cmd_model, 0, &msg ) );
				CHECK( 0, fd_msg_hdr ( msg, &msgdata ) );
				
				/* Add a session id */
				CHECK( 0, fd_msg_new_session( msg, (os0_t)"testmsg", strlen("testmsg") ) );
				
				/* Create two instances of Proxy-Info */
				ADD_AVP( msg, MSG_BRW_LAST_CHILD, pi1, 0, "Proxy-Info");
				ADD_AVP( msg, MSG_BRW_LAST_CHILD, pi2, 0, "Proxy-Info");

				ADD_AVP( pi1, MSG_BRW_LAST_CHILD, avp, 0, "Proxy-State");
				value.os.data = (os0_t)"ps_pi1";
				value.os.len = strlen((char *)value.os.data);
				CHECK( 0, fd_msg_avp_setvalue ( avp, &value ) );
				
				ADD_AVP( pi2, MSG_BRW_LAST_CHILD, avp, 0, "Proxy-State");
				value.os.data = (os0_t)"pi2_state";
				value.os.len = strlen((char *)value.os.data);
				CHECK( 0, fd_msg_avp_setvalue ( avp, &value ) );
				
				ADD_AVP( pi1, MSG_BRW_FIRST_CHILD, avp, 0, "Proxy-Host");
				value.os.data = (os0_t)host1;
				value.os.len = strlen(host1);
				CHECK( 0, fd_msg_avp_setvalue ( avp, &value ) );
				
				ADD_AVP( pi2, MSG_BRW_LAST_CHILD, avp, 0, "Proxy-Host");
				value.os.data = (os0_t)host2;
				value.os.len = strlen(host2);
				CHECK( 0, fd_msg_avp_setvalue ( avp, &value ) );
				
				ADD_AVP( pi2, MSG_BRW_LAST_CHILD, avp, 73565, "AVP Test - i64");
				value.i64 = 0x123456789abcdeLL;
				CHECK( 0, fd_msg_avp_setvalue ( avp, &value ) );
				
				
				/* Now call the fd_msg_new_answer_from_req function */
				CHECK( 0, fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, &msg, 0 ) );
				
				/* Check there is a Session-Id AVP */
				{
					struct session * sess;
					int new;
					CHECK( 0, fd_msg_sess_get(fd_g_config->cnf_dict, msg, &sess, &new) );
					CHECK( 1, sess == NULL ? 0 : 1 );
					CHECK( 0, new ? 1 : 0 );
				}
				
				/* Check there are two Proxy-Info with the two hosts */
				{
					int got_h1 = 0, got_h2=0;
					CHECK( 0, fd_msg_browse ( msg, MSG_BRW_FIRST_CHILD, &avp, NULL) );
					while(avp) {
						struct avp_hdr * avpdata = NULL;
						CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
						if (avpdata->avp_code == AC_PROXY_INFO) {
							struct avp * iavp;
							CHECK( 0, fd_msg_browse ( avp, MSG_BRW_FIRST_CHILD, &iavp, NULL) );
							while(iavp) {
								struct avp_hdr * iavpdata = NULL;
								CHECK( 0, fd_msg_avp_hdr ( iavp, &iavpdata ) );
								if (iavpdata->avp_code == AC_PROXY_HOST) {
									if (!memcmp(host1, iavpdata->avp_value->os.data, strlen(host1)))
										got_h1++;
									if (!memcmp(host2, iavpdata->avp_value->os.data, strlen(host2)))
										got_h2++;
								}
								CHECK( 0, fd_msg_browse ( iavp, MSG_BRW_NEXT, &iavp, NULL) );
							}
						}
						
						CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
					}
					
					CHECK(1, got_h1);
					CHECK(1, got_h2);
				}
				
			}
			
		}
	}
	
	/* Test the msg_avp_value_interpret and msg_avp_value_encode functions. use the Address type and Host-IP-Address AVPs */
	{
		struct dict_object * cer_model = NULL;
		struct msg * cer = NULL;
		
		struct dict_object * hia_model = NULL;
		struct avp *avp4, *avp6;
		#define TEST_IP4 "192.168.100.101"
		char buf4[INET_ADDRSTRLEN];
		#define	TEST_IP6 "1111:2222:3333:4444:1234:5678:9abc:def0"
		char buf6[INET6_ADDRSTRLEN];
		
		struct sockaddr_storage ss;
		struct sockaddr_in  sin,  *psin;
		struct sockaddr_in6 sin6, *psin6;
		
		/* Find the CER dictionary object */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Capabilities-Exchange-Request", &cer_model, ENOENT ) );

		/* Now find the Host-IP-Address dictionary object */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Host-IP-Address", &hia_model, ENOENT ) );

		/* Create the msg instance */
		CHECK( 0, fd_msg_new ( cer_model, 0, &cer ) );

		/* Create the avp instances */
		CHECK( 0, fd_msg_avp_new ( hia_model, 0, &avp4 ) );
		CHECK( 0, fd_msg_avp_new ( hia_model, 0, &avp6 ) );
		
		/* Set the value of the IP avp */
		sin.sin_family = AF_INET;
		CHECK( 1, inet_pton( AF_INET, TEST_IP4, &sin.sin_addr.s_addr ) );
		CHECK( 0, fd_msg_avp_value_encode ( &sin, avp4 ) );
		
		/* Set the value of the IP6 avp */
		sin6.sin6_family = AF_INET6;
		CHECK( 1, inet_pton( AF_INET6, TEST_IP6, &sin6.sin6_addr.s6_addr ) );
		CHECK( 0, fd_msg_avp_value_encode ( &sin6, avp6 ) );
		
		/* Add these AVPs in the message */
		CHECK( 0, fd_msg_avp_add( cer, MSG_BRW_LAST_CHILD, avp4) );
		CHECK( 0, fd_msg_avp_add( cer, MSG_BRW_LAST_CHILD, avp6) );
		
		/* Create the buffer for this message */
		CHECK( 0, fd_msg_bufferize( cer, &buf, NULL ) );
		
		/* Now free the message, we keep only the buffer. */
		CHECK( 0, fd_msg_free( cer ) );
		
		/* Check the content of the buffer is correct (skip command header) */
		CHECK( 0x00, buf[20] ); /* First AVP (IP4) begins: code 257 = 0x00000101 */
		CHECK( 0x00, buf[21] );
		CHECK( 0x01, buf[22] );
		CHECK( 0x01, buf[23] );
		CHECK( 0x40, buf[24] ); /* flags: M */
		CHECK( 0x00, buf[25] ); /* length: 8+6 = 0x00000e */
		CHECK( 0x00, buf[26] );
		CHECK( 0x0E, buf[27] );
		CHECK( 0x00, buf[28] ); /* Value: AddressType 1 */
		CHECK( 0x01, buf[29] ); 
		CHECK(  192, buf[30] ); /* 192.168.100.101 */
		CHECK(  168, buf[31] ); 
		CHECK(  100, buf[32] ); 
		CHECK(  101, buf[33] );
		
		CHECK( 0x00, buf[34] ); /* Padding */
		CHECK( 0x00, buf[35] );
		
		CHECK( 0x00, buf[36] ); /* Second AVP (IP6) begins: code 257 = 0x00000101 */
		CHECK( 0x00, buf[37] );
		CHECK( 0x01, buf[38] );
		CHECK( 0x01, buf[39] );
		CHECK( 0x40, buf[40] ); /* flags: M */
		CHECK( 0x00, buf[41] ); /* length: 8+18 = 0x00001a */
		CHECK( 0x00, buf[42] );
		CHECK( 0x1A, buf[43] );
		CHECK( 0x00, buf[44] ); /* Value: AddressType 2 */
		CHECK( 0x02, buf[45] ); 
		CHECK( 0x11, buf[46] ); /* 1111:2222:3333:4444:1234:5678:9abc:def0 */
		CHECK( 0x11, buf[47] ); 
		CHECK( 0x22, buf[48] ); 
		CHECK( 0x22, buf[49] );
		CHECK( 0x33, buf[50] );
		CHECK( 0x33, buf[51] );
		CHECK( 0x44, buf[52] );
		CHECK( 0x44, buf[53] );
		CHECK( 0x12, buf[54] );
		CHECK( 0x34, buf[55] );
		CHECK( 0x56, buf[56] );
		CHECK( 0x78, buf[57] );
		CHECK( 0x9a, buf[58] );
		CHECK( 0xbc, buf[59] );
		CHECK( 0xde, buf[60] );
		CHECK( 0xf0, buf[61] );
		
		/* Ok, now let's recreate the message */
		CHECK( 0, fd_msg_parse_buffer( &buf, 64, &cer) );
		CHECK( 0, fd_msg_parse_dict( cer, fd_g_config->cnf_dict, NULL ) );
		
		/* Get the pointers to the first and last AVP */
		CHECK( 0, fd_msg_browse( cer, MSG_BRW_FIRST_CHILD, &avp4, NULL) );
		CHECK( 0, fd_msg_browse( cer, MSG_BRW_LAST_CHILD,  &avp6, NULL) );
		
		/* Try and interpret the data in the AVPs */
		CHECK( 0, fd_msg_avp_value_interpret ( avp4, &ss ) );
		psin = (struct sockaddr_in *)&ss;
		CHECK( AF_INET, psin->sin_family );
		CHECK( 0, (inet_ntop( AF_INET, &psin->sin_addr.s_addr, buf4, sizeof(buf4) ) == NULL) ? errno : 0 );
		CHECK( 0, strcmp( buf4, TEST_IP4 ) );
		
		CHECK( 0, fd_msg_avp_value_interpret ( avp6, &ss ) );
		psin6 = (struct sockaddr_in6 *)&ss;
		CHECK( AF_INET6, psin6->sin6_family );
		CHECK( 0, (inet_ntop( AF_INET6, &psin6->sin6_addr.s6_addr, buf6, sizeof(buf6) ) == NULL) ? errno : 0 );
		CHECK( 0, strcasecmp( buf6, TEST_IP6 ) );
		
		/* Ok, it's done */
		CHECK( 0, fd_msg_free( cer ) );
	}
	
	/* Check proper encoding / decoding for all basic types of AVP */
	{
		{
			struct dict_avp_data avp_data = { 91001, 0, "AVP Test 2 - os", 0, 0, AVP_TYPE_OCTETSTRING };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		{
			struct dict_avp_data avp_data = { 91002, 0, "AVP Test 2 - i32", 0, 0, AVP_TYPE_INTEGER32 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		{
			struct dict_avp_data avp_data = { 91003, 0, "AVP Test 2 - i64", 0, 0, AVP_TYPE_INTEGER64 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		{
			struct dict_avp_data avp_data = { 91004, 0, "AVP Test 2 - u32", 0, 0, AVP_TYPE_UNSIGNED32 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		{
			struct dict_avp_data avp_data = { 91005, 0, "AVP Test 2 - u64", 0, 0, AVP_TYPE_UNSIGNED64 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		{
			struct dict_avp_data avp_data = { 91006, 0, "AVP Test 2 - f32", 0, 0, AVP_TYPE_FLOAT32 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		{
			struct dict_avp_data avp_data = { 91007, 0, "AVP Test 2 - f64", 0, 0, AVP_TYPE_FLOAT64 };
			CHECK( 0, fd_dict_new ( fd_g_config->cnf_dict, DICT_AVP, &avp_data , NULL, NULL ) );
		}
		
		{
			struct dict_object * cmd_model = NULL;
			struct msg         * msg = NULL;
			struct avp         * avp = NULL;
			union avp_value      value;
			struct avp         * avpi = NULL;
			struct avp_hdr     * avpdata = NULL;
			struct msg_hdr     * msgdata = NULL;

			CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Test-Command-Request", &cmd_model, ENOENT ) );

			/* Create a message */
			CHECK( 0, fd_msg_new ( cmd_model, 0, &msg ) );
			CHECK( 0, fd_msg_hdr ( msg, &msgdata ) );
			
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0, 	"AVP Test 2 - os" );
			value.os.data = (unsigned char *) "waaad";
			value.os.len = 6;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );

			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,	"AVP Test 2 - i32" );
			value.i32 = 0x123456;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );

			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,	"AVP Test 2 - i32" );
			value.i32 = -0x123456;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );

			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,	"AVP Test 2 - i64" );
			value.i64 = 0x11223344556677LL;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );

			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,	"AVP Test 2 - i64" );
			value.i64 = -0x11223344556677LL;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );

			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,	"AVP Test 2 - u32" );
			value.u32 = 0xFEDCBA98;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );

			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,	"AVP Test 2 - u64" );
			value.u64 = 0x123456789abcdef0LL;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );

			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,	"AVP Test 2 - f32" );
			value.f32 = 2097153.0F;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );

			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 0,	"AVP Test 2 - f64" );
			value.f64 = -1099511627777LL;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
			
			/* Ok now bufferize */
			CHECK( 0, fd_msg_bufferize( msg, &buf, NULL ) );
			
			/* Test the first bytes */
			CHECK( 0x01, buf[0] ); /* Version */
			CHECK( 0x00, buf[1] ); /* Length: 148 = 0x000094 */
			CHECK( 0x00, buf[2] );
			CHECK( 0x94, buf[3] );
			CHECK( 0x80, buf[4] ); /* flags: only "R" is set. */
			CHECK( 0x01, buf[5] ); /* Command code: 73573 = 0x011F65 */
			CHECK( 0x1F, buf[6] );
			CHECK( 0x65, buf[7] );
			CHECK( 0x00, buf[8] ); /* App ID */
			CHECK( 0x01, buf[9] ); 
			CHECK( 0x1F, buf[10] );
			CHECK( 0x5E, buf[11] );
			CHECK( 0x00, buf[12] ); /* hop-by-hop id */
			CHECK( 0x00, buf[13] );
			CHECK( 0x00, buf[14] );
			CHECK( 0x00, buf[15] );
			CHECK( 0x00, buf[16] ); /* end-to-end id */
			CHECK( 0x00, buf[17] );
			CHECK( 0x00, buf[18] );
			CHECK( 0x00, buf[19] );
			
			CHECK( 0x00, buf[20] ); /* First AVP (AVP Test 2 - os) begin: code 91001 = 0x00016379 */
			CHECK( 0x01, buf[21] );
			CHECK( 0x63, buf[22] );
			CHECK( 0x79, buf[23] );
			CHECK( 0x00, buf[24] ); /* flags: 0 */
			CHECK( 0x00, buf[25] ); /* length: 14 = 0x00000e */
			CHECK( 0x00, buf[26] );
			CHECK( 0x0e, buf[27] );
			
			CHECK( 0x77, buf[28] ); /* "waaad\0" + padding */
			CHECK( 0x61, buf[29] );
			CHECK( 0x61, buf[30] );
			CHECK( 0x61, buf[31] );
			CHECK( 0x64, buf[32] );
			CHECK( 0x00, buf[33] );
			CHECK( 0x00, buf[34] );
			CHECK( 0x00, buf[35] );
			
			/* 36 ~ 43 : 2nd AVP header (size at last octet) */
			CHECK( 0x0c, buf[43] );
			CHECK( 0x00, buf[44] ); /* 0x123456 stored in integer32 in network byte order */
			CHECK( 0x12, buf[45] );
			CHECK( 0x34, buf[46] );
			CHECK( 0x56, buf[47] );
			
			/* 48 ~ 55 : next AVP header */
			CHECK( 0xff, buf[56] ); /* -0x123456 stored in integer32 in network byte order. */ 
			CHECK( 0xed, buf[57] ); /* We assume that two's complement is the correct representation, although it's not clearly specified. */
			CHECK( 0xcb, buf[58] ); /* 00 12 34 56 inversed => FF ED CB A9 */
			CHECK( 0xaa, buf[59] ); /* then "+1" => FF ED CB AA */
			
			/* 60 ~ 67 : next header */
			CHECK( 0x10, buf[67] ); /* (the size) */
			CHECK( 0x00, buf[68] ); /* 0x11223344556677 in network byte order */
			CHECK( 0x11, buf[69] );
			CHECK( 0x22, buf[70] );
			CHECK( 0x33, buf[71] );
			CHECK( 0x44, buf[72] );
			CHECK( 0x55, buf[73] );
			CHECK( 0x66, buf[74] );
			CHECK( 0x77, buf[75] );
			
			/* 76 ~ 83 : next header */
			CHECK( 0xFF, buf[84] ); /*  - 0x11223344556677 (in two's complement) */
			CHECK( 0xEE, buf[85] ); /* gives FF EE DD CC BB AA 99 89 */
			CHECK( 0xDD, buf[86] );
			CHECK( 0xCC, buf[87] );
			CHECK( 0xBB, buf[88] );
			CHECK( 0xAA, buf[89] );
			CHECK( 0x99, buf[90] );
			CHECK( 0x89, buf[91] );
			
			/* 92 ~ 99 : next header */
			CHECK( 0x0c, buf[99] ); /* (the size) */
			CHECK( 0xFE, buf[100]); /* 0xFEDCBA98 in network byte order */
			CHECK( 0xDC, buf[101]);
			CHECK( 0xBA, buf[102]);
			CHECK( 0x98, buf[103]);
			
			/* 104 ~ 111 : next header */
			CHECK( 0x10, buf[111] ); /* (the size) */
			CHECK( 0x12, buf[112]); /* 0x123456789abcdef0LL in network byte order */
			CHECK( 0x34, buf[113]);
			CHECK( 0x56, buf[114]);
			CHECK( 0x78, buf[115]);
			CHECK( 0x9a, buf[116]);
			CHECK( 0xbc, buf[117]);
			CHECK( 0xde, buf[118]);
			CHECK( 0xf0, buf[119]);
			
			/* 120 ~ 127 : next header */
			CHECK( 0x0c, buf[127] ); /* (the size) */
			CHECK( 0x4a, buf[128]); /* http://en.wikipedia.org/wiki/IEEE_754-1985 to get descvription of the format */
			CHECK( 0x00, buf[129]); /* v = 2097153 = 2^21 + 2 ^ 0; sign : "+", 2^21 <= v < 2^22 => exponent = 21; biaised on 8 bits => 21 + 127 => 100 1010 0 */
			CHECK( 0x00, buf[130]); /* v = (+1) * (1 ^ 21) * ( 1 + 2^-21 ) => significand 000 0000 0000 0000 0000 0100 */
			CHECK( 0x04, buf[131]); /* result: 4a 00 00 04 */
			
			/* 132 ~ 139 : next header */
			CHECK( 0x10, buf[139] ); /* (the size) */
			CHECK( 0xc2, buf[140]); /* -1099511627777L ( 2^40 + 1 ) in network byte order */
			CHECK( 0x70, buf[141]); /* sign: - => most significant bit = 1 */
			CHECK( 0x00, buf[142]); /* 2^40 <= v < 2^41 => biaised exponent on 11 bits: 1023 + 40: 100 0010  0111 */
			CHECK( 0x00, buf[143]); /* significand: 1 + 2^-40 => 0000  0000 0000  0000 0000  0000 0000  0000 0000  0001 0000  0000 0000 */
			CHECK( 0x00, buf[144]); /* result: c2 70 00 00 00 00 10 00 */
			CHECK( 0x00, buf[145]);
			CHECK( 0x10, buf[146]);
			CHECK( 0x00, buf[147]);
			
			
			
			/* Okay, now delete the message and parse the buffer, then check we obtain the same values back */
			#if 0
			fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, msg, fd_g_config->cnf_dict, 0, 1));
			#endif
			CHECK( 0, fd_msg_free( msg ) );
			
			CHECK( 0, fd_msg_parse_buffer( &buf, 148, &msg) );
			CHECK( 0, fd_msg_parse_dict( msg, fd_g_config->cnf_dict, NULL ) );
			#if 0
			fd_log_debug("%s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, msg, fd_g_config->cnf_dict, 0, 1));
			#endif
			
			CHECK( 0, fd_msg_browse ( msg, MSG_BRW_FIRST_CHILD, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( 6, avpdata->avp_value->os.len );
			CHECK( 'w', (char)(avpdata->avp_value->os.data[0]) );
			CHECK( 'a', (char)(avpdata->avp_value->os.data[1]) );
			CHECK( 'd', (char)(avpdata->avp_value->os.data[4]) );
			CHECK( '\0', (char)(avpdata->avp_value->os.data[5]) );
			
			CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( 0x123456, avpdata->avp_value->i32 );
			
			CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( -0x123456, avpdata->avp_value->i32 );
			
			CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( 0x11223344556677LL, avpdata->avp_value->i64 );
			
			CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( -0x11223344556677LL, avpdata->avp_value->i64 );
			
			CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( 0xFEDCBA98, avpdata->avp_value->u32 );
			
			CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( 0x123456789abcdef0LL, avpdata->avp_value->u64 );
			
			CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( 2097153.0F, avpdata->avp_value->f32 );
			
			CHECK( 0, fd_msg_browse ( avp, MSG_BRW_NEXT, &avp, NULL) );
			CHECK( 0, fd_msg_avp_hdr ( avp, &avpdata ) );
			CHECK( -1099511627777LL, avpdata->avp_value->f64 );
			
			CHECK( 0, fd_msg_free( msg ) );
		}
	}
	

	/* That's all for the tests yet */
	PASSTEST();
} 
	
