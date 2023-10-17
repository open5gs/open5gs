
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_data_subset_any_of.h"

char* OpenAPI_policy_data_subset_any_of_ToString(OpenAPI_policy_data_subset_any_of_e policy_data_subset_any_of)
{
    const char *policy_data_subset_any_ofArray[] =  { "NULL", "AM_POLICY_DATA", "SM_POLICY_DATA", "UE_POLICY_DATA", "UM_DATA", "OPERATOR_SPECIFIC_DATA" };
    size_t sizeofArray = sizeof(policy_data_subset_any_ofArray) / sizeof(policy_data_subset_any_ofArray[0]);
    if (policy_data_subset_any_of < sizeofArray)
        return (char *)policy_data_subset_any_ofArray[policy_data_subset_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_policy_data_subset_any_of_e OpenAPI_policy_data_subset_any_of_FromString(char* policy_data_subset_any_of)
{
    int stringToReturn = 0;
    const char *policy_data_subset_any_ofArray[] =  { "NULL", "AM_POLICY_DATA", "SM_POLICY_DATA", "UE_POLICY_DATA", "UM_DATA", "OPERATOR_SPECIFIC_DATA" };
    size_t sizeofArray = sizeof(policy_data_subset_any_ofArray) / sizeof(policy_data_subset_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(policy_data_subset_any_of, policy_data_subset_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

