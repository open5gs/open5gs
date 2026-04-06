
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_type_1.h"

char* OpenAPI_event_type_1_ToString(OpenAPI_event_type_1_e event_type_1)
{
    const char *event_type_1Array[] =  { "NULL", "QOS_MONITORING", "USER_DATA_USAGE_MEASURES", "USER_DATA_USAGE_TRENDS", "TSC_MNGT_INFO", "UE_NAT_MAPPING_INFO", "HANDLING_OF_PAYLOAD_HEADERS_INFO", "SUBSCRIPTION_TERMINATION" };
    size_t sizeofArray = sizeof(event_type_1Array) / sizeof(event_type_1Array[0]);
    if (event_type_1 < sizeofArray)
        return (char *)event_type_1Array[event_type_1];
    else
        return (char *)"Unknown";
}

OpenAPI_event_type_1_e OpenAPI_event_type_1_FromString(char* event_type_1)
{
    int stringToReturn = 0;
    const char *event_type_1Array[] =  { "NULL", "QOS_MONITORING", "USER_DATA_USAGE_MEASURES", "USER_DATA_USAGE_TRENDS", "TSC_MNGT_INFO", "UE_NAT_MAPPING_INFO", "HANDLING_OF_PAYLOAD_HEADERS_INFO", "SUBSCRIPTION_TERMINATION" };
    size_t sizeofArray = sizeof(event_type_1Array) / sizeof(event_type_1Array[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_type_1, event_type_1Array[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

