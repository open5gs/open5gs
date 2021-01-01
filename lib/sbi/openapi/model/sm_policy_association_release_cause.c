
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_association_release_cause.h"

char* OpenAPI_sm_policy_association_release_cause_ToString(OpenAPI_sm_policy_association_release_cause_e sm_policy_association_release_cause)
{
    const char *sm_policy_association_release_causeArray[] =  { "NULL", "UNSPECIFIED", "UE_SUBSCRIPTION", "INSUFFICIENT_RES", "VALIDATION_CONDITION_NOT_MET" };
    size_t sizeofArray = sizeof(sm_policy_association_release_causeArray) / sizeof(sm_policy_association_release_causeArray[0]);
    if (sm_policy_association_release_cause < sizeofArray)
        return (char *)sm_policy_association_release_causeArray[sm_policy_association_release_cause];
    else
        return (char *)"Unknown";
}

OpenAPI_sm_policy_association_release_cause_e OpenAPI_sm_policy_association_release_cause_FromString(char* sm_policy_association_release_cause)
{
    int stringToReturn = 0;
    const char *sm_policy_association_release_causeArray[] =  { "NULL", "UNSPECIFIED", "UE_SUBSCRIPTION", "INSUFFICIENT_RES", "VALIDATION_CONDITION_NOT_MET" };
    size_t sizeofArray = sizeof(sm_policy_association_release_causeArray) / sizeof(sm_policy_association_release_causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sm_policy_association_release_cause, sm_policy_association_release_causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

