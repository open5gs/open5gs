#ifndef __HSS_CONTEXT_H__
#define __HSS_CONTEXT_H__

#include "fd/s6a/s6a_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HSS_KEY_LEN                 16
#define HSS_AMF_LEN                 2

#define HSS_MAX_SQN                 0x7ffffffffff

extern int __hss_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __hss_log_domain

typedef struct _hss_db_auth_info_t {
    uint8_t       k[HSS_KEY_LEN];
    uint8_t       use_opc;
    uint8_t       opc[HSS_KEY_LEN];
    uint8_t       op[HSS_KEY_LEN];
    uint8_t       amf[HSS_AMF_LEN];
    uint8_t       rand[RAND_LEN];
    uint64_t      sqn;
} hss_db_auth_info_t;

typedef struct _fd_config_t fd_config_t;
typedef struct _hss_context_t {
    const char      *fd_conf_path;      /* HSS freeDiameter conf path */
    fd_config_t     *fd_config;         /* HSS freeDiameter config */

    void            *subscriberCollection;
    ogs_thread_mutex_t db_lock;
} hss_context_t;

int hss_context_init(void);
int hss_context_final(void);
hss_context_t* hss_self(void);

int hss_context_parse_config(void);

int hss_db_init(void);
int hss_db_final(void);

int hss_db_auth_info(char *imsi_bcd, hss_db_auth_info_t *auth_info);
int hss_db_update_rand_and_sqn(char *imsi_bcd, uint8_t *rand, uint64_t sqn);
int hss_db_increment_sqn(char *imsi_bcd);

int hss_db_subscription_data(
    char *imsi_bcd, s6a_subscription_data_t *subscription_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HSS_CONTEXT_H__ */
