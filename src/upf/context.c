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
static OGS_POOL(upf_sdf_filter_pool, upf_sdf_filter_t);

static int context_initiaized = 0;

int num_sessions = 0;
void stats_add_session(void) {
    num_sessions = num_sessions + 1;
    ogs_info("Added a session. Number of active sessions is now %d",
            num_sessions);
}

void stats_remove_session(void) {
    num_sessions = num_sessions - 1;
    ogs_info("Removed a session. Number of active sessions is now %d",
            num_sessions);
}

void upf_context_init(void)
{
    ogs_assert(context_initiaized == 0);

    /* Initialize UPF context */
    memset(&self, 0, sizeof(upf_context_t));

    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", ogs_core()->log.level);
    ogs_log_install_domain(&__upf_log_domain, "upf", ogs_core()->log.level);

    ogs_gtp_node_init(512);

    ogs_list_init(&self.sess_list);

    ogs_list_init(&self.gtpu_list);
    ogs_list_init(&self.gtpu_resource_list);

    ogs_list_init(&self.gnb_n3_list);

    ogs_pool_init(&upf_sess_pool, ogs_config()->pool.sess);
    ogs_pool_init(&upf_sdf_filter_pool,
            ogs_config()->pool.sess * OGS_MAX_NUM_OF_RULE);

    self.sess_hash = ogs_hash_make();
    self.ipv4_hash = ogs_hash_make();
    self.ipv6_hash = ogs_hash_make();

    context_initiaized = 1;
}

