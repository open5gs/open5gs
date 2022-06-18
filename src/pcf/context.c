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

static pcf_context_t self;

int __pcf_log_domain;

static OGS_POOL(pcf_ue_pool, pcf_ue_t);
static OGS_POOL(pcf_sess_pool, pcf_sess_t);
static OGS_POOL(pcf_app_pool, pcf_app_t);

static int context_initialized = 0;

static void clear_ipv4addr(pcf_sess_t *sess);
static void clear_ipv6prefix(pcf_sess_t *sess);

void pcf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize PCF context */
    memset(&self, 0, sizeof(pcf_context_t));

    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", ogs_core()->log.level);
    ogs_log_install_domain(&__pcf_log_domain, "pcf", ogs_core()->log.level);

    ogs_pool_init(&pcf_ue_pool, ogs_app()->max.ue);
    ogs_pool_init(&pcf_sess_pool, ogs_app()->pool.sess);
    ogs_pool_init(&pcf_app_pool, ogs_app()->pool.sess);

    ogs_list_init(&self.pcf_ue_list);

    self.supi_hash = ogs_hash_make();
    ogs_assert(self.supi_hash);
    self.ipv4addr_hash = ogs_hash_make();
    ogs_assert(self.ipv4addr_hash);
    self.ipv6prefix_hash = ogs_hash_make();
    ogs_assert(self.ipv6prefix_hash);

    context_initialized = 1;
}

void pcf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    pcf_ue_remove_all();

    ogs_assert(self.supi_hash);
    ogs_hash_destroy(self.supi_hash);
    ogs_assert(self.ipv4addr_hash);
    ogs_hash_destroy(self.ipv4addr_hash);
    ogs_assert(self.ipv6prefix_hash);
    ogs_hash_destroy(self.ipv6prefix_hash);

    ogs_pool_final(&pcf_app_pool);
    ogs_pool_final(&pcf_sess_pool);
    ogs_pool_final(&pcf_ue_pool);

    context_initialized = 0;
}

pcf_context_t *pcf_self(void)
{
    return &self;
}

static int pcf_context_prepare(void)
{
    return OGS_OK;
}

static int pcf_context_validation(void)
{
    return OGS_OK;
}

int pcf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = pcf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "pcf")) {
            ogs_yaml_iter_t pcf_iter;
            ogs_yaml_iter_recurse(&root_iter, &pcf_iter);
            while (ogs_yaml_iter_next(&pcf_iter)) {
                const char *pcf_key = ogs_yaml_iter_key(&pcf_iter);
                ogs_assert(pcf_key);
                if (!strcmp(pcf_key, "sbi")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", pcf_key);
            }
        }
    }

    rv = pcf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

pcf_ue_t *pcf_ue_add(char *supi)
{
    pcf_event_t e;
    pcf_ue_t *pcf_ue = NULL;

    ogs_assert(supi);

    ogs_pool_alloc(&pcf_ue_pool, &pcf_ue);
    ogs_assert(pcf_ue);
    memset(pcf_ue, 0, sizeof *pcf_ue);

    /* SBI Type */
    pcf_ue->sbi.type = OGS_SBI_OBJ_UE_TYPE;

    /* SBI Features */
    OGS_SBI_FEATURES_SET(pcf_ue->am_policy_control_features,
            OGS_SBI_NPCF_AM_POLICY_CONTROL_UE_AMBR_AUTHORIZATION);

    pcf_ue->association_id = ogs_msprintf("%d",
            (int)ogs_pool_index(&pcf_ue_pool, pcf_ue));
    ogs_assert(pcf_ue->association_id);

    pcf_ue->supi = ogs_strdup(supi);
    ogs_assert(pcf_ue->supi);
    ogs_hash_set(self.supi_hash, pcf_ue->supi, strlen(pcf_ue->supi), pcf_ue);

    memset(&e, 0, sizeof(e));
    e.pcf_ue = pcf_ue;
    ogs_fsm_create(&pcf_ue->sm, pcf_am_state_initial, pcf_am_state_final);
    ogs_fsm_init(&pcf_ue->sm, &e);

    ogs_list_add(&self.pcf_ue_list, pcf_ue);

    return pcf_ue;
}

