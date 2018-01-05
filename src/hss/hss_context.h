#ifndef __HSS_CONTEXT_H__
#define __HSS_CONTEXT_H__

#include "core_errno.h"
#include "core_mutex.h"

#include "fd/s6a/s6a_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HSS_KEY_LEN                 16
#define HSS_AMF_LEN                 2

#define HSS_MAX_SQN                 0x7ffffffffff

typedef struct _hss_db_auth_info_t {
    c_uint8_t       k[HSS_KEY_LEN];
    c_uint8_t       use_opc;
    c_uint8_t       opc[HSS_KEY_LEN];
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
    c_uint8_t       rand[RAND_LEN];
    c_uint64_t      sqn;
} hss_db_auth_info_t;

typedef struct _fd_config_t fd_config_t;
typedef struct _hss_context_t {
    const char      *fd_conf_path;      /* HSS freeDiameter conf path */
    fd_config_t     *fd_config;         /* HSS freeDiameter config */

    void            *subscriberCollection;
    mutex_id        db_lock;
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
    char *imsi_bcd, s6a_subscription_data_t *subscription_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HSS_CONTEXT_H__ */