void upf_context_final(void)
{
    ogs_assert(context_initiaized == 1);

    upf_sess_remove_all();

    ogs_assert(self.sess_hash);
    ogs_hash_destroy(self.sess_hash);
    ogs_assert(self.ipv4_hash);
    ogs_hash_destroy(self.ipv4_hash);
    ogs_assert(self.ipv6_hash);
    ogs_hash_destroy(self.ipv6_hash);

    ogs_pool_final(&upf_sess_pool);
    ogs_pool_final(&upf_sdf_filter_pool);

    ogs_gtp_node_remove_all(&self.gnb_n3_list);

    ogs_gtp_node_final();
    ogs_pfcp_gtpu_resource_remove_all(&self.gtpu_resource_list);

    context_initiaized = 0;
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
        ogs_error("No upf.gtpu in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int upf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_config()->document;
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
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpu_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;
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
                                &self.gtpu_resource_list) >=
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
                            } else if (!strcmp(gtpu_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&gtpu_iter);
                                if (v) {
                                    port = atoi(v);
                                    self.gtpu_port = port;
                                }
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
                            if (ogs_config()->parameter.no_ipv4 == 0)
                                ogs_socknode_add(&list, AF_INET, addr);
                            if (ogs_config()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(&list6, AF_INET6, addr);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                ogs_config()->parameter.no_ipv4 ? NULL : &list,
                                ogs_config()->parameter.no_ipv6 ? NULL : &list6,
                                dev, self.gtpu_port);
                            ogs_assert(rv == OGS_OK);
                        }

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
                                    node ? node->addr : NULL,
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
                                &self.gtpu_resource_list, &info);
                        }

                        ogs_list_for_each_safe(&list, next_iter, iter)
                            ogs_list_add(&self.gtpu_list, iter);
                        ogs_list_for_each_safe(&list6, next_iter, iter)
                            ogs_list_add(&self.gtpu_list, iter);

                    } while (ogs_yaml_iter_type(&gtpu_array) == 
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.gtpu_list) == NULL) {
                        ogs_list_init(&list);
                        ogs_list_init(&list6);

                        rv = ogs_socknode_probe(
                            ogs_config()->parameter.no_ipv4 ? NULL : &list,
                            ogs_config()->parameter.no_ipv6 ? NULL : &list6,
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
                                &self.gtpu_resource_list, &info);
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

upf_sess_t *upf_sess_add(ogs_pfcp_f_seid_t *cp_f_seid,
        const char *apn, uint8_t pdn_type, ogs_pfcp_ue_ip_addr_t *ue_ip,
        ogs_pfcp_pdr_id_t default_pdr_id)
{
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];
    upf_sess_t *sess = NULL;

    ogs_assert(cp_f_seid);
    ogs_assert(apn);
    ogs_assert(ue_ip);

    ogs_pool_alloc(&upf_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    sess->index = ogs_pool_index(&upf_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_config()->pool.sess);

    sess->upf_n4_seid = sess->index;
    sess->smf_n4_seid = cp_f_seid->seid;
    ogs_hash_set(self.sess_hash, &sess->smf_n4_seid,
            sizeof(sess->smf_n4_seid), sess);

    /* Set APN */
    ogs_cpystrn(sess->pdn.apn, apn, OGS_MAX_APN_LEN+1);

    /* Set PDN-Type and UE IP Address */
    sess->pdn.pdn_type = pdn_type;
    if (pdn_type == OGS_GTP_PDN_TYPE_IPV4) {
        if (ue_ip->ipv4 == 0) {
            ogs_error("Cannot support PDN-Type[%d] != [IPv4:%d, IPv6:%d]",
                    pdn_type, ue_ip->ipv4, ue_ip->ipv6);
            goto cleanup;
        }
        sess->ipv4 = ogs_pfcp_ue_ip_alloc(
                AF_INET, apn, (uint8_t *)&(ue_ip->addr));
        ogs_assert(sess->ipv4);
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
    } else if (pdn_type == OGS_GTP_PDN_TYPE_IPV6) {
        if (ue_ip->ipv6 == 0) {
            ogs_error("Cannot support PDN-Type[%d] != [IPv4:%d, IPv6:%d]",
                    pdn_type, ue_ip->ipv4, ue_ip->ipv6);
            goto cleanup;
        }
        sess->ipv6 = ogs_pfcp_ue_ip_alloc(AF_INET6, apn, ue_ip->addr6);
        ogs_assert(sess->ipv6);
        ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, sess);
    } else if (pdn_type == OGS_GTP_PDN_TYPE_IPV4V6) {
        if (ue_ip->ipv4 == 0 || ue_ip->ipv6 == 0) {
            ogs_error("Cannot support PDN-Type[%d] != [IPv4:%d, IPv6:%d]",
                    pdn_type, ue_ip->ipv4, ue_ip->ipv6);
            goto cleanup;
        }
        sess->ipv4 = ogs_pfcp_ue_ip_alloc(
                AF_INET, apn, (uint8_t *)&(ue_ip->both.addr));
        ogs_assert(sess->ipv4);
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);

        sess->ipv6 = ogs_pfcp_ue_ip_alloc(AF_INET6, apn, ue_ip->both.addr6);
        ogs_assert(sess->ipv6);
        ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, sess);
    } else {
        ogs_error("Cannot support PDN-Type[%d] != [IPv4:%d, IPv6:%d]",
                pdn_type, ue_ip->ipv4, ue_ip->ipv6);
        goto cleanup;
    }

    /* Set Default PDR */
    OGS_SETUP_DEFAULT_PDR(&sess->pfcp,
        ogs_pfcp_pdr_find_or_add(&sess->pfcp, default_pdr_id));

    ogs_info("UE F-SEID[CP:%ld,UP:%ld] "
             "APN[%s] PDN-Type[%d] IPv4[%s] IPv6[%s], Default PDR ID[%d]",
        (long)sess->upf_n4_seid, (long)sess->smf_n4_seid,
        apn, pdn_type,
        sess->ipv4 ? INET_NTOP(&sess->ipv4->addr, buf1) : "",
        sess->ipv6 ? INET6_NTOP(&sess->ipv6->addr, buf2) : "",
        sess->pfcp.default_pdr->id);

    ogs_list_add(&self.sess_list, sess);
    
    stats_add_session();

    return sess;

cleanup:
    ogs_pool_free(&upf_sess_pool, sess);
    return NULL;
}