void pcf_ue_remove(pcf_ue_t *pcf_ue)
{
    pcf_event_t e;

    ogs_assert(pcf_ue);

    ogs_list_remove(&self.pcf_ue_list, pcf_ue);

    memset(&e, 0, sizeof(e));
    e.pcf_ue = pcf_ue;
    ogs_fsm_fini(&pcf_ue->sm, &e);
    ogs_fsm_delete(&pcf_ue->sm);

    /* Free SBI object memory */
    ogs_sbi_object_free(&pcf_ue->sbi);

    pcf_sess_remove_all(pcf_ue);

    OpenAPI_policy_association_request_free(pcf_ue->policy_association_request);
    if (pcf_ue->subscribed_ue_ambr)
        OpenAPI_ambr_free(pcf_ue->subscribed_ue_ambr);

    ogs_assert(pcf_ue->association_id);
    ogs_free(pcf_ue->association_id);

    ogs_assert(pcf_ue->supi);
    ogs_hash_set(self.supi_hash, pcf_ue->supi, strlen(pcf_ue->supi), NULL);
    ogs_free(pcf_ue->supi);

    if (pcf_ue->notification_uri)
        ogs_free(pcf_ue->notification_uri);
    if (pcf_ue->namf.client)
        ogs_sbi_client_remove(pcf_ue->namf.client);

    if (pcf_ue->gpsi)
        ogs_free(pcf_ue->gpsi);
    if (pcf_ue->pei)
        ogs_free(pcf_ue->pei);

    ogs_pool_free(&pcf_ue_pool, pcf_ue);
}

void pcf_ue_remove_all()
{
    pcf_ue_t *pcf_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.pcf_ue_list, next, pcf_ue)
        pcf_ue_remove(pcf_ue);
}

pcf_ue_t *pcf_ue_find_by_supi(char *supi)
{
    ogs_assert(supi);
    return (pcf_ue_t *)ogs_hash_get(self.supi_hash, supi, strlen(supi));
}

pcf_ue_t *pcf_ue_find_by_association_id(char *association_id)
{
    ogs_assert(association_id);
    return ogs_pool_find(&pcf_ue_pool, atoll(association_id));
}

