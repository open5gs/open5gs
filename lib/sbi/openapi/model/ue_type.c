
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_type.h"

char* OpenAPI_ue_type_ToString(OpenAPI_ue_type_e ue_type)
{
    const char *ue_typeArray[] =  { "NULL", "AERIAL_UE" };
    size_t sizeofArray = sizeof(ue_typeArray) / sizeof(ue_typeArray[0]);
    if (ue_type < sizeofArray)
        return (char *)ue_typeArray[ue_type];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_type_e OpenAPI_ue_type_FromString(char* ue_type)
{
    int stringToReturn = 0;
    const char *ue_typeArray[] =  { "NULL", "AERIAL_UE" };
    size_t sizeofArray = sizeof(ue_typeArray) / sizeof(ue_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_type, ue_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

