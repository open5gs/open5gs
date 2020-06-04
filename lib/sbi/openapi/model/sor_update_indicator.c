
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sor_update_indicator.h"

char* OpenAPI_sor_update_indicator_ToString(OpenAPI_sor_update_indicator_e sor_update_indicator)
{
    const char *sor_update_indicatorArray[] =  { "NULL", "INITIAL_REGISTRATION", "EMERGENCY_REGISTRATION" };
    size_t sizeofArray = sizeof(sor_update_indicatorArray) / sizeof(sor_update_indicatorArray[0]);
    if (sor_update_indicator < sizeofArray)
        return (char *)sor_update_indicatorArray[sor_update_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_sor_update_indicator_e OpenAPI_sor_update_indicator_FromString(char* sor_update_indicator)
{
    int stringToReturn = 0;
    const char *sor_update_indicatorArray[] =  { "NULL", "INITIAL_REGISTRATION", "EMERGENCY_REGISTRATION" };
    size_t sizeofArray = sizeof(sor_update_indicatorArray) / sizeof(sor_update_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sor_update_indicator, sor_update_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

