
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redundant_transmission_type.h"

char* OpenAPI_redundant_transmission_type_ToString(OpenAPI_redundant_transmission_type_e redundant_transmission_type)
{
    const char *redundant_transmission_typeArray[] =  { "NULL", "NON_TRANSMISSION", "END_TO_END_USER_PLANE_PATHS", "N3_N9", "TRANSPORT_LAYER" };
    size_t sizeofArray = sizeof(redundant_transmission_typeArray) / sizeof(redundant_transmission_typeArray[0]);
    if (redundant_transmission_type < sizeofArray)
        return (char *)redundant_transmission_typeArray[redundant_transmission_type];
    else
        return (char *)"Unknown";
}

OpenAPI_redundant_transmission_type_e OpenAPI_redundant_transmission_type_FromString(char* redundant_transmission_type)
{
    int stringToReturn = 0;
    const char *redundant_transmission_typeArray[] =  { "NULL", "NON_TRANSMISSION", "END_TO_END_USER_PLANE_PATHS", "N3_N9", "TRANSPORT_LAYER" };
    size_t sizeofArray = sizeof(redundant_transmission_typeArray) / sizeof(redundant_transmission_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(redundant_transmission_type, redundant_transmission_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

