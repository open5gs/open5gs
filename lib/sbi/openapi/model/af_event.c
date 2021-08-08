
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_event.h"

char* OpenAPI_af_event_ToString(OpenAPI_af_event_e af_event)
{
    const char *af_eventArray[] =  { "NULL", "ACCESS_TYPE_CHANGE", "ANI_REPORT", "CHARGING_CORRELATION", "EPS_FALLBACK", "FAILED_RESOURCES_ALLOCATION", "OUT_OF_CREDIT", "PLMN_CHG", "QOS_MONITORING", "QOS_NOTIF", "RAN_NAS_CAUSE", "REALLOCATION_OF_CREDIT", "SUCCESSFUL_RESOURCES_ALLOCATION", "TSN_BRIDGE_INFO", "USAGE_REPORT" };
    size_t sizeofArray = sizeof(af_eventArray) / sizeof(af_eventArray[0]);
    if (af_event < sizeofArray)
        return (char *)af_eventArray[af_event];
    else
        return (char *)"Unknown";
}

OpenAPI_af_event_e OpenAPI_af_event_FromString(char* af_event)
{
    int stringToReturn = 0;
    const char *af_eventArray[] =  { "NULL", "ACCESS_TYPE_CHANGE", "ANI_REPORT", "CHARGING_CORRELATION", "EPS_FALLBACK", "FAILED_RESOURCES_ALLOCATION", "OUT_OF_CREDIT", "PLMN_CHG", "QOS_MONITORING", "QOS_NOTIF", "RAN_NAS_CAUSE", "REALLOCATION_OF_CREDIT", "SUCCESSFUL_RESOURCES_ALLOCATION", "TSN_BRIDGE_INFO", "USAGE_REPORT" };
    size_t sizeofArray = sizeof(af_eventArray) / sizeof(af_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(af_event, af_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

