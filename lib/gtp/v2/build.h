/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_GTP_INSIDE) && !defined(OGS_GTP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GTP2_BUILD_H
#define OGS_GTP2_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *ogs_gtp2_build_echo_request(
        uint8_t type, uint8_t recovery, uint8_t features);
ogs_pkbuf_t *ogs_gtp2_build_echo_response(
        uint8_t type, uint8_t recovery, uint8_t features);

ogs_pkbuf_t *ogs_gtp1_build_error_indication(
        uint32_t teid, ogs_sockaddr_t *addr);

void ogs_gtp2_fill_header(
        ogs_gtp2_header_t *gtp_hdesc, ogs_gtp2_extension_header_t *ext_hdesc,
        ogs_pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP2_BUILD_H */
