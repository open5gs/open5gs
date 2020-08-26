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

#include "ogs-dbi.h"
#include "hss-context.h"

static hss_context_t self;
static ogs_diam_config_t g_diam_conf;

int __hss_log_domain;

static int context_initialized = 0;

hss_context_t* hss_self(void)
{
    return &self;
}

void hss_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initial FreeDiameter Config */
    memset(&g_diam_conf, 0, sizeof(ogs_diam_config_t));

    /* Initialize HSS context */
    memset(&self, 0, sizeof(hss_context_t));
    self.diam_config = &g_diam_conf;

    ogs_log_install_domain(&__ogs_diam_domain, "diam", ogs_core()->log.level);
    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", ogs_core()->log.level);
    ogs_log_install_domain(&__hss_log_domain, "hss", ogs_core()->log.level);

    ogs_thread_mutex_init(&self.db_lock);

    context_initialized = 1;
}

void hss_context_final(void)
{
    ogs_assert(context_initialized == 1);

    ogs_thread_mutex_destroy(&self.db_lock);

    context_initialized = 0;
}

static int hss_context_prepare(void)
{
    self.diam_config->cnf_port = DIAMETER_PORT;
    self.diam_config->cnf_port_tls = DIAMETER_SECURE_PORT;

    return OGS_OK;
}

static int hss_context_validation(void)
{
    if (self.diam_conf_path == NULL &&
        (self.diam_config->cnf_diamid == NULL ||
        self.diam_config->cnf_diamrlm == NULL ||
        self.diam_config->cnf_addr == NULL)) {
        ogs_error("No hss.freeDiameter in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int hss_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = hss_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "hss")) {
            ogs_yaml_iter_t hss_iter;
            ogs_yaml_iter_recurse(&root_iter, &hss_iter);
            while (ogs_yaml_iter_next(&hss_iter)) {
                const char *hss_key = ogs_yaml_iter_key(&hss_iter);
                ogs_assert(hss_key);
                if (!strcmp(hss_key, "freeDiameter")) {
                    yaml_node_t *node = 
                        yaml_document_get_node(document, hss_iter.pair->value);
                    ogs_assert(node);
                    if (node->type == YAML_SCALAR_NODE) {
                        self.diam_conf_path = ogs_yaml_iter_value(&hss_iter);
                    } else if (node->type == YAML_MAPPING_NODE) {
                        ogs_yaml_iter_t fd_iter;
                        ogs_yaml_iter_recurse(&hss_iter, &fd_iter);

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
                                                &conn_array) ==
                                        YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&conn_array))
                                            break;
                                        ogs_yaml_iter_recurse(
                                                &conn_array, &conn_iter);
                                    } else if (ogs_yaml_iter_type(
                                                &conn_array) ==
                                        YAML_SCALAR_NODE) {
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
                                        } else if (!strcmp(conn_key, "addr")) {
                                            addr = ogs_yaml_iter_value(
                                                    &conn_iter);
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
                } else
                    ogs_warn("unknown key `%s`", hss_key);
            }
        }
    }

    rv = hss_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

int hss_db_auth_info(char *imsi_bcd, ogs_dbi_auth_info_t *auth_info)
{
    int rv;
    char *supi = NULL;

    ogs_assert(imsi_bcd);
    ogs_assert(auth_info);

    ogs_thread_mutex_lock(&self.db_lock);
    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
    ogs_assert(supi);

    rv = ogs_dbi_auth_info(supi, auth_info);

    ogs_free(supi);
    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int hss_db_update_rand_and_sqn(
    char *imsi_bcd, uint8_t *rand, uint64_t sqn)
{
    int rv;
    char *supi = NULL;

    ogs_assert(imsi_bcd);

    ogs_thread_mutex_lock(&self.db_lock);
    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
    ogs_assert(supi);

    rv = ogs_dbi_update_sqn(supi, sqn);

    ogs_free(supi);
    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int hss_db_increment_sqn(char *imsi_bcd)
{
    int rv;
    char *supi = NULL;

    ogs_assert(imsi_bcd);

    ogs_thread_mutex_lock(&self.db_lock);
    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
    ogs_assert(supi);

    rv = ogs_dbi_increment_sqn(supi);

    ogs_free(supi);
    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int hss_db_subscription_data(
    char *imsi_bcd, ogs_subscription_data_t *subscription_data)
{
    int rv;
    char *supi = NULL;

    ogs_assert(imsi_bcd);
    ogs_assert(subscription_data);

    ogs_thread_mutex_lock(&self.db_lock);
    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
    ogs_assert(supi);

    rv = ogs_dbi_subscription_data(supi, subscription_data);

    ogs_free(supi);
    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}
