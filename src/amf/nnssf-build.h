/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef AMF_NNSSF_BUILD_H
#define AMF_NNSSF_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct amf_nnssf_nsselection_param_s {
    struct {
        bool presence;
        ogs_s_nssai_t *snssai;
        OpenAPI_roaming_indication_e roaming_indication;
        ogs_s_nssai_t *home_snssai;
    } slice_info_for_pdu_session;
    ogs_plmn_id_t *home_plmn_id;
    ogs_5gs_tai_t *tai;
} amf_nnssf_nsselection_param_t;

ogs_sbi_request_t *amf_nnssf_nsselection_build_get(
        amf_sess_t *sess, void *data);

#ifdef __cplusplus
}
#endif

#endif /* AMF_NNSSF_BUILD_H */
