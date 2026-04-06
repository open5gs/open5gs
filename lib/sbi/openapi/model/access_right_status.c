
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_right_status.h"

char* OpenAPI_access_right_status_ToString(OpenAPI_access_right_status_e access_right_status)
{
    const char *access_right_statusArray[] =  { "NULL", "FULLY_ALLOWED", "PREVIEW_ALLOWED", "NO_ALLOWED" };
    size_t sizeofArray = sizeof(access_right_statusArray) / sizeof(access_right_statusArray[0]);
    if (access_right_status < sizeofArray)
        return (char *)access_right_statusArray[access_right_status];
    else
        return (char *)"Unknown";
}

OpenAPI_access_right_status_e OpenAPI_access_right_status_FromString(char* access_right_status)
{
    int stringToReturn = 0;
    const char *access_right_statusArray[] =  { "NULL", "FULLY_ALLOWED", "PREVIEW_ALLOWED", "NO_ALLOWED" };
    size_t sizeofArray = sizeof(access_right_statusArray) / sizeof(access_right_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(access_right_status, access_right_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

