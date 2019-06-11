#ifndef __NAS_CONV_H__
#define __NAS_CONV_H__

#include "nas/nas-message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nas_imsi_to_buffer(
    nas_mobile_identity_imsi_t *imsi, uint8_t imsi_len, 
    uint8_t *buf, uint8_t *buf_len);

void nas_imsi_to_bcd(
    nas_mobile_identity_imsi_t *imsi, uint8_t imsi_len, char *bcd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_CONV_H__ */

