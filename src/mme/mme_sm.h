#ifndef __MME_SM_H__
#define __MME_SM_H__

#include "core_param.h"
#include "core_fsm.h"
#include "core_event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _mme_sm_t {
    fsm_t fsm;
    void *ctx;
} mme_sm_t;

void mme_state_initial(mme_sm_t *s, event_t *e);
void mme_state_final(mme_sm_t *s, event_t *e);
void mme_state_operational(mme_sm_t *s, event_t *e);
void mme_state_exception(mme_sm_t *s, event_t *e);

typedef struct _s1ap_sm_t {
    fsm_t fsm;
    void *ctx;
} s1ap_sm_t;

void s1ap_state_initial(s1ap_sm_t *s, event_t *e);
void s1ap_state_final(s1ap_sm_t *s, event_t *e);
void s1ap_state_operational(s1ap_sm_t *s, event_t *e);
void s1ap_state_exception(s1ap_sm_t *s, event_t *e);

typedef struct _emm_sm_t {
    fsm_t fsm;
    void *ctx;
} emm_sm_t;

void emm_state_initial(emm_sm_t *s, event_t *e);
void emm_state_final(emm_sm_t *s, event_t *e);
void emm_state_operational(emm_sm_t *s, event_t *e);
void emm_state_exception(emm_sm_t *s, event_t *e);

typedef struct _esm_sm_t {
    fsm_t fsm;
    void *ctx;
} esm_sm_t;

void esm_state_initial(esm_sm_t *s, event_t *e);
void esm_state_final(esm_sm_t *s, event_t *e);
void esm_state_operational(esm_sm_t *s, event_t *e);
void esm_state_exception(esm_sm_t *s, event_t *e);

#define mme_sm_print(__pe) \
    d_print("%s(): %s\n", __func__, mme_event_get_name(__pe))

#define mme_sm_trace(__l, __pe) \
    d_trace(__l, "%s(): %s\n", __func__, mme_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__MME_SM_H__ */
