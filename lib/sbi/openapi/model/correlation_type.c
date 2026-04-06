
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "correlation_type.h"

char* OpenAPI_correlation_type_ToString(OpenAPI_correlation_type_e correlation_type)
{
    const char *correlation_typeArray[] =  { "NULL", "COMMON_DNAI", "COMMON_EAS" };
    size_t sizeofArray = sizeof(correlation_typeArray) / sizeof(correlation_typeArray[0]);
    if (correlation_type < sizeofArray)
        return (char *)correlation_typeArray[correlation_type];
    else
        return (char *)"Unknown";
}

OpenAPI_correlation_type_e OpenAPI_correlation_type_FromString(char* correlation_type)
{
    int stringToReturn = 0;
    const char *correlation_typeArray[] =  { "NULL", "COMMON_DNAI", "COMMON_EAS" };
    size_t sizeofArray = sizeof(correlation_typeArray) / sizeof(correlation_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(correlation_type, correlation_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

