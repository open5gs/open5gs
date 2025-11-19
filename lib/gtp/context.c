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

#include "ogs-gtp.h"

int __ogs_gtp_domain;
static ogs_gtp_context_t self;
static int context_initialized = 0;

static OGS_POOL(pool, ogs_gtp_node_t);
static OGS_POOL(ogs_gtpu_resource_pool, ogs_gtpu_resource_t);

void ogs_gtp_context_init(int num_of_gtpu_resource)
{
    ogs_assert(context_initialized == 0);

    /* Initialize GTP context */
    memset(&self, 0, sizeof(ogs_gtp_context_t));

    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", ogs_core()->log.level);

    ogs_pool_init(&pool, ogs_app()->pool.gtp_node);
    ogs_pool_init(&ogs_gtpu_resource_pool, num_of_gtpu_resource);

    context_initialized = 1;
}

void ogs_gtp_context_final(void)
{
    ogs_assert(context_initialized == 1);

    ogs_gtpu_resource_remove_all(&self.gtpu_resource_list);
    ogs_pool_final(&ogs_gtpu_resource_pool);

    ogs_gtp_node_remove_all(&self.gtpu_peer_list);
    ogs_pool_final(&pool);

    context_initialized = 0;
}

ogs_gtp_context_t *ogs_gtp_self(void)
{
    return &self;
}

static int ogs_gtp_context_prepare(void)
{
    self.gtpc_port = OGS_GTPV2_C_UDP_PORT;
    self.gtpu_port = OGS_GTPV1_U_UDP_PORT;

    return OGS_OK;
}

static int ogs_gtp_context_validation(const char *local)
{
    return OGS_OK;
}

