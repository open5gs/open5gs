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

#include "context.h"

static bsf_context_t self;

int __bsf_log_domain;

static OGS_POOL(bsf_sess_pool, bsf_sess_t);

static int context_initialized = 0;

static void clear_ipv4addr(bsf_sess_t *sess);
static void clear_ipv6prefix(bsf_sess_t *sess);

void bsf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize BSF context */
    memset(&self, 0, sizeof(bsf_context_t));

    ogs_log_install_domain(&__bsf_log_domain, "bsf", ogs_core()->log.level);

    ogs_pool_init(&bsf_sess_pool, ogs_app()->pool.sess);

    self.ipv4addr_hash = ogs_hash_make();
    ogs_assert(self.ipv4addr_hash);
    self.ipv6prefix_hash = ogs_hash_make();
    ogs_assert(self.ipv6prefix_hash);

    context_initialized = 1;
}

void bsf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    bsf_sess_remove_all();

    ogs_assert(self.ipv4addr_hash);
    ogs_hash_destroy(self.ipv4addr_hash);
    ogs_assert(self.ipv6prefix_hash);
    ogs_hash_destroy(self.ipv6prefix_hash);

    ogs_pool_final(&bsf_sess_pool);

    context_initialized = 0;
}

bsf_context_t *bsf_self(void)
{
    return &self;
}

static int bsf_context_prepare(void)
{
    return OGS_OK;
}

static int bsf_context_validation(void)
{
    return OGS_OK;
}

int bsf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = bsf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "bsf")) {
            ogs_yaml_iter_t bsf_iter;
            ogs_yaml_iter_recurse(&root_iter, &bsf_iter);
            while (ogs_yaml_iter_next(&bsf_iter)) {
                const char *bsf_key = ogs_yaml_iter_key(&bsf_iter);
                ogs_assert(bsf_key);
                if (!strcmp(bsf_key, "sbi")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", bsf_key);
            }
        }
    }

    rv = bsf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

bsf_sess_t *bsf_sess_add_by_snssai_and_dnn(ogs_s_nssai_t *s_nssai, char *dnn)
{
    bsf_sess_t *sess = NULL;

    ogs_assert(s_nssai);
    ogs_assert(s_nssai->sst);
    ogs_assert(dnn);

    ogs_pool_alloc(&bsf_sess_pool, &sess);
    if (!sess) {
        ogs_error("Maximum number of session[%lld] reached",
            (long long)ogs_app()->pool.sess);
        return NULL;
    }
    memset(sess, 0, sizeof *sess);

    /* SBI Features */
    OGS_SBI_FEATURES_SET(sess->management_features,
            OGS_SBI_NBSF_MANAGEMENT_BINDING_UPDATE);

    sess->s_nssai.sst = s_nssai->sst;
    sess->s_nssai.sd.v = s_nssai->sd.v;

    sess->dnn = ogs_strdup(dnn);
    ogs_assert(sess->dnn);

    sess->binding_id = ogs_msprintf("%d",
            (int)ogs_pool_index(&bsf_sess_pool, sess));
    ogs_assert(sess->binding_id);

    ogs_list_add(&self.sess_list, sess);

    return sess;
}

void bsf_sess_remove(bsf_sess_t *sess)
{
    int i;

    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);

    /* Free SBI object memory */
    ogs_sbi_object_free(&sess->sbi);

    ogs_assert(sess->binding_id);
    ogs_free(sess->binding_id);

    if (sess->supi)
        ogs_free(sess->supi);
    if (sess->gpsi)
        ogs_free(sess->gpsi);

    clear_ipv4addr(sess);
    clear_ipv6prefix(sess);

    ogs_assert(sess->dnn);
    ogs_free(sess->dnn);

    if (sess->pcf_fqdn)
        ogs_free(sess->pcf_fqdn);

    for (i = 0; i < sess->num_of_pcf_ip; i++) {
        if (sess->pcf_ip[i].addr)
            ogs_free(sess->pcf_ip[i].addr);
        if (sess->pcf_ip[i].addr6)
            ogs_free(sess->pcf_ip[i].addr6);
    }
    sess->num_of_pcf_ip = 0;

    ogs_pool_free(&bsf_sess_pool, sess);
}

void bsf_sess_remove_all(void)
{
    bsf_sess_t *sess = NULL, *next_sess = NULL;

    ogs_list_for_each_safe(&self.sess_list, next_sess, sess)
        bsf_sess_remove(sess);
}