pcf_sess_t *pcf_sess_add(pcf_ue_t *pcf_ue, uint8_t psi)
{
    pcf_event_t e;
    pcf_sess_t *sess = NULL;

    ogs_assert(pcf_ue);
    ogs_assert(psi != OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED);

    ogs_pool_alloc(&pcf_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    ogs_list_init(&sess->app_list);

    /* SBI Type */
    sess->sbi.type = OGS_SBI_OBJ_SESS_TYPE;

    /* SBI Features */
    OGS_SBI_FEATURES_SET(sess->smpolicycontrol_features,
            OGS_SBI_NPCF_SMPOLICYCONTROL_DN_AUTHORIZATION);

    OGS_SBI_FEATURES_SET(sess->management_features,
            OGS_SBI_NBSF_MANAGEMENT_BINDING_UPDATE);

    OGS_SBI_FEATURES_SET(sess->policyauthorization_features,
            OGS_SBI_NPCF_POLICYAUTHORIZATION_SPONSORED_CONNECTIVITY);
    OGS_SBI_FEATURES_SET(sess->policyauthorization_features,
            OGS_SBI_NPCF_POLICYAUTHORIZATION_IMS_SBI);

    sess->sm_policy_id = ogs_msprintf("%d",
            (int)ogs_pool_index(&pcf_sess_pool, sess));
    ogs_assert(sess->sm_policy_id);

    sess->pcf_ue = pcf_ue;
    sess->psi = psi;

    sess->s_nssai.sst = 0;
    sess->s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    memset(&e, 0, sizeof(e));
    e.sess = sess;
    ogs_fsm_create(&sess->sm, pcf_sm_state_initial, pcf_sm_state_final);
    ogs_fsm_init(&sess->sm, &e);

    ogs_list_add(&pcf_ue->sess_list, sess);

    return sess;
}

void pcf_sess_remove(pcf_sess_t *sess)
{
    pcf_event_t e;

    ogs_assert(sess);
    ogs_assert(sess->pcf_ue);

    ogs_list_remove(&sess->pcf_ue->sess_list, sess);

    memset(&e, 0, sizeof(e));
    e.sess = sess;
    ogs_fsm_fini(&sess->sm, &e);
    ogs_fsm_delete(&sess->sm);

    /* Free SBI object memory */
    ogs_sbi_object_free(&sess->sbi);

    pcf_app_remove_all(sess);

    ogs_assert(sess->sm_policy_id);
    ogs_free(sess->sm_policy_id);

    if (sess->binding_id)
        ogs_free(sess->binding_id);

    if (sess->dnn)
        ogs_free(sess->dnn);

    if (sess->notification_uri)
        ogs_free(sess->notification_uri);
    if (sess->nsmf.client)
        ogs_sbi_client_remove(sess->nsmf.client);

    clear_ipv4addr(sess);
    clear_ipv6prefix(sess);

    if (sess->subscribed_sess_ambr)
        OpenAPI_ambr_free(sess->subscribed_sess_ambr);
    if (sess->subscribed_default_qos)
        OpenAPI_subscribed_default_qos_free(sess->subscribed_default_qos);

    ogs_pool_free(&pcf_sess_pool, sess);
}

void pcf_sess_remove_all(pcf_ue_t *pcf_ue)
{
    pcf_sess_t *sess = NULL, *next_sess = NULL;

    ogs_assert(pcf_ue);

    ogs_list_for_each_safe(&pcf_ue->sess_list, next_sess, sess)
        pcf_sess_remove(sess);
}

static void clear_ipv4addr(pcf_sess_t *sess)
{
    ogs_assert(sess);

    if (sess->ipv4addr_string) {
        ogs_hash_set(self.ipv4addr_hash,
                &sess->ipv4addr, sizeof(sess->ipv4addr), NULL);
        ogs_free(sess->ipv4addr_string);
    }
}

static void clear_ipv6prefix(pcf_sess_t *sess)
{
    ogs_assert(sess);

    if (sess->ipv6prefix_string) {
        ogs_hash_set(self.ipv6prefix_hash,
                &sess->ipv6prefix, (sess->ipv6prefix.len >> 3) + 1, NULL);
        ogs_free(sess->ipv6prefix_string);
    }
}

bool pcf_sess_set_ipv4addr(pcf_sess_t *sess, char *ipv4addr_string)
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

bool pcf_sess_set_ipv6prefix(pcf_sess_t *sess, char *ipv6prefix_string)
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

pcf_sess_t *pcf_sess_find(uint32_t index)
{
    return ogs_pool_find(&pcf_sess_pool, index);
}

pcf_sess_t *pcf_sess_find_by_sm_policy_id(char *sm_policy_id)
{
    ogs_assert(sm_policy_id);
    return pcf_sess_find(atoll(sm_policy_id));
}

pcf_sess_t *pcf_sess_find_by_psi(pcf_ue_t *pcf_ue, uint8_t psi)
{
    pcf_sess_t *sess = NULL;

    ogs_list_for_each(&pcf_ue->sess_list, sess)
        if (psi == sess->psi) return sess;

    return NULL;
}

pcf_sess_t *pcf_sess_find_by_ipv4addr(char *ipv4addr_string)
{
    uint32_t ipv4addr;
    int rv;

    ogs_assert(ipv4addr_string);

    rv = ogs_ipv4_from_string(&ipv4addr, ipv4addr_string);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);

    return ogs_hash_get(self.ipv4addr_hash, &ipv4addr, sizeof(ipv4addr));
}

