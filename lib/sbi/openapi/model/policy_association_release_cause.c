
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_association_release_cause.h"

char* OpenAPI_policy_association_release_cause_ToString(OpenAPI_policy_association_release_cause_e policy_association_release_cause)
{
    const char *policy_association_release_causeArray[] =  { "NULL", "UNSPECIFIED", "UE_SUBSCRIPTION", "INSUFFICIENT_RES" };
    size_t sizeofArray = sizeof(policy_association_release_causeArray) / sizeof(policy_association_release_causeArray[0]);
    if (policy_association_release_cause < sizeofArray)
        return (char *)policy_association_release_causeArray[policy_association_release_cause];
    else
        return (char *)"Unknown";
}

OpenAPI_policy_association_release_cause_e OpenAPI_policy_association_release_cause_FromString(char* policy_association_release_cause)
{
    int stringToReturn = 0;
    const char *policy_association_release_causeArray[] =  { "NULL", "UNSPECIFIED", "UE_SUBSCRIPTION", "INSUFFICIENT_RES" };
    size_t sizeofArray = sizeof(policy_association_release_causeArray) / sizeof(policy_association_release_causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(policy_association_release_cause, policy_association_release_causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

