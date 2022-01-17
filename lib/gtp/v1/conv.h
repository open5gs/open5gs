/*
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#ifndef OGS_GTP1_CONV_H
#define OGS_GTP1_CONV_H

#ifdef __cplusplus
extern "C" {
#endif

int ogs_gtp1_gsn_addr_to_sockaddr(const ogs_gtp1_gsn_addr_t *gsnaddr,
        uint16_t gsnaddr_len, uint16_t port, ogs_sockaddr_t **list);
int ogs_gtp1_sockaddr_to_gsn_addr(const ogs_sockaddr_t *addr,
        const ogs_sockaddr_t *addr6, ogs_gtp1_gsn_addr_t *gsnaddr, int *len);
int ogs_gtp1_gsn_addr_to_ip(const ogs_gtp1_gsn_addr_t *gsnaddr, uint16_t gsnaddr_len,
        ogs_ip_t *ip);

int ogs_gtp1_eua_to_ip(const ogs_eua_t *eua, uint16_t eua_len, ogs_ip_t *ip,
        uint8_t *pdu_session_type);

int ogs_gtp1_ip_to_eua(uint8_t pdu_session_type, const ogs_ip_t *ip,
        ogs_eua_t *eua, uint8_t *eua_len);

int ogs_gtp1_qos_profile_to_qci(const ogs_gtp1_qos_profile_decoded_t *decoded,
        uint8_t *qci);

#ifdef __cplusplus
}
#endif

#endif
