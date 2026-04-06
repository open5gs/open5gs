
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_type.h"

char* OpenAPI_event_type_ToString(OpenAPI_event_type_e event_type)
{
    const char *event_typeArray[] =  { "NULL", "QOS_MONITORING", "USER_DATA_USAGE_MEASURES", "USER_DATA_USAGE_TRENDS", "TSC_MNGT_INFO", "UE_NAT_MAPPING_INFO", "HANDLING_OF_PAYLOAD_HEADERS_INFO", "SUBSCRIPTION_TERMINATION" };
    size_t sizeofArray = sizeof(event_typeArray) / sizeof(event_typeArray[0]);
    if (event_type < sizeofArray)
        return (char *)event_typeArray[event_type];
    else
        return (char *)"Unknown";
}

OpenAPI_event_type_e OpenAPI_event_type_FromString(char* event_type)
{
    int stringToReturn = 0;
    const char *event_typeArray[] =  { "NULL", "QOS_MONITORING", "USER_DATA_USAGE_MEASURES", "USER_DATA_USAGE_TRENDS", "TSC_MNGT_INFO", "UE_NAT_MAPPING_INFO", "HANDLING_OF_PAYLOAD_HEADERS_INFO", "SUBSCRIPTION_TERMINATION" };
    size_t sizeofArray = sizeof(event_typeArray) / sizeof(event_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_type, event_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

