
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_reachability.h"

char* OpenAPI_ue_reachability_ToString(OpenAPI_ue_reachability_e ue_reachability)
{
    const char *ue_reachabilityArray[] =  { "NULL", "UNREACHABLE", "REACHABLE", "REGULATORY_ONLY" };
    size_t sizeofArray = sizeof(ue_reachabilityArray) / sizeof(ue_reachabilityArray[0]);
    if (ue_reachability < sizeofArray)
        return (char *)ue_reachabilityArray[ue_reachability];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_reachability_e OpenAPI_ue_reachability_FromString(char* ue_reachability)
{
    int stringToReturn = 0;
    const char *ue_reachabilityArray[] =  { "NULL", "UNREACHABLE", "REACHABLE", "REGULATORY_ONLY" };
    size_t sizeofArray = sizeof(ue_reachabilityArray) / sizeof(ue_reachabilityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_reachability, ue_reachabilityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

