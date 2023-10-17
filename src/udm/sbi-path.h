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

#ifndef UDM_SBI_PATH_H
#define UDM_SBI_PATH_H

#include "nudr-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int udm_sbi_open(void);
void udm_sbi_close(void);

bool udm_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);
int udm_ue_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(udm_ue_t *udm_ue, void *data),
        udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, void *data);
int udm_sess_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(udm_sess_t *sess, void *data),
        udm_sess_t *sess, ogs_sbi_stream_t *stream, void *data);

#ifdef __cplusplus
}
#endif

#endif /* UDM_SBI_PATH_H */
