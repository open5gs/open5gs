
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uuaa_mm_status.h"

char* OpenAPI_uuaa_mm_status_ToString(OpenAPI_uuaa_mm_status_e uuaa_mm_status)
{
    const char *uuaa_mm_statusArray[] =  { "NULL", "SUCCESS", "PENDING", "FAILED" };
    size_t sizeofArray = sizeof(uuaa_mm_statusArray) / sizeof(uuaa_mm_statusArray[0]);
    if (uuaa_mm_status < sizeofArray)
        return (char *)uuaa_mm_statusArray[uuaa_mm_status];
    else
        return (char *)"Unknown";
}

OpenAPI_uuaa_mm_status_e OpenAPI_uuaa_mm_status_FromString(char* uuaa_mm_status)
{
    int stringToReturn = 0;
    const char *uuaa_mm_statusArray[] =  { "NULL", "SUCCESS", "PENDING", "FAILED" };
    size_t sizeofArray = sizeof(uuaa_mm_statusArray) / sizeof(uuaa_mm_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(uuaa_mm_status, uuaa_mm_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

