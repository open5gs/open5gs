
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "locality_type.h"

char* OpenAPI_locality_type_ToString(OpenAPI_locality_type_e locality_type)
{
    const char *locality_typeArray[] =  { "NULL", "DATA_CENTER", "CITY", "COUNTY", "DISTRICT", "STATE", "CANTON", "REGION", "PROVINCE", "PREFECTURE", "COUNTRY" };
    size_t sizeofArray = sizeof(locality_typeArray) / sizeof(locality_typeArray[0]);
    if (locality_type < sizeofArray)
        return (char *)locality_typeArray[locality_type];
    else
        return (char *)"Unknown";
}

OpenAPI_locality_type_e OpenAPI_locality_type_FromString(char* locality_type)
{
    int stringToReturn = 0;
    const char *locality_typeArray[] =  { "NULL", "DATA_CENTER", "CITY", "COUNTY", "DISTRICT", "STATE", "CANTON", "REGION", "PROVINCE", "PREFECTURE", "COUNTRY" };
    size_t sizeofArray = sizeof(locality_typeArray) / sizeof(locality_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(locality_type, locality_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

