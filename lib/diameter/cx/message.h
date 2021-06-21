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

#if !defined(OGS_DIAMETER_INSIDE) && !defined(OGS_DIAMETER_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DIAM_CX_MESSAGE_H
#define OGS_DIAM_CX_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_DIAM_CX_APPLICATION_ID 16777216

extern struct dict_object *ogs_diam_cx_application;

extern struct dict_object *ogs_diam_cx_cmd_uar;
extern struct dict_object *ogs_diam_cx_cmd_uaa;
extern struct dict_object *ogs_diam_cx_cmd_mar;
extern struct dict_object *ogs_diam_cx_cmd_maa;
extern struct dict_object *ogs_diam_cx_cmd_sar;
extern struct dict_object *ogs_diam_cx_cmd_saa;
extern struct dict_object *ogs_diam_cx_cmd_lir;
extern struct dict_object *ogs_diam_cx_cmd_lia;

extern struct dict_object *ogs_diam_cx_public_identity;
extern struct dict_object *ogs_diam_cx_server_name;

extern struct dict_object *ogs_diam_cx_sip_number_auth_items;
extern struct dict_object *ogs_diam_cx_sip_item_number;
extern struct dict_object *ogs_diam_cx_sip_auth_data_item;
#define OGS_DIAM_CX_AUTH_SCHEME_IMS_AKA             "Digest-AKAv1-MD5"
#define OGS_DIAM_CX_AUTH_SCHEME_SIP_DIGEST          "SIP Digest"
#define OGS_DIAM_CX_AUTH_SCHEME_NASS_BUNDLED        "NASS-Bundled"
#define OGS_DIAM_CX_AUTH_SCHEME_GPRS_IMS_BUNDLED    "Early-IMS-Security"
#define OGS_DIAM_CX_AUTH_SCHEME_UNKNOWN             "Unknown"
extern struct dict_object *ogs_diam_cx_sip_authentication_scheme;
extern struct dict_object *ogs_diam_cx_sip_authenticate;
extern struct dict_object *ogs_diam_cx_sip_authorization;
extern struct dict_object *ogs_diam_cx_confidentiality_key;
extern struct dict_object *ogs_diam_cx_integrity_key;
#define OGS_DIAM_CX_SERVER_NO_ASSIGNMENT 0
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_REGISTRATION 1
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_RE_REGISTRATION 2
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_UNREGISTERED_USER 3
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_TIMEOUT_DEREGISTRATION 4
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_USER_DEREGISTRATION 5
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_TIMEOUT_DEREGISTRATION_STORE_SERVER_NAME 6
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_USER_DEREGISTRATION_STORE_SERVER_NAME 7
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_ADMINISTRATIVE_DEREGISTRATION 8
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_AUTHENTICATION_FAILURE 9
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_AUTHENTICATION_TIMEOUT 10
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_DEREGISTRATION_TOO_MUCH_DATA 11
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_AAA_USER_DATA_REQUEST 12
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_PGW_UPDATE 13
#define OGS_DIAM_CX_SERVER_ASSIGNMENT_RESTORATION 14
extern struct dict_object *ogs_diam_cx_server_assignment_type;
#define OGS_DIAM_CX_USER_DATA_NOT_AVAILABLE         0
#define OGS_DIAM_CX_USER_DATA_ALREADY_AVAILABLE     1
extern struct dict_object *ogs_diam_cx_user_data_already_available;
extern struct dict_object *ogs_diam_cx_user_data;
extern struct dict_object *ogs_diam_cx_charging_information;
extern struct dict_object *ogs_diam_cx_primary_event_charging_function_name;
extern struct dict_object *ogs_diam_cx_secondary_event_charging_function_name;
extern struct dict_object
    *ogs_diam_cx_primary_charging_collection_function_name;
extern struct dict_object
    *ogs_diam_cx_secondary_charging_collection_function_name;

#define OGS_DIAM_CX_FIRST_REGISTRATION                      2001
#define OGS_DIAM_CX_SUBSEQUENT_REGISTRATION                 2002
#define OGS_DIAM_CX_UNREGISTERED_SERVICE                    2003
#define OGS_DIAM_CX_SERVER_NAME_NOT_STORED                  2004
#define OGS_DIAM_CX_ERROR_USER_UNKNOWN                      5001
#define OGS_DIAM_CX_ERROR_IDENTITIES_DONT_MATCH             5002
#define OGS_DIAM_CX_ERROR_IDENTITY_NOT_REGISTERED           5003
#define OGS_DIAM_CX_ERROR_ROAMING_NOT_ALLOWED               5004
#define OGS_DIAM_CX_ERROR_IDENTITY_ALREADY_REGISTERED       5005
#define OGS_DIAM_CX_ERROR_AUTH_SCHEME_NOT_SUPPORTED         5006
#define OGS_DIAM_CX_ERROR_IN_ASSIGNMENT_TYPE                5007
#define OGS_DIAM_CX_ERROR_TOO_MUCH_DATA                     5008
#define OGS_DIAM_CX_ERROR_NOT_SUPPORTED_USER_DATA           5009
#define OGS_DIAM_CX_ERROR_FEATURE_UNSUPPORTED               5011
#define OGS_DIAM_CX_ERROR_SERVING_NODE_FEATURE_UNSUPPORTED  5012

