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

#include "app/ogs-app.h"
#include "ogs-pfcp.h"

static ogs_pfcp_context_t self;

static OGS_POOL(ogs_pfcp_node_pool, ogs_pfcp_node_t);
static OGS_POOL(ogs_pfcp_gtpu_resource_pool, ogs_pfcp_gtpu_resource_t);

static OGS_POOL(ogs_pfcp_sess_pool, ogs_pfcp_sess_t);
static OGS_POOL(ogs_pfcp_pdr_pool, ogs_pfcp_pdr_t);
static OGS_POOL(ogs_pfcp_far_pool, ogs_pfcp_far_t);
static OGS_POOL(ogs_pfcp_urr_pool, ogs_pfcp_urr_t);
static OGS_POOL(ogs_pfcp_qer_pool, ogs_pfcp_qer_t);
static OGS_POOL(ogs_pfcp_bar_pool, ogs_pfcp_bar_t);

static OGS_POOL(ogs_pfcp_dev_pool, ogs_pfcp_dev_t);
static OGS_POOL(ogs_pfcp_subnet_pool, ogs_pfcp_subnet_t);

static int context_initiaized = 0;

void ogs_pfcp_context_init(int num_of_gtpu_resource)
{
    struct timeval tv;
    ogs_assert(context_initiaized == 0);

    /* Initialize SMF context */
    memset(&self, 0, sizeof(ogs_pfcp_context_t));

    /*
     * PFCP entity uses NTP timestamp(1900), but Open5GS uses UNIX(1970).
     *
     * One is the offset between the two epochs. 
     * Unix uses an epoch located at 1/1/1970-00:00h (UTC) and
     * NTP uses 1/1/1900-00:00h. This leads to an offset equivalent 
     * to 70 years in seconds (there are 17 leap years
     * between the two dates so the offset is
     *
     *  (70*365 + 17)*86400 = 2208988800
     *
     * to be substracted from NTP time to get Unix struct timeval.
     */
    ogs_gettimeofday(&tv);
    self.pfcp_started = tv.tv_sec + 2208988800;

    ogs_log_install_domain(&__ogs_pfcp_domain, "pfcp", ogs_core()->log.level);

    ogs_pool_init(&ogs_pfcp_node_pool, ogs_config()->pool.pfcp);
    ogs_pool_init(&ogs_pfcp_gtpu_resource_pool, num_of_gtpu_resource);

    ogs_list_init(&self.n4_list);

    ogs_pool_init(&ogs_pfcp_sess_pool, ogs_config()->pool.sess);

    ogs_pool_init(&ogs_pfcp_pdr_pool,
            ogs_config()->pool.sess * OGS_MAX_NUM_OF_PDR);
    ogs_pool_init(&ogs_pfcp_far_pool,
            ogs_config()->pool.sess * OGS_MAX_NUM_OF_FAR);
    ogs_pool_init(&ogs_pfcp_urr_pool,
            ogs_config()->pool.sess * OGS_MAX_NUM_OF_URR);
    ogs_pool_init(&ogs_pfcp_qer_pool,
            ogs_config()->pool.sess * OGS_MAX_NUM_OF_QER);
    ogs_pool_init(&ogs_pfcp_bar_pool,
            ogs_config()->pool.sess * OGS_MAX_NUM_OF_BAR);

    ogs_list_init(&self.dev_list);
    ogs_pool_init(&ogs_pfcp_dev_pool, OGS_MAX_NUM_OF_DEV);
    ogs_list_init(&self.subnet_list);
    ogs_pool_init(&ogs_pfcp_subnet_pool, OGS_MAX_NUM_OF_SUBNET);

    self.pdr_hash = ogs_hash_make();

    context_initiaized = 1;
}

void ogs_pfcp_context_final(void)
{
    ogs_assert(context_initiaized == 1);

    ogs_assert(self.pdr_hash);
    ogs_hash_destroy(self.pdr_hash);

    ogs_pfcp_dev_remove_all();
    ogs_pfcp_subnet_remove_all();

    ogs_pool_final(&ogs_pfcp_dev_pool);
    ogs_pool_final(&ogs_pfcp_subnet_pool);

    ogs_pool_final(&ogs_pfcp_sess_pool);
    ogs_pool_final(&ogs_pfcp_pdr_pool);
    ogs_pool_final(&ogs_pfcp_far_pool);
    ogs_pool_final(&ogs_pfcp_urr_pool);
    ogs_pool_final(&ogs_pfcp_qer_pool);
    ogs_pool_final(&ogs_pfcp_bar_pool);

    ogs_pfcp_node_remove_all(&ogs_pfcp_self()->n4_list);

    ogs_pool_final(&ogs_pfcp_node_pool);
    ogs_pool_final(&ogs_pfcp_gtpu_resource_pool);

    context_initiaized = 0;
}

ogs_pfcp_context_t *ogs_pfcp_self(void)
{
    return &self;
}

