
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "one_time_event_type.h"

char* OpenAPI_one_time_event_type_ToString(OpenAPI_one_time_event_type_e one_time_event_type)
{
    const char *one_time_event_typeArray[] =  { "NULL", "IEC", "PEC" };
    size_t sizeofArray = sizeof(one_time_event_typeArray) / sizeof(one_time_event_typeArray[0]);
    if (one_time_event_type < sizeofArray)
        return (char *)one_time_event_typeArray[one_time_event_type];
    else
        return (char *)"Unknown";
}

OpenAPI_one_time_event_type_e OpenAPI_one_time_event_type_FromString(char* one_time_event_type)
{
    int stringToReturn = 0;
    const char *one_time_event_typeArray[] =  { "NULL", "IEC", "PEC" };
    size_t sizeofArray = sizeof(one_time_event_typeArray) / sizeof(one_time_event_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(one_time_event_type, one_time_event_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

