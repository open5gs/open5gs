/*
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-metrics.h"

#define DEFAULT_PROMETHEUS_HTTP_PORT       9090

int __ogs_metrics_domain;
static ogs_metrics_context_t self;
static int context_initialized = 0;

void ogs_metrics_context_init(void)
{
    ogs_assert(context_initialized == 0);

    ogs_log_install_domain(&__ogs_metrics_domain, "metrics", ogs_core()->log.level);

    /* Initialize METRICS context */
    memset(&self, 0, sizeof(ogs_metrics_context_t));

    ogs_metrics_spec_init(ogs_metrics_self());
    ogs_metrics_server_init(ogs_metrics_self());

    context_initialized = 1;
}

void ogs_metrics_context_open(ogs_metrics_context_t *ctx)
{
    ogs_metrics_server_open(ctx);
}
void ogs_metrics_context_close(ogs_metrics_context_t *ctx)
{
    ogs_metrics_server_close(ctx);
}

void ogs_metrics_context_final(void)
{
    ogs_assert(context_initialized == 1);

    ogs_metrics_spec_final(ogs_metrics_self());
    ogs_metrics_server_final(ogs_metrics_self());

    context_initialized = 0;
}

ogs_metrics_context_t *ogs_metrics_self(void)
{
    return &self;
}

static int ogs_metrics_context_prepare(void)
{
    self.metrics_port = DEFAULT_PROMETHEUS_HTTP_PORT;

    return OGS_OK;
}

int ogs_metrics_context_parse_config(const char *local)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;
    int idx = 0;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = ogs_metrics_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (local && !strcmp(root_key, local) &&
            (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t local_iter;
            ogs_yaml_iter_recurse(&root_iter, &local_iter);
            while (ogs_yaml_iter_next(&local_iter)) {
                const char *local_key = ogs_yaml_iter_key(&local_iter);
                ogs_assert(local_key);
                if (!strcmp(local_key, "metrics")) {
                    ogs_yaml_iter_t metrics_iter;
                    ogs_yaml_iter_recurse(&local_iter, &metrics_iter);
                    while (ogs_yaml_iter_next(&metrics_iter)) {
                        const char *metrics_key =
                            ogs_yaml_iter_key(&metrics_iter);
                        ogs_assert(metrics_key);
                        if (!strcmp(metrics_key, "server")) {
                            ogs_list_t list, list6;
                            ogs_socknode_t *node = NULL, *node6 = NULL;

                            ogs_yaml_iter_t server_iter, server_array;
                            ogs_yaml_iter_recurse(&metrics_iter, &server_array);
                            do {
                                int i, family = AF_UNSPEC;
                                int num = 0;
                                const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];

                                uint16_t port = self.metrics_port;
                                const char *dev = NULL;
                                ogs_sockaddr_t *addr = NULL;

                                ogs_sockopt_t option;
                                bool is_option = false;

                                if (ogs_yaml_iter_type(&server_array) ==
                                        YAML_MAPPING_NODE) {
                                    memcpy(&server_iter, &server_array,
                                            sizeof(ogs_yaml_iter_t));
                                } else if (ogs_yaml_iter_type(&server_array) ==
                                    YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(&server_array))
                                        break;
                                    ogs_yaml_iter_recurse(
                                            &server_array, &server_iter);
                                } else if (ogs_yaml_iter_type(&server_array) ==
                                    YAML_SCALAR_NODE) {
                                    break;
                                } else
                                    ogs_assert_if_reached();

                                while (ogs_yaml_iter_next(&server_iter)) {
                                    const char *server_key =
                                        ogs_yaml_iter_key(&server_iter);
                                    ogs_assert(server_key);
                                    if (!strcmp(server_key, "family")) {
                                        const char *v = ogs_yaml_iter_value(
                                                &server_iter);
                                        if (v) family = atoi(v);
                                        if (family != AF_UNSPEC &&
                                            family != AF_INET &&
                                            family != AF_INET6) {
                                            ogs_warn("Ignore family(%d) : "
                                                "AF_UNSPEC(%d), "
                                                "AF_INET(%d), AF_INET6(%d) ",
                                                family,
                                                AF_UNSPEC, AF_INET, AF_INET6);
                                            family = AF_UNSPEC;
                                        }
                                    } else if (!strcmp(server_key, "address")) {
                                        ogs_yaml_iter_t hostname_iter;
                                        ogs_yaml_iter_recurse(&server_iter,
                                                &hostname_iter);
                                        ogs_assert(ogs_yaml_iter_type(
                                                    &hostname_iter) !=
                                                YAML_MAPPING_NODE);

                                        do {
                                            if (ogs_yaml_iter_type(
                                                        &hostname_iter) ==
                                                    YAML_SEQUENCE_NODE) {
                                                if (!ogs_yaml_iter_next(
                                                            &hostname_iter))
                                                    break;
                                            }

                                            ogs_assert(num <
                                                    OGS_MAX_NUM_OF_HOSTNAME);
                                            hostname[num++] =
                                                ogs_yaml_iter_value(
                                                        &hostname_iter);
                                        } while (ogs_yaml_iter_type(
                                                    &hostname_iter) ==
                                                YAML_SEQUENCE_NODE);
                                    } else if (!strcmp(server_key, "port")) {
                                        const char *v = ogs_yaml_iter_value(
                                                &server_iter);
                                        if (v)
                                            port = atoi(v);
                                    } else if (!strcmp(server_key, "dev")) {
                                        dev = ogs_yaml_iter_value(&server_iter);
                                    } else if (!strcmp(server_key, "option")) {
                                        rv = ogs_app_parse_sockopt_config(
                                                &server_iter, &option);
                                        if (rv != OGS_OK) {
                                            ogs_error("ogs_app_parse_sockopt_"
                                                    "config() failed");
                                            return rv;
                                        }
                                        is_option = true;
                                    } else
                                        ogs_warn("unknown key `%s`",
                                                server_key);
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
                                    if (ogs_global_conf()->parameter.no_ipv4 == 0)
                                        ogs_socknode_add(
                                            &list, AF_INET, addr, NULL);
                                    if (ogs_global_conf()->parameter.no_ipv6 == 0)
                                        ogs_socknode_add(
                                            &list6, AF_INET6, addr, NULL);
                                    ogs_freeaddrinfo(addr);
                                }

                                if (dev) {
                                    rv = ogs_socknode_probe(
                                        ogs_global_conf()->parameter.no_ipv4 ?
                                        NULL : &list,
                                        ogs_global_conf()->parameter.no_ipv6 ?
                                        NULL : &list6,
                                        dev, port, NULL);
                                    ogs_assert(rv == OGS_OK);
                                }

                                node = ogs_list_first(&list);
                                if (node) {
                                    ogs_metrics_server_t *server =
                                        ogs_metrics_server_add(
                                            node->addr,
                                            is_option ? &option : NULL);
                                    ogs_assert(server);
                                }
                                node6 = ogs_list_first(&list6);
                                if (node6) {
                                    ogs_metrics_server_t *server =
                                        ogs_metrics_server_add(
                                            node6->addr,
                                            is_option ? &option : NULL);
                                    ogs_assert(server);
                                }

                                ogs_socknode_remove_all(&list);
                                ogs_socknode_remove_all(&list6);

                            } while (ogs_yaml_iter_type(&server_array) ==
                                    YAML_SEQUENCE_NODE);
                        } else
                            ogs_warn("unknown key `%s`", metrics_key);
                    }
                }
            }
        }
    }

    return OGS_OK;
}
