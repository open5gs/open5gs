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

#ifndef OGS_DIAM_MESSAGE_H
#define OGS_DIAM_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_3GPP_VENDOR_ID 10415

#define OGS_DIAM_AVP_CODE_FRAME_IP_ADDRESS          8
#define OGS_DIAM_AVP_CODE_FRAME_IPV6_PREFIX         97

/* Result-Code AVP */
#define OGS_DIAM_UNKNOWN_PEER                       3010
#define OGS_DIAM_AVP_UNSUPPORTED                    5001
#define OGS_DIAM_UNKNOWN_SESSION_ID                 5002
#define OGS_DIAM_AUTHORIZATION_REJECTED             5003
#define OGS_DIAM_MISSING_AVP                        5004
#define OGS_DIAM_INVALID_AVP_VALUE                  5005
#define OGS_DIAM_USER_UNKNOWN                       5030

extern struct dict_object *ogs_diam_session_id;
extern struct dict_object *ogs_diam_termination_cause;
#define OGS_DIAM_TERMINATION_CAUSE_DIAMETER_LOGOUT                   1
#define OGS_DIAM_TERMINATION_CAUSE_DIAMETER_SERVICE_NOT_PROVIDED     2
#define OGS_DIAM_TERMINATION_CAUSE_DIAMETER_BAD_ANSWER               3
#define OGS_DIAM_TERMINATION_CAUSE_DIAMETER_DIAMETER_ADMINISTRATIVE  4
#define OGS_DIAM_TERMINATION_CAUSE_DIAMETER_LINK_BROKEN              5
#define OGS_DIAM_TERMINATION_CAUSE_DIAMETER_AUTH_EXPIRED             6
#define OGS_DIAM_TERMINATION_CAUSE_DIAMETER_USER_MOVED               7
#define OGS_DIAM_TERMINATION_CAUSE_DIAMETER_SESSION_TIMEOUT          8
extern struct dict_object *ogs_diam_origin_host;
extern struct dict_object *ogs_diam_origin_realm;
extern struct dict_object *ogs_diam_destination_host;
extern struct dict_object *ogs_diam_destination_realm;
extern struct dict_object *ogs_diam_user_name;
extern struct dict_object *ogs_diam_origin_state_id;
extern struct dict_object *ogs_diam_event_timestamp;
extern struct dict_object *ogs_diam_subscription_id;
#define OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_E164      0
#define OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_IMSI      1
#define OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_SIP_URI   2
#define OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_NAI       3
extern struct dict_object *ogs_diam_subscription_id_type;
extern struct dict_object *ogs_diam_subscription_id_data;
#define OGS_DIAM_AUTH_SESSION_STATE_MAINTAINED                0
#define OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED             1
extern struct dict_object *ogs_diam_auth_session_state;
extern struct dict_object *ogs_diam_auth_application_id;
#define OGS_DIAM_AUTH_REQUEST_TYPE_AUTHENTICATE_ONLY          1
#define OGS_DIAM_AUTH_REQUEST_TYPE_AUTHORIZE_ONLY             2
#define OGS_DIAM_AUTH_REQUEST_TYPE_AUTHORIZE_AUTHENTICATE     3
extern struct dict_object *ogs_diam_auth_request_type;
#define OGS_DIAM_RE_AUTH_REQUEST_TYPE_AUTHORIZE_ONLY          0
#define OGS_DIAM_RE_AUTH_REQUEST_TYPE_AUTHORIZE_AUTHENTICATE  1
extern struct dict_object *ogs_diam_re_auth_request_type;
extern struct dict_object *ogs_diam_result_code;
extern struct dict_object *ogs_diam_experimental_result;
extern struct dict_object *ogs_diam_experimental_result_code;
extern struct dict_object *ogs_diam_vendor_specific_application_id;
extern struct dict_object *ogs_diam_mip6_agent_info;
extern struct dict_object *ogs_diam_mip_home_agent_address;
extern struct dict_object *ogs_diam_authorization_lifetime;
extern struct dict_object *ogs_diam_auth_grace_period;
extern struct dict_object *ogs_diam_session_timeout;
extern struct dict_object *ogs_diam_service_context_id;
#define OGS_DIAM_RAT_TYPE_WLAN                              0
#define OGS_DIAM_RAT_TYPE_VIRTUAL                           1
#define OGS_DIAM_RAT_TYPE_UTRAN                             1000
#define OGS_DIAM_RAT_TYPE_GERAN                             1001
#define OGS_DIAM_RAT_TYPE_GAN                               1002
#define OGS_DIAM_RAT_TYPE_HSPA_EVOLUTION                    1003
#define OGS_DIAM_RAT_TYPE_EUTRAN                            1004
#define OGS_DIAM_RAT_TYPE_EUTRAN_NB_IoT                     1005
#define OGS_DIAM_RAT_TYPE_CDMA2000_1X                       2000
#define OGS_DIAM_RAT_TYPE_HRPD                              2001
#define OGS_DIAM_RAT_TYPE_UMB                               2002
#define OGS_DIAM_RAT_TYPE_EHRPD                             2003
extern struct dict_object *ogs_diam_rat_type;
extern struct dict_object *ogs_diam_service_selection;
extern struct dict_object *ogs_diam_visited_plmn_id;
extern struct dict_object *ogs_diam_visited_network_identifier;

extern struct dict_object *ogs_diam_vendor;
extern struct dict_object *ogs_diam_vendor_id;

int ogs_diam_message_init(void);
int ogs_diam_message_session_id_set(struct msg *msg, uint8_t *sid, size_t sidlen);
int ogs_diam_message_experimental_rescode_set(
        struct msg *msg, uint32_t result_code);
int ogs_diam_message_vendor_specific_appid_set(struct msg *msg, uint32_t app_id);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_MESSAGE_H */
