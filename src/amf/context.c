/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "ngap-path.h"

static amf_context_t self;

int __amf_log_domain;
int __gmm_log_domain;

static OGS_POOL(amf_gnb_pool, amf_gnb_t);
static OGS_POOL(amf_ue_pool, amf_ue_t);
static OGS_POOL(ran_ue_pool, ran_ue_t);
static OGS_POOL(amf_sess_pool, amf_sess_t);

static OGS_POOL(m_tmsi_pool, amf_m_tmsi_t);

static int context_initialized = 0;

static int num_of_ran_ue = 0;
static int num_of_amf_sess = 0;

static void stats_add_ran_ue(void);
static void stats_remove_ran_ue(void);
static void stats_add_amf_session(void);
static void stats_remove_amf_session(void);
static bool amf_namf_comm_parse_guti(ogs_nas_5gs_guti_t *guti, char *ue_context_id);

void amf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize AMF context */
    memset(&self, 0, sizeof(amf_context_t));

    ogs_log_install_domain(&__ogs_sctp_domain, "sctp", ogs_core()->log.level);
    ogs_log_install_domain(&__ogs_ngap_domain, "ngap", ogs_core()->log.level);
    ogs_log_install_domain(&__ogs_nas_domain, "nas", ogs_core()->log.level);
    ogs_log_install_domain(&__amf_log_domain, "amf", ogs_core()->log.level);
    ogs_log_install_domain(&__gmm_log_domain, "gmm", ogs_core()->log.level);

    ogs_list_init(&self.ngap_list);
    ogs_list_init(&self.ngap_list6);

    /* Allocate TWICE the pool to check if maximum number of gNBs is reached */
    ogs_pool_init(&amf_gnb_pool, ogs_global_conf()->max.peer*2);
    ogs_pool_init(&amf_ue_pool, ogs_global_conf()->max.ue);
    ogs_pool_init(&ran_ue_pool, ogs_global_conf()->max.ue);
    ogs_pool_init(&amf_sess_pool, ogs_app()->pool.sess);
    /* Increase size of TMSI pool (#1827) */
    ogs_pool_init(&m_tmsi_pool, ogs_global_conf()->max.ue*2);
    ogs_pool_random_id_generate(&m_tmsi_pool);
#if 0 /* For debugging : Verify whether there are duplicates of M_TMSI. */
    ogs_pool_assert_if_has_duplicate(&m_tmsi_pool);
#endif

    ogs_list_init(&self.gnb_list);
    ogs_list_init(&self.amf_ue_list);

    self.gnb_addr_hash = ogs_hash_make();
    ogs_assert(self.gnb_addr_hash);
    self.gnb_id_hash = ogs_hash_make();
    ogs_assert(self.gnb_id_hash);
    self.guti_ue_hash = ogs_hash_make();
    ogs_assert(self.guti_ue_hash);
    self.suci_hash = ogs_hash_make();
    ogs_assert(self.suci_hash);
    self.supi_hash = ogs_hash_make();
    ogs_assert(self.supi_hash);

    context_initialized = 1;
}

void amf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    amf_gnb_remove_all();
    amf_ue_remove_all();

    ogs_assert(self.gnb_addr_hash);
    ogs_hash_destroy(self.gnb_addr_hash);
    ogs_assert(self.gnb_id_hash);
    ogs_hash_destroy(self.gnb_id_hash);

    ogs_assert(self.guti_ue_hash);
    ogs_hash_destroy(self.guti_ue_hash);
    ogs_assert(self.suci_hash);
    ogs_hash_destroy(self.suci_hash);
    ogs_assert(self.supi_hash);
    ogs_hash_destroy(self.supi_hash);

    ogs_pool_final(&m_tmsi_pool);
    ogs_pool_final(&amf_sess_pool);
    ogs_pool_final(&amf_ue_pool);
    ogs_pool_final(&ran_ue_pool);
    ogs_pool_final(&amf_gnb_pool);

    context_initialized = 0;
}

amf_context_t *amf_self(void)
{
    return &self;
}

static int amf_context_prepare(void)
{
    self.relative_capacity = 0xff;

    self.ngap_port = OGS_NGAP_SCTP_PORT;

    return OGS_OK;
}

