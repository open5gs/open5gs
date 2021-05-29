
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_usage.h"

char* OpenAPI_flow_usage_ToString(OpenAPI_flow_usage_e flow_usage)
{
    const char *flow_usageArray[] =  { "NULL", "NO_INFO", "RTCP", "AF_SIGNALLING" };
    size_t sizeofArray = sizeof(flow_usageArray) / sizeof(flow_usageArray[0]);
    if (flow_usage < sizeofArray)
        return (char *)flow_usageArray[flow_usage];
    else
        return (char *)"Unknown";
}

OpenAPI_flow_usage_e OpenAPI_flow_usage_FromString(char* flow_usage)
{
    int stringToReturn = 0;
    const char *flow_usageArray[] =  { "NULL", "NO_INFO", "RTCP", "AF_SIGNALLING" };
    size_t sizeofArray = sizeof(flow_usageArray) / sizeof(flow_usageArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(flow_usage, flow_usageArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

