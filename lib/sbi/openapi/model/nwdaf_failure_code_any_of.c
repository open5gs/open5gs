
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_failure_code_any_of.h"

char* OpenAPI_nwdaf_failure_code_any_of_ToString(OpenAPI_nwdaf_failure_code_any_of_e nwdaf_failure_code_any_of)
{
    const char *nwdaf_failure_code_any_ofArray[] =  { "NULL", "UNAVAILABLE_DATA", "BOTH_STAT_PRED_NOT_ALLOWED", "UNSATISFIED_REQUESTED_ANALYTICS_TIME", "OTHER" };
    size_t sizeofArray = sizeof(nwdaf_failure_code_any_ofArray) / sizeof(nwdaf_failure_code_any_ofArray[0]);
    if (nwdaf_failure_code_any_of < sizeofArray)
        return (char *)nwdaf_failure_code_any_ofArray[nwdaf_failure_code_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_nwdaf_failure_code_any_of_e OpenAPI_nwdaf_failure_code_any_of_FromString(char* nwdaf_failure_code_any_of)
{
    int stringToReturn = 0;
    const char *nwdaf_failure_code_any_ofArray[] =  { "NULL", "UNAVAILABLE_DATA", "BOTH_STAT_PRED_NOT_ALLOWED", "UNSATISFIED_REQUESTED_ANALYTICS_TIME", "OTHER" };
    size_t sizeofArray = sizeof(nwdaf_failure_code_any_ofArray) / sizeof(nwdaf_failure_code_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nwdaf_failure_code_any_of, nwdaf_failure_code_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