static int ogs_pfcp_context_prepare(void)
{
    self.pfcp_port = OGS_PFCP_UDP_PORT;
    self.tun_ifname = "ogstun";

    return OGS_OK;
}

static int ogs_pfcp_context_validation(const char *local)
{
    if (ogs_list_first(&self.pfcp_list) == NULL &&
        ogs_list_first(&self.pfcp_list6) == NULL) {
        ogs_error("No %s.pfcp: in '%s'", local, ogs_config()->file);
        return OGS_ERROR;
    }
    if (ogs_list_first(&self.subnet_list) == NULL) {
        ogs_error("No %s.pdn: in '%s'", local, ogs_config()->file);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_pfcp_context_parse_config(const char *local, const char *remote)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_config()->document;
    ogs_assert(document);

    rv = ogs_pfcp_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, local)) {
            ogs_yaml_iter_t local_iter;
            ogs_yaml_iter_recurse(&root_iter, &local_iter);
            while (ogs_yaml_iter_next(&local_iter)) {
                const char *local_key = ogs_yaml_iter_key(&local_iter);
                ogs_assert(local_key);
                if (!strcmp(local_key, "pfcp")) {
                    ogs_yaml_iter_t pfcp_array, pfcp_iter;
                    ogs_yaml_iter_recurse(&local_iter, &pfcp_array);
                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.pfcp_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;

                        if (ogs_yaml_iter_type(&pfcp_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&pfcp_iter, &pfcp_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&pfcp_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&pfcp_array))
                                break;
                            ogs_yaml_iter_recurse(&pfcp_array, &pfcp_iter);
                        } else if (ogs_yaml_iter_type(&pfcp_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&pfcp_iter)) {
                            const char *pfcp_key =
                                ogs_yaml_iter_key(&pfcp_iter);
                            ogs_assert(pfcp_key);
                            if (!strcmp(pfcp_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&pfcp_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(pfcp_key, "addr") ||
                                    !strcmp(pfcp_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&pfcp_iter,
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

                                    ogs_assert(num <= OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] = 
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(pfcp_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&pfcp_iter);
                                if (v) {
                                    port = atoi(v);
                                    self.pfcp_port = port;
                                }
                            } else if (!strcmp(pfcp_key, "dev")) {
                                dev = ogs_yaml_iter_value(&pfcp_iter);
                            } else if (!strcmp(pfcp_key, "apn")) {
                                /* Skip */
                            } else
                                ogs_warn("unknown key `%s`", pfcp_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        if (addr) {
                            if (ogs_config()->parameter.no_ipv4 == 0)
                                ogs_socknode_add(
                                        &self.pfcp_list, AF_INET, addr);
                            if (ogs_config()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(
                                        &self.pfcp_list6, AF_INET6, addr);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                    ogs_config()->parameter.no_ipv4 ?
                                        NULL : &self.pfcp_list,
                                    ogs_config()->parameter.no_ipv6 ?
                                        NULL : &self.pfcp_list6,
                                    dev, self.pfcp_port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while (ogs_yaml_iter_type(&pfcp_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.pfcp_list) == NULL &&
                        ogs_list_first(&self.pfcp_list6) == NULL) {
                        rv = ogs_socknode_probe(
                                ogs_config()->parameter.no_ipv4 ?
                                    NULL : &self.pfcp_list,
                                ogs_config()->parameter.no_ipv6 ?
                                    NULL : &self.pfcp_list6,
                                NULL, self.pfcp_port);
                        ogs_assert(rv == OGS_OK);
                    }
                } else if (!strcmp(local_key, "pdn")) {
                    ogs_yaml_iter_t pdn_array, pdn_iter;
                    ogs_yaml_iter_recurse(&local_iter, &pdn_array);

                    do {
                        ogs_pfcp_subnet_t *subnet = NULL;
                        const char *ipstr = NULL;
                        const char *mask_or_numbits = NULL;
                        const char *apn = NULL;
                        const char *dev = self.tun_ifname;
                        const char *low[MAX_NUM_OF_SUBNET_RANGE];
                        const char *high[MAX_NUM_OF_SUBNET_RANGE];
                        int i, num = 0;

                        if (ogs_yaml_iter_type(&pdn_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&pdn_iter, &pdn_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&pdn_array) ==
                                YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&pdn_array))
                                break;
                            ogs_yaml_iter_recurse(&pdn_array, &pdn_iter);
                        } else if (ogs_yaml_iter_type(&pdn_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&pdn_iter)) {
                            const char *pdn_key = ogs_yaml_iter_key(&pdn_iter);
                            ogs_assert(pdn_key);
                            if (!strcmp(pdn_key, "addr")) {
                                char *v =
                                    (char *)ogs_yaml_iter_value(&pdn_iter);
                                if (v) {
                                    ipstr = (const char *)strsep(&v, "/");
                                    if (ipstr) {
                                        mask_or_numbits = (const char *)v;
                                    }
                                }
                            } else if (!strcmp(pdn_key, "apn")) {
                                apn = ogs_yaml_iter_value(&pdn_iter);
                            } else if (!strcmp(pdn_key, "dev")) {
                                dev = ogs_yaml_iter_value(&pdn_iter);
                            } else if (!strcmp(pdn_key, "range")) {
                                ogs_yaml_iter_t range_iter;
                                ogs_yaml_iter_recurse(&pdn_iter, &range_iter);
                                ogs_assert(ogs_yaml_iter_type(&range_iter) !=
                                    YAML_MAPPING_NODE);
                                do {
                                    char *v = NULL;

                                    if (ogs_yaml_iter_type(&range_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&range_iter))
                                            break;
                                    }

                                    v = (char *)
                                        ogs_yaml_iter_value(&range_iter);
                                    if (v) {
                                        ogs_assert(num <=
                                                MAX_NUM_OF_SUBNET_RANGE);
                                        low[num] =
                                            (const char *)strsep(&v, "-");
                                        if (low[num] && strlen(low[num]) == 0)
                                            low[num] = NULL;

                                        high[num] = (const char *)v;
                                        if (high[num] && strlen(high[num]) == 0)
                                            high[num] = NULL;
                                    }

                                    if (low[num] || high[num]) num++;
                                } while (
                                    ogs_yaml_iter_type(&range_iter) ==
                                    YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", pdn_key);
                        }

                        subnet = ogs_pfcp_subnet_add(
                                ipstr, mask_or_numbits, apn, dev);
                        ogs_assert(subnet);

                        subnet->num_of_range = num;
                        for (i = 0; i < subnet->num_of_range; i++) {
                            subnet->range[i].low = low[i];
                            subnet->range[i].high = high[i];
                        }

                    } while (ogs_yaml_iter_type(&pdn_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.subnet_list) == NULL) {
                        /* The followings are default configuration
                         * if no PDN configration */
                        ogs_pfcp_subnet_t *subnet = NULL;

#if defined(__linux)
                        /*
                         * On Linux, we can use a persitent tun/tap interface
                         * which has already been setup. As such,
                         * we do not need to get the IP address
                         * from configuration.
                         *
                         * If there is no APN and TUN mapping,
                         * the default subnet is added with `ogstun` name
                         */
                        subnet = ogs_pfcp_subnet_add(
                                NULL, NULL, NULL, self.tun_ifname);
                        ogs_assert(subnet);
#else
                        /*
                         * On MacOSX/FreeBSD, There is no persitent tun/tap
                         * interface, TUN IP address is required.
                         * The default configuration is same as below.
                         *
                         *   pdn:
                         *     - addr: 10.45.0.1/16
                         *     - addr: cafe::1/64
                         */
                        subnet = ogs_pfcp_subnet_add(
                                "10.45.0.1", "16", NULL, self.tun_ifname);
                        ogs_assert(subnet);
                        subnet = ogs_pfcp_subnet_add(
                                "cafe::1", "64", NULL, self.tun_ifname);
                        ogs_assert(subnet);
#endif
                    }
                }
            }
        } else if (!strcmp(root_key, remote)) {
            ogs_yaml_iter_t remote_iter;
            ogs_yaml_iter_recurse(&root_iter, &remote_iter);
            while (ogs_yaml_iter_next(&remote_iter)) {
                const char *remote_key = ogs_yaml_iter_key(&remote_iter);
                ogs_assert(remote_key);
                if (!strcmp(remote_key, "pfcp")) {
                    ogs_yaml_iter_t pfcp_array, pfcp_iter;
                    ogs_yaml_iter_recurse(&remote_iter, &pfcp_array);
                    do {
                        ogs_pfcp_node_t *node = NULL;
                        ogs_sockaddr_t *addr = NULL;
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.pfcp_port;
                        uint16_t tac[OGS_MAX_NUM_OF_TAI] = {0,};
                        uint8_t num_of_tac = 0;

                        if (ogs_yaml_iter_type(&pfcp_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&pfcp_iter, &pfcp_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&pfcp_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&pfcp_array))
                                break;
                            ogs_yaml_iter_recurse(&pfcp_array, &pfcp_iter);
                        } else if (ogs_yaml_iter_type(&pfcp_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&pfcp_iter)) {
                            const char *pfcp_key =
                                ogs_yaml_iter_key(&pfcp_iter);
                            ogs_assert(pfcp_key);
                            if (!strcmp(pfcp_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&pfcp_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(pfcp_key, "addr") ||
                                    !strcmp(pfcp_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&pfcp_iter,
                                        &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num <= OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] = 
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(pfcp_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&pfcp_iter);
                                if (v) port = atoi(v);
                            } else if (!strcmp(pfcp_key, "tac")) {
                                ogs_yaml_iter_t tac_iter;
                                ogs_yaml_iter_recurse(&pfcp_iter, &tac_iter);
                                ogs_assert(ogs_yaml_iter_type(&tac_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    const char *v = NULL;

                                    ogs_assert(num_of_tac <=
                                            OGS_MAX_NUM_OF_TAI);
                                    if (ogs_yaml_iter_type(&tac_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&tac_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&tac_iter);
                                    if (v) {
                                        tac[num_of_tac] = atoi(v);
                                        num_of_tac++;
                                    }
                                } while (
                                    ogs_yaml_iter_type(&tac_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", pfcp_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        ogs_filter_ip_version(&addr,
                                ogs_config()->parameter.no_ipv4,
                                ogs_config()->parameter.no_ipv6,
                                ogs_config()->parameter.prefer_ipv4);

                        node = ogs_pfcp_node_new(addr);
                        ogs_assert(node);
                        ogs_list_add(&self.n4_list, node);

                        node->num_of_tac = num_of_tac;
                        if (num_of_tac != 0)
                            memcpy(node->tac, tac, sizeof(node->tac));

                    } while (ogs_yaml_iter_type(&pfcp_array) ==
                            YAML_SEQUENCE_NODE);
                }
            }
        }
    }

    rv = ogs_pfcp_context_validation(local);
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

ogs_pfcp_node_t *ogs_pfcp_node_new(ogs_sockaddr_t *sa_list)
{
    ogs_pfcp_node_t *node = NULL;

    ogs_assert(sa_list);

    ogs_pool_alloc(&ogs_pfcp_node_pool, &node);
    ogs_assert(node);
    memset(node, 0, sizeof(ogs_pfcp_node_t));

    node->sa_list = sa_list;

    ogs_list_init(&node->local_list);
    ogs_list_init(&node->remote_list);

    ogs_list_init(&node->gtpu_resource_list);

    return node;
}

void ogs_pfcp_node_free(ogs_pfcp_node_t *node)
{
    ogs_assert(node);

    ogs_pfcp_gtpu_resource_remove_all(&node->gtpu_resource_list);

    if (node->sock)
        ogs_sock_destroy(node->sock);

    ogs_pfcp_xact_delete_all(node);

    ogs_freeaddrinfo(node->sa_list);
    ogs_pool_free(&ogs_pfcp_node_pool, node);
}

ogs_pfcp_node_t *ogs_pfcp_node_add(
        ogs_list_t *list, ogs_sockaddr_t *addr)
{
    ogs_pfcp_node_t *node = NULL;
    ogs_sockaddr_t *new = NULL;

    ogs_assert(list);
    ogs_assert(addr);

    ogs_copyaddrinfo(&new, addr);
    node = ogs_pfcp_node_new(new);

    ogs_assert(node);
    memcpy(&node->addr, new, sizeof node->addr);

    ogs_list_add(list, node);

    return node;
}

ogs_pfcp_node_t *ogs_pfcp_node_find(
        ogs_list_t *list, ogs_sockaddr_t *addr)
{
    ogs_pfcp_node_t *node = NULL;

    ogs_assert(list);
    ogs_assert(addr);

    ogs_list_for_each(list, node) {
        if (ogs_sockaddr_is_equal(&node->addr, addr) == true)
            break;
    }

    return node;
}

void ogs_pfcp_node_remove(ogs_list_t *list, ogs_pfcp_node_t *node)
{
    ogs_assert(list);
    ogs_assert(node);

    ogs_list_remove(list, node);
    ogs_pfcp_node_free(node);
}

void ogs_pfcp_node_remove_all(ogs_list_t *list)
{
    ogs_pfcp_node_t *node = NULL, *next_node = NULL;

    ogs_assert(list);
    
    ogs_list_for_each_safe(list, next_node, node)
        ogs_pfcp_node_remove(list, node);
}

ogs_pfcp_gtpu_resource_t *ogs_pfcp_gtpu_resource_add(ogs_list_t *list,
        ogs_pfcp_user_plane_ip_resource_info_t *info)
{
    ogs_pfcp_gtpu_resource_t *resource = NULL;

    ogs_assert(list);
    ogs_assert(info);

    ogs_pool_alloc(&ogs_pfcp_gtpu_resource_pool, &resource);
    ogs_assert(resource);

    memcpy(&resource->info, info, sizeof(*info));

    ogs_list_add(list, resource);

    return resource;
}

ogs_pfcp_gtpu_resource_t *ogs_pfcp_gtpu_resource_find(ogs_list_t *list,
        char *apn, ogs_pfcp_interface_t source_interface)
{
    ogs_pfcp_gtpu_resource_t *resource = NULL;

    ogs_assert(list);

    ogs_list_for_each(list, resource) {
        bool match = true;

        if (resource->info.assoni &&
            strlen(resource->info.network_instance) &&
            apn && strlen(apn) &&
            strcmp(apn, resource->info.network_instance) != 0) {
            match = false;
        }

        if (resource->info.assosi &&
            resource->info.source_interface <= OGS_PFCP_INTERFACE_LI_FUNCTION &&
            source_interface <= OGS_PFCP_INTERFACE_LI_FUNCTION &&
            source_interface != resource->info.source_interface) {
            match = false;
        }

        if (match == true) return resource;
    }

    return NULL;
}

void ogs_pfcp_gtpu_resource_remove(ogs_list_t *list,
        ogs_pfcp_gtpu_resource_t *resource)
{
    ogs_assert(list);
    ogs_assert(resource);

    ogs_list_remove(list, resource);

    ogs_pool_free(&ogs_pfcp_gtpu_resource_pool, resource);
}

void ogs_pfcp_gtpu_resource_remove_all(ogs_list_t *list)
{
    ogs_pfcp_gtpu_resource_t *resource = NULL, *next_resource = NULL;

    ogs_assert(list);

    ogs_list_for_each_safe(list, next_resource, resource)
        ogs_pfcp_gtpu_resource_remove(list, resource);
}

ogs_pfcp_pdr_t *ogs_pfcp_sess_default_pdr(ogs_pfcp_sess_t *sess)
{
    ogs_assert(sess);
    return sess->default_pdr;
}

void ogs_pfcp_sess_clear(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_pdr_remove_all(sess);
    ogs_pfcp_far_remove_all(sess);
    ogs_pfcp_urr_remove_all(sess);
    ogs_pfcp_qer_remove_all(sess);
    if (sess->bar) ogs_pfcp_bar_delete(sess->bar);
}

static int precedence_compare(ogs_pfcp_pdr_t *pdr1, ogs_pfcp_pdr_t *pdr2)
{
    if (pdr1->precedence == pdr2->precedence)
        return 0;
    else if (pdr1->precedence < pdr2->precedence)
        return -1;
    else
        return 1;
}

ogs_pfcp_pdr_t *ogs_pfcp_pdr_add(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_pdr_t *pdr = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&ogs_pfcp_pdr_pool, &pdr);
    ogs_assert(pdr);
    memset(pdr, 0, sizeof *pdr);

    pdr->sess = sess;
    ogs_list_add(&sess->pdr_list, pdr);

    return pdr;
}

ogs_pfcp_pdr_t *ogs_pfcp_pdr_find(
        ogs_pfcp_sess_t *sess, ogs_pfcp_pdr_id_t id)
{
    ogs_pfcp_pdr_t *pdr = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->pdr_list, pdr)
        if (pdr->id == id) return pdr;

    return NULL;
}

ogs_pfcp_pdr_t *ogs_pfcp_pdr_find_by_teid(uint32_t teid)
{
    return (ogs_pfcp_pdr_t *)ogs_hash_get(self.pdr_hash, &teid, sizeof(teid));
}

ogs_pfcp_pdr_t *ogs_pfcp_pdr_find_or_add(
        ogs_pfcp_sess_t *sess, ogs_pfcp_pdr_id_t id)
{
    ogs_pfcp_pdr_t *pdr = NULL;

    ogs_assert(sess);

    pdr = ogs_pfcp_pdr_find(sess, id);
    if (!pdr) {
        pdr = ogs_pfcp_pdr_add(sess);
        ogs_assert(pdr);
        pdr->id = id;
    }

    return pdr;
}

void ogs_pfcp_pdr_reorder_by_precedence(
        ogs_pfcp_pdr_t *pdr, ogs_pfcp_precedence_t precedence)
{
    ogs_pfcp_sess_t *sess = NULL;

    ogs_assert(pdr);
    sess = pdr->sess;
    ogs_assert(sess);

    ogs_list_remove(&sess->pdr_list, pdr);

    pdr->precedence = precedence;
    ogs_list_insert_sorted(&sess->pdr_list, pdr, precedence_compare);
}

void ogs_pfcp_pdr_associate_far(ogs_pfcp_pdr_t *pdr, ogs_pfcp_far_t *far)
{
    ogs_assert(pdr);
    ogs_assert(far);
    
    pdr->far = far;
}
void ogs_pfcp_pdr_associate_urr(ogs_pfcp_pdr_t *pdr, ogs_pfcp_urr_t *urr)
{
    ogs_assert(pdr);
    ogs_assert(urr);

    pdr->urr = urr;
}
void ogs_pfcp_pdr_associate_qer(ogs_pfcp_pdr_t *pdr, ogs_pfcp_qer_t *qer)
{
    ogs_assert(pdr);
    ogs_assert(qer);

    pdr->qer = qer;
}

void ogs_pfcp_pdr_remove(ogs_pfcp_pdr_t *pdr)
{
    ogs_assert(pdr);
    ogs_assert(pdr->sess);

    ogs_list_remove(&pdr->sess->pdr_list, pdr);

    if (pdr->f_teid.teid)
        ogs_hash_set(self.pdr_hash, &pdr->f_teid.teid,
                sizeof(pdr->f_teid.teid), NULL);

    ogs_pool_free(&ogs_pfcp_pdr_pool, pdr);
}

void ogs_pfcp_pdr_remove_all(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_pdr_t *pdr = NULL, *next_pdr = NULL;

    ogs_assert(sess);
    ogs_list_for_each_safe(&sess->pdr_list, next_pdr, pdr)
        ogs_pfcp_pdr_remove(pdr);
}

ogs_pfcp_far_t *ogs_pfcp_far_add(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_far_t *far = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&ogs_pfcp_far_pool, &far);
    ogs_assert(far);
    memset(far, 0, sizeof *far);

    far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

    far->sess = sess;
    ogs_list_add(&sess->far_list, far);

    return far;
}

ogs_pfcp_far_t *ogs_pfcp_far_find(
        ogs_pfcp_sess_t *sess, ogs_pfcp_far_id_t id)
{
    ogs_pfcp_far_t *far = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->far_list, far)
        if (far->id == id) return far;

    return NULL;
}

ogs_pfcp_far_t *ogs_pfcp_far_find_or_add(
        ogs_pfcp_sess_t *sess, ogs_pfcp_far_id_t id)
{
    ogs_pfcp_far_t *far = NULL;

    ogs_assert(sess);

    far = ogs_pfcp_far_find(sess, id);
    if (!far) {
        far = ogs_pfcp_far_add(sess);
        ogs_assert(far);
        far->id = id;
    }

    return far;
}

void ogs_pfcp_far_remove(ogs_pfcp_far_t *far)
{
    ogs_pfcp_sess_t *sess = NULL;

    ogs_assert(far);
    sess = far->sess;
    ogs_assert(sess);

    ogs_list_remove(&sess->far_list, far);
    ogs_pool_free(&ogs_pfcp_far_pool, far);
}

void ogs_pfcp_far_remove_all(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_far_t *far = NULL, *next_far = NULL;

    ogs_assert(sess);

    ogs_list_for_each_safe(&sess->far_list, next_far, far)
        ogs_pfcp_far_remove(far);
}

ogs_pfcp_urr_t *ogs_pfcp_urr_add(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_urr_t *urr = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&ogs_pfcp_urr_pool, &urr);
    ogs_assert(urr);
    memset(urr, 0, sizeof *urr);

    urr->sess = sess;
    ogs_list_add(&sess->urr_list, urr);

    return urr;
}

ogs_pfcp_urr_t *ogs_pfcp_urr_find(
        ogs_pfcp_sess_t *sess, ogs_pfcp_urr_id_t id)
{
    ogs_pfcp_urr_t *urr = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->urr_list, urr)
        if (urr->id == id) return urr;

    return NULL;
}

ogs_pfcp_urr_t *ogs_pfcp_urr_find_or_add(
        ogs_pfcp_sess_t *sess, ogs_pfcp_urr_id_t id)
{
    ogs_pfcp_urr_t *urr = NULL;

    ogs_assert(sess);

    urr = ogs_pfcp_urr_find(sess, id);
    if (!urr) {
        urr = ogs_pfcp_urr_add(sess);
        ogs_assert(urr);
        urr->id = id;
    }

    return urr;
}

void ogs_pfcp_urr_remove(ogs_pfcp_urr_t *urr)
{
    ogs_pfcp_sess_t *sess = NULL;

    ogs_assert(urr);
    sess = urr->sess;
    ogs_assert(sess);

    ogs_list_remove(&sess->urr_list, urr);
    ogs_pool_free(&ogs_pfcp_urr_pool, urr);
}

void ogs_pfcp_urr_remove_all(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_urr_t *urr = NULL, *next_urr = NULL;

    ogs_assert(sess);

    ogs_list_for_each_safe(&sess->urr_list, next_urr, urr)
        ogs_pfcp_urr_remove(urr);
}

ogs_pfcp_qer_t *ogs_pfcp_qer_add(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&ogs_pfcp_qer_pool, &qer);
    ogs_assert(qer);
    memset(qer, 0, sizeof *qer);

    qer->sess = sess;
    ogs_list_add(&sess->qer_list, qer);

    return qer;
}

ogs_pfcp_qer_t *ogs_pfcp_qer_find(
        ogs_pfcp_sess_t *sess, ogs_pfcp_qer_id_t id)
{
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->qer_list, qer)
        if (qer->id == id) return qer;

    return NULL;
}

