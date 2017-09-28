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


#include "diameap_common.h"

/* Dictionary Object templates */
struct dict_object * dataobj_diameap_cmd = NULL; /* Diameter-EAP-Request Command Code */
struct dict_object * dataobj_diameap_app = NULL; /* Diameter EAP Application object */
struct dict_object * dataobj_diameap_ven = NULL; /* Diameter EAP Application Vendor object */
struct dict_object * dataobj_accounting_eap_auth_method = NULL;
struct dict_object * dataobj_acct_interim_interval = NULL;
struct dict_object * dataobj_auth_application_id = NULL;
struct dict_object * dataobj_auth_grace_period = NULL;
struct dict_object * dataobj_auth_request_type = NULL;
struct dict_object * dataobj_auth_session_state = NULL;
struct dict_object * dataobj_authorization_lifetime = NULL;
struct dict_object * dataobj_callback_id = NULL;
struct dict_object * dataobj_callback_number = NULL;
struct dict_object * dataobj_called_station_id = NULL;
struct dict_object * dataobj_calling_station_id = NULL;
struct dict_object * dataobj_class = NULL;
struct dict_object * dataobj_configuration_token = NULL;
struct dict_object * dataobj_connect_info = NULL;
struct dict_object * dataobj_destination_host = NULL;
struct dict_object * dataobj_destination_realm = NULL;
struct dict_object * dataobj_eap_master_session_key = NULL;
struct dict_object * dataobj_eap_key_name = NULL;
struct dict_object * dataobj_eap_payload = NULL;
struct dict_object * dataobj_eap_reissued_payload = NULL;
struct dict_object * dataobj_error_message = NULL;
struct dict_object * dataobj_error_reporting_host = NULL;
struct dict_object * dataobj_failed_avp = NULL;
struct dict_object * dataobj_filter_id = NULL;
struct dict_object * dataobj_framed_appletalk_link = NULL;
struct dict_object * dataobj_framed_appletalk_network = NULL;
struct dict_object * dataobj_framed_appletalk_zone = NULL;
struct dict_object * dataobj_framed_compression = NULL;
struct dict_object * dataobj_framed_interface_id = NULL;
struct dict_object * dataobj_framed_ip_address = NULL;
struct dict_object * dataobj_framed_ip_netmask = NULL;
struct dict_object * dataobj_framed_ipv6_prefix = NULL;
struct dict_object * dataobj_framed_ipv6_pool = NULL;
struct dict_object * dataobj_framed_ipv6_route = NULL;
struct dict_object * dataobj_framed_ipx_network = NULL;
struct dict_object * dataobj_framed_mtu = NULL;
struct dict_object * dataobj_framed_pool = NULL;
struct dict_object * dataobj_framed_protocol = NULL;
struct dict_object * dataobj_framed_route = NULL;
struct dict_object * dataobj_framed_routing = NULL;
struct dict_object * dataobj_idle_timeout = NULL;
struct dict_object * dataobj_multi_round_time_out = NULL;
struct dict_object * dataobj_nas_filter_rule = NULL;
struct dict_object * dataobj_nas_identifier = NULL;
struct dict_object * dataobj_nas_ip_address = NULL;
struct dict_object * dataobj_nas_ipv6_address = NULL;
struct dict_object * dataobj_nas_port = NULL;
struct dict_object * dataobj_nas_port_id = NULL;
struct dict_object * dataobj_nas_port_type = NULL;
struct dict_object * dataobj_originating_line_info = NULL;
struct dict_object * dataobj_origin_host = NULL;
struct dict_object * dataobj_origin_realm = NULL;
struct dict_object * dataobj_origin_state_id = NULL;
struct dict_object * dataobj_port_limit = NULL;
struct dict_object * dataobj_proxy_info = NULL;
struct dict_object * dataobj_qos_filter_rule = NULL;
struct dict_object * dataobj_re_auth_request_type = NULL;
struct dict_object * dataobj_redirect_host = NULL;
struct dict_object * dataobj_redirect_host_usage = NULL;
struct dict_object * dataobj_redirect_max_cache_time = NULL;
struct dict_object * dataobj_reply_message = NULL;
struct dict_object * dataobj_result_code = NULL;
struct dict_object * dataobj_route_record = NULL;
struct dict_object * dataobj_service_type = NULL;
struct dict_object * dataobj_session_id = NULL;
struct dict_object * dataobj_session_timeout = NULL;
struct dict_object * dataobj_state = NULL;
struct dict_object * dataobj_tunneling = NULL;
struct dict_object * dataobj_user_name = NULL;



