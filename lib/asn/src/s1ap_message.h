#ifndef _S1AP_MESSAGE_H__
#define _S1AP_MESSAGE_H__

#include "core_pkbuf.h"

#include "s1ap_ies_defs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S1AP_SDU_SIZE 2048

CORE_DECLARE(int) s1ap_decode_pdu(s1ap_message *message, pkbuf_t *pkbuf);

CORE_DECLARE(int) s1ap_encode_pdu(s1ap_message *message, 
        uint8_t **buffer, uint32_t *len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

