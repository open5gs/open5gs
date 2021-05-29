
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "deregistration_reason.h"

char* OpenAPI_deregistration_reason_ToString(OpenAPI_deregistration_reason_e deregistration_reason)
{
    const char *deregistration_reasonArray[] =  { "NULL", "UE_INITIAL_REGISTRATION", "UE_REGISTRATION_AREA_CHANGE", "SUBSCRIPTION_WITHDRAWN", "_5GS_TO_EPS_MOBILITY", "_5GS_TO_EPS_MOBILITY_UE_INITIAL_REGISTRATION", "REREGISTRATION_REQUIRED", "SMF_CONTEXT_TRANSFERRED" };
    size_t sizeofArray = sizeof(deregistration_reasonArray) / sizeof(deregistration_reasonArray[0]);
    if (deregistration_reason < sizeofArray)
        return (char *)deregistration_reasonArray[deregistration_reason];
    else
        return (char *)"Unknown";
}

OpenAPI_deregistration_reason_e OpenAPI_deregistration_reason_FromString(char* deregistration_reason)
{
    int stringToReturn = 0;
    const char *deregistration_reasonArray[] =  { "NULL", "UE_INITIAL_REGISTRATION", "UE_REGISTRATION_AREA_CHANGE", "SUBSCRIPTION_WITHDRAWN", "_5GS_TO_EPS_MOBILITY", "_5GS_TO_EPS_MOBILITY_UE_INITIAL_REGISTRATION", "REREGISTRATION_REQUIRED", "SMF_CONTEXT_TRANSFERRED" };
    size_t sizeofArray = sizeof(deregistration_reasonArray) / sizeof(deregistration_reasonArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(deregistration_reason, deregistration_reasonArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

