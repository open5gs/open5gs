#ifndef __3GPP_TYPES_H__
#define __3GPP_TYPES_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _plmn_id_t {
    c_uint16_t      mcc;
    c_uint16_t      mnc;
    c_uint16_t      mnc_len;
} plmn_id_t;

typedef struct _nas_plmn_t {
ED2(c_uint8_t mcc_digit2:4;,
    c_uint8_t mcc_digit1:4;)
ED2(c_uint8_t mnc_digit3:4;,
    c_uint8_t mcc_digit3:4;)
ED2(c_uint8_t mnc_digit2:4;,
    c_uint8_t mnc_digit1:4;)
} __attribute__ ((packed)) nas_plmn_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__3GPP_TYPES_H__ */
