#define TRACE_MODULE _sgw_evt

#include "core_debug.h"
#include "event.h"
#include "context.h"

static char EVT_NAME_TM_MME_S11_T3[] = "TM_MME_S11_T3";

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

        case EVT_TM_SGW_S11_T3: 
           return EVT_NAME_TM_MME_S11_T3;

        case EVT_MSG_SGW_S11: 
               return EVT_NAME_MSG_SGW_S11;

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
