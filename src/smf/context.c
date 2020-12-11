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

static smf_context_t self;
static ogs_diam_config_t g_diam_conf;

int __smf_log_domain;
int __gsm_log_domain;

static OGS_POOL(smf_ue_pool, smf_ue_t);
static OGS_POOL(smf_sess_pool, smf_sess_t);
static OGS_POOL(smf_bearer_pool, smf_bearer_t);

static OGS_POOL(smf_pf_pool, smf_pf_t);

static int context_initialized = 0;

static int num_of_smf_sess = 0;

static void stats_add_smf_session(void);
static void stats_remove_smf_session(void);

void smf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initial FreeDiameter Config */
    memset(&g_diam_conf, 0, sizeof(ogs_diam_config_t));

    /* Initialize SMF context */
    memset(&self, 0, sizeof(smf_context_t));
    self.diam_config = &g_diam_conf;

    ogs_log_install_domain(&__ogs_ngap_domain, "ngap", ogs_core()->log.level);
    ogs_log_install_domain(&__ogs_nas_domain, "nas", ogs_core()->log.level);
    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", ogs_core()->log.level);
    ogs_log_install_domain(&__ogs_diam_domain, "diam", ogs_core()->log.level);
    ogs_log_install_domain(&__smf_log_domain, "smf", ogs_core()->log.level);
    ogs_log_install_domain(&__gsm_log_domain, "gsm", ogs_core()->log.level);

    ogs_gtp_node_init();

    ogs_list_init(&self.smf_ue_list);

    ogs_list_init(&self.gtpc_list);
    ogs_list_init(&self.gtpc_list6);

    ogs_list_init(&self.sgw_s5c_list);

    ogs_pool_init(&smf_ue_pool, ogs_app()->max.ue);
    ogs_pool_init(&smf_sess_pool, ogs_app()->pool.sess);
    ogs_pool_init(&smf_bearer_pool, ogs_app()->pool.bearer);

    ogs_pool_init(&smf_pf_pool, ogs_app()->pool.bearer * OGS_MAX_NUM_OF_PF);

    self.supi_hash = ogs_hash_make();
    self.imsi_hash = ogs_hash_make();
    self.ipv4_hash = ogs_hash_make();
    self.ipv6_hash = ogs_hash_make();

    context_initialized = 1;
}

void smf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    smf_ue_remove_all();

    ogs_assert(self.supi_hash);
    ogs_hash_destroy(self.supi_hash);
    ogs_assert(self.imsi_hash);
    ogs_hash_destroy(self.imsi_hash);
    ogs_assert(self.ipv4_hash);
    ogs_hash_destroy(self.ipv4_hash);
    ogs_assert(self.ipv6_hash);
    ogs_hash_destroy(self.ipv6_hash);

    ogs_pool_final(&smf_ue_pool);
    ogs_pool_final(&smf_bearer_pool);
    ogs_pool_final(&smf_sess_pool);

    ogs_pool_final(&smf_pf_pool);

    ogs_gtp_node_remove_all(&self.sgw_s5c_list);

    ogs_gtp_node_final();

    context_initialized = 0;
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
    self.nf_type = OpenAPI_nf_type_SMF;

    return OGS_OK;
}

