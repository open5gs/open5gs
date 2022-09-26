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

#include "ogs-sbi.h"

int __ogs_sbi_domain;
static ogs_sbi_context_t self;
static int context_initialized = 0;

static OGS_POOL(nf_instance_pool, ogs_sbi_nf_instance_t);
static OGS_POOL(nf_service_pool, ogs_sbi_nf_service_t);
static OGS_POOL(xact_pool, ogs_sbi_xact_t);
static OGS_POOL(subscription_data_pool, ogs_sbi_subscription_data_t);
static OGS_POOL(smf_info_pool, ogs_sbi_smf_info_t);
static OGS_POOL(nf_info_pool, ogs_sbi_nf_info_t);

void ogs_sbi_context_init(void)
{
    char nf_instance_id[OGS_UUID_FORMATTED_LENGTH + 1];

    ogs_assert(context_initialized == 0);

    /* Initialize SBI context */
    memset(&self, 0, sizeof(ogs_sbi_context_t));

    ogs_log_install_domain(&__ogs_sbi_domain, "sbi", ogs_core()->log.level);

    ogs_sbi_message_init(ogs_app()->pool.message, ogs_app()->pool.message);
    ogs_sbi_server_init(ogs_app()->pool.event, ogs_app()->pool.event);
    ogs_sbi_client_init(ogs_app()->pool.event, ogs_app()->pool.event);

    ogs_list_init(&self.nf_instance_list);
    ogs_pool_init(&nf_instance_pool, ogs_app()->pool.nf);
    ogs_pool_init(&nf_service_pool, ogs_app()->pool.nf_service);

    ogs_pool_init(&xact_pool, ogs_app()->pool.xact);

    ogs_list_init(&self.subscription_data_list);
    ogs_pool_init(&subscription_data_pool, ogs_app()->pool.subscription);

    ogs_pool_init(&smf_info_pool, ogs_app()->pool.nf);

    ogs_pool_init(&nf_info_pool, ogs_app()->pool.nf * OGS_MAX_NUM_OF_NF_INFO);

    /* Add SELF NF instance */
    self.nf_instance = ogs_sbi_nf_instance_add();
    ogs_assert(self.nf_instance);

    ogs_uuid_get(&self.uuid);
    ogs_uuid_format(nf_instance_id, &self.uuid);

    ogs_sbi_nf_instance_set_id(self.nf_instance, nf_instance_id);

    context_initialized = 1;
}

void ogs_sbi_context_final(void)
{
    ogs_assert(context_initialized == 1);

    ogs_sbi_subscription_data_remove_all();
    ogs_pool_final(&subscription_data_pool);

    ogs_pool_final(&xact_pool);

    ogs_sbi_nf_instance_remove_all();

    ogs_pool_final(&nf_instance_pool);
    ogs_pool_final(&nf_service_pool);
    ogs_pool_final(&smf_info_pool);

    ogs_pool_final(&nf_info_pool);

    ogs_sbi_client_final();
    ogs_sbi_server_final();
    ogs_sbi_message_final();

    context_initialized = 0;
}

ogs_sbi_context_t *ogs_sbi_self(void)
{
    return &self;
}

static int ogs_sbi_context_prepare(void)
{
    self.sbi_port = OGS_SBI_HTTP_PORT;

#if ENABLE_ACCEPT_ENCODING
    self.content_encoding = "gzip";
#endif

    return OGS_OK;
}

static int ogs_sbi_context_validation(
        const char *local, const char *nrf, const char *scp)
{
    /* If SMF is only used in 4G EPC, no SBI interface is required.  */
    if (strcmp(local, "smf") != 0 &&
        ogs_list_first(&self.server_list) == NULL) {
        ogs_error("No %s.sbi: in '%s'", local, ogs_app()->file);
        return OGS_ERROR;
    }

    ogs_assert(context_initialized == 1);
    switch (self.discovery_config.delegated) {
    case OGS_SBI_DISCOVERY_DELEGATED_AUTO:
        if (strcmp(local, "nrf") != 0 && /* Skip NRF */
            strcmp(local, "smf") != 0 && /* Skip SMF since SMF can run 4G */
            ogs_sbi_self()->nrf_instance == NULL &&
            ogs_sbi_self()->scp_instance == NULL) {
            ogs_error("DELEGATED_AUTO - Both NRF and %s are unavailable",
                    strcmp(scp, "next_scp") == 0 ? "Next-hop SCP" : "SCP");
            return OGS_ERROR;
        }
        break;
    case OGS_SBI_DISCOVERY_DELEGATED_YES:
        if (ogs_sbi_self()->scp_instance == NULL) {
            ogs_error("DELEGATED_YES - no %s available",
                    strcmp(scp, "next_scp") == 0 ? "Next-hop SCP" : "SCP");
            return OGS_ERROR;
        }
        break;
    case OGS_SBI_DISCOVERY_DELEGATED_NO:
        if (ogs_sbi_self()->nrf_instance == NULL) {
            ogs_error("DELEGATED_NO - no NRF available");
            return OGS_ERROR;
        }
        break;
    default:
        ogs_fatal("Invalid dicovery-config delegated [%d]",
                    self.discovery_config.delegated);
        ogs_assert_if_reached();
    }

    return OGS_OK;
}

ogs_sbi_nf_instance_t *ogs_sbi_scp_instance(void)
{
    return NULL;
}

