/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef OGS_SBI_PATH_H
#define OGS_SBI_PATH_H

#include "nnrf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int ogs_sbi_server_handler(ogs_sbi_request_t *request, void *data);
int ogs_sbi_client_handler(
        int status, ogs_sbi_response_t *response, void *data);

int ogs_sbi_discover_and_send(ogs_sbi_xact_t *xact);
int ogs_sbi_discover_only(ogs_sbi_xact_t *xact);

bool ogs_sbi_send_request_to_nf_instance(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);
bool ogs_sbi_send_request_to_client(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data);
bool ogs_sbi_send_request_to_nrf(
        ogs_sbi_service_type_e nrf_service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data);

#define ogs_sbi_send_http_status_no_content(__sTREAM) \
        ogs_sbi_send_response(__sTREAM, OGS_SBI_HTTP_STATUS_NO_CONTENT)
bool ogs_sbi_send_response(ogs_sbi_stream_t *stream, int status);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_PATH_H */
