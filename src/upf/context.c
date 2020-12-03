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

#include "context.h"

static upf_context_t self;

int __upf_log_domain;

static OGS_POOL(upf_sess_pool, upf_sess_t);

static int context_initialized = 0;

void upf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize UPF context */
    memset(&self, 0, sizeof(upf_context_t));

    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", ogs_core()->log.level);
    ogs_log_install_domain(&__upf_log_domain, "upf", ogs_core()->log.level);

    /* Setup UP Function Features */
    ogs_pfcp_self()->up_function_features.ftup = 1;
    ogs_pfcp_self()->up_function_features.empu = 1;
    ogs_pfcp_self()->up_function_features_len = 2;

    ogs_gtp_node_init();

    ogs_list_init(&self.sess_list);
    ogs_list_init(&self.gtpu_list);
    ogs_list_init(&self.peer_list);

    ogs_pool_init(&upf_sess_pool, ogs_app()->pool.sess);

    self.sess_hash = ogs_hash_make();
    self.ipv4_hash = ogs_hash_make();
    self.ipv6_hash = ogs_hash_make();

    context_initialized = 1;
}

void upf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    upf_sess_remove_all();

    ogs_assert(self.sess_hash);
    ogs_hash_destroy(self.sess_hash);
    ogs_assert(self.ipv4_hash);
    ogs_hash_destroy(self.ipv4_hash);
    ogs_assert(self.ipv6_hash);
    ogs_hash_destroy(self.ipv6_hash);

    ogs_pool_final(&upf_sess_pool);

    ogs_gtp_node_remove_all(&self.peer_list);

    ogs_gtp_node_final();

    context_initialized = 0;
}

upf_context_t *upf_self(void)
{
    return &self;
}

static int upf_context_prepare(void)
{
    self.gtpu_port = OGS_GTPV1_U_UDP_PORT;

    return OGS_OK;
}

