#ifndef __CORE_FSM_H__
#define __CORE_FSM_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern char *FSM_NAME_INIT_SIG;
extern char *FSM_NAME_ENTRY_SIG;
extern char *FSM_NAME_EXIT_SIG;

typedef enum _fsm_signal_t {
    FSM_ENTRY_SIG,
    FSM_EXIT_SIG,
    FSM_USER_SIG
} fsm_signal_t;

typedef c_uintptr_t fsm_event_t;
typedef c_uintptr_t fsm_state_t;

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


CORE_DECLARE(void) fsm_init(void *s, void *e);
CORE_DECLARE(void) fsm_dispatch(void *s, void *e);
CORE_DECLARE(void) fsm_final(void *s, void *e);

#define FSM_TRAN(__s, __target) \
    ((fsm_t *)__s)->state = (fsm_handler_t)(__target)

#define FSM_STATE(__s) \
    (((fsm_t *)__s)->state)

#define FSM_CHECK(__s, __f) \
    (FSM_STATE(__s) == (fsm_handler_t)__f)

#ifdef __cplusplus
}
#endif

#endif /* ! __CORE_FSM_H__ */
