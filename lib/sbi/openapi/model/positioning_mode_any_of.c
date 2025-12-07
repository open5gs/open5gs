
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "positioning_mode_any_of.h"

char* OpenAPI_positioning_mode_any_of_ToString(OpenAPI_positioning_mode_any_of_e positioning_mode_any_of)
{
    const char *positioning_mode_any_ofArray[] =  { "NULL", "UE_BASED", "UE_ASSISTED", "CONVENTIONAL" };
    size_t sizeofArray = sizeof(positioning_mode_any_ofArray) / sizeof(positioning_mode_any_ofArray[0]);
    if (positioning_mode_any_of < sizeofArray)
        return (char *)positioning_mode_any_ofArray[positioning_mode_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_positioning_mode_any_of_e OpenAPI_positioning_mode_any_of_FromString(char* positioning_mode_any_of)
{
    int stringToReturn = 0;
    const char *positioning_mode_any_ofArray[] =  { "NULL", "UE_BASED", "UE_ASSISTED", "CONVENTIONAL" };
    size_t sizeofArray = sizeof(positioning_mode_any_ofArray) / sizeof(positioning_mode_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(positioning_mode_any_of, positioning_mode_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

