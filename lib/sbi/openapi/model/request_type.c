
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "request_type.h"

char* OpenAPI_request_type_ToString(OpenAPI_request_type_e request_type)
{
    const char *request_typeArray[] =  { "NULL", "INITIAL_REQUEST", "EXISTING_PDU_SESSION", "INITIAL_EMERGENCY_REQUEST", "EXISTING_EMERGENCY_PDU_SESSION" };
    size_t sizeofArray = sizeof(request_typeArray) / sizeof(request_typeArray[0]);
    if (request_type < sizeofArray)
        return (char *)request_typeArray[request_type];
    else
        return (char *)"Unknown";
}

OpenAPI_request_type_e OpenAPI_request_type_FromString(char* request_type)
{
    int stringToReturn = 0;
    const char *request_typeArray[] =  { "NULL", "INITIAL_REQUEST", "EXISTING_PDU_SESSION", "INITIAL_EMERGENCY_REQUEST", "EXISTING_EMERGENCY_PDU_SESSION" };
    size_t sizeofArray = sizeof(request_typeArray) / sizeof(request_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(request_type, request_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

