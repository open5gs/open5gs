
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aerial_ue_indication.h"

char* OpenAPI_aerial_ue_indication_ToString(OpenAPI_aerial_ue_indication_e aerial_ue_indication)
{
    const char *aerial_ue_indicationArray[] =  { "NULL", "AERIAL_UE_ALLOWED", "AERIAL_UE_NOT_ALLOWED" };
    size_t sizeofArray = sizeof(aerial_ue_indicationArray) / sizeof(aerial_ue_indicationArray[0]);
    if (aerial_ue_indication < sizeofArray)
        return (char *)aerial_ue_indicationArray[aerial_ue_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_aerial_ue_indication_e OpenAPI_aerial_ue_indication_FromString(char* aerial_ue_indication)
{
    int stringToReturn = 0;
    const char *aerial_ue_indicationArray[] =  { "NULL", "AERIAL_UE_ALLOWED", "AERIAL_UE_NOT_ALLOWED" };
    size_t sizeofArray = sizeof(aerial_ue_indicationArray) / sizeof(aerial_ue_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(aerial_ue_indication, aerial_ue_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

