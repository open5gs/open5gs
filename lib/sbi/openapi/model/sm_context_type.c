
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_type.h"

char* OpenAPI_sm_context_type_ToString(OpenAPI_sm_context_type_e sm_context_type)
{
    const char *sm_context_typeArray[] =  { "NULL", "EPS_PDN_CONNECTION", "SM_CONTEXT" };
    size_t sizeofArray = sizeof(sm_context_typeArray) / sizeof(sm_context_typeArray[0]);
    if (sm_context_type < sizeofArray)
        return (char *)sm_context_typeArray[sm_context_type];
    else
        return (char *)"Unknown";
}

OpenAPI_sm_context_type_e OpenAPI_sm_context_type_FromString(char* sm_context_type)
{
    int stringToReturn = 0;
    const char *sm_context_typeArray[] =  { "NULL", "EPS_PDN_CONNECTION", "SM_CONTEXT" };
    size_t sizeofArray = sizeof(sm_context_typeArray) / sizeof(sm_context_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sm_context_type, sm_context_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