int ogs_sbi_context_parse_config(
        const char *local, const char *nrf, const char *scp)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = ogs_sbi_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (local && !strcmp(root_key, local)) {
            ogs_yaml_iter_t local_iter;
            ogs_yaml_iter_recurse(&root_iter, &local_iter);
            while (ogs_yaml_iter_next(&local_iter)) {
                const char *local_key = ogs_yaml_iter_key(&local_iter);
                ogs_assert(local_key);
                if (!strcmp(local_key, "sbi")) {
                    ogs_list_t list, list6;
                    ogs_socknode_t *node = NULL, *node6 = NULL;

                    ogs_yaml_iter_t sbi_array, sbi_iter;
                    ogs_yaml_iter_recurse(&local_iter, &sbi_array);
                    do {
                        int i, family = AF_UNSPEC;
                        int num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        int num_of_advertise = 0;
                        const char *advertise[OGS_MAX_NUM_OF_HOSTNAME];
                        const char *key = NULL;
                        const char *pem = NULL;

                        uint16_t port = self.sbi_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;

                        ogs_sockopt_t option;
                        bool is_option = false;

                        if (ogs_yaml_iter_type(&sbi_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&sbi_iter, &sbi_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&sbi_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&sbi_array))
                                break;
                            ogs_yaml_iter_recurse(&sbi_array, &sbi_iter);
                        } else if (ogs_yaml_iter_type(&sbi_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&sbi_iter)) {
                            const char *sbi_key =
                                ogs_yaml_iter_key(&sbi_iter);
                            ogs_assert(sbi_key);
                            if (!strcmp(sbi_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&sbi_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ",
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(sbi_key, "addr") ||
                                    !strcmp(sbi_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&sbi_iter,
                                        &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(
                                                    &hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num < OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] =
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(sbi_key, "advertise")) {
                                ogs_yaml_iter_t advertise_iter;
                                ogs_yaml_iter_recurse(&sbi_iter,
                                        &advertise_iter);
                                ogs_assert(ogs_yaml_iter_type(
                                    &advertise_iter) != YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&advertise_iter) ==
                                                YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(
                                                    &advertise_iter))
                                            break;
                                    }

                                    ogs_assert(num_of_advertise <
                                            OGS_MAX_NUM_OF_HOSTNAME);
                                    advertise[num_of_advertise++] =
                                        ogs_yaml_iter_value(&advertise_iter);
                                } while (
                                    ogs_yaml_iter_type(&advertise_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(sbi_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&sbi_iter);
                                if (v)
                                    port = atoi(v);
                            } else if (!strcmp(sbi_key, "dev")) {
                                dev = ogs_yaml_iter_value(&sbi_iter);
                            } else if (!strcmp(sbi_key, "option")) {
                                rv = ogs_app_config_parse_sockopt(
                                        &sbi_iter, &option);
                                if (rv != OGS_OK) return rv;
                                is_option = true;
                            } else if (!strcmp(sbi_key, "tls")) {
                                ogs_yaml_iter_t tls_iter;
                                ogs_yaml_iter_recurse(&sbi_iter, &tls_iter);

                                while (ogs_yaml_iter_next(&tls_iter)) {
                                    const char *tls_key =
                                        ogs_yaml_iter_key(&tls_iter);
                                    ogs_assert(tls_key);

                                    if (!strcmp(tls_key, "key")) {
                                        key = ogs_yaml_iter_value(&tls_iter);
                                    } else if (!strcmp(tls_key, "pem")) {
                                        pem = ogs_yaml_iter_value(&tls_iter);
                                    } else
                                        ogs_warn("unknown key `%s`", tls_key);
                                }
                            } else
                                ogs_warn("unknown key `%s`", sbi_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        ogs_list_init(&list);
                        ogs_list_init(&list6);

                        if (addr) {
                            if (ogs_app()->parameter.no_ipv4 == 0)
                                ogs_socknode_add(
                                    &list, AF_INET, addr, NULL);
                            if (ogs_app()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(
                                    &list6, AF_INET6, addr, NULL);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                ogs_app()->parameter.no_ipv4 ? NULL : &list,
                                ogs_app()->parameter.no_ipv6 ? NULL : &list6,
                                dev, port, NULL);
                            ogs_assert(rv == OGS_OK);
                        }

                        addr = NULL;
                        for (i = 0; i < num_of_advertise; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, advertise[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        node = ogs_list_first(&list);
                        if (node) {
                            ogs_sbi_server_t *server = ogs_sbi_server_add(
                                    node->addr, is_option ? &option : NULL);
                            ogs_assert(server);

                            if (addr && ogs_app()->parameter.no_ipv4 == 0)
                                ogs_sbi_server_set_advertise(
                                        server, AF_INET, addr);

                            if (key) server->tls.key = key;
                            if (pem) server->tls.pem = pem;
                        }
                        node6 = ogs_list_first(&list6);
                        if (node6) {
                            ogs_sbi_server_t *server = ogs_sbi_server_add(
                                    node6->addr, is_option ? &option : NULL);
                            ogs_assert(server);

                            if (addr && ogs_app()->parameter.no_ipv6 == 0)
                                ogs_sbi_server_set_advertise(
                                        server, AF_INET6, addr);

                            if (key) server->tls.key = key;
                            if (pem) server->tls.pem = pem;
                        }

                        if (addr)
                            ogs_freeaddrinfo(addr);

                        ogs_socknode_remove_all(&list);
                        ogs_socknode_remove_all(&list6);

                    } while (ogs_yaml_iter_type(&sbi_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.server_list) == 0) {
                        ogs_list_init(&list);
                        ogs_list_init(&list6);

                        rv = ogs_socknode_probe(
                            ogs_app()->parameter.no_ipv4 ? NULL : &list,
                            ogs_app()->parameter.no_ipv6 ? NULL : &list6,
                            NULL, self.sbi_port, NULL);
                        ogs_assert(rv == OGS_OK);

                        node = ogs_list_first(&list);
                        if (node) ogs_sbi_server_add(node->addr, NULL);
                        node6 = ogs_list_first(&list6);
                        if (node6) ogs_sbi_server_add(node6->addr, NULL);

                        ogs_socknode_remove_all(&list);
                        ogs_socknode_remove_all(&list6);
                    }
                } else if (!strcmp(local_key, "service_name")) {
                    ogs_yaml_iter_t service_name_iter;
                    ogs_yaml_iter_recurse(&local_iter, &service_name_iter);
                    ogs_assert(ogs_yaml_iter_type(
                                &service_name_iter) != YAML_MAPPING_NODE);

                    do {
                        const char *v = NULL;

                        if (ogs_yaml_iter_type(&service_name_iter) ==
                                YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&service_name_iter))
                                break;
                        }

                        v = ogs_yaml_iter_value(&service_name_iter);
                        if (v && strlen(v))
                            self.service_name[self.num_of_service_name++] = v;

                    } while (ogs_yaml_iter_type(
                                &service_name_iter) == YAML_SEQUENCE_NODE);

                } else if (!strcmp(local_key, "discovery")) {
                    ogs_yaml_iter_t discovery_iter;
                    ogs_yaml_iter_recurse(&local_iter, &discovery_iter);
                    while (ogs_yaml_iter_next(&discovery_iter)) {
                        const char *discovery_key =
                            ogs_yaml_iter_key(&discovery_iter);
                        ogs_assert(discovery_key);
                        if (!strcmp(discovery_key, "delegated")) {
                            const char *delegated =
                                ogs_yaml_iter_value(&discovery_iter);
                            if (!strcmp(delegated, "auto"))
                                self.discovery_config.delegated =
                                    OGS_SBI_DISCOVERY_DELEGATED_AUTO;
                            else if (!strcmp(delegated, "yes"))
                                self.discovery_config.delegated =
                                    OGS_SBI_DISCOVERY_DELEGATED_YES;
                            else if (!strcmp(delegated, "no"))
                                self.discovery_config.delegated =
                                    OGS_SBI_DISCOVERY_DELEGATED_NO;
                            else
                                ogs_warn("unknown 'delegated' value `%s`",
                                        delegated);
                        } else if (!strcmp(discovery_key, "option")) {
                            ogs_yaml_iter_t option_iter;
                            ogs_yaml_iter_recurse(
                                    &discovery_iter, &option_iter);

                            while (ogs_yaml_iter_next(&option_iter)) {
                                const char *option_key =
                                    ogs_yaml_iter_key(&option_iter);
                                ogs_assert(option_key);

                                if (!strcmp(option_key, "no_service_names")) {
                                    self.discovery_config.no_service_names =
                                        ogs_yaml_iter_bool(&option_iter);
                                } else if (!strcmp(option_key,
                                        "prefer_requester_nf_instance_id")) {
                                    self.discovery_config.
                                        prefer_requester_nf_instance_id =
                                            ogs_yaml_iter_bool(&option_iter);
                                } else
                                    ogs_warn("unknown key `%s`", option_key);
                            }
                        } else
                            ogs_warn("unknown key `%s`", discovery_key);
                    }
                }
            }
        } else if (nrf && !strcmp(root_key, nrf)) {
            ogs_yaml_iter_t nrf_iter;
            ogs_yaml_iter_recurse(&root_iter, &nrf_iter);
            while (ogs_yaml_iter_next(&nrf_iter)) {
                const char *nrf_key = ogs_yaml_iter_key(&nrf_iter);
                ogs_assert(nrf_key);
                if (!strcmp(nrf_key, "sbi")) {
                    ogs_yaml_iter_t sbi_array, sbi_iter;
                    ogs_yaml_iter_recurse(&nrf_iter, &sbi_array);
                    do {
                        ogs_sbi_nf_instance_t *nrf_instance = NULL;
                        ogs_sbi_client_t *client = NULL;
                        ogs_sockaddr_t *addr = NULL;
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.sbi_port;
                        const char *key = NULL;
                        const char *pem = NULL;

                        if (ogs_yaml_iter_type(&sbi_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&sbi_iter, &sbi_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&sbi_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&sbi_array))
                                break;
                            ogs_yaml_iter_recurse(&sbi_array, &sbi_iter);
                        } else if (ogs_yaml_iter_type(&sbi_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&sbi_iter)) {
                            const char *sbi_key =
                                ogs_yaml_iter_key(&sbi_iter);
                            ogs_assert(sbi_key);
                            if (!strcmp(sbi_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&sbi_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ",
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(sbi_key, "addr") ||
                                    !strcmp(sbi_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&sbi_iter,
                                        &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num < OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] =
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(sbi_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&sbi_iter);
                                if (v) port = atoi(v);
                            } else if (!strcmp(sbi_key, "tls")) {
                                ogs_yaml_iter_t tls_iter;
                                ogs_yaml_iter_recurse(&sbi_iter, &tls_iter);

                                while (ogs_yaml_iter_next(&tls_iter)) {
                                    const char *tls_key =
                                        ogs_yaml_iter_key(&tls_iter);
                                    ogs_assert(tls_key);

                                    if (!strcmp(tls_key, "key")) {
                                        key = ogs_yaml_iter_value(&tls_iter);
                                    } else if (!strcmp(tls_key, "pem")) {
                                        pem = ogs_yaml_iter_value(&tls_iter);
                                    } else
                                        ogs_warn("unknown key `%s`", tls_key);
                                }
                            } else if (!strcmp(sbi_key, "advertise")) {
                                /* Nothing in client */
                            } else
                                ogs_warn("unknown key `%s`", sbi_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        ogs_filter_ip_version(&addr,
                                ogs_app()->parameter.no_ipv4,
                                ogs_app()->parameter.no_ipv6,
                                ogs_app()->parameter.prefer_ipv4);

                        if (addr == NULL) continue;

                        client = ogs_sbi_client_add(addr);
                        ogs_assert(client);

                        ogs_sbi_self()->nrf_instance =
                            nrf_instance = ogs_sbi_nf_instance_add();
                        ogs_assert(nrf_instance);
                        ogs_sbi_nf_instance_set_type(
                                nrf_instance, OpenAPI_nf_type_NRF);

                        OGS_SBI_SETUP_CLIENT(nrf_instance, client);

                        if (key) client->tls.key = key;
                        if (pem) client->tls.pem = pem;

                        ogs_freeaddrinfo(addr);

                    } while (ogs_yaml_iter_type(&sbi_array) ==
                            YAML_SEQUENCE_NODE);
                }
            }
        } else if (scp && !strcmp(root_key, scp)) {
            ogs_yaml_iter_t scp_iter;
            ogs_yaml_iter_recurse(&root_iter, &scp_iter);
            while (ogs_yaml_iter_next(&scp_iter)) {
                const char *scp_key = ogs_yaml_iter_key(&scp_iter);
                ogs_assert(scp_key);
                if (!strcmp(scp_key, "sbi")) {
                    ogs_yaml_iter_t sbi_array, sbi_iter;
                    ogs_yaml_iter_recurse(&scp_iter, &sbi_array);
                    do {
                        ogs_sbi_nf_instance_t *scp_instance = NULL;
                        ogs_sbi_client_t *client = NULL;
                        ogs_sockaddr_t *addr = NULL;
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.sbi_port;
                        const char *key = NULL;
                        const char *pem = NULL;

                        if (ogs_yaml_iter_type(&sbi_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&sbi_iter, &sbi_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&sbi_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&sbi_array))
                                break;
                            ogs_yaml_iter_recurse(&sbi_array, &sbi_iter);
                        } else if (ogs_yaml_iter_type(&sbi_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&sbi_iter)) {
                            const char *sbi_key =
                                ogs_yaml_iter_key(&sbi_iter);
                            ogs_assert(sbi_key);
                            if (!strcmp(sbi_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&sbi_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ",
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(sbi_key, "addr") ||
                                    !strcmp(sbi_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&sbi_iter,
                                        &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num < OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] =
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(sbi_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&sbi_iter);
                                if (v) port = atoi(v);
                            } else if (!strcmp(sbi_key, "tls")) {
                                ogs_yaml_iter_t tls_iter;
                                ogs_yaml_iter_recurse(&sbi_iter, &tls_iter);

                                while (ogs_yaml_iter_next(&tls_iter)) {
                                    const char *tls_key =
                                        ogs_yaml_iter_key(&tls_iter);
                                    ogs_assert(tls_key);

                                    if (!strcmp(tls_key, "key")) {
                                        key = ogs_yaml_iter_value(&tls_iter);
                                    } else if (!strcmp(tls_key, "pem")) {
                                        pem = ogs_yaml_iter_value(&tls_iter);
                                    } else
                                        ogs_warn("unknown key `%s`", tls_key);
                                }
                            } else if (!strcmp(sbi_key, "advertise")) {
                                /* Nothing in client */
                            } else
                                ogs_warn("unknown key `%s`", sbi_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        ogs_filter_ip_version(&addr,
                                ogs_app()->parameter.no_ipv4,
                                ogs_app()->parameter.no_ipv6,
                                ogs_app()->parameter.prefer_ipv4);

                        if (addr == NULL) continue;

                        client = ogs_sbi_client_add(addr);
                        ogs_assert(client);

                        ogs_sbi_self()->scp_instance =
                            scp_instance = ogs_sbi_nf_instance_add();
                        ogs_assert(scp_instance);
                        ogs_sbi_nf_instance_set_type(
                                scp_instance, OpenAPI_nf_type_SCP);

                        OGS_SBI_SETUP_CLIENT(scp_instance, client);

                        if (key) client->tls.key = key;
                        if (pem) client->tls.pem = pem;

                        ogs_freeaddrinfo(addr);

                    } while (ogs_yaml_iter_type(&sbi_array) ==
                            YAML_SEQUENCE_NODE);
                }
            }
        }
    }

    rv = ogs_sbi_context_validation(local, nrf, scp);
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

bool ogs_sbi_nf_service_is_available(const char *name)
{
    int i;

    ogs_assert(name);

    if (self.num_of_service_name == 0)
        /* If no service name is configured, all services are available */
        return true;

    for (i = 0; i < self.num_of_service_name; i++)
        /* Only services in the configuration are available */
        if (strcmp(self.service_name[i], name) == 0)
            return true;

    return false;
}

ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_add(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_pool_alloc(&nf_instance_pool, &nf_instance);
    ogs_assert(nf_instance);
    memset(nf_instance, 0, sizeof(ogs_sbi_nf_instance_t));

    ogs_debug("ogs_sbi_nf_instance_add()");

    OGS_OBJECT_REF(nf_instance);

    nf_instance->time.heartbeat_interval =
            ogs_app()->time.nf_instance.heartbeat_interval;

    nf_instance->priority = OGS_SBI_DEFAULT_PRIORITY;
    nf_instance->capacity = OGS_SBI_DEFAULT_CAPACITY;
    nf_instance->load = OGS_SBI_DEFAULT_LOAD;

    ogs_list_add(&ogs_sbi_self()->nf_instance_list, nf_instance);

    return nf_instance;
}

void ogs_sbi_nf_instance_set_id(ogs_sbi_nf_instance_t *nf_instance, char *id)
{
    ogs_assert(nf_instance);
    ogs_assert(id);

    nf_instance->id = ogs_strdup(id);
    ogs_assert(nf_instance->id);
}

void ogs_sbi_nf_instance_set_type(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e nf_type)
{
    ogs_assert(nf_instance);
    ogs_assert(nf_type);

    nf_instance->nf_type = nf_type;
}

void ogs_sbi_nf_instance_set_status(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_status_e nf_status)
{
    ogs_assert(nf_instance);
    ogs_assert(nf_status);

    nf_instance->nf_status = nf_status;
}

void ogs_sbi_nf_instance_add_allowed_nf_type(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e allowed_nf_type)
{
    ogs_assert(nf_instance);
    ogs_assert(allowed_nf_type);

    if (nf_instance->num_of_allowed_nf_type < OGS_SBI_MAX_NUM_OF_NF_TYPE) {
        nf_instance->allowed_nf_type[nf_instance->num_of_allowed_nf_type] =
            allowed_nf_type;
        nf_instance->num_of_allowed_nf_type++;
    }
}

bool ogs_sbi_nf_instance_is_allowed_nf_type(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e allowed_nf_type)
{
    int i;

    ogs_assert(nf_instance);
    ogs_assert(allowed_nf_type);

    if (!nf_instance->num_of_allowed_nf_type) {
        return true;
    }

    for (i = 0; i < nf_instance->num_of_allowed_nf_type; i++) {
        if (nf_instance->allowed_nf_type[i] == allowed_nf_type)
            return true;
    }

    ogs_error("Not allowed nf-type[%s] in nf-instance[%s]",
            OpenAPI_nf_type_ToString(allowed_nf_type),
            OpenAPI_nf_type_ToString(nf_instance->nf_type));
    return false;
}

void ogs_sbi_nf_instance_clear(ogs_sbi_nf_instance_t *nf_instance)
{
    int i;

    ogs_assert(nf_instance);

    if (nf_instance->fqdn)
        ogs_free(nf_instance->fqdn);
    nf_instance->fqdn = NULL;

    for (i = 0; i < nf_instance->num_of_ipv4; i++) {
        if (nf_instance->ipv4[i])
            ogs_freeaddrinfo(nf_instance->ipv4[i]);
    }
    nf_instance->num_of_ipv4 = 0;

    for (i = 0; i < nf_instance->num_of_ipv6; i++) {
        if (nf_instance->ipv6[i])
            ogs_freeaddrinfo(nf_instance->ipv6[i]);
    }
    nf_instance->num_of_ipv6 = 0;

    nf_instance->num_of_allowed_nf_type = 0;
}

void ogs_sbi_nf_instance_remove(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_assert(nf_instance);

    ogs_debug("ogs_sbi_nf_instance_remove()");

    if (OGS_OBJECT_IS_REF(nf_instance)) {
        OGS_OBJECT_UNREF(nf_instance);
        return;
    }

    ogs_list_remove(&ogs_sbi_self()->nf_instance_list, nf_instance);

    ogs_sbi_nf_info_remove_all(&nf_instance->nf_info_list);

    ogs_sbi_nf_service_remove_all(nf_instance);

    ogs_sbi_nf_instance_clear(nf_instance);

    if (nf_instance->id) {
        ogs_sbi_subscription_data_remove_all_by_nf_instance_id(nf_instance->id);
        ogs_free(nf_instance->id);
    }

    if (nf_instance->client)
        ogs_sbi_client_remove(nf_instance->client);

    ogs_pool_free(&nf_instance_pool, nf_instance);
}

void ogs_sbi_nf_instance_remove_all(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL, *next_nf_instance = NULL;

    ogs_list_for_each_safe(
            &ogs_sbi_self()->nf_instance_list, next_nf_instance, nf_instance)
        ogs_sbi_nf_instance_remove(nf_instance);
}

ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find(char *id)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(id);

    ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance) {
        if (nf_instance->id && strcmp(nf_instance->id, id) == 0)
            break;
    }

    return nf_instance;
}

ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find_by_discovery_param(
        OpenAPI_nf_type_e target_nf_type,
        ogs_sbi_discovery_option_t *discovery_option)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(target_nf_type);

    ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance) {
        if (ogs_sbi_discovery_param_is_matched(
                    nf_instance, target_nf_type, discovery_option) == false)
            continue;

        return nf_instance;
    }

    return NULL;
}

ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find_by_service_type(
        ogs_sbi_service_type_e service_type)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;

    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    char *service_name = NULL;

    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    service_name = (char *)ogs_sbi_service_type_to_name(service_type);
    ogs_assert(service_name);

    discovery_option = ogs_sbi_discovery_option_new();
    ogs_assert(discovery_option);
    ogs_sbi_discovery_option_add_service_names(discovery_option, service_name);

    nf_instance = ogs_sbi_nf_instance_find_by_discovery_param(
            target_nf_type, discovery_option);

    ogs_sbi_discovery_option_free(discovery_option);

    return nf_instance;
}

