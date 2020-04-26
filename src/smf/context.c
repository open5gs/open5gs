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
#include "smf-sm.h"

static smf_context_t self;
static ogs_diam_config_t g_diam_conf;

int __smf_log_domain;

static OGS_POOL(smf_sess_pool, smf_sess_t);
static OGS_POOL(smf_bearer_pool, smf_bearer_t);

static OGS_POOL(smf_pf_pool, smf_pf_t);

static int context_initiaized = 0;

int num_sessions = 0;
void stats_add_session(void) {
    num_sessions = num_sessions + 1;
    ogs_info("Added a session. Number of active sessions is now %d", num_sessions);
}

void stats_remove_session(void) {
    num_sessions = num_sessions - 1;
    ogs_info("Removed a session. Number of active sessions is now %d", num_sessions);
}

void smf_context_init(void)
{
    ogs_assert(context_initiaized == 0);

    /* Initial FreeDiameter Config */
    memset(&g_diam_conf, 0, sizeof(ogs_diam_config_t));

    /* Initialize SMF context */
    memset(&self, 0, sizeof(smf_context_t));
    self.diam_config = &g_diam_conf;

    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", ogs_core()->log.level);
    ogs_log_install_domain(&__ogs_diam_domain, "diam", ogs_core()->log.level);
    ogs_log_install_domain(&__smf_log_domain, "smf", ogs_core()->log.level);

    ogs_gtp_node_init(512);

    ogs_list_init(&self.sess_list);

    ogs_list_init(&self.gtpc_list);
    ogs_list_init(&self.gtpc_list6);

    ogs_list_init(&self.sgw_s5c_list);

    ogs_pool_init(&smf_sess_pool, ogs_config()->pool.sess);
    ogs_pool_init(&smf_bearer_pool, ogs_config()->pool.bearer);

    ogs_pool_init(&smf_pf_pool, ogs_config()->pool.pf);

    self.sess_hash = ogs_hash_make();
    self.ipv4_hash = ogs_hash_make();
    self.ipv6_hash = ogs_hash_make();

    context_initiaized = 1;
}

void smf_context_final(void)
{
    ogs_assert(context_initiaized == 1);

    smf_sess_remove_all();

    ogs_assert(self.sess_hash);
    ogs_hash_destroy(self.sess_hash);
    ogs_assert(self.ipv4_hash);
    ogs_hash_destroy(self.ipv4_hash);
    ogs_assert(self.ipv6_hash);
    ogs_hash_destroy(self.ipv6_hash);

    ogs_pool_final(&smf_bearer_pool);
    ogs_pool_final(&smf_sess_pool);

    ogs_pool_final(&smf_pf_pool);

    ogs_gtp_node_remove_all(&self.sgw_s5c_list);

    ogs_gtp_node_final();

    context_initiaized = 0;
}

smf_context_t *smf_self(void)
{
    return &self;
}

static int smf_context_prepare(void)
{
    self.gtpc_port = OGS_GTPV2_C_UDP_PORT;
    self.diam_config->cnf_port = DIAMETER_PORT;
    self.diam_config->cnf_port_tls = DIAMETER_SECURE_PORT;

    return OGS_OK;
}

