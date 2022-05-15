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

#define TEST_MME_IPV4           "127.0.0.1"
#define TEST_ENB_IPV4           "127.0.0.5"
#define TEST_AMF_IPV4           TEST_MME_IPV4
#define TEST_GNB_IPV4           TEST_ENB_IPV4

#define TEST_PING_IPV4          "10.45.0.1"
#define TEST_PING_IPV6          "2001:db8:cafe::1"

#define MAX_NUM_OF_SERVED_GUAMI     8

#define TEST_MSISDN             "491725670014"
#define TEST_ADDITIONAL_MSISDN  "491725670015"

#define TEST_HSS_IDENTITY "hss.localdomain"
#define TEST_PCRF_IDENTITY "pcrf.localdomain"
#define TEST_SMF_IDENTITY "smf.localdomain"

typedef struct test_context_s {
    uint16_t        ngap_port;      /* Default NGAP Port */
    ogs_list_t      ngap_list;      /* AMF NGAP IPv4 Server List */
    ogs_list_t      ngap_list6;     /* AMF NGAP IPv6 Server List */
    ogs_sockaddr_t  *ngap_addr;     /* AMF NGAP IPv4 Address */
    ogs_sockaddr_t  *ngap_addr6;    /* AMF NGAP IPv6 Address */

    uint16_t        s1ap_port;      /* Default S1AP Port */
    ogs_list_t      s1ap_list;      /* MME S1AP IPv4 Server List */
    ogs_list_t      s1ap_list6;     /* MME S1AP IPv6 Server List */
    ogs_sockaddr_t  *s1ap_addr;     /* MME S1AP IPv4 Address */
    ogs_sockaddr_t  *s1ap_addr6;    /* MME S1AP IPv6 Address */

    ogs_sockaddr_t  *gnb1_addr;
    ogs_sockaddr_t  *gnb1_addr6;
    ogs_sockaddr_t  *gnb2_addr;
    ogs_sockaddr_t  *gnb2_addr6;

    uint32_t        gtpc_port;      /* SMF GTPC local port */
    ogs_list_t      gtpc_list;      /* SMF GTPC Client List */

    /* 5G PLMN Support */
    uint8_t num_of_plmn_support;
    struct {
        ogs_plmn_id_t plmn_id;
        int num_of_s_nssai;
        ogs_s_nssai_t s_nssai[OGS_MAX_NUM_OF_SLICE];
    } plmn_support[OGS_MAX_NUM_OF_PLMN];

    /* Served EPC TAI */
    uint8_t num_of_e_served_tai;
    struct {
        ogs_eps_tai0_list_t list0;
        ogs_eps_tai2_list_t list2;
    } e_served_tai[OGS_MAX_NUM_OF_SERVED_TAI];

    ogs_eps_tai_t e_tai;

    /* Served 5GC TAI */
    uint8_t num_of_nr_served_tai;
    struct {
        ogs_5gs_tai0_list_t list0;
        ogs_5gs_tai2_list_t list2;
    } nr_served_tai[OGS_MAX_NUM_OF_SERVED_TAI];

    ogs_5gs_tai_t nr_tai;
    ogs_nr_cgi_t nr_cgi;

    ogs_list_t      test_ue_list;
} test_context_t;

typedef struct test_sess_s test_sess_t;

typedef struct test_initial_ue_param_s {
    struct {
    ED8(uint8_t gummei_id:1;,
        uint8_t gummei_type:1;,
        uint8_t spare1:1;,
        uint8_t spare2:1;,
        uint8_t spare3:1;,
        uint8_t spare4:1;,
        uint8_t spare5:1;,
        uint8_t spare6:1;)
    };
    struct {
        uint16_t mme_gid;
        uint8_t mme_code;
        uint8_t type;
    } gummei;
} __attribute__ ((packed)) test_initial_ue_param_t;

typedef struct test_registration_request_param_s {
    struct {
    ED7(uint8_t spare1:1;,
        uint8_t guti:1;,
        uint8_t gmm_capability:1;,
        uint8_t requested_nssai:1;,
        uint8_t last_visited_registered_tai:1;,
        uint8_t ue_usage_setting:1;,
        uint8_t update_type:1;)
    };
    struct {
    ED5(uint8_t uplink_data_status:1;,
        uint8_t pdu_session_status:1;,
        uint8_t allowed_pdu_session_status:1;,
        uint8_t s1_ue_network_capability:1;,
        uint8_t spare2:4;)
    };
    struct {
        uint16_t pdu_session_status;
        uint16_t uplink_data_status;
        uint16_t allowed_pdu_session_status;
    } psimask;
} __attribute__ ((packed)) test_registration_request_param_t;

