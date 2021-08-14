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

static upf_context_t self;

int __upf_log_domain;

static OGS_POOL(upf_sess_pool, upf_sess_t);

static int context_initialized = 0;

void upf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize UPF context */
    memset(&self, 0, sizeof(upf_context_t));

    ogs_log_install_domain(&__upf_log_domain, "upf", ogs_core()->log.level);

    /* Setup UP Function Features */
    ogs_pfcp_self()->up_function_features.ftup = 1;
    ogs_pfcp_self()->up_function_features.empu = 1;
    ogs_pfcp_self()->up_function_features_len = 2;

    ogs_list_init(&self.sess_list);
    ogs_pool_init(&upf_sess_pool, ogs_app()->pool.sess);

    self.sess_hash = ogs_hash_make();
    ogs_assert(self.sess_hash);
    self.ipv4_hash = ogs_hash_make();
    ogs_assert(self.ipv4_hash);
    self.ipv6_hash = ogs_hash_make();
    ogs_assert(self.ipv6_hash);

    context_initialized = 1;
}

void upf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    upf_sess_remove_all();

    ogs_assert(self.sess_hash);
    ogs_hash_destroy(self.sess_hash);
    ogs_assert(self.ipv4_hash);
    ogs_hash_destroy(self.ipv4_hash);
    ogs_assert(self.ipv6_hash);
    ogs_hash_destroy(self.ipv6_hash);

    ogs_pool_final(&upf_sess_pool);

    context_initialized = 0;
}

upf_context_t *upf_self(void)
{
    return &self;
}

static int upf_context_prepare(void)
{
    return OGS_OK;
}

static int upf_context_validation(void)
{
    if (ogs_list_first(&ogs_gtp_self()->gtpu_list) == NULL) {
        ogs_error("No upf.gtpu in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    if (ogs_list_first(&ogs_pfcp_self()->subnet_list) == NULL) {
        ogs_error("No upf.subnet: in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int upf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = upf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "upf")) {
            ogs_yaml_iter_t upf_iter;
            ogs_yaml_iter_recurse(&root_iter, &upf_iter);
            while (ogs_yaml_iter_next(&upf_iter)) {
                const char *upf_key = ogs_yaml_iter_key(&upf_iter);
                ogs_assert(upf_key);
                if (!strcmp(upf_key, "gtpu")) {
                    /* handle config in gtp library */
                } else if (!strcmp(upf_key, "pfcp")) {
                    /* handle config in pfcp library */
                } else if (!strcmp(upf_key, "subnet")) {
                    /* handle config in pfcp library */
                } else
                    ogs_warn("unknown key `%s`", upf_key);
            }
        }
    }

    rv = upf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

upf_sess_t *upf_sess_add(ogs_pfcp_f_seid_t *cp_f_seid)
{
    upf_sess_t *sess = NULL;

    ogs_assert(cp_f_seid);

    ogs_pool_alloc(&upf_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    ogs_pfcp_pool_init(&sess->pfcp);

    sess->index = ogs_pool_index(&upf_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_app()->pool.sess);

    sess->upf_n4_seid = sess->index;
    sess->smf_n4_seid = cp_f_seid->seid;
    ogs_hash_set(self.sess_hash, &sess->smf_n4_seid,
            sizeof(sess->smf_n4_seid), sess);

    ogs_list_add(&self.sess_list, sess);

    ogs_info("[Added] Number of UPF-Sessions is now %d",
            ogs_list_count(&self.sess_list));

    return sess;
}

int upf_sess_remove(upf_sess_t *sess)
{
    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);
    ogs_pfcp_sess_clear(&sess->pfcp);

    ogs_hash_set(self.sess_hash, &sess->smf_n4_seid,
            sizeof(sess->smf_n4_seid), NULL);

    if (sess->ipv4) {
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv4);
    }
    if (sess->ipv6) {
        ogs_hash_set(self.ipv6_hash,
                sess->ipv6->addr, OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv6);
    }

    ogs_pfcp_pool_final(&sess->pfcp);

    ogs_pool_free(&upf_sess_pool, sess);

    ogs_info("[Removed] Number of UPF-sessions is now %d",
            ogs_list_count(&self.sess_list));

    return OGS_OK;
}

void upf_sess_remove_all(void)
{
    upf_sess_t *sess = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.sess_list, next, sess) {
        upf_sess_remove(sess);
    }
}

upf_sess_t *upf_sess_find(uint32_t index)
{
    return ogs_pool_find(&upf_sess_pool, index);
}

upf_sess_t *upf_sess_find_by_cp_seid(uint64_t seid)
{
    return (upf_sess_t *)ogs_hash_get(self.sess_hash, &seid, sizeof(seid));
}

upf_sess_t *upf_sess_find_by_up_seid(uint64_t seid)
{
    return upf_sess_find(seid);
}

upf_sess_t *upf_sess_find_by_ipv4(uint32_t addr)
{
    ogs_assert(self.ipv4_hash);
    return (upf_sess_t *)ogs_hash_get(self.ipv4_hash, &addr, OGS_IPV4_LEN);
}

