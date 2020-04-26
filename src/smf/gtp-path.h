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

#ifndef SMF_GTP_PATH_H
#define SMF_GTP_PATH_H

#include "ogs-tun.h"
#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

int smf_gtp_open(void);
void smf_gtp_close(void);

void smf_gtp_send_create_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact);
void smf_gtp_send_delete_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact);

#ifdef __cplusplus
}
#endif

#endif /* SMF_GTP_PATH_H */
