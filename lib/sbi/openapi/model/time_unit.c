
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_unit.h"

char* OpenAPI_time_unit_ToString(OpenAPI_time_unit_e time_unit)
{
    const char *time_unitArray[] =  { "NULL", "MINUTE", "HOUR", "DAY" };
    size_t sizeofArray = sizeof(time_unitArray) / sizeof(time_unitArray[0]);
    if (time_unit < sizeofArray)
        return (char *)time_unitArray[time_unit];
    else
        return (char *)"Unknown";
}

OpenAPI_time_unit_e OpenAPI_time_unit_FromString(char* time_unit)
{
    int stringToReturn = 0;
    const char *time_unitArray[] =  { "NULL", "MINUTE", "HOUR", "DAY" };
    size_t sizeofArray = sizeof(time_unitArray) / sizeof(time_unitArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(time_unit, time_unitArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