static int smf_context_validation(void)
{
    if (self.diam_conf_path == NULL &&
        (self.diam_config->cnf_diamid == NULL ||
        self.diam_config->cnf_diamrlm == NULL ||
        self.diam_config->cnf_addr == NULL)) {
        ogs_error("No smf.freeDiameter in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }
    if (ogs_list_first(&self.gtpc_list) == NULL &&
        ogs_list_first(&self.gtpc_list6) == NULL) {
        ogs_error("No smf.gtpc in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }
    if (self.dns[0] == NULL && self.dns6[0] == NULL) {
        ogs_error("No smf.dns in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int smf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_config()->document;
    ogs_assert(document);

    rv = smf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "smf")) {
            ogs_yaml_iter_t smf_iter;
            ogs_yaml_iter_recurse(&root_iter, &smf_iter);
            while (ogs_yaml_iter_next(&smf_iter)) {
                const char *smf_key = ogs_yaml_iter_key(&smf_iter);
                ogs_assert(smf_key);
                if (!strcmp(smf_key, "freeDiameter")) {
                    yaml_node_t *node = 
                        yaml_document_get_node(document, smf_iter.pair->value);
                    ogs_assert(node);
                    if (node->type == YAML_SCALAR_NODE) {
                        self.diam_conf_path = ogs_yaml_iter_value(&smf_iter);
                    } else if (node->type == YAML_MAPPING_NODE) {
                        ogs_yaml_iter_t fd_iter;
                        ogs_yaml_iter_recurse(&smf_iter, &fd_iter);

                        while (ogs_yaml_iter_next(&fd_iter)) {
                            const char *fd_key = ogs_yaml_iter_key(&fd_iter);
                            ogs_assert(fd_key);
                            if (!strcmp(fd_key, "identity")) {
                                self.diam_config->cnf_diamid = 
                                    ogs_yaml_iter_value(&fd_iter);
                            } else if (!strcmp(fd_key, "realm")) {
                                self.diam_config->cnf_diamrlm = 
                                    ogs_yaml_iter_value(&fd_iter);
                            } else if (!strcmp(fd_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&fd_iter);
                                if (v) self.diam_config->cnf_port = atoi(v);
                            } else if (!strcmp(fd_key, "sec_port")) {
                                const char *v = ogs_yaml_iter_value(&fd_iter);
                                if (v) self.diam_config->cnf_port_tls = atoi(v);
                            } else if (!strcmp(fd_key, "listen_on")) {
                                self.diam_config->cnf_addr = 
                                    ogs_yaml_iter_value(&fd_iter);
                            } else if (!strcmp(fd_key, "load_extension")) {
                                ogs_yaml_iter_t ext_array, ext_iter;
                                ogs_yaml_iter_recurse(&fd_iter, &ext_array);
                                do {
                                    const char *module = NULL;
                                    const char *conf = NULL;

                                    if (ogs_yaml_iter_type(&ext_array) ==
                                        YAML_MAPPING_NODE) {
                                        memcpy(&ext_iter, &ext_array,
                                                sizeof(ogs_yaml_iter_t));
                                    } else if (ogs_yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&ext_array))
                                            break;
                                        ogs_yaml_iter_recurse(
                                                &ext_array, &ext_iter);
                                    } else if (ogs_yaml_iter_type(&ext_array) ==
                                        YAML_SCALAR_NODE) {
                                        break;
                                    } else
                                        ogs_assert_if_reached();

                                    while (ogs_yaml_iter_next(&ext_iter)) {
                                        const char *ext_key =
                                            ogs_yaml_iter_key(&ext_iter);
                                        ogs_assert(ext_key);
                                        if (!strcmp(ext_key, "module")) {
                                            module = ogs_yaml_iter_value(
                                                    &ext_iter);
                                        } else if (!strcmp(ext_key, "conf")) {
                                            conf = ogs_yaml_iter_value(
                                                    &ext_iter);
                                        } else
                                            ogs_warn("unknown key `%s`",
                                                    ext_key);
                                    }

                                    if (module) {
                                        self.diam_config->
                                            ext[self.diam_config->num_of_ext].
                                                module = module;
                                        self.diam_config->
                                            ext[self.diam_config->num_of_ext].
                                                conf = conf;
                                        self.diam_config->num_of_ext++;
                                    }
                                } while (ogs_yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(fd_key, "connect")) {
                                ogs_yaml_iter_t conn_array, conn_iter;
                                ogs_yaml_iter_recurse(&fd_iter, &conn_array);
                                do {
                                    const char *identity = NULL;
                                    const char *addr = NULL;
                                    uint16_t port = 0;

                                    if (ogs_yaml_iter_type(&conn_array) ==
                                        YAML_MAPPING_NODE) {
                                        memcpy(&conn_iter, &conn_array,
                                                sizeof(ogs_yaml_iter_t));
                                    } else if (ogs_yaml_iter_type(
                                        &conn_array) == YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&conn_array))
                                            break;
                                        ogs_yaml_iter_recurse(&conn_array,
                                                &conn_iter);
                                    } else if (ogs_yaml_iter_type(
                                            &conn_array) == YAML_SCALAR_NODE) {
                                        break;
                                    } else
                                        ogs_assert_if_reached();

                                    while (ogs_yaml_iter_next(&conn_iter)) {
                                        const char *conn_key =
                                            ogs_yaml_iter_key(&conn_iter);
                                        ogs_assert(conn_key);
                                        if (!strcmp(conn_key, "identity")) {
                                            identity = 
                                                ogs_yaml_iter_value(&conn_iter);
                                        } else if (!strcmp(conn_key, "addr")) {
                                            addr =
                                                ogs_yaml_iter_value(&conn_iter);
                                        } else if (!strcmp(conn_key, "port")) {
                                            const char *v =
                                                ogs_yaml_iter_value(&conn_iter);
                                            if (v) port = atoi(v);
                                        } else
                                            ogs_warn("unknown key `%s`",
                                                    conn_key);
                                    }

                                    if (identity && addr) {
                                        self.diam_config->
                                            conn[self.diam_config->num_of_conn].
                                                identity = identity;
                                        self.diam_config->
                                            conn[self.diam_config->num_of_conn].
                                                addr = addr;
                                        self.diam_config->
                                            conn[self.diam_config->num_of_conn].
                                                port = port;
                                        self.diam_config->num_of_conn++;
                                    }
                                } while (ogs_yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", fd_key);
                        }
                    }
                } else if (!strcmp(smf_key, "gtpc")) {
                    ogs_yaml_iter_t gtpc_array, gtpc_iter;
                    ogs_yaml_iter_recurse(&smf_iter, &gtpc_array);
                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpc_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;

                        if (ogs_yaml_iter_type(&gtpc_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&gtpc_iter, &gtpc_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&gtpc_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&gtpc_array))
                                break;
                            ogs_yaml_iter_recurse(&gtpc_array, &gtpc_iter);
                        } else if (ogs_yaml_iter_type(&gtpc_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&gtpc_iter)) {
                            const char *gtpc_key =
                                ogs_yaml_iter_key(&gtpc_iter);
                            ogs_assert(gtpc_key);
                            if (!strcmp(gtpc_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&gtpc_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(gtpc_key, "addr") ||
                                    !strcmp(gtpc_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&gtpc_iter,
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
                            } else if (!strcmp(gtpc_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&gtpc_iter);
                                if (v) {
                                    port = atoi(v);
                                    self.gtpc_port = port;
                                }
                            } else if (!strcmp(gtpc_key, "dev")) {
                                dev = ogs_yaml_iter_value(&gtpc_iter);
                            } else if (!strcmp(gtpc_key, "apn")) {
                                /* Skip */
                            } else
                                ogs_warn("unknown key `%s`", gtpc_key);
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
                                        &self.gtpc_list, AF_INET, addr);
                            if (ogs_config()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(
                                        &self.gtpc_list6, AF_INET6, addr);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                    ogs_config()->parameter.no_ipv4 ?
                                        NULL : &self.gtpc_list,
                                    ogs_config()->parameter.no_ipv6 ?
                                        NULL : &self.gtpc_list6,
                                    dev, self.gtpc_port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while (ogs_yaml_iter_type(&gtpc_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.gtpc_list) == NULL &&
                        ogs_list_first(&self.gtpc_list6) == NULL) {
                        rv = ogs_socknode_probe(
                                ogs_config()->parameter.no_ipv4 ?
                                    NULL : &self.gtpc_list,
                                ogs_config()->parameter.no_ipv6 ?
                                    NULL : &self.gtpc_list6,
                                NULL, self.gtpc_port);
                        ogs_assert(rv == OGS_OK);
                    }
                } else if (!strcmp(smf_key, "pfcp")) {
                    /* handle config in pfcp library */
                } else if (!strcmp(smf_key, "pdn")) {
                    /* handle config in pfcp library */
                } else if (!strcmp(smf_key, "dns")) {
                    ogs_yaml_iter_t dns_iter;
                    ogs_yaml_iter_recurse(&smf_iter, &dns_iter);
                    ogs_assert(ogs_yaml_iter_type(&dns_iter) !=
                        YAML_MAPPING_NODE);

                    do {
                        const char *v = NULL;

                        if (ogs_yaml_iter_type(&dns_iter) ==
                                YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&dns_iter))
                                break;
                        }

                        v = ogs_yaml_iter_value(&dns_iter);
                        if (v && strlen(v)) {
                            ogs_ipsubnet_t ipsub;
                            rv = ogs_ipsubnet(&ipsub, v, NULL);
                            ogs_assert(rv == OGS_OK);

                            if (ipsub.family == AF_INET) {
                                if (self.dns[0] && self.dns[1])
                                    ogs_warn("Ignore DNS : %s", v);
                                else if (self.dns[0]) self.dns[1] = v;
                                else self.dns[0] = v;
                            }
                            else if (ipsub.family == AF_INET6) {
                                if (self.dns6[0] && self.dns6[1])
                                    ogs_warn("Ignore DNS : %s", v);
                                else if (self.dns6[0]) self.dns6[1] = v;
                                else self.dns6[0] = v;
                            } else
                                ogs_warn("Ignore DNS : %s", v);
                        }

                    } while (
                        ogs_yaml_iter_type(&dns_iter) ==
                            YAML_SEQUENCE_NODE);
                } else if (!strcmp(smf_key, "mtu")) {
                    ogs_assert(ogs_yaml_iter_type(&smf_iter) !=
                            YAML_SCALAR_NODE);
                    self.mtu = atoi(ogs_yaml_iter_value(&smf_iter));
                    ogs_assert(self.mtu);
                } else if (!strcmp(smf_key, "p-cscf")) {
                    ogs_yaml_iter_t dns_iter;
                    ogs_yaml_iter_recurse(&smf_iter, &dns_iter);
                    ogs_assert(ogs_yaml_iter_type(&dns_iter) !=
                        YAML_MAPPING_NODE);

                    self.num_of_p_cscf = 0;
                    self.num_of_p_cscf6 = 0;
                    do {
                        const char *v = NULL;

                        if (ogs_yaml_iter_type(&dns_iter) ==
                                YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&dns_iter))
                                break;
                        }

                        v = ogs_yaml_iter_value(&dns_iter);
                        if (v) {
                            ogs_ipsubnet_t ipsub;
                            rv = ogs_ipsubnet(&ipsub, v, NULL);
                            ogs_assert(rv == OGS_OK);

                            if (ipsub.family == AF_INET) {
                                if (self.num_of_p_cscf >= MAX_NUM_OF_P_CSCF)
                                    ogs_warn("Ignore P-CSCF : %s", v);
                                else self.p_cscf[self.num_of_p_cscf++] = v;
                            }
                            else if (ipsub.family == AF_INET6) {
                                if (self.num_of_p_cscf6 >= MAX_NUM_OF_P_CSCF)
                                    ogs_warn("Ignore P-CSCF : %s", v);
                                else self.p_cscf6[self.num_of_p_cscf6++] = v;
                            } else
                                ogs_warn("Ignore P-CSCF : %s", v);
                        }

                    } while (
                        ogs_yaml_iter_type(&dns_iter) ==
                            YAML_SEQUENCE_NODE);
                }
                else
                    ogs_warn("unknown key `%s`", smf_key);
            }
        }
    }

    rv = smf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

static void *sess_hash_keygen(uint8_t *out, int *out_len,
        uint8_t *imsi, int imsi_len, char *apn)
{
    memcpy(out, imsi, imsi_len);
    ogs_cpystrn((char*)(out+imsi_len), apn, OGS_MAX_APN_LEN+1);
    *out_len = imsi_len+strlen((char*)(out+imsi_len));

    return out;
}

smf_sess_t *smf_sess_add(
        uint8_t *imsi, int imsi_len, char *apn, 
        uint8_t pdn_type, uint8_t ebi, ogs_paa_t *paa)
{
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];
    smf_sess_t *sess = NULL;
    smf_bearer_t *bearer = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_subnet_t *subnet6 = NULL;

    ogs_assert(imsi);
    ogs_assert(apn);
    ogs_assert(paa);

    ogs_pool_alloc(&smf_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    sess->index = ogs_pool_index(&smf_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_config()->pool.sess);

    /* Set TEID & SEID */
    sess->smf_n4_teid = sess->index;
    sess->smf_n4_seid = sess->index;

    /* Set IMSI */
    sess->imsi_len = imsi_len;
    memcpy(sess->imsi, imsi, sess->imsi_len);
    ogs_buffer_to_bcd(sess->imsi, sess->imsi_len, sess->imsi_bcd);

    /* Set APN */
    ogs_cpystrn(sess->pdn.apn, apn, OGS_MAX_APN_LEN+1);

    /* UE IP Address */
    sess->pdn.paa.pdn_type = pdn_type;
    ogs_expect(pdn_type == paa->pdn_type);

    if (pdn_type == OGS_GTP_PDN_TYPE_IPV4) {
        sess->ipv4 = ogs_pfcp_ue_ip_alloc(
                AF_INET, apn, (uint8_t *)&(paa->addr));
        ogs_assert(sess->ipv4);
        sess->pdn.paa.addr = sess->ipv4->addr[0];
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
    } else if (pdn_type == OGS_GTP_PDN_TYPE_IPV6) {
        sess->ipv6 = ogs_pfcp_ue_ip_alloc(
                AF_INET6, apn, (paa->addr6));
        ogs_assert(sess->ipv6);

        subnet6 = sess->ipv6->subnet;
        ogs_assert(subnet6);

        sess->pdn.paa.len = subnet6->prefixlen;
        memcpy(sess->pdn.paa.addr6, sess->ipv6->addr, OGS_IPV6_LEN);
        ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, sess);
    } else if (pdn_type == OGS_GTP_PDN_TYPE_IPV4V6) {
        sess->ipv4 = ogs_pfcp_ue_ip_alloc(
                AF_INET, apn, (uint8_t *)&(paa->both.addr));
        ogs_assert(sess->ipv4);
        sess->ipv6 = ogs_pfcp_ue_ip_alloc(
                AF_INET6, apn, (paa->both.addr6));
        ogs_assert(sess->ipv6);

        subnet6 = sess->ipv6->subnet;
        ogs_assert(subnet6);

        sess->pdn.paa.both.addr = sess->ipv4->addr[0];
        sess->pdn.paa.both.len = subnet6->prefixlen;
        memcpy(sess->pdn.paa.both.addr6, sess->ipv6->addr, OGS_IPV6_LEN);
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
        ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, sess);
    } else
        ogs_assert_if_reached();

    ogs_info("UE IMSI:[%s] APN:[%s] IPv4:[%s] IPv6:[%s]",
	    sess->imsi_bcd, apn,
        sess->ipv4 ? INET_NTOP(&sess->ipv4->addr, buf1) : "",
        sess->ipv6 ? INET6_NTOP(&sess->ipv6->addr, buf2) : "");

    /* Generate Hash Key : IMSI + APN */
    sess_hash_keygen(sess->hash_keybuf, &sess->hash_keylen,
            imsi, imsi_len, apn);
    ogs_hash_set(self.sess_hash, sess->hash_keybuf, sess->hash_keylen, sess);

    /* Select UPF with round-robin manner */
    if (ogs_pfcp_self()->node == NULL)
        ogs_pfcp_self()->node = ogs_list_first(&ogs_pfcp_self()->n4_list);

    for (; ogs_pfcp_self()->node;
        ogs_pfcp_self()->node = ogs_list_next(ogs_pfcp_self()->node)) {
        if (OGS_FSM_CHECK(
                &ogs_pfcp_self()->node->sm, smf_pfcp_state_associated)) {
            OGS_SETUP_PFCP_NODE(sess, ogs_pfcp_self()->node);
            break;
        }
    }

    /* Set Default Bearer */
    ogs_list_init(&sess->bearer_list);

    bearer = smf_bearer_add(sess);
    ogs_assert(bearer);

    bearer->ebi = ebi;

    /* Default PDRs is set to lowest precedence(highest precedence value). */
    ogs_list_for_each(&bearer->pfcp.pdr_list, pdr)
        pdr->precedence = 0xffffffff;

    ogs_list_add(&self.sess_list, sess);
    
    stats_add_session();

    return sess;
}