bool ogs_sbi_nf_instance_maximum_number_is_reached()
{
    return nf_instance_pool.avail <= 0;
}

ogs_sbi_nf_service_t *ogs_sbi_nf_service_add(
        ogs_sbi_nf_instance_t *nf_instance,
        char *id, const char *name, OpenAPI_uri_scheme_e scheme)
{
    ogs_sbi_nf_service_t *nf_service = NULL;

    ogs_assert(nf_instance);
    ogs_assert(id);
    ogs_assert(name);

    ogs_pool_alloc(&nf_service_pool, &nf_service);
    ogs_assert(nf_service);
    memset(nf_service, 0, sizeof(ogs_sbi_nf_service_t));

    nf_service->id = ogs_strdup(id);
    ogs_assert(nf_service->id);
    nf_service->name = ogs_strdup(name);
    ogs_assert(nf_service->name);
    nf_service->scheme = scheme;

    nf_service->status = OpenAPI_nf_service_status_REGISTERED;

    nf_service->priority = OGS_SBI_DEFAULT_PRIORITY;
    nf_service->capacity = OGS_SBI_DEFAULT_CAPACITY;
    nf_service->load = OGS_SBI_DEFAULT_LOAD;

    nf_service->nf_instance = nf_instance;

    ogs_list_add(&nf_instance->nf_service_list, nf_service);

    return nf_service;
}

