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

    if (fsm->initial != (fsm_state_t)0)
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
    fsm->state = (fsm_handler_t)0;

    (*tmp)(s, e);
    if (fsm->state != (fsm_state_t)0)
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
            (*tmp)(s, &entry_event);
        }
    }
    else
    {
        fsm->state = tmp;
    }
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

    if (fsm->final != (fsm_state_t)0)
    {
        (*fsm->final)(s, e);
    }

    fsm->state = fsm->initial;
}
