/*
 * Copyright (C) 2022-2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef OGS_NNRF_PATH_H
#define OGS_NNRF_PATH_H

#include "nnrf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

bool ogs_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance);
bool ogs_nnrf_nfm_send_nf_update(ogs_sbi_nf_instance_t *nf_instance);
bool ogs_nnrf_nfm_send_nf_de_register(ogs_sbi_nf_instance_t *nf_instance);
bool ogs_nnrf_nfm_send_nf_status_subscribe(
        OpenAPI_nf_type_e req_nf_type, char *req_nf_instance_id,
        OpenAPI_nf_type_e subscr_cond_nf_type,
        char *subscr_cond_service_name);
bool ogs_nnrf_nfm_send_nf_status_update(
        ogs_sbi_subscription_data_t *subscription_data);
bool ogs_nnrf_nfm_send_nf_status_unsubscribe(
        ogs_sbi_subscription_data_t *subscription_data);

bool ogs_nnrf_nfm_send_to_nrf(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NNRF_PATH_H */
