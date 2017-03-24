#define TRACE_MODULE _mme_evt

#include "event.h"

static char EVT_NAME_LO_ENB_S1AP_ACCEPT[] = "LO_ENB_S1AP_ACCEPT";
static char EVT_NAME_LO_ENB_S1AP_CONNREFUSED[] = "LO_ENB_S1AP_CONNREFUSED";

static char EVT_NAME_MSG_ENB_S1AP[] = "MSG_ENB_S1AP";
static char EVT_NAME_MSG_UE_EMM[] = "MSG_UE_EMM";
static char EVT_NAME_MSG_MME_S11[] = "MSG_MME_S11";

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

        case EVT_LO_ENB_S1AP_ACCEPT: 
               return EVT_NAME_LO_ENB_S1AP_ACCEPT;
        case EVT_LO_ENB_S1AP_CONNREFUSED: 
               return EVT_NAME_LO_ENB_S1AP_CONNREFUSED;

        case EVT_MSG_ENB_S1AP: 
               return EVT_NAME_MSG_ENB_S1AP;
        case EVT_MSG_UE_EMM: 
               return EVT_NAME_MSG_UE_EMM;
        case EVT_MSG_MME_S11: 
               return EVT_NAME_MSG_MME_S11;

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}
