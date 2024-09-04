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

#include "ogs-dbi.h"
#include "pcrf-context.h"
#include "pcrf-fd-path.h"

static pcrf_context_t self;
static ogs_diam_config_t g_diam_conf;

int __pcrf_log_domain;

static int context_initialized = 0;

pcrf_context_t *pcrf_self(void)
{
    return &self;
}

void pcrf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initial FreeDiameter Config */
    memset(&g_diam_conf, 0, sizeof(ogs_diam_config_t));

    /* Initialize PCRF context */
    memset(&self, 0, sizeof(pcrf_context_t));
    self.diam_config = &g_diam_conf;

    ogs_log_install_domain(&__ogs_diam_domain, "diam", ogs_core()->log.level);
    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", ogs_core()->log.level);
    ogs_log_install_domain(&__pcrf_log_domain, "pcrf", ogs_core()->log.level);

    ogs_thread_mutex_init(&self.db_lock);

    ogs_thread_mutex_init(&self.hash_lock);
    self.ip_hash = ogs_hash_make();
    ogs_assert(self.ip_hash);

    context_initialized = 1;
}

void pcrf_context_final(void)
{
    ogs_assert(context_initialized == 1);
    ogs_assert(self.ip_hash);
    ogs_hash_destroy(self.ip_hash);
    ogs_thread_mutex_destroy(&self.hash_lock);

    ogs_thread_mutex_destroy(&self.db_lock);

    context_initialized = 0;
}

static int pcrf_context_prepare(void)
{
    self.diam_config->cnf_port = DIAMETER_PORT;
    self.diam_config->cnf_port_tls = DIAMETER_SECURE_PORT;
    self.diam_config->stats.priv_stats_size = sizeof(pcrf_diam_stats_t);

    return OGS_OK;
}