ogs_pfcp_qer_t *ogs_pfcp_qer_find_or_add(
        ogs_pfcp_sess_t *sess, ogs_pfcp_qer_id_t id)
{
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(sess);

    qer = ogs_pfcp_qer_find(sess, id);
    if (!qer) {
        qer = ogs_pfcp_qer_add(sess);
        ogs_assert(qer);
        qer->id = id;
    }

    return qer;
}

void ogs_pfcp_qer_remove(ogs_pfcp_qer_t *qer)
{
    ogs_pfcp_sess_t *sess = NULL;

    ogs_assert(qer);
    sess = qer->sess;
    ogs_assert(sess);

    ogs_list_remove(&sess->qer_list, qer);
    ogs_pool_free(&ogs_pfcp_qer_pool, qer);
}

void ogs_pfcp_qer_remove_all(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_qer_t *qer = NULL, *next_qer = NULL;

    ogs_assert(sess);

    ogs_list_for_each_safe(&sess->qer_list, next_qer, qer)
        ogs_pfcp_qer_remove(qer);
}

ogs_pfcp_bar_t *ogs_pfcp_bar_new(ogs_pfcp_sess_t *sess)
{
    ogs_pfcp_bar_t *bar = NULL;

    ogs_assert(sess);
    ogs_assert(sess->bar == NULL); /* Only One BAR is supported */

    ogs_pool_alloc(&ogs_pfcp_bar_pool, &bar);
    ogs_assert(bar);
    memset(bar, 0, sizeof *bar);

    bar->sess = sess;
    sess->bar = bar;

    return bar;
}

