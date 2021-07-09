
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_reachability_any_of.h"

char* OpenAPI_ue_reachability_any_of_ToString(OpenAPI_ue_reachability_any_of_e ue_reachability_any_of)
{
    const char *ue_reachability_any_ofArray[] =  { "NULL", "UNREACHABLE", "REACHABLE", "REGULATORY_ONLY" };
    size_t sizeofArray = sizeof(ue_reachability_any_ofArray) / sizeof(ue_reachability_any_ofArray[0]);
    if (ue_reachability_any_of < sizeofArray)
        return (char *)ue_reachability_any_ofArray[ue_reachability_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_reachability_any_of_e OpenAPI_ue_reachability_any_of_FromString(char* ue_reachability_any_of)
{
    int stringToReturn = 0;
    const char *ue_reachability_any_ofArray[] =  { "NULL", "UNREACHABLE", "REACHABLE", "REGULATORY_ONLY" };
    size_t sizeofArray = sizeof(ue_reachability_any_ofArray) / sizeof(ue_reachability_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_reachability_any_of, ue_reachability_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

