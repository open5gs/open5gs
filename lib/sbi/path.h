/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

void ogs_sbi_send(ogs_sbi_nf_instance_t *nf_instance,
        ogs_sbi_client_cb_f client_cb, ogs_sbi_xact_t *xact);
bool ogs_sbi_discover_and_send(ogs_sbi_xact_t *xact,
        ogs_fsm_handler_t nf_state_registered, ogs_sbi_client_cb_f client_cb);

void ogs_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance);
void ogs_nnrf_nfm_send_nf_update(ogs_sbi_nf_instance_t *nf_instance);
void ogs_nnrf_nfm_send_nf_de_register(ogs_sbi_nf_instance_t *nf_instance);

void ogs_nnrf_nfm_send_nf_status_subscribe(ogs_sbi_client_t *client,
        OpenAPI_nf_type_e nf_type, char *nf_instance_id);
void ogs_nnrf_nfm_send_nf_status_unsubscribe(
        ogs_sbi_subscription_t *subscription);

void ogs_nnrf_disc_send_nf_discover(ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e target_nf_type, void *data);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_PATH_H */
