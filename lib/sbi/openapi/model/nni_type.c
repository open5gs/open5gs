
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nni_type.h"

char* OpenAPI_nni_type_ToString(OpenAPI_nni_type_e nni_type)
{
    const char *nni_typeArray[] =  { "NULL", "NON_ROAMING", "ROAMING_NO_LOOPBACK", "ROAMING_LOOPBACK" };
    size_t sizeofArray = sizeof(nni_typeArray) / sizeof(nni_typeArray[0]);
    if (nni_type < sizeofArray)
        return (char *)nni_typeArray[nni_type];
    else
        return (char *)"Unknown";
}

OpenAPI_nni_type_e OpenAPI_nni_type_FromString(char* nni_type)
{
    int stringToReturn = 0;
    const char *nni_typeArray[] =  { "NULL", "NON_ROAMING", "ROAMING_NO_LOOPBACK", "ROAMING_LOOPBACK" };
    size_t sizeofArray = sizeof(nni_typeArray) / sizeof(nni_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nni_type, nni_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

