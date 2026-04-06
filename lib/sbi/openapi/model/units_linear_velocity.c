
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "units_linear_velocity.h"

char* OpenAPI_units_linear_velocity_ToString(OpenAPI_units_linear_velocity_e units_linear_velocity)
{
    const char *units_linear_velocityArray[] =  { "NULL", "MPERS", "CMPERS" };
    size_t sizeofArray = sizeof(units_linear_velocityArray) / sizeof(units_linear_velocityArray[0]);
    if (units_linear_velocity < sizeofArray)
        return (char *)units_linear_velocityArray[units_linear_velocity];
    else
        return (char *)"Unknown";
}

OpenAPI_units_linear_velocity_e OpenAPI_units_linear_velocity_FromString(char* units_linear_velocity)
{
    int stringToReturn = 0;
    const char *units_linear_velocityArray[] =  { "NULL", "MPERS", "CMPERS" };
    size_t sizeofArray = sizeof(units_linear_velocityArray) / sizeof(units_linear_velocityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(units_linear_velocity, units_linear_velocityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

