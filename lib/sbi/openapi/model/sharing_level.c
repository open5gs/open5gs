
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sharing_level.h"

char* OpenAPI_sharing_level_ToString(OpenAPI_sharing_level_e sharing_level)
{
    const char *sharing_levelArray[] =  { "NULL", "SHARED", "NON_SHARED" };
    size_t sizeofArray = sizeof(sharing_levelArray) / sizeof(sharing_levelArray[0]);
    if (sharing_level < sizeofArray)
        return (char *)sharing_levelArray[sharing_level];
    else
        return (char *)"Unknown";
}

OpenAPI_sharing_level_e OpenAPI_sharing_level_FromString(char* sharing_level)
{
    int stringToReturn = 0;
    const char *sharing_levelArray[] =  { "NULL", "SHARED", "NON_SHARED" };
    size_t sizeofArray = sizeof(sharing_levelArray) / sizeof(sharing_levelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sharing_level, sharing_levelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