static int upf_context_validation(void)
{
    if (ogs_list_first(&self.gtpu_list) == NULL) {
        ogs_error("No upf.gtpu in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    if (ogs_list_first(&ogs_pfcp_self()->subnet_list) == NULL) {
        ogs_error("No upf.pdn: in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int upf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = upf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "upf")) {
            ogs_yaml_iter_t upf_iter;
            ogs_yaml_iter_recurse(&root_iter, &upf_iter);
            while (ogs_yaml_iter_next(&upf_iter)) {
                const char *upf_key = ogs_yaml_iter_key(&upf_iter);
                ogs_assert(upf_key);
                if (!strcmp(upf_key, "gtpu")) {
                    ogs_list_t list, list6;
                    ogs_socknode_t *node = NULL, *node6 = NULL;
                    ogs_socknode_t *iter = NULL, *next_iter = NULL;

                    ogs_yaml_iter_t gtpu_array, gtpu_iter;
                    ogs_yaml_iter_recurse(&upf_iter, &gtpu_array);

                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        int adv_num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        const char *adv_hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpu_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;
                        ogs_sockaddr_t *adv_addr = NULL;
                        ogs_sockaddr_t *adv_addr6 = NULL;
                        const char *teid_range_indication = NULL;
                        const char *teid_range = NULL;
                        const char *network_instance = NULL;
                        const char *source_interface = NULL;

                        if (ogs_yaml_iter_type(&gtpu_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&gtpu_iter, &gtpu_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&gtpu_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&gtpu_array))
                                break;
                            ogs_yaml_iter_recurse(&gtpu_array, &gtpu_iter);
                        } else if (ogs_yaml_iter_type(&gtpu_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&gtpu_iter)) {
                            const char *gtpu_key =
                                ogs_yaml_iter_key(&gtpu_iter);
                            ogs_assert(gtpu_key);

                            if (ogs_list_count(
                                    &ogs_pfcp_self()->gtpu_resource_list) >=
                                OGS_MAX_NUM_OF_GTPU_RESOURCE) {
                                ogs_warn("[Overflow]: Number of User Plane "
                                    "IP Resource <= %d",
                                    OGS_MAX_NUM_OF_GTPU_RESOURCE);
                                break;
                            }

                            if (!strcmp(gtpu_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&gtpu_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d)"
                                        ": AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(gtpu_key, "addr") ||
                                    !strcmp(gtpu_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(
                                        &gtpu_iter, &hostname_iter);
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
                            } else if (!strcmp(gtpu_key, "advertise_addr") ||
                                    !strcmp(gtpu_key, "advertise_name")) {
                                ogs_yaml_iter_t adv_hostname_iter;
                                ogs_yaml_iter_recurse(
                                        &gtpu_iter, &adv_hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(
                                    &adv_hostname_iter) != YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(
                                        &adv_hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(
                                            &adv_hostname_iter))
                                            break;
                                    }

                                    ogs_assert(adv_num <=
                                            OGS_MAX_NUM_OF_HOSTNAME);
                                    adv_hostname[adv_num++] =
                                        ogs_yaml_iter_value(&adv_hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&adv_hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(gtpu_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&gtpu_iter);
                                if (v) port = atoi(v);
                            } else if (!strcmp(gtpu_key, "dev")) {
                                dev = ogs_yaml_iter_value(&gtpu_iter);
                            } else if (!strcmp(gtpu_key,
                                        "teid_range_indication")) {
                                teid_range_indication =
                                    ogs_yaml_iter_value(&gtpu_iter);
                            } else if (!strcmp(gtpu_key,
                                        "teid_range")) {
                                teid_range = ogs_yaml_iter_value(&gtpu_iter);
                            } else if (!strcmp(gtpu_key,
                                        "network_instance")) {
                                network_instance =
                                    ogs_yaml_iter_value(&gtpu_iter);
                            } else if (!strcmp(gtpu_key,
                                        "source_interface")) {
                                source_interface =
                                    ogs_yaml_iter_value(&gtpu_iter);
                            } else
                                ogs_warn("unknown key `%s`", gtpu_key);
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
                                ogs_socknode_add(&list, AF_INET, addr);
                            if (ogs_app()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(&list6, AF_INET6, addr);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                ogs_app()->parameter.no_ipv4 ? NULL : &list,
                                ogs_app()->parameter.no_ipv6 ? NULL : &list6,
                                dev, port);
                            ogs_assert(rv == OGS_OK);
                        }

                        adv_addr = NULL;
                        for (i = 0; i < adv_num; i++) {
                            rv = ogs_addaddrinfo(&adv_addr,
                                    family, adv_hostname[i], port, 0);
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
                         * 127.0.0.4, 127.0.0.5 and cafe::1 are ignored
                         * on PFCP Assocation Response message's
                         * user plane IP resource information.
                         *
                         * gtpu:
                         *   - addr:
                         *     - 127.0.0.3
                         *     - ::1
                         *     - 127.0.0.4
                         *     - 127.0.0.5
                         *     - cafe::1
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
                         *     - cafe::1
                         */
                        node = ogs_list_first(&list);
                        node6 = ogs_list_first(&list6);
                        if (node || node6) {
                            ogs_pfcp_user_plane_ip_resource_info_t info;

                            memset(&info, 0, sizeof(info));
                            ogs_pfcp_sockaddr_to_user_plane_ip_resource_info(
                                    adv_addr ? adv_addr :
                                        node ? node->addr : NULL,
                                    adv_addr6 ? adv_addr6 :
                                        node6 ? node6->addr : NULL,
                                    &info);

                            if (teid_range_indication) {
                                info.teidri = atoi(teid_range_indication);
                                if (teid_range) {
                                    info.teid_range = atoi(teid_range);
                                }
                            }
                            if (network_instance) {
                                info.assoni = 1;
                                ogs_cpystrn(info.network_instance,
                                    network_instance, OGS_MAX_APN_LEN+1);
                            }
                            if (source_interface) {
                                info.assosi = 1;
                                info.source_interface = atoi(source_interface);
                            }

                            ogs_pfcp_gtpu_resource_add(
                                &ogs_pfcp_self()->gtpu_resource_list, &info);
                        }

                        ogs_list_for_each_safe(&list, next_iter, iter)
                            ogs_list_add(&self.gtpu_list, iter);
                        ogs_list_for_each_safe(&list6, next_iter, iter)
                            ogs_list_add(&self.gtpu_list, iter);

                        ogs_freeaddrinfo(adv_addr);
                        ogs_freeaddrinfo(adv_addr6);

                    } while (ogs_yaml_iter_type(&gtpu_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.gtpu_list) == NULL) {
                        ogs_list_init(&list);
                        ogs_list_init(&list6);

                        rv = ogs_socknode_probe(
                            ogs_app()->parameter.no_ipv4 ? NULL : &list,
                            ogs_app()->parameter.no_ipv6 ? NULL : &list6,
                            NULL, self.gtpu_port);
                        ogs_assert(rv == OGS_OK);

                        /*
                         * The first tuple IPv4/IPv6 are added
                         * in User Plane IP resource information.
                         *
                         * TEID Range, Network Instance, Source Interface
                         * cannot be configured in automatic IP detection.
                         */
                        node = ogs_list_first(&list);
                        node6 = ogs_list_first(&list6);
                        if (node || node6) {
                            ogs_pfcp_user_plane_ip_resource_info_t info;

                            memset(&info, 0, sizeof(info));
                            ogs_pfcp_sockaddr_to_user_plane_ip_resource_info(
                                    node ? node->addr : NULL,
                                    node6 ? node6->addr : NULL,
                                    &info);

                            ogs_pfcp_gtpu_resource_add(
                                &ogs_pfcp_self()->gtpu_resource_list, &info);
                        }

                        ogs_list_for_each_safe(&list, next_iter, iter)
                            ogs_list_add(&self.gtpu_list, iter);
                        ogs_list_for_each_safe(&list6, next_iter, iter)
                            ogs_list_add(&self.gtpu_list, iter);
                    }
                } else if (!strcmp(upf_key, "pdn")) {
                    /* handle config in pfcp library */
                }
            }
        }
    }

    rv = upf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

upf_sess_t *upf_sess_add(ogs_pfcp_f_seid_t *cp_f_seid)
{
    upf_sess_t *sess = NULL;

    ogs_assert(cp_f_seid);

    ogs_pool_alloc(&upf_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    ogs_pfcp_pool_init(&sess->pfcp);

    sess->index = ogs_pool_index(&upf_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_app()->pool.sess);

    sess->upf_n4_seid = sess->index;
    sess->smf_n4_seid = cp_f_seid->seid;
    ogs_hash_set(self.sess_hash, &sess->smf_n4_seid,
            sizeof(sess->smf_n4_seid), sess);

    ogs_list_add(&self.sess_list, sess);

    ogs_info("[Added] Number of UPF-Sessions is now %d",
            ogs_list_count(&self.sess_list));

    return sess;
}

int upf_sess_remove(upf_sess_t *sess)
{
    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);
    ogs_pfcp_sess_clear(&sess->pfcp);

    ogs_hash_set(self.sess_hash, &sess->smf_n4_seid,
            sizeof(sess->smf_n4_seid), NULL);

    if (sess->ipv4) {
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv4);
    }
    if (sess->ipv6) {
        ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv6);
    }

    ogs_pfcp_pool_final(&sess->pfcp);

    ogs_pool_free(&upf_sess_pool, sess);

    ogs_info("[Removed] Number of UPF-sessions is now %d",
            ogs_list_count(&self.sess_list));

    return OGS_OK;
}

void upf_sess_remove_all(void)
{
    upf_sess_t *sess = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.sess_list, next, sess) {
        upf_sess_remove(sess);
    }
}

