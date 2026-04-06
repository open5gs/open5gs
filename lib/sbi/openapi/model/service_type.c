
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_type.h"

char* OpenAPI_service_type_ToString(OpenAPI_service_type_e service_type)
{
    const char *service_typeArray[] =  { "NULL", "AF_GUIDANCE_FOR_URSP", "AF_REQUESTED_QOS", "AF_PROVISION_N3GPP_DEV_ID_INFO" };
    size_t sizeofArray = sizeof(service_typeArray) / sizeof(service_typeArray[0]);
    if (service_type < sizeofArray)
        return (char *)service_typeArray[service_type];
    else
        return (char *)"Unknown";
}

OpenAPI_service_type_e OpenAPI_service_type_FromString(char* service_type)
{
    int stringToReturn = 0;
    const char *service_typeArray[] =  { "NULL", "AF_GUIDANCE_FOR_URSP", "AF_REQUESTED_QOS", "AF_PROVISION_N3GPP_DEV_ID_INFO" };
    size_t sizeofArray = sizeof(service_typeArray) / sizeof(service_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(service_type, service_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

