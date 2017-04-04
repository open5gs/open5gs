#ifndef __3GPP_DEFS_H__
#define __3GPP_DEFS_H__

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __3GPP_DEFS_H__ */