typedef struct test_attach_request_param_s {
    struct {
    ED7(uint8_t spare1:1;,
        uint8_t guti:1;,
        uint8_t last_visited_registered_tai:1;,
        uint8_t drx_parameter:1;,
        uint8_t location_area_identication:1;,
        uint8_t mobile_station_classmark_2:1;,
        uint8_t ue_usage_setting:1;)
    };
    struct {
    ED8(uint8_t old_guti_type:1;,
        uint8_t nri_container:1;,
        uint8_t ue_additional_security_capability:1;,
        uint8_t tmsi_status:1;,
        uint8_t ms_network_feature_support:1;,
        uint8_t ms_network_capability:1;,
        uint8_t mobile_station_classmark_3:1;,
        uint8_t supported_codecs:1;)
    };
    struct {
    ED8(uint8_t additional_update_type:1;,
        uint8_t spare2:1;,
        uint8_t spare3:1;,
        uint8_t spare4:1;,
        uint8_t spare5:1;,
        uint8_t spare6:1;,
        uint8_t spare7:1;,
        uint8_t spare8:1;)
    };
} __attribute__ ((packed)) test_attach_request_param_t;

typedef struct test_tau_request_param_s {
    struct {
    ED8(uint8_t ue_network_capability:1;,
        uint8_t eps_bearer_context_status:1;,
        uint8_t guti:1;,
        uint8_t last_visited_registered_tai:1;,
        uint8_t drx_parameter:1;,
        uint8_t location_area_identication:1;,
        uint8_t mobile_station_classmark_2:1;,
        uint8_t ue_usage_setting:1;)
    };
    struct {
    ED8(uint8_t old_guti_type:1;,
        uint8_t nri_container:1;,
        uint8_t ue_additional_security_capability:1;,
        uint8_t tmsi_status:1;,
        uint8_t ms_network_feature_support:1;,
        uint8_t ms_network_capability:1;,
        uint8_t mobile_station_classmark_3:1;,
        uint8_t supported_codecs:1;)
    };
    struct {
    ED3(uint8_t additional_update_type:1;,
        uint8_t device_properties:1;,
        uint8_t spare2:6;)
    };
} __attribute__ ((packed)) test_tau_request_param_t;

typedef struct test_service_request_param_s {
    union {
        struct {
        ED4(uint8_t pdu_session_status:1;,
            uint8_t uplink_data_status:1;,
            uint8_t allowed_pdu_session_status:1;,
            uint8_t reserved:5;)
        };
        uint8_t value;
    };
    struct {
        uint16_t pdu_session_status;
        uint16_t uplink_data_status;
        uint16_t allowed_pdu_session_status;
    } psimask;
} __attribute__ ((packed)) test_service_request_param_t;

typedef struct test_extended_service_request_param_s {
    union {
        struct {
        ED3(uint8_t csfb_response:1;,
            uint8_t eps_bearer_context_status:1;,
            uint8_t spare:6;)
        };
        uint8_t value;
    };
    struct {
        uint8_t response;
    } csfb;
} __attribute__ ((packed)) test_extended_service_request_param_t;

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

typedef struct test_pdu_session_establishment_param_s {
    union {
        struct {
        ED3(uint8_t ssc_mode:1;,
            uint8_t epco:1;,
            uint8_t reserved:6;)
        };
        uint8_t value;
    };
} __attribute__ ((packed)) test_pdu_session_establishment_param_t;

typedef struct test_pdn_connectivity_param_s {
    union {
        struct {
        ED6(uint8_t eit:1;,
            uint8_t eit_no_required:1;,
            uint8_t apn:1;,
            uint8_t pco:1;,
            uint8_t spare:1;,
            uint8_t request_type:3;)
        };
        uint8_t value;
    };
} __attribute__ ((packed)) test_pdn_connectivity_param_t;

