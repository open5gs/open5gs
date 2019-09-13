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
#include <dirent.h>
#include <libgen.h>
#include <dlfcn.h>

#ifndef BUILD_DIR
#error "Missing BUILD_DIR information"
#endif /* BUILD_DIR */


/* The number of times each operation is repeated to measure the average operation time */
#define DEFAULT_NUMBER_OF_SAMPLES	100000

static void display_result(int nr, struct timespec * start, struct timespec * end, char * fct, char * type, char *op)
{
	long double dur = (long double)end->tv_sec + (long double)end->tv_nsec/1000000000;
	dur -= (long double)start->tv_sec + (long double)start->tv_nsec/1000000000;
	long double thrp = (long double)nr / dur;
	printf("%-19s: %d %-8s %-7s in %.6LFs (%.1LFmsg/s)\n", fct, nr, type, op, dur, thrp);
}

struct ext_info {
	struct fd_list	chain;		/* link in the list */
	void 		*handler;	/* object returned by dlopen() */
	int 		(*init_cb)(int, int, char *);
	char		*ext_name;	/* points to the extension name, either inside depends, or basename(filename) */
	int		free_ext_name;	/* must be freed if it was malloc'd */
	const char 	**depends;	/* names of the other extensions this one depends on (if provided) */
};
	
static void load_all_extensions(char * prefix)
{
	DIR *dir;
	struct dirent *dp;
	char fullname[512];
	int pathlen;
	struct fd_list all_extensions = FD_LIST_INITIALIZER(all_extensions);
	struct fd_list ext_with_depends = FD_LIST_INITIALIZER(ext_with_depends);

	/* Find all extensions which have been compiled along the test */
	LOG_D("Loading %s*.fdx from: '%s'", BUILD_DIR "/extensions", prefix ?: "");
	CHECK( 0, (dir = opendir (BUILD_DIR "/extensions")) == NULL ? 1 : 0 );
	pathlen = snprintf(fullname, sizeof(fullname), BUILD_DIR "/extensions/");
	
	while ((dp = readdir (dir)) != NULL) {
		char * dot = strrchr(dp->d_name, '.');
		if (dot && ((!prefix) || !(strncmp(dp->d_name, prefix, strlen(prefix)))) && (!(strcmp(dot, ".fdx")))) {
			/* We found a file with name dict_*.fdx, attempt to load it */
			struct ext_info * new = malloc(sizeof(struct ext_info));
			CHECK( 1, new ? 1:0);
			fd_list_init(&new->chain, new);
			
			snprintf(fullname + pathlen, sizeof(fullname) - pathlen, "%s", dp->d_name);
			
			LOG_D("Extension: '%s'", dp->d_name);
			
			/* load */
			new->handler = dlopen(fullname, RTLD_NOW | RTLD_GLOBAL);
			if (!new->handler) {
				TRACE_DEBUG(INFO, "Unable to load '%s': %s.", fullname, dlerror());
			}
			CHECK( 0, new->handler == NULL ? 1 : 0 );
			
			/* resolve entry */
			new->init_cb = dlsym( new->handler, "fd_ext_init" );
			if (!new->init_cb) {
				TRACE_DEBUG(INFO, "No 'fd_ext_init' entry point in '%s': %s.", fullname, dlerror());
			}
			CHECK( 0, new->init_cb == NULL ? 1 : 0 );
			
			new->depends = dlsym( new->handler, "fd_ext_depends" );
			if (new->depends) {
				new->ext_name = (char *)new->depends[0];
				new->free_ext_name = 0;
				if ( new->depends[1] ) {
					fd_list_insert_before(&ext_with_depends, &new->chain);
				} else {
					fd_list_insert_before(&all_extensions, &new->chain);
				}
			} else {
				new->ext_name = strdup(basename(dp->d_name));
				new->free_ext_name = 1;
				fd_list_insert_before(&all_extensions, &new->chain);
			}
			
		}
	}
	
	/* Now, reorder the list by dependencies */
	{
		int count, prevcount = 0;
		struct fd_list * li;
		do {
			count = 0;
			for (li=ext_with_depends.next; li != &ext_with_depends; li=li->next) {
				struct ext_info * e = li->o;
				int d;
				int satisfied=0;
				
				/* Can we satisfy all dependencies? */
				for (d=1;  ;d++) {
					struct fd_list * eli;
					if (!e->depends[d]) {
						satisfied = 1;
						break;
					}
					
					/* can we find this dependency in the list? */
					for (eli=all_extensions.next; eli != &all_extensions; eli = eli->next) {
						struct ext_info * de = eli->o;
						if (!strcasecmp(de->ext_name, e->depends[d]))
							break; /* this dependency is satisfied */
					}
					
					if (eli == &all_extensions) {
						satisfied = 0;
						break;
					}
				}
				
				if (satisfied) {
					/* OK, we have all our dependencies in the list */
					li=li->prev;
					fd_list_unlink(&e->chain);
					fd_list_insert_before(&all_extensions, &e->chain);
				} else {
					count++;
				}
			}
			
			if (prevcount && (prevcount == count)) {
				LOG_E("Some extensions cannot have their dependencies satisfied, e.g.: %s", ((struct ext_info *)ext_with_depends.next->o)->ext_name);
				CHECK(0, 1);
			}
			prevcount = count;
			
			if (FD_IS_LIST_EMPTY(&ext_with_depends))
				break;
		} while (1);
	}
	
	/* Now, load all the extensions */
	{
		struct fd_list * li;
		for (li=all_extensions.next; li != &all_extensions; li=li->next) {
			struct ext_info * e = li->o;
			int ret = (*e->init_cb)( FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, NULL );
			LOG_N("Initializing extension '%s': %s", e->ext_name, ret ? strerror(ret) : "Success");
		}
	}
	
	/* We should probably clean the list here ? */
}

