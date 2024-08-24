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

#ifndef SMF_NUDM_BUILD_H
#define SMF_NUDM_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_sbi_request_t *smf_nudm_sdm_build_get(smf_sess_t *sess, void *data);

ogs_sbi_request_t *smf_nudm_uecm_build_registration(
        smf_sess_t *sess, void *data);
ogs_sbi_request_t *smf_nudm_uecm_build_deregistration(
        smf_sess_t *sess, void *data);
ogs_sbi_request_t *smf_nudm_sdm_build_subscription(
        smf_sess_t *sess, void *data);
ogs_sbi_request_t *smf_nudm_sdm_build_subscription_delete(
        smf_sess_t *sess, void *data);

#ifdef __cplusplus
}
#endif

#endif /* SMF_NUDM_BUILD_H */
