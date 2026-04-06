
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "congestion_type.h"

char* OpenAPI_congestion_type_ToString(OpenAPI_congestion_type_e congestion_type)
{
    const char *congestion_typeArray[] =  { "NULL", "USER_PLANE", "CONTROL_PLANE", "USER_AND_CONTROL_PLANE" };
    size_t sizeofArray = sizeof(congestion_typeArray) / sizeof(congestion_typeArray[0]);
    if (congestion_type < sizeofArray)
        return (char *)congestion_typeArray[congestion_type];
    else
        return (char *)"Unknown";
}

OpenAPI_congestion_type_e OpenAPI_congestion_type_FromString(char* congestion_type)
{
    int stringToReturn = 0;
    const char *congestion_typeArray[] =  { "NULL", "USER_PLANE", "CONTROL_PLANE", "USER_AND_CONTROL_PLANE" };
    size_t sizeofArray = sizeof(congestion_typeArray) / sizeof(congestion_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(congestion_type, congestion_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

