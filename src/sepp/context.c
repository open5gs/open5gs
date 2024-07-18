/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

static sepp_context_t self;

int __sepp_log_domain;

static OGS_POOL(sepp_node_pool, sepp_node_t);
static OGS_POOL(sepp_assoc_pool, sepp_assoc_t);

static int context_initialized = 0;

static int max_num_of_sepp_node = 0;
static int max_num_of_sepp_assoc = 0;

void sepp_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize SEPP context */
    memset(&self, 0, sizeof(sepp_context_t));

    ogs_log_install_domain(&__sepp_log_domain, "sepp", ogs_core()->log.level);

    max_num_of_sepp_node = ogs_app()->pool.nf;
    ogs_pool_init(&sepp_node_pool, max_num_of_sepp_node);

#define MAX_NUM_OF_SEPP_ASSOC 8
    max_num_of_sepp_assoc = ogs_global_conf()->max.ue * MAX_NUM_OF_SEPP_ASSOC;
    ogs_pool_init(&sepp_assoc_pool, max_num_of_sepp_assoc);

    context_initialized = 1;
}

void sepp_context_final(void)
{
    ogs_assert(context_initialized == 1);

    ogs_sbi_nf_instance_remove_all();

    sepp_node_remove_all();
    sepp_assoc_remove_all();

    ogs_pool_final(&sepp_node_pool);
    ogs_pool_final(&sepp_assoc_pool);

    if (self.sender)
        ogs_free(self.sender);

    context_initialized = 0;
}

sepp_context_t *sepp_self(void)
{
    return &self;
}

static int sepp_context_prepare(void)
{
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_info_t *nf_info = NULL;
    ogs_sbi_sepp_info_t *sepp_info = NULL;

    /*********************************************************************
     * SEPP Port Configuration
     *********************************************************************/
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);

    nf_info = ogs_sbi_nf_info_add(
            &nf_instance->nf_info_list, OpenAPI_nf_type_SEPP);
    if (!nf_info) {
        ogs_error("ogs_sbi_nf_info_add() failed");
        return OGS_ERROR;
    }

    sepp_info = &nf_info->sepp;

    for (server = ogs_sbi_server_first();
            server; server = ogs_sbi_server_next(server)) {
        ogs_sockaddr_t *advertise = NULL;

        advertise = server->advertise;
        if (!advertise)
            advertise = server->node.addr;
        ogs_assert(advertise);

        if (server->scheme == OpenAPI_uri_scheme_https) {
            sepp_info->https.presence = true;
            sepp_info->https.port = OGS_PORT(advertise);
        } else if (server->scheme == OpenAPI_uri_scheme_http) {
            sepp_info->http.presence = true;
            sepp_info->http.port = OGS_PORT(advertise);
        } else {
            ogs_error("Unknown scheme[%d]", server->scheme);
            ogs_assert_if_reached();
        }
    }

    /*********************************************************************
     * Default Configuration
     *********************************************************************/
    self.security_capability.tls = true;
    self.target_apiroot_supported = true;

    return OGS_OK;
}

