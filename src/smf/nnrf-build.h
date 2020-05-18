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

#ifndef SMF_NNRF_BUILD_H
#define SMF_NNRF_BUILD_H

#include "ogs-sbi.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_sbi_request_t *smf_nnrf_build_nf_register(
        ogs_sbi_nf_instance_t *nf_instance);
ogs_sbi_request_t *smf_nnrf_build_nf_update(
        ogs_sbi_nf_instance_t *nf_instance);
ogs_sbi_request_t *smf_nnrf_build_nf_de_register(
        ogs_sbi_nf_instance_t *nf_instance);

ogs_sbi_request_t *smf_nnrf_build_nf_status_subscribe(
        ogs_sbi_subscription_t *subscription);
ogs_sbi_request_t *smf_nnrf_build_nf_status_unsubscribe(
        ogs_sbi_subscription_t *subscription);

ogs_sbi_request_t *smf_nnrf_build_nf_discover(
        OpenAPI_nf_type_e target_nf_type, OpenAPI_nf_type_e requester_nf_type);

#ifdef __cplusplus
}
#endif

#endif /* SMF_NNRF_BUILD_H */
