
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "condition_event_type.h"

char* OpenAPI_condition_event_type_ToString(OpenAPI_condition_event_type_e condition_event_type)
{
    const char *condition_event_typeArray[] =  { "NULL", "NF_ADDED", "NF_REMOVED" };
    size_t sizeofArray = sizeof(condition_event_typeArray) / sizeof(condition_event_typeArray[0]);
    if (condition_event_type < sizeofArray)
        return (char *)condition_event_typeArray[condition_event_type];
    else
        return (char *)"Unknown";
}

OpenAPI_condition_event_type_e OpenAPI_condition_event_type_FromString(char* condition_event_type)
{
    int stringToReturn = 0;
    const char *condition_event_typeArray[] =  { "NULL", "NF_ADDED", "NF_REMOVED" };
    size_t sizeofArray = sizeof(condition_event_typeArray) / sizeof(condition_event_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(condition_event_type, condition_event_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

