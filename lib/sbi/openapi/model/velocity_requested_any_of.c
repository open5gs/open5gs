
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "velocity_requested_any_of.h"

char* OpenAPI_velocity_requested_any_of_ToString(OpenAPI_velocity_requested_any_of_e velocity_requested_any_of)
{
    const char *velocity_requested_any_ofArray[] =  { "NULL", "VELOCITY_IS_NOT_REQUESTED", "VELOCITY_IS_REQUESTED" };
    size_t sizeofArray = sizeof(velocity_requested_any_ofArray) / sizeof(velocity_requested_any_ofArray[0]);
    if (velocity_requested_any_of < sizeofArray)
        return (char *)velocity_requested_any_ofArray[velocity_requested_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_velocity_requested_any_of_e OpenAPI_velocity_requested_any_of_FromString(char* velocity_requested_any_of)
{
    int stringToReturn = 0;
    const char *velocity_requested_any_ofArray[] =  { "NULL", "VELOCITY_IS_NOT_REQUESTED", "VELOCITY_IS_REQUESTED" };
    size_t sizeofArray = sizeof(velocity_requested_any_ofArray) / sizeof(velocity_requested_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(velocity_requested_any_of, velocity_requested_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

