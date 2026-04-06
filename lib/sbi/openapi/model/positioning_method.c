
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "positioning_method.h"

char* OpenAPI_positioning_method_ToString(OpenAPI_positioning_method_e positioning_method)
{
    const char *positioning_methodArray[] =  { "NULL", "CELLID", "ECID", "OTDOA", "BAROMETRIC_PRESSURE", "WLAN", "BLUETOOTH", "MBS", "MOTION_SENSOR", "DL_TDOA", "DL_AOD", "MULTI-RTT", "NR_ECID", "UL_TDOA", "UL_AOA", "NETWORK_SPECIFIC", "SL_TDOA", "SL_TOA", "SL_AoA", "SL_RT", "AIML" };
    size_t sizeofArray = sizeof(positioning_methodArray) / sizeof(positioning_methodArray[0]);
    if (positioning_method < sizeofArray)
        return (char *)positioning_methodArray[positioning_method];
    else
        return (char *)"Unknown";
}

OpenAPI_positioning_method_e OpenAPI_positioning_method_FromString(char* positioning_method)
{
    int stringToReturn = 0;
    const char *positioning_methodArray[] =  { "NULL", "CELLID", "ECID", "OTDOA", "BAROMETRIC_PRESSURE", "WLAN", "BLUETOOTH", "MBS", "MOTION_SENSOR", "DL_TDOA", "DL_AOD", "MULTI-RTT", "NR_ECID", "UL_TDOA", "UL_AOA", "NETWORK_SPECIFIC", "SL_TDOA", "SL_TOA", "SL_AoA", "SL_RT", "AIML" };
    size_t sizeofArray = sizeof(positioning_methodArray) / sizeof(positioning_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(positioning_method, positioning_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

