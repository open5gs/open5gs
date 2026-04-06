
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_event.h"

char* OpenAPI_nwdaf_event_ToString(OpenAPI_nwdaf_event_e nwdaf_event)
{
    const char *nwdaf_eventArray[] =  { "NULL", "SLICE_LOAD_LEVEL", "NETWORK_PERFORMANCE", "NF_LOAD", "SERVICE_EXPERIENCE", "UE_MOBILITY", "UE_COMMUNICATION", "QOS_SUSTAINABILITY", "ABNORMAL_BEHAVIOUR", "USER_DATA_CONGESTION", "NSI_LOAD_LEVEL", "DN_PERFORMANCE", "DISPERSION", "RED_TRANS_EXP", "WLAN_PERFORMANCE", "SM_CONGESTION", "PFD_DETERMINATION", "PDU_SESSION_TRAFFIC", "E2E_DATA_VOL_TRANS_TIME", "MOVEMENT_BEHAVIOUR", "LOC_ACCURACY", "RELATIVE_PROXIMITY", "SIGNALLING_STORM", "QOS_POLICY_ASSIST" };
    size_t sizeofArray = sizeof(nwdaf_eventArray) / sizeof(nwdaf_eventArray[0]);
    if (nwdaf_event < sizeofArray)
        return (char *)nwdaf_eventArray[nwdaf_event];
    else
        return (char *)"Unknown";
}

OpenAPI_nwdaf_event_e OpenAPI_nwdaf_event_FromString(char* nwdaf_event)
{
    int stringToReturn = 0;
    const char *nwdaf_eventArray[] =  { "NULL", "SLICE_LOAD_LEVEL", "NETWORK_PERFORMANCE", "NF_LOAD", "SERVICE_EXPERIENCE", "UE_MOBILITY", "UE_COMMUNICATION", "QOS_SUSTAINABILITY", "ABNORMAL_BEHAVIOUR", "USER_DATA_CONGESTION", "NSI_LOAD_LEVEL", "DN_PERFORMANCE", "DISPERSION", "RED_TRANS_EXP", "WLAN_PERFORMANCE", "SM_CONGESTION", "PFD_DETERMINATION", "PDU_SESSION_TRAFFIC", "E2E_DATA_VOL_TRANS_TIME", "MOVEMENT_BEHAVIOUR", "LOC_ACCURACY", "RELATIVE_PROXIMITY", "SIGNALLING_STORM", "QOS_POLICY_ASSIST" };
    size_t sizeofArray = sizeof(nwdaf_eventArray) / sizeof(nwdaf_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nwdaf_event, nwdaf_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

