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

#ifndef SMF_N4_BUILD_H
#define SMF_N4_BUILD_H

#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *smf_n4_build_session_establishment_request(
        uint8_t type, smf_sess_t *sess);
ogs_pkbuf_t *smf_n4_build_pdr_to_modify_list(
        uint8_t type, smf_sess_t *sess, ogs_pfcp_xact_t *xact);
ogs_pkbuf_t *smf_n4_build_qos_flow_to_modify_list(
        uint8_t type, smf_sess_t *sess, ogs_pfcp_xact_t *xact);
ogs_pkbuf_t *smf_n4_build_session_deletion_request(
        uint8_t type, smf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* SMF_N4_BUILD_H */
