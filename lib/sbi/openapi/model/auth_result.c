
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "auth_result.h"

char* OpenAPI_auth_result_ToString(OpenAPI_auth_result_e auth_result)
{
    const char *auth_resultArray[] =  { "NULL", "AUTHENTICATION_SUCCESS", "AUTHENTICATION_FAILURE", "AUTHENTICATION_ONGOING" };
    size_t sizeofArray = sizeof(auth_resultArray) / sizeof(auth_resultArray[0]);
    if (auth_result < sizeofArray)
        return (char *)auth_resultArray[auth_result];
    else
        return (char *)"Unknown";
}

OpenAPI_auth_result_e OpenAPI_auth_result_FromString(char* auth_result)
{
    int stringToReturn = 0;
    const char *auth_resultArray[] =  { "NULL", "AUTHENTICATION_SUCCESS", "AUTHENTICATION_FAILURE", "AUTHENTICATION_ONGOING" };
    size_t sizeofArray = sizeof(auth_resultArray) / sizeof(auth_resultArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(auth_result, auth_resultArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