static int smf_context_validation(void)
{
    if (self.dns[0] == NULL && self.dns6[0] == NULL) {
        ogs_error("No smf.dns in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    if (ogs_list_first(&ogs_pfcp_self()->subnet_list) == NULL) {
        ogs_error("No smf.pdn: in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int smf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
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
                                if (v) port = atoi(v);
                            } else if (!strcmp(gtpc_key, "dev")) {
                                dev = ogs_yaml_iter_value(&gtpc_iter);
                            } else if (!strcmp(gtpc_key, "apn") ||
                                        !strcmp(gtpc_key, "dnn")) {
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
                            if (ogs_app()->parameter.no_ipv4 == 0)
                                ogs_socknode_add(
                                        &self.gtpc_list, AF_INET, addr);
                            if (ogs_app()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(
                                        &self.gtpc_list6, AF_INET6, addr);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                    ogs_app()->parameter.no_ipv4 ?
                                        NULL : &self.gtpc_list,
                                    ogs_app()->parameter.no_ipv6 ?
                                        NULL : &self.gtpc_list6,
                                    dev, port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while (ogs_yaml_iter_type(&gtpc_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.gtpc_list) == NULL &&
                        ogs_list_first(&self.gtpc_list6) == NULL) {
                        rv = ogs_socknode_probe(
                                ogs_app()->parameter.no_ipv4 ?
                                    NULL : &self.gtpc_list,
                                ogs_app()->parameter.no_ipv6 ?
                                    NULL : &self.gtpc_list6,
                                NULL, self.gtpc_port);
                        ogs_assert(rv == OGS_OK);
                    }
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
                } else if (!strcmp(smf_key, "pfcp")) {
                    /* handle config in pfcp library */
                } else if (!strcmp(smf_key, "pdn")) {
                    /* handle config in pfcp library */
                } else if (!strcmp(smf_key, "sbi")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", smf_key);
            }
        }
    }

    rv = smf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

smf_ue_t *smf_ue_add_by_supi(char *supi)
{
    smf_ue_t *smf_ue = NULL;

    ogs_assert(supi);

    ogs_pool_alloc(&smf_ue_pool, &smf_ue);
    ogs_assert(smf_ue);
    memset(smf_ue, 0, sizeof *smf_ue);

    ogs_list_init(&smf_ue->sess_list);

    smf_ue->supi = ogs_strdup(supi);
    ogs_assert(smf_ue->supi);
    ogs_hash_set(self.supi_hash, smf_ue->supi, strlen(smf_ue->supi), smf_ue);

    ogs_list_add(&self.smf_ue_list, smf_ue);

    ogs_info("[Added] Number of SMF-UEs is now %d",
            ogs_list_count(&self.smf_ue_list));

    return smf_ue;
}

smf_ue_t *smf_ue_add_by_imsi(uint8_t *imsi, int imsi_len)
{
    smf_ue_t *smf_ue = NULL;

    ogs_assert(imsi);
    ogs_assert(imsi_len);

    ogs_pool_alloc(&smf_ue_pool, &smf_ue);
    ogs_assert(smf_ue);
    memset(smf_ue, 0, sizeof *smf_ue);

    ogs_list_init(&smf_ue->sess_list);

    smf_ue->imsi_len = imsi_len;
    memcpy(smf_ue->imsi, imsi, smf_ue->imsi_len);
    ogs_buffer_to_bcd(smf_ue->imsi, smf_ue->imsi_len, smf_ue->imsi_bcd);
    ogs_hash_set(self.imsi_hash, smf_ue->imsi, smf_ue->imsi_len, smf_ue);

    ogs_list_add(&self.smf_ue_list, smf_ue);

    ogs_info("[Added] Number of SMF-UEs is now %d",
            ogs_list_count(&self.smf_ue_list));

    return smf_ue;
}

void smf_ue_remove(smf_ue_t *smf_ue)
{
    ogs_assert(smf_ue);

    ogs_list_remove(&self.smf_ue_list, smf_ue);

    if (smf_ue->supi) {
        ogs_hash_set(self.supi_hash, smf_ue->supi, strlen(smf_ue->supi), NULL);
        ogs_free(smf_ue->supi);
    }

    if (smf_ue->imsi_len) {
        ogs_hash_set(self.imsi_hash, smf_ue->imsi, smf_ue->imsi_len, NULL);
    }

    smf_sess_remove_all(smf_ue);

    ogs_pool_free(&smf_ue_pool, smf_ue);

    ogs_info("[Removed] Number of SMF-UEs is now %d",
            ogs_list_count(&self.smf_ue_list));
}

void smf_ue_remove_all(void)
{
    smf_ue_t *smf_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.smf_ue_list, next, smf_ue)
        smf_ue_remove(smf_ue);
}

smf_ue_t *smf_ue_find_by_supi(char *supi)
{
    ogs_assert(supi);
    return (smf_ue_t *)ogs_hash_get(self.supi_hash, supi, strlen(supi));
}

smf_ue_t *smf_ue_find_by_imsi(uint8_t *imsi, int imsi_len)
{
    ogs_assert(imsi);
    ogs_assert(imsi_len);
    return (smf_ue_t *)ogs_hash_get(self.imsi_hash, imsi, imsi_len);
}

static bool compare_ue_info(ogs_pfcp_node_t *node, smf_sess_t *sess)
{
    int i;

    ogs_assert(node);
    ogs_assert(sess);

    for (i = 0; i < node->num_of_dnn; i++)
        if (ogs_strcasecmp(node->dnn[i], sess->pdn.dnn) == 0) return true;

    for (i = 0; i < node->num_of_e_cell_id; i++)
        if (node->e_cell_id[i] == sess->e_cgi.cell_id) return true;

    for (i = 0; i < node->num_of_nr_cell_id; i++)
        if (node->nr_cell_id[i] == sess->nr_cgi.cell_id) return true;

    for (i = 0; i < node->num_of_tac; i++)
        if ((node->tac[i] == sess->e_tai.tac) ||
            (node->tac[i] == sess->nr_tai.tac.v)) return true;

    return false;
}

static ogs_pfcp_node_t *selected_upf_node(
        ogs_pfcp_node_t *current, smf_sess_t *sess)
{
    ogs_pfcp_node_t *next, *node;

    ogs_assert(current);
    ogs_assert(sess);

    int RR = 0, selected = 0;

    while (!selected) {
        /* continue search from current position */
        next = ogs_list_next(current);
        for (node = next; node; node = ogs_list_next(node)) {
            if (!RR) {
                if (OGS_FSM_CHECK(&node->sm, smf_pfcp_state_associated) &&
                    compare_ue_info(node, sess) == true) return node;
            } else {
                /*
                 * we are in RR mode - use next PFCP associated
                 * node that is suited for full list RR
                 */
                if (OGS_FSM_CHECK(&node->sm, smf_pfcp_state_associated) &&
                    node->rr_enable == 1) return node;
            }
        }
        /* cyclic search from top to current position */
        for (node = ogs_list_first(&ogs_pfcp_self()->peer_list);
                node != next; node = ogs_list_next(node)) {
            if (!RR) {
                if (OGS_FSM_CHECK(&node->sm, smf_pfcp_state_associated) &&
                    compare_ue_info(node, sess) == true) return node;
            } else {
                /*
                 * we are in RR mode - use next PFCP associated
                 * node that is suited for full list RR
                 */
                if (OGS_FSM_CHECK(&node->sm, smf_pfcp_state_associated) &&
                    node->rr_enable == 1) return node;
            }
        }

        /* if a round robin search has already been carried out */
        if (RR) break;

        /*
         * re-run search in round robin mode,
         * find and use next PFCP associated node
         */
        RR = 1;
    }

    ogs_error("No UPFs are PFCP associated that are suited to RR");
    return ogs_list_first(&ogs_pfcp_self()->peer_list);
}

void smf_sess_select_upf(smf_sess_t *sess)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(sess);

    /*
     * When used for the first time, if last node is set,
     * the search is performed from the first UPF in a round-robin manner.
     */
    if (ogs_pfcp_self()->node == NULL)
        ogs_pfcp_self()->node = ogs_list_last(&ogs_pfcp_self()->peer_list);

    /* setup GTP session with selected UPF */
    ogs_pfcp_self()->node = selected_upf_node(ogs_pfcp_self()->node, sess);
    ogs_assert(ogs_pfcp_self()->node);
    OGS_SETUP_PFCP_NODE(sess, ogs_pfcp_self()->node);
    ogs_debug("UE using UPF on IP[%s]",
            OGS_ADDR(&ogs_pfcp_self()->node->addr, buf));
}

