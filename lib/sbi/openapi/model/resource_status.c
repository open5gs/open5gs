
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resource_status.h"

char* OpenAPI_resource_status_ToString(OpenAPI_resource_status_e resource_status)
{
    const char *resource_statusArray[] =  { "NULL", "RELEASED", "UNCHANGED", "TRANSFERRED", "UPDATED", "ALT_ANCHOR_SMF" };
    size_t sizeofArray = sizeof(resource_statusArray) / sizeof(resource_statusArray[0]);
    if (resource_status < sizeofArray)
        return (char *)resource_statusArray[resource_status];
    else
        return (char *)"Unknown";
}

OpenAPI_resource_status_e OpenAPI_resource_status_FromString(char* resource_status)
{
    int stringToReturn = 0;
    const char *resource_statusArray[] =  { "NULL", "RELEASED", "UNCHANGED", "TRANSFERRED", "UPDATED", "ALT_ANCHOR_SMF" };
    size_t sizeofArray = sizeof(resource_statusArray) / sizeof(resource_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(resource_status, resource_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

