
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "no_profile_match_reason.h"

char* OpenAPI_no_profile_match_reason_ToString(OpenAPI_no_profile_match_reason_e no_profile_match_reason)
{
    const char *no_profile_match_reasonArray[] =  { "NULL", "REQUESTER_PLMN_NOT_ALLOWED", "TARGET_NF_SUSPENDED", "TARGET_NF_UNDISCOVERABLE", "QUERY_PARAMS_COMBINATION_NO_MATCH", "TARGET_NF_TYPE_NOT_SUPPORTED", "UNSPECIFIED" };
    size_t sizeofArray = sizeof(no_profile_match_reasonArray) / sizeof(no_profile_match_reasonArray[0]);
    if (no_profile_match_reason < sizeofArray)
        return (char *)no_profile_match_reasonArray[no_profile_match_reason];
    else
        return (char *)"Unknown";
}

OpenAPI_no_profile_match_reason_e OpenAPI_no_profile_match_reason_FromString(char* no_profile_match_reason)
{
    int stringToReturn = 0;
    const char *no_profile_match_reasonArray[] =  { "NULL", "REQUESTER_PLMN_NOT_ALLOWED", "TARGET_NF_SUSPENDED", "TARGET_NF_UNDISCOVERABLE", "QUERY_PARAMS_COMBINATION_NO_MATCH", "TARGET_NF_TYPE_NOT_SUPPORTED", "UNSPECIFIED" };
    size_t sizeofArray = sizeof(no_profile_match_reasonArray) / sizeof(no_profile_match_reasonArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(no_profile_match_reason, no_profile_match_reasonArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