int ogs_gtp_context_parse_config(const char *local, const char *remote)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;
    int idx = 0;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = ogs_gtp_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if ((!strcmp(root_key, local)) &&
            (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t local_iter;
            ogs_yaml_iter_recurse(&root_iter, &local_iter);
            while (ogs_yaml_iter_next(&local_iter)) {
                const char *local_key = ogs_yaml_iter_key(&local_iter);
                ogs_assert(local_key);
                if (!strcmp(local_key, "gtpc")) {
                    ogs_yaml_iter_t gtpc_iter;
                    ogs_yaml_iter_recurse(&local_iter, &gtpc_iter);
                    while (ogs_yaml_iter_next(&gtpc_iter)) {
                        const char *gtpc_key = ogs_yaml_iter_key(&gtpc_iter);
                        ogs_assert(gtpc_key);
                        if (!strcmp(gtpc_key, "server")) {
                            ogs_yaml_iter_t server_array, server_iter;
                            ogs_yaml_iter_recurse(&gtpc_iter, &server_array);
                            do {
                                int family = AF_UNSPEC;
                                int i, num = 0;
                                const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                                uint16_t port = self.gtpc_port;
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
                                        const char *v =
                                            ogs_yaml_iter_value(&server_iter);
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
                                        const char *v =
                                            ogs_yaml_iter_value(&server_iter);
                                        if (v) port = atoi(v);
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
                                    } else if (!strcmp(server_key, "tac")) {
                                        /* Nothing */
                                    } else if (!strcmp(
                                                server_key, "e_cell_id")) {
                                        /* Nothing */
                                    } else
                                        ogs_warn("unknown key `%s`",
                                                server_key);
                                }

                                /* Add address information */
                                addr = NULL;
                                for (i = 0; i < num; i++) {
                                    rv = ogs_addaddrinfo(&addr,
                                            family, hostname[i], port, 0);
                                    ogs_assert(rv == OGS_OK);
                                }

                                /* Add each address as a separate socknode */
                                if (addr) {
                                    ogs_sockaddr_t *current = addr;
                                    while (current) {
                                        if (current->ogs_sa_family ==
                                                AF_INET &&
                                            ogs_global_conf()->
                                                parameter.no_ipv4 == 0) {
                                            ogs_socknode_add(&self.gtpc_list,
                                                             AF_INET, current,
                                                             is_option ?
                                                             &option : NULL);
                                        }
                                        if (current->ogs_sa_family ==
                                                AF_INET6 &&
                                            ogs_global_conf()->
                                                parameter.no_ipv6 == 0) {
                                            ogs_socknode_add(&self.gtpc_list6,
                                                             AF_INET6, current,
                                                             is_option ?
                                                             &option : NULL);
                                        }
                                        current = current->next;
                                    }
                                    ogs_freeaddrinfo(addr);
                                }

                                if (dev) {
                                    rv = ogs_socknode_probe(
                                            ogs_global_conf()->parameter.
                                            no_ipv4 ?
                                                NULL : &self.gtpc_list,
                                            ogs_global_conf()->parameter.
                                            no_ipv6 ?
                                                NULL : &self.gtpc_list6,
                                            dev, port,
                                            is_option ? &option : NULL);
                                    ogs_assert(rv == OGS_OK);
                                }

                            } while (ogs_yaml_iter_type(&server_array) ==
                                    YAML_SEQUENCE_NODE);

                        } else if (!strcmp(gtpc_key, "client")) {
                            /* handle config in application */
                        } else
                            ogs_warn("unknown key `%s`", gtpc_key);
                    }
                } else if (!strcmp(local_key, "gtpu")) {
                    ogs_list_t list, list6;
                    ogs_socknode_t *node = NULL, *node6 = NULL;
                    ogs_socknode_t *iter = NULL, *next_iter = NULL;

                    ogs_yaml_iter_t gtpu_iter;
                    ogs_yaml_iter_recurse(&local_iter, &gtpu_iter);
                    while (ogs_yaml_iter_next(&gtpu_iter)) {
                        const char *gtpu_key = ogs_yaml_iter_key(&gtpu_iter);
                        ogs_assert(gtpu_key);
                        if (!strcmp(gtpu_key, "server")) {
                            ogs_yaml_iter_t server_array, server_iter;
                            ogs_yaml_iter_recurse(&gtpu_iter, &server_array);
                            do {
                                int family = AF_UNSPEC;
                                int i, num_of_hostname = 0;
                                const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                                int num_of_advertise = 0;
                                const char *advertise[OGS_MAX_NUM_OF_HOSTNAME];
                                uint16_t port = self.gtpu_port;
                                const char *dev = NULL;
                                ogs_sockaddr_t *addr = NULL;
                                ogs_sockaddr_t *adv_addr = NULL;
                                ogs_sockaddr_t *adv_addr6 = NULL;
                                const char *teid_range_indication = NULL;
                                const char *teid_range = NULL;
                                const char *network_instance = NULL;
                                const char *source_interface = NULL;

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

                                    if (ogs_list_count(
                                                &self.gtpu_resource_list) >=
                                        OGS_MAX_NUM_OF_GTPU_RESOURCE) {
                                        ogs_warn(
                                            "[Overflow]: Number of User Plane "
                                            "IP Resource <= %d",
                                            OGS_MAX_NUM_OF_GTPU_RESOURCE);
                                        break;
                                    }

                                    if (!strcmp(server_key, "family")) {
                                        const char *v =
                                            ogs_yaml_iter_value(&server_iter);
                                        if (v) family = atoi(v);
                                        if (family != AF_UNSPEC &&
                                            family != AF_INET &&
                                            family != AF_INET6) {
                                            ogs_warn("Ignore family(%d)"
                                                ": AF_UNSPEC(%d), "
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

                                            ogs_assert(num_of_hostname <
                                                    OGS_MAX_NUM_OF_HOSTNAME);
                                            hostname[num_of_hostname++] =
                                                ogs_yaml_iter_value(
                                                        &hostname_iter);
                                        } while (ogs_yaml_iter_type(
                                                    &hostname_iter) ==
                                                YAML_SEQUENCE_NODE);
                                    } else if (!strcmp(server_key,
                                                "advertise")) {
                                        ogs_yaml_iter_t advertise_iter;
                                        ogs_yaml_iter_recurse(
                                                &server_iter, &advertise_iter);
                                        ogs_assert(ogs_yaml_iter_type(
                                                    &advertise_iter) !=
                                                YAML_MAPPING_NODE);

                                        do {
                                            if (ogs_yaml_iter_type(
                                                        &advertise_iter) ==
                                                    YAML_SEQUENCE_NODE) {
                                                if (!ogs_yaml_iter_next(
                                                            &advertise_iter))
                                                    break;
                                            }

                                            ogs_assert(num_of_advertise <
                                                    OGS_MAX_NUM_OF_HOSTNAME);
                                            advertise[num_of_advertise++] =
                                                ogs_yaml_iter_value(
                                                        &advertise_iter);
                                        } while (ogs_yaml_iter_type(
                                                    &advertise_iter) ==
                                                YAML_SEQUENCE_NODE);
                                    } else if (!strcmp(server_key, "port")) {
                                        const char *v =
                                            ogs_yaml_iter_value(&server_iter);
                                        if (v) port = atoi(v);
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
                                    } else if (!strcmp(server_key,
                                                "teid_range_indication")) {
                                        teid_range_indication =
                                            ogs_yaml_iter_value(&server_iter);
                                    } else if (!strcmp(server_key,
                                                "teid_range")) {
                                        teid_range = ogs_yaml_iter_value(
                                                &server_iter);
                                    } else if (!strcmp(server_key,
                                                "network_instance")) {
                                        network_instance =
                                            ogs_yaml_iter_value(&server_iter);
                                    } else if (!strcmp(server_key,
                                                "source_interface")) {
                                        source_interface =
                                            ogs_yaml_iter_value(&server_iter);
                                    } else
                                        ogs_warn("unknown key `%s`",
                                                server_key);
                                }

                                addr = NULL;
                                for (i = 0; i < num_of_hostname; i++) {
                                    rv = ogs_addaddrinfo(&addr,
                                            family, hostname[i], port, 0);
                                    ogs_assert(rv == OGS_OK);
                                }

                                ogs_list_init(&list);
                                ogs_list_init(&list6);

                                if (addr) {
                                    ogs_sockaddr_t *current = addr;
                                    while (current) {
                                        if (current->ogs_sa_family ==
                                                AF_INET &&
                                            ogs_global_conf()->
                                                parameter.no_ipv4 == 0) {
                                            ogs_socknode_add(&list,
                                                             AF_INET, current,
                                                             is_option ?
                                                             &option : NULL);
                                        }
                                        if (current->ogs_sa_family ==
                                                AF_INET6 &&
                                            ogs_global_conf()->
                                                parameter.no_ipv6 == 0) {
                                            ogs_socknode_add(&list6,
                                                             AF_INET6, current,
                                                             is_option ?
                                                             &option : NULL);
                                        }
                                        current = current->next;
                                    }
                                    ogs_freeaddrinfo(addr);
                                }

                                if (dev) {
                                    rv = ogs_socknode_probe(
                                        ogs_global_conf()->parameter.no_ipv4 ?
                                        NULL : &list,
                                        ogs_global_conf()->parameter.no_ipv6 ?
                                        NULL : &list6,
                                        dev, port,
                                        is_option ? &option : NULL);
                                    ogs_assert(rv == OGS_OK);
                                }

                                adv_addr = NULL;
                                for (i = 0; i < num_of_advertise; i++) {
                                    rv = ogs_addaddrinfo(&adv_addr,
                                            family, advertise[i], port, 0);
                                    ogs_assert(rv == OGS_OK);
                                }
                                rv = ogs_copyaddrinfo(&adv_addr6, adv_addr);
                                ogs_assert(rv == OGS_OK);

                                rv = ogs_filteraddrinfo(&adv_addr, AF_INET);
                                ogs_assert(rv == OGS_OK);
                                rv = ogs_filteraddrinfo(&adv_addr6, AF_INET6);
                                ogs_assert(rv == OGS_OK);

                        /* Find first IPv4/IPv6 address in the list.
                         *
                         * In the following configuration,
                         * 127.0.0.4, 127.0.0.5 and 2001:db8:cafe::1 are ignored
                         * on PFCP Assocation Response message's
                         * user plane IP resource information.
                         *
                         * gtpu:
                         *   - addr:
                         *     - 127.0.0.3
                         *     - ::1
                         *     - 127.0.0.4
                         *     - 127.0.0.5
                         *     - 2001:db8:cafe::1
                         *
                         * To include all user plane IP resource information,
                         * configure as below:
                         *
                         * gtpu:
                         *   - addr:
                         *     - 127.0.0.3
                         *     - ::1
                         *   - addr: 127.0.0.4
                         *   - addr
                         *     - 127.0.0.5
                         *     - 2001:db8:cafe::1
                         */
                                node = ogs_list_first(&list);
                                node6 = ogs_list_first(&list6);
                                if (node || node6) {
                                    ogs_user_plane_ip_resource_info_t info;

                                    memset(&info, 0, sizeof(info));
                                    ogs_sockaddr_to_user_plane_ip_resource_info(
                                            adv_addr ? adv_addr :
                                                node ? node->addr : NULL,
                                            adv_addr6 ? adv_addr6 :
                                                node6 ? node6->addr : NULL,
                                            &info);

                                    if (teid_range_indication) {
                                        info.teidri = atoi(
                                                teid_range_indication);
                                        if (teid_range) {
                                            info.teid_range = atoi(teid_range);
                                        }
                                    }
                                    if (network_instance) {
                                        info.assoni = 1;
                                        ogs_cpystrn(info.network_instance,
                                            network_instance,
                                            OGS_MAX_APN_LEN+1);
                                    }
                                    if (source_interface) {
                                        info.assosi = 1;
                                        info.source_interface =
                                            atoi(source_interface);
                                    }

                                    ogs_gtpu_resource_add(
                                        &self.gtpu_resource_list, &info);
                                }

                                ogs_list_for_each_safe(&list, next_iter, iter)
                                    ogs_list_add(&self.gtpu_list, iter);
                                ogs_list_for_each_safe(&list6, next_iter, iter)
                                    ogs_list_add(&self.gtpu_list, iter);

                                ogs_freeaddrinfo(adv_addr);
                                ogs_freeaddrinfo(adv_addr6);

                            } while (ogs_yaml_iter_type(&server_array) ==
                                    YAML_SEQUENCE_NODE);
                        } else
                            ogs_warn("unknown key `%s`", gtpu_key);
                    }
                }
            }
        }
    }

    rv = ogs_gtp_context_validation(local);
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

