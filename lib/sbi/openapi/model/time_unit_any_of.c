
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_unit_any_of.h"

char* OpenAPI_time_unit_any_of_ToString(OpenAPI_time_unit_any_of_e time_unit_any_of)
{
    const char *time_unit_any_ofArray[] =  { "NULL", "MINUTE", "HOUR", "DAY" };
    size_t sizeofArray = sizeof(time_unit_any_ofArray) / sizeof(time_unit_any_ofArray[0]);
    if (time_unit_any_of < sizeofArray)
        return (char *)time_unit_any_ofArray[time_unit_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_time_unit_any_of_e OpenAPI_time_unit_any_of_FromString(char* time_unit_any_of)
{
    int stringToReturn = 0;
    const char *time_unit_any_ofArray[] =  { "NULL", "MINUTE", "HOUR", "DAY" };
    size_t sizeofArray = sizeof(time_unit_any_ofArray) / sizeof(time_unit_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(time_unit_any_of, time_unit_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

