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
#include "pcrf-context.h"

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
    
    return OGS_OK;
}

static int pcrf_context_validation(void)
{
    if (self.diam_conf_path == NULL &&
        (self.diam_config->cnf_diamid == NULL ||
        self.diam_config->cnf_diamrlm == NULL ||
        self.diam_config->cnf_addr == NULL)) {
        ogs_error("No pcrf.freeDiameter in '%s'",
                ogs_config()->file);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int pcrf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_config()->document;
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
                    ogs_warn("unknown key `%s`", pcrf_key);
            }
        }
    }

    rv = pcrf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

int pcrf_db_init()
{
    int rv;

    rv = ogs_mongoc_init(ogs_config()->db_uri);
    if (rv != OGS_OK) return rv;

    if (ogs_mongoc()->client && ogs_mongoc()->name) {
        self.subscriberCollection = mongoc_client_get_collection(
            ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
        ogs_assert(self.subscriberCollection);
    }

    return OGS_OK;
}

int pcrf_db_final()
{
    if (self.subscriberCollection) {
        mongoc_collection_destroy(self.subscriberCollection);
    }

    ogs_mongoc_final();

    return OGS_OK;
}

int pcrf_db_qos_data(char *imsi_bcd, char *apn,
        ogs_diam_gx_message_t *gx_message)
{
    int rv = OGS_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_t *opts = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter, child2_iter, child3_iter;
    bson_iter_t child4_iter, child5_iter, child6_iter;
    const char *utf8 = NULL;
    uint32_t length = 0;

    ogs_assert(imsi_bcd);
    ogs_assert(apn);
    ogs_assert(gx_message);

    ogs_thread_mutex_lock(&self.db_lock);

    query = BCON_NEW(
            "imsi", BCON_UTF8(imsi_bcd),
            "pdn.apn", BCON_UTF8(apn));
#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 5
    opts = BCON_NEW(
            "projection", "{",
                "imsi", BCON_INT64(1),
                "pdn.$", BCON_INT64(1),
            "}"
            );
    cursor = mongoc_collection_find_with_opts(
            self.subscriberCollection, query, opts, NULL);
#else
    opts = BCON_NEW(
            "imsi", BCON_INT64(1),
            "pdn.$", BCON_INT64(1)
            );
    cursor = mongoc_collection_find(self.subscriberCollection,
            MONGOC_QUERY_NONE, 0, 0, 0, query, opts, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        ogs_error("Cannot find IMSI(%s)+APN(%s) in DB", imsi_bcd, apn);

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

    while (bson_iter_next(&iter)) {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "pdn") &&
            BSON_ITER_HOLDS_ARRAY(&iter)) {
            int pdn_index = 0;

            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                const char *child1_key = bson_iter_key(&child1_iter);
                ogs_pdn_t *pdn = NULL;

                ogs_assert(child1_key);
                pdn_index = atoi(child1_key);
                ogs_assert(pdn_index == 0);

                pdn = &gx_message->pdn;
                bson_iter_recurse(&child1_iter, &child2_iter);
                while (bson_iter_next(&child2_iter)) {
                    const char *child2_key = bson_iter_key(&child2_iter);
                    if (!strcmp(child2_key, "apn") &&
                        BSON_ITER_HOLDS_UTF8(&child2_iter)) {
                        utf8 = bson_iter_utf8(&child2_iter, &length);
                        ogs_cpystrn(pdn->apn, utf8,
                            ogs_min(length, OGS_MAX_APN_LEN)+1);
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
                    } else if (!strcmp(child2_key, "pcc_rule") &&
                        BSON_ITER_HOLDS_ARRAY(&child2_iter)) {
                        int pcc_rule_index = 0;

                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while (bson_iter_next(&child3_iter)) {
                            const char *child3_key =
                                bson_iter_key(&child3_iter);
                            ogs_pcc_rule_t *pcc_rule = NULL;

                            ogs_assert(child3_key);
                            pcc_rule_index = atoi(child3_key);
                            ogs_assert(pcc_rule_index <
                                    OGS_MAX_NUM_OF_PCC_RULE);

                            pcc_rule = &gx_message->pcc_rule[pcc_rule_index];
                            bson_iter_recurse(&child3_iter, &child4_iter);
                            while (bson_iter_next(&child4_iter)) {
                                const char *child4_key =
                                    bson_iter_key(&child4_iter);

                                if (!strcmp(child4_key, "qos") &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key, "qci") &&
                                            BSON_ITER_HOLDS_INT32(
                                                &child5_iter)) {
                                            pcc_rule->qos.qci =
                                                bson_iter_int32(&child5_iter);
                                        } else if (!strcmp(child5_key, "arp") &&
                                            BSON_ITER_HOLDS_DOCUMENT(
                                                &child5_iter)) {
                                            bson_iter_recurse(
                                                &child5_iter, &child6_iter);
                                            while (bson_iter_next(
                                                        &child6_iter)) {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                        "priority_level") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    pcc_rule->qos.arp.
                                                        priority_level =
                                                        bson_iter_int32(
                                                            &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                    "pre_emption_capability") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    pcc_rule->qos.arp.
                                                    pre_emption_capability =
                                                        bson_iter_int32(
                                                            &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                    "pre_emption_vulnerability")
                                                    && BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    pcc_rule->qos.arp.
                                                    pre_emption_vulnerability =
                                                        bson_iter_int32(
                                                            &child6_iter);
                                                }
                                            }
                                        } else if (!strcmp(child5_key, "mbr") &&
                                            BSON_ITER_HOLDS_DOCUMENT(
                                                &child5_iter)) {
                                            bson_iter_recurse(
                                                &child5_iter, &child6_iter);
                                            while (bson_iter_next(
                                                        &child6_iter)) {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                        "downlink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter)) {
                                                    pcc_rule->qos.mbr.downlink =
                                                        bson_iter_int64(
                                                        &child6_iter) * 1024;
                                                } else if (!strcmp(child6_key,
                                                        "uplink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter)) {
                                                    pcc_rule->qos.mbr.uplink =
                                                        bson_iter_int64(
                                                        &child6_iter) * 1024;
                                                }
                                            }
                                        } else if (!strcmp(child5_key, "gbr") &&
                                            BSON_ITER_HOLDS_DOCUMENT(
                                                &child5_iter)) {
                                            bson_iter_recurse(&child5_iter,
                                                &child6_iter);
                                            while (bson_iter_next(
                                                        &child6_iter)) {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                        "downlink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter)) {
                                                    pcc_rule->qos.gbr.downlink =
                                                        bson_iter_int64(
                                                        &child6_iter) * 1024;
                                                } else if (!strcmp(child6_key,
                                                        "uplink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter)) {
                                                    pcc_rule->qos.gbr.uplink =
                                                        bson_iter_int64(
                                                        &child6_iter) * 1024;
                                                }
                                            }
                                        }
                                    }
                                } else if (!strcmp(child4_key, "flow") &&
                                    BSON_ITER_HOLDS_ARRAY(&child4_iter)) {
                                    int flow_index = 0;

                                    bson_iter_recurse(&child4_iter,
                                        &child5_iter);
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        ogs_flow_t *flow = NULL;

                                        ogs_assert(child5_key);
                                        flow_index = atoi(child5_key);
                                        ogs_assert(
                                            flow_index < OGS_MAX_NUM_OF_FLOW);

                                        flow = &pcc_rule->flow[flow_index];
                                        bson_iter_recurse(
                                            &child5_iter, &child6_iter);
                                        while (bson_iter_next(&child6_iter)) {
                                            const char *child6_key =
                                                bson_iter_key(&child6_iter);
                                            if (!strcmp(child6_key,
                                                    "direction") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child6_iter)) {
                                                flow->direction =
                                                    bson_iter_int32(
                                                        &child6_iter);
                                            } else if (!strcmp(child6_key,
                                                        "description") &&
                                                BSON_ITER_HOLDS_UTF8(
                                                    &child6_iter)) {
                                                utf8 = bson_iter_utf8(
                                                        &child6_iter, &length);
                                                flow->description =
                                                    ogs_malloc(length+1);
                                                ogs_cpystrn(
                                                    (char*)flow->description,
                                                    utf8, length+1);
                                            }
                                        }
                                        flow_index++;
                                    }
                                    pcc_rule->num_of_flow = flow_index;
                                }
                            }
                            /* Charing-Rule-Name is automatically configured */
                            if (pcc_rule->name) {
                                ogs_error("PCC Rule Name has already "
                                        "been defined");
                                ogs_free(pcc_rule->name);
                            }
                            pcc_rule->name = ogs_calloc(
                                    1, OGS_MAX_PCC_RULE_NAME_LEN);
                            ogs_assert(pcc_rule->name);
                            snprintf(pcc_rule->name, OGS_MAX_PCC_RULE_NAME_LEN,
                                    "%s%d", apn, pcc_rule_index+1);
                            pcc_rule->precedence = pcc_rule_index+1;
                            pcc_rule->flow_status = OGS_FLOW_STATUS_ENABLED;
                            pcc_rule_index++;
                        }
                        gx_message->num_of_pcc_rule = pcc_rule_index;
                    }
                }
            }
        }
    }

out:
    if (query) bson_destroy(query);
    if (opts) bson_destroy(opts);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

int pcrf_sess_set_ipv4(const void *key, uint8_t *sid)
{
    ogs_assert(self.ip_hash);

    ogs_thread_mutex_lock(&self.hash_lock);

    ogs_hash_set(self.ip_hash, key, OGS_IPV4_LEN, sid);

    ogs_thread_mutex_unlock(&self.hash_lock);

    return OGS_OK;
}
int pcrf_sess_set_ipv6(const void *key, uint8_t *sid)
{
    ogs_assert(self.ip_hash);

    ogs_thread_mutex_lock(&self.hash_lock);

    ogs_hash_set(self.ip_hash, key, OGS_IPV6_LEN, sid);

    ogs_thread_mutex_unlock(&self.hash_lock);

    return OGS_OK;
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

    sid = (uint8_t *)ogs_hash_get(self.ip_hash, key, OGS_IPV6_LEN);

    ogs_thread_mutex_unlock(&self.hash_lock);

    return sid;
}

