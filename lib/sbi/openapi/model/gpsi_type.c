
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gpsi_type.h"

char* OpenAPI_gpsi_type_ToString(OpenAPI_gpsi_type_e gpsi_type)
{
    const char *gpsi_typeArray[] =  { "NULL", "MSISDN", "EXT_ID", "EXT_GROUP_ID" };
    size_t sizeofArray = sizeof(gpsi_typeArray) / sizeof(gpsi_typeArray[0]);
    if (gpsi_type < sizeofArray)
        return (char *)gpsi_typeArray[gpsi_type];
    else
        return (char *)"Unknown";
}

OpenAPI_gpsi_type_e OpenAPI_gpsi_type_FromString(char* gpsi_type)
{
    int stringToReturn = 0;
    const char *gpsi_typeArray[] =  { "NULL", "MSISDN", "EXT_ID", "EXT_GROUP_ID" };
    size_t sizeofArray = sizeof(gpsi_typeArray) / sizeof(gpsi_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(gpsi_type, gpsi_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

