
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "required_access_info.h"

char* OpenAPI_required_access_info_ToString(OpenAPI_required_access_info_e required_access_info)
{
    const char *required_access_infoArray[] =  { "NULL", "USER_LOCATION", "UE_TIME_ZONE" };
    size_t sizeofArray = sizeof(required_access_infoArray) / sizeof(required_access_infoArray[0]);
    if (required_access_info < sizeofArray)
        return (char *)required_access_infoArray[required_access_info];
    else
        return (char *)"Unknown";
}

OpenAPI_required_access_info_e OpenAPI_required_access_info_FromString(char* required_access_info)
{
    int stringToReturn = 0;
    const char *required_access_infoArray[] =  { "NULL", "USER_LOCATION", "UE_TIME_ZONE" };
    size_t sizeofArray = sizeof(required_access_infoArray) / sizeof(required_access_infoArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(required_access_info, required_access_infoArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

