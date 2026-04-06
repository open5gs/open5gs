
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reachability_filter.h"

char* OpenAPI_reachability_filter_ToString(OpenAPI_reachability_filter_e reachability_filter)
{
    const char *reachability_filterArray[] =  { "NULL", "UE_REACHABILITY_STATUS_CHANGE", "UE_REACHABLE_DL_TRAFFIC" };
    size_t sizeofArray = sizeof(reachability_filterArray) / sizeof(reachability_filterArray[0]);
    if (reachability_filter < sizeofArray)
        return (char *)reachability_filterArray[reachability_filter];
    else
        return (char *)"Unknown";
}

OpenAPI_reachability_filter_e OpenAPI_reachability_filter_FromString(char* reachability_filter)
{
    int stringToReturn = 0;
    const char *reachability_filterArray[] =  { "NULL", "UE_REACHABILITY_STATUS_CHANGE", "UE_REACHABLE_DL_TRAFFIC" };
    size_t sizeofArray = sizeof(reachability_filterArray) / sizeof(reachability_filterArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reachability_filter, reachability_filterArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

