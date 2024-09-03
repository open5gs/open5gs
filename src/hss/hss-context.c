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
#include "hss-context.h"
#include "hss-event.h"
#include "hss-fd-path.h"
#include "hss-s6a-path.h"


typedef struct hss_impi_s hss_impi_t;

typedef struct hss_imsi_s {
    ogs_lnode_t lnode;

    char *id;
    char *visited_network_identifier;
} hss_imsi_t;

typedef struct hss_impi_s {
    ogs_lnode_t lnode;

    char *id;

    hss_imsi_t *imsi;
    ogs_list_t impu_list;
} hss_impi_t;

typedef struct hss_impu_s {
    ogs_lnode_t lnode;

    char *id;
    char *server_name;

    hss_impi_t *impi;
} hss_impu_t;

static hss_context_t self;
static ogs_diam_config_t g_diam_conf;

int __hss_log_domain;

static int context_initialized = 0;

static OGS_POOL(imsi_pool, hss_imsi_t);
static OGS_POOL(impi_pool, hss_impi_t);
static OGS_POOL(impu_pool, hss_impu_t);

static hss_imsi_t *imsi_add(char *id);
static void imsi_remove(hss_imsi_t *imsi);
static void imsi_remove_all(void);
static hss_imsi_t *imsi_find_by_id(char *id);

static hss_impi_t *impi_add(char *id);
static void impi_remove(hss_impi_t *impi);
static void impi_remove_all(void);

static hss_impi_t *impi_find_by_id(char *id);
static char *impi_get_server_name(hss_impi_t *impi);

static hss_impu_t *impu_add(hss_impi_t *impi, char *id);
static void impu_remove(hss_impu_t *impu);
static void impu_remove_all(hss_impi_t *impi);
static hss_impu_t *impu_find_by_id(char *id);
static hss_impu_t *impu_find_by_impi_and_id(hss_impi_t *impi, char *id);

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

    ogs_pool_init(&imsi_pool, ogs_app()->pool.impi);
    ogs_pool_init(&impi_pool, ogs_app()->pool.impi);
    ogs_pool_init(&impu_pool, ogs_app()->pool.impu);

    self.imsi_hash = ogs_hash_make();
    ogs_assert(self.imsi_hash);
    self.impi_hash = ogs_hash_make();
    ogs_assert(self.impi_hash);
    self.impu_hash = ogs_hash_make();
    ogs_assert(self.impu_hash);

    ogs_thread_mutex_init(&self.db_lock);
    ogs_thread_mutex_init(&self.cx_lock);

    context_initialized = 1;
}

void hss_context_final(void)
{
    ogs_assert(context_initialized == 1);

    imsi_remove_all();
    impi_remove_all();

    ogs_assert(self.imsi_hash);
    ogs_hash_destroy(self.imsi_hash);
    ogs_assert(self.impi_hash);
    ogs_hash_destroy(self.impi_hash);
    ogs_assert(self.impu_hash);
    ogs_hash_destroy(self.impu_hash);

    ogs_pool_final(&imsi_pool);
    ogs_pool_final(&impi_pool);
    ogs_pool_final(&impu_pool);

    ogs_thread_mutex_destroy(&self.db_lock);
    ogs_thread_mutex_destroy(&self.cx_lock);

    context_initialized = 0;
}