int smf_sess_remove(smf_sess_t *sess)
{
    int i;

    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);

    OGS_TLV_CLEAR_DATA(&sess->ue_pco);
    OGS_TLV_CLEAR_DATA(&sess->user_location_information);
    OGS_TLV_CLEAR_DATA(&sess->ue_timezone);

    for (i = 0; i < sess->num_of_pcc_rule; i++)
        OGS_PCC_RULE_FREE(&sess->pcc_rule[i]);
    sess->num_of_pcc_rule = 0;

    ogs_hash_set(self.sess_hash, sess->hash_keybuf, sess->hash_keylen, NULL);

    if (sess->ipv4) {
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv4);
    }
    if (sess->ipv6) {
        ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv6);
    }

    smf_bearer_remove_all(sess);

    ogs_pool_free(&smf_sess_pool, sess);

    stats_remove_session();

    return OGS_OK;
}

void smf_sess_remove_all(void)
{
    smf_sess_t *sess = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.sess_list, next, sess)
        smf_sess_remove(sess);
}

smf_sess_t *smf_sess_find(uint32_t index)
{
    ogs_assert(index);
    return ogs_pool_find(&smf_sess_pool, index);
}

smf_sess_t *smf_sess_find_by_teid(uint32_t teid)
{
    return smf_sess_find(teid);
}

