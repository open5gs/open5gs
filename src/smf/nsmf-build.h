/*
 * Copyright (C) 2024-2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SMF_NSMF_BUILD_H
#define SMF_NSMF_BUILD_H

#include "context.h"
#include "gsmue-message.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_sbi_request_t *smf_nsmf_pdusession_build_create_request(
        smf_sess_t *sess, void *data);
ogs_sbi_request_t *smf_nsmf_pdusession_build_release_request(
        smf_sess_t *sess, void *data);

#ifdef __cplusplus
}
#endif

#endif /* SMF_NSMF_BUILD_H */
