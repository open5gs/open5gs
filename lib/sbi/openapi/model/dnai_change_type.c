
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnai_change_type.h"

char* OpenAPI_dnai_change_type_ToString(OpenAPI_dnai_change_type_e dnai_change_type)
{
    const char *dnai_change_typeArray[] =  { "NULL", "EARLY", "EARLY_LATE", "LATE" };
    size_t sizeofArray = sizeof(dnai_change_typeArray) / sizeof(dnai_change_typeArray[0]);
    if (dnai_change_type < sizeofArray)
        return (char *)dnai_change_typeArray[dnai_change_type];
    else
        return (char *)"Unknown";
}

OpenAPI_dnai_change_type_e OpenAPI_dnai_change_type_FromString(char* dnai_change_type)
{
    int stringToReturn = 0;
    const char *dnai_change_typeArray[] =  { "NULL", "EARLY", "EARLY_LATE", "LATE" };
    size_t sizeofArray = sizeof(dnai_change_typeArray) / sizeof(dnai_change_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dnai_change_type, dnai_change_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