smf_sess_t *smf_sess_find_by_seid(uint64_t seid)
{
    return smf_sess_find(seid);
}

smf_sess_t *smf_sess_find_by_imsi_apn(
    uint8_t *imsi, int imsi_len, char *apn)
{
    uint8_t keybuf[OGS_MAX_IMSI_LEN+OGS_MAX_APN_LEN+1];
    int keylen = 0;

    ogs_assert(self.sess_hash);

    sess_hash_keygen(keybuf, &keylen, imsi, imsi_len, apn);
    return (smf_sess_t *)ogs_hash_get(self.sess_hash, keybuf, keylen);
}

smf_sess_t *smf_sess_find_by_ipv4(uint32_t addr)
{
    ogs_assert(self.ipv4_hash);
    return (smf_sess_t *)ogs_hash_get(self.ipv4_hash, &addr, OGS_IPV4_LEN);
}

smf_sess_t *smf_sess_find_by_ipv6(uint32_t *addr6)
{
    ogs_assert(self.ipv6_hash);
    ogs_assert(addr6);
    return (smf_sess_t *)ogs_hash_get(self.ipv6_hash, addr6, OGS_IPV6_LEN);
}

smf_sess_t *smf_sess_add_by_message(ogs_gtp_message_t *message)
{
    smf_sess_t *sess = NULL;
    ogs_paa_t *paa = NULL;
    char apn[OGS_MAX_APN_LEN];

    ogs_gtp_create_session_request_t *req = &message->create_session_request;

    if (req->imsi.presence == 0) {
        ogs_error("No IMSI");
        return NULL;
    }
    if (req->access_point_name.presence == 0) {
        ogs_error("No APN");
        return NULL;
    }
    if (req->bearer_contexts_to_be_created.presence == 0) {
        ogs_error("No Bearer");
        return NULL;
    }
    if (req->bearer_contexts_to_be_created.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        return NULL;
    }
    if (req->pdn_type.presence == 0) {
        ogs_error("No PDN Type");
        return NULL;
    }

    if (req->pdn_address_allocation.presence == 0) {
        ogs_error("No PAA Type");
        return NULL;
    }

    ogs_fqdn_parse(apn,
            req->access_point_name.data, req->access_point_name.len);

    ogs_trace("smf_sess_add_by_message() [APN:%s, PDN:%d, EDI:%d]",
            apn, req->pdn_type.u8,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);

    paa = (ogs_paa_t *)req->pdn_address_allocation.data;

    /* 
     * 7.2.1 in 3GPP TS 29.274 Release 15
     *
     * If the new Create Session Request received by the SMF collides with
     * an existing PDN connection context (the existing PDN connection context
     * is identified with the triplet [IMSI, EPS Bearer ID, Interface type],
     * where applicable Interface type here is S2a TWAN GTP-C interface or
     * S2b ePDG GTP-C interface or S5/S8 SGW GTP-C interface, and where IMSI
     * shall be replaced by TAC and SNR part of ME Identity for emergency
     * attached UE without UICC or authenticated IMSI), this Create Session
     * Request shall be treated as a request for a new session. Before creating
     * the new session, the SMF should delete:
     *
     * - the existing PDN connection context, if the Create Session Request
     *   collides with the default bearer of an existing PDN connection context;
     * - the existing dedicated bearer context, if the Create Session Request
     *   collides with a dedicated bearer of an existing PDN connection context.
     */
    sess = smf_sess_find_by_imsi_apn(req->imsi.data, req->imsi.len, apn);
    if (sess) {
        ogs_warn("OLD Session Release [IMSI:%s,APN:%s]",
                sess->imsi_bcd, sess->pdn.apn);
        smf_sess_remove(sess);
    }
    sess = smf_sess_add(req->imsi.data, req->imsi.len, apn,
                    req->pdn_type.u8,
                    req->bearer_contexts_to_be_created.eps_bearer_id.u8, paa);
    return sess;
}

