
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "restriction_type.h"

char* OpenAPI_restriction_type_ToString(OpenAPI_restriction_type_e restriction_type)
{
    const char *restriction_typeArray[] =  { "NULL", "ALLOWED_AREAS", "NOT_ALLOWED_AREAS" };
    size_t sizeofArray = sizeof(restriction_typeArray) / sizeof(restriction_typeArray[0]);
    if (restriction_type < sizeofArray)
        return (char *)restriction_typeArray[restriction_type];
    else
        return (char *)"Unknown";
}

OpenAPI_restriction_type_e OpenAPI_restriction_type_FromString(char* restriction_type)
{
    int stringToReturn = 0;
    const char *restriction_typeArray[] =  { "NULL", "ALLOWED_AREAS", "NOT_ALLOWED_AREAS" };
    size_t sizeofArray = sizeof(restriction_typeArray) / sizeof(restriction_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(restriction_type, restriction_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

