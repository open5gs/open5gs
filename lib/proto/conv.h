/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_PROTO_INSIDE) && !defined(OGS_PROTO_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PROTO_CONV_H
#define OGS_PROTO_CONV_H

#ifdef __cplusplus
extern "C" {
#endif

void ogs_extract_digit_from_string(char *digit, char *string);

int ogs_paa_to_ip(const ogs_paa_t *paa, ogs_ip_t *ip);
int ogs_ip_to_paa(const ogs_ip_t *ip, ogs_paa_t *paa);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PROTO_CONV_H */
