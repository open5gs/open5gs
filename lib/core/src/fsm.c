#include "core_fsm.h"

static fsm_event_t entry_event = {
    FSM_ENTRY_SIG,
};
static fsm_event_t exit_event = {
    FSM_EXIT_SIG,
};

char *FSM_NAME_INIT_SIG = "INIT";
char *FSM_NAME_ENTRY_SIG = "ENTRY";
char *FSM_NAME_EXIT_SIG = "EXIT";

void fsm_init(fsm_t *s, fsm_event_t *e)
{
    if (s->initial != (fsm_state_t)0)
    {
        (*s->initial)(s, e);
        if (s->initial != s->state)
        {
            if (e)
            {
                e->event = FSM_ENTRY_SIG;
                (*s->state)(s, e);
            }
            else
            {
                (*s->state)(s, &entry_event);
            }
        }
    }
}

void fsm_dispatch(fsm_t *s, fsm_event_t *e)
{
    fsm_handler_t tmp = s->state;
    s->state = (fsm_handler_t)0;

    (*tmp)(s, e);
    if (s->state != (fsm_state_t)0)
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
            (*s->state)(s, e);
        }
        else
        {
            (*tmp)(s, &entry_event);
        }
    }
    else
    {
        s->state = tmp;
    }
}

void fsm_final(fsm_t *s, fsm_event_t *e)
{
    if (s->final != s->state)
    {
        if (e)
        {
            e->event = FSM_EXIT_SIG;
            (*s->state)(s, e);
        }
        else
        {
            (*s->state)(s, &exit_event);
        }
    }

    if (s->final != (fsm_state_t)0)
    {
        (*s->final)(s, e);
    }

    s->state = s->initial;
}