smf_bearer_t *smf_bearer_add(smf_sess_t *sess)
{
    smf_bearer_t *bearer = NULL;
    ogs_pfcp_gtpu_resource_t *resource = NULL;

    ogs_pfcp_pdr_t *dl_pdr = NULL;
    ogs_pfcp_pdr_t *ul_pdr = NULL;
    ogs_pfcp_far_t *dl_far = NULL;
    ogs_pfcp_far_t *ul_far = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&smf_bearer_pool, &bearer);
    ogs_assert(bearer);
    memset(bearer, 0, sizeof *bearer);

    bearer->index = ogs_pool_index(&smf_bearer_pool, bearer);
    ogs_assert(bearer->index > 0 && bearer->index <=
            ogs_config()->pool.bearer);

    ogs_list_init(&bearer->pf_list);

    dl_pdr = ogs_pfcp_pdr_add(&bearer->pfcp);
    ogs_assert(dl_pdr);
    dl_pdr->id = OGS_NEXT_ID(sess->pdr_id, 1, OGS_MAX_NUM_OF_PDR+1);
    dl_pdr->src_if = OGS_PFCP_INTERFACE_CORE;

    ul_pdr = ogs_pfcp_pdr_add(&bearer->pfcp);
    ogs_assert(ul_pdr);
    ul_pdr->id = OGS_NEXT_ID(sess->pdr_id, 1, OGS_MAX_NUM_OF_PDR+1);
    ul_pdr->src_if = OGS_PFCP_INTERFACE_ACCESS;

    dl_far = ogs_pfcp_far_add(&bearer->pfcp);
    ogs_assert(dl_far);
    dl_far->id = OGS_NEXT_ID(sess->far_id, 1, OGS_MAX_NUM_OF_FAR+1);
    dl_far->dst_if = OGS_PFCP_INTERFACE_ACCESS;
    ogs_pfcp_pdr_associate_far(dl_pdr, dl_far);

    ul_far = ogs_pfcp_far_add(&bearer->pfcp);
    ogs_assert(ul_far);
    ul_far->id = OGS_NEXT_ID(sess->far_id, 1, OGS_MAX_NUM_OF_FAR+1);
    ul_far->dst_if = OGS_PFCP_INTERFACE_CORE;
    ogs_pfcp_pdr_associate_far(ul_pdr, ul_far);

    resource = ogs_pfcp_gtpu_resource_find(
            &sess->pfcp_node->gtpu_resource_list,
            sess->pdn.apn, OGS_PFCP_INTERFACE_ACCESS);
    if (resource) {
        ogs_pfcp_user_plane_ip_resource_info_to_sockaddr(&resource->info,
            &bearer->upf_addr, &bearer->upf_addr6);
        ogs_assert(bearer->upf_addr || bearer->upf_addr6);
        if (resource->info.teidri)
            bearer->upf_n3_teid = UPF_S5U_INDEX_TO_TEID(
                    bearer->index, resource->info.teidri,
                    resource->info.teid_range);
        else
            bearer->upf_n3_teid = bearer->index;
    } else {
        if (sess->pfcp_node->addr.ogs_sa_family == AF_INET)
            ogs_copyaddrinfo(&bearer->upf_addr, &sess->pfcp_node->addr);
        else if (sess->pfcp_node->addr.ogs_sa_family == AF_INET6)
            ogs_copyaddrinfo(&bearer->upf_addr6, &sess->pfcp_node->addr);
        else
            ogs_assert_if_reached();
        ogs_assert(bearer->upf_addr || bearer->upf_addr6);

        bearer->upf_n3_teid = bearer->index;
    }

    bearer->sess = sess;

    ogs_list_add(&sess->bearer_list, bearer);

    return bearer;
}

