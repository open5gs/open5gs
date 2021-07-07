
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rm_state_any_of.h"

char* OpenAPI_rm_state_any_of_ToString(OpenAPI_rm_state_any_of_e rm_state_any_of)
{
    const char *rm_state_any_ofArray[] =  { "NULL", "REGISTERED", "DEREGISTERED" };
    size_t sizeofArray = sizeof(rm_state_any_ofArray) / sizeof(rm_state_any_ofArray[0]);
    if (rm_state_any_of < sizeofArray)
        return (char *)rm_state_any_ofArray[rm_state_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_rm_state_any_of_e OpenAPI_rm_state_any_of_FromString(char* rm_state_any_of)
{
    int stringToReturn = 0;
    const char *rm_state_any_ofArray[] =  { "NULL", "REGISTERED", "DEREGISTERED" };
    size_t sizeofArray = sizeof(rm_state_any_ofArray) / sizeof(rm_state_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rm_state_any_of, rm_state_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