void ogs_sbi_nf_service_add_version(ogs_sbi_nf_service_t *nf_service,
        const char *in_uri, const char *full, const char *expiry)
{
    ogs_assert(nf_service);

    ogs_assert(in_uri);
    ogs_assert(full);

    if (nf_service->num_of_version < OGS_SBI_MAX_NUM_OF_SERVICE_VERSION) {
        nf_service->version[nf_service->num_of_version].in_uri =
            ogs_strdup(in_uri);
        ogs_assert(nf_service->version[nf_service->num_of_version].in_uri);
        nf_service->version[nf_service->num_of_version].full =
            ogs_strdup(full);
        ogs_assert(nf_service->version[nf_service->num_of_version].full);
        if (expiry) {
            nf_service->version[nf_service->num_of_version].expiry =
                ogs_strdup(expiry);
            ogs_assert(
                nf_service->version[nf_service->num_of_version].expiry);

        }
        nf_service->num_of_version++;
    }
}

void ogs_sbi_nf_service_add_allowed_nf_type(
        ogs_sbi_nf_service_t *nf_service, OpenAPI_nf_type_e allowed_nf_type)
{
    ogs_assert(nf_service);
    ogs_assert(allowed_nf_type);

    if (nf_service->num_of_allowed_nf_type < OGS_SBI_MAX_NUM_OF_NF_TYPE) {
        nf_service->allowed_nf_type[nf_service->num_of_allowed_nf_type] =
            allowed_nf_type;
        nf_service->num_of_allowed_nf_type++;
    }
}

