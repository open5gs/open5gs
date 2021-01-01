
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_functionality.h"

char* OpenAPI_steering_functionality_ToString(OpenAPI_steering_functionality_e steering_functionality)
{
    const char *steering_functionalityArray[] =  { "NULL", "MPTCP", "ATSSS_LL" };
    size_t sizeofArray = sizeof(steering_functionalityArray) / sizeof(steering_functionalityArray[0]);
    if (steering_functionality < sizeofArray)
        return (char *)steering_functionalityArray[steering_functionality];
    else
        return (char *)"Unknown";
}

OpenAPI_steering_functionality_e OpenAPI_steering_functionality_FromString(char* steering_functionality)
{
    int stringToReturn = 0;
    const char *steering_functionalityArray[] =  { "NULL", "MPTCP", "ATSSS_LL" };
    size_t sizeofArray = sizeof(steering_functionalityArray) / sizeof(steering_functionalityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(steering_functionality, steering_functionalityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