int pcf_sessions_number_by_snssai_and_dnn(
        pcf_ue_t *pcf_ue, ogs_s_nssai_t *s_nssai, char *dnn)
{
    int number_of_sessions = 0;
    pcf_sess_t *sess = NULL;

    ogs_assert(s_nssai);
    ogs_assert(dnn);

    ogs_list_for_each(&pcf_ue->sess_list, sess)
        if (sess->s_nssai.sst == s_nssai->sst &&
            sess->dnn && strcmp(sess->dnn, dnn) == 0)
            number_of_sessions++;

    return number_of_sessions;
}

pcf_sess_t *pcf_sess_find_by_ipv6addr(char *ipv6addr_string)
{
    int rv;
    ogs_sockaddr_t tmp;
    struct {
        uint8_t len;
        uint8_t addr6[OGS_IPV6_LEN];
    } ipv6prefix;

    ogs_assert(ipv6addr_string);

    rv = ogs_inet_pton(AF_INET6, ipv6addr_string, &tmp);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);

    memcpy(ipv6prefix.addr6, tmp.sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
    ipv6prefix.len = OGS_IPV6_128_PREFIX_LEN;

    return ogs_hash_get(self.ipv6prefix_hash,
            &ipv6prefix, (ipv6prefix.len >> 3) + 1);
}

pcf_sess_t *pcf_sess_find_by_ipv6prefix(char *ipv6prefix_string)
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

pcf_ue_t *pcf_ue_cycle(pcf_ue_t *pcf_ue)
{
    return ogs_pool_cycle(&pcf_ue_pool, pcf_ue);
}

pcf_sess_t *pcf_sess_cycle(pcf_sess_t *sess)
{
    return ogs_pool_cycle(&pcf_sess_pool, sess);
}

void pcf_ue_select_nf(pcf_ue_t *pcf_ue, OpenAPI_nf_type_e nf_type)
{
    ogs_assert(pcf_ue);
    ogs_assert(nf_type);

    ogs_sbi_select_nf(&pcf_ue->sbi, nf_type, pcf_nf_state_registered);
}

void pcf_sess_select_nf(pcf_sess_t *sess, OpenAPI_nf_type_e nf_type)
{
    ogs_assert(sess);
    ogs_assert(nf_type);

    ogs_sbi_select_nf(&sess->sbi, nf_type, pcf_nf_state_registered);
}

pcf_app_t *pcf_app_add(pcf_sess_t *sess)
{
    pcf_app_t *app = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&pcf_app_pool, &app);
    ogs_assert(app);
    memset(app, 0, sizeof *app);

    app->app_session_id = ogs_msprintf("%d",
            (int)ogs_pool_index(&pcf_app_pool, app));
    ogs_assert(app->app_session_id);

    app->sess = sess;

    ogs_list_add(&sess->app_list, app);

    return app;
}

int pcf_app_remove(pcf_app_t *app)
{
    int i;

    ogs_assert(app);
    ogs_assert(app->sess);

    ogs_list_remove(&app->sess->app_list, app);

    for (i = 0; i < app->num_of_pcc_rule; i++) {
        OGS_PCC_RULE_FREE(&app->pcc_rule[i]);
    }

    if (app->notif_uri)
        ogs_free(app->notif_uri);
    if (app->naf.client)
        ogs_sbi_client_remove(app->naf.client);

    ogs_assert(app->app_session_id);
    ogs_free(app->app_session_id);

    ogs_pool_free(&pcf_app_pool, app);

    return OGS_OK;
}

void pcf_app_remove_all(pcf_sess_t *sess)
{
    pcf_app_t *app = NULL, *next_app = NULL;

    ogs_assert(sess);
    ogs_list_for_each_safe(&sess->app_list, next_app, app)
        pcf_app_remove(app);
}

pcf_app_t *pcf_app_find(uint32_t index)
{
    return ogs_pool_find(&pcf_app_pool, index);
}

pcf_app_t *pcf_app_find_by_app_session_id(char *app_session_id)
{
    ogs_assert(app_session_id);
    return pcf_app_find(atoll(app_session_id));
}
