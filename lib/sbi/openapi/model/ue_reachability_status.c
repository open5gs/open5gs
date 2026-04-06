
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_reachability_status.h"

char* OpenAPI_ue_reachability_status_ToString(OpenAPI_ue_reachability_status_e ue_reachability_status)
{
    const char *ue_reachability_statusArray[] =  { "NULL", "REACHABLE", "UNREACHABLE" };
    size_t sizeofArray = sizeof(ue_reachability_statusArray) / sizeof(ue_reachability_statusArray[0]);
    if (ue_reachability_status < sizeofArray)
        return (char *)ue_reachability_statusArray[ue_reachability_status];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_reachability_status_e OpenAPI_ue_reachability_status_FromString(char* ue_reachability_status)
{
    int stringToReturn = 0;
    const char *ue_reachability_statusArray[] =  { "NULL", "REACHABLE", "UNREACHABLE" };
    size_t sizeofArray = sizeof(ue_reachability_statusArray) / sizeof(ue_reachability_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_reachability_status, ue_reachability_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

