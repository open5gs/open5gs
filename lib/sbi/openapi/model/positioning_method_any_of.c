
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "positioning_method_any_of.h"

char* OpenAPI_positioning_method_any_of_ToString(OpenAPI_positioning_method_any_of_e positioning_method_any_of)
{
    const char *positioning_method_any_ofArray[] =  { "NULL", "CELLID", "ECID", "OTDOA", "BAROMETRIC_PRESSURE", "WLAN", "BLUETOOTH", "MBS", "MOTION_SENSOR", "DL_TDOA", "DL_AOD", "MULTI-RTT", "NR_ECID", "UL_TDOA", "UL_AOA", "NETWORK_SPECIFIC" };
    size_t sizeofArray = sizeof(positioning_method_any_ofArray) / sizeof(positioning_method_any_ofArray[0]);
    if (positioning_method_any_of < sizeofArray)
        return (char *)positioning_method_any_ofArray[positioning_method_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_positioning_method_any_of_e OpenAPI_positioning_method_any_of_FromString(char* positioning_method_any_of)
{
    int stringToReturn = 0;
    const char *positioning_method_any_ofArray[] =  { "NULL", "CELLID", "ECID", "OTDOA", "BAROMETRIC_PRESSURE", "WLAN", "BLUETOOTH", "MBS", "MOTION_SENSOR", "DL_TDOA", "DL_AOD", "MULTI-RTT", "NR_ECID", "UL_TDOA", "UL_AOA", "NETWORK_SPECIFIC" };
    size_t sizeofArray = sizeof(positioning_method_any_ofArray) / sizeof(positioning_method_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(positioning_method_any_of, positioning_method_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

