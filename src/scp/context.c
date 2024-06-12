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

#include "context.h"

static scp_context_t self;

int __scp_log_domain;

static OGS_POOL(scp_assoc_pool, scp_assoc_t);

static int context_initialized = 0;

static int max_num_of_scp_assoc = 0;

void scp_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize SCP context */
    memset(&self, 0, sizeof(scp_context_t));

    ogs_log_install_domain(&__scp_log_domain, "scp", ogs_core()->log.level);

#define MAX_NUM_OF_SCP_ASSOC 8
    max_num_of_scp_assoc = ogs_global_conf()->max.ue * MAX_NUM_OF_SCP_ASSOC;

    ogs_pool_init(&scp_assoc_pool, max_num_of_scp_assoc);

    context_initialized = 1;
}

void scp_context_final(void)
{
    ogs_assert(context_initialized == 1);

    scp_assoc_remove_all();

    ogs_pool_final(&scp_assoc_pool);

    context_initialized = 0;
}

scp_context_t *scp_self(void)
{
    return &self;
}

static int scp_context_prepare(void)
{
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_info_t *nf_info = NULL;
    ogs_sbi_scp_info_t *scp_info = NULL;

    /*********************************************************************
     * SCP Port Configuration
     *********************************************************************/
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);

    nf_info = ogs_sbi_nf_info_add(
            &nf_instance->nf_info_list, OpenAPI_nf_type_SCP);
    if (!nf_info) {
        ogs_error("ogs_sbi_nf_info_add() failed");
        return OGS_ERROR;
    }

    scp_info = &nf_info->scp;

    for (server = ogs_sbi_server_first();
            server; server = ogs_sbi_server_next(server)) {
        ogs_sockaddr_t *advertise = NULL;

        advertise = server->advertise;
        if (!advertise)
            advertise = server->node.addr;
        ogs_assert(advertise);

        if (server->scheme == OpenAPI_uri_scheme_https) {
            scp_info->https.presence = true;
            scp_info->https.port = OGS_PORT(advertise);
        } else if (server->scheme == OpenAPI_uri_scheme_http) {
            scp_info->http.presence = true;
            scp_info->http.port = OGS_PORT(advertise);
        } else {
            ogs_error("Unknown scheme[%d]", server->scheme);
            ogs_assert_if_reached();
        }
    }

    return OGS_OK;
}

static int scp_context_validation(void)
{
    return OGS_OK;
}

