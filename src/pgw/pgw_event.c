#define TRACE_MODULE _pgw_event

#include "pgw_event.h"
#include "pgw_context.h"

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

        case PGW_EVT_S5C_MESSAGE:
            return "PGW_EVT_S5C_MESSAGE";
        case PGW_EVT_S5C_T3_RESPONSE:
            return "PGW_EVT_S5C_T3_RESPONSE";
        case PGW_EVT_S5C_T3_HOLDING:
            return "PGW_EVT_S5C_T3_HOLDING";

        case PGW_EVT_GX_MESSAGE:
            return "PGW_EVT_GX_SESSION_MSG";

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
