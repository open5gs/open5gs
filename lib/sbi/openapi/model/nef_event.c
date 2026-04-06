
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nef_event.h"

char* OpenAPI_nef_event_ToString(OpenAPI_nef_event_e nef_event)
{
    const char *nef_eventArray[] =  { "NULL", "SVC_EXPERIENCE", "UE_MOBILITY", "UE_COMM", "EXCEPTIONS", "USER_DATA_CONGESTION", "PERF_DATA", "DISPERSION", "COLLECTIVE_BEHAVIOUR", "MS_QOE_METRICS", "MS_CONSUMPTION", "MS_NET_ASSIST_INVOCATION", "MS_DYN_POLICY_INVOCATION", "MS_ACCESS_ACTIVITY", "GNSS_ASSISTANCE_DATA", "DATA_VOLUME_TRANSFER_TIME", "APP_ACTIVE_TIME", "SIGNALLING_INFO", "UE_ALTITUDE" };
    size_t sizeofArray = sizeof(nef_eventArray) / sizeof(nef_eventArray[0]);
    if (nef_event < sizeofArray)
        return (char *)nef_eventArray[nef_event];
    else
        return (char *)"Unknown";
}

OpenAPI_nef_event_e OpenAPI_nef_event_FromString(char* nef_event)
{
    int stringToReturn = 0;
    const char *nef_eventArray[] =  { "NULL", "SVC_EXPERIENCE", "UE_MOBILITY", "UE_COMM", "EXCEPTIONS", "USER_DATA_CONGESTION", "PERF_DATA", "DISPERSION", "COLLECTIVE_BEHAVIOUR", "MS_QOE_METRICS", "MS_CONSUMPTION", "MS_NET_ASSIST_INVOCATION", "MS_DYN_POLICY_INVOCATION", "MS_ACCESS_ACTIVITY", "GNSS_ASSISTANCE_DATA", "DATA_VOLUME_TRANSFER_TIME", "APP_ACTIVE_TIME", "SIGNALLING_INFO", "UE_ALTITUDE" };
    size_t sizeofArray = sizeof(nef_eventArray) / sizeof(nef_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nef_event, nef_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