int ogs_diam_cx_init(void);

extern const char *ogs_diam_cx_xml_version;
extern const char *ogs_diam_cx_xml_ims_subscription_s;
extern const char *ogs_diam_cx_xml_ims_subscription_e;
extern const char *ogs_diam_cx_xml_private_id_s;
extern const char *ogs_diam_cx_xml_private_id_e;
extern const char *ogs_diam_cx_xml_service_profile_s;
extern const char *ogs_diam_cx_xml_service_profile_e;
extern const char *ogs_diam_cx_xml_public_id_s;
extern const char *ogs_diam_cx_xml_public_id_e;
extern const char *ogs_diam_cx_xml_barring_indication_s;
extern const char *ogs_diam_cx_xml_barring_indication_e;
extern const char *ogs_diam_cx_xml_identity_s;
extern const char *ogs_diam_cx_xml_identity_e;
extern const char *ogs_diam_cx_xml_identity_type_s;
extern const char *ogs_diam_cx_xml_identity_type_e;
extern const char *ogs_diam_cx_xml_wildcarded_psi_s;
extern const char *ogs_diam_cx_xml_wildcarded_psi_e;
extern const char *ogs_diam_cx_xml_display_name_s;
extern const char *ogs_diam_cx_xml_display_name_e;

extern const char *ogs_diam_cx_xml_ifc_s;
extern const char *ogs_diam_cx_xml_ifc_e;
extern const char *ogs_diam_cx_xml_priority_s;
extern const char *ogs_diam_cx_xml_priority_e;
extern const char *ogs_diam_cx_xml_tp_s;
extern const char *ogs_diam_cx_xml_tp_e;
extern const char *ogs_diam_cx_xml_cnf_s;
extern const char *ogs_diam_cx_xml_cnf_e;
extern const char *ogs_diam_cx_xml_spt_s;
extern const char *ogs_diam_cx_xml_spt_e;
extern const char *ogs_diam_cx_xml_condition_negated_s;
extern const char *ogs_diam_cx_xml_condition_negated_e;
extern const char *ogs_diam_cx_xml_group_s;
extern const char *ogs_diam_cx_xml_group_e;
extern const char *ogs_diam_cx_xml_req_uri_s;
extern const char *ogs_diam_cx_xml_req_uri_e;
extern const char *ogs_diam_cx_xml_method_s;
extern const char *ogs_diam_cx_xml_method_e;
extern const char *ogs_diam_cx_xml_sip_hdr_s;
extern const char *ogs_diam_cx_xml_sip_hdr_e;
extern const char *ogs_diam_cx_xml_session_case_s;
extern const char *ogs_diam_cx_xml_session_case_e;
extern const char *ogs_diam_cx_xml_session_desc_s;
extern const char *ogs_diam_cx_xml_session_desc_e;
extern const char *ogs_diam_cx_xml_registration_type_s;
extern const char *ogs_diam_cx_xml_registration_type_e;
extern const char *ogs_diam_cx_xml_header_s;
extern const char *ogs_diam_cx_xml_header_e;
extern const char *ogs_diam_cx_xml_content_s;
extern const char *ogs_diam_cx_xml_content_e;
extern const char *ogs_diam_cx_xml_line_s;
extern const char *ogs_diam_cx_xml_line_e;

extern const char *ogs_diam_cx_xml_app_server_s;
extern const char *ogs_diam_cx_xml_app_server_e;
extern const char *ogs_diam_cx_xml_server_name_s;
extern const char *ogs_diam_cx_xml_server_name_e;
extern const char *ogs_diam_cx_xml_default_handling_s;
extern const char *ogs_diam_cx_xml_default_handling_e;
extern const char *ogs_diam_cx_xml_service_info_s;
extern const char *ogs_diam_cx_xml_service_info_e;
extern const char *ogs_diam_cx_xml_include_register_request;
extern const char *ogs_diam_cx_xml_include_register_response;

extern const char *ogs_diam_cx_xml_profile_part_ind_s;
extern const char *ogs_diam_cx_xml_profile_part_ind_e;

extern const char *ogs_diam_cx_xml_cn_services_auth_s;
extern const char *ogs_diam_cx_xml_cn_services_auth_e;
extern const char *ogs_diam_cx_xml_subs_media_profile_id_s;
extern const char *ogs_diam_cx_xml_subs_media_profile_id_e;
extern const char *ogs_diam_cx_xml_shared_ifc_set_id_s;
extern const char *ogs_diam_cx_xml_shared_ifc_set_id_e;

extern const char *ogs_diam_cx_xml_extension_s;
extern const char *ogs_diam_cx_xml_extension_e;

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_OGS_DIAM_CX_MESSAGE_H */
