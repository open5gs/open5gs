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

static af_context_t self;

int __af_log_domain;

static OGS_POOL(af_sess_pool, af_sess_t);

static int context_initialized = 0;

void af_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize AF context */
    memset(&self, 0, sizeof(af_context_t));

    ogs_log_install_domain(&__af_log_domain, "af", ogs_core()->log.level);

    ogs_pool_init(&af_sess_pool, ogs_app()->pool.sess);

    self.supi_hash = ogs_hash_make();
    ogs_assert(self.supi_hash);
    self.ipv4_hash = ogs_hash_make();
    ogs_assert(self.ipv4_hash);
    self.ipv6_hash = ogs_hash_make();
    ogs_assert(self.ipv6_hash);
    self.pcf_app_session_id_hash = ogs_hash_make();
    ogs_assert(self.pcf_app_session_id_hash);

    context_initialized = 1;
}

void af_context_final(void)
{
    ogs_assert(context_initialized == 1);

    af_sess_remove_all();

    ogs_assert(self.supi_hash);
    ogs_hash_destroy(self.supi_hash);
    ogs_assert(self.ipv4_hash);
    ogs_hash_destroy(self.ipv4_hash);
    ogs_assert(self.ipv6_hash);
    ogs_hash_destroy(self.ipv6_hash);
    ogs_assert(self.pcf_app_session_id_hash);
    ogs_hash_destroy(self.pcf_app_session_id_hash);

    ogs_pool_final(&af_sess_pool);

    context_initialized = 0;
}

af_context_t *af_self(void)
{
    return &self;
}

static int af_context_prepare(void)
{
    return OGS_OK;
}

static int af_context_validation(void)
{
    return OGS_OK;
}

int af_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = af_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "af")) {
            ogs_yaml_iter_t af_iter;
            ogs_yaml_iter_recurse(&root_iter, &af_iter);
            while (ogs_yaml_iter_next(&af_iter)) {
                const char *af_key = ogs_yaml_iter_key(&af_iter);
                ogs_assert(af_key);
                if (!strcmp(af_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(af_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(af_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(af_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(af_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(af_key, "discovery")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", af_key);
            }
        }
    }

    rv = af_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

af_sess_t *af_sess_add_by_ue_address(ogs_ip_t *ue_address)
{
    af_sess_t *sess = NULL;

    ogs_assert(ue_address);
    ogs_assert(ue_address->ipv4 || ue_address->ipv6);

    ogs_pool_alloc(&af_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    sess->app_session.af.id = ogs_msprintf("%d",
            (int)ogs_pool_index(&af_sess_pool, sess));
    ogs_assert(sess->app_session.af.id);

    if (ue_address->ipv4) {
        sess->ipv4addr = ogs_ipv4_to_string(ue_address->addr);
        ogs_assert(sess->ipv4addr);
    }

    if (ue_address->ipv6) {
        sess->ipv6addr = ogs_ipv6addr_to_string(ue_address->addr6);
        ogs_assert(sess->ipv6addr);
        sess->ipv6prefix = ogs_ipv6prefix_to_string(
                ue_address->addr6, OGS_IPV6_128_PREFIX_LEN);
        ogs_assert(sess->ipv6prefix);
    }

    OGS_SBI_FEATURES_SET(sess->policyauthorization_features,
            OGS_SBI_NPCF_POLICYAUTHORIZATION_SPONSORED_CONNECTIVITY);
    OGS_SBI_FEATURES_SET(sess->policyauthorization_features,
            OGS_SBI_NPCF_POLICYAUTHORIZATION_IMS_SBI);

    ogs_list_add(&self.sess_list, sess);

    return sess;
}

void af_sess_remove(af_sess_t *sess)
{
    int i;

    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);

    /* Free SBI object memory */
    ogs_sbi_object_free(&sess->sbi);

    if (sess->app_session.af.id)
        ogs_free(sess->app_session.af.id);

    PCF_APP_SESSION_CLEAR(sess);
    if (sess->app_session.pcf.client)
        ogs_sbi_client_remove(sess->app_session.pcf.client);

    if (sess->ipv4addr)
        ogs_free(sess->ipv4addr);
    if (sess->ipv6addr)
        ogs_free(sess->ipv6addr);
    if (sess->ipv6prefix)
        ogs_free(sess->ipv6prefix);

    if (sess->supi)
        ogs_free(sess->supi);
    if (sess->gpsi)
        ogs_free(sess->gpsi);

    if (sess->dnn)
        ogs_free(sess->dnn);

    if (sess->pcf.fqdn)
        ogs_free(sess->pcf.fqdn);

    for (i = 0; i < sess->pcf.num_of_ip; i++) {
        if (sess->pcf.ip[i].addr)
            ogs_freeaddrinfo(sess->pcf.ip[i].addr);
        if (sess->pcf.ip[i].addr6)
            ogs_freeaddrinfo(sess->pcf.ip[i].addr6);
    }
    sess->pcf.num_of_ip = 0;

    if (sess->pcf.client)
        ogs_sbi_client_remove(sess->pcf.client);

    ogs_pool_free(&af_sess_pool, sess);
}

void af_sess_remove_all(void)
{
    af_sess_t *sess = NULL, *next_sess = NULL;

    ogs_list_for_each_safe(&self.sess_list, next_sess, sess)
        af_sess_remove(sess);
}

af_sess_t *af_sess_find(uint32_t index)
{
    return ogs_pool_find(&af_sess_pool, index);
}

af_sess_t *af_sess_find_by_af_app_session_id(char *af_app_session_id)
{
    ogs_assert(af_app_session_id);
    return af_sess_find(atoll(af_app_session_id));
}

void af_sess_associate_pcf_client(af_sess_t *sess)
{
    ogs_sbi_client_t *client = NULL;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;

    ogs_assert(sess);

    scheme = ogs_sbi_self()->tls.client.scheme;
    ogs_assert(scheme);

    if (!client) {
        /* At this point, CLIENT selection method is very simple. */
        if (sess->pcf.num_of_ip) {
            addr = sess->pcf.ip[0].addr;
            addr6 = sess->pcf.ip[0].addr6;
        }

        if (sess->pcf.fqdn || addr || addr6) {
            client = ogs_sbi_client_find(
                    scheme, sess->pcf.fqdn, 0, addr, addr6);
            if (!client) {
                ogs_debug("%s: ogs_sbi_client_add()", OGS_FUNC);
                client = ogs_sbi_client_add(
                        scheme, sess->pcf.fqdn, 0, addr, addr6);
                if (!client) {
                    ogs_error("%s: ogs_sbi_client_add() failed", OGS_FUNC);
                    return;
                }
            }
        }
    }

    if (client)
        OGS_SBI_SETUP_CLIENT(&sess->pcf, client);
}