static int hss_context_prepare(void)
{
    self.diam_config->cnf_port = DIAMETER_PORT;
    self.diam_config->cnf_port_tls = DIAMETER_SECURE_PORT;
    self.diam_config->stats.priv_stats_size = sizeof(hss_diam_stats_t);

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
                                    int tc_timer = 0;

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
                } else if (!strcmp(hss_key, "diameter_stats_interval")) {
                    const char *v = ogs_yaml_iter_value(&hss_iter);
                    if (v) self.diam_config->stats.interval_sec = atoi(v);
                } else if (!strcmp(hss_key, "sms_over_ims")) {
                            self.sms_over_ims =
                                ogs_yaml_iter_value(&hss_iter);
                } else if (!strcmp(hss_key, "use_mongodb_change_stream")) {
#if MONGOC_CHECK_VERSION(1, 9, 0)
                    self.use_mongodb_change_stream =
                        ogs_yaml_iter_bool(&hss_iter);
#else
                    self.use_mongodb_change_stream = false;
#endif
                } else if (!strcmp(hss_key, "metrics")) {
                    /* handle config in metrics library */
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

int hss_db_update_sqn(char *imsi_bcd, uint8_t *rand, uint64_t sqn)
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

int hss_db_update_imeisv(char *imsi_bcd, char *imeisv)
{
    int rv;
    char *supi = NULL;

    ogs_assert(imsi_bcd);

    ogs_thread_mutex_lock(&self.db_lock);
    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
    ogs_assert(supi);

    rv = ogs_dbi_update_imeisv(supi, imeisv);

    ogs_free(supi);
    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int hss_db_update_mme(char *imsi_bcd, char *mme_host, char *mme_realm,
    bool purge_flag)
{
    int rv;
    char *supi = NULL;

    ogs_assert(imsi_bcd);

    ogs_thread_mutex_lock(&self.db_lock);
    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
    ogs_assert(supi);

    rv = ogs_dbi_update_mme(supi, mme_host, mme_realm, purge_flag);

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

int hss_db_msisdn_data(char *imsi_or_msisdn_bcd, ogs_msisdn_data_t *msisdn_data)
{
    int rv;

    ogs_assert(imsi_or_msisdn_bcd);
    ogs_assert(msisdn_data);

    ogs_thread_mutex_lock(&self.db_lock);

    rv = ogs_dbi_msisdn_data(imsi_or_msisdn_bcd, msisdn_data);

    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int hss_db_ims_data(char *imsi_bcd, ogs_ims_data_t *ims_data)
{
    int rv;
    char *supi = NULL;

    ogs_assert(imsi_bcd);
    ogs_assert(ims_data);

    ogs_thread_mutex_lock(&self.db_lock);
    supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
    ogs_assert(supi);

    rv = ogs_dbi_ims_data(supi, ims_data);

    ogs_free(supi);
    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

static hss_imsi_t *imsi_add(char *id)
{
    hss_imsi_t *imsi = NULL;

    ogs_assert(id);

    ogs_pool_alloc(&imsi_pool, &imsi);
    ogs_assert(imsi);
    memset(imsi, 0, sizeof *imsi);

    imsi->id = ogs_strdup(id);
    ogs_assert(imsi->id);

    ogs_hash_set(self.imsi_hash, imsi->id, strlen(imsi->id), imsi);

    ogs_list_add(&self.imsi_list, imsi);
    hss_metrics_inst_global_inc(HSS_METR_GLOB_GAUGE_IMSI);

    return imsi;
}

static void imsi_remove(hss_imsi_t *imsi)
{
    ogs_assert(imsi);

    ogs_list_remove(&self.imsi_list, imsi);
    hss_metrics_inst_global_dec(HSS_METR_GLOB_GAUGE_IMSI);

    ogs_assert(imsi->id);
    ogs_hash_set(self.imsi_hash, imsi->id, strlen(imsi->id), NULL);
    ogs_free(imsi->id);

    ogs_assert(imsi->visited_network_identifier);
    ogs_free(imsi->visited_network_identifier);

    ogs_pool_free(&imsi_pool, imsi);
}

static void imsi_remove_all(void)
{
    hss_imsi_t *imsi = NULL, *next = NULL;

    ogs_list_for_each_safe(&self.imsi_list, next, imsi)
        imsi_remove(imsi);
}

static hss_imsi_t *imsi_find_by_id(char *id)
{
    ogs_assert(id);
    return (hss_imsi_t *)ogs_hash_get(self.imsi_hash, id, strlen(id));
}

static hss_impi_t *impi_add(char *id)
{
    hss_impi_t *impi = NULL;

    ogs_assert(id);

    ogs_pool_alloc(&impi_pool, &impi);
    ogs_assert(impi);
    memset(impi, 0, sizeof *impi);

    impi->id = ogs_strdup(id);
    ogs_assert(impi->id);

    ogs_hash_set(self.impi_hash, impi->id, strlen(impi->id), impi);

    ogs_list_add(&self.impi_list, impi);
    hss_metrics_inst_global_inc(HSS_METR_GLOB_GAUGE_IMPI);

    return impi;
}

static void impi_remove(hss_impi_t *impi)
{
    ogs_assert(impi);

    ogs_list_remove(&self.impi_list, impi);
    hss_metrics_inst_global_dec(HSS_METR_GLOB_GAUGE_IMPI);

    impu_remove_all(impi);

    ogs_assert(impi->id);
    ogs_hash_set(self.impi_hash, impi->id, strlen(impi->id), NULL);
    ogs_free(impi->id);

    ogs_pool_free(&impi_pool, impi);
}

static void impi_remove_all(void)
{
    hss_impi_t *impi = NULL, *next = NULL;

    ogs_list_for_each_safe(&self.impi_list, next, impi)
        impi_remove(impi);
}

static hss_impi_t *impi_find_by_id(char *id)
{
    ogs_assert(id);
    return (hss_impi_t *)ogs_hash_get(self.impi_hash, id, strlen(id));
}

static char *impi_get_server_name(hss_impi_t *impi)
{
    hss_impu_t *impu = NULL;

    ogs_assert(impi);

    ogs_list_for_each(&impi->impu_list, impu) {
        if (impu->server_name)
            return impu->server_name;
    }

    return NULL;
}

static hss_impu_t *impu_add(hss_impi_t *impi, char *id)
{
    hss_impu_t *impu = NULL;

    ogs_assert(impi);
    ogs_assert(id);

    ogs_pool_alloc(&impu_pool, &impu);
    ogs_assert(impu);
    memset(impu, 0, sizeof *impu);

    impu->id = ogs_strdup(id);
    ogs_assert(impu->id);

    ogs_hash_set(self.impu_hash, impu->id, strlen(impu->id), impu);

    impu->impi = impi;

    ogs_list_add(&impi->impu_list, impu);
    hss_metrics_inst_global_inc(HSS_METR_GLOB_GAUGE_IMPU);

    return impu;
}

static void impu_remove(hss_impu_t *impu)
{
    hss_impi_t *impi = NULL;

    ogs_assert(impu);
    impi = impu->impi;
    ogs_assert(impi);

    ogs_list_remove(&impi->impu_list, impu);
    hss_metrics_inst_global_dec(HSS_METR_GLOB_GAUGE_IMPU);

    ogs_assert(impu->id);
    ogs_hash_set(self.impu_hash, impu->id, strlen(impu->id), NULL);
    ogs_free(impu->id);

    if (impu->server_name)
        ogs_free(impu->server_name);

    ogs_pool_free(&impu_pool, impu);
}

static void impu_remove_all(hss_impi_t *impi)
{
    hss_impu_t *impu = NULL, *next = NULL;

    ogs_list_for_each_safe(&impi->impu_list, next, impu)
        impu_remove(impu);
}

static hss_impu_t *impu_find_by_id(char *id)
{
    ogs_assert(id);
    return (hss_impu_t *)ogs_hash_get(self.impu_hash, id, strlen(id));
}

static hss_impu_t *impu_find_by_impi_and_id(hss_impi_t *impi, char *id)
{
    hss_impu_t *impu = NULL;

    ogs_assert(impi);
    ogs_assert(id);

    ogs_list_for_each(&impi->impu_list, impu) {
        if (!strcmp(impu->id, id))
            return impu;
    }

    return NULL;
}

void hss_cx_associate_identity(char *user_name, char *public_identity)
{
    hss_impi_t *impi = NULL;
    hss_impu_t *impu = NULL;

    ogs_assert(user_name);
    ogs_assert(public_identity);

    ogs_thread_mutex_lock(&self.cx_lock);

    impi = impi_find_by_id(user_name);
    if (!impi) {
        impi = impi_add(user_name);
        ogs_assert(impi);
    }

    impu = impu_find_by_id(public_identity);
    if (!impu) {
        impu = impu_add(impi, public_identity);
        ogs_assert(impu);
    }

    ogs_thread_mutex_unlock(&self.cx_lock);
}

bool hss_cx_identity_is_associated(char *user_name, char *public_identity)
{
    bool match_result = false;

    hss_impi_t *impi = NULL;
    hss_impu_t *impu = NULL;

    ogs_thread_mutex_lock(&self.cx_lock);

    impi = impi_find_by_id(user_name);
    if (impi) {
        impu = impu_find_by_impi_and_id(impi, public_identity);
        if (impu) {
            match_result = true;
        }
    }

    ogs_thread_mutex_unlock(&self.cx_lock);

    return match_result;
}

void hss_cx_set_imsi_bcd(char *user_name,
        char *imsi_bcd, char *visited_network_identifier)
{
    hss_imsi_t *imsi = NULL;
    hss_impi_t *impi = NULL;

    ogs_assert(user_name);
    ogs_assert(imsi_bcd);

    ogs_thread_mutex_lock(&self.cx_lock);

    impi = impi_find_by_id(user_name);
    ogs_assert(impi);

    imsi = imsi_find_by_id(imsi_bcd);
    if (!imsi) {
        imsi = imsi_add(imsi_bcd);
        ogs_assert(imsi);
    }

    impi->imsi = imsi;

    if (imsi->visited_network_identifier)
        ogs_free(imsi->visited_network_identifier);
    imsi->visited_network_identifier = ogs_strdup(visited_network_identifier);
    ogs_assert(imsi->visited_network_identifier);

    ogs_thread_mutex_unlock(&self.cx_lock);
}

char *hss_cx_get_imsi_bcd(char *public_identity)
{
    hss_impi_t *impi = NULL;
    hss_impu_t *impu = NULL;

    char *imsi_bcd = NULL;

    ogs_thread_mutex_lock(&self.cx_lock);

    impu = impu_find_by_id(public_identity);
    if (impu) {
        impi = impu->impi;
        ogs_assert(impi);

        if (impi->imsi)
            imsi_bcd = impi->imsi->id;
    }

    ogs_thread_mutex_unlock(&self.cx_lock);

    return imsi_bcd;
}

char *hss_cx_get_visited_network_identifier(char *public_identity)
{
    hss_impi_t *impi = NULL;
    hss_impu_t *impu = NULL;

    char *visited_network_identifier = NULL;

    ogs_thread_mutex_lock(&self.cx_lock);

    impu = impu_find_by_id(public_identity);
    if (impu) {
        impi = impu->impi;
        ogs_assert(impi);

        if (impi->imsi)
            visited_network_identifier = impi->imsi->visited_network_identifier;
    }

    ogs_thread_mutex_unlock(&self.cx_lock);

    return visited_network_identifier;
}

char *hss_cx_get_user_name(char *public_identity)
{
    hss_impi_t *impi = NULL;
    hss_impu_t *impu = NULL;

    char *user_name = NULL;

    ogs_thread_mutex_lock(&self.cx_lock);

    impu = impu_find_by_id(public_identity);
    if (impu) {
        impi = impu->impi;
        ogs_assert(impi);

        user_name = impi->id;
    }

    ogs_thread_mutex_unlock(&self.cx_lock);

    return user_name;
}

char *hss_cx_get_server_name(char *public_identity)
{
    char *server_name = NULL;

    hss_impi_t *impi = NULL;
    hss_impu_t *impu = NULL;

    ogs_thread_mutex_lock(&self.cx_lock);

    impu = impu_find_by_id(public_identity);
    if (impu) {
        server_name = impu->server_name;
        if (!server_name) {
            impi = impu->impi;
            ogs_assert(impi);

            server_name = impi_get_server_name(impi);
        }
    }

    ogs_thread_mutex_unlock(&self.cx_lock);

    return server_name;
}

void hss_cx_set_server_name(
        char *public_identity, char *server_name, bool overwrite)
{
    hss_impu_t *impu = NULL;

    ogs_thread_mutex_lock(&self.cx_lock);

    impu = impu_find_by_id(public_identity);
    ogs_assert(impu);

    if (!impu->server_name) {
        impu->server_name = ogs_strdup(server_name);
        ogs_assert(impu->server_name);
    } else {
        if (strcmp(impu->server_name, server_name) != 0) {
            if (overwrite == true) {
                ogs_warn("S-CSCF reassignment[%s->%s]",
                        impu->server_name, server_name);
                ogs_free(impu->server_name);
                impu->server_name = ogs_strdup(server_name);
                ogs_assert(impu->server_name);
            } else {
                ogs_error("Use Old S-CSCF[%s!=%s]",
                        server_name, impu->server_name);
            }
        }
    }

    ogs_thread_mutex_unlock(&self.cx_lock);
}

char *hss_cx_download_user_data(
        char *user_name, char *visited_network_identifier,
        ogs_ims_data_t *ims_data)
{
    char *user_data = NULL;

    hss_impi_t *impi = NULL;
    hss_impu_t *impu = NULL;

    bool barring_indication_presence = true;
    int i;

    ogs_assert(user_name);
    ogs_assert(visited_network_identifier);
    ogs_assert(ims_data);

    /* Download User-Data */
    for (i = 0; i < ims_data->num_of_msisdn; i++) {
        char *public_identity = NULL;

        public_identity = ogs_msprintf("sip:%s@%s",
                ims_data->msisdn[i].bcd, visited_network_identifier);
        ogs_assert(public_identity);
        hss_cx_associate_identity(user_name, public_identity);
        ogs_free(public_identity);

        public_identity = ogs_msprintf("tel:%s", ims_data->msisdn[i].bcd);
        ogs_assert(public_identity);
        hss_cx_associate_identity(user_name, public_identity);
        ogs_free(public_identity);
    }


    /* Generate XML Data */

    ogs_thread_mutex_lock(&self.cx_lock);

    user_data = ogs_strdup(ogs_diam_cx_xml_version);
    ogs_assert(user_data);

    user_data = ogs_mstrcatf(user_data, "%s",
                ogs_diam_cx_xml_ims_subscription_s);
    ogs_assert(user_data);

      user_data = ogs_mstrcatf(user_data, "%s%s%s",
                  ogs_diam_cx_xml_private_id_s,
                  user_name,
                  ogs_diam_cx_xml_private_id_e);
      ogs_assert(user_data);

      user_data = ogs_mstrcatf(user_data, "%s",
                  ogs_diam_cx_xml_service_profile_s);
      ogs_assert(user_data);

        impi = impi_find_by_id(user_name);
        ogs_assert(impi);

        barring_indication_presence = true;

        ogs_list_for_each(&impi->impu_list, impu) {
            user_data = ogs_mstrcatf(user_data, "%s",
                        ogs_diam_cx_xml_public_id_s);
            ogs_assert(user_data);

              if (barring_indication_presence == true) {
                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_barring_indication_s,
                              "1",
                              ogs_diam_cx_xml_barring_indication_e);
                  ogs_assert(user_data);
                  barring_indication_presence = false;
              }

              user_data = ogs_mstrcatf(user_data, "%s%s%s",
                          ogs_diam_cx_xml_identity_s,
                          impu->id,
                          ogs_diam_cx_xml_identity_e);
              ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_extension_s);
              ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s%s%s",
                            ogs_diam_cx_xml_identity_type_s,
                            "0",
                            ogs_diam_cx_xml_identity_type_e);
                ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_extension_e);
              ogs_assert(user_data);

            user_data = ogs_mstrcatf(user_data, "%s",
                        ogs_diam_cx_xml_public_id_e);
            ogs_assert(user_data);
        }

        if(self.sms_over_ims) {
            user_data = ogs_mstrcatf(user_data, "%s",
                        ogs_diam_cx_xml_ifc_s);
            ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s%s%s",
                          ogs_diam_cx_xml_priority_s,
                          "2",
                          ogs_diam_cx_xml_priority_e);
              ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_tp_s);
              ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s%s%s",
                            ogs_diam_cx_xml_cnf_s,
                            "1",
                            ogs_diam_cx_xml_cnf_e);
                ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s",
                            ogs_diam_cx_xml_spt_s);
                ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_condition_negated_s,
                              "0",
                              ogs_diam_cx_xml_condition_negated_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_group_s,
                              "1",
                              ogs_diam_cx_xml_group_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_method_s,
                              "MESSAGE",
                              ogs_diam_cx_xml_method_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s",
                              ogs_diam_cx_xml_extension_s);
                  ogs_assert(user_data);

                    user_data = ogs_mstrcatf(user_data, "%s%s%s",
                                ogs_diam_cx_xml_registration_type_s,
                                "0",
                                ogs_diam_cx_xml_registration_type_e);
                    ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s",
                              ogs_diam_cx_xml_extension_e);
                  ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s",
                            ogs_diam_cx_xml_spt_e);
                ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s",
                            ogs_diam_cx_xml_spt_s);
                ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_condition_negated_s,
                              "0",
                              ogs_diam_cx_xml_condition_negated_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_group_s,
                              "2",
                              ogs_diam_cx_xml_group_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s",
                              ogs_diam_cx_xml_sip_hdr_s);
                  ogs_assert(user_data);

                    user_data = ogs_mstrcatf(user_data, "%s%s%s",
                                ogs_diam_cx_xml_header_s,
                                "Content-Type",
                                ogs_diam_cx_xml_header_e);
                    ogs_assert(user_data);

                    user_data = ogs_mstrcatf(user_data, "%s%s%s",
                                ogs_diam_cx_xml_content_s,
                                "application/vnd.3gpp.sms",
                                ogs_diam_cx_xml_content_e);
                    ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s",
                              ogs_diam_cx_xml_sip_hdr_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s",
                              ogs_diam_cx_xml_extension_s);
                  ogs_assert(user_data);

                    user_data = ogs_mstrcatf(user_data, "%s%s%s",
                                ogs_diam_cx_xml_registration_type_s,
                                "0",
                                ogs_diam_cx_xml_registration_type_e);
                    ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s",
                              ogs_diam_cx_xml_extension_e);
                  ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s",
                            ogs_diam_cx_xml_spt_e);
                ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s",
                            ogs_diam_cx_xml_spt_s);
                ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_condition_negated_s,
                              "0",
                              ogs_diam_cx_xml_condition_negated_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_group_s,
                              "3",
                              ogs_diam_cx_xml_group_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s%s%s",
                              ogs_diam_cx_xml_session_case_s,
                              "0",
                              ogs_diam_cx_xml_session_case_e);
                  ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s",
                              ogs_diam_cx_xml_extension_s);
                  ogs_assert(user_data);

                    user_data = ogs_mstrcatf(user_data, "%s%s%s",
                                ogs_diam_cx_xml_registration_type_s,
                                "0",
                                ogs_diam_cx_xml_registration_type_e);
                    ogs_assert(user_data);

                  user_data = ogs_mstrcatf(user_data, "%s",
                              ogs_diam_cx_xml_extension_e);
                  ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s",
                            ogs_diam_cx_xml_spt_e);
                ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_tp_e);
              ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_app_server_s);
              ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s%s%s",
                            ogs_diam_cx_xml_server_name_s,
                            self.sms_over_ims,
                            ogs_diam_cx_xml_server_name_e);
                ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s%s%s",
                            ogs_diam_cx_xml_default_handling_s,
                            "0",
                            ogs_diam_cx_xml_default_handling_e);
                ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_app_server_e);
              ogs_assert(user_data);

            user_data = ogs_mstrcatf(user_data, "%s",
                        ogs_diam_cx_xml_ifc_e);
            ogs_assert(user_data);
        }

      user_data = ogs_mstrcatf(user_data, "%s",
                  ogs_diam_cx_xml_service_profile_e);
      ogs_assert(user_data);

    user_data = ogs_mstrcatf(user_data,
                "%s", ogs_diam_cx_xml_ims_subscription_e);
    ogs_assert(user_data);

    ogs_thread_mutex_unlock(&self.cx_lock);

    return user_data;
}

