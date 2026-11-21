
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_transfer_type.h"

char* OpenAPI_access_transfer_type_ToString(OpenAPI_access_transfer_type_e access_transfer_type)
{
    const char *access_transfer_typeArray[] =  { "NULL", "PS_TO_CS", "CS_TO_PS", "PS_TO_PS", "CS_TO_CS" };
    size_t sizeofArray = sizeof(access_transfer_typeArray) / sizeof(access_transfer_typeArray[0]);
    if (access_transfer_type < sizeofArray)
        return (char *)access_transfer_typeArray[access_transfer_type];
    else
        return (char *)"Unknown";
}

OpenAPI_access_transfer_type_e OpenAPI_access_transfer_type_FromString(char* access_transfer_type)
{
    int stringToReturn = 0;
    const char *access_transfer_typeArray[] =  { "NULL", "PS_TO_CS", "CS_TO_PS", "PS_TO_PS", "CS_TO_CS" };
    size_t sizeofArray = sizeof(access_transfer_typeArray) / sizeof(access_transfer_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(access_transfer_type, access_transfer_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

