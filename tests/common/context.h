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

#if !defined(OGS_TEST_INSIDE) && !defined(OGS_TEST_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef TEST_COMMON_CONTEXT_H
#define TEST_COMMON_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_OF_SERVED_GUAMI     8

typedef struct test_context_s {
    /* Served GUMME */
    uint8_t num_of_served_guami;
    struct {
        ogs_plmn_id_t plmn_id;
        ogs_amf_id_t amf_id;
    } served_guami[MAX_NUM_OF_SERVED_GUAMI];

    /* Served TAI */
    uint8_t num_of_served_tai;
    struct {
        ogs_5gs_tai0_list_t list0;
        ogs_5gs_tai2_list_t list2;
    } served_tai[OGS_MAX_NUM_OF_SERVED_TAI];

    /* PLMN Support */
    uint8_t num_of_plmn_support;
    struct {
        ogs_plmn_id_t plmn_id;
        int num_of_s_nssai;
        ogs_s_nssai_t s_nssai[OGS_MAX_NUM_OF_S_NSSAI];
    } plmn_support[OGS_MAX_NUM_OF_PLMN];

    ogs_5gs_tai_t tai;
    ogs_nr_cgi_t nr_cgi;
} test_context_t;

typedef struct test_sess_s test_sess_t;

typedef struct test_registration_request_param_s {
    struct {
    ED8(uint8_t integrity_protected:1;,
        uint8_t ciphered:1;,
        uint8_t guti:1;,
        uint8_t gmm_capability:1;,
        uint8_t requested_nssai:1;,
        uint8_t last_visited_registered_tai:1;,
        uint8_t ue_usage_setting:1;,
        uint8_t update_type:1;)
    };
    struct {
    ED4(uint8_t uplink_data_status:1;,
        uint8_t pdu_session_status:1;,
        uint8_t allowed_pdu_session_status:1;,
        uint8_t spare2:5;)
    };
    struct {
        uint16_t pdu_session_status;
        uint16_t uplink_data_status;
        uint16_t allowed_pdu_session_status;
    } psimask;
} __attribute__ ((packed)) test_registration_request_param_t;

typedef struct test_service_request_param_s {
    union {
        struct {
        ED6(uint8_t integrity_protected:1;,
            uint8_t ciphered:1;,
            uint8_t pdu_session_status:1;,
            uint8_t uplink_data_status:1;,
            uint8_t allowed_pdu_session_status:1;,
            uint8_t reserved:3;)
        };
        uint8_t value;
    };
    uint8_t service_type;
    struct {
        uint16_t pdu_session_status;
        uint16_t uplink_data_status;
        uint16_t allowed_pdu_session_status;
    } psimask;
} __attribute__ ((packed)) test_service_request_param_t;

typedef struct test_ul_nas_transport_param_s {
    union {
        struct {
        ED4(uint8_t request_type:4;,
            uint8_t dnn:1;,
            uint8_t s_nssai:1;,
            uint8_t reserved:2;)
        };
        uint8_t value;
    };
} __attribute__ ((packed)) test_ul_nas_transport_param_t;

typedef struct test_ue_s {
    uint32_t ran_ue_ngap_id; /* eNB-UE-NGAP-ID received from eNB */
    uint64_t amf_ue_ngap_id; /* AMF-UE-NGAP-ID received from AMF */

    char *imsi;
    char *suci; /* TS33.501 : SUCI */
    char *supi; /* TS33.501 : SUPI */

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    ogs_nas_mobile_identity_imeisv_t mobile_identity_imeisv;
    uint16_t mobile_identity_suci_length;
    ogs_nas_5gs_guti_t nas_guti;

    uint8_t k[OGS_KEY_LEN];
    uint8_t opc[OGS_KEY_LEN];

    uint8_t rand[OGS_RAND_LEN];
    uint8_t autn[OGS_AUTN_LEN];
    uint8_t abba[OGS_NAS_MAX_ABBA_LEN];
    uint8_t abba_len;
    uint8_t kamf[OGS_SHA256_DIGEST_SIZE];

    struct {
        int access_type; /* 3GPP or Non-3GPP */

        union {
            struct {
            ED3(uint8_t tsc:1;,
                uint8_t ksi:3;,
                uint8_t value:4;)
            };
            ogs_nas_5gs_registration_type_t registration;
            ogs_nas_de_registration_type_t de_registration;

            uint8_t data;
        };

    } __attribute__ ((packed)) nas;

    uint8_t         knas_int[OGS_SHA256_DIGEST_SIZE/2];
    uint8_t         knas_enc[OGS_SHA256_DIGEST_SIZE/2];
    uint32_t        ul_count;
    union {
        struct {
        ED3(uint8_t spare;,
            uint16_t overflow;,
            uint8_t sqn;)
        } __attribute__ ((packed));
        uint32_t i32;
    } dl_count;
    uint8_t         kgnb[OGS_SHA256_DIGEST_SIZE];

    uint8_t selected_enc_algorithm;
    uint8_t selected_int_algorithm;

#define TEST_SECURITY_CONTEXT_IS_VALID(__tEST) \
    ((__tEST) && \
    ((__tEST)->security_context_available == 1) && \
     ((__tEST)->mac_failed == 0) && \
     ((__tEST)->nas.ksi != OGS_NAS_KSI_NO_KEY_IS_AVAILABLE))
#define TEST_CLEAR_SECURITY_CONTEXT(__tEST) \
    do { \
        ogs_assert((__tEST)); \
        (__tEST)->security_context_available = 0; \
        (__tEST)->mac_failed = 0; \
        (__tEST)->nas.ksi = 0; \
    } while(0)
    int             security_context_available;
    int             mac_failed;

    test_registration_request_param_t registration_request_param;
    test_service_request_param_t service_request_param;
    uint8_t         gmm_message_type; /* Last received 5GMM message type */

    uint16_t pdu_session_status;
    uint16_t pdu_session_reactivation_result;

    test_sess_t *sess;
} test_ue_t;

typedef struct test_sess_s {
    uint8_t psi;

    uint8_t pti;
    uint8_t pdu_session_type;
    char *dnn;

    ogs_ip_t ue_ip;
    ogs_ip_t upf_n3_ip;
    uint32_t upf_n3_teid;
    ogs_ip_t gnb_n3_ip;
    uint32_t gnb_n3_teid;

    uint8_t qfi;

    test_ul_nas_transport_param_t ul_nas_transport_param;

    test_ue_t *test_ue;
} test_sess_t;

void test_context_init(void);
void test_context_final(void);
test_context_t *test_self(void);

int test_context_parse_config(void);

void test_ue_set_mobile_identity(test_ue_t *test_ue,
        ogs_nas_5gs_mobile_identity_t *mobile_identity);
void test_ue_set_mobile_identity_suci(test_ue_t *test_ue,
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci,
    uint16_t mobile_identity_suci_length);
void test_ue_remove(test_ue_t *test_ue);

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_CONTEXT_H */