int scp_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = scp_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "scp")) {
            ogs_yaml_iter_t scp_iter;
            ogs_yaml_iter_recurse(&root_iter, &scp_iter);
            while (ogs_yaml_iter_next(&scp_iter)) {
                const char *scp_key = ogs_yaml_iter_key(&scp_iter);
                ogs_assert(scp_key);
                if (!strcmp(scp_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(scp_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(scp_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(scp_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(scp_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(scp_key, "discovery")) {
                    /* handle config in sbi library */
                } else if (!strcmp(scp_key, "info")) {
                    ogs_sbi_nf_instance_t *nf_instance = NULL;
                    ogs_sbi_nf_info_t *nf_info = NULL;
                    ogs_sbi_scp_info_t *scp_info = NULL;

                    ogs_yaml_iter_t info_iter;
                    ogs_yaml_iter_recurse(&scp_iter, &info_iter);

                    nf_instance = ogs_sbi_self()->nf_instance;
                    ogs_assert(nf_instance);

                    nf_info = ogs_sbi_nf_info_find(
                                &nf_instance->nf_info_list,
                                    OpenAPI_nf_type_SCP);
                    ogs_assert(nf_info);

                    scp_info = &nf_info->scp;

                    while (ogs_yaml_iter_next(&info_iter)) {
                        const char *info_key =
                            ogs_yaml_iter_key(&info_iter);
                        ogs_assert(info_key);
                        if (!strcmp(info_key, "domain")) {
                            ogs_yaml_iter_t domain_array, domain_iter;
                            ogs_yaml_iter_recurse(&info_iter, &domain_array);

                            do {
                                if (ogs_yaml_iter_type(&domain_array) ==
                                        YAML_MAPPING_NODE) {
                                    memcpy(&domain_iter, &domain_array,
                                            sizeof(ogs_yaml_iter_t));
                                } else if (ogs_yaml_iter_type(&domain_array) ==
                                            YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(&domain_array))
                                        break;
                                    ogs_yaml_iter_recurse(&domain_array,
                                            &domain_iter);
                                } else if (ogs_yaml_iter_type(&domain_array) ==
                                            YAML_SCALAR_NODE) {
                                    break;
                                } else
                                    ogs_assert_if_reached();

                                while (ogs_yaml_iter_next(&domain_iter)) {
                                    const char *domain_key =
                                        ogs_yaml_iter_key(&domain_iter);
                                    ogs_assert(domain_key);
                                    if (!strcmp(domain_key, "port")) {
                                        ogs_yaml_iter_t port_iter;
                                        ogs_yaml_iter_recurse(&domain_iter,
                                                &port_iter);
                                        while (ogs_yaml_iter_next(&port_iter)) {
                                            const char *port_key =
                                                ogs_yaml_iter_key(&port_iter);
                                            ogs_assert(port_key);
                                            if (!strcmp(port_key, "http")) {
                                                const char *v =
                                                    ogs_yaml_iter_value(
                                                            &port_iter);
                                                if (v) {
                                                    scp_info->domain[
                                                        scp_info->
                                                            num_of_domain].
                                                            http.presence =
                                                                true;
                                                    scp_info->domain[
                                                        scp_info->
                                                            num_of_domain].
                                                            http.port = atoi(v);
                                                }
                                            } else if (!strcmp(port_key,
                                                        "https")) {
                                                const char *v =
                                                    ogs_yaml_iter_value(
                                                            &port_iter);
                                                if (v) {
                                                    scp_info->domain[
                                                        scp_info->
                                                            num_of_domain].
                                                            https.presence =
                                                                true;
                                                    scp_info->domain[
                                                        scp_info->
                                                            num_of_domain].
                                                            https.port =
                                                                atoi(v);
                                                }
                                            } else
                                                ogs_warn("unknown key `%s`",
                                                        port_key);
                                        }
                                    } else if (!strcmp(domain_key, "name")) {
                                        const char *v = ogs_yaml_iter_value(
                                                    &domain_iter);
                                        if (v)
                                            scp_info->domain[
                                                scp_info->num_of_domain].
                                                name = ogs_strdup(v);
                                    } else if (!strcmp(domain_key, "fqdn")) {
                                        const char *v = ogs_yaml_iter_value(
                                                    &domain_iter);
                                        if (v)
                                            scp_info->domain[
                                                scp_info->num_of_domain].
                                                fqdn = ogs_strdup(v);
                                    } else
                                        ogs_warn("unknown key `%s`",
                                                domain_key);
                                }

                                if (scp_info->domain[
                                        scp_info->num_of_domain].name)
                                    scp_info->num_of_domain++;

                            } while (ogs_yaml_iter_type(&domain_array) ==
                                    YAML_SEQUENCE_NODE);

                        } else if (!strcmp(info_key, "port")) {
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
                                        scp_info->http.presence = true;
                                        scp_info->http.port = atoi(v);
                                    }
                                } else if (!strcmp(port_key, "https")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&port_iter);
                                    if (v) {
                                        scp_info->https.presence = true;
                                        scp_info->https.port = atoi(v);
                                    }
                                } else
                                    ogs_warn("unknown key `%s`", port_key);
                            }
                        } else
                            ogs_warn("unknown key `%s`", info_key);
                    }
                } else
                    ogs_warn("unknown key `%s`", scp_key);
            }
        }
    }

    rv = scp_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

scp_assoc_t *scp_assoc_add(ogs_pool_id_t stream_id)
{
    scp_assoc_t *assoc = NULL;

    ogs_assert(stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID);

    ogs_pool_alloc(&scp_assoc_pool, &assoc);
    if (!assoc) {
        ogs_error("Maximum number of association[%d] reached",
                    max_num_of_scp_assoc);
        return NULL;
    }
    memset(assoc, 0, sizeof *assoc);

    assoc->stream_id = stream_id;

    assoc->discovery_option = ogs_sbi_discovery_option_new();
    ogs_assert(assoc->discovery_option);

    ogs_list_add(&self.assoc_list, assoc);

    return assoc;
}

void scp_assoc_remove(scp_assoc_t *assoc)
{
    ogs_assert(assoc);

    ogs_list_remove(&self.assoc_list, assoc);

    ogs_assert(assoc->discovery_option);
    ogs_sbi_discovery_option_free(assoc->discovery_option);

    if (assoc->client)
        ogs_sbi_client_remove(assoc->client);
    if (assoc->nrf_client)
        ogs_sbi_client_remove(assoc->nrf_client);

    if (assoc->target_apiroot)
        ogs_free(assoc->target_apiroot);

    ogs_pool_free(&scp_assoc_pool, assoc);
}

void scp_assoc_remove_all(void)
{
    scp_assoc_t *assoc = NULL, *next_assoc = NULL;

    ogs_list_for_each_safe(&self.assoc_list, next_assoc, assoc)
        scp_assoc_remove(assoc);
}
