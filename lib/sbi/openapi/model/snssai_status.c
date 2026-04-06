
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snssai_status.h"

char* OpenAPI_snssai_status_ToString(OpenAPI_snssai_status_e snssai_status)
{
    const char *snssai_statusArray[] =  { "NULL", "AVAILABLE", "UNAVAILABLE" };
    size_t sizeofArray = sizeof(snssai_statusArray) / sizeof(snssai_statusArray[0]);
    if (snssai_status < sizeofArray)
        return (char *)snssai_statusArray[snssai_status];
    else
        return (char *)"Unknown";
}

OpenAPI_snssai_status_e OpenAPI_snssai_status_FromString(char* snssai_status)
{
    int stringToReturn = 0;
    const char *snssai_statusArray[] =  { "NULL", "AVAILABLE", "UNAVAILABLE" };
    size_t sizeofArray = sizeof(snssai_statusArray) / sizeof(snssai_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(snssai_status, snssai_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

