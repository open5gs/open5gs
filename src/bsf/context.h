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

#ifndef BSF_CONTEXT_H
#define BSF_CONTEXT_H

#include "ogs-sbi.h"
#include "ogs-app.h"

#include "bsf-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __bsf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __bsf_log_domain

typedef struct bsf_context_s {
    ogs_hash_t          *ipv4addr_hash;
    ogs_hash_t          *ipv6prefix_hash;

    ogs_list_t          sess_list;
} bsf_context_t;

typedef struct bsf_sess_s {
    ogs_sbi_object_t sbi;

    char *binding_id;

    char *supi;
    char *gpsi;

    char *ipv4addr_string;
    char *ipv6prefix_string;

    OpenAPI_list_t *ipv4_frame_route_list;
    OpenAPI_list_t *ipv6_frame_route_list;

    uint32_t ipv4addr;
    struct {
        uint8_t len;
        uint8_t addr6[OGS_IPV6_LEN];
    } ipv6prefix;

    ogs_s_nssai_t s_nssai;
    char *dnn;

    /* PCF address information */
    char *pcf_fqdn;

    int num_of_pcf_ip;
    struct {
        char *addr;
        char *addr6;
        bool is_port;
        int port;
    } pcf_ip[OGS_SBI_MAX_NUM_OF_IP_ADDRESS];

    /* SBI Features */
    uint64_t management_features;

} bsf_sess_t;

void bsf_context_init(void);
void bsf_context_final(void);
bsf_context_t *bsf_self(void);

int bsf_context_parse_config(void);

bsf_sess_t *bsf_sess_add_by_ip_address(
            char *ipv4addr_string, char *ipv6prefix_string);
void bsf_sess_remove(bsf_sess_t *sess);
void bsf_sess_remove_all(void);

bool bsf_sess_set_ipv4addr(bsf_sess_t *sess, char *ipv4addr);
bool bsf_sess_set_ipv6prefix(bsf_sess_t *sess, char *ipv6prefix);

bsf_sess_t *bsf_sess_find(uint32_t index);
bsf_sess_t *bsf_sess_find_by_snssai_and_dnn(ogs_s_nssai_t *s_nssai, char *dnn);
bsf_sess_t *bsf_sess_find_by_binding_id(char *binding_id);
bsf_sess_t *bsf_sess_find_by_ipv4addr(char *ipv4addr_string);
bsf_sess_t *bsf_sess_find_by_ipv6prefix(char *ipv6prefix_string);
int get_sess_load(void);

#ifdef __cplusplus
}
#endif

#endif /* BSF_CONTEXT_H */
