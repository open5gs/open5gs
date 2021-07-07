
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reachability_filter_any_of.h"

char* OpenAPI_reachability_filter_any_of_ToString(OpenAPI_reachability_filter_any_of_e reachability_filter_any_of)
{
    const char *reachability_filter_any_ofArray[] =  { "NULL", "UE_REACHABILITY_STATUS_CHANGE", "UE_REACHABLE_DL_TRAFFIC" };
    size_t sizeofArray = sizeof(reachability_filter_any_ofArray) / sizeof(reachability_filter_any_ofArray[0]);
    if (reachability_filter_any_of < sizeofArray)
        return (char *)reachability_filter_any_ofArray[reachability_filter_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_reachability_filter_any_of_e OpenAPI_reachability_filter_any_of_FromString(char* reachability_filter_any_of)
{
    int stringToReturn = 0;
    const char *reachability_filter_any_ofArray[] =  { "NULL", "UE_REACHABILITY_STATUS_CHANGE", "UE_REACHABLE_DL_TRAFFIC" };
    size_t sizeofArray = sizeof(reachability_filter_any_ofArray) / sizeof(reachability_filter_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reachability_filter_any_of, reachability_filter_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

