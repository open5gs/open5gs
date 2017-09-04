#ifndef __SGW_EVENT_H__
#define __SGW_EVENT_H__

#include "core_event.h"
#include "core_fsm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    SGW_EVT_BASE = FSM_USER_SIG,

    SGW_EVT_S11_MESSAGE,
    SGW_EVT_S5C_MESSAGE,

    SGW_EVT_T3_RESPONSE,
    SGW_EVT_T3_HOLDING,

    SGW_EVT_LO_DLDATA_NOTI,

    SGW_EVT_TOP,

} event_e;

#define sgw_event_send(__ptr_e) event_send(sgw_self()->queue_id, (__ptr_e))

CORE_DECLARE(char*) sgw_event_get_name(event_t *e);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_EVENT_H__ */