bool ogs_sbi_nf_service_is_allowed_nf_type(
        ogs_sbi_nf_service_t *nf_service, OpenAPI_nf_type_e allowed_nf_type)
{
    int i;

    ogs_assert(nf_service);
    ogs_assert(allowed_nf_type);

    if (!nf_service->num_of_allowed_nf_type) {
        return true;
    }

    for (i = 0; i < nf_service->num_of_allowed_nf_type; i++) {
        if (nf_service->allowed_nf_type[i] == allowed_nf_type)
            return true;
    }

    ogs_assert(nf_service->name);
    ogs_error("Not allowed nf-type[%s] in nf-service[%s]",
            OpenAPI_nf_type_ToString(allowed_nf_type),
            nf_service->name);
    return false;
}

void ogs_sbi_nf_service_clear(ogs_sbi_nf_service_t *nf_service)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    int i;

    ogs_assert(nf_service);
    nf_instance = nf_service->nf_instance;
    ogs_assert(nf_instance);

    if (nf_service->fqdn)
        ogs_free(nf_service->fqdn);
    nf_service->fqdn = NULL;

    for (i = 0; i < nf_service->num_of_version; i++) {
        if (nf_service->version[i].in_uri)
            ogs_free(nf_service->version[i].in_uri);
        if (nf_service->version[i].full)
            ogs_free(nf_service->version[i].full);
        if (nf_service->version[i].expiry)
            ogs_free(nf_service->version[i].expiry);
    }
    nf_service->num_of_version = 0;

    for (i = 0; i < nf_service->num_of_addr; i++) {
        if (nf_service->addr[i].ipv4)
            ogs_freeaddrinfo(nf_service->addr[i].ipv4);
        if (nf_service->addr[i].ipv6)
            ogs_freeaddrinfo(nf_service->addr[i].ipv6);
    }
    nf_service->num_of_addr = 0;

    nf_service->num_of_allowed_nf_type = 0;
}

void ogs_sbi_nf_service_remove(ogs_sbi_nf_service_t *nf_service)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(nf_service);
    nf_instance = nf_service->nf_instance;
    ogs_assert(nf_instance);

    ogs_list_remove(&nf_instance->nf_service_list, nf_service);

    ogs_assert(nf_service->id);
    ogs_free(nf_service->id);

    ogs_assert(nf_service->name);
    ogs_free(nf_service->name);

    ogs_sbi_nf_service_clear(nf_service);

    if (nf_service->client)
        ogs_sbi_client_remove(nf_service->client);

    ogs_pool_free(&nf_service_pool, nf_service);
}

void ogs_sbi_nf_service_remove_all(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_nf_service_t *nf_service = NULL, *next_nf_service = NULL;

    ogs_assert(nf_instance);

    ogs_list_for_each_safe(&nf_instance->nf_service_list,
            next_nf_service, nf_service)
        ogs_sbi_nf_service_remove(nf_service);
}

ogs_sbi_nf_service_t *ogs_sbi_nf_service_find_by_id(
        ogs_sbi_nf_instance_t *nf_instance, char *id)
{
    ogs_sbi_nf_service_t *nf_service = NULL;

    ogs_assert(nf_instance);
    ogs_assert(id);

    ogs_list_for_each(&nf_instance->nf_service_list, nf_service) {
        ogs_assert(nf_service->id);
        if (strcmp(nf_service->id, id) == 0)
            break;
    }

    return nf_service;
}

ogs_sbi_nf_service_t *ogs_sbi_nf_service_find_by_name(
        ogs_sbi_nf_instance_t *nf_instance, char *name)
{
    ogs_sbi_nf_service_t *nf_service = NULL;

    ogs_assert(nf_instance);
    ogs_assert(name);

    ogs_list_for_each(&nf_instance->nf_service_list, nf_service) {
        ogs_assert(nf_service->name);
        if (strcmp(nf_service->name, name) == 0)
            return nf_service;
    }

    return NULL;
}

ogs_sbi_nf_info_t *ogs_sbi_nf_info_add(
        ogs_list_t *list, OpenAPI_nf_type_e nf_type)
{
    ogs_sbi_nf_info_t *nf_info = NULL;

    ogs_assert(list);
    ogs_assert(nf_type);

    ogs_pool_alloc(&nf_info_pool, &nf_info);
    ogs_assert(nf_info);
    memset(nf_info, 0, sizeof(*nf_info));

    nf_info->nf_type = nf_type;

    ogs_list_add(list, nf_info);

    return nf_info;
}

