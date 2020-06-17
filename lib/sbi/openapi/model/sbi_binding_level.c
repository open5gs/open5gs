
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sbi_binding_level.h"

char* OpenAPI_sbi_binding_level_ToString(OpenAPI_sbi_binding_level_e sbi_binding_level)
{
    const char *sbi_binding_levelArray[] =  { "NULL", "NF_INSTANCE_BINDING", "NF_SET_BINDING", "NF_SERVICE_SET_BINDING" };
    size_t sizeofArray = sizeof(sbi_binding_levelArray) / sizeof(sbi_binding_levelArray[0]);
    if (sbi_binding_level < sizeofArray)
        return (char *)sbi_binding_levelArray[sbi_binding_level];
    else
        return (char *)"Unknown";
}

OpenAPI_sbi_binding_level_e OpenAPI_sbi_binding_level_FromString(char* sbi_binding_level)
{
    int stringToReturn = 0;
    const char *sbi_binding_levelArray[] =  { "NULL", "NF_INSTANCE_BINDING", "NF_SET_BINDING", "NF_SERVICE_SET_BINDING" };
    size_t sizeofArray = sizeof(sbi_binding_levelArray) / sizeof(sbi_binding_levelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sbi_binding_level, sbi_binding_levelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

