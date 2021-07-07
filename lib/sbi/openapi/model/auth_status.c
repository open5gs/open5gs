
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "auth_status.h"

char* OpenAPI_auth_status_ToString(OpenAPI_auth_status_e auth_status)
{
    const char *auth_statusArray[] =  { "NULL", "EAP_SUCCESS", "EAP_FAILURE", "PENDING" };
    size_t sizeofArray = sizeof(auth_statusArray) / sizeof(auth_statusArray[0]);
    if (auth_status < sizeofArray)
        return (char *)auth_statusArray[auth_status];
    else
        return (char *)"Unknown";
}

OpenAPI_auth_status_e OpenAPI_auth_status_FromString(char* auth_status)
{
    int stringToReturn = 0;
    const char *auth_statusArray[] =  { "NULL", "EAP_SUCCESS", "EAP_FAILURE", "PENDING" };
    size_t sizeofArray = sizeof(auth_statusArray) / sizeof(auth_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(auth_status, auth_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

