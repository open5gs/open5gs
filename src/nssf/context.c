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
    int idx = 0;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = nssf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if ((!strcmp(root_key, "nssf")) &&
            (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t nssf_iter;
            ogs_yaml_iter_recurse(&root_iter, &nssf_iter);
            while (ogs_yaml_iter_next(&nssf_iter)) {
                const char *nssf_key = ogs_yaml_iter_key(&nssf_iter);
                ogs_assert(nssf_key);
                if (!strcmp(nssf_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(nssf_key, "sbi")) {
                    /* handle config in sbi library */
                    ogs_yaml_iter_t sbi_iter;
                    ogs_yaml_iter_recurse(&nssf_iter, &sbi_iter);
                    while (ogs_yaml_iter_next(&sbi_iter)) {
                        const char *sbi_key = ogs_yaml_iter_key(&sbi_iter);
                        ogs_assert(sbi_key);
                        if (!strcmp(sbi_key, "server")) {
                        } else if (!strcmp(sbi_key, "client")) {
                            ogs_yaml_iter_t client_iter;
                            ogs_yaml_iter_recurse(&sbi_iter, &client_iter);
                            while (ogs_yaml_iter_next(&client_iter)) {
                                const char *client_key =
                                    ogs_yaml_iter_key(&client_iter);
                                ogs_assert(client_key);
                                if (!strcmp(client_key, "nsi")) {
                                    ogs_yaml_iter_t nsi_array, nsi_iter;
                                    ogs_yaml_iter_recurse(&client_iter,
                                            &nsi_array);
                                    do {
                                        const char *uri = NULL;
                                        const char *sst = NULL, *sd = NULL;

                                        if (ogs_yaml_iter_type(&nsi_array) ==
                                                YAML_MAPPING_NODE) {
                                            memcpy(&nsi_iter, &nsi_array,
                                                    sizeof(ogs_yaml_iter_t));
                                        } else if (ogs_yaml_iter_type(
                                                    &nsi_array) ==
                                                YAML_SEQUENCE_NODE) {
                                            if (!ogs_yaml_iter_next(&nsi_array))
                                                break;
                                            ogs_yaml_iter_recurse(
                                                    &nsi_array, &nsi_iter);
                                        } else if (ogs_yaml_iter_type(
                                                    &nsi_array) ==
                                                YAML_SCALAR_NODE) {
                                            break;
                                        } else
                                            ogs_assert_if_reached();

                                        while (ogs_yaml_iter_next(&nsi_iter)) {
                                            const char *nsi_key =
                                                ogs_yaml_iter_key(&nsi_iter);
                                            ogs_assert(nsi_key);
                                            if (!strcmp(nsi_key, "uri")) {
                                                uri = ogs_yaml_iter_value(
                                                        &nsi_iter);
                                            } else if (!strcmp(nsi_key,
                                                        "s_nssai")) {
                                                ogs_yaml_iter_t s_nssai_iter;
                                                ogs_yaml_iter_recurse(&nsi_iter,
                                                        &s_nssai_iter);

                                                while (ogs_yaml_iter_next(
                                                            &s_nssai_iter)) {
                                                    const char *s_nssai_key =
                                                        ogs_yaml_iter_key(
                                                                &s_nssai_iter);
                                                    ogs_assert(s_nssai_key);

                                                    if (!strcmp(s_nssai_key,
                                                                "sst")) {
                                                        sst =
                                                            ogs_yaml_iter_value(
                                                                &s_nssai_iter);
                                                    } else if (!strcmp(
                                                                s_nssai_key,
                                                                "sd")) {
                                                        sd =
                                                            ogs_yaml_iter_value(
                                                                &s_nssai_iter);
                                                    } else
                                                        ogs_warn(
                                                            "unknown key `%s`",
                                                            s_nssai_key);
                                                }
                                            } else
                                                ogs_warn("unknown key `%s`",
                                                        nsi_key);
                                        }

                                        if (uri) {
                                            bool rc;

                                            OpenAPI_uri_scheme_e scheme =
                                                OpenAPI_uri_scheme_NULL;

                                            char *fqdn = NULL;
                                            uint16_t fqdn_port = 0;
                                            ogs_sockaddr_t *addr = NULL;
                                            ogs_sockaddr_t *addr6 = NULL;

                                            ogs_sbi_header_t h;
                                            uint16_t port = 0;

                                            nssf_nsi_t *nsi = NULL;
                                            char *nrf_id = NULL;

                                            rc = ogs_sbi_getaddr_from_uri(
                                                    &scheme, &fqdn, &fqdn_port,
                                                    &addr, &addr6,
                                                    (char *)uri);
                                            if (rc == false) {
                                                if (!scheme)
                                                    ogs_error("Invalid Scheme "
                                                            "in URI[%s]", uri);
                                                else
                                                    ogs_error("Invalid URI[%s]",
                                                            uri);

                                                return OGS_ERROR;
                                            }

                                            if (fqdn) {
                                                port = fqdn_port;
                                            } else {
                                                if (addr6) {
                                                    port = ogs_sbi_uri_port_from_scheme_and_addr(
                                                            scheme, addr6);
                                                } else if (addr) {
                                                    port = ogs_sbi_uri_port_from_scheme_and_addr(
                                                            scheme, addr);
                                                }
                                            }

                                            memset(&h, 0, sizeof(h));
                                            h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_DISC;
                                            h.api.version =
                                                (char *)OGS_SBI_API_V1;
                                            h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;

                                            nrf_id = ogs_uridup(
                                                    scheme, fqdn,
                                                    addr, addr6, port, &h);
                                            ogs_assert(nrf_id);

					    /* Clang scan-build SA: Argument with nonnull attribute passed null:
					     * sst may be NULL in atoi(sst) if the "uri" key path is followed. */
					    ogs_assert(sst);

                                            nsi = nssf_nsi_add(
                                                    nrf_id,
                                                    atoi(sst),
                                                    ogs_s_nssai_sd_from_string(
                                                        sd));
                                            ogs_assert(nsi);

                                            ogs_free(nrf_id);

                                            ogs_free(fqdn);
                                            ogs_freeaddrinfo(addr);
                                            ogs_freeaddrinfo(addr6);
                                        }

                                    } while (ogs_yaml_iter_type(&nsi_array) ==
                                            YAML_SEQUENCE_NODE);
                                }
                            }
                        } else
                            ogs_warn("unknown key `%s`", sbi_key);
                    }
                } else if (!strcmp(nssf_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(nssf_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(nssf_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(nssf_key, "discovery")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", nssf_key);
            }
        }
    }

    rv = nssf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

nssf_nsi_t *nssf_nsi_add(char *nrf_id, uint8_t sst, ogs_uint24_t sd)
{
    nssf_nsi_t *nsi = NULL;

    ogs_assert(nrf_id);

    ogs_pool_alloc(&nssf_nsi_pool, &nsi);
    ogs_assert(nsi);
    memset(nsi, 0, sizeof *nsi);

    nsi->nsi_id = ogs_msprintf("%d", (int)ogs_pool_index(&nssf_nsi_pool, nsi));
    ogs_assert(nsi->nsi_id);

    nsi->nrf_id = ogs_strdup(nrf_id);
    ogs_assert(nsi->nrf_id);

    nsi->s_nssai.sst = sst;
    nsi->s_nssai.sd.v = sd.v;

    ogs_list_add(&self.nsi_list, nsi);

    return nsi;
}

void nssf_nsi_remove(nssf_nsi_t *nsi)
{
    ogs_assert(nsi);

    ogs_list_remove(&self.nsi_list, nsi);

    ogs_assert(nsi->nrf_id);
    ogs_free(nsi->nrf_id);

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

int get_nsi_load(void)
{
    return (((ogs_pool_size(&nssf_nsi_pool) -
            ogs_pool_avail(&nssf_nsi_pool)) * 100) /
            ogs_pool_size(&nssf_nsi_pool));
}
