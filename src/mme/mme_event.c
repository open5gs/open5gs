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

        case MME_EVT_S1AP_ENB_MSG:
            return "MME_EVT_S1AP_ENB_MSG";
        case MME_EVT_S1AP_ENB_LO_ACCEPT:
            return "MME_EVT_S1AP_ENB_LO_ACCEPT";
        case MME_EVT_S1AP_ENB_LO_CONNREFUSED:
            return"MME_EVT_S1AP_ENB_LO_CONNREFUSED";

        case MME_EVT_EMM_UE_MSG:
            return "MME_EVT_EMM_UE_MSG";
        case MME_EVT_EMM_UE_FROM_S6A:
            return "MME_EVT_EMM_UE_FROM_S6A";
        case MME_EVT_EMM_UE_T3413:
            return "MME_EVT_EMM_UE_T3413";

        case MME_EVT_ESM_BEARER_MSG:
            return "MME_EVT_ESM_BEARER_MSG";

        case MME_EVT_S11_UE_MSG:
            return "MME_EVT_S11_UE_MSG";
        case MME_EVT_S11_TRANSACTION_T3:
            return "MME_EVT_S11_TRANSACTION_T3";

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
