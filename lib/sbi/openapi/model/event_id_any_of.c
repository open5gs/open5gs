
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_id_any_of.h"

char* OpenAPI_event_id_any_of_ToString(OpenAPI_event_id_any_of_e event_id_any_of)
{
    const char *event_id_any_ofArray[] =  { "NULL", "LOAD_LEVEL_INFORMATION", "NETWORK_PERFORMANCE", "NF_LOAD", "SERVICE_EXPERIENCE", "UE_MOBILITY", "UE_COMMUNICATION", "QOS_SUSTAINABILITY", "ABNORMAL_BEHAVIOUR", "USER_DATA_CONGESTION", "NSI_LOAD_LEVEL", "SM_CONGESTION", "DISPERSION", "RED_TRANS_EXP", "WLAN_PERFORMANCE", "DN_PERFORMANCE" };
    size_t sizeofArray = sizeof(event_id_any_ofArray) / sizeof(event_id_any_ofArray[0]);
    if (event_id_any_of < sizeofArray)
        return (char *)event_id_any_ofArray[event_id_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_event_id_any_of_e OpenAPI_event_id_any_of_FromString(char* event_id_any_of)
{
    int stringToReturn = 0;
    const char *event_id_any_ofArray[] =  { "NULL", "LOAD_LEVEL_INFORMATION", "NETWORK_PERFORMANCE", "NF_LOAD", "SERVICE_EXPERIENCE", "UE_MOBILITY", "UE_COMMUNICATION", "QOS_SUSTAINABILITY", "ABNORMAL_BEHAVIOUR", "USER_DATA_CONGESTION", "NSI_LOAD_LEVEL", "SM_CONGESTION", "DISPERSION", "RED_TRANS_EXP", "WLAN_PERFORMANCE", "DN_PERFORMANCE" };
    size_t sizeofArray = sizeof(event_id_any_ofArray) / sizeof(event_id_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_id_any_of, event_id_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

