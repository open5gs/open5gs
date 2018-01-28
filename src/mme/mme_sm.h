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
void emm_state_de_registered(fsm_t *s, event_t *e);
void emm_state_authentication(fsm_t *s, event_t *e);
void emm_state_security_mode(fsm_t *s, event_t *e);
void emm_state_initial_context_setup(fsm_t *s, event_t *e);
void emm_state_registered(fsm_t *s, event_t *e);
void emm_state_exception(fsm_t *s, event_t *e);

void esm_state_initial(fsm_t *s, event_t *e);
void esm_state_final(fsm_t *s, event_t *e);
void esm_state_inactive(fsm_t *s, event_t *e);
void esm_state_active(fsm_t *s, event_t *e);
void esm_state_pdn_will_disconnect(fsm_t *s, event_t *e);
void esm_state_pdn_did_disconnect(fsm_t *s, event_t *e);
void esm_state_bearer_deactivated(fsm_t *s, event_t *e);
void esm_state_exception(fsm_t *s, event_t *e);

#define mme_sm_print(__pe) \
    d_print("%s(): %s\n", __func__, mme_event_get_name(__pe))

#define mme_sm_trace(__l, __pe) \
    d_trace(__l, "%s(): %s\n", __func__, mme_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__MME_SM_H__ */
