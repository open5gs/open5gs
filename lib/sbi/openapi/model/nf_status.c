
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_status.h"

char* OpenAPI_nf_status_ToString(OpenAPI_nf_status_e nf_status)
{
    const char *nf_statusArray[] =  { "NULL", "REGISTERED", "SUSPENDED", "UNDISCOVERABLE" };
    size_t sizeofArray = sizeof(nf_statusArray) / sizeof(nf_statusArray[0]);
    if (nf_status < sizeofArray)
        return (char *)nf_statusArray[nf_status];
    else
        return (char *)"Unknown";
}

OpenAPI_nf_status_e OpenAPI_nf_status_FromString(char* nf_status)
{
    int stringToReturn = 0;
    const char *nf_statusArray[] =  { "NULL", "REGISTERED", "SUSPENDED", "UNDISCOVERABLE" };
    size_t sizeofArray = sizeof(nf_statusArray) / sizeof(nf_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nf_status, nf_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