static void amf_info_free(ogs_sbi_amf_info_t *amf_info)
{
    /* Nothing */
}

static void smf_info_free(ogs_sbi_smf_info_t *smf_info)
{
    int i, j;
    ogs_assert(smf_info);

    for (i = 0; i < smf_info->num_of_slice; i++) {
        for (j = 0; j < smf_info->slice[i].num_of_dnn; j++)
            ogs_free(smf_info->slice[i].dnn[j]);
        smf_info->slice[i].num_of_dnn = 0;
    }
    smf_info->num_of_slice = 0;
    smf_info->num_of_nr_tai = 0;
    smf_info->num_of_nr_tai_range = 0;

    ogs_pool_free(&smf_info_pool, smf_info);
}

void ogs_sbi_nf_info_remove(ogs_list_t *list, ogs_sbi_nf_info_t *nf_info)
{
    ogs_assert(list);
    ogs_assert(nf_info);

    ogs_list_remove(list, nf_info);

    switch(nf_info->nf_type) {
    case OpenAPI_nf_type_AMF:
        amf_info_free(&nf_info->amf);
        break;
    case OpenAPI_nf_type_SMF:
        smf_info_free(&nf_info->smf);
        break;
    default:
        ogs_fatal("Not implemented NF-type[%s]",
                OpenAPI_nf_type_ToString(nf_info->nf_type));
        ogs_assert_if_reached();
    }

    ogs_pool_free(&nf_info_pool, nf_info);
}

void ogs_sbi_nf_info_remove_all(ogs_list_t *list)
{
    ogs_sbi_nf_info_t *nf_info = NULL, *next_nf_info = NULL;

    ogs_assert(list);

    ogs_list_for_each_safe(list, next_nf_info, nf_info)
        ogs_sbi_nf_info_remove(list, nf_info);
}

ogs_sbi_nf_info_t *ogs_sbi_nf_info_find(
        ogs_list_t *list, OpenAPI_nf_type_e nf_type)
{
    ogs_sbi_nf_info_t *nf_info = NULL;

    ogs_assert(list);
    ogs_assert(nf_type);

    ogs_list_for_each(list, nf_info) {
        if (nf_info->nf_type == nf_type)
            return nf_info;
    }

    return NULL;
}

void ogs_sbi_nf_instance_build_default(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e nf_type)
{
    ogs_sbi_server_t *server = NULL;
    char *hostname = NULL;

    ogs_assert(nf_instance);
    ogs_assert(nf_type);

    ogs_sbi_nf_instance_set_type(nf_instance, nf_type);
    ogs_sbi_nf_instance_set_status(nf_instance, OpenAPI_nf_status_REGISTERED);

    hostname = NULL;
    ogs_list_for_each(&ogs_sbi_self()->server_list, server) {
        ogs_sockaddr_t *advertise = NULL;

        advertise = server->advertise;
        if (!advertise)
            advertise = server->node.addr;
        ogs_assert(advertise);

        /* First FQDN is selected */
        if (!hostname) {
            hostname = ogs_gethostname(advertise);
            if (hostname)
                continue;
        }

        if (nf_instance->num_of_ipv4 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {
            ogs_sockaddr_t *addr = NULL;
            ogs_assert(OGS_OK == ogs_copyaddrinfo(&addr, advertise));
            ogs_assert(addr);

            if (addr->ogs_sa_family == AF_INET) {
                nf_instance->ipv4[nf_instance->num_of_ipv4] = addr;
                nf_instance->num_of_ipv4++;
            } else if (addr->ogs_sa_family == AF_INET6) {
                nf_instance->ipv6[nf_instance->num_of_ipv6] = addr;
                nf_instance->num_of_ipv6++;
            } else
                ogs_assert_if_reached();
        }
    }

    if (hostname) {
        nf_instance->fqdn = ogs_strdup(hostname);
        ogs_assert(nf_instance->fqdn);
    }

    nf_instance->time.heartbeat_interval =
            ogs_app()->time.nf_instance.heartbeat_interval;
}

ogs_sbi_nf_service_t *ogs_sbi_nf_service_build_default(
        ogs_sbi_nf_instance_t *nf_instance, const char *name)
{
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_nf_service_t *nf_service = NULL;
    ogs_uuid_t uuid;
    char id[OGS_UUID_FORMATTED_LENGTH + 1];
    char *hostname = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;

    ogs_assert(nf_instance);
    ogs_assert(name);

    ogs_uuid_get(&uuid);
    ogs_uuid_format(id, &uuid);

    ogs_list_for_each(&ogs_sbi_self()->server_list, server) {
        OpenAPI_uri_scheme_e s;

        if (server->tls.key && server->tls.pem)
            s = OpenAPI_uri_scheme_https;
        else
            s = OpenAPI_uri_scheme_http;

        if (scheme == OpenAPI_uri_scheme_NULL) {
            scheme = s;
        } else if (scheme != s) {
            ogs_fatal("Please CHECK CONFIGURATION - sbi[%d:%s]",
                    nf_instance->nf_type,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            ogs_assert_if_reached();
            return NULL;
        }
    }

    nf_service = ogs_sbi_nf_service_add(nf_instance, id, name, scheme);
    ogs_assert(nf_service);

    hostname = NULL;
    ogs_list_for_each(&ogs_sbi_self()->server_list, server) {
        ogs_sockaddr_t *advertise = NULL;

        advertise = server->advertise;
        if (!advertise)
            advertise = server->node.addr;
        ogs_assert(advertise);

        /* First FQDN is selected */
        if (!hostname) {
            hostname = ogs_gethostname(advertise);
            if (hostname)
                continue;
        }

        if (nf_service->num_of_addr < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {
            int port = 0;
            ogs_sockaddr_t *addr = NULL;
            ogs_assert(OGS_OK == ogs_copyaddrinfo(&addr, advertise));
            ogs_assert(addr);

            port = OGS_PORT(addr);
            if (nf_service->scheme == OpenAPI_uri_scheme_https) {
                if (port == OGS_SBI_HTTPS_PORT) port = 0;
            } else if (nf_service->scheme == OpenAPI_uri_scheme_http) {
                if (port == OGS_SBI_HTTP_PORT) port = 0;
            }

            nf_service->addr[nf_service->num_of_addr].port = port;
            if (addr->ogs_sa_family == AF_INET) {
                nf_service->addr[nf_service->num_of_addr].ipv4 = addr;
            } else if (addr->ogs_sa_family == AF_INET6) {
                nf_service->addr[nf_service->num_of_addr].ipv6 = addr;
            } else
                ogs_assert_if_reached();

            nf_service->num_of_addr++;
        }
    }

    if (hostname) {
        nf_service->fqdn = ogs_strdup(hostname);
        ogs_assert(nf_service->fqdn);
    }

    ogs_info("NF Service [%s]", nf_service->name);

    return nf_service;
}

static ogs_sbi_client_t *find_client_by_fqdn(char *fqdn, int port)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_sbi_client_t *client = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, fqdn,
            port ? port : ogs_sbi_self()->sbi_port, 0);
    if (rv != OGS_OK) {
        ogs_error("Invalid NFProfile.fqdn");
        return NULL;
    }

    client = ogs_sbi_client_find(addr);
    if (!client) {
        client = ogs_sbi_client_add(addr);
        ogs_assert(client);
    }

    ogs_freeaddrinfo(addr);

    return client;
}

static ogs_sbi_client_t *nf_instance_find_client(
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_client_t *client = NULL;
    ogs_sockaddr_t *addr = NULL;

    if (nf_instance->fqdn)
        client = find_client_by_fqdn(nf_instance->fqdn, 0);

    if (!client) {
        /* At this point, CLIENT selection method is very simple. */
        if (nf_instance->num_of_ipv4) addr = nf_instance->ipv4[0];
        if (nf_instance->num_of_ipv6) addr = nf_instance->ipv6[0];

        if (addr) {
            client = ogs_sbi_client_find(addr);
            if (!client) {
                client = ogs_sbi_client_add(addr);
                ogs_assert(client);
            }
        }
    }

    return client;
}

static void nf_service_associate_client(ogs_sbi_nf_service_t *nf_service)
{
    ogs_sbi_client_t *client = NULL;
    ogs_sockaddr_t *addr = NULL;

    if (nf_service->fqdn)
        client = find_client_by_fqdn(nf_service->fqdn, 0);

    if (!client) {
        /* At this point, CLIENT selection method is very simple. */
        if (nf_service->num_of_addr) {
            addr = nf_service->addr[0].ipv6;
            if (!addr)
                addr = nf_service->addr[0].ipv4;
        }

        if (addr) {
            client = ogs_sbi_client_find(addr);
            if (!client) {
                client = ogs_sbi_client_add(addr);
                ogs_assert(client);
            }
        }
    }

    if (client)
        OGS_SBI_SETUP_CLIENT(nf_service, client);
}

static void nf_service_associate_client_all(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_nf_service_t *nf_service = NULL;

    ogs_assert(nf_instance);

    ogs_list_for_each(&nf_instance->nf_service_list, nf_service)
        nf_service_associate_client(nf_service);
}

bool ogs_sbi_discovery_option_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option)
{
    ogs_assert(nf_instance);
    ogs_assert(requester_nf_type);
    ogs_assert(discovery_option);

    if (discovery_option->target_nf_instance_id &&
        nf_instance->id && strcmp(nf_instance->id,
            discovery_option->target_nf_instance_id) != 0) {
        return false;
    }

    if (discovery_option->num_of_service_names) {
        ogs_sbi_nf_service_t *nf_service = NULL;

        bool exist = false;
        int i;

        ogs_list_for_each(&nf_instance->nf_service_list, nf_service) {
            for (i = 0; i < discovery_option->num_of_service_names; i++) {
                if (nf_service->name &&
                    discovery_option->service_names[i] &&
                    strcmp(nf_service->name,
                        discovery_option->service_names[i]) == 0) {
                    if (ogs_sbi_nf_service_is_allowed_nf_type(
                            nf_service, requester_nf_type) == true) {
                        exist = true;
                        break;
                    }
                }
            }
            if (exist == true) break;
        }
        if (exist == false) return false;
    }

    return true;
}

