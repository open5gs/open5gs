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

#ifndef PGW_S5C_BUILD_H
#define PGW_S5C_BUILD_H

#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

int pgw_s5c_build_create_session_response(
        ogs_pkbuf_t **pkbuf, uint8_t type, pgw_sess_t *sess,
        ogs_diam_gx_message_t *gx_message,
        ogs_gtp_create_session_request_t *req);
int pgw_s5c_build_delete_session_response(
        ogs_pkbuf_t **pkbuf, uint8_t type,
        ogs_diam_gx_message_t *gx_message,
        ogs_gtp_delete_session_request_t *req);

int pgw_s5c_build_create_bearer_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, pgw_bearer_t *bearer);
int pgw_s5c_build_update_bearer_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, pgw_bearer_t *bearer,
        int qos_presence, int tft_presence);
int pgw_s5c_build_delete_bearer_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, pgw_bearer_t *bearer);
#ifdef __cplusplus
}
#endif

#endif /* PGW_S5C_BUILD_H */
