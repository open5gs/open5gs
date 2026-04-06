
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_data_subset.h"

char* OpenAPI_policy_data_subset_ToString(OpenAPI_policy_data_subset_e policy_data_subset)
{
    const char *policy_data_subsetArray[] =  { "NULL", "AM_POLICY_DATA", "SM_POLICY_DATA", "UE_POLICY_DATA", "UM_DATA", "OPERATOR_SPECIFIC_DATA" };
    size_t sizeofArray = sizeof(policy_data_subsetArray) / sizeof(policy_data_subsetArray[0]);
    if (policy_data_subset < sizeofArray)
        return (char *)policy_data_subsetArray[policy_data_subset];
    else
        return (char *)"Unknown";
}

OpenAPI_policy_data_subset_e OpenAPI_policy_data_subset_FromString(char* policy_data_subset)
{
    int stringToReturn = 0;
    const char *policy_data_subsetArray[] =  { "NULL", "AM_POLICY_DATA", "SM_POLICY_DATA", "UE_POLICY_DATA", "UM_DATA", "OPERATOR_SPECIFIC_DATA" };
    size_t sizeofArray = sizeof(policy_data_subsetArray) / sizeof(policy_data_subsetArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(policy_data_subset, policy_data_subsetArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