ogs_gtp_node_t *ogs_gtp_node_new(ogs_sockaddr_t *sa_list)
{
    ogs_gtp_node_t *node = NULL;

    ogs_assert(sa_list);

    ogs_pool_alloc(&pool, &node);
    if (!node) {
        ogs_error("ogs_pool_alloc() failed");
        return NULL;
    }
    memset(node, 0, sizeof(ogs_gtp_node_t));

    node->sa_list = sa_list;

    ogs_list_init(&node->local_list);
    ogs_list_init(&node->remote_list);

    return node;
}

void ogs_gtp_node_free(ogs_gtp_node_t *node)
{
    ogs_assert(node);

    ogs_gtp_xact_delete_all(node);

    ogs_freeaddrinfo(node->sa_list);
    ogs_pool_free(&pool, node);
}

ogs_gtp_node_t *ogs_gtp_node_add_by_f_teid(
        ogs_list_t *list, ogs_gtp2_f_teid_t *f_teid, uint16_t port)
{
    int rv;
    ogs_gtp_node_t *node = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(list);
    ogs_assert(f_teid);
    ogs_assert(port);

    rv = ogs_gtp2_f_teid_to_sockaddr(f_teid, port, &addr);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp2_f_teid_to_sockaddr() failed");
        return NULL;
    }

    rv = ogs_filter_ip_version(
            &addr,
            ogs_global_conf()->parameter.no_ipv4,
            ogs_global_conf()->parameter.no_ipv6,
            ogs_global_conf()->parameter.prefer_ipv4);
    if (!addr) {
        ogs_error("ogs_filter_ip_version() failed");
        return NULL;
    }

