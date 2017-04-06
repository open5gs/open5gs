#define TRACE_MODULE _sgw_event

#include "sgw_event.h"
#include "sgw_context.h"

static char EVT_NAME_TM_SGW_T3[] = "TM_SGW_SGW_T3";

static char EVT_NAME_MSG_SGW_S11[] = "MSG_SGW_S11";
static char EVT_NAME_MSG_SGW_S5C[] = "MSG_SGW_S5C";

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

        case EVT_TM_SGW_T3: 
           return EVT_NAME_TM_SGW_T3;

        case EVT_MSG_SGW_S11: 
               return EVT_NAME_MSG_SGW_S11;
        case EVT_MSG_SGW_S5C: 
               return EVT_NAME_MSG_SGW_S5C;

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
