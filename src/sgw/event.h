#ifndef __SGW_EVENT_H__
#define __SGW_EVENT_H__

#include "core_event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    SGW_EVT_BASE = FSM_USER_SIG,

    EVT_MSG_SGW_S11,
    
    SGW_EVT_TOP,

} event_e;

CORE_DECLARE(char*) sgw_event_get_name(event_t *e);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_EVENT_H__ */
