
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allowed_service_operation.h"

char* OpenAPI_allowed_service_operation_ToString(OpenAPI_allowed_service_operation_e allowed_service_operation)
{
    const char *allowed_service_operationArray[] =  { "NULL", "INVENTORY", "READ", "WRITE", "PERMANENT_DISABLE" };
    size_t sizeofArray = sizeof(allowed_service_operationArray) / sizeof(allowed_service_operationArray[0]);
    if (allowed_service_operation < sizeofArray)
        return (char *)allowed_service_operationArray[allowed_service_operation];
    else
        return (char *)"Unknown";
}

OpenAPI_allowed_service_operation_e OpenAPI_allowed_service_operation_FromString(char* allowed_service_operation)
{
    int stringToReturn = 0;
    const char *allowed_service_operationArray[] =  { "NULL", "INVENTORY", "READ", "WRITE", "PERMANENT_DISABLE" };
    size_t sizeofArray = sizeof(allowed_service_operationArray) / sizeof(allowed_service_operationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(allowed_service_operation, allowed_service_operationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

