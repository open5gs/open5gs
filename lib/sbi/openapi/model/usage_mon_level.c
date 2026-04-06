
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_mon_level.h"

char* OpenAPI_usage_mon_level_ToString(OpenAPI_usage_mon_level_e usage_mon_level)
{
    const char *usage_mon_levelArray[] =  { "NULL", "SESSION_LEVEL", "SERVICE_LEVEL" };
    size_t sizeofArray = sizeof(usage_mon_levelArray) / sizeof(usage_mon_levelArray[0]);
    if (usage_mon_level < sizeofArray)
        return (char *)usage_mon_levelArray[usage_mon_level];
    else
        return (char *)"Unknown";
}

OpenAPI_usage_mon_level_e OpenAPI_usage_mon_level_FromString(char* usage_mon_level)
{
    int stringToReturn = 0;
    const char *usage_mon_levelArray[] =  { "NULL", "SESSION_LEVEL", "SERVICE_LEVEL" };
    size_t sizeofArray = sizeof(usage_mon_levelArray) / sizeof(usage_mon_levelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(usage_mon_level, usage_mon_levelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

