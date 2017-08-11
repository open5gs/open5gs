#ifndef __HSS_CONTEXT_H__
#define __HSS_CONTEXT_H__

#include "core_errno.h"
#include "core_mutex.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HSS_KEY_LEN                 16
#define HSS_AMF_LEN                 2

typedef struct _hss_db_auth_info_t {
    c_uint8_t       k[HSS_KEY_LEN];
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
    c_uint8_t       rand[RAND_LEN];
    c_uint64_t      sqn;
} hss_db_auth_info_t;

typedef struct _hss_db_apn_t {
    c_uint32_t      context_identifier;
    c_int8_t        service_selection[MAX_APN_LEN];
#define S6A_PDN_TYPE_IPV4                               0
#define S6A_PDN_TYPE_IPV6                               1
#define S6A_PDN_TYPE_IPV4_AND_IPV6                      2
#define S6A_PDN_TYPE_IPV4_OR_IPV6                       3
    c_int8_t        pdn_type;
    paa_t           paa;

    qos_t           qos;
} hss_db_apn_t;

typedef struct _hss_db_subscription_data_t {
#define HSS_ACCESS_RESTRICTION_UTRAN_NOT_ALLOWED                    (1)
#define HSS_ACCESS_RESTRICTION_GERAN_NOT_ALLOWED                    (1<<1)
#define HSS_ACCESS_RESTRICTION_GAN_NOT_ALLOWED                      (1<<2)
#define HSS_ACCESS_RESTRICTION_I_HSPA_EVOLUTION_NOT_ALLOWED         (1<<3)
#define HSS_ACCESS_RESTRICTION_WB_E_UTRAN_NOT_ALLOWED               (1<<4)
#define HSS_ACCESS_RESTRICTION_HO_TO_NON_3GPP_ACCESS_NOT_ALLOWED    (1<<5)
#define HSS_ACCESS_RESTRICTION_NB_IOT_NOT_ALLOWED                   (1<<6)
    c_uint32_t      access_restriction_data;
#define HSS_SUBSCRIBER_STATUS_SERVICE_GRANTED               0
#define HSS_SUBSCRIBER_STATUS_OPERATOR_DETERMINED_BARRING   1 
    c_uint32_t      subscriber_status;
#define HSS_NETWORK_ACCESS_MODE_PACKET_AND_CIRCUIT          0
#define HSS_NETWORK_ACCESS_MODE_RESERVED                    1
#define HSS_NETWORK_ACCESS_MODE_ONLY_PACKET                 2
    c_uint32_t      network_access_mode;

    c_uint32_t      max_bandwidth_ul; /* Kbps */
    c_uint32_t      max_bandwidth_dl; /* Kbps */

    c_uint32_t      subscribed_rau_tau_timer; /* minutes */

    hss_db_apn_t    apn[MAX_NUM_OF_PDN];
    int             num_of_apn;
} hss_db_subscription_data_t;

typedef struct _hss_context_t {
    char*           fd_conf_path;       /* HSS freeDiameter conf path */
    char*           hss_s6a_addr;       /* HSS S6A local address (STIRNG)*/
    c_uint16_t      hss_s6a_port;       /* HSS S6A local port */
    c_uint16_t      hss_s6a_tls_port;   /* HSS S6A local TLS port */
    char*           mme_s6a_addr;       /* MME S6A address (STIRNG)*/
    c_uint16_t      mme_s6a_port;       /* MME S6A port */
    c_uint16_t      mme_s6a_tls_port;   /* MME S6A TLS port */

    void *subscriberCollection;
    mutex_id db_lock;
} hss_context_t;

CORE_DECLARE(status_t) hss_context_init(void);
CORE_DECLARE(status_t) hss_context_final(void);
CORE_DECLARE(hss_context_t*) hss_self(void);

CORE_DECLARE(status_t) hss_context_parse_config(void);
CORE_DECLARE(status_t) hss_context_setup_trace_module(void);

CORE_DECLARE(status_t) hss_db_init(void);
CORE_DECLARE(status_t) hss_db_final(void);

CORE_DECLARE(status_t) hss_db_auth_info(
        char *imsi_bcd, hss_db_auth_info_t *auth_info);
CORE_DECLARE(status_t) hss_db_update_rand_and_sqn(
    char *imsi_bcd, c_uint8_t *rand, c_uint64_t sqn);
CORE_DECLARE(status_t) hss_db_increment_sqn(char *imsi_bcd);

CORE_DECLARE(status_t) hss_db_subscription_data(
    char *imsi_bcd, hss_db_subscription_data_t *subscription_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HSS_CONTEXT_H__ */
