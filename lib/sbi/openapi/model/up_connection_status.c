
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_connection_status.h"

char* OpenAPI_up_connection_status_ToString(OpenAPI_up_connection_status_e up_connection_status)
{
    const char *up_connection_statusArray[] =  { "NULL", "ESTABLISHED", "RELEASED", "MOVE" };
    size_t sizeofArray = sizeof(up_connection_statusArray) / sizeof(up_connection_statusArray[0]);
    if (up_connection_status < sizeofArray)
        return (char *)up_connection_statusArray[up_connection_status];
    else
        return (char *)"Unknown";
}

OpenAPI_up_connection_status_e OpenAPI_up_connection_status_FromString(char* up_connection_status)
{
    int stringToReturn = 0;
    const char *up_connection_statusArray[] =  { "NULL", "ESTABLISHED", "RELEASED", "MOVE" };
    size_t sizeofArray = sizeof(up_connection_statusArray) / sizeof(up_connection_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(up_connection_status, up_connection_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

