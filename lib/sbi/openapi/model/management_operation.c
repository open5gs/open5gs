
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "management_operation.h"

char* OpenAPI_management_operation_ToString(OpenAPI_management_operation_e management_operation)
{
    const char *management_operationArray[] =  { "NULL", "CreateMOI", "ModifyMOIAttributes", "DeleteMOI", "CREATE_MOI", "MODIFY_MOI_ATTR", "DELETE_MOI", "NOTIFY_MOI_CREATION", "NOTIFY_MOI_ATTR_CHANGE", "NOTIFY_MOI_DELETION" };
    size_t sizeofArray = sizeof(management_operationArray) / sizeof(management_operationArray[0]);
    if (management_operation < sizeofArray)
        return (char *)management_operationArray[management_operation];
    else
        return (char *)"Unknown";
}

OpenAPI_management_operation_e OpenAPI_management_operation_FromString(char* management_operation)
{
    int stringToReturn = 0;
    const char *management_operationArray[] =  { "NULL", "CreateMOI", "ModifyMOIAttributes", "DeleteMOI", "CREATE_MOI", "MODIFY_MOI_ATTR", "DELETE_MOI", "NOTIFY_MOI_CREATION", "NOTIFY_MOI_ATTR_CHANGE", "NOTIFY_MOI_DELETION" };
    size_t sizeofArray = sizeof(management_operationArray) / sizeof(management_operationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(management_operation, management_operationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

