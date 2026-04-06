
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_direction.h"

char* OpenAPI_traffic_direction_ToString(OpenAPI_traffic_direction_e traffic_direction)
{
    const char *traffic_directionArray[] =  { "NULL", "UL_AND_DL", "UL", "DL" };
    size_t sizeofArray = sizeof(traffic_directionArray) / sizeof(traffic_directionArray[0]);
    if (traffic_direction < sizeofArray)
        return (char *)traffic_directionArray[traffic_direction];
    else
        return (char *)"Unknown";
}

OpenAPI_traffic_direction_e OpenAPI_traffic_direction_FromString(char* traffic_direction)
{
    int stringToReturn = 0;
    const char *traffic_directionArray[] =  { "NULL", "UL_AND_DL", "UL", "DL" };
    size_t sizeofArray = sizeof(traffic_directionArray) / sizeof(traffic_directionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(traffic_direction, traffic_directionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

