
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_repl_outcome.h"

char* OpenAPI_slice_repl_outcome_ToString(OpenAPI_slice_repl_outcome_e slice_repl_outcome)
{
    const char *slice_repl_outcomeArray[] =  { "NULL", "SUCCESSFUL", "UNSUCCESSFUL" };
    size_t sizeofArray = sizeof(slice_repl_outcomeArray) / sizeof(slice_repl_outcomeArray[0]);
    if (slice_repl_outcome < sizeofArray)
        return (char *)slice_repl_outcomeArray[slice_repl_outcome];
    else
        return (char *)"Unknown";
}

OpenAPI_slice_repl_outcome_e OpenAPI_slice_repl_outcome_FromString(char* slice_repl_outcome)
{
    int stringToReturn = 0;
    const char *slice_repl_outcomeArray[] =  { "NULL", "SUCCESSFUL", "UNSUCCESSFUL" };
    size_t sizeofArray = sizeof(slice_repl_outcomeArray) / sizeof(slice_repl_outcomeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(slice_repl_outcome, slice_repl_outcomeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

