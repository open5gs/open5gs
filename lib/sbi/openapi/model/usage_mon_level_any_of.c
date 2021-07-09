
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_mon_level_any_of.h"

char* OpenAPI_usage_mon_level_any_of_ToString(OpenAPI_usage_mon_level_any_of_e usage_mon_level_any_of)
{
    const char *usage_mon_level_any_ofArray[] =  { "NULL", "SESSION_LEVEL", "SERVICE_LEVEL" };
    size_t sizeofArray = sizeof(usage_mon_level_any_ofArray) / sizeof(usage_mon_level_any_ofArray[0]);
    if (usage_mon_level_any_of < sizeofArray)
        return (char *)usage_mon_level_any_ofArray[usage_mon_level_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_usage_mon_level_any_of_e OpenAPI_usage_mon_level_any_of_FromString(char* usage_mon_level_any_of)
{
    int stringToReturn = 0;
    const char *usage_mon_level_any_ofArray[] =  { "NULL", "SESSION_LEVEL", "SERVICE_LEVEL" };
    size_t sizeofArray = sizeof(usage_mon_level_any_ofArray) / sizeof(usage_mon_level_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(usage_mon_level_any_of, usage_mon_level_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