/* Main test routine */
int main(int argc, char *argv[])
{
	struct msg * acr = NULL;
	unsigned char * buf = NULL;
	
	int dictionaries_loaded = 0;
	
	test_parameter = DEFAULT_NUMBER_OF_SAMPLES;
	
	/* First, initialize the daemon modules */
	INIT_FD();
	CHECK( 0, fd_queues_init()  );
	CHECK( 0, fd_msg_init()  );
	CHECK( 0, fd_rtdisp_init()  );
	
	
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
		}
	}
	
	/* Now create some values and check the length is correctly handled */
	{
		struct dict_object * cmd_model = NULL;
		struct msg         * msg = NULL;
		struct dict_object * avp_model = NULL;
		union avp_value      value;
		
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Test-Command-Request", &cmd_model, ENOENT ) );
		
		/* Check the sizes are handled properly */
		{
			struct avp * avpi = NULL;
			struct avp * avpch = NULL;
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
			
			/* Add a vendor AVP, integer64 => size = 20 */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - i64" );
			value.i64 = 0x123456789abcdeLL;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
			
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
			}
			
			/* Now add a value which is not a constant into an enumerated AVP */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - enumi32" );
			value.i32 = -10;
			CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
			
			/* Add an octetstring AVP */
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - os" );
			{
				unsigned char buf[90];
				memcpy(&buf, "This\0 is a buffer of dat\a. It is not a string so we can have any c\0ntr\0l character here...\0\0", 89);
				value.os.data = buf;
				value.os.len = 89;
				CHECK( 0, fd_msg_avp_setvalue ( avpi, &value ) );
				memset(&buf, 0, sizeof(buf)); /* Test that the OS value is really copied */
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
				request.search.enum_name = "os const test (waaad)";
				CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &request, &value_model, ENOENT ) );
				CHECK( 0, fd_dict_getval ( value_model, &request.search ) );
				CHECK( 0, fd_msg_avp_setvalue ( avpi, &request.search.enum_value ) );
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
			}
				
			/* Now test the grouped AVPs */	
			ADD_AVP( msg, MSG_BRW_LAST_CHILD, avpi, 73565, "AVP Test - grouped" );
			  ADD_AVP( avpi, MSG_BRW_LAST_CHILD, avpch, 73565, "AVP Test - os" );
			  {
				value.os.data = (unsigned char *)"12345678";
				value.os.len = 8;
				CHECK( 0, fd_msg_avp_setvalue ( avpch, &value ) );
			  }
			  ADD_AVP( avpi, MSG_BRW_LAST_CHILD, avpch, 73565, "AVP Test - os" );
			  {
				value.os.data = (unsigned char *)"123456789";
				value.os.len = 9;
				CHECK( 0, fd_msg_avp_setvalue ( avpch, &value ) );
			  }
			
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
			
			/* Set the application to the test application: 73566 */
			msgdata->msg_appl = 73566;
			
			/* Set the hop-by-hop ID to a random value: 0x4b44b41d */
			msgdata->msg_hbhid = 0x4b44b41d;
			/* Set the end-to-end ID to a random value: 0xe2ee2e1d */
			msgdata->msg_eteid = 0xe2ee2e1d;
		}
		
		CHECK( 0, fd_msg_bufferize( msg, &buf, NULL ) );
		
		LOG_D( "Test message: %s", fd_msg_dump_treeview(FD_DUMP_TEST_PARAMS, msg, NULL, 0, 1));
		
		/* Now free the message, we keep only the buffer. */
		CHECK( 0, fd_msg_free( msg ) );
		
	}
	
	/* We have our "buf" now, length is 344 -- cf. testmesg.c. */
