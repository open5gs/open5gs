
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aggregation_level.h"

char* OpenAPI_aggregation_level_ToString(OpenAPI_aggregation_level_e aggregation_level)
{
    const char *aggregation_levelArray[] =  { "NULL", "UE", "AOI" };
    size_t sizeofArray = sizeof(aggregation_levelArray) / sizeof(aggregation_levelArray[0]);
    if (aggregation_level < sizeofArray)
        return (char *)aggregation_levelArray[aggregation_level];
    else
        return (char *)"Unknown";
}

OpenAPI_aggregation_level_e OpenAPI_aggregation_level_FromString(char* aggregation_level)
{
    int stringToReturn = 0;
    const char *aggregation_levelArray[] =  { "NULL", "UE", "AOI" };
    size_t sizeofArray = sizeof(aggregation_levelArray) / sizeof(aggregation_levelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(aggregation_level, aggregation_levelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

