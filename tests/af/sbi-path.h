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

#ifndef AF_SBI_PATH_H
#define AF_SBI_PATH_H

#include "nnrf-build.h"
#include "nbsf-build.h"
#include "npcf-build.h"

#ifdef __cplusplus
extern "C" {
#endif

int af_sbi_open(void);
void af_sbi_close(void);

bool af_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance);

void af_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);

void af_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        af_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data));

void af_sbi_send_to_pcf(
        af_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data));

#ifdef __cplusplus
}
#endif

#endif /* AF_SBI_PATH_H */