redo:	
	/* Test the throughput of the different functions function */
	{
		struct stress_struct {
			struct msg * m;
			uint8_t * b;
		} * stress_array;
		int i;
		struct timespec start, end;
		
		/* Create the copies of the message buffer */
		stress_array = calloc(test_parameter, sizeof(struct stress_struct));
		CHECK( stress_array ? 1 : 0, 1);
		
		for (i=0; i < test_parameter; i++) {
			stress_array[i].b = malloc(344);
			if (!stress_array[i].b)
				break;
			memcpy(stress_array[i].b, buf, 344);
		}
		CHECK( test_parameter, i ); /* if false, a malloc failed */
		
	/* fd_msg_parse_buffer */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &start) );
		
		/* Test the msg_parse_buffer function */
		for (i=0; i < test_parameter; i++) {
			if (0 != fd_msg_parse_buffer( &stress_array[i].b, 344, &stress_array[i].m) )
				break;
		}
		CHECK( test_parameter, i ); /* if false, a call failed */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &end) );
		display_result(test_parameter, &start, &end, "fd_msg_parse_buffer", "buffers", "parsed");
		
	/* fd_msg_parse_dict */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &start) );
		
		/* Test the fd_msg_parse_dict function */
		for (i=0; i < test_parameter; i++) {
			if (0 != fd_msg_parse_dict( stress_array[i].m, fd_g_config->cnf_dict, NULL ) )
				break;
		}
		CHECK( test_parameter, i ); /* if false, a call failed */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &end) );
		display_result(test_parameter, &start, &end, "fd_msg_parse_dict", "messages", "parsed");
		
		
	/* fd_msg_parse_rules */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &start) );
		
		/* Test the fd_msg_parse_rules function */
		for (i=0; i < test_parameter; i++) {
			if (0 != fd_msg_parse_rules( stress_array[i].m, fd_g_config->cnf_dict, NULL ) )
				break;
		}
		CHECK( test_parameter, i ); /* if false, a call failed */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &end) );
		display_result(test_parameter, &start, &end, "fd_msg_parse_rules", "messages", "parsed");
		
		
	/* fd_msg_new_answer_from_req (0) */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &start) );
		
		/* Test the fd_msg_new_answer_from_req function */
		for (i=0; i < test_parameter; i++) {
			if (0 != fd_msg_new_answer_from_req( fd_g_config->cnf_dict, &stress_array[i].m, 0 ) )
				break;
		}
		CHECK( test_parameter, i ); /* if false, a call failed */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &end) );
		display_result(test_parameter, &start, &end, "new_answer(normal)", "messages", "created");
		
		/* unlink answers and go back to request messages */
		for (i=0; i < test_parameter; i++) {
			struct msg * ans = stress_array[i].m;
			if (0 != fd_msg_answ_getq( ans, &stress_array[i].m ) )
				break;
			if (0 != fd_msg_answ_detach( ans ) )
				break;
			fd_msg_free( ans );
		}
		CHECK( test_parameter, i ); /* if false, a call failed */
		
		
	/* fd_msg_new_answer_from_req (MSGFL_ANSW_ERROR) */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &start) );
		
		/* Test the fd_msg_new_answer_from_req function */
		for (i=0; i < test_parameter; i++) {
			if ( 0 != fd_msg_new_answer_from_req( fd_g_config->cnf_dict, &stress_array[i].m, MSGFL_ANSW_ERROR ) )
				break;
		}
		CHECK( test_parameter, i ); /* if false, a call failed */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &end) );
		display_result(test_parameter, &start, &end, "new_answer(error)", "messages", "created");
		
		/* unlink answers and go back to request messages */
		for (i=0; i < test_parameter; i++) {
			struct msg * ans = stress_array[i].m;
			if (0 != fd_msg_answ_getq( ans, &stress_array[i].m ) )
				break;
			if (0 != fd_msg_answ_detach( ans ) )
				break;
			fd_msg_free( ans );
		}
		
		
	/* fd_msg_bufferize */
		

		CHECK( 0, clock_gettime(CLOCK_REALTIME, &start) );
		
		/* Test the fd_msg_bufferize function */
		for (i=0; i < test_parameter; i++) {
			size_t len = 0;
			if (0 != fd_msg_bufferize( stress_array[i].m, &stress_array[i].b, &len ) )
				break;
		}
		CHECK( test_parameter, i ); /* if false, a call failed */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &end) );
		display_result(test_parameter, &start, &end, "fd_msg_bufferize", "buffers", "created");
		
		
	/* fd_msg_free */
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &start) );
		
		/* Free those messages */
		for (i=0; i < test_parameter; i++) {
			fd_msg_free( stress_array[i].m );
		}
		
		CHECK( 0, clock_gettime(CLOCK_REALTIME, &end) );
		display_result(test_parameter, &start, &end, "fd_msg_free", "messages", "freed");
		
		
		for (i=0; i < test_parameter; i++) {
			free(stress_array[i].b);
		}
		free(stress_array);
	}
	
	if (!dictionaries_loaded) {
		load_all_extensions("dict_");
		dictionaries_loaded = 1;
		printf("Loaded all dictionary extensions, restarting...\n");
		goto redo;
	}
	
	/* That's all for the tests yet */
	PASSTEST();
} 
	
