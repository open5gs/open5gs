
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_type_any_of.h"

char* OpenAPI_service_type_any_of_ToString(OpenAPI_service_type_any_of_e service_type_any_of)
{
    const char *service_type_any_ofArray[] =  { "NULL", "AF_GUIDANCE_FOR_URSP" };
    size_t sizeofArray = sizeof(service_type_any_ofArray) / sizeof(service_type_any_ofArray[0]);
    if (service_type_any_of < sizeofArray)
        return (char *)service_type_any_ofArray[service_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_service_type_any_of_e OpenAPI_service_type_any_of_FromString(char* service_type_any_of)
{
    int stringToReturn = 0;
    const char *service_type_any_ofArray[] =  { "NULL", "AF_GUIDANCE_FOR_URSP" };
    size_t sizeofArray = sizeof(service_type_any_ofArray) / sizeof(service_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(service_type_any_of, service_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

