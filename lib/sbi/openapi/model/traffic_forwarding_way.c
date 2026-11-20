
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_forwarding_way.h"

char* OpenAPI_traffic_forwarding_way_ToString(OpenAPI_traffic_forwarding_way_e traffic_forwarding_way)
{
    const char *traffic_forwarding_wayArray[] =  { "NULL", "N6", "N19", "LOCAL_SWITCH" };
    size_t sizeofArray = sizeof(traffic_forwarding_wayArray) / sizeof(traffic_forwarding_wayArray[0]);
    if (traffic_forwarding_way < sizeofArray)
        return (char *)traffic_forwarding_wayArray[traffic_forwarding_way];
    else
        return (char *)"Unknown";
}

OpenAPI_traffic_forwarding_way_e OpenAPI_traffic_forwarding_way_FromString(char* traffic_forwarding_way)
{
    int stringToReturn = 0;
    const char *traffic_forwarding_wayArray[] =  { "NULL", "N6", "N19", "LOCAL_SWITCH" };
    size_t sizeofArray = sizeof(traffic_forwarding_wayArray) / sizeof(traffic_forwarding_wayArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(traffic_forwarding_way, traffic_forwarding_wayArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

