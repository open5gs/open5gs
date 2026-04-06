
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "association_type.h"

char* OpenAPI_association_type_ToString(OpenAPI_association_type_e association_type)
{
    const char *association_typeArray[] =  { "NULL", "IMEI_CHANGE", "IMEISV_CHANGE" };
    size_t sizeofArray = sizeof(association_typeArray) / sizeof(association_typeArray[0]);
    if (association_type < sizeofArray)
        return (char *)association_typeArray[association_type];
    else
        return (char *)"Unknown";
}

OpenAPI_association_type_e OpenAPI_association_type_FromString(char* association_type)
{
    int stringToReturn = 0;
    const char *association_typeArray[] =  { "NULL", "IMEI_CHANGE", "IMEISV_CHANGE" };
    size_t sizeofArray = sizeof(association_typeArray) / sizeof(association_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(association_type, association_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

