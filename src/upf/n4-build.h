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

#ifndef UPF_N4_BUILD_H
#define UPF_N4_BUILD_H

#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *upf_n4_build_session_establishment_response(uint8_t type,
    upf_sess_t *sess, ogs_pfcp_pdr_t *created_pdr[], int num_of_created_pdr);
ogs_pkbuf_t *upf_n4_build_session_modification_response(uint8_t type,
    upf_sess_t *sess, ogs_pfcp_pdr_t *created_pdr[], int num_of_created_pdr);
ogs_pkbuf_t *upf_n4_build_session_deletion_response(uint8_t type,
    upf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* UPF_N4_BUILD_H */