static int sepp_context_validation(void)
{
    if (sepp_self()->security_capability.tls == false &&
        sepp_self()->security_capability.prins == false) {
        ogs_error("No Security Capability [%d:%d] in '%s'",
                sepp_self()->security_capability.tls,
                sepp_self()->security_capability.prins,
                ogs_app()->file);
        return OGS_ERROR;
    }

    if (!sepp_self()->sender) {
        ogs_error("No n32.server.sender");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int sepp_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = sepp_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "sepp")) {
            ogs_yaml_iter_t sepp_iter;
            ogs_yaml_iter_recurse(&root_iter, &sepp_iter);
            while (ogs_yaml_iter_next(&sepp_iter)) {
                const char *sepp_key = ogs_yaml_iter_key(&sepp_iter);
                ogs_assert(sepp_key);
                if (!strcmp(sepp_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(sepp_key, "n32")) {
                    ogs_yaml_iter_t sbi_iter;
                    ogs_yaml_iter_recurse(&sepp_iter, &sbi_iter);
                    while (ogs_yaml_iter_next(&sbi_iter)) {
                        const char *sbi_key = ogs_yaml_iter_key(&sbi_iter);
                        ogs_assert(sbi_key);
                        if (!strcmp(sbi_key, "server")) {
                            const char *sender = NULL;
                            ogs_yaml_iter_t server_iter, server_array;
                            ogs_yaml_iter_recurse(&sbi_iter, &server_array);
                            do {
                                if (ogs_yaml_iter_type(&server_array) ==
                                        YAML_MAPPING_NODE) {
                                    memcpy(&server_iter, &server_array,
                                            sizeof(ogs_yaml_iter_t));
                                } else if (ogs_yaml_iter_type(&server_array) ==
                                        YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(&server_array))
                                        break;
                                    ogs_yaml_iter_recurse(
                                            &server_array, &server_iter);
                                } else if (ogs_yaml_iter_type(&server_array) ==
                                        YAML_SCALAR_NODE) {
                                    break;
                                } else
                                    ogs_assert_if_reached();

                                while (ogs_yaml_iter_next(&server_iter)) {
                                    const char *server_key =
                                        ogs_yaml_iter_key(&server_iter);
                                    ogs_assert(server_key);
                                    if (!strcmp(server_key, "sender")) {
                                        sender =
                                            ogs_yaml_iter_value(&server_iter);
                                    }
                                }
                            } while (ogs_yaml_iter_type(&server_array) ==
                                    YAML_SEQUENCE_NODE);

                            if (!sender) {
                                ogs_error("No n32.server.sender");
                                return OGS_ERROR;
                            }

                            if (self.sender)
                                ogs_free(self.sender);
                            self.sender = ogs_strdup(sender);
                            if (!self.sender) {
                                ogs_error("No memory for sender");
                                return OGS_ERROR;
                            }

                            rv = ogs_sbi_context_parse_server_config(
                                    &sbi_iter, OGS_SBI_INTERFACE_NAME_SEPP);
                            if (rv != OGS_OK) {
                                ogs_error("ogs_sbi_context_parse_server_"
                                        "config() failed");
                                return rv;
                            }

                        } else if (!strcmp(sbi_key, "client")) {
                            ogs_yaml_iter_t client_iter;
                            ogs_yaml_iter_recurse(&sbi_iter, &client_iter);
                            while (ogs_yaml_iter_next(&client_iter)) {
                                const char *client_key =
                                    ogs_yaml_iter_key(&client_iter);
                                ogs_assert(client_key);
                                if (!strcmp(client_key, "sepp")) {
                                    ogs_yaml_iter_t peer_array, peer_iter;
                                    ogs_yaml_iter_t saved_iter;
                                    ogs_yaml_iter_recurse(
                                            &client_iter, &peer_array);
                                    do {
                                        sepp_node_t *sepp_node = NULL;
                                        ogs_sbi_client_t *client = NULL;
                                        const char *receiver = NULL;
                                        const char *mnc = NULL, *mcc = NULL;

                                        if (ogs_yaml_iter_type(&peer_array) ==
                                                YAML_MAPPING_NODE) {
                                            memcpy(&peer_iter, &peer_array,
                                                    sizeof(ogs_yaml_iter_t));
                                        } else if (ogs_yaml_iter_type(
                                                    &peer_array) ==
                                            YAML_SEQUENCE_NODE) {
                                            if (!ogs_yaml_iter_next(&peer_array))
                                                break;
                                            ogs_yaml_iter_recurse(
                                                    &peer_array, &peer_iter);
                                        } else if (ogs_yaml_iter_type(
                                                    &peer_array) ==
                                                YAML_SCALAR_NODE) {
                                            break;
                                        } else
                                            ogs_assert_if_reached();

                                        memcpy(&saved_iter, &peer_iter,
                                                sizeof(saved_iter));

                                        while (ogs_yaml_iter_next(&peer_iter)) {
                                            const char *peer_key =
                                                ogs_yaml_iter_key(&peer_iter);
                                            ogs_assert(peer_key);
                                            if (!strcmp(peer_key, "receiver")) {
                                                receiver = ogs_yaml_iter_value(
                                                        &peer_iter);
                                            } else if (!strcmp(peer_key,
                                                        "target_plmn_id")) {
                                                ogs_yaml_iter_t plmn_id_iter;

                                                ogs_yaml_iter_recurse(
                                                        &peer_iter,
                                                        &plmn_id_iter);
                                                while (ogs_yaml_iter_next(
                                                            &plmn_id_iter)) {
                                                    const char *plmn_id_key =
                                                        ogs_yaml_iter_key(
                                                                &plmn_id_iter);
                                                    ogs_assert(plmn_id_key);
                                                    if (!strcmp(plmn_id_key,
                                                                "mcc")) {
                                                        mcc =
                                                            ogs_yaml_iter_value(
                                                                &plmn_id_iter);
                                                    } else if (!strcmp(
                                                                plmn_id_key,
                                                                "mnc")) {
                                                        mnc =
                                                            ogs_yaml_iter_value(
                                                                &plmn_id_iter);
                                                    }
                                                }
                                            }
                                        }

                                        if (!receiver) {
                                            ogs_error(
                                                "No n32c.client.sepp.receiver");
                                            return OGS_ERROR;
                                        }

                                        sepp_node =
                                            sepp_node_find_by_receiver(
                                                    (char *)receiver);
                                        if (sepp_node) {
                                            ogs_error("SEPP duplicated "
                                                    "[receiver:%s]",
                                                    receiver);
                                            return OGS_ERROR;
                                        }

                                        sepp_node = sepp_node_add(
                                                (char *)receiver);
                                        ogs_assert(sepp_node);

                                        if (mcc && mnc) {
                                            ogs_plmn_id_build(
                                                &sepp_node->target_plmn_id,
                                                atoi(mcc), atoi(mnc),
                                                strlen(mnc));
                                            sepp_node->
                                                target_plmn_id_presence =
                                                true;
                                        }

                                        memcpy(&peer_iter, &saved_iter,
                                                sizeof(peer_iter));

                                        client =
                                            ogs_sbi_context_parse_client_config(
                                                &peer_iter);

                                        if (!client) {
                                            ogs_error("ogs_sbi_context_parse_"
                                                    "client_config() failed");
                                            sepp_node_remove(sepp_node);
                                            return OGS_ERROR;
                                        }

                                        OGS_SBI_SETUP_CLIENT(
                                                sepp_node, client);

                                        memcpy(&peer_iter, &saved_iter,
                                                sizeof(peer_iter));

                                        while (ogs_yaml_iter_next(&peer_iter)) {
                                            const char *peer_key =
                                                ogs_yaml_iter_key(&peer_iter);
                                            ogs_assert(peer_key);
                                            if (!strcmp(peer_key,
                                                OGS_SBI_INTERFACE_NAME_N32F)) {
                                                ogs_yaml_iter_t n32f_iter;
                                                ogs_yaml_iter_recurse(
                                                        &peer_iter, &n32f_iter);
                                                client =
                                                    ogs_sbi_context_parse_client_config(
                                                            &n32f_iter);
                                                if (!client) {
                                                    ogs_error(
                                                        "ogs_sbi_context_parse_"
                                                        "client_config() "
                                                        "failed");
                                                    sepp_node_remove(sepp_node);
                                                    return OGS_ERROR;
                                                }
                                                OGS_SBI_SETUP_CLIENT(
                                                    &sepp_node->n32f, client);
                                            }
                                        }

                                    } while (ogs_yaml_iter_type(&peer_array) ==
                                            YAML_SEQUENCE_NODE);
                                }
                            }
                        } else
                            ogs_warn("unknown key `%s`", sbi_key);
                    }
                } else if (!strcmp(sepp_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(sepp_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(sepp_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(sepp_key, "discovery")) {
                    /* handle config in sbi library */
                } else if (!strcmp(sepp_key, "info")) {
                    ogs_sbi_nf_instance_t *nf_instance = NULL;
                    ogs_sbi_nf_info_t *nf_info = NULL;
                    ogs_sbi_sepp_info_t *sepp_info = NULL;

                    ogs_yaml_iter_t info_iter;
                    ogs_yaml_iter_recurse(&sepp_iter, &info_iter);

                    nf_instance = ogs_sbi_self()->nf_instance;
                    ogs_assert(nf_instance);

                    nf_info = ogs_sbi_nf_info_find(
                                &nf_instance->nf_info_list,
                                    OpenAPI_nf_type_SEPP);
                    ogs_assert(nf_info);

                    sepp_info = &nf_info->sepp;

                    while (ogs_yaml_iter_next(&info_iter)) {
                        const char *info_key =
                            ogs_yaml_iter_key(&info_iter);
                        ogs_assert(info_key);
                        if (!strcmp(info_key, "port")) {
                            ogs_yaml_iter_t port_iter;
                            ogs_yaml_iter_recurse(&info_iter, &port_iter);
                            while (ogs_yaml_iter_next(&port_iter)) {
                                const char *port_key =
                                    ogs_yaml_iter_key(&port_iter);
                                ogs_assert(port_key);
                                if (!strcmp(port_key, "http")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&port_iter);
                                    if (v) {
                                        sepp_info->http.presence = true;
                                        sepp_info->http.port = atoi(v);
                                    }
                                } else if (!strcmp(port_key, "https")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&port_iter);
                                    if (v) {
                                        sepp_info->https.presence = true;
                                        sepp_info->https.port = atoi(v);
                                    }
                                } else
                                    ogs_warn("unknown key `%s`", port_key);
                            }
                        } else
                            ogs_warn("unknown key `%s`", info_key);
                    }
                }
            }
        }
    }

    rv = sepp_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