static int poll_change_stream(void);
static int process_change_stream(const bson_t *document);

int hss_db_poll_change_stream(void)
{
    int rv;

    ogs_thread_mutex_lock(&self.db_lock);

    rv = poll_change_stream();

    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

static int poll_change_stream(void)
{
#if MONGOC_CHECK_VERSION(1, 9, 0)
    int rv;

    const bson_t *document;
    const bson_t *err_document;
    bson_error_t error;

    while (mongoc_change_stream_next(ogs_mongoc()->stream, &document)) {
        rv = process_change_stream(document);
        if (rv != OGS_OK) return rv;
    }

    if (mongoc_change_stream_error_document(ogs_mongoc()->stream, &error,
            &err_document)) {
        if (!bson_empty (err_document)) {
            ogs_debug("Server Error: %s\n",
            bson_as_relaxed_extended_json(err_document, NULL));
        } else {
            ogs_debug("Client Error: %s\n", error.message);
        }
        return OGS_ERROR;
    }

    return OGS_OK;
# else
    return OGS_ERROR;
#endif
}

static int process_change_stream(const bson_t *document)
{
    int rv;

    hss_event_t *e = NULL;

    e = hss_event_new(HSS_EVENT_DBI_MESSAGE);
    ogs_assert(e);
    e->dbi.document = bson_copy(document);
    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        bson_destroy(e->dbi.document);
        hss_event_free(e);
    } else {
        ogs_pollset_notify(ogs_app()->pollset);
    }

    return OGS_OK;
}

