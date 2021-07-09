
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "periodic_communication_indicator.h"

char* OpenAPI_periodic_communication_indicator_ToString(OpenAPI_periodic_communication_indicator_e periodic_communication_indicator)
{
    const char *periodic_communication_indicatorArray[] =  { "NULL", "PIORIODICALLY", "ON_DEMAND" };
    size_t sizeofArray = sizeof(periodic_communication_indicatorArray) / sizeof(periodic_communication_indicatorArray[0]);
    if (periodic_communication_indicator < sizeofArray)
        return (char *)periodic_communication_indicatorArray[periodic_communication_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_periodic_communication_indicator_e OpenAPI_periodic_communication_indicator_FromString(char* periodic_communication_indicator)
{
    int stringToReturn = 0;
    const char *periodic_communication_indicatorArray[] =  { "NULL", "PIORIODICALLY", "ON_DEMAND" };
    size_t sizeofArray = sizeof(periodic_communication_indicatorArray) / sizeof(periodic_communication_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(periodic_communication_indicator, periodic_communication_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

