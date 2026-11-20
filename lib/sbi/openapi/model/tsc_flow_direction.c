
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsc_flow_direction.h"

char* OpenAPI_tsc_flow_direction_ToString(OpenAPI_tsc_flow_direction_e tsc_flow_direction)
{
    const char *tsc_flow_directionArray[] =  { "NULL", "UPLINK", "DOWNLINK" };
    size_t sizeofArray = sizeof(tsc_flow_directionArray) / sizeof(tsc_flow_directionArray[0]);
    if (tsc_flow_direction < sizeofArray)
        return (char *)tsc_flow_directionArray[tsc_flow_direction];
    else
        return (char *)"Unknown";
}

OpenAPI_tsc_flow_direction_e OpenAPI_tsc_flow_direction_FromString(char* tsc_flow_direction)
{
    int stringToReturn = 0;
    const char *tsc_flow_directionArray[] =  { "NULL", "UPLINK", "DOWNLINK" };
    size_t sizeofArray = sizeof(tsc_flow_directionArray) / sizeof(tsc_flow_directionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(tsc_flow_direction, tsc_flow_directionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

