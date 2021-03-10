
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_auth.h"

char* OpenAPI_ue_auth_ToString(OpenAPI_ue_auth_e ue_auth)
{
    const char *ue_authArray[] =  { "NULL", "AUTHORIZED", "NOT_AUTHORIZED" };
    size_t sizeofArray = sizeof(ue_authArray) / sizeof(ue_authArray[0]);
    if (ue_auth < sizeofArray)
        return (char *)ue_authArray[ue_auth];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_auth_e OpenAPI_ue_auth_FromString(char* ue_auth)
{
    int stringToReturn = 0;
    const char *ue_authArray[] =  { "NULL", "AUTHORIZED", "NOT_AUTHORIZED" };
    size_t sizeofArray = sizeof(ue_authArray) / sizeof(ue_authArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_auth, ue_authArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