typedef struct test_esm_information_param_s {
    union {
        struct {
        ED8(uint8_t pco:1;,
            uint8_t spare1:1;,
            uint8_t spare2:1;,
            uint8_t spare3:1;,
            uint8_t spare4:1;,
            uint8_t spare5:1;,
            uint8_t spare6:1;,
            uint8_t spare7:1;)
        };
        uint8_t value;
    };
} __attribute__ ((packed)) test_esm_information_param_t;

typedef struct test_ue_s {
    ogs_lnode_t     lnode;          /**< A node of list_t */

    uint32_t ran_ue_ngap_id; /* gNB-UE-NGAP-ID received from gNB */
    uint64_t amf_ue_ngap_id; /* AMF-UE-NGAP-ID received from AMF */
    uint32_t enb_ue_s1ap_id; /* eNB-UE-S1AP-ID received from eNB */
    uint32_t mme_ue_s1ap_id; /* MME-UE-S1AP-ID received from MME */

    char *imsi;
    char *suci; /* TS33.501 : SUCI */
    char *supi; /* TS33.501 : SUPI */

    uint8_t imsi_buf[OGS_MAX_IMSI_LEN];
    int imsi_len;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    ogs_nas_mobile_identity_imeisv_t mobile_identity_imeisv;
    bool mobile_identity_imeisv_presence;
    uint16_t mobile_identity_suci_length;
    ogs_nas_5gs_guti_t nas_5gs_guti;
    ogs_nas_eps_guti_t nas_eps_guti;
    ogs_nas_mobile_identity_imsi_t mobile_identity_imsi;

    ogs_eps_tai_t e_tai;
    ogs_e_cgi_t e_cgi;
    ogs_5gs_tai_t nr_tai;
    ogs_nr_cgi_t nr_cgi;

    uint8_t k[OGS_KEY_LEN];
    const char *k_string;
    uint8_t opc[OGS_KEY_LEN];
    const char *opc_string;

    uint8_t rand[OGS_RAND_LEN];
    uint8_t autn[OGS_AUTN_LEN];
    uint8_t abba[OGS_NAS_MAX_ABBA_LEN];
    uint8_t abba_len;
    uint8_t kamf[OGS_SHA256_DIGEST_SIZE];
    uint8_t kasme[OGS_SHA256_DIGEST_SIZE];

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
            ogs_nas_eps_attach_type_t attach;

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

    ogs_nas_ue_security_capability_t ue_security_capability;
    ogs_nas_ue_network_capability_t ue_network_capability;

    struct {
        int num_of_s_nssai;
        ogs_nas_s_nssai_ie_t s_nssai[OGS_MAX_NUM_OF_SLICE];
    } requested_nssai, allowed_nssai;

    struct {
        int num_of_s_nssai;
        ogs_nas_rejected_s_nssai_t s_nssai[OGS_MAX_NUM_OF_SLICE];
    } rejected_nssai;

    test_initial_ue_param_t initial_ue_param;

    test_registration_request_param_t registration_request_param;
    test_service_request_param_t service_request_param;
    test_extended_service_request_param_t extended_service_request_param;

    uint16_t pdu_session_status;
    uint16_t pdu_session_reactivation_result;

    test_attach_request_param_t attach_request_param;
    test_tau_request_param_t tau_request_param;

    /* 5GC: Last received message */
    S1AP_ProcedureCode_t ngap_procedure_code;
    uint8_t gmm_message_type;
    uint8_t gsm_message_type;

    /* EPC: Last received message */
    S1AP_ProcedureCode_t s1ap_procedure_code;
    uint8_t emm_message_type;
    uint8_t esm_message_type;

    test_sess_t *sess;

    ogs_list_t sess_list;
} test_ue_t;

