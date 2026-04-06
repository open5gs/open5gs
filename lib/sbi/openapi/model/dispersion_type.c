
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_type.h"

char* OpenAPI_dispersion_type_ToString(OpenAPI_dispersion_type_e dispersion_type)
{
    const char *dispersion_typeArray[] =  { "NULL", "DVDA", "TDA", "DVDA_AND_TDA" };
    size_t sizeofArray = sizeof(dispersion_typeArray) / sizeof(dispersion_typeArray[0]);
    if (dispersion_type < sizeofArray)
        return (char *)dispersion_typeArray[dispersion_type];
    else
        return (char *)"Unknown";
}

OpenAPI_dispersion_type_e OpenAPI_dispersion_type_FromString(char* dispersion_type)
{
    int stringToReturn = 0;
    const char *dispersion_typeArray[] =  { "NULL", "DVDA", "TDA", "DVDA_AND_TDA" };
    size_t sizeofArray = sizeof(dispersion_typeArray) / sizeof(dispersion_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dispersion_type, dispersion_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

