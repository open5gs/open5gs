#ifndef __PGW_EVENT_H__
#define __PGW_EVENT_H__

#include "core_event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    PGW_EVT_BASE = FSM_USER_SIG,

    EVT_TM_PGW_T3,

    EVT_MSG_PGW_S5C,
    
    PGW_EVT_TOP,

} event_e;

#define pgw_event_send(__ptr_e) event_send(pgw_self()->queue_id, (__ptr_e))

CORE_DECLARE(char*) pgw_event_get_name(event_t *e);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_EVENT_H__ */
