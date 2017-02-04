#ifndef _S1AP_CODECS_H__
#define _S1AP_CODECS_H__

#include "core.h"

#include "s1ap_ies_defs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(int) s1ap_decode_pdu(s1ap_message *message, 
        uint8_t *buffer, uint32_t len);

CORE_DECLARE(int) s1ap_encode_pdu(s1ap_message *message, 
        uint8_t **buffer, uint32_t *len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