static void clear_ipv4addr(bsf_sess_t *sess)
{
    ogs_assert(sess);

    if (sess->ipv4addr_string) {
        ogs_hash_set(self.ipv4addr_hash,
                &sess->ipv4addr, sizeof(sess->ipv4addr), NULL);
        ogs_free(sess->ipv4addr_string);
    }
}

static void clear_ipv6prefix(bsf_sess_t *sess)
{
    ogs_assert(sess);

    if (sess->ipv6prefix_string) {
        ogs_hash_set(self.ipv6prefix_hash,
                &sess->ipv6prefix, (sess->ipv6prefix.len >> 3) + 1, NULL);
        ogs_free(sess->ipv6prefix_string);
    }
}

bool bsf_sess_set_ipv4addr(bsf_sess_t *sess, char *ipv4addr_string)
{
    int rv;

    ogs_assert(sess);
    ogs_assert(ipv4addr_string);

    clear_ipv4addr(sess);

    rv = ogs_ipv4_from_string(&sess->ipv4addr, ipv4addr_string);
    ogs_expect_or_return_val(rv == OGS_OK, false);

    sess->ipv4addr_string = ogs_strdup(ipv4addr_string);
    ogs_expect_or_return_val(sess->ipv4addr_string, false);

    ogs_hash_set(self.ipv4addr_hash,
            &sess->ipv4addr, sizeof(sess->ipv4addr), sess);

    return true;
}

bool bsf_sess_set_ipv6prefix(bsf_sess_t *sess, char *ipv6prefix_string)
{
    int rv;

    ogs_assert(sess);
    ogs_assert(ipv6prefix_string);

    clear_ipv6prefix(sess);

    rv = ogs_ipv6prefix_from_string(
            sess->ipv6prefix.addr6, &sess->ipv6prefix.len, ipv6prefix_string);
    ogs_expect_or_return_val(rv == OGS_OK, false);

    ogs_assert(sess->ipv6prefix.len == OGS_IPV6_128_PREFIX_LEN);

    sess->ipv6prefix_string = ogs_strdup(ipv6prefix_string);
    ogs_expect_or_return_val(sess->ipv6prefix_string, false);

    ogs_hash_set(self.ipv6prefix_hash,
            &sess->ipv6prefix, (sess->ipv6prefix.len >> 3) + 1, sess);

    return true;
}

bsf_sess_t *bsf_sess_find(uint32_t index)
{
    return ogs_pool_find(&bsf_sess_pool, index);
}

bsf_sess_t *bsf_sess_find_by_snssai_and_dnn(ogs_s_nssai_t *s_nssai, char *dnn)
{
    bsf_sess_t *sess = NULL;

    ogs_assert(s_nssai);
    ogs_assert(dnn);

    ogs_list_for_each(&self.sess_list, sess)
        if (sess->s_nssai.sst == s_nssai->sst &&
            sess->dnn && strcmp(sess->dnn, dnn) == 0)
            return sess;

    return NULL;
}

bsf_sess_t *bsf_sess_find_by_binding_id(char *binding_id)
{
    ogs_assert(binding_id);
    return bsf_sess_find(atoll(binding_id));
}

bsf_sess_t *bsf_sess_find_by_ipv4addr(char *ipv4addr_string)
{
    uint32_t ipv4addr;
    int rv;

    ogs_assert(ipv4addr_string);

    rv = ogs_ipv4_from_string(&ipv4addr, ipv4addr_string);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);

    return ogs_hash_get(self.ipv4addr_hash, &ipv4addr, sizeof(ipv4addr));
}

bsf_sess_t *bsf_sess_find_by_ipv6prefix(char *ipv6prefix_string)
{
    int rv;
    struct {
        uint8_t len;
        uint8_t addr6[OGS_IPV6_LEN];
    } ipv6prefix;

    ogs_assert(ipv6prefix_string);

    rv = ogs_ipv6prefix_from_string(
            ipv6prefix.addr6, &ipv6prefix.len, ipv6prefix_string);
    ogs_assert(rv == OGS_OK);

    ogs_assert(ipv6prefix.len == OGS_IPV6_128_PREFIX_LEN);

    return ogs_hash_get(self.ipv6prefix_hash,
            &ipv6prefix, (ipv6prefix.len >> 3) + 1);
}

void bsf_sess_select_nf(bsf_sess_t *sess, OpenAPI_nf_type_e nf_type)
{
    ogs_assert(sess);
    ogs_assert(nf_type);

    ogs_sbi_select_nf(&sess->sbi, nf_type, bsf_nf_state_registered);
}
