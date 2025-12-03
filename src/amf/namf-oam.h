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

#ifndef AMF_NAMF_OAM_H
#define AMF_NAMF_OAM_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Namf_OAM API Handler
 *
 * OAM = Operations, Administration and Maintenance
 * Custom API for dynamic configuration management (not part of 3GPP standard)
 */
bool amf_namf_oam_handler(ogs_sbi_stream_t *stream, ogs_sbi_message_t *message, ogs_sbi_request_t *request);

/* Individual endpoint handlers */
bool namf_oam_handle_plmns_get(ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);
bool namf_oam_handle_plmns_post(ogs_sbi_stream_t *stream,
    ogs_sbi_message_t *message,
    ogs_sbi_request_t *request);

bool namf_oam_handle_plmns_delete(
    ogs_sbi_stream_t *stream,
    ogs_sbi_message_t *message);

bool namf_oam_handle_plmns_get_by_id(
    ogs_sbi_stream_t *stream,
    ogs_sbi_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* AMF_NAMF_OAM_H */
