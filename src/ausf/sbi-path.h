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

#ifndef AUSF_SBI_PATH_H
#define AUSF_SBI_PATH_H

#include "nnrf-build.h"
#include "nudm-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int ausf_sbi_open(void);
void ausf_sbi_close(void);

bool ausf_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance);

bool ausf_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);
bool ausf_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        ausf_ue_t *ausf_ue, ogs_sbi_stream_t *stream, void *data,
        ogs_sbi_request_t *(*build)(ausf_ue_t *ausf_ue, void *data));

#ifdef __cplusplus
}
#endif

#endif /* AUSF_SBI_PATH_H */
