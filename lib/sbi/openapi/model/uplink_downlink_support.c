
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uplink_downlink_support.h"

char* OpenAPI_uplink_downlink_support_ToString(OpenAPI_uplink_downlink_support_e uplink_downlink_support)
{
    const char *uplink_downlink_supportArray[] =  { "NULL", "UL", "DL", "UL_DL" };
    size_t sizeofArray = sizeof(uplink_downlink_supportArray) / sizeof(uplink_downlink_supportArray[0]);
    if (uplink_downlink_support < sizeofArray)
        return (char *)uplink_downlink_supportArray[uplink_downlink_support];
    else
        return (char *)"Unknown";
}

OpenAPI_uplink_downlink_support_e OpenAPI_uplink_downlink_support_FromString(char* uplink_downlink_support)
{
    int stringToReturn = 0;
    const char *uplink_downlink_supportArray[] =  { "NULL", "UL", "DL", "UL_DL" };
    size_t sizeofArray = sizeof(uplink_downlink_supportArray) / sizeof(uplink_downlink_supportArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(uplink_downlink_support, uplink_downlink_supportArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

