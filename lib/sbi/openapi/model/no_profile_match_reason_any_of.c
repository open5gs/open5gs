
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "no_profile_match_reason_any_of.h"

char* OpenAPI_no_profile_match_reason_any_of_ToString(OpenAPI_no_profile_match_reason_any_of_e no_profile_match_reason_any_of)
{
    const char *no_profile_match_reason_any_ofArray[] =  { "NULL", "REQUESTER_PLMN_NOT_ALLOWED", "TARGET_NF_SUSPENDED", "TARGET_NF_UNDISCOVERABLE", "QUERY_PARAMS_COMBINATION_NO_MATCH", "UNSPECIFIED" };
    size_t sizeofArray = sizeof(no_profile_match_reason_any_ofArray) / sizeof(no_profile_match_reason_any_ofArray[0]);
    if (no_profile_match_reason_any_of < sizeofArray)
        return (char *)no_profile_match_reason_any_ofArray[no_profile_match_reason_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_no_profile_match_reason_any_of_e OpenAPI_no_profile_match_reason_any_of_FromString(char* no_profile_match_reason_any_of)
{
    int stringToReturn = 0;
    const char *no_profile_match_reason_any_ofArray[] =  { "NULL", "REQUESTER_PLMN_NOT_ALLOWED", "TARGET_NF_SUSPENDED", "TARGET_NF_UNDISCOVERABLE", "QUERY_PARAMS_COMBINATION_NO_MATCH", "UNSPECIFIED" };
    size_t sizeofArray = sizeof(no_profile_match_reason_any_ofArray) / sizeof(no_profile_match_reason_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(no_profile_match_reason_any_of, no_profile_match_reason_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

