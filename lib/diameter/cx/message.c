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

#include "ogs-diameter-cx.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_cx_application = NULL;

struct dict_object *ogs_diam_cx_cmd_uar = NULL;
struct dict_object *ogs_diam_cx_cmd_uaa = NULL;

struct dict_object *ogs_diam_cx_cmd_mar = NULL;
struct dict_object *ogs_diam_cx_cmd_maa = NULL;

struct dict_object *ogs_diam_cx_cmd_sar = NULL;
struct dict_object *ogs_diam_cx_cmd_saa = NULL;

struct dict_object *ogs_diam_cx_cmd_lir = NULL;
struct dict_object *ogs_diam_cx_cmd_lia = NULL;

struct dict_object *ogs_diam_cx_public_identity = NULL;
struct dict_object *ogs_diam_cx_server_name = NULL;

struct dict_object *ogs_diam_cx_sip_number_auth_items = NULL;
struct dict_object *ogs_diam_cx_sip_item_number = NULL;
struct dict_object *ogs_diam_cx_sip_auth_data_item = NULL;
struct dict_object *ogs_diam_cx_sip_authentication_scheme = NULL;
struct dict_object *ogs_diam_cx_sip_authenticate = NULL;
struct dict_object *ogs_diam_cx_sip_authorization = NULL;
struct dict_object *ogs_diam_cx_confidentiality_key = NULL;
struct dict_object *ogs_diam_cx_integrity_key = NULL;
struct dict_object *ogs_diam_cx_server_assignment_type = NULL;
struct dict_object *ogs_diam_cx_user_data_already_available = NULL;
struct dict_object *ogs_diam_cx_user_data = NULL;

struct dict_object *ogs_diam_cx_charging_information = NULL;
struct dict_object *ogs_diam_cx_primary_event_charging_function_name = NULL;
struct dict_object *ogs_diam_cx_secondary_event_charging_function_name = NULL;
struct dict_object
    *ogs_diam_cx_primary_charging_collection_function_name = NULL;
struct dict_object
    *ogs_diam_cx_secondary_charging_collection_function_name = NULL;

const char *ogs_diam_cx_xml_version="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
const char *ogs_diam_cx_xml_ims_subscription_s="<IMSSubscription>";
const char *ogs_diam_cx_xml_ims_subscription_e="</IMSSubscription>";
const char *ogs_diam_cx_xml_private_id_s="<PrivateID>";
const char *ogs_diam_cx_xml_private_id_e="</PrivateID>";
const char *ogs_diam_cx_xml_service_profile_s="<ServiceProfile>";
const char *ogs_diam_cx_xml_service_profile_e="</ServiceProfile>";
const char *ogs_diam_cx_xml_public_id_s="<PublicIdentity>";
const char *ogs_diam_cx_xml_public_id_e="</PublicIdentity>";
const char *ogs_diam_cx_xml_barring_indication_s="<BarringIndication>";
const char *ogs_diam_cx_xml_barring_indication_e="</BarringIndication>";
const char *ogs_diam_cx_xml_identity_s="<Identity>";
const char *ogs_diam_cx_xml_identity_e="</Identity>";
const char *ogs_diam_cx_xml_identity_type_s="<IdentityType>";
const char *ogs_diam_cx_xml_identity_type_e="</IdentityType>";
const char *ogs_diam_cx_xml_wildcarded_psi_s="<WildcardedPSI>";
const char *ogs_diam_cx_xml_wildcarded_psi_e="</WildcardedPSI>";
const char *ogs_diam_cx_xml_display_name_s="<DisplayName>";
const char *ogs_diam_cx_xml_display_name_e="</DisplayName>";

const char *ogs_diam_cx_xml_ifc_s="<InitialFilterCriteria>";
const char *ogs_diam_cx_xml_ifc_e="</InitialFilterCriteria>";
const char *ogs_diam_cx_xml_priority_s="<Priority>";
const char *ogs_diam_cx_xml_priority_e="</Priority>";
const char *ogs_diam_cx_xml_tp_s="<TriggerPoint>";
const char *ogs_diam_cx_xml_tp_e="</TriggerPoint>";
const char *ogs_diam_cx_xml_cnf_s="<ConditionTypeCNF>";
const char *ogs_diam_cx_xml_cnf_e="</ConditionTypeCNF>";
const char *ogs_diam_cx_xml_spt_s="<SPT>";
const char *ogs_diam_cx_xml_spt_e="</SPT>";
const char *ogs_diam_cx_xml_condition_negated_s="<ConditionNegated>";
const char *ogs_diam_cx_xml_condition_negated_e="</ConditionNegated>";
const char *ogs_diam_cx_xml_group_s="<Group>";
const char *ogs_diam_cx_xml_group_e="</Group>";
const char *ogs_diam_cx_xml_req_uri_s="<RequestURI>";
const char *ogs_diam_cx_xml_req_uri_e="</RequestURI>";
const char *ogs_diam_cx_xml_method_s="<Method>";
const char *ogs_diam_cx_xml_method_e="</Method>";
const char *ogs_diam_cx_xml_sip_hdr_s="<SIPHeader>";
const char *ogs_diam_cx_xml_sip_hdr_e="</SIPHeader>";
const char *ogs_diam_cx_xml_session_case_s="<SessionCase>";
const char *ogs_diam_cx_xml_session_case_e="</SessionCase>";
const char *ogs_diam_cx_xml_session_desc_s="<SessionDescription>";
const char *ogs_diam_cx_xml_session_desc_e="</SessionDescription>";
const char *ogs_diam_cx_xml_registration_type_s="<RegistrationType>";
const char *ogs_diam_cx_xml_registration_type_e="</RegistrationType>";
const char *ogs_diam_cx_xml_header_s="<Header>";
const char *ogs_diam_cx_xml_header_e="</Header>";
const char *ogs_diam_cx_xml_content_s="<Content>";
const char *ogs_diam_cx_xml_content_e="</Content>";
const char *ogs_diam_cx_xml_line_s="<Line>";
const char *ogs_diam_cx_xml_line_e="</Line>";