static int amf_context_validation(void)
{
    ogs_nas_gprs_timer_t gprs_timer;

    if (ogs_list_first(&self.ngap_list) == NULL &&
        ogs_list_first(&self.ngap_list6) == NULL) {
        ogs_error("No amf.ngap.address in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    if (self.num_of_served_guami == 0) {
        ogs_error("No amf.guami in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    if (self.num_of_served_tai == 0) {
        ogs_error("No amf.tai in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    if (self.num_of_plmn_support == 0) {
        ogs_error("No amf.plmn_support in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    if (self.plmn_support[0].num_of_s_nssai == 0) {
        ogs_error("No amf.plmn_support.s_nssai in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    if (self.served_tai[0].list0.tai[0].num == 0 &&
        self.served_tai[0].list1.tai[0].num == 0 &&
        self.served_tai[0].list2.num == 0) {
        ogs_error("No amf.tai.plmn_id|tac in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    if (self.amf_name == NULL) {
        ogs_error("No amf.amf_name in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }

    if (self.num_of_integrity_order == 0) {
        ogs_error("No amf.security.integrity_order in '%s'",
                ogs_app()->file);
        return OGS_ERROR;
    }

    if (self.num_of_ciphering_order == 0) {
        ogs_error("No amf.security.ciphering_order in '%s'",
                ogs_app()->file);
        return OGS_ERROR;
    }
    if (ogs_nas_gprs_timer_from_sec(&gprs_timer, self.time.t3502.value) !=
        OGS_OK) {
        ogs_error("Not support GPRS Timer 2 [%d]", (int)self.time.t3502.value);
        return OGS_ERROR;
    }
    if (!self.time.t3512.value) {
        ogs_error("No amf.time.t3512.value in '%s'",
                ogs_app()->file);
        return OGS_ERROR;
    }
    if (ogs_nas_gprs_timer_3_from_sec(&gprs_timer, self.time.t3512.value) !=
        OGS_OK) {
        ogs_error("Not support GPRS Timer 3 [%d]", (int)self.time.t3512.value);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int amf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;
    int idx = 0;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = amf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if ((!strcmp(root_key, "amf")) &&
            (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t amf_iter;
            ogs_yaml_iter_recurse(&root_iter, &amf_iter);
            while (ogs_yaml_iter_next(&amf_iter)) {
                const char *amf_key = ogs_yaml_iter_key(&amf_iter);
                ogs_assert(amf_key);
                if (!strcmp(amf_key, "relative_capacity")) {
                    const char *v = ogs_yaml_iter_value(&amf_iter);
                    if (v) self.relative_capacity = atoi(v);
                } else if (!strcmp(amf_key, "ngap")) {
                    ogs_yaml_iter_t ngap_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &ngap_iter);
                    while (ogs_yaml_iter_next(&ngap_iter)) {
                        const char *ngap_key = ogs_yaml_iter_key(&ngap_iter);
                        ogs_assert(ngap_key);
                        if (!strcmp(ngap_key, "server")) {
                            ogs_yaml_iter_t server_iter, server_array;
                            ogs_yaml_iter_recurse(&ngap_iter, &server_array);
                            do {
                                int family = AF_UNSPEC;
                                int i, num = 0;
                                const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                                uint16_t port = self.ngap_port;
                                const char *dev = NULL;
                                ogs_sockaddr_t *addr = NULL;

                                ogs_sockopt_t option;
                                bool is_option = false;

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
                                    if (!strcmp(server_key, "family")) {
                                        const char *v =
                                            ogs_yaml_iter_value(&server_iter);
                                        if (v) family = atoi(v);
                                        if (family != AF_UNSPEC &&
                                            family != AF_INET &&
                                            family != AF_INET6) {
                                            ogs_warn("Ignore family(%d) : "
                                                "AF_UNSPEC(%d), "
                                                "AF_INET(%d), AF_INET6(%d) ",
                                                family,
                                                AF_UNSPEC, AF_INET, AF_INET6);
                                            family = AF_UNSPEC;
                                        }
                                    } else if (!strcmp(server_key, "address")) {
                                        ogs_yaml_iter_t hostname_iter;
                                        ogs_yaml_iter_recurse(
                                                &server_iter, &hostname_iter);
                                        ogs_assert(ogs_yaml_iter_type(
                                                    &hostname_iter) !=
                                                YAML_MAPPING_NODE);

                                        do {
                                            if (ogs_yaml_iter_type(
                                                        &hostname_iter) ==
                                                    YAML_SEQUENCE_NODE) {
                                                if (!ogs_yaml_iter_next(
                                                            &hostname_iter))
                                                    break;
                                            }

                                            ogs_assert(num <
                                                    OGS_MAX_NUM_OF_HOSTNAME);
                                            hostname[num++] =
                                                ogs_yaml_iter_value(
                                                        &hostname_iter);
                                        } while (
                                            ogs_yaml_iter_type(
                                                &hostname_iter) ==
                                            YAML_SEQUENCE_NODE);
                                    } else if (!strcmp(server_key, "port")) {
                                        const char *v =
                                            ogs_yaml_iter_value(&server_iter);
                                        if (v) port = atoi(v);
                                    } else if (!strcmp(server_key, "dev")) {
                                        dev = ogs_yaml_iter_value(&server_iter);
                                    } else if (!strcmp(server_key, "option")) {
                                        rv = ogs_app_parse_sockopt_config(
                                                &server_iter, &option);
                                        if (rv != OGS_OK) {
                                            ogs_error("ogs_app_parse_socket_"
                                                    "config() failed");
                                            return rv;
                                        }
                                        is_option = true;
                                    } else
                                        ogs_warn("unknown key `%s`",
                                                server_key);
                                }

                                addr = NULL;
                                for (i = 0; i < num; i++) {
                                    rv = ogs_addaddrinfo(&addr,
                                            family, hostname[i], port, 0);
                                    ogs_assert(rv == OGS_OK);
                                }

                                if (addr) {
                                    if (ogs_global_conf()->parameter.
                                            no_ipv4 == 0)
                                        ogs_socknode_add(
                                            &self.ngap_list, AF_INET, addr,
                                            is_option ? &option : NULL);
                                    if (ogs_global_conf()->parameter.
                                            no_ipv6 == 0)
                                        ogs_socknode_add(
                                            &self.ngap_list6, AF_INET6, addr,
                                            is_option ? &option : NULL);
                                    ogs_freeaddrinfo(addr);
                                }

                                if (dev) {
                                    rv = ogs_socknode_probe(
                                            ogs_global_conf()->parameter.
                                            no_ipv4 ?
                                                NULL : &self.ngap_list,
                                            ogs_global_conf()->parameter.
                                            no_ipv6 ?
                                                NULL : &self.ngap_list6,
                                            dev, port,
                                            is_option ? &option : NULL);
                                    ogs_assert(rv == OGS_OK);
                                }

                            } while (ogs_yaml_iter_type(&server_array) ==
                                    YAML_SEQUENCE_NODE);
                        } else
                            ogs_warn("unknown key `%s`", ngap_key);
                    }
                } else if (!strcmp(amf_key, "guami")) {
                    ogs_yaml_iter_t guami_array, guami_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &guami_array);
                    do {
                        const char *mcc = NULL, *mnc = NULL;
                        const char *region = NULL, *set = NULL;
                        const char *pointer = NULL;

                        if (ogs_yaml_iter_type(&guami_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&guami_iter, &guami_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&guami_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&guami_array))
                                break;
                            ogs_yaml_iter_recurse(&guami_array,
                                    &guami_iter);
                        } else if (ogs_yaml_iter_type(&guami_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        ogs_assert(self.num_of_served_guami <
                                OGS_MAX_NUM_OF_SERVED_GUAMI);

                        while (ogs_yaml_iter_next(&guami_iter)) {
                            const char *guami_key =
                                ogs_yaml_iter_key(&guami_iter);
                            ogs_assert(guami_key);
                            if (!strcmp(guami_key, "plmn_id")) {
                                ogs_yaml_iter_t plmn_id_iter;

                                ogs_yaml_iter_recurse(&guami_iter,
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
                                        &self.served_guami[
                                            self.num_of_served_guami].
                                                plmn_id,
                                        atoi(mcc), atoi(mnc), strlen(mnc));
                                }
                            } else if (!strcmp(guami_key, "amf_id")) {
                                ogs_yaml_iter_t amf_id_iter;

                                ogs_yaml_iter_recurse(&guami_iter,
                                        &amf_id_iter);
                                while (ogs_yaml_iter_next(&amf_id_iter)) {
                                    const char *amf_id_key =
                                        ogs_yaml_iter_key(&amf_id_iter);
                                    ogs_assert(amf_id_key);
                                    if (!strcmp(amf_id_key, "region")) {
                                        region = ogs_yaml_iter_value(
                                                &amf_id_iter);
                                    } else if (!strcmp(amf_id_key, "set")) {
                                        set = ogs_yaml_iter_value(
                                                &amf_id_iter);
                                    } else if (!strcmp(amf_id_key, "pointer")) {
                                        pointer = ogs_yaml_iter_value(
                                                &amf_id_iter);
                                    }
                                }

                                if (region && set) {
                                    ogs_amf_id_build(
                                        &self.served_guami[
                                            self.num_of_served_guami].
                                                amf_id,
                                        atoi(region), atoi(set),
                                        pointer ? atoi(pointer) : 0);
                                }
                            } else
                                ogs_warn("unknown key `%s`", guami_key);
                        }

                        if (mnc && mcc && region && set) {
                            self.num_of_served_guami++;
                        } else {
                            ogs_warn("Ignore guami : "
                                    "mcc(%s), mnc(%s), region(%s), set(%s)",
                                    mcc, mnc, region, set);
                        }
                    } while (ogs_yaml_iter_type(&guami_array) ==
                            YAML_SEQUENCE_NODE);
                } else if (!strcmp(amf_key, "tai")) {
                    int num_of_list0 = 0;
                    int num_of_list1 = 0;
                    ogs_5gs_tai0_list_t *list0 = NULL;
                    ogs_5gs_tai1_list_t *list1 = NULL;
                    ogs_5gs_tai2_list_t *list2 = NULL;

                    ogs_assert(self.num_of_served_tai <
                            OGS_MAX_NUM_OF_SUPPORTED_TA);
                    list0 = &self.served_tai[self.num_of_served_tai].list0;
                    list1 = &self.served_tai[self.num_of_served_tai].list1;
                    list2 = &self.served_tai[self.num_of_served_tai].list2;

                    ogs_yaml_iter_t tai_array, tai_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &tai_array);
                    do {
                        const char *mcc = NULL, *mnc = NULL;
                        int num_of_tac = 0;
                        ogs_uint24_t start[OGS_MAX_NUM_OF_TAI];
                        ogs_uint24_t end[OGS_MAX_NUM_OF_TAI];

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
                                    char *v = NULL;
                                    char *low = NULL, *high = NULL;

                                    if (ogs_yaml_iter_type(&tac_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&tac_iter))
                                            break;
                                    }

                                    v = (char *)ogs_yaml_iter_value(
                                                &tac_iter);
                                    if (v) {
                                        low = strsep(&v, "-");
                                        if (low && strlen(low) == 0)
                                            low = NULL;

                                        high = v;
                                        if (high && strlen(high) == 0)
                                            high = NULL;

                                        if (low) {
                                            ogs_assert(num_of_tac <
                                                OGS_MAX_NUM_OF_TAI);
                                            start[num_of_tac].v = atoi(low);
                                            if (high) {
                                                end[num_of_tac].v = atoi(high);
                                                if (end[num_of_tac].v <
                                                    start[num_of_tac].v)
                                                    ogs_error(
                                                        "Invalid TAI range: "
                                                        "LOW:%s,HIGH:%s",
                                                            low, high);
                                                else if (
                                                    (end[num_of_tac].v-
                                                    start[num_of_tac].v+1) >
                                                        OGS_MAX_NUM_OF_TAI)
                                                    ogs_error(
                                                        "Overflow TAI range: "
                                                        "LOW:%s,HIGH:%s",
                                                            low, high);
                                                else
                                                    num_of_tac++;
                                            } else {
                                                end[num_of_tac].v =
                                                    start[num_of_tac].v;
                                                num_of_tac++;
                                            }
                                        }
                                    }
                                } while (
                                    ogs_yaml_iter_type(&tac_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", tai_key);
                        }

                        if (mcc && mnc && num_of_tac) {
                            if (num_of_tac == 1 && start[0].v == end[0].v) {
                                ogs_assert(list2->num < OGS_MAX_NUM_OF_TAI);

                                list2->type = OGS_TAI2_TYPE;

                                ogs_plmn_id_build(
                                    &list2->tai[list2->num].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                list2->tai[list2->num].tac.v = start[0].v;

                                list2->num++;

                            } else {
                                int tac, count = 0;
                                for (tac = 0; tac < num_of_tac; tac++) {
                                    ogs_assert(end[tac].v >= start[tac].v);
                                    if (start[tac].v == end[tac].v) {
                                        ogs_assert(num_of_list0 <
                                                OGS_MAX_NUM_OF_TAI);

                                        list0->tai[num_of_list0].type =
                                            OGS_TAI0_TYPE;

                                        ogs_plmn_id_build(
                                            &list0->tai[num_of_list0].plmn_id,
                                            atoi(mcc), atoi(mnc), strlen(mnc));
                                        list0->tai[num_of_list0].
                                            tac[count].v = start[tac].v;

                                        list0->tai[num_of_list0].num =
                                            ++count;

                                    } else if (start[tac].v < end[tac].v) {
                                        ogs_assert(num_of_list1 <
                                                OGS_MAX_NUM_OF_TAI);

                                        list1->tai[num_of_list1].type =
                                            OGS_TAI1_TYPE;

                                        ogs_plmn_id_build(
                                            &list1->tai[num_of_list1].plmn_id,
                                            atoi(mcc), atoi(mnc), strlen(mnc));
                                        list1->tai[num_of_list1].tac.v =
                                            start[tac].v;

                                        list1->tai[num_of_list1].num =
                                            end[tac].v-start[tac].v+1;

                                        num_of_list1++;
                                    }
                                }

                                if (count)
                                    num_of_list0++;
                            }
                        } else {
                            ogs_warn("Ignore tai : mcc(%p), mnc(%p), "
                                    "num_of_tac(%d)", mcc, mnc, num_of_tac);
                        }
                    } while (ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE);

                    if (list2->num || num_of_list1 || num_of_list0) {
                        self.num_of_served_tai++;
                    }
                } else if (!strcmp(amf_key, "plmn_support")) {
                    ogs_yaml_iter_t plmn_support_array, plmn_support_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &plmn_support_array);
                    do {
                        const char *mnc = NULL, *mcc = NULL;

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

                        ogs_assert(self.num_of_plmn_support <
                                OGS_MAX_NUM_OF_PLMN);

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

                                    ogs_assert(
                                        self.plmn_support[
                                            self.num_of_plmn_support].
                                                num_of_s_nssai <
                                            OGS_MAX_NUM_OF_SLICE_SUPPORT);
                                    s_nssai = &self.plmn_support[
                                            self.num_of_plmn_support].s_nssai[
                                                self.plmn_support[
                                                    self.num_of_plmn_support].
                                                        num_of_s_nssai];
                                    ogs_assert(s_nssai);

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
                                                ogs_uint24_from_string_hexadecimal(
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
                } else if (!strcmp(amf_key, "access_control")) {
                    ogs_yaml_iter_t access_control_array, access_control_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &access_control_array);
                    do {

                        if (ogs_yaml_iter_type(&access_control_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&access_control_iter, &access_control_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&access_control_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&access_control_array))
                                break;
                            ogs_yaml_iter_recurse(&access_control_array,
                                    &access_control_iter);
                        } else if (ogs_yaml_iter_type(&access_control_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        ogs_assert(self.num_of_access_control <
                                OGS_MAX_NUM_OF_PLMN);

                        while (ogs_yaml_iter_next(&access_control_iter)) {
                            const char *mnc = NULL, *mcc = NULL;
                            int reject_cause = 0;
                            const char *access_control_key =
                                ogs_yaml_iter_key(&access_control_iter);
                            ogs_assert(access_control_key);
                            if (!strcmp(access_control_key,
                                        "default_reject_cause")) {
                                const char *v = ogs_yaml_iter_value(
                                        &access_control_iter);
                                if (v) self.default_reject_cause = atoi(v);
                            } else if (!strcmp(access_control_key, "plmn_id")) {
                                ogs_yaml_iter_t plmn_id_iter;

                                ogs_yaml_iter_recurse(&access_control_iter,
                                        &plmn_id_iter);
                                while (ogs_yaml_iter_next(&plmn_id_iter)) {
                                    const char *plmn_id_key =
                                        ogs_yaml_iter_key(&plmn_id_iter);
                                    ogs_assert(plmn_id_key);
                                    if (!strcmp(plmn_id_key, "reject_cause")) {
                                        const char *v = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                        if (v) reject_cause = atoi(v);
                                    } else if (!strcmp(plmn_id_key, "mcc")) {
                                        mcc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    } else if (!strcmp(plmn_id_key, "mnc")) {
                                        mnc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    }
                                }

                                if (mcc && mnc) {
                                    ogs_plmn_id_build(
                                        &self.access_control[
                                            self.num_of_access_control].
                                                plmn_id,
                                        atoi(mcc), atoi(mnc), strlen(mnc));
                                    if (reject_cause)
                                        self.access_control[
                                            self.num_of_access_control].
                                                reject_cause = reject_cause;
                                    self.num_of_access_control++;
                                }
                            } else
                                ogs_warn("unknown key `%s`",
                                        access_control_key);
                        }

                    } while (ogs_yaml_iter_type(&access_control_array) ==
                            YAML_SEQUENCE_NODE);
                } else if (!strcmp(amf_key, "security")) {
                    ogs_yaml_iter_t security_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &security_iter);
                    while (ogs_yaml_iter_next(&security_iter)) {
                        const char *security_key =
                            ogs_yaml_iter_key(&security_iter);
                        ogs_assert(security_key);
                        if (!strcmp(security_key, "integrity_order")) {
                            ogs_yaml_iter_t integrity_order_iter;
                            ogs_yaml_iter_recurse(&security_iter,
                                    &integrity_order_iter);
                            ogs_assert(ogs_yaml_iter_type(
                                        &integrity_order_iter) !=
                                YAML_MAPPING_NODE);

                            do {
                                const char *v = NULL;

                                if (ogs_yaml_iter_type(&integrity_order_iter) ==
                                        YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(
                                                &integrity_order_iter))
                                        break;
                                }

                                v = ogs_yaml_iter_value(&integrity_order_iter);
                                if (v) {
                                    int integrity_index =
                                        self.num_of_integrity_order;
                                    if (strcmp(v, "NIA0") == 0) {
                                        self.integrity_order[integrity_index] =
                                        OGS_NAS_SECURITY_ALGORITHMS_NIA0;
                                        self.num_of_integrity_order++;
                                    } else if (strcmp(v, "NIA1") == 0) {
                                        self.integrity_order[integrity_index] =
                                        OGS_NAS_SECURITY_ALGORITHMS_128_NIA1;
                                        self.num_of_integrity_order++;
                                    } else if (strcmp(v, "NIA2") == 0) {
                                        self.integrity_order[integrity_index] =
                                        OGS_NAS_SECURITY_ALGORITHMS_128_NIA2;
                                        self.num_of_integrity_order++;
                                    } else if (strcmp(v, "NIA3") == 0) {
                                        self.integrity_order[integrity_index] =
                                        OGS_NAS_SECURITY_ALGORITHMS_128_NIA3;
                                        self.num_of_integrity_order++;
                                    }
                                }
                            } while (
                                ogs_yaml_iter_type(&integrity_order_iter) ==
                                    YAML_SEQUENCE_NODE);
                        } else if (!strcmp(security_key, "ciphering_order")) {
                            ogs_yaml_iter_t ciphering_order_iter;
                            ogs_yaml_iter_recurse(&security_iter,
                                    &ciphering_order_iter);
                            ogs_assert(ogs_yaml_iter_type(
                                &ciphering_order_iter) != YAML_MAPPING_NODE);

                            do {
                                const char *v = NULL;

                                if (ogs_yaml_iter_type(&ciphering_order_iter) ==
                                        YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(
                                                &ciphering_order_iter))
                                        break;
                                }

                                v = ogs_yaml_iter_value(&ciphering_order_iter);
                                if (v) {
                                    int ciphering_index =
                                        self.num_of_ciphering_order;
                                    if (strcmp(v, "NEA0") == 0) {
                                        self.ciphering_order[ciphering_index] =
                                            OGS_NAS_SECURITY_ALGORITHMS_NEA0;
                                        self.num_of_ciphering_order++;
                                    } else if (strcmp(v, "NEA1") == 0) {
                                        self.ciphering_order[ciphering_index] =
                                        OGS_NAS_SECURITY_ALGORITHMS_128_NEA1;
                                        self.num_of_ciphering_order++;
                                    } else if (strcmp(v, "NEA2") == 0) {
                                        self.ciphering_order[ciphering_index] =
                                        OGS_NAS_SECURITY_ALGORITHMS_128_NEA2;
                                        self.num_of_ciphering_order++;
                                    } else if (strcmp(v, "NEA3") == 0) {
                                        self.ciphering_order[ciphering_index] =
                                        OGS_NAS_SECURITY_ALGORITHMS_128_NEA3;
                                        self.num_of_ciphering_order++;
                                    }
                                }
                            } while (
                                ogs_yaml_iter_type(&ciphering_order_iter) ==
                                    YAML_SEQUENCE_NODE);
                        } else
                            ogs_warn("unknown key `%s`", security_key);
                    }
                } else if (!strcmp(amf_key, "network_name")) {
                    ogs_yaml_iter_t network_name_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &network_name_iter);

                    while (ogs_yaml_iter_next(&network_name_iter)) {
                        const char *network_name_key =
                        ogs_yaml_iter_key(&network_name_iter);
                        ogs_assert(network_name_key);
                        if (!strcmp(network_name_key, "full")) {
                            ogs_nas_network_name_t *network_full_name =
                                &self.full_name;
                            const char *c_network_name =
                                ogs_yaml_iter_value(&network_name_iter);
                            uint8_t size = strlen(c_network_name);
                            uint8_t i;
                            for (i = 0;i<size;i++) {
                                /* Workaround to convert the ASCII to USC-2 */
                                network_full_name->name[i*2] = 0;
                                network_full_name->name[(i*2)+1] =
                                    c_network_name[i];

                            }
                            network_full_name->length = size*2+1;
                            network_full_name->coding_scheme = 1;
                            network_full_name->ext = 1;
                        } else if (!strcmp(network_name_key, "short")) {
                            ogs_nas_network_name_t *network_short_name =
                                &self.short_name;
                            const char *c_network_name =
                                ogs_yaml_iter_value(&network_name_iter);
                            uint8_t size = strlen(c_network_name);
                            uint8_t i;
                            for (i = 0;i<size;i++) {
                                /* Workaround to convert the ASCII to USC-2 */
                                network_short_name->name[i*2] = 0;
                                network_short_name->name[(i*2)+1] =
                                    c_network_name[i];

                            }
                            network_short_name->length = size*2+1;
                            network_short_name->coding_scheme = 1;
                            network_short_name->ext = 1;
                        } else
                            ogs_warn("unknown key `%s`", network_name_key);
                    }
                } else if (!strcmp(amf_key, "amf_name")) {
                    self.amf_name = ogs_yaml_iter_value(&amf_iter);

                } else if (!strcmp(amf_key, "time")) {
                    ogs_yaml_iter_t time_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &time_iter);
                    while (ogs_yaml_iter_next(&time_iter)) {
                        const char *time_key = ogs_yaml_iter_key(&time_iter);
                        ogs_assert(time_key);
                        if (!strcmp(time_key, "t3502")) {
                            ogs_yaml_iter_t t3502_iter;
                            ogs_yaml_iter_recurse(&time_iter, &t3502_iter);

                            while (ogs_yaml_iter_next(&t3502_iter)) {
                                const char *t3502_key =
                                    ogs_yaml_iter_key(&t3502_iter);
                                ogs_assert(t3502_key);

                                if (!strcmp(t3502_key, "value")) {
                                    const char *v = ogs_yaml_iter_value(&t3502_iter);
                                    if (v)
                                        self.time.t3502.value = atoll(v);
                                } else
                                    ogs_warn("unknown key `%s`", t3502_key);
                            }
                        } else if (!strcmp(time_key, "t3512")) {
                            ogs_yaml_iter_t t3512_iter;
                            ogs_yaml_iter_recurse(&time_iter, &t3512_iter);

                            while (ogs_yaml_iter_next(&t3512_iter)) {
                                const char *t3512_key =
                                    ogs_yaml_iter_key(&t3512_iter);
                                ogs_assert(t3512_key);

                                if (!strcmp(t3512_key, "value")) {
                                    const char *v = ogs_yaml_iter_value(&t3512_iter);
                                    if (v)
                                        self.time.t3512.value = atoll(v);
                                } else
                                    ogs_warn("unknown key `%s`", t3512_key);
                            }
                        } else if (!strcmp(time_key, "t3412")) {
                            /* handle config in mme */
                        } else if (!strcmp(time_key, "nf_instance")) {
                            /* handle config in app library */
                        } else if (!strcmp(time_key, "subscription")) {
                            /* handle config in app library */
                        } else if (!strcmp(time_key, "message")) {
                            /* handle config in app library */
                        } else if (!strcmp(time_key, "handover")) {
                            /* handle config in app library */
                        } else
                            ogs_warn("unknown key `%s`", time_key);
                    }
                } else if (!strcmp(amf_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(amf_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(amf_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(amf_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(amf_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(amf_key, "discovery")) {
                    /* handle config in sbi library */
                } else if (!strcmp(amf_key, "metrics")) {
                    /* handle config in metrics library */
                } else
                    ogs_warn("unknown key `%s`", amf_key);
            }
        }
    }

    rv = amf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

int amf_context_nf_info(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_info_t *nf_info = NULL;

    int served_i, next_new_i, info_i;
    bool next_found;
    served_i = 0;
    next_new_i = 0;
    next_found = false;
    do {
        nf_instance = ogs_sbi_self()->nf_instance;
        ogs_assert(nf_instance);

        nf_info = ogs_sbi_nf_info_add(
                &nf_instance->nf_info_list, OpenAPI_nf_type_AMF);
        ogs_assert(nf_info);

        nf_info->amf.amf_set_id =
                ogs_amf_set_id(&self.served_guami[next_new_i].amf_id);
        nf_info->amf.amf_region_id =
                ogs_amf_region_id(&self.served_guami[next_new_i].amf_id);

        next_found = false;
        info_i = 0;
        for (served_i = next_new_i; served_i <
                self.num_of_served_guami; served_i++) {
            if ((ogs_amf_set_id(&self.served_guami[served_i].amf_id) ==
                    nf_info->amf.amf_set_id) &&
                (ogs_amf_region_id(&self.served_guami[served_i].amf_id) ==
                    nf_info->amf.amf_region_id)) {
                nf_info->amf.guami[info_i] = self.served_guami[served_i];
                nf_info->amf.num_of_guami++;
                info_i++;
            } else {
                if (!next_found) {
                    int handled_i;
                    for (handled_i = 0; handled_i < served_i; handled_i++) {
                        if ((ogs_amf_set_id(
                                &self.served_guami[handled_i].amf_id) ==
                             ogs_amf_set_id(
                                &self.served_guami[served_i].amf_id)) &&
                            (ogs_amf_region_id(
                                &self.served_guami[handled_i].amf_id) ==
                             ogs_amf_region_id(
                                &self.served_guami[served_i].amf_id))) {
                            break;
                        }
                        next_found = true;
                        next_new_i = served_i;
                    }
                }
            }
        }


        int i, j, k;
        for (i = 0; i < self.num_of_served_tai; i++) {
            ogs_5gs_tai0_list_t *list0 = &self.served_tai[i].list0;
            ogs_5gs_tai1_list_t *list1 = &self.served_tai[i].list1;
            ogs_5gs_tai2_list_t *list2 = &self.served_tai[i].list2;

            for (j = 0; list0->tai[j].num; j++) {
                for (k = 0; k < list0->tai[j].num; k++) {
                    for (served_i = 0; served_i < info_i; served_i++) {
                        if (nf_info->amf.num_of_nr_tai >= OGS_MAX_NUM_OF_TAI) {
                            ogs_warn("Maximum number of TAI reached");
                            break;
                        }

                        if (ogs_plmn_id_hexdump(&list0->tai[j].plmn_id) ==
                            ogs_plmn_id_hexdump(&nf_info->amf.guami[served_i].plmn_id)) {
                            ogs_5gs_tai_t *tai =
                                &nf_info->amf.nr_tai[
                                    nf_info->amf.num_of_nr_tai];

                            tai->plmn_id = list0->tai[j].plmn_id;
                            tai->tac = list0->tai[j].tac[k];

                            nf_info->amf.num_of_nr_tai++;
                        }
                    }
                }
            }


            for (j = 0; list1->tai[j].num; j++) {
                for (served_i = 0; served_i < info_i; served_i++) {
                    if (nf_info->amf.num_of_nr_tai_range >= OGS_MAX_NUM_OF_TAI) {
                        ogs_warn("Maximum number of TAI range reached");
                        break;
                    }

                    if (ogs_plmn_id_hexdump(&list1->tai[j].plmn_id) ==
                        ogs_plmn_id_hexdump(&nf_info->amf.guami[served_i].plmn_id)) {
                        nf_info->amf.nr_tai_range[
                            nf_info->amf.num_of_nr_tai_range].plmn_id =
                                list1->tai[j].plmn_id;
                        nf_info->amf.nr_tai_range[
                            nf_info->amf.num_of_nr_tai_range].start[0].v =
                                list1->tai[j].tac.v;
                        nf_info->amf.nr_tai_range[
                            nf_info->amf.num_of_nr_tai_range].end[0].v =
                                list1->tai[j].tac.v + list1->tai[j].num - 1;
                        /* Supported is only 1 TAC range per TAI */
                        nf_info->amf.nr_tai_range[
                            nf_info->amf.num_of_nr_tai_range].num_of_tac_range = 1;

                        nf_info->amf.num_of_nr_tai_range++;
                    }
                }
            }

            if (list2->num) {
                for (j = 0; j < list2->num; j++) {
                    for (served_i = 0; served_i < info_i; served_i++) {
                        if (nf_info->amf.num_of_nr_tai >= OGS_MAX_NUM_OF_TAI) {
                            ogs_warn("Maximum number of TAI reached");
                            break;
                        }

                        if (ogs_plmn_id_hexdump(&list2->tai[j].plmn_id) ==
                            ogs_plmn_id_hexdump(&nf_info->amf.guami[served_i].plmn_id)) {
                            ogs_5gs_tai_t *tai =
                                &nf_info->amf.nr_tai[
                                    nf_info->amf.num_of_nr_tai];

                            tai->plmn_id = list2->tai[j].plmn_id;
                            tai->tac = list2->tai[j].tac;

                            nf_info->amf.num_of_nr_tai++;
                        }
                    }
                }
            }
        }
    } while (next_found);

    return OGS_OK;
}

amf_gnb_t *amf_gnb_add(ogs_sock_t *sock, ogs_sockaddr_t *addr)
{
    amf_gnb_t *gnb = NULL;
    amf_event_t e;

    ogs_assert(sock);
    ogs_assert(addr);

    ogs_pool_id_calloc(&amf_gnb_pool, &gnb);
    if (!gnb) {
        ogs_error("ogs_pool_id_calloc() failed");
        return NULL;
    }

    /* Defaut RAT-Type */
    gnb->rat_type = OpenAPI_rat_type_NR;

    gnb->sctp.sock = sock;
    gnb->sctp.addr = addr;
    gnb->sctp.type = amf_gnb_sock_type(gnb->sctp.sock);

    if (gnb->sctp.type == SOCK_STREAM) {
        gnb->sctp.poll.read = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLIN, sock->fd, ngap_recv_upcall, sock);
        ogs_assert(gnb->sctp.poll.read);
    }

    gnb->max_num_of_ostreams = 0;
    gnb->ostream_id = 0;

    ogs_list_init(&gnb->ran_ue_list);

    ogs_hash_set(self.gnb_addr_hash,
            gnb->sctp.addr, sizeof(ogs_sockaddr_t), gnb);

    memset(&e, 0, sizeof(e));
    e.gnb_id = gnb->id;
    ogs_fsm_init(&gnb->sm, ngap_state_initial, ngap_state_final, &e);

    ogs_list_add(&self.gnb_list, gnb);
    amf_metrics_inst_global_inc(AMF_METR_GLOB_GAUGE_GNB);

    ogs_info("[Added] Number of gNBs is now %d",
            ogs_list_count(&self.gnb_list));

    return gnb;
}

void amf_gnb_remove(amf_gnb_t *gnb)
{
    amf_event_t e;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_list_remove(&self.gnb_list, gnb);

    memset(&e, 0, sizeof(e));
    e.gnb_id = gnb->id;
    ogs_fsm_fini(&gnb->sm, &e);

    ogs_hash_set(self.gnb_addr_hash,
            gnb->sctp.addr, sizeof(ogs_sockaddr_t), NULL);
    if (gnb->gnb_id_presence == true)
        ogs_hash_set(self.gnb_id_hash, &gnb->gnb_id, sizeof(gnb->gnb_id), NULL);

    ogs_sctp_flush_and_destroy(&gnb->sctp);

    ogs_pool_id_free(&amf_gnb_pool, gnb);
    amf_metrics_inst_global_dec(AMF_METR_GLOB_GAUGE_GNB);
    ogs_info("[Removed] Number of gNBs is now %d",
            ogs_list_count(&self.gnb_list));
}

void amf_gnb_remove_all(void)
{
    amf_gnb_t *gnb = NULL, *next_gnb = NULL;

    ogs_list_for_each_safe(&self.gnb_list, next_gnb, gnb)
        amf_gnb_remove(gnb);
}

amf_gnb_t *amf_gnb_find_by_addr(ogs_sockaddr_t *addr)
{
    ogs_assert(addr);
    return (amf_gnb_t *)ogs_hash_get(self.gnb_addr_hash,
            addr, sizeof(ogs_sockaddr_t));

    return NULL;
}

amf_gnb_t *amf_gnb_find_by_gnb_id(uint32_t gnb_id)
{
    return (amf_gnb_t *)ogs_hash_get(self.gnb_id_hash, &gnb_id, sizeof(gnb_id));
}

int amf_gnb_set_gnb_id(amf_gnb_t *gnb, uint32_t gnb_id)
{
    ogs_assert(gnb);

    if (gnb->gnb_id_presence == true)
        ogs_hash_set(self.gnb_id_hash, &gnb->gnb_id, sizeof(gnb->gnb_id), NULL);

    gnb->gnb_id = gnb_id;
    ogs_hash_set(self.gnb_id_hash, &gnb->gnb_id, sizeof(gnb->gnb_id), gnb);

    gnb->gnb_id_presence = true;

    return OGS_OK;
}

int amf_gnb_sock_type(ogs_sock_t *sock)
{
    ogs_socknode_t *snode = NULL;

    ogs_assert(sock);

    ogs_list_for_each(&amf_self()->ngap_list, snode)
        if (snode->sock == sock) return SOCK_SEQPACKET;

    ogs_list_for_each(&amf_self()->ngap_list6, snode)
        if (snode->sock == sock) return SOCK_SEQPACKET;

    return SOCK_STREAM;
}

amf_gnb_t *amf_gnb_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&amf_gnb_pool, id);
}

/** ran_ue_context handling function */
ran_ue_t *ran_ue_add(amf_gnb_t *gnb, uint64_t ran_ue_ngap_id)
{
    ran_ue_t *ran_ue = NULL;

    ogs_assert(gnb);

    ogs_pool_id_calloc(&ran_ue_pool, &ran_ue);
    if (ran_ue == NULL) {
        ogs_error("Could not allocate ran_ue context from pool");
        return NULL;
    }

    ran_ue->t_ng_holding = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_ng_holding_timer_expire,
            OGS_UINT_TO_POINTER(ran_ue->id));
    if (!ran_ue->t_ng_holding) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&ran_ue_pool, ran_ue);
        return NULL;
    }

    ran_ue->index = ogs_pool_index(&ran_ue_pool, ran_ue);
    ogs_assert(ran_ue->index > 0 && ran_ue->index <= ogs_global_conf()->max.ue);

    ran_ue->ran_ue_ngap_id = ran_ue_ngap_id;
    ran_ue->amf_ue_ngap_id = ran_ue->index;

    /*
     * SCTP output stream identification
     * Default ogs_global_conf()->parameter.sctp_streams : 30
     *   0 : Non UE signalling
     *   1-29 : UE specific association
     */
    ogs_assert((gnb->max_num_of_ostreams-1) >= 1); /* NEXT_ID(MAX >= MIN) */
    ran_ue->gnb_ostream_id =
        OGS_NEXT_ID(gnb->ostream_id, 1, gnb->max_num_of_ostreams-1);

    ran_ue->gnb_id = gnb->id;

    ogs_list_add(&gnb->ran_ue_list, ran_ue);

    stats_add_ran_ue();

    return ran_ue;
}

void ran_ue_remove(ran_ue_t *ran_ue)
{
    amf_gnb_t *gnb = NULL;

    ogs_assert(ran_ue);

    gnb = amf_gnb_find_by_id(ran_ue->gnb_id);

    if (gnb) ogs_list_remove(&gnb->ran_ue_list, ran_ue);

    ogs_assert(ran_ue->t_ng_holding);
    ogs_timer_delete(ran_ue->t_ng_holding);

    ogs_pool_id_free(&ran_ue_pool, ran_ue);

    stats_remove_ran_ue();
}

void ran_ue_switch_to_gnb(ran_ue_t *ran_ue, amf_gnb_t *new_gnb)
{
    amf_gnb_t *gnb = NULL;

    ogs_assert(ran_ue);
    ogs_assert(new_gnb);

    gnb = amf_gnb_find_by_id(ran_ue->gnb_id);
    ogs_assert(gnb);

    /* Remove from the old gnb */
    ogs_list_remove(&gnb->ran_ue_list, ran_ue);

    /* Add to the new gnb */
    ogs_list_add(&new_gnb->ran_ue_list, ran_ue);

    /* Switch to gnb */
    ran_ue->gnb_id = new_gnb->id;
}

ran_ue_t *ran_ue_find_by_ran_ue_ngap_id(
        amf_gnb_t *gnb, uint64_t ran_ue_ngap_id)
{
    ran_ue_t *ran_ue = NULL;

    ogs_list_for_each(&gnb->ran_ue_list, ran_ue) {
        if (ran_ue_ngap_id == ran_ue->ran_ue_ngap_id)
            break;
    }

    return ran_ue;
}

ran_ue_t *ran_ue_find(uint32_t index)
{
    return ogs_pool_find(&ran_ue_pool, index);
}

ran_ue_t *ran_ue_find_by_amf_ue_ngap_id(uint64_t amf_ue_ngap_id)
{
    return ran_ue_find(amf_ue_ngap_id);
}

ran_ue_t *ran_ue_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&ran_ue_pool, id);
}

