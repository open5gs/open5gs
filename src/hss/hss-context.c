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

#include <mongoc.h>
#include <yaml.h>

#include "fd/fd-lib.h"

#include "app/context.h"
#include "hss-context.h"

static hss_context_t self;
static fd_config_t g_fd_conf;

int __hss_log_domain;

static int context_initialized = 0;

hss_context_t* hss_self()
{
    return &self;
}

void hss_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initial FreeDiameter Config */
    memset(&g_fd_conf, 0, sizeof(fd_config_t));

    /* Initialize HSS context */
    memset(&self, 0, sizeof(hss_context_t));
    self.fd_config = &g_fd_conf;

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

static int hss_context_prepare()
{
    self.fd_config->cnf_port = DIAMETER_PORT;
    self.fd_config->cnf_port_tls = DIAMETER_SECURE_PORT;

    return OGS_OK;
}

static int hss_context_validation()
{
    if (self.fd_conf_path == NULL &&
        (self.fd_config->cnf_diamid == NULL ||
        self.fd_config->cnf_diamrlm == NULL ||
        self.fd_config->cnf_addr == NULL)) {
        ogs_error("No hss.freeDiameter in '%s'", context_self()->config.file);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int hss_context_parse_config()
{
    int rv;
    config_t *config = &context_self()->config;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    ogs_assert(config);
    document = config->document;
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
                        self.fd_conf_path = ogs_yaml_iter_value(&hss_iter);
                    } else if (node->type == YAML_MAPPING_NODE) {
                        ogs_yaml_iter_t fd_iter;
                        ogs_yaml_iter_recurse(&hss_iter, &fd_iter);

                        while (ogs_yaml_iter_next(&fd_iter)) {
                            const char *fd_key = ogs_yaml_iter_key(&fd_iter);
                            ogs_assert(fd_key);
                            if (!strcmp(fd_key, "identity")) {
                                self.fd_config->cnf_diamid = 
                                    ogs_yaml_iter_value(&fd_iter);
                            } else if (!strcmp(fd_key, "realm")) {
                                self.fd_config->cnf_diamrlm = 
                                    ogs_yaml_iter_value(&fd_iter);
                            } else if (!strcmp(fd_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&fd_iter);
                                if (v) self.fd_config->cnf_port = atoi(v);
                            } else if (!strcmp(fd_key, "sec_port")) {
                                const char *v = ogs_yaml_iter_value(&fd_iter);
                                if (v) self.fd_config->cnf_port_tls = atoi(v);
                            } else if (!strcmp(fd_key, "listen_on")) {
                                self.fd_config->cnf_addr = 
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
                                        self.fd_config->
                                            ext[self.fd_config->num_of_ext].
                                                module = module;
                                        self.fd_config->
                                            ext[self.fd_config->num_of_ext].
                                                conf = conf;
                                        self.fd_config->num_of_ext++;
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
                                    } else if (ogs_yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&conn_array))
                                            break;
                                        ogs_yaml_iter_recurse(&conn_array, &conn_iter);
                                    } else if (ogs_yaml_iter_type(&conn_array) ==
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
                                            addr = ogs_yaml_iter_value(&conn_iter);
                                        } else if (!strcmp(conn_key, "port")) {
                                            const char *v =
                                                ogs_yaml_iter_value(&conn_iter);
                                            if (v) port = atoi(v);
                                        } else
                                            ogs_warn("unknown key `%s`", conn_key);
                                    }

                                    if (identity && addr) {
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                identity = identity;
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                addr = addr;
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                port = port;
                                        self.fd_config->num_of_conn++;
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

int hss_db_init()
{
    if (context_self()->db.client && context_self()->db.name) {
        self.subscriberCollection = mongoc_client_get_collection(
            context_self()->db.client, 
            context_self()->db.name, "subscribers");
        ogs_assert(self.subscriberCollection);
    }

    return OGS_OK;
}

int hss_db_final()
{
    if (self.subscriberCollection) {
        mongoc_collection_destroy(self.subscriberCollection);
    }

    return OGS_OK;
}

int hss_db_auth_info(
    char *imsi_bcd, hss_db_auth_info_t *auth_info)
{
    int rv = OGS_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t inner_iter;
    char buf[HSS_KEY_LEN];
    char *utf8 = NULL;
    uint32_t length = 0;

    ogs_assert(imsi_bcd);
    ogs_assert(auth_info);

    ogs_thread_mutex_lock(&self.db_lock);

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 5
    cursor = mongoc_collection_find_with_opts(
            self.subscriberCollection, query, NULL, NULL);
#else
    cursor = mongoc_collection_find(self.subscriberCollection,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        ogs_warn("Cannot find IMSI in DB : %s", imsi_bcd);

        rv = OGS_ERROR;
        goto out;
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cursor Failure: %s", error.message);

        rv = OGS_ERROR;
        goto out;
    }

    if (!bson_iter_init_find(&iter, document, "security")) {
        ogs_error("No 'security' field in this document");

        rv = OGS_ERROR;
        goto out;
    }

    memset(auth_info, 0, sizeof(hss_db_auth_info_t));
    bson_iter_recurse(&iter, &inner_iter);
    while (bson_iter_next(&inner_iter)) {
        const char *key = bson_iter_key(&inner_iter);

        if (!strcmp(key, "k") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->k, OGS_HEX(utf8, length, buf), HSS_KEY_LEN);
        } else if (!strcmp(key, "opc") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            auth_info->use_opc = 1;
            memcpy(auth_info->opc, OGS_HEX(utf8, length, buf), HSS_KEY_LEN);
        } else if (!strcmp(key, "op") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->op, OGS_HEX(utf8, length, buf), HSS_KEY_LEN);
        } else if (!strcmp(key, "amf") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->amf, OGS_HEX(utf8, length, buf), HSS_AMF_LEN);
        } else if (!strcmp(key, "rand") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->rand, OGS_HEX(utf8, length, buf), RAND_LEN);
        } else if (!strcmp(key, "sqn") && BSON_ITER_HOLDS_INT64(&inner_iter)) {
            auth_info->sqn = bson_iter_int64(&inner_iter);
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int hss_db_update_rand_and_sqn(
    char *imsi_bcd, uint8_t *rand, uint64_t sqn)
{
    int rv = OGS_OK;
    bson_t *query = NULL;
    bson_t *update = NULL;
    bson_error_t error;
    char printable_rand[128];

    ogs_assert(rand);
    ogs_hex_to_ascii(rand, RAND_LEN, printable_rand, sizeof(printable_rand));

    ogs_thread_mutex_lock(&self.db_lock);

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    update = BCON_NEW("$set",
            "{",
                "security.rand", printable_rand,
                "security.sqn", BCON_INT64(sqn),
            "}");

    if (!mongoc_collection_update(self.subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
        ogs_error("mongoc_collection_update() failure: %s", error.message);

        rv = OGS_ERROR;
    }

    if (query) bson_destroy(query);
    if (update) bson_destroy(update);

    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int hss_db_increment_sqn(char *imsi_bcd)
{
    int rv = OGS_OK;
    bson_t *query = NULL;
    bson_t *update = NULL;
    bson_error_t error;
    uint64_t max_sqn = HSS_MAX_SQN;

    ogs_thread_mutex_lock(&self.db_lock);

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    update = BCON_NEW("$inc",
            "{",
                "security.sqn", BCON_INT64(32),
            "}");
    if (!mongoc_collection_update(self.subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
        ogs_error("mongoc_collection_update() failure: %s", error.message);

        rv = OGS_ERROR;
        goto out;
    }
    bson_destroy(update);

    update = BCON_NEW("$bit",
            "{",
                "security.sqn", 
                "{", "and", BCON_INT64(max_sqn), "}",
            "}");
    if (!mongoc_collection_update(self.subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
        ogs_error("mongoc_collection_update() failure: %s", error.message);

        rv = OGS_ERROR;
    }

out:
    if (query) bson_destroy(query);
    if (update) bson_destroy(update);

    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int hss_db_subscription_data(
    char *imsi_bcd, s6a_subscription_data_t *subscription_data)
{
    int rv = OGS_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter, child2_iter, child3_iter, child4_iter;
    const char *utf8 = NULL;
    uint32_t length = 0;

    ogs_assert(imsi_bcd);
    ogs_assert(subscription_data);

    ogs_thread_mutex_lock(&self.db_lock);

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 5
    cursor = mongoc_collection_find_with_opts(
            self.subscriberCollection, query, NULL, NULL);
#else
    cursor = mongoc_collection_find(self.subscriberCollection,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        ogs_error("Cannot find IMSI in DB : %s", imsi_bcd);

        rv = OGS_ERROR;
        goto out;
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cursor Failure: %s", error.message);

        rv = OGS_ERROR;
        goto out;
    }

    if (!bson_iter_init(&iter, document)) {
        ogs_error("bson_iter_init failed in this document");

        rv = OGS_ERROR;
        goto out;
    }

    memset(subscription_data, 0, sizeof(s6a_subscription_data_t));
    while (bson_iter_next(&iter)) {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "access_restriction_data") &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->access_restriction_data =
                bson_iter_int32(&iter);

        } else if (!strcmp(key, "subscriber_status") &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->subscriber_status =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, "network_access_mode") &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->network_access_mode =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, "subscribed_rau_tau_timer") &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->subscribed_rau_tau_timer =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, "ambr") &&
            BSON_ITER_HOLDS_DOCUMENT(&iter)) {
            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                const char *child1_key = bson_iter_key(&child1_iter);
                if (!strcmp(child1_key, "uplink") &&
                    BSON_ITER_HOLDS_INT64(&child1_iter)) {
                    subscription_data->ambr.uplink =
                        bson_iter_int64(&child1_iter) * 1024;
                } else if (!strcmp(child1_key, "downlink") &&
                    BSON_ITER_HOLDS_INT64(&child1_iter)) {
                    subscription_data->ambr.downlink =
                        bson_iter_int64(&child1_iter) * 1024;
                }
            }
        } else if (!strcmp(key, "pdn") &&
            BSON_ITER_HOLDS_ARRAY(&iter)) {
            int pdn_index = 0;

            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                const char *child1_key = bson_iter_key(&child1_iter);
                pdn_t *pdn = NULL;

                ogs_assert(child1_key);
                pdn_index = atoi(child1_key);
                ogs_assert(pdn_index < MAX_NUM_OF_SESS);

                pdn = &subscription_data->pdn[pdn_index];

                bson_iter_recurse(&child1_iter, &child2_iter);
                while (bson_iter_next(&child2_iter)) {
                    const char *child2_key = bson_iter_key(&child2_iter);
                    if (!strcmp(child2_key, "apn") &&
                        BSON_ITER_HOLDS_UTF8(&child2_iter)) {
                        utf8 = bson_iter_utf8(&child2_iter, &length);
                        ogs_cpystrn(pdn->apn, utf8,
                            ogs_min(length, MAX_APN_LEN)+1);
                    } else if (!strcmp(child2_key, "type") &&
                        BSON_ITER_HOLDS_INT32(&child2_iter)) {
                        pdn->pdn_type = bson_iter_int32(&child2_iter);
                    } else if (!strcmp(child2_key, "qos") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child2_iter)) {
                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while (bson_iter_next(&child3_iter)) {
                            const char *child3_key =
                                bson_iter_key(&child3_iter);
                            if (!strcmp(child3_key, "qci") &&
                                BSON_ITER_HOLDS_INT32(&child3_iter)) {
                                pdn->qos.qci = bson_iter_int32(&child3_iter);
                            } else if (!strcmp(child3_key, "arp") &&
                                BSON_ITER_HOLDS_DOCUMENT(&child3_iter)) {
                                bson_iter_recurse(&child3_iter, &child4_iter);
                                while (bson_iter_next(&child4_iter)) {
                                    const char *child4_key =
                                        bson_iter_key(&child4_iter);
                                    if (!strcmp(child4_key, "priority_level") &&
                                        BSON_ITER_HOLDS_INT32(&child4_iter)) {
                                        pdn->qos.arp.priority_level =
                                            bson_iter_int32(&child4_iter);
                                    } else if (!strcmp(child4_key,
                                                "pre_emption_capability") &&
                                        BSON_ITER_HOLDS_INT32(&child4_iter)) {
                                        pdn->qos.arp.pre_emption_capability =
                                            bson_iter_int32(&child4_iter);
                                    } else if (!strcmp(child4_key,
                                                "pre_emption_vulnerability") &&
                                        BSON_ITER_HOLDS_INT32(&child4_iter)) {
                                        pdn->qos.arp.pre_emption_vulnerability =
                                            bson_iter_int32(&child4_iter);
                                    }
                                }
                            }
                        }
                    } else if (!strcmp(child2_key, "ambr") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child2_iter)) {
                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while (bson_iter_next(&child3_iter)) {
                            const char *child3_key =
                                bson_iter_key(&child3_iter);
                            if (!strcmp(child3_key, "uplink") &&
                                BSON_ITER_HOLDS_INT64(&child3_iter)) {
                                pdn->ambr.uplink =
                                    bson_iter_int64(&child3_iter) * 1024;
                            } else if (!strcmp(child3_key, "downlink") &&
                                BSON_ITER_HOLDS_INT64(&child3_iter)) {
                                pdn->ambr.downlink =
                                    bson_iter_int64(&child3_iter) * 1024;
                            }
                        }
                    } else if (!strcmp(child2_key, "pgw") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child2_iter)) {
                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while (bson_iter_next(&child3_iter)) {
                            const char *child3_key =
                                bson_iter_key(&child3_iter);
                            if (!strcmp(child3_key, "addr") &&
                                BSON_ITER_HOLDS_UTF8(&child3_iter)) {
                                ogs_ipsubnet_t ipsub;
                                const char *v = 
                                    bson_iter_utf8(&child3_iter, &length);
                                rv = ogs_ipsubnet(&ipsub, v, NULL);
                                if (rv == OGS_OK) {
                                    pdn->pgw_ip.ipv4 = 1;
                                    pdn->pgw_ip.both.addr = ipsub.sub[0];
                                }
                            } else if (!strcmp(child3_key, "addr6") &&
                                BSON_ITER_HOLDS_UTF8(&child3_iter)) {
                                ogs_ipsubnet_t ipsub;
                                const char *v = 
                                    bson_iter_utf8(&child3_iter, &length);
                                rv = ogs_ipsubnet(&ipsub, v, NULL);
                                if (rv == OGS_OK) {
                                    pdn->pgw_ip.ipv6 = 1;
                                    memcpy(pdn->pgw_ip.both.addr6,
                                            ipsub.sub, sizeof(ipsub.sub));
                                }
                            }
                        }
                    }
                }
                pdn_index++;
            }
            subscription_data->num_of_pdn = pdn_index;
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}