int smf_bearer_remove(smf_bearer_t *bearer)
{
    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    ogs_list_remove(&bearer->sess->bearer_list, bearer);
    ogs_pfcp_sess_clear(&bearer->pfcp);

    if (bearer->name)
        ogs_free(bearer->name);
    if (bearer->upf_addr)
        ogs_freeaddrinfo(bearer->upf_addr);
    if (bearer->upf_addr6)
        ogs_freeaddrinfo(bearer->upf_addr6);

    smf_pf_remove_all(bearer);

    ogs_pool_free(&smf_bearer_pool, bearer);

    return OGS_OK;
}

void smf_bearer_remove_all(smf_sess_t *sess)
{
    smf_bearer_t *bearer = NULL, *next_bearer = NULL;

    ogs_assert(sess);
    ogs_list_for_each_safe(&sess->bearer_list, next_bearer, bearer)
        smf_bearer_remove(bearer);
}

smf_bearer_t *smf_bearer_find(uint32_t index)
{
    ogs_assert(index);
    return ogs_pool_find(&smf_bearer_pool, index);
}

smf_bearer_t *smf_bearer_find_by_smf_s5u_teid(uint32_t smf_s5u_teid)
{
    return smf_bearer_find(smf_s5u_teid);
}

smf_bearer_t *smf_bearer_find_by_ebi(smf_sess_t *sess, uint8_t ebi)
{
    smf_bearer_t *bearer = NULL;
    
    ogs_assert(sess);

    bearer = smf_bearer_first(sess);
    while (bearer) {
        if (bearer->ebi == ebi)
            break;

        bearer = smf_bearer_next(bearer);
    }

    return bearer;
}