void amf_ue_new_guti(amf_ue_t *amf_ue)
{
    if (amf_ue->next.m_tmsi) {
        ogs_warn("GUTI has already been allocated");
        return;
    }

    /*
     * TS24.501
     * 5.3.3 Temporary identities
     *
     * The AMF shall assign a new 5G-GUTI for a particular UE:
     *
     * a) during a successful initial registration procedure;
     * b) during a successful registration procedure
     *    for mobility registration update; and
     * c) after a successful service request procedure invoked
     *    as a response to a paging request from the network and
     *    before the release of the N1 NAS signalling connection
     *    as specified in subclause 5.4.4.1.
     *
     * The AMF should assign a new 5G-GUTI for a particular UE
     * during a successful registration procedure
     * for periodic registration update.
     *
     * The AMF may assign a new 5G-GUTI at any time for a particular UE
     * by performing the generic UE configuration update procedure.
     */

    memset(&amf_ue->next.guti, 0, sizeof(ogs_nas_5gs_guti_t));

    ogs_assert(amf_ue->guami);
    ogs_nas_from_plmn_id(
            &amf_ue->next.guti.nas_plmn_id, &amf_ue->guami->plmn_id);
    memcpy(&amf_ue->next.guti.amf_id,
            &amf_ue->guami->amf_id, sizeof(ogs_amf_id_t));

    amf_ue->next.m_tmsi = amf_m_tmsi_alloc();
    ogs_assert(amf_ue->next.m_tmsi);
    amf_ue->next.guti.m_tmsi = *(amf_ue->next.m_tmsi);
}

