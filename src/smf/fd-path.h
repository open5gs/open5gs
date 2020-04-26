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

#ifndef SMF_FD_PATH_H
#define SMF_FD_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gtp_xact_s gtp_xact_t;

int smf_fd_init(void);
void smf_fd_final(void);

void smf_gx_send_ccr(smf_sess_t *sess, ogs_gtp_xact_t *xact,
        uint32_t cc_request_type);

#ifdef __cplusplus
}
#endif

#endif /* SMF_FD_PATH_H */

