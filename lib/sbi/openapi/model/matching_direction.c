
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "matching_direction.h"

char* OpenAPI_matching_direction_ToString(OpenAPI_matching_direction_e matching_direction)
{
    const char *matching_directionArray[] =  { "NULL", "ASCENDING", "DESCENDING", "CROSSED" };
    size_t sizeofArray = sizeof(matching_directionArray) / sizeof(matching_directionArray[0]);
    if (matching_direction < sizeofArray)
        return (char *)matching_directionArray[matching_direction];
    else
        return (char *)"Unknown";
}

OpenAPI_matching_direction_e OpenAPI_matching_direction_FromString(char* matching_direction)
{
    int stringToReturn = 0;
    const char *matching_directionArray[] =  { "NULL", "ASCENDING", "DESCENDING", "CROSSED" };
    size_t sizeofArray = sizeof(matching_directionArray) / sizeof(matching_directionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(matching_direction, matching_directionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

