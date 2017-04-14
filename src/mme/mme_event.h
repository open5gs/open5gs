#ifndef __MME_EVENT_H__
#define __MME_EVENT_H__

#include "core_event.h"
#include "core_pkbuf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S6A_CMD_UPDATE_LOCATION 316
#define S6A_CMD_AUTHENTICATION_INFORMATION 318

/* forward declaration */
typedef enum {
    MME_EVT_BASE = FSM_USER_SIG,

    MME_EVT_S1AP_ENB_MSG,
    MME_EVT_S1AP_ENB_LO_ACCEPT,
    MME_EVT_S1AP_ENB_LO_CONNREFUSED,

    MME_EVT_EMM_UE_MSG,
    MME_EVT_EMM_BEARER_LO_MODIFY_BEARER,

    MME_EVT_ESM_BEARER_MSG,
    MME_EVT_ESM_BEARER_LO_INFO_REQ,
    MME_EVT_ESM_BEARER_LO_MODIFY_BEARER,

    MME_EVT_S11_UE_MSG,
    MME_EVT_S11_TRANSACTION_T3,

    MME_EVT_EMM_UE_FROM_S6A,
    MME_EVT_EMM_UE_TO_S6A,
    MME_EVT_EMM_BEARER_FROM_S11,
    MME_EVT_EMM_UE_TO_S11,

    MME_EVT_ESM_BEARER_FROM_S11,
    MME_EVT_ESM_BEARER_TO_S11,

    MME_EVT_TOP,

} event_e;

#define mme_event_send(__ptr_e) event_send(mme_self()->queue_id, (__ptr_e))

CORE_DECLARE(char*) mme_event_get_name(event_t *e);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_EVENT_H__ */
