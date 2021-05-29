
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "binding_level.h"

char* OpenAPI_binding_level_ToString(OpenAPI_binding_level_e binding_level)
{
    const char *binding_levelArray[] =  { "NULL", "NF_SET", "NF_INSTANCE" };
    size_t sizeofArray = sizeof(binding_levelArray) / sizeof(binding_levelArray[0]);
    if (binding_level < sizeofArray)
        return (char *)binding_levelArray[binding_level];
    else
        return (char *)"Unknown";
}

OpenAPI_binding_level_e OpenAPI_binding_level_FromString(char* binding_level)
{
    int stringToReturn = 0;
    const char *binding_levelArray[] =  { "NULL", "NF_SET", "NF_INSTANCE" };
    size_t sizeofArray = sizeof(binding_levelArray) / sizeof(binding_levelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(binding_level, binding_levelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

