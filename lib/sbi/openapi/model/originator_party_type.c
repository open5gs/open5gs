
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "originator_party_type.h"

char* OpenAPI_originator_party_type_ToString(OpenAPI_originator_party_type_e originator_party_type)
{
    const char *originator_party_typeArray[] =  { "NULL", "CALLING", "CALLED" };
    size_t sizeofArray = sizeof(originator_party_typeArray) / sizeof(originator_party_typeArray[0]);
    if (originator_party_type < sizeofArray)
        return (char *)originator_party_typeArray[originator_party_type];
    else
        return (char *)"Unknown";
}

OpenAPI_originator_party_type_e OpenAPI_originator_party_type_FromString(char* originator_party_type)
{
    int stringToReturn = 0;
    const char *originator_party_typeArray[] =  { "NULL", "CALLING", "CALLED" };
    size_t sizeofArray = sizeof(originator_party_typeArray) / sizeof(originator_party_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(originator_party_type, originator_party_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