void amf_ue_confirm_guti(amf_ue_t *amf_ue)
{
    ogs_assert(amf_ue->next.m_tmsi);

    if (amf_ue->current.m_tmsi) {
        /* AMF has a VALID GUTI
         * As such, we need to remove previous GUTI in hash table */
        ogs_hash_set(self.guti_ue_hash,
                &amf_ue->current.guti, sizeof(ogs_nas_5gs_guti_t), NULL);
        ogs_assert(amf_m_tmsi_free(amf_ue->current.m_tmsi) == OGS_OK);
    }

    /*
     * TS24.501
     * 5.3.3 Temporary identities
     *
     * The AMF shall assign a new 5G-GUTI for a particular UE:
     *
     * a) during a successful initial registration procedure;
     * b) during a successful registration procedure
     *    for mobility registration update; and
     * c) after a successful service request procedure invoked
     *    as a response to a paging request from the network and
     *    before the release of the N1 NAS signalling connection
     *    as specified in subclause 5.4.4.1.
     *
     * The AMF should assign a new 5G-GUTI for a particular UE
     * during a successful registration procedure
     * for periodic registration update.
     *
     * The AMF may assign a new 5G-GUTI at any time for a particular UE
     * by performing the generic UE configuration update procedure.
     */

    /* Copying from Next to Current Guti */
    amf_ue->current.m_tmsi = amf_ue->next.m_tmsi;
    memcpy(&amf_ue->current.guti,
            &amf_ue->next.guti, sizeof(ogs_nas_5gs_guti_t));

    /* Hashing Current GUTI */
    ogs_hash_set(self.guti_ue_hash,
            &amf_ue->current.guti, sizeof(ogs_nas_5gs_guti_t), amf_ue);

    /* Clear Next GUTI */
    amf_ue->next.m_tmsi = NULL;
}

amf_ue_t *amf_ue_add(ran_ue_t *ran_ue)
{
    amf_gnb_t *gnb = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(ran_ue);

    gnb = amf_gnb_find_by_id(ran_ue->gnb_id);
    if (!gnb) {
        ogs_error("[%d] gNB has already been removed", ran_ue->gnb_id);
        return NULL;
    }

    ogs_pool_id_calloc(&amf_ue_pool, &amf_ue);
    if (amf_ue == NULL) {
        ogs_error("Could not allocate amf_ue context from pool");
        return NULL;
    }

    /* Add All Timers */
    amf_ue->t3513.timer = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_t3513_expire,
            OGS_UINT_TO_POINTER(amf_ue->id));
    if (!amf_ue->t3513.timer) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&amf_ue_pool, amf_ue);
        return NULL;
    }
    amf_ue->t3513.pkbuf = NULL;
    amf_ue->t3522.timer = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_t3522_expire,
            OGS_UINT_TO_POINTER(amf_ue->id));
    if (!amf_ue->t3522.timer) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&amf_ue_pool, amf_ue);
        return NULL;
    }
    amf_ue->t3522.pkbuf = NULL;
    amf_ue->t3550.timer = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_t3550_expire,
            OGS_UINT_TO_POINTER(amf_ue->id));
    if (!amf_ue->t3550.timer) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&amf_ue_pool, amf_ue);
        return NULL;
    }
    amf_ue->t3550.pkbuf = NULL;
    amf_ue->t3555.timer = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_t3555_expire, amf_ue);
    if (!amf_ue->t3555.timer) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&amf_ue_pool, amf_ue);
        return NULL;
    }
    amf_ue->t3555.pkbuf = NULL;
    amf_ue->t3560.timer = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_t3560_expire,
            OGS_UINT_TO_POINTER(amf_ue->id));
    if (!amf_ue->t3560.timer) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&amf_ue_pool, amf_ue);
        return NULL;
    }
    amf_ue->t3560.pkbuf = NULL;
    amf_ue->t3570.timer = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_t3570_expire,
            OGS_UINT_TO_POINTER(amf_ue->id));
    if (!amf_ue->t3570.timer) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&amf_ue_pool, amf_ue);
        return NULL;
    }
    amf_ue->t3570.pkbuf = NULL;
    amf_ue->mobile_reachable.timer = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_mobile_reachable_expire,
            OGS_UINT_TO_POINTER(amf_ue->id));
    if (!amf_ue->mobile_reachable.timer) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&amf_ue_pool, amf_ue);
        return NULL;
    }
    amf_ue->mobile_reachable.pkbuf = NULL;
    amf_ue->implicit_deregistration.timer = ogs_timer_add(
            ogs_app()->timer_mgr, amf_timer_implicit_deregistration_expire,
            OGS_UINT_TO_POINTER(amf_ue->id));
    if (!amf_ue->implicit_deregistration.timer) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_id_free(&amf_ue_pool, amf_ue);
        return NULL;
    }
    amf_ue->implicit_deregistration.pkbuf = NULL;

    /* SBI Type */
    amf_ue->sbi.type = OGS_SBI_OBJ_UE_TYPE;

    /* SBI Features */
    OGS_SBI_FEATURES_SET(amf_ue->am_policy_control_features,
            OGS_SBI_NPCF_AM_POLICY_CONTROL_UE_AMBR_AUTHORIZATION);

    amf_ue->rat_restrictions = OpenAPI_list_create();
    amf_ue->to_release_session_list = OpenAPI_list_create();

    ogs_list_init(&amf_ue->sess_list);

    /* Initialization */
    amf_ue->guami = &amf_self()->served_guami[0];
    amf_ue->nas.access_type = OGS_ACCESS_TYPE_3GPP;
    amf_ue->nas.amf.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    amf_ue->abba_len = 2;

    amf_ue_fsm_init(amf_ue);

    ogs_list_add(&self.amf_ue_list, amf_ue);

    ogs_info("[Added] Number of AMF-UEs is now %d",
            ogs_list_count(&self.amf_ue_list));

    return amf_ue;
}

