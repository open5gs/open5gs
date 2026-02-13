
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vertical_direction.h"

char* OpenAPI_vertical_direction_ToString(OpenAPI_vertical_direction_e vertical_direction)
{
    const char *vertical_directionArray[] =  { "NULL", "UPWARD", "DOWNWARD" };
    size_t sizeofArray = sizeof(vertical_directionArray) / sizeof(vertical_directionArray[0]);
    if (vertical_direction < sizeofArray)
        return (char *)vertical_directionArray[vertical_direction];
    else
        return (char *)"Unknown";
}

OpenAPI_vertical_direction_e OpenAPI_vertical_direction_FromString(char* vertical_direction)
{
    int stringToReturn = 0;
    const char *vertical_directionArray[] =  { "NULL", "UPWARD", "DOWNWARD" };
    size_t sizeofArray = sizeof(vertical_directionArray) / sizeof(vertical_directionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(vertical_direction, vertical_directionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

