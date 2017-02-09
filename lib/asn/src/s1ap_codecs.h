#ifndef _S1AP_CODECS_H__
#define _S1AP_CODECS_H__

#include "core_pkbuf.h"
#include "s1ap_ies_defs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S1AP_SDU_SIZE 2048

CORE_DECLARE(int) s1ap_decode_pdu(s1ap_message *message, pkbuf_t *pkbuf);
CORE_DECLARE(int) s1ap_encode_pdu(pkbuf_t **pkbuf, s1ap_message *message);
CORE_DECLARE(int) s1ap_free_pdu(s1ap_message *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

