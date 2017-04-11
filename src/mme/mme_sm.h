#ifndef __MME_SM_H__
#define __MME_SM_H__

#include "core_param.h"
#include "core_fsm.h"
#include "core_event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void mme_state_initial(fsm_t *s, event_t *e);
void mme_state_final(fsm_t *s, event_t *e);
void mme_state_operational(fsm_t *s, event_t *e);
void mme_state_exception(fsm_t *s, event_t *e);

void s1ap_state_initial(fsm_t *s, event_t *e);
void s1ap_state_final(fsm_t *s, event_t *e);
void s1ap_state_operational(fsm_t *s, event_t *e);
void s1ap_state_exception(fsm_t *s, event_t *e);

void emm_state_initial(fsm_t *s, event_t *e);
void emm_state_final(fsm_t *s, event_t *e);
void emm_state_operational(fsm_t *s, event_t *e);
void emm_state_exception(fsm_t *s, event_t *e);

void esm_state_initial(fsm_t *s, event_t *e);
void esm_state_final(fsm_t *s, event_t *e);
void esm_state_operational(fsm_t *s, event_t *e);
void esm_state_exception(fsm_t *s, event_t *e);

#define mme_sm_print(__pe) \
    d_print("%s(): %s\n", __func__, mme_event_get_name(__pe))

#define mme_sm_trace(__l, __pe) \
    d_trace(__l, "%s(): %s\n", __func__, mme_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__MME_SM_H__ */
