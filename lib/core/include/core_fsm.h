#ifndef __CORE_FSM_H__
#define __CORE_FSM_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum _fsm_signal_t {
    FSM_ENTRY_SIG,
    FSM_EXIT_SIG,
    FSM_USER_SIG
} fsm_signal_t;

typedef c_uint32_t fsm_event_t;
typedef c_uint32_t fsm_state_t;
typedef void (*fsm_handler_t)(void *s, void *e);

typedef struct _fsm_t {
    fsm_handler_t initial;
    fsm_handler_t final;
    fsm_handler_t state;
} fsm_t;

#define fsm_create(__s, __i, __f) \
    (((__s)->initial = (__s)->state = (fsm_handler_t)(__i)), \
     (__s)->final = (fsm_handler_t)(__f))

#define fsm_clear(__s) \
    ((__s)->initial = (__s)->state = (__s)->final = NULL)


CORE_DECLARE(void) fsm_init(fsm_t *s, fsm_event_t *e);
CORE_DECLARE(void) fsm_dispatch(fsm_t *s, fsm_event_t *e);
CORE_DECLARE(void) fsm_final(fsm_t *s, fsm_event_t *e);

#define FSM_TRAN(__s, __target) \
    ((fsm_t *)__s)->state = (fsm_handler_t)(__target)

#define FSM_STATE(__s) \
    (((fsm_t *)__s)->state)

#ifdef __cplusplus
}
#endif

#endif /* ! __CORE_FSM_H__ */
