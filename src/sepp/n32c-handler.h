/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SEPP_N32C_HANDLER_H
#define SEPP_N32C_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

bool sepp_n32c_handshake_handle_security_capability_request(
    sepp_node_t *sepp_node,
    ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);
bool sepp_n32c_handshake_handle_security_capability_response(
    sepp_node_t *sepp_node, ogs_sbi_message_t *recvmsg);

#ifdef __cplusplus
}
#endif

#endif /* SEPP_N32C_HANDLER_H */
