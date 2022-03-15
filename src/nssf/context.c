/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "sbi-path.h"

static nssf_context_t self;

int __nssf_log_domain;

static OGS_POOL(nssf_nsi_pool, nssf_nsi_t);

static int context_initialized = 0;

void nssf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize NSSF context */
    memset(&self, 0, sizeof(nssf_context_t));

    ogs_log_install_domain(&__nssf_log_domain, "nssf", ogs_core()->log.level);

    ogs_pool_init(&nssf_nsi_pool, ogs_app()->pool.nf);

    context_initialized = 1;
}

void nssf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    nssf_nsi_remove_all();

    ogs_pool_final(&nssf_nsi_pool);

    context_initialized = 0;
}

nssf_context_t *nssf_self(void)
{
    return &self;
}

static int nssf_context_prepare(void)
{
    self.nf_type = OpenAPI_nf_type_NSSF;

    return OGS_OK;
}

static int nssf_context_validation(void)
{
    if (ogs_list_first(&self.nsi_list) == NULL) {
        ogs_error("No nssf.nsi in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int nssf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = nssf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "nssf")) {
            ogs_yaml_iter_t nssf_iter;
            ogs_yaml_iter_recurse(&root_iter, &nssf_iter);
            while (ogs_yaml_iter_next(&nssf_iter)) {
                const char *nssf_key = ogs_yaml_iter_key(&nssf_iter);
                ogs_assert(nssf_key);
                if (!strcmp(nssf_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(nssf_key, "nsi")) {
                    ogs_list_t list, list6;
                    ogs_socknode_t *node = NULL, *node6 = NULL;

                    ogs_yaml_iter_t nsi_array, nsi_iter;
                    ogs_yaml_iter_recurse(&nssf_iter, &nsi_array);
                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = ogs_sbi_self()->sbi_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;
                        const char *key = NULL, *pem = NULL;
                        const char *sst = NULL, *sd = NULL;

                        ogs_sockopt_t option;
                        bool is_option = false;

                        if (ogs_yaml_iter_type(&nsi_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&nsi_iter, &nsi_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&nsi_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&nsi_array))
                                break;
                            ogs_yaml_iter_recurse(&nsi_array, &nsi_iter);
                        } else if (ogs_yaml_iter_type(&nsi_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&nsi_iter)) {
                            const char *nsi_key =
                                ogs_yaml_iter_key(&nsi_iter);
                            ogs_assert(nsi_key);
                            if (!strcmp(nsi_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&nsi_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ",
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(nsi_key, "addr") ||
                                    !strcmp(nsi_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&nsi_iter,
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

                                    ogs_assert(num < OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] =
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(nsi_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&nsi_iter);
                                if (v) {
                                    port = atoi(v);
                                }
                            } else if (!strcmp(nsi_key, "dev")) {
                                dev = ogs_yaml_iter_value(&nsi_iter);
                            } else if (!strcmp(nsi_key, "option")) {
                                rv = ogs_app_config_parse_sockopt(
                                        &nsi_iter, &option);
                                if (rv != OGS_OK) return rv;
                                is_option = true;
                            } else if (!strcmp(nsi_key, "tls")) {
                                ogs_yaml_iter_t tls_iter;
                                ogs_yaml_iter_recurse(&nsi_iter, &tls_iter);

                                while (ogs_yaml_iter_next(&tls_iter)) {
                                    const char *tls_key =
                                        ogs_yaml_iter_key(&tls_iter);
                                    ogs_assert(tls_key);

                                    if (!strcmp(tls_key, "key")) {
                                        key = ogs_yaml_iter_value(&tls_iter);
                                    } else if (!strcmp(tls_key, "pem")) {
                                        pem = ogs_yaml_iter_value(&tls_iter);
                                    } else
                                        ogs_warn("unknown key `%s`", tls_key);
                                }
                            } else if (!strcmp(nsi_key, "s_nssai")) {
                                ogs_yaml_iter_t s_nssai_iter;
                                ogs_yaml_iter_recurse(&nsi_iter, &s_nssai_iter);

                                while (ogs_yaml_iter_next(&s_nssai_iter)) {
                                    const char *s_nssai_key =
                                        ogs_yaml_iter_key(&s_nssai_iter);
                                    ogs_assert(s_nssai_key);

                                    if (!strcmp(s_nssai_key, "sst")) {
                                        sst = ogs_yaml_iter_value(
                                                &s_nssai_iter);
                                    } else if (!strcmp(s_nssai_key, "sd")) {
                                        sd = ogs_yaml_iter_value(&s_nssai_iter);
                                    } else
                                        ogs_warn("unknown key `%s`",
                                                s_nssai_key);
                                }
                            } else
                                ogs_warn("unknown key `%s`", nsi_key);
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
                            if (ogs_app()->parameter.no_ipv4 == 0)
                                ogs_socknode_add(
                                    &list, AF_INET, addr,
                                    is_option ? &option : NULL);
                            if (ogs_app()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(
                                    &list6, AF_INET6, addr,
                                    is_option ? &option : NULL);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                ogs_app()->parameter.no_ipv4 ? NULL : &list,
                                ogs_app()->parameter.no_ipv6 ? NULL : &list6,
                                dev, port,
                                is_option ? &option : NULL);
                            ogs_assert(rv == OGS_OK);
                        }

                        node = ogs_list_first(&list);
                        if (node) {
                            ogs_assert(sst);

                            nssf_nsi_t *nsi = nssf_nsi_add(node->addr,
                                    atoi(sst), ogs_s_nssai_sd_from_string(sd));
                            ogs_assert(nsi);

                            if (key) nsi->tls.key = key;
                            if (pem) nsi->tls.pem = pem;
                        }
                        node6 = ogs_list_first(&list6);
                        if (node6) {
                            ogs_assert(sst);

                            nssf_nsi_t *nsi = nssf_nsi_add(node6->addr,
                                    atoi(sst), ogs_s_nssai_sd_from_string(sd));
                            ogs_assert(nsi);

                            if (key) nsi->tls.key = key;
                            if (pem) nsi->tls.pem = pem;
                        }

                        ogs_socknode_remove_all(&list);
                        ogs_socknode_remove_all(&list6);
                    } while (ogs_yaml_iter_type(&nsi_array) ==
                            YAML_SEQUENCE_NODE);
                } else
                    ogs_warn("unknown key `%s`", nssf_key);
            }
        }
    }

    rv = nssf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

nssf_nsi_t *nssf_nsi_add(ogs_sockaddr_t *addr, uint8_t sst, ogs_uint24_t sd)
{
    nssf_nsi_t *nsi = NULL;

    ogs_assert(addr);
    ogs_assert(sst);

    ogs_pool_alloc(&nssf_nsi_pool, &nsi);
    ogs_assert(nsi);
    memset(nsi, 0, sizeof *nsi);

    nsi->nsi_id = ogs_msprintf("%d", (int)ogs_pool_index(&nssf_nsi_pool, nsi));
    ogs_assert(nsi->nsi_id);

    ogs_assert(OGS_OK == ogs_copyaddrinfo(&nsi->addr, addr));

    nsi->s_nssai.sst = sst;
    nsi->s_nssai.sd.v = sd.v;

    ogs_list_add(&self.nsi_list, nsi);

    return nsi;
}

void nssf_nsi_remove(nssf_nsi_t *nsi)
{
    ogs_assert(nsi);

    ogs_list_remove(&self.nsi_list, nsi);

    ogs_assert(nsi->addr);
    ogs_freeaddrinfo(nsi->addr);

    ogs_assert(nsi->nsi_id);
    ogs_free(nsi->nsi_id);

    ogs_pool_free(&nssf_nsi_pool, nsi);
}

void nssf_nsi_remove_all(void)
{
    nssf_nsi_t *nsi = NULL, *next_nsi = NULL;

    ogs_list_for_each_safe(&self.nsi_list, next_nsi, nsi)
        nssf_nsi_remove(nsi);
}

nssf_nsi_t *nssf_nsi_find_by_s_nssai(ogs_s_nssai_t *s_nssai)
{
    nssf_nsi_t *nsi = NULL;

    ogs_assert(s_nssai);

    ogs_list_for_each(&self.nsi_list, nsi) {
        /* Compare S-NSSAI */
        if (nsi->s_nssai.sst == s_nssai->sst &&
            nsi->s_nssai.sd.v == s_nssai->sd.v) {
            return nsi;
        }
    }

    return NULL;
}

char *nssf_nsi_nrf_uri(nssf_nsi_t *nsi)
{
    ogs_sbi_header_t h;
    bool https = false;

    ogs_assert(nsi);

    memset(&h, 0, sizeof(h));

    if (nsi->tls.key && nsi->tls.pem)
        https = true;

    h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_DISC;
    h.api.version = (char *)OGS_SBI_API_V1;
    h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;

    return ogs_uridup(https, nsi->addr, &h);
}
