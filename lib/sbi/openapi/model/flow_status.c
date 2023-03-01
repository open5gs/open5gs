
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_status.h"

char* OpenAPI_flow_status_ToString(OpenAPI_flow_status_e flow_status)
{
    const char *flow_statusArray[] =  { "NULL", "ENABLED-UPLINK", "ENABLED-DOWNLINK", "ENABLED", "DISABLED", "REMOVED" };
    size_t sizeofArray = sizeof(flow_statusArray) / sizeof(flow_statusArray[0]);
    if (flow_status < sizeofArray)
        return (char *)flow_statusArray[flow_status];
    else
        return (char *)"Unknown";
}

OpenAPI_flow_status_e OpenAPI_flow_status_FromString(char* flow_status)
{
    int stringToReturn = 0;
    const char *flow_statusArray[] =  { "NULL", "ENABLED-UPLINK", "ENABLED-DOWNLINK", "ENABLED", "DISABLED", "REMOVED" };
    size_t sizeofArray = sizeof(flow_statusArray) / sizeof(flow_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(flow_status, flow_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

