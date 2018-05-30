#define TRACE_MODULE _mme_event

#include "core_debug.h"

#include "mme_event.h"

char* mme_event_get_name(event_t *e)
{
    if (e == NULL)
        return FSM_NAME_INIT_SIG;

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG: 
            return FSM_NAME_ENTRY_SIG;
        case FSM_EXIT_SIG: 
            return FSM_NAME_EXIT_SIG;

        case MME_EVT_S1AP_MESSAGE:
            return "MME_EVT_S1AP_MESSAGE";
        case MME_EVT_S1AP_DELAYED_SEND:
            return "MME_EVT_S1AP_DELAYED_SEND";
        case MME_EVT_S1AP_LO_ACCEPT:
            return "MME_EVT_S1AP_LO_ACCEPT";
        case MME_EVT_S1AP_LO_SCTP_COMM_UP:
            return "MME_EVT_S1AP_LO_SCTP_COMM_UP";
        case MME_EVT_S1AP_LO_CONNREFUSED:
            return "MME_EVT_S1AP_LO_CONNREFUSED";
        case MME_EVT_S1AP_S1_HOLDING_TIMER:
            return "MME_EVT_S1AP_S1_HOLDING_TIMER";

        case MME_EVT_EMM_MESSAGE:
            return "MME_EVT_EMM_MESSAGE";
        case MME_EVT_EMM_T3413:
            return "MME_EVT_EMM_T3413";

        case MME_EVT_ESM_MESSAGE:
            return "MME_EVT_ESM_MESSAGE";

        case MME_EVT_S11_MESSAGE:
            return "MME_EVT_S11_MESSAGE";
        case MME_EVT_S11_T3_RESPONSE:
            return "MME_EVT_S11_T3_RESPONSE";
        case MME_EVT_S11_T3_HOLDING:
            return "MME_EVT_S11_T3_HOLDING";

        case MME_EVT_S6A_MESSAGE:
            return "MME_EVT_S6A_MESSAGE";

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
