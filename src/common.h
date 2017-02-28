#ifndef __CELLWIRE_COMMON_H__
#define __CELLWIRE_COMMON_H__

#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _plmn_id_t {
    c_uint16_t      mcc;
    c_uint16_t      mnc;
    c_uint16_t      mnc_len;
} plmn_id_t;

CORE_DECLARE(void) encode_plmn_id(c_uint8_t *buf, plmn_id_t *plmn_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__CELLWIRE_COMMON_H__ */