static int pcrf_context_validation(void)
{
    if (self.diam_conf_path == NULL &&
        (self.diam_config->cnf_diamid == NULL ||
        self.diam_config->cnf_diamrlm == NULL ||
        self.diam_config->cnf_addr == NULL)) {
        ogs_error("No pcrf.freeDiameter in '%s'",
                ogs_app()->file);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int session_conf_prepare(void)
{
    ogs_app_policy_conf_t *policy_conf = NULL;
    ogs_app_slice_conf_t *slice_conf = NULL;

    ogs_plmn_id_t plmn_id;
    ogs_s_nssai_t s_nssai;

    ogs_plmn_id_build(&plmn_id, 999, 70, 2);

    s_nssai.sst = 1;
    s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    /* Added Dummy POLICY for EPC */
    policy_conf = ogs_app_policy_conf_add(&plmn_id);
    if (!policy_conf) {
        ogs_error("ogs_app_policy_conf_add() failed");
        return OGS_ERROR;
    }

    /* Added Dummy SLICE for EPC */
    slice_conf = ogs_app_slice_conf_add(policy_conf, &s_nssai);
    if (!slice_conf) {
        ogs_error("ogs_app_slice_conf_add() failed");
        return OGS_ERROR;
    }
    slice_conf->data.default_indicator = true;

    return OGS_OK;
}

static int session_conf_validation(void)
{
    int rv;

    rv = ogs_app_check_policy_conf();
    if (rv != OGS_OK) {
        ogs_error("ogs_app_check_policy_conf() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int parse_session_conf(ogs_yaml_iter_t *parent)
{
    int rv;
    ogs_app_policy_conf_t *policy_conf = NULL;
    ogs_app_slice_conf_t *slice_conf = NULL;

    ogs_assert(parent);

    rv = session_conf_prepare();
    if (rv != OGS_OK) return rv;

    policy_conf = ogs_list_first(&ogs_local_conf()->policy_list);
    ogs_assert(policy_conf);

    slice_conf = ogs_list_first(&policy_conf->slice_list);
    ogs_assert(slice_conf);

    rv = ogs_app_parse_session_conf(parent, slice_conf);
    if (rv != OGS_OK) return rv;

    rv = session_conf_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

int pcrf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = pcrf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "pcrf")) {
            ogs_yaml_iter_t pcrf_iter;
            ogs_yaml_iter_recurse(&root_iter, &pcrf_iter);
            while (ogs_yaml_iter_next(&pcrf_iter)) {
                const char *pcrf_key = ogs_yaml_iter_key(&pcrf_iter);
                ogs_assert(pcrf_key);
                if (!strcmp(pcrf_key, "freeDiameter")) {
                    yaml_node_t *node =
                        yaml_document_get_node(document, pcrf_iter.pair->value);
                    ogs_assert(node);
                    if (node->type == YAML_SCALAR_NODE) {
                        self.diam_conf_path = ogs_yaml_iter_value(&pcrf_iter);
                    } else if (node->type == YAML_MAPPING_NODE) {
                        ogs_yaml_iter_t fd_iter;
                        ogs_yaml_iter_recurse(&pcrf_iter, &fd_iter);

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
                            } else if (!strcmp(fd_key, "no_fwd")) {
                                self.diam_config->cnf_flags.no_fwd =
                                    ogs_yaml_iter_bool(&fd_iter);
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
                                        if (!strcmp(ext_key, "module"))
                                        {
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
                                } while(ogs_yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(fd_key, "connect")) {
                                ogs_yaml_iter_t conn_array, conn_iter;
                                ogs_yaml_iter_recurse(&fd_iter, &conn_array);
                                do {
                                    const char *identity = NULL;
                                    const char *addr = NULL;
                                    uint16_t port = 0;
                                    int tc_timer = 0;

                                    if (ogs_yaml_iter_type(&conn_array) ==
                                        YAML_MAPPING_NODE) {
                                        memcpy(&conn_iter, &conn_array,
                                                sizeof(ogs_yaml_iter_t));
                                    } else if (ogs_yaml_iter_type(&conn_array)
                                            == YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&conn_array))
                                            break;
                                        ogs_yaml_iter_recurse(
                                                &conn_array, &conn_iter);
                                    } else if (ogs_yaml_iter_type(&conn_array)
                                            == YAML_SCALAR_NODE) {
                                        break;
                                    } else
                                        ogs_assert_if_reached();

                                    while (ogs_yaml_iter_next(&conn_iter)) {
                                        const char *conn_key =
                                            ogs_yaml_iter_key(&conn_iter);
                                        ogs_assert(conn_key);
                                        if (!strcmp(conn_key, "identity")) {
                                            identity = ogs_yaml_iter_value(
                                                    &conn_iter);
                                        } else if (!strcmp(conn_key,
                                                    "address")) {
                                            addr = ogs_yaml_iter_value(
                                                    &conn_iter);
                                        } else if (!strcmp(conn_key, "port")) {
                                            const char *v =
                                                ogs_yaml_iter_value(&conn_iter);
                                            if (v) port = atoi(v);
                                        } else if (!strcmp(conn_key, "tc_timer")) {
                                            const char *v =
                                                ogs_yaml_iter_value(&conn_iter);
                                            if (v) tc_timer = atoi(v);
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
                                        self.diam_config->
                                            conn[self.diam_config->num_of_conn].
                                                tc_timer = tc_timer;
                                        self.diam_config->num_of_conn++;
                                    }
                                } while (ogs_yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(fd_key, "tc_timer")) {
                                const char *v = ogs_yaml_iter_value(&fd_iter);
                                if (v) self.diam_config->cnf_timer_tc = atoi(v);
                            } else
                                ogs_warn("unknown key `%s`", fd_key);
                        }
                    }
                } else if (!strcmp(pcrf_key, OGS_SESSION_STRING)) {
                    rv = parse_session_conf(&pcrf_iter);
                    if (rv != OGS_OK) {
                        ogs_error("parse_session_conf() failed");
                        return rv;
                    }
                } else if (!strcmp(pcrf_key, "diameter_stats_interval")) {
                    const char *v = ogs_yaml_iter_value(&pcrf_iter);
                    if (v) self.diam_config->stats.interval_sec = atoi(v);
                } else if (!strcmp(pcrf_key, "metrics")) {
                    /* handle config in metrics library */
                } else
                    ogs_warn("unknown key `%s`", pcrf_key);
            }
        }
    }

    rv = pcrf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

int pcrf_db_qos_data(
        char *imsi_bcd, char *apn, ogs_session_data_t *session_data)
{
    int rv, i;
    char *supi = NULL;

    ogs_app_policy_conf_t *policy_conf = NULL;
    ogs_app_slice_conf_t *slice_conf = NULL;

    ogs_assert(imsi_bcd);
    ogs_assert(apn);
    ogs_assert(session_data);

    ogs_thread_mutex_lock(&self.db_lock);

    memset(session_data, 0, sizeof(*session_data));

    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
    ogs_assert(supi);

    policy_conf = ogs_list_first(&ogs_local_conf()->policy_list);
    if (policy_conf)
        slice_conf = ogs_list_first(&policy_conf->slice_list);

    if (slice_conf) {
        rv = ogs_app_config_session_data(NULL, NULL, apn, session_data);
        if (rv != OGS_OK)
            ogs_error("ogs_app_config_session_data() failed for APN(%s)", apn);
    } else {
        rv = ogs_dbi_session_data(supi, NULL, apn, session_data);
        if (rv != OGS_OK)
            ogs_error("ogs_dbi_session_data() failed for IMSI(%s)+APN(%s)",
                    imsi_bcd, apn);
    }

    /* For EPC, we need to inialize Flow-Status in Pcc-Rule */
    for (i = 0; i < session_data->num_of_pcc_rule; i++) {
        ogs_pcc_rule_t *pcc_rule = &session_data->pcc_rule[i];
        pcc_rule->flow_status = OGS_DIAM_RX_FLOW_STATUS_ENABLED;
    }

    ogs_free(supi);
    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

void pcrf_sess_set_ipv4(const void *key, uint8_t *sid)
{
    ogs_assert(self.ip_hash);

    ogs_thread_mutex_lock(&self.hash_lock);

    ogs_hash_set(self.ip_hash, key, OGS_IPV4_LEN, sid);

    ogs_thread_mutex_unlock(&self.hash_lock);
}
void pcrf_sess_set_ipv6(const void *key, uint8_t *sid)
{
    ogs_assert(self.ip_hash);

    ogs_thread_mutex_lock(&self.hash_lock);

    ogs_hash_set(self.ip_hash, key, OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, sid);

    ogs_thread_mutex_unlock(&self.hash_lock);
}

uint8_t *pcrf_sess_find_by_ipv4(const void *key)
{
    uint8_t *sid = NULL;
    ogs_assert(key);

    ogs_thread_mutex_lock(&self.hash_lock);

    sid = (uint8_t *)ogs_hash_get(self.ip_hash, key, OGS_IPV4_LEN);

    ogs_thread_mutex_unlock(&self.hash_lock);

    return sid;
}

uint8_t *pcrf_sess_find_by_ipv6(const void *key)
{
    uint8_t *sid = NULL;
    ogs_assert(key);

    ogs_thread_mutex_lock(&self.hash_lock);

    sid = (uint8_t *)ogs_hash_get(
            self.ip_hash, key, OGS_IPV6_DEFAULT_PREFIX_LEN >> 3);

    ogs_thread_mutex_unlock(&self.hash_lock);

    return sid;
}
