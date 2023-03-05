
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ie_type_any_of.h"

char* OpenAPI_ie_type_any_of_ToString(OpenAPI_ie_type_any_of_e ie_type_any_of)
{
    const char *ie_type_any_ofArray[] =  { "NULL", "UEID", "LOCATION", "KEY_MATERIAL", "AUTHENTICATION_MATERIAL", "AUTHORIZATION_TOKEN", "OTHER", "NONSENSITIVE" };
    size_t sizeofArray = sizeof(ie_type_any_ofArray) / sizeof(ie_type_any_ofArray[0]);
    if (ie_type_any_of < sizeofArray)
        return (char *)ie_type_any_ofArray[ie_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_ie_type_any_of_e OpenAPI_ie_type_any_of_FromString(char* ie_type_any_of)
{
    int stringToReturn = 0;
    const char *ie_type_any_ofArray[] =  { "NULL", "UEID", "LOCATION", "KEY_MATERIAL", "AUTHENTICATION_MATERIAL", "AUTHORIZATION_TOKEN", "OTHER", "NONSENSITIVE" };
    size_t sizeofArray = sizeof(ie_type_any_ofArray) / sizeof(ie_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ie_type_any_of, ie_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

