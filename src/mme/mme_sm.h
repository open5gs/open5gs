#ifndef __MME_SM_H__
#define __MME_SM_H__

#include "mme_event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void mme_state_initial(ogs_fsm_t *s, mme_event_t *e);
void mme_state_final(ogs_fsm_t *s, mme_event_t *e);
void mme_state_operational(ogs_fsm_t *s, mme_event_t *e);
void mme_state_exception(ogs_fsm_t *s, mme_event_t *e);

void s1ap_state_initial(ogs_fsm_t *s, mme_event_t *e);
void s1ap_state_final(ogs_fsm_t *s, mme_event_t *e);
void s1ap_state_operational(ogs_fsm_t *s, mme_event_t *e);
void s1ap_state_exception(ogs_fsm_t *s, mme_event_t *e);

void emm_state_initial(ogs_fsm_t *s, mme_event_t *e);
void emm_state_final(ogs_fsm_t *s, mme_event_t *e);
void emm_state_de_registered(ogs_fsm_t *s, mme_event_t *e);
void emm_state_authentication(ogs_fsm_t *s, mme_event_t *e);
void emm_state_security_mode(ogs_fsm_t *s, mme_event_t *e);
void emm_state_initial_context_setup(ogs_fsm_t *s, mme_event_t *e);
void emm_state_registered(ogs_fsm_t *s, mme_event_t *e);
void emm_state_exception(ogs_fsm_t *s, mme_event_t *e);

void esm_state_initial(ogs_fsm_t *s, mme_event_t *e);
void esm_state_final(ogs_fsm_t *s, mme_event_t *e);
void esm_state_inactive(ogs_fsm_t *s, mme_event_t *e);
void esm_state_active(ogs_fsm_t *s, mme_event_t *e);
void esm_state_pdn_will_disconnect(ogs_fsm_t *s, mme_event_t *e);
void esm_state_pdn_did_disconnect(ogs_fsm_t *s, mme_event_t *e);
void esm_state_bearer_deactivated(ogs_fsm_t *s, mme_event_t *e);
void esm_state_exception(ogs_fsm_t *s, mme_event_t *e);

#define mme_sm_debug(__pe) \
    ogs_debug("%s(): %s\n", __func__, mme_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__MME_SM_H__ */