void amf_ue_remove(amf_ue_t *amf_ue)
{
    int i;

    ogs_assert(amf_ue);

    ogs_list_remove(&self.amf_ue_list, amf_ue);

    amf_ue_fsm_fini(amf_ue);

    /* Clear Paging Info */
    AMF_UE_CLEAR_PAGING_INFO(amf_ue);

    /* Clear N2 Transfer */
    AMF_UE_CLEAR_N2_TRANSFER(amf_ue, pdu_session_resource_setup_request);

    /* Clear 5GSM Message */
    AMF_UE_CLEAR_5GSM_MESSAGE(amf_ue);

    OpenAPI_list_free(amf_ue->rat_restrictions);
    OpenAPI_list_free(amf_ue->to_release_session_list);

    /* Remove all session context */
    amf_sess_remove_all(amf_ue);

    if (amf_ue->current.m_tmsi) {
        ogs_hash_set(self.guti_ue_hash,
                &amf_ue->current.guti, sizeof(ogs_nas_5gs_guti_t), NULL);
        ogs_assert(amf_m_tmsi_free(amf_ue->current.m_tmsi) == OGS_OK);
    }
    if (amf_ue->next.m_tmsi) {
        ogs_assert(amf_m_tmsi_free(amf_ue->next.m_tmsi) == OGS_OK);
    }
    if (amf_ue->suci) {
        ogs_hash_set(self.suci_hash, amf_ue->suci, strlen(amf_ue->suci), NULL);
        ogs_free(amf_ue->suci);
    }
    if (amf_ue->supi) {
        ogs_hash_set(self.supi_hash, amf_ue->supi, strlen(amf_ue->supi), NULL);
        ogs_free(amf_ue->supi);
    }

    if (amf_ue->pei)
        ogs_free(amf_ue->pei);

    for (i = 0; i < amf_ue->num_of_msisdn; i++) {
        ogs_assert(amf_ue->msisdn[i]);
        ogs_free(amf_ue->msisdn[i]);
    }

    /* Clear SubscribedInfo */
    amf_clear_subscribed_info(amf_ue);

    PCF_AM_POLICY_CLEAR(amf_ue);
    if (amf_ue->policy_association.client)
        ogs_sbi_client_remove(amf_ue->policy_association.client);

    UDM_SDM_CLEAR(amf_ue);
    if (amf_ue->data_change_subscription.client)
        ogs_sbi_client_remove(amf_ue->data_change_subscription.client);

    CLEAR_5G_AKA_CONFIRMATION(amf_ue);
    if (amf_ue->confirmation_for_5g_aka.client)
        ogs_sbi_client_remove(amf_ue->confirmation_for_5g_aka.client);

    /* Free UeRadioCapability */
    OGS_ASN_CLEAR_DATA(&amf_ue->ueRadioCapability);

    /* Clear Transparent Container */
    OGS_ASN_CLEAR_DATA(&amf_ue->handover.container);

    /* Delete All Timers */
    CLEAR_AMF_UE_ALL_TIMERS(amf_ue);
    ogs_timer_delete(amf_ue->t3513.timer);
    ogs_timer_delete(amf_ue->t3522.timer);
    ogs_timer_delete(amf_ue->t3550.timer);
    ogs_timer_delete(amf_ue->t3555.timer);
    ogs_timer_delete(amf_ue->t3560.timer);
    ogs_timer_delete(amf_ue->t3570.timer);
    ogs_timer_delete(amf_ue->mobile_reachable.timer);
    ogs_timer_delete(amf_ue->implicit_deregistration.timer);

    /* Free SBI object memory */
    if (ogs_list_count(&amf_ue->sbi.xact_list))
        ogs_error("UE transaction [%d]",
                ogs_list_count(&amf_ue->sbi.xact_list));
    ogs_sbi_object_free(&amf_ue->sbi);

    amf_ue_deassociate(amf_ue);

    ogs_pool_id_free(&amf_ue_pool, amf_ue);

    ogs_info("[Removed] Number of AMF-UEs is now %d",
            ogs_list_count(&self.amf_ue_list));
}

void amf_ue_remove_all(void)
{
    amf_ue_t *amf_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.amf_ue_list, next, amf_ue) {
        ran_ue_t *ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);

        if (ran_ue) ran_ue_remove(ran_ue);

        amf_ue_remove(amf_ue);
    }
}

void amf_ue_fsm_init(amf_ue_t *amf_ue)
{
    amf_event_t e;

    ogs_assert(amf_ue);

    memset(&e, 0, sizeof(e));
    e.amf_ue_id = amf_ue->id;
    ogs_fsm_init(&amf_ue->sm, gmm_state_initial, gmm_state_final, &e);
}

void amf_ue_fsm_fini(amf_ue_t *amf_ue)
{
    amf_event_t e;

    ogs_assert(amf_ue);

    memset(&e, 0, sizeof(e));
    e.amf_ue_id = amf_ue->id;
    ogs_fsm_fini(&amf_ue->sm, &e);
}

amf_ue_t *amf_ue_find_by_guti(ogs_nas_5gs_guti_t *guti)
{
    ogs_assert(guti);

    return (amf_ue_t *)ogs_hash_get(
            self.guti_ue_hash, guti, sizeof(ogs_nas_5gs_guti_t));
}

amf_ue_t *amf_ue_find_by_suci(char *suci)
{
    ogs_assert(suci);
    return (amf_ue_t *)ogs_hash_get(self.suci_hash, suci, strlen(suci));
}

amf_ue_t *amf_ue_find_by_supi(char *supi)
{
    ogs_assert(supi);
    return (amf_ue_t *)ogs_hash_get(self.supi_hash, supi, strlen(supi));
}

