
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qoe_service_type.h"

char* OpenAPI_qoe_service_type_ToString(OpenAPI_qoe_service_type_e qoe_service_type)
{
    const char *qoe_service_typeArray[] =  { "NULL", "DASH", "MTSI", "VR" };
    size_t sizeofArray = sizeof(qoe_service_typeArray) / sizeof(qoe_service_typeArray[0]);
    if (qoe_service_type < sizeofArray)
        return (char *)qoe_service_typeArray[qoe_service_type];
    else
        return (char *)"Unknown";
}

OpenAPI_qoe_service_type_e OpenAPI_qoe_service_type_FromString(char* qoe_service_type)
{
    int stringToReturn = 0;
    const char *qoe_service_typeArray[] =  { "NULL", "DASH", "MTSI", "VR" };
    size_t sizeofArray = sizeof(qoe_service_typeArray) / sizeof(qoe_service_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qoe_service_type, qoe_service_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

