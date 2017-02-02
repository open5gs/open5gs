#include "core_fsm.h"

static fsm_event_t fsm_event[] = {
    FSM_ENTRY_SIG,
    FSM_EXIT_SIG
};

void fsm_init(fsm_t *s, fsm_event_t *e)
{
    if (s->initial != (fsm_state_t)0)
    {
        (*s->initial)(s, e);
        if (s->initial != s->state)
        {
            (*s->state)(s, &fsm_event[FSM_ENTRY_SIG]);
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
        (*tmp)(s, &fsm_event[FSM_EXIT_SIG]);
        (*s->state)(s, &fsm_event[FSM_ENTRY_SIG]);
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
        (*s->state)(s, &fsm_event[FSM_EXIT_SIG]);
    }

    if (s->final != (fsm_state_t)0)
    {
        (*s->final)(s, 0);
    }

    s->state = s->initial;
}
