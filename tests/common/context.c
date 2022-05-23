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

#include "test-config-private.h"
#include "test-common.h"

static test_context_t self;

static OGS_POOL(test_ue_pool, test_ue_t);
static OGS_POOL(test_sess_pool, test_sess_t);
static OGS_POOL(test_bearer_pool, test_bearer_t);

static int context_initialized = 0;

void test_context_init(void)
{
    int rv;

    ogs_assert(context_initialized == 0);

    /* Initialize AMF context */
    memset(&self, 0, sizeof(test_context_t));

    ogs_pool_init(&test_ue_pool, ogs_app()->max.ue);
    ogs_pool_init(&test_sess_pool, ogs_app()->pool.sess);
    ogs_pool_init(&test_bearer_pool, ogs_app()->pool.bearer);

    rv = ogs_getaddrinfo(&test_self()->gnb1_addr, AF_UNSPEC,
            "127.0.0.2", OGS_GTPV1_U_UDP_PORT, 0);
    ogs_assert(rv == OGS_OK);
    rv = ogs_getaddrinfo(&test_self()->gnb1_addr6, AF_UNSPEC,
            "fd69:f21d:873c:fa::2", OGS_GTPV1_U_UDP_PORT, 0);
    ogs_assert(rv == OGS_OK);

    rv = ogs_getaddrinfo(&test_self()->gnb2_addr, AF_UNSPEC,
            "127.0.0.3", OGS_GTPV1_U_UDP_PORT, 0);
    ogs_assert(rv == OGS_OK);
    rv = ogs_getaddrinfo(&test_self()->gnb2_addr6, AF_UNSPEC,
            "fd69:f21d:873c:fa::3", OGS_GTPV1_U_UDP_PORT, 0);
    ogs_assert(rv == OGS_OK);

    context_initialized = 1;
}

void test_context_final(void)
{
    ogs_assert(context_initialized == 1);

    ogs_socknode_remove_all(&self.ngap_list);
    ogs_socknode_remove_all(&self.ngap_list6);

    ogs_socknode_remove_all(&self.s1ap_list);
    ogs_socknode_remove_all(&self.s1ap_list6);

    ogs_freeaddrinfo(test_self()->gnb1_addr);
    ogs_freeaddrinfo(test_self()->gnb1_addr6);
    ogs_freeaddrinfo(test_self()->gnb2_addr);
    ogs_freeaddrinfo(test_self()->gnb2_addr6);

    ogs_pool_final(&test_ue_pool);
    ogs_pool_final(&test_bearer_pool);
    ogs_pool_final(&test_sess_pool);

    context_initialized = 0;
}

test_context_t *test_self(void)
{
    return &self;
}

static int test_context_prepare(void)
{
    self.s1ap_port = OGS_S1AP_SCTP_PORT;
    self.ngap_port = OGS_NGAP_SCTP_PORT;

    return OGS_OK;
}

static int test_context_validation(void)
{
    ogs_socknode_t *snode = NULL;

    snode = ogs_list_first(&test_self()->s1ap_list);
    if (snode) test_self()->s1ap_addr = snode->addr;
    snode = ogs_list_first(&test_self()->s1ap_list6);
    if (snode) test_self()->s1ap_addr6 = snode->addr;

    snode = ogs_list_first(&test_self()->ngap_list);
    if (snode) test_self()->ngap_addr = snode->addr;
    snode = ogs_list_first(&test_self()->ngap_list6);
    if (snode) test_self()->ngap_addr6 = snode->addr;

    if (test_self()->e_served_tai[0].list2.num) {
        memcpy(&test_self()->e_tai,
            &test_self()->e_served_tai[0].list2.tai[0], sizeof(ogs_5gs_tai_t));
    } else if (test_self()->e_served_tai[0].list0.tai[0].num) {
        test_self()->e_tai.tac =
            test_self()->e_served_tai[0].list0.tai[0].tac[0];
        memcpy(&test_self()->e_tai.plmn_id,
                &test_self()->e_served_tai[0].list0.tai[0].plmn_id,
                OGS_PLMN_ID_LEN);
    }

    if (test_self()->nr_served_tai[0].list2.num) {
        memcpy(&test_self()->nr_tai,
            &test_self()->nr_served_tai[0].list2.tai[0], sizeof(ogs_5gs_tai_t));
    } else if (test_self()->nr_served_tai[0].list0.tai[0].num) {
        test_self()->nr_tai.tac =
            test_self()->nr_served_tai[0].list0.tai[0].tac[0];
        memcpy(&test_self()->nr_tai.plmn_id,
                &test_self()->nr_served_tai[0].list0.tai[0].plmn_id,
                OGS_PLMN_ID_LEN);
    }

    memcpy(&test_self()->nr_cgi.plmn_id, &test_self()->nr_tai.plmn_id,
            OGS_PLMN_ID_LEN);
    test_self()->nr_cgi.cell_id = 0x40001;

    return OGS_OK;
}