upf_sess_t *upf_sess_find(uint32_t index)
{
    ogs_assert(index);
    return ogs_pool_find(&upf_sess_pool, index);
}

upf_sess_t *upf_sess_find_by_cp_seid(uint64_t seid)
{
    return (upf_sess_t *)ogs_hash_get(self.sess_hash, &seid, sizeof(seid));
}

upf_sess_t *upf_sess_find_by_up_seid(uint64_t seid)
{
    return upf_sess_find(seid);
}

upf_sess_t *upf_sess_find_by_ipv4(uint32_t addr)
{
    ogs_assert(self.ipv4_hash);
    return (upf_sess_t *)ogs_hash_get(self.ipv4_hash, &addr, OGS_IPV4_LEN);
}

upf_sess_t *upf_sess_find_by_ipv6(uint32_t *addr6)
{
    ogs_assert(self.ipv6_hash);
    ogs_assert(addr6);
    return (upf_sess_t *)ogs_hash_get(self.ipv6_hash, addr6, OGS_IPV6_LEN);
}

upf_sess_t *upf_sess_add_by_message(ogs_pfcp_message_t *message)
{
    upf_sess_t *sess = NULL;
    ogs_pfcp_f_seid_t *f_seid = NULL;

    ogs_pfcp_session_establishment_request_t *req =
        &message->pfcp_session_establishment_request;;

    f_seid = req->cp_f_seid.data;
    if (req->cp_f_seid.presence == 0 || f_seid == NULL) {
        ogs_error("No CP F-SEID");
        return NULL;
    }
    f_seid->seid = be64toh(f_seid->seid);

    sess = upf_sess_find_by_cp_seid(f_seid->seid);
    if (!sess) {
        sess = upf_sess_add(f_seid);
        if (!sess) return NULL;
    }
    ogs_assert(sess);

    return sess;
}

