/*
 * Copyright (C) 2019,2024 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef OGS_SBI_SERVER_H
#define OGS_SBI_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>

typedef struct ogs_sbi_stream_s ogs_sbi_stream_t;

typedef struct ogs_sbi_server_s {
    ogs_socknode_t  node;
    ogs_sockaddr_t  *advertise;

    ogs_pool_id_t   id;

    char *interface;
    OpenAPI_uri_scheme_e scheme;
    char *private_key, *cert, *sslkeylog;
    bool verify_client;
    char *verify_client_cacert;

    SSL_CTX *ssl_ctx;

    int (*cb)(ogs_sbi_request_t *request, void *data);
    ogs_list_t      session_list;

    void            *mhd; /* Used by MHD */
} ogs_sbi_server_t;

typedef struct ogs_sbi_server_actions_s {
    void (*init)(int num_of_session_pool, int num_of_stream_pool);
    void (*cleanup)(void);

    int (*start)(ogs_sbi_server_t *server,
            int (*cb)(ogs_sbi_request_t *request, void *data));
    void (*stop)(ogs_sbi_server_t *server);

    bool (*send_rspmem_persistent)(
            ogs_sbi_stream_t *stream, ogs_sbi_response_t *response);
    bool (*send_response)(
            ogs_sbi_stream_t *stream, ogs_sbi_response_t *response);

    ogs_sbi_server_t *(*server_from_stream)(ogs_sbi_stream_t *stream);

    ogs_pool_id_t (*id_from_stream)(ogs_sbi_stream_t *stream);
    void *(*stream_find_by_id)(ogs_pool_id_t id);
} ogs_sbi_server_actions_t;

void ogs_sbi_server_init(int num_of_session_pool, int num_of_stream_pool);
void ogs_sbi_server_final(void);

ogs_sbi_server_t *ogs_sbi_server_add(
        const char *interface,
        OpenAPI_uri_scheme_e scheme, ogs_sockaddr_t *addr,
        ogs_sockopt_t *option);
void ogs_sbi_server_remove(ogs_sbi_server_t *server);
void ogs_sbi_server_remove_all(void);

void ogs_sbi_server_set_advertise(
        ogs_sbi_server_t *server, int family, ogs_sockaddr_t *advertise);

int ogs_sbi_server_start_all(
        int (*cb)(ogs_sbi_request_t *request, void *data));
void ogs_sbi_server_stop_all(void);

bool ogs_sbi_server_send_rspmem_persistent(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response);
bool ogs_sbi_server_send_response(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response);
bool ogs_sbi_server_send_error(ogs_sbi_stream_t *stream,
        int status, ogs_sbi_message_t *message,
        const char *title, const char *detail, const char *cause);

bool ogs_sbi_server_send_problem(
        ogs_sbi_stream_t *stream, OpenAPI_problem_details_t *problem);

ogs_sbi_server_t *ogs_sbi_server_from_stream(ogs_sbi_stream_t *stream);

ogs_pool_id_t ogs_sbi_id_from_stream(ogs_sbi_stream_t *stream);
void *ogs_sbi_stream_find_by_id(ogs_pool_id_t id);

ogs_sbi_server_t *ogs_sbi_server_first(void);
ogs_sbi_server_t *ogs_sbi_server_next(ogs_sbi_server_t *current);
ogs_sbi_server_t *ogs_sbi_server_first_by_interface(const char *interface);
ogs_sbi_server_t *ogs_sbi_server_next_by_interface(
        ogs_sbi_server_t *current, const char *interface);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_SERVER_H */
