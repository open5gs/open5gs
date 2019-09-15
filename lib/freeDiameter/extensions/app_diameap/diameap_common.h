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


#ifndef DIAMEAP_COMMON_H_
#define DIAMEAP_COMMON_H_

#include "libdiameap.h"
#include "libcrypt.h"
#include "diameap.h"
#include "diameap_plugins.h"
#include "diameap_eap.h"
#include "diameap_server.h"
#include <regex.h>



/* Dictionary Object templates */
extern struct dict_object * dataobj_diameap_cmd; /* Diameter-EAP-Request Command Code */
extern struct dict_object * dataobj_diameap_app; /* Diameter EAP Application object */
extern struct dict_object * dataobj_diameap_ven; /* Diameter EAP Application Vendor object */
extern struct dict_object * dataobj_accounting_eap_auth_method;
extern struct dict_object * dataobj_acct_interim_interval;
extern struct dict_object * dataobj_auth_application_id;
extern struct dict_object * dataobj_auth_grace_period;
extern struct dict_object * dataobj_auth_request_type;
extern struct dict_object * dataobj_auth_session_state;
extern struct dict_object * dataobj_authorization_lifetime;
extern struct dict_object * dataobj_callback_id;
extern struct dict_object * dataobj_callback_number;
extern struct dict_object * dataobj_called_station_id;
extern struct dict_object * dataobj_calling_station_id;
extern struct dict_object * dataobj_class;
extern struct dict_object * dataobj_configuration_token;
extern struct dict_object * dataobj_connect_info;
extern struct dict_object * dataobj_destination_host;
extern struct dict_object * dataobj_destination_realm;
extern struct dict_object * dataobj_eap_master_session_key;
extern struct dict_object * dataobj_eap_key_name;
extern struct dict_object * dataobj_eap_payload;
extern struct dict_object * dataobj_eap_reissued_payload;
extern struct dict_object * dataobj_error_message;
extern struct dict_object * dataobj_error_reporting_host;
extern struct dict_object * dataobj_failed_avp;
extern struct dict_object * dataobj_filter_id;
extern struct dict_object * dataobj_framed_appletalk_link;
extern struct dict_object * dataobj_framed_appletalk_network;
extern struct dict_object * dataobj_framed_appletalk_zone;
extern struct dict_object * dataobj_framed_compression;
extern struct dict_object * dataobj_framed_interface_id;
extern struct dict_object * dataobj_framed_ip_address;
extern struct dict_object * dataobj_framed_ip_netmask;
extern struct dict_object * dataobj_framed_ipv6_prefix;
extern struct dict_object * dataobj_framed_ipv6_pool;
extern struct dict_object * dataobj_framed_ipv6_route;
extern struct dict_object * dataobj_framed_ipx_network;
extern struct dict_object * dataobj_framed_mtu;
extern struct dict_object * dataobj_framed_pool;
extern struct dict_object * dataobj_framed_protocol;
extern struct dict_object * dataobj_framed_route;
extern struct dict_object * dataobj_framed_routing;
extern struct dict_object * dataobj_idle_timeout;
extern struct dict_object * dataobj_multi_round_time_out;
extern struct dict_object * dataobj_nas_filter_rule;
extern struct dict_object * dataobj_nas_identifier;
extern struct dict_object * dataobj_nas_ip_address;
extern struct dict_object * dataobj_nas_ipv6_address;
extern struct dict_object * dataobj_nas_port;
extern struct dict_object * dataobj_nas_port_id;
extern struct dict_object * dataobj_nas_port_type;
extern struct dict_object * dataobj_originating_line_info;
extern struct dict_object * dataobj_origin_host;
extern struct dict_object * dataobj_origin_realm;
extern struct dict_object * dataobj_origin_state_id;
extern struct dict_object * dataobj_port_limit;
extern struct dict_object * dataobj_proxy_info;
extern struct dict_object * dataobj_qos_filter_rule;
extern struct dict_object * dataobj_re_auth_request_type;
extern struct dict_object * dataobj_redirect_host;
extern struct dict_object * dataobj_redirect_host_usage;
extern struct dict_object * dataobj_redirect_max_cache_time;
extern struct dict_object * dataobj_reply_message;
extern struct dict_object * dataobj_result_code;
extern struct dict_object * dataobj_route_record;
extern struct dict_object * dataobj_service_type;
extern struct dict_object * dataobj_session_id;
extern struct dict_object * dataobj_session_timeout;
extern struct dict_object * dataobj_state;
extern struct dict_object * dataobj_tunneling;
extern struct dict_object * dataobj_user_name;


#endif /* DIAMEAP_COMMON_H_ */