void upf_sess_set_ue_ip(upf_sess_t *sess,
        uint8_t pdn_type, ogs_pfcp_pdr_t *pdr)
{
    ogs_pfcp_ue_ip_addr_t *ue_ip = NULL;
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];

    ogs_assert(sess);
    ogs_assert(pdn_type);
    ogs_assert(pdr);
    ogs_assert(pdr->ue_ip_addr_len);
    ue_ip = &pdr->ue_ip_addr;
    ogs_assert(ue_ip);

    /* Set PDN-Type and UE IP Address */
    sess->pdn.pdn_type = pdn_type;
    if (pdn_type == OGS_GTP_PDN_TYPE_IPV4) {
        if (ue_ip->ipv4 || pdr->dnn) {
            sess->ipv4 = ogs_pfcp_ue_ip_alloc(
                    AF_INET, pdr->dnn, (uint8_t *)&(ue_ip->addr));
            ogs_assert(sess->ipv4);
            ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                pdn_type, ue_ip->ipv4, ue_ip->ipv6, pdr->dnn ? pdr->dnn : "");
        }
    } else if (pdn_type == OGS_GTP_PDN_TYPE_IPV6) {
        if (ue_ip->ipv6 || pdr->dnn) {
            sess->ipv6 = ogs_pfcp_ue_ip_alloc(AF_INET6, pdr->dnn, ue_ip->addr6);
            ogs_assert(sess->ipv6);
            ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                pdn_type, ue_ip->ipv4, ue_ip->ipv6, pdr->dnn ? pdr->dnn : "");
        }
    } else if (pdn_type == OGS_GTP_PDN_TYPE_IPV4V6) {
        if (ue_ip->ipv4 || pdr->dnn) {
            sess->ipv4 = ogs_pfcp_ue_ip_alloc(
                    AF_INET, pdr->dnn, (uint8_t *)&(ue_ip->both.addr));
            ogs_assert(sess->ipv4);
            ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                pdn_type, ue_ip->ipv4, ue_ip->ipv6, pdr->dnn ? pdr->dnn : "");
        }

        if (ue_ip->ipv6 || pdr->dnn) {
            sess->ipv6 = ogs_pfcp_ue_ip_alloc(
                    AF_INET6, pdr->dnn, ue_ip->both.addr6);
            ogs_assert(sess->ipv6);
            ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                pdn_type, ue_ip->ipv4, ue_ip->ipv6, pdr->dnn ? pdr->dnn : "");
        }
    } else {
        ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                pdn_type, ue_ip->ipv4, ue_ip->ipv6, pdr->dnn ? pdr->dnn : "");
    }

    ogs_info("UE F-SEID[CP:0x%lx,UP:0x%lx] "
             "APN[%s] PDN-Type[%d] IPv4[%s] IPv6[%s]",
        (long)sess->upf_n4_seid, (long)sess->smf_n4_seid,
        pdr->dnn, pdn_type,
        sess->ipv4 ? OGS_INET_NTOP(&sess->ipv4->addr, buf1) : "",
        sess->ipv6 ? OGS_INET6_NTOP(&sess->ipv6->addr, buf2) : "");
}
