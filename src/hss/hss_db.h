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

CORE_DECLARE(status_t) hss_db_init(void);
CORE_DECLARE(status_t) hss_db_final(void);

CORE_DECLARE(status_t) hss_db_auth_info(
    char *imsi_bcd, hss_db_auth_info_t *auth_info);
CORE_DECLARE(status_t) hss_db_update_rand_and_sqn(
    char *imsi_bcd, c_uint8_t *rand, c_uint64_t sqn);
CORE_DECLARE(status_t) hss_db_increment_sqn(char *imsi_bcd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HSS_DB_H__ */