#if 0 /* deprecated */
    rv = ogs_socknode_fill_scope_id_in_local(addr);
    ogs_assert(rv == OGS_OK);
#endif

    node = ogs_gtp_node_new(addr);
    if (!node) {
        ogs_error("ogs_gtp_node_new() failed");
        ogs_freeaddrinfo(addr);
        return NULL;
    }

    rv = ogs_gtp2_f_teid_to_ip(f_teid, &node->ip);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp2_f_teid_to_ip() failed");
        ogs_freeaddrinfo(addr);
        return NULL;
    }

    ogs_list_add(list, node);

    return node;
}

ogs_gtp_node_t *ogs_gtp_node_add_by_addr(ogs_list_t *list, ogs_sockaddr_t *addr)
{
    ogs_gtp_node_t *gnode = NULL;
    ogs_sockaddr_t *new = NULL;

    ogs_assert(list);
    ogs_assert(addr);

    ogs_assert(OGS_OK == ogs_copyaddrinfo(&new, addr));
    gnode = ogs_gtp_node_new(new);
    if (!gnode) {
        ogs_error("ogs_gtp_node_new() failed");
        ogs_freeaddrinfo(new);
        return NULL;
    }

    memcpy(&gnode->addr, new, sizeof gnode->addr);

    ogs_list_add(list, gnode);

    return gnode;
}

