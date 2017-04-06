#ifndef __3GPP_COMMON_H__
#define __3GPP_COMMON_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_UE_PER_ENB      128
#define MAX_NUM_OF_ENB      128
#define MAX_NUM_OF_UE       (MAX_NUM_OF_ENB * MAX_UE_PER_ENB)

#define IPV6_LEN            16

#define MAX_SDU_LEN         2048

#define PLMN_ID_LEN         3
#define MAX_IMSI_LEN        15

#define RAND_LEN            16
#define AUTN_LEN            16
#define MAX_RES_LEN         16

typedef struct _plmn_id_t {
ED2(c_uint8_t mcc2:4;,
    c_uint8_t mcc1:4;)
ED2(c_uint8_t mnc1:4;,
    c_uint8_t mcc3:4;)
ED2(c_uint8_t mnc3:4;,
    c_uint8_t mnc2:4;)
} __attribute__ ((packed)) plmn_id_t;

CORE_DECLARE(c_uint16_t) plmn_id_mcc(plmn_id_t *plmn_id);
CORE_DECLARE(c_uint16_t) plmn_id_mnc(plmn_id_t *plmn_id);
CORE_DECLARE(c_uint16_t) plmn_id_mnc_len(plmn_id_t *plmn_id);

CORE_DECLARE(void *) plmn_id_build(plmn_id_t *plmn_id, 
        c_uint16_t mcc, c_uint16_t mnc, c_uint16_t mnc_len);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __3GPP_COMMON_H__ */