int upf_sess_remove(upf_sess_t *sess)
{
    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);
    ogs_pfcp_sess_clear(&sess->pfcp);
    upf_sdf_filter_remove_all(sess);

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

    ogs_pool_free(&upf_sess_pool, sess);

    stats_remove_session();

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
    char apn[OGS_MAX_APN_LEN];
    ogs_pfcp_ue_ip_addr_t *addr = NULL;
    bool default_pdr_found = false;
    ogs_pfcp_pdr_id_t default_pdr_id;

    ogs_pfcp_session_establishment_request_t *req =
        &message->pfcp_session_establishment_request;;
    int i;

    f_seid = req->cp_f_seid.data;
    if (req->cp_f_seid.presence == 0 || f_seid == NULL) {
        ogs_error("No CP F-SEID");
        return NULL;
    }
    f_seid->seid = be64toh(f_seid->seid);

    if (req->pdn_type.presence == 0) {
        ogs_error("No PDN Type");
        return NULL;
    }

    /* Find the Default PDR :
     * - PDR ID is existed
     * - SDF Filter is NOT existed
     * - APN(Network Instance) is existed
     * - UE IP Address is existed
     * - Downlink PDR
     */
    memset(apn, 0, sizeof(apn));
    for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
        ogs_pfcp_tlv_create_pdr_t *message = &req->create_pdr[i];
        ogs_assert(message);
        if (message->presence == 0)
            continue;
        if (message->pdr_id.presence == 0)
            continue;
        if (message->pdi.presence == 0)
            continue;
        if (message->pdi.sdf_filter[0].presence) /* No SDF Filter */
            continue;
        if (message->pdi.network_instance.presence == 0)
            continue;
        if (message->pdi.ue_ip_address.presence == 0)
            continue;
        if (message->pdi.source_interface.presence == 0)
            continue;
        if (message->pdi.source_interface.u8 != OGS_PFCP_INTERFACE_CORE)
            continue;

        default_pdr_id = message->pdr_id.u16;
        ogs_fqdn_parse(apn,
            message->pdi.network_instance.data,
            message->pdi.network_instance.len);
        addr = message->pdi.ue_ip_address.data;

        default_pdr_found = true;
        break;
    }

    if (!default_pdr_found) {
        ogs_error("Cannot find Default PDR");
        return NULL;
    }

    if (strlen(apn) == 0) {
        ogs_error("No APN in PDR");
        return NULL;
    }

    if (!addr) {
        ogs_error("No UE IP Address in PDR");
        return NULL;
    }

    sess = upf_sess_find_by_cp_seid(f_seid->seid);
    if (!sess) {
        sess = upf_sess_add(
                f_seid, apn, req->pdn_type.u8, addr, default_pdr_id);
        if (!sess) return NULL;
    }
    ogs_assert(sess);

    return sess;
}

upf_sdf_filter_t *upf_sdf_filter_add(ogs_pfcp_pdr_t *pdr)
{
    upf_sdf_filter_t *sdf_filter = NULL;
    ogs_pfcp_sess_t *pfcp = NULL;
    upf_sess_t *sess = NULL;

    ogs_assert(pdr);
    pfcp = pdr->sess;
    ogs_assert(pfcp);
    sess = UPF_SESS(pfcp);
    ogs_assert(sess);

    ogs_pool_alloc(&upf_sdf_filter_pool, &sdf_filter);
    ogs_assert(sdf_filter);
    memset(sdf_filter, 0, sizeof *sdf_filter);

    sdf_filter->pdr = pdr;
    ogs_list_add(&sess->sdf_filter_list, sdf_filter);

    return sdf_filter;
}

void upf_sdf_filter_remove(upf_sdf_filter_t *sdf_filter)
{
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_sess_t *pfcp = NULL;
    upf_sess_t *sess = NULL;

    ogs_assert(sdf_filter);
    pdr = sdf_filter->pdr;
    ogs_assert(pdr);
    pfcp = pdr->sess;
    ogs_assert(pfcp);
    sess = UPF_SESS(pfcp);
    ogs_assert(sess);

    ogs_list_remove(&sess->sdf_filter_list, sdf_filter);
    ogs_pool_free(&upf_sdf_filter_pool, sdf_filter);
}

void upf_sdf_filter_remove_all(upf_sess_t *sess)
{
    upf_sdf_filter_t *sdf_filter = NULL, *next_sdf_filter = NULL;

    ogs_assert(sess);

    ogs_list_for_each_safe(&sess->sdf_filter_list, next_sdf_filter, sdf_filter)
        upf_sdf_filter_remove(sdf_filter);
}
