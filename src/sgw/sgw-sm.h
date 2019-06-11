#ifndef __SGW_SM_H__
#define __SGW_SM_H__

#include "sgw-event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void sgw_state_initial(ogs_fsm_t *s, sgw_event_t *e);
void sgw_state_final(ogs_fsm_t *s, sgw_event_t *e);
void sgw_state_operational(ogs_fsm_t *s, sgw_event_t *e);
void sgw_state_exception(ogs_fsm_t *s, sgw_event_t *e);

#define sgw_sm_debug(__pe) \
    ogs_debug("%s(): %s\n", __func__, sgw_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__SGW_SM_H__ */
