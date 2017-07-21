#ifndef __HSS_DB_H__
#define __HSS_DB_H__

#include "core_errno.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HSS_DB_KEY_LEN                 16
#define HSS_DB_AMF_LEN                 2

typedef struct _hss_db_auth_info_t {
    c_uint8_t       k[HSS_DB_KEY_LEN];
    c_uint8_t       op[HSS_DB_KEY_LEN];
    c_uint8_t       amf[HSS_DB_AMF_LEN];
    c_uint8_t       rand[RAND_LEN];
    c_uint64_t      sqn;
} hss_db_auth_info_t;

typedef struct _hss_db_subscription_data_t {
    c_uint32_t      access_restriction_data;
    c_uint32_t      subscriber_status;
    c_uint32_t      network_access_mode;

    c_uint32_t      max_bandwidth_ul; /* Kbps */
    c_uint32_t      max_bandwidth_dl; /* Kbps */

    c_uint32_t      subscribed_rau_tau_timer; /* minutes */

    pdn_t           pdn[MAX_NUM_OF_PDN];
    int             num_of_pdn;
} hss_db_subscription_data_t;

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

#endif /* __HSS_DB_H__ */
