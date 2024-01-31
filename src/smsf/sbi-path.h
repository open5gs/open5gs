/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SMSF_SBI_PATH_H
#define SMSF_SBI_PATH_H

#include "context.h"
#include "namf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int smsf_sbi_open(void);
void smsf_sbi_close(void);

bool smsf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);
int smsf_ue_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(smsf_ue_t *smsf_ue, void *data),
        smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, void *data);

void smsf_namf_comm_send_n1_n2_message_transfer(
        smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream,
        smsf_n1_n2_message_transfer_param_t *param);

#ifdef __cplusplus
}
#endif

#endif /* SMSF_SBI_PATH_H */