void ogs_pfcp_bar_delete(ogs_pfcp_bar_t *bar)
{
    ogs_pfcp_sess_t *sess = NULL;

    ogs_assert(bar);
    sess = bar->sess;
    ogs_assert(sess);

    bar->sess = NULL;
    sess->bar = NULL;

    ogs_pool_free(&ogs_pfcp_bar_pool, bar);
}

int ogs_pfcp_ue_pool_generate(void)
{
    int i, rv;
    ogs_pfcp_subnet_t *subnet = NULL;

    ogs_list_for_each(&self.subnet_list, subnet) {
        int maxbytes = 0;
        int lastindex = 0;
        uint32_t start[4], end[4], broadcast[4];
        int rangeindex, num_of_range;
        int poolindex;
        int inc;

        if (subnet->family == AF_INET) {
            maxbytes = 4;
            lastindex = 0;
        } else if (subnet->family == AF_INET6) {
            maxbytes = 16;
            lastindex = 3;
        } else {
            /* subnet->family might be AF_UNSPEC. So, skip it */
            continue;
        }

        for (i = 0; i < 4; i++) {
            broadcast[i] = subnet->sub.sub[i] + ~subnet->sub.mask[i];
        }

        num_of_range = subnet->num_of_range;
        if (!num_of_range) num_of_range = 1;

        poolindex = 0;
        for (rangeindex = 0; rangeindex < num_of_range; rangeindex++) {

            if (subnet->num_of_range &&
                subnet->range[rangeindex].low) {
                ogs_ipsubnet_t low;
                rv = ogs_ipsubnet(
                        &low, subnet->range[rangeindex].low, NULL);
                ogs_assert(rv == OGS_OK);
                memcpy(start, low.sub, maxbytes);
            } else {
                memcpy(start, subnet->sub.sub, maxbytes);
            }

            if (subnet->num_of_range &&
                subnet->range[rangeindex].high) {
                ogs_ipsubnet_t high;
                rv = ogs_ipsubnet(
                        &high, subnet->range[rangeindex].high, NULL);
                ogs_assert(rv == OGS_OK);
                high.sub[lastindex] += htobe32(1);
                memcpy(end, high.sub, maxbytes);
            } else {
                memcpy(end, broadcast, maxbytes);
            }

            inc = 0;
            while(poolindex < ogs_config()->pool.sess) {
                ogs_pfcp_ue_ip_t *ue_ip = NULL;

                ue_ip = &subnet->pool.array[poolindex];
                ogs_assert(ue_ip);
                memset(ue_ip, 0, sizeof *ue_ip);
                ue_ip->subnet = subnet;

                memcpy(ue_ip->addr, start, maxbytes);
                ue_ip->addr[lastindex] += htobe32(inc);
                inc++;

                if (memcmp(ue_ip->addr, end, maxbytes) == 0)
                    break;

                /* Exclude Network Address */
                if (memcmp(ue_ip->addr, subnet->sub.sub, maxbytes) == 0)
                    continue;

                /* Exclude TUN IP Address */
                if (memcmp(ue_ip->addr, subnet->gw.sub, maxbytes) == 0)
                    continue;

                ogs_trace("[%d] - %x:%x:%x:%x",
                        poolindex,
                        ue_ip->addr[0], ue_ip->addr[1],
                        ue_ip->addr[2], ue_ip->addr[3]);

                poolindex++;
            }
        }
        subnet->pool.size = subnet->pool.avail = poolindex;
    }

    return OGS_OK;
}