bool ogs_sbi_discovery_param_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e target_nf_type,
        ogs_sbi_discovery_option_t *discovery_option)
{
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;

    ogs_assert(nf_instance);
    ogs_assert(target_nf_type);
    ogs_assert(ogs_sbi_self()->nf_instance);
    requester_nf_type = ogs_sbi_self()->nf_instance->nf_type;
    ogs_assert(requester_nf_type);

    if (!OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_registered))
        return false;

    if (nf_instance->nf_type != target_nf_type)
        return false;

    if (discovery_option &&
        ogs_sbi_discovery_option_is_matched(
            nf_instance, requester_nf_type, discovery_option) == false)
        return false;

    return true;
}

void ogs_sbi_client_associate(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);

    client = nf_instance_find_client(nf_instance);
    ogs_assert(client);

    OGS_SBI_SETUP_CLIENT(nf_instance, client);

    nf_service_associate_client_all(nf_instance);
}

OpenAPI_uri_scheme_e ogs_sbi_default_uri_scheme(void)
{
    return OpenAPI_uri_scheme_http;
}

ogs_sbi_client_t *ogs_sbi_client_find_by_service_name(
        ogs_sbi_nf_instance_t *nf_instance, char *name, char *version)
{
    ogs_sbi_nf_service_t *nf_service = NULL;
    int i;

    ogs_assert(nf_instance);
    ogs_assert(name);
    ogs_assert(version);

    ogs_list_for_each(&nf_instance->nf_service_list, nf_service) {
        ogs_assert(nf_service->name);
        if (strcmp(nf_service->name, name) == 0) {
            for (i = 0; i < nf_service->num_of_version; i++) {
                if (strcmp(nf_service->version[i].in_uri, version) == 0) {
                    return nf_service->client;
                }
            }
        }
    }

    ogs_error("[Fallback] Cannot find NF service[%s:%s]", name, version);
    return nf_instance->client;
}

void ogs_sbi_object_free(ogs_sbi_object_t *sbi_object)
{
    int i;

    ogs_assert(sbi_object);

    if (ogs_list_count(&sbi_object->xact_list))
        ogs_error("SBI running [%d]", ogs_list_count(&sbi_object->xact_list));

    for (i = 0; i < OGS_SBI_MAX_NUM_OF_SERVICE_TYPE; i++) {
        ogs_sbi_nf_instance_t *nf_instance =
            sbi_object->service_type_array[i].nf_instance;
        if (nf_instance)
            ogs_sbi_nf_instance_remove(nf_instance);
    }
    for (i = 0; i < OGS_SBI_MAX_NUM_OF_NF_TYPE; i++) {
        ogs_sbi_nf_instance_t *nf_instance =
            sbi_object->nf_type_array[i].nf_instance;
        if (nf_instance)
            ogs_sbi_nf_instance_remove(nf_instance);
    }
}

