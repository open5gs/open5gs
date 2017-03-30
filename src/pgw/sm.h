#ifndef __PGW_SM_H__
#define __PGW_SM_H__

#include "core_param.h"
#include "core_fsm.h"

#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _pgw_sm_t {
    fsm_t fsm;
    void *ctx;
} pgw_sm_t;

void pgw_state_initial(pgw_sm_t *s, event_t *e);
void pgw_state_final(pgw_sm_t *s, event_t *e);
void pgw_state_operational(pgw_sm_t *s, event_t *e);
void pgw_state_exception(pgw_sm_t *s, event_t *e);

#define pgw_sm_print(__pe) \
    d_print("%s(): %s\n", __func__, pgw_event_get_name(__pe))

#define pgw_sm_trace(__l, __pe) \
    d_trace(__l, "%s(): %s\n", __func__, pgw_event_get_name(__pe))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__PGW_SM_H__ */