int hss_handle_change_event(const bson_t *document)
{
    bson_iter_t iter, child1_iter, child2_iter;

    char *utf8 = NULL;
    uint32_t length = 0;

    bool send_clr_flag = false;
    bool send_idr_flag = false;
    uint32_t subdatamask = 0;

    char *imsi_bcd = NULL;

#if BSON_MAJOR_VERSION >= 1 && BSON_MINOR_VERSION >= 7
    char *as_json = bson_as_relaxed_extended_json(document, NULL);
    ogs_debug("Received change stream document: %s\n", as_json);
    bson_free (as_json);
# else
    ogs_debug("Received change stream document.");
#endif
    if (!bson_iter_init_find(&iter, document, "fullDocument")) {
        ogs_error("No 'imsi' field in this document.");
        return OGS_ERROR;
    } else {
        bson_iter_recurse(&iter, &child1_iter);
        while (bson_iter_next(&child1_iter)) {
            const char *key = bson_iter_key(&child1_iter);
            if (!strcmp(key, "imsi") &&
                    BSON_ITER_HOLDS_UTF8(&child1_iter)) {
                utf8 = (char *)bson_iter_utf8(&child1_iter, &length);
                imsi_bcd = ogs_strndup(utf8,
                    ogs_min(length, OGS_MAX_IMSI_BCD_LEN) + 1);
                ogs_assert(imsi_bcd);
            }
        }
    }

    if (!imsi_bcd) {
        ogs_error("No 'imsi' field in this document.");
        return OGS_ERROR;
    }

    if (bson_iter_init_find(&iter, document, "updateDescription")) {
        bson_iter_recurse(&iter, &child1_iter);
        while (bson_iter_next(&child1_iter)) {
            const char *key = bson_iter_key(&child1_iter);
            if (!strcmp(key, "updatedFields") &&
                    BSON_ITER_HOLDS_DOCUMENT(&child1_iter)) {
                bson_iter_recurse(&child1_iter, &child2_iter);
                while (bson_iter_next(&child2_iter)) {
                    const char *child2_key = bson_iter_key(&child2_iter);
                    if (!strcmp(child2_key,
                            "request_cancel_location") &&
                            BSON_ITER_HOLDS_BOOL(&child2_iter)) {
                        send_clr_flag = (char *)bson_iter_bool(&child2_iter);
                    } else if (!strncmp(child2_key,
                                OGS_ACCESS_RESTRICTION_DATA_STRING,
                                strlen(OGS_ACCESS_RESTRICTION_DATA_STRING))) {
                        send_idr_flag = true;
                        subdatamask = (subdatamask | OGS_DIAM_S6A_SUBDATA_ARD);
                    } else if (!strncmp(child2_key,
                                OGS_SUBSCRIBER_STATUS_STRING,
                                strlen(OGS_SUBSCRIBER_STATUS_STRING))) {
                        send_idr_flag = true;
                        subdatamask = (subdatamask |
                            OGS_DIAM_S6A_SUBDATA_SUB_STATUS);
                    } else if (!strncmp(child2_key,
                                OGS_OPERATOR_DETERMINED_BARRING_STRING,
                            strlen(OGS_OPERATOR_DETERMINED_BARRING_STRING))) {
                        send_idr_flag = true;
                        subdatamask = (subdatamask |
                            OGS_DIAM_S6A_SUBDATA_OP_DET_BARRING);
                    } else if (!strncmp(child2_key,
                                OGS_NETWORK_ACCESS_MODE_STRING,
                                strlen(OGS_NETWORK_ACCESS_MODE_STRING))) {
                        send_idr_flag = true;
                        subdatamask = (subdatamask | OGS_DIAM_S6A_SUBDATA_NAM);
                    } else if (!strncmp(child2_key, "ambr", strlen("ambr"))) {
                        send_idr_flag = true;
                        subdatamask = (subdatamask |
                            OGS_DIAM_S6A_SUBDATA_UEAMBR);
                    } else if (!strncmp(child2_key,
                                OGS_SUBSCRIBED_RAU_TAU_TIMER_STRING,
                                strlen(OGS_SUBSCRIBED_RAU_TAU_TIMER_STRING))) {
                        send_idr_flag = true;
                        subdatamask = (subdatamask |
                            OGS_DIAM_S6A_SUBDATA_RAU_TAU_TIMER);
                    } else if (!strncmp(child2_key, "slice", strlen("slice"))) {
                        send_idr_flag = true;
                        subdatamask = (subdatamask |
                            OGS_DIAM_S6A_SUBDATA_APN_CONFIG);
                    }
                }
            }
        }
    } else {
        ogs_debug("No 'updateDescription' field in this document");
    }

    if (send_clr_flag) {
        ogs_info("[%s] Cancel Location Requested", imsi_bcd);
        hss_s6a_send_clr(imsi_bcd, NULL, NULL,
            OGS_DIAM_S6A_CT_SUBSCRIPTION_WITHDRAWL);
    } else if (send_idr_flag) {
        ogs_info("[%s] Subscription-Data Changed", imsi_bcd);
        hss_s6a_send_idr(imsi_bcd, 0, subdatamask);
    }

    ogs_free(imsi_bcd);

    return OGS_OK;
}