ogs_sbi_xact_t *ogs_sbi_xact_add(
        ogs_sbi_object_t *sbi_object,
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_build_f build, void *context, void *data)
{
    ogs_sbi_xact_t *xact = NULL;

    ogs_assert(sbi_object);

    ogs_pool_alloc(&xact_pool, &xact);
    ogs_expect_or_return_val(xact, NULL);
    memset(xact, 0, sizeof(ogs_sbi_xact_t));

    xact->sbi_object = sbi_object;
    xact->service_type = service_type;

    /* Always insert one service-name in the discovery option */
    if (!discovery_option) {
        discovery_option = ogs_sbi_discovery_option_new();
        ogs_assert(discovery_option);
    }

    if (!discovery_option->num_of_service_names) {
        ogs_sbi_discovery_option_add_service_names(
                discovery_option,
                (char *)ogs_sbi_service_type_to_name(service_type));
    }
    xact->discovery_option = discovery_option;

    xact->t_response = ogs_timer_add(
            ogs_app()->timer_mgr, ogs_timer_sbi_client_wait_expire, xact);
    if (!xact->t_response) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_free(&xact_pool, xact);
        return NULL;
    }

    ogs_timer_start(xact->t_response,
            ogs_app()->time.message.sbi.client_wait_duration);

    if (build) {
        xact->request = (*build)(context, data);
        if (!xact->request) {
            ogs_error("SBI build failed");
            ogs_timer_delete(xact->t_response);
            ogs_pool_free(&xact_pool, xact);
            return NULL;
        }
        if (!xact->request->h.uri) {
            const char *service_name = NULL;

            ogs_assert(xact->service_type);
            service_name = ogs_sbi_service_type_to_name(xact->service_type);
            ogs_assert(service_name);
            ogs_assert(xact->request->h.service.name);

            /*
             * Make sure the service matches
             * between discover and build functions:
             *
             * DISCOVER : amf_ue_sbi_discover_and_send(
             *              OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
             * BUILD    : amf_npcf_am_policy_control_build_create()
             *            message.h.service.name =
             *              (char *)OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL;
             */

            if (strcmp(service_name, xact->request->h.service.name) != 0) {
                ogs_fatal("[%s:%d] is not the same with [%s]",
                            service_name, xact->service_type,
                            xact->request->h.service.name);
                ogs_assert_if_reached();
            }
        }
    }

    ogs_list_add(&sbi_object->xact_list, xact);

    return xact;
}

void ogs_sbi_xact_remove(ogs_sbi_xact_t *xact)
{
    ogs_sbi_object_t *sbi_object = NULL;

    ogs_assert(xact);

    sbi_object = xact->sbi_object;
    ogs_assert(sbi_object);

    if (xact->discovery_option)
        ogs_sbi_discovery_option_free(xact->discovery_option);

    ogs_assert(xact->t_response);
    ogs_timer_delete(xact->t_response);

    if (xact->request)
        ogs_sbi_request_free(xact->request);

    ogs_list_remove(&sbi_object->xact_list, xact);
    ogs_pool_free(&xact_pool, xact);
}

void ogs_sbi_xact_remove_all(ogs_sbi_object_t *sbi_object)
{
    ogs_sbi_xact_t *xact = NULL, *next_xact = NULL;

    ogs_assert(sbi_object);

    ogs_list_for_each_safe(&sbi_object->xact_list, next_xact, xact)
        ogs_sbi_xact_remove(xact);
}

ogs_sbi_xact_t *ogs_sbi_xact_cycle(ogs_sbi_xact_t *xact)
{
    return ogs_pool_cycle(&xact_pool, xact);
}

ogs_sbi_subscription_data_t *ogs_sbi_subscription_data_add(void)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;

    ogs_pool_alloc(&subscription_data_pool, &subscription_data);
    ogs_assert(subscription_data);
    memset(subscription_data, 0, sizeof(ogs_sbi_subscription_data_t));

    subscription_data->time.validity_duration =
            ogs_app()->time.subscription.validity_duration;

    ogs_list_add(&ogs_sbi_self()->subscription_data_list, subscription_data);

    return subscription_data;
}

void ogs_sbi_subscription_data_set_id(
        ogs_sbi_subscription_data_t *subscription_data, char *id)
{
    ogs_assert(subscription_data);
    ogs_assert(id);

    subscription_data->id = ogs_strdup(id);
    ogs_assert(subscription_data->id);
}

void ogs_sbi_subscription_data_remove(
        ogs_sbi_subscription_data_t *subscription_data)
{
    ogs_assert(subscription_data);

    ogs_list_remove(&ogs_sbi_self()->subscription_data_list, subscription_data);

    if (subscription_data->id)
        ogs_free(subscription_data->id);

    if (subscription_data->notification_uri)
        ogs_free(subscription_data->notification_uri);

    if (subscription_data->req_nf_instance_id)
        ogs_free(subscription_data->req_nf_instance_id);

    if (subscription_data->subscr_cond.service_name)
        ogs_free(subscription_data->subscr_cond.service_name);

    if (subscription_data->t_validity)
        ogs_timer_delete(subscription_data->t_validity);

    if (subscription_data->client)
        ogs_sbi_client_remove(subscription_data->client);

    ogs_pool_free(&subscription_data_pool, subscription_data);
}

void ogs_sbi_subscription_data_remove_all_by_nf_instance_id(
        char *nf_instance_id)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_subscription_data_t *next_subscription_data = NULL;

    ogs_assert(nf_instance_id);

    ogs_list_for_each_safe(&ogs_sbi_self()->subscription_data_list,
            next_subscription_data, subscription_data) {
        if (subscription_data->req_nf_instance_id &&
            strcmp(subscription_data->req_nf_instance_id,
                nf_instance_id) == 0) {
            ogs_sbi_subscription_data_remove(subscription_data);
        }
    }
}

void ogs_sbi_subscription_data_remove_all(void)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_subscription_data_t *next_subscription_data = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->subscription_data_list,
            next_subscription_data, subscription_data)
        ogs_sbi_subscription_data_remove(subscription_data);
}

ogs_sbi_subscription_data_t *ogs_sbi_subscription_data_find(char *id)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;

    ogs_assert(id);

    ogs_list_for_each(&ogs_sbi_self()->subscription_data_list,
            subscription_data) {
        ogs_assert(subscription_data->id);
        if (strcmp(subscription_data->id, id) == 0)
            break;
    }

    return subscription_data;
}

void ogs_sbi_subscription_data_build_default(
        OpenAPI_nf_type_e nf_type, const char *service_name)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL, *nrf_instance = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_type);

    nrf_instance = ogs_sbi_self()->nrf_instance;
    if (!nrf_instance) {
        ogs_warn("[%s:%s] has no NRF",
                OpenAPI_nf_type_ToString(nf_type), service_name);
        return;
    }

    client = nrf_instance->client;
    ogs_assert(client);

    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_assert(nf_instance->id);
    ogs_assert(nf_instance->nf_type);

    subscription_data = ogs_sbi_subscription_data_add();
    ogs_assert(subscription_data);

    OGS_SBI_SETUP_CLIENT(subscription_data, client);
    subscription_data->req_nf_type = nf_instance->nf_type;
    if (nf_instance->id)
        subscription_data->req_nf_instance_id = ogs_strdup(nf_instance->id);
    subscription_data->subscr_cond.nf_type = nf_type;
    if (service_name)
        subscription_data->subscr_cond.service_name = ogs_strdup(service_name);
}
