
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sponsoring_status.h"

char* OpenAPI_sponsoring_status_ToString(OpenAPI_sponsoring_status_e sponsoring_status)
{
    const char *sponsoring_statusArray[] =  { "NULL", "SPONSOR_DISABLED", "SPONSOR_ENABLED" };
    size_t sizeofArray = sizeof(sponsoring_statusArray) / sizeof(sponsoring_statusArray[0]);
    if (sponsoring_status < sizeofArray)
        return (char *)sponsoring_statusArray[sponsoring_status];
    else
        return (char *)"Unknown";
}

OpenAPI_sponsoring_status_e OpenAPI_sponsoring_status_FromString(char* sponsoring_status)
{
    int stringToReturn = 0;
    const char *sponsoring_statusArray[] =  { "NULL", "SPONSOR_DISABLED", "SPONSOR_ENABLED" };
    size_t sizeofArray = sizeof(sponsoring_statusArray) / sizeof(sponsoring_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sponsoring_status, sponsoring_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

