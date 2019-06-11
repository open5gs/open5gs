#ifndef __PGW_SM_H__
#define __PGW_SM_H__

#include "pgw-event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void pgw_state_initial(ogs_fsm_t *s, pgw_event_t *e);
void pgw_state_final(ogs_fsm_t *s, pgw_event_t *e);
void pgw_state_operational(ogs_fsm_t *s, pgw_event_t *e);
void pgw_state_exception(ogs_fsm_t *s, pgw_event_t *e);

#define pgw_sm_debug(__pe) \
    ogs_debug("%s(): %s", __func__, pgw_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__PGW_SM_H__ */
