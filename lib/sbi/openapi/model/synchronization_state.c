
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "synchronization_state.h"

char* OpenAPI_synchronization_state_ToString(OpenAPI_synchronization_state_e synchronization_state)
{
    const char *synchronization_stateArray[] =  { "NULL", "LOCKED", "HOLDOVER", "FREERUN" };
    size_t sizeofArray = sizeof(synchronization_stateArray) / sizeof(synchronization_stateArray[0]);
    if (synchronization_state < sizeofArray)
        return (char *)synchronization_stateArray[synchronization_state];
    else
        return (char *)"Unknown";
}

OpenAPI_synchronization_state_e OpenAPI_synchronization_state_FromString(char* synchronization_state)
{
    int stringToReturn = 0;
    const char *synchronization_stateArray[] =  { "NULL", "LOCKED", "HOLDOVER", "FREERUN" };
    size_t sizeofArray = sizeof(synchronization_stateArray) / sizeof(synchronization_stateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(synchronization_state, synchronization_stateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

