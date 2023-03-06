
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "failure_reason_any_of.h"

char* OpenAPI_failure_reason_any_of_ToString(OpenAPI_failure_reason_any_of_e failure_reason_any_of)
{
    const char *failure_reason_any_ofArray[] =  { "NULL", "INVALID_JSON_POINTER", "INVALID_INDEX_TO_ENCRYPTED_BLOCK", "INVALID_HTTP_HEADER" };
    size_t sizeofArray = sizeof(failure_reason_any_ofArray) / sizeof(failure_reason_any_ofArray[0]);
    if (failure_reason_any_of < sizeofArray)
        return (char *)failure_reason_any_ofArray[failure_reason_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_failure_reason_any_of_e OpenAPI_failure_reason_any_of_FromString(char* failure_reason_any_of)
{
    int stringToReturn = 0;
    const char *failure_reason_any_ofArray[] =  { "NULL", "INVALID_JSON_POINTER", "INVALID_INDEX_TO_ENCRYPTED_BLOCK", "INVALID_HTTP_HEADER" };
    size_t sizeofArray = sizeof(failure_reason_any_ofArray) / sizeof(failure_reason_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(failure_reason_any_of, failure_reason_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

