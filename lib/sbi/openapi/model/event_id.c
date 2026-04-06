
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_id.h"

char* OpenAPI_event_id_ToString(OpenAPI_event_id_e event_id)
{
    const char *event_idArray[] =  { "NULL", "LOAD_LEVEL_INFORMATION", "NETWORK_PERFORMANCE", "NF_LOAD", "SERVICE_EXPERIENCE", "UE_MOBILITY", "UE_COMMUNICATION", "QOS_SUSTAINABILITY", "ABNORMAL_BEHAVIOUR", "USER_DATA_CONGESTION", "NSI_LOAD_LEVEL", "SM_CONGESTION", "DISPERSION", "RED_TRANS_EXP", "WLAN_PERFORMANCE", "DN_PERFORMANCE", "PDU_SESSION_TRAFFIC", "E2E_DATA_VOL_TRANS_TIME", "MOVEMENT_BEHAVIOUR", "LOC_ACCURACY", "RELATIVE_PROXIMITY", "SIGNALLING_STORM", "QOS_POLICY_ASSIST" };
    size_t sizeofArray = sizeof(event_idArray) / sizeof(event_idArray[0]);
    if (event_id < sizeofArray)
        return (char *)event_idArray[event_id];
    else
        return (char *)"Unknown";
}

OpenAPI_event_id_e OpenAPI_event_id_FromString(char* event_id)
{
    int stringToReturn = 0;
    const char *event_idArray[] =  { "NULL", "LOAD_LEVEL_INFORMATION", "NETWORK_PERFORMANCE", "NF_LOAD", "SERVICE_EXPERIENCE", "UE_MOBILITY", "UE_COMMUNICATION", "QOS_SUSTAINABILITY", "ABNORMAL_BEHAVIOUR", "USER_DATA_CONGESTION", "NSI_LOAD_LEVEL", "SM_CONGESTION", "DISPERSION", "RED_TRANS_EXP", "WLAN_PERFORMANCE", "DN_PERFORMANCE", "PDU_SESSION_TRAFFIC", "E2E_DATA_VOL_TRANS_TIME", "MOVEMENT_BEHAVIOUR", "LOC_ACCURACY", "RELATIVE_PROXIMITY", "SIGNALLING_STORM", "QOS_POLICY_ASSIST" };
    size_t sizeofArray = sizeof(event_idArray) / sizeof(event_idArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_id, event_idArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

