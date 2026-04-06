
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "direction.h"

char* OpenAPI_direction_ToString(OpenAPI_direction_e direction)
{
    const char *directionArray[] =  { "NULL", "NORTH", "SOUTH", "EAST", "WEST", "NORTHWEST", "NORTHEAST", "SOUTHWEST", "SOUTHEAST" };
    size_t sizeofArray = sizeof(directionArray) / sizeof(directionArray[0]);
    if (direction < sizeofArray)
        return (char *)directionArray[direction];
    else
        return (char *)"Unknown";
}

OpenAPI_direction_e OpenAPI_direction_FromString(char* direction)
{
    int stringToReturn = 0;
    const char *directionArray[] =  { "NULL", "NORTH", "SOUTH", "EAST", "WEST", "NORTHWEST", "NORTHEAST", "SOUTHWEST", "SOUTHEAST" };
    size_t sizeofArray = sizeof(directionArray) / sizeof(directionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(direction, directionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