amf_ue_t *amf_ue_find_by_message(ogs_nas_5gs_message_t *message)
{
    amf_ue_t *amf_ue = NULL;
    ogs_nas_5gs_registration_request_t *registration_request = NULL;
    ogs_nas_5gs_deregistration_request_from_ue_t *deregistration_request = NULL;
    ogs_nas_5gs_service_request_t *service_request = NULL;
    ogs_nas_5gs_mobile_identity_t *mobile_identity = NULL;
    ogs_nas_5gs_mobile_identity_header_t *mobile_identity_header = NULL;
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci = NULL;
    ogs_nas_5gs_mobile_identity_guti_t *mobile_identity_guti = NULL;
    ogs_nas_5gs_mobile_identity_s_tmsi_t *mobile_identity_s_tmsi = NULL;
    ogs_nas_5gs_guti_t nas_guti;

    char *suci = NULL;

    ogs_assert(message);
    memset(&nas_guti, 0, sizeof(ogs_nas_5gs_guti_t));

    switch (message->gmm.h.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        registration_request = &message->gmm.registration_request;
        ogs_assert(registration_request);
        mobile_identity = &registration_request->mobile_identity;
        ogs_assert(mobile_identity);

        if (!mobile_identity->length || !mobile_identity->buffer) {
            ogs_error("No Mobilie Identity [%d]", mobile_identity->length);
            return NULL;
        }

        mobile_identity_header =
                (ogs_nas_5gs_mobile_identity_header_t *)mobile_identity->buffer;

        switch (mobile_identity_header->type) {
        case OGS_NAS_5GS_MOBILE_IDENTITY_SUCI:
            mobile_identity_suci =
                (ogs_nas_5gs_mobile_identity_suci_t *)mobile_identity->buffer;

            if (mobile_identity_suci->h.supi_format !=
                    OGS_NAS_5GS_SUPI_FORMAT_IMSI) {
                ogs_error("Not implemented SUPI format [%d]",
                    mobile_identity_suci->h.supi_format);
                return NULL;
            }
            if (mobile_identity_suci->protection_scheme_id !=
                    OGS_PROTECTION_SCHEME_NULL &&
                mobile_identity_suci->protection_scheme_id !=
                    OGS_PROTECTION_SCHEME_PROFILE_A &&
                mobile_identity_suci->protection_scheme_id !=
                    OGS_PROTECTION_SCHEME_PROFILE_B) {
                ogs_error("Invalid ProtectionSchemeID(%d) in SUCI",
                    mobile_identity_suci->protection_scheme_id);
                return NULL;
            }

            suci = ogs_nas_5gs_suci_from_mobile_identity(mobile_identity);
            if (!suci) {
                ogs_error("Cannot get the SUCI from Mobilie Identity");
                ogs_log_hexdump(OGS_LOG_ERROR,
                        mobile_identity->buffer, mobile_identity->length);
                return NULL;
            }

            amf_ue = amf_ue_find_by_suci(suci);
            if (amf_ue) {
                ogs_info("[%s] known UE by SUCI", suci);
            } else {
                ogs_info("[%s] Unknown UE by SUCI", suci);
            }
            ogs_free(suci);
            break;
        case OGS_NAS_5GS_MOBILE_IDENTITY_GUTI:
            mobile_identity_guti =
                (ogs_nas_5gs_mobile_identity_guti_t *)mobile_identity->buffer;
            ogs_assert(mobile_identity_guti);

            ogs_nas_5gs_mobile_identity_guti_to_nas_guti(
                mobile_identity_guti, &nas_guti);

            amf_ue = amf_ue_find_by_guti(&nas_guti);
            if (amf_ue) {
                ogs_info("[%s] Known UE by 5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                    amf_ue->suci ? amf_ue->suci : "Unknown",
                    ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
            } else {
                ogs_info("Unknown UE by 5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                    ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
            }
            break;
        default:
            ogs_error("Unknown Mobile Identity type [%d]",
                    mobile_identity_header->type);
        }
        break;
    case OGS_NAS_5GS_SERVICE_REQUEST:
        service_request = &message->gmm.service_request;
        ogs_assert(service_request);
        mobile_identity = &service_request->s_tmsi;
        ogs_assert(mobile_identity);

        if (!mobile_identity->length || !mobile_identity->buffer) {
            ogs_error("No Mobilie Identity [%d]", mobile_identity->length);
            return NULL;
        }

        mobile_identity_header =
                (ogs_nas_5gs_mobile_identity_header_t *)mobile_identity->buffer;

        switch (mobile_identity_header->type) {
        case OGS_NAS_5GS_MOBILE_IDENTITY_S_TMSI:
            mobile_identity_s_tmsi =
                (ogs_nas_5gs_mobile_identity_s_tmsi_t *)mobile_identity->buffer;
            ogs_assert(mobile_identity_s_tmsi);

            /* Use the first configured plmn_id and mme group id */
            ogs_nas_from_plmn_id(&nas_guti.nas_plmn_id,
                    &amf_self()->served_guami[0].plmn_id);
            nas_guti.amf_id.region = amf_self()->served_guami[0].amf_id.region;

            /* Getting from S_TMSI */
            nas_guti.amf_id.set1 = mobile_identity_s_tmsi->set1;
            nas_guti.amf_id.set2 = mobile_identity_s_tmsi->set2;
            nas_guti.amf_id.pointer = mobile_identity_s_tmsi->pointer;

            nas_guti.m_tmsi = be32toh(mobile_identity_s_tmsi->m_tmsi);

            amf_ue = amf_ue_find_by_guti(&nas_guti);
            if (amf_ue) {
                ogs_info("[%s] Known UE by 5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                    amf_ue->suci ? amf_ue->suci : "Unknown",
                    ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
            } else {
                ogs_info("Unknown UE by 5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                    ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
            }
            break;
        default:
            ogs_error("Unknown Mobile Identity type [%d]",
                    mobile_identity_header->type);
        }
        break;
    case OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE:
        deregistration_request = &message->gmm.deregistration_request_from_ue;
        ogs_assert(deregistration_request);
        mobile_identity = &deregistration_request->mobile_identity;
        ogs_assert(mobile_identity);

        if (!mobile_identity->length || !mobile_identity->buffer) {
            ogs_error("No Mobilie Identity [%d]", mobile_identity->length);
            return NULL;
        }

        mobile_identity_header =
                (ogs_nas_5gs_mobile_identity_header_t *)mobile_identity->buffer;

        switch (mobile_identity_header->type) {
        case OGS_NAS_5GS_MOBILE_IDENTITY_GUTI:
            mobile_identity_guti =
                (ogs_nas_5gs_mobile_identity_guti_t *)mobile_identity->buffer;
            ogs_assert(mobile_identity_guti);

            ogs_nas_5gs_mobile_identity_guti_to_nas_guti(
                mobile_identity_guti, &nas_guti);

            amf_ue = amf_ue_find_by_guti(&nas_guti);
            if (amf_ue) {
                ogs_info("[%s] Known UE by 5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                    amf_ue->suci ? amf_ue->suci : "Unknown",
                    ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
            } else {
                ogs_info("Unknown UE by 5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                    ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
            }
            break;
        default:
            ogs_error("Unknown Mobile Identity type [%d]",
                    mobile_identity_header->type);
        }
        break;
    default:
        break;
    }

    return amf_ue;
}

static bool amf_namf_comm_parse_guti(ogs_nas_5gs_guti_t *guti, char *ue_context_id)
{
#define MIN_LENGTH_OF_MNC 2
#define MAX_LENGTH_OF_MNC 3
#define LENGTH_OF_MCC 3
#define LENGTH_OF_AMF_ID 6
#define LENGTH_OF_TMSI 8

    char amf_id_string[LENGTH_OF_AMF_ID + 1];
    char tmsi_string[LENGTH_OF_TMSI + 1];
    char mcc_string[LENGTH_OF_MCC + 1];
    char mnc_string[MAX_LENGTH_OF_MNC + 1];
    OpenAPI_plmn_id_t Plmn_id;
    ogs_plmn_id_t plmn_id;

    /* TS29.518 6.1.3.2.2 Guti pattern (27 or 28 characters):
    "5g-guti-[0-9]{5,6}[0-9a-fA-F]{14}" */

    short index = 8; /* start parsing guti after "5g-guti-" */

    ogs_cpystrn(mcc_string, &ue_context_id[index], LENGTH_OF_MCC+1);
    index += LENGTH_OF_MCC;

    if (strlen(ue_context_id) == OGS_MAX_5G_GUTI_LEN - 1) {
        /* mnc is 2 characters long */
        ogs_cpystrn(mnc_string, &ue_context_id[index], MIN_LENGTH_OF_MNC+1);
        index += MIN_LENGTH_OF_MNC;
    } else if (strlen(ue_context_id) == OGS_MAX_5G_GUTI_LEN) {
        /* mnc is 3 characters long */
        ogs_cpystrn(mnc_string, &ue_context_id[index], MAX_LENGTH_OF_MNC+1);
        index += MAX_LENGTH_OF_MNC;
    } else {
        ogs_error("Invalid Ue context id");
        return false;
    }

    ogs_cpystrn(amf_id_string, &ue_context_id[index], LENGTH_OF_AMF_ID+1);
    index += LENGTH_OF_AMF_ID;

    ogs_cpystrn(tmsi_string, &ue_context_id[index], LENGTH_OF_TMSI+1);

    memset(&Plmn_id, 0, sizeof(Plmn_id));
    Plmn_id.mcc = mcc_string;
    Plmn_id.mnc = mnc_string;

    memset(&plmn_id, 0, sizeof(plmn_id));
    ogs_sbi_parse_plmn_id(&plmn_id, &Plmn_id);
    ogs_nas_from_plmn_id(&guti->nas_plmn_id, &plmn_id);
    ogs_amf_id_from_string(&guti->amf_id, amf_id_string);

    guti->m_tmsi = (u_int32_t)strtol(tmsi_string, NULL, 16);
    return true;
}

amf_ue_t *amf_ue_find_by_ue_context_id(char *ue_context_id)
{
    amf_ue_t *amf_ue = NULL;

    ogs_assert(ue_context_id);

    if (strncmp(ue_context_id, OGS_ID_SUPI_TYPE_IMSI,
            strlen(OGS_ID_SUPI_TYPE_IMSI)) == 0) {

        amf_ue = amf_ue_find_by_supi(ue_context_id);
        if (!amf_ue) {
            ogs_info("[%s] Unknown UE by SUPI", ue_context_id);
            return NULL;
        }

    } else if (strncmp(ue_context_id, OGS_ID_5G_GUTI_TYPE,
            strlen(OGS_ID_5G_GUTI_TYPE)) == 0) {

        ogs_nas_5gs_guti_t guti;
        memset(&guti, 0, sizeof(guti));

        if (amf_namf_comm_parse_guti(&guti, ue_context_id) == false) {
            ogs_error("amf_namf_comm_parse_guti() failed");
            return NULL;
        }

        amf_ue = amf_ue_find_by_guti(&guti);
        if (!amf_ue) {
            ogs_info("[%s] Unknown UE by GUTI", ue_context_id);
            return NULL;
        }

    } else {
        ogs_error("Unsupported UE context ID type");
        return NULL;
    }

    return amf_ue;
}

void amf_ue_set_suci(amf_ue_t *amf_ue,
        ogs_nas_5gs_mobile_identity_t *mobile_identity)
{
    amf_ue_t *old_amf_ue = NULL;
    amf_sess_t *old_sess = NULL;
    char *suci = NULL;

    ogs_assert(amf_ue);
    ogs_assert(mobile_identity);

    suci = ogs_nas_5gs_suci_from_mobile_identity(mobile_identity);
    ogs_assert(suci);

    /* Check if OLD amf_ue_t is existed */
    old_amf_ue = amf_ue_find_by_suci(suci);
    if (old_amf_ue) {
        /* Check if OLD amf_ue_t is different with NEW amf_ue_t */
        if (ogs_pool_index(&amf_ue_pool, amf_ue) !=
            ogs_pool_index(&amf_ue_pool, old_amf_ue)) {
            ogs_warn("[%s] OLD UE Context Release", suci);
            if (CM_CONNECTED(old_amf_ue)) {
                ran_ue_t *ran_ue = ran_ue_find_by_id(old_amf_ue->ran_ue_id);
                /* Implcit NG release */
                ogs_warn("[%s] Implicit NG release", suci);
                if (ran_ue) {
                    ogs_warn("[%s]    RAN_UE_NGAP_ID[%lld] "
                            "AMF_UE_NGAP_ID[%lld]",
                            old_amf_ue->suci,
                            (long long)ran_ue->ran_ue_ngap_id,
                            (long long)ran_ue->amf_ue_ngap_id);
                    ran_ue_remove(ran_ue);
                } else {
                    ogs_error("[%s] RAN-NG Context has already been removed",
                                old_amf_ue->suci);
                }
            }

    /*
     * We should delete the AMF-Session Context in the AMF-UE Context.
     * Otherwise, all unnecessary SESSIONs remain in SMF/UPF.
     *
     * In order to do this, AMF-Session Context should be moved
     * from OLD AMF-UE Context to NEW AMF-UE Context.
     *
     * If needed, The Session deletion process in NEW-AMF UE context will work.
     *
     * Note that we should not send Session-Release to the SMF at this point.
     * Another SBI Transaction can cause fatal errors.
     */

            /* Phase-1 : Change AMF-UE Context in Session Context */
            ogs_list_for_each(&old_amf_ue->sess_list, old_sess)
                old_sess->amf_ue_id = amf_ue->id;

            /* Phase-2 : Move Session Context from OLD to NEW AMF-UE Context */
            memcpy(&amf_ue->sess_list,
                    &old_amf_ue->sess_list, sizeof(amf_ue->sess_list));

            /* Phase-3 : Clear Session Context in OLD AMF-UE Context */
            memset(&old_amf_ue->sess_list, 0, sizeof(old_amf_ue->sess_list));

            amf_ue_remove(old_amf_ue);
        }
    }

    if (amf_ue->suci) {
        ogs_hash_set(self.suci_hash, amf_ue->suci, strlen(amf_ue->suci), NULL);
        ogs_free(amf_ue->suci);
    }
    amf_ue->suci = suci;
    ogs_hash_set(self.suci_hash, amf_ue->suci, strlen(amf_ue->suci), amf_ue);
}

void amf_ue_set_supi(amf_ue_t *amf_ue, char *supi)
{
    ogs_assert(supi);

    if (amf_ue->supi) {
        ogs_hash_set(self.supi_hash, amf_ue->supi, strlen(amf_ue->supi), NULL);
        ogs_free(amf_ue->supi);
    }
    amf_ue->supi = ogs_strdup(supi);
    ogs_assert(amf_ue->supi);
    ogs_hash_set(self.supi_hash, amf_ue->supi, strlen(amf_ue->supi), amf_ue);
}

OpenAPI_rat_type_e amf_ue_rat_type(amf_ue_t *amf_ue)
{
    amf_gnb_t *gnb = NULL;
    ran_ue_t *ran_ue = NULL;

    ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
    if (!ran_ue) {
        ogs_error("[%s] RAN-NG Context has already been removed", amf_ue->suci);
        return OpenAPI_rat_type_NULL;
    }

    gnb = amf_gnb_find_by_id(ran_ue->gnb_id);
    if (!gnb) {
        ogs_error("[%d] gNB has already been removed", ran_ue->gnb_id);
        return OpenAPI_rat_type_NULL;
    }

    return gnb->rat_type;
}

void amf_ue_associate_ran_ue(amf_ue_t *amf_ue, ran_ue_t *ran_ue)
{
    ogs_assert(amf_ue);
    ogs_assert(ran_ue);

    amf_ue->ran_ue_id = ran_ue->id;
    ran_ue->amf_ue_id = amf_ue->id;
}

void ran_ue_deassociate(ran_ue_t *ran_ue)
{
    ogs_assert(ran_ue);
    ran_ue->amf_ue_id = OGS_INVALID_POOL_ID;
}

void amf_ue_deassociate(amf_ue_t *amf_ue)
{
    ogs_assert(amf_ue);
    amf_ue->ran_ue_id = OGS_INVALID_POOL_ID;
}

void source_ue_associate_target_ue(
        ran_ue_t *source_ue, ran_ue_t *target_ue)
{
    ogs_assert(source_ue);
    ogs_assert(target_ue);

    target_ue->amf_ue_id = source_ue->amf_ue_id;
    target_ue->source_ue_id = source_ue->id;
    source_ue->target_ue_id = target_ue->id;
}

void source_ue_deassociate_target_ue(ran_ue_t *ran_ue)
{
    ran_ue_t *source_ue = NULL;
    ran_ue_t *target_ue = NULL;
    ogs_assert(ran_ue);

    if (ran_ue->target_ue_id >= OGS_MIN_POOL_ID &&
        ran_ue->target_ue_id <= OGS_MAX_POOL_ID) {
        source_ue = ran_ue;
        target_ue = ran_ue_find_by_id(ran_ue->target_ue_id);

        ogs_assert(source_ue->target_ue_id >= OGS_MIN_POOL_ID &&
                source_ue->target_ue_id <= OGS_MAX_POOL_ID);
        ogs_assert(target_ue->source_ue_id >= OGS_MIN_POOL_ID &&
                target_ue->source_ue_id <= OGS_MAX_POOL_ID);
        source_ue->target_ue_id = OGS_INVALID_POOL_ID;
        target_ue->source_ue_id = OGS_INVALID_POOL_ID;
    } else if (ran_ue->source_ue_id >= OGS_MIN_POOL_ID &&
                ran_ue->source_ue_id <= OGS_MAX_POOL_ID) {
        target_ue = ran_ue;
        source_ue = ran_ue_find_by_id(ran_ue->source_ue_id);

        ogs_assert(source_ue->target_ue_id >= OGS_MIN_POOL_ID &&
                source_ue->target_ue_id <= OGS_MAX_POOL_ID);
        ogs_assert(target_ue->source_ue_id >= OGS_MIN_POOL_ID &&
                target_ue->source_ue_id <= OGS_MAX_POOL_ID);
        source_ue->target_ue_id = OGS_INVALID_POOL_ID;
        target_ue->source_ue_id = OGS_INVALID_POOL_ID;
    }
}

amf_sess_t *amf_sess_add(amf_ue_t *amf_ue, uint8_t psi)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);
    ogs_assert(psi != OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED);

    ogs_pool_id_calloc(&amf_sess_pool, &sess);
    ogs_assert(sess);

    sess->sbi.type = OGS_SBI_OBJ_SESS_TYPE;

    sess->amf_ue_id = amf_ue->id;
    sess->psi = psi;

    sess->s_nssai.sst = 0;
    sess->s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    sess->mapped_hplmn_presence = false;
    sess->mapped_hplmn.sst = 0;
    sess->mapped_hplmn.sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    ogs_list_add(&amf_ue->sess_list, sess);

    stats_add_amf_session();

    return sess;
}

void amf_sess_remove(amf_sess_t *sess)
{
    amf_ue_t *amf_ue = NULL;

    ogs_assert(sess);

    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    if (amf_ue)
        ogs_list_remove(&amf_ue->sess_list, sess);
    else
        ogs_error("UE(amf-ue) context has already been removed");

    /* Free SBI object memory */
    if (ogs_list_count(&sess->sbi.xact_list))
        ogs_error("Session transaction [%d]",
                ogs_list_count(&sess->sbi.xact_list));
    ogs_sbi_object_free(&sess->sbi);

    CLEAR_SESSION_CONTEXT(sess);

    if (sess->sm_context.client)
        ogs_sbi_client_remove(sess->sm_context.client);

    if (sess->payload_container)
        ogs_pkbuf_free(sess->payload_container);
    if (sess->dnn)
        ogs_free(sess->dnn);

    if (sess->pdu_session_establishment_accept)
        ogs_pkbuf_free(sess->pdu_session_establishment_accept);

    AMF_SESS_CLEAR_N2_TRANSFER(sess, pdu_session_resource_setup_request);
    AMF_SESS_CLEAR_N2_TRANSFER(sess, pdu_session_resource_modification_command);
    AMF_SESS_CLEAR_N2_TRANSFER(sess, path_switch_request_ack);
    AMF_SESS_CLEAR_N2_TRANSFER(sess, handover_request);
    AMF_SESS_CLEAR_N2_TRANSFER(sess, handover_command);

    if (sess->paging.client)
        ogs_sbi_client_remove(sess->paging.client);

    if (sess->nssf.nsi_id)
        ogs_free(sess->nssf.nsi_id);
    if (sess->nssf.nrf.id)
        ogs_free(sess->nssf.nrf.id);
    if (sess->nssf.nrf.client)
        ogs_sbi_client_remove(sess->nssf.nrf.client);

    ogs_pool_id_free(&amf_sess_pool, sess);

    stats_remove_amf_session();
}

void amf_sess_remove_all(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL, *next_sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each_safe(&amf_ue->sess_list, next_sess, sess)
        amf_sess_remove(sess);
}

amf_sess_t *amf_sess_find_by_psi(amf_ue_t *amf_ue, uint8_t psi)
{
    amf_sess_t *sess = NULL;

    ogs_list_for_each(&amf_ue->sess_list, sess)
        if (psi == sess->psi) return sess;

    return NULL;
}

amf_ue_t *amf_ue_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&amf_ue_pool, id);
}

amf_sess_t *amf_sess_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&amf_sess_pool, id);
}

void amf_sbi_select_nf(
        ogs_sbi_object_t *sbi_object,
        ogs_sbi_service_type_e service_type,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option)
{
    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(sbi_object);
    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    ogs_assert(requester_nf_type);

    nf_instance = ogs_sbi_nf_instance_find_by_discovery_param(
                    target_nf_type, requester_nf_type, discovery_option);
    if (nf_instance)
        OGS_SBI_SETUP_NF_INSTANCE(
                sbi_object->service_type_array[service_type], nf_instance);
}

int amf_sess_xact_count(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;
    int xact_count = 0;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess)
        xact_count += ogs_list_count(&sess->sbi.xact_list);

    return xact_count;
}

int amf_sess_xact_state_count(amf_ue_t *amf_ue, int state)
{
    amf_sess_t *sess = NULL;
    ogs_sbi_xact_t *xact = NULL;
    int xact_count = 0;

    ogs_assert(amf_ue);
    ogs_assert(state);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        ogs_list_for_each(&sess->sbi.xact_list, xact)
            if (xact->state == state) xact_count++;
    }

    return xact_count;
}

