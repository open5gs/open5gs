#ifndef __MME_EVENT_H__
#define __MME_EVENT_H__

#include "core_event.h"
#include "core_pkbuf.h"

#include "s1ap_message.h"

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    MME_EVT_BASE = FSM_USER_SIG,

    EVT_LO_ENB_S1AP_ACCEPT,
    EVT_LO_ENB_S1AP_CONNREFUSED,

    EVT_MSG_ENB_S1AP,
    EVT_MSG_UE_EMM,
    EVT_MSG_MME_S11,
    
    MME_EVT_TOP,

} event_e;

CORE_DECLARE(char*) mme_event_get_name(event_t *e);

CORE_DECLARE(void) mme_event_s1ap_to_nas(ue_ctx_t *ue, S1ap_NAS_PDU_t *nasPdu);
CORE_DECLARE(void) mme_event_nas_to_s1ap(ue_ctx_t *ue, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_EVENT_H__ */
