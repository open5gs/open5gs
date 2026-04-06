
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_repl_failure_cause.h"

char* OpenAPI_slice_repl_failure_cause_ToString(OpenAPI_slice_repl_failure_cause_e slice_repl_failure_cause)
{
    const char *slice_repl_failure_causeArray[] =  { "NULL", "UNSPECIFIED", "NOT_WITHIN_UE_SUBSCRIPTION", "NOT_SUPPORTED_IN_RA", "PARTIAL_NET_SLICE_NOT_SUPPORTED", "INSUFFICIENT_RES" };
    size_t sizeofArray = sizeof(slice_repl_failure_causeArray) / sizeof(slice_repl_failure_causeArray[0]);
    if (slice_repl_failure_cause < sizeofArray)
        return (char *)slice_repl_failure_causeArray[slice_repl_failure_cause];
    else
        return (char *)"Unknown";
}

OpenAPI_slice_repl_failure_cause_e OpenAPI_slice_repl_failure_cause_FromString(char* slice_repl_failure_cause)
{
    int stringToReturn = 0;
    const char *slice_repl_failure_causeArray[] =  { "NULL", "UNSPECIFIED", "NOT_WITHIN_UE_SUBSCRIPTION", "NOT_SUPPORTED_IN_RA", "PARTIAL_NET_SLICE_NOT_SUPPORTED", "INSUFFICIENT_RES" };
    size_t sizeofArray = sizeof(slice_repl_failure_causeArray) / sizeof(slice_repl_failure_causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(slice_repl_failure_cause, slice_repl_failure_causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