upf_sess_t *upf_sess_find_by_ipv6(uint32_t *addr6)
{
    ogs_assert(self.ipv6_hash);
    ogs_assert(addr6);
    return (upf_sess_t *)ogs_hash_get(
            self.ipv6_hash, addr6, OGS_IPV6_DEFAULT_PREFIX_LEN >> 3);
}

upf_sess_t *upf_sess_add_by_message(ogs_pfcp_message_t *message)
{
    upf_sess_t *sess = NULL;
    ogs_pfcp_f_seid_t *f_seid = NULL;

    ogs_pfcp_session_establishment_request_t *req =
        &message->pfcp_session_establishment_request;;

    f_seid = req->cp_f_seid.data;
    if (req->cp_f_seid.presence == 0 || f_seid == NULL) {
        ogs_error("No CP F-SEID");
        return NULL;
    }
    f_seid->seid = be64toh(f_seid->seid);

    sess = upf_sess_find_by_cp_seid(f_seid->seid);
    if (!sess) {
        sess = upf_sess_add(f_seid);
        if (!sess) return NULL;
    }
    ogs_assert(sess);

    return sess;
}

uint8_t upf_sess_set_ue_ip(upf_sess_t *sess,
        uint8_t session_type, ogs_pfcp_pdr_t *pdr)
{
    ogs_pfcp_ue_ip_addr_t *ue_ip = NULL;
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];

    uint8_t cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    ogs_assert(sess);
    ogs_assert(session_type);
    ogs_assert(pdr);
    ogs_assert(pdr->ue_ip_addr_len);
    ue_ip = &pdr->ue_ip_addr;
    ogs_assert(ue_ip);

    if (sess->ipv4) {
        ogs_hash_set(self.ipv4_hash,
                sess->ipv4->addr, OGS_IPV4_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv4);
    }
    if (sess->ipv6) {
        ogs_hash_set(self.ipv6_hash,
                sess->ipv6->addr, OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv6);
    }

    /* Set PDN-Type and UE IP Address */
    if (session_type == OGS_PDU_SESSION_TYPE_IPV4) {
        if (ue_ip->ipv4 || pdr->dnn) {
            sess->ipv4 = ogs_pfcp_ue_ip_alloc(&cause_value, AF_INET,
                            pdr->dnn, (uint8_t *)&(ue_ip->addr));
            if (!sess->ipv4) {
                ogs_error("ogs_pfcp_ue_ip_alloc() failed[%d]", cause_value);
                ogs_assert(cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
                return cause_value;
            }
            ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
        }
    } else if (session_type == OGS_PDU_SESSION_TYPE_IPV6) {
        if (ue_ip->ipv6 || pdr->dnn) {
            sess->ipv6 = ogs_pfcp_ue_ip_alloc(&cause_value, AF_INET6,
                            pdr->dnn, ue_ip->addr6);
            if (!sess->ipv6) {
                ogs_error("ogs_pfcp_ue_ip_alloc() failed[%d]", cause_value);
                ogs_assert(cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
                return cause_value;
            }
            ogs_hash_set(self.ipv6_hash, sess->ipv6->addr,
                    OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
        }
    } else if (session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        if (ue_ip->ipv4 || pdr->dnn) {
            sess->ipv4 = ogs_pfcp_ue_ip_alloc(&cause_value, AF_INET,
                            pdr->dnn, (uint8_t *)&(ue_ip->both.addr));
            if (!sess->ipv4) {
                ogs_error("ogs_pfcp_ue_ip_alloc() failed[%d]", cause_value);
                ogs_assert(cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
                return cause_value;
            }
            ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
        }

        if (ue_ip->ipv6 || pdr->dnn) {
            sess->ipv6 = ogs_pfcp_ue_ip_alloc(&cause_value, AF_INET6,
                            pdr->dnn, ue_ip->both.addr6);
            if (!sess->ipv6) {
                ogs_error("ogs_pfcp_ue_ip_alloc() failed[%d]", cause_value);
                ogs_assert(cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
                if (sess->ipv4) {
                    ogs_hash_set(self.ipv4_hash,
                            sess->ipv4->addr, OGS_IPV4_LEN, NULL);
                    ogs_pfcp_ue_ip_free(sess->ipv4);
                    sess->ipv4 = NULL;
                }
                return cause_value;
            }
            ogs_hash_set(self.ipv6_hash, sess->ipv6->addr,
                    OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
        }
    } else {
        ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
    }

    ogs_info("UE F-SEID[CP:0x%lx UP:0x%lx] "
             "APN[%s] PDN-Type[%d] IPv4[%s] IPv6[%s]",
        (long)sess->upf_n4_seid, (long)sess->smf_n4_seid,
        pdr->dnn, session_type,
        sess->ipv4 ? OGS_INET_NTOP(&sess->ipv4->addr, buf1) : "",
        sess->ipv6 ? OGS_INET6_NTOP(&sess->ipv6->addr, buf2) : "");

    return cause_value;
}