int test_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = test_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "amf")) {
            ogs_yaml_iter_t amf_iter;
            ogs_yaml_iter_recurse(&root_iter, &amf_iter);
            while (ogs_yaml_iter_next(&amf_iter)) {
                const char *amf_key = ogs_yaml_iter_key(&amf_iter);
                ogs_assert(amf_key);
                if (!strcmp(amf_key, "ngap")) {
                    ogs_yaml_iter_t ngap_array, ngap_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &ngap_array);
                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.ngap_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;

                        ogs_sockopt_t option;
                        bool is_option = false;

                        if (ogs_yaml_iter_type(&ngap_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&ngap_iter, &ngap_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&ngap_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&ngap_array))
                                break;
                            ogs_yaml_iter_recurse(&ngap_array, &ngap_iter);
                        } else if (ogs_yaml_iter_type(&ngap_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&ngap_iter)) {
                            const char *ngap_key =
                                ogs_yaml_iter_key(&ngap_iter);
                            ogs_assert(ngap_key);
                            if (!strcmp(ngap_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&ngap_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ",
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(ngap_key, "addr") ||
                                    !strcmp(ngap_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(
                                        &ngap_iter, &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num <= OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] =
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(ngap_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&ngap_iter);
                                if (v) port = atoi(v);
                            } else if (!strcmp(ngap_key, "dev")) {
                                dev = ogs_yaml_iter_value(&ngap_iter);
                            } else if (!strcmp(ngap_key, "option")) {
                                rv = ogs_app_config_parse_sockopt(
                                        &ngap_iter, &option);
                                if (rv != OGS_OK) return rv;
                                is_option = true;
                            } else
                                ogs_warn("unknown key `%s`", ngap_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        if (addr) {
                            if (ogs_app()->parameter.no_ipv4 == 0)
                                ogs_socknode_add(
                                    &self.ngap_list, AF_INET, addr,
                                    is_option ? &option : NULL);
                            if (ogs_app()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(
                                    &self.ngap_list6, AF_INET6, addr,
                                    is_option ? &option : NULL);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                    ogs_app()->parameter.no_ipv4 ?
                                        NULL : &self.ngap_list,
                                    ogs_app()->parameter.no_ipv6 ?
                                        NULL : &self.ngap_list6,
                                    dev, port,
                                    is_option ? &option : NULL);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while (ogs_yaml_iter_type(&ngap_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.ngap_list) == NULL &&
                        ogs_list_first(&self.ngap_list6) == NULL) {
                        rv = ogs_socknode_probe(
                                ogs_app()->parameter.no_ipv4 ?
                                    NULL : &self.ngap_list,
                                ogs_app()->parameter.no_ipv6 ?
                                    NULL : &self.ngap_list6,
                                NULL, self.ngap_port, NULL);
                        ogs_assert(rv == OGS_OK);
                    }
                } if (!strcmp(amf_key, "tai")) {
                    int num_of_list0 = 0;
                    ogs_5gs_tai0_list_t *list0 = NULL;
                    ogs_5gs_tai2_list_t *list2 = NULL;

                    ogs_assert(self.num_of_nr_served_tai <=
                            OGS_MAX_NUM_OF_SERVED_TAI);
                    list0 =
                        &self.nr_served_tai[self.num_of_nr_served_tai].list0;
                    ogs_assert(list0);
                    list2 =
                        &self.nr_served_tai[self.num_of_nr_served_tai].list2;
                    ogs_assert(list2);

                    ogs_yaml_iter_t tai_array, tai_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &tai_array);
                    do {
                        const char *mcc = NULL, *mnc = NULL;
                        ogs_uint24_t tac[OGS_MAX_NUM_OF_TAI];
                        int num_of_tac = 0;

                        if (ogs_yaml_iter_type(&tai_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&tai_iter, &tai_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&tai_array))
                                break;
                            ogs_yaml_iter_recurse(&tai_array,
                                    &tai_iter);
                        } else if (ogs_yaml_iter_type(&tai_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&tai_iter)) {
                            const char *tai_key = ogs_yaml_iter_key(&tai_iter);
                            ogs_assert(tai_key);
                            if (!strcmp(tai_key, "plmn_id")) {
                                ogs_yaml_iter_t plmn_id_iter;

                                ogs_yaml_iter_recurse(&tai_iter, &plmn_id_iter);
                                while (ogs_yaml_iter_next(&plmn_id_iter)) {
                                    const char *plmn_id_key =
                                        ogs_yaml_iter_key(&plmn_id_iter);
                                    ogs_assert(plmn_id_key);
                                    if (!strcmp(plmn_id_key, "mcc")) {
                                        mcc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    } else if (!strcmp(plmn_id_key, "mnc")) {
                                        mnc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    }
                                }
                            } else if (!strcmp(tai_key, "tac")) {
                                ogs_yaml_iter_t tac_iter;
                                ogs_yaml_iter_recurse(&tai_iter, &tac_iter);
                                ogs_assert(ogs_yaml_iter_type(&tac_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    const char *v = NULL;

                                    ogs_assert(num_of_tac <=
                                            OGS_MAX_NUM_OF_TAI);
                                    if (ogs_yaml_iter_type(&tac_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&tac_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&tac_iter);
                                    if (v) {
                                        tac[num_of_tac].v = atoi(v);
                                        num_of_tac++;
                                    }
                                } while (
                                    ogs_yaml_iter_type(&tac_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", tai_key);
                        }

                        if (mcc && mnc && num_of_tac) {
                            if (num_of_tac == 1) {
                                ogs_plmn_id_build(
                                    &list2->tai[list2->num].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                list2->tai[list2->num].tac.v = tac[0].v;

                                list2->num++;
                                if (list2->num > 1)
                                    list2->type = OGS_TAI2_TYPE;
                                else
                                    list2->type = OGS_TAI1_TYPE;
                            } else if (num_of_tac > 1) {
                                int i;
                                ogs_plmn_id_build(
                                    &list0->tai[num_of_list0].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                for (i = 0; i < num_of_tac; i++) {
                                    list0->tai[num_of_list0].tac[i].v =
                                        tac[i].v;
                                }

                                list0->tai[num_of_list0].num = num_of_tac;
                                list0->tai[num_of_list0].type = OGS_TAI0_TYPE;

                                num_of_list0++;
                            }
                        } else {
                            ogs_warn("Ignore tai : mcc(%p), mnc(%p), "
                                    "num_of_tac(%d)", mcc, mnc, num_of_tac);
                        }
                    } while (ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE);

                    if (list2->num || num_of_list0) {
                        self.num_of_nr_served_tai++;
                    }
                } else if (!strcmp(amf_key, "plmn_support")) {
                    ogs_yaml_iter_t plmn_support_array, plmn_support_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &plmn_support_array);
                    do {
                        const char *mnc = NULL, *mcc = NULL;
                        ogs_assert(self.num_of_plmn_support <=
                                OGS_MAX_NUM_OF_PLMN);

                        if (ogs_yaml_iter_type(&plmn_support_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&plmn_support_iter, &plmn_support_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&plmn_support_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&plmn_support_array))
                                break;
                            ogs_yaml_iter_recurse(&plmn_support_array,
                                    &plmn_support_iter);
                        } else if (ogs_yaml_iter_type(&plmn_support_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&plmn_support_iter)) {
                            const char *plmn_support_key =
                                ogs_yaml_iter_key(&plmn_support_iter);
                            ogs_assert(plmn_support_key);
                            if (!strcmp(plmn_support_key, "plmn_id")) {
                                ogs_yaml_iter_t plmn_id_iter;

                                ogs_yaml_iter_recurse(&plmn_support_iter,
                                        &plmn_id_iter);
                                while (ogs_yaml_iter_next(&plmn_id_iter)) {
                                    const char *plmn_id_key =
                                        ogs_yaml_iter_key(&plmn_id_iter);
                                    ogs_assert(plmn_id_key);
                                    if (!strcmp(plmn_id_key, "mcc")) {
                                        mcc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    } else if (!strcmp(plmn_id_key, "mnc")) {
                                        mnc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    }
                                }

                                if (mcc && mnc) {
                                    ogs_plmn_id_build(
                                        &self.plmn_support[
                                            self.num_of_plmn_support].
                                                plmn_id,
                                        atoi(mcc), atoi(mnc), strlen(mnc));
                                }
                            } else if (!strcmp(plmn_support_key, "s_nssai")) {
                                ogs_yaml_iter_t s_nssai_array, s_nssai_iter;
                                ogs_yaml_iter_recurse(&plmn_support_iter,
                                        &s_nssai_array);
                                do {
                                    ogs_s_nssai_t *s_nssai = NULL;
                                    const char *sst = NULL, *sd = NULL;
                                    ogs_assert(
                                        self.plmn_support[
                                            self.num_of_plmn_support].
                                                num_of_s_nssai <=
                                            OGS_MAX_NUM_OF_SLICE);
                                    s_nssai = &self.plmn_support[
                                            self.num_of_plmn_support].s_nssai[
                                                self.plmn_support[
                                                    self.num_of_plmn_support].
                                                        num_of_s_nssai];
                                    ogs_assert(s_nssai);

                                    if (ogs_yaml_iter_type(&s_nssai_array) ==
                                            YAML_MAPPING_NODE) {
                                        memcpy(&s_nssai_iter, &s_nssai_array,
                                                sizeof(ogs_yaml_iter_t));
                                    } else if (ogs_yaml_iter_type(
                                                &s_nssai_array) ==
                                                YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&s_nssai_array))
                                            break;
                                        ogs_yaml_iter_recurse(&s_nssai_array,
                                                &s_nssai_iter);
                                    } else if (ogs_yaml_iter_type(
                                                &s_nssai_array) ==
                                                YAML_SCALAR_NODE) {
                                        break;
                                    } else
                                        ogs_assert_if_reached();

                                    while (ogs_yaml_iter_next(&s_nssai_iter)) {
                                        const char *s_nssai_key =
                                            ogs_yaml_iter_key(&s_nssai_iter);
                                        ogs_assert(s_nssai_key);
                                        if (!strcmp(s_nssai_key, "sst")) {
                                            sst = ogs_yaml_iter_value(
                                                    &s_nssai_iter);
                                        } else if (!strcmp(
                                                    s_nssai_key, "sd")) {
                                            sd = ogs_yaml_iter_value(
                                                    &s_nssai_iter);
                                        }
                                    }

                                    if (sst) {
                                        s_nssai->sst = atoi(sst);
                                        if (sd)
                                            s_nssai->sd =
                                                ogs_uint24_from_string(
                                                        (char*)sd);
                                        else
                                            s_nssai->sd.v =
                                                OGS_S_NSSAI_NO_SD_VALUE;

                                        self.plmn_support[
                                            self.num_of_plmn_support].
                                                num_of_s_nssai++;
                                    }

                                } while (ogs_yaml_iter_type(&s_nssai_array) ==
                                        YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", plmn_support_key);
                        }

                        if (self.plmn_support[
                                self.num_of_plmn_support].num_of_s_nssai &&
                                mcc && mnc) {
                            self.num_of_plmn_support++;
                        } else {
                            ogs_warn("Ignore plmn : "
                                    "s_nssai(%d) mcc(%s), mnc(%s)",
                                    self.plmn_support[
                                    self.num_of_plmn_support].num_of_s_nssai,
                                    mcc, mnc);
                            self.plmn_support[
                                self.num_of_plmn_support].num_of_s_nssai = 0;
                        }
                    } while (ogs_yaml_iter_type(&plmn_support_array) ==
                            YAML_SEQUENCE_NODE);
                } else if (!strcmp(amf_key, "sbi")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", amf_key);
            }
        } else if (!strcmp(root_key, "mme")) {
            ogs_yaml_iter_t mme_iter;
            ogs_yaml_iter_recurse(&root_iter, &mme_iter);
            while (ogs_yaml_iter_next(&mme_iter)) {
                const char *mme_key = ogs_yaml_iter_key(&mme_iter);
                ogs_assert(mme_key);
                if (!strcmp(mme_key, "s1ap")) {
                    ogs_yaml_iter_t s1ap_array, s1ap_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &s1ap_array);
                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.s1ap_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;

                        ogs_sockopt_t option;
                        bool is_option = false;

                        if (ogs_yaml_iter_type(&s1ap_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&s1ap_iter, &s1ap_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&s1ap_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&s1ap_array))
                                break;
                            ogs_yaml_iter_recurse(&s1ap_array, &s1ap_iter);
                        } else if (ogs_yaml_iter_type(&s1ap_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&s1ap_iter)) {
                            const char *s1ap_key =
                                ogs_yaml_iter_key(&s1ap_iter);
                            ogs_assert(s1ap_key);
                            if (!strcmp(s1ap_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&s1ap_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ",
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(s1ap_key, "addr") ||
                                    !strcmp(s1ap_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(
                                        &s1ap_iter, &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num <= OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] =
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(s1ap_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&s1ap_iter);
                                if (v) port = atoi(v);
                            } else if (!strcmp(s1ap_key, "dev")) {
                                dev = ogs_yaml_iter_value(&s1ap_iter);
                            } else if (!strcmp(s1ap_key, "option")) {
                                rv = ogs_app_config_parse_sockopt(
                                        &s1ap_iter, &option);
                                if (rv != OGS_OK) return rv;
                                is_option = true;
                            } else
                                ogs_warn("unknown key `%s`", s1ap_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        if (addr) {
                            if (ogs_app()->parameter.no_ipv4 == 0)
                                ogs_socknode_add(
                                    &self.s1ap_list, AF_INET, addr,
                                    is_option ? &option : NULL);
                            if (ogs_app()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(
                                    &self.s1ap_list6, AF_INET6, addr,
                                    is_option ? &option : NULL);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                    ogs_app()->parameter.no_ipv4 ?
                                        NULL : &self.s1ap_list,
                                    ogs_app()->parameter.no_ipv6 ?
                                        NULL : &self.s1ap_list6,
                                    dev, port,
                                    is_option ? &option : NULL);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while (ogs_yaml_iter_type(&s1ap_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.s1ap_list) == NULL &&
                        ogs_list_first(&self.s1ap_list6) == NULL) {
                        rv = ogs_socknode_probe(
                                ogs_app()->parameter.no_ipv4 ?
                                    NULL : &self.s1ap_list,
                                ogs_app()->parameter.no_ipv6 ?
                                    NULL : &self.s1ap_list6,
                                NULL, self.s1ap_port, NULL);
                        ogs_assert(rv == OGS_OK);
                    }
                } else if (!strcmp(mme_key, "tai")) {
                    int num_of_list0 = 0;
                    ogs_eps_tai0_list_t *list0 = NULL;
                    ogs_eps_tai2_list_t *list2 = NULL;

                    ogs_assert(self.num_of_e_served_tai <=
                            OGS_MAX_NUM_OF_SERVED_TAI);
                    list0 = &self.e_served_tai[self.num_of_e_served_tai].list0;
                    ogs_assert(list0);
                    list2 = &self.e_served_tai[self.num_of_e_served_tai].list2;
                    ogs_assert(list2);

                    ogs_yaml_iter_t tai_array, tai_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &tai_array);
                    do {
                        const char *mcc = NULL, *mnc = NULL;
                        uint16_t tac[OGS_MAX_NUM_OF_TAI];
                        int num_of_tac = 0;

                        if (ogs_yaml_iter_type(&tai_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&tai_iter, &tai_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&tai_array))
                                break;
                            ogs_yaml_iter_recurse(&tai_array,
                                    &tai_iter);
                        } else if (ogs_yaml_iter_type(&tai_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&tai_iter)) {
                            const char *tai_key = ogs_yaml_iter_key(&tai_iter);
                            ogs_assert(tai_key);
                            if (!strcmp(tai_key, "plmn_id")) {
                                ogs_yaml_iter_t plmn_id_iter;

                                ogs_yaml_iter_recurse(&tai_iter, &plmn_id_iter);
                                while (ogs_yaml_iter_next(&plmn_id_iter)) {
                                    const char *plmn_id_key =
                                        ogs_yaml_iter_key(&plmn_id_iter);
                                    ogs_assert(plmn_id_key);
                                    if (!strcmp(plmn_id_key, "mcc")) {
                                        mcc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    } else if (!strcmp(plmn_id_key, "mnc")) {
                                        mnc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    }
                                }
                            } else if (!strcmp(tai_key, "tac")) {
                                ogs_yaml_iter_t tac_iter;
                                ogs_yaml_iter_recurse(&tai_iter, &tac_iter);
                                ogs_assert(ogs_yaml_iter_type(&tac_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    const char *v = NULL;

                                    ogs_assert(num_of_tac <=
                                            OGS_MAX_NUM_OF_TAI);
                                    if (ogs_yaml_iter_type(&tac_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&tac_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&tac_iter);
                                    if (v) {
                                        tac[num_of_tac] = atoi(v);
                                        num_of_tac++;
                                    }
                                } while (
                                    ogs_yaml_iter_type(&tac_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", tai_key);
                        }

                        if (mcc && mnc && num_of_tac) {
                            if (num_of_tac == 1) {
                                ogs_plmn_id_build(
                                    &list2->tai[list2->num].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                list2->tai[list2->num].tac = tac[0];

                                list2->num++;
                                if (list2->num > 1)
                                    list2->type = OGS_TAI2_TYPE;
                                else
                                    list2->type = OGS_TAI1_TYPE;
                            } else if (num_of_tac > 1) {
                                int i;
                                ogs_plmn_id_build(
                                    &list0->tai[num_of_list0].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                for (i = 0; i < num_of_tac; i++) {
                                    list0->tai[num_of_list0].tac[i] = tac[i];
                                }

                                list0->tai[num_of_list0].num = num_of_tac;
                                list0->tai[num_of_list0].type = OGS_TAI0_TYPE;

                                num_of_list0++;
                            }
                        } else {
                            ogs_warn("Ignore tai : mcc(%p), mnc(%p), "
                                    "num_of_tac(%d)", mcc, mnc, num_of_tac);
                        }
                    } while (ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE);

                    if (list2->num || num_of_list0) {
                        self.num_of_e_served_tai++;
                    }
                }
            }
        }
    }

    rv = test_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

void test_ue_set_mobile_identity_suci(test_ue_t *test_ue,
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci,
    uint16_t mobile_identity_suci_length)
{
    ogs_nas_5gs_mobile_identity_t mobile_identity;

    ogs_assert(test_ue);
    ogs_assert(mobile_identity_suci);
    ogs_assert(mobile_identity_suci_length);

    test_ue->mobile_identity_suci_length = mobile_identity_suci_length;
    memcpy(&test_ue->mobile_identity_suci, mobile_identity_suci,
            mobile_identity_suci_length);

    mobile_identity.length = test_ue->mobile_identity_suci_length;
    mobile_identity.buffer = &test_ue->mobile_identity_suci;

    if (test_ue->suci)
        ogs_free(test_ue->suci);
    test_ue->suci = ogs_nas_5gs_suci_from_mobile_identity(&mobile_identity);
    if (test_ue->supi)
        ogs_free(test_ue->supi);
    test_ue->supi = ogs_supi_from_suci(test_ue->suci);
    ogs_assert(test_ue->supi);
    if (test_ue->imsi)
        ogs_free(test_ue->imsi);
    test_ue->imsi = ogs_id_get_value(test_ue->supi);
    ogs_assert(test_ue->imsi);

    ogs_bcd_to_buffer(test_ue->imsi, test_ue->imsi_buf, &test_ue->imsi_len);
}

static void test_ue_set_mobile_identity_imsi(test_ue_t *test_ue)
{
    ogs_assert(test_ue);
    ogs_assert(test_ue->imsi);

    test_ue->mobile_identity_imsi.odd_even = OGS_NAS_MOBILE_IDENTITY_ODD;
    test_ue->mobile_identity_imsi.type = OGS_NAS_MOBILE_IDENTITY_IMSI;
    test_ue->mobile_identity_imsi.digit1 = test_ue->imsi[0] - '0';
    test_ue->mobile_identity_imsi.digit2 = test_ue->imsi[1] - '0';
    test_ue->mobile_identity_imsi.digit3 = test_ue->imsi[2] - '0';
    test_ue->mobile_identity_imsi.digit4 = test_ue->imsi[3] - '0';
    test_ue->mobile_identity_imsi.digit5 = test_ue->imsi[4] - '0';
    test_ue->mobile_identity_imsi.digit6 = test_ue->imsi[5] - '0';
    test_ue->mobile_identity_imsi.digit7 = test_ue->imsi[6] - '0';
    test_ue->mobile_identity_imsi.digit8 = test_ue->imsi[7] - '0';
    test_ue->mobile_identity_imsi.digit9 = test_ue->imsi[8] - '0';
    test_ue->mobile_identity_imsi.digit10 = test_ue->imsi[9] - '0';
    test_ue->mobile_identity_imsi.digit11 = test_ue->imsi[10] - '0';
    test_ue->mobile_identity_imsi.digit12 = test_ue->imsi[11] - '0';
    test_ue->mobile_identity_imsi.digit13 = test_ue->imsi[12] - '0';
    test_ue->mobile_identity_imsi.digit14 = test_ue->imsi[13] - '0';
    test_ue->mobile_identity_imsi.digit15 = test_ue->imsi[14] - '0';
}

static void test_ue_set_mobile_identity_imsisv(test_ue_t *test_ue)
{
    ogs_assert(test_ue);

    memset(&test_ue->mobile_identity_imeisv, 0,
            sizeof(ogs_nas_mobile_identity_imeisv_t));
    test_ue->mobile_identity_imeisv.type = OGS_NAS_5GS_MOBILE_IDENTITY_IMEISV;
    test_ue->mobile_identity_imeisv.odd_even = OGS_NAS_MOBILE_IDENTITY_EVEN;
    test_ue->mobile_identity_imeisv.digit1 = 8;
    test_ue->mobile_identity_imeisv.digit2 = 6;
    test_ue->mobile_identity_imeisv.digit3 = 6;
    test_ue->mobile_identity_imeisv.digit4 = 5;
    test_ue->mobile_identity_imeisv.digit5 = 0;
    test_ue->mobile_identity_imeisv.digit6 = 7;
    test_ue->mobile_identity_imeisv.digit7 = 0;
    test_ue->mobile_identity_imeisv.digit8 = 4;
    test_ue->mobile_identity_imeisv.digit9 = 0;
    test_ue->mobile_identity_imeisv.digit10 = 0;
    test_ue->mobile_identity_imeisv.digit11 = 4;
    test_ue->mobile_identity_imeisv.digit12 = 0;
    test_ue->mobile_identity_imeisv.digit13 = 5;
    test_ue->mobile_identity_imeisv.digit14 = 3;
    test_ue->mobile_identity_imeisv.digit15 = 0;
    test_ue->mobile_identity_imeisv.digit16 = 1;
    test_ue->mobile_identity_imeisv.digit17 = 0xf;
}

test_ue_t *test_ue_add_by_suci(
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci,
    uint16_t mobile_identity_suci_length)
{
    int i, j;
    test_ue_t *test_ue = NULL;

    ogs_assert(mobile_identity_suci);
    ogs_assert(mobile_identity_suci_length);

    ogs_pool_alloc(&test_ue_pool, &test_ue);
    ogs_assert(test_ue);
    memset(test_ue, 0, sizeof *test_ue);

    test_ue->nas.access_type = OGS_ACCESS_TYPE_3GPP;
    test_ue->abba_len = 2;

    test_ue->ue_security_capability.nr_ea = 0xf0;
    test_ue->ue_security_capability.nr_ia = 0xf0;
#if 0
    test_ue->ue_security_capability.eutra_ea = 0xf0;
    test_ue->ue_security_capability.eutra_ia = 0xf0;
#endif
    test_ue->ue_network_capability.eea = 0xf0;
    test_ue->ue_network_capability.eia = 0xf0;

    memcpy(&test_ue->e_tai, &test_self()->e_tai, sizeof(ogs_eps_tai_t));
    memcpy(&test_ue->e_cgi.plmn_id, &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN);

    memcpy(&test_ue->nr_tai, &test_self()->nr_tai, sizeof(ogs_5gs_tai_t));
    memcpy(&test_ue->nr_cgi.plmn_id, &test_ue->nr_tai.plmn_id, OGS_PLMN_ID_LEN);

    if (test_self()->nr_tai.tac.v)
        ogs_nas_from_plmn_id(
            &mobile_identity_suci->nas_plmn_id, &test_ue->nr_tai.plmn_id);
    else
        ogs_nas_from_plmn_id(
            &mobile_identity_suci->nas_plmn_id, &test_ue->e_tai.plmn_id);

    for (i = 0; i < test_self()->num_of_plmn_support; i++) {
        for (j = 0; j < test_self()->plmn_support[i].num_of_s_nssai; j++) {
            ogs_nas_s_nssai_ie_t *s_nssai = &test_ue->requested_nssai.
                    s_nssai[test_ue->requested_nssai.num_of_s_nssai];

            s_nssai->sst = 0;
            s_nssai->sd.v = OGS_S_NSSAI_NO_SD_VALUE;
            s_nssai->mapped_hplmn_sst = 0;
            s_nssai->mapped_hplmn_sd.v = OGS_S_NSSAI_NO_SD_VALUE;

            memcpy(s_nssai, &test_self()->plmn_support[i].s_nssai[j],
                    sizeof(ogs_s_nssai_t));

            test_ue->requested_nssai.num_of_s_nssai++;
        }
    }

    test_ue_set_mobile_identity_suci(
            test_ue, mobile_identity_suci, mobile_identity_suci_length);

    test_ue_set_mobile_identity_imsi(test_ue);
    test_ue_set_mobile_identity_imsisv(test_ue);

    ogs_list_add(&self.test_ue_list, test_ue);

    return test_ue;
}

void test_ue_remove(test_ue_t *test_ue)
{
    ogs_assert(test_ue);

    ogs_list_remove(&self.test_ue_list, test_ue);

    if (test_ue->suci)
        ogs_free(test_ue->suci);
    if (test_ue->supi)
        ogs_free(test_ue->supi);
    if (test_ue->imsi)
        ogs_free(test_ue->imsi);

    test_sess_remove_all(test_ue);

    ogs_pool_free(&test_ue_pool, test_ue);
}

void test_ue_remove_all(void)
{
    test_ue_t *test_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.test_ue_list, next, test_ue)
        test_ue_remove(test_ue);
}

test_sess_t *test_sess_add_by_apn(
        test_ue_t *test_ue, char *apn, uint8_t rat_type)
{
    test_sess_t *sess = NULL;

    ogs_assert(test_ue);
    ogs_assert(apn);

    ogs_pool_alloc(&test_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    sess->index = ogs_pool_index(&test_sess_pool, sess);

    sess->apn = ogs_strdup(apn);
    ogs_assert(sess->apn);
    sess->gtp_rat_type = rat_type;
    ogs_assert(sess->gtp_rat_type);

    sess->pti = 1; /* Default PTI : 1 */

    sess->epdg_s2b_c_teid = sess->index;

    sess->test_ue = test_ue;

    ogs_list_add(&test_ue->sess_list, sess);

    return sess;
}

test_sess_t *test_sess_add_by_dnn_and_psi(
        test_ue_t *test_ue, char *dnn, uint8_t psi)
{
    test_sess_t *sess = NULL;

    ogs_assert(test_ue);
    ogs_assert(dnn);

    ogs_pool_alloc(&test_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    sess->index = ogs_pool_index(&test_sess_pool, sess);

    sess->dnn = ogs_strdup(dnn);
    ogs_assert(sess->dnn);
    sess->psi = psi;
    sess->pti = 1; /* Default PTI : 1 */

    sess->gnb_n3_addr = test_self()->gnb1_addr;
    sess->gnb_n3_addr6 = test_self()->gnb1_addr6;
    sess->gnb_n3_teid = sess->index;

    sess->pdu_session_type = OGS_PDU_SESSION_TYPE_IPV4V6;

    sess->test_ue = test_ue;

    ogs_list_add(&test_ue->sess_list, sess);

    return sess;
}

void test_sess_remove(test_sess_t *sess)
{
    test_ue_t *test_ue = NULL;

    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    ogs_list_remove(&test_ue->sess_list, sess);

    if (sess->dnn)
        ogs_free(sess->dnn);

    test_bearer_remove_all(sess);

    ogs_pool_free(&test_sess_pool, sess);
}

void test_sess_remove_all(test_ue_t *test_ue)
{
    test_sess_t *sess = NULL, *next = NULL;;

    ogs_assert(test_ue);

    ogs_list_for_each_safe(&test_ue->sess_list, next, sess)
        test_sess_remove(sess);
}

test_sess_t *test_sess_find_by_apn(
        test_ue_t *test_ue, char *apn, uint8_t rat_type)
{
    test_sess_t *sess = NULL;

    ogs_assert(test_ue);
    ogs_assert(apn);

    ogs_list_for_each(&test_ue->sess_list, sess)
        if (ogs_strcasecmp(sess->apn, apn) == 0 &&
            sess->gtp_rat_type == rat_type)
            return sess;

    return NULL;
}

test_sess_t *test_sess_find_by_psi(test_ue_t *test_ue, uint8_t psi)
{
    test_sess_t *sess = NULL;

    ogs_assert(test_ue);

    ogs_list_for_each(&test_ue->sess_list, sess)
        if (sess->psi == psi) return sess;

    return NULL;
}

test_bearer_t *test_bearer_add(test_sess_t *sess, uint8_t ebi)
{
    test_bearer_t *bearer = NULL;

    ogs_assert(sess);
    ogs_assert(ebi);

    ogs_pool_alloc(&test_bearer_pool, &bearer);
    ogs_assert(bearer);
    memset(bearer, 0, sizeof *bearer);

    bearer->index = ogs_pool_index(&test_bearer_pool, bearer);

    bearer->ebi = ebi;

    bearer->enb_s1u_addr = test_self()->gnb1_addr;
    bearer->enb_s1u_addr6 = test_self()->gnb1_addr6;
    bearer->enb_s1u_teid = bearer->index;

    bearer->sess = sess;

    ogs_list_add(&sess->bearer_list, bearer);

    return bearer;
}

test_bearer_t *test_qos_flow_add(test_sess_t *sess)
{
    test_bearer_t *qos_flow = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&test_bearer_pool, &qos_flow);
    ogs_assert(qos_flow);
    memset(qos_flow, 0, sizeof *qos_flow);

    qos_flow->index = ogs_pool_index(&test_bearer_pool, qos_flow);

    qos_flow->sess = sess;

    ogs_list_add(&sess->bearer_list, qos_flow);

    return qos_flow;
}

void test_bearer_remove(test_bearer_t *bearer)
{
    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    ogs_list_remove(&bearer->sess->bearer_list, bearer);

    ogs_pool_free(&test_bearer_pool, bearer);
}

void test_bearer_remove_all(test_sess_t *sess)
{
    test_bearer_t *bearer = NULL, *next_bearer = NULL;

    ogs_assert(sess);
    ogs_list_for_each_safe(&sess->bearer_list, next_bearer, bearer)
        test_bearer_remove(bearer);
}

test_bearer_t *test_bearer_find_by_sess_ebi(test_sess_t *sess, uint8_t ebi)
{
    test_bearer_t *bearer = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->bearer_list, bearer)
        if (ebi == bearer->ebi) return bearer;

    return NULL;
}

test_bearer_t *test_bearer_find_by_ue_ebi(test_ue_t *test_ue, uint8_t ebi)
{
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    ogs_assert(test_ue);

    ogs_list_for_each(&test_ue->sess_list, sess) {
        if (sess->gtp_rat_type != OGS_GTP2_RAT_TYPE_EUTRAN)
            continue;

        bearer = test_bearer_find_by_sess_ebi(sess, ebi);
        if (bearer) return bearer;

    }

    return NULL;
}

test_bearer_t *test_qos_flow_find_by_qfi(test_sess_t *sess, uint8_t qfi)
{
    test_bearer_t *qos_flow = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->bearer_list, qos_flow)
        if (qfi == qos_flow->qfi) return qos_flow;

    return NULL;
}

int test_db_insert_ue(test_ue_t *test_ue, bson_t *doc)
{
    mongoc_collection_t *collection = NULL;
    bson_t *key = NULL;
    int64_t count = 0;
    bson_error_t error;

    ogs_assert(test_ue);
    ogs_assert(doc);

    OGS_HEX(test_ue->k_string, strlen(test_ue->k_string), test_ue->k);
    OGS_HEX(test_ue->opc_string, strlen(test_ue->opc_string), test_ue->opc);

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    if (!collection) {
        ogs_error("mongoc_client_get_collection() failed");
        return OGS_ERROR;
    }
    key = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ogs_assert(key);

    count = mongoc_collection_count (
        collection, MONGOC_QUERY_NONE, key, 0, 0, NULL, &error);
    if (count) {
        if (mongoc_collection_remove(collection,
                MONGOC_REMOVE_SINGLE_REMOVE, key, NULL, &error) != true) {
            ogs_error("mongoc_collection_remove() failed");
            bson_destroy(key);
            return OGS_ERROR;
        }
    }
    bson_destroy(key);

    if (mongoc_collection_insert(collection,
                MONGOC_INSERT_NONE, doc, NULL, &error) != true) {
        ogs_error("mongoc_collection_insert() failed");
        bson_destroy(doc);
        return OGS_ERROR;
    }
    bson_destroy(doc);

    key = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ogs_assert(key);
    do {
        count = mongoc_collection_count(
            collection, MONGOC_QUERY_NONE, key, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(key);

    mongoc_collection_destroy(collection);

    return OGS_OK;
}

int test_db_remove_ue(test_ue_t *test_ue)
{
    mongoc_collection_t *collection = NULL;
    bson_t *key = NULL;
    bson_error_t error;

    ogs_assert(test_ue);

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    if (!collection) {
        ogs_error("mongoc_client_get_collection() failed");
        return OGS_ERROR;
    }

    key = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ogs_assert(key);
    if (mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, key, NULL, &error) != true) {
        ogs_error("mongoc_collection_remove() failed");
        bson_destroy(key);
        return OGS_ERROR;
    }
    bson_destroy(key);

    mongoc_collection_destroy(collection);

    return OGS_OK;
}

bson_t *test_db_new_simple(test_ue_t *test_ue)
{
    bson_t *doc = NULL;

    ogs_assert(test_ue);

    doc = BCON_NEW(
            "imsi", BCON_UTF8(test_ue->imsi),
            "msisdn", "[",
                BCON_UTF8(TEST_MSISDN),
                BCON_UTF8(TEST_ADDITIONAL_MSISDN),
            "]",
            "ambr", "{",
                "downlink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
                "uplink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
            "}",
            "slice", "[", "{",
                "sst", BCON_INT32(1),
                "default_indicator", BCON_BOOL(true),
                "session", "[", "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                "}", "]",
            "}", "]",
            "security", "{",
                "k", BCON_UTF8(test_ue->k_string),
                "opc", BCON_UTF8(test_ue->opc_string),
                "amf", BCON_UTF8("8000"),
                "sqn", BCON_INT64(64),
            "}",
            "subscribed_rau_tau_timer", BCON_INT32(12),
            "network_access_mode", BCON_INT32(0),
            "subscriber_status", BCON_INT32(0),
            "access_restriction_data", BCON_INT32(32)
          );
    ogs_assert(doc);

    return doc;
}

bson_t *test_db_new_qos_flow(test_ue_t *test_ue)
{
    bson_t *doc = NULL;

    ogs_assert(test_ue);

    doc = BCON_NEW(
            "imsi", BCON_UTF8(test_ue->imsi),
            "msisdn", "[",
                BCON_UTF8(TEST_MSISDN),
                BCON_UTF8(TEST_ADDITIONAL_MSISDN),
            "]",
            "ambr", "{",
                "downlink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
                "uplink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
            "}",
            "slice", "[", "{",
                "sst", BCON_INT32(1),
                "default_indicator", BCON_BOOL(true),
                "session", "[", "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[", "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(2),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(64),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(44),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(64),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(44),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                        "flow", "[",
                            "{", "direction", BCON_INT32(2),
                                 "description", BCON_UTF8("permit out icmp from any to assigned"), "}",
                            "{", "direction", BCON_INT32(1),
                                 "description", BCON_UTF8("permit out icmp from any to assigned"), "}",
                            "{", "direction", BCON_INT32(2),
                                 "description", BCON_UTF8("permit out udp from 10.200.136.98/32 23455 to assigned 1-65535"), "}",
                            "{", "direction", BCON_INT32(1),
                                 "description", BCON_UTF8("permit out udp from 10.200.136.98/32 1-65535 to assigned 50021"), "}",
                        "]",
                    "}", "]",
                "}", "]",
            "}", "]",
            "security", "{",
                "k", BCON_UTF8(test_ue->k_string),
                "opc", BCON_UTF8(test_ue->opc_string),
                "amf", BCON_UTF8("8000"),
                "sqn", BCON_INT64(64),
            "}",
            "subscribed_rau_tau_timer", BCON_INT32(12),
            "network_access_mode", BCON_INT32(0),
            "subscriber_status", BCON_INT32(0),
            "access_restriction_data", BCON_INT32(32)
          );
    ogs_assert(doc);

    return doc;
}

bson_t *test_db_new_session(test_ue_t *test_ue)
{
    bson_t *doc = NULL;

    ogs_assert(test_ue);

    doc = BCON_NEW(
            "imsi", BCON_UTF8(test_ue->imsi),
            "msisdn", "[",
                BCON_UTF8(TEST_MSISDN),
                BCON_UTF8(TEST_ADDITIONAL_MSISDN),
            "]",
            "ambr", "{",
                "downlink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
                "uplink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
            "}",
            "slice", "[", "{",
                "sst", BCON_INT32(1),
                "default_indicator", BCON_BOOL(true),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("ims"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(6),
                        "arp", "{",
                            "priority_level", BCON_INT32(6),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[", "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(64),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(44),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(64),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(44),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                        "flow", "[",
                            "{", "direction", BCON_INT32(2),
                                 "description", BCON_UTF8("permit out icmp from any to assigned"), "}",
                            "{", "direction", BCON_INT32(1),
                                 "description", BCON_UTF8("permit out icmp from any to assigned"), "}",
                            "{", "direction", BCON_INT32(2),
                                 "description", BCON_UTF8("permit out udp from 10.200.136.98/32 23455 to assigned 1-65535"), "}",
                            "{", "direction", BCON_INT32(1),
                                 "description", BCON_UTF8("permit out udp from 10.200.136.98/32 1-65535 to assigned 50021"), "}",
                        "]",
                    "}", "]",
                  "}",
                "]",
            "}", "]",
            "security", "{",
                "k", BCON_UTF8(test_ue->k_string),
                "opc", BCON_UTF8(test_ue->opc_string),
                "amf", BCON_UTF8("8000"),
                "sqn", BCON_INT64(64),
            "}",
            "subscribed_rau_tau_timer", BCON_INT32(12),
            "network_access_mode", BCON_INT32(0),
            "subscriber_status", BCON_INT32(0),
            "access_restriction_data", BCON_INT32(32)
          );
    ogs_assert(doc);

    return doc;
}

bson_t *test_db_new_ims(test_ue_t *test_ue)
{
    bson_t *doc = NULL;

    ogs_assert(test_ue);

    doc = BCON_NEW(
            "imsi", BCON_UTF8(test_ue->imsi),
            "msisdn", "[",
                BCON_UTF8(TEST_MSISDN),
                BCON_UTF8(TEST_ADDITIONAL_MSISDN),
            "]",
            "ambr", "{",
                "downlink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
                "uplink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
            "}",
            "slice", "[", "{",
                "sst", BCON_INT32(1),
                "default_indicator", BCON_BOOL(true),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("ims"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(5),
                        "arp", "{",
                            "priority_level", BCON_INT32(1),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(2),
                            "arp", "{",
                                "priority_level", BCON_INT32(4),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                    "]",
                  "}",
                "]",
            "}", "]",
            "security", "{",
                "k", BCON_UTF8(test_ue->k_string),
                "opc", BCON_UTF8(test_ue->opc_string),
                "amf", BCON_UTF8("8000"),
                "sqn", BCON_INT64(64),
            "}",
            "subscribed_rau_tau_timer", BCON_INT32(12),
            "network_access_mode", BCON_INT32(0),
            "subscriber_status", BCON_INT32(0),
            "access_restriction_data", BCON_INT32(32)
          );
    ogs_assert(doc);

    return doc;
}

bson_t *test_db_new_slice_with_same_dnn(test_ue_t *test_ue)
{
    bson_t *doc = NULL;

    ogs_assert(test_ue);

    doc = BCON_NEW(
            "imsi", BCON_UTF8(test_ue->imsi),
            "msisdn", "[",
                BCON_UTF8(TEST_MSISDN),
                BCON_UTF8(TEST_ADDITIONAL_MSISDN),
            "]",
            "ambr", "{",
                "downlink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
                "uplink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
            "}",
            "slice", "[",
              "{",
                "sst", BCON_INT32(1),
                "default_indicator", BCON_BOOL(true),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("ims"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(5),
                        "arp", "{",
                            "priority_level", BCON_INT32(1),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(2),
                            "arp", "{",
                                "priority_level", BCON_INT32(4),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                    "]",
                  "}",
                "]",
              "}",
              "{",
                "sst", BCON_INT32(1),
                "sd", BCON_UTF8("000080"),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("ims"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(5),
                        "arp", "{",
                            "priority_level", BCON_INT32(1),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(2),
                            "arp", "{",
                                "priority_level", BCON_INT32(4),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                    "]",
                  "}",
                "]",
              "}",
              "{",
                "sst", BCON_INT32(1),
                "sd", BCON_UTF8("009000"),
                "default_indicator", BCON_BOOL(true),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("ims"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(5),
                        "arp", "{",
                            "priority_level", BCON_INT32(1),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(2),
                            "arp", "{",
                                "priority_level", BCON_INT32(4),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                    "]",
                  "}",
                "]",
              "}",
            "]",
            "security", "{",
                "k", BCON_UTF8(test_ue->k_string),
                "opc", BCON_UTF8(test_ue->opc_string),
                "amf", BCON_UTF8("8000"),
                "sqn", BCON_INT64(64),
            "}",
            "subscribed_rau_tau_timer", BCON_INT32(12),
            "network_access_mode", BCON_INT32(0),
            "subscriber_status", BCON_INT32(0),
            "access_restriction_data", BCON_INT32(32)
          );
    ogs_assert(doc);

    return doc;
}

bson_t *test_db_new_slice_with_different_dnn(test_ue_t *test_ue)
{
    bson_t *doc = NULL;

    ogs_assert(test_ue);

    doc = BCON_NEW(
            "imsi", BCON_UTF8(test_ue->imsi),
            "msisdn", "[",
                BCON_UTF8(TEST_MSISDN),
                BCON_UTF8(TEST_ADDITIONAL_MSISDN),
            "]",
            "ambr", "{",
                "downlink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
                "uplink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
            "}",
            "slice", "[",
              "{",
                "sst", BCON_INT32(1),
                "default_indicator", BCON_BOOL(true),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("ims"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(5),
                        "arp", "{",
                            "priority_level", BCON_INT32(1),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(2),
                            "arp", "{",
                                "priority_level", BCON_INT32(4),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                    "]",
                  "}",
                "]",
              "}",
              "{",
                "sst", BCON_INT32(1),
                "sd", BCON_UTF8("000080"),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet2"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("ims2"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(5),
                        "arp", "{",
                            "priority_level", BCON_INT32(1),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(2),
                            "arp", "{",
                                "priority_level", BCON_INT32(4),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                    "]",
                  "}",
                "]",
              "}",
              "{",
                "sst", BCON_INT32(1),
                "sd", BCON_UTF8("009000"),
                "default_indicator", BCON_BOOL(true),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet3"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("ims3"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(5),
                        "arp", "{",
                            "priority_level", BCON_INT32(1),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(2),
                            "arp", "{",
                                "priority_level", BCON_INT32(4),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                    "]",
                  "}",
                "]",
              "}",
            "]",
            "security", "{",
                "k", BCON_UTF8(test_ue->k_string),
                "opc", BCON_UTF8(test_ue->opc_string),
                "amf", BCON_UTF8("8000"),
                "sqn", BCON_INT64(64),
            "}",
            "subscribed_rau_tau_timer", BCON_INT32(12),
            "network_access_mode", BCON_INT32(0),
            "subscriber_status", BCON_INT32(0),
            "access_restriction_data", BCON_INT32(32)
          );
    ogs_assert(doc);

    return doc;
}

bson_t *test_db_new_non3gpp(test_ue_t *test_ue)
{
    bson_t *doc = NULL;

    ogs_assert(test_ue);

    doc = BCON_NEW(
            "imsi", BCON_UTF8(test_ue->imsi),
            "msisdn", "[",
                BCON_UTF8(TEST_MSISDN),
                BCON_UTF8(TEST_ADDITIONAL_MSISDN),
            "]",
            "ambr", "{",
                "downlink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
                "uplink", "{",
                    "value", BCON_INT32(1),
                    "unit", BCON_INT32(3),
                "}",
            "}",
            "slice", "[", "{",
                "sst", BCON_INT32(1),
                "default_indicator", BCON_BOOL(true),
                "session", "[",
                  "{",
                    "name", BCON_UTF8("internet"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(9),
                        "arp", "{",
                            "priority_level", BCON_INT32(8),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "smf", "{",
                        "addr", BCON_UTF8("127.0.0.4"),
                        "addr6", BCON_UTF8("::1"),
                    "}",
                  "}",
                  "{",
                    "name", BCON_UTF8("wlan"),
                    "type", BCON_INT32(3),
                    "ambr", "{",
                        "downlink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                        "uplink", "{",
                            "value", BCON_INT32(1),
                            "unit", BCON_INT32(3),
                        "}",
                    "}",
                    "qos", "{",
                        "index", BCON_INT32(5),
                        "arp", "{",
                            "priority_level", BCON_INT32(1),
                            "pre_emption_vulnerability", BCON_INT32(1),
                            "pre_emption_capability", BCON_INT32(1),
                        "}",
                    "}",
                    "pcc_rule", "[",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(1),
                            "arp", "{",
                                "priority_level", BCON_INT32(3),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(82),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                      "{",
                        "qos", "{",
                            "index", BCON_INT32(2),
                            "arp", "{",
                                "priority_level", BCON_INT32(4),
                                "pre_emption_vulnerability", BCON_INT32(2),
                                "pre_emption_capability", BCON_INT32(2),
                            "}",
                            "mbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                            "gbr", "{",
                                "downlink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                                "uplink", "{",
                                    "value", BCON_INT32(802),
                                    "unit", BCON_INT32(1),
                                "}",
                            "}",
                        "}",
                      "}",
                    "]",
                  "}",
                "]",
            "}", "]",
            "security", "{",
                "k", BCON_UTF8(test_ue->k_string),
                "opc", BCON_UTF8(test_ue->opc_string),
                "amf", BCON_UTF8("8000"),
                "sqn", BCON_INT64(64),
            "}",
            "subscribed_rau_tau_timer", BCON_INT32(12),
            "network_access_mode", BCON_INT32(0),
            "subscriber_status", BCON_INT32(0),
            "access_restriction_data", BCON_INT32(32)
          );
    ogs_assert(doc);

    return doc;
}
