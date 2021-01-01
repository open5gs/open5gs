
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_direction.h"

char* OpenAPI_flow_direction_ToString(OpenAPI_flow_direction_e flow_direction)
{
    const char *flow_directionArray[] =  { "NULL", "DOWNLINK", "UPLINK", "BIDIRECTIONAL", "UNSPECIFIED" };
    size_t sizeofArray = sizeof(flow_directionArray) / sizeof(flow_directionArray[0]);
    if (flow_direction < sizeofArray)
        return (char *)flow_directionArray[flow_direction];
    else
        return (char *)"Unknown";
}

OpenAPI_flow_direction_e OpenAPI_flow_direction_FromString(char* flow_direction)
{
    int stringToReturn = 0;
    const char *flow_directionArray[] =  { "NULL", "DOWNLINK", "UPLINK", "BIDIRECTIONAL", "UNSPECIFIED" };
    size_t sizeofArray = sizeof(flow_directionArray) / sizeof(flow_directionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(flow_direction, flow_directionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

