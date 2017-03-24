#ifndef __NAS_CONV_H__
#define __NAS_CONV_H__

#include "nas_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) nas_imsi_bcd_to_buffer(
    nas_mobile_identity_imsi_t *bcd, c_uint8_t bcd_len, 
    c_uint8_t *buf, c_uint8_t *buf_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_CONV_H__ */

