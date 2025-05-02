/*
 * Copyright (C) 2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SMF_LOCAL_PATH_H
#define SMF_LOCAL_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

void smf_trigger_session_release(smf_sess_t *sess,
    ogs_sbi_stream_t *stream, int pfcp_trigger);

#ifdef __cplusplus
}
#endif

#endif /* SMF_LOCAL_PATH_H */
