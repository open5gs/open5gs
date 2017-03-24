#define TRACE_MODULE _mme_evt

#include "event.h"

static char EVT_NAME_MSG_SGW_S11[] = "MSG_MME_S11";

char* sgw_event_get_name(event_t *e)
{
    if (e == NULL)
        return FSM_NAME_INIT_SIG;

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG: 
            return FSM_NAME_ENTRY_SIG;
        case FSM_EXIT_SIG: 
            return FSM_NAME_EXIT_SIG;

        case EVT_MSG_SGW_S11: 
               return EVT_NAME_MSG_SGW_S11;

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
