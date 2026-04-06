
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "activation_status.h"

char* OpenAPI_activation_status_ToString(OpenAPI_activation_status_e activation_status)
{
    const char *activation_statusArray[] =  { "NULL", "ACTIVE", "NOT_ACTIVE" };
    size_t sizeofArray = sizeof(activation_statusArray) / sizeof(activation_statusArray[0]);
    if (activation_status < sizeofArray)
        return (char *)activation_statusArray[activation_status];
    else
        return (char *)"Unknown";
}

OpenAPI_activation_status_e OpenAPI_activation_status_FromString(char* activation_status)
{
    int stringToReturn = 0;
    const char *activation_statusArray[] =  { "NULL", "ACTIVE", "NOT_ACTIVE" };
    size_t sizeofArray = sizeof(activation_statusArray) / sizeof(activation_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(activation_status, activation_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

