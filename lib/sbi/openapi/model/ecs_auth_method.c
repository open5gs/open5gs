
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecs_auth_method.h"

char* OpenAPI_ecs_auth_method_ToString(OpenAPI_ecs_auth_method_e ecs_auth_method)
{
    const char *ecs_auth_methodArray[] =  { "NULL", "TLS_CLIENT_SERVER_CERTIFICATE", "TLS_AKMA", "TLS_GBA" };
    size_t sizeofArray = sizeof(ecs_auth_methodArray) / sizeof(ecs_auth_methodArray[0]);
    if (ecs_auth_method < sizeofArray)
        return (char *)ecs_auth_methodArray[ecs_auth_method];
    else
        return (char *)"Unknown";
}

OpenAPI_ecs_auth_method_e OpenAPI_ecs_auth_method_FromString(char* ecs_auth_method)
{
    int stringToReturn = 0;
    const char *ecs_auth_methodArray[] =  { "NULL", "TLS_CLIENT_SERVER_CERTIFICATE", "TLS_AKMA", "TLS_GBA" };
    size_t sizeofArray = sizeof(ecs_auth_methodArray) / sizeof(ecs_auth_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ecs_auth_method, ecs_auth_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

