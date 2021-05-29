
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_info_status.h"

char* OpenAPI_service_info_status_ToString(OpenAPI_service_info_status_e service_info_status)
{
    const char *service_info_statusArray[] =  { "NULL", "FINAL", "PRELIMINARY" };
    size_t sizeofArray = sizeof(service_info_statusArray) / sizeof(service_info_statusArray[0]);
    if (service_info_status < sizeofArray)
        return (char *)service_info_statusArray[service_info_status];
    else
        return (char *)"Unknown";
}

OpenAPI_service_info_status_e OpenAPI_service_info_status_FromString(char* service_info_status)
{
    int stringToReturn = 0;
    const char *service_info_statusArray[] =  { "NULL", "FINAL", "PRELIMINARY" };
    size_t sizeofArray = sizeof(service_info_statusArray) / sizeof(service_info_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(service_info_status, service_info_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

