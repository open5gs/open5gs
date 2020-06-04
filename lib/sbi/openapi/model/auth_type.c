
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "auth_type.h"

char* OpenAPI_auth_type_ToString(OpenAPI_auth_type_e auth_type)
{
    const char *auth_typeArray[] =  { "NULL", "5G_AKA", "EAP_AKA_PRIME", "EAP_TLS" };
    size_t sizeofArray = sizeof(auth_typeArray) / sizeof(auth_typeArray[0]);
    if (auth_type < sizeofArray)
        return (char *)auth_typeArray[auth_type];
    else
        return (char *)"Unknown";
}

OpenAPI_auth_type_e OpenAPI_auth_type_FromString(char* auth_type)
{
    int stringToReturn = 0;
    const char *auth_typeArray[] =  { "NULL", "5G_AKA", "EAP_AKA_PRIME", "EAP_TLS" };
    size_t sizeofArray = sizeof(auth_typeArray) / sizeof(auth_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(auth_type, auth_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

