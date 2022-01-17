/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#ifndef SMF_GN_BUILD_H
#define SMF_GN_BUILD_H

#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *smf_gn_build_create_pdp_context_response(
        uint8_t type, smf_sess_t *sess);
ogs_pkbuf_t *smf_gn_build_delete_pdp_context_response(
        uint8_t type, smf_sess_t *sess);

ogs_pkbuf_t *smf_gn_build_update_pdp_context_response(
        uint8_t type, smf_sess_t *sess, const smf_bearer_t *bearer);

#ifdef __cplusplus
}
#endif

#endif /* SMF_GN_BUILD_H */
