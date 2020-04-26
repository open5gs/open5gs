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

#ifndef OGS_PFCP_N4_BUILD_H
#define OGS_PFCP_N4_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *ogs_pfcp_n4_build_heartbeat_request(uint8_t type);
ogs_pkbuf_t *ogs_pfcp_n4_build_heartbeat_response(uint8_t type);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_N4_BUILD_H */