sepp_node_t *sepp_node_add(char *receiver)
{
    sepp_node_t *sepp_node = NULL;

    ogs_assert(receiver);

    ogs_pool_alloc(&sepp_node_pool, &sepp_node);
    if (!sepp_node) {
        ogs_error("Maximum number of nodeiation[%d] reached",
                    max_num_of_sepp_node);
        return NULL;
    }
    memset(sepp_node, 0, sizeof *sepp_node);

    sepp_node->receiver = ogs_strdup(receiver);
    ogs_assert(sepp_node->receiver);

    ogs_list_add(&self.peer_list, sepp_node);

    return sepp_node;
}

void sepp_node_remove(sepp_node_t *sepp_node)
{
    ogs_assert(sepp_node);

    ogs_list_remove(&self.peer_list, sepp_node);

    if (sepp_node->client)
        ogs_sbi_client_remove(sepp_node->client);
    if (sepp_node->n32f.client)
        ogs_sbi_client_remove(sepp_node->n32f.client);

    if (sepp_node->receiver)
        ogs_free(sepp_node->receiver);

    ogs_pool_free(&sepp_node_pool, sepp_node);
}

void sepp_node_remove_all(void)
{
    sepp_node_t *sepp_node = NULL, *next_sepp_node = NULL;

    ogs_list_for_each_safe(&self.peer_list, next_sepp_node, sepp_node)
        sepp_node_remove(sepp_node);
}

