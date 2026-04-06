
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "units_angular_velocity.h"

char* OpenAPI_units_angular_velocity_ToString(OpenAPI_units_angular_velocity_e units_angular_velocity)
{
    const char *units_angular_velocityArray[] =  { "NULL", "DEGPERSEC1", "DEGPERSEC01" };
    size_t sizeofArray = sizeof(units_angular_velocityArray) / sizeof(units_angular_velocityArray[0]);
    if (units_angular_velocity < sizeofArray)
        return (char *)units_angular_velocityArray[units_angular_velocity];
    else
        return (char *)"Unknown";
}

OpenAPI_units_angular_velocity_e OpenAPI_units_angular_velocity_FromString(char* units_angular_velocity)
{
    int stringToReturn = 0;
    const char *units_angular_velocityArray[] =  { "NULL", "DEGPERSEC1", "DEGPERSEC01" };
    size_t sizeofArray = sizeof(units_angular_velocityArray) / sizeof(units_angular_velocityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(units_angular_velocity, units_angular_velocityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

