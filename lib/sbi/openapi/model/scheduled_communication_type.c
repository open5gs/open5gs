
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scheduled_communication_type.h"

char* OpenAPI_scheduled_communication_type_ToString(OpenAPI_scheduled_communication_type_e scheduled_communication_type)
{
    const char *scheduled_communication_typeArray[] =  { "NULL", "DOWNLINK_ONLY", "UPLINK_ONLY", "BIDIRECTIONAL" };
    size_t sizeofArray = sizeof(scheduled_communication_typeArray) / sizeof(scheduled_communication_typeArray[0]);
    if (scheduled_communication_type < sizeofArray)
        return (char *)scheduled_communication_typeArray[scheduled_communication_type];
    else
        return (char *)"Unknown";
}

OpenAPI_scheduled_communication_type_e OpenAPI_scheduled_communication_type_FromString(char* scheduled_communication_type)
{
    int stringToReturn = 0;
    const char *scheduled_communication_typeArray[] =  { "NULL", "DOWNLINK_ONLY", "UPLINK_ONLY", "BIDIRECTIONAL" };
    size_t sizeofArray = sizeof(scheduled_communication_typeArray) / sizeof(scheduled_communication_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(scheduled_communication_type, scheduled_communication_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

