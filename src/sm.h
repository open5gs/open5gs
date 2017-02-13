#ifndef __SM_H__
#define __SM_H__

#include "core_param.h"
#include "core_fsm.h"

#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _mme_s1_sm_t {

    fsm_t fsm;
    void *ctx;

} mme_s1_sm_t;

void mme_s1_state_initial(mme_s1_sm_t *s, event_t *e);
void mme_s1_state_final(mme_s1_sm_t *s, event_t *e);
void mme_s1_state_operational(mme_s1_sm_t *s, event_t *e);
void mme_s1_state_exception(mme_s1_sm_t *s, event_t *e);

typedef struct _enb_s1_sm_t {

    fsm_t fsm;
    void *ctx;

} enb_s1_sm_t;

void enb_s1_state_initial(enb_s1_sm_t *s, event_t *e);
void enb_s1_state_final(enb_s1_sm_t *s, event_t *e);
void enb_s1_state_operational(enb_s1_sm_t *s, event_t *e);
void enb_s1_state_exception(enb_s1_sm_t *s, event_t *e);

typedef struct _master_sm_t {

    fsm_t fsm;
    void *ctx;

} master_sm_t;

void master_state_initial(master_sm_t *s, event_t *e);
void master_state_final(master_sm_t *s, event_t *e);
void master_state_operational(master_sm_t *s, event_t *e);
void master_state_exception(master_sm_t *s, event_t *e);

#define sm_print(__pe) \
    d_print("%s(): %s\n", __func__, event_get_name(__pe))

#define sm_trace(__l, __pe) \
    d_trace(__l, "%s(): %s\n", __func__, event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__SM_H__ */
