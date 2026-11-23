
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "administrative_state.h"

char* OpenAPI_administrative_state_ToString(OpenAPI_administrative_state_e administrative_state)
{
    const char *administrative_stateArray[] =  { "NULL", "LOCKED", "UNLOCKED" };
    size_t sizeofArray = sizeof(administrative_stateArray) / sizeof(administrative_stateArray[0]);
    if (administrative_state < sizeofArray)
        return (char *)administrative_stateArray[administrative_state];
    else
        return (char *)"Unknown";
}

OpenAPI_administrative_state_e OpenAPI_administrative_state_FromString(char* administrative_state)
{
    int stringToReturn = 0;
    const char *administrative_stateArray[] =  { "NULL", "LOCKED", "UNLOCKED" };
    size_t sizeofArray = sizeof(administrative_stateArray) / sizeof(administrative_stateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(administrative_state, administrative_stateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

