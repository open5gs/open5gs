#ifndef __SGW_SM_H__
#define __SGW_SM_H__

#include "core_param.h"
#include "core_fsm.h"
#include "core_event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _sgw_sm_t {
    fsm_t fsm;
} sgw_sm_t;

void sgw_state_initial(sgw_sm_t *s, event_t *e);
void sgw_state_final(sgw_sm_t *s, event_t *e);
void sgw_state_operational(sgw_sm_t *s, event_t *e);
void sgw_state_exception(sgw_sm_t *s, event_t *e);

#define sgw_sm_print(__pe) \
    d_print("%s(): %s\n", __func__, sgw_event_get_name(__pe))

#define sgw_sm_trace(__l, __pe) \
    d_trace(__l, "%s(): %s\n", __func__, sgw_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__SGW_SM_H__ */