static int diameap_init_config(char * conffile)
{
	TRACE_ENTRY("%p",conffile);

	if (conffile != NULL)
	{
		diameap_config->conffile = conffile;
	}
	else
	{
		TRACE_DEBUG(FULL,"%sNo configuration file provided with the DiamEAP extension: default parameters will be used.",DIAMEAP_EXTENSION);
	}

	diameap_config->application_id = 5;
	diameap_config->vendor_id = 0;
	diameap_config->command_code = 268;
	diameap_config->diam_realm = strdup(fd_g_config->cnf_diamrlm);
	diameap_config->max_invalid_eap_packet = 5;
	diameap_config->multi_round_time_out = 30;
	check_user_identity = TRUE;

	return 0;
}

static int diameap_init_obj(void)
{

	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_ID, &(diameap_config->application_id),&dataobj_diameap_app, ENOENT));
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND,CMD_BY_CODE_R, &(diameap_config->command_code), &dataobj_diameap_cmd, ENOENT));
	//	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_VENDOR,VENDOR_OF_APPLICATION, &dataobj_diameap_app, &dataobj_diameap_ven,ENOENT) );
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Accounting-EAP-Auth-Method", &dataobj_accounting_eap_auth_method,
					ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Acct-Interim-Interval", &dataobj_acct_interim_interval, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Auth-Application-Id", &dataobj_auth_application_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Auth-Grace-Period", &dataobj_auth_grace_period, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Auth-Request-Type", &dataobj_auth_request_type, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Auth-Session-State", &dataobj_auth_session_state, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Authorization-Lifetime", &dataobj_authorization_lifetime, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Callback-Id", &dataobj_callback_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Callback-Number", &dataobj_callback_number, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Called-Station-Id", &dataobj_called_station_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Calling-Station-Id", &dataobj_calling_station_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Class", &dataobj_class, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Configuration-Token", &dataobj_configuration_token, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Connect-Info", &dataobj_connect_info, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Destination-Host", &dataobj_destination_host, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Destination-Realm", &dataobj_destination_realm, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"EAP-Master-Session-Key", &dataobj_eap_master_session_key, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"EAP-Key-Name", &dataobj_eap_key_name, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"EAP-Payload", &dataobj_eap_payload, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"EAP-Reissued-Payload", &dataobj_eap_reissued_payload, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Error-Message", &dataobj_error_message, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Error-Reporting-Host", &dataobj_error_reporting_host, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Failed-AVP", &dataobj_failed_avp, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Filter-Id", &dataobj_filter_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-AppleTalk-Link", &dataobj_framed_appletalk_link, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-AppleTalk-Network", &dataobj_framed_appletalk_network,
					ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-AppleTalk-Zone", &dataobj_framed_appletalk_zone, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-Compression", &dataobj_framed_compression, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-Interface-Id", &dataobj_framed_interface_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-IP-Address", &dataobj_framed_ip_address, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-IP-Netmask", &dataobj_framed_ip_netmask, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-IPv6-Prefix", &dataobj_framed_ipv6_prefix, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-IPv6-Pool", &dataobj_framed_ipv6_pool, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-IPv6-Route", &dataobj_framed_ipv6_route, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-IPX-Network", &dataobj_framed_ipx_network, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-MTU", &dataobj_framed_mtu, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-Pool", &dataobj_framed_pool, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-Protocol", &dataobj_framed_protocol, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-Route", &dataobj_framed_route, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Framed-Routing", &dataobj_framed_routing, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Idle-Timeout", &dataobj_idle_timeout, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Multi-Round-Time-Out", &dataobj_multi_round_time_out, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"NAS-Filter-Rule", &dataobj_nas_filter_rule, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"NAS-Identifier", &dataobj_nas_identifier, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"NAS-IP-Address", &dataobj_nas_ip_address, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"NAS-IPv6-Address", &dataobj_nas_ipv6_address, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"NAS-Port", &dataobj_nas_port, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"NAS-Port-Id", &dataobj_nas_port_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"NAS-Port-Type", &dataobj_nas_port_type, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Originating-Line-Info", &dataobj_originating_line_info, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Origin-Host", &dataobj_origin_host, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Origin-Realm", &dataobj_origin_realm, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Origin-State-Id", &dataobj_origin_state_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Port-Limit", &dataobj_port_limit, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Proxy-Info", &dataobj_proxy_info, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"QoS-Filter-Rule", &dataobj_qos_filter_rule, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Re-Auth-Request-Type", &dataobj_re_auth_request_type, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Redirect-Host", &dataobj_redirect_host, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Redirect-Host-Usage", &dataobj_redirect_host_usage, ENOENT));
	CHECK_FCT(
			fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Redirect-Max-Cache-Time",
					&dataobj_redirect_max_cache_time, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Reply-Message", &dataobj_reply_message, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Result-Code", &dataobj_result_code, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Route-Record", &dataobj_route_record, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Service-Type", &dataobj_service_type, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Session-Id", &dataobj_session_id, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Session-Timeout", &dataobj_session_timeout, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"State", &dataobj_state, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"Tunneling", &dataobj_tunneling, ENOENT));
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
					"User-Name", &dataobj_user_name, ENOENT));

	return 0;
}

