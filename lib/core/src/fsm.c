#include "core_fsm.h"

typedef struct _event_t {
    fsm_event_t event;
} event_t;

static event_t entry_event = {
    FSM_ENTRY_SIG,
};
static event_t exit_event = {
    FSM_EXIT_SIG,
};

char *FSM_NAME_INIT_SIG = "INIT";
char *FSM_NAME_ENTRY_SIG = "ENTRY";
char *FSM_NAME_EXIT_SIG = "EXIT";

void fsm_init(void *s, void *_e)
{
    fsm_t *fsm = s;
    event_t *e = _e;

    if (fsm->initial != NULL)
    {
        (*fsm->initial)(s, e);
        if (fsm->initial != fsm->state)
        {
            if (e)
            {
                e->event = FSM_ENTRY_SIG;
                (*fsm->state)(s, e);
            }
            else
            {
                (*fsm->state)(s, &entry_event);
            }
        }
    }
}

void fsm_dispatch(void *s, void *_e)
{
    fsm_t *fsm = s;
    event_t *e = _e;
    fsm_handler_t tmp = fsm->state;
#if OLD_FSM_DISPATCH
    fsm->state = (fsm_handler_t)0;
#endif

    (*tmp)(s, e);
#if OLD_FSM_DISPATCH
    if (fsm->state != NULL)
#else
    if (fsm->state != tmp)
#endif
    {
        if (e)
        {
            e->event = FSM_EXIT_SIG;
            (*tmp)(s, e);
        }
        else
        {
            (*tmp)(s, &exit_event);
        }
        if (e)
        {
            e->event = FSM_ENTRY_SIG;
            (*fsm->state)(s, e);
        }
        else
        {
#if OLD_FSM_DISPATCH
            (*tmp)(s, &entry_event);
#else
            (*fsm->state)(s, &entry_event);
#endif
        }
    }
#if OLD_FSM_DISPATCH
    else
    {
        fsm->state = tmp;
    }
#endif
}

void fsm_final(void *s, void *_e)
{
    fsm_t *fsm = s;
    event_t *e = _e;

    if (fsm->final != fsm->state)
    {
        if (e)
        {
            e->event = FSM_EXIT_SIG;
            (*fsm->state)(s, e);
        }
        else
        {
            (*fsm->state)(s, &exit_event);
        }
    }

    if (fsm->final != NULL)
    {
        (*fsm->final)(s, e);
    }

    fsm->state = fsm->initial;
}
