
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "target_cause_id.h"

char* OpenAPI_target_cause_id_ToString(OpenAPI_target_cause_id_e target_cause_id)
{
    const char *target_cause_idArray[] =  { "NULL", "SIGNALLING_STORM_CAUSED_BY_UE", "SIGNALLING_STORM_CAUSED_BY_NF" };
    size_t sizeofArray = sizeof(target_cause_idArray) / sizeof(target_cause_idArray[0]);
    if (target_cause_id < sizeofArray)
        return (char *)target_cause_idArray[target_cause_id];
    else
        return (char *)"Unknown";
}

OpenAPI_target_cause_id_e OpenAPI_target_cause_id_FromString(char* target_cause_id)
{
    int stringToReturn = 0;
    const char *target_cause_idArray[] =  { "NULL", "SIGNALLING_STORM_CAUSED_BY_UE", "SIGNALLING_STORM_CAUSED_BY_NF" };
    size_t sizeofArray = sizeof(target_cause_idArray) / sizeof(target_cause_idArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(target_cause_id, target_cause_idArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

