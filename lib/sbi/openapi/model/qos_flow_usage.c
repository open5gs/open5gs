
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_usage.h"

char* OpenAPI_qos_flow_usage_ToString(OpenAPI_qos_flow_usage_e qos_flow_usage)
{
    const char *qos_flow_usageArray[] =  { "NULL", "GENERAL", "IMS_SIG" };
    size_t sizeofArray = sizeof(qos_flow_usageArray) / sizeof(qos_flow_usageArray[0]);
    if (qos_flow_usage < sizeofArray)
        return (char *)qos_flow_usageArray[qos_flow_usage];
    else
        return (char *)"Unknown";
}

OpenAPI_qos_flow_usage_e OpenAPI_qos_flow_usage_FromString(char* qos_flow_usage)
{
    int stringToReturn = 0;
    const char *qos_flow_usageArray[] =  { "NULL", "GENERAL", "IMS_SIG" };
    size_t sizeofArray = sizeof(qos_flow_usageArray) / sizeof(qos_flow_usageArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qos_flow_usage, qos_flow_usageArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

