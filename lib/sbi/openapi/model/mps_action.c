
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mps_action.h"

char* OpenAPI_mps_action_ToString(OpenAPI_mps_action_e mps_action)
{
    const char *mps_actionArray[] =  { "NULL", "DISABLE_MPS_FOR_DTS", "ENABLE_MPS_FOR_DTS", "AUTHORIZE_AND_ENABLE_MPS_FOR_DTS" };
    size_t sizeofArray = sizeof(mps_actionArray) / sizeof(mps_actionArray[0]);
    if (mps_action < sizeofArray)
        return (char *)mps_actionArray[mps_action];
    else
        return (char *)"Unknown";
}

OpenAPI_mps_action_e OpenAPI_mps_action_FromString(char* mps_action)
{
    int stringToReturn = 0;
    const char *mps_actionArray[] =  { "NULL", "DISABLE_MPS_FOR_DTS", "ENABLE_MPS_FOR_DTS", "AUTHORIZE_AND_ENABLE_MPS_FOR_DTS" };
    size_t sizeofArray = sizeof(mps_actionArray) / sizeof(mps_actionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(mps_action, mps_actionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