smf_bearer_t *smf_bearer_find_by_name(smf_sess_t *sess, char *name)
{
    smf_bearer_t *bearer = NULL;
    
    ogs_assert(sess);
    ogs_assert(name);

    bearer = smf_bearer_first(sess);
    while (bearer) {
        if (bearer->name && strcmp(bearer->name, name) == 0)
            return bearer;

        bearer = smf_bearer_next(bearer);
    }

    return NULL;
}

smf_bearer_t *smf_bearer_find_by_qci_arp(smf_sess_t *sess, 
                                uint8_t qci,
                                uint8_t priority_level,
                                uint8_t pre_emption_capability,
                                uint8_t pre_emption_vulnerability)
{
    smf_bearer_t *bearer = NULL;

    ogs_assert(sess);

    bearer = smf_default_bearer_in_sess(sess);
    if (!bearer) return NULL;

    if (sess->pdn.qos.qci == qci &&
        sess->pdn.qos.arp.priority_level == priority_level &&
        sess->pdn.qos.arp.pre_emption_capability == 
            pre_emption_capability &&
        sess->pdn.qos.arp.pre_emption_vulnerability == 
            pre_emption_vulnerability) {
        return bearer;
    }

    bearer = smf_bearer_next(bearer);
    while (bearer) {
        if (bearer->qos.qci == qci &&
            bearer->qos.arp.priority_level == priority_level &&
            bearer->qos.arp.pre_emption_capability == 
                pre_emption_capability &&
            bearer->qos.arp.pre_emption_vulnerability == 
                pre_emption_vulnerability) {
            return bearer;
        }
        bearer = smf_bearer_next(bearer);
    }

    return NULL;
}

