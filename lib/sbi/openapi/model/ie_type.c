
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ie_type.h"

char* OpenAPI_ie_type_ToString(OpenAPI_ie_type_e ie_type)
{
    const char *ie_typeArray[] =  { "NULL", "UEID", "LOCATION", "KEY_MATERIAL", "AUTHENTICATION_MATERIAL", "AUTHORIZATION_TOKEN", "OTHER", "NONSENSITIVE" };
    size_t sizeofArray = sizeof(ie_typeArray) / sizeof(ie_typeArray[0]);
    if (ie_type < sizeofArray)
        return (char *)ie_typeArray[ie_type];
    else
        return (char *)"Unknown";
}

OpenAPI_ie_type_e OpenAPI_ie_type_FromString(char* ie_type)
{
    int stringToReturn = 0;
    const char *ie_typeArray[] =  { "NULL", "UEID", "LOCATION", "KEY_MATERIAL", "AUTHENTICATION_MATERIAL", "AUTHORIZATION_TOKEN", "OTHER", "NONSENSITIVE" };
    size_t sizeofArray = sizeof(ie_typeArray) / sizeof(ie_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ie_type, ie_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

