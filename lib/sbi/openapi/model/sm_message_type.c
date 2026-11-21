
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_message_type.h"

char* OpenAPI_sm_message_type_ToString(OpenAPI_sm_message_type_e sm_message_type)
{
    const char *sm_message_typeArray[] =  { "NULL", "SUBMISSION", "DELIVERY_REPORT", "SM_SERVICE_REQUEST", "DELIVERY" };
    size_t sizeofArray = sizeof(sm_message_typeArray) / sizeof(sm_message_typeArray[0]);
    if (sm_message_type < sizeofArray)
        return (char *)sm_message_typeArray[sm_message_type];
    else
        return (char *)"Unknown";
}

OpenAPI_sm_message_type_e OpenAPI_sm_message_type_FromString(char* sm_message_type)
{
    int stringToReturn = 0;
    const char *sm_message_typeArray[] =  { "NULL", "SUBMISSION", "DELIVERY_REPORT", "SM_SERVICE_REQUEST", "DELIVERY" };
    size_t sizeofArray = sizeof(sm_message_typeArray) / sizeof(sm_message_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sm_message_type, sm_message_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

