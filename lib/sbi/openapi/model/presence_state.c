
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "presence_state.h"

char* OpenAPI_presence_state_ToString(OpenAPI_presence_state_e presence_state)
{
    const char *presence_stateArray[] =  { "NULL", "IN_AREA", "OUT_OF_AREA", "UNKNOWN", "INACTIVE" };
    size_t sizeofArray = sizeof(presence_stateArray) / sizeof(presence_stateArray[0]);
    if (presence_state < sizeofArray)
        return (char *)presence_stateArray[presence_state];
    else
        return (char *)"Unknown";
}

OpenAPI_presence_state_e OpenAPI_presence_state_FromString(char* presence_state)
{
    int stringToReturn = 0;
    const char *presence_stateArray[] =  { "NULL", "IN_AREA", "OUT_OF_AREA", "UNKNOWN", "INACTIVE" };
    size_t sizeofArray = sizeof(presence_stateArray) / sizeof(presence_stateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(presence_state, presence_stateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

