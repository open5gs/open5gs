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

#ifndef SMF_S5C_BUILD_H
#define SMF_S5C_BUILD_H

#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *smf_s5c_build_create_session_response(
        uint8_t type, smf_sess_t *sess);
ogs_pkbuf_t *smf_s5c_build_delete_session_response(
        uint8_t type, smf_sess_t *sess);

ogs_pkbuf_t *smf_s5c_build_create_bearer_request(
        uint8_t type, smf_bearer_t *bearer, ogs_gtp_tft_t *tft);
ogs_pkbuf_t *smf_s5c_build_update_bearer_request(
        uint8_t type, smf_bearer_t *bearer, uint8_t pti,
        ogs_gtp_tft_t *tft, int qos_presence);
ogs_pkbuf_t *smf_s5c_build_delete_bearer_request(
        uint8_t type, smf_bearer_t *bearer, uint8_t pti);
#ifdef __cplusplus
}
#endif

#endif /* SMF_S5C_BUILD_H */
