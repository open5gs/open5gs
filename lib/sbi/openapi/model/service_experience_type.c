
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_experience_type.h"

char* OpenAPI_service_experience_type_ToString(OpenAPI_service_experience_type_e service_experience_type)
{
    const char *service_experience_typeArray[] =  { "NULL", "VOICE", "VIDEO", "OTHER" };
    size_t sizeofArray = sizeof(service_experience_typeArray) / sizeof(service_experience_typeArray[0]);
    if (service_experience_type < sizeofArray)
        return (char *)service_experience_typeArray[service_experience_type];
    else
        return (char *)"Unknown";
}

OpenAPI_service_experience_type_e OpenAPI_service_experience_type_FromString(char* service_experience_type)
{
    int stringToReturn = 0;
    const char *service_experience_typeArray[] =  { "NULL", "VOICE", "VIDEO", "OTHER" };
    size_t sizeofArray = sizeof(service_experience_typeArray) / sizeof(service_experience_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(service_experience_type, service_experience_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

