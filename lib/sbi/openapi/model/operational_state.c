
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "operational_state.h"

char* OpenAPI_operational_state_ToString(OpenAPI_operational_state_e operational_state)
{
    const char *operational_stateArray[] =  { "NULL", "ENABLED", "DISABLED" };
    size_t sizeofArray = sizeof(operational_stateArray) / sizeof(operational_stateArray[0]);
    if (operational_state < sizeofArray)
        return (char *)operational_stateArray[operational_state];
    else
        return (char *)"Unknown";
}

OpenAPI_operational_state_e OpenAPI_operational_state_FromString(char* operational_state)
{
    int stringToReturn = 0;
    const char *operational_stateArray[] =  { "NULL", "ENABLED", "DISABLED" };
    size_t sizeofArray = sizeof(operational_stateArray) / sizeof(operational_stateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(operational_state, operational_stateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

