
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_service_type.h"

char* OpenAPI_mbs_service_type_ToString(OpenAPI_mbs_service_type_e mbs_service_type)
{
    const char *mbs_service_typeArray[] =  { "NULL", "MULTICAST", "BROADCAST" };
    size_t sizeofArray = sizeof(mbs_service_typeArray) / sizeof(mbs_service_typeArray[0]);
    if (mbs_service_type < sizeofArray)
        return (char *)mbs_service_typeArray[mbs_service_type];
    else
        return (char *)"Unknown";
}

OpenAPI_mbs_service_type_e OpenAPI_mbs_service_type_FromString(char* mbs_service_type)
{
    int stringToReturn = 0;
    const char *mbs_service_typeArray[] =  { "NULL", "MULTICAST", "BROADCAST" };
    size_t sizeofArray = sizeof(mbs_service_typeArray) / sizeof(mbs_service_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(mbs_service_type, mbs_service_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

