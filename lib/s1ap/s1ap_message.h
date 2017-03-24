#ifndef _S1AP_MESSAGE_H__
#define _S1AP_MESSAGE_H__

#include "core_pkbuf.h"
#include "s1ap_ies_defs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(int) s1ap_decode_pdu(s1ap_message_t *message, pkbuf_t *pkbuf);
CORE_DECLARE(int) s1ap_encode_pdu(pkbuf_t **pkbuf, s1ap_message_t *message);
CORE_DECLARE(int) s1ap_free_pdu(s1ap_message_t *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