smf_sess_t *smf_sess_add_by_apn(smf_ue_t *smf_ue, char *apn)
{
    smf_event_t e;

    smf_sess_t *sess = NULL;

    ogs_assert(smf_ue);
    ogs_assert(apn);

    ogs_pool_alloc(&smf_sess_pool, &sess);
    if (!sess) {
        ogs_error("Maximum number of session[%lld] reached",
                    (long long)ogs_app()->pool.sess);
        return NULL;
    }
    memset(sess, 0, sizeof *sess);

    ogs_pfcp_pool_init(&sess->pfcp);
    smf_qfi_pool_init(sess);

    sess->index = ogs_pool_index(&smf_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_app()->pool.sess);

    /* Set TEID & SEID */
    sess->smf_n4_teid = sess->index;
    sess->smf_n4_seid = sess->index;

    /* Create BAR in PFCP Session */
    ogs_pfcp_bar_new(&sess->pfcp);

    /* Set APN */
    ogs_cpystrn(sess->pdn.apn, apn, OGS_MAX_APN_LEN+1);

    /* Setup Timer */
    sess->t_release_holding = ogs_timer_add(
            ogs_app()->timer_mgr, smf_timer_release_holding_expire, sess);

    memset(&e, 0, sizeof(e));
    e.sess = sess;
    ogs_fsm_create(&sess->sm, smf_gsm_state_initial, smf_gsm_state_final);
    ogs_fsm_init(&sess->sm, &e);

    sess->smf_ue = smf_ue;

    ogs_list_add(&smf_ue->sess_list, sess);

    stats_add_smf_session();

    return sess;
}

smf_sess_t *smf_sess_add_by_gtp_message(ogs_gtp_message_t *message)
{
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
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

    ogs_fqdn_parse(apn,
            req->access_point_name.data, req->access_point_name.len);

    ogs_trace("smf_sess_add_by_message() [APN:%s]", apn);

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

    smf_ue = smf_ue_find_by_imsi(req->imsi.data, req->imsi.len);
    if (!smf_ue) {
        smf_ue = smf_ue_add_by_imsi(req->imsi.data, req->imsi.len);
        ogs_assert(smf_ue);
    }

    sess = smf_sess_find_by_apn(smf_ue, apn);
    if (sess) {
        ogs_warn("OLD Session Release [IMSI:%s,APN:%s]",
                smf_ue->imsi_bcd, sess->pdn.apn);
        smf_sess_remove(sess);
    }

    sess = smf_sess_add_by_apn(smf_ue, apn);
    return sess;
}

smf_sess_t *smf_sess_add_by_psi(smf_ue_t *smf_ue, uint8_t psi)
{
    smf_event_t e;

    smf_sess_t *sess = NULL;

    ogs_assert(smf_ue);
    ogs_assert(psi != OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED);

    ogs_pool_alloc(&smf_sess_pool, &sess);
    if (!sess) {
        ogs_error("Maximum number of session[%lld] reached",
            (long long)ogs_app()->pool.sess);
        return NULL;
    }
    memset(sess, 0, sizeof *sess);

    ogs_pfcp_pool_init(&sess->pfcp);
    smf_qfi_pool_init(sess);

    sess->index = ogs_pool_index(&smf_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_app()->pool.sess);

    sess->sm_context_ref = ogs_msprintf("%d",
            (int)ogs_pool_index(&smf_sess_pool, sess));
    ogs_assert(sess->sm_context_ref);

    /* Create BAR in PFCP Session */
    ogs_pfcp_bar_new(&sess->pfcp);

    /* Set PSI */
    sess->psi = psi;

    /* Set S-NSSAI */
    sess->s_nssai.sst = 0;
    sess->s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    sess->s_nssai.mapped_hplmn_sst = 0;
    sess->s_nssai.mapped_hplmn_sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    /* Set TEID & SEID */
    sess->smf_n4_teid = sess->index;
    sess->smf_n4_seid = sess->index;

    /* Setup Timer */
    sess->t_release_holding = ogs_timer_add(
            ogs_app()->timer_mgr, smf_timer_release_holding_expire, sess);

    memset(&e, 0, sizeof(e));
    e.sess = sess;
    ogs_fsm_create(&sess->sm, smf_gsm_state_initial, smf_gsm_state_final);
    ogs_fsm_init(&sess->sm, &e);

    sess->smf_ue = smf_ue;

    ogs_list_add(&smf_ue->sess_list, sess);

    stats_add_smf_session();

    return sess;
}

smf_sess_t *smf_sess_add_by_sbi_message(ogs_sbi_message_t *message)
{
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;

    OpenAPI_sm_context_create_data_t *SmContextCreateData = NULL;

    ogs_assert(message);
    SmContextCreateData = message->SmContextCreateData;
    ogs_assert(SmContextCreateData);

    if (!SmContextCreateData->supi) {
        ogs_error("No SUPI");
        return NULL;
    }

    if (SmContextCreateData->pdu_session_id ==
            OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
        ogs_error("PDU session identitiy is unassigned");
        return NULL;
    }

    smf_ue = smf_ue_find_by_supi(SmContextCreateData->supi);
    if (!smf_ue) {
        smf_ue = smf_ue_add_by_supi(SmContextCreateData->supi);
        ogs_assert(smf_ue);
    }

    sess = smf_sess_find_by_psi(smf_ue, SmContextCreateData->pdu_session_id);
    if (sess) {
        ogs_warn("OLD Session Release [SUPI:%s,PDU Session identity:%d]",
                SmContextCreateData->supi, SmContextCreateData->pdu_session_id);
        smf_sess_remove(sess);
    }

    sess = smf_sess_add_by_psi(smf_ue, SmContextCreateData->pdu_session_id);

    return sess;
}

