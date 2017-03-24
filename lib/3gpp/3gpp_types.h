#ifndef __3GPP_TYPES_H__
#define __3GPP_TYPES_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _plmn_id_t {
ED2(c_uint8_t mcc2:4;,
    c_uint8_t mcc1:4;)
ED2(c_uint8_t mnc1:4;,
    c_uint8_t mcc3:4;)
ED2(c_uint8_t mnc3:4;,
    c_uint8_t mnc2:4;)
} __attribute__ ((packed)) plmn_id_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__3GPP_TYPES_H__ */
