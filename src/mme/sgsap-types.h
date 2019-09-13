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

#ifndef SGSAP_TYPES_H
#define SGSAP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define SGSAP_PAGING_REQUEST                    1
#define SGSAP_PAGING_REJECT                     2
#define SGSAP_SERVICE_REQUEST                   6
#define SGSAP_DOWNLINK_UNITDATA                 7
#define SGSAP_UPLINK_UNITDATA                   8
#define SGSAP_LOCATION_UPDATE_REQUEST           9
#define SGSAP_LOCATION_UPDATE_ACCEPT            10
#define SGSAP_LOCATION_UPDATE_REJECT            11
#define SGSAP_TMSI_REALLOCATION_COMPLETE        12
#define SGSAP_EPS_DETACH_INDICATION             17
#define SGSAP_EPS_DETACH_ACK                    18
#define SGSAP_IMSI_DETACH_INDICATION            19
#define SGSAP_IMSI_DETACH_ACK                   20
#define SGSAP_RESET_INDICATION                  21
#define SGSAP_RESET_ACK                         22
#define SGSAP_MO_CSFB_INDICIATION               24
#define SGSAP_MM_INFORMATION_REQUEST            26
#define SGSAP_RELEASE_REQUEST                   27
#define SGSAP_UE_UNREACHABLE                    31

#define SGSAP_IE_IMSI_TYPE                      1
#define SGSAP_IE_IMSI_LEN                       OGS_MAX_IMSI_LEN
#define SGSAP_IE_VLR_NAME_TYPE                  2
#define SGSAP_IE_VLR_NAME_LEN                   OGS_MAX_FQDN_LEN
#define SGSAP_IE_LAI_TYPE                       4
#define SGSAP_IE_LAI_LEN                        5
#define SGSAP_IE_SGS_CAUSE_TYPE                 8
#define SGSAP_IE_SGS_CAUSE_LEN                  1
#define SGSAP_IE_MME_NAME_TYPE                  9
#define SGSAP_IE_MME_NAME_LEN                   55
#define SGSAP_IE_EPS_UPDATE_TYPE                10
#define SGSAP_IE_EPS_UPDATE_LEN                 1
#define SGSAP_IE_MOBILE_IDENTITY_TYPE           14
#define SGSAP_IE_MOBILE_IDENTITY_LEN            5
#define SGSAP_IE_REJECT_CAUSE_TYPE              15
#define SGSAP_IE_REJECT_CAUSE_LEN               1
#define SGSAP_IE_EPS_DETACH_INDICATION_TYPE     16
#define SGSAP_IE_EPS_DETACH_INDICATION_LEN      1
#define SGSAP_IE_IMSI_DETACH_INDICATION_TYPE    17
#define SGSAP_IE_IMSI_DETACH_INDICATION_LEN     1
#define SGSAP_IE_NAS_MESSAGE_CONTAINER_TYPE     22
#define SGSAP_IE_MM_INFORMATION_TYPE            23
#define SGSAP_IE_SERVICE_INDICATOR_TYPE         32
#define SGSAP_IE_SERVICE_INDICATOR_LEN          1
#define SGSAP_IE_TAI_TYPE                       35
#define SGSAP_IE_TAI_LEN                        7
#define SGSAP_IE_E_CGI_TYPE                     36
#define SGSAP_IE_E_CGI_LEN                      9
#define SGSAP_IE_UE_EMM_MODE_TYPE               37
#define SGSAP_IE_UE_EMM_MODE_LEN                1

#define SGSAP_EPS_UPDATE_IMSI_ATTACH                                1
#define SGSAP_EPS_UPDATE_NORMAL                                     2

#define SGSAP_CS_CALL_SERVICE_INDICATOR                             1
#define SGSAP_SMS_SERVICE_INDICATOR                                 2

#define SGSAP_EPS_DETACH_NETWORK_INITIATED                          1
#define SGSAP_EPS_DETACH_UE_INITIATED                               2
#define SGSAP_EPS_DETACH_EPS_SERVICE_NOT_ALLOWED                    3
#define SGSAP_IMSI_DETACH_EXPLICIT_UE_INITIATED                     1
#define SGSAP_IMSI_DETACH_COMBINED_UE_INITIATED                     2
#define SGSAP_IMSI_DETACH_IMPLICIT_NETWORK_INITIATED                3

#define SGSAP_EMM_IDLE_MODE                                         0
#define SGSAP_EMM_CONNECTED_MODE                                    1

#define SGSAP_SGS_CAUSE_IMSI_DETACHED_EPS                           1
#define SGSAP_SGS_CAUSE_IMSI_DETACHED_EPS_AND_NON_EPS               2
#define SGSAP_SGS_CAUSE_IMSI_UNKNOWN                                3
#define SGSAP_SGS_CAUSE_IMSI_DETACHED_NON_EPS                       4
#define SGSAP_SGS_CAUSE_IMSI_IMPLICITLY_DETACHED_NON_EPS            5
#define SGSAP_SGS_CAUSE_UE_UNREACHABLE                              6
#define SGSAP_SGS_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE  7
#define SGSAP_SGS_CAUSE_MISSING_MANDATORY_IE                        8
#define SGSAP_SGS_CAUSE_INVALID_MANDATORY_IE                        9
#define SGSAP_SGS_CAUSE_CONDITIONAL_IE_ERROR                        10
#define SGSAP_SGS_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE              11
#define SGSAP_SGS_CAUSE_MESSAGE_UNKNOWN                             12
#define SGSAP_SGS_CAUSE_MT_CS_FALLBACK_REJECT_BY_USER               13
#define SGSAP_SGS_CAUSE_UE_TEMPORARILY_UNREACHABLE                  14

#ifdef __cplusplus
}
#endif

#endif /* SGSAP_TYPES_H */
