
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ssc_mode.h"

char* OpenAPI_ssc_mode_ToString(OpenAPI_ssc_mode_e ssc_mode)
{
    const char *ssc_modeArray[] =  { "NULL", "SSC_MODE_1", "SSC_MODE_2", "SSC_MODE_3" };
    size_t sizeofArray = sizeof(ssc_modeArray) / sizeof(ssc_modeArray[0]);
    if (ssc_mode < sizeofArray)
        return (char *)ssc_modeArray[ssc_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_ssc_mode_e OpenAPI_ssc_mode_FromString(char* ssc_mode)
{
    int stringToReturn = 0;
    const char *ssc_modeArray[] =  { "NULL", "SSC_MODE_1", "SSC_MODE_2", "SSC_MODE_3" };
    size_t sizeofArray = sizeof(ssc_modeArray) / sizeof(ssc_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ssc_mode, ssc_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

