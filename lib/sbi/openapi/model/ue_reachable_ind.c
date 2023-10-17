
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_reachable_ind.h"

char* OpenAPI_ue_reachable_ind_ToString(OpenAPI_ue_reachable_ind_e ue_reachable_ind)
{
    const char *ue_reachable_indArray[] =  { "NULL", "REACHABLE", "NOT_REACHABLE", "UNKNOWN" };
    size_t sizeofArray = sizeof(ue_reachable_indArray) / sizeof(ue_reachable_indArray[0]);
    if (ue_reachable_ind < sizeofArray)
        return (char *)ue_reachable_indArray[ue_reachable_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_reachable_ind_e OpenAPI_ue_reachable_ind_FromString(char* ue_reachable_ind)
{
    int stringToReturn = 0;
    const char *ue_reachable_indArray[] =  { "NULL", "REACHABLE", "NOT_REACHABLE", "UNKNOWN" };
    size_t sizeofArray = sizeof(ue_reachable_indArray) / sizeof(ue_reachable_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_reachable_ind, ue_reachable_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

