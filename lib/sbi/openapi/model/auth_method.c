
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "auth_method.h"

char* OpenAPI_auth_method_ToString(OpenAPI_auth_method_e auth_method)
{
    const char *auth_methodArray[] =  { "NULL", "5G_AKA", "EAP_AKA_PRIME", "EAP_TLS" };
    size_t sizeofArray = sizeof(auth_methodArray) / sizeof(auth_methodArray[0]);
    if (auth_method < sizeofArray)
        return (char *)auth_methodArray[auth_method];
    else
        return (char *)"Unknown";
}

OpenAPI_auth_method_e OpenAPI_auth_method_FromString(char* auth_method)
{
    int stringToReturn = 0;
    const char *auth_methodArray[] =  { "NULL", "5G_AKA", "EAP_AKA_PRIME", "EAP_TLS" };
    size_t sizeofArray = sizeof(auth_methodArray) / sizeof(auth_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(auth_method, auth_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

