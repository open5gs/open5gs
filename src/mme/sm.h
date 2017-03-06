#ifndef __SM_H__
#define __SM_H__

#include "core_param.h"
#include "core_fsm.h"

#include "event.h"

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

typedef struct _enb_s1ap_sm_t {
    fsm_t fsm;
    void *ctx;
} enb_s1ap_sm_t;

void enb_s1ap_state_initial(enb_s1ap_sm_t *s, event_t *e);
void enb_s1ap_state_final(enb_s1ap_sm_t *s, event_t *e);
void enb_s1ap_state_operational(enb_s1ap_sm_t *s, event_t *e);
void enb_s1ap_state_exception(enb_s1ap_sm_t *s, event_t *e);

typedef struct _ue_emm_sm_t {
    fsm_t fsm;
    void *ctx;
} ue_emm_sm_t;

void ue_emm_state_initial(ue_emm_sm_t *s, event_t *e);
void ue_emm_state_final(ue_emm_sm_t *s, event_t *e);
void ue_emm_state_operational(ue_emm_sm_t *s, event_t *e);
void ue_emm_state_exception(ue_emm_sm_t *s, event_t *e);

#define sm_print(__pe) \
    d_print("%s(): %s\n", __func__, event_get_name(__pe))

#define sm_trace(__l, __pe) \
    d_trace(__l, "%s(): %s\n", __func__, event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__SM_H__ */