void smf_sess_set_ue_ip(smf_sess_t *sess)
{
    ogs_pfcp_subnet_t *subnet6 = NULL;
    smf_ue_t *smf_ue = NULL;

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    if (sess->pdn.pdn_type == OGS_PDU_SESSION_TYPE_IPV4V6) {

        /*
         * This is the case when the HSS is set to IPv4v6 and
         * the UE requests IPv4v6.
         *
         * At this time, it was changed to operate normally
         * even when SMF has no IPv4 or IPv6 subnet.
         *
         * If there is no IPv6 subnet, only IPv4 IP address is assigned.
         * If there is no IPv4 subnet, only IPv6 IP address is assigned.
         * If there are IPv4/IPv6 subnet, IPv4/IPv6 IP address are assigned.
         */

        ogs_pfcp_subnet_t *subnet = NULL;
        ogs_pfcp_subnet_t *subnet6 = NULL;

        subnet = ogs_pfcp_find_subnet_by_dnn(AF_INET, sess->pdn.dnn);
        subnet6 = ogs_pfcp_find_subnet_by_dnn(AF_INET6, sess->pdn.dnn);

        if (subnet != NULL && subnet6 == NULL)
            sess->pdn.pdn_type = OGS_PDU_SESSION_TYPE_IPV4;
        else if (subnet == NULL && subnet6 != NULL)
            sess->pdn.pdn_type = OGS_PDU_SESSION_TYPE_IPV6;
    }

    sess->pdn.paa.pdn_type = sess->pdn.pdn_type;
    ogs_assert(sess->pdn.pdn_type);

    if (sess->ipv4) {
        ogs_hash_set(smf_self()->ipv4_hash,
                sess->ipv4->addr, OGS_IPV4_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv4);
    }
    if (sess->ipv6) {
        ogs_hash_set(smf_self()->ipv6_hash,
                sess->ipv6->addr, OGS_IPV6_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv6);
    }

    if (sess->pdn.pdn_type == OGS_PDU_SESSION_TYPE_IPV4) {
        sess->ipv4 = ogs_pfcp_ue_ip_alloc(
                AF_INET, sess->pdn.dnn, (uint8_t *)&sess->pdn.ue_ip.addr);
        ogs_assert(sess->ipv4);
        sess->pdn.paa.addr = sess->ipv4->addr[0];
        ogs_hash_set(smf_self()->ipv4_hash,
                sess->ipv4->addr, OGS_IPV4_LEN, sess);
    } else if (sess->pdn.pdn_type == OGS_PDU_SESSION_TYPE_IPV6) {
        sess->ipv6 = ogs_pfcp_ue_ip_alloc(
                AF_INET6, sess->pdn.dnn, sess->pdn.ue_ip.addr6);
        ogs_assert(sess->ipv6);

        subnet6 = sess->ipv6->subnet;
        ogs_assert(subnet6);

        sess->pdn.paa.len = subnet6->prefixlen;
        memcpy(sess->pdn.paa.addr6, sess->ipv6->addr, OGS_IPV6_LEN);
        ogs_hash_set(smf_self()->ipv6_hash,
                sess->ipv6->addr, OGS_IPV6_LEN, sess);
    } else if (sess->pdn.pdn_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        sess->ipv4 = ogs_pfcp_ue_ip_alloc(
                AF_INET, sess->pdn.dnn, (uint8_t *)&sess->pdn.ue_ip.addr);
        ogs_assert(sess->ipv4);
        sess->ipv6 = ogs_pfcp_ue_ip_alloc(
                AF_INET6, sess->pdn.dnn, sess->pdn.ue_ip.addr6);
        ogs_assert(sess->ipv6);

        subnet6 = sess->ipv6->subnet;
        ogs_assert(subnet6);

        sess->pdn.paa.both.addr = sess->ipv4->addr[0];
        sess->pdn.paa.both.len = subnet6->prefixlen;
        memcpy(sess->pdn.paa.both.addr6, sess->ipv6->addr, OGS_IPV6_LEN);
        ogs_hash_set(smf_self()->ipv4_hash,
                sess->ipv4->addr, OGS_IPV4_LEN, sess);
        ogs_hash_set(smf_self()->ipv6_hash,
                sess->ipv6->addr, OGS_IPV6_LEN, sess);
    } else {
        ogs_fatal("Invalid sess->pdn.pdn_type[%d]", sess->pdn.pdn_type);
        ogs_assert_if_reached();
    }
}

