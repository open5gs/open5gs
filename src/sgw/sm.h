#ifndef __SGW_SM_H__
#define __SGW_SM_H__

#include "core_param.h"
#include "core_fsm.h"

#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _sgw_sm_t {
    fsm_t fsm;
    void *ctx;
} sgw_sm_t;

void sgw_state_initial(sgw_sm_t *s, event_t *e);
void sgw_state_final(sgw_sm_t *s, event_t *e);
void sgw_state_operational(sgw_sm_t *s, event_t *e);
void sgw_state_exception(sgw_sm_t *s, event_t *e);

typedef struct _s11_sm_t {
    fsm_t fsm;
    void *ctx;
} s11_sm_t;

void sgw_s11_state_initial(s11_sm_t *s, event_t *e);
void sgw_s11_state_final(s11_sm_t *s, event_t *e);
void sgw_s11_state_operational(s11_sm_t *s, event_t *e);
void sgw_s11_state_exception(s11_sm_t *s, event_t *e);

typedef struct _s5c_sm_t {
    fsm_t fsm;
    void *ctx;
} s5c_sm_t;

void sgw_s5c_state_initial(s5c_sm_t *s, event_t *e);
void sgw_s5c_state_final(s5c_sm_t *s, event_t *e);
void sgw_s5c_state_operational(s5c_sm_t *s, event_t *e);
void sgw_s5c_state_exception(s5c_sm_t *s, event_t *e);

#define sgw_sm_print(__pe) \
    d_print("%s(): %s\n", __func__, sgw_event_get_name(__pe))

#define sgw_sm_trace(__l, __pe) \
    d_trace(__l, "%s(): %s\n", __func__, sgw_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__SGW_SM_H__ */
