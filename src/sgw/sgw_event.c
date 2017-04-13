#define TRACE_MODULE _sgw_event

#include "sgw_event.h"
#include "sgw_context.h"

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

        case SGW_EVT_S11_SESSION_MSG:
            return "SGW_EVT_S11_SESSION_MSG";
        case SGW_EVT_S5C_SESSION_MSG:
            return "SGW_EVT_S5C_SESSION_MSG";

        case SGW_EVT_TRANSACTION_T3:
            return "SGW_EVT_TRANSACTION_T3";

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
