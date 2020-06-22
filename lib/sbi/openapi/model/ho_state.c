
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ho_state.h"

char* OpenAPI_ho_state_ToString(OpenAPI_ho_state_e ho_state)
{
    const char *ho_stateArray[] =  { "NULL", "NONE", "PREPARING", "PREPARED", "COMPLETED", "CANCELLED" };
    size_t sizeofArray = sizeof(ho_stateArray) / sizeof(ho_stateArray[0]);
    if (ho_state < sizeofArray)
        return (char *)ho_stateArray[ho_state];
    else
        return (char *)"Unknown";
}

OpenAPI_ho_state_e OpenAPI_ho_state_FromString(char* ho_state)
{
    int stringToReturn = 0;
    const char *ho_stateArray[] =  { "NULL", "NONE", "PREPARING", "PREPARED", "COMPLETED", "CANCELLED" };
    size_t sizeofArray = sizeof(ho_stateArray) / sizeof(ho_stateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ho_state, ho_stateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

