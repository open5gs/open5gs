
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_type.h"

char* OpenAPI_access_type_ToString(OpenAPI_access_type_e access_type)
{
    const char *access_typeArray[] =  { "NULL", "3GPP_ACCESS", "NON_3GPP_ACCESS" };
    size_t sizeofArray = sizeof(access_typeArray) / sizeof(access_typeArray[0]);
    if (access_type < sizeofArray)
        return (char *)access_typeArray[access_type];
    else
        return (char *)"Unknown";
}

OpenAPI_access_type_e OpenAPI_access_type_FromString(char* access_type)
{
    int stringToReturn = 0;
    const char *access_typeArray[] =  { "NULL", "3GPP_ACCESS", "NON_3GPP_ACCESS" };
    size_t sizeofArray = sizeof(access_typeArray) / sizeof(access_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(access_type, access_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

