
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "management_operation_status.h"

char* OpenAPI_management_operation_status_ToString(OpenAPI_management_operation_status_e management_operation_status)
{
    const char *management_operation_statusArray[] =  { "NULL", "OPERATION_SUCCEEDED", "OPERATION_FAILED" };
    size_t sizeofArray = sizeof(management_operation_statusArray) / sizeof(management_operation_statusArray[0]);
    if (management_operation_status < sizeofArray)
        return (char *)management_operation_statusArray[management_operation_status];
    else
        return (char *)"Unknown";
}

OpenAPI_management_operation_status_e OpenAPI_management_operation_status_FromString(char* management_operation_status)
{
    int stringToReturn = 0;
    const char *management_operation_statusArray[] =  { "NULL", "OPERATION_SUCCEEDED", "OPERATION_FAILED" };
    size_t sizeofArray = sizeof(management_operation_statusArray) / sizeof(management_operation_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(management_operation_status, management_operation_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

