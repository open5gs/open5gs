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

#ifndef AF_NPCF_BUILD_H
#define AF_NPCF_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct af_npcf_policyauthorization_param_s {
    OpenAPI_media_type_e med_type;
    int flow_type;
    int qos_type;
} af_npcf_policyauthorization_param_t;

ogs_sbi_request_t *af_npcf_policyauthorization_build_create(
        af_sess_t *sess, void *data);
ogs_sbi_request_t *af_npcf_policyauthorization_build_update(
        af_sess_t *sess, void *data);
ogs_sbi_request_t *af_npcf_policyauthorization_build_delete(
        af_sess_t *sess, void *data);

ogs_sbi_request_t *af_npcf_policyauthorization_build_create_video(
        af_sess_t *sess, void *data);

#ifdef __cplusplus
}
#endif

#endif /* AF_NPCF_BUILD_H */
