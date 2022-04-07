/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-diameter-common.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_session_id = NULL;
struct dict_object *ogs_diam_termination_cause = NULL;
struct dict_object *ogs_diam_origin_host = NULL;
struct dict_object *ogs_diam_origin_realm = NULL;
struct dict_object *ogs_diam_destination_host = NULL;
struct dict_object *ogs_diam_destination_realm = NULL;
struct dict_object *ogs_diam_user_name = NULL;
struct dict_object *ogs_diam_origin_state_id = NULL;
struct dict_object *ogs_diam_event_timestamp = NULL;
struct dict_object *ogs_diam_subscription_id = NULL;
struct dict_object *ogs_diam_subscription_id_type = NULL;
struct dict_object *ogs_diam_subscription_id_data = NULL;
struct dict_object *ogs_diam_auth_session_state = NULL;
struct dict_object *ogs_diam_auth_application_id = NULL;
struct dict_object *ogs_diam_auth_request_type = NULL;
struct dict_object *ogs_diam_re_auth_request_type = NULL;
struct dict_object *ogs_diam_result_code = NULL;
struct dict_object *ogs_diam_experimental_result = NULL;
struct dict_object *ogs_diam_experimental_result_code = NULL;
struct dict_object *ogs_diam_vendor_specific_application_id = NULL;
struct dict_object *ogs_diam_mip6_agent_info = NULL;
struct dict_object *ogs_diam_mip_home_agent_address = NULL;
struct dict_object *ogs_diam_authorization_lifetime = NULL;
struct dict_object *ogs_diam_auth_grace_period = NULL;
struct dict_object *ogs_diam_session_timeout = NULL;
struct dict_object *ogs_diam_service_context_id = NULL;
struct dict_object *ogs_diam_rat_type = NULL;
struct dict_object *ogs_diam_service_selection = NULL;
struct dict_object *ogs_diam_visited_plmn_id = NULL;
struct dict_object *ogs_diam_visited_network_identifier = NULL;

struct dict_object *ogs_diam_vendor = NULL;
struct dict_object *ogs_diam_vendor_id = NULL;

extern int ogs_dict_common_entry(char *conffile);

int ogs_diam_message_init()
{
    vendor_id_t id = OGS_3GPP_VENDOR_ID;

    ogs_assert(ogs_dict_common_entry(NULL) == 0);

    CHECK_dict_search( DICT_VENDOR, VENDOR_BY_ID, (void *)&id, &ogs_diam_vendor);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Vendor-Id", &ogs_diam_vendor_id);

    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Session-Id", &ogs_diam_session_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME, "Termination-Cause", &ogs_diam_termination_cause);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Origin-Host", &ogs_diam_origin_host);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Origin-Realm", &ogs_diam_origin_realm);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Destination-Host", &ogs_diam_destination_host);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Destination-Realm", &ogs_diam_destination_realm);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "User-Name", &ogs_diam_user_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME, "Origin-State-Id", &ogs_diam_origin_state_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME, "Event-Timestamp", &ogs_diam_event_timestamp);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Subscription-Id", &ogs_diam_subscription_id);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Subscription-Id-Type", &ogs_diam_subscription_id_type);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Subscription-Id-Data", &ogs_diam_subscription_id_data);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Auth-Session-State", &ogs_diam_auth_session_state);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Auth-Application-Id", &ogs_diam_auth_application_id);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Auth-Request-Type", &ogs_diam_auth_request_type);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Re-Auth-Request-Type", &ogs_diam_re_auth_request_type);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Result-Code", &ogs_diam_result_code);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Experimental-Result", &ogs_diam_experimental_result);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Experimental-Result-Code", &ogs_diam_experimental_result_code);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Vendor-Specific-Application-Id", &ogs_diam_vendor_specific_application_id);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "MIP6-Agent-Info", &ogs_diam_mip6_agent_info);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "MIP-Home-Agent-Address", &ogs_diam_mip_home_agent_address);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Authorization-Lifetime", &ogs_diam_authorization_lifetime);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Auth-Grace-Period", &ogs_diam_auth_grace_period);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Session-Timeout", &ogs_diam_session_timeout);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Service-Context-Id", &ogs_diam_service_context_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RAT-Type", &ogs_diam_rat_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Service-Selection", &ogs_diam_service_selection);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Visited-PLMN-Id", &ogs_diam_visited_plmn_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Visited-Network-Identifier", &ogs_diam_visited_network_identifier);

    return 0;
}

int ogs_diam_message_session_id_set(
        struct msg *msg, uint8_t *sid, size_t sidlen)
{
    struct avp *avp;
    union avp_value val;

	/* Create an AVP to hold it */
	CHECK_FCT( fd_msg_avp_new( ogs_diam_session_id, 0, &avp ) );

	/* Set its value */
	memset(&val, 0, sizeof(val));
	val.os.data = sid;
	val.os.len  = sidlen;
	CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );

	/* Add it to the message */
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_FIRST_CHILD, avp ) );

    return 0;
}

int ogs_diam_message_experimental_rescode_set(
        struct msg *msg, uint32_t result_code)
{
    struct avp *avp;
    struct avp *avp_vendor;
    struct avp *avp_experimental_result_code;
    union avp_value value;

    CHECK_FCT( fd_msg_avp_new(ogs_diam_experimental_result, 0, &avp) );

    CHECK_FCT( fd_msg_avp_new(ogs_diam_vendor_id, 0, &avp_vendor) );
    value.u32 = OGS_3GPP_VENDOR_ID;
    CHECK_FCT( fd_msg_avp_setvalue(avp_vendor, &value) );
    CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_vendor) );

    CHECK_FCT( fd_msg_avp_new(
            ogs_diam_experimental_result_code, 0, &avp_experimental_result_code) );
    value.u32 = result_code;
    CHECK_FCT( fd_msg_avp_setvalue(avp_experimental_result_code, &value) );
    CHECK_FCT( fd_msg_avp_add(
            avp, MSG_BRW_LAST_CHILD, avp_experimental_result_code) );

    CHECK_FCT( fd_msg_avp_add(msg, MSG_BRW_LAST_CHILD, avp) );

    CHECK_FCT( fd_msg_add_origin(msg, 0) );

    return 0;
}

int ogs_diam_message_vendor_specific_appid_set(struct msg *msg, uint32_t app_id)
{
    struct avp *avp;
    struct avp *avp_vendor;
    struct avp *avp_vendor_specific_application_id;
    union avp_value value;

    CHECK_FCT( fd_msg_avp_new(ogs_diam_vendor_specific_application_id, 0, &avp) );

    CHECK_FCT( fd_msg_avp_new(ogs_diam_vendor_id, 0, &avp_vendor) );
    value.u32 = OGS_3GPP_VENDOR_ID;
    CHECK_FCT( fd_msg_avp_setvalue(avp_vendor, &value) );
    CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_vendor) );

    CHECK_FCT( fd_msg_avp_new(
            ogs_diam_auth_application_id, 0, &avp_vendor_specific_application_id) );
    value.u32 = app_id;
    CHECK_FCT(
            fd_msg_avp_setvalue(avp_vendor_specific_application_id, &value) );
    CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD,
                avp_vendor_specific_application_id) );

    CHECK_FCT( fd_msg_avp_add(msg, MSG_BRW_LAST_CHILD, avp) );

    return 0;
}
