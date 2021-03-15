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

#if !defined(OGS_PFCP_INSIDE) && !defined(OGS_PFCP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PFCP_RULE_MATCH_H
#define OGS_PFCP_RULE_MATCH_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pfcp_rule_t *ogs_pfcp_pdr_rule_find_by_packet(
                    ogs_pfcp_pdr_t *pdr, ogs_pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_RULE_MATCH_H */