static ogs_pfcp_subnet_t *find_subnet(int family, const char *apn)
{
    ogs_pfcp_subnet_t *subnet = NULL;

    ogs_assert(apn);
    ogs_assert(family == AF_INET || family == AF_INET6);

    ogs_list_for_each(&self.subnet_list, subnet) {
        if ((subnet->family == AF_UNSPEC || subnet->family == family) &&
            (strlen(subnet->apn) == 0 ||
                (strlen(subnet->apn) && strcmp(subnet->apn, apn) == 0))) {
            return subnet;
        }
    }

    if (subnet == NULL)
        ogs_error("CHECK CONFIGURATION: Cannot find PDN[family:%d, apn:%s]",
                family, apn);

    return subnet;
}

ogs_pfcp_ue_ip_t *ogs_pfcp_ue_ip_alloc(
        int family, const char *apn, uint8_t *addr)
{
    ogs_pfcp_subnet_t *subnet = NULL;
    ogs_pfcp_ue_ip_t *ue_ip = NULL;

    uint8_t zero[16];
    size_t maxbytes = 0;

    ogs_assert(apn);
    subnet = find_subnet(family, apn);
    ogs_assert(subnet);

    memset(zero, 0, sizeof zero);
    if (family == AF_INET) {
        maxbytes = 4;
    } else if (family == AF_INET6) {
        maxbytes = 16;
    } else {
        ogs_fatal("Invalid family[%d]", family);
        ogs_assert_if_reached();
    }

    // if assigning a static IP, do so. If not, assign dynamically!
    if (memcmp(addr, zero, maxbytes) != 0) {
        ue_ip = ogs_calloc(1, sizeof(ogs_pfcp_ue_ip_t));

        ue_ip->subnet = subnet;
        ue_ip->static_ip = true;
        memcpy(ue_ip->addr, addr, maxbytes);
    } else {
        ogs_pool_alloc(&subnet->pool, &ue_ip);
    }

    ogs_assert(ue_ip);
    return ue_ip;
}

