
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_event.h"

char* OpenAPI_af_event_ToString(OpenAPI_af_event_e af_event)
{
    const char *af_eventArray[] =  { "NULL", "SVC_EXPERIENCE", "UE_MOBILITY", "UE_COMM", "EXCEPTIONS", "USER_DATA_CONGESTION", "PERF_DATA", "DISPERSION", "COLLECTIVE_BEHAVIOUR", "MS_QOE_METRICS", "MS_CONSUMPTION", "MS_NET_ASSIST_INVOCATION", "MS_DYN_POLICY_INVOCATION", "MS_ACCESS_ACTIVITY" };
    size_t sizeofArray = sizeof(af_eventArray) / sizeof(af_eventArray[0]);
    if (af_event < sizeofArray)
        return (char *)af_eventArray[af_event];
    else
        return (char *)"Unknown";
}

OpenAPI_af_event_e OpenAPI_af_event_FromString(char* af_event)
{
    int stringToReturn = 0;
    const char *af_eventArray[] =  { "NULL", "SVC_EXPERIENCE", "UE_MOBILITY", "UE_COMM", "EXCEPTIONS", "USER_DATA_CONGESTION", "PERF_DATA", "DISPERSION", "COLLECTIVE_BEHAVIOUR", "MS_QOE_METRICS", "MS_CONSUMPTION", "MS_NET_ASSIST_INVOCATION", "MS_DYN_POLICY_INVOCATION", "MS_ACCESS_ACTIVITY" };
    size_t sizeofArray = sizeof(af_eventArray) / sizeof(af_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(af_event, af_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

