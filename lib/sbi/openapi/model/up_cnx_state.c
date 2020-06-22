
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_cnx_state.h"

char* OpenAPI_up_cnx_state_ToString(OpenAPI_up_cnx_state_e up_cnx_state)
{
    const char *up_cnx_stateArray[] =  { "NULL", "ACTIVATED", "DEACTIVATED", "ACTIVATING", "SUSPENDED" };
    size_t sizeofArray = sizeof(up_cnx_stateArray) / sizeof(up_cnx_stateArray[0]);
    if (up_cnx_state < sizeofArray)
        return (char *)up_cnx_stateArray[up_cnx_state];
    else
        return (char *)"Unknown";
}

OpenAPI_up_cnx_state_e OpenAPI_up_cnx_state_FromString(char* up_cnx_state)
{
    int stringToReturn = 0;
    const char *up_cnx_stateArray[] =  { "NULL", "ACTIVATED", "DEACTIVATED", "ACTIVATING", "SUSPENDED" };
    size_t sizeofArray = sizeof(up_cnx_stateArray) / sizeof(up_cnx_stateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(up_cnx_state, up_cnx_stateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

