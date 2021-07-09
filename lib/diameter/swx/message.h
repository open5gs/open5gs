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

#ifndef OGS_DIAM_SWX_MESSAGE_H
#define OGS_DIAM_SWX_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_DIAM_SWX_APPLICATION_ID 16777265

extern struct dict_object *ogs_diam_swx_application;

extern struct dict_object *ogs_diam_swx_non_3gpp_user_data;
#define OGS_DIAM_SWX_NON_3GPP_SUBSCRIPTION_ALLOWED      0
#define OGS_DIAM_SWX_NON_3GPP_SUBSCRIPTION_BARRED       1
extern struct dict_object *ogs_diam_swx_non_3gpp_ip_access;
#define OGS_DIAM_SWX_NON_3GPP_APNS_ENABLE               0
#define OGS_DIAM_SWX_NON_3GPP_APNS_DISABLE              1
extern struct dict_object *ogs_diam_swx_non_3gpp_ip_access_apn;

#define OGS_DIAM_SWX_AUTH_SCHEME_EAP_AKA             "EAP-AKA"
#define OGS_DIAM_SWX_AUTH_SCHEME_EAP_AKA_QUOT        "EAP-AKA'"

int ogs_diam_swx_init(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_OGS_DIAM_SWX_MESSAGE_H */
