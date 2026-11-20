
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "api_direction.h"

char* OpenAPI_api_direction_ToString(OpenAPI_api_direction_e api_direction)
{
    const char *api_directionArray[] =  { "NULL", "INVOCATION", "NOTIFICATION" };
    size_t sizeofArray = sizeof(api_directionArray) / sizeof(api_directionArray[0]);
    if (api_direction < sizeofArray)
        return (char *)api_directionArray[api_direction];
    else
        return (char *)"Unknown";
}

OpenAPI_api_direction_e OpenAPI_api_direction_FromString(char* api_direction)
{
    int stringToReturn = 0;
    const char *api_directionArray[] =  { "NULL", "INVOCATION", "NOTIFICATION" };
    size_t sizeofArray = sizeof(api_directionArray) / sizeof(api_directionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(api_direction, api_directionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