smf_bearer_t *smf_default_bearer_in_sess(smf_sess_t *sess)
{
    return smf_bearer_first(sess);
}

bool smf_bearer_is_default(smf_bearer_t *bearer)
{
    smf_sess_t *sess = NULL;
    smf_bearer_t *default_bearer = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    default_bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(default_bearer);

    return bearer->ebi == default_bearer->ebi;
}

smf_bearer_t *smf_bearer_first(smf_sess_t *sess)
{
    ogs_assert(sess);
    return ogs_list_first(&sess->bearer_list);
}

smf_bearer_t *smf_bearer_next(smf_bearer_t *bearer)
{
    return ogs_list_next(bearer);
}

smf_pf_t *smf_pf_add(smf_bearer_t *bearer, uint32_t precedence)
{
    smf_pf_t *pf = NULL;

    ogs_assert(bearer);

    ogs_pool_alloc(&smf_pf_pool, &pf);
    ogs_assert(pf);
    memset(pf, 0, sizeof *pf);

    pf->identifier = OGS_NEXT_ID(bearer->pf_identifier, 1, 15);
    pf->bearer = bearer;

    ogs_list_add(&bearer->pf_list, pf);

    return pf;
}

int smf_pf_remove(smf_pf_t *pf)
{
    ogs_assert(pf);
    ogs_assert(pf->bearer);

    ogs_list_remove(&pf->bearer->pf_list, pf);
    ogs_pool_free(&smf_pf_pool, pf);

    return OGS_OK;
}

void smf_pf_remove_all(smf_bearer_t *bearer)
{
    smf_pf_t *pf = NULL, *next_pf = NULL;

    ogs_assert(bearer);
    ogs_list_for_each_safe(&bearer->pf_list, next_pf, pf)
        smf_pf_remove(pf);
}

smf_pf_t *smf_pf_find_by_id(smf_bearer_t *bearer, uint8_t id)
{
    smf_pf_t *pf = NULL;
    
    ogs_list_for_each(&bearer->pf_list, pf) {
        if (pf->identifier == id) return pf;
    }

    return NULL;
}

smf_pf_t *smf_pf_first(smf_bearer_t *bearer)
{
    return ogs_list_first(&bearer->pf_list);
}

smf_pf_t *smf_pf_next(smf_pf_t *pf)
{
    return ogs_list_next(pf);
}
