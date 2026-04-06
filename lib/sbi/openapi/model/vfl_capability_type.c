
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vfl_capability_type.h"

char* OpenAPI_vfl_capability_type_ToString(OpenAPI_vfl_capability_type_e vfl_capability_type)
{
    const char *vfl_capability_typeArray[] =  { "NULL", "VFL_SERVER", "VFL_CLIENT", "VFL_SERVER_AND_CLIENT" };
    size_t sizeofArray = sizeof(vfl_capability_typeArray) / sizeof(vfl_capability_typeArray[0]);
    if (vfl_capability_type < sizeofArray)
        return (char *)vfl_capability_typeArray[vfl_capability_type];
    else
        return (char *)"Unknown";
}

OpenAPI_vfl_capability_type_e OpenAPI_vfl_capability_type_FromString(char* vfl_capability_type)
{
    int stringToReturn = 0;
    const char *vfl_capability_typeArray[] =  { "NULL", "VFL_SERVER", "VFL_CLIENT", "VFL_SERVER_AND_CLIENT" };
    size_t sizeofArray = sizeof(vfl_capability_typeArray) / sizeof(vfl_capability_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(vfl_capability_type, vfl_capability_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

