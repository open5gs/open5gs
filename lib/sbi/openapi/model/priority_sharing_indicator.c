
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "priority_sharing_indicator.h"

char* OpenAPI_priority_sharing_indicator_ToString(OpenAPI_priority_sharing_indicator_e priority_sharing_indicator)
{
    const char *priority_sharing_indicatorArray[] =  { "NULL", "ENABLED", "DISABLED" };
    size_t sizeofArray = sizeof(priority_sharing_indicatorArray) / sizeof(priority_sharing_indicatorArray[0]);
    if (priority_sharing_indicator < sizeofArray)
        return (char *)priority_sharing_indicatorArray[priority_sharing_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_priority_sharing_indicator_e OpenAPI_priority_sharing_indicator_FromString(char* priority_sharing_indicator)
{
    int stringToReturn = 0;
    const char *priority_sharing_indicatorArray[] =  { "NULL", "ENABLED", "DISABLED" };
    size_t sizeofArray = sizeof(priority_sharing_indicatorArray) / sizeof(priority_sharing_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(priority_sharing_indicator, priority_sharing_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

