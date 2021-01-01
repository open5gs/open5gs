
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steer_mode_value.h"

char* OpenAPI_steer_mode_value_ToString(OpenAPI_steer_mode_value_e steer_mode_value)
{
    const char *steer_mode_valueArray[] =  { "NULL", "ACTIVE_STANDBY", "LOAD_BALANCING", "SMALLEST_DELAY", "PRIORITY_BASED" };
    size_t sizeofArray = sizeof(steer_mode_valueArray) / sizeof(steer_mode_valueArray[0]);
    if (steer_mode_value < sizeofArray)
        return (char *)steer_mode_valueArray[steer_mode_value];
    else
        return (char *)"Unknown";
}

OpenAPI_steer_mode_value_e OpenAPI_steer_mode_value_FromString(char* steer_mode_value)
{
    int stringToReturn = 0;
    const char *steer_mode_valueArray[] =  { "NULL", "ACTIVE_STANDBY", "LOAD_BALANCING", "SMALLEST_DELAY", "PRIORITY_BASED" };
    size_t sizeofArray = sizeof(steer_mode_valueArray) / sizeof(steer_mode_valueArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(steer_mode_value, steer_mode_valueArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

