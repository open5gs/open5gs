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

#ifndef FD_MESSAGE_H
#define FD_MESSAGE_H

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FD_AVP_CODE_FRAME_IP_ADDRESS                    8
#define FD_AVP_CODE_FRAME_IPV6_PREFIX                   97 

/* Result-Code AVP */
#define FD_DIAMETER_UNKNOWN_PEER                        3010
#define FD_DIAMETER_AVP_UNSUPPORTED                     5001
#define FD_DIAMETER_UNKNOWN_SESSION_ID                  5002
#define FD_DIAMETER_AUTHORIZATION_REJECTED              5003
#define FD_DIAMETER_MISSING_AVP                         5004
#define FD_DIAMETER_INVALID_AVP_VALUE                   5005

extern struct dict_object *fd_session_id;
extern struct dict_object *fd_origin_host;
extern struct dict_object *fd_origin_realm;
extern struct dict_object *fd_destination_host;
extern struct dict_object *fd_destination_realm;
extern struct dict_object *fd_user_name;
#define FD_AUTH_SESSION_STATE_MAINTAINED                0
#define FD_AUTH_SESSION_NO_STATE_MAINTAINED             1
extern struct dict_object *fd_auth_session_state;
extern struct dict_object *fd_auth_application_id;
#define FD_AUTH_REQUEST_TYPE_AUTHENTICATE_ONLY          1
#define FD_AUTH_REQUEST_TYPE_AUTHORIZE_ONLY             2
#define FD_AUTH_REQUEST_TYPE_AUTHORIZE_AUTHENTICATE     3
extern struct dict_object *fd_auth_request_type;
#define FD_RE_AUTH_REQUEST_TYPE_AUTHORIZE_ONLY          0
#define FD_RE_AUTH_REQUEST_TYPE_AUTHORIZE_AUTHENTICATE  1
extern struct dict_object *fd_re_auth_request_type;
extern struct dict_object *fd_result_code;
extern struct dict_object *fd_experimental_result;
extern struct dict_object *fd_experimental_result_code;
extern struct dict_object *fd_vendor_specific_application_id;
extern struct dict_object *fd_mip6_agent_info;
extern struct dict_object *fd_mip_home_agent_address;

extern struct dict_object *fd_vendor;
extern struct dict_object *fd_vendor_id;

int fd_message_init(void);
int fd_message_session_id_set(struct msg *msg, uint8_t *sid, size_t sidlen);
int fd_message_experimental_rescode_set(
        struct msg *msg, uint32_t result_code);
int fd_message_vendor_specific_appid_set(struct msg *msg, uint32_t app_id);

#ifdef __cplusplus
}
#endif

#endif /* FD_MESSAGE_H */
