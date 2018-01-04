#ifndef __NAS_CONV_H__
#define __NAS_CONV_H__

#include "nas/nas_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) nas_imsi_to_buffer(
    nas_mobile_identity_imsi_t *imsi, c_uint8_t imsi_len, 
    c_uint8_t *buf, c_uint8_t *buf_len);

CORE_DECLARE(void) nas_imsi_to_bcd(
    nas_mobile_identity_imsi_t *imsi, c_uint8_t imsi_len, c_int8_t *bcd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_CONV_H__ */

