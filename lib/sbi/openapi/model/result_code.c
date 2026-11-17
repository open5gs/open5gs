
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "result_code.h"

char* OpenAPI_result_code_ToString(OpenAPI_result_code_e result_code)
{
    const char *result_codeArray[] =  { "NULL", "SUCCESS", "END_USER_SERVICE_DENIED", "QUOTA_MANAGEMENT_NOT_APPLICABLE", "QUOTA_LIMIT_REACHED", "END_USER_SERVICE_REJECTED", "USER_UNKNOWN", "RATING_FAILED", "QUOTA_MANAGEMENT" };
    size_t sizeofArray = sizeof(result_codeArray) / sizeof(result_codeArray[0]);
    if (result_code < sizeofArray)
        return (char *)result_codeArray[result_code];
    else
        return (char *)"Unknown";
}

OpenAPI_result_code_e OpenAPI_result_code_FromString(char* result_code)
{
    int stringToReturn = 0;
    const char *result_codeArray[] =  { "NULL", "SUCCESS", "END_USER_SERVICE_DENIED", "QUOTA_MANAGEMENT_NOT_APPLICABLE", "QUOTA_LIMIT_REACHED", "END_USER_SERVICE_REJECTED", "USER_UNKNOWN", "RATING_FAILED", "QUOTA_MANAGEMENT" };
    size_t sizeofArray = sizeof(result_codeArray) / sizeof(result_codeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(result_code, result_codeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

