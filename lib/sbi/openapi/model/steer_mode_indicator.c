
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steer_mode_indicator.h"

char* OpenAPI_steer_mode_indicator_ToString(OpenAPI_steer_mode_indicator_e steer_mode_indicator)
{
    const char *steer_mode_indicatorArray[] =  { "NULL", "AUTO_LOAD_BALANCE", "UE_ASSISTANCE" };
    size_t sizeofArray = sizeof(steer_mode_indicatorArray) / sizeof(steer_mode_indicatorArray[0]);
    if (steer_mode_indicator < sizeofArray)
        return (char *)steer_mode_indicatorArray[steer_mode_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_steer_mode_indicator_e OpenAPI_steer_mode_indicator_FromString(char* steer_mode_indicator)
{
    int stringToReturn = 0;
    const char *steer_mode_indicatorArray[] =  { "NULL", "AUTO_LOAD_BALANCE", "UE_ASSISTANCE" };
    size_t sizeofArray = sizeof(steer_mode_indicatorArray) / sizeof(steer_mode_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(steer_mode_indicator, steer_mode_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

