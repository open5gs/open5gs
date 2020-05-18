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

#if !defined(OGS_SBI_INSIDE) && !defined(OGS_SBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SBI_CONV_H
#define OGS_SBI_CONV_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_server_s ogs_sbi_server_t;
typedef struct ogs_sbi_client_s ogs_sbi_client_t;

char *ogs_sbi_server_uri(ogs_sbi_server_t *server,
    const char *service_name, const char *api_version,
    const char *resource_name, const char *resource_id);

char *ogs_sbi_client_uri(ogs_sbi_client_t *client,
    const char *service_name, const char *api_version,
    const char *resource_name, const char *resource_id);

char *ogs_sbi_parse_url(char *url, const char *delim, char **saveptr);
ogs_sockaddr_t *ogs_sbi_getaddr_from_uri(char *uri);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_CONV_H */
