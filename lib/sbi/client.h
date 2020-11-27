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

#ifndef OGS_SBI_CLIENT_H
#define OGS_SBI_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_SETUP_SBI_CLIENT(__cTX, __pCLIENT) \
    do { \
        ogs_assert((__cTX)); \
        ogs_assert((__pCLIENT)); \
        if ((__cTX)->client != __pCLIENT) \
            __pCLIENT->reference_count++; \
        (__cTX)->client = __pCLIENT; \
        ogs_trace("client->reference_count = %d", __pCLIENT->reference_count); \
    } while(0)

typedef int (*ogs_sbi_client_cb_f)(ogs_sbi_response_t *response, void *data);

typedef struct ogs_sbi_client_s {
    ogs_socknode_t  node;

    struct {
        const char  *key;
        const char  *pem;
    } tls;

    ogs_sbi_client_cb_f cb;             /* Only used when NF send to NRF */

    ogs_timer_t     *t_curl;            /* timer for CURL */
    ogs_list_t      connection_list;    /* CURL connection list */

    void            *multi;             /* CURL multi handle */
    int             still_running;      /* number of running CURL handle */

    unsigned int    reference_count;    /* reference count for memory free */
} ogs_sbi_client_t;

typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;

void ogs_sbi_client_init(int num_of_sockinfo_pool, int num_of_connection_pool);
void ogs_sbi_client_final(void);

ogs_sbi_client_t *ogs_sbi_client_add(ogs_sockaddr_t *addr);
void ogs_sbi_client_remove(ogs_sbi_client_t *client);
ogs_sbi_client_t *ogs_sbi_client_find(ogs_sockaddr_t *addr);

void ogs_sbi_client_send_request(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_CLIENT_H */
