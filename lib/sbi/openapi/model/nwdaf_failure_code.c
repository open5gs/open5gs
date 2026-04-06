
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_failure_code.h"

char* OpenAPI_nwdaf_failure_code_ToString(OpenAPI_nwdaf_failure_code_e nwdaf_failure_code)
{
    const char *nwdaf_failure_codeArray[] =  { "NULL", "UNAVAILABLE_DATA", "BOTH_STAT_PRED_NOT_ALLOWED", "PREDICTION_NOT_ALLOWED", "UNSATISFIED_REQUESTED_ANALYTICS_TIME", "NO_ROAMING_SUPPORT", "OTHER" };
    size_t sizeofArray = sizeof(nwdaf_failure_codeArray) / sizeof(nwdaf_failure_codeArray[0]);
    if (nwdaf_failure_code < sizeofArray)
        return (char *)nwdaf_failure_codeArray[nwdaf_failure_code];
    else
        return (char *)"Unknown";
}

OpenAPI_nwdaf_failure_code_e OpenAPI_nwdaf_failure_code_FromString(char* nwdaf_failure_code)
{
    int stringToReturn = 0;
    const char *nwdaf_failure_codeArray[] =  { "NULL", "UNAVAILABLE_DATA", "BOTH_STAT_PRED_NOT_ALLOWED", "PREDICTION_NOT_ALLOWED", "UNSATISFIED_REQUESTED_ANALYTICS_TIME", "NO_ROAMING_SUPPORT", "OTHER" };
    size_t sizeofArray = sizeof(nwdaf_failure_codeArray) / sizeof(nwdaf_failure_codeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nwdaf_failure_code, nwdaf_failure_codeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