bool amf_pdu_res_setup_req_transfer_needed(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess)
        if (sess->transfer.pdu_session_resource_setup_request)
            return true;

    return false;
}

bool amf_handover_request_transfer_needed(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess)
        if (sess->transfer.handover_request)
            return true;

    return false;
}

bool amf_paging_ongoing(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        if (sess->paging.ongoing == true)
            return true;
    }

    return false;
}

bool amf_downlink_signalling_pending(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess)
        if (sess->gsm_message.type)
            return true;

    return false;
}

int amf_find_served_tai(ogs_5gs_tai_t *nr_tai)
{
    int i = 0, j = 0, k = 0;

    ogs_assert(nr_tai);

    for (i = 0; i < self.num_of_served_tai; i++) {
        ogs_5gs_tai0_list_t *list0 = &self.served_tai[i].list0;
        ogs_5gs_tai1_list_t *list1 = &self.served_tai[i].list1;
        ogs_5gs_tai2_list_t *list2 = &self.served_tai[i].list2;

        for (j = 0; list0->tai[j].num; j++) {
            ogs_assert(list0->tai[j].type == OGS_TAI0_TYPE);
            ogs_assert(list0->tai[j].num <= OGS_MAX_NUM_OF_TAI);

            for (k = 0; k < list0->tai[j].num; k++) {
                if (memcmp(&list0->tai[j].plmn_id,
                            &nr_tai->plmn_id, OGS_PLMN_ID_LEN) == 0 &&
                    list0->tai[j].tac[k].v == nr_tai->tac.v) {
                    return i;
                }
            }
        }

        for (j = 0; list1->tai[j].num; j++) {
            ogs_assert(list1->tai[j].type == OGS_TAI1_TYPE);
            ogs_assert(list1->tai[j].num <= OGS_MAX_NUM_OF_TAI);

            if (list1->tai[j].tac.v <= nr_tai->tac.v &&
                nr_tai->tac.v < (list1->tai[j].tac.v+list1->tai[j].num))
                return i;
        }

        if (list2->num) {
            ogs_assert(list2->type == OGS_TAI2_TYPE);
            ogs_assert(list2->num <= OGS_MAX_NUM_OF_TAI);

            for (j = 0; j < list2->num; j++) {
                if (memcmp(&list2->tai[j].plmn_id,
                            &nr_tai->plmn_id, OGS_PLMN_ID_LEN) == 0 &&
                    list2->tai[j].tac.v == nr_tai->tac.v) {
                    return i;
                }
            }
        }
    }

    return -1;
}

ogs_s_nssai_t *amf_find_s_nssai(
        ogs_plmn_id_t *served_plmn_id, ogs_s_nssai_t *s_nssai)
{
    int i, j;

    ogs_assert(served_plmn_id);
    ogs_assert(s_nssai);

    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        if (memcmp(&amf_self()->plmn_support[i].plmn_id,
                    served_plmn_id, OGS_PLMN_ID_LEN) != 0)
            continue;

        for (j = 0; j < amf_self()->plmn_support[i].num_of_s_nssai; j++) {
            /* Compare S-NSSAI */
            if (amf_self()->plmn_support[i].s_nssai[j].sst == s_nssai->sst &&
                amf_self()->plmn_support[i].s_nssai[j].sd.v == s_nssai->sd.v) {
                return &amf_self()->plmn_support[i].s_nssai[j];
            }
        }
    }

    return NULL;
}

#if 0 /* DEPRECATED */
int amf_m_tmsi_pool_generate(void)
{
    int j;
    int index = 0;

    ogs_trace("M-TMSI Pool try to generate...");
    while (index < ogs_global_conf()->max.ue*2) {
        amf_m_tmsi_t *m_tmsi = NULL;
        int conflict = 0;

        m_tmsi = &m_tmsi_pool.array[index];
        ogs_assert(m_tmsi);
        *m_tmsi = ogs_random32();

        /* for mapped-GUTI */
        *m_tmsi |= 0xc0000000;
        *m_tmsi &= 0xff00ffff;

        for (j = 0; j < index; j++) {
            if (*m_tmsi == m_tmsi_pool.array[j]) {
                conflict = 1;
                ogs_trace("[M-TMSI CONFLICT]  %d:0x%x == %d:0x%x",
                        index, *m_tmsi, j, m_tmsi_pool.array[j]);
                break;
            }
        }
        if (conflict == 1) {
            continue;
        }

        index++;
    }
    m_tmsi_pool.size = index;
    ogs_trace("M-TMSI Pool generate...done");

    return OGS_OK;
}
#endif

amf_m_tmsi_t *amf_m_tmsi_alloc(void)
{
    amf_m_tmsi_t *m_tmsi = NULL;

    ogs_pool_alloc(&m_tmsi_pool, &m_tmsi);
    ogs_assert(m_tmsi);

    /* TS23.003
     * 2.8.2.1.2 Mapping in the UE
     *
     * E-UTRAN <M-TMSI> maps as follows:
     * - 6 bits of the E-UTRAN <M-TMSI> starting at bit 29 and down to bit 24
     * are mapped into bit 29 and down to bit 24 of the GERAN/UTRAN <P-TMSI>;
     * - 16 bits of the E-UTRAN <M-TMSI> starting at bit 15 and down to bit 0
     * are mapped into bit 15 and down to bit 0 of the GERAN/UTRAN <P-TMSI>;
     * - and the remaining 8 bits of the E-UTRAN <M-TMSI> are
     * mapped into the 8 Most Significant Bits of the <P-TMSI signature> field.
     *
     * The UE shall fill the remaining 2 octets of the <P-TMSI signature>
     * according to clauses 9.1.1, 9.4.1, 10.2.1, or 10.5.1
     * of 3GPP TS.33.401 [89] , as appropriate, for RAU/Attach procedures
     */

    ogs_assert(*m_tmsi <= 0x003fffff);

    *m_tmsi = ((*m_tmsi & 0xffff) | ((*m_tmsi & 0x003f0000) << 8));
    *m_tmsi |= 0xc0000000;

    return m_tmsi;
}

int amf_m_tmsi_free(amf_m_tmsi_t *m_tmsi)
{
    ogs_assert(m_tmsi);

    /* Restore M-TMSI by Issue #2307 */
    *m_tmsi &= 0x3fffffff;
    *m_tmsi = ((*m_tmsi & 0xffff) | ((*m_tmsi & 0x3f000000) >> 8));
    ogs_pool_free(&m_tmsi_pool, m_tmsi);

    return OGS_OK;
}

uint8_t amf_selected_int_algorithm(amf_ue_t *amf_ue)
{
    int i;

    ogs_assert(amf_ue);

    for (i = 0; i < amf_self()->num_of_integrity_order; i++) {
        if (amf_ue->ue_security_capability.nr_ia &
                (0x80 >> amf_self()->integrity_order[i])) {
            return amf_self()->integrity_order[i];
        }
    }

    return 0;
}

uint8_t amf_selected_enc_algorithm(amf_ue_t *amf_ue)
{
    int i;

    ogs_assert(amf_ue);

    for (i = 0; i < amf_self()->num_of_ciphering_order; i++) {
        if (amf_ue->ue_security_capability.nr_ea &
                (0x80 >> amf_self()->ciphering_order[i])) {
            return amf_self()->ciphering_order[i];
        }
    }

    return 0;
}

/*
 * Save the sensitive (partial) context fields
 * from the UE context into the memento
 */
void amf_ue_save_memento(amf_ue_t *amf_ue, amf_ue_memento_t *memento)
{
    ogs_assert(amf_ue);
    ogs_assert(memento);

    memcpy(&memento->ue_security_capability, &amf_ue->ue_security_capability,
           sizeof(memento->ue_security_capability));
    memcpy(&memento->ue_network_capability, &amf_ue->ue_network_capability,
           sizeof(memento->ue_network_capability));
    memcpy(memento->rand, amf_ue->rand, OGS_RAND_LEN);
    memcpy(memento->autn, amf_ue->autn, OGS_AUTN_LEN);
    memcpy(memento->xres_star, amf_ue->xres_star, OGS_MAX_RES_LEN);
    memcpy(memento->abba, amf_ue->abba, OGS_NAS_MAX_ABBA_LEN);
    memento->abba_len = amf_ue->abba_len;
    memcpy(memento->hxres_star, amf_ue->hxres_star, OGS_MAX_RES_LEN);
    memcpy(memento->kamf, amf_ue->kamf, OGS_SHA256_DIGEST_SIZE);
    memcpy(memento->knas_int, amf_ue->knas_int, OGS_SHA256_DIGEST_SIZE/2);
    memcpy(memento->knas_enc, amf_ue->knas_enc, OGS_SHA256_DIGEST_SIZE/2);
    memento->dl_count = amf_ue->dl_count;
    memento->ul_count = amf_ue->ul_count.i32;
    memcpy(memento->kgnb, amf_ue->kgnb, OGS_SHA256_DIGEST_SIZE);
    memcpy(memento->nh, amf_ue->nh, OGS_SHA256_DIGEST_SIZE);
    memento->selected_enc_algorithm = amf_ue->selected_enc_algorithm;
    memento->selected_int_algorithm = amf_ue->selected_int_algorithm;
}

/* Restore the sensitive context fields into the UE context */
void amf_ue_restore_memento(amf_ue_t *amf_ue, const amf_ue_memento_t *memento)
{
    ogs_assert(amf_ue);
    ogs_assert(memento);

    memcpy(&amf_ue->ue_security_capability, &memento->ue_security_capability,
           sizeof(amf_ue->ue_security_capability));
    memcpy(&amf_ue->ue_network_capability, &memento->ue_network_capability,
           sizeof(amf_ue->ue_network_capability));
    memcpy(amf_ue->rand, memento->rand, OGS_RAND_LEN);
    memcpy(amf_ue->autn, memento->autn, OGS_AUTN_LEN);
    memcpy(amf_ue->xres_star, memento->xres_star, OGS_MAX_RES_LEN);
    memcpy(amf_ue->abba, memento->abba, OGS_NAS_MAX_ABBA_LEN);
    amf_ue->abba_len = memento->abba_len;
    memcpy(amf_ue->hxres_star, memento->hxres_star, OGS_MAX_RES_LEN);
    memcpy(amf_ue->kamf, memento->kamf, OGS_SHA256_DIGEST_SIZE);
    memcpy(amf_ue->knas_int, memento->knas_int, OGS_SHA256_DIGEST_SIZE/2);
    memcpy(amf_ue->knas_enc, memento->knas_enc, OGS_SHA256_DIGEST_SIZE/2);
    amf_ue->dl_count = memento->dl_count;
    amf_ue->ul_count.i32 = memento->ul_count;
    memcpy(amf_ue->kgnb, memento->kgnb, OGS_SHA256_DIGEST_SIZE);
    memcpy(amf_ue->nh, memento->nh, OGS_SHA256_DIGEST_SIZE);
    amf_ue->selected_enc_algorithm = memento->selected_enc_algorithm;
    amf_ue->selected_int_algorithm = memento->selected_int_algorithm;
}

void amf_clear_subscribed_info(amf_ue_t *amf_ue)
{
    int i, j;

    ogs_assert(amf_ue);

    ogs_assert(amf_ue->num_of_slice <= OGS_MAX_NUM_OF_SLICE);
    for (i = 0; i < amf_ue->num_of_slice; i++) {
        ogs_assert(amf_ue->slice[i].num_of_session <= OGS_MAX_NUM_OF_SESS);
        for (j = 0; j < amf_ue->slice[i].num_of_session; j++) {
            ogs_assert(amf_ue->slice[i].session[j].name);
            ogs_free(amf_ue->slice[i].session[j].name);
        }
        amf_ue->slice[i].num_of_session = 0;
    }
    amf_ue->num_of_slice = 0;
}

static void stats_add_ran_ue(void)
{
    amf_metrics_inst_global_inc(AMF_METR_GLOB_GAUGE_RAN_UE);
    num_of_ran_ue = num_of_ran_ue + 1;
    ogs_info("[Added] Number of gNB-UEs is now %d", num_of_ran_ue);
}

static void stats_remove_ran_ue(void)
{
    amf_metrics_inst_global_dec(AMF_METR_GLOB_GAUGE_RAN_UE);
    num_of_ran_ue = num_of_ran_ue - 1;
    ogs_info("[Removed] Number of gNB-UEs is now %d", num_of_ran_ue);
}

int amf_instance_get_load(void)
{
    return (((ogs_pool_size(&ran_ue_pool) -
            ogs_pool_avail(&ran_ue_pool)) * 100) /
            ogs_pool_size(&ran_ue_pool));
}

static void stats_add_amf_session(void)
{
    amf_metrics_inst_global_inc(AMF_METR_GLOB_GAUGE_AMF_SESS);
    num_of_amf_sess = num_of_amf_sess + 1;
    ogs_info("[Added] Number of AMF-Sessions is now %d", num_of_amf_sess);
}

static void stats_remove_amf_session(void)
{
    amf_metrics_inst_global_dec(AMF_METR_GLOB_GAUGE_AMF_SESS);
    num_of_amf_sess = num_of_amf_sess - 1;
    ogs_info("[Removed] Number of AMF-Sessions is now %d", num_of_amf_sess);
}

