#define TRACE_MODULE _pgw_event

#include "pgw_event.h"
#include "pgw_ctx.h"

static char EVT_NAME_TM_PGW_T3[] = "TM_PGW_PGW_T3";

static char EVT_NAME_MSG_PGW_S5C[] = "MSG_PGW_S5C";

char* pgw_event_get_name(event_t *e)
{
    if (e == NULL)
        return FSM_NAME_INIT_SIG;

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG: 
            return FSM_NAME_ENTRY_SIG;
        case FSM_EXIT_SIG: 
            return FSM_NAME_EXIT_SIG;

        case EVT_TM_PGW_T3: 
           return EVT_NAME_TM_PGW_T3;

        case EVT_MSG_PGW_S5C: 
               return EVT_NAME_MSG_PGW_S5C;

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