void ogs_pfcp_ue_ip_free(ogs_pfcp_ue_ip_t *ue_ip)
{
    ogs_pfcp_subnet_t *subnet = NULL;

    ogs_assert(ue_ip);
    subnet = ue_ip->subnet;

    ogs_assert(subnet);

    if (ue_ip->static_ip) {
        ogs_free(ue_ip);
    } else {
        ogs_pool_free(&subnet->pool, ue_ip);
    }
}

ogs_pfcp_dev_t *ogs_pfcp_dev_add(const char *ifname)
{
    ogs_pfcp_dev_t *dev = NULL;

    ogs_assert(ifname);

    ogs_pool_alloc(&ogs_pfcp_dev_pool, &dev);
    ogs_assert(dev);
    memset(dev, 0, sizeof *dev);

    strcpy(dev->ifname, ifname);

    ogs_list_add(&self.dev_list, dev);

    return dev;
}

void ogs_pfcp_dev_remove(ogs_pfcp_dev_t *dev)
{
    ogs_assert(dev);

    ogs_list_remove(&self.dev_list, dev);

    if (dev->link_local_addr)
        ogs_freeaddrinfo(dev->link_local_addr);

    ogs_pool_free(&ogs_pfcp_dev_pool, dev);
}

void ogs_pfcp_dev_remove_all(void)
{
    ogs_pfcp_dev_t *dev = NULL, *next_dev = NULL;

    ogs_list_for_each_safe(&self.dev_list, next_dev, dev)
        ogs_pfcp_dev_remove(dev);
}

