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

static sgwu_context_t self;

int __sgwu_log_domain;

static OGS_POOL(sgwu_sess_pool, sgwu_sess_t);

static int context_initialized = 0;

void sgwu_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize SGWU context */
    memset(&self, 0, sizeof(sgwu_context_t));

    ogs_log_install_domain(&__sgwu_log_domain, "sgwu", ogs_core()->log.level);

    /* Setup UP Function Features */
    ogs_pfcp_self()->up_function_features.ftup = 1;
    ogs_pfcp_self()->up_function_features.empu = 1;
    ogs_pfcp_self()->up_function_features_len = 2;

    ogs_list_init(&self.sess_list);
    ogs_pool_init(&sgwu_sess_pool, ogs_app()->pool.sess);

    self.sess_hash = ogs_hash_make();
    ogs_assert(self.sess_hash);

    context_initialized = 1;
}

void sgwu_context_final(void)
{
    ogs_assert(context_initialized == 1);

    sgwu_sess_remove_all();

    ogs_assert(self.sess_hash);
    ogs_hash_destroy(self.sess_hash);

    ogs_pool_final(&sgwu_sess_pool);

    context_initialized = 0;
}

sgwu_context_t *sgwu_self(void)
{
    return &self;
}

static int sgwu_context_prepare(void)
{
    return OGS_OK;
}

static int sgwu_context_validation(void)
{
    if (ogs_list_first(&ogs_gtp_self()->gtpu_list) == NULL) {
        ogs_error("No sgwu.gtpu in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int sgwu_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = sgwu_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "sgwu")) {
            ogs_yaml_iter_t sgwu_iter;
            ogs_yaml_iter_recurse(&root_iter, &sgwu_iter);
            while (ogs_yaml_iter_next(&sgwu_iter)) {
                const char *sgwu_key = ogs_yaml_iter_key(&sgwu_iter);
                ogs_assert(sgwu_key);
                if (!strcmp(sgwu_key, "gtpu")) {
                    /* handle config in gtp library */
                } else if (!strcmp(sgwu_key, "pfcp")) {
                    /* handle config in pfcp library */
                } else
                    ogs_warn("unknown key `%s`", sgwu_key);
            }
        }
    }

    rv = sgwu_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

sgwu_sess_t *sgwu_sess_add(ogs_pfcp_f_seid_t *cp_f_seid)
{
    sgwu_sess_t *sess = NULL;

    ogs_assert(cp_f_seid);

    ogs_pool_alloc(&sgwu_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    ogs_pfcp_pool_init(&sess->pfcp);

    sess->index = ogs_pool_index(&sgwu_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_app()->pool.sess);

    sess->sgwu_sxa_seid = sess->index;
    sess->sgwc_sxa_seid = cp_f_seid->seid;
    ogs_hash_set(self.sess_hash, &sess->sgwc_sxa_seid,
            sizeof(sess->sgwc_sxa_seid), sess);

    ogs_info("UE F-SEID[CP:0x%lx UP:0x%lx]",
        (long)sess->sgwu_sxa_seid, (long)sess->sgwc_sxa_seid);

    ogs_list_add(&self.sess_list, sess);

    ogs_info("[Added] Number of SGWU-Sessions is now %d",
            ogs_list_count(&self.sess_list));

    return sess;
}

int sgwu_sess_remove(sgwu_sess_t *sess)
{
    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);
    ogs_pfcp_sess_clear(&sess->pfcp);

    ogs_hash_set(self.sess_hash, &sess->sgwc_sxa_seid,
            sizeof(sess->sgwc_sxa_seid), NULL);

    ogs_pfcp_pool_final(&sess->pfcp);

    ogs_pool_free(&sgwu_sess_pool, sess);

    ogs_info("[Removed] Number of SGWU-sessions is now %d",
            ogs_list_count(&self.sess_list));

    return OGS_OK;
}

void sgwu_sess_remove_all(void)
{
    sgwu_sess_t *sess = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.sess_list, next, sess) {
        sgwu_sess_remove(sess);
    }
}

sgwu_sess_t *sgwu_sess_find(uint32_t index)
{
    return ogs_pool_find(&sgwu_sess_pool, index);
}

sgwu_sess_t *sgwu_sess_find_by_cp_seid(uint64_t seid)
{
    return (sgwu_sess_t *)ogs_hash_get(self.sess_hash, &seid, sizeof(seid));
}

sgwu_sess_t *sgwu_sess_find_by_up_seid(uint64_t seid)
{
    return sgwu_sess_find(seid);
}

sgwu_sess_t *sgwu_sess_add_by_message(ogs_pfcp_message_t *message)
{
    sgwu_sess_t *sess = NULL;

    ogs_pfcp_f_seid_t *f_seid = NULL;

    ogs_pfcp_session_establishment_request_t *req =
        &message->pfcp_session_establishment_request;;

    f_seid = req->cp_f_seid.data;
    if (req->cp_f_seid.presence == 0 || f_seid == NULL) {
        ogs_error("No CP F-SEID");
        return NULL;
    }
    f_seid->seid = be64toh(f_seid->seid);

    sess = sgwu_sess_find_by_cp_seid(f_seid->seid);
    if (!sess) {
        sess = sgwu_sess_add(f_seid);
        if (!sess) return NULL;
    }
    ogs_assert(sess);

    return sess;
}
