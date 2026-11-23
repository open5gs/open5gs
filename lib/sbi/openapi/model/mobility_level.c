
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mobility_level.h"

char* OpenAPI_mobility_level_ToString(OpenAPI_mobility_level_e mobility_level)
{
    const char *mobility_levelArray[] =  { "NULL", "STATIONARY", "NOMADIC", "RESTRICTED_MOBILITY", "FULL_MOBILITY" };
    size_t sizeofArray = sizeof(mobility_levelArray) / sizeof(mobility_levelArray[0]);
    if (mobility_level < sizeofArray)
        return (char *)mobility_levelArray[mobility_level];
    else
        return (char *)"Unknown";
}

OpenAPI_mobility_level_e OpenAPI_mobility_level_FromString(char* mobility_level)
{
    int stringToReturn = 0;
    const char *mobility_levelArray[] =  { "NULL", "STATIONARY", "NOMADIC", "RESTRICTED_MOBILITY", "FULL_MOBILITY" };
    size_t sizeofArray = sizeof(mobility_levelArray) / sizeof(mobility_levelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(mobility_level, mobility_levelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

