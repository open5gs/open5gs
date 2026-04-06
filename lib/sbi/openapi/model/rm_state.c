
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rm_state.h"

char* OpenAPI_rm_state_ToString(OpenAPI_rm_state_e rm_state)
{
    const char *rm_stateArray[] =  { "NULL", "REGISTERED", "DEREGISTERED" };
    size_t sizeofArray = sizeof(rm_stateArray) / sizeof(rm_stateArray[0]);
    if (rm_state < sizeofArray)
        return (char *)rm_stateArray[rm_state];
    else
        return (char *)"Unknown";
}

OpenAPI_rm_state_e OpenAPI_rm_state_FromString(char* rm_state)
{
    int stringToReturn = 0;
    const char *rm_stateArray[] =  { "NULL", "REGISTERED", "DEREGISTERED" };
    size_t sizeofArray = sizeof(rm_stateArray) / sizeof(rm_stateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rm_state, rm_stateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