void ogs_gtp_node_remove(ogs_list_t *list, ogs_gtp_node_t *node)
{
    ogs_assert(node);

    ogs_list_remove(list, node);

    ogs_gtp_node_free(node);
}

void ogs_gtp_node_remove_all(ogs_list_t *list)
{
    ogs_gtp_node_t *node = NULL, *next_node = NULL;

    ogs_list_for_each_safe(list, next_node, node)
        ogs_gtp_node_remove(list, node);
}

ogs_gtp_node_t *ogs_gtp_node_find_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr)
{
    ogs_gtp_node_t *node = NULL;

    ogs_assert(list);
    ogs_assert(addr);

    ogs_list_for_each(list, node) {
        if (ogs_sockaddr_is_equal(&node->addr, addr) == true)
            break;
    }

    return node;
}

ogs_gtp_node_t *ogs_gtp_node_find_by_f_teid(
        ogs_list_t *list, ogs_gtp2_f_teid_t *f_teid)
{
    int rv;
    ogs_gtp_node_t *node = NULL;
    ogs_ip_t ip;

    ogs_assert(list);
    ogs_assert(f_teid);

    rv = ogs_gtp2_f_teid_to_ip(f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    ogs_list_for_each(list, node) {
        if (memcmp(&node->ip, &ip, sizeof(ip)) == 0)
            break;
    }

    return node;
}

ogs_gtp_node_t *ogs_gtp_node_add_by_ip(
        ogs_list_t *list, ogs_ip_t *ip, uint16_t port)
{
    int rv;
    ogs_gtp_node_t *node = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(list);
    ogs_assert(ip);
    ogs_assert(port);

    rv = ogs_ip_to_sockaddr(ip, port, &addr);
    if (rv != OGS_OK) {
        ogs_error("ogs_ip_to_sockaddr() failed");
        return NULL;
    }

    rv = ogs_filter_ip_version(
            &addr,
            ogs_global_conf()->parameter.no_ipv4,
            ogs_global_conf()->parameter.no_ipv6,
            ogs_global_conf()->parameter.prefer_ipv4);
    if (!addr) {
        ogs_error("ogs_filter_ip_version() failed");
        return NULL;
    }

#if 0 /* deprecated */
    rv = ogs_socknode_fill_scope_id_in_local(addr);
    ogs_assert(rv == OGS_OK);
#endif

    node = ogs_gtp_node_new(addr);
    if (!node) {
        ogs_error("ogs_gtp_node_new() failed");
        ogs_freeaddrinfo(addr);
        return NULL;
    }

    memcpy(&node->ip, ip, sizeof(*ip));

    ogs_list_add(list, node);

    return node;
}

ogs_gtp_node_t *ogs_gtp_node_find_by_ip(ogs_list_t *list, ogs_ip_t *ip)
{
    ogs_gtp_node_t *node = NULL;

    ogs_assert(list);
    ogs_assert(ip);

    ogs_list_for_each(list, node) {
        if (memcmp(&node->ip, ip, sizeof(*ip)) == 0)
            break;
    }

    return node;
}

ogs_gtpu_resource_t *ogs_gtpu_resource_add(ogs_list_t *list,
        ogs_user_plane_ip_resource_info_t *info)
{
    ogs_gtpu_resource_t *resource = NULL;

    ogs_assert(list);
    ogs_assert(info);

    ogs_pool_alloc(&ogs_gtpu_resource_pool, &resource);
    ogs_assert(resource);

    memcpy(&resource->info, info, sizeof(*info));

    ogs_list_add(list, resource);

    return resource;
}

void ogs_gtpu_resource_remove(ogs_list_t *list,
        ogs_gtpu_resource_t *resource)
{
    ogs_assert(list);
    ogs_assert(resource);

    ogs_list_remove(list, resource);

    ogs_pool_free(&ogs_gtpu_resource_pool, resource);
}

void ogs_gtpu_resource_remove_all(ogs_list_t *list)
{
    ogs_gtpu_resource_t *resource = NULL, *next_resource = NULL;

    ogs_assert(list);

    ogs_list_for_each_safe(list, next_resource, resource)
        ogs_gtpu_resource_remove(list, resource);
}
