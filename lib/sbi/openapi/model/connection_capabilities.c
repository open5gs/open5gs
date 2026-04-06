
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "connection_capabilities.h"

char* OpenAPI_connection_capabilities_ToString(OpenAPI_connection_capabilities_e connection_capabilities)
{
    const char *connection_capabilitiesArray[] =  { "NULL", "IMS", "MMS", "SUPL", "INTERNET", "IOT_DELAY_TOLERANT", "IOT_NON_DELAY_TOLERANT", "DL_STREAMING", "UL_STREAMING", "VEHIC_COMM", "REAL_TIME_INTERACTIVE", "UNIFIED_COMM", "BACKGROUND", "MISS_CRITICAL", "TIME_CRITICAL", "LOW_LAT_LOSS_TOL_UNACK" };
    size_t sizeofArray = sizeof(connection_capabilitiesArray) / sizeof(connection_capabilitiesArray[0]);
    if (connection_capabilities < sizeofArray)
        return (char *)connection_capabilitiesArray[connection_capabilities];
    else
        return (char *)"Unknown";
}

OpenAPI_connection_capabilities_e OpenAPI_connection_capabilities_FromString(char* connection_capabilities)
{
    int stringToReturn = 0;
    const char *connection_capabilitiesArray[] =  { "NULL", "IMS", "MMS", "SUPL", "INTERNET", "IOT_DELAY_TOLERANT", "IOT_NON_DELAY_TOLERANT", "DL_STREAMING", "UL_STREAMING", "VEHIC_COMM", "REAL_TIME_INTERACTIVE", "UNIFIED_COMM", "BACKGROUND", "MISS_CRITICAL", "TIME_CRITICAL", "LOW_LAT_LOSS_TOL_UNACK" };
    size_t sizeofArray = sizeof(connection_capabilitiesArray) / sizeof(connection_capabilitiesArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(connection_capabilities, connection_capabilitiesArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

