
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_service_status.h"

char* OpenAPI_nf_service_status_ToString(OpenAPI_nf_service_status_e nf_service_status)
{
    const char *nf_service_statusArray[] =  { "NULL", "REGISTERED", "SUSPENDED", "UNDISCOVERABLE" };
    size_t sizeofArray = sizeof(nf_service_statusArray) / sizeof(nf_service_statusArray[0]);
    if (nf_service_status < sizeofArray)
        return (char *)nf_service_statusArray[nf_service_status];
    else
        return (char *)"Unknown";
}

OpenAPI_nf_service_status_e OpenAPI_nf_service_status_FromString(char* nf_service_status)
{
    int stringToReturn = 0;
    const char *nf_service_statusArray[] =  { "NULL", "REGISTERED", "SUSPENDED", "UNDISCOVERABLE" };
    size_t sizeofArray = sizeof(nf_service_statusArray) / sizeof(nf_service_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nf_service_status, nf_service_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

