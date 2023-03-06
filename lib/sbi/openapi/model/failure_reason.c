
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "failure_reason.h"

char* OpenAPI_failure_reason_ToString(OpenAPI_failure_reason_e failure_reason)
{
    const char *failure_reasonArray[] =  { "NULL", "INVALID_JSON_POINTER", "INVALID_INDEX_TO_ENCRYPTED_BLOCK", "INVALID_HTTP_HEADER" };
    size_t sizeofArray = sizeof(failure_reasonArray) / sizeof(failure_reasonArray[0]);
    if (failure_reason < sizeofArray)
        return (char *)failure_reasonArray[failure_reason];
    else
        return (char *)"Unknown";
}

OpenAPI_failure_reason_e OpenAPI_failure_reason_FromString(char* failure_reason)
{
    int stringToReturn = 0;
    const char *failure_reasonArray[] =  { "NULL", "INVALID_JSON_POINTER", "INVALID_INDEX_TO_ENCRYPTED_BLOCK", "INVALID_HTTP_HEADER" };
    size_t sizeofArray = sizeof(failure_reasonArray) / sizeof(failure_reasonArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(failure_reason, failure_reasonArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

