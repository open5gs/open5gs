
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cm_state_any_of.h"

char* OpenAPI_cm_state_any_of_ToString(OpenAPI_cm_state_any_of_e cm_state_any_of)
{
    const char *cm_state_any_ofArray[] =  { "NULL", "IDLE", "CONNECTED" };
    size_t sizeofArray = sizeof(cm_state_any_ofArray) / sizeof(cm_state_any_ofArray[0]);
    if (cm_state_any_of < sizeofArray)
        return (char *)cm_state_any_ofArray[cm_state_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_cm_state_any_of_e OpenAPI_cm_state_any_of_FromString(char* cm_state_any_of)
{
    int stringToReturn = 0;
    const char *cm_state_any_ofArray[] =  { "NULL", "IDLE", "CONNECTED" };
    size_t sizeofArray = sizeof(cm_state_any_ofArray) / sizeof(cm_state_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(cm_state_any_of, cm_state_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

