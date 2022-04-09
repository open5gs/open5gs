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

#include "ogs-diameter-rx.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_rx_application = NULL;

struct dict_object *ogs_diam_rx_cmd_aar = NULL;
struct dict_object *ogs_diam_rx_cmd_aaa = NULL;
struct dict_object *ogs_diam_rx_cmd_asr = NULL;
struct dict_object *ogs_diam_rx_cmd_asa = NULL;
struct dict_object *ogs_diam_rx_cmd_str = NULL;
struct dict_object *ogs_diam_rx_cmd_sta = NULL;

struct dict_object *ogs_diam_rx_af_application_identifier = NULL;
struct dict_object *ogs_diam_rx_media_component_description = NULL;
struct dict_object *ogs_diam_rx_media_component_number = NULL;
struct dict_object *ogs_diam_rx_media_type = NULL;
struct dict_object *ogs_diam_rx_max_requested_bandwidth_ul = NULL;
struct dict_object *ogs_diam_rx_max_requested_bandwidth_dl = NULL;
struct dict_object *ogs_diam_rx_min_requested_bandwidth_ul = NULL;
struct dict_object *ogs_diam_rx_min_requested_bandwidth_dl = NULL;
struct dict_object *ogs_diam_rx_rr_bandwidth = NULL;
struct dict_object *ogs_diam_rx_rs_bandwidth = NULL;
struct dict_object *ogs_diam_rx_flow_status = NULL;
struct dict_object *ogs_diam_rx_codec_data = NULL;
struct dict_object *ogs_diam_rx_media_sub_component = NULL;
struct dict_object *ogs_diam_rx_flow_number = NULL;
struct dict_object *ogs_diam_rx_flow_usage = NULL;
struct dict_object *ogs_diam_rx_flow_description = NULL;
struct dict_object *ogs_diam_rx_subscription_id = NULL;
struct dict_object *ogs_diam_rx_subscription_id_type = NULL;
struct dict_object *ogs_diam_rx_subscription_id_data = NULL;
struct dict_object *ogs_diam_rx_reservation_priority = NULL;
struct dict_object *ogs_diam_rx_specific_action = NULL;
struct dict_object *ogs_diam_rx_framed_ip_address = NULL;
struct dict_object *ogs_diam_rx_framed_ipv6_prefix = NULL;
struct dict_object *ogs_diam_rx_ip_can_type = NULL;
struct dict_object *ogs_diam_rx_abort_cause = NULL;

extern int ogs_dict_rx_entry(char *conffile);

int ogs_diam_rx_init(void)
{
    application_id_t id = OGS_DIAM_RX_APPLICATION_ID;

    ogs_assert(ogs_dict_rx_entry(NULL) == 0);

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &ogs_diam_rx_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "AA-Request", &ogs_diam_rx_cmd_aar);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "AA-Answer", &ogs_diam_rx_cmd_aaa);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Abort-Session-Request", &ogs_diam_rx_cmd_asr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Abort-Session-Answer", &ogs_diam_rx_cmd_asa);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Session-Termination-Request", &ogs_diam_rx_cmd_str);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Session-Termination-Answer", &ogs_diam_rx_cmd_sta);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AF-Application-Identifier", &ogs_diam_rx_af_application_identifier);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Component-Description", &ogs_diam_rx_media_component_description);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Component-Number", &ogs_diam_rx_media_component_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Type", &ogs_diam_rx_media_type);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-UL" , &ogs_diam_rx_max_requested_bandwidth_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-DL" , &ogs_diam_rx_max_requested_bandwidth_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Min-Requested-Bandwidth-UL" , &ogs_diam_rx_min_requested_bandwidth_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Min-Requested-Bandwidth-DL" , &ogs_diam_rx_min_requested_bandwidth_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RR-Bandwidth" , &ogs_diam_rx_rr_bandwidth);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RS-Bandwidth" , &ogs_diam_rx_rs_bandwidth);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Status", &ogs_diam_rx_flow_status);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Codec-Data", &ogs_diam_rx_codec_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Sub-Component", &ogs_diam_rx_media_sub_component);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Number", &ogs_diam_rx_flow_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Usage", &ogs_diam_rx_flow_usage);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Description", &ogs_diam_rx_flow_description);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id", &ogs_diam_rx_subscription_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id-Type", &ogs_diam_rx_subscription_id_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id-Data", &ogs_diam_rx_subscription_id_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Reservation-Priority", &ogs_diam_rx_reservation_priority);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Specific-Action", &ogs_diam_rx_specific_action);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Framed-IP-Address", &ogs_diam_rx_framed_ip_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Framed-IPv6-Prefix", &ogs_diam_rx_framed_ipv6_prefix);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "IP-CAN-Type", &ogs_diam_rx_ip_can_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Abort-Cause", &ogs_diam_rx_abort_cause);

    return 0;
}