void smf_sess_remove(smf_sess_t *sess)
{
    int i;
    smf_ue_t *smf_ue = NULL;
    smf_event_t e;
    
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);
   
    ogs_info("Removed Session: UE IMSI:[%s] DNN:[%s] IPv4:[%s] IPv6:[%s]",
           smf_ue->imsi_bcd,
           sess->pdn.dnn,
       sess->ipv4 ? OGS_INET_NTOP(&sess->ipv4->addr, buf1) : "",
       sess->ipv6 ? OGS_INET6_NTOP(&sess->ipv6->addr, buf2) : "");

    ogs_list_remove(&smf_ue->sess_list, sess);

    memset(&e, 0, sizeof(e));
    e.sess = sess;
    ogs_fsm_fini(&sess->sm, &e);
    ogs_fsm_delete(&sess->sm);

    OGS_TLV_CLEAR_DATA(&sess->gtp.ue_pco);
    OGS_TLV_CLEAR_DATA(&sess->gtp.user_location_information);
    OGS_TLV_CLEAR_DATA(&sess->gtp.ue_timezone);

    OGS_NAS_CLEAR_DATA(&sess->nas.ue_pco);

    for (i = 0; i < sess->num_of_pcc_rule; i++)
        OGS_PCC_RULE_FREE(&sess->pcc_rule[i]);
    sess->num_of_pcc_rule = 0;

    if (sess->ipv4) {
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv4);
    }
    if (sess->ipv6) {
        ogs_hash_set(self.ipv6_hash, sess->ipv6->addr, OGS_IPV6_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv6);
    }

    if (sess->sm_context_ref)
        ogs_free(sess->sm_context_ref);

    if (sess->sm_context_status_uri)
        ogs_free(sess->sm_context_status_uri);
    if (sess->namf.client)
        ogs_sbi_client_remove(sess->namf.client);

    if (sess->policy_association_id)
        ogs_free(sess->policy_association_id);

    if (sess->dnn)
        ogs_free(sess->dnn);

    if (sess->upf_n3_addr)
        ogs_freeaddrinfo(sess->upf_n3_addr);
    if (sess->upf_n3_addr6)
        ogs_freeaddrinfo(sess->upf_n3_addr6);

    /* Free SBI object memory */
    ogs_sbi_object_free(&sess->sbi);

    ogs_timer_delete(sess->t_release_holding);

    smf_bearer_remove_all(sess);

    ogs_assert(sess->pfcp.bar);
    ogs_pfcp_bar_delete(sess->pfcp.bar);

    ogs_pfcp_pool_final(&sess->pfcp);
    smf_qfi_pool_final(sess);

    ogs_pool_free(&smf_sess_pool, sess);

    stats_remove_smf_session();
}

