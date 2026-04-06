
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "clock_quality_detail_level.h"

char* OpenAPI_clock_quality_detail_level_ToString(OpenAPI_clock_quality_detail_level_e clock_quality_detail_level)
{
    const char *clock_quality_detail_levelArray[] =  { "NULL", "CLOCK_QUALITY_METRICS", "ACCEPT_INDICATION" };
    size_t sizeofArray = sizeof(clock_quality_detail_levelArray) / sizeof(clock_quality_detail_levelArray[0]);
    if (clock_quality_detail_level < sizeofArray)
        return (char *)clock_quality_detail_levelArray[clock_quality_detail_level];
    else
        return (char *)"Unknown";
}

OpenAPI_clock_quality_detail_level_e OpenAPI_clock_quality_detail_level_FromString(char* clock_quality_detail_level)
{
    int stringToReturn = 0;
    const char *clock_quality_detail_levelArray[] =  { "NULL", "CLOCK_QUALITY_METRICS", "ACCEPT_INDICATION" };
    size_t sizeofArray = sizeof(clock_quality_detail_levelArray) / sizeof(clock_quality_detail_levelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(clock_quality_detail_level, clock_quality_detail_levelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