/* Parse the configuration file (using the yacc parser) */
static int diameap_parse_conffile(void)
{
	if (!diameap_config->conffile)
	{
		TRACE_DEBUG (INFO,"%sNo configuration file to parse for DiamEAP Extension.",DIAMEAP_EXTENSION);
		return EINVAL;
	}
	extern FILE * diameapin;

	TRACE_DEBUG (FULL, "%sParsing configuration file: %s",DIAMEAP_EXTENSION, diameap_config->conffile);
	diameapin = fopen(diameap_config->conffile, "r");
	if (!diameapin)
	{
		TRACE_DEBUG(INFO,"%sUnable to open configuration file %s for reading: %s",DIAMEAP_EXTENSION, diameap_config->conffile, strerror(errno));
		return errno;
	}

	/* call yacc parser */
	CHECK_FCT(diameapparse(diameap_config));

	/* close the file */
	CHECK_FCT(fclose(diameapin));
	return 0;
}

int diameap_set_mysql_param(char * user, char * passwd, char * server,
		char * database)
{
	TRACE_ENTRY("%p %p %p %p",user,passwd,server,database);
	if (user == NULL)
	{
		TRACE_DEBUG(INFO,"%sUnable to connect to MySQL Database Server: Empty user name or missing in the configuration file.",DIAMEAP_EXTENSION);
		return EINVAL;
	}

	if (server == NULL)
	{
		TRACE_DEBUG(INFO,"%sUnable to connect to MySQL Database Server: MySQL Database Server address missing in the configuration file.",DIAMEAP_EXTENSION);
		return EINVAL;
	}
	if (database == NULL)
	{
		TRACE_DEBUG(INFO,"%sUnable to connect to MySQL Database Server: MySQL Database name missing in configuration file.",DIAMEAP_EXTENSION);
		return EINVAL;
	}
	diameap_config->db.user = user;
	diameap_config->db.password = passwd;
	diameap_config->db.server = server;
	diameap_config->db.database = database;
	return 0;
}

/* Connecting to MySQL Database */
int diameap_mysql_connect(void)
{
	TRACE_ENTRY();
	db_conn = mysql_init(NULL);
	const my_bool mysql_reconnect_val=1;
	mysql_options(db_conn,MYSQL_OPT_RECONNECT,&mysql_reconnect_val);
	/* Connect to database */
	if (!mysql_real_connect(db_conn, diameap_config->db.server,
			diameap_config->db.user, diameap_config->db.password,
			diameap_config->db.database, 0, NULL, 0))
	{
		TRACE_DEBUG(INFO,"%sConnection to MySQL Database Server failed: %s",DIAMEAP_EXTENSION, mysql_error(db_conn));
		return errno;
	}
	return 0;
}

static void diameap_conf_dump(void)
{

	if (!TRACE_BOOL(INFO))
		return;

	fd_log_debug("-------- DiamEAP extension : Configuration parameters (Dump) -------------");
	fd_log_debug("\t-Configuration file.....: %s", diameap_config->conffile);
	fd_log_debug("\t-EAP Application Id.....: %u",
			diameap_config->application_id);
	fd_log_debug("\t-EAP Application Command: %u",
			diameap_config->command_code);
	fd_log_debug("\t-EAP Application Vendor.: %u", diameap_config->vendor_id);
	fd_log_debug("\t-Max invalid EAP packets: %u", diameap_config->max_invalid_eap_packet);
	fd_log_debug("\t-Multi-Round Timeout....: %u", diameap_config->multi_round_time_out);
	if (diameap_config->db.user)
	{
		fd_log_debug("\t-MySQL Database Params..:");
		fd_log_debug("\t\tUser .......:%s", diameap_config->db.user);
		fd_log_debug("\t\tServer .....:%s", diameap_config->db.server);
		fd_log_debug("\t\tDatabase....:%s", diameap_config->db.database);
	}

	fd_log_debug("\t-EAP Method Plugins.....: ");
	diameap_plugin_dump();

	fd_log_debug("-------- DiamEAP extension : Configuration parameters (End) ---------------");
}

int diameap_init(char * conffile)
{
	TRACE_ENTRY("%p",conffile);

	/* Initialize the diameap_config structure*/
	CHECK_FCT(diameap_init_config(conffile));

	if (diameap_config->conffile)
	{
		/* parse the configuration file*/
		CHECK_FCT(diameap_parse_conffile());

		/* Load EAP methods plug-ins */
		CHECK_FCT(diameap_plugin_load());

	}
	else
	{
		TRACE_DEBUG(FULL,"%sNo EAP method plug-in available with a default configuration.",DIAMEAP_EXTENSION);
	}

	/* Initialize Dictionary templates */
	CHECK_FCT(diameap_init_obj());

	/* Initialize access to user's information Database */
	CHECK_FCT(diameap_mysql_connect());

	/* Dump DiamEAP extension configuration */
	diameap_conf_dump();

	return 0;
}
