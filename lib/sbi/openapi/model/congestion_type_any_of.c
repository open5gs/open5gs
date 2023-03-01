
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "congestion_type_any_of.h"

char* OpenAPI_congestion_type_any_of_ToString(OpenAPI_congestion_type_any_of_e congestion_type_any_of)
{
    const char *congestion_type_any_ofArray[] =  { "NULL", "USER_PLANE", "CONTROL_PLANE", "USER_AND_CONTROL_PLANE" };
    size_t sizeofArray = sizeof(congestion_type_any_ofArray) / sizeof(congestion_type_any_ofArray[0]);
    if (congestion_type_any_of < sizeofArray)
        return (char *)congestion_type_any_ofArray[congestion_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_congestion_type_any_of_e OpenAPI_congestion_type_any_of_FromString(char* congestion_type_any_of)
{
    int stringToReturn = 0;
    const char *congestion_type_any_ofArray[] =  { "NULL", "USER_PLANE", "CONTROL_PLANE", "USER_AND_CONTROL_PLANE" };
    size_t sizeofArray = sizeof(congestion_type_any_ofArray) / sizeof(congestion_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(congestion_type_any_of, congestion_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