/*
 * Issues #2482
 *
 * Changed to that registration can be accepted only
 * when the UE slice is available in the RAN slice.
 *
 * TS23.502
 * 4.2.2 Registration Management procedures
 * 4.2.2.2 Registration procedures
 * 4.2.2.2.2 General Registration
 *
 * 21. ...
 * If the Requested NSSAI does not include S-NSSAIs which map to S-NSSAIs
 * of the HPLMN subject to Network Slice-Specific Authentication and
 * Authorization and the AMF determines that no S-NSSAI can be provided
 * in the Allowed NSSAI for the UE in the current UE's Tracking Area and
 * if no default S-NSSAI(s) not yet involved in the current UE Registration
 * procedure could be further considered, the AMF shall reject the UE
 * Registration and shall include in the rejection message the list
 * of Rejected S-NSSAIs, each of them with the appropriate rejection
 * cause value.
 */
static bool gnb_ta_is_supported(
        amf_gnb_t *gnb,
        ogs_plmn_id_t *plmn_id, ogs_uint24_t tac, ogs_s_nssai_t *s_nssai)
{
    int i, j, k;

    ogs_assert(gnb);
    ogs_assert(plmn_id);

    for (i = 0; i < gnb->num_of_supported_ta_list; i++) {
        if (gnb->supported_ta_list[i].tac.v != tac.v)
            continue;

        for (j = 0; j < gnb->supported_ta_list[i].num_of_bplmn_list; j++) {
            if (memcmp(&gnb->supported_ta_list[i].bplmn_list[j].plmn_id,
                        plmn_id, sizeof(*plmn_id)) != 0)
                continue;

            for (k = 0;
                    k < gnb->supported_ta_list[i].bplmn_list[j].num_of_s_nssai;
                    k++) {
                if (gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sst ==
                        s_nssai->sst &&
                    gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sd.v ==
                        s_nssai->sd.v)
                    return true;
            }
        }
    }

    return false;
}


bool amf_update_allowed_nssai(amf_ue_t *amf_ue)
{
    int i, j, k;
    amf_gnb_t *gnb = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
    if (!ran_ue) {
        ogs_error("[%s] RAN-NG Context has already been removed",
                    amf_ue->supi);
        return false;
    }

    gnb = amf_gnb_find_by_id(ran_ue->gnb_id);
    if (!gnb) {
        ogs_error("[%d] gNB has already been removed", ran_ue->gnb_id);
        return false;
    }

    /*
     * TS23.501
     *
     * 5.15.4 UE NSSAI configuration and NSSAI storage aspects
     * 5.15.4.1 General
     * 5.15.4.1.1 UE Network Slice configuration
     *
     * S-NSSAIs that the UE provides in the Requested NSSAI which are neither
     * in the Allowed NSSAI nor provided as a rejected S-NSSAI, shall, by the
     * UE, not be regarded as rejected, i.e. the UE may request to register
     * these S-NSSAIs again next time the UE sends a Requested NSSAI
     *
     * 5.15.5 Detailed Operation Overview
     *
     * 5.15.5.2 Selection of a Serving AMF supporting the Network Slices
     * 5.15.5.2.1 Registration to a set of Network Slices
     *
     * AMF checks whether it can serve all the S-NSSAI(s) from
     * the Requested NSSAI present in the Subscribed S-NSSAIs
     * (potentially using configuration for mapping S-NSSAI values
     * between HPLMN and Serving PLMN), or all the S-NSSAI(s) marked
     * as default in the Subscribed S-NSSAIs in the case that
     * no Requested NSSAI was provided or none of the S-NSSAIs
     * in the Requested NSSAI are permitted,
     * i.e. do not match any of the Subscribed S-NSSAIs or not available
     * at the current UE's Tracking Area (see clause 5.15.3).
     *
     *
     * TS24.501
     *
     * 4.6.2 Mobility management aspects
     * 4.6.2.1 General
     *
     * The UE in NB-N1 mode does not include the requested NSSAI during
     * the registration procedure if the 5GS registration type IE indicates
     * "mobility registration updating", procedure is not initiated
     * to change the slice(s) that the UE is currently registered to,
     * and the UE is still in the current registration area. The AMF does not
     * include the allowed NSSAI during a registration procedure with the 5GS
     * registration type IE indicating "mobility registration updating" except
     * if the allowed NSSAI has changed for the UE. The UE considers
     * the last received allowed NSSAI as valid until the UE receives
     * a new allowed NSSAI.
     *
     * 5.5.1.2.4 Initial registration accepted by the network
     *
     * The AMF shall include the allowed NSSAI for the current PLMN
     * and shall include the mapped S-NSSAI(s) for the allowed NSSAI
     * contained in the requested NSSAI from the UE if available,
     * in the REGISTRATION ACCEPT message if the UE included
     * the requested NSSAI in the REGISTRATION REQUEST message
     * and the AMF allows one or more S-NSSAIs in the requested NSSAI.
     *
     * 8.2.7.5 Allowed NSSAI
     *
     * This IE shall be included:
     * a) if:
     *   1) one or more S-NSSAIs in the requested NSSAI of
     *      the REGISTRATION REQUEST message are allowed by the AMF
     *      for a network not supporting NSSAA;
     *   2) one or more S-NSSAIs in the requested NSSAI of
     *      the REGISTRATION REQUEST message are not subject
     *      to network slice-specific authentication and authorization
     *      and are allowed by the AMF; or
     *   3) the network slice-specific authentication and authorization
     *      has been successfully performed for one or more S-NSSAIs in
     *      the requested NSSAI of the REGISTRATION REQUEST message; or
     * b) if:
     *   1) the requested NSSAI was not included in the REGISTRATION
     *      REQUEST message or none of the requested NSSAI are allowed;
     *   2) the network not supporting NSSAA has one or more subscribed
     *      S-NSSAIs marked as default that are available; or
     *   3) the network has one or more subscribed S-NSSAIs marked
     *      as default which are not subject to network slice-specific
     *      authentication and authorization that are available.
     */

    amf_ue->allowed_nssai.num_of_s_nssai = 0;
    amf_ue->rejected_nssai.num_of_s_nssai = 0;

    if (amf_ue->requested_nssai.num_of_s_nssai) {
        for (i = 0; i < amf_ue->requested_nssai.num_of_s_nssai; i++) {
            ogs_slice_data_t *slice = NULL;
            ogs_nas_s_nssai_ie_t *requested =
                    &amf_ue->requested_nssai.s_nssai[i];
            ogs_nas_s_nssai_ie_t *allowed =
                    &amf_ue->allowed_nssai.
                        s_nssai[amf_ue->allowed_nssai.num_of_s_nssai];
            ogs_nas_rejected_s_nssai_t *rejected =
                    &amf_ue->rejected_nssai.
                        s_nssai[amf_ue->rejected_nssai.num_of_s_nssai];
            bool ta_supported = false;

            ogs_assert(amf_ue->num_of_slice);
            slice = ogs_slice_find_by_s_nssai(
                    amf_ue->slice, amf_ue->num_of_slice,
                    (ogs_s_nssai_t *)requested);
            if (slice)
                ta_supported = gnb_ta_is_supported(gnb,
                        &amf_ue->nr_tai.plmn_id, amf_ue->nr_tai.tac,
                        &slice->s_nssai);

            if (ta_supported == true) {

                allowed->sst = requested->sst;
                allowed->sd.v = requested->sd.v;
                allowed->mapped_hplmn_sst_presence =
                        requested->mapped_hplmn_sst_presence;
                allowed->mapped_hplmn_sst = requested->mapped_hplmn_sst;
                allowed->mapped_hplmn_sd.v = requested->mapped_hplmn_sd.v;

                amf_ue->allowed_nssai.num_of_s_nssai++;

            } else {
                rejected->sst = requested->sst;
                rejected->sd.v = requested->sd.v;

                if (rejected->sd.v != OGS_S_NSSAI_NO_SD_VALUE)
                    rejected->length_of_rejected_s_nssai = 4;
                else
                    rejected->length_of_rejected_s_nssai = 1;

                rejected->cause_value =
                    OGS_NAS_REJECTED_S_NSSAI_NOT_AVIALABLE_IN_PLMN;

                amf_ue->rejected_nssai.num_of_s_nssai++;
            }
        }
    }

    if (!amf_ue->allowed_nssai.num_of_s_nssai) {
        for (i = 0; i < amf_ue->num_of_slice; i++) {
            ogs_slice_data_t *slice = &amf_ue->slice[i];
            ogs_nas_s_nssai_ie_t *allowed =
                &amf_ue->allowed_nssai.
                    s_nssai[amf_ue->allowed_nssai.num_of_s_nssai];

            if (slice->default_indicator == true &&
                gnb_ta_is_supported(gnb,
                    &amf_ue->nr_tai.plmn_id, amf_ue->nr_tai.tac,
                    &slice->s_nssai) == true) {

                allowed->sst = slice->s_nssai.sst;
                allowed->sd.v = slice->s_nssai.sd.v;
                allowed->mapped_hplmn_sst_presence = false;
                allowed->mapped_hplmn_sst = 0;
                allowed->mapped_hplmn_sd.v = OGS_S_NSSAI_NO_SD_VALUE;

                amf_ue->allowed_nssai.num_of_s_nssai++;

            }
        }
    }

    if (!amf_ue->allowed_nssai.num_of_s_nssai) {
        ogs_error("No Allowed-NSSAI");
        ogs_error("    Number of Subscribed S-NSSAI [%d]",
                amf_ue->num_of_slice);
        for (i = 0; i < amf_ue->num_of_slice; i++) {
            ogs_slice_data_t *slice = &amf_ue->slice[i];
            if (slice->default_indicator == true) {
                ogs_error(
                    "        Default S_NSSAI[SST:%d SD:0x%x]",
                    slice->s_nssai.sst, slice->s_nssai.sd.v);
            } else {
                ogs_error(
                    "        S_NSSAI[SST:%d SD:0x%x]",
                    slice->s_nssai.sst, slice->s_nssai.sd.v);
            }
        }
        ogs_error("    Number of Requested NSSAI [%d]",
                amf_ue->requested_nssai.num_of_s_nssai);
        for (i = 0; i < amf_ue->requested_nssai.
                num_of_s_nssai; i++) {
            ogs_error("        PLMN_ID[MCC:%d MNC:%d]",
                    ogs_plmn_id_mcc(&amf_ue->nr_tai.plmn_id),
                    ogs_plmn_id_mnc(&amf_ue->nr_tai.plmn_id));
            ogs_error("        S_NSSAI[SST:%d SD:0x%x]",
                    amf_ue->requested_nssai.s_nssai[i].sst,
                    amf_ue->requested_nssai.s_nssai[i].sd.v);
        }

        ogs_error("    (gNB) Number of TA List [%d]", gnb->num_of_supported_ta_list);
        for (i = 0; i < gnb->num_of_supported_ta_list; i++) {
            ogs_error("        TAC:%d", gnb->supported_ta_list[i].tac.v);
            ogs_error("        Number of BPLMN List [%d]",
                    gnb->supported_ta_list[i].num_of_bplmn_list);
            for (j = 0; j < gnb->supported_ta_list[i].num_of_bplmn_list; j++) {
                ogs_plmn_id_t *plmn_id =
                    &gnb->supported_ta_list[i].bplmn_list[j].plmn_id;
                ogs_error("        PLMN_ID[MCC:%d MNC:%d]",
                        ogs_plmn_id_mcc(plmn_id), ogs_plmn_id_mnc(plmn_id));
                ogs_error("        Number of S_NSSAI [%d]",
                        gnb->supported_ta_list[i].bplmn_list[j].num_of_s_nssai);
                for (k = 0; k <
                        gnb->supported_ta_list[i].bplmn_list[j].num_of_s_nssai;
                        k++) {
                    ogs_s_nssai_t *s_nssai =
                        &gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k];
                    ogs_error("        S_NSSAI[SST:%d SD:0x%x]",
                            s_nssai->sst, s_nssai->sd.v);
                }
            }
        }

        return false;
    }

    return true;
}

bool amf_ue_is_rat_restricted(amf_ue_t *amf_ue)
{
    OpenAPI_lnode_t *node = NULL;
    OpenAPI_rat_type_e rat;

    ogs_assert(amf_ue);

    rat = amf_ue_rat_type(amf_ue);

    OpenAPI_list_for_each(amf_ue->rat_restrictions, node) {
        if (node->data == (void *)rat) {
            return true;
        }
    }
    return false;
}

void amf_ue_save_to_release_session_list(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;

    OpenAPI_list_clear(amf_ue->to_release_session_list);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        bool supported_s_nssai = false;
        int i;
        for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
            int j;
            for (j = 0; j < amf_self()->plmn_support[i].num_of_s_nssai; j++) {
                if (memcmp(&sess->s_nssai,
                        &amf_self()->plmn_support[i].s_nssai[j],
                        sizeof(ogs_s_nssai_t)) == 0) {
                    supported_s_nssai = true;
                    break;
                }
            }
            if (supported_s_nssai)
                break;
        }
        if (!supported_s_nssai) {
            double *psi = ogs_calloc(1, sizeof(*psi));
            ogs_assert(psi);
            *psi = (double)sess->psi;
            OpenAPI_list_add(amf_ue->to_release_session_list, psi);
        }
    }
}
