
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_distribution_method.h"

char* OpenAPI_time_distribution_method_ToString(OpenAPI_time_distribution_method_e time_distribution_method)
{
    const char *time_distribution_methodArray[] =  { "NULL", "GPTP", "ASTI" };
    size_t sizeofArray = sizeof(time_distribution_methodArray) / sizeof(time_distribution_methodArray[0]);
    if (time_distribution_method < sizeofArray)
        return (char *)time_distribution_methodArray[time_distribution_method];
    else
        return (char *)"Unknown";
}

OpenAPI_time_distribution_method_e OpenAPI_time_distribution_method_FromString(char* time_distribution_method)
{
    int stringToReturn = 0;
    const char *time_distribution_methodArray[] =  { "NULL", "GPTP", "ASTI" };
    size_t sizeofArray = sizeof(time_distribution_methodArray) / sizeof(time_distribution_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(time_distribution_method, time_distribution_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

