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

#ifndef SMSF_NUDM_BUILD_H
#define SMSF_NUDM_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_sbi_request_t *smsf_nudm_uecm_build_smsf_registration(
        smsf_ue_t *smsf_ue, void *data);
ogs_sbi_request_t *smsf_nudm_uecm_build_smsf_registration_delete(
        smsf_ue_t *smsf_ue, void *data);
ogs_sbi_request_t *smsf_nudm_sdm_build_get(smsf_ue_t *smsf_ue, void *data);
ogs_sbi_request_t *smsf_nudm_sdm_build_subscription(
        smsf_ue_t *smsf_ue, void *data);
ogs_sbi_request_t *smsf_nudm_sdm_build_subscription_delete(
        smsf_ue_t *smsf_ue, void *data);

#ifdef __cplusplus
}
#endif

#endif /* SMSF_NUDM_BUILD_H */
