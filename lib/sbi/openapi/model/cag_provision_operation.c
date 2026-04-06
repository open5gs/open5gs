
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cag_provision_operation.h"

char* OpenAPI_cag_provision_operation_ToString(OpenAPI_cag_provision_operation_e cag_provision_operation)
{
    const char *cag_provision_operationArray[] =  { "NULL", "ADD", "REPLACE", "REMOVE" };
    size_t sizeofArray = sizeof(cag_provision_operationArray) / sizeof(cag_provision_operationArray[0]);
    if (cag_provision_operation < sizeofArray)
        return (char *)cag_provision_operationArray[cag_provision_operation];
    else
        return (char *)"Unknown";
}

OpenAPI_cag_provision_operation_e OpenAPI_cag_provision_operation_FromString(char* cag_provision_operation)
{
    int stringToReturn = 0;
    const char *cag_provision_operationArray[] =  { "NULL", "ADD", "REPLACE", "REMOVE" };
    size_t sizeofArray = sizeof(cag_provision_operationArray) / sizeof(cag_provision_operationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(cag_provision_operation, cag_provision_operationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