typedef struct test_sess_s {
    ogs_lnode_t     lnode;          /**< A node of list_t */
    uint32_t        index;

    uint8_t psi;

    uint8_t pti;
    uint8_t pdu_session_type;
    union {
        char *dnn;
        char *apn;
    };

    /* RAT Type */
    uint8_t gtp_rat_type;
    OpenAPI_rat_type_e sbi_rat_type;

    ogs_ip_t ue_ip;

    ogs_ip_t upf_n3_ip;             /* UPF-N3 IPv4/IPv6 */
    uint32_t upf_n3_teid;           /* UPF-N3 TEID */

    uint32_t gnb_n3_teid;           /* gNB-N3 TEID */
    ogs_sockaddr_t *gnb_n3_addr;    /* gNB-N3 IPv4 */
    ogs_sockaddr_t *gnb_n3_addr6;   /* gNB-N3 IPv6 */

    test_ul_nas_transport_param_t ul_nas_transport_param;
    test_pdu_session_establishment_param_t pdu_session_establishment_param;
    test_pdn_connectivity_param_t pdn_connectivity_param;
    test_esm_information_param_t esm_information_param;

    struct {
        bool data_forwarding_not_possible;

        /* Indirect DL Forwarding */
        uint32_t upf_dl_teid;
        ogs_ip_t upf_dl_ip;
    } handover;

    /* ePDG */
    uint32_t epdg_s2b_c_teid;
    uint32_t smf_s2b_c_teid;
    ogs_gtp_node_t *gnode;

    ogs_list_t bearer_list;

    ogs_list_t qos_flow_to_modify_list;

    test_ue_t *test_ue;
} test_sess_t;

typedef struct test_bearer_s {
    ogs_lnode_t     lnode;          /**< A node of list_t */

    ogs_lnode_t     to_modify_node;

    uint32_t        index;

    uint8_t         qfi;            /* 5GC */
    uint8_t         ebi;            /* EPC */

    uint32_t        sgw_s1u_teid;   /* SGW-S1U TEID */
    ogs_ip_t        sgw_s1u_ip;     /* SGW-S1U IPv4/IPv6 */

    uint32_t        enb_s1u_teid;   /* eNB-S1U TEID */
    ogs_sockaddr_t  *enb_s1u_addr;  /* eNB-S1U IPv4 */
    ogs_sockaddr_t  *enb_s1u_addr6; /* eNB-S1U IPv6 */

    test_sess_t     *sess;
} test_bearer_t;

void test_context_init(void);
void test_context_final(void);
test_context_t *test_self(void);

int test_context_parse_config(void);

void test_ue_set_mobile_identity_suci(test_ue_t *test_ue,
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci,
    uint16_t mobile_identity_suci_length);

test_ue_t *test_ue_add_by_suci(
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci,
    uint16_t mobile_identity_suci_length);
void test_ue_remove(test_ue_t *test_ue);
void test_ue_remove_all(void);

test_sess_t *test_sess_add_by_dnn_and_psi(
        test_ue_t *test_ue, char *dnn, uint8_t psi);
test_sess_t *test_sess_add_by_apn(
        test_ue_t *test_ue, char *apn, uint8_t rat_type);
void test_sess_remove(test_sess_t *sess);
void test_sess_remove_all(test_ue_t *test_ue);

test_sess_t *test_sess_find_by_apn(
        test_ue_t *test_ue, char *apn, uint8_t rat_type);
test_sess_t *test_sess_find_by_psi(test_ue_t *test_ue, uint8_t psi);

test_bearer_t *test_bearer_add(test_sess_t *sess, uint8_t ebi);
test_bearer_t *test_qos_flow_add(test_sess_t *sess);
void test_bearer_remove(test_bearer_t *bearer);
void test_bearer_remove_all(test_sess_t *sess);

test_bearer_t *test_bearer_find_by_sess_ebi(test_sess_t *sess, uint8_t ebi);
test_bearer_t *test_bearer_find_by_ue_ebi(test_ue_t *test_ue, uint8_t ebi);

test_bearer_t *test_qos_flow_find_by_qfi(test_sess_t *sess, uint8_t qfi);

int test_db_insert_ue(test_ue_t *test_ue, bson_t *doc);
int test_db_remove_ue(test_ue_t *test_ue);

bson_t *test_db_new_simple(test_ue_t *test_ue);
bson_t *test_db_new_qos_flow(test_ue_t *test_ue);
bson_t *test_db_new_session(test_ue_t *test_ue);
bson_t *test_db_new_ims(test_ue_t *test_ue);
bson_t *test_db_new_slice_with_same_dnn(test_ue_t *test_ue);
bson_t *test_db_new_slice_with_different_dnn(test_ue_t *test_ue);
bson_t *test_db_new_non3gpp(test_ue_t *test_ue);

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_CONTEXT_H */
