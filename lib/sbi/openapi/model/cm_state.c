
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cm_state.h"

char* OpenAPI_cm_state_ToString(OpenAPI_cm_state_e cm_state)
{
    const char *cm_stateArray[] =  { "NULL", "IDLE", "CONNECTED" };
    size_t sizeofArray = sizeof(cm_stateArray) / sizeof(cm_stateArray[0]);
    if (cm_state < sizeofArray)
        return (char *)cm_stateArray[cm_state];
    else
        return (char *)"Unknown";
}

OpenAPI_cm_state_e OpenAPI_cm_state_FromString(char* cm_state)
{
    int stringToReturn = 0;
    const char *cm_stateArray[] =  { "NULL", "IDLE", "CONNECTED" };
    size_t sizeofArray = sizeof(cm_stateArray) / sizeof(cm_stateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(cm_state, cm_stateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