sepp_node_t *sepp_node_find_by_receiver(char *receiver)
{
    sepp_node_t *sepp_node = NULL;

    ogs_assert(receiver);

    ogs_list_for_each(&self.peer_list, sepp_node) {
        ogs_assert(sepp_node->receiver);
        if (strcmp(sepp_node->receiver, receiver) == 0) {
            return sepp_node;
        }
    }

    return NULL;
}

sepp_node_t *sepp_node_find_by_plmn_id(uint16_t mcc, uint16_t mnc)
{
    sepp_node_t *sepp_node = NULL;

    ogs_assert(mcc);
    ogs_assert(mnc);

    ogs_list_for_each(&self.peer_list, sepp_node) {
        int i;
        ogs_assert(sepp_node->receiver);
        for (i = 0; i < sepp_node->num_of_plmn_id; i++) {
            if (mcc == ogs_plmn_id_mcc(&sepp_node->plmn_id[i]) &&
                mnc == ogs_plmn_id_mnc(&sepp_node->plmn_id[i])) {
                return sepp_node;
            }
        }
    }

    return NULL;
}

sepp_assoc_t *sepp_assoc_add(ogs_pool_id_t stream_id)
{
    sepp_assoc_t *assoc = NULL;

    ogs_assert(stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID);

    ogs_pool_alloc(&sepp_assoc_pool, &assoc);
    if (!assoc) {
        ogs_error("Maximum number of association[%d] reached",
                    max_num_of_sepp_assoc);
        return NULL;
    }
    memset(assoc, 0, sizeof *assoc);

    assoc->stream_id = stream_id;

    ogs_list_add(&self.assoc_list, assoc);

    return assoc;
}

void sepp_assoc_remove(sepp_assoc_t *assoc)
{
    ogs_assert(assoc);

    ogs_list_remove(&self.assoc_list, assoc);

    if (assoc->client)
        ogs_sbi_client_remove(assoc->client);
    if (assoc->nrf_client)
        ogs_sbi_client_remove(assoc->nrf_client);

    ogs_pool_free(&sepp_assoc_pool, assoc);
}

void sepp_assoc_remove_all(void)
{
    sepp_assoc_t *assoc = NULL, *next_assoc = NULL;

    ogs_list_for_each_safe(&self.assoc_list, next_assoc, assoc)
        sepp_assoc_remove(assoc);
}