void smf_sess_remove_all(smf_ue_t *smf_ue)
{
    smf_sess_t *sess = NULL, *next = NULL;;

    ogs_assert(smf_ue);

    ogs_list_for_each_safe(&smf_ue->sess_list, next, sess)
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

smf_sess_t *smf_sess_find_by_apn(smf_ue_t *smf_ue, char *apn)
{
    smf_sess_t *sess = NULL;

    ogs_assert(smf_ue);
    ogs_assert(apn);

    ogs_list_for_each(&smf_ue->sess_list, sess) {
        if (!ogs_strcasecmp(sess->pdn.apn, apn))
            return sess;
    }

    return NULL;
}

smf_sess_t *smf_sess_find_by_psi(smf_ue_t *smf_ue, uint8_t psi)
{
    smf_sess_t *sess = NULL;

    ogs_assert(smf_ue);
    ogs_assert(psi != OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED);

    ogs_list_for_each(&smf_ue->sess_list, sess) {
        if (sess->psi == psi)
            return sess;
    }

    return NULL;
}

smf_sess_t *smf_sess_find_by_sm_context_ref(char *sm_context_ref)
{
    ogs_assert(sm_context_ref);
    return smf_sess_find(atoll(sm_context_ref));
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

smf_ue_t *smf_ue_cycle(smf_ue_t *smf_ue)
{
    return ogs_pool_cycle(&smf_ue_pool, smf_ue);
}

smf_sess_t *smf_sess_cycle(smf_sess_t *sess)
{
    return ogs_pool_cycle(&smf_sess_pool, sess);
}

smf_bearer_t *smf_qos_flow_add(smf_sess_t *sess)
{
    smf_bearer_t *qos_flow = NULL;

    ogs_pfcp_pdr_t *dl_pdr = NULL;
    ogs_pfcp_pdr_t *ul_pdr = NULL;
    ogs_pfcp_far_t *dl_far = NULL;
    ogs_pfcp_far_t *ul_far = NULL;
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&smf_bearer_pool, &qos_flow);
    ogs_assert(qos_flow);
    memset(qos_flow, 0, sizeof *qos_flow);

    qos_flow->index = ogs_pool_index(&smf_bearer_pool, qos_flow);
    ogs_assert(qos_flow->index > 0 && qos_flow->index <=
            ogs_app()->pool.bearer);

    ogs_list_init(&qos_flow->pf_list);

    dl_pdr = ogs_pfcp_pdr_add(&sess->pfcp);
    ogs_assert(dl_pdr);
    qos_flow->dl_pdr = dl_pdr;

    dl_pdr->src_if = OGS_PFCP_INTERFACE_CORE;

    if (strlen(sess->pdn.apn))
        dl_pdr->apn = ogs_strdup(sess->pdn.apn);

    ul_pdr = ogs_pfcp_pdr_add(&sess->pfcp);
    ogs_assert(ul_pdr);
    qos_flow->ul_pdr = ul_pdr;

    ul_pdr->src_if = OGS_PFCP_INTERFACE_ACCESS;

    if (strlen(sess->pdn.apn))
        ul_pdr->apn = ogs_strdup(sess->pdn.apn);

    ul_pdr->outer_header_removal_len = 1;
    if (sess->pdn.pdn_type == OGS_GTP_PDN_TYPE_IPV4) {
        ul_pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4;
    } else if (sess->pdn.pdn_type == OGS_GTP_PDN_TYPE_IPV6) {
        ul_pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IPV6;
    } else if (sess->pdn.pdn_type == OGS_GTP_PDN_TYPE_IPV4V6) {
        ul_pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IP;
    } else
        ogs_assert_if_reached();

    dl_far = ogs_pfcp_far_add(&sess->pfcp);
    ogs_assert(dl_far);
    qos_flow->dl_far = dl_far;

    dl_far->dst_if = OGS_PFCP_INTERFACE_ACCESS;
    ogs_pfcp_pdr_associate_far(dl_pdr, dl_far);

    dl_far->apply_action =
        OGS_PFCP_APPLY_ACTION_BUFF| OGS_PFCP_APPLY_ACTION_NOCP;
    ogs_assert(sess->pfcp.bar);

    ul_far = ogs_pfcp_far_add(&sess->pfcp);
    ogs_assert(ul_far);
    qos_flow->ul_far = ul_far;

    ul_far->dst_if = OGS_PFCP_INTERFACE_CORE;
    ogs_pfcp_pdr_associate_far(ul_pdr, ul_far);

    ul_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

    qer = ogs_pfcp_qer_add(&sess->pfcp);
    ogs_assert(qer);
    qos_flow->qer = qer;

    ogs_pfcp_pdr_associate_qer(dl_pdr, qer);
    ogs_pfcp_pdr_associate_qer(ul_pdr, qer);

    /* Allocate QFI */
    ogs_pool_alloc(&sess->qfi_pool, &qos_flow->qfi_node);
    ogs_assert(qos_flow->qfi_node);

    qos_flow->qfi = ul_pdr->qfi = qer->qfi = *(qos_flow->qfi_node);

    qos_flow->sess = sess;

    ogs_list_add(&sess->bearer_list, qos_flow);

    return qos_flow;
}

smf_bearer_t *smf_qos_flow_find_by_qfi(smf_sess_t *sess, uint8_t qfi)
{
    smf_bearer_t *qos_flow = NULL;

    ogs_assert(sess);
    ogs_list_for_each(&sess->bearer_list, qos_flow) {
        if (qos_flow->qfi == qfi)
            return qos_flow;
    }

    return qos_flow;
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

    smf_pf_identifier_pool_init(bearer);

    bearer->index = ogs_pool_index(&smf_bearer_pool, bearer);
    ogs_assert(bearer->index > 0 && bearer->index <=
            ogs_app()->pool.bearer);

    ogs_list_init(&bearer->pf_list);

    dl_pdr = ogs_pfcp_pdr_add(&sess->pfcp);
    ogs_assert(dl_pdr);
    bearer->dl_pdr = dl_pdr;

    dl_pdr->src_if = OGS_PFCP_INTERFACE_CORE;

    if (strlen(sess->pdn.apn))
        dl_pdr->apn = ogs_strdup(sess->pdn.apn);

    ul_pdr = ogs_pfcp_pdr_add(&sess->pfcp);
    ogs_assert(ul_pdr);
    bearer->ul_pdr = ul_pdr;

    ul_pdr->src_if = OGS_PFCP_INTERFACE_ACCESS;

    if (strlen(sess->pdn.apn))
        ul_pdr->apn = ogs_strdup(sess->pdn.apn);

    ul_pdr->outer_header_removal_len = 1;
    if (sess->pdn.pdn_type == OGS_GTP_PDN_TYPE_IPV4) {
        ul_pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4;
    } else if (sess->pdn.pdn_type == OGS_GTP_PDN_TYPE_IPV6) {
        ul_pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IPV6;
    } else if (sess->pdn.pdn_type == OGS_GTP_PDN_TYPE_IPV4V6) {
        ul_pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IP;
    } else
        ogs_assert_if_reached();

    dl_far = ogs_pfcp_far_add(&sess->pfcp);
    ogs_assert(dl_far);
    bearer->dl_far = dl_far;

    dl_far->dst_if = OGS_PFCP_INTERFACE_ACCESS;
    ogs_pfcp_pdr_associate_far(dl_pdr, dl_far);

    dl_far->apply_action =
        OGS_PFCP_APPLY_ACTION_BUFF| OGS_PFCP_APPLY_ACTION_NOCP;
    ogs_assert(sess->pfcp.bar);

    ul_far = ogs_pfcp_far_add(&sess->pfcp);
    ogs_assert(ul_far);
    bearer->ul_far = ul_far;

    ul_far->dst_if = OGS_PFCP_INTERFACE_CORE;
    ogs_pfcp_pdr_associate_far(ul_pdr, ul_far);

    ul_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

    ogs_assert(sess->pfcp_node);
    if (sess->pfcp_node->up_function_features.ftup) {
        ul_pdr->f_teid.ch = 1;
        ul_pdr->f_teid_len = 1;
    } else {
        resource = ogs_pfcp_gtpu_resource_find(
                &sess->pfcp_node->gtpu_resource_list,
                sess->pdn.apn, OGS_PFCP_INTERFACE_ACCESS);
        if (resource) {
            ogs_pfcp_user_plane_ip_resource_info_to_sockaddr(&resource->info,
                &bearer->pgw_s5u_addr, &bearer->pgw_s5u_addr6);
            if (resource->info.teidri)
                bearer->pgw_s5u_teid = OGS_PFCP_GTPU_INDEX_TO_TEID(
                        bearer->index, resource->info.teidri,
                        resource->info.teid_range);
            else
                bearer->pgw_s5u_teid = bearer->index;
        } else {
            if (sess->pfcp_node->addr.ogs_sa_family == AF_INET)
                ogs_copyaddrinfo(&bearer->pgw_s5u_addr, &sess->pfcp_node->addr);
            else if (sess->pfcp_node->addr.ogs_sa_family == AF_INET6)
                ogs_copyaddrinfo(
                        &bearer->pgw_s5u_addr6, &sess->pfcp_node->addr);
            else
                ogs_assert_if_reached();

            bearer->pgw_s5u_teid = bearer->index;
        }

        ogs_assert(bearer->pgw_s5u_addr || bearer->pgw_s5u_addr6);
        ogs_pfcp_sockaddr_to_f_teid(bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6,
                &ul_pdr->f_teid, &ul_pdr->f_teid_len);
        ul_pdr->f_teid.teid = bearer->pgw_s5u_teid;
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

    ogs_pfcp_pdr_remove(bearer->dl_pdr);
    ogs_pfcp_pdr_remove(bearer->ul_pdr);
    ogs_pfcp_far_remove(bearer->dl_far);
    ogs_pfcp_far_remove(bearer->ul_far);
    if (bearer->qer)
        ogs_pfcp_qer_remove(bearer->qer);

    if (bearer->name)
        ogs_free(bearer->name);
    if (bearer->pgw_s5u_addr)
        ogs_freeaddrinfo(bearer->pgw_s5u_addr);
    if (bearer->pgw_s5u_addr6)
        ogs_freeaddrinfo(bearer->pgw_s5u_addr6);

    smf_pf_remove_all(bearer);

    smf_pf_identifier_pool_final(bearer);

    if (bearer->qfi_node)
        ogs_pool_free(&bearer->sess->qfi_pool, bearer->qfi_node);

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

smf_bearer_t *smf_bearer_find_by_pgw_s5u_teid(
        smf_sess_t *sess, uint32_t pgw_s5u_teid)
{
    smf_bearer_t *bearer = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->bearer_list, bearer) {
        if (bearer->pgw_s5u_teid == pgw_s5u_teid)
            return bearer;
    }

    return NULL;
}

smf_bearer_t *smf_bearer_find_by_ebi(smf_sess_t *sess, uint8_t ebi)
{
    smf_bearer_t *bearer = NULL;
    
    ogs_assert(sess);

    ogs_list_for_each(&sess->bearer_list, bearer) {
        if (bearer->ebi == ebi)
            return bearer;
    }

    return NULL;
}

smf_bearer_t *smf_bearer_find_by_name(smf_sess_t *sess, char *name)
{
    smf_bearer_t *bearer = NULL;
    
    ogs_assert(sess);
    ogs_assert(name);

    ogs_list_for_each(&sess->bearer_list, bearer) {
        if (bearer->name && strcmp(bearer->name, name) == 0)
            return bearer;
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

smf_bearer_t *smf_bearer_find_by_pdr_id(
        smf_sess_t *sess, ogs_pfcp_pdr_id_t pdr_id)
{
    smf_bearer_t *bearer = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_pfcp_pdr_t *dl_pdr = NULL;
        ogs_pfcp_pdr_t *ul_pdr = NULL;

        dl_pdr = bearer->dl_pdr;
        ogs_assert(dl_pdr);
        ul_pdr = bearer->ul_pdr;
        ogs_assert(ul_pdr);

        if (dl_pdr->id == pdr_id || ul_pdr->id == pdr_id)
            return bearer;
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

    return bearer == default_bearer;
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

    ogs_pool_alloc(&bearer->pf_identifier_pool, &pf->identifier_node);
    ogs_assert(pf->identifier_node);

    pf->index = *(pf->identifier_node);
    ogs_assert(pf->index > 0 && pf->index <= OGS_MAX_NUM_OF_PF);

    pf->identifier = pf->index;
    pf->bearer = bearer;

    ogs_list_add(&bearer->pf_list, pf);

    return pf;
}

int smf_pf_remove(smf_pf_t *pf)
{
    ogs_assert(pf);
    ogs_assert(pf->bearer);

    ogs_list_remove(&pf->bearer->pf_list, pf);
    if (pf->flow_description)
        ogs_free(pf->flow_description);

    if (pf->identifier_node)
        ogs_pool_free(&pf->bearer->pf_identifier_pool, pf->identifier_node);

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

int smf_pco_build(uint8_t *pco_buf, uint8_t *buffer, int length)
{
    int rv;
    ogs_pco_t ue, smf;
    ogs_pco_ipcp_t pco_ipcp;
    ogs_ipsubnet_t dns_primary, dns_secondary, dns6_primary, dns6_secondary;
    ogs_ipsubnet_t p_cscf, p_cscf6;
    int size = 0;
    int i = 0;
    uint16_t mtu = 0;

    ogs_assert(pco_buf);
    ogs_assert(buffer);
    ogs_assert(length);

    size = ogs_pco_parse(&ue, buffer, length);
    ogs_assert(size);

    memset(&smf, 0, sizeof(ogs_pco_t));
    smf.ext = ue.ext;
    smf.configuration_protocol = ue.configuration_protocol;

    for (i = 0; i < ue.num_of_id; i++) {
        uint8_t *data = ue.ids[i].data;
        switch(ue.ids[i].id) {
        case OGS_PCO_ID_CHALLENGE_HANDSHAKE_AUTHENTICATION_PROTOCOL:
            if (data[0] == 2) { /* Code : Response */
                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = 4;
                smf.ids[smf.num_of_id].data =
                    (uint8_t *)"\x03\x00\x00\x04"; /* Code : Success */
                smf.num_of_id++;
            }
            break;
        case OGS_PCO_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL:
            if (data[0] == 1) { /* Code : Configuration Request */
                uint16_t len = 0;

                ogs_assert(smf_self()->dns[0] || smf_self()->dns[1]);

                memset(&pco_ipcp, 0, sizeof(ogs_pco_ipcp_t));
                pco_ipcp.code = 2; /* Code : Configuration Ack */
                pco_ipcp.len = htobe16(len);

                len = 4;
                /* Primary DNS Server IP Address */
                if (smf_self()->dns[0]) {
                    rv = ogs_ipsubnet(
                            &dns_primary, smf_self()->dns[0], NULL);
                    ogs_assert(rv == OGS_OK);
                    pco_ipcp.options[0].type = 129;
                    pco_ipcp.options[0].len = 6;
                    pco_ipcp.options[0].addr = dns_primary.sub[0];
                    len += 6;
                }

                /* Secondary DNS Server IP Address */
                if (smf_self()->dns[1]) {
                    rv = ogs_ipsubnet(
                            &dns_secondary, smf_self()->dns[1], NULL);
                    ogs_assert(rv == OGS_OK);
                    pco_ipcp.options[1].type = 131;
                    pco_ipcp.options[1].len = 6;
                    pco_ipcp.options[1].addr = dns_secondary.sub[0];
                    len += 6;
                }

                pco_ipcp.len = htobe16(len);

                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = len;
                smf.ids[smf.num_of_id].data = (uint8_t *)&pco_ipcp;

                smf.num_of_id++;
            }
            break;
        case OGS_PCO_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST:
            if (smf_self()->dns[0]) {
                rv = ogs_ipsubnet(
                        &dns_primary, smf_self()->dns[0], NULL);
                ogs_assert(rv == OGS_OK);
                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = OGS_IPV4_LEN;
                smf.ids[smf.num_of_id].data = dns_primary.sub;
                smf.num_of_id++;
            }

            if (smf_self()->dns[1]) {
                rv = ogs_ipsubnet(
                        &dns_secondary, smf_self()->dns[1], NULL);
                ogs_assert(rv == OGS_OK);
                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = OGS_IPV4_LEN;
                smf.ids[smf.num_of_id].data = dns_secondary.sub;
                smf.num_of_id++;
            }
            break;
        case OGS_PCO_ID_DNS_SERVER_IPV6_ADDRESS_REQUEST:
            if (smf_self()->dns6[0]) {
                rv = ogs_ipsubnet(
                        &dns6_primary, smf_self()->dns6[0], NULL);
                ogs_assert(rv == OGS_OK);
                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = OGS_IPV6_LEN;
                smf.ids[smf.num_of_id].data = dns6_primary.sub;
                smf.num_of_id++;
            }

            if (smf_self()->dns6[1]) {
                rv = ogs_ipsubnet(
                        &dns6_secondary, smf_self()->dns6[1], NULL);
                ogs_assert(rv == OGS_OK);
                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = OGS_IPV6_LEN;
                smf.ids[smf.num_of_id].data = dns6_secondary.sub;
                smf.num_of_id++;
            }
            break;
        case OGS_PCO_ID_P_CSCF_IPV4_ADDRESS_REQUEST:
            if (smf_self()->num_of_p_cscf) {
                rv = ogs_ipsubnet(&p_cscf,
                    smf_self()->p_cscf[smf_self()->p_cscf_index], NULL);
                ogs_assert(rv == OGS_OK);
                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = OGS_IPV4_LEN;
                smf.ids[smf.num_of_id].data = p_cscf.sub;
                smf.num_of_id++;

                smf_self()->p_cscf_index++;
                smf_self()->p_cscf_index %= smf_self()->num_of_p_cscf;
            }
            break;
        case OGS_PCO_ID_P_CSCF_IPV6_ADDRESS_REQUEST:
            if (smf_self()->num_of_p_cscf6) {
                rv = ogs_ipsubnet(&p_cscf6,
                    smf_self()->p_cscf6[smf_self()->p_cscf6_index], NULL);
                ogs_assert(rv == OGS_OK);
                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = OGS_IPV6_LEN;
                smf.ids[smf.num_of_id].data = p_cscf6.sub;
                smf.num_of_id++;

                smf_self()->p_cscf6_index++;
                smf_self()->p_cscf6_index %= smf_self()->num_of_p_cscf6;
            }
            break;
        case OGS_PCO_ID_IPV4_LINK_MTU_REQUEST:
            if (smf_self()->mtu) {
                mtu = htons(smf_self()->mtu);
                smf.ids[smf.num_of_id].id = ue.ids[i].id;
                smf.ids[smf.num_of_id].len = sizeof(uint16_t);
                smf.ids[smf.num_of_id].data = &mtu;
                smf.num_of_id++;
            }
            break;
        case OGS_PCO_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING:
            /* TODO */
            break;
        case OGS_PCO_ID_MS_SUPPORTS_BCM:
            /* TODO */
            break;
        case OGS_PCO_ID_MS_SUPPORT_LOCAL_ADDR_TFT_INDICATOR:
            /* TODO */
            break;
        case OGS_PCO_ID_P_CSCF_RE_SELECTION_SUPPORT:
            /* TODO */
            break;
        default:
            ogs_warn("Unknown PCO ID:(0x%x)", ue.ids[i].id);
        }
    }

    size = ogs_pco_build(pco_buf, OGS_MAX_PCO_LEN, &smf);
    return size;
}

void smf_qfi_pool_init(smf_sess_t *sess)
{
    int i;

    ogs_assert(sess);

    ogs_index_init(&sess->qfi_pool, OGS_MAX_QOS_FLOW_ID);

    for (i = 1; i <= OGS_MAX_QOS_FLOW_ID; i++) {
        sess->qfi_pool.array[i-1] = i;
    }
}

void smf_qfi_pool_final(smf_sess_t *sess)
{
    ogs_assert(sess);

    ogs_index_final(&sess->qfi_pool);
}

void smf_pf_identifier_pool_init(smf_bearer_t *bearer)
{
    int i;

    ogs_assert(bearer);

    ogs_index_init(&bearer->pf_identifier_pool, OGS_MAX_NUM_OF_PF);

    for (i = 1; i <= OGS_MAX_NUM_OF_PF; i++) {
        bearer->pf_identifier_pool.array[i-1] = i;
    }
}

void smf_pf_identifier_pool_final(smf_bearer_t *bearer)
{
    ogs_assert(bearer);

    ogs_index_final(&bearer->pf_identifier_pool);
}

static void stats_add_smf_session(void)
{
    num_of_smf_sess = num_of_smf_sess + 1;
    ogs_info("[Added] Number of SMF-Sessions is now %d", num_of_smf_sess);
}

static void stats_remove_smf_session(void)
{
    num_of_smf_sess = num_of_smf_sess - 1;
    ogs_info("[Removed] Number of SMF-Sessions is now %d", num_of_smf_sess);
}
