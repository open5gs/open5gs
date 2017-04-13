#ifndef __MME_EVENT_H__
#define __MME_EVENT_H__

#include "core_event.h"
#include "core_pkbuf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* forward declaration */
typedef struct _mme_ue_t mme_ue_t;
typedef struct _mme_bearer_t mme_bearer_t;
typedef struct _nas_esm_message_container_t nas_esm_message_container_t;
typedef struct OCTET_STRING S1ap_NAS_PDU_t;

typedef enum {
    MME_EVT_BASE = FSM_USER_SIG,

    MME_EVT_S1AP_ENB_MSG,
    MME_EVT_S1AP_ENB_LO_ACCEPT,
    MME_EVT_S1AP_ENB_LO_CONNREFUSED,

    MME_EVT_EMM_UE_MSG,
    MME_EVT_EMM_UE_LO_AUTH_REQ,
    MME_EVT_EMM_UE_LO_LOCATION_UPDATE,
    MME_EVT_EMM_BEARER_LO_CREATE_SESSION,

    MME_EVT_ESM_BEARER_MSG,
    MME_EVT_ESM_BEARER_LO_INFO_REQ,

    MME_EVT_S11_TRANSACTION_T3,

    /* FIXME : remove below */
    EVT_LO_MME_S1AP_ACCEPT,
    EVT_LO_MME_S1AP_CONNREFUSED,
    EVT_LO_MME_EMM_AUTH_REQ,
    EVT_LO_MME_EMM_LOCATION_UPDATE,

    EVT_TM_MME_S11_T3,

    EVT_MSG_MME_S1AP,
    EVT_MSG_MME_EMM,
    EVT_MSG_MME_ESM,
    EVT_MSG_MME_S11,
    
    MME_EVT_TOP,

} event_e;

#define mme_event_send(__ptr_e) event_send(mme_self()->queue_id, (__ptr_e))

CORE_DECLARE(char*) mme_event_get_name(event_t *e);

CORE_DECLARE(void) mme_event_s1ap_to_nas(mme_ue_t *ue, S1ap_NAS_PDU_t *nasPdu);
CORE_DECLARE(void) mme_event_emm_to_esm(mme_bearer_t *bearer, 
                        nas_esm_message_container_t *bearer_message_container);
CORE_DECLARE(void) mme_event_nas_to_s1ap(mme_ue_t *ue, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_EVENT_H__ */