const char *ogs_diam_cx_xml_app_server_s="<ApplicationServer>";
const char *ogs_diam_cx_xml_app_server_e="</ApplicationServer>";
const char *ogs_diam_cx_xml_server_name_s="<ServerName>";
const char *ogs_diam_cx_xml_server_name_e="</ServerName>";
const char *ogs_diam_cx_xml_default_handling_s="<DefaultHandling>";
const char *ogs_diam_cx_xml_default_handling_e="</DefaultHandling>";
const char *ogs_diam_cx_xml_service_info_s="<ServiceInfo>";
const char *ogs_diam_cx_xml_service_info_e="</ServiceInfo>";
const char *ogs_diam_cx_xml_include_register_request="<IncludeRegisterRequest/>";
const char *ogs_diam_cx_xml_include_register_response="<IncludeRegisterResponse/>";

const char *ogs_diam_cx_xml_profile_part_ind_s="<ProfilePartIndicator>";
const char *ogs_diam_cx_xml_profile_part_ind_e="</ProfilePartIndicator>";

const char *ogs_diam_cx_xml_cn_services_auth_s="<CoreNetworkServicesAuthorization>";
const char *ogs_diam_cx_xml_cn_services_auth_e="</CoreNetworkServicesAuthorization>";
const char *ogs_diam_cx_xml_subs_media_profile_id_s="<SubscribedMediaProfileId>";
const char *ogs_diam_cx_xml_subs_media_profile_id_e="</SubscribedMediaProfileId>";
const char *ogs_diam_cx_xml_shared_ifc_set_id_s="<Extension><SharedIFCSetID>";
const char *ogs_diam_cx_xml_shared_ifc_set_id_e="</SharedIFCSetID></Extension>";

const char *ogs_diam_cx_xml_extension_s="<Extension>";
const char *ogs_diam_cx_xml_extension_e="</Extension>";

extern int ogs_dict_cx_entry(char *conffile);

int ogs_diam_cx_init(void)
{
    application_id_t id = OGS_DIAM_CX_APPLICATION_ID;

    ogs_assert(ogs_dict_cx_entry(NULL) == 0);

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID,
            (void *)&id, &ogs_diam_cx_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME,
            "3GPP/User-Authorization-Request", &ogs_diam_cx_cmd_uar);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME,
            "3GPP/User-Authorization-Answer", &ogs_diam_cx_cmd_uaa);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME,
            "3GPP/Multimedia-Auth-Request", &ogs_diam_cx_cmd_mar);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME,
            "3GPP/Multimedia-Auth-Answer", &ogs_diam_cx_cmd_maa);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME,
            "3GPP/Server-Assignment-Request", &ogs_diam_cx_cmd_sar);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME,
            "3GPP/Server-Assignment-Answer", &ogs_diam_cx_cmd_saa);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME,
            "3GPP/Location-Info-Request", &ogs_diam_cx_cmd_lir);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME,
            "3GPP/Location-Info-Answer", &ogs_diam_cx_cmd_lia);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Public-Identity", &ogs_diam_cx_public_identity);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Server-Name", &ogs_diam_cx_server_name);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "SIP-Number-Auth-Items", &ogs_diam_cx_sip_number_auth_items);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "SIP-Item-Number", &ogs_diam_cx_sip_item_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "SIP-Auth-Data-Item", &ogs_diam_cx_sip_auth_data_item);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "SIP-Authentication-Scheme",
            &ogs_diam_cx_sip_authentication_scheme);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "SIP-Authenticate", &ogs_diam_cx_sip_authenticate);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "SIP-Authorization", &ogs_diam_cx_sip_authorization);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Confidentiality-Key", &ogs_diam_cx_confidentiality_key);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Integrity-Key", &ogs_diam_cx_integrity_key);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Server-Assignment-Type", &ogs_diam_cx_server_assignment_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "User-Data-Already-Available",
            &ogs_diam_cx_user_data_already_available);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "User-Data", &ogs_diam_cx_user_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Charging-Information", &ogs_diam_cx_charging_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Primary-Event-Charging-Function-Name",
            &ogs_diam_cx_primary_event_charging_function_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Secondary-Event-Charging-Function-Name",
            &ogs_diam_cx_secondary_event_charging_function_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Primary-Charging-Collection-Function-Name",
            &ogs_diam_cx_primary_charging_collection_function_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Secondary-Charging-Collection-Function-Name",
            &ogs_diam_cx_secondary_charging_collection_function_name);

    return 0;
}