ogs_pfcp_dev_t *ogs_pfcp_dev_find_by_ifname(const char *ifname)
{
    ogs_pfcp_dev_t *dev = NULL;

    ogs_assert(ifname);

    ogs_list_for_each(&ogs_pfcp_self()->dev_list, dev) {
        if (strcmp(dev->ifname, ifname) == 0)
            return dev;
    }

    return OGS_OK;
}

ogs_pfcp_subnet_t *ogs_pfcp_subnet_add(
        const char *ipstr, const char *mask_or_numbits,
        const char *apn, const char *ifname)
{
    int rv;
    ogs_pfcp_dev_t *dev = NULL;
    ogs_pfcp_subnet_t *subnet = NULL;

    ogs_assert(ifname);

    dev = ogs_pfcp_dev_find_by_ifname(ifname);
    if (!dev)
        dev = ogs_pfcp_dev_add(ifname);
    ogs_assert(dev);

    ogs_pool_alloc(&ogs_pfcp_subnet_pool, &subnet);
    ogs_assert(subnet);
    memset(subnet, 0, sizeof *subnet);

    subnet->family = AF_UNSPEC;
    subnet->dev = dev;

    if (ipstr && mask_or_numbits) {
        rv = ogs_ipsubnet(&subnet->gw, ipstr, NULL);
        ogs_assert(rv == OGS_OK);

        rv = ogs_ipsubnet(&subnet->sub, ipstr, mask_or_numbits);
        ogs_assert(rv == OGS_OK);

        subnet->family = subnet->gw.family;
        subnet->prefixlen = atoi(mask_or_numbits);
    }

    if (apn)
        strcpy(subnet->apn, apn);

    ogs_pool_init(&subnet->pool, ogs_config()->pool.sess);

    ogs_list_add(&self.subnet_list, subnet);

    return subnet;
}

void ogs_pfcp_subnet_remove(ogs_pfcp_subnet_t *subnet)
{
    ogs_assert(subnet);

    ogs_list_remove(&self.subnet_list, subnet);

    ogs_pool_final(&subnet->pool);

    ogs_pool_free(&ogs_pfcp_subnet_pool, subnet);
}

void ogs_pfcp_subnet_remove_all(void)
{
    ogs_pfcp_subnet_t *subnet = NULL, *next_subnet = NULL;

    ogs_list_for_each_safe(&self.subnet_list, next_subnet, subnet)
        ogs_pfcp_subnet_remove(subnet);
}
